/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef MPSL_DPPI_PROTOCOL_API_H__
#define MPSL_DPPI_PROTOCOL_API_H__

#include "nrf_peripherals.h"
#if (defined DPPIC_PRESENT || defined DOXYGEN)

/**
 * @file mpsl_dppi_protocol_api.h
 *
 * @defgroup mpsl_dppi_protocol_api MPSL protocol API for managing fixed DPPI channels
 * @ingroup  mpsl
 *
 * API for fixed DPPI channel.
 *
 * Fixed DPPI channels are DPPI channels where a publisher or subscriber remain attached to the channel
 * for the entire duration of a radio event.
 * This allows the application to add additional subscribers to channels that have a fixed publisher.
 * For example, the application can configure a pin to toggle on every transaction by connecting
 * NRF_GPIOTE->SUBSCRIBE_OUT[i] to @ref MPSL_DPPI_RADIO_PUBLISH_READY_CHANNEL_IDX or
 * NRF_GPIOTE->SUBSCRIBE_OUT[i] to @ref MPSL_DPPI_RADIO_PUBLISH_DISABLED_CH_IDX.
 *
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Fixed channel for NRF_RADIO->PUBLISH_READY. */
#define MPSL_DPPI_RADIO_PUBLISH_READY_CHANNEL_IDX    4U

/** @brief Fixed channel for NRF_RADIO->PUBLISH_ADDRESS. */
#define MPSL_DPPI_RADIO_PUBLISH_ADDRESS_CHANNEL_IDX  5U

/** @brief Fixed channel for NRF_RADIO->PUBLISH_END. */
#define MPSL_DPPI_RADIO_PUBLISH_END_CHANNEL_IDX      6U

/** @brief Fixed channel for NRF_RADIO->PUBLISH_DISABLED. */
#define MPSL_DPPI_RADIO_PUBLISH_DISABLED_CH_IDX      7U

/** @brief Fixed channel for NRF_RADIO->PUBLISH_PHYEND. */
#define MPSL_DPPI_RADIO_PUBLISH_PHYEND_CHANNEL_IDX   12U

/** @brief Fixed channel for NRF_RADIO->SUBSCRIBE_DISABLE.
 * Publishing to this channel will result in undefined behavior.
 */
#define MPSL_DPPI_RADIO_SUBSCRIBE_DISABLED_CHANNEL_IDX  11U

/** @brief Setup the DPPI channels. */
void mpsl_dppi_fixed_channels_set(void);

/** @brief Clear the DPPI channels. */
void mpsl_dppi_fixed_channels_clear(void);

#ifdef __cplusplus
}
#endif

/**@} */

#endif // DPPIC_PRESENT || DOXYGEN
#endif // MPSL_DPPI_PROTOCOL_API_H__
