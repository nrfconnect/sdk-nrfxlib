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

/**
 * @file
 *   This file implements the CSL IE injector for the 802.15.4 driver.
 *
 */

#include "mac_features/nrf_802154_ie_writer.h"

#include "mac_features/nrf_802154_frame_parser.h"
#include "mac_features/nrf_802154_delayed_trx.h"
#include "nrf_802154_utils.h"

#include <assert.h>

#if NRF_802154_IE_WRITER_ENABLED

typedef enum
{
    IE_WRITER_RESET,
    IE_WRITER_PREPARE,
    IE_WRITER_COMMIT
} writer_state_t;

static writer_state_t m_writer_state = IE_WRITER_RESET; ///< IE writer state

#if NRF_802154_DELAYED_TRX_ENABLED

static uint8_t * mp_csl_phase_addr; ///< Cached CSL information element phase field address

/**
 * @brief Writes CSL phase to previously set memory address.
 */
static void csl_ie_write_commit(void)
{
    uint32_t time_remaining;
    uint32_t symbols;
    uint32_t csl_phase;

    if (mp_csl_phase_addr == NULL)
    {
        // CSL writer not armed. Nothing to be done.
        return;
    }

    if (nrf_802154_delayed_trx_nearest_drx_time_to_midpoint_get(&time_remaining) == false)
    {
        // No delayed DRX is pending. Do not write to the CSL IE.
        return;
    }

    /*
     * Note: The csl_ie_write_commit executes after the FRAMESTART event, which is triggered
     *       by the radio peripheral after the SHR (nRF52840 PS v1.2 -- 6.20.12.6 Transmit sequence).
     *       The symbol calculation should take into account the time needed for two symbols of the PHR.
     *       However, since we are measuring time to the DRX midpoint and not to the beginning, there
     *       is a bit or margin to spare and the calculation does not have to account for the PHR.
     */
    symbols   = time_remaining / PHY_US_PER_SYMBOL;
    csl_phase = symbols / IE_CSL_SYMBOLS_PER_UNIT;

    if (csl_phase > IE_CSL_PERIOD_MAX)
    {
        // CSL phase exceeds the maximum value. Do not write to the CSL IE
        return;
    }

    mp_csl_phase_addr[0] = (uint8_t)(csl_phase & 0xffU);
    mp_csl_phase_addr[1] = (uint8_t)((csl_phase >> 8) & 0xffU);;
}

/**
 * @brief Finds and prepare memory address where CSL phase will be written.
 *
 * @param[in]  p_iterator  Information Element parser iterator.
 *
 * @retval  true  The write prepare operation to CSL IE was successful.
 * @retval  false An improperly formatted CSL IE was detected.
 */
static bool csl_ie_write_prepare(const uint8_t * p_iterator)
{
    assert(p_iterator != NULL);

    if (nrf_802154_frame_parser_ie_length_get(p_iterator) < IE_CSL_SIZE_MIN)
    {
        // The IE is too small to be a valid CSL IE.
        return false;
    }

    mp_csl_phase_addr = (uint8_t *)nrf_802154_frame_parser_ie_content_address_get(p_iterator);

    return true;
}

/**
 * @brief Resets CSL writer to pristine state.
 */
static void csl_ie_write_reset(void)
{
    mp_csl_phase_addr = NULL;
}

#else

/**
 * @brief Writes CSL phase to previously set memory address.
 */
static void csl_ie_write_commit(void)
{
    // Intentionally empty
}

/**
 * @brief Finds and prepare memory address where CSL phase will be written.
 *
 * @param[in]  p_iterator  Information Element parser iterator.
 *
 * @retval  true  The write prepare operation to CSL IE was successful.
 * @retval  false An improperly formatted CSL IE was detected.
 */
static bool csl_ie_write_prepare(const uint8_t * p_iterator)
{
    // Intentionally empty
    return true;
}

/**
 * @brief Resets CSL writer to pristine state.
 */
