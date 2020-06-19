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
/* PURPOSE: ZBOSS Zigbee cluster library API header
*/
#ifndef ZBOSS_API_ZCL_H
#define ZBOSS_API_ZCL_H 1

#include "zb_types.h"

#include "zcl/zb_zcl_config.h"

#include "zcl/zb_zcl_common.h"
#include "zcl/zb_zcl_commands.h"

#if !(defined ZB_ZCL_DISABLE_REPORTING)
#include "zcl/zb_zcl_reporting.h"
#endif

#include "zcl/zb_zcl_scenes.h"
#include "zcl/zb_zcl_basic.h"
#include "zcl/zb_zcl_alarms.h"
#include "zcl/zb_zcl_power_config.h"
#include "zcl/zb_zcl_identify.h"
#include "zcl/zb_zcl_on_off_switch_conf.h"
#include "zcl/zb_zcl_on_off.h"
#include "zcl/zb_zcl_groups.h"
#include "zcl/zb_zcl_door_lock.h"
#include "zcl/zb_zcl_binary_input.h"
#include "zcl/zb_zcl_level_control.h"
#include "zcl/zb_zcl_window_covering.h"
#include "zcl/zb_zcl_color_control.h"
#include "zcl/zb_zcl_shade_config.h"
#include "zcl/zb_zcl_cvc_commands.h"

#ifdef ZB_CONTROL4_NETWORK_SUPPORT
#include "zcl/zb_zcl_control4_networking.h"
#endif

#include "zcl/zb_zcl_temp_measurement.h"
#include "zcl/zb_zcl_rel_humidity_measurement.h"
#include "zcl/zb_zcl_illuminance_measurement.h"

#include "zcl/zb_zcl_ias_ace.h"
#include "zcl/zb_zcl_ias_wd.h"
#include "zcl/zb_zcl_ias_zone.h"
#include "zcl/zb_zcl_diagnostics.h"

#include "zcl/zb_zcl_custom_cluster.h"

#include "zcl/zb_zcl_time.h"
#include "zcl/zb_zcl_dehumidification_control.h"
#include "zcl/zb_zcl_el_measurement.h"
#include "zcl/zb_zcl_fan_control.h"
#include "zcl/zb_zcl_diagnostics.h"
#include "zcl/zb_zcl_en50523_appliance_events_and_alerts.h"
#include "zcl/zb_zcl_metering.h"
#include "zcl/zb_zcl_thermostat.h"
#include "zcl/zb_zcl_thermostat_ui_config.h"
#include "zcl/zb_zcl_occupancy_sensing.h"
#include "zcl/zb_zcl_meter_identification.h"
/* #ifdef ZB_ZCL_SUPPORT_CLUSTER_WWAH */
#include "zcl/zb_zcl_wwah.h"
/* #endif /\* ZB_ZCL_SUPPORT_CLUSTER_WWAH *\/ */

#include "zcl/zb_zcl_poll_control.h"
#include "zcl/zb_zcl_ota_upgrade.h"

#include "zcl/zb_zcl_price.h"
#include "zcl/zb_zcl_drlc.h"
#include "zcl/zb_zcl_metering.h"
#include "zcl/zb_zcl_messaging.h"
#if defined ZB_ZCL_SUPPORT_CLUSTER_DAILY_SCHEDULE
#include "zcl/zb_zcl_daily_schedule.h"
#endif
#include "zcl/zb_zcl_tunneling.h"

#if defined ZB_ZCL_SUPPORT_CLUSTER_PREPAYMENT
#include "zcl/zb_zcl_prepayment.h"
#endif
#ifdef ZB_ZCL_SUPPORT_CLUSTER_CALENDAR
#include "zcl/zb_zcl_calendar.h"
#endif



#if defined ZB_BDB_MODE
//#include "zb_bdb_comissioning.h"
#endif

#if defined ZB_ENABLE_ZGP_CLUSTER
//#include "zgp/zgp_internal.h"
#endif

#include "zcl/zb_zcl_grpw_bed_sensor.h"
#include "zcl/zb_zcl_grpw_freq_agility.h"

/**
 *  \addtogroup ZB_ZCL_INITIALIZATION
 *  @{
 *    @details
 *    Most of initialization is being handled automatically during device start. However,
 *    application may set additional callbacks for event handling. Particularly:

 *    @li @ref ZB_ZCL_SET_MODIFY_ATTR_VALUE_CB macro sets callback for attribute change handling.
 *    This event could arise, for example, as a result of @ref write_attr_cmd "Write attributes"
 *    command execution.
 *    @li @ref ZB_ZCL_SET_REPORT_ATTR_CB and @ref ZB_ZCL_SET_NO_REPORTING_CB macros set
 *    callbacks for handling "report attribute" and "no reporting" callbacks respectively. For more
 *    information see @ref ZB_ZCL_REPORTING "Attribute reporting" description.
 *
 *    Sample use ZB_ZCL_SET_REPORT_ATTR_CB
 *    @snippet light_sample/dimmable_light/bulb.c zb_zcl_set_report_attr_cb_example
 *    @snippet light_sample/dimmable_light/bulb.c zb_zcl_set_report_attr_cb
 *
 *    Sample use ZB_ZCL_SET_NO_REPORTING_CB
 *    @snippet doxygen_snippets.dox ZB_ZCL_SET_NO_REPORTING_CB_func_snippet_reporting_srv_th_c
 *    @snippet doxygen_snippets.dox ZB_ZCL_SET_NO_REPORTING_CB_snippet_reporting_srv_th_c
 *
 */

/** @brief ZCL version. 
*     According ZCL 7 spec:
*     3.2.2.2.1 ZCLVersion Attribute
*     The ZCLVersion attribute represents a published set of foundation items (in Chapter 2), 
*     such as global commands and functional descriptions. 
*     For this version of the ZCL, this attribute SHALL be set to 0x03.
*/
#define ZB_ZCL_VERSION       0x03

/**
 *  @brief Set modify attribute value callback.
 *  The macro sets a callback being called on external attribute change. The callback will be
 *  provided with all data necessary for correct value change handling.
 *  @param func_ptr - a callback to set (of type @ref zb_zcl_modify_attr_value_cb_t).
 *  @hideinitializer
 */
#define ZB_ZCL_SET_MODIFY_ATTR_VALUE_CB(func_ptr) ZCL_CTX().modify_attr_value_cb = (func_ptr)

#if !(defined ZB_ZCL_DISABLE_REPORTING) || defined(DOXYGEN)

/**
 *  @brief Set report attribute callback.
 *  The macro sets a callback being called on receive attribute report. The callback will
 *  be provided with all data necessary for correct attribute handling.
 *  @param func_ptr - callback to set (of type @ref zb_zcl_report_attr_cb_t).
 *  @hideinitializer
 */
#define ZB_ZCL_SET_REPORT_ATTR_CB(func_ptr) ZCL_CTX().report_attr_cb = (func_ptr)

/**
 *  @brief Set no reporting callback.
 *  The macro sets a callback being called on remote attribute reporting timeout. The callback will
 *  be provided with all data necessary for correct attribute report timeout handling.
 *  @param func_ptr - callback to set (of type @ref zb_zcl_no_reporting_cb_t).
 *  @hideinitializer
 */
#define ZB_ZCL_SET_NO_REPORTING_CB(func_ptr) ZCL_CTX().no_reporting_cb = (func_ptr)

#endif

/**
 *  @brief Set "Set default value all attribute" callback.
 *  The macro sets a callback being called for need to set attribute to default value.
 *  @param func_ptr - callback to set (of type @ref zb_zcl_set_default_value_attr_cb_t).
 *  @hideinitializer
 */
#define ZB_ZCL_SET_DEFAULT_VALUE_CB(func_ptr) ZCL_CTX().set_default_attr_cb = (func_ptr)

#if defined ZB_ENABLE_HA
/**
 *  @brief Set "Check attribute value" callback.
 *  The macro sets a callback being called for need to check attribute value.
 *  @param func_ptr - callback to set (of type @ref zb_zcl_app_check_attr_value_cb_t).
 *  @hideinitializer
 */
#define ZB_ZCL_CHECK_ATTR_VALUE_CB(func_ptr) ZCL_CTX().app_check_attr_value_cb = (func_ptr)

/**
    @brief Set "Discovery manufacture command" callback.
    The macro sets a callback that will be called to get a list of manufacture specific commands.
    @param func_ptr - callback to set (of type @ref zb_zcl_disc_manuf_cmd_cb_t).
    @hideinitializer

    Sample use Discovery manufacture command callback

    @par Example
    Example custom callback
    @code
    zb_uint8_t list_my_manufacture_command_generate[] = {
      MY_COMMAND_GENERATE_1,
      MY_COMMAND_GENERATE_2,
      MY_COMMAND_GENERATE_3,
      MY_COMMAND_GENERATE_4,
    };

    zb_uint8_t list_my_manufacture_command_receive[] = {
      MY_COMMAND_RECEIVE_1,
      MY_COMMAND_RECEIVE_2,
      MY_COMMAND_RECEIVE_3,
    };

    zb_uint8_t my_disc_manuf_cmd_cb(
        zb_uint8_t direction, zb_uint8_t ep, zb_uint16_t cluster_id, zb_uint16_t manuf_id, zb_bool_t recv_cmd_type, zb_uint8_t **value)
    {
       zb_uint8_t ret;

       if(manuf_id!=MY_MANUFACTURE_ID && manuf_id!=ZB_ZCL_MANUFACTURER_WILDCARD_ID)
         return 0;

      if (!recv_cmd_type)
      {
        value = &list_my_manufacture_command_generate;
        ret = sizeof(list_my_manufacture_command_generate);
      }
      else
      {
        value = &list_my_manufacture_command_receive;
        ret = sizeof(list_my_manufacture_command_receive);
      }
      return ret;
    }
    @endcode
    @par

    @par Example
    Example register callback
    @code
    ZB_ZCL_SET_DISC_MANUFACTURE_COMMAND_CB(&my_disc_manuf_cmd_cb);
    @endcode
    @par
 */
