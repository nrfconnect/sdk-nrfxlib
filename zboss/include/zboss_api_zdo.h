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
/*  PURPOSE: Public ZDO layer API
*/
#ifndef ZB_ZBOSS_API_ZDO_H
#define ZB_ZBOSS_API_ZDO_H 1

#ifdef ZB_ENABLE_ZGP_SINK
#include "zboss_api_zgp.h"
#endif

/*! \addtogroup zb_comm_signals */
/*! @{ */
/**
   ZDP status values
   (2.4.5 ZDP Enumeration Description)
 */
typedef enum zb_zdp_status_e
{
   ZB_ZDP_STATUS_SUCCESS         = 0x00, /**< The requested operation or transmission
                                          *was completed successfully. */

   ZB_ZDP_STATUS_INV_REQUESTTYPE = 0x80, /**< The supplied request type was invalid. */

   ZB_ZDP_STATUS_DEVICE_NOT_FOUND = 0x81,/**< The requested device did not exist on a device
                                          * following a child descriptor request to a parent.*/

   ZB_ZDP_STATUS_INVALID_EP = 0x82, /**< The supplied endpoint was equal to 0x00 or
                                    * between 0xf1 and 0xff. */

   ZB_ZDP_STATUS_NOT_ACTIVE = 0x83,/**< The requested endpoint is not described by
                                   *a simple descriptor. */

   ZB_ZDP_STATUS_NOT_SUPPORTED = 0x84,/**< The requested optional feature
                                      *is not supported on the target device. */

   ZB_ZDP_STATUS_TIMEOUT = 0x85,/**< A timeout has occurred with the requested operation. */

   ZB_ZDP_STATUS_NO_MATCH = 0x86,/**< The end device bind request was unsuccessful due to a failure
                                  * to match any suitable clusters.*/

   ZB_ZDP_STATUS_NO_ENTRY = 0x88,/**< The unbind request was unsuccessful due to the coordinator or
                                  * source device not having an  entry in its binding table to unbind.*/

   ZB_ZDP_STATUS_NO_DESCRIPTOR = 0x89,/**< A child descriptor was not available following a
                                       * discovery request to a parent. */

   ZB_ZDP_STATUS_INSUFFICIENT_SPACE = 0x8a,/**< The device does not have storage space to support
                                            * the requested operation. */

   ZB_ZDP_STATUS_NOT_PERMITTED = 0x8b,/**< The device is not in the proper state to support the
                                       * requested operation. */

   ZB_ZDP_STATUS_TABLE_FULL = 0x8c,/**< The device does not have table space to support the
                                    * operation. */

   ZB_ZDP_STATUS_NOT_AUTHORIZED = 0x8d,/**< The permissions configuration table on the target
                                        * indicates that the request is not authorized from this
                                        * device.*/

   ZB_ZDP_STATUS_INVALID_INDEX = 0x8f,/**< The index in the received command is out of bounds. */

   ZB_ZDP_STATUS_TIMEOUT_BY_STACK = 0xff,/**< Custom internal statuses. */
}
zb_zdp_status_t;

/**
 * The
 * signals passed to zboss_signal_handler()
 */

/**
 * @note If an application requires the parameters to be passed to the @ref zboss_signal_handler, it is recommended
 * to inherit @ref zb_zdo_app_signal_type_t and introduce the structure according to the pattern below:
 *
 * @code
 * typedef struct zb_zdo_app_my_custom_signal_s
 * {
 *   zb_zdo_app_signal_type_t super;
 *   zb_my_custom_data_type_t my_custom_data;
 * }
 * @endcode
 *
 */
typedef enum zb_zdo_app_signal_type_e
{
  ZB_ZDO_SIGNAL_DEFAULT_START,  /*!< Obsolete signal, used for pre-R21 ZBOSS API
                                 * compatibility. Not recommended to use.
                                 * @parblock
                                 * When generated:
                                 *  - After the device has been started and the non-BDB commissioning
                                 * completed.
                                 *  - In case of the commissioning error.
                                 *
                                 * Status codes:
                                 *  - RET_OK: Device has started and joined the network.
                                 *  - RET_ERROR: An error of any type.
                                 *
                                 * Signal parameters:
 				 *  - none
				   @endparblock */
  ZB_ZDO_SIGNAL_SKIP_STARTUP,   /*!< Notifies the application that ZBOSS framework (scheduler,
                                 * buffer pool, etc.) has started, but no join/rejoin/formation/BDB
                                 * initialization has been done yet.
                                 * @parblock
                                 * When generated:
                                 *  - When the application calls
                                 * zboss_start_no_autostart() instead of
                                 * zboss_start() before the
                                 * zboss_main_loop(). Used
                                 * when some hardware must be initialized before the
                                 * Zigbee commissioning, but already has ZBOSS
                                 * scheduler running. Example: sensor connected through UART.
                                 *
                                 * Status codes:
                                 *  - RET_OK: Only ZBOSS framework has been initialized.
                                 *  - Does not return error status.
                                 *
                                 * Signal parameters:
 				                         *  - none
                                 *
                                 * @snippet smart_plug_v2/sp_device/sp_device.c signal_skip_startup
                                 * @endparblock */
  ZB_ZDO_SIGNAL_DEVICE_ANNCE,   /*!< Notifies the application about the new device appearance.
                                 * @parblock
                                 * When generated:
                                 *  - Upon receiving the Device_annce command.
                                 *
                                 * Status codes:
                                 *  - RET_OK:  Device_annce command was received.
                                 *  - Does not return error status.
                                 *
                                 * Signal parameters:
                                 * - @ref zb_zdo_signal_device_annce_params_t
                                 *
                                 * @snippet simple_gw/simple_gw.c signal_device_annce
                                 * @endparblock */
  ZB_ZDO_SIGNAL_LEAVE,          /*!< Notifies the application that the device itself has left the network.
                                 * @parblock
                                 * When generated:
                                 *  - Upon sending the "Leave" command.
                                 *
                                 * Status codes:
                                 *  - RET_OK: Device has sent the "Leave" command.
                                 *  - Does not return error status.
                                 *
                                 * Signal parameters:
                                 *  - @ref zb_zdo_signal_leave_params_t
                                 *
                                 * @snippet light_sample/light_control/light_control.c signal_leave
                                   @endparblock */
  ZB_ZDO_SIGNAL_ERROR,          /*!< Corrupted or incorrect signal information.
                                 * @parblock
                                 * When generated:
                                 *  - Incorrect buffer length detected by zb_get_app_signal (less then zb_zdo_app_signal_hdr_t)
                                 *
                                 * Status codes:
                                 *  - none
                                 *
                                 * Signal parameters:
                                 *  - none
                                 *  @endparblock */
  ZB_BDB_SIGNAL_DEVICE_FIRST_START, /*!< Device started for the first time after the NVRAM erase.
                                     * @parblock
                                     * When generated:
                                     *  - Upon completing the typical device startup (network formation
                                     * for ZC or rejoin/discovery/association for ZR and ZED).
                                     *  - This signal is exclusive to @ref ZB_BDB_SIGNAL_DEVICE_REBOOT
                                     *
                                     * @note No @ref ZB_BDB_SIGNAL_STEERING or @ref ZB_BDB_SIGNAL_FORMATION signals will be generated.
                                     *
                                     * Status codes:
                                     *  - RET_OK: Device started after the NVRAM erase
                                     *  - RET_ERROR: An error of any type.
                                     *
                                     * Signal parameters:
                                     *  - none
                                     *
                                     * @snippet HA_samples/on_off_switch/sample_zed.c signal_first
                                     *  @endparblock */
  ZB_BDB_SIGNAL_DEVICE_REBOOT,      /*!< Device started using the NVRAM contents.
                                     * @parblock
                                     * When generated:
                                     *  - Upon the device joining/rejoining Zigbee network using restored parameters.
                                     *  - This signal is exclusive to @ref ZB_BDB_SIGNAL_DEVICE_FIRST_START
                                     *
                                     * @note No @ref ZB_BDB_SIGNAL_STEERING or @ref ZB_BDB_SIGNAL_FORMATION signals will be generated.
                                     *
                                     * Status codes:
                                     *  - RET_OK: Device started using configuration stored in NVRAM
                                     *  - RET_ERROR: An error of any type.
                                     *
                                     * Signal parameters:
                                     *  - none
                                     *
                                     * @snippet HA_samples/on_off_switch/sample_zed.c signal_reboot
                                     * @endparblock */
/** @cond DOXYGEN_BDB_SECTION */
#define ZB_SIGNAL_DEVICE_FIRST_START ZB_BDB_SIGNAL_DEVICE_FIRST_START
#define ZB_SIGNAL_DEVICE_REBOOT ZB_BDB_SIGNAL_DEVICE_REBOOT
  ZB_BDB_SIGNAL_STEERING,           /*!< BDB network steering completed (Network steering only).
                                     * @parblock
                                     * When generated:
                                     *  - Upon completing Network steering initiated by bdb_start_top_level_commissioning(ZB_BDB_NETWORK_STEERING).
                                     *
                                     * Status codes:
                                     *  - RET_OK: Network steering completed.
                                     *  - Does not return error status.
                                     *
                                     * Has additional data of type
                                     * zb_zdo_signal_leave_indication_params_t.
                                     * Signal parameters:
                                     *  - none
                                     * @endparblock */
  ZB_BDB_SIGNAL_FORMATION,          /*!< BDB network formation completed (Network formation only).
                                     * @parblock
                                     * When generated:
                                     *  - Upon completing Network formation initiated by bdb_start_top_level_commissioning(ZB_BDB_NETWORK_FORMATION).
                                     *
                                     * Status codes:
                                     *  - RET_OK: Network formation completed.
                                     *  - Does not return error status.
                                     *
                                     * Signal parameters:
                                     *  - none
                                     * @endparblock */
  ZB_BDB_SIGNAL_FINDING_AND_BINDING_TARGET_FINISHED,     /*!< BDB finding and binding for a target endpoint completed.
                                     * @parblock
                                     * When generated:
                                     *  - F&B target timeout expires.
                                     *
                                     * Status codes:
                                     *  - RET_OK: F&B completed successfully.
                                     *  - RET_ERROR: An error of any type.
                                     *
                                     * Signal parameters:
                                     *  - none
                                     * @endparblock */
  ZB_BDB_SIGNAL_FINDING_AND_BINDING_INITIATOR_FINISHED,  /*!< BDB finding and binding for an initiator endpoint completed.
                                     * @parblock
                                     * When generated:
                                     *  - F&B with a Target succeeded or F&B initiator timeout expired or cancelled.
                                     *
                                     * Status codes:
                                     *  - @ref zb_zdo_fb_initiator_finished_status_e
                                     *
                                     * Signal parameters:
                                     *  - @ref zb_zdo_signal_fb_initiator_finished_params_t
                                     * @endparblock */

/** @endcond */ /* DOXYGEN_BDB_SECTION */
  ZB_NWK_SIGNAL_DEVICE_ASSOCIATED,  /*!< Obsolete signal, used for pre-R21 ZBOSS API
                                     * compatibility. Use ZB_ZDO_SIGNAL_DEVICE_ANNCE signal instead!
                                     * @parblock
                                     * When generated:
                                     *  - Indicates that there is a new device associated with Zigbee Coordinator or router.
                                     *
                                     * Status codes:
                                     *  - RET_OK: New device associated.
                                     *  - Does not return error status.
                                     *
                                     * Signal parameters:
                                     *  - @ref zb_nwk_signal_device_associated_params_t
                                       @endparblock */
  ZB_ZDO_SIGNAL_LEAVE_INDICATION,   /*!< Network leave indication.
                                     * @parblock
                                     * When generated:
                                     *  - The child device has left the network.
                                     *
                                     * Status codes:
                                     *  - RET_OK: Leave command received from the child device.
                                     *  - RET_ERROR: An error of any type.
                                     *
                                     * Signal parameters:
                                     *  - @ref zb_zdo_signal_leave_indication_params_t
                                     *
                                     * @snippet simple_gw/simple_gw.c signal_leave_ind
                                       @endparblock */
  ZB_BDB_SIGNAL_WWAH_REJOIN_STARTED,  /*!< WWAH Rejoin start indication.
                                       * @parblock
                                       * When generated:
                                       *  - Device lost communication with the parent and started
                                       * WWAH Rejoin procedure.
                                       *
                                       * Status codes:
                                       *  - RET_OK: Leave command received from the child device.
                                       *  - Does not return error status.
                                       *
                                       * Signal parameters: no
                                       @endparblock */
/**
   @cond DOXYGEN_ZGP_SECTION
   @addtogroup zgp_sink
   @{
*/
  ZB_ZGP_SIGNAL_COMMISSIONING,       /*!< ZGP commissioning complete signal.
                                     * @parblock
                                     * When generated:
                                     *  - GPCB left the commissioning mode.
                                     *
                                     * Status codes:
                                     * - RET_OK: commissioning complete.
                                     * - RET_ERROR: An error of any type.
                                     *
                                     * Signal parameters:
                                     *  - zb_zgp_signal_commissioning_params_s
                                     *
                                     * @snippet light_sample/light_coordinator_combo/light_zc.c signal_zgp_commissioning
                                       @endparblock */
/*! @} */ /* zgp_sink */
/** @endcond */ /* DOXYGEN_ZGP_SECTION */
  ZB_COMMON_SIGNAL_CAN_SLEEP,       /*!< Notifies the application that the MCU can sleep for the specified period of time.
                                     * @note Signal will not be generated if the sleep interval is less than sleep threshold ZB_SCHED_SLEEP_THRESHOLD_MS.
                                     * To change the sleep threshold see zb_ret_t zb_sleep_set_threshold()
                                     * @parblock
                                     * When generated:
                                     *  - ZBOSS scheduler detects that the sleep mode can be enabled on the device.
                                     *
                                     * Status codes:
                                     * - RET_OK: The sleep mode can be enabled on the device.
                                     * - Does not return error status.
                                     *
                                     * Signal parameters:
                                     *  - @ref zb_zdo_signal_can_sleep_params_t
                                     *
                                     * @snippet onoff_server/on_off_switch_zed.c signal_can_sleep
                                     * @endparblock
                                     */
  ZB_ZDO_SIGNAL_PRODUCTION_CONFIG_READY, /*!< Notifies the application that the application specific part of
                                          * the production configuration was found and restored and provides
                                          * the application with this data.
                                          * @parblock
                                          * When generated:
                                          *  - After restoring the production configuration.
                                          *
                                          * Status codes:
                                          *  - RET_OK: The application specific part of the production configuration exists.
                                          *  - RET_ERROR: An error of any type.
                                          *
                                          * Signal parameters:
                                          *  - Passed up with application specific data from production configuration.
                                            @endparblock */
  ZB_NWK_SIGNAL_NO_ACTIVE_LINKS_LEFT,    /*!< Notifies the application about the Neighbor Table expiration.
                                          * It means that the neighbor devices did not send
                                          * the Link Status messages for @ref ZB_NWK_ROUTER_AGE_LIMIT * @ref ZB_NWK_LINK_STATUS_PERIOD seconds.
                                          * @parblock
                                          * When generated:
                                          *  - All routes expired (usually when the link is down).
                                          *
                                          * Status codes:
                                          *  - RET_OK: All routers expired.
                                          *  - Does not return error status.
                                          *
                                          * Signal parameters:
                                          *  - none
                                            @endparblock */

  ZB_SIGNAL_SUBGHZ_SUSPEND,                    /*!< ZC / our MAC suspended our device tx: out of Duty cycle (sub-ghz only)
                                                 * @par Signal parameter - number of minutes to suspend, -1 if unknown
                                                 */
  ZB_SIGNAL_SUBGHZ_RESUME,                    /*!< ZC resumed our device tx (sub-ghz only)
                                                  */
  /* ****************** */
/** @endcond */ /* DOXYGEN_SE_SECTION */
#ifdef NCP_MODE
  ZB_SIGNAL_NWK_INIT_DONE,
#endif

  ZB_ZDO_SIGNAL_DEVICE_AUTHORIZED,       /*!< Notifies the Zigbee Trust center application about a new device is authorized
                                          * in the network. For Zigbee R21 devices it means that the TCLK exchange procedure
                                          * was finished, for the legacy devices it means that the
                                          * Transport key was send to the device.
                                          *
                                          * When generated:
                                          *  - Authorization success
                                          *  - Authorization failed
                                          *  - Authorization timeout
                                          *
                                          * Status codes:
                                          *  - RET_OK: Device information updated.
                                          *  - Does not return error status.
                                          * Signal parameters:
                                          *  - @ref zb_zdo_signal_device_authorized_params_t
                                          */

  ZB_ZDO_SIGNAL_DEVICE_UPDATE,           /*!< Notifies the Zigbee Trust center or parent router application about a new device
                                          * joined, rejoined or left the network.
                                          *
                                          * When generated:
                                          *  - Standard device secured rejoin
                                          *  - Standard device unsecured join
                                          *  - Device left
                                          *  - Standard device trust center rejoin
                                          *
                                          * Status codes:
                                          *  - RET_OK: Device information updated.
                                          *  - Does not return error status.
                                          *
                                          * Signal parameters:
                                          *  - @ref zb_zdo_signal_device_update_params_t
                                          */
  ZB_NWK_SIGNAL_PANID_CONFLICT_DETECTED,        /*!< PAN ID conflict has been detected. Application
                                                 * has to decide whether to initiate its resolution
                                                 * or not.
                                                 * @par Signal parameter
                                                 * @see zb_start_pan_id_conflict_resolution
                                                */

  ZB_NLME_STATUS_INDICATION,          /*!< NLME status indication. Application
                                       * may implement additional logic based on receiving some
                                       * statuses.
                                       * @par Signal parameter
                                       * @ref zb_zdo_signal_nlme_status_indication_params_t
                                       * @see zb_nwk_command_status_t is the full list of NLME
                                       * statuses
                                       */
/** @cond internals_doc */
  ZB_ZDO_APP_SIGNAL_COUNT
/** @endcond */ /* internals_doc */
} zb_zdo_app_signal_type_t;
/* Legacy API: zb_zdo_app_signal_type_t == zb_zdo_app_signal_t */
typedef zb_zdo_app_signal_type_t zb_zdo_app_signal_t;

