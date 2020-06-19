/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2020 DSR Corporation, Denver CO, USA.
 * http://www.dsr-zboss.com
 * http://www.dsr-corporation.com
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
/*  PURPOSE: ZBOSS API header
*/
#ifndef ZBOSS_API_H
#define ZBOSS_API_H 1

#include "zb_version.h"
#include "zboss_api_core.h"
#include "zboss_api_buf.h"
#include "zboss_api_internal.h"
#include "zboss_api_nwk.h"
#include "zboss_api_af.h"
#include "zboss_api_zdo.h"
#include "zboss_api_aps.h"
#ifdef ZB_ENABLE_HA
#include "zb_ha.h"
#endif
#include "zb_address.h"
#include "zboss_api_zcl.h"
#ifdef ZB_ENABLE_ZGP
#include "zboss_api_zgp.h"
#endif

/** @cond (!DOXYGEN_ERL_SECTION) */
/*! @cond DOXYGEN_SECUR_SECTION */
/*! @addtogroup secur_api */
/*! @{ */

/** @fn void zb_secur_setup_nwk_key(zb_uint8_t *key, zb_uint8_t i)
 *  @brief Setup nwk key to be used by ZCP tests.
 *  @param key - key to be used
 *  @param i - key number (0-3)
 *  @details Call this routine at TC to change random nwk key generated from zb_ib_set_defaults
 *    to well-known key. To be used in certification tests.
 *  @snippet simple_gw/simple_gw.c zb_secur_setup_preconfigured_key_value
 *  @snippet simple_gw/simple_gw.c zb_secur_setup_preconfigured_key
 *  @see TP_R20_BV-01 sample.
 */
void zb_secur_setup_nwk_key(zb_uint8_t *key, zb_uint8_t i);

/**
 *  Initiate procedure of NWK key switching.
 *
 *  Generate next NWK key if it not exists, broadcast new NWK key, after delay broadcast NWK key switch command.
 *  Can run at TC only.
 *
 *  @param param - work buffer ID or 0 (is zero, function allocates buffer itself)
 */
void zb_secur_nwk_key_switch_procedure(zb_uint8_t param);

/**
 *  Specifies whether unsecure Trust Center Rejoin is allowed.
 *
 *  If set to ZB_FALSE, devices that attempted unsecure rejoin will not be authorized.
 *
 *  @param enable - whether to enable or disable TC Rejoin.
 */
void zb_secur_set_tc_rejoin_enabled(zb_bool_t enable);
/**
 *  Specifies whether Trust Center Rejoin is ignored.
 *
 *  If set to ZB_TRUE, devices that attempted unsecure rejoin will be ignored.
 *
 *  @param enable - whether to enable or disable TC Rejoin ignore.
 */
void zb_secur_set_ignore_tc_rejoin(zb_bool_t enable);
/*! @} */ /* secur_api*/
/** @endcond */ /* DOXYGEN_SECUR_SECTION */

/*! @cond DOXYGEN_SE_SECTION */
/*! @addtogroup se_secur */
/*! @{ */

/**
 *  Initiate procedure of NWK key switching.
 *
 *  Generate link key with device, update link key.
 *  Can run at TC only.
 *
 *  @param addr_of_interest - network address of ZC to ask ZC for KEC.
 */
#ifdef ZB_COORDINATOR_ROLE
#ifndef ZB_LITE_NO_TRUST_CENTER_REQUIRE_KEY_EXCHANGE
void se_tc_update_link_key(zb_uint16_t addr_of_interest);
#endif
#endif


typedef enum {
  ZB_IC_TYPE_48 = 0,
  ZB_IC_TYPE_64,
  ZB_IC_TYPE_96,
  ZB_IC_TYPE_128,
  ZB_IC_TYPE_MAX
} zb_ic_types_t;

/** @fn zb_ret_t zb_secur_ic_add(zb_ieee_addr_t address, zb_uint8_t ic_type, zb_uint8_t *ic)
 *  @brief Add install code for the device with specified long address
 *  @param[in]  address - long address of the device to add the install code
 *  @param[in]  ic_type - install code type as enumerated in
 *  @if ZB_SECUR
 *    @ref zb_ic_types_t
 *  @else
 *    zb_ic_types_t
 *  @endif
 *  @param[in]  ic - pointer to the install code buffer
 *  @details This call allows to add install codes for several devices into NVRAM. It makes
 *    sense for Trust Center/Coordinator devices only as usual device doesn't need to have
 *    someone's else install codes.
 *  @note This call is valid only for the TC (ZC)!
 *  @return RET_OK on success or RET_CONVERSION_ERROR on error in install code
 *  @par Example
 *  @snippet doxygen_snippets.dox wrong_ic_snippet_cs_ick_tc_02_dut_using_ic_c
 */
zb_ret_t zb_secur_ic_add(zb_ieee_addr_t address, zb_uint8_t ic_type, zb_uint8_t *ic);


/** @fn zb_ret_t zb_secur_ic_set(zb_uint8_t ic_type, zb_uint8_t *ic)
 *  @brief Set install code for the device
 *  @param[in]  ic_type - install code type as enumerated in
 *  @if ZB_SECUR
 *    @ref zb_ic_types_t
 *  @else
 *    zb_ic_types_t
 *  @endif
 *  @param[in]  ic - pointer to the install code buffer
 *  @details This call allows to set the install code for the current device. It is assumed
 *    that device can have only one install code.
 *  @note This function is to be run at client side (not at TC).
 *  @return RET_OK on success or RET_CONVERSION_ERROR on error in install code
 */