#define ZB_ZCL_SET_DISC_MANUFACTURE_COMMAND_CB(func_ptr) ZCL_CTX().disc_manuf_cmd_cb = (func_ptr)

#endif /* ZB_ENABLE_HA */

/** @cond internals_doc */
/**
 *  @brief Get current time in units.
 *  Uint = 1/10 sec
 */
#define ZB_GET_TIME_IN_UNITS()    ZB_BEACON_INTERVAL_TO_TIME_UNITS(ZB_TIMER_GET())

/**
 *  @brief Convert beacon interval to units
 *  Uint = 1/10 sec
 */
#define ZB_BEACON_INTERVAL_TO_TIME_UNITS(beacons)    ZB_TIME_BEACON_INTERVAL_TO_MSEC(beacons) / 100

/**
 *  @brief Convert units to beacon interval.
 *  Uint = 1/10 sec
 */
#define ZB_TIME_UNITS_TO_BEACON_INTERVAL(uints)  ZB_MILLISECONDS_TO_BEACON_INTERVAL((uints)*100)
/** @endcond */ /* internals_doc */

/**
 * Types of device user application callbacks
 * @see zb_zcl_device_callback_param_t
 */
typedef enum zb_zcl_device_callback_id_e
{
  /** Inform user about attribute value modification */
  ZB_ZCL_SET_ATTR_VALUE_CB_ID = 0,
  /** Inform user about received attribute report */
  ZB_ZCL_REPORT_ATTR_CB_ID,
  /** Inform user that report was not received during  defined time interval */
  ZB_ZCL_NO_REPORTING_CB_ID,
  /** Inform user about change On/Off effect @see ZLL spec 6.6.1.4.4.2 */
  ZB_ZCL_ON_OFF_WITH_EFFECT_VALUE_CB_ID,
  /** */
  /* TODO: describe this callback */
  ZB_ZCL_SHADE_SET_VALUE_CB_ID,
  /** */
  /* TODO: describe this callback */
  ZB_ZCL_SHADE_GET_VALUE_CB_ID,
  /** Inform user about call identify effect command @see ZLL spec 6.3.1.2.1 */
  ZB_ZCL_IDENTIFY_EFFECT_CB_ID,
  /** Inform user about ZCL Level Control cluster attributes value modification */
  ZB_ZCL_LEVEL_CONTROL_SET_VALUE_CB_ID,
  /** Inform user about enroll result command @see ZCL spec 8.2.2.3.1 */
  ZB_ZCL_IAS_ZONE_ENROLL_RESPONSE_VALUE_CB_ID,
  /** Inform user about call Start warning command @see ZCL spec 8.4.2.3.1 */
  ZB_ZCL_IAS_WD_START_WARNING_VALUE_CB_ID,
  /** Inform user about call Squawk command @see ZCL spec 8.4.2.3.2 */
  ZB_ZCL_IAS_WD_SQUAWK_VALUE_CB_ID,
  /** @b Server. Inform user about IAS ACE Arm command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_ias_ace_arm_t
   * @param[out] param_out @ref zb_zcl_ias_ace_arm_resp_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   *                  Send @ref ZB_ZCL_CMD_IAS_ACE_ARM_RESP_ID "Arm Response" command.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_IAS_ACE_ARM_CB_ID,
  /** @b Server. Inform user about IAS ACE Bypass command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_ias_ace_bypass_t
   * @param[out] param_out @ref zb_zcl_ias_ace_bypass_resp_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   *                  Send @ref ZB_ZCL_CMD_IAS_ACE_BYPASS_RESPONSE_ID "Bypass Response" command.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_IAS_ACE_BYPASS_CB_ID,
  /** @b Server. Inform user about IAS ACE Emergency command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_IAS_ACE_EMERGENCY_CB_ID,
  /** @b Server. Inform user about IAS ACE Fire command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_IAS_ACE_FIRE_CB_ID,
  /** @b Server. Inform user about IAS ACE Panic command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_IAS_ACE_PANIC_CB_ID,
  /** @b Server. Inform user about IAS ACE Get Panel Status command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[out] param_out @ref zb_zcl_ias_ace_get_panel_status_resp_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   *                  Send @ref ZB_ZCL_CMD_IAS_ACE_GET_PANEL_STATUS_RESPONSE_ID "Get Panel Status Response" command.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_IAS_ACE_GET_PANEL_STATUS_CB_ID,
  /** @b Server. Inform user about IAS ACE Get Bypassed Zone List command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[out] param_out @ref zb_zcl_ias_ace_set_bypassed_zone_list_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   *                  Send @ref ZB_ZCL_CMD_IAS_ACE_SET_BYPASSED_ZONE_LIST_ID "Set Bypassed Zone List" command.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_IAS_ACE_GET_BYPASSED_ZONE_LIST_CB_ID,
  /** @b Server. Inform user about IAS ACE Get Zone Status command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_ias_ace_get_zone_status_t
   * @param[out] param_out @ref zb_zcl_ias_ace_get_zone_status_resp_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   *                  Send @ref ZB_ZCL_CMD_IAS_ACE_GET_ZONE_STATUS_RESPONSE_ID "Get Zone Status Response" command.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_IAS_ACE_GET_ZONE_STATUS_CB_ID,
  /** @b Client. Inform user about IAS ACE Arm Response command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_ias_ace_arm_resp_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_IAS_ACE_ARM_RESP_CB_ID,
  /** @b Client. Inform user about IAS ACE Get Zone ID Map Response command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_ias_ace_get_zone_id_map_resp_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_IAS_ACE_GET_ZONE_ID_MAP_RESP_CB_ID,
  /** @b Client. Inform user about IAS ACE Get Zone Info Response command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_ias_ace_get_zone_info_resp_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_IAS_ACE_GET_ZONE_INFO_RESP_CB_ID,
  /** @b Client. Inform user about IAS ACE Zone Status Changed command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_ias_ace_zone_status_changed_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_IAS_ACE_ZONE_STATUS_CHANGED_CB_ID,
  /** @b Client. Inform user about IAS ACE Panel Status Changed command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_ias_ace_panel_status_changed_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_IAS_ACE_PANEL_STATUS_CHANGED_CB_ID,
  /** @b Client. Inform user about IAS ACE Get Panel Status Response command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_ias_ace_get_panel_status_resp_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_IAS_ACE_GET_PANEL_STATUS_RESP_CB_ID,
  /** @b Client. Inform user about IAS ACE Set Bypassed Zone List command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_ias_ace_set_bypassed_zone_list_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_IAS_ACE_SET_BYPASSED_ZONE_LIST_CB_ID,
  /** @b Client. Inform user about IAS ACE Bypass Response command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_ias_ace_bypass_resp_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_IAS_ACE_BYPASS_RESP_CB_ID,
  /** @b Client. Inform user about IAS ACE Get Zone Status Response command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_ias_ace_get_zone_status_resp_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_IAS_ACE_GET_ZONE_STATUS_RESP_CB_ID,
  /** Inform user about OTA Upgrade commands  */
  ZB_ZCL_OTA_UPGRADE_VALUE_CB_ID,
  /** Inform user about Basic Reset to Factory Defaults commands  */
  ZB_ZCL_BASIC_RESET_CB_ID,
  /** Inform user about call Thermostat command @see HA spec 10.1.3.3 */
  ZB_ZCL_THERMOSTAT_VALUE_CB_ID,
  /** Inform user about Check-In command from polled device */
  ZB_ZCL_POLL_CONTROL_CHECK_IN_CLI_CB_ID,
  /** @b Server. Inform user about EN50523 Appliance Events and Alerts Get Alerts command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[out] param_out zb_zcl_en50523_appl_ev_and_alerts_get_alerts_resp_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_GET_ALERTS_CB_ID,
  /** @b Client. Inform user about EN50523 Appliance Events and Alerts Get Alerts Response command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in zb_zcl_en50523_appl_ev_and_alerts_get_alerts_resp_t
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_GET_ALERTS_RESP_CB_ID,
  /** @b Client. Inform user about EN50523 Appliance Events and Alerts Alerts Notification command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in zb_zcl_en50523_appl_ev_and_alerts_alerts_notif_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_ALERTS_NOTIFICATION_CB_ID,
  /** @b Client. Inform user about EN50523 Appliance Events and Alerts Event Notification command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in zb_zcl_en50523_appl_ev_and_alerts_event_notif_t
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   */
  ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_EVENT_NOTIFICATION_CB_ID,
  /** @b Server. Inform user about Query next image request */
  ZB_ZCL_OTA_UPGRADE_SRV_QUERY_IMAGE_CB_ID,
  /** @b Server. Inform user about the first Image block request */
  ZB_ZCL_OTA_UPGRADE_SRV_UPGRADE_STARTED_CB_ID,
  /** @b Server. Inform user about the aborted Image block request  */
  ZB_ZCL_OTA_UPGRADE_SRV_UPGRADE_ABORTED_CB_ID,
  /** @b Server. Inform user about finished OTA upgrade */
  ZB_ZCL_OTA_UPGRADE_SRV_UPGRADE_END_CB_ID,
  /**@b Server. Ask user about payload for PublishPrice request.
   *
   * User's application callback is initialized by RET_NOT_FOUND status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_price_get_current_price_payload_t
   * @param[out] param_out @ref zb_zcl_price_publish_price_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - price data is available.
   *                  Send @ref ZB_ZCL_PRICE_SRV_CMD_PUBLISH_PRICE "PublishPrice" command.
   * @return RET_NOT_FOUND - price data isn't available. Default Response will be send if requested.
   *
   */
  ZB_ZCL_PRICE_GET_CURRENT_PRICE_CB_ID,
  /**@b Server. Notify user about GetScheduledPrices request.
   *
   * User's application callback is initialized by RET_NOT_FOUND status of device
   * callback parameters.
   * @param[out] param_out param_in @ref zb_zcl_price_get_scheduled_prices_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - price data is available.
   * @return RET_NOT_FOUND - price data isn't available. Default Response will be send if requested.
   *
   */
  ZB_ZCL_PRICE_GET_SCHEDULED_PRICES_CB_ID,
  /** @b Server. Notify user about GetTierLabels request.
   *
   * User's application callback is initialized by RET_NOT_FOUND status of device
   * callback parameters.
   * @param[in] param_out param_in @ref zb_zcl_price_get_tier_labels_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - price data is available. In this case application must send reply using
   * new buffer. Stack free buffer in this case.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   * @return RET_NOT_FOUND - price data isn't available. Default Response will be send if requested.
   *
   */
  ZB_ZCL_PRICE_GET_TIER_LABELS_CB_ID,
  /** @b Client. Inform user about PublishPrice request.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_price_publish_price_payload_t
   * @param[out] param_out @ref zb_zcl_price_ack_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   *                  Send @ref ZB_ZCL_PRICE_CLI_CMD_PRICE_ACK "PriceAcknowledgement" command.
   * @return RET_ERROR - command is handled with errors
   *
   */
  ZB_ZCL_PRICE_PUBLISH_PRICE_CB_ID,
  /** @b Client. Inform user about PublishTierLabels request.
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_price_publish_tier_labels_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors
   *
   */
  ZB_ZCL_PRICE_PUBLISH_TIER_LABELS_CB_ID,
  /** @b Server. Inform user about PriceAck command.
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_price_ack_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_PRICE_PRICE_ACK_CB_ID,
  /** @b Server. Inform user about change of state for an active Load Control event
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_drlc_report_event_status_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   */
  ZB_ZCL_DRLC_REPORT_EVENT_STATUS_CB_ID,
  /** @b Server. Ask user about currently active or scheduled Load Control Events
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_drlc_get_scheduled_events_payload_t
   * @param[out] param_out @ref zb_zcl_drlc_lce_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   *                  Send @ref ZB_ZCL_DRLC_SRV_CMD_LOAD_CONTROL_EVENT "LoadControlEvent" command
   * @return RET_ERROR - command is handled with errors.
   */
  ZB_ZCL_DRLC_GET_SCHEDULED_EVENTS_CB_ID,
  /** @b Client. Inform user about received Load Control Event
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_drlc_lce_payload_t
   * @param[out] param_out @ref zb_zcl_drlc_report_event_status_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   *                  Send @ref ZB_ZCL_DRLC_CLI_CMD_REPORT_EVENT_STATUS "ReportEventStatus" in response.
   * @return RET_IGNORE - application doesn't received and resolved time. Drop event.
   *                      Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   */
  ZB_ZCL_DRLC_LOAD_CONTROL_EVENT_CB_ID,
  /** @b Client. Inform user about Load Control Event cancellation
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_price_publish_price_payload_t
   * @param[out] param_out @ref zb_zcl_price_ack_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   *                  Send @ref ZB_ZCL_DRLC_CLI_CMD_REPORT_EVENT_STATUS "ReportEventStatus" in response.
   * @return RET_IGNORE - application doesn't received and resolved time. Drop event.
   *                      Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   */
  ZB_ZCL_DRLC_CANCEL_LOAD_CONTROL_EVENT_CB_ID,
  /** @b Client. Inform user about all Load Control Events cancellation
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_drlc_cancel_lce_payload_t
   * @param[out] param_out @ref zb_zcl_drlc_report_event_status_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_IGNORE - application doesn't received and resolved time. Drop events.
   *                      Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   */
  ZB_ZCL_DRLC_CANCEL_ALL_LOAD_CONTROL_EVENTS_CB_ID,
  /** @b Server. Inform user about received Message Confirmation request
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_messaging_message_confirm_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   */
  ZB_ZCL_MESSAGING_MSG_CONFIRMATION_CB_ID,
  /** @b Server. Ask user about payload for Display Message request
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[out] param_out @ref zb_zcl_messaging_get_last_message_response_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   *                  Send @ref ZB_ZCL_MESSAGING_SRV_CMD_DISPLAY_MESSAGE "DisplayMessage" command
   * @return RET_NOT_FOUND - no messages is available. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   */
  ZB_ZCL_MESSAGING_GET_LAST_MSG_CB_ID,
  /** @b Client. Inform user about received Cancel Message request
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_messaging_cancel_message_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   */
  ZB_ZCL_MESSAGING_CANCEL_MSG_CB_ID,
  /** @b Client. Inform user about received Display Message request
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_messaging_display_message_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   */
  ZB_ZCL_MESSAGING_DISPLAY_MSG_CB_ID,
  /** @b Server. Inform user about request to setup a tunnel association with the server
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_tunneling_request_tunnel_t
   * @param[out] param_out tunnel_status
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   * @return RET_ERROR - command is handled with errors.
   */
  ZB_ZCL_TUNNELING_REQUEST_TUNNEL_CB_ID,
  /** @b Client. Inform user about response to a RequestTunnel command
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_tunneling_request_tunnel_response_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   */
  ZB_ZCL_TUNNELING_REQUEST_TUNNEL_RESPONSE_CB_ID,
  /** @b Server. Inform user about received TransferData request
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_tunneling_transfer_data_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   * @return RET_ERROR - command is handled with errors.
   *                     Send @ref ZB_ZCL_TUNNELING_SRV_CMD_TRANSFER_DATA_ERROR "TransferDataError" command
   */
  ZB_ZCL_TUNNELING_TRANSFER_DATA_CLI_CB_ID,
  /** @b Client. Inform user about received TransferData request
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_tunneling_transfer_data_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   * @return RET_ERROR - command is handled with errors.
   *                     Send @ref ZB_ZCL_TUNNELING_SRV_CMD_TRANSFER_DATA_ERROR "TransferDataError" command
   */
  ZB_ZCL_TUNNELING_TRANSFER_DATA_SRV_CB_ID,
  /** @b Server. Inform user about received TransferDataError request
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_tunneling_transfer_data_error_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   * @return RET_ERROR - command is handled with errors.
   */
  ZB_ZCL_TUNNELING_TRANSFER_DATA_ERROR_CLI_CB_ID,
  /** @b Client. Inform user about received TransferDataError request
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_tunneling_transfer_data_error_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   * @return RET_ERROR - command is handled with errors.
   */
  ZB_ZCL_TUNNELING_TRANSFER_DATA_ERROR_SRV_CB_ID,
  /** @b Server. Inform user about closing the tunnel with the server.
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_tunneling_close_tunnel_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_NOT_FOUND - @e TunnelID value doesn't match an active tunnel on the server.
   *                         Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   */
  ZB_ZCL_TUNNELING_CLOSE_TUNNEL_CB_ID,
  /** @b Server. Inform user about GetProfile request.
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_metering_get_profile_payload_t
   * @param[out] param_out @ref zb_zcl_metering_get_profile_response_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   *                  Send @ref ZB_ZCL_METERING_SRV_CMD_GET_PROFILE_RESPONSE "GetProfileResponse" command.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   *
   */
  ZB_ZCL_METERING_GET_PROFILE_CB_ID,
  /** @b Server. Inform user about RequestFastPollMode request.
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_metering_request_fast_poll_mode_payload_t
   * @param[out] param_out @ref zb_zcl_metering_request_fast_poll_mode_response_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   *                  Send @ref ZB_ZCL_METERING_SRV_CMD_REQUEST_FAST_POLL_MODE_RESPONSE "RequestFastPollModeResponse" command.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   *
   */
  ZB_ZCL_METERING_REQUEST_FAST_POLL_MODE_CB_ID,
  /** @b Server. Inform user about GetSnapshot request.
   *
   * User's application callback is initialized by RET_NOT_FOUND status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_metering_get_snapshot_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.  In this case application must send reply using
   *                  new buffer. Stack free buffer in this case.
   * @return RET_NOT_FOUND - price data isn't available. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   *
   */
  ZB_ZCL_METERING_GET_SNAPSHOT_CB_ID,
  /** @b Server. Inform user about GetSampledData request.
   *
   * User's application callback is initialized by RET_NOT_FOUND status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_metering_get_sampled_data_payload_t
   * @param[out] param_out @ref zb_zcl_metering_get_sampled_data_response_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   *                  Send @ref ZB_ZCL_METERING_SRV_CMD_GET_SAMPLED_DATA_RESPONSE "GetSampledData" command.
   * @return RET_NOT_FOUND - price data isn't available. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   *
   */
  ZB_ZCL_METERING_GET_SAMPLED_DATA_CB_ID,
  /** @b Client. Inform user about response to GetProfileResponse command.
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_metering_get_profile_response_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   *
   */
  ZB_ZCL_METERING_GET_PROFILE_RESPONSE_CB_ID,
  /** @b Client. Inform user about response to RequestFastPollMode command.
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_metering_request_fast_poll_mode_response_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   *
   */
  ZB_ZCL_METERING_REQUEST_FAST_POLL_MODE_RESPONSE_CB_ID,
  /** @b Client. Inform user about response to GetSnapshot command.
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_metering_publish_snapshot_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   *
   */
  ZB_ZCL_METERING_PUBLISH_SNAPSHOT_CB_ID,
  /** @b Client. Inform user about response to GetSampledData command.
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_metering_get_sampled_data_response_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   *
   */
  ZB_ZCL_METERING_GET_SAMPLED_DATA_RESPONSE_CB_ID,
  /** Inform user about APS fragmented data transfer completion */
  ZB_ZCL_BIG_DATA_TRANSFER_COMPLETE_CB_ID,
  /** Inform user about Time receiving from Time server */
  ZB_ZCL_TIME_SYNC_CB_ID,
  ZB_ZCL_TIME_SYNC_FAILED_CB_ID,
  /** @b Server. Inform user about Window Covering Up/Open command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_WINDOW_COVERING_UP_OPEN_CB_ID,
  /** @b Server. Inform user about Window Covering Down/Close command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_WINDOW_COVERING_DOWN_CLOSE_CB_ID,
  /** @b Server. Inform user about Window Covering Stop command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_WINDOW_COVERING_STOP_CB_ID,
  /** @b Server. Inform user about Window Covering Go to Lift Percentage command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_go_to_lift_percentage_req_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_WINDOW_COVERING_GO_TO_LIFT_PERCENTAGE_CB_ID,
  /** @b Server. Inform user about Window Covering Go to Tilt Percentage command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_go_to_tilt_percentage_req_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_WINDOW_COVERING_GO_TO_TILT_PERCENTAGE_CB_ID,
  /** @b Server. Inform application about incoming Add Scene request.
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_scenes_add_scene_req_t Fixed part of Add Scene request. Scene
   * Extension Set is placed after this structure (no little-endian conversion).
   * @param[out] param_out @ref zb_uint8_t ZCL status - @ref ZB_ZCL_STATUS_INVALID_FIELD,
   * @ref ZB_ZCL_STATUS_SUCCESS or @ref ZB_ZCL_STATUS_INSUFF_SPACE
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command (except the case of updating existing entry). Send
   * @ref ZB_ZCL_CMD_SCENES_ADD_SCENE_RESPONSE "Add Scene Response" command (with ZCL status from
   * param_out).
   * @return RET_ALREADY_EXISTS - successfully handle command (update existing entry). Send
   * @ref ZB_ZCL_CMD_SCENES_ADD_SCENE_RESPONSE "Add Scene Response" command (with ZCL status from param_out).
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   *
   */
  ZB_ZCL_SCENES_ADD_SCENE_CB_ID,
  /** @b Server. Inform application about incoming Store Scene request.
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_scenes_store_scene_req_t
   * @param[out] param_out @ref zb_uint8_t ZCL status - @ref ZB_ZCL_STATUS_INVALID_FIELD,
   * @ref ZB_ZCL_STATUS_SUCCESS or @ref ZB_ZCL_STATUS_INSUFF_SPACE
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command (except the case of updating existing entry). Send
   * @ref ZB_ZCL_CMD_SCENES_STORE_SCENE_RESPONSE "Store Scene Response" command (with ZCL status from
   * param_out).
   * @return RET_ALREADY_EXISTS - successfully handle command (update existing entry). Send
   * @ref ZB_ZCL_CMD_SCENES_STORE_SCENE_RESPONSE "Store Scene Response" command (with ZCL status from param_out).
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   *
   */
  ZB_ZCL_SCENES_STORE_SCENE_CB_ID,
  /** @b Server. Inform application about incoming View Scene request.
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_scenes_view_scene_req_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. In this case application must send
   * @ref ZB_ZCL_CMD_SCENES_VIEW_SCENE_RESPONSE "View Scene Response" using new buffer. Stack free
   * buffer in this case.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   *
   */
  ZB_ZCL_SCENES_VIEW_SCENE_CB_ID,
  /** @b Server. Inform application about incoming Remove Scene request.
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_scenes_remove_scene_req_t
   * @param[out] param_out @ref zb_uint8_t ZCL status - @ref ZB_ZCL_STATUS_INVALID_FIELD,
   * @ref ZB_ZCL_STATUS_SUCCESS or @ref ZB_ZCL_STATUS_NOT_FOUND
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Send
   * @ref ZB_ZCL_CMD_SCENES_REMOVE_SCENE_RESPONSE "Remove Scene Response" command (with ZCL status
   * from param_out).
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   *
   */
  ZB_ZCL_SCENES_REMOVE_SCENE_CB_ID,
  /** @b Server. Inform application about incoming Remove All Scenes request.
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_scenes_remove_all_scenes_req_t
   * @param[out] param_out @ref zb_uint8_t ZCL status - @ref ZB_ZCL_STATUS_INVALID_FIELD or
   * @ref ZB_ZCL_STATUS_SUCCESS
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Send
   * @ref ZB_ZCL_CMD_SCENES_REMOVE_ALL_SCENES_RESPONSE "Remove All Scenes Response" command (with
   * ZCL status from param_out).
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   *
   */
  ZB_ZCL_SCENES_REMOVE_ALL_SCENES_CB_ID,
  /** @b Server. Inform application about incoming Recall Scene request.
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_scenes_recall_scene_req_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   *
   */
  ZB_ZCL_SCENES_RECALL_SCENE_CB_ID,
  /** @b Server. Inform application about incoming Get Scene Membership request.
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_scenes_get_scene_membership_req_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.  In this case application must send
   * @ref ZB_ZCL_CMD_SCENES_GET_SCENE_MEMBERSHIP_RESPONSE "Get Scene Membership Response" using new
   * buffer. Stack free buffer in this case.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   *
   */
  ZB_ZCL_SCENES_GET_SCENE_MEMBERSHIP_CB_ID,
  /** @b Server. Internal: indicate to application that it is needed to remove all scenes on all
   * endpoints for given group_id.
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_scenes_remove_all_scenes_req_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_SCENES_INTERNAL_REMOVE_ALL_SCENES_ALL_ENDPOINTS_CB_ID,
  /** @b Server. Internal: indicate to application that it is needed to remove all scenes on all
   * endpoints for all groups (full reset of scenes table).
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_SCENES_INTERNAL_REMOVE_ALL_SCENES_ALL_ENDPOINTS_ALL_GROUPS_CB_ID,
  /** @b Server. Inform user about Door Lock Lock Door command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[out] param_out @ref zb_zcl_door_lock_read_lock_door_res_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   *                  Send @ref ZB_ZCL_DOOR_LOCK_SEND_LOCK_DOOR_RES "Lock Door Response" command.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_DOOR_LOCK_LOCK_DOOR_CB_ID,
  /** @b Server. Inform user about Door Lock Unlock Door command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[out] param_out @ref zb_zcl_door_lock_read_unlock_door_res_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   *                  Send @ref ZB_ZCL_DOOR_LOCK_SEND_UNLOCK_DOOR_RES "Unlock Door Response" command.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_DOOR_LOCK_UNLOCK_DOOR_CB_ID,
  /** @b Client. Inform user about Door Lock Lock Door Response command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_door_lock_read_lock_door_res_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_DOOR_LOCK_LOCK_DOOR_RESP_CB_ID,
  /** @b Client. Inform user about Door Lock Unlock Door Response command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_door_lock_read_unlock_door_res_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_DOOR_LOCK_UNLOCK_DOOR_RESP_CB_ID,
  /** @b Server. Inform user about Alarms Reset Alarm command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_alarms_reset_alarm_req_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_ALARMS_RESET_ALARM_CB_ID,
  /** @b Server. Inform user about Alarms Reset All Alarms command.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_ALARMS_RESET_ALL_ALARMS_CB_ID,
  /** @b Client. Inform user about Alarms Alarm command. 
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in zb_zcl_alarms_alarm_res_t
   * 
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handled command. Response will be send if requested.
   * @return RET_ERROR - command is handled with errors
   * 
  */
  ZB_ZCL_ALARMS_ALARM_CB_ID,
  /** @b Server. Inform user about receiving "Enable WWAH App Event Retry Algorithm" command.
   *
   * User's application callback is initialized by RET_NOT_FOUND status of device
   * callback parameters.
   * @param[in] param_in zb_zcl_wwah_enable_wwah_app_event_retry_algorithm_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   *
   */
  ZB_ZCL_WWAH_ENABLE_APP_EVENT_RETRY_ALGORITHM_CB_ID,
  /** @b Server. Inform user about receiving "Disable WWAH App Event Retry Algorithm" command.
   *
   * User's application callback is initialized by RET_NOT_FOUND status of device
   * callback parameters.
   * @param[in] param_in zb_zcl_wwah_enable_wwah_app_event_retry_algorithm_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   *
   */
  ZB_ZCL_WWAH_DISABLE_APP_EVENT_RETRY_ALGORITHM_CB_ID,
  /** @b Server. Inform user application about Control4 Network ZAP information
   * changes (event=ZB_ZCL_CONTROL4_NETWORK_ZAP_UPDATED) or failure to get changes
   * (event=ZB_ZCL_CONTROL4_NETWORK_ZAP_DISCOVER_FAILED) after expiring the
   * ZB_ZCL_CONTROL4_NETWORK_SEARCHING_ZAP_ATTEMPTS retries
   *
   * @param[in] param_in zb_zcl_control4_zap_info_notify_t
   */
  ZB_ZCL_CONTROL4_NETWORK_ZAP_INFO_CB_ID,
  /** @b Server. Inform user about receiving "Debug Report Query" command.
   * If debug report exists, application must return pointer to this report   
   * User's application callback is initialized by RET_NOT_FOUND status of device
   * callback parameter.
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Debug Report Query Response will be send.
   *                  param out must point to zb_zcl_wwah_debug_report_t
   *                  For example:
   *                  zb_zcl_wwah_debug_report_t debug_report_table[10]
   *                  ZB_ZCL_DEVICE_CMD_PARAM_OUT_SET(param, &debug_report_table[4]);
   * @return RET_ERROR, RET_NOT_FOUND - command is handled with errors. 
   *                  Default Response will be send.
   */
  ZB_ZCL_WWAH_DEBUG_REPORT_QUERY_CB_ID,
  /** @b Server. Inform user about receiving "Set IAS Zone Enrollment Method" command.
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   * @return RET_ERROR - command is handled with errors.
   *                  Default Response will be send.
   */
  ZB_ZCL_WWAH_SET_IAS_ZONE_ENROLLMENT_METHOD_CB_ID,

} zb_zcl_device_callback_id_t;