/**
   Obtains pointer to parameters passed with application signal.

   @param sg_p - pointer to application signal
   @param type - parameter type
   @return pointer to signal parameters

@b Example
@code
  zb_zdo_signal_device_annce_params_t *dev_annce_params = ZB_ZDO_SIGNAL_GET_PARAMS(sg_p, zb_zdo_signal_device_annce_params_t);
  simple_gw_dev_annce_cb(dev_annce_params->device_short_addr);
@endcode
  */
#define ZB_ZDO_SIGNAL_GET_PARAMS(sg_p, type) ((type *)ZB_SAFE_PTR_CAST((((zb_uint8_t *)sg_p) + sizeof(zb_zdo_app_signal_hdr_t))))


/**
   @brief Cuts ZBOSS signal headers while keeping the signal parameter (if it was in the buffer).

   @param buf - pointer to a zb_buf_t structure. The buffer must be the one passed into zboss_signal_handler.
 */
#define ZB_ZDO_SIGNAL_CUT_HEADER(buf) zb_buf_cut_left(buf, sizeof(zb_zdo_app_signal_hdr_t))

/**
   @brief Leave parameters

   Stack passes this parameter to application when device leave the network.
  */
typedef struct zb_zdo_signal_leave_params_s
{
  zb_nwk_leave_type_t leave_type; /** Leave type. @ref zb_nwk_leave_type_t */
} zb_zdo_signal_leave_params_t;

/**
   @brief NLME status indication parameters

   Stack passes this parameter to application when NLME status indication is generated.
  */
typedef struct zb_zdo_signal_nlme_status_indication_params_s
{
  zb_nlme_status_indication_t nlme_status; /** NLME status indication payload */
} zb_zdo_signal_nlme_status_indication_params_t;

/**
   @brief Leave Indication parameters

   Stack passes this parameter to application upon receipt of leave indication
   primitive.
  */
typedef struct zb_zdo_signal_leave_indication_params_s
{
  zb_ieee_addr_t device_addr; /*!< Long address of device requested to leave or leaving device*/
  zb_uint8_t rejoin;          /*!< 1 if this was leave with rejoin; 0 - otherwise */
} zb_zdo_signal_leave_indication_params_t;

/** @cond DOXYGEN_SE_SECTION */
/**
 *  @brief Service Discovery Bind signal parameters
 *  @details Contains info about the device found during Service Discovery. Stack passes this
 *           parameter to the application with signal @ref ZB_SE_SIGNAL_SERVICE_DISCOVERY_DO_BIND
 */
typedef struct zb_se_signal_service_discovery_bind_params_s
{
  zb_ieee_addr_t device_addr;   /**< Long address of the device */
  zb_uint8_t endpoint;          /**< Device endpoint */
  zb_uint16_t cluster_id;       /**< Discovered cluster */
  zb_uint8_t commodity_type;    /**< Commodity type (for multiple commodity networks) */
} zb_se_signal_service_discovery_bind_params_t;
/** @endcond */

/** @cond DOXYGEN_ZGP_SECTION */
#if defined DOXYGEN && defined ZB_DOCUMENT_ZGP
/** @addtogroup zgp_sink
 *  @{
 */
#endif

#ifdef ZB_ENABLE_ZGP_SINK
/**
   @brief ZGP commissioning parameters

   Stack passes this parameter to application to notify about GPD commissioning
   result.
  */
typedef struct zb_zgp_signal_commissioning_params_s
{
  zb_zgpd_id_t zgpd_id;       /*!< Pointer to GPD ID */
  zb_zgp_comm_status_t result; /*!< commissioning result, see @ref zb_zgp_comm_status_t */
} zb_zgp_signal_commissioning_params_t;
#endif  /* ZB_ENABLE_ZGP_SINK */
/** @} */
/** @endcond */
/**
   @brief Association parameters

   Stack passes this parameter to application when some device associates to that.
  */
typedef struct zb_nwk_signal_device_associated_params_s
{
  zb_ieee_addr_t device_addr; /*!< address of associated device */
} zb_nwk_signal_device_associated_params_t;

/**
 *@brief Association parameters
 *
 * Stack passes this parameter to application when some device joins/rejoins to
 * network.
 */
typedef struct zb_zdo_signal_device_annce_params_s
{
  zb_uint16_t device_short_addr; /*!< address of device that recently joined to network */
  zb_ieee_addr_t   ieee_addr;
  zb_uint8_t       capability;
} zb_zdo_signal_device_annce_params_t;

/** @cond touchlink */

typedef struct zb_bdb_signal_touchlink_nwk_started_params_s
{
  zb_ieee_addr_t device_ieee_addr; /*!< address of device that started the network */
  zb_uint8_t endpoint;
  zb_uint16_t profile_id;
} zb_bdb_signal_touchlink_nwk_started_params_t;

typedef struct zb_bdb_signal_touchlink_nwk_joined_router_s
{
  zb_ieee_addr_t device_ieee_addr; /*!< address of device that started the network */
  zb_uint8_t endpoint;
  zb_uint16_t profile_id;
} zb_bdb_signal_touchlink_nwk_joined_router_t;

/** @endcond */ /* touchlink */

/**
   @brief Sleep signal parameters.

   Stack passes this parameter to application when device ready to sleep.
  */
typedef struct zb_zdo_signal_can_sleep_params_s
{
  zb_uint32_t sleep_tmo; /*!< sleep duration in milliseconds */
} zb_zdo_signal_can_sleep_params_t;

/**
   @brief Macsplit device boot parameters.

   Macsplit passes this parameter to application when device is booted.
  */
typedef struct zb_zdo_signal_macsplit_dev_boot_params_s
{
  zb_uint32_t    dev_version; /*!< macsplit device version */
#ifdef USE_HW_LONG_ADDR
  zb_ieee_addr_t extended_address;          /*!< The 64-bit (IEEE) address assigned to the device. */
#endif
} zb_zdo_signal_macsplit_dev_boot_params_t;

/**
 * @brief Status codes for F&B Initiator Finished signal
 */
typedef enum zb_zdo_fb_initiator_finished_status_e
{
  ZB_ZDO_FB_INITIATOR_STATUS_SUCCESS = 0, /*!< F&B with a Target on the Initiator side is success
                                           * completed */

  ZB_ZDO_FB_INITIATOR_STATUS_CANCEL, /*!< F&B on the Initiator side is canceled */

  ZB_ZDO_FB_INITIATOR_STATUS_ALARM,  /*!< F&B on the Initiator side is finished by timeout */

  ZB_ZDO_FB_INITIATOR_STATUS_ERROR, /*!< F&B on the Initiator side is failure finished */
} zb_zdo_fb_initiator_finished_status_t;

/**
 * @brief F&B Initiator Finished signal parameters
 */
typedef struct zb_zdo_signal_fb_initiator_finished_params_s
{
  /*!< Status of the F&B procedure on the Initiator side */
  zb_zdo_fb_initiator_finished_status_t status;
} zb_zdo_signal_fb_initiator_finished_params_t;

/**
 * @brief Device Updated signal parameters
 */
typedef struct zb_zdo_signal_device_update_params_s
{
  /*!< Long Address of the updated device */
  zb_ieee_addr_t long_addr;

  /*!< Short Address of the updated device */
  zb_uint16_t short_addr;

  /*!< Indicates the updated status of the device
   *
   * 0x00 = Standard device secured rejoin
   * 0x01 = Standard device unsecured join
   * 0x02 = Device left
   * 0x03 = Standard device trust center rejoin
   * 0x04 – 0x07 = Reserved
   *
   * see r21 spec, 4.4.3.2 APSME-UPDATE-DEVICE.indication,
   * Table 4.15 APSME-UPDATE-DEVICE.indication Parameters
   */
  zb_uint8_t status;
} zb_zdo_signal_device_update_params_t;

/**
 * @brief Authorization types for
 *        @ref ZB_ZDO_SIGNAL_DEVICE_AUTHORIZED
 */
/* TODO: add other types if need (SE CBKE, r23 DLK) */
typedef enum zb_zdo_authorization_type_e
{
  ZB_ZDO_AUTHORIZATION_TYPE_LEGACY, /*!< Authorization type for legacy devices ( < r21) */

  ZB_ZDO_AUTHORIZATION_TYPE_R21_TCLK, /*!< Authorization type for r21 device through TCLK */
}
zb_zdo_authorization_type_t;

/**
 * @brief Authorization statuses for
 *        ZB_ZDO_AUTHORIZATION_TYPE_LEGACY value
 *        of @ref zb_zdo_authorization_type_t type
 */
typedef enum zb_zdo_legacy_device_authorization_status_e
{
  ZB_ZDO_LEGACY_DEVICE_AUTHORIZATION_SUCCESS, /*!< Authorization success */

  ZB_ZDO_LEGACY_DEVICE_AUTHORIZATION_FAILED, /*!< Authorization failed */
}
zb_zdo_legacy_device_authorization_status_t;

/**
 * @brief Authorization statuses for
 *        ZB_ZDO_AUTHORIZATION_TYPE_R21_TCLK value
 *        of @ref zb_zdo_authorization_type_t type
 */
