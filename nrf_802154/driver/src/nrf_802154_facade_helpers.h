/*
 * Copyright (c) 2025, Nordic Semiconductor ASA
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

#ifndef NRF_802154_FACADE_HELPERS_H__
#define NRF_802154_FACADE_HELPERS_H__

#include "nrfx.h"
#include "nrf_802154.h"
#include "nrf_802154_config.h"
#include "nrf_802154_types.h"

#ifdef __STATIC_INLINE__
#undef __STATIC_INLINE__
#endif

#ifdef NRF_802154_FACADE_HELPERS_DECLARE_ONLY
#define __STATIC_INLINE__
#else
#define __STATIC_INLINE__ __STATIC_INLINE
#endif

/** @brief Gets a pointer to the default transmit metadata.
 *
 *  @return Pointer to default transmit metadata. The pointed data can be
 *          used directly, no need to copy them unless modification is needed.
 */
__STATIC_INLINE__ const nrf_802154_transmit_metadata_t *
nrf_802154_transmit_metadata_default_ptr_get(void);

/** @brief Checks if transmit metadata is valid.
 *
 * @param[in] p_metadata Metadata to check.
 *
 * @retval false  Metadata is invalid.
 * @retval true   Metadata is valid.
 */
__STATIC_INLINE__ bool nrf_802154_transmit_metadata_is_valid(
    const nrf_802154_transmit_metadata_t * p_metadata);

/** @brief Prepares a default transmit metadata for timed transmissions.
 *
 * @param[out] p_metadata  Default metadata returned through this pointer.
 */
__STATIC_INLINE__ void nrf_802154_transmit_at_metadata_default_prepare(
    nrf_802154_transmit_at_metadata_t * p_metadata);

/** @brief Checks if transmit metadata for timed transmissions is valid.
 *
 * @param[in] p_metadata Metadata to check.
 *
 * @retval false  Metadata is invalid.
 * @retval true   Metadata is valid.
 */
__STATIC_INLINE__ bool nrf_802154_transmit_at_metadata_is_valid(
    const nrf_802154_transmit_at_metadata_t * p_metadata);

#if NRF_802154_CSMA_CA_ENABLED

/** @brief Gets a pointer to the default CSMA/CA transmit metadata.
 *
 *  @return Pointer to default CSMA/CA transmit metadata. The pointed data can be
 *          used directly, no need to copy them unless modification is needed.
 */
__STATIC_INLINE__ const nrf_802154_transmit_csma_ca_metadata_t *
nrf_802154_transmit_csma_ca_metadata_default_ptr_get(void);

/** @brief Checks if CSMA/CA transmit metadata is valid.
 *
 * @param[in] p_metadata Metadata to check.
 *
 * @retval false  Metadata is invalid.
 * @retval true   Metadata is valid.
 */
__STATIC_INLINE__ bool nrf_802154_transmit_csma_ca_metadata_is_valid(
    const nrf_802154_transmit_csma_ca_metadata_t * p_metadata);

#endif /* NRF_802154_CSMA_CA_ENABLED */

#ifndef NRF_802154_FACADE_HELPERS_DECLARE_ONLY

static inline bool are_frame_properties_valid(const nrf_802154_transmitted_frame_props_t * p_props)
{
    return p_props->dynamic_data_is_set || !(p_props->is_secured);
}

static inline bool are_extra_cca_attempts_valid(
    const nrf_802154_transmit_at_metadata_t * p_metadata)
{
    return !p_metadata->cca || (p_metadata->extra_cca_attempts < UINT8_MAX);
}

static inline bool is_tx_timestamp_request_valid(const bool tx_timestamp_encode)
{
#if NRF_802154_TX_TIMESTAMP_PROVIDER_ENABLED
    return true;
#else
    return !tx_timestamp_encode;
#endif
}

__STATIC_INLINE__ const nrf_802154_transmit_metadata_t *
nrf_802154_transmit_metadata_default_ptr_get(void)
{
    static const nrf_802154_transmit_metadata_t metadata = {
        .frame_props         = NRF_802154_TRANSMITTED_FRAME_PROPS_DEFAULT_INIT,
        .cca                 = true,
        .tx_power            = {.use_metadata_value = false},
        .tx_channel          = {.use_metadata_value = false},
        .tx_timestamp_encode = false,
    };

    return &metadata;
}

__STATIC_INLINE__ bool nrf_802154_transmit_metadata_is_valid(
    const nrf_802154_transmit_metadata_t * p_metadata)
{
    return are_frame_properties_valid(&p_metadata->frame_props) &&
           is_tx_timestamp_request_valid(p_metadata->tx_timestamp_encode);
}

__STATIC_INLINE__ void nrf_802154_transmit_at_metadata_default_prepare(
    nrf_802154_transmit_at_metadata_t * p_metadata)
{
    static const nrf_802154_transmit_at_metadata_t initial_metadata =
    {
        .frame_props         = NRF_802154_TRANSMITTED_FRAME_PROPS_DEFAULT_INIT,
        .cca                 = true,
        .tx_power            = {.use_metadata_value = false},
        .extra_cca_attempts  = 0,
        .tx_timestamp_encode = false,
    };

    *p_metadata         = initial_metadata;
    p_metadata->channel = nrf_802154_channel_get();
}

__STATIC_INLINE__ bool nrf_802154_transmit_at_metadata_is_valid(
    const nrf_802154_transmit_at_metadata_t * p_metadata)
{
    return are_frame_properties_valid(&p_metadata->frame_props) &&
           are_extra_cca_attempts_valid(p_metadata) &&
           is_tx_timestamp_request_valid(p_metadata->tx_timestamp_encode);
}

#if NRF_802154_CSMA_CA_ENABLED

__STATIC_INLINE__ const nrf_802154_transmit_csma_ca_metadata_t *
nrf_802154_transmit_csma_ca_metadata_default_ptr_get(void)
{
    static const nrf_802154_transmit_csma_ca_metadata_t metadata =
    {
        .frame_props         = NRF_802154_TRANSMITTED_FRAME_PROPS_DEFAULT_INIT,
        .tx_power            = {.use_metadata_value = false},
        .tx_channel          = {.use_metadata_value = false},
        .tx_timestamp_encode = false,
    };

    return &metadata;
}

__STATIC_INLINE__ bool nrf_802154_transmit_csma_ca_metadata_is_valid(
    const nrf_802154_transmit_csma_ca_metadata_t * p_metadata)
{
    return are_frame_properties_valid(&p_metadata->frame_props) &&
           is_tx_timestamp_request_valid(p_metadata->tx_timestamp_encode);
}

#endif /* NRF_802154_CSMA_CA_ENABLED */

#endif /* NRF_802154_FACADE_HELPERS_DECLARE_ONLY */

#endif // NRF_802154_FACADE_HELPERS_H__