zb_ret_t zb_secur_ic_set(zb_uint8_t ic_type, zb_uint8_t *ic);

/**
   Get list of the install codes.

   It is valid only for the TC (ZC).

   @param table - pointer to the allocated space for the entries.
   @param cnt - pointer to the counter that indicates count of the output items in the table.

   @return RET_OK on success or RET_ERROR in case of fail
*/
zb_ret_t zb_secur_ic_get_list(zb_uint8_t* table, zb_uint8_t *cnt);

/**
   Get the install code by index.

   It is valid only for the TC (ZC).

   @param param - pointer to the allocated space for the entry.
   @param index - the index of the entry in the install codes table.

   @return RET_OK on success or RET_ERROR in case of fail
*/

zb_ret_t zb_secur_ic_get_by_idx(zb_uint8_t* param, zb_uint8_t index);

/**
   Remove the install code for the device with specified long
   address.

   It is valid only for the TC (ZC).

   @param address - long address of the device for which the install code is to be deleted.

   @return RET_OK on success or RET_NO_MATCH if installcode
   for address isn't present
*/
zb_ret_t zb_secur_ic_remove(zb_ieee_addr_t address);

/**
   Remove the install code for all devices.

   It is valid only for the TC (ZC).

   @return RET_OK on success or RET_ERROR in case of fail
*/
zb_ret_t zb_secur_ic_remove_all(void);


/** @fn zb_ret_t zb_secur_ic_str_set(char *ic_str)
 *  @brief Set install code for the device from character string
 *  @param[in]  ic_str - install code text string
 *  @details This call allows to set the install code for the current device. It is assumed
 *    that device can have only one install code.
 *  @note This function is to be run at client side (not at TC).
 *  @returns RET_OK on success.
 */
zb_ret_t zb_secur_ic_str_set(char *ic_str);

/** @fn zb_ret_t zb_secur_ic_str_add(zb_ieee_addr_t address, char *ic_str)
 *  @brief Add install code for the device from character string
 *  @param[in]  address - long address of the device to add the install code for
 *  @param[in]  ic_str - install code text string
 *  @details This call allows to add install codes for several devices into NVRAM. It makes
 *    sense for Trust Center/Coordinator devices only as usual device doesn't need to have
 *    someone's else install codes.
 *  @note This call is valid only for the TC (ZC)!
 *  @returns RET_OK on success.
 */
zb_ret_t zb_secur_ic_str_add(zb_ieee_addr_t address, char *ic_str);

/**
   Set installcode policy flag.

   @param allow_ic_only - use ZB_TRUE value to check installcodes
*/
void zb_set_installcode_policy(zb_bool_t allow_ic_only);

#ifdef ZB_ROUTER_ROLE
/**
   Disable APS-encryption of Transport Key from the ZC.

   This valid is only for the TC (ZC).
*/
void zb_disable_transport_key_aps_encryption(void);
/**
   Enable APS-encryption of Transport Key from the ZC.

   This valid is only for the TC (ZC).
*/
void zb_enable_transport_key_aps_encryption(void);
/**
   Get the status of the APS-encryption of Transport Key from the ZC.

   This valid is only for the TC (ZC).

   @return ZB_TRUE when APS-encryption of Transport Key is enabled, ZB_FALSE otherwise
*/
zb_bool_t zb_is_transport_key_aps_encryption_enabled(void);
#endif /* ZB_ROUTER_ROLE */

/*! @} */ /* se_secur */
/*! @endcond */ /* DOXYGEN_SE_SECTION */
/*! @endcond */ /* DOXYGEN_ERL_SECTION */

/*! @cond DOXYGEN_GENERAL_SECTION */
/*! @addtogroup zboss_general_api */
/*! @{ */

#ifdef DOXYGEN
/**
   ZBOSS application entry point.

   Application program using ZBOSS must use MAIN() as an entry point and do not implement function main().
   At some, but not at all platforms MAIN() is an alias for main().
 */
#define MAIN() main(a,v)


/**
   Return statement of ZBOSS application entry point.
   Corresponds to exit() call from main().
   Platform-dependent.
 */
#define MAIN_RETURN(retc) return (retc)

#endif  /* DOXYGEN */

/** @cond internals_doc */
/**
   Check whether the right library is selected at application compilation time

   To be called from ZB_INIT() at start.
 */
#ifdef ZB_ED_ROLE
#define ZB_CHECK_LIBRARY()  since_you_got_that_symbol_unresolved_you_probably_use_ZB_ED_ROLE_preprocessor_define_while_linking_with_zc_library();
void since_you_got_that_symbol_unresolved_you_probably_use_ZB_ED_ROLE_preprocessor_define_while_linking_with_zc_library(void);
#else
#define ZB_CHECK_LIBRARY()  since_you_got_that_symbol_unresolved_you_probably_forget_use_ZB_ED_ROLE_preprocessor_define_while_linking_with_zed_library();
void since_you_got_that_symbol_unresolved_you_probably_forget_use_ZB_ED_ROLE_preprocessor_define_while_linking_with_zed_library(void);
#endif /* ZB_ED_ROLE */

