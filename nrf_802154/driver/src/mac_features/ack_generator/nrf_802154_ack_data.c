/*
 * Copyright (c) 2018, Nordic Semiconductor ASA
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
 * @file
 *   This file implements procedures to set pending bit and 802.15.4-2015 information
 *   elements in nRF 802.15.4 radio driver.
 *
 */

#include "nrf_802154_ack_data.h"

#include "nrf_802154_assert.h"
#include <string.h>

#include "mac_features/nrf_802154_frame.h"
#include "nrf_802154_bsmap.h"
#include "nrf_802154_config.h"
#include "nrf_802154_const.h"

/** Maximum number of Short Addresses of nodes for which there is ACK data to set. */
#define NUM_SHORT_ADDRESSES    NRF_802154_PENDING_SHORT_ADDRESSES
/** Maximum number of Extended Addresses of nodes for which there is ACK data to set. */
#define NUM_EXTENDED_ADDRESSES NRF_802154_PENDING_EXTENDED_ADDRESSES

#define BSMAP_PEER_AUX_MEM_SIZE                              \
    NRF_802154_BSMAP_AUX_MEMORY_SIZE(                        \
        NRFX_MAX(EXTENDED_ADDRESS_SIZE, SHORT_ADDRESS_SIZE), \
        sizeof(nrf_802154_peer_rec_t))

static nrf_802154_bsmap_t m_bsmap_shortaddr;
static uint8_t m_bsmap_shortaddr_mem[NRF_802154_BSMAP_MEMORY_SIZE(
                                         SHORT_ADDRESS_SIZE,
                                         sizeof(nrf_802154_peer_rec_t),
                                         NUM_SHORT_ADDRESSES)];

static nrf_802154_bsmap_t m_bsmap_extaddr;
static uint8_t            m_bsmap_extaddr_mem[NRF_802154_BSMAP_MEMORY_SIZE(
                                                  EXTENDED_ADDRESS_SIZE,
                                                  sizeof(nrf_802154_peer_rec_t),
                                                  NUM_EXTENDED_ADDRESSES)];

static bool                        m_pending_bit_enabled;
static nrf_802154_src_addr_match_t m_src_matching_method;

static inline nrf_802154_bsmap_t * peer_bsmap_get(bool extended)
{
    return extended ? &m_bsmap_extaddr : &m_bsmap_shortaddr;
}

/**
 * @brief Thread implementation of the pending bit value determination.
 *
 * @param[in]  p_frame_data  Pointer to the frame parser data for which the ACK frame is being prepared.
 *
 * @retval true   Pending bit is to be set.
 * @retval false  Pending bit is to be cleared.
 */
static bool pending_bit_should_be_set_thread(const nrf_802154_frame_t    * p_frame_data,
                                             const nrf_802154_peer_rec_t * p_peer_rec)
{
    if ((!m_pending_bit_enabled) ||
        (nrf_802154_frame_src_addr_get(p_frame_data) == NULL))
    {
        return true;
    }

    if (p_peer_rec == NULL)
    {
        /* The peer record does not exist. */
        return false;
    }

    return p_peer_rec->pending_bit;
}

/**
 * @brief Zigbee implementation of the pending bit value determination.
 *
 * @param[in]  p_frame_data  Pointer to the frame parser data for which the ACK frame is being prepared.
 *
 * @retval true   Pending bit is to be set.
 * @retval false  Pending bit is to be cleared.
 */
static bool pending_bit_should_be_set_zigbee(const nrf_802154_frame_t    * p_frame_data,
                                             const nrf_802154_peer_rec_t * p_peer_rec)
{
    uint8_t         src_addr_type;
    const uint8_t * p_cmd;
    bool            ret = false;

    if (!m_pending_bit_enabled)
    {
        return true;
    }

    /* Check the frame type. */
    src_addr_type = nrf_802154_frame_src_addr_type_get(p_frame_data);

    p_cmd = nrf_802154_frame_mac_command_id_get(p_frame_data);

    /* Check frame type and command type. */
    if ((p_cmd != NULL) && (*p_cmd == MAC_CMD_DATA_REQ))
    {
        /* Check addressing type - in long case address, pb should always be 1. */
        if (src_addr_type == SRC_ADDR_TYPE_SHORT)
        {
            /* Return true if record does not exist in peer table. */
            ret = (p_peer_rec == NULL) ? true : p_peer_rec->pending_bit;
        }
        else
        {
            ret = true;
        }
    }

    return ret;
}

