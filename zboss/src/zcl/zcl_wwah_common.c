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
/* PURPOSE: ZCL WWAH cluster specific commands handling
*/

#define ZB_TRACE_FILE_ID 12082

#include "zb_common.h"

#if (defined ZB_ZCL_ENABLE_WWAH_SERVER || defined ZB_ZCL_ENABLE_WWAH_CLIENT)

void zb_zcl_wwah_set_wwah_behavior(zb_uint8_t behavior)
{
  if (behavior < ZB_ZCL_WWAH_BEHAVIOR_RESERVED)
  {
    WWAH_CTX().wwah_behavior = behavior;
  }
}

/*
 * Check if WWAH client (TC) behaviour is enabled
 * @return ZB_TRUE if WWAH client (TC) behaviour is enabled and WWAH client endpoint is exist
 *         ZB_FALSE otherwise */

zb_bool_t zb_is_wwah_client()
{
  zb_uint8_t wwah_endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_WWAH,
                                                     ZB_ZCL_CLUSTER_CLIENT_ROLE);
  return (zb_bool_t)(((zb_zcl_wwah_behavior_t)WWAH_CTX().wwah_behavior == ZB_ZCL_WWAH_BEHAVIOR_CLIENT) && wwah_endpoint);
}

/*
 * Check if WWAH server behaviour is enabled
 * @return ZB_TRUE if WWAH server behaviour is enabled and WWAH server endpoint is exist
 *         ZB_FALSE otherwise */

zb_bool_t zb_is_wwah_server()
{
  zb_uint8_t wwah_endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_WWAH,
                                                     ZB_ZCL_CLUSTER_SERVER_ROLE);
  return (zb_bool_t)(((zb_zcl_wwah_behavior_t)WWAH_CTX().wwah_behavior == ZB_ZCL_WWAH_BEHAVIOR_SERVER) && wwah_endpoint);
}

#endif
