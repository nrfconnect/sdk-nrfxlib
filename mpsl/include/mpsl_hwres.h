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

/*  The xx_SERIES macros are defined here to keep this file independent
    of the MDK. This allows the header file to be used in nrfx without
    creating circular dependencies. */
#if defined(NRF52805_XXAA) || defined(NRF52810_XXAA) || \
    defined(NRF52811_XXAA) || defined(NRF52820_XXAA) || \
    defined(NRF52832_XXAA) || defined(NRF52832_XXAB) || \
    defined(NRF52833_XXAA) || defined(NRF52840_XXAA)
    #ifndef NRF52_SERIES
        #define NRF52_SERIES
    #endif
#endif

#if defined(NRF5340_XXAA) || defined(NRF5340_XXAA_APPLICATION) || defined(NRF5340_XXAA_NETWORK)
    #ifndef NRF53_SERIES
        #define NRF53_SERIES
    #endif
#endif

#if defined(NRF54L05_XXAA) || defined(NRF54LV10A_ENGA_XXAA) || \
    defined(NRF54L10_XXAA) || defined(NRF54L15_XXAA) || \
    defined(NRF54LM20A_ENGA_XXAA) || \
    defined(NRF54LS05B_ENGA_XXAA) || \
    defined(NRF7120_ENGA_XXAA)
    #ifndef LUMOS_XXAA
        #define LUMOS_XXAA
    #endif
#endif

#if defined(NRF54H20_XXAA)
    #ifndef NRF54H_SERIES
        #define NRF54H_SERIES
    #endif
#endif

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
#elif defined(NRF54H_SERIES)
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
