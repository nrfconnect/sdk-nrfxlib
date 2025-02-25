/*
 * Copyright (c) 2017, Nordic Semiconductor ASA
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
 * @file
 *   This file supplies bitmasks specifying which of the peripherals are used
 *   by the 802.15.4 driver.
 *
 * Bitmasks currently provided applies to:
 *   - PPI or DPPI channels (g_nrf_802154_used_nrf_ppi_channels)
 *   - PPI or DPPI channel groups (g_nrf_802154_used_nrf_ppi_groups)
 */

#include "nrf_802154_peripherals.h"

#include <stdint.h>

#if NRF_802154_VERIFY_PERIPHS_ALLOC_AGAINST_MPSL
/* Obtaining the MPSL_RESERVED_.. macros */
#include "mpsl.h"
#endif

#if defined(PPI_PRESENT)
#define NRF_802154_PPI_CH_USED_MSK NRF_802154_PPI_CHANNELS_USED_MASK
#define NRF_802154_PPI_GR_USED_MSK NRF_802154_PPI_GROUPS_USED_MASK
#elif defined(DPPI_PRESENT)
#define NRF_802154_PPI_CH_USED_MSK NRF_802154_DPPI_CHANNELS_USED_MASK
#define NRF_802154_PPI_GR_USED_MSK NRF_802154_DPPI_GROUPS_USED_MASK
#else
#error Unsupported chip family
#endif

const uint32_t g_nrf_802154_used_nrf_ppi_channels = NRF_802154_PPI_CH_USED_MSK;
const uint32_t g_nrf_802154_used_nrf_ppi_groups = NRF_802154_PPI_GR_USED_MSK;

#if NRF_802154_VERIFY_PERIPHS_ALLOC_AGAINST_MPSL

#if ((NRF_802154_PPI_CH_USED_MSK & MPSL_RESERVED_PPI_CHANNELS) != 0UL)
#error PPI channels for 802.15.4 driver overlap with MPSL channels
#endif

#if ((NRF_802154_PPI_GR_USED_MSK & MPSL_RESERVED_PPI_GROUPS) != 0UL)
#error PPI groups for 802.15.4 driver overlap with MPSL groups
#endif

#endif // NRF_802154_VERIFY_PERIPHS_ALLOC_AGAINST_MPSL
