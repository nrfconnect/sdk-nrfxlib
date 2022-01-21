/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2021 DSR Corporation, Denver CO, USA.
 * www.dsr-zboss.com
 * www.dsr-corporation.com
 * All rights reserved.
 *
 *
 * Use in source and binary forms, redistribution in binary form only, with
 * or without modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 2. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 3. This software, with or without modification, must only be used with a Nordic
 *    Semiconductor ASA integrated circuit.
 *
 * 4. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/* PURPOSE: Main header for OS and platform dependent stuff
*/

#ifndef ZB_OSIF_H
#define ZB_OSIF_H 1


/**
 @internal

 \addtogroup ZB_OSIF */
/*! @{ */

/**
@par OS/HW abstraction
OS/HW platform abstraction is necessary to achieve high portability.
C language tool to separate abstraction layer is C preprocessor.

Main idea is to avoid number of ifdefs related to portability in the code, and
decrease number of ifdefs in the header files not related to the OS abstraction
layer.

Platform abstraction is implemented as C functions placed into OS abstraction
layers and platform-dependent global typedefs and definitions placed into header
files.  All platform-related stuff is in osif/ directory.  Global definitions
and typedefs can be used anywhere - that is why on the architecture picture OS
abstraction layer depicted as global.

Following things are platform-dependent:
*   typedefs for base types (8-bit controller vs 32-bit Linux device)
*   definitions for different 8051 compilers (SDCC and Keil)
*   transceiver i/o (interrupts handling for 8051 vs file i/o in Linux);
*   wait for i/o (device sleep for 8051, wait in select() in Linux)
*   trace i/o (UART for 8051, file in Linux);
*   MAC traffic dump (UART for 8051, file in Linux);
*   Timer (8051 timer at device, select() timeout in Linux)

 */



#include "zb_config.h"

 /**
OSIF platform selection. One of pre-defined platform should be selected in
zb_config.h configurations.
zb_osif_platform.h is different in different platforms repo.
*/
/**
 * OSIF timer expiration callback type
 * @param user_data - any user specified data which will be sent to this callback
 * Might be used to determine particular timer which cause callback
 *
*/
typedef void (*zb_osif_timer_exp_cb_t)(void *user_data);

#include "zb_osif_platform.h"

#include "zb_ringbuffer.h"

#ifndef ZB_PLATFORM_INIT
#error Define ZB_PLATFORM_INIT in you platform file
//#define ZB_PLATFORM_INIT()
#endif

#ifndef ZB_OSIF_IS_EXIT
#define ZB_OSIF_IS_EXIT() ZB_FALSE
#endif

/* Default (C standard) definition of MAIN() if not redefined in zb_osif_platform.h */
#ifndef MAIN
#define MAIN() int main(int argc, char *argv[])
#define FAKE_ARGV
#define ARGV_UNUSED ZVUNUSED(argc) ; ZVUNUSED(argv)
#define MAIN_RETURN(v) return (v)
#endif

#ifndef ZB_SET_TRACE_TRANSPORT
/*
 * If platform supports choosing transport at runtime
 * (i.e. Telink can choose between UART and USB debug interface)
 * define this macro.
 * Note that this is intended to be called before ZB_TRACE_INIT is called as
 * only one interface will be configured and the rest of them will be left as is
 * so that it would be usable from non-Zigbee purposes (application) if needed.
 */
#define ZB_SET_TRACE_TRANSPORT(transport)
#endif

/*
 * If platform supports NCP architecture and the ability to put the SoC to
 * sleep state, this function should be implemented.
 *
 * This function is meant to be called from zboss_signal_handler(), after
 * receiving ZB_COMMON_SIGNAL_CAN_SLEEP signal.
 * It should configure the NCP transport in a way that is can wake up the MCU
 * through external interrupt and call the zb_sleep_now().
 * Afterwards, it should recover the NCP transport, so it is able to continue
 * its normal operation.
 */