/** @cond internals_doc */
/** Generic in/out params for cluster commands handlers. */
typedef struct zb_zcl_device_cmd_generic_param_s
{
  /** Input packet header metadata.
   * Represents packet header information for incoming packets.
   */
  const zb_zcl_parsed_hdr_t *in_cmd_info;

  /** Input packet payload. */
  const void *in;

  /** Output packet payload.
   * User application should fill the payload with specific data.
   * ZCL can fill some of fields of payload with default/calculated values.
   */
  void *out;
} zb_zcl_device_cmd_generic_param_t;
/** @endcond */ /* internals_doc */

/**
 *  @brief Data for Device user application callback.
 *  This structure has receive to User application callback (see @ref zb_callback_t).
 *  @hideinitializer
 * */
typedef struct zb_zcl_device_callback_param_s
{
  /** Type of device callback (see @ref zb_zcl_device_callback_id_e) */
  zb_zcl_device_callback_id_t device_cb_id;
  zb_uint8_t endpoint;
  zb_zcl_attr_access_t attr_type;
  /** Return status (see zb_ret_t) */
  zb_ret_t status;
  /** Callback custom data */
  union
  {
    /* TODO: If def by cluster */
    zb_zcl_set_attr_value_param_t  set_attr_value_param;
    /* Off with effect command, On/Off cluster */
    zb_zcl_on_off_set_effect_value_param_t  on_off_set_effect_value_param;
    /* */

#if defined(ZB_ZCL_SUPPORT_CLUSTER_IDENTIFY)
    zb_zcl_identify_effect_value_param_t  identify_effect_value_param;
#endif

   // TODO: descript parameters struct for translate to User App ZB_ZCL_SHADE_SET_VALUE_CB_ID event
   zb_zcl_shade_set_value_param_t  shade_set_value_param;
   // TODO: descript parameters struct for translate to User App ZB_ZCL_SHADE_GET_VALUE_CB_ID event
   zb_zcl_shade_get_value_param_t  shade_get_value_param;
   //
   zb_zcl_level_control_set_value_param_t  level_control_set_value_param;
   //
   zb_zcl_ias_wd_start_warning_value_param_t  start_warning_value_param;
#if defined ZB_ENABLE_HA
   //
   zb_zcl_ias_zone_enroll_response_value_param_t enroll_response_value_param;
   //
   zb_zcl_thermostat_value_param_t thermostat_value_param;
#endif /* defined ZB_ENABLE_HA */
   //
   zb_zcl_ias_wd_squawk_value_param_t  squawk_value_param;
#if defined ZB_ENABLE_HA
#if defined ZB_HA_ENABLE_OTA_UPGRADE_CLIENT || defined DOXYGEN
   //
   zb_zcl_ota_upgrade_value_param_t  ota_value_param;
#endif /*defined ZB_HA_ENABLE_OTA_UPGRADE_CLIENT || defined DOXYGEN */
   zb_zcl_poll_control_check_in_cli_param_t checkin_cli_param;
#if defined ZB_HA_ENABLE_OTA_UPGRADE_SERVER
   zb_zcl_ota_upgrade_srv_query_img_param_t       ota_upgrade_srv_query_img_param;
   zb_zcl_ota_upgrade_srv_upgrade_started_param_t ota_upgrade_srv_upgrade_started_param;
   zb_zcl_ota_upgrade_srv_upgrade_aborted_param_t ota_upgrade_srv_upgrade_aborted_param;
   zb_zcl_ota_upgrade_srv_upgrade_end_param_t     ota_upgrade_srv_upgrade_end_param;
#endif
#endif /* defined ZB_ENABLE_HA */
    zb_zcl_device_cmd_generic_param_t gnr;
  }
  cb_param;
} zb_zcl_device_callback_param_t;


