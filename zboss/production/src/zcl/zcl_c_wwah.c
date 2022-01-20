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
/* PURPOSE: ZCL WWAH cluster specific commands handling - Client role
*/

#define ZB_TRACE_FILE_ID 12083

#include "zb_common.h"

#if defined ZB_ZCL_SUPPORT_CLUSTER_WWAH

#ifdef ZB_ZCL_ENABLE_WWAH_CLIENT

static const zb_uint8_t gs_wwah_client_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_WWAH_CLIENT_ROLE_RECEIVED_CMD_LIST
};

static const zb_uint8_t gs_wwah_client_generated_commands[] =
{
  ZB_ZCL_CLUSTER_ID_WWAH_CLIENT_ROLE_GENERATED_CMD_LIST
};

zb_discover_cmd_list_t gs_wwah_client_cmd_list =
{
  sizeof(gs_wwah_client_received_commands), (zb_uint8_t *)gs_wwah_client_received_commands,
  sizeof(gs_wwah_client_generated_commands), (zb_uint8_t *)gs_wwah_client_generated_commands
};

zb_bool_t zb_zcl_process_wwah_specific_commands_cli(zb_uint8_t param);

void zb_zcl_wwah_init_client()
{
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_init_client", (FMT__0));
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_WWAH,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_wwah_specific_commands_cli);
}

zb_bool_t zb_zcl_process_wwah_specific_commands_cli(zb_uint8_t param)
{
  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_wwah_client_cmd_list;
    return ZB_TRUE;
  }
  return ZB_FALSE;
}

#endif  /* ZB_ZCL_ENABLE_WWAH_CLIENT */

#endif /* defined ZB_ZCL_SUPPORT_CLUSTER_WWAH */