/* minor internal hack for CI system */
#define ZB_SET_NS_UART_CB_STUB()

/** @endcond*/ /*internals_doc*/

/*! @addtogroup zb_general_start */
/*! @{ */
#if defined ZB_INIT_HAS_ARGS || defined DOXYGEN
/**
   Global stack initialization.

   To be called from MAIN() at start.

   Usual initialization sequence: ZB_INIT(), then assign some IB values, then zboss_start().

   @param trace_comment - trace file name component (valid for Unix, ignored at MCU)

   @b Example:
@code
  ZB_INIT("zdo_zc");
@endcode

  See any sample
 */
#define ZB_INIT(trace_comment)               \
{                                            \
  ZB_CHECK_LIBRARY();                        \
  zb_init((zb_char_t *)trace_comment);       \
  ZB_SET_NS_UART_CB_STUB();                  \
}
/** @cond internals_doc */
/** @brief Global stack initialization.

    Don't call directly, use ZB_INIT() instead
*/
void zb_init(zb_char_t *trace_comment);
/** @endcond */ /* internals_doc */
#else
/**
   Global stack initialization.

   To be called from MAIN() at start.

   Usual initialization sequence: ZB_INIT(), then assign some IB values, then zboss_start().

   @param trace_comment - trace file name component (valid for Unix, ignored at MCU)

   @b Example:
@code
  ZB_INIT("zdo_zc");
@endcode

  See any sample
 */
#define ZB_INIT(trace_comment)               \
{                                            \
  ZB_CHECK_LIBRARY();                        \
  zb_init();                                 \
  ZB_SET_NS_UART_CB_STUB();                  \
}
void zb_init(void);
#endif  /* ZB_INIT_HAS_ARGS || defined DOXYGEN */
/*! @} */ /* zb_general_start */

/*! @addtogroup zb_general_set */
/*! @{ */
/**
   Set channel mask on device.
   @param channel_mask - Zigbee channel mask

   @b Example:
@code
  zb_set_channel((1l<<13) | (1l<<14));
@endcode
*/
void zb_set_channel_mask(zb_uint32_t channel_mask);

/** @cond DOXYGEN_BDB_SECTION */

#ifdef ZB_OSIF_CONFIGURABLE_TX_POWER
/**
   Set transmit power.
   @param tx_power - Transmit power [dBm].

   @b Example:
@code
  zb_set_tx_power(0x32);
@endcode
*/
zb_ret_t zb_set_tx_power(zb_uint8_t tx_power);
#endif

/** @endcond */ /* DOXYGEN_BDB_SECTION */

/**
   Set RxOnWhenIdle attribute
   @param rx_on - attribute value


   @b Example:
@code
  zb_set_rx_on_when_idle(ZB_FALSE); // switch device to rx off
@endcode
*/
void zb_set_rx_on_when_idle(zb_bool_t rx_on);

/*! @} */ /* zb_general_set */

/*! @addtogroup zb_general_get */
/*! @{ */

/**
   Returns current state of RX-ON-When-Idle of Zigbee ZED

   @return ZB_TRUE if ZED is not sleepy ZED, ZB_FALSE if ZED is a sleepy ZED
 */
zb_bool_t zb_get_rx_on_when_idle(void);

/*! @} */ /* zb_general_get */

/*! @addtogroup zb_general_start */
/*! @{ */

/** @brief ZBOSS start function.
  *
  * Typical device start: init, load some parameters from NVRAM and proceed with startup.
  *
  * Startup means either Formation (for ZC), rejoin or discovery/association join.  After startup
  * complete @ref zboss_signal_handler callback is called, so application will know when to do
  * some useful things.
  *
  * Precondition: stack must be inited by @ref ZB_INIT() call. @ref ZB_INIT() sets default IB
  * parameters, so caller has a chance to change some of them. Note that NVRAM will be loaded after
  * zboss_start() call.
  * @note ZB is not looped in this routine. Instead, it schedules callback and returns.  Caller
  * must run @ref zboss_main_loop() after this routine.
  *
  * @return RET_OK on success.
  */
zb_ret_t zboss_start(void);


/*! @} */ /* zb_general_start  */

/*! @addtogroup zb_general_get */
/*! @{ */
/**
 *  @brief Get ZBOSS version.
 *  @returns Pointer to zero-terminated version string.
 */
const zb_char_t ZB_IAR_CODE *zb_get_version(void);
/*! @} */ /* zb_general_get */

/*! @addtogroup zb_general_start */
/*! @{ */

/**
   Start ZBOSS with only minimal initialization, without starting commissioning.

   This function initializes scheduler and buffers pool, but not MAC and upper layers.
   Typically zboss_start_no_autostart() is used when application wants to do something before
   starting joining the network.

   For example, you can use this function if it is needed to enable leds, timers
   or any other devices on periphery to work with them before starting working in a network. It's
   also useful if you want to run something locally during joining.

   Application should later call ZBOSS commissioning initiation - for
   instance,
   @if zb_bdb_mode
      @ref bdb_start_top_level_commissioning().
   @else
      bdb_start_top_level_commissioning()
   @endif


   To finish node initialization without triggering commissioning call @ref
   bdb_start_top_level_commissioning() with commissioning mask 0 (no steering, no formation,
   initialization only).

   @return RET_OK on success.
 */
