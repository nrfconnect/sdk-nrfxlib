/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#ifndef ZB_ZCL_THERMOSTAT_ADDONS_H__
#define ZB_ZCL_THERMOSTAT_ADDONS_H__

#include "zboss_api.h"

/*! \addtogroup zb_zcl_thermostat_addons */
/*! @{ */

/**@brief Thermostat Information cluster attributes according to ZCL Spec 6.3.2.2.1. */
typedef struct {
    zb_int16_t local_temperature;
    zb_int16_t outdoor_temperature;
    zb_uint8_t occupancy;
    zb_int16_t abs_min_heat_setpoint_imit;
    zb_int16_t abs_max_heat_setpoint_limit;
    zb_int16_t abs_min_cool_setpoint_limit;
    zb_int16_t abs_max_cool_setpoint_limit;
    zb_int16_t PI_cooling_demand;
    zb_int16_t PI_heating_demand;
    zb_uint8_t HVAC_system_type_configuration;
} zb_zcl_thermostat_info_attrs_t;

/**@brief Thermostat Settings cluster attributes according to ZCL Spec 6.3.2.2.2. */
typedef struct {
    zb_int8_t local_temperature_calibration;
    zb_int16_t occupied_cooling_setpoint;
    zb_int16_t occupied_heating_setpoint;
    zb_int16_t unoccupied_cooling_setpoint;
    zb_int16_t unoccupied_heating_setpoint;
    zb_int16_t min_heat_setpoint_limit;
    zb_int16_t max_heat_setpoint_limit;
    zb_int16_t min_cool_setpoint_limit;
    zb_int16_t max_cool_setpoint_limit;
    zb_int8_t min_setpoint_dead_band;
    zb_uint8_t remote_sensing;
    zb_uint8_t control_sequence_of_operation;
    zb_uint8_t system_mode;
    zb_uint8_t alarm_mask;
    zb_uint8_t thermostat_running_mode;
} zb_zcl_thermostat_settings_attrs_t;

/**@brief Thermostat Thermostat Schedule & HVAC Relay
   cluster attributes according to ZCL Spec 6.3.2.2.2. */
typedef struct {
    zb_uint8_t start_of_week;
    zb_uint8_t number_of_weekly_transitions;
    zb_uint8_t number_of_daily_transitions;
    zb_uint8_t temperature_setpoint_hold;
    zb_uint16_t temperature_setpoint_hold_ouration;
    zb_uint8_t thermostat_programming_operation_mode;
    zb_int16_t thermostat_running_state;
} zb_zcl_thermostat_schedule_and_HVAC_attrs_t;

/** @} */

#endif /* ZB_ZCL_THERMOSTAT_ADDONS_H__ */