/**
 * @brief Standard-compliant implementation of the pending bit value determination.
 *
 * Function always returns true. It is IEEE 802.15.4 compliant, as per 6.7.3.
 * Higher layer should ensure empty data frame with no AR is sent afterwards.
 *
 * @param[in]  p_frame_data  Pointer to the frame parser data for which the ACK frame is being prepared.
 *
 * @retval true   Pending bit is to be set.
 */
static bool pending_bit_should_be_set_standard_compliant(void)
{
    return true;
}

/**
 * @brief Replace or append an Information Element to the ACK data.
 *
 * If the target ACK data already contains an Information Element with the same
 * ID as the new Information Element, the existing IE is replaced with the new
 * one. Otherwise, the new IE is appended to the target ACK data.
 *
 * @param[in]  location     Index of the ACK data buffer to be modified.
 * @param[in]  extended     Indication whether the ACK data buffer for
 *                          an extended or a short address is to be modified.
 * @param[in]  p_data       New Information Element data.
 * @param[in]  data_len     New Information Element data length.
 *
 * @retval true     The new Information Element has been added successfully.
 * @retval false    The new Information Element has not fitted in the buffer.
 */
static bool ie_data_set(nrf_802154_peer_ie_data_t * p_ie_data,
                        const uint8_t             * p_data,
                        uint8_t                     data_len)
{
    const uint8_t new_ie_id = nrf_802154_frame_ie_id_get(p_data);

    for (const uint8_t * ie = nrf_802154_frame_header_ie_iterator_begin(p_ie_data->data);
         !nrf_802154_frame_ie_iterator_end(ie, &p_ie_data->data[p_ie_data->len]);
         ie = nrf_802154_frame_ie_iterator_next(ie))
    {
        if (nrf_802154_frame_ie_id_get(ie) != new_ie_id)
        {
            continue;
        }

        if (IE_DATA_OFFSET + nrf_802154_frame_ie_length_get(ie) != data_len)
        {
            /* Overwriting an existing IE with a different size is not supported. */
            return false;
        }

        memcpy((uint8_t *)ie, p_data, data_len);
        return true;
    }

    /* Append IE data with the new IE. */
    if (p_ie_data->len + data_len > NRF_802154_MAX_ACK_IE_SIZE)
    {
        /* No space to fit it the new IE. */
        return false;
    }

    memcpy(&p_ie_data->data[p_ie_data->len], p_data, data_len);
    p_ie_data->len += data_len;

    return true;
}

void nrf_802154_ack_data_init(void)
{
    nrf_802154_bsmap_init(&m_bsmap_shortaddr,
                          SHORT_ADDRESS_SIZE,
                          sizeof(nrf_802154_peer_rec_t),
                          NUM_SHORT_ADDRESSES,
                          m_bsmap_shortaddr_mem);

    nrf_802154_bsmap_init(&m_bsmap_extaddr,
                          EXTENDED_ADDRESS_SIZE,
                          sizeof(nrf_802154_peer_rec_t),
                          NUM_EXTENDED_ADDRESSES,
                          m_bsmap_extaddr_mem);

    m_pending_bit_enabled = true;
    m_src_matching_method = NRF_802154_SRC_ADDR_MATCH_THREAD;
}

void nrf_802154_ack_data_enable(bool enabled)
{
    m_pending_bit_enabled = enabled;
}

bool nrf_802154_peer_rec_get(const uint8_t         * p_addr,
                             bool                    extended,
                             nrf_802154_peer_rec_t * p_peer_rec)
{
    const nrf_802154_bsmap_t * p_bsmap = peer_bsmap_get(extended);

    return nrf_802154_bsmap_rec_get(p_bsmap, p_addr, p_peer_rec);
}

bool nrf_802154_peer_rec_write(const uint8_t               * p_addr,
                               bool                          extended,
                               const nrf_802154_peer_rec_t * p_peer_rec)
{
    nrf_802154_bsmap_t * p_bsmap = peer_bsmap_get(extended);
    uint8_t              aux_mem[BSMAP_PEER_AUX_MEM_SIZE];

    return nrf_802154_bsmap_rec_write(p_bsmap, p_addr, p_peer_rec, aux_mem);
}

bool nrf_802154_peer_rec_delete(const uint8_t * p_addr,
                                bool            extended)
{
    nrf_802154_bsmap_t * p_bsmap = peer_bsmap_get(extended);
    uint8_t              aux_mem[BSMAP_PEER_AUX_MEM_SIZE];

    return nrf_802154_bsmap_rec_delete(p_bsmap, p_addr, aux_mem);
}

