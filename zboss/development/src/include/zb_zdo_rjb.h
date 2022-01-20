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
/*  PURPOSE: Rejoin backoff
*/

#ifndef ZB_ZDO_REJOIN_BACKOFF_H
#define ZB_ZDO_REJOIN_BACKOFF_H 1

#ifdef ZB_REJOIN_BACKOFF

#define ZB_ZDO_REJOIN_BACKOFF_MAX_TIMEOUT  (30U*60U * ZB_TIME_ONE_SECOND) /* 30 minutes */
#define ZB_ZDO_REJOIN_BACKOFF_MAX_COUNT 11U
#define ZB_ZDO_REJOIN_BACKOFF_TIMEOUT(_cnt) (((_cnt) == 11U) ? ZB_ZDO_REJOIN_BACKOFF_MAX_TIMEOUT : (1U << (_cnt)) * ZB_TIME_ONE_SECOND) /* timeout value: 2, 4, 8, ... */

enum zb_zdo_rejoin_backoff_state_e
{
  ZB_ZDO_REJOIN_BACKOFF_IDLE = 0,
  ZB_ZDO_REJOIN_BACKOFF_TIMER_RUNNING,
  ZB_ZDO_REJOIN_BACKOFF_COMMAND_SCHEDULED,
  ZB_ZDO_REJOIN_BACKOFF_COMMAND_SENT,
  ZB_ZDO_REJOIN_BACKOFF_COMMAND_DONE
};

/** Rejoin backoff context */
typedef struct zb_zdo_rejoin_backoff_s
{
  enum zb_zdo_rejoin_backoff_state_e rjb_state;
  zb_uint8_t                         rjb_cnt;
  /* Determines whether device goes secure rejoin. */
  zb_bool_t                          rjb_do_secure;
  /* If set to ZB_FALSE device will search on current channel only */
  zb_bool_t                          rjb_scan_all_chan_mask;
  zb_bool_t                          rjb_insecure;
  zb_channel_page_t                  rjb_chan_mask_list[ZB_CHANNEL_PAGES_NUM];
}
zb_zdo_rejoin_backoff_t;

/** Set ZB_ZDO_REJOIN_BACKOFF_COMMAND_DONE state. For internal use. */
void zb_zdo_rejoin_backoff_iteration_done(void);

#endif /* ZB_REJOIN_BACKOFF */

#endif /* ZB_ZDO_REJOIN_BACKOFF_H */