typedef enum zb_zdo_tclk_authorization_status_e
{
  ZB_ZDO_TCLK_AUTHORIZATION_SUCCESS, /*!< Authorization success */

  ZB_ZDO_TCLK_AUTHORIZATION_TIMEOUT, /*!< Authorization timeout */

  ZB_ZDO_TCLK_AUTHORIZATION_FAILED, /*!< Authorization failed */
}
zb_zdo_tclk_authorization_status_t;

/**
 * @brief Device Authorized signal parameters
 */
typedef struct zb_zdo_signal_device_authorized_params_s
{
  /*!< Long Address of the updated device */
  zb_ieee_addr_t long_addr;

  /*!< Short Address of the updated device */
  zb_uint16_t short_addr;

  /*!< Type of the authorization procedure,
   *   @ref zb_zdo_authorization_type_t
   */
  zb_uint8_t authorization_type;

  /*!< Status of the authorization procedure
   *   (depends on @ref zb_zdo_authorization_type_t)
   */
  zb_uint8_t authorization_status;
} zb_zdo_signal_device_authorized_params_t;

/** Application signal header */
typedef struct zb_zdo_app_signal_hdr_s
{
  /* 32 bit to have data section aligned well */
  zb_uint32_t sig_type;            /** Application signal type, @ref zb_zdo_app_signal_type_t */
} zb_zdo_app_signal_hdr_t;
/* Legacy API: zb_zdo_app_signal_hdr_t == zb_zdo_app_event_t */
typedef zb_zdo_app_signal_hdr_t zb_zdo_app_event_t;

/** @} */ /* comm_signals*/
/*! \addtogroup zdo_base */
/*! @{ */

#define ZB_ZDO_INVALID_TSN 0xFF

typedef ZB_PACKED_PRE struct zb_zdo_callback_info_s
{
  zb_uint8_t tsn;
  zb_uint8_t status;
} ZB_PACKED_STRUCT zb_zdo_callback_info_t;

typedef ZB_PACKED_PRE struct zb_zdo_default_resp_s
{
  zb_uint8_t tsn;
  zb_uint8_t status;
} ZB_PACKED_STRUCT zb_zdo_default_resp_t;

/** @} */
/*! \addtogroup zb_comm_signals */
/*! @{ */

/**
   @brief Unpack application signal buffer in zboss_signal_handler()

   @param param - parameter of zboss_signal_handler()
   @param sg_p - pointer to the extended signal info, if application wants it. Can be NULL.

   @return Application signal
 */

zb_zdo_app_signal_type_t zb_get_app_signal(zb_uint8_t param, zb_zdo_app_signal_hdr_t **sg_p);
/** @cond internals_doc */
/* Legacy API: zb_get_app_event(param, sg_p) == zb_get_app_signal(param, sg_p) */
#define zb_get_app_event(param, sg_p) zb_get_app_signal(param, sg_p)
/** @endcond */ /* internals_doc */

/**
   @brief Get status from the application signal
   @param param - buffer reference
   @return status zb_ret_t
  */
#define ZB_GET_APP_SIGNAL_STATUS(param) zb_buf_get_status(param)

/** @} */
/*! \addtogroup zdo_base */
/*! @{ */


/**
   @brief Obtains last known LQI and RSSI values from device with specified short address

   @param short_address - address of device
   @param lqi [in] - pointer to @ref zb_uint8_t variable to store lqi value
   @param rssi [in] - pointer to @ref zb_uint8_t variable to store rssi value

@b Example:
@code
  zb_uint8_t lqi, rssi;

  zb_zdo_get_diag_data(0x0000, &lqi, &rssi);
@endcode
 */
void zb_zdo_get_diag_data(zb_uint16_t short_address, zb_uint8_t *lqi, zb_int8_t *rssi);

/*! @} */


/*! \addtogroup zdo_addr */
/*! @{ */



#define ZB_ZDO_SINGLE_DEVICE_RESP   0 /*!<  Request Type parameter value of @ref zb_zdo_nwk_addr_req_t and @ref zb_zdo_ieee_addr_req_t for receiving a Single device response used in the following commands accordingly:
                                       * - @ref zb_zdo_nwk_addr_req
                                       * - @ref zb_zdo_ieee_addr_req
                                       *   @see spec 2.4.3.1, 2.4.4.1
                                       */
#define ZB_ZDO_EXTENDED_DEVICE_RESP 1 /*!< Request Type parameter value of @ref zb_zdo_nwk_addr_req_t and @ref zb_zdo_ieee_addr_req_t for receiving an Extended device response used in the following commands accordingly:
                                       * - @ref zb_zdo_nwk_addr_req
                                       * - @ref zb_zdo_ieee_addr_req_t
                                       *   @see spec 2.4.3.1, 2.4.4.1
                                       */

/** @brief NWK_addr_req command primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_nwk_addr_req_s
{
  zb_ieee_addr_t   ieee_addr;    /*!< IEEE address to be matched by the
                                   Remote Device  */
  zb_uint8_t       request_type; /*!< Request type for this command:
                                   0x00  Single device response
                                   0x01  Extended response  */
  zb_uint8_t       start_index; /*!< If the Request type for this command is
                                  Extended response, the StartIndex
                                  provides the starting index for the
                                  requested elements of the associated
                                  devices list  */
} ZB_PACKED_STRUCT zb_zdo_nwk_addr_req_t;

/** @brief Parameters for nwk_addr_req command */
typedef struct zb_zdo_nwk_addr_req_param_s
{
  zb_uint16_t      dst_addr;     /*!< Destination address */
  zb_ieee_addr_t   ieee_addr;    /*!< IEEE address to be matched by the
                                   Remote Device  */
  zb_uint8_t       request_type; /*!< Request type for this command:
                                   0x00  Single device response
                                   0x01  Extended response  */
  zb_uint8_t       start_index; /*!< If the Request type for this command is
                                  Extended response, the StartIndex
                                  provides the starting index for the
                                  requested elements of the associated
                                  devices list  */
}
zb_zdo_nwk_addr_req_param_t;

/** @brief NWK_addr_req response frame. */
typedef ZB_PACKED_PRE struct zb_zdo_nwk_addr_resp_head_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t status; /*!< The status of the NWK_addr_req command. */
  zb_ieee_addr_t ieee_addr; /*!< 64-bit address for the Remote Device. */
  zb_uint16_t nwk_addr; /*!< 16-bit address for the Remote Device. */
}
ZB_PACKED_STRUCT
zb_zdo_nwk_addr_resp_head_t;

/** @brief NWK_addr_req response frame tail */
typedef ZB_PACKED_PRE struct zb_zdo_nwk_addr_resp_ext_s
{
  zb_uint8_t num_assoc_dev; /*!< Count of the ED List. */
}
ZB_PACKED_STRUCT
zb_zdo_nwk_addr_resp_ext_t;

/** @brief NWK_addr_req response frame tail */
typedef ZB_PACKED_PRE struct zb_zdo_nwk_addr_resp_ext2_s
{
  zb_uint8_t start_index; /*!< Starting index to begin reporting for the ED List.*/
}
ZB_PACKED_STRUCT
zb_zdo_nwk_addr_resp_ext2_t;

/** @brief Sends NWK_addr_req primitive.

   @param param - index of buffer with primitive parameters - \ref zb_zdo_nwk_addr_req_param_s
   @param cb    - user's function to call when got response from the
                  remote. \ref zb_zdo_nwk_addr_resp_head_s passed to cb as parameter.
   @return        - ZDP transaction sequence number or 0xFF if operation cannot be
                  performed now (nor enough memory, resources, etc.)

@b Example:
@code
{
  zb_bufid_t buf = param;
  zb_uint8_t tsn;
  zb_zdo_nwk_addr_req_param_t *req_param = ZB_BUF_GET_PARAM(buf, zb_zdo_nwk_addr_req_param_t);

  req_param->dst_addr = 0;
  zb_address_ieee_by_ref(req_param->ieee_addr, short_addr);
  req_param->request_type = ZB_ZDO_SINGLE_DEVICE_RESP;
  req_param->start_index = 0;
  tsn = zb_zdo_nwk_addr_req(param, zb_get_peer_short_addr_cb);

  if (tsn == ZB_ZDO_INVALID_TSN)
  {
    TRACE_MSG(TRACE_ZDO2, "request failed", (FMT__0));
  }
}

void zb_get_peer_short_addr_cb(zb_uint8_t param)
{
  zb_bufid_t buf = param;
  zb_zdo_nwk_addr_resp_head_t *resp;
  zb_ieee_addr_t ieee_addr;
  zb_uint16_t nwk_addr;
  zb_address_ieee_ref_t addr_ref;

  TRACE_MSG(TRACE_ZDO2, "zb_get_peer_short_addr_cb param %hd", (FMT__H, param));

  resp = (zb_zdo_nwk_addr_resp_head_t*)zb_buf_begin(buf);
  TRACE_MSG(TRACE_ZDO2, "resp status %hd, nwk addr %d", (FMT__H_D, resp->status, resp->nwk_addr));
  ZB_DUMP_IEEE_ADDR(resp->ieee_addr);

  // additionally check tsn if needed
  //if (resp->tsn == my_saved_tsn)

  if (resp->status == ZB_ZDP_STATUS_SUCCESS)
  {
    ZB_LETOH64(ieee_addr, resp->ieee_addr);
    ZB_LETOH16(&nwk_addr, &resp->nwk_addr);
    zb_address_update(ieee_addr, nwk_addr, ZB_TRUE, &addr_ref);
  }
  zb_buf_free(buf);
}
@endcode

See tp_zdo_bv-31 sample
*/
zb_uint8_t zb_zdo_nwk_addr_req(zb_uint8_t param, zb_callback_t cb);

/** @cond internals_doc */
/**
   @brief Broadcast NWK_addr_req primitive.
   Used internally in stack.

   @param param - index of buffer with primitive parameters - \ref zb_zdo_nwk_addr_req_param_s
   @param ieee_addr - IEEE address to be matched by the Remote Device
*/
zb_uint8_t zb_zdo_initiate_nwk_addr_req(zb_uint8_t param, zb_ieee_addr_t ieee_addr);
/* Used internally in stack. */
void zb_zdo_initiate_nwk_addr_req_2param(zb_uint8_t param, zb_uint16_t user_param);
void zb_zdo_initiate_node_desc_req_2param(zb_uint8_t param, zb_uint16_t user_param);
void zb_zdo_initiate_node_desc_req_direct(zb_uint16_t addr, zb_callback_t user_cb);
/** @endcond */ /* internals_doc */

/** @brief Parameters of IEEE_addr_req primitive.

   To be put into buffer as data (means - after space alloc).
 */
typedef ZB_PACKED_PRE struct zb_zdo_ieee_addr_req_s
{
  zb_uint16_t      nwk_addr;    /*!< NWK address that is used for IEEE
                                  address mapping.  */
  zb_uint8_t       request_type; /*!< Request type for this command:
                                   0x00  Single device response
                                   0x01  Extended response  */
  zb_uint8_t       start_index; /*!< If the Request type for this command is
                                  Extended response, the StartIndex
                                  provides the starting index for the
                                  requested elements of the associated
                                  devices list  */
} ZB_PACKED_STRUCT zb_zdo_ieee_addr_req_t;

/** @brief Parameters for ieee_addr_req command */
typedef struct zb_zdo_ieee_addr_req_param_s
{
  zb_uint16_t      dst_addr;     /*!< Destination address */
  zb_uint16_t      nwk_addr;    /*!< NWK address that is used for IEEE
                                  address mapping.  */
  zb_uint8_t       request_type; /*!< Request type for this command:
                                   0x00  Single device response
                                   0x01  Extended response  */
  zb_uint8_t       start_index; /*!< If the Request type for this command is
                                  Extended response, the StartIndex
                                  provides the starting index for the
                                  requested elements of the associated
                                  devices list  */
}
zb_zdo_ieee_addr_req_param_t;

/** brief ZDO IEEE address response frame */
typedef ZB_PACKED_PRE struct zb_zdo_ieee_addr_resp_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t status; /*!< The status of the NWK_addr_req command. */
  zb_ieee_addr_t ieee_addr_remote_dev; /*!< 64-bit address for the Remote Device. */
  zb_uint16_t nwk_addr_remote_dev; /*!< 16-bit address for the Remote Device. */
}
ZB_PACKED_STRUCT
zb_zdo_ieee_addr_resp_t;

/** @brief ZDO IEEE address response frame tail */
typedef ZB_PACKED_PRE struct zb_zdo_ieee_addr_resp_ext_s
{
  zb_uint8_t num_assoc_dev; /*!< Count of the ED List. */
}
ZB_PACKED_STRUCT
zb_zdo_ieee_addr_resp_ext_t;

/** @brief ZDO IEEE address response frame tail */
typedef ZB_PACKED_PRE struct zb_zdo_ieee_addr_resp_ext2_s
{
  zb_uint8_t start_index; /*!< Starting index to begin reporting for the ED List.*/
}
ZB_PACKED_STRUCT
zb_zdo_ieee_addr_resp_ext2_t;