#ifndef ZB_OSIF_NCP_TRANSPORT_PREPARE_TO_SLEEP
#define ZB_OSIF_NCP_TRANSPORT_PREPARE_TO_SLEEP()
#endif

#if !defined ZB_KICK_HW_WATCHDOG
#define ZB_KICK_HW_WATCHDOG()
#endif

/*! @} */


/* common osif API */
zb_uint32_t zb_random_seed(void);
zb_uint32_t zb_get_utc_time(void);

zb_uint32_t osif_get_time_ms(void);

/* note: that api is only for some platforms */
zb_ret_t osif_set_transmit_power(zb_uint8_t channel, zb_int8_t power);
void osif_set_default_trasnmit_powers(zb_int8_t *tx_powers);

#if defined ZB_MACSPLIT_TRANSPORT_SERIAL
void zb_osif_serial_transport_init();
void zb_osif_serial_transport_put_bytes(zb_uint8_t *buf, zb_short_t len);
#endif

#ifndef ZB_SERIAL_INT_DISABLE
#define ZB_SERIAL_INT_DISABLE()  ZB_OSIF_GLOBAL_LOCK()
#endif /* ZB_SERIAL_INT_DISABLE */

#ifndef ZB_SERIAL_INT_ENABLE
#define ZB_SERIAL_INT_ENABLE()   ZB_OSIF_GLOBAL_UNLOCK()
#endif /* ZB_SERIAL_INT_ENABLE */

#if defined ZB_HAVE_SERIAL || defined DOXYGEN

/* Serial interface (trace, traffic dump, serial transport) */

/** @cond DOXYGEN_UART_SECTION */
/*! \addtogroup uart */
/*! @{ */

/**
   Initialize UART low level.

   If ZBOSS uses UART for trace or traffic dump, it calls zb_osif_serial_init()
   itself.
   If UART is used by application, application must call zb_osif_serial_init().
 */
void zb_osif_serial_init(void);

/**
   Deinitialize UART low level.
 */
void zb_osif_serial_deinit(void);

/**
   Setup callback to be called when single byte received over UART

   @param hnd user's rx callback
 */
void zb_osif_set_uart_byte_received_cb(zb_callback_t hnd);


/* errors generated by serial transport */

#define ZB_ERROR_SERIAL_INIT_FAILED 1U
#define ZB_ERROR_SERIAL_READ_FAILED 2U

/**
   Set user's buffer to be used by UART TX logic.

   ZBOSS normally uses its internal UART buffer. The buffer is not too big -
   about 200 bytes which is enough for its usage by ZBOSS (trace).
   Some applications needs large io buffers. So declare there type placeholder for use ringbuffer zb_byte_array_t;
   by default application will set user tx buffer to stack tx buffer and access it through pointer;
   User can override this pointer to use it's own serial buffer.

   @param buf_ptr user's buffer
   @param capacity buffer capacity
 */
void zb_osif_set_user_io_buffer(zb_byte_array_t *buf_ptr, zb_ushort_t capacity);

/**
   Set user callback that will be called from uart IRQ handler.
   Replaces zboss uart irq handler.

   Note: that call is to be used when application initializes and uses UART
   itself.
   Do not use this call if use ZBOSS serial port API.
   Call to that functions disables ZBOSS debug trace and traffic dump.
*/
void zb_osif_set_uart_user_irq_handler(void (*irq_hnd)());

/**
   Prepare UART to a sleep. Shall be invoked before putting ZBOSS to a sleep.
   Perform OS-dependent actions.
*/
void zb_osif_uart_sleep(void);

/**
   Restore UART from a sleep. Shall be invoked after wake up.
   Perform OS-dependent actions.
*/
void zb_osif_uart_wake_up(void);

#if defined(ZB_HAVE_ASYNC_SERIAL) || defined(DOXYGEN)
/* Asynchronous API extension for serial interface. */

