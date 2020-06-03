/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#ifndef NFC_T4T_LIB_H__
#define NFC_T4T_LIB_H__

/** @file
 *
 * @addtogroup nfc_api
 *
 * @defgroup nfc_t4t NFC Type 4 Tag
 * @ingroup nfc_api
 * @brief Implementation of NFC Type 4 Tag.
 *
 * @defgroup nfc_t4t_lib NFC tag 4 type emulation library
 * @{
 * @ingroup nfc_t4t
 * @brief The T4T emulation library interface
 *
 * This is the NFC Forum NDEF tag 4 type emulation library. It implements the
 * ISO14443-4A protocol (ISO-DEP) and additionally can emulate a read-writable
 * NDEF content. If the emulation of the NDEF content is not needed, the library
 * works in a raw mode where all APDUs are delivered to the user, who is then
 * responsible to generate a timely RPDU as a response.
 *
 * The sequence of initializing functions determines whether the NDEF emulation
 * will run or whether the raw mode is used.
 *
 *   - E.g. NDEF emulation
 *        * @ref nfc_t4t_setup
 *        * @ref nfc_t4t_ndef_rwpayload_set or
 @ref nfc_t4t_ndef_staticpayload_set
 *        * @ref nfc_t4t_emulation_start
 *        * ... running in NDEF emulation mode
 *   - E.g. RAW mode
 *        * @ref nfc_t4t_setup
 *        * @ref nfc_t4t_emulation_start
 *        * ... running in RAW emulation mode
 *
 * @note If you are using nRF52832 chip (in IC rev. Engineering B or
 * Engineering C) or if You are using nRF52840 chip (in IC rev. Engineering A,
 * B or C) library will use TIMER 4 to apply workarounds for the anomalies.
 */

#include <zephyr/types.h>
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NFC_T4T_MAX_PAYLOAD_SIZE  0xFFF0U

/**< @brief Emulation mode. */
enum nfc_t4t_emu_mode {
	NFC_T4T_EMUMODE_NDEF,   /**< Emulated NDEF AID and EF-Files. */
	NFC_T4T_EMUMODE_PICC    /**< Run just ISO-DEP, deliver I-Frames up. */
};

/**< @brief Event identifiers used by the @ref nfc_t4t_callback_t */
enum nfc_t4t_event {
	NFC_T4T_EVENT_NONE,
	/**< This ID is never used. Dummy value for completeness. */

	NFC_T4T_EVENT_FIELD_ON,
	/**< External Reader polling detected. */

	NFC_T4T_EVENT_FIELD_OFF,
	/**< External Reader polling ended. */

	NFC_T4T_EVENT_NDEF_READ,
	/**< External Reader has read static NDEF-Data from Emulation. */
	/**<
	 * A Read operation happened on last byte of NDEF-Data.
	 */

	NFC_T4T_EVENT_NDEF_UPDATED,
	/**< External Reader has written to length information of NDEF-Data from
	 *   Emulation.
	 */
	/**<
	 * The usual behavior of a Reader-Writer that accesses NDEF information
	 * for update is to set the length to zero at the beginning of the
	 * update process. It then writes the content of NDEF-Data. When all
	 * content is written it will update the length information inside the
	 * NDEF file. This event will be generated every time an update to the
	 * length is happening. This length information is residing in the first
	 * 2 bytes of the NDEF-Content container and is called 'NLEN'. Since
	 * this callback is triggered on any access to these bytes  the returned
	 * data_length information might not be consistent (e.g. in case of only
	 * a single byte write to the length).
	 *
	 * @param data Pointer to current data of NDEF message
	 * @param data_length Current value of NDEF content length information
	 * i.e. 'NLEN' field.
	 */

	NFC_T4T_EVENT_DATA_TRANSMITTED,
	/**< In Raw mode it signals that the data from @ref
	 *     nfc_t4t_response_pdu_send have been sent out.
	 */

	NFC_T4T_EVENT_DATA_IND,
	/**< In Raw mode delivers the APDU fragments */
	/**<
	 * All @ref NFC_T4T_EVENT_DATA_IND events that have  the @ref
	 * NFC_T4T_DI_FLAG_MORE flag set belong to the same APDU. The first
	 * @ref NFC_T4T_EVENT_DATA_IND without @ref NFC_T4T_DI_FLAG_MORE
	 * flag signals the completeness of the APDU. The Application then has
	 * to reply with a call to @ref nfc_t4t_response_pdu_send. The library
	 * will handle internally the fragmentation of the response towards the
	 * Reader-Writer. The data of the response PDU must be kept valid until
	 * the next callback from the library happens (e.g. next @ref
	 * NFC_T4T_EVENT_DATA_IND or @ref NFC_T4T_EVENT_FIELD_OFF).
	 *
	 * @param data Pointer to the fragment of APDU.
	 * @param data_length Length of data.
	 * @param flags @ref nfc_t4t_data_ind_flags.
	 */
};