/** @brief IEEE_addr_req primitive.
  *
  * @param param - index of buffer with primitive parameters \ref zb_zdo_ieee_addr_req_s.
  *                Parameters must be put into the buffer as data (allocated).
  * @param cb    - user's function to call when got response from the remote.
  * @return      - ZDP transaction sequence number or 0xFF if operation cannot be
  *                performed now (nor enough memory, resources, etc.)
  *
  * @b Example:
  * @code
  * {
  *   zb_zdo_ieee_addr_req_param_t *req = NULL;
  *   zb_uint8_t tsn;
  *
  *   req = ZB_BUF_GET_PARAM(buf, zb_zdo_ieee_addr_req_param_t);
  *
  *   req->nwk_addr = ind->src_addr;
  *   req->dst_addr = req->nwk_addr;
  *   req->request_type = ZB_ZDO_SINGLE_DEV_RESPONSE;
  *   req->start_index = 0;
  *   tsn = zb_zdo_ieee_addr_req(buf, ieee_addr_callback);
  *
  *   if (tsn == ZB_ZDO_INVALID_TSN)
  *   {
  *     TRACE_MSG(TRACE_ZDO2, "request failed", (FMT__0));
  *   }
  * }
  *
  * void ieee_addr_callback(zb_uint8_t param)
  * {
  *   zb_bufid_t buf = param;
  *   zb_zdo_nwk_addr_resp_head_t *resp;
  *   zb_ieee_addr_t ieee_addr;
  *   zb_uint16_t nwk_addr;
  *   zb_address_ieee_ref_t addr_ref;
  *
  *   TRACE_MSG(TRACE_ZDO2, "zb_get_peer_short_addr_cb param %hd", (FMT__H, param));
  *
  *   resp = (zb_zdo_nwk_addr_resp_head_t*)zb_buf_begin(buf);
  *   TRACE_MSG(
  *       TRACE_ZDO2, "resp status %hd, nwk addr %d", (FMT__H_D, resp->status, resp->nwk_addr));
  *   ZB_DUMP_IEEE_ADDR(resp->ieee_addr);
  *   if (resp->status == ZB_ZDP_STATUS_SUCCESS)
  *   {
  *     ZB_LETOH64(ieee_addr, resp->ieee_addr);
  *     ZB_LETOH16(&nwk_addr, &resp->nwk_addr);
  *     zb_address_update(ieee_addr, nwk_addr, ZB_TRUE, &addr_ref);
  *   }
      zb_buf_free(buf);
  * }
  *
  * @endcode
  *
  * See tp_zdo_bv-31 sample
  */
zb_uint8_t zb_zdo_ieee_addr_req(zb_uint8_t param, zb_callback_t cb);

/** @cond internals_doc */
/* Used internally in stack. */
zb_uint8_t zb_zdo_initiate_ieee_addr_req(zb_uint8_t param, zb_uint16_t nwk_addr);
/** @endcond */ /* internals_doc */

/** @} */

/**
   @addtogroup zdo_disc
   @{
*/

/** @brief Parameters of Node_desc_req primitive.

   To be put into buffer as data (means - after space alloc).
 */
typedef ZB_PACKED_PRE struct zb_zdo_node_desc_req_s
{
  zb_uint16_t      nwk_addr;    /*!< NWK address that is used for IEEE
                                  address mapping.  */
} ZB_PACKED_STRUCT zb_zdo_node_desc_req_t;


/** @brief Header of Node_desc_resp primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_desc_resp_hdr_s
{
  zb_uint8_t      tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t      status;   /*!< The status of the Desc_req command. @ref zb_zdp_status_t */
  zb_uint16_t     nwk_addr; /*!< NWK address for the request  */
} ZB_PACKED_STRUCT
zb_zdo_desc_resp_hdr_t;

/** @brief Parameters of Node_desc_resp primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_node_desc_resp_s
{
  zb_zdo_desc_resp_hdr_t hdr;  /*!< header for response */
  zb_af_node_desc_t node_desc; /*!< Node Descriptor */
} ZB_PACKED_STRUCT
zb_zdo_node_desc_resp_t;

/** @brief Header of simple_desc_resp primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_simple_desc_resp_hdr_s
{
  zb_uint8_t      tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t      status;   /*!< The status of the Desc_req command. @ref zb_zdp_status_t */
  zb_uint16_t     nwk_addr; /*!< NWK address for the request  */
  zb_uint8_t      length;   /*!< Length of the simple descriptor */
} ZB_PACKED_STRUCT
zb_zdo_simple_desc_resp_hdr_t;

#ifdef ZB_FIXED_OPTIONAL_DESC_RESPONSES
typedef zb_zdo_simple_desc_resp_hdr_t     zb_zdo_complex_desc_resp_hdr_t;
typedef zb_zdo_simple_desc_resp_hdr_t     zb_zdo_user_desc_resp_hdr_t;
typedef zb_zdo_desc_resp_hdr_t            zb_zdo_user_desc_conf_hdr_t;
#endif

/** @brief Parameters of simple_desc_resp primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_simple_desc_resp_s
{
  zb_zdo_simple_desc_resp_hdr_t hdr;  /*!< header for response */
  zb_af_simple_desc_1_1_t simple_desc; /*!< Simple Descriptor */
} ZB_PACKED_STRUCT
zb_zdo_simple_desc_resp_t;


typedef ZB_PACKED_PRE struct zb_zdo_child_info_s
{
  zb_ieee_addr_t  ext_addr;     /*!< 64-bit IEEE address that is
                                 * unique to every device.*/
}ZB_PACKED_STRUCT
zb_zdo_child_info_t;

/** @brief Jitter used for sending Parent Annce */
#define ZB_PARENT_ANNCE_JITTER() (ZB_APS_PARENT_ANNOUNCE_BASE_TIMER + ZB_RANDOM_JTR(ZB_APS_PARENT_ANNOUNCE_JITTER_MAX))


/** @brief Header of parent_annce primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_parent_annce_s
{
  zb_uint8_t      tsn;               /*!< ZDO sequence number */
  zb_uint8_t      num_of_children;   /*!< Length of the simple descriptor */
} ZB_PACKED_STRUCT
zb_zdo_parent_annce_t;


/** @brief Header of parent_annce_rsp primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_parent_annce_rsp_hdr_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t      status;   /*!< The status of the Desc_req command. @ref zb_zdp_status_t */
  zb_uint8_t      num_of_children;   /*!< Length of the simple descriptor */
} ZB_PACKED_STRUCT
zb_zdo_parent_annce_rsp_hdr_t;

/** @brief Parameters of parent_annce_rsp primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_parent_annce_rsp_s
{
  zb_zdo_parent_annce_rsp_hdr_t hdr;  /*!< header for response */
} ZB_PACKED_STRUCT
zb_zdo_parent_annce_rsp_t;

/** @brief Parameters of Power_desc_resp primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_power_desc_resp_s
{
  zb_zdo_desc_resp_hdr_t hdr;  /*!< header for response */
  zb_af_node_power_desc_t power_desc; /*!< Power Descriptor */
} ZB_PACKED_STRUCT
zb_zdo_power_desc_resp_t;

/** @brief Node_desc_req primitive.
  *
  * @param param - index of buffer with primitive parameters \ref zb_zdo_node_desc_req_s.
  * Parameters must be put into buffer as data (allocated).
  * @param cb    - user's function to call when got response from the remote.
  * @return        - ZDP transaction sequence number or 0xFF if operation cannot be
  *                performed now (nor enough memory, resources, etc.)
  *
  * @b Example:
  * @snippet onoff_server/on_off_switch_zed.c send_node_desc_req
  * @snippet onoff_server/on_off_switch_zed.c node_req_cb
  *
  * See application/onoff_server sample
  */
zb_uint8_t zb_zdo_node_desc_req(zb_uint8_t param, zb_callback_t cb);


/** @brief Parameters of Power_desc_req primitive.

   To be put into buffer as data (means - after space alloc).
 */
typedef ZB_PACKED_PRE struct zb_zdo_power_desc_req_s
{
  zb_uint16_t      nwk_addr;    /*!< NWK address that is used for IEEE
                                  address mapping.  */
} ZB_PACKED_STRUCT zb_zdo_power_desc_req_t;

/** @brief Power_desc_req primitive.
  *
  * @param param - index of buffer with primitive parameters \ref zb_zdo_power_desc_req_s.
  * Parameters must be put into buffer as data (allocated).
  * @param cb    - user's function to call when got response from the remote.
  * @return        - ZDP transaction sequence number or 0xFF if operation cannot be
  *                performed now (nor enough memory, resources, etc.)
  *
  * @b Example:
  * @snippet onoff_server/on_off_switch_zed.c send_power_desc_req
  * @snippet onoff_server/on_off_switch_zed.c power_desc_cb
  *
  * See application/onoff_server sample
  */
zb_uint8_t zb_zdo_power_desc_req(zb_uint8_t param, zb_callback_t cb);



/** @brief Parameters of Simple_desc_req primitive.

   To be put into buffer as data (means - after space alloc).
 */
typedef ZB_PACKED_PRE struct zb_zdo_simple_desc_req_s
{
  zb_uint16_t      nwk_addr;    /*!< NWK address that is used for IEEE
                                  address mapping.  */
  zb_uint8_t       endpoint;    /*!< The endpoint on the destination  */
} ZB_PACKED_STRUCT zb_zdo_simple_desc_req_t;

/** @brief Simple_desc_req primitive.
  *
  * @param param - index of buffer with primitive parameters \ref zb_zdo_simple_desc_req_s.
  * @param cb    - user's function to call when got response from the remote.
  * @return        - ZDP transaction sequence number or 0xFF if operation cannot be
  *                performed now (nor enough memory, resources, etc.)
  *
  * @b Example:
  * @snippet onoff_server/on_off_switch_zed.c send_simple_desc_req
  * @snippet onoff_server/on_off_switch_zed.c simple_desc_cb
  *
  * See application/onoff_server sample
  */
zb_uint8_t zb_zdo_simple_desc_req(zb_uint8_t param, zb_callback_t cb);


/** @brief Parameters of Active_desc_req primitive.

   To be put into buffer as data (means - after space alloc).
 */
typedef ZB_PACKED_PRE struct zb_zdo_active_ep_req_s
{
  zb_uint16_t      nwk_addr;    /*!< NWK address that is used for IEEE
                                  address mapping.  */
} ZB_PACKED_STRUCT zb_zdo_active_ep_req_t;


/** @brief Active EP response */
typedef ZB_PACKED_PRE struct zb_zdo_ep_resp_s
{
  zb_uint8_t tsn;       /*!< ZDP transaction sequence number */
  zb_uint8_t status;    /*!< The status of the Active_EP_req command. */
  zb_uint16_t nwk_addr; /*!< NWK address for the request. */
  zb_uint8_t ep_count;  /*!< The count of active endpoints on the Remote Device. */
}
ZB_PACKED_STRUCT
zb_zdo_ep_resp_t;

/**
 * @brief Active_desc_req primitive.
 *
 * @param param - index of buffer with primitive parameters \ref zb_zdo_active_ep_req_s. Parameters must be
 * put into buffer as data (allocated).
 * @param cb    - user's function to call when got response from the remote.
 * @return        - ZDP transaction sequence number or 0xFF if operation cannot be
 *                performed now (nor enough memory, resources, etc.)
 *
 * @b Example:
 * @snippet onoff_server/on_off_switch_zed.c send_active_ep_req
 * @snippet onoff_server/on_off_switch_zed.c active_ep_cb
 *
 * See application/onoff_server sample
 */
zb_uint8_t zb_zdo_active_ep_req(zb_uint8_t param, zb_callback_t cb);

/** @brief Parameters of match_desc_req primitive.

   To be put into buffer as data (means - after space alloc).
 */
typedef ZB_PACKED_PRE struct zb_zdo_match_desc_param_s
{
  zb_uint16_t      nwk_addr;    /*!< NWK address that is used for IEEE
                                  address mapping.  */
  zb_uint16_t     addr_of_interest; /*!< NWK address of interest */

  zb_uint16_t      profile_id;  /*!< Profile ID to be matched at the
                                  destination.  */
  zb_uint8_t       num_in_clusters; /*!< The number of Input Clusters
                                      provided for matching within the
                                      InClusterList.  */
  zb_uint8_t       num_out_clusters; /*!< The number of Output Clusters
                                       provided for matching within
                                       OutClusterList.  */
  zb_uint16_t      cluster_list[1]; /*!< Variable size: [num_in_clusters] +  [num_out_clusters]
                                         List of Input ClusterIDs to be used
                                         for matching; the InClusterList is
                                         the desired list to be matched by
                                         the Remote Device (the elements
                                         of the InClusterList are the
                                         supported output clusters of the
                                         Local Device).
                                         List of Output ClusterIDs to be
                                         used for matching; the
                                         OutClusterList is the desired list to
                                         be matched by the Remote Device
                                         (the elements of the
                                         OutClusterList are the supported
                                         input clusters of the Local
                                         Device). */
}
ZB_PACKED_STRUCT
zb_zdo_match_desc_param_t;

/** @brief Match_desc_req head */
typedef ZB_PACKED_PRE struct zb_zdo_match_desc_req_head_s
{
  zb_uint16_t      nwk_addr;        /*!< NWK address that is used for IEEE
                                         address mapping.  */
  zb_uint16_t      profile_id;      /*!< Profile ID to be matched at the
                                         destination.  */
  zb_uint8_t       num_in_clusters; /*!< The number of Input Clusters
                                         provided for matching within the
                                         InClusterList.  */
}
ZB_PACKED_STRUCT
zb_zdo_match_desc_req_head_t;

/** @brief Match_desc_req tail */
typedef ZB_PACKED_PRE struct zb_zdo_match_desc_req_tail_s
{
  zb_uint8_t       num_out_clusters; /*!< The number of Output Clusters
                                       provided for matching within
                                       OutClusterList.  */
}
ZB_PACKED_STRUCT
zb_zdo_match_desc_req_tail_t;

/** @brief Match_Desc_rsp response structure
  * @see ZB spec, subclause 2.4.4.1.7
  */
