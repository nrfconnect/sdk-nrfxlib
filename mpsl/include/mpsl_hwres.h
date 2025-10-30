/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_hwres.h
 *
 * @defgroup mpsl_hwres MPSL hardware resources API
 * @ingroup  mpsl
 *
 * This file defines fixed hardware resources used by the MPSL.
 * @{
 */

#ifndef MPSL_HWRES_H__
#define MPSL_HWRES_H__

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_peripherals.h"
#include "mpsl_hwres_ppi.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Reserved (D)PPI, PPIB and IPCT resources for the supported platforms. */
#if defined(NRF52_SERIES)
    #define MPSL_RESERVED_PPI_CHANNELS ((1UL << 19) | (1UL << 30) | (1UL << 31))
    /* This corresponds to the PPI channels 19, 30, and 31. */
    #define MPSL_PPI_CHANNELS_USED_MASK (0xc0080000)
#elif defined(NRF53_SERIES)
    #define MPSL_RESERVED_PPI_CHANNELS ((1UL << 0) | (1UL << 1) | (1UL << 2))
    /* This corresponds to the DPPI channels 0, 1, and 2. */
    #define MPSL_DPPIC_CHANNELS_USED_MASK (0x00000007)
#elif defined(LUMOS_XXAA)
    #define MPSL_RESERVED_PPI_CHANNELS (1UL << 0)
    #define MPSL_DPPIC10_CHANNELS_USED_MASK (0x00000001)
    #define MPSL_DPPIC20_CHANNELS_USED_MASK (0x00000001)
    #define MPSL_PPIB11_CHANNELS_USED_MASK  (0x00000001)
    #define MPSL_PPIB21_CHANNELS_USED_MASK  (0x00000001)
#elif defined(NRF54H_SERIES) || defined(GRTC_PRESENT)
    #define MPSL_RESERVED_PPI_CHANNELS (1UL << 0)
    #define MPSL_DPPIC020_CHANNELS_USED_MASK (0x00000001)
    #define MPSL_IPCT130_CHANNELS_USED_MASK  (0x00000001)
#else
    #error Unknown NRF series.
#endif

/* Defines which timer is being used by the MPSL implementation */
#if defined(NRF52_SERIES)
    #define MPSL_TIMER0 NRF_TIMER0
#elif defined(NRF53_SERIES)
    #define MPSL_TIMER0 NRF_TIMER0_NS
#elif defined(LUMOS_XXAA)
    #define MPSL_TIMER0 NRF_TIMER10
#else
    #define MPSL_TIMER0 NRF_TIMER020
#endif

#ifdef __cplusplus
}
#endif

#endif // MPSL_HWRES_H__

/**@} */
