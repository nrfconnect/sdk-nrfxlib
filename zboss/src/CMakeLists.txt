#
# Copyright (c) 2021 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

# Create a library for parts of ZBOSS, distributed in source code.
# It will be pulled if the module links to the ZBOSS interface library.
zephyr_library_named(zboss-sources)

# Private ZBOSS API
zephyr_library_include_directories(
    include
  )

# Add source files

zephyr_library_sources(commissioning/legacy/zdo_commissioning_classic.c)

zephyr_library_sources(commissioning/bdb/bdb_finding_binding.c)
zephyr_library_sources(commissioning/bdb/zdo_commissioning_bdb.c)
zephyr_library_sources(commissioning/bdb/zdo_commissioning_bdb_formation.c)

zephyr_library_sources(zcl/ha_sas.c)
zephyr_library_sources(zcl/zcl_alarms_commands.c)
zephyr_library_sources(zcl/zcl_attr_value.c)
zephyr_library_sources(zcl/zcl_basic_commands.c)
zephyr_library_sources(zcl/zcl_binary_input.c)
zephyr_library_sources(zcl/zcl_c_daily_schedule.c)
zephyr_library_sources(zcl/zcl_c_drlc.c)
zephyr_library_sources(zcl/zcl_c_messaging.c)
zephyr_library_sources(zcl/zcl_c_metering.c)
zephyr_library_sources(zcl/zcl_c_price.c)
zephyr_library_sources(zcl/zcl_c_wwah.c)
zephyr_library_sources(zcl/zcl_color_control_commands.c)
zephyr_library_sources(zcl/zcl_common.c)
zephyr_library_sources(zcl/zcl_continuous_value_change_commands.c)
zephyr_library_sources(zcl/zcl_control4_networking.c)
zephyr_library_sources(zcl/zcl_dehumid_control.c)
zephyr_library_sources(zcl/zcl_diagnostics_commands.c)
zephyr_library_sources(zcl/zcl_door_lock.c)
zephyr_library_sources(zcl/zcl_el_measurement.c)
zephyr_library_sources(zcl/zcl_en50523_appliance_events_and_alerts.c)
zephyr_library_sources(zcl/zcl_fan_control.c)
zephyr_library_sources(zcl/zcl_general_commands.c)
zephyr_library_sources(zcl/zcl_groups.c)
zephyr_library_sources(zcl/zcl_ias_ace_commands.c)
zephyr_library_sources(zcl/zcl_ias_wd_commands.c)
zephyr_library_sources(zcl/zcl_ias_zone_commands.c)
zephyr_library_sources(zcl/zcl_identify_commands.c)
zephyr_library_sources(zcl/zcl_illuminance_measurement.c)
zephyr_library_sources(zcl/zcl_level_control_commands.c)
zephyr_library_sources(zcl/zcl_main.c)
zephyr_library_sources(zcl/zcl_meter_identification.c)
zephyr_library_sources(zcl/zcl_nvram.c)
zephyr_library_sources(zcl/zcl_occupancy_sensing_commands.c)
zephyr_library_sources(zcl/zcl_on_off_commands.c)
zephyr_library_sources(zcl/zcl_on_off_switch_config.c)
zephyr_library_sources(zcl/zcl_ota_upgrade_commands.c)
zephyr_library_sources(zcl/zcl_ota_upgrade_common.c)
zephyr_library_sources(zcl/zcl_ota_upgrade_minimal.c)
zephyr_library_sources(zcl/zcl_ota_upgrade_srv_commands.c)
zephyr_library_sources(zcl/zcl_poll_control_client.c)
zephyr_library_sources(zcl/zcl_poll_control_commands.c)
zephyr_library_sources(zcl/zcl_power_config_commands.c)
zephyr_library_sources(zcl/zcl_pressure_measurement.c)
zephyr_library_sources(zcl/zcl_rel_humidity.c)
zephyr_library_sources(zcl/zcl_reporting.c)
zephyr_library_sources(zcl/zcl_s_daily_schedule.c)
zephyr_library_sources(zcl/zcl_s_drlc.c)
zephyr_library_sources(zcl/zcl_s_messaging.c)
zephyr_library_sources(zcl/zcl_s_metering.c)
zephyr_library_sources(zcl/zcl_s_price.c)
zephyr_library_sources(zcl/zcl_s_wwah.c)
zephyr_library_sources(zcl/zcl_scenes.c)
zephyr_library_sources(zcl/zcl_shade_config_commands.c)
zephyr_library_sources(zcl/zcl_temp_measurement.c)
zephyr_library_sources(zcl/zcl_thermostat_commands.c)
zephyr_library_sources(zcl/zcl_thermostat_ui_config.c)
zephyr_library_sources(zcl/zcl_time.c)
zephyr_library_sources(zcl/zcl_window_covering.c)
zephyr_library_sources(zcl/zcl_wwah_common.c)


# Link with ZBOSS interface library.
zephyr_library_link_libraries(zboss)
