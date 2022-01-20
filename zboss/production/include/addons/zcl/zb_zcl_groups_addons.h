/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef ZB_ZCL_GROUPS_ADDONS_H__
#define ZB_ZCL_GROUPS_ADDONS_H__

#include "zboss_api.h"

/*! \addtogroup zb_zcl_groups_addons */
/*! @{ */

/**@brief Groups cluster attributes according to ZCL Spec 3.6.2.2. */
typedef struct
{
    zb_uint8_t name_support;
} zb_zcl_groups_attrs_t;

/** @} */

#endif /* ZB_ZCL_GROUPS_ADDONS_H__ */
