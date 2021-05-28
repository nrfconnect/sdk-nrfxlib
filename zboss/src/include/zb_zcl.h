/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2021 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: Zigbee cluster library
*/
#ifndef ZB_ZCL_H
#define ZB_ZCL_H 1

#include "zboss_api_zcl.h"
#ifndef ZB_ZCL_DISABLE_REPORTING
#include "zcl/zb_zcl_reporting.h"
#endif

#ifdef ZB_ZCL_SUPPORT_CLUSTER_WWAH
#define WWAH_CTX() ZCL_CTX().wwah_ctx
#endif /* ZB_ZCL_SUPPORT_CLUSTER_WWAH */


/** @cond internals_doc */

/** @addtogroup zcl_api
 * @{
 */

/** @brief ZCL command handler
  * @param param - reference to the buffer with packet.
  */
void zb_zcl_process_device_command(zb_uint8_t param);


/** @brief Parsed ZCL command handler
  * @param param - reference to the buffer with parsed ZCL header
  *                of type @ref zb_zcl_parsed_hdr_t stored as buffer parameter.
  */
void zb_zcl_process_parsed_zcl_cmd(zb_uint8_t param);
/********************** ZCL main functions declaration ******************************/
/** @brief Initialize Zigbee cluster library.  */
void zb_zcl_init(void);

/** @brief Deinitialize Zigbee cluster library.  */
void zb_zcl_deinit(void);

/** @brief ZCL main loop. */
void zcl_main_loop(void);

/** @brief Parse ZCL header.
  * @param param - reference to the buffer containing ZCL packet.
  * @param cmd_info - pointer to the resulting parsed header representation (in the buffer).
  * @return - header not contain error
  */
zb_zcl_status_t zb_zcl_parse_header(zb_uint8_t param, zb_zcl_parsed_hdr_t *cmd_info);

#ifndef ZB_ZCL_DISABLE_REPORTING
void zb_zcl_put_default_reporting_info_for_cluster(zb_uint8_t endpoint, zb_uint16_t profile_id, struct zb_zcl_cluster_desc_s *cluster_desc);

void zb_zcl_mark_report_not_sent(zb_zcl_reporting_info_t *rep_info);

void zb_zcl_update_reporting_info(zb_zcl_reporting_info_t *rep_info);

void zb_zcl_adjust_reporting_timer(zb_uint8_t param);
#endif  /* ZB_ZCL_DISABLE_REPORTING */

/** Internal functions for gettings ZCL cluster handlers */
zb_zcl_cluster_handler_t zb_zcl_get_cluster_handler_internal(zb_uint8_t endpoint, zb_uint16_t cluster_id, zb_uint8_t cluster_role);
zb_zcl_cluster_check_value_t zb_zcl_internal_get_cluster_check_value(zb_uint8_t endpoint, zb_uint16_t cluster_id, zb_uint8_t cluster_role);
zb_zcl_cluster_write_attr_hook_t zb_zcl_internal_get_cluster_write_attr_hook(zb_uint8_t endpoint, zb_uint16_t cluster_id, zb_uint8_t cluster_role);

/** @endcond */ /* internals_doc */

/*! @} */ /* zcl_api */

#endif /* ZB_ZCL_H */
