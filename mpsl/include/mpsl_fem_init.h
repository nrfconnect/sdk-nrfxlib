/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_fem_init.h
 *
 * @defgroup mpsl_fem_init Initialization API of MPSL FEM
 * @ingroup  mpsl_fem
 *
 * @{
 */

#ifndef MPSL_FEM_INIT_H__
#define MPSL_FEM_INIT_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Initializes API of MPSL FEM.
 *
 * @note This function must be called before any protocol driver calls MPSL FEM protocol API.
 * It must be called even if no FEM is selected explicitly. In such case @c none FEM implementation
 * is implicitly used. Call to @ref mpsl_fem_init ensures that modules which are needed
 * by FEM are properly initialized.
 *
 * @note This function does not need to be called if @ref mpsl_init is called.
 */
void mpsl_fem_init(void);

#ifdef __cplusplus
}
#endif

#endif // MPSL_FEM_INIT_H__

/**@} */
