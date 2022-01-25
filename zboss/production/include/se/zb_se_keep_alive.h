/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2020 DSR Corporation, Denver CO, USA.
 * www.dsr-zboss.com
 * www.dsr-corporation.com
 * All rights reserved.
 *
 *
 * Use in source and binary forms, redistribution in binary form only, with
 * or without modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 2. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 3. This software, with or without modification, must only be used with a Nordic
 *    Semiconductor ASA integrated circuit.
 *
 * 4. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/* PURPOSE: Definitions of SE's Keep-Alive cluster (SE spec, Annex A.3)
*/

#ifndef ZB_SE_KEEP_ALIVE_H
#define ZB_SE_KEEP_ALIVE_H 1

#include "zcl/zb_zcl_common.h"
#include "zcl/zb_zcl_commands.h"

/** @cond DOXYGEN_SE_SECTION */

/** @addtogroup ZB_ZCL_KEEP_ALIVE
 *  @{
 *    @details
 *    This cluster supports the commands and attributes directed to the network Trust Center in
 *    order to determine whether communication with the Trust Center is still available.
 *
 */

/* Cluster ZB_ZCL_CLUSTER_ID_KEEP_ALIVE */

/*! @name Keep-Alive cluster attributes
    @{
*/

/*! @brief Keep-Alive cluster attribute identifiers
    @see SE spec, subclause A.3.2
*/
enum zb_zcl_keep_alive_attr_e
{
  /*! @brief TC Keep-Alive Base attribute */
  ZB_ZCL_ATTR_KEEP_ALIVE_TC_KEEP_ALIVE_BASE_ID   = 0,
  /*! @brief TC Keep-Alive Jitter attribute */
  ZB_ZCL_ATTR_KEEP_ALIVE_TC_KEEP_ALIVE_JITTER_ID = 1
};

/** @brief Default value for TC Keep-Alive Base attribute */
#define ZB_ZCL_ATTR_KEEP_ALIVE_TC_KEEP_ALIVE_BASE_DEFAULT_VALUE   0x0a
/** @brief Default value for TC Keep-Alive Jitter attribute */
#define ZB_ZCL_ATTR_KEEP_ALIVE_TC_KEEP_ALIVE_JITTER_DEFAULT_VALUE 0x012c

/*! @} */ /* Keep-Alive cluster attributes */

/*! @name Keep-Alive cluster commands
    @{
*/
/*! @} */ /* Keep-Alive cluster commands */

/*! @cond internals_doc
    @internal @name Keep-Alive cluster internals
    Internal structures for attribute representation in cluster definitions.
    @{
*/

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_KEEP_ALIVE_TC_KEEP_ALIVE_BASE_ID(data_ptr)   \
{                                                                                      \
  ZB_ZCL_ATTR_KEEP_ALIVE_TC_KEEP_ALIVE_BASE_ID,                                         \
  ZB_ZCL_ATTR_TYPE_U8,                                                                 \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                        \
  (void*) data_ptr                                                                \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_KEEP_ALIVE_TC_KEEP_ALIVE_JITTER_ID(data_ptr) \
{                                                                                      \
  ZB_ZCL_ATTR_KEEP_ALIVE_TC_KEEP_ALIVE_JITTER_ID,                                       \
  ZB_ZCL_ATTR_TYPE_U16,                                                                \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                        \
  (void*) data_ptr                                                                \
}

/** @internal @brief Declare attribute list for Keep-Alive cluster
    @param attr_list - attribute list name
    @param base - pointer to variable to store TC Keep-Alive Base attribute value
    @param jitter - pointer to variable to store TC Keep-Alive Jitter attribute value
*/
#define ZB_ZCL_DECLARE_KEEP_ALIVE_ATTR_LIST_FULL(attr_list, base, jitter)          \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_KEEP_ALIVE_TC_KEEP_ALIVE_BASE_ID, (base))     \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_KEEP_ALIVE_TC_KEEP_ALIVE_JITTER_ID, (jitter)) \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*! @internal Number of attributes mandatory for reporting in Keep-Alive cluster */
#define ZB_ZCL_KEEP_ALIVE_REPORT_ATTR_COUNT 0

/*! @}
    @endcond */ /* Keep-Alive cluster internals */


/**
 *  @brief Keep-Alive cluster attributes
 */
typedef struct zb_zcl_keep_alive_attrs_s
{

  /** @copydoc ZB_ZCL_ATTR_KEEP_ALIVE_TC_KEEP_ALIVE_BASE_ID
   * @see ZB_ZCL_ATTR_KEEP_ALIVE_TC_KEEP_ALIVE_BASE_ID
   */
  zb_uint8_t base;

  /** @copydoc ZB_ZCL_ATTR_KEEP_ALIVE_TC_KEEP_ALIVE_JITTER_ID
   * @see ZB_ZCL_ATTR_KEEP_ALIVE_TC_KEEP_ALIVE_JITTER_ID
   */
  zb_uint16_t jitter;
} zb_zcl_keep_alive_attrs_t;


/** @brief Declare attribute list for Keep-Alive cluster cluster
 *  @param[in]  attr_list - attribute list variable name
 *  @param[in]  attrs - pointer to @ref zb_zcl_keep_alive_attrs_s structure
 */
#define ZB_ZCL_DECLARE_KEEP_ALIVE_ATTR_LIST(attr_list, attrs) \
    ZB_ZCL_DECLARE_KEEP_ALIVE_ATTR_LIST_FULL(attr_list, &attrs.base, &attrs.jitter )


/*! @} */ /* SE Keep-Alive cluster definitions */

/** @endcond */ /* DOXYGEN_SE_SECTION */

#define ZB_ZCL_CLUSTER_ID_KEEP_ALIVE_SERVER_ROLE_INIT (zb_zcl_cluster_init_t)NULL
#define ZB_ZCL_CLUSTER_ID_KEEP_ALIVE_CLIENT_ROLE_INIT (zb_zcl_cluster_init_t)NULL

#endif /* ZB_SE_KEEP_ALIVE_H */
