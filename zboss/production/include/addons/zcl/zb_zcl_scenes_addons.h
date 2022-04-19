/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef ZB_ZCL_SCENES_ADDONS_H__
#define ZB_ZCL_SCENES_ADDONS_H__

#include "zboss_api.h"

/*! \addtogroup zb_zcl_scenes_addons */
/*! @{ */

/* Scene cluster attributes according to ZCL Spec 3.7.2.2. */
typedef struct
{
    zb_uint8_t  scene_count;
    zb_uint8_t  current_scene;
    zb_uint8_t  scene_valid;
    zb_uint8_t  name_support;
    zb_uint16_t current_group;
} zb_zcl_scenes_attrs_t;

/**
 *  @brief Declare attribute list for Scenes cluster (client).
 *  @param attr_list - attribute list name.
 */
#define ZB_ZCL_DECLARE_SCENES_CLIENT_ATTRIB_LIST(attr_list)                     \
    ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attr_list, ZB_ZCL_SCENES) \
    ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/** @} */

#endif /* ZB_ZCL_SCENES_ADDONS_H__ */