#define SERIAL_SEND_SUCCESS         0U   /**< Serial interface has sent the data. */
#define SERIAL_SEND_BUSY            1U   /**< Serial interface is busy. */
#define SERIAL_SEND_TIMEOUT_EXPIRED 2U   /**< Transmission operation timed out. */
#define SERIAL_SEND_ERROR           255U /**< Serial interface internal error. */


/**
   Type of callback called by serial interface when it received requested buffer.

   @param buf - pointer to data received
   @param len - length of received data
 */
typedef void (*serial_recv_data_cb_t)(zb_uint8_t *buf, zb_ushort_t len);

/**
   Type of callback called by serial interface when it completed transmission of data.

   @param status - status of the transmission
 */
typedef void (*serial_send_data_cb_t)(zb_uint8_t status);


/**
  Receive data from serial interface.

  @param buf - pointer to the buffer for received data
  @param len - length of the buffer
 */
void zb_osif_serial_recv_data(zb_uint8_t *buf, zb_ushort_t len);

/**
  Set serial interface callback for receiving completion.

  @param cb - receiving completion callback
 */
void zb_osif_serial_set_cb_recv_data(serial_recv_data_cb_t cb);

/**
  Send data over serial interface.

  @param buf - pointer to the buffer with data to send
  @param len - length of the buffer
 */
void zb_osif_serial_send_data(zb_uint8_t *buf, zb_ushort_t len);

/**
  Set serial interface callback for transmission completion.

  @param cb - transmission completion callback
 */
void zb_osif_serial_set_cb_send_data(serial_send_data_cb_t cb);

#endif /* ZB_HAVE_ASYNC_SERIAL */
#endif /* ZB_HAVE_SERIAL || DOXYGEN */

#if defined ZB_BINARY_TRACE || defined ZB_HAVE_SERIAL || defined ZB_TRACE_OVER_USART || defined DOXYGEN || defined ZB_NSNG

/**
   TX data over UART

   Put data to internal buffer to be transmitted over UART.
   It is guaranteed that all data will be sent.
   Block is no space in the buffer waiting for previous TX complete.

   Note: this is low level routine. Its direct usage may conflict with ZBOSS
   debug trace and traffic dump (if enabled).

   @param buf data buffer
   @param len data length.
 */
void zb_osif_serial_put_bytes(const zb_uint8_t *buf, zb_short_t len);
#endif

#if defined ZB_SERIAL_FOR_TRACE && !defined ZB_OSIF_SERIAL_FLUSH
#define ZB_OSIF_SERIAL_FLUSH()
#endif /* ZB_SERIAL_FOR_TRACE && !ZB_OSIF_SERIAL_FLUSH */

#ifdef ZB_TRACE_OVER_JTAG
void zb_osif_jtag_put_bytes(const zb_uint8_t *buf, zb_short_t len);
void zb_osif_jtag_flush(void);
#endif

/*! @} */
/** @endcond */ /* DOXYGEN_UART_SECTION */



#ifdef ZB_TRACE_OVER_SIF
void zb_osif_sif_put_bytes(const zb_uint8_t *buf, zb_short_t len);
void zb_osif_sif_init(void);
void zb_osif_sif_debug_trace(zb_uint8_t param);
#endif