/**< @brief Flags coming with nfc_t4t_callback_t at @ref
 *   NFC_T4T_EVENT_DATA_IND event.
 */
enum nfc_t4t_data_ind_flags {
	NFC_T4T_DI_FLAG_NONE = 0x00,  /**< Dummy value. */
	NFC_T4T_DI_FLAG_MORE = 0x01
	/**< This signals that more data is expected to be received. */
};

/**< @brief Parameter IDs that can be set/get with @ref nfc_t4t_parameter_set or
 *   @ref nfc_t4t_parameter_get.
 */
enum nfc_t4t_param_id {
	NFC_T4T_PARAM_TESTING,	/**< Internal usage only for Unit-Testing. */
	NFC_T4T_PARAM_FWI,	/**< Frame Wait Time parameter */
	NFC_T4T_PARAM_SELRES,
	/**< Parameter for setting 'Protocol' bits for SEL_RES packet */
	NFC_T4T_PARAM_NFCID1,
	/**< NFCID1 value, data can be 4, 7, or 10 bytes long  (single, double,
	 *   or triple size). To use default NFCID1 of specific length pass one
	 *   byte containing requested length. Default 7-byte NFCID1 will be
	 *   used if this parameter was not set. This parameter can be set
	 *   before nfc_t2t_setup() to set initial NFCID1 and it can be changed
	 *   later.
	 */
};

/** @brief Callback to pass events from NFCLib to application.
 *
 * @param context Application context for callback execution.
 * @param event The event that occurred. see @ref nfc_t4t_event.
 * @param data Data to send to the application (event specific).
 * @param data_length Length of the data. In case of @ref
 * NFC_T4T_EVENT_NDEF_UPDATED, this parameter contains the value of the 'NLEN'
 * field of the NDEF File; if the value is non-zero, it corresponds to the new
 * size of the NDEF Message in the updated NDEF File.
 * @param flags Some events deliver flags. see @ref nfc_t4t_event for details.
 */
typedef void (*nfc_t4t_callback_t)(void *context,
				   enum nfc_t4t_event event,
				   const u8_t *data,
				   size_t data_length,
				   u32_t flags);

/** @brief Register the application callback for event signaling.
 *
 * The callback will be called by NFCLib to notify the application of relevant
 * events. It will be called from the HAL_NFC callback context. The library
 * support 3 different Modes of Emulation:
 *  - Raw ISO-Dep exchanges. All PDUs are signaled through the callback.
 *  - Read-Only T4T NDEF-Tag. A static buffer is served. Only Field-Status
 *    callbacks.
 *  - Read-Write T4T NDEF-Tag. A mutable buffer is used. Only Field-Status
 *    callbacks.
 *
 * The default mode is Raw ISO-Dep mode. The two other NDEF T4T modes are
 * activated through the corresponding @ref nfc_t4t_ndef_rwpayload_set/ @ref
 * nfc_t4t_ndef_staticpayload_set functions. The mode is locked in with a call
 * to @ref nfc_t4t_emulation_start.
 *
 * @param callback Function pointer to the callback.
 * @param context Pointer to a memory area used by the callback for execution
 * (optional).
 *
 * @retval 0 Success.
 * @retval -ENOTSUP If emulation is in running state.
 */
int nfc_t4t_setup(nfc_t4t_callback_t callback, void *context);

/** @brief Set emulation buffer and content for a NDEF Tag emulation that is
 *  Read/Writable.
 *
 * The buffer needs to be kept accessible for the lifetime of the emulation.
 * If an external Reader-Writer changes the NDEF content it is signaled through
 * the app-callback. Buffer can be changed during the lifetime of the emulation,
 * when NDEF READ or UPDATE procedure is pending, and it will be changed after
 * this procedure is finished. To perform this procedure safely use
 * critical sections or disable the interrupts.
 *
 * @param emulation_buffer Buffer pointer
 * @param buffer_length Length of buffer (maximum writable NDEF size)
 *
 * @retval 0 Success.
 * @retval -EINVAL Invalid argument (e.g. wrong data length).
 * @retval -EINVAL Invalid argument (e.g. NULL pointer).
 * @retval -ENOTSUP If the new buffer has a different length than the first one.
 * @retval -EFAULT If the provided buffer is the currently used buffer.
 */