void nrf_802154_peer_table_clear(bool extended)
{
    nrf_802154_bsmap_t * p_bsmap = peer_bsmap_get(extended);

    nrf_802154_bsmap_clear(p_bsmap);
}

/**
 * @brief Returns pending bit value as nrf_802154_ack_data_pending_bit_should_be_set will
 *        return if a peer record doesn't exist.
 */
static inline bool peer_rec_pending_bit_when_record_doesnt_exist(void)
{
    return m_src_matching_method == NRF_802154_SRC_ADDR_MATCH_ZIGBEE ? true : false;
}

/**
 * @brief Determines if observable behavior will not change if the peer record is deleted
 *        instead of having provided content.
 *
 * @param[in] p_peer_rec    Peer record which content is to be checked.
 *
 * @retval @c false The observable behavior would change if the record is deleted.
 *                  Don't allow to delete the peer record.
 * @retval @c true  The observable behavior would not  change if the record is deleted.
 *                  Allow to delete the peer record.
 */
static bool peer_rec_can_be_deleted(const nrf_802154_peer_rec_t * p_peer_rec)
{
    /* From pending_bit point of view the record can be deleted when
     * the result of nrf_802154_ack_data_pending_bit_should_be_set will not change.
     */
    switch (m_src_matching_method)
    {
        case NRF_802154_SRC_ADDR_MATCH_THREAD:
            if (p_peer_rec->pending_bit)
            {
                return false; /* Prevent record deletion. */
            }
            break;

        case NRF_802154_SRC_ADDR_MATCH_ZIGBEE:
            if (!p_peer_rec->pending_bit)
            {
                /* For Zigbee the pending bit can be 0 only when explicity requested by
                 * the peer record's pending_bit set to false. We cannot remove such record.
                 */
                return false; /* Prevent record deletion. */
            }
            break;

        case NRF_802154_SRC_ADDR_MATCH_ALWAYS_1:
            /* For this method the stored pending bit is irrelevant.
             * The pending_bit value does not inhibit removal of such record.
             */
            break;

        default:
            NRF_802154_ASSERT(false);
            break;
    }

    if (p_peer_rec->ie_data.len != 0U)
    {
        return false; /* Prevent record deletion. */
    }

    return true;      /* Allow record deletion. */
}

static void peer_rec_fill_defaults_that_could_be_deleted(nrf_802154_peer_rec_t * p_peer_rec)
{
    *p_peer_rec             = (nrf_802154_peer_rec_t){0};
    p_peer_rec->pending_bit = peer_rec_pending_bit_when_record_doesnt_exist();
}

bool nrf_802154_ack_data_for_addr_set(const uint8_t       * p_addr,
                                      bool                  extended,
                                      nrf_802154_ack_data_t data_type,
                                      const void          * p_data,
                                      uint8_t               data_len)
{
    nrf_802154_bsmap_t  * p_bsmap = peer_bsmap_get(extended);
    nrf_802154_peer_rec_t peer_rec;
    uint8_t               aux_mem[BSMAP_PEER_AUX_MEM_SIZE];
    bool                  found = true;

    if (!nrf_802154_bsmap_rec_get(p_bsmap, p_addr, &peer_rec))
    {
        /* Record does not exist yet, make a default one */
        peer_rec_fill_defaults_that_could_be_deleted(&peer_rec);
        found = false;
    }

    switch (data_type)
    {
        case NRF_802154_ACK_DATA_IE:
            if (!ie_data_set(&peer_rec.ie_data, p_data, data_len))
            {
                return false;
            }
            break;

        case NRF_802154_ACK_DATA_PENDING_BIT:
            peer_rec.pending_bit = !peer_rec_pending_bit_when_record_doesnt_exist();
            break;

        default:
            NRF_802154_ASSERT(false);
            return false;
    }

    bool result = true;

    if (peer_rec_can_be_deleted(&peer_rec))
    {
        if (found)
        {
            result = nrf_802154_bsmap_rec_delete(p_bsmap, p_addr, aux_mem);
            /* As we found it shall be possible to remove. */
            NRF_802154_ASSERT(result);
        }
        else
        {
            /* The record had not been existing so nothing to delete. */
        }
    }
    else
    {
        result = nrf_802154_bsmap_rec_write(p_bsmap, p_addr, &peer_rec, aux_mem);
    }

    return result;
}

