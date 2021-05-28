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
/* PURPOSE: Provides example of matching table declaration for ZGPS sink
*/

#ifndef ZB_ZGP_DEFAULT_MATCH_INFO_H
#define ZB_ZGP_DEFAULT_MATCH_INFO_H 1

#include "zb_common.h"

#ifdef ZB_ENABLE_ZGP_SINK
#include "zboss_api_zgp.h"

/** 
 * @ingroup zgp_sink
 * @file zb_zgp_default_match_info.h
 * Provides example of matching table declaration for ZGPS sink
 * @include zb_zgp_default_match_info.h
 */

/**
 * Default matching table.
 */
zgps_dev_match_rec_t g_zgps_match_tbl[] =
{
#ifdef ZGPS_MATCH_TBL_TEMPERATURE_SENSOR_ENTRY
  {
    ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT,
    ZB_ZCL_CLUSTER_CLIENT_ROLE,
    {ZB_GPDF_CMD_ATTR_REPORT, ZB_GPDF_CMD_MULTI_CLUSTER_ATTR_REPORT, 0},
    ZB_ZGP_TEMPERATURE_SENSOR_DEV_ID,
  },
#endif

};


/**
 * Mapping of ZGPD commands IDs to Zigbee ZCL commands IDs
 */
zgp_to_zb_cmd_mapping_t g_cmd_mapping[] =
{
#if defined ZGPS_MATCH_TBL_TEMPERATURE_SENSOR_ENTRY
    {ZB_GPDF_CMD_ATTR_REPORT, ZB_ZCL_CMD_REPORT_ATTRIB},
#endif
};


/**
 * Default matching info
 */
zb_zgps_match_info_t  g_default_zgps_match_info =
{
  ZB_ARRAY_SIZE(g_zgps_match_tbl),
  g_zgps_match_tbl,
  ZB_ARRAY_SIZE(g_cmd_mapping),
  g_cmd_mapping,
  0,
  NULL
};
#endif  /* ZB_ENABLE_ZGP_SINK */
#endif //ZB_ZGP_DEFAULT_MATCH_INFO_H