#ifdef ZB_HAVE_FILE
/* File  */
zb_uint32_t zb_osif_get_file_size(zb_char_t *name);
zb_bool_t zb_osif_check_dir_exist(const zb_char_t *name);
int zb_osif_create_dir(const zb_char_t *name);
zb_bool_t zb_osif_check_file_exist(const zb_char_t *name, const zb_uint8_t mode);
void zb_osif_file_copy(const zb_char_t *name_src, const zb_char_t *name_dst);
zb_osif_file_t *zb_osif_file_open(const zb_char_t *name, const zb_char_t *mode);
zb_osif_file_t *zb_osif_init_trace(zb_char_t *name);
zb_osif_file_t *zb_osif_file_stdout(void);
zb_osif_file_t *zb_osif_file_stdin(void);
void zb_osif_file_close(zb_osif_file_t *f);
int zb_osif_file_remove(const zb_char_t *name);
void zb_osif_trace_printf(zb_osif_file_t *f, const zb_char_t *format, ...);
void zb_osif_trace_vprintf(zb_osif_file_t *f, const zb_char_t *format, va_list arglist);
void zb_osif_trace_lock(void);
void zb_osif_trace_unlock(void);
zb_osif_file_t *zb_osif_init_dump(zb_char_t *name);
int zb_osif_file_read(zb_osif_file_t *f, zb_uint8_t *buf, zb_uint_t len);
int zb_osif_file_write(zb_osif_file_t *f, const zb_uint8_t *buf, zb_uint_t len);
int zb_osif_file_is_eof(zb_osif_file_t *f);
int zb_osif_file_is_err(zb_osif_file_t *f);
int zb_osif_file_flush(zb_osif_file_t *f);
int zb_osif_file_seek(zb_osif_file_t *f, zb_uint32_t off, zb_uint8_t mode);
int zb_osif_file_get_size(zb_osif_file_t *f);
int zb_osif_file_truncate(zb_osif_file_t *f, zb_uint32_t off);
int zb_osif_file_sync(zb_osif_file_t *f);
void zb_osif_trace_get_time(zb_uint_t *sec, zb_uint_t *msec);
zb_osif_file_t *zb_osif_popen(zb_char_t *arg);

int zb_osif_stream_read(zb_osif_file_t *stream, zb_uint8_t *buf, zb_uint_t len);
int zb_osif_stream_write(zb_osif_file_t *stream, zb_uint8_t *buf, zb_uint_t len);

enum zb_file_path_base_type_e
{
  ZB_FILE_PATH_BASE_NOT_SPECIFIED,     /* not specified base type - allows to use default base path */
  ZB_FILE_PATH_BASE_ROMFS_BINARIES,    /* ROM FS */  /* elf binaries, etc */
  ZB_FILE_PATH_BASE_MNTFS_BINARIES,    /* RW FS */   /* prod config, etc */
  ZB_FILE_PATH_BASE_MNTFS_USER_DATA,   /* RW FS */   /* nvram. etc */
  ZB_FILE_PATH_BASE_MNTFS_TRACE_LOGS,  /* RW FS */
  ZB_FILE_PATH_BASE_RAMFS_UNIX_SOCKET, /* RAM FS */
  ZB_FILE_PATH_BASE_RAMFS_TRACE_LOGS,  /* RAM FS */
  ZB_FILE_PATH_BASE_RAMFS_TMP_DATA,    /* RAM FS */

  ZB_FILE_PATH_BASE_MAX_TYPE
};

#ifndef ZB_TRACE_LOG_FILE_EXTENSION
#define ZB_TRACE_LOG_FILE_EXTENSION ""
#endif /* ZB_TRACE_LOG_FILE_EXTENSION */

#define ZB_MAX_FILE_PATH_SIZE 256

#ifdef ZB_FILE_PATH_MGMNT
#ifndef ZB_FILE_PATH_MAX_TYPES
#define ZB_FILE_PATH_MAX_TYPES (ZB_FILE_PATH_BASE_MAX_TYPE - 1)
#endif

typedef struct zb_file_path_base_type_s
{
  zb_bool_t declared;
  char      base[ZB_MAX_FILE_PATH_SIZE];
} zb_file_path_base_type_t;

void zb_file_path_init(void);
zb_ret_t zb_file_path_declare(zb_uint8_t base_type, const char *base);
const char* zb_file_path_get(zb_uint8_t base_type, const char *default_base);
void zb_file_path_get_with_postfix(zb_uint8_t base_type, const char *default_base, const char *postfix, char *file_path);
#define ZB_FILE_PATH_GET(base_type, default_base) \
  zb_file_path_get(base_type, default_base)
#define ZB_FILE_PATH_GET_WITH_POSTFIX(base_type, default_base, postfix, file_path) \
  zb_file_path_get_with_postfix(base_type, default_base, postfix, file_path)
