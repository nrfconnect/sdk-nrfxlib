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
/* PURPOSE: Zigbee cluster library, Startup Attribute Sets
*/

#define ZB_TRACE_FILE_ID 2058

#include "zb_common.h"

#if defined ZB_ENABLE_ZCL

#if defined ZB_ENABLE_HA_SAS

#include "zb_zcl.h"
#include "ha/zb_ha_sas.h"

static zb_ieee_addr_t g_zb_ha_sas_default_ext_pan_id = ZB_HA_SAS_EXT_PAN_ID;

void zb_ha_process_sas(void)
{
  TRACE_MSG(TRACE_ZCL1, "> zb_ha_process_sas", (FMT__0));

  /* Our stack had already set protocol version as appropriate (see
     ZB_PROTOCOL_VERSION) */
  /* Our stack had already set stack profile version, see ZB_STACK_PROFILE */
  /* Startup control parameter is an indicator one, not implemented */
  /* Config_Rejoin_Interval and Config_Max_Rejoin_Interval attributes are
     optional, so not supported */
  /* Master, network, and default trust center link keys are not set because
     they are same as stack defaults. Also this point should be controlled for
     possible changes in the future. */
  if (!ZB_JOINED())
  {
    ZB_PIBCACHE_NETWORK_ADDRESS() = ZB_HA_SAS_SHORT_ADDRESS;
    ZB_PIBCACHE_PAN_ID() = ZB_HA_SAS_PAN_ID;
    ZB_EXTPANID_COPY(ZB_NIB_EXT_PAN_ID(), g_zb_ha_sas_default_ext_pan_id);
#ifndef DEBUG
    zb_aib_channel_page_list_set_2_4GHz_mask(ZB_HA_SAS_CHANNEL_MASK); /* MMDEVSTUBS */
#endif
    ZB_IEEE_ADDR_ZERO(ZB_AIB().trust_center_address);
  }
  COMM_CTX().discovery_ctx.nwk_scan_attempts = ZB_HA_SAS_SCAN_ATTEMPTS;
  COMM_CTX().discovery_ctx.nwk_time_btwn_scans = ZB_HA_SAS_TIME_BTWN_SCANS;

  TRACE_MSG(TRACE_ZCL1, "< zb_ha_process_sas", (FMT__0));
}

#endif /* defined ZB_ENABLE_HA_SAS */

#endif /* defined ZB_ENABLE_ZCL */