int nfc_t4t_ndef_rwpayload_set(u8_t *emulation_buffer,
			       size_t buffer_length);

/** @brief Set emulationBuffer and Content for a NDEF Tag emulation that is
 *  Read-Only.
 *
 * The buffer needs to be kept accessible for the lifetime of the emulation.
 * Since no write access is done to the buffer, the content could reside in
 * flash memory.
 *
 * @param emulation_buffer Const buffer pointer
 * @param buffer_length Length of contained NDEF payload message
 *
 * @retval 0 Success.
 * @retval -EINVAL Invalid argument (e.g. wrong data length).
 * @retval -EINVAL Invalid argument (e.g. NULL pointer).
 * @retval -ENOTSUP Emulation is in running stated.
 */
int nfc_t4t_ndef_staticpayload_set(const u8_t *emulation_buffer,
				   size_t buffer_length);

/** @brief Send a raw response PDU after getting a Request PDU callback.
 *
 * When the library works in raw ISO-DEP mode it will signal request PDUs
 * through the callback. The application then has to answer with a response PDU.
 * It will use this function to send back the response PDU. This function can
 * not be used in T4T NDEF (RW / STATIC) emulation modes.
 *
 * The lower ISODEP layer will handle the defragmentation of a long response PDU
 * into smalleR pieces that the PCD can understand.
 *
 * @param pdu Const PDU pointer.
 * @param pdu_length Length of PDU.
 *
 * @retval 0 Success.
 * @retval -EINVAL Invalid argument (e.g. wrong data length).
 * @retval -EINVAL Invalid argument (e.g. NULL pointer).
 * @retval -ENOTSUP Emulation is in running state.
 */
int nfc_t4t_response_pdu_send(const u8_t *pdu, size_t pdu_length);

/** @brief Set an NFC parameter.
 *
 * Allows to set an NFC configuration parameter.
 *
 * @param id ID of the parameter to set.
 * @param data Pointer to a buffer containing the data to set.
 * @param data_length Size of the buffer containing the data to set.
 *
 * @retval 0 Success.
 * @retval -EINVAL Invalid argument (e.g. wrong data length).
 * @retval -EINVAL Invalid argument (e.g. NULL pointer).
 */
int nfc_t4t_parameter_set(enum nfc_t4t_param_id id,
			  void *data,
			  size_t data_length);

/** @brief Query an NFC parameter value.
 *
 * The queried value will be placed into the passed data buffer.
 * If the buffer is too small, max_data_length will contain the required
 * buffer size. If the buffer is big enough, max_data_length will contain
 * the actual size of the data.
 *
 * @param id ID of the parameter to query.
 * @param data Pointer to a buffer receiving the queried data.
 * @param max_data_length Size of the buffer, receives actual size of queried
 * data.
 *
 * @retval 0 Success.
 * @retval -EINVAL Invalid argument (e.g. wrong data length).
 * @retval -EINVAL Invalid argument (e.g. NULL pointer).
 */
int nfc_t4t_parameter_get(enum nfc_t4t_param_id id,
			  void *data,
			  size_t *max_data_length);

/** @brief Activate the NFC frontend.
 *
 * Only after calling this function, events will be posted to the application
 * callback.
 *
 * @retval 0 Success.
 * @retval -ENOTSUP Already started.
 */
int nfc_t4t_emulation_start(void);

/**
 * @brief Deactivate the NFC frontend.
 *
 * After calling this function, no more events will be posted to the application
 * callback.
 *
 * @retval 0 Success.
 * @retval -ENOTSUP Emulation was already stopped
 */
int nfc_t4t_emulation_stop(void);

/**
 * @brief Release reference to application callback.
 *
 * After calling this function, the passed callback pointer is no longer
 * considered valid.
 * After calling this function, the passed ndef pointer is no longer
 * considered valid.
 *
 * You need to restart with @ref nfc_t4t_setup to run a new Emulation.
 *
 * @retval 0 Always succeeds.
 */
int nfc_t4t_done(void);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /** NFC_T4T_LIB_H__ */