zb_ret_t zboss_start_no_autostart(void);

void zboss_start_continue(void);

#ifdef ZB_PROMISCUOUS_MODE

/**
   Start ZBOSS in the sniffer mode

   Initialize ZBOSS MAC layer to work as a sniffer.
   Once ZBOSS is initialized in the sniffer mode, it can't be commissioned in the
   normal mode until reboot.
 */
zb_ret_t zboss_start_in_sniffer_mode(void);

/**
   Start sniffing

   ZBOSS must be started in the sniffer mode.

   @param channel - channel to work on
   @param data_ind_cb - callback to be called to pass data to the sniffer application
 */
void zboss_sniffer_start(zb_uint8_t channel, zb_callback_t data_ind_cb);

/**
   Stop sniffing or do nothing if not sniffing now.

   ZBOSS must be started in the sniffer mode.
 */
void zboss_sniffer_stop(void);
#endif  /* ZB_PROMISCUOUS_MODE */

/*! @} */ /* zb_general_start */

/*! @addtogroup zb_general_get */
/*! @{ */

/** @cond internals_doc */
zb_bool_t zb_zdo_joined(void);
/** @endcond */

/**
   Checks that device is joined to the network.
   @return true if device is joined; false - otherwise
*/
#define ZB_JOINED()  zb_zdo_joined()

/*! @} */ /*zb_general_get */

/*! @addtogroup zb_general_main_loop */
/*! @{ */

/**
   ZBOSS main loop.
   Must be called after ZB_INIT() and zboss_start().
   Loops infinitely.

   @snippet light_sample/dimmable_light/bulb.c zboss_main_loop_init
   @snippet light_sample/dimmable_light/bulb.c zboss_main_loop
*/
void zboss_main_loop(void);

/**
   ZBOSS main loop iteration.

   This function is useful if application wants to do something special in the
   main loop without involving ZBOSS scheduler.

   Must be called after ZB_INIT() and zboss_start() / zboss_start_no_autostart()
   inside the application's main cycle.
   @snippet onoff_server/on_off_switch_zed.c zboss_main_loop_iteration_declaring
   @snippet onoff_server/on_off_switch_zed.c zboss_main_loop_iteration_usage
*/

void zboss_main_loop_iteration(void);

/*! @} */ /* zb_general_main_loop */

/*! @addtogroup zb_general_set */
/*! @{ */

/**
   Set 64-bit long address
   @param addr - long address structure
*/
void zb_set_long_address(const zb_ieee_addr_t addr);

/** @fn void zb_set_pan_id(zb_uint16_t pan_id)
 *  @brief Set 16-bit PAN ID
 *  @param[in] pan_id - PAN ID
 */
void zb_set_pan_id(zb_uint16_t pan_id);

/**
   Set Manufacturer code of Node Descriptor
   @param manuf_code - Manufacturer code
*/
void zb_set_node_descriptor_manufacturer_code(zb_uint16_t manuf_code);

/**
   Set Extended Pan ID (apsUseExtendedPANID attribute)
   @param ext_pan_id - Long (64-bit) Extended Pan ID
*/
void zb_set_extended_pan_id(const zb_ext_pan_id_t ext_pan_id);

/*! @} */ /* zb_general_set */

/*! @addtogroup zb_general_get */
/*! @{ */

/**
   Get 64-bit long address
   @param addr - pointer to memory where result will be stored
 */
void zb_get_long_address(zb_ieee_addr_t addr);

/**
    Get 16-bit PAN ID
*/
zb_uint16_t zb_get_pan_id(void);

/** Get current short address of the device
 */
zb_uint16_t zb_get_short_address(void);

/**
   Get Extended Pan ID (nwkExtendedPANId attribute)
   @param ext_pan_id - pointer to memory where result will be stored
 */
void zb_get_extended_pan_id(zb_ext_pan_id_t ext_pan_id);

/**
   Get the currently used channel page.
*/
zb_uint8_t zb_get_current_page(void);

/**
   Get the currently used channel.
*/
zb_uint8_t zb_get_current_channel(void);

/*! @} */ /* zb_general_get */

/*! @addtogroup zb_general_set */
/*! @{ */

/**
   Initiate device as a Zigbee 3.0 (not SE!) coordinator
   @param channel_mask - Zigbee channel mask
*/
void zb_set_network_coordinator_role(zb_uint32_t channel_mask);
/**
   Initiate device as a legacy (pre-r21) Zigbee coordinator
   @param channel_mask - Zigbee channel mask
*/
void zb_set_network_coordinator_role_legacy(zb_uint32_t channel_mask);

/**
   Initiate device as a Zigbee Zigbee 3.0 (not SE!) router
   @param channel_mask - Zigbee channel mask
*/
void zb_set_network_router_role(zb_uint32_t channel_mask);
/**
   Initiate device as a legacy (pre-r21) Zigbee router
   @param channel_mask - Zigbee channel mask
*/
void zb_set_network_router_role_legacy(zb_uint32_t channel_mask);

