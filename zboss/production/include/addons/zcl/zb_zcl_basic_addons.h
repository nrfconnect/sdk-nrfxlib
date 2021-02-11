/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef ZB_ZCL_BASIC_ADDONS_H__
#define ZB_ZCL_BASIC_ADDONS_H__

#include "zboss_api.h"

/*! \addtogroup zb_zcl_basic_addons */
/*! @{ */

/** @brief Basic cluster attributes according to ZCL Spec 3.2.2.2 */
typedef struct
{
    zb_uint8_t zcl_version;
    zb_uint8_t app_version;
    zb_uint8_t stack_version;
    zb_uint8_t hw_version;
    zb_char_t  mf_name[33];
    zb_char_t  model_id[33];
    zb_char_t  date_code[17];
    zb_uint8_t power_source;
    zb_char_t  location_id[17];
    zb_uint8_t ph_env;
    zb_char_t  sw_ver[17];
} zb_zcl_basic_attrs_ext_t;

/** @} */

#endif /* ZB_ZCL_BASIC_ADDONS_H__ */
