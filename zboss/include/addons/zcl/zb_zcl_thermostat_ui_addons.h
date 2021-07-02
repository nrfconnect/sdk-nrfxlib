/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#ifndef ZB_ZCL_THERMOSTAT_UI_ADDONS_H__
#define ZB_ZCL_THERMOSTAT_UI_ADDONS_H__

#include "zboss_api.h"

/*! \addtogroup zb_zcl_thermostat_ui_addons */
/*! @{ */

/**@brief Thermostat UI cluster attributes according to ZCL Spec 6.6.2.2 */

typedef struct {
    uint8_t temperature_display_mode;
    uint8_t keypad_lockout;
    uint8_t schedule_programming_visibility;
} zb_zcl_thermostat_ui_attrs_t;

/** @} */

#endif /* ZB_ZCL_THERMOSTAT_UI_ADDONS_H__ */
