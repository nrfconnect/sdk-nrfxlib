/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_fem_nrf2220_protocol_api.h
 *
 * @defgroup mpsl_fem_nrf2220_protocol_api MPSL nRF2220 Front End Module Protocol API Extension.
 * @ingroup  mpsl_fem
 *
 * @{
 */

#ifndef MPSL_FEM_NRF2220_PROTOCOL_API_H__
#define MPSL_FEM_NRF2220_PROTOCOL_API_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Bit mask of @c mpsl_fem_pa_power_control_t for transmission involving transition to "transmit" state.
 * If set the "transmit" state is used during transmission.
 * If cleared the "bypass" state is used during transmission.
 */
#define MPSL_FEM_NRF2220_PA_POWER_CONTROL_USE_TRANSMIT      0x01U

#ifdef __cplusplus
}
#endif

#endif // MPSL_FEM_NRF2220_PROTOCOL_API_H__

/**@} */
