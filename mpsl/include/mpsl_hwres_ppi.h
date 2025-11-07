/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_hwres_ppi.h
 *
 * @defgroup mpsl_hwres_ppi MPSL dynamic PPI/DPPI allocation API
 * @ingroup  mpsl
 *
 * This file defines an API to dynamically allocate PPI or DPPI
 * instances. The allocation functions are expected to be implemented
 * outside of the MPSL.
 * @{
 */

#ifndef MPSL_HWRES_PPI_H__
#define MPSL_HWRES_PPI_H__

#include <stdbool.h>
#include <stdint.h>
#include <nrfx.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(DPPI_PRESENT) || defined(DOXYGEN)

/** @brief Allocate a DPPI channel of a DPPIC controller instance.
 *
 *  @param[in]  p_dppic    Pointer to a DPPIC controller instance.
 *  @param[out] p_dppi_ch  Allocated DPPI channel number on the given DPPIC instance.
 *                         Value written at this pointer is valid only on successful
 *                         allocation.
 *
 *  @retval true   Allocation successful.
 *  @retval false  Allocation failed.
 */
bool mpsl_hwres_dppi_channel_alloc(NRF_DPPIC_Type * p_dppic, uint8_t * p_dppi_ch);

#endif /* DPPI_PRESENT */

#if defined(PPIB_PRESENT) || defined(DOXYGEN)

/** @brief Allocate a PPIB channel of a PPIB-to-PPIB interconnection.
 *
 * @param[in]  p_ppib    Pointer to a PPIB instance.
 * @param[out] p_ppib_ch Allocated PPIB channel number the for PPIB-to-PPIB
 *                       interconnection identified by the @p p_ppib being one
 *                       of the sides of the interconnection.
 *                       Value written at this pointer valid only on successful
 *                       allocation.
 *
 *  @retval true   Allocation successful.
 *  @retval false  Allocation failed.
 */
bool mpsl_hwres_ppib_channel_alloc(NRF_PPIB_Type * p_ppib, uint8_t * p_ppib_ch);

#endif /* PPIB_PRESENT */

#ifdef __cplusplus
}
#endif

#endif // MPSL_HWRES_PPI_H__

/**@} */
