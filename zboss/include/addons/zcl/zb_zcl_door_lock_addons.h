/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef ZB_ZCL_DOOR_LOCK_ADDONS_H__
#define ZB_ZCL_DOOR_LOCK_ADDONS_H__

#include "zboss_api.h"

/*! \addtogroup zb_zcl_door_lock_addons */
/*! @{ */

/**@brief Door Lock cluster attributes according to ZCL Spec 7.3.2.10. */
typedef struct
{
    zb_uint8_t lock_state;
    zb_uint8_t lock_type;
    zb_bool_t  actuator_enabled;
} zb_zcl_door_lock_attrs_t;

/** @} */

#endif /* ZB_ZCL_DOOR_LOCK_ADDONS_H__ */