/**
   Initiate device as a Zigbee Zigbee 3.0 (not SE!) End Device
   @param channel_mask - Zigbee channel mask
*/
void zb_set_network_ed_role(zb_uint32_t channel_mask);

/**
   Initiate device as a legacy (pre-r21) Zigbee End Device
   @param channel_mask - Zigbee channel mask
*/
void zb_set_network_ed_role_legacy(zb_uint32_t channel_mask);

/**
   Initiate device as a Zigbee 3.0 BDB coordinator with channel list.
   Provides functionality to set mask for 2.4 GHz or Sub-GHz page.
   @param channel_list - Zigbee channels list
*/
void zb_set_network_coordinator_role_ext(zb_channel_list_t channel_list);

/**
   Initiate device as a Zigbee 3.0 BDB End Device with channel list.
   Provides functionality to set mask for 2.4 GHz or Sub-GHz page.
   @param channel_list - Zigbee channels list
*/
void zb_set_network_ed_role_ext(zb_channel_list_t channel_list);

/** @} */
/** @addtogroup zb_general_get
@{
*/

/*! @addtogroup zb_general_get */
/*! @{ */

/**
 * Returns the device role ID
 *
 * @return - zb_nwk_device_type_t device_role_id
 */
zb_nwk_device_type_t zb_get_network_role(void);

/*! @} */ /* zb_general_get */

/** @addtogroup zb_general_set
@{
*/

void zb_se_set_bdb_mode_enabled(zb_uint8_t enabled);

/**
   Enum for channel pages' numbers
  */
enum zb_channel_page_num_e
{
  ZB_CHANNEL_PAGE0_2_4_GHZ  =  0,
  ZB_CHANNEL_PAGE28_SUB_GHZ = 28,
  ZB_CHANNEL_PAGE29_SUB_GHZ = 29,
  ZB_CHANNEL_PAGE30_SUB_GHZ = 30,
  ZB_CHANNEL_PAGE31_SUB_GHZ = 31,
};

/**
   Initialize a channel list
   @param channel_list - pointer to a channel list
 */
void zb_channel_list_init(zb_channel_list_t channel_list);


/**
   Add channel mask for a specified channel page in a channel list
   @param channel_list - pointer to a channel list
   @param page_num - channel page number - @ref zb_channel_page_num_e
   @param channel_mask - Zigbee channel mask

   @return RET_OK if ok, else error code
 */
zb_ret_t zb_channel_list_add(zb_channel_list_t channel_list, zb_uint8_t page_num, zb_uint32_t channel_mask);

/**
   Set the maximum number of connected devices. Used for coordinators
   and routers
   @param max_children - maximum number of connected devices.
*/
void zb_set_max_children(zb_uint8_t max_children);


/**
 *  Enumeration, which store all values, which can be used in End Device timeout request
 */
typedef enum nwk_requested_timeout_e
{
  ED_AGING_TIMEOUT_10SEC = 0, /*!< 10 second timeout */
  ED_AGING_TIMEOUT_2MIN,      /*!< 2 minutes */
  ED_AGING_TIMEOUT_4MIN,      /*!< 4 minutes */
  ED_AGING_TIMEOUT_8MIN,      /*!< 8 minutes */
  ED_AGING_TIMEOUT_16MIN,     /*!< 16 minutes */
  ED_AGING_TIMEOUT_32MIN,     /*!< 32 minutes */
  ED_AGING_TIMEOUT_64MIN,     /*!< 64 minutes */
  ED_AGING_TIMEOUT_128MIN,    /*!< 128 minutes */
  ED_AGING_TIMEOUT_256MIN,    /*!< 256 minutes */
  ED_AGING_TIMEOUT_512MIN,    /*!< 512 minutes */
  ED_AGING_TIMEOUT_1024MIN,   /*!< 1024 minutes */
  ED_AGING_TIMEOUT_2048MIN,   /*!< 2048 minutes */
  ED_AGING_TIMEOUT_4096MIN,   /*!< 4096 minutes */
  ED_AGING_TIMEOUT_8192MIN,   /*!< 8192 minutes */
  ED_AGING_TIMEOUT_16384MIN   /*!< 16384 minutes */
} nwk_requested_timeout_t;

/**
   Set end device timeout.

   Use it on End Device to specify End Device timeout period used in ED Timeout request.
   @param to - index in @ref nwk_requested_timeout_e
*/
void zb_set_ed_timeout(zb_uint_t to);

/**
   Set keep alive timeout.

   Use it on End Device to specify how often device should poll its parent.
   @param to - timeout, in beacon intervals
*/
void zb_set_keepalive_timeout(zb_uint_t to);

/** @} */ /* zb_general_set */


/**
 *  @addtogroup zb_comm_signals
 * @{
 */

/**
   ZBOSS application signal handler.

   Mandatory for all applications implemented on the top of ZBOSS stack.
   See @ref zb_zdo_app_signal_type_e.

   @param param - reference to the buffer which contains signal. See @ref zb_get_app_signal.
*/
void zboss_signal_handler(zb_uint8_t param);

/** @} */ /* zb_comm_signals */

/*! @endcond */ /* DOXYGEN_GENERAL_SECTION */

/*! @cond DOXYGEN_LL_SECTION */
/*! @addtogroup zboss_nvram */
/** @{ */