/** @cond internals_doc */
/** The way of accessing to @ref zb_zcl_device_callback_param_t from buf ref. */
#define ZB_ZCL_DEVICE_CMD_PARAM(_param) \
    (ZB_BUF_GET_PARAM((_param), zb_zcl_device_callback_param_t))

/** Set INPUT device callback parameter with specific value. */
#define ZB_ZCL_DEVICE_CMD_PARAM_IN_SET(_param, _pvalue) \
  ((ZB_ZCL_DEVICE_CMD_PARAM(_param))->cb_param.gnr.in = _pvalue)

/** Set OUTPUT device callback parameter with specific value. */
#define ZB_ZCL_DEVICE_CMD_PARAM_OUT_SET(_param, _pvalue) \
  ((ZB_ZCL_DEVICE_CMD_PARAM(_param))->cb_param.gnr.out = _pvalue)

/** Init all fields of device callback params. */
#define ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(_param, _cb_id, _status, _cmd_info, _in, _out) \
  (ZB_BZERO(ZB_ZCL_DEVICE_CMD_PARAM(_param), sizeof(*ZB_ZCL_DEVICE_CMD_PARAM(_param))), \
   (ZB_ZCL_DEVICE_CMD_PARAM_CB_ID(_param) = _cb_id, \
    (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(_param) = _status, \
     (ZB_ZCL_DEVICE_CMD_PARAM_CMD_INFO(_param) = _cmd_info, \
      (ZB_ZCL_DEVICE_CMD_PARAM_IN_SET(_param, _in), \
       (ZB_ZCL_DEVICE_CMD_PARAM_OUT_SET(_param, _out)))))))
