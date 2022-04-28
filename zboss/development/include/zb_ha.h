/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2022 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: HA global definitions
*/

#ifndef ZB_HA_H
#define ZB_HA_H 1

#ifdef ZB_ENABLE_HA
#include "ha/zb_ha_config.h"

#ifdef ZB_ENABLE_HA_SAS
#include "ha/zb_ha_sas.h"
#endif

#ifdef ZB_HA_DEFINE_DEVICE_DOOR_LOCK
#include "ha/zb_ha_door_lock.h"
#endif
#ifdef ZB_HA_DEFINE_DEVICE_DOOR_LOCK_CONTROLLER
#include "ha/zb_ha_door_lock_controller.h"
#endif
#ifdef ZB_HA_DEFINE_DEVICE_ON_OFF_OUTPUT
#include "ha/zb_ha_on_off_output.h"
#endif
#ifdef ZB_HA_DEFINE_DEVICE_ON_OFF_SWITCH
#include "ha/zb_ha_on_off_switch.h"
#endif
#ifdef ZB_HA_DEFINE_DEVICE_SIMPLE_SENSOR
#include "ha/zb_ha_simple_sensor.h"
#endif
#ifdef ZB_HA_DEFINE_DEVICE_COMBINED_INTERFACE
#include "ha/zb_ha_combined_interface.h"
#endif
#ifdef ZB_HA_DEFINE_DEVICE_SCENE_SELECTOR
#include "ha/zb_ha_scene_selector.h"
#endif
#ifdef ZB_HA_DEFINE_DEVICE_CONFIGURATION_TOOL
#include "ha/zb_ha_configuration_tool.h"
#endif
#ifdef ZB_HA_DEFINE_DEVICE_MAINS_POWER_OUTLET
#include "ha/zb_ha_mains_power_outlet.h"
#endif
#ifdef ZB_HA_DEFINE_DEVICE_RANGE_EXTENDER
#include "ha/zb_ha_range_extender.h"
#endif
#ifdef ZB_HA_DEFINE_DEVICE_LEVEL_CONTROL_SWITCH
#include "ha/zb_ha_level_control_switch.h"
#endif
#ifdef ZB_HA_DEFINE_DEVICE_LEVEL_CONTROLLABLE_OUTPUT
#include "ha/zb_ha_level_controllable_output.h"
#endif
#ifdef ZB_HA_DEFINE_DEVICE_WINDOW_COVERING
#include "ha/zb_ha_window_covering.h"
#endif
#ifdef ZB_HA_DEFINE_DEVICE_WINDOW_COVERING_CONTROLLER
#include "ha/zb_ha_window_covering_controller.h"
#endif
#ifdef ZB_HA_DEFINE_DEVICE_SHADE
#include "ha/zb_ha_shade.h"
#endif
#ifdef ZB_HA_DEFINE_DEVICE_SHADE_CONTROLLER
#include "ha/zb_ha_shade_controller.h"
#endif

#ifdef ZB_HA_DEFINE_DEVICE_TEMPERATURE_SENSOR
#include "ha/zb_ha_temperature_sensor.h"
#endif

#ifdef ZB_HA_DEFINE_DEVICE_IAS_CONTROL_INDICATING_EQUIPMENT
#include "ha/zb_ha_ias_control_indicating_equipment.h"
#endif
#ifdef ZB_HA_DEFINE_DEVICE_IAS_ANCILLARY_CONTROL_EQUIPMENT
#include "ha/zb_ha_ias_ancillary_control_equipment.h"
#endif
#ifdef ZB_HA_DEFINE_DEVICE_IAS_ZONE
#include "ha/zb_ha_ias_zone.h"
#endif
#ifdef ZB_HA_DEFINE_DEVICE_IAS_WARNING_DEVICE
#include "ha/zb_ha_ias_warning_device.h"
#endif

#ifdef ZB_HA_DEFINE_DEVICE_CUSTOM_ATTR
#include "ha/zb_ha_custom_attr.h"
#endif
#ifdef ZB_HA_DEFINE_DEVICE_DIMMABLE_LIGHT
#include "ha/zb_ha_dimmable_light.h"
#endif
#ifdef ZB_HA_DEFINE_DEVICE_DIMMER_SWITCH
#include "ha/zb_ha_dimmer_switch.h"
#endif
#ifdef ZB_HA_DEFINE_DEVICE_SMART_PLUG
#include "ha/zb_ha_smart_plug.h"
#endif
#ifdef ZB_HA_DEFINE_DEVICE_THERMOSTAT
#include "ha/zb_ha_thermostat.h"
#endif

#ifdef ZB_HA_DEFINE_DEVICE_TEST_DEVICE
#include "ha/zb_ha_test_device.h"
#endif

#ifdef ZB_HA_DEFINE_DEVICE_ERL_INTERFACE_DEVICE
#include "ha/zb_ha_erl_device_interface.h"
#endif
#ifdef ZB_HA_DEFINE_DEVICE_ERL_GW
#include "ha/zb_ha_erl_gw_device.h"
#endif
#endif
#endif /* ZB_HA_H */
