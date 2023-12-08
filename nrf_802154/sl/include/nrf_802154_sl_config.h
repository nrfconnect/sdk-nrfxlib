/*
 * Copyright (c) 2020, Nordic Semiconductor ASA
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 * @file This file implements the nRF 802.15.4 Service Layer configuration options.
 *
 */

#ifndef NRF_802154_SL_CONFIG_H__
#define NRF_802154_SL_CONFIG_H__

#include <nrfx.h>

#if NRF_802154_USE_INTERNAL_INCLUDES
#include "nrf_802154_sl_config_internal.h"
#endif

/**
 * @def NRF_802154_SL_RTC_IRQ_PRIORITY
 *
 * The priority of RTC interrupt used in the standalone timer driver implementation.
 *
 * @note This configuration is only applicable for the Low Power Timer Abstraction Layer
 *       implementation in nrf_802154_lp_timer.c.
 *
 */
#ifndef NRF_802154_SL_RTC_IRQ_PRIORITY
#define NRF_802154_SL_RTC_IRQ_PRIORITY 5
#endif

/**
 * @def NRF_802154_SL_TIMESTAMP_ENABLED
 *
 * Configures if timestamps are to be added to the requested events.
 */
#ifndef NRF_802154_SL_TIMESTAMP_ENABLED
#define NRF_802154_SL_TIMESTAMP_ENABLED 1
#endif

/**
 * @def NRF_802154_SL_ANT_DIV_ENABLED
 *
 * Configures if antenna diversity is to be enabled.
 */
#ifndef NRF_802154_SL_ANT_DIV_ENABLED
#if defined(PPI_PRESENT)
#define NRF_802154_SL_ANT_DIV_ENABLED 1
#else
#define NRF_802154_SL_ANT_DIV_ENABLED 0
#endif
#endif

#endif // NRF_802154_SL_CONFIG_H__
