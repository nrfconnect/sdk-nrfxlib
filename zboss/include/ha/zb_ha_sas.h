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
/* PURPOSE: HA startup attribute sets definitions
*/
#ifndef ZB_HA_SAS_H
#define ZB_HA_SAS_H 1

#include "zb_ha_config.h"

#if defined ZB_ENABLE_HA_SAS

#define ZB_HA_SAS_SHORT_ADDRESS ZB_NWK_BROADCAST_ALL_DEVICES
#define ZB_HA_SAS_EXT_PAN_ID {0, 0, 0, 0, 0, 0, 0, 0}
#define ZB_HA_SAS_PAN_ID ZB_BROADCAST_PAN_ID
#define ZB_HA_SAS_CHANNEL_MASK ZB_TRANSCEIVER_ALL_CHANNELS_MASK

#define ZB_HA_SAS_TRUST_CENTER_ADDR 0
#define ZB_HA_SAS_MASTER_KEY NULL
#define ZB_HA_SAS_NETWORK_KEY NULL
#define ZB_HA_SAS_DTC_LINK_KEY  \
  "\x5a\x69\x67\x42\x65\x65\x41\x6C\x6C\x69\x61\x6E\x63\x65\x30\x39"

#define ZB_HA_SAS_INSECURE_JOIN ZB_TRUE

#define ZB_HA_SAS_SCAN_ATTEMPTS 3

/* 5.3.2 Join Parameters
TimeBetweenScans
(1 second) Determines the number of seconds between each unsuccessful scan
attempt
 */
#define ZB_HA_SAS_TIME_BTWN_SCANS 1000

void zb_ha_process_sas(void);

#endif /* defined ZB_ENABLE_HA_SAS */

#endif /* ZB_HA_SAS_H */