/**
 * Enum All NVRAM dataset types
 *
 * ZB_NVRAM_DATASET_NUMBER - count of dataset types. Not a real dataset type!
 */
typedef enum zb_nvram_dataset_types_e
{
  /* Do not use 0 as dataset type: it can be used internally */
  ZB_NVRAM_RESERVED              = 0, /**< Reserved value */
  ZB_NVRAM_COMMON_DATA           = 1, /**< Dataset, contains common Zigbee data */
  ZB_NVRAM_HA_DATA               = 2, /**< Dataset, contains HA profile Zigbee data */
  ZB_NVRAM_ZCL_REPORTING_DATA    = 3, /**< Dataset, contains ZCL reporting data */
  ZB_NVRAM_APS_SECURE_DATA_GAP   = 4, /**< Reserved value */
  ZB_NVRAM_APS_BINDING_DATA_GAP  = 5, /**< Reserved value */
  ZB_NVRAM_HA_POLL_CONTROL_DATA  = 6, /**< Dataset, contains HA POLL CONTROL data */
  ZB_IB_COUNTERS                 = 7, /**< Dataset, contains NIB outgoing frame counter */
  ZB_NVRAM_DATASET_GRPW_DATA     = 8, /**< Green Power dataset */
  ZB_NVRAM_APP_DATA1             = 9, /**< Application-specific data #1 */
  ZB_NVRAM_APP_DATA2             = 10, /**< Application-specific data #2 */
  ZB_NVRAM_ADDR_MAP              = 11, /**< Dataset stores address map info */
  ZB_NVRAM_NEIGHBOUR_TBL         = 12, /**< Dataset stores Neighbor table info */
  ZB_NVRAM_INSTALLCODES          = 13, /**< Dataset contains APS installcodes data */
  ZB_NVRAM_APS_SECURE_DATA       = 14, /**< Dataset, contains APS secure keys data */
  ZB_NVRAM_APS_BINDING_DATA      = 15, /**< Dataset, contains APS binding data */
  ZB_NVRAM_DATASET_GP_PRPOXYT    = 16, /**< Green Power Proxy table */
  ZB_NVRAM_DATASET_GP_SINKT      = 17, /**< Green Power Sink table */
  ZB_NVRAM_DATASET_GP_CLUSTER    = 18, /**< Green Power Cluster data */
  ZB_NVRAM_APS_GROUPS_DATA       = 19, /**< Dataset, contains APS groups data */
  ZB_NVRAM_DATASET_SE_CERTDB     = 20, /**< Smart Energy Dataset - Certificates DataBase */
  ZB_NVRAM_ZCL_WWAH_DATA         = 21, /**< Dataset, contains ZCL WWAH data */
  ZB_NVRAM_DATASET_GP_APP_TBL    = 22, /**< Dataset, contains ZCL WWAH data */
  /* Note: added new app_data datasets down and created a hole for new system datasets.
   */
  ZB_NVRAM_APP_DATA3             = 27, /**< Application-specific data #3 */
  ZB_NVRAM_APP_DATA4             = 28, /**< Application-specific data #4 */
  ZB_NVRAM_DATASET_NUMBER,             /**< Count of Dataset */
  ZB_NVRAM_DATA_SET_TYPE_PAGE_HDR = 0x1e /**< Special internal dataset type  */
} zb_nvram_dataset_types_t;


/**
 * Declares application callback used for reading application datasets from NVRAM.
 *
 * @param page - page in NVRAM from data will be read
 * @param pos - offset in page
 * @param payload_length - number of bytes to read
 */
typedef void (*zb_nvram_read_app_data_t)(zb_uint8_t page, zb_uint32_t pos, zb_uint16_t payload_length);

/**
 * Declares application callback used for writing application datasets into NVRAM.
 *
 * @param page - page in NVRAM where data will be stored
 * @param pos - offset in page
 *
 * @return - RET_OK on successful write
 */
typedef zb_ret_t (*zb_nvram_write_app_data_t)(zb_uint8_t page, zb_uint32_t pos);

/**
 * Returns size of application datasets
 *
 * @return - size of application dataset in bytes
 */
typedef zb_uint16_t (*zb_nvram_get_app_data_size_t)(void);

/**
 * Register application callback for reading @ref ZB_NVRAM_APP_DATA1 from NVRAM
 *
 * @param cb - callback that will be called upon reading dataset
 */
void zb_nvram_register_app1_read_cb(zb_nvram_read_app_data_t cb);

/**
 * Register application callback for reading @ref ZB_NVRAM_APP_DATA2 from NVRAM
 *
 * @param cb - callback that will be called upon reading dataset
 */
void zb_nvram_register_app2_read_cb(zb_nvram_read_app_data_t cb);

/**
 * Register application callback for reading @ref ZB_NVRAM_APP_DATA3 from NVRAM
 *
 * @param cb - callback that will be called upon reading dataset
 */
void zb_nvram_register_app3_read_cb(zb_nvram_read_app_data_t cb);

/**
 * Register application callback for reading @ref ZB_NVRAM_APP_DATA4 from NVRAM
 *
 * @param cb - callback that will be called upon reading dataset
 */
void zb_nvram_register_app4_read_cb(zb_nvram_read_app_data_t cb);

