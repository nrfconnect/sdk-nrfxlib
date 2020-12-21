/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#ifndef ZB_ZCL_FAN_ADDONS_H__
#define ZB_ZCL_FAN_ADDONS_H__

#include "zboss_api.h"

/*! \addtogroup zb_zcl_fan_addons */
/*! @{ */

/**@brief Fan cluster attributes according to ZCL Spec 6.4.2.2 */
typedef struct {
    uint8_t fan_mode;
    uint8_t fan_mode_sequence;
} zb_zcl_fan_attrs_t;

/** @} */

#endif /* ZB_ZCL_FAN_ADDONS_H__ */
