/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_tx_power.h
 *
 * @defgroup mpsl_tx_power MPSL TX Power interface
 * @ingroup  mpsl
 *
 * The MPSL TX Power interface provides APIs to set the maximum TX power per channel.
 *
 * @{
 */

#ifndef MPSL_TX_POWER_H__
#define MPSL_TX_POWER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/** TX power, dBm. */
typedef int8_t mpsl_tx_power_t;

/** @brief Converts radio power in dBm to RADIO.TXPOWER register code.
 *
 *  @param[in] req_radio_power  Requested TX power desired for RADIO peripheral.
 *                              The power value in dBm must be supported by the RADIO peripheral.
 *
 *  @return RADIO.TXPOWER register code corresponding to radio power in dBm.
 */
uint32_t mpsl_tx_power_dbm_to_radio_register_convert(mpsl_tx_power_t req_radio_power);

#ifdef __cplusplus
}
#endif

#endif // MPSL_TX_POWER_H__

/**@} */