/** @endcond */ /* internals_doc */

/** Get INPUT device callback parameter from buffer reference.
 * @param _param - Reference to buffer.
 * @param _ptype - Target data type.
 * @return - Pointer to @e _ptype data.
 * @see @ref zb_zcl_device_callback_param_t.
 */
#define ZB_ZCL_DEVICE_CMD_PARAM_IN_GET(_param, _ptype) \
  ((const _ptype *) ((ZB_ZCL_DEVICE_CMD_PARAM(_param))->cb_param.gnr.in))

/** Get OUTPUT device callback parameter from buffer reference.
 * @param _param - Reference to buffer.
 * @param _ptype - Target data type
 * @return - Pointer to @e _ptype data.
 */
#define ZB_ZCL_DEVICE_CMD_PARAM_OUT_GET(_param, _ptype) \
  ((_ptype *) ((ZB_ZCL_DEVICE_CMD_PARAM(_param))->cb_param.gnr.out))

/** Get access to @ref zb_zcl_parsed_hdr_t data from device callback parameter.
 * @param _param - Reference to buffer.
 * @return - Pointer to @ref zb_zcl_parsed_hdr_t structure.
 */
#define ZB_ZCL_DEVICE_CMD_PARAM_CMD_INFO(_param) \
  ((ZB_ZCL_DEVICE_CMD_PARAM(_param))->cb_param.gnr.in_cmd_info)

/** Get access to @ref zb_zcl_device_callback_param_t::device_cb_id of device callback parameters.
 * @param _param - Reference to buffer.
 */
#define ZB_ZCL_DEVICE_CMD_PARAM_CB_ID(_param) \
  ((ZB_ZCL_DEVICE_CMD_PARAM(_param))->device_cb_id)

/** Get access to @ref zb_zcl_device_callback_param_t::status of device callback parameters.
 * @param _param - Reference to buffer.
 * @return - zb_ret_t value of current status.
 */
#define ZB_ZCL_DEVICE_CMD_PARAM_STATUS(_param) \
  ((ZB_ZCL_DEVICE_CMD_PARAM(_param))->status)

/** @cond internals_doc */
/** @brief ZCL default handler.
  *
  * This function handles ZCL commands which was not processed by application.
  *
  * @param param - reference to the buffer containing ZCL packet.
  * @return @ref ZB_TRUE if packet was processed; @ref ZB_FALSE - otherwise
  */
zb_uint8_t zb_zcl_handle(zb_uint8_t param);
/**
   Get ZCL TSN stored in ZCL frame
   @param buffer - pointer to a buffer with ZCL frame
   @return TSN value
*/
zb_uint8_t zb_zcl_get_tsn_from_packet(zb_bufid_t buffer);

#if !(defined ZB_ZCL_DISABLE_REPORTING)

