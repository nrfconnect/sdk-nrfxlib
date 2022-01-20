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
/* PURPOSE: General purpose software watchdog for ZB apps
*/

#ifndef ZB_WATCHDOG_H
#define ZB_WATCHDOG_H 1

#define ZB_WATCHDOG_DISABLED 0U
#define ZB_WATCHDOG_ENABLED  1U

#include "zb_common.h"
typedef struct zb_watchdow_s
{
  zb_time_t timeout;
  zb_time_t last_kick;
  zb_uint8_t state;             /* 0 - disabled, 1 - enabled */
} zb_watchdog_t;

typedef enum zb_watchdog_types_e
{
  ZB_WD_ZB_TRAFFIC,

  ZB_WD_CHECKIN,

  ZB_N_STD_WATCHDOGS
} zb_watchdog_types_t;

#define ZB_N_WATCHDOG 8U

/* error codes for error indication */
/* a watchdog type will be put in additional_info */
#define ZB_ERROR_WATCHDOG_TRIGGERED 1U

#ifdef USE_ZB_WATCHDOG

void zb_add_watchdog(zb_uint8_t wd_number, zb_time_t timeout);
void zb_kick_watchdog(zb_uint8_t wd_number);
void zb_watchdog_scheduler(zb_uint8_t param);
void zb_stop_watchdog(zb_uint8_t wd_number);
void zb_enable_watchdog(zb_uint8_t wd_number);
void zb_disable_watchdog(zb_uint8_t wd_number);

#define ZB_KICK_WATCHDOG(n) zb_kick_watchdog((zb_uint8_t)(n))
#define ZB_ENABLE_WATCHDOG(n) zb_enable_watchdog((zb_uint8_t)(n))
#define ZB_DISABLE_WATCHDOG(n) zb_disable_watchdog((zb_uint8_t)(n))

#else

#define ZB_KICK_WATCHDOG(n)
#define ZB_ENABLE_WATCHDOG(n)
#define ZB_DISABLE_WATCHDOG(n)

#endif

#ifdef ZB_USE_CHECKIN_WATCHDOG

#ifndef ZB_WD_CHECKIN_MIN_TO
#define ZB_WD_CHECKIN_MIN_TO (60U*60U*ZB_TIME_ONE_SECOND)
#endif

#endif /* ZB_USE_CHECKIN_WATCHDOG */


#ifdef ZB_USE_ZB_TRAFFIC_WATCHDOG

#ifndef ZB_WD_ZB_TRAFFIC_TO
#define ZB_WD_ZB_TRAFFIC_TO (240U*60U*ZB_TIME_ONE_SECOND)
#endif

#endif /* ZB_USE_ZB_TRAFFIC_WATCHDOG */

#endif /* ZB_WATCHDOG_H */
