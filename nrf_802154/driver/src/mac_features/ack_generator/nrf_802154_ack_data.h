/*
 * Copyright (c) 2017, Nordic Semiconductor ASA
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
 * @brief Module that contains an ACK data generator for the nRF 802.15.4 radio driver.
 *
 * @note  The current implementation supports setting pending bit and IEs in 802.15.4-2015 Enh-Ack frames.
 */

#ifndef NRF_802154_ACK_DATA_H
#define NRF_802154_ACK_DATA_H

#include <stdbool.h>
#include <stdint.h>

#include "nrf_802154_types.h"
#include "mac_features/nrf_802154_frame.h"

/** @brief Structure representing a single IE record for a peer. */
typedef struct
{
    uint8_t data[NRF_802154_MAX_ACK_IE_SIZE]; /**< IE data buffer. */
    uint8_t len;                              /**< Length of the buffer. */
} nrf_802154_peer_ie_data_t;

/** @brief Information record about a peer. */
typedef struct
{
    /** @brief Pending bit to be set for ACK frames sent back to given peer. */
    bool                      pending_bit : 1;
    /** @brief IE data generated for the peer. */
    nrf_802154_peer_ie_data_t ie_data;
} nrf_802154_peer_rec_t;

/**
 * @brief Initialize the ACK data generator module.
 */
void nrf_802154_ack_data_init(void);

/**
 * @brief Enable or disable the ACK data generator module.
 *
 * @param[in]  enabled  True if the module is to be enabled. False otherwise.
 */
void nrf_802154_ack_data_enable(bool enabled);

/** @brief Retrieve a peer record for given peer address from the peer table.
 *
 *  @param[in]  p_addr      Pointer to a peer address.
 *  @param[in]  extended    Selects peer table and format of @p p_addr.
 *                          @c false the @p p_addr points to short address.
 *                          @c true the @p p_addr points to extended address.
 *  @param[out] p_peer_rec  Pointer to peer record to be filled in based on the content
 *                          of peer table.
 *
 *  @retval @c false    Peer record for given peer address not found in peer table.
 *                      The content point of @p p_rec is undefined.
 *  @retval @c true     Peer record found and stored at @p p_rec pointer.
 */
bool nrf_802154_peer_rec_get(const uint8_t         * p_addr,
                             bool                    extended,
                             nrf_802154_peer_rec_t * p_peer_rec);

/** @brief Write a peer record for given peer addres to the peer table.
 *
 *  @param[in]  p_addr      Pointer to a peer address.
 *  @param[in]  extended    Selects peer table and format of @p p_addr.
 *                          @c false the @p p_addr points to short address.
 *                          @c true the @p p_addr points to extended address.
 *  @param[out] p_peer_rec  Pointer to peer record to be filled in based on the content
 *                          of peer table.
 *
 *  @retval @c false    The peer record did not exist and could not be added because the
 *                      peer table is full.
 *  @retval @c true     Peer record for given peer address stored in the peer table.
 */
bool nrf_802154_peer_rec_write(const uint8_t               * p_addr,
                               bool                          extended,
                               const nrf_802154_peer_rec_t * p_peer_rec);

/** @brief Delete a peer record for given peer address from the peer table.
 *
 *  @param[in]  p_addr      Pointer to a peer address.
 *  @param[in]  extended    Selects peer table and format of @p p_addr.
 *                          @c false the @p p_addr points to short address.
 *                          @c true the @p p_addr points to extended address.
 *
 *  @retval @c false  Given peer address not found in the peer table. Note that the
 *                    goal of the function that is to not have a record for given peer address
 *                    is reached.
 *  @retval @c true   Given peer address was found and deleted from the peer table.
 */
bool nrf_802154_peer_rec_delete(const uint8_t * p_addr,
                                bool            extended);

/** @brief Delete all peer records from given peer table.
 *
 *  @param[in]  extended    Selects peer table.
 *                          @c false for the short address peer table.
 *                          @c true for the extended address peer table.
 */
void nrf_802154_peer_table_clear(bool extended);

/**
 * @brief Add an address to the ACK data list.
 *
 * ACK frames sent in response to frames with the source address matching any address from the ACK data list
 * will have the appropriate data set. If the source address does not match any of the addresses in the
 * list, the ACK frame will not have the data set.
 *
 * @param[in]  p_addr    Pointer to the address that is to be added to the list.
 * @param[in]  extended  Indication if @p p_addr is an extended address or a short address.
 * @param[in]  data_type Type of data to be set. Refer to the @ref nrf_802154_ack_data_t type.
 * @param[in]  p_data    Pointer to the data to be set.
 * @param[in]  data_len  Length of the @p p_data buffer.
 *
 * @retval true   Address successfully added to the list.
 * @retval false  Address not added to the list (list is full).
 */
bool nrf_802154_ack_data_for_addr_set(const uint8_t       * p_addr,
                                      bool                  extended,
                                      nrf_802154_ack_data_t data_type,
                                      const void          * p_data,
                                      uint8_t               data_len);

/**
 * @brief Remove an address from the ACK data list.
 *
 * ACK frames sent in response to frames with the source address matching any address from
 * the ACK data list will have the appropriate data set. If the source address does not match
 * any of the addresses in the list, the ACK frame will not have the data set.
 *
 * @param[in]  p_addr    Pointer to the address that is to be removed from the list.
 * @param[in]  extended  Indication if @p p_addr is an extended address or a short address.
 * @param[in]  data_type Type of data that is to be cleared for @p p_addr.
 *
 * @retval true   Address successfully removed from the list.
 * @retval false  Address not removed from the list (address is missing from the list).
 */
bool nrf_802154_ack_data_for_addr_clear(const uint8_t       * p_addr,
                                        bool                  extended,
                                        nrf_802154_ack_data_t data_type);

/**
 * @brief Remove all addresses of a given length from the ACK data list.
 *
 * @param[in]  extended  Indication if all extended addresses or all short addresses are
 *                       to be removed from the list.
 * @param[in]  data_type Type of data that is to be cleared for all addresses of a given length.
 */
void nrf_802154_ack_data_reset(bool extended, nrf_802154_ack_data_t data_type);

/**
 * @brief Select the source matching algorithm.
 *
 * @note This function is to be called after the driver initialization, but before the transceiver is enabled.
 *
 * When calling @ref nrf_802154_ack_data_pending_bit_should_be_set, one of several algorithms
 * for source address matching will be chosen. To ensure a specific algorithm is selected,
 * call this function before @ref nrf_802154_ack_data_pending_bit_should_be_set.
 *
 * @param[in]  match_method Source matching method to be used.
 */
void nrf_802154_ack_data_src_addr_matching_method_set(nrf_802154_src_addr_match_t match_method);

/**
 * @brief Check if a pending bit is to be set in the ACK frame sent in response to a given frame.
 *
 * @param[in]  p_frame_data  Pointer to the frame parser data for which the ACK frame is being prepared.
 * @param[in]  p_peer_rec    Pointer to the peer record already searched for given @p p_frame_data.
 *                           The search should have been performed by a call to the
 *                           @ref nrf_802154_peer_rec_get function. If the record was not found, use
 *                           @c NULL to indicate that the record does not exist.
 *
 * @retval true   Pending bit is to be set.
 * @retval false  Pending bit is to be cleared.
 */
bool nrf_802154_ack_data_pending_bit_should_be_set(const nrf_802154_frame_t    * p_frame_data,
                                                   const nrf_802154_peer_rec_t * p_peer_rec);

#endif /* NRF_802154_ACK_DATA_H */