#else
#define ZB_FILE_PATH_GET(base_type, default_base) default_base
#define ZB_FILE_PATH_GET_WITH_POSTFIX(base_type, default_base, postfix, file_path) \
  { \
    zb_uint_t sn_ret; \
    sn_ret = snprintf(file_path, ZB_MAX_FILE_PATH_SIZE, "%s", (default_base postfix)); \
    ZB_ASSERT(sn_ret > 0); \
    ZB_ASSERT(sn_ret < ZB_MAX_FILE_PATH_SIZE); \
  }
#endif  /* ZB_FILE_PATH_MGMNT */

#ifdef ZB_USE_LOGFILE_ROTATE
zb_uint32_t zb_osif_get_max_logfile_size(void);
zb_ret_t zb_osif_set_max_logfile_size(zb_uint32_t val);

zb_uint32_t zb_osif_get_max_logfiles_count(void);
zb_ret_t zb_osif_set_max_logfiles_count(zb_uint32_t val);

zb_ret_t zb_osif_file_rotate(const zb_char_t *file_path, const zb_char_t *file_name);
#endif /* ZB_USE_LOGFILE_ROTATE */
#endif /* ZB_HAVE_FILE */

/*! \addtogroup zb_platform */
/*! @{ */

/**
   Platform dependent soft reset
*/
void zb_reset(zb_uint8_t param);

/**
 * @name Possible reset sources
 * @anchor reset_source
 *
 * Note: These values were members of `enum zb_reset_source_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_RESET_SRC_POWER_ON   0U
#define ZB_RESET_SRC_SW_RESET   1U
#define ZB_RESET_SRC_RESET_PIN  2U
#define ZB_RESET_SRC_BROWN_OUT  3U
#define ZB_RESET_SRC_CLOCK_LOSS 4U
#define ZB_RESET_SRC_OTHER      5U
/** @} */

/**
 * @brief Get the reason that triggered the last reset
 *
 * @return @ref reset_source
 * */
zb_uint8_t zb_get_reset_source(void);

/*! @} */

#if defined ZB_USE_NVRAM || defined DOXYGEN
/**
 * @brief osif NVRAM initializer
 */
void zb_osif_nvram_init(const zb_char_t *name);


/**
 * @brief Deinitialize osif-layer NVRAM support
 */
void zb_osif_nvram_deinit(void);

/**
 * @brief Get NVRAM page length
 *
 * @return NVRAM page length
 */
zb_uint32_t zb_get_nvram_page_length(void);

/**
 * @brief Get NVRAM page count
 *
 * @return NVRAM page count
 */
zb_uint8_t zb_get_nvram_page_count(void);

/**
 * @brief Read from NVRAM directly, by address
 * Read some bytes from NVRAM use address
 *
 * @param address - NVRAM address
 * @param len - count bytes from read data
 * @param buf - (in) buffer for contains read data
 *
 * @return RET_OK if success or code error
 */
zb_ret_t zb_osif_nvram_read_memory(zb_uint32_t address, zb_uint32_t len, zb_uint8_t *buf);

/**
 * @brief Read from NVRAM page
 * Read some bytes from NVRAM
 *
 * @param page - NVRAM page
 * @param pos - Start position
 * @param buf - (in) buffer for contains read data
 * @param len - count bytes from read data
 *
 * @return RET_OK if success or code error
 */
zb_ret_t zb_osif_nvram_read(zb_uint8_t page, zb_uint32_t pos, zb_uint8_t *buf, zb_uint16_t len );

/**
 * @brief Read from NVRAM page with test
 * Read some bytes from NVRAM with test contents.
 * If all byte equal 0xFF then return RET_ERROR
 * Exists not for all platforms.
 *
 * @param page - NVRAM page
 * @param pos - Start position
 * @param buf - (in) buffer for contains read data
 * @param len - count bytes from read data
 *
 * @return RET_OK if success or code error
 */
zb_ret_t zb_osif_nvram_read_test(zb_uint8_t page, zb_uint32_t pos, zb_uint8_t *buf, zb_uint16_t len );

