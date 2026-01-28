/*
 * Copyright (c) 2025, Nordic Semiconductor ASA
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

#ifndef NRF_802154_CONFIG_SOC_H_
#define NRF_802154_CONFIG_SOC_H_

/* This file provides information about the SoC to the nRF 802.15.4 driver.
 * It defines nrf 802.15.4 driver's internal macros to denote SoC series and
 * SoC features based on macros provided by the build system.
 *
 * This file intentionally does not include nrf.h or nrfx.h to avoid
 * circular include dependencies.
 */

#ifdef NRF_802154_SOC_SUPPORTED
    #error "Do not define NRF_802154_SOC_SUPPORTED externally"
#endif

#ifdef NRF_802154_SOC_NRF52_SERIES
    #error "Do not define NRF_802154_SOC_NRF52_SERIES externally"
#endif

#if defined (NRF52833_XXAA) || defined (NRF52840_XXAA)
    #define NRF_802154_SOC_NRF52_SERIES
    #define NRF_802154_SOC_SUPPORTED
#endif

#ifdef NRF_802154_SOC_NRF53_SERIES
    #error "Do not define NRF_802154_SOC_NRF53_SERIES externally"
#endif

#if defined (NRF5340_XXAA) || defined(NRF5340_XXAA_APPLICATION) || defined(NRF5340_XXAA_NETWORK)
    #define NRF_802154_SOC_NRF53_SERIES
    #define NRF_802154_SOC_SUPPORTED
#endif

#ifdef NRF_802154_SOC_NRF54H_SERIES
    #error "Do not define NRF_802154_SOC_NRF54H_SERIES externally"
#endif

#if defined (NRF54H20_XXAA)
    #define NRF_802154_SOC_NRF54H_SERIES
    #define NRF_802154_SOC_SUPPORTED
#endif

#ifdef NRF_802154_SOC_NRF54L_SERIES
    #error "Do not define NRF_802154_SOC_NRF54L_SERIES externally"
#endif

#if defined (NRF54L05_XXAA) || defined (NRF54LV10A_ENGA_XXAA) || defined (NRF54L10_XXAA) \
    || defined (NRF54L15_XXAA) ||  defined (NRF54LM20A_ENGA_XXAA) \
    || defined (NRF54LV10A_XXAA) ||  defined (NRF54LM20A_XXAA)
    #define NRF_802154_SOC_NRF54L_SERIES
    #define NRF_802154_SOC_SUPPORTED
#endif

#ifdef NRF_802154_SOC_PPI_PRESENT
    #error "Do not define NRF_802154_SOC_PPI_PRESENT externally"
#endif

#ifdef NRF_802154_SOC_DPPI_PRESENT
    #error "Do not define NRF_802154_SOC_DPPI_PRESENT externally"
#endif

#if defined(NRF_802154_SOC_NRF52_SERIES)
    #define NRF_802154_SOC_PPI_PRESENT
#endif

#if defined(NRF_802154_SOC_NRF53_SERIES)
    #define NRF_802154_SOC_DPPI_PRESENT
#endif

#if defined(NRF_802154_SOC_NRF54H_SERIES)
    #define NRF_802154_SOC_DPPI_PRESENT
#endif

#if defined(NRF_802154_SOC_NRF54L_SERIES)
    #define NRF_802154_SOC_DPPI_PRESENT
#endif

#ifdef NRF_802154_USE_INTERNAL_INCLUDES
#include "nrf_802154_soc_config_internal.h"
#endif

#ifndef NRF_802154_SOC_SUPPORTED
#error "nRF 802.15.4 driver is not supported on the selected SoC."
#endif

#endif /* NRF_802154_CONFIG_SOC_H_ */
