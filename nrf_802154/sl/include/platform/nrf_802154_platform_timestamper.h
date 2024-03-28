/*
 * Copyright (c) 2024, Nordic Semiconductor ASA
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

/**
 * @brief Abstraction layer for peripheral-to-peripheral hardware connections needed for timestamping.
 */

#ifndef NRF_802154_PLATFORM_TIMESTAMPER_H_
#define NRF_802154_PLATFORM_TIMESTAMPER_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the timestamper platform.
 */
void nrf_802154_platform_timestamper_init(void);

/**
 * @brief Sets up cross-domain hardware connections necessary to capture a timestamp.
 *
 * This function configures cross-domain hardware connections necessary to capture a timestamp of
 * an event from the local domain. These connections are identical for all local domain events.
 *
 * @note Every call to this function must be paired with a call to @ref
 * nrf_802154_platform_timestamper_cross_domain_connections_clear.
 */
void nrf_802154_platform_timestamper_cross_domain_connections_setup(void);

/**
 * @brief Clears cross-domain hardware connections necessary to capture a timestamp.
 */
void nrf_802154_platform_timestamper_cross_domain_connections_clear(void);

/**
 * @brief Sets up local domain hardware connections necessary to capture a timestamp.
 *
 * This function configures local domain hardware connections necessary to capture a timestamp of
 * an event from the local domain. These connections must be setup separately for every local domain
 * event.
 *
 * @param[in]  dppi_ch   Local domain DPPI channel that the event to be timestamped publishes to.
 */
void nrf_802154_platform_timestamper_local_domain_connections_setup(uint32_t dppi_ch);

/**
 * @brief Clears local domain hardware connections necessary to capture a timestamp.
 *
 * @param[in]  dppi_ch   Local domain DPPI channel that the event to be timestamped publishes to.
 */
void nrf_802154_platform_timestamper_local_domain_connections_clear(uint32_t dppi_ch);

/**
 * @brief Reads timestamp captured using the configured hardware connections.
 *
 * @param[out]  p_timestamp   Captured timestamp. Only valid if @c true is returned, undefined otherwise.
 *
 * @retval  true   The timestamp was captured and read successfully.
 * @retval  false  The timestamp could not be retrieved.
 */
bool nrf_802154_platform_timestamper_captured_timestamp_read(uint64_t * p_captured);

#ifdef __cplusplus
}
#endif

#endif // NRF_802154_PLATFORM_TIMESTAMPER_H_