/**
 * @brief Write from NVRAM page
 * Write some bytes to NVRAM
 *
 * @param page - NVRAM page
 * @param pos - Start position
 * @param buf - buffer contains data for write
 * @param len - count bytes for write data
 *
 * @return RET_OK if success or code error
 */
zb_ret_t zb_osif_nvram_write(zb_uint8_t page, zb_uint32_t pos, void *buf, zb_uint16_t len );

/**
 * @brief Write to NVRAM directly, by address
 * Write bytes into NVRAM use address
 *
 * @param address - NVRAM address
 * @param len - count bytes for write data
 * @param buf - (in) buffer contains data to write
 *
 * @return RET_OK if success or code error
 */
zb_ret_t zb_osif_nvram_write_memory(zb_uint32_t address, zb_uint32_t len, zb_uint8_t *buf);

/**
 * @brief Erase NVRAM directly, by address
 *
 * @param address - NVRAM address
 * @param len - count bytes for erase, aligned
 *
 * @return RET_OK if success or code error
 */
zb_ret_t zb_osif_nvram_erase_memory(zb_uint32_t address, zb_uint32_t len);


/**
 * @brief Erase NVRAM page
 * Fill NVRAM page by 0xFF.
 *
 * @param page - page index
 *
 * @return RET_OK if success or code error
 */
zb_ret_t zb_osif_nvram_erase_async(zb_uint8_t page);

void zb_osif_nvram_wait_for_last_op(void);

/**
 * @brief Flush NVRAM page
 * Flash NVRAM page to file or NVRAM.
 * Different to hardware device
 *
 */
void zb_osif_nvram_flush(void);

#endif

#ifdef ZB_USE_OSIF_OTA_ROUTINES
/**
 *  @addtogroup ll
 *  @{
 */

/**
 *  @defgroup ota Low level API for OTA
 */
/*! @{ */

/**
 * Size of sub-element includes OTA header.
 *
 * @see Zigbee Cluster Library specification revision 7 subsection 11.4.3 Sub-element Format
 */
#define OTA_UPGRADE_HEADING_DATA \
  (sizeof(zb_zcl_ota_upgrade_file_header_t) + 2 /* fw tag Id */ + 4 /* fw length */)

/**
 * Size of the OTA image hash.
 *
 * @see Zigbee Cluster Library specification revision 7 subsection 11.7.2.1 Hash Value Calculation
 */
#define OTA_UPGRADE_HASH_SIZE 16

/**
 * Size of sub-element includes a hash value.
 *
 * @see Zigbee Cluster Library specification revision 7 subsection 11.4.8 Image Integrity Code Sub-element
 */
#define OTA_UPGRADE_TRAILING_DATA                                       \
  (2 /* hash tag Id */ + 4 /* hash length */ + OTA_UPGRADE_HASH_SIZE /* Hash */)


/**
   Open device to be used for OTA image write or read.

   Take into account which flash (A or B) current FW is running at.
   @see zb_osif_ota_close_storage

   @return handle
 */
void *zb_osif_ota_open_storage(void);

/**
   Close device used for OTA image write

   @param dev flash device handle used to write OTA image @see zb_osif_ota_open_storage
*/
void zb_osif_ota_close_storage(void *dev);

/**
   Erase flash to be used for OTA image store.

   @note: offset and size must be aligned to minimal eraseable page boundary -
   see zb_osif_ota_get_erase_portion().

   @param dev flash device handle @see zb_osif_ota_open_storage
   @param offset file offset in flash memory
   @param size OTA file image size
 */
void zb_osif_ota_erase_fw(void *dev, zb_uint_t offset, zb_uint_t size);

/**
   Get portion of OTA storage which can be erased at once.

   @return erase block size in bytes
 */
zb_uint32_t zb_osif_ota_get_erase_portion(void);