typedef ZB_PACKED_PRE struct zb_zdo_match_desc_resp_s
{
  zb_uint8_t tsn;       /*!< ZDP transaction sequence number */
  zb_uint8_t status;    /*!< The status of the Match_Desc_req command.*/
  zb_uint16_t nwk_addr; /*!< NWK address for the request. */
  zb_uint8_t match_len; /*!< The count of endpoints on the Remote Device that match the
                          request criteria.*/
}
ZB_PACKED_STRUCT
zb_zdo_match_desc_resp_t;


/** @brief Match_desc_req primitive.

   @param param - index of buffer with primitive parameters \ref zb_zdo_match_desc_param_s.
   @param cb    - user's function to call when got response from the remote. If command is \n
                  broadcast, then user's function will be called as many times as number of \n
                  responses received plus one more time with status \ref ZB_ZDP_STATUS_TIMEOUT
                  to indicate that no more responses will be received.
   @return        - ZDP transaction sequence number or 0xFF if operation cannot be
                  performed now (nor enough memory, resources, etc.)

   @b Example:
   @snippet light_sample/light_control/light_control.c zdo_match_desc_req

   See light_sample
*/
zb_uint8_t zb_zdo_match_desc_req(zb_uint8_t param, zb_callback_t cb);


#ifndef ZB_LITE_NO_ZDO_SYSTEM_SERVER_DISCOVERY
/** @brief Request parameters for System_Server_Discovery_req.
  * @see ZB spec, subclause 2.4.3.1.13.
  */
typedef ZB_PACKED_PRE struct zb_zdo_system_server_discovery_req_s
{
  zb_uint16_t server_mask; /*!< Server mask for device discovery */
}
ZB_PACKED_STRUCT
zb_zdo_system_server_discovery_req_t;

/** @brief Parameters for System_Server_Discovery_req call.
  * @see ZB spec, subclause 2.4.3.1.13.
  */
typedef zb_zdo_system_server_discovery_req_t zb_zdo_system_server_discovery_param_t;


/** @brief Response parameters for System_Server_Discovery_rsp.
  * @see ZB spec, subclause 2.4.4.1.10.
  */
typedef ZB_PACKED_PRE struct zb_zdo_system_server_discovery_resp_s
{
  zb_uint8_t tsn;          /*!< ZDP transaction sequence number */
  zb_uint8_t status;       /*!< Status of the operation */
  zb_uint16_t server_mask; /*!< Mask of the supported features */
}
ZB_PACKED_STRUCT
zb_zdo_system_server_discovery_resp_t;


/**
 * @brief Performs System_Server_Discovery_req
 * @param param - index of buffer with request parameters
 * @ref zb_zdo_system_server_discovery_param_t
 * @param cb - user's function to call when got response from the remote. \ref
 *                zb_zdo_system_server_discovery_resp_s
 *
 * @return ZDP transaction sequence number
 * @return 0xFF if operation cannot be
 *          performed now (nor enough memory, resources, etc.)
 *
 *  @b Example:
 *  @snippet onoff_server/on_off_switch_zed.c system_server_discovery_req
 *  @snippet onoff_server/on_off_switch_zed.c system_server_discovery_cb
 *
 *  See application/onoff_server sample
 */
zb_uint8_t zb_zdo_system_server_discovery_req(zb_uint8_t param, zb_callback_t cb);
#endif  /*ZB_LITE_NO_ZDO_SYSTEM_SERVER_DISCOVERY */

/*! @} */

/*! \addtogroup zdo_mgmt */
/*! @{ */