/**
 * Register application callback for writing @ref ZB_NVRAM_APP_DATA1 to NVRAM
 *
 * @param wcb - callback that will be called upon writing
 * @param gcb - this callback will be called upon writing to determine dataset size
 */
void zb_nvram_register_app1_write_cb(
    zb_nvram_write_app_data_t wcb,
    zb_nvram_get_app_data_size_t gcb);

/**
 * Register application callback for writing @ref ZB_NVRAM_APP_DATA2 to NVRAM
 *
 * @param wcb - callback that will be called upon writing
 * @param gcb - this callback will be called upon writing to determine dataset size
 */
void zb_nvram_register_app2_write_cb(
    zb_nvram_write_app_data_t wcb,
    zb_nvram_get_app_data_size_t gcb);

/**
 * Register application callback for writing @ref ZB_NVRAM_APP_DATA3 to NVRAM
 *
 * @param wcb - callback that will be called upon writing
 * @param gcb - this callback will be called upon writing to determine dataset size
 */
void zb_nvram_register_app3_write_cb(
    zb_nvram_write_app_data_t wcb,
    zb_nvram_get_app_data_size_t gcb);

/**
 * Register application callback for writing @ref ZB_NVRAM_APP_DATA4 to NVRAM
 *
 * @param wcb - callback that will be called upon writing
 * @param gcb - this callback will be called upon writing to determine dataset size
 */
void zb_nvram_register_app4_write_cb(
    zb_nvram_write_app_data_t wcb,
    zb_nvram_get_app_data_size_t gcb);

/**
 * Write specified dataset into NVRAM
 *
 * @param t - dataset index, see @ref zb_nvram_dataset_types_e
 *
 * @b Example @b:
 * @snippet light_sample/dimmable_light/bulb.c nvram_usage_example
 *
 * See light_sample
 */
void zb_nvram_write_dataset(zb_nvram_dataset_types_t t);
/** @cond internals_doc */
/**
 * Clears all datasets except @ref ZB_IB_COUNTERS and application datasets.
 */
void zb_nvram_clear(void);
/** @endcond */
/**
   Erase Informational Bases to NVRAM or other persistent storage
 */
void zb_nvram_erase(void);

/**
   Enable or disable NVRAM erasing on every application startup.
   @param erase - 1 to enable erasing; 0 - disable.
   Erasing is disabled by default.
*/
void zb_set_nvram_erase_at_start(zb_bool_t erase);

/**
   Start NVRAM transaction.

   Start writing multiple datasets at once.
   A transaction must finished by calling zb_nvram_transaction_commit().
 */
void zb_nvram_transaction_start(void);

/**
   Commit NVRAM transaction.

   Finish writing multiple datasets at once.
   A transaction must started by calling zb_nvram_transaction_start().
 */
void zb_nvram_transaction_commit(void);

/** @} */ /* zboss_nvram */
/*! @endcond */ /* DOXYGEN_LL_SECTION */

/** @cond internals_doc */
#define ZB_INVALID_TX_POWER_VALUE     0x7F
/** @endcond */

#ifdef ZB_PRODUCTION_CONFIG
/** @cond internals_doc */

/** Maximum size of production configuration */
#define ZB_PRODUCTION_CONFIG_APP_MAX_SIZE 128

typedef ZB_PACKED_PRE struct zb_production_config_hdr_s
{
  zb_uint32_t crc; /*!< crc for the whole production configuration (including application specific part) */
  zb_uint16_t len; /*!< length of production configuration (including application specific part) */
  zb_uint16_t version; /*!< Version of production configuration (reserved for future changes) */
}
ZB_PACKED_STRUCT zb_production_config_hdr_t;

typedef ZB_PACKED_PRE struct zb_production_config_ver_1_s
{
  zb_production_config_hdr_t hdr; /*!< Header */
  zb_uint32_t aps_channel_mask;   /*!< APS Channel Mask  */
  zb_64bit_addr_t extended_address; /*!< IEEE address */
  zb_int8_t mac_tx_power[16]; /*!< Tx power specified for every possible channel */
  zb_uint8_t install_code[ZB_CCM_KEY_SIZE+2]; /*!< Installation code + its own crc */
}
ZB_PACKED_STRUCT zb_production_config_ver_1_t;

#define ZB_PROD_CFG_APS_CHANNEL_LIST_SIZE 5
#define ZB_PROD_CFG_MAC_TX_POWER_CHANNEL_N 27

#define ZB_PROD_CFG_OPTIONS_IC_TYPE_MASK      0x03
#define ZB_PROD_CFG_OPTIONS_CERT_PRESENT_MASK 0x80

typedef ZB_PACKED_PRE struct zb_production_config_ver_2_s
{
  zb_production_config_hdr_t hdr; /*!< Header */
  zb_channel_page_t aps_channel_mask_list[ZB_PROD_CFG_APS_CHANNEL_LIST_SIZE]; /*!< Channel mask for device to operate on */
  zb_64bit_addr_t extended_address; /*!< IEEE address */
  zb_int8_t mac_tx_power[ZB_PROD_CFG_APS_CHANNEL_LIST_SIZE][ZB_PROD_CFG_MAC_TX_POWER_CHANNEL_N]; /*! < Tx power specified for every possible channel */
  zb_uint8_t options;/*low 2 bits - ic_type field *//*7th bit - certificates block presents*/
  zb_uint8_t install_code[ZB_CCM_KEY_SIZE+2]; /*!< Installation code + its own crc */
}
ZB_PACKED_STRUCT zb_production_config_ver_2_t;

