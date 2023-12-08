/*
 * Copyright (c) 2021, Nordic Semiconductor ASA
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

#ifndef NRF_802154_SECURITY_PIB_H__
#define NRF_802154_SECURITY_PIB_H__

#include "nrf_802154_types.h"

/**
 * @brief Initialises the Key Storage inside the nRF 802.15.4 Radio Driver.
 *
 * @return NRF_802154_SECURITY_ERROR_NONE if no error, otherwise the corresponding
 *         error code.
 */
nrf_802154_security_error_t nrf_802154_security_pib_init(void);

/**
 * @brief Deinitialises the Key Storage inside the nRF 802.15.4 Radio Driver.
 *
 * @return NRF_802154_SECURITY_ERROR_NONE if no error, otherwise the corresponding
 *         error code.
 */
nrf_802154_security_error_t nrf_802154_security_pib_deinit(void);

/**
 * @brief Stores the 802.15.4 MAC Security Key inside the nRF 802.15.4 Radio Driver.
 *
 * @param[in] p_key Pointer to the key to store. Refer to @ref nrf_802154_key_t for details.
 *                  Storing the key copies the content of the key into the Radio Driver.
 *                  This input parameter can be destroyed after the call.
 *
 * @note This function is not reentrant and must be called from thread context only.
 *
 * @retval NRF_802154_SECURITY_ERROR_NONE               Storing of key is successful.
 * @retval NRF_802154_SECURITY_ERROR_TYPE_NOT_SUPPORTED Type of the key is not supported.
 * @retval NRF_802154_SECURITY_ERROR_MODE_NOT_SUPPORTED ID mode of the key is not supported.
 * @retval NRF_802154_SECURITY_ERROR_ALREADY_PRESENT    Failed to store the key - key of such id is already
 *                                                      present. Remove the key first to overwrite.
 * @retval NRF_802154_SECURITY_ERROR_STORAGE_FULL       Failed to store the key - storage full.
 */
nrf_802154_security_error_t nrf_802154_security_pib_key_store(nrf_802154_key_t * p_key);

/**
 * @brief Removes the 802.15.4 MAC Security Key from the nRF 802.15.4 Radio Driver.
 *
 * @param[in] p_id Pointer to the ID of the key to remove.
 *
 * @note This function is not reentrant and must be called from thread context only.
 *
 * @retval NRF_802154_SECURITY_ERROR_NONE          Removal of key is successful.
 * @retval NRF_802154_SECURITY_ERROR_KEY_NOT_FOUND Failed to remove the key - no such key found.
 */
nrf_802154_security_error_t nrf_802154_security_pib_key_remove(nrf_802154_key_id_t * p_id);

/**
 * @brief Removes all stored 802.15.4 MAC Security Keys from the nRF 802.15.4 Radio Driver.
 *
 * @note This function is not reentrant and must be called from thread context only.
 */
void nrf_802154_security_pib_key_remove_all(void);

/**
 * @brief Uses the 802.15.4 MAC Security Key stored previously in the nRF 802.15.4 Radio Driver.
 *
 * @param[in]  p_id        Pointer to the ID of the key to use.
 * @param[out] destination Destination where the key must be copied. In case of
 *                         hardware accelerated solutions this parameter is ignored.
 *
 * @retval NRF_802154_SECURITY_ERROR_NONE          Usage of key is successful.
 * @retval NRF_802154_SECURITY_ERROR_KEY_NOT_FOUND Failed to use the key - no such key found.
 */
nrf_802154_security_error_t nrf_802154_security_pib_key_use(nrf_802154_key_id_t * p_id,
                                                            void                * destination);

/**
 * @brief Sets nRF 802.15.4 Radio Driver MAC Global Frame Counter.
 *
 * @param[in] frame_counter Frame counter to set.
 */
void nrf_802154_security_pib_global_frame_counter_set(uint32_t frame_counter);

/**
 * @brief Sets nRF 802.15.4 Radio Driver MAC Global Frame Counter if the value passed is larger than current.
 *
 * @param[in] frame_counter Frame counter to set.
 */
void nrf_802154_security_pib_global_frame_counter_set_if_larger(uint32_t frame_counter);

/**
 * @brief Get the next 802.15.4 global frame counter.
 *
 * @param[out] p_frame_counter Pointer to the frame counter to populate.
 * @param[in]  p_id            Pointer to the ID of the key to get the frame counter for.
 *
 * @retval NRF_802154_SECURITY_ERROR_NONE                   The p_frame_counter field was
 *                                                          successfully populated.
 * @retval NRF_802154_SECURITY_ERROR_FRAME_COUNTER_OVERFLOW No more available frame counters,
 *                                                          they must be reset.
 * @retval NRF_802154_SECURITY_ERROR_KEY_NOT_FOUND          The associated key was not found.
 */
nrf_802154_security_error_t nrf_802154_security_pib_frame_counter_get_next(
    uint32_t            * p_frame_counter,
    nrf_802154_key_id_t * p_id);

#endif // NRF_802154_SECURITY_PIB_H__
