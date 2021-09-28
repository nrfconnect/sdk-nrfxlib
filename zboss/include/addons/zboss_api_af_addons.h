/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef ZBOSS_API_AF_ADDONS_H__
#define ZBOSS_API_AF_ADDONS_H__

#include "zboss_api.h"

/*! \addtogroup zboss_api_af_addons */
/*! @{ */

/**@brief Redefinition of __CAT__ as variadic macro.
 *
 * @param[in]  a   Mandatory argument to concatenate.
 * @param[in]  b   Mandatory argument to concatenate.
 * @param[in]  ... Optional argument to concatenate.
 */
#define __CAT_VA__(a, b, ...)                               a## __VA_ARGS__## b

/**@brief Redefinition of ZB_AF_SIMPLE_DESC_TYPE as variadic macro.
 *
 * @param[in]  in_num   Number of input clusters.
 * @param[in]  out_num  Number of output clusters.
 * @param[in]  ...      Optional argument to concatenate to the type name.
 */
#define ZB_AF_SIMPLE_DESC_TYPE_VA(in_num, out_num, ...)     __CAT_VA__(zb_af_simple_desc_, _t, __VA_ARGS__)

/**@brief Redefinition of ZB_DECLARE_SIMPLE_DESC as variadic macro.
 *
 * @param[in]  in_clusters_count   Number of input clusters.
 * @param[in]  out_clusters_count  Number of output clusters.
 * @param[in]  ...                 Optional argument to concatenate to the type name.
 */
#define ZB_DECLARE_SIMPLE_DESC_VA(in_clusters_count, out_clusters_count, ...)             \
  typedef ZB_PACKED_PRE struct zb_af_simple_desc_## __VA_ARGS__## _s                      \
  {                                                                                       \
    zb_uint8_t    endpoint;                 /* Endpoint */                                \
    zb_uint16_t   app_profile_id;           /* Application profile identifier */          \
    zb_uint16_t   app_device_id;            /* Application device identifier */           \
    zb_bitfield_t app_device_version:4;     /* Application device version */              \
    zb_bitfield_t reserved:4;               /* Reserved */                                \
    zb_uint8_t    app_input_cluster_count;  /* Application input cluster count */         \
    zb_uint8_t    app_output_cluster_count; /* Application output cluster count */        \
    /* Application input and output cluster list */                                       \
    zb_uint16_t   app_cluster_list[(in_clusters_count) + (out_clusters_count)];           \
    zb_uint8_t    cluster_encryption[((in_clusters_count) + (out_clusters_count) + 7)/8]; \
  } ZB_PACKED_STRUCT zb_af_simple_desc_## __VA_ARGS__## _t

/**@brief Redefinition of ZBOSS_DECLARE_DEVICE_CTX_N_EP as variadic macro.
 *
 * @param[in] device_ctx_name  Device context variable name.
 * @param[in] ...              Variables holding context for endpoints.
 */
#define ZBOSS_DECLARE_DEVICE_CTX_EP_VA(device_ctx_name, ...)            \
  zb_af_endpoint_desc_t *ep_list_##device_ctx_name[] = {                \
    __VA_ARGS__                                                         \
  };                                                                    \
  ZBOSS_DECLARE_DEVICE_CTX(device_ctx_name, ep_list_##device_ctx_name,  \
                           (ZB_ZCL_ARRAY_SIZE(ep_list_##device_ctx_name, zb_af_endpoint_desc_t*)))



/** @} */
                           
#endif /* ZBOSS_API_AF_ADDONS_H__ */