typedef zb_production_config_ver_2_t zb_production_config_t;

#define ZB_CS1_PUBLISHER_PUBLIC_KEY_SIZE 22
#define ZB_CS1_CERTIFICATE_SIZE 48
#define ZB_CS1_PRIVATE_KEY_SIZE 21

#define ZB_CS2_PUBLISHER_PUBLIC_KEY_SIZE 37
#define ZB_CS2_CERTIFICATE_SIZE 74
#define ZB_CS2_PRIVATE_KEY_SIZE 36

typedef ZB_PACKED_PRE struct zb_cs1_key_material_s
{
  zb_uint8_t publisher_public_key[ZB_CS1_PUBLISHER_PUBLIC_KEY_SIZE];
  zb_uint8_t certificate[ZB_CS1_CERTIFICATE_SIZE];
  zb_uint8_t private_key[ZB_CS1_PRIVATE_KEY_SIZE];
} ZB_PACKED_STRUCT zb_cs1_key_material_t;

typedef ZB_PACKED_PRE struct zb_cs2_key_material_s
{
  zb_uint8_t publisher_public_key[ZB_CS2_PUBLISHER_PUBLIC_KEY_SIZE];
  zb_uint8_t certificate[ZB_CS2_CERTIFICATE_SIZE];
  zb_uint8_t private_key[ZB_CS2_PRIVATE_KEY_SIZE];
} ZB_PACKED_STRUCT zb_cs2_key_material_t;

typedef ZB_PACKED_PRE struct zb_cs_key_material_header_s
{
  zb_uint16_t certificate_mask;
} ZB_PACKED_STRUCT zb_cs_key_material_header_t;

#define ZB_PROD_CFG_IS_PRESENT_CERT_ANY(v) ((v->options) & 0x80)
/** @endcond */ /*internals_doc*/

/**
 * Disable usage of production configuration at runtime.
 *
 * @param val - ZB_TRUE means production configuration is disabled, ZB_FALSE - enabled.
 */
void zb_production_config_disable(zb_bool_t val);

/**
 * Get state of production configuration disabling.
 *
 * @return ZB_TRUE if production configuration is disabled, ZB_FALSE - enabled.
 */
zb_bool_t zb_is_production_config_disabled(void);

zb_ret_t zb_production_config_check(zb_production_config_hdr_t *prod_cfg_hdr);

#endif /* ZB_PRODUCTION_CONFIG */




#ifdef ZB_USE_SLEEP
/*! @cond DOXYGEN_LL_SECTION */
/*! @addtogroup sleep_api */
/*! @{ */

/** Default sleep threshold. Do not sleep when it is less then 1 Beacon Interval to wake up. */
#define ZB_SCHED_SLEEP_THRESHOLD_MS 20

/** Maximum sleep threshold. */
#define ZB_MAXIMUM_SLEEP_THRESHOLD_MS 86400000

/**
  Set sleep threshold on device; when scheduler detects that device can be put in sleep mode
  it will notify application with signal @see ZB_COMMON_SIGNAL_CAN_SLEEP.
  Device can not be put into sleep mode when sleep interval less than this threshold.

  @param threshold_ms - sleep threshold in milliseconds
             If threshold is 0, means zero threshold, application will be notified each time when stack is ready to sleep
             (no immediate callbacks in queue).
  @return RET_OK if new threshold is valid and applied
  @return RET_ERROR if user wants to set threshold greater that @ref ZB_MAXIMUM_SLEEP_THRESHOLD_MS or lesser than ZB_SCHED_SLEEP_THRESHOLD_MS.
*/
zb_ret_t zb_sleep_set_threshold(zb_uint32_t threshold_ms);

/**
  Get sleep threshold value from stack.
  @return threshold value in milliseconds
*/
zb_uint32_t zb_get_sleep_threshold(void);

/**
  Blocking function responsible for putting device into sleep mode.

*/
void zb_sleep_now(void);

/*! @} */ /* sleep_api */
/*! @endcond */ /* DOXYGEN_LL_SECTION */
#endif /* ZB_USE_SLEEP */



#ifdef ZB_SECURITY_INSTALLCODES
/**
  Set using of install codes by TC

  @param use_ic - enable/disable install code usage
*/
void zb_tc_set_use_installcode(zb_uint8_t use_ic);
#endif /* ZB_SECURITY_INSTALLCODES */

#if defined ZB_ED_FUNC && defined ZB_CONTROL4_NETWORK_SUPPORT
/**
  Permit joining Control4 Network
*/
void zb_permit_control4_network(void);

/**
   Return if joining Control4 Network is allowed
*/
zb_bool_t zb_control4_network_permitted(void);

#endif /* defined ZB_ED_FUNC && defined ZB_CONTROL4_NETWORK_SUPPORT */

/**
 * @brief Set the 'macTransactionPersistenceTime' attribute in the MAC PIB
 *
 * @param ms - value in milliseconds
 */
void zb_set_mac_transaction_persistence_time(zb_uint16_t ms);

#endif /*ZBOSS_API_H*/