/** @brief Header of parameters for Mgmt_NWK_Update_req */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_nwk_update_req_hdr_s
{
  zb_uint32_t scan_channels;   /*!< Channels bitmask */
  zb_uint8_t scan_duration;    /*!< A value used to calculate the
                               * length of time to spend scanning
                               * each channel. */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_nwk_update_req_hdr_t;

/** @brief Parameters for Mgmt_NWK_Update_req */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_nwk_update_req_s
{
  zb_zdo_mgmt_nwk_update_req_hdr_t hdr; /*!< Request header */
  zb_uint8_t scan_count;       /*!< This field represents the number
                                * of energy scans to be conducted and reported */
  zb_uint8_t update_id;     /*!< This value is set by the Network
                               * Channel Manager prior to sending
                               * the message. This field shall only
                               * be present of the ScanDuration is 0xfe or 0xff */
  zb_uint16_t manager_addr; /*!< This field shall be present only
                               * if the ScanDuration is set to 0xff,
                               * and, where present, indicates the
                               * NWK address for the device with the
                               * Network Manager bit set in its Node Descriptor. */
  zb_uint16_t dst_addr;     /*!< Destination address */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_nwk_update_req_t;

#ifdef ZB_MGMT_NWK_ENHANCED_UPDATE_ENABLED

/** @brief Parameters for Mgmt_NWK_Update_req internal call*/
typedef struct zb_zdo_mgmt_nwk_enhanced_update_req_param_s
{
  zb_channel_list_t channel_list; /*!< ZBOSS internal channel list */
  zb_uint8_t scan_duration;       /*!< A value used to calculate the
                                   * length of time to spend scanning
                                   * each channel. */
  zb_uint8_t scan_count;          /*!< This field represents the number
                                   * of energy scans to be conducted and reported */
  zb_uint8_t update_id;           /*!< This value is set by the Network
                                   * Channel Manager prior to sending
                                   * the message. This field shall only
                                   * be present of the ScanDuration is 0xfe or 0xff */
  zb_uint16_t manager_addr;       /*!< This field shall be present only
                                   * if the ScanDuration is set to 0xff,
                                   * and, where present, indicates the
                                   * NWK address for the device with the
                                   * Network Manager bit set in its Node Descriptor. */
  zb_uint16_t dst_addr;     /*!< Destination address */
} zb_zdo_mgmt_nwk_enhanced_update_req_param_t;

/** @brief Header of parameters for Mgmt_NWK_Update_req */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_nwk_enhanced_update_req_hdr_s
{
  zb_uint8_t channel_page_count;     /* The number of Channel Page Structures
                                      * contained within the Channel List Structure */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_nwk_enhanced_update_req_hdr_t;

#endif /* ZB_MGMT_NWK_ENHANCED_UPDATE_ENABLED */

/** @brief Header parameters for mgmt_nwk_update_notify */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_nwk_update_notify_hdr_s
{
  zb_uint8_t tsn;        /*!< tsn value */
  zb_uint8_t status;     /*!< The status of the Mgmt_NWK_Update_notify command. */
  zb_uint32_t scanned_channels;  /*!< List of channels scanned by the request */
  zb_uint16_t total_transmissions;  /*!< Count of the total transmissions reported by the device */
  zb_uint16_t transmission_failures;  /*!< Sum of the total transmission failures reported by the
                                            device */
  zb_uint8_t scanned_channels_list_count;  /*!< The list shall contain the number of records
                                            * contained in the EnergyValues parameter. */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_nwk_update_notify_hdr_t;

/** @brief Parameters for mgmt_nwk_update_notify */
typedef struct zb_zdo_mgmt_nwk_update_notify_param_s
{
  zb_zdo_mgmt_nwk_update_notify_hdr_t hdr;             /*!< Fixed parameters set */
  zb_uint8_t energy_values[ZB_MAC_SUPPORTED_CHANNELS]; /*!< ed scan values */
  zb_uint16_t dst_addr;  /*!< destination address */
  zb_uint8_t enhanced;   /*!< If set to one, send Enhanced notify command  */
}
zb_zdo_mgmt_nwk_update_notify_param_t;

#ifdef ZB_MGMT_NWK_ENHANCED_UPDATE_ENABLED

typedef zb_zdo_mgmt_nwk_update_notify_param_t zb_zdo_mgmt_nwk_enhanced_update_notify_param_t;

#endif /* ZB_MGMT_NWK_ENHANCED_UPDATE_ENABLED */

/** @brief Performs Mgmt_NWK_Update_req request

   @param param - index of buffer with call parameters. Parameters must be
   put into buffer as parameters. \ref zb_zdo_mgmt_nwk_update_req_s
   @param cb    - user's function to call when got response from the remote.
   \ref zb_zdo_mgmt_nwk_update_notify_hdr_s
   @return        - ZDP transaction sequence number or 0xFF if operation cannot be
                  performed now (nor enough memory, resources, etc.)

   @b Example:
@code
{
  zb_uint8_t tsn;
  zb_zdo_mgmt_nwk_update_req_t *req;

  req = ZB_BUF_GET_PARAM(buf, zb_zdo_mgmt_nwk_update_req_t);

  req->hdr.scan_channels = ZB_MAC_ALL_CHANNELS_MASK;
  req->hdr.scan_duration = TEST_SCAN_DURATION;
  req->scan_count = TEST_SCAN_COUNT;
  req->update_id = ZB_NIB_UPDATE_ID();

  req->dst_addr = 0;

  tsn = zb_zdo_mgmt_nwk_update_req(param, mgmt_nwk_update_ok_cb);
}


void mgmt_nwk_update_ok_cb(zb_uint8_t param)
{
  zb_bufid_t buf = param;
  zb_uint8_t *zdp_cmd = zb_buf_begin(buf);
  zb_zdo_mgmt_nwk_update_notify_hdr_t *notify_resp = (zb_zdo_mgmt_nwk_update_notify_hdr_t *)zdp_cmd;

  TRACE_MSG(TRACE_APS3,
            "notify_resp status %hd, scanned_channels %x %x, total_transmissions %hd, "
            "transmission_failures %hd, scanned_channels_list_count %hd, buf len %hd",
            (FMT__H_D_D_H_H_H_H, notify_resp->status, (zb_uint16_t)notify_resp->scanned_channels,
             *((zb_uint16_t*)&notify_resp->scanned_channels + 1),
             notify_resp->total_transmissions, notify_resp->transmission_failures,
             notify_resp->scanned_channels_list_count, zb_buf_len(buf)));

  if (notify_resp->status == ZB_ZDP_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_APS3, "mgmt_nwk_update_notify received, Ok", (FMT__0));
  }
  else
  {
    TRACE_MSG(TRACE_ERROR, "mgmt_nwk_update_notify received, ERROR incorrect status %x",
              (FMT__D, notify_resp->status));
  }

  zb_buf_free(buf);
}
@endcode

See TP_PRO_BV-37 sample
 */
zb_uint8_t zb_zdo_mgmt_nwk_update_req(zb_uint8_t param, zb_callback_t cb);

#ifdef ZB_MGMT_NWK_ENHANCED_UPDATE_ENABLED
/** @brief Performs Mgmt_NWK_Enhanced_Update_req request

   @param param - index of buffer with call parameters. Parameters must be
   put into buffer as parameters. \ref zb_zdo_mgmt_nwk_enhanced_update_req_param_s
   @param cb    - user's function to call when got response from the remote.
   \ref zb_zdo_mgmt_nwk_update_notify_hdr_s
   @return        - ZDP transaction sequence number or 0xFF if operation cannot be
                  performed now (nor enough memory, resources, etc.)

*/
zb_uint8_t zb_zdo_mgmt_nwk_enhanced_update_req(zb_uint8_t param, zb_callback_t cb);

/** @brief Notification for Mgmt_NWK_Unsolicited_Enhanced_Update_Notify
  * @see ZB spec, subclause 2.4.4.4.12
  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_nwk_unsol_enh_update_notify_s
{
  zb_uint8_t        status;
  zb_channel_page_t channel_in_use;
  zb_uint16_t       mac_tx_ucast_total;
  zb_uint16_t       mac_tx_ucast_failures;
  zb_uint16_t       mac_tx_ucast_retries;
  zb_uint8_t        period;
} ZB_PACKED_STRUCT
zb_zdo_mgmt_nwk_unsol_enh_update_notify_t;

/** @brief Parameters for Mgmt_NWK_Unsolicited_Enhanced_Update_Notify
  * @see ZB spec, subclause 2.4.4.4.12
  */
typedef struct zb_zdo_mgmt_nwk_unsol_enh_update_notify_param_s
{
  zb_zdo_mgmt_nwk_unsol_enh_update_notify_t notification;
  zb_uint16_t addr;
}
zb_zdo_mgmt_nwk_unsol_enh_update_notify_param_t;

/** @brief Performs Mgmt_NWK_Enhanced_Update_req request

   @param param - index of buffer with call parameters. Parameters must be
   put into buffer as parameters. \ref zb_zdo_mgmt_nwk_unsol_enh_update_notify_param_t
   @param cb    - user's function to call when the notification has been sent.
*/
void zb_zdo_mgmt_nwk_unsol_enh_update_notify(zb_uint8_t param, zb_callback_t cb);

#endif /* ZB_MGMT_NWK_ENHANCED_UPDATE_ENABLED */

/** @brief Parameters for Mgmt_Lqi_req.
  * @see ZB spec, subclause 2.4.3.3.2.
  */
typedef struct zb_zdo_mgmt_lqi_param_s
{
  zb_uint8_t start_index; /*!< Starting Index for the requested elements
                           * of the Neighbor Table */
  zb_uint16_t dst_addr;   /*!< destination address */
}
zb_zdo_mgmt_lqi_param_t;

/** @brief Request for Mgmt_Lqi_req.
  * @see ZB spec, subclause 2.4.3.3.2.
  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_lqi_req_s
{
  zb_uint8_t start_index; /*!< Starting Index for the requested elements
                           * of the Neighbor Table */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_lqi_req_t;

/** @brief Response for Mgmt_Lqi_rsp.
  * @see ZB spec, subclause 2.4.4.3.2.
  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_lqi_resp_s
{
  zb_uint8_t tsn;                        /*!< ZDP transaction sequence number */
  zb_uint8_t status;                     /*!< The status of the Mgmt_Lqi_req command.*/
  zb_uint8_t neighbor_table_entries;     /*!< Total number of Neighbor
                                          * Table entries within the Remote Device */
  zb_uint8_t start_index;                /*!< Starting index within the Neighbor
                                          * Table to begin reporting for the NeighborTableList.*/
  zb_uint8_t neighbor_table_list_count;  /*!< Number of Neighbor Table
                                          * entries included within NeighborTableList*/
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_lqi_resp_t;


/* bits 0 - 1, mask 0x3 */
/**
   Set device type of neighbor table record to type 'type': bits 0 - 1, mask 0x3;

   @param var - neighbor table record type_flags
   @param type - Zigbee device type value

   @ref zb_zdo_neighbor_table_record_s
  */
#define ZB_ZDO_RECORD_SET_DEVICE_TYPE(var, type) ( var &= ~3, var |= type )
/**
   Get device type of neighbor table record.

   @param var - neighbor table record type_flags

   @ref zb_zdo_neighbor_table_record_s
  */
#define ZB_ZDO_RECORD_GET_DEVICE_TYPE(var) ( var & 3 )

/* bits 2 - 3, mask 0xC */
/**
   Set RxOnWhenIdle attribute of neighbor table record to type 'type':
   bits 2 - 3, mask 0xC;

   @param var - neighbor table record type_flags
   @param type - RxOnWhenIdle value

   @ref zb_zdo_neighbor_table_record_s
  */
#define ZB_ZDO_RECORD_SET_RX_ON_WHEN_IDLE(var, type) ( var &= ~0xC, var |= (type << 2) )
/**
   Get RxOnWhenIdle of neighbor table record.

   @param var - neighbor table record type_flags

   @ref zb_zdo_neighbor_table_record_s
  */
#define ZB_ZDO_RECORD_GET_RX_ON_WHEN_IDLE(var) ( (var & 0xC) >> 2 )

/* bits 4 - 6, mask 0x70 */
/**
   Set relationship attribute of neighbor table record to type 'type':
   bits 4 - 6, mask 0x70;

   @param var - neighbor table record type_flags
   @param type - Zigbee relationship value

   @ref zb_zdo_neighbor_table_record_s
  */
#define ZB_ZDO_RECORD_SET_RELATIONSHIP(var, type) ( var &= ~0x70, var |= (type << 4) )
/**
   Get relationship of neighbor table record.

   @param var - neighbor table record type_flags

   @ref zb_zdo_neighbor_table_record_s
  */
#define ZB_ZDO_RECORD_GET_RELATIONSHIP(var) ( (var & 0x70) >> 4 )

/** @brief NeighborTableList Record Format for Mgmt_Lqi_resp */
typedef ZB_PACKED_PRE struct zb_zdo_neighbor_table_record_s
{
  zb_ext_pan_id_t ext_pan_id;   /*!< 64-bit extended Pan
                                 * ID of the neighboring device.*/
  zb_ieee_addr_t  ext_addr;     /*!< 64-bit IEEE address that is
                                 * unique to every device.*/
  zb_uint16_t     network_addr; /*!< The 16-bit network address of the
                                 * neighboring device */
  zb_uint8_t      type_flags;   /*!< device type, rx_on_when_idle,
                                 * relationship */
  zb_uint8_t      permit_join;  /*!< An indication of whether the
                                 * neighbor device is accepting join requests*/
  zb_uint8_t      depth;        /*!< The tree depth of the neighbor device. */
  zb_uint8_t      lqi;          /*!< The estimated link quality for RF
                                 * transmissions from this device */
}
ZB_PACKED_STRUCT
zb_zdo_neighbor_table_record_t;


/** @brief Sends  Mgmt_Lqi_req (see Zigbee spec 2.4.3.3.2)
   @param param - index of buffer with Lqi request parameters. \ref zb_zdo_mgmt_lqi_param_s
   @param cb    - user's function to call when got response from the remote.
   @return        - ZDP transaction sequence number or 0xFF if operation cannot be
                  performed now (nor enough memory, resources, etc.)
   @ref zb_zdo_mgmt_lqi_resp_s, \ref zb_zdo_neighbor_table_record_s

   @b Example:
@code
{
  zb_uint8_t tsn;
  zb_zdo_mgmt_lqi_param_t *req_param;

  req_param = ZB_BUF_GET_PARAM(buf, zb_zdo_mgmt_lqi_param_t);
  req_param->start_index = 0;
  req_param->dst_addr = 0; //coord short addr

  tsn = zb_zdo_mgmt_lqi_req(ZB_REF_FROM_BUF(buf), get_lqi_cb);
}


void get_lqi_cb(zb_uint8_t param)
{
  zb_bufid_t buf = param;
  zb_uint8_t *zdp_cmd = zb_buf_begin(buf);
  zb_zdo_mgmt_lqi_resp_t *resp = (zb_zdo_mgmt_lqi_resp_t*)(zdp_cmd);
  zb_zdo_neighbor_table_record_t *record = (zb_zdo_neighbor_table_record_t*)(resp + 1);
  zb_uint_t i;

  TRACE_MSG(TRACE_APS1, "get_lqi_cb status %hd, neighbor_table_entries %hd, start_index %hd, neighbor_table_list_count %d",
            (FMT__H_H_H_H, resp->status, resp->neighbor_table_entries, resp->start_index, resp->neighbor_table_list_count));

  for (i = 0; i < resp->neighbor_table_list_count; i++)
  {
    TRACE_MSG(TRACE_APS1, "#%hd: long addr " TRACE_FORMAT_64 " pan id " TRACE_FORMAT_64,
              (FMT__H_A_A, i, TRACE_ARG_64(record->ext_addr), TRACE_ARG_64(record->ext_pan_id)));

    TRACE_MSG(TRACE_APS1,
      "#%hd: network_addr %d, dev_type %hd, rx_on_wen_idle %hd, relationship %hd, permit_join %hd, depth %hd, lqi %hd",
      (FMT_H_D_H_H_H_H_H_H, i, record->network_addr,
       ZB_ZDO_RECORD_GET_DEVICE_TYPE(record->type_flags),
       ZB_ZDO_RECORD_GET_RX_ON_WHEN_IDLE(record->type_flags),
       ZB_ZDO_RECORD_GET_RELATIONSHIP(record->type_flags),
       record->permit_join, record->depth, record->lqi));

    record++;
  }
}

@endcode

See zdpo_lqi sample
*/
zb_uint8_t zb_zdo_mgmt_lqi_req(zb_uint8_t param, zb_callback_t cb);

/** @brief RoutingTableList Record Format for mgmt_rtg_resp */
typedef ZB_PACKED_PRE struct zb_zdo_routing_table_record_s
{
  zb_uint16_t     dest_addr;     /*!< The 16-bit network address of the
                                  * destination device */
  zb_uint8_t      flags;         /*!< Routing flags */
  zb_uint16_t     next_hop_addr; /*!< The 16-bit network address of the
                                  * next-hop device */
}
ZB_PACKED_STRUCT
zb_zdo_routing_table_record_t;

/** @} */ /* zdo_mgmt */
/** @addtogroup zdo_bind
    @{
  */

/** @brief The addressing mode for the destination address used in
  * @ref zb_zdo_binding_table_record_s, @ref zb_zdo_bind_req_param_s,
  * @ref zb_zdo_bind_req_head_s command. This field can take one of
  * the non-reserved values from the list @ref zb_bind_dst_addr_mode_e.
  * Values 0x00, 0x02, 0x04-0xff are reserved
  * @see ZB Spec, subclause 2.4.3.2.2.
  */
typedef enum zb_bind_dst_addr_mode_e
{
  ZB_BIND_DST_ADDR_MODE_16_BIT_GROUP = 0x01,    /*!< 16-bit group address
                                                     for DstAddress and DstEndp not present */
  ZB_BIND_DST_ADDR_MODE_64_BIT_EXTENDED = 0x03, /*!< 64-bit extended address
                                                     for DstAddress and DstEndp present */
} zb_bind_dst_addr_mode_t;

/** @brief Parameters for Mgmt_Bind_req.
  * @see ZB spec, subclause 2.4.3.3.4.
  */
typedef struct zb_zdo_mgmt_bind_param_s
{
  zb_uint8_t start_index; /*!< Starting Index for the requested elements
                           * of the Binding Table */
  zb_uint16_t dst_addr;   /*!< destination address */
}
zb_zdo_mgmt_bind_param_t;

/** @brief Request for Mgmt_Bind_req.
  * @see ZB spec, subclause 2.4.3.3.4.
  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_bind_req_s
{
  zb_uint8_t start_index; /*!< Starting Index for the requested elements
                           * of the Binding Table */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_bind_req_t;

/** @brief Response for Mgmt_Bind_rsp.
  * @see ZB spec, subclause 2.4.4.3.4.
  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_bind_resp_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t status; /*!< The status of the Mgmt_Rtg_req command.*/
  zb_uint8_t binding_table_entries; /*!< Total number of Binding Table
                                     * entries within the Remote Device*/
  zb_uint8_t start_index; /*!< Starting index within the Binding
                           * Table to begin reporting for the BindingTableList.*/
  zb_uint8_t binding_table_list_count; /*!< Number of Binding Table
                                         * entries included within BindingTableList*/
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_bind_resp_t;


/** @brief BindingTableList Record Format for mgmt_bind_resp. See ZB
 * spec, Table 2.131 */
typedef ZB_PACKED_PRE struct zb_zdo_binding_table_record_s
{
  zb_ieee_addr_t src_address; /*!< The source IEEE address for the binding entry. */
  zb_uint8_t src_endp;        /*!< The source endpoint for the binding entry. */
  zb_uint16_t cluster_id;     /*!< The identifier of the cluster on the
                               *   source device that is bound to the
                               *   destination device. */
  zb_uint8_t dst_addr_mode;   /*!< Destination address mode @ref zb_bind_dst_addr_mode_t */
  zb_addr_u dst_address;      /*!< The destination address for the
                               *   binding entry.16 or 64 bit. As specified by the
                               *   dst_addr_mode field.*/
  zb_uint8_t dst_endp;        /*!< This field shall be present only if the
                               *    DstAddrMode field has a value of
                               *    ZB_BIND_DST_ADDR_MODE_64_BIT_EXTENDED @ref zb_bind_dst_addr_mode_t
                               *    and, if present, shall be the
                               *    destination endpoint for the binding
                               *    entry. */
}
ZB_PACKED_STRUCT
zb_zdo_binding_table_record_t;


/** @brief Sends Mgmt_Bind_req request.
  * @param param reference to the buffer to put request data to.
  * @param cb callback to be called on operation finish.
  * @return ZDP transaction sequence number
  * @return 0xFF if operation cannot be
  *         performed now (nor enough memory, resources, etc.)
  *
  * See zdo_binding sample
  */
zb_uint8_t zb_zdo_mgmt_bind_req(zb_uint8_t param, zb_callback_t cb);

/**
   @brief Sends 2.4.4.3.4 Mgmt_Bind_rsp
   @param param - index of buffer with Mgmt_Bind request
 */
void zdo_mgmt_bind_resp(zb_uint8_t param);




/** @brief Parameters for Bind_req API call
  * @see ZB spec, subclause 2.4.3.2.2.
  */
typedef struct zb_zdo_bind_req_param_s
{
  zb_ieee_addr_t src_address;  /*!< The IEEE address for the source. */
  zb_uint8_t src_endp;         /*!< The source endpoint for the binding entry. */
  zb_uint16_t cluster_id;      /*!< The identifier of the cluster on the
                                *   source device that is bound to the destination. */
  zb_uint8_t dst_addr_mode;    /*!< Destination address mode @ref zb_bind_dst_addr_mode_t */
  zb_addr_u dst_address;       /*!< The destination address for the
                                *   binding entry. */
  zb_uint8_t dst_endp;         /*!< This field shall be present only if the
                                *   DstAddrMode field has a value of
                                *   @ref ZB_BIND_DST_ADDR_MODE_64_BIT_EXTENDED @ref zb_bind_dst_addr_mode_t
                                *   and, if present, shall be the
                                *   destination endpoint for the binding
                                *   entry. */
  zb_uint16_t req_dst_addr;    /*!< Destination address of the request */
}
zb_zdo_bind_req_param_t;


/** @brief Bind_req request head send to the remote.
  * @see ZB spec, subclause 2.4.3.2.2.
  */
typedef ZB_PACKED_PRE struct zb_zdo_bind_req_head_s
{
  zb_ieee_addr_t src_address; /*!< The IEEE address for the source. */
  zb_uint8_t src_endp;        /*!< The source endpoint for the binding entry. */
  zb_uint16_t cluster_id;     /*!< The identifier of the cluster on the
                               *   source device that is bound to the destination. */
  zb_uint8_t dst_addr_mode;   /*!< Destination address mode @ref zb_bind_dst_addr_mode_t */
}
ZB_PACKED_STRUCT
zb_zdo_bind_req_head_t;

/** @brief Bind_req request tail 1st variant send to the remote.
  * @see ZB spec, subclause 2.4.3.2.2.
  */
typedef ZB_PACKED_PRE struct zb_zdo_bind_req_tail_1_s
{
  zb_uint16_t dst_addr; /*!< The destination address for the
                         *   binding entry. */
}
ZB_PACKED_STRUCT
zb_zdo_bind_req_tail_1_t;

/** @brief Bind_req request tail 2nd variant send to the remote.
  * @see ZB spec, subclause 2.4.3.2.2.
  */
typedef ZB_PACKED_PRE struct zb_zdo_bind_req_tail_2_s
{
  zb_ieee_addr_t dst_addr; /*!< The destination address for the
                            * binding entry. */
  zb_uint8_t dst_endp;  /*!< The destination address for the
                         * binding entry. */
}
ZB_PACKED_STRUCT
zb_zdo_bind_req_tail_2_t;

/** @brief Response by Bind_req. */
typedef ZB_PACKED_PRE struct zb_zdo_bind_resp_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t status; /**< Operation status. */
}
ZB_PACKED_STRUCT
zb_zdo_bind_resp_t;


/** @brief Bind_req request.

   @param param - index of buffer with request. \ref zb_apsme_binding_req_s
   @param cb    - user's function to call when got response from the
   remote. \ref zb_zdo_bind_resp_s
   @return ZDP transaction sequence number
   @return 0xFF if operation cannot be performed now (nor enough memory, resources, etc.)

   @b Example:
@code
{
  zb_apsme_binding_req_t *req;

  req = ZB_BUF_GET_PARAM(buf, zb_apsme_binding_req_t);
  ZB_MEMCPY(&req->src_addr, &g_ieee_addr, sizeof(zb_ieee_addr_t));
  req->src_endpoint = i;
  req->clusterid = 1;
  req->addr_mode = ZB_APS_ADDR_MODE_64_ENDP_PRESENT;
  ZB_MEMCPY(&req->dst_addr.addr_long, &g_ieee_addr_d, sizeof(zb_ieee_addr_t));
  req->dst_endpoint = 240;

  zb_zdo_bind_req(ZB_REF_FROM_BUF(buf), zb_bind_callback);
  ret = buf->u.hdr.status;
  if (ret == RET_TABLE_FULL)
  {
    TRACE_MSG(TRACE_ERROR, "TABLE FULL %d", (FMT__D, ret));
  }
}

void zb_bind_callback(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_bufid_t buf = param;
  zb_uint8_t *aps_body = NULL;

  aps_body = zb_buf_begin(buf);
  ZB_MEMCPY(&ret, aps_body, sizeof(ret));

  TRACE_MSG(TRACE_INFO1, "zb_bind_callback %hd", (FMT__H, ret));
  if (ret == RET_OK)
  {
    // bind ok
  }
}
@endcode

See tp_zdo_bv-12 sample
 */
zb_uint8_t zb_zdo_bind_req(zb_uint8_t param, zb_callback_t cb);


/** @brief Unbind_req request.

   @param param - index of buffer with request. @ref zb_zdo_bind_req_param_s
   @param cb    - user's function to call when got response from the
   remote. @ref zb_zdo_bind_resp_s
   @return ZDP transaction sequence number
   @return 0xFF if operation cannot be performed now (nor enough memory, resources, etc.)

   @b Example:
@code
{
  zb_bufid_t buf = param;
  zb_zdo_bind_req_param_t *bind_param;

  TRACE_MSG(TRACE_COMMON1, "unbind_device_1", (FMT__0));

  zb_buf_initial_alloc(buf, 0);
  bind_param = ZB_BUF_GET_PARAM(buf, zb_zdo_bind_req_param_t);
  ZB_MEMCPY(bind_param->src_address, g_ieee_addr_ed1, sizeof(zb_ieee_addr_t));
  bind_param->src_endp = TEST_ED1_EP;
  bind_param->cluster_id = TP_BUFFER_TEST_REQUEST_CLID;
  bind_param->dst_addr_mode = ZB_APS_ADDR_MODE_64_ENDP_PRESENT;
  ZB_MEMCPY(bind_param->dst_address.addr_long, g_ieee_addr_ed2, sizeof(zb_ieee_addr_t));
  bind_param->dst_endp = TEST_ED2_EP;
  bind_param->req_dst_addr = zb_address_short_by_ieee(g_ieee_addr_ed1);
  TRACE_MSG(TRACE_COMMON1, "dst addr %d", (FMT__D, bind_param->req_dst_addr));

  zb_zdo_unbind_req(buf, unbind_device1_cb);
}


void unbind_device1_cb(zb_uint8_t param)
{
  zb_bufid_t buf = ZB_BUF_FROM_REF(param);
  zb_zdo_bind_resp_t *bind_resp = (zb_zdo_bind_resp_t*)zb_buf_begin(buf);

  TRACE_MSG(TRACE_COMMON1, "unbind_device1_cb resp status %hd", (FMT__H, bind_resp->status));
  if (bind_resp->status != ZB_ZDP_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_COMMON1, "Error bind device 1. Test status failed", (FMT__0));
  }
  zb_free_buf(buf);

}
@endcode

  See tp_aps_bv-23-i, tp_zdo_bv-12 samples
 */
zb_uint8_t zb_zdo_unbind_req(zb_uint8_t param, zb_callback_t cb);

/**
 * @brief Checks existence of  bind table entries with selected endpoint and cluster ID.
 *
 * @param src_end - source endpoint
 * @param cluster_id - source cluster ID
 *
 * @return ZB_TRUE if binding is found on given endpoint, ZB_FALSE otherwise
 */
zb_bool_t zb_zdo_find_bind_src(zb_uint8_t src_end, zb_uint16_t cluster_id);

/** @} */

/** @addtogroup zdo_mgmt
    @{
*/

/** @brief Request for Mgmt_Leave_req
    @see ZB spec, subclause 2.4.3.3.5.

   Problem in the specification:
   in 2.4.3.3.5  Mgmt_Leave_req only one DeviceAddress exists.
   But, in such case it is impossible to satisfy 2.4.3.3.5.1:
   "The Mgmt_Leave_req is generated from a Local Device requesting that a Remote
   Device leave the network or to request that another device leave the network."
   Also, in the PRO TC document, 14.2TP/NWK/BV-04 ZR-ZDO-APL RX Join/Leave is
   following note:
   "gZC sends Mgmt_Leave.request with DevAddr=all zero, DstAddr=ZR"
 */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_leave_param_s
{
  zb_ieee_addr_t device_address;   /*!< 64-bit IEEE address */
  zb_uint16_t    dst_addr;         /*!< Destination address. Not defined in
                                    *   the specification - let's it be short address */
  zb_bitfield_t reserved:6;        /*!< Reserve */
  zb_bitfield_t remove_children:1; /*!< Remove children */
  zb_bitfield_t rejoin:1;          /*!< Rejoin */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_leave_param_t;

/** Request for Mgmt_Leave_req.
  * @see ZB spec, subclause 2.4.3.3.5.
  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_leave_req_s
{
  zb_ieee_addr_t device_address;   /*!< 64-bit IEEE address */
  zb_bitfield_t reserved:6;        /*!< Reserve */
  zb_bitfield_t remove_children:1; /*!< Remove children */
  zb_bitfield_t rejoin:1;          /*!< Rejoin */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_leave_req_t;

/** @brief Response for Mgmt_Leave_rsp.
  * @see ZB spec, subclause 2.4.4.3.5.
  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_leave_res_s
{
  zb_uint8_t tsn;    /*!< ZDP transaction sequence number */
  zb_uint8_t status; /**< Operation status. */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_leave_res_t;

/** @brief Sends Mgmt_Leave_req
    @see ZB spec, subclause 2.4.3.3.2.

   @param param - index of buffer with Lqi request parameters. @ref zb_zdo_mgmt_leave_param_s
   @param cb    - user's function to call when got response from the remote.
   @return        - transaction sequence number of request or 0xFF if operation not be
                  performed right now (in case if not exist free slot for registering callback)

   @b Example:
@code
{
  zb_bufid_t buf = param;
  zb_zdo_mgmt_leave_param_t *req = NULL;
  zb_ret_t ret = RET_OK;
  zb_uint8_t tsn;

  TRACE_MSG(TRACE_ERROR, "zb_leave_req", (FMT__0));

  req = ZB_BUF_GET_PARAM(buf, zb_zdo_mgmt_leave_param_t);

  ZB_MEMSET(req->device_address, 0, sizeof(zb_ieee_addr_t));
  req->remove_children = ZB_FALSE;
  req->rejoin = ZB_FALSE;
  req->dst_addr = 1;
  tsn = zdo_mgmt_leave_req(param, leave_callback);
}

void leave_callback(zb_uint8_t param)
{
  zb_zdo_mgmt_leave_res_t *resp = (zb_zdo_mgmt_leave_res_t *)zb_buf_begin(param);

  TRACE_MSG(TRACE_ERROR, "LEAVE CALLBACK status %hd", (FMT__H, resp->status));
}
@endcode

See nwk_leave sample
*/
zb_uint8_t zdo_mgmt_leave_req(zb_uint8_t param, zb_callback_t cb);

/** @} */
/** @addtogroup zdo_bind
    @{
*/


/** @brief End_Device_Bind_req command head.
  * @see ZB spec, subclause 2.4.3.2.1.
  */
typedef ZB_PACKED_PRE struct zb_zdo_end_device_bind_req_head_s
{
  zb_uint16_t binding_target;   /*!< The address of the target for the
                                 * binding. This can be either the
                                 * primary binding cache device or the
                                 * short address of the local device. */
  zb_ieee_addr_t src_ieee_addr; /*!< IEEE address of the device generating the request */
  zb_uint8_t src_endp;          /*!< The endpoint on the device generating the request */
  zb_uint16_t profile_id;       /*!< ProfileID which is to be matched
                                 * between two End_Device_Bind_req
                                 * received at the Zigbee Coordinator */
  zb_uint8_t num_in_cluster;    /*!< The number of Input Clusters
                                 * provided for end device binding
                                 * within the InClusterList. */
}
ZB_PACKED_STRUCT
zb_zdo_end_device_bind_req_head_t;

/** @brief End_Device_Bind_req command head.
  * @see ZB spec, subclause 2.4.3.2.1.
  */
typedef ZB_PACKED_PRE struct zb_zdo_end_device_bind_req_tail_s
{
  zb_uint8_t num_out_cluster;   /*!< The number of Output Clusters
                                 * provided for matching within OutClusterList */
}
ZB_PACKED_STRUCT
zb_zdo_end_device_bind_req_tail_t;

/** @brief Parameters for End_Device_Bind_req.
  * @see ZB spec, subclause 2.4.3.2.1.
  */
typedef ZB_PACKED_PRE struct zb_end_device_bind_req_param_s
{
  zb_uint16_t dst_addr;                         /*!< Destination address */
  zb_zdo_end_device_bind_req_head_t head_param; /*!< Parameters for command head */
  zb_zdo_end_device_bind_req_tail_t tail_param; /*!< Parameters for command tail */
  zb_uint16_t cluster_list[1];                  /*!< List of Input and Output
                                                 * ClusterIDs to be used for matching */
} ZB_PACKED_STRUCT
zb_end_device_bind_req_param_t;

/**
   sends 2.4.3.2.1 End_Device_Bind_req command
   @param param - index of buffer with request
   @param cb    - user's function to call when got response from the remote.
   @return ZDP transaction sequence number or
   @return 0xFF if operation cannot be performed now (nor enough memory, resources, etc.)
*/
zb_uint8_t zb_end_device_bind_req(zb_uint8_t param, zb_callback_t cb);


/** @brief Response from End_Device_Bind_req.
  * @see ZB spec, subclause 2.4.3.2.1.
  */
typedef ZB_PACKED_PRE struct zb_zdo_end_device_bind_resp_s
{
  zb_uint8_t tsn;    /*!< ZDP transaction sequence number */
  zb_uint8_t status; /**< Operation status. */
}
ZB_PACKED_STRUCT
zb_zdo_end_device_bind_resp_t;

/** @} */
/** @addtogroup zdo_mgmt
    @{
*/

/** @brief Parameters for Mgmt_Permit_Joining_req.
  * @see ZB spec, subclause 2.4.3.3.7.
  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_permit_joining_req_s
{
  zb_uint8_t permit_duration;   /**< The length of time in seconds.  0x00 and 0xff indicate that
                                  * permission is disabled or enabled
                                  */
  zb_uint8_t tc_significance;   /**< If this is set to 0x01 and the remote device is the Trust
                                  * Center, the command affects the
                                  * Trust Center authentication policy as described in the
                                  * sub-clauses below; According to
                                  * r21, should be always set to 0x01.
                                  */
} ZB_PACKED_STRUCT
zb_zdo_mgmt_permit_joining_req_t;

/** @brief Parameters for zb_zdo_mgmt_permit_joining_req.  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_permit_joining_req_param_s
{
  zb_uint16_t dest_addr;        /**< Destination address */
  zb_uint8_t permit_duration;   /**< The length of time in seconds.  0x00 and 0xff indicate that
                                  * permission is disabled or enabled
                                  */
  zb_uint8_t tc_significance;   /**< If this is set to 0x01 and the remote device is the Trust
                                  * Center, the command affects the Trust Center authentication
                                  * policy as described in the sub-clauses below; If this is set to
                                  * 0x00, there is no effect on the Trust
                                  * Center.
                                  * Ignored for r21.
                                  */
} ZB_PACKED_STRUCT
zb_zdo_mgmt_permit_joining_req_param_t;

/**
   @brief sends   Mgmt_Permit_Joining_req (See Zigbee spec 2.4.3.3.7)
   @param param - Index of buffer with request
   @param cb    - user's function to call when got response from the remote.
   @return        ZDP transaction sequence number
   @return        0xFF if operation cannot be performed now (nor enough memory, resources, etc.)

   @snippet onoff_server/on_off_switch_zed.c zdo_mgmt_permit_joining_req

   See onoff_server sample
 */
zb_uint8_t zb_zdo_mgmt_permit_joining_req(zb_uint8_t param, zb_callback_t cb);

/** @} */
/** @addtogroup zdo_groups
    @{
*/


/** @brief Response from zb_zdo_mgmt_permit_joining_req.  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_permit_joining_resp_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t status; /**< Operation status. */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_permit_joining_resp_t;

/** @brief Not Supported Response */
typedef ZB_PACKED_PRE struct zb_zdo_not_supported_resp_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t status; /**< Operation status. */
}
ZB_PACKED_STRUCT
zb_zdo_not_supported_resp_t;

/**
 * @brief Parameters for Mgmt_NWK_IEEE_Joining_List_req.
 * @see ZB r22 spec, subclause 2.4.3.3.11.
 */
typedef struct zb_zdo_mgmt_nwk_ieee_joining_list_param_s
{
  zb_uint8_t start_index; /*!< Starting Index for nwkIeeeJoiningList to be returned */
  zb_uint16_t dst_addr;   /*!< destination address */
}
zb_zdo_mgmt_nwk_ieee_joining_list_param_t;

/**
 * @brief Request for Mgmt_NWK_IEEE_Joining_List_req.
 * @see ZB r22 spec, subclause 2.4.3.3.11.
 */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_nwk_ieee_joining_list_req_s
{
  zb_uint8_t start_index; /*!< Starting Index for nwkIeeeJoiningList to be returned */
}
ZB_PACKED_STRUCT zb_zdo_mgmt_nwk_ieee_joining_list_req_t;


/**
 * @brief Response for Mgmt_NWK_IEEE_Joining_List_rsp.
 * @see ZB spec, subclause 2.4.4.4.11.
 */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_nwk_ieee_joining_list_rsp_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */

  zb_uint8_t status; /*!< The status of the Mgmt_NWK_IEEE_Joining_List_req command. */

  zb_uint8_t ieee_joining_list_update_id;

  zb_uint8_t joining_policy; /*<! Joining policy enumeration. */

  zb_uint8_t ieee_joining_list_total;

  zb_uint8_t start_index; /*<! The starting index in the mibIeeeJoiningList. */

  zb_uint8_t ieee_joining_count; /*<! Number of ieee addresses following this structure. */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_nwk_ieee_joining_list_rsp_t;
//!!


/** @brief ZDO interface for ADD-GROUP.request.

   @note zb_apsme_add_group_request does not call confirm callback.

   @param param - (in/out) buffer with parameters
      - in - @ref zb_apsme_add_group_req_s
      - out - @ref zb_apsme_add_group_conf_s

   @b Example
   @snippet doxygen_snippets.dox tp_pro_bv-46_zr_certification_TP_PRO_BV-46_tp_pro_bv-46_zr_c

   See tp_pro_bv-46 sample
 */
void zb_zdo_add_group_req(zb_uint8_t param);


/** @brief ZDO interface for REMOVE-GROUP.request
  * @param param - (in/out) buffer with parameters
  * @snippet doxygen_snippets.dox tp_pro_bv-46_zed_certification_TP_PRO_BV-46_tp_pro_bv-46_zed_c
  *
  * See tp_pro_bv-46 sample
  */
void zb_zdo_remove_group_req(zb_uint8_t param);

/** @brief ZDO interface for REMOVE-ALL-GROUPS.request
  * @param param - (in/out) buffer with parameters
  *
  * @snippet doxygen_snippets.dox add_remove_all_groups_aps_group_management_aps_group_zc_c
  *
  * See aps_group_management sample
  */
void zb_zdo_remove_all_groups_req(zb_uint8_t param);

/** @brief ZDO interface for ZCL Get Group Membership Command
  * @param param - (in/out) buffer with parameters
  *
  * @snippet doxygen_snippets.dox zb_zdo_get_group_membership_req_aps_group_membership_req_aps_group_zc_c
  *
  * See aps_group_management sample
  */
void zb_zdo_get_group_membership_req(zb_uint8_t param);

/** @} */
/** @addtogroup zdo_mgmt
    @{ */

/**
   Parameters of Device_annce primitive.

   To be put into buffer as data (means - after space alloc).
 */
typedef ZB_PACKED_PRE struct zb_zdo_device_annce_s
{
  zb_uint8_t       tsn;         /*!< TSN of command */
  zb_uint16_t      nwk_addr;    /*!< NWK address for the Local Device  */
  zb_ieee_addr_t   ieee_addr;   /*!< IEEE address for the Local Device  */
  zb_uint8_t       capability;  /*!< Capability of the local device */
} ZB_PACKED_STRUCT zb_zdo_device_annce_t;


/**
   Arguments of the NLME-LEAVE.indication routine.
*/
typedef ZB_PACKED_PRE struct zb_nlme_leave_indication_s
{
  zb_ieee_addr_t device_address; /**< 64-bit IEEE address of the device to remove, zero fill if device
                                  * itself */
  zb_uint8_t rejoin;            /**< Join after leave */
} ZB_PACKED_STRUCT
zb_nlme_leave_indication_t;

/*! @} */

#ifndef ZB_COORDINATOR_ONLY

/**
  *  @addtogroup zdo_poll_control
  *  @{
  */


/**
 * @brief Toggle Turbo Poll feature.
 * @param permit ZB_TRUE to enable, ZB_FALSE to disable Turbo Poll.
*/
void zb_zdo_pim_permit_turbo_poll(zb_uint8_t permit);

/**
 * @brief Start Turbo Poll adaptive algorithm for the number of packets.
 * Turbo Poll will be disabled after receiving the specified number of packets
 * or after the poll timeout will rise up to the Long Poll interval.
 * @param n_packets Number of packets.
 */
void zb_zdo_pim_start_turbo_poll_packets(zb_uint8_t n_packets);

/**
 * @brief Start the Turbo Poll adaptive algorithm for the specified timeout.
 * @param turbo_poll_timeout_ms Timeout in milliseconds.
 */
void zb_zdo_pim_start_turbo_poll_continuous(zb_time_t turbo_poll_timeout_ms);

/**
 * @brief Stop the Turbo Poll adaptive algorithm initiated by @ref zb_zdo_pim_start_turbo_poll_continuous before the timeout expires.
 * Restores the Long Poll interval.
 * @param param Not used, added for the correct scheduling of this function. Usually specified as zero.
 */
void zb_zdo_pim_turbo_poll_continuous_leave(zb_uint8_t param);

/**
 * @brief Change the Long Poll interval.
 * @note
 *  - A preferable way to change the Long Poll interval if the Poll Control cluster is not defined.
 * Else the Poll Control cluster API should be used to set the Long Poll Interval attribute.
 *  - Long Poll interval configuration should be done after the join procedure because during join the Long Poll
 * interval will be set to the default value @ref ZB_PIM_DEFAULT_LONG_POLL_INTERVAL.
 * @param ms New Long Poll interval in milliseconds.
 */
void zb_zdo_pim_set_long_poll_interval(zb_time_t ms);

/**
 * @brief Get the Long Poll Interval.
 * @return Long Poll interval in milliseconds.
*/
zb_time_t zb_zdo_pim_get_long_poll_interval_ms(void);

#else
#define zb_zdo_pim_turbo_poll_continuous_leave(param)
#define zb_zdo_pim_permit_turbo_poll(permit)
#define zb_zdo_pim_start_turbo_poll_continuous(turbo_poll_timeout_ms)
#define zb_zdo_pim_start_turbo_poll_packets(n_packets)
#define zb_zdo_pim_set_long_poll_interval(ms)
#define zb_zdo_pim_get_long_poll_interval_ms() 0
#endif

/** @} */

#ifdef ZB_REJOIN_BACKOFF
/** @addtogroup zdo_rejoin_backoff
    @{ */

/** @brief Start rejoin backoff procedure.
 * If the method is to be directly called from application, it is expected that MAC layer and PIB cache
 * are properly initialized. For starting device call zb_zdo_dev_start_cont instead - if device is commissioned it will
 * reinit network settings and trigger rejoin from inside the stack.
 *
 * @param insecure_rejoin - specify if insecure rejoin is allowed
 * @return RET_OK on success
 * @return RET_ALREADY_EXISTS if rejoin backoff is running already
 */
zb_ret_t zb_zdo_rejoin_backoff_start(zb_bool_t insecure_rejoin);

/** @brief Run next rejoin backoff iteration */
void zb_zdo_rejoin_backoff_continue(zb_uint8_t param);

/** @brief Force rejoin backoff iteration start */
zb_bool_t zb_zdo_rejoin_backoff_force(void);

/** @brief Return rejoin backoff status: running (ZB_TRUE) or not running (ZB_FALSE) */
zb_bool_t zb_zdo_rejoin_backoff_is_running(void);

/** @brief Clear rejoin backoff context, cancel scheduled function */
void zb_zdo_rejoin_backoff_cancel(void);
/*! @} */
#endif /* ZB_REJOIN_BACKOFF */


#define ZB_NWK_LPD_NOTIFICATION  0x01
#define ZB_NWK_LPD_REQUEST       0x02

#ifndef ZB_LITE_NO_CONFIGURABLE_POWER_DELTA
zb_ret_t zb_zdo_set_lpd_cmd_mode(zb_uint8_t mode);
zb_void_t zb_zdo_set_lpd_cmd_timeout(zb_uint8_t timeout);
#else
#define zb_zdo_set_lpd_cmd_mode(mode)
#define zb_zdo_set_lpd_cmd_timeout(timeout)
#endif  /* ZB_LITE_NO_CONFIGURABLE_POWER_DELTA */

/**
   @brief Disable channel update (affected for coordinator role only)

   The current ZB channel can be changed inside the stack for internal reasons or
   as a result of processing Mgmt NWK Update Notify commands. Call this function to
   prevent such channel changes if it is not allowable for the application.

   @param disable - specify if channel update is disabled (enabled by default)
*/
void zb_zdo_disable_network_mgmt_channel_update(zb_bool_t disable);

#ifdef ZB_DISTRIBUTED_SECURITY_ON
/**
 * @addtogroup zdo_distributed_security
 * @{
 */
/**
 *  @brief Set custom distributed key
 *
 *  @param key_ptr - pointer to the key
 */
void zb_zdo_set_tc_standard_distributed_key(zb_uint8_t *key_ptr);

/**
 *  @brief Allow to setup network as distributed when started
 */
void zb_zdo_setup_network_as_distributed(void);

/**
   Enable distributed security linkage

   Without that call ZR is not able to create a Distributed network.
 */
void zb_enable_distributed(void);
/** @} */ /* zdo_distributed_security */
#endif /* ZB_DISTRIBUTED_SECURITY_ON */

/*! @addtogroup af_api */
/*! @{ */

/**
 * @addtogroup af_management_service AF management service
 * @{
 */
/**
 *  @brief Set up application callback to be called for APS data packets.
 *
 *  That API call may be useful for tests which uses custom profile id or which needs to send raw
 *  data over APS.
 *  @attention Function is called on receive of APS data frame BEFORE internal processing. Please
 *  make sure that this callback does not handle packets on standard ZB profiles - this should be
 *  done by the stack.
 *
 *  @param cb - callback to call when AF got APS packet. @ref zb_apsde_data_indication_t
 *
 *  @b Example:
 *  @code
 *  void zboss_signal_handler(zb_uint8_t param)
 *  {
 *    zb_zdo_app_signal_hdr_t *sg_p = NULL;
 *    zb_zdo_app_signal_type_t sig = zb_get_app_signal(param, &sg_p);
 *
 *    if (ZB_GET_APP_SIGNAL_STATUS(param) == 0)
 *    {
 *      switch(sig)
 *      {
 *        case ZB_ZDO_SIGNAL_DEFAULT_START:
 *          TRACE_MSG(TRACE_APS1, "Device STARTED OK", (FMT__0));
 *          zb_af_set_data_indication(data_indication);
 *          break;
 *        default:
 *          TRACE_MSG(TRACE_APP1, "Unknown signal", (FMT__0));
 *          break;
 *      }
 *      zb_buf_free(buf);
 *    }
 *  }
 *
 *  zb_uint8_t data_indication(zb_bufid_t param)
 *  {
 *    zb_ushort_t i;
 *    zb_uint8_t *ptr;
 *    zb_apsde_data_indication_t *ind = ZB_GET_BUF_PARAM(param, zb_apsde_data_indication_t);
 *
 *    if (ind->profileid == TEST_CUSTOM_PROFILE_ID)
 *    {
 *      ptr = ZB_APS_HDR_CUT(param);
 *
 *      TRACE_MSG(TRACE_APS3, "apsde_data_indication: packet %p len %hd status 0x%hx from %d",
 *                (FMT__P_D_D_D, param, zb_buf_len(param), zb_buf_get_status(param), ind->src_addr));
 *
 *      for (i = 0 ; i < zb_buf_len(param) ; ++i)
 *      {
 *        TRACE_MSG(TRACE_APS3, "%x %c", (FMT__D_C, (int)ptr[i], ptr[i]));
 *      }
 *      zb_buf_free(param);
 *      return ZB_TRUE;
 *    }
 *    return ZB_FALSE;
 *  }
 *  @endcode
 */
void zb_af_set_data_indication(zb_device_handler_t cb);

/**
 *  @brief Perform "Reset with a Local Action" procedure (as described in BDB spec, chapter 9.5).
 *  The device will perform the NLME leave and clean all Zigbee persistent data except the outgoing NWK
 *  frame counter and application datasets (if any).
 *  The reset can be performed at any time once the device is started (see @ref zboss_start).
 *  After the reset, the application will receive the @ref ZB_ZDO_SIGNAL_LEAVE signal.
 *
 *  @param param - buffer reference (if 0, buffer will be allocated automatically)
 */
void zb_bdb_reset_via_local_action(zb_uint8_t param);
/** @} */ /* af_management_service */
/*! @} */

#endif /*ZB_ZBOSS_API_ZDO_H*/