void zb_zcl_adjust_reporting_timer(zb_uint8_t param);

void zb_zcl_update_reporting_info(zb_zcl_reporting_info_t *rep_info);

zb_ret_t zb_zcl_put_default_reporting_info(zb_zcl_reporting_info_t* default_rep_info_ptr);

void zb_zcl_remove_default_reporting_info(zb_uint16_t cluster_id, zb_uint8_t cluster_role);

void zb_zcl_mark_report_not_sent(zb_zcl_reporting_info_t *rep_info);

#endif

/**
   Register device context.
   @param _device_ctx - pointer to device context

   Use @ref ZB_AF_REGISTER_DEVICE_CTX.
 */
void zb_zcl_register_device_ctx(zb_af_device_ctx_t *device_ctx);
zb_bool_t zb_zcl_check_cluster_list(void);
/** @endcond */ /* internals_doc */
/** @} */ /* ZB_ZCL_INITIALIZATION */


#ifdef ZB_BDB_MODE
/*! \addtogroup zboss_bdb_api
@{
 @defgroup zboss_bdb_comm_params BDB commissioning parameters
 @defgroup zboss_bdb_comm_start BDB commissioning start & status
 @defgroup zboss_bdb_comm_fb BDB Finding and Binding
@}
*/

/**
   @addtogroup zboss_bdb_comm_params
   @{
*/

/** @cond internals_doc */
/**
  @brief BDB error codes
  */
enum zb_bdb_error_codes_e
{
  ZB_BDB_STATUS_SUCCESS = 0,                 /*!<The commissioning sub-procedure was successful.*/
  ZB_BDB_STATUS_IN_PROGRESS,                 /*!< One of the commissioning sub-procedures has started but is not yet complete.*/
  ZB_BDB_STATUS_NOT_AA_CAPABLE,              /*!< The initiator is not address assignment capable during touchlink. */
  ZB_BDB_STATUS_NO_NETWORK,                  /*!< A network has not been found during network steering or touchlink.*/
  ZB_BDB_STATUS_TARGET_FAILURE,              /*!< A node has not joined a network when requested during touchlink.*/
  ZB_BDB_STATUS_FORMATION_FAILURE,           /*!< A network could not be formed during network formation. */
  ZB_BDB_STATUS_NO_IDENTIFY_QUERY_RESPONSE,  /*!< No response to an identify query command has been received during finding and binding.*/
  ZB_BDB_STATUS_BINDING_TABLE_FULL,          /*!< A binding table entry could not be created due to insufficient space in the binding table during finding and binding. */
  ZB_BDB_STATUS_NO_SCAN_RESPONSE,            /*!< No response to a scan request inter-PAN command has been received during touchlink. */
  ZB_BDB_STATUS_NOT_PERMITTED,               /*!< A touchlink (steal) attempt was made when a node is already connected to a centralized security network.*/
  ZB_BDB_STATUS_TCLK_EX_FAILURE              /*!< The Trust Center link key exchange procedure has failed attempting to join a centralized security network.*/
};
/** @endcond */ /* internals_doc */
/** @} */

/**
   @addtogroup zboss_bdb_comm_start
   @{
*/

/** @brief BDB commissioning mode mask bits */
typedef enum zb_bdb_commissioning_mode_mask_e
{
  /** @cond internals_doc */
  /** Used internally */
  ZB_BDB_INITIALIZATION = 0,
  /** @endcond */ /* internals_doc */
  /** @cond touchlink */
  /** Touchlink: 0 = Do not attempt Touchlink commissioning;
                  1 = Attempt Touchlink commissioning
   */
  ZB_BDB_TOUCHLINK_COMMISSIONING = 1,
  /** @endcond */ /* touchlink */

  /** Network steering: 0 = Do not attempt network steering;
                         1 = Attempt network steering
   */
  ZB_BDB_NETWORK_STEERING = 2,
  /** Network formation: 0 = Do not attempt to form a network;
                          1 = Attempt to form a network, according to device type2
   */
  ZB_BDB_NETWORK_FORMATION = 4,
  /** @cond internals_doc */
  /** Finding and binding: 0 = Do not attempt finding and binding;
                          1 = Attempt finding and binding
    @note actually this mode does not call finding and binding procedure. Use
          @ref zb_bdb_finding_binding_target or @ref zb_bdb_finding_binding_initiator.
   */
  ZB_BDB_FINDING_N_BINDING = 8,
  
  /* Used internally */
  ZB_BDB_LAST_COMMISSIONING_STEP = 0x10,
  /* Used internally */
  ZB_BDB_COMMISSIONING_STOP = 0x20,

  /** @cond touchlink */
  /* Used internally */
  ZB_BDB_TOUCHLINK_TARGET = 0x40,
  /** @endcond */ /* touchlink */
#ifndef BDB_OLD
  /* Used internally */
  ZB_BDB_REJOIN = 0x80,
#endif
} zb_bdb_commissioning_mode_mask_t;

/**
   @brief Start top level commissioning procedure with specified mode mask.
   When the selected commissioning procedure finishes one of the following ZBOSS signals is generated:
    - @ref ZB_BDB_SIGNAL_STEERING 
    - @ref ZB_BDB_SIGNAL_FORMATION
    
   @param mode_mask - commissioning modes, see @ref zb_bdb_commissioning_mode_mask_e
   
   @return ZB_TRUE - in case the device starts successfully
   @return ZB_FALSE - ZB_FALSE -- in case an error occurred (for example: the device has already been running)

   @b Example:
   @code
   zb_zdo_app_signal_hdr_t *sg_p = NULL;
   zb_zdo_app_signal_type_type_t sig = zb_get_app_signal(param, &sg_p);

   switch(sig)
     {
       case ZB_BDB_SIGNAL_DEVICE_FIRST_START:
         TRACE_MSG(TRACE_APP1, "Device STARTED OK", (FMT__0));
         zb_bdb_set_legacy_device_support(1);
         bdb_start_top_level_commissioning(ZB_BDB_NETWORK_STEERING);
         break;

       case ZB_BDB_SIGNAL_STEERING:
         TRACE_MSG(TRACE_APP1, "Successful steering", (FMT__0));
         break;
     }
   @endcode
  */
zb_bool_t bdb_start_top_level_commissioning(zb_uint8_t mode_mask);

/**
   Check if device is factory new.

   @return ZB_TRUE if factory new.
   @b Example:
@code
    if (!zb_bdb_is_factory_new())
    {
      zb_zdo_rejoin_backoff_start(ZB_FALSE);
    }
@endcode
*/
zb_bool_t zb_bdb_is_factory_new(void);

/** @} */

#if defined(ZB_BDB_ENABLE_FINDING_BINDING) || defined(DOXYGEN)

/**
   @addtogroup zboss_bdb_comm_fb
   @{
*/
/**
   @brief Starts EZ-Mode Finding and binding mechanism at the target's endpoint.

   Put device into identifying mode; default duration is 3 minutes.

   @param endpoint - target endpoint
   @return RET_OK if procedure was successfully started

   @note: endpoint should be registered on target

   @b Example
   @snippet onoff_server/on_off_output_zc.c zb_bdb_finding_binding_target_usage
  */
zb_ret_t zb_bdb_finding_binding_target(zb_uint8_t endpoint);


/**
 * List of EZ-Mode binding callback states
 */
typedef enum zb_bdb_comm_binding_cb_state_e
{
  /** Previously user applied bind finished successfully */
  ZB_BDB_COMM_BIND_SUCCESS = 0,
  /** Previously user applied bind failed */
  ZB_BDB_COMM_BIND_FAIL = 1,
  /** Ask user whether to perform binding */
  ZB_BDB_COMM_BIND_ASK_USER = 2,
} zb_bdb_comm_binding_cb_state_t;

/**
 *  @brief BDB finding and binding callback template.
 *
 *  Function is used both to interact with user application, get decision
 *  if new binding is needed or not, and to report the binding result
 *
 *  @param status - status of the binding (ask user, success or fail) @see zb_bdb_comm_binding_cb_state_t
 *  @param addr - extended address of a device to bind
 *  @param ep - endpoint of a device to bind
 *  @param cluster - cluster ID to bind
 *  @return bool - agree or disagree
 *
 * @b Example:
 * @snippet onoff_server/on_off_switch_zed.c zb_bdb_finding_binding_initiator_cb_example
 */
typedef zb_bool_t (ZB_CODE * zb_bdb_comm_binding_callback_t)(
  zb_int16_t status, zb_ieee_addr_t addr, zb_uint8_t ep, zb_uint16_t cluster);

/**
 *  @brief Start BDB finding and binding procedure on initiator.
 *
 *  Summary: Finding and binding as initiator zb_bdb_finding_binding_initiator()
 *  returns RET_OK if procedure was started successfully, error code otherwise. To report procedure
 *  status, user callback is called. It may be called several times with Success status and only once
 *  with error status. If any error appears, finding and binding stops.
 *
 *  @param endpoint - initiator endpoint
 *  @param user_binding_cb - user callback, see @ref zb_bdb_comm_binding_callback_t
 *  @return bool - RET_OK or error code
 *
 * @b Example:
 *
 * Define callback:
 * @snippet onoff_server/on_off_switch_zed.c zb_bdb_finding_binding_initiator_cb_example
 *
 * Start finding and binding as initiator:
 * @snippet onoff_server/on_off_switch_zed.c zb_bdb_finding_binding_initiator
 */