/**
   Write OTA image data to flash.

   Deal with alignment.

   @param dev flash device handle @see zb_osif_ota_open_storage
   @param data pointer to the data to write
   @param off offset in OTA file
   @param block_size amount of data to write
   @param image_size total image size
 */
void zb_osif_ota_write(void *dev, zb_uint8_t *data, zb_uint32_t off, zb_uint32_t block_size, zb_uint32_t image_size);

/**
   Mark FW as downloaded and ready to use by bootloader.

   To be called when finishing OTA, after FW loaded and verified.

   @param ota_dev flash device handle used to write OTA image @see zb_osif_ota_open_storage
   @param size OTA image size, without CRC
   @param revision OTA image revision
 */
void zb_osif_ota_mark_fw_ready(void *ota_dev, zb_uint32_t size, zb_uint32_t revision);


/**
   Mark new FW as absent so bootloader does not attempt to re-flash it.

   To be called when starting OTA.
 */
void zb_osif_ota_mark_fw_absent(void);


/**
   Mark FW upgrade finished, so bootloader just loads current FW.

   To be called from newly started application FW.
   Usually not need to be called by the application. ZBOSS kernel cares about it.
 */
void zb_osif_ota_mark_fw_updated(void);

/**
   Verify OTA file image written to device @p dev.

   @param dev flash device handle used to write OTA image @see zb_osif_ota_open_storage
   @param raw_len - OTA image file length
   @return ZB_TRUE if ok, ZB_FALSE if error
 */
zb_bool_t zb_osif_ota_verify_integrity(void *dev, zb_uint32_t raw_len);

/**
   Verify OTA file image written to device @p dev asynchronously.

   @param dev flash device handle used to write OTA image @see zb_osif_ota_open_storage
   @param raw_len - OTA image file length
   @return ZB_TRUE if ok, ZB_FALSE if error
 */
zb_bool_t zb_osif_ota_verify_integrity_async(void *dev, zb_uint32_t raw_len);

/**
 * Check whether OTA image recording was successful and finalize OTA.
 *
 * @param integrity_is_ok - is verification of OTA image recording successfull
 */
void zb_osif_ota_verify_integrity_done(zb_uint8_t integrity_is_ok);

/**
 * Read OTA image data from flash.
 *
 * @param dev flash device handle @see zb_osif_ota_open_storage
 * @param data pointer to the read data
 * @param addr
 * @param size amount of data to read
 */
void zb_osif_ota_read(void *dev, zb_uint8_t *data, zb_uint32_t addr, zb_uint32_t size);

/**
   Check whether the current device flash memory has enough space for the OTA file image.

   @param image_size OTA file image size
   @return ZB_TRUE if ok, ZB_FALSE if error
 */
zb_bool_t zb_osif_ota_fw_size_ok(zb_uint32_t image_size);

/**
   Get OTA image data at OTA server

   @param dev flash device handle used to write OTA image @see zb_osif_ota_open_storage
   @param offset offset in the OTA image
   @param len - length of image block to get
   @return pointer to image block
 */
zb_uint8_t *zb_osif_ota_srv_get_image(void *dev, zb_uint32_t offset, zb_uint32_t len);

/**
   Get OTA header at OTA server

   @param dev flash device handle used to write OTA image @see zb_osif_ota_open_storage
   @return pointer to header block
 */
zb_uint8_t *zb_osif_ota_srv_get_image_header(void *dev);

/** @} */ /* ota */
/** @} */ /* ll */

#endif /* ZB_USE_OSIF_OTA_ROUTINES */

/**
 * Run the bootloader after next reset
 *
 * @return RET_OK is success, RET_ERROR otherwise
 */
zb_ret_t zb_osif_bootloader_run_after_reboot(void);

/**
 * Inform that image was successfully loaded
 */
void zb_osif_bootloader_report_successful_loading(void);


#if defined ZB_PROFILE_STACK_USAGE
/* Both functions are platform-specific */

/**
 * @brief Prepare stack profiler.
 * Fill stack area with a predefined pattern.
 *
 */
void zb_stack_profiler_pre(void);

