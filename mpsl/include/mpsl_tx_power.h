/*
 * Copyright (c) 2018 - 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
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
#include "nrf_errno.h"

/** @brief The number of channels for BLE protocol. */
#define MPSL_TOTAL_NUM_OF_BLE_CHANNELS    40
/** @brief The number of channels for 802.15.4 protocol. */
#define MPSL_TOTAL_NUM_OF_802154_CHANNELS 16

/** @brief PHYs supported for limiting channel TX power */
typedef enum
{
    MPSL_PHY_BLE_1M,
    MPSL_PHY_BLE_2M,
    MPSL_PHY_BLE_LR125Kbit,
    MPSL_PHY_BLE_LR500Kbit,
    MPSL_PHY_Ieee802154_250Kbit
} mpsl_phy_t;

/** TX power, dBm. */
typedef int8_t mpsl_tx_power_t;

/** @brief Max TX power envelope. It is up to the application to allocate memory for an envelope. */
typedef struct
{
    mpsl_phy_t phy; /**< The PHY that this envelope applies to. */
    union
    {
        mpsl_tx_power_t tx_power_ble[MPSL_TOTAL_NUM_OF_BLE_CHANNELS];
        mpsl_tx_power_t tx_power_802154[MPSL_TOTAL_NUM_OF_802154_CHANNELS];
    } envelope; /**< Array of maximum TX power value per channel. From 11 to 26 for 802.15.4 protocol. From 0 to 40 for BLE protocol. */
} mpsl_tx_power_envelope_t;

/** @brief Sets maximum TX power envelope. If the PHY already has an envelope configured, it will be over-written.
 *
 * @note Calling this function while an envelope is being used to limit current TX Power may lead to undefined behavior.
 *
 * @note The memory for the provided envelope is not copied and must be valid until it is replaced
 *       with another envelope for the same PHY.
 *
 * @param[in] p_envelope The pointer to an envelope, if NULL the envelopes for all PHYs are reset to
 *                       maximum power possible.
 *
 * @retval  0              TX power channel successfully set.
 * @retval  -NRF_EINVAL   Invalid parameter provided.
 */
int32_t mpsl_tx_power_channel_map_set(const mpsl_tx_power_envelope_t *const p_envelope);

#ifdef __cplusplus
}
#endif

#endif// MPSL_TX_POWER_H__

/**@} */