zb_ret_t zb_bdb_finding_binding_initiator(zb_uint8_t endpoint, zb_bdb_comm_binding_callback_t user_binding_cb);

/** Cancel previously started finding and binding procedure on target */
void zb_bdb_finding_binding_target_cancel(void);

/** Cancel previously started finding and binding procedure on initiator */
void zb_bdb_finding_binding_initiator_cancel(void);

/** @} */

#endif

/**
   @addtogroup zboss_bdb_comm_params
   @{
*/
/**
   Set the primary channel set for the BDB energy scan. Beacon request
   will be send on these channels
   @param channel_mask - Channel mask.
*/
void zb_set_bdb_primary_channel_set(zb_uint32_t channel_mask);
/**
   Get the primary channel set for the BDB energy scan.
   @return channel_mask - Channel mask.
*/
zb_uint32_t zb_get_bdb_primary_channel_set(void);

/**
   Set the secondary channel set for the BDB energy scan. Beacon request
   will be send on these channels if no network found after energy
   scan on the primary channels.
   @param channel_mask - Channel mask.
*/
void zb_set_bdb_secondary_channel_set(zb_uint32_t channel_mask);

/**
   Get the secondary channel set for the BDB energy scan.
   @return channel_mask - Channel mask.
*/
zb_uint32_t zb_get_bdb_secondary_channel_set(void);

/**
 Enable Zigbee PRO complaint commissioning support
 @param state - 1 to disable trust center require key exchange; 0 -
 to enable.
*/
void zb_bdb_set_legacy_device_support(zb_uint8_t state);

/** @} */

/**
   @addtogroup zboss_bdb_comm_start
   @{
*/
/**
  * @brief Set commissioning mode.
  * @param  commissioning_mode - bitfield with the bdbCommissioningMode
  * attribute. Set 1 to the corresponding bit to enable, 0 to disable. 
  * All the possible mask bits are stored in @ref zb_bdb_commissioning_mode_mask_t
 */
void zb_set_bdb_commissioning_mode(zb_uint8_t commissioning_mode);

/** @} */

/**
   @addtogroup zboss_bdb_comm_params
   @{
*/
/**
 * Maximum endpoints of the "respondent" that can be served
 */
#define ZB_BDB_COMM_ACTIVE_ENDP_LIST_LEN 4

/**
 * Identify query responses queue size
 */
#define BDB_MAX_IDENTIFY_QUERY_RESP_NUMBER 4

/**
 * List of BDB commissioning states
 */
typedef enum zb_bdb_comm_state_e
{
  ZB_BDB_COMM_IDLE                       = 0,   /*!< EZ-Mode isn't invoked */
  ZB_BDB_COMM_FINDING_AND_BINDING        = 4,   /*!< EZ-Mode finding and binding in progress (on initiator) */
  ZB_BDB_COMM_FINDING_AND_BINDING_TARGET = 5,   /*!< EZ-Mode finding and binding in progress (on target) */
}
  zb_bdb_comm_state_t;

/** @cond internals_doc */
typedef ZB_PACKED_PRE struct zb_bdb_comm_respondent_info_s
{
  zb_address_ieee_ref_t addr_ref; /*!< Address reference in Translation Table for "respondent" */
  zb_uint8_t ep_list[ZB_BDB_COMM_ACTIVE_ENDP_LIST_LEN];  /*!< Respondent's active endpoints list */
#if defined(ZB_BDB_ENABLE_FINDING_BINDING)
  zb_uint8_t ep_cnt;   /*!< Count of endpoints received during finding and binding */
#endif

  zb_uint8_t eps_checked; /*!< Count of the currently checked endpoints */
} ZB_PACKED_STRUCT zb_bdb_comm_respondent_info_t;

typedef enum bdb_commissioning_signal_e
{
  BDB_COMM_SIGNAL_BAD,          /* 0 */

  BDB_COMM_SIGNAL_INIT_START,   /* 1 */
  BDB_COMM_SIGNAL_INIT_SECURE_REJOIN,
  BDB_COMM_SIGNAL_INIT_TC_REJOIN,
  BDB_COMM_SIGNAL_INIT_FINISH,

  BDB_COMM_SIGNAL_TOUCHLINK_START, /* 5 */
  BDB_COMM_SIGNAL_TOUCHLINK_INITIATOR_DONE,
  BDB_COMM_SIGNAL_TOUCHLINK_INITIATOR_FAILED,
  BDB_COMM_SIGNAL_TOUCHLINK_NOTIFY_TASK_RESULT,
  BDB_COMM_SIGNAL_TOUCHLINK_FINISH,

  BDB_COMM_SIGNAL_NETWORK_STEERING_START, /* 10 */
  BDB_COMM_SIGNAL_NETWORK_STEERING_DISCOVERY_FAILED,
  BDB_COMM_SIGNAL_NETWORK_STEERING_TCLK_EX_FAILURE,
  BDB_COMM_SIGNAL_NETWORK_STEERING_TCLK_DONE,
  /* TODO: BDB_COMM_SIGNAL_NETWORK_STEERING_LEAVE, */
  BDB_COMM_SIGNAL_NETWORK_STEERING_FINISH,

  BDB_COMM_SIGNAL_NETWORK_FORMATION_START, /* 15 */
  BDB_COMM_SIGNAL_NETWORK_FORMATION_FINISH,

  BDB_COMM_SIGNAL_FINDING_N_BINDING_START, /* 17 */
  BDB_COMM_SIGNAL_FINDING_N_BINDING_FINISH,

  BDB_COMM_SIGNAL_REJOIN_START, /* 19 */
  BDB_COMM_SIGNAL_REJOIN_TRY_SECURE_REJOIN_ON_CURRENT_CHANNEL,
  BDB_COMM_SIGNAL_REJOIN_TRY_TC_REJOIN_ON_CURRENT_CHANNEL,
  BDB_COMM_SIGNAL_REJOIN_TRY_TC_REJOIN_ON_ALL_CHANNELS,
  BDB_COMM_SIGNAL_REJOIN_TRY_SECURE_REJOIN_ON_ALL_CHANNELS,
  BDB_COMM_SIGNAL_REJOIN_FINISH,

  BDB_COMM_SIGNAL_FINISH, /* 25 */

  BDB_COMM_SIGNAL_NWK_FORMATION_OK, /* 26 */
  BDB_COMM_SIGNAL_NWK_START_ROUTER_CONF,
  BDB_COMM_SIGNAL_LEAVE_DONE,
  BDB_COMM_SIGNAL_NWK_JOIN_FAILED,
  BDB_COMM_SIGNAL_NWK_JOIN_DONE,
  BDB_COMM_SIGNAL_NWK_AUTH_FAILED,

  BDB_COMM_N_SIGNALS
} bdb_commissioning_signal_t;

enum bdb_commissioning_rejoin_reason_e
{
  BDB_COMM_REJOIN_REASON_UNSPECIFIED = 0,
  BDB_COMM_REJOIN_REASON_POLL_CONTROL_CHECK_IN,
};
/*!
* @ brief BDB Commissioning rejoin context
*/
typedef ZB_PACKED_PRE struct bdb_commissioning_rejoin_ctx_t
{
  zb_bitfield_t rejoin_by_checkin_failure:1;
  zb_bitfield_t rr_have_unique_tclk:1;
  zb_bitfield_t waiting:1;
  zb_bitfield_t reserved:5;

  zb_uint8_t rr_sv_device_type; /* zb_nwk_device_type_t */
  zb_uint16_t rr_sv_parent_short;
  zb_ieee_addr_t rr_sv_parent_long;
  zb_neighbor_tbl_ent_t rr_sv_parent_nent;
  zb_uint8_t rr_sv_authenticated;
  zb_uint8_t rr_retries;
  zb_uint8_t rr_ignore_start_router_conf;
  zb_uint16_t rr_global_retries;
  zb_uint8_t rr_skip_savepoint;

  bdb_commissioning_signal_t last_rejoin_signal;
  bdb_commissioning_signal_t next_rejoin_signal;
}
ZB_PACKED_STRUCT bdb_commissioning_rejoin_ctx_t;

/**
 * BDB commissioning context
 */