/**
 * @brief Get stack usage by previously called function.
 *
 * @return Count of bytes in stack area used
 */
zb_uint16_t zb_stack_profiler_usage(void);

#endif  /* ZB_PROFILE_STACK_USAGE */


#ifdef ZB_USE_SLEEP
zb_uint32_t zb_osif_sleep(zb_uint32_t sleep_tmo);
void zb_osif_wake_up(void);
#endif

#ifdef ZB_PRODUCTION_CONFIG

/* Check whether production configuration block is present in memory */
zb_bool_t zb_osif_prod_cfg_check_presence(void);

/* Read data from production configuration header
 *
 * @param prod_cfg_hdr - pointer to production configuration header
 * @param hdr_len - size of production configuration header
 *
 * @return RET_OK is success, RET_ERROR otherwise
 */
zb_ret_t zb_osif_prod_cfg_read_header(zb_uint8_t *prod_cfg_hdr, zb_uint16_t hdr_len);

/* Read data from production configuration block
 *
 * @param buffer - buffer to read into
 * @param len - length of block to read
 * @param offset - offset to read from
 *
 * @return
 */
zb_ret_t zb_osif_prod_cfg_read(zb_uint8_t *buffer, zb_uint16_t len, zb_uint16_t offset);

#ifdef ZB_OSIF_CONFIGURABLE_TX_POWER
/* Set transmit power of radio on selected channel
 *
 * @param channel - channel on which radio applies new transmit power
 * @param power - transmit power in dBm
 *
 * return RET_OK if power was set successfully, RET_ERROR otherwise
 */
zb_ret_t zb_osif_set_transmit_power(zb_uint8_t channel, zb_int8_t power);
#endif /* ZB_OSIF_CONFIGURABLE_TX_POWER */

#endif

/* Wait for a given number of empty cycle iterations. Timeout of 1 iteration is platform-specific
 *
 * @param count - number of empty wait cycles
 */
void zb_osif_busy_loop_delay(zb_uint32_t count);


/* Get OSIF timer counter value in microseconds
 *
 * @return Timer counter value in microseconds
 */
zb_uint32_t zb_osif_get_timer_reminder(void);

#ifndef ZB_USE_INTERNAL_HEADERS
/* Init leds and buttons
 *
 */
void zb_osif_led_button_init(void);

/* Set a LED on
 * It must be initialized first.
 *
 * @param led_no - id of the LED in board (check platform specific headers to identify LED)
 */
void zb_osif_led_on(zb_uint8_t led_no);

/* Set a LED off
 * It must be initialized first.
 *
 * @param led_no - id of the LED in board (check platform specific headers to identify LED)
 */
void zb_osif_led_off(zb_uint8_t led_no);

/* Get the current button tate
 * It must be initialized first.
 *
 * @param arg - id of the LED in board (check platform specific headers to identify LED)
 *
 * @return ZB_FALSE is button is pressed, ZB_TRUE if button is not being pressed
 */
zb_bool_t zb_osif_button_state(zb_uint8_t arg);

/* Inform osif layer that button callback is being set
 * @return ZB_TRUE if leds and buttons are not initialized yet
 *  with zb_osif_led_button_init(). In this case stack logic
 *  should call it explicitly.
 */
zb_bool_t zb_setup_buttons_cb(zb_callback_t cb);

void zb_osif_button_cb(zb_uint8_t arg);

#endif /* ZB_USE_INTERNAL_HEADERS */

/* Configure one LED to be dimmable
 *
 * @param led_no - Led id to be dimmable
 */
zb_bool_t zb_osif_led_level_init(zb_uint8_t led_no);

/* Set level of a dimmable LED (only one led is supported)
 *  It must be initialized first with zb_osif_led_level_init(zb_uint8_t led_no)
 *
 * @param level - brightness level to set from (0 to 255) to the dimmable led
 */
void zb_osif_led_on_set_level(zb_uint8_t level);

#endif /* ZB_OSIF_H */
