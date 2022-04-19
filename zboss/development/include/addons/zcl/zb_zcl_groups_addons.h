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

/**
 *  @brief Declare attribute list for Groups cluster (client).
 *  @param attr_list - attribute list name.
 */
#define ZB_ZCL_DECLARE_GROUPS_CLIENT_ATTRIB_LIST(attr_list)                   \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attr_list, ZB_ZCL_GROUPS) \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/** @} */

#endif /* ZB_ZCL_GROUPS_ADDONS_H__ */