typedef ZB_PACKED_PRE struct zb_bdb_comm_ctx_s
{
  /** State of commissioning */
  zb_bdb_comm_state_t state;
  zb_ret_t status;
  /** Callback function invoked when factory fresh or network steering operations finish */
  zb_callback_t user_cb;

#if defined(ZB_BDB_ENABLE_FINDING_BINDING)
  /** Callback function invoked when binding operation start */
  zb_bdb_comm_binding_callback_t finding_binding_progress_cb;
/* [AV] After having separated binding user callbacks from f&b complete callback
    the last one can be stored in the user_cb field. */
#endif

  /** Duration of PermitJoining and IdentifyTime */
  zb_uint16_t duration;
/*Data array to store info from Identity_Query_Resp */
  zb_bdb_comm_respondent_info_t respondent[BDB_MAX_IDENTIFY_QUERY_RESP_NUMBER];
  zb_uint8_t respondent_number;

#if defined(ZB_BDB_ENABLE_FINDING_BINDING)
  /** Endpoint which participate in finding and binding*/
  zb_uint8_t ep;
#endif

  /** Endpoint descriptor*/
  zb_af_endpoint_desc_t *ep_desc;

#if defined(ZB_BDB_ENABLE_FINDING_BINDING)
  /** Count of endpoints received during finding and binding */
  zb_uint8_t ep_cnt;

  /** Signals that at least one endpoint was bound during finding and binding;
    * it is used to invoke user callback if no endpoint was bound
    */
  zb_bool_t was_bound;
#endif
  /*EZ mode in progress flag. Sets for EZScanTimeout*/
  zb_bool_t ez_scan_inprogress;
  /** Reference to the buffer holding simple descriptor response */
  zb_uint8_t simple_desc_buf_ref;

  /* ------------- */
  bdb_commissioning_signal_t signal;
  bdb_commissioning_rejoin_ctx_t rejoin;

  /* Moved here from BDB_CTX */
#define FIRST_GENERAL_BDB_FIELD bdb_commissioning_group_id
  /* BDB attributes */
  zb_uint16_t bdb_commissioning_group_id; /*!< specifies the identifier of the group on which the initiator applies finding & binding.  */
  zb_uint8_t bdb_commissioning_mode;      /*!< @see zb_bdb_commissioning_mode_mask_t See Table 4  Bits of the bdbCommissioningMode attribute:
0
Touchlink: 0 = Do not attempt Touchlink commissioning 1 = Attempt Touchlink commissioning
1
Network steering: 0 = Do not attempt network steering 1 = Attempt network steering
2
Network formation: 0 = Do not attempt to form a network 1 = Attempt to form a network, according to device type2
3
Finding & binding: 0 = Do not attempt finding & binding 1 = Attempt finding & binding
                                           */
  zb_uint8_t  bdb_commissioning_status; /*!< see zb_bdb_error_codes_e  */

  zb_uint32_t   bdb_primary_channel_set;
  zb_uint32_t   bdb_secondary_channel_set;
  zb_uint32_t   v_scan_channels;
  zb_uint32_t   bdb_commissioning_time;
  zb_uint8_t    bdb_scan_duration;
  zb_uint8_t    bdb_commissioning_step;

  zb_bitfield_t v_do_primary_scan:3; /*!< a bit more than vDoPromaryScan in
                                      * BDB: really scan & join machine
                                      * state. @see enum bdb_join_machine_e */

  zb_bitfield_t bdb_ext_channel_scan:1; /*!< Touchlink performs ext scan if 1 */
  zb_bitfield_t ignore_aps_channel_mask:1; /*!< Non standard, but useful: if 1,
                                            * use hard-coded channels set. if 0,
                                            * mask channels sets by
                                            * aps_channel_mask. To be used to
                                            * debug at single channel, or 2
                                            * channels etc */


  zb_uint8_t    bdb_application_signal;  /* Application signal code to be passed into
                                          * zb_zdo_startup_complete */
#ifdef ZB_BDB_TOUCHLINK
  zb_uint8_t    tl_first_channel_rpt;
  zb_uint8_t    tl_channel_i;
#endif  /* ZB_BDB_TOUCHLINK */
  zb_bitfield_t bdb_force_router_rejoin:1;      /* Force rejoin for the router */
  zb_bitfield_t nfn:1;                     /*!< if 1, device is not factory new and bdb init checks
                                            * are ok - no need to reinit commissioning */
  zb_bitfield_t host_bdb_mode:1;
  } ZB_PACKED_STRUCT zb_bdb_comm_ctx_t;
/** @endcond */ /* internals_doc */

/** @} */

#endif /* ZB_BDB_MODE*/


/** \addtogroup ZB_ZCL_INITIALIZATION
 *  @{
 */

typedef struct zcl_cluster_handlers_s
{
  zb_uint16_t cluster_id;
  zb_uint8_t cluster_role;
  zb_zcl_cluster_handler_t cluster_handler;
  zb_zcl_cluster_check_value_t cluster_check_value;
  zb_zcl_cluster_write_attr_hook_t cluster_write_attr_hook;
} zcl_cluster_handlers_t;

#define ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM 0xff

#define ZB_ZCL_CLUSTER_HANDLERS_TABLE_SIZE 20

typedef struct zb_discover_cmd_list
{
  zb_uint8_t received_cnt;
  zb_uint8_t *received;
  zb_uint8_t generated_cnt;
  zb_uint8_t *generated;
} zb_discover_cmd_list_t;

/** @cond internals_doc */
typedef struct zb_zcl_globals_s
{
  /** @internal ZCL sequence number used when send zcl frames */
  zb_uint8_t seq_number;
  /** @internal Buffer to store runtime data during ZCL commands processing */

//TODO: can be used for SHADE PROCESSING?
  zb_bufid_t runtime_buf;

  /** @internal Store device context information */
  zb_af_device_ctx_t *device_ctx;
  /** @internal Array for internal device ctx and user device ctx. To be used
   * by ZGP. */
  zb_af_device_ctx_t *device_ctx_arr[2];
  /** @internal ZCL transaction hash */
  zcl_cb_hash_ent_t zcl_cb[ZCL_TRAN_TABLE_SIZE];
  /** @internal User defined callback, used to inform user about attribute value  modification */
  zb_zcl_modify_attr_value_cb_t modify_attr_value_cb;

#if !(defined ZB_ZCL_DISABLE_REPORTING)
  /** @internal User defined callback, is used to inform user about received attribute report */
  zb_zcl_report_attr_cb_t report_attr_cb;
  /** @internal User defined callback, is used to inform user that report was not received during
    * defined time interval */
  zb_zcl_no_reporting_cb_t no_reporting_cb;
  /** @internal User defined API callback, is used to connection with User Application */
#endif

  zb_callback_t device_cb;
  zb_callback_t checkin_cb;

#if !(defined ZB_ZCL_DISABLE_REPORTING)
  /** @internal Stores context information for reporting */
  zb_zcl_reporting_ctx_t reporting_ctx;
#endif

  /** @internal Set default value all attributes */
  zb_zcl_set_default_value_attr_cb_t set_default_attr_cb;
  /** @internal Check attribute value */
  zb_zcl_app_check_attr_value_cb_t app_check_attr_value_cb;
#ifdef ZB_HA_ENABLE_OTA_UPGRADE_SERVER
  zb_zcl_ota_upgrade_next_data_ind_t ota_upgrade_next_data_ind_cb;
#endif /* ZB_HA_ENABLE_OTA_UPGRADE_SERVER */
#ifdef ZB_HA_ENABLE_OTA_UPGRADE_CLIENT
  zb_zcl_ota_upgrade_cli_ctx_t ota_cli;
#endif
#if defined ZB_BDB_MODE
  /** @internal BDB commissioning context */
  zb_bdb_comm_ctx_t bdb_comm_ctx;
#endif /*ZB_BDB_MODE*/

#if defined ZB_ENABLE_HA
  zb_zcl_disc_manuf_cmd_cb_t disc_manuf_cmd_cb;
#endif /* ZB_ENABLE_HA */

  zcl_cluster_handlers_t zcl_handlers[ZB_ZCL_CLUSTER_HANDLERS_TABLE_SIZE];
#ifdef ZB_CONTROL4_NETWORK_SUPPORT
  /* Control4 Network cluster specific handlers. Server and Client cluster roles */
  zcl_cluster_handlers_t control4_zcl_handlers[2];
#endif /* ZB_CONTROL4_NETWORK_SUPPORT */
  zb_uint8_t zcl_handlers_cnt;
  zb_discover_cmd_list_t *zb_zcl_cluster_cmd_list;
} zb_zcl_globals_t;

/**
   Returns ZCL context.
   @return pointer ZCL context
  */
zb_zcl_globals_t *zb_zcl_get_ctx(void);

/** @endcond */ /* internals_doc */

/** Returns pointer to ZCL context */
#define ZCL_CTX() (*zb_zcl_get_ctx())

/*! @} */ /* ZB_ZCL_INITIALIZATION */

#define ZB_ZCL_MAX_EP_NUMBER 10

typedef zb_uint8_t *(*zb_zcl_put_payload_cb_t)(zb_uint8_t *, const void *);

void zb_zcl_send_cmd(
  zb_uint8_t param,
  const zb_addr_u *dst_addr,
  zb_aps_addr_mode_t dst_addr_mode,
  zb_uint8_t dst_ep,
  zb_zcl_frame_direction_t direction,
  zb_uint8_t src_ep,
  const void *payload,
  zb_uint8_t payload_size,
  zb_zcl_put_payload_cb_t put_payload,
  zb_zcl_cluster_id_t cluster_id,
  zb_zcl_disable_default_response_t def_resp,
  zb_uint16_t cmd_id,
  zb_callback_t cb
);

void zb_zcl_send_cmd_tsn(
  zb_uint8_t param,
  const zb_addr_u *dst_addr,
  enum zb_aps_addr_mode_e dst_addr_mode,
  zb_uint8_t dst_ep,
  zb_zcl_frame_direction_t direction,
  zb_uint8_t src_ep,
  const void *payload,
  zb_uint8_t payload_size,
  zb_zcl_put_payload_cb_t put_payload,
  zb_zcl_cluster_id_t cluster_id,
  zb_zcl_disable_default_response_t def_resp,
  zb_uint16_t cmd_id,
  zb_uint8_t tsn,
  zb_callback_t cb
);

zb_bool_t zb_zcl_send_default_handler(zb_uint8_t param,
  const zb_zcl_parsed_hdr_t *cmd_info, zb_zcl_status_t status);
void zb_zcl_send_default_resp_ext(zb_uint8_t param,
  const zb_zcl_parsed_hdr_t *cmd_info, zb_zcl_status_t status);

void zb_zcl_set_cluster_encryption(zb_uint8_t endpoint_id, zb_uint16_t cluster_id, zb_uint8_t encrypt);

#endif /* ZBOSS_API_ZCL_H */