static void csl_ie_write_reset(void)
{
    // Intentionally empty
}

#endif // NRF_802154_DELAYED_TRX_ENABLED

/**
 * @brief Resets IE writer to pristine state.
 */
static void ie_writer_reset(void)
{
    m_writer_state = IE_WRITER_RESET;

    csl_ie_write_reset();
}

/**
 * @brief Performs IE write preparations.
 *
 * This function prepares the write operation for all recognized information elements and
 * sets the state to IE_WRITER_PREPARE.
 *
 * If any of the information elements fails the boundary check or is not properly formatted,
 * the writer state shall be set to IE_WRITER_RESET.
 *
 * @param[in]  p_ie_header  Pointer to the beginning of header IEs.
 * @param[in]  p_end_addr   Pointer to the first invalid address after p_ie_header.
 */
static void ie_writer_prepare(uint8_t * p_ie_header, const uint8_t * p_end_addr)
{
    assert(m_writer_state == IE_WRITER_RESET);
    m_writer_state = IE_WRITER_PREPARE;

    const uint8_t * p_iterator = nrf_802154_frame_parser_header_ie_iterator_begin(p_ie_header);
    bool            result     = true;

    while (nrf_802154_frame_parser_ie_iterator_end(p_iterator, p_end_addr) == false)
    {
        const uint8_t * p_ie_end_addr = nrf_802154_frame_parser_ie_content_address_get(p_iterator)
                                        + nrf_802154_frame_parser_ie_length_get(p_iterator);

        // Boundary check
        if (p_ie_end_addr > p_end_addr)
        {
            // Reset writer module state
            ie_writer_reset();
            return;
        }

        switch (nrf_802154_frame_parser_ie_id_get(p_iterator))
        {
            case IE_CSL_ID:
                result = csl_ie_write_prepare(p_iterator);
                break;

            default:
                break;
        }

        if (result == false)
        {
            ie_writer_reset();
            return;
        }

        p_iterator = nrf_802154_frame_parser_ie_iterator_next(p_iterator);
    }
}

/**
 * @brief Commits data to recognized information elements.
 */
static void ie_writer_commit(void)
{
    assert(m_writer_state == IE_WRITER_PREPARE);
    m_writer_state = IE_WRITER_COMMIT;

    csl_ie_write_commit();
}

void nrf_802154_ie_writer_prepare(uint8_t * p_ie_header, const uint8_t * p_end_addr)
{
    assert(p_ie_header != NULL);
    assert(p_ie_header < p_end_addr);

    ie_writer_reset();
    ie_writer_prepare(p_ie_header, p_end_addr);
}

bool nrf_802154_ie_writer_pretransmission(const uint8_t * p_frame, bool cca, bool immediate)
{
    const uint8_t * p_mfr_addr;
    uint8_t       * p_ie_header;

    (void)cca;
    (void)immediate;

    p_ie_header = (uint8_t *)nrf_802154_frame_parser_ie_header_get(p_frame);
    p_mfr_addr  = nrf_802154_frame_parser_mfr_address_get(p_frame);

    if (p_ie_header == NULL)
    {
        return true;
    }

    nrf_802154_ie_writer_prepare(p_ie_header, p_mfr_addr);

    return true;
}

bool nrf_802154_ie_writer_tx_started_hook(const uint8_t * p_frame)
{
    (void)p_frame;

    if (m_writer_state != IE_WRITER_PREPARE)
    {
        return true;
    }

    ie_writer_commit();
    ie_writer_reset();

    return true;
}

void nrf_802154_ie_writer_tx_ack_started_hook(const uint8_t * p_ack)
{
    (void)p_ack;

    if (m_writer_state != IE_WRITER_PREPARE)
    {
        return;
    }

    ie_writer_commit();
    ie_writer_reset();
}

#ifdef TEST
/**@brief Unit test facility */
void nrf_802154_ie_writer_module_reset(void)
{
    ie_writer_reset();
}

#endif

#endif // NRF_802154_IE_WRITER_ENABLED
