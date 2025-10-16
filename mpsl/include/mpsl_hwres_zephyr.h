/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_hwres_zephyr.h
 *
 * @defgroup mpsl_hwres MPSL hardware resources API
 * @ingroup  mpsl
 *
 * This file defines fixed hardware resources used by the MPSL in Zephyr.
 * @{
 */

#ifndef MPSL_HWRES_ZEPHYR_H__
#define MPSL_HWRES_ZEPHYR_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Reserved (D)PPI, PPIB and IPCT resources for the supported platforms. */
/* This corresponds to the PPI channels 19, 30, and 31. */
#define MPSL_NRF52_PPI_CHANNELS_USED_MASK       (0xc0080000)
/* This corresponds to the DPPI channels 0, 1, and 2. */
#define MPSL_NRF53_DPPIC_CHANNELS_USED_MASK     (0x00000007)
#define MPSL_NRF54L_DPPIC10_CHANNELS_USED_MASK  (0x00000001)
#define MPSL_NRF54L_DPPIC20_CHANNELS_USED_MASK  (0x00000001)
#define MPSL_NRF54L_PPIB11_CHANNELS_USED_MASK   (0x00000001)
#define MPSL_NRF54L_PPIB21_CHANNELS_USED_MASK   (0x00000001)
#define MPSL_NRF54H_DPPIC020_CHANNELS_USED_MASK (0x00000001)
#define MPSL_NRF54H_IPCT130_CHANNELS_USED_MASK  (0x00000001)

#ifdef __cplusplus
}
#endif

#endif // MPSL_HWRES_ZEPHYR_H__

/**@} */