static void peer_rec_by_data_type_clear(nrf_802154_peer_rec_t * p_peer_rec,
                                        nrf_802154_ack_data_t   data_type)
{
    switch (data_type)
    {
        case NRF_802154_ACK_DATA_IE:
            p_peer_rec->ie_data.len = 0U;
            break;

        case NRF_802154_ACK_DATA_PENDING_BIT:
            p_peer_rec->pending_bit = peer_rec_pending_bit_when_record_doesnt_exist();
            break;

        default:
            NRF_802154_ASSERT(false);
            break;
    }
}

bool nrf_802154_ack_data_for_addr_clear(const uint8_t       * p_addr,
                                        bool                  extended,
                                        nrf_802154_ack_data_t data_type)
{
    nrf_802154_bsmap_t  * p_bsmap = peer_bsmap_get(extended);
    nrf_802154_peer_rec_t peer_rec;
    uint8_t               aux_mem[BSMAP_PEER_AUX_MEM_SIZE];

    if (!nrf_802154_bsmap_rec_get(p_bsmap, p_addr, &peer_rec))
    {
        return false;
    }

    peer_rec_by_data_type_clear(&peer_rec, data_type);

    bool result;

    if (peer_rec_can_be_deleted(&peer_rec))
    {
        result = nrf_802154_bsmap_rec_delete(p_bsmap, p_addr, aux_mem);
    }
    else
    {
        result = nrf_802154_bsmap_rec_write(p_bsmap, p_addr, &peer_rec, aux_mem);
    }

    /* As we found it shall be possible to delete or write. */
    NRF_802154_ASSERT(result);

    return result;
}

void nrf_802154_ack_data_reset(bool extended, nrf_802154_ack_data_t data_type)
{
    /* The public API of nRF 802.15.4 Radio Driver expects that it is possible
     * to reset given data_type independently from other data types.
     * The nrf_802154_peer_rec_t record is shared and stores many kinds of
     * data_type. We need to iterate and reset each data_type in each
     * record deleting the record if peer_rec_can_be_deleted allows.
     */

    nrf_802154_bsmap_t        * p_bsmap = peer_bsmap_get(extended);
    nrf_802154_bsmap_iterator_t iter;
    nrf_802154_peer_rec_t       peer_rec;
    uint8_t                     aux_mem[BSMAP_PEER_AUX_MEM_SIZE];

    /* Backward iteration will likely result in deleting records which are
     * the last one without moving data.
     */
    nrf_802154_bsmap_iterator_begin(p_bsmap, &iter, false);

    while (nrf_802154_bsmap_iterator_is_valid(&iter))
    {
        nrf_802154_bsmap_iterator_rec_value_get(&iter, &peer_rec);

        peer_rec_by_data_type_clear(&peer_rec, data_type);

        if (peer_rec_can_be_deleted(&peer_rec))
        {
            nrf_802154_bsmap_iterator_rec_delete(&iter, aux_mem);
        }
        else
        {
            nrf_802154_bsmap_iterator_rec_value_write(&iter, &peer_rec, aux_mem);
        }

        nrf_802154_bsmap_iterator_next(&iter);
    }

    nrf_802154_bsmap_iterator_finish(&iter);
}

void nrf_802154_ack_data_src_addr_matching_method_set(nrf_802154_src_addr_match_t match_method)
{
    switch (match_method)
    {
        case NRF_802154_SRC_ADDR_MATCH_THREAD:
        case NRF_802154_SRC_ADDR_MATCH_ZIGBEE:
        case NRF_802154_SRC_ADDR_MATCH_ALWAYS_1:
            m_src_matching_method = match_method;
            break;

        default:
            NRF_802154_ASSERT(false);
    }
}

bool nrf_802154_ack_data_pending_bit_should_be_set(const nrf_802154_frame_t    * p_frame_data,
                                                   const nrf_802154_peer_rec_t * p_peer_rec)
{
    bool ret;

    switch (m_src_matching_method)
    {
        case NRF_802154_SRC_ADDR_MATCH_THREAD:
            ret = pending_bit_should_be_set_thread(p_frame_data, p_peer_rec);
            break;

        case NRF_802154_SRC_ADDR_MATCH_ZIGBEE:
            ret = pending_bit_should_be_set_zigbee(p_frame_data, p_peer_rec);
            break;

        case NRF_802154_SRC_ADDR_MATCH_ALWAYS_1:
            ret = pending_bit_should_be_set_standard_compliant();
            break;

        default:
            ret = false;
            NRF_802154_ASSERT(false);
    }

    return ret;
}
