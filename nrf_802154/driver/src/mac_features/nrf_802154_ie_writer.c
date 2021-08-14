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
#include "nrf_802154_core.h"
#include "nrf_802154_nrfx_addons.h"
#include "nrf_802154_tx_work_buffer.h"
#include "nrf_802154_utils.h"
#include "nrf_802154_utils_byteorder.h"

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

static uint8_t * mp_csl_phase_addr;  ///< Cached CSL information element phase field address
static uint8_t * mp_csl_period_addr; ///< Cached CSL information element period field address
static uint16_t  m_csl_period;       ///< CSL period value that will be injected to CSL information element

/**
 * @brief Writes CSL phase to previously set memory address.
 *
 * @param[inout]  p_written  Flag set to true if CSL IE was written. If CSL IE is not written, the flag
 *                           is not modified.
 */
static void csl_ie_write_commit(bool * p_written)
{
    uint32_t time_remaining;
    uint32_t symbols;
    uint32_t csl_phase;

    if ((mp_csl_phase_addr == NULL) || (mp_csl_period_addr == NULL))
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

    host_16_to_little(csl_phase, mp_csl_phase_addr);
    host_16_to_little(m_csl_period, mp_csl_period_addr);

    *p_written = true;
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

    mp_csl_phase_addr  = (uint8_t *)nrf_802154_frame_parser_ie_content_address_get(p_iterator);
    mp_csl_period_addr = mp_csl_phase_addr + sizeof(uint16_t);

    return true;
}

/**
 * @brief Resets CSL writer to pristine state.
 */
static void csl_ie_write_reset(void)
{
    mp_csl_phase_addr  = NULL;
    mp_csl_period_addr = NULL;
}

#else

/**
 * @brief Writes CSL phase to previously set memory address.
 */
static void csl_ie_write_commit(bool * p_written)
{
    // Intentionally empty
    (void)p_written;
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

static uint8_t * mp_lm_rssi_addr;   ///< Cached Link Metrics information element RSSI field address
static uint8_t * mp_lm_margin_addr;  ///< Cached Link Metrics information element link margin field address
static uint8_t * mp_lm_lqi_addr; ///< Cached Link Metrics information element LQI field address

static uint8_t rssi_scale(int8_t rssi)
{
    int16_t rssi_lim;     ///< RSSI value after applying limits
    int16_t intermediate; ///< RSSI value after casting to a wider type
    uint8_t scaled;       ///< RSSI value after scaling

    // Apply lower limit
    rssi_lim =
        ((int16_t)rssi < IE_VENDOR_THREAD_RSSI_FLOOR) ? IE_VENDOR_THREAD_RSSI_FLOOR : (int16_t)rssi;
    // Apply upper limit
    rssi_lim = (rssi_lim > IE_VENDOR_THREAD_RSSI_CEIL) ? IE_VENDOR_THREAD_RSSI_CEIL : rssi_lim;

    // Cast to a wider type to avoid premature overflow
    intermediate = (int16_t)rssi_lim - IE_VENDOR_THREAD_RSSI_FLOOR;
    // Scale linearly to range 0 - UINT8_MAX
    scaled = (uint8_t)((intermediate * (UINT8_MAX - 0)) /
                       (IE_VENDOR_THREAD_RSSI_CEIL - IE_VENDOR_THREAD_RSSI_FLOOR));

    return scaled;
}

static uint8_t margin_scale(int16_t margin)
{
    int16_t margin_lim; ///< Margin value after applying limits
    uint8_t scaled;     ///< Margin value after scaling

    // Apply lower limit
    margin_lim = (margin < IE_VENDOR_THREAD_MARGIN_FLOOR) ? IE_VENDOR_THREAD_MARGIN_FLOOR : margin;
    // Apply upper limit
    margin_lim =
        (margin_lim > IE_VENDOR_THREAD_MARGIN_CEIL) ? IE_VENDOR_THREAD_MARGIN_CEIL : margin_lim;

    // Scale linearly to range 0 - UINT8_MAX
    scaled = (uint8_t)(((margin_lim - IE_VENDOR_THREAD_MARGIN_FLOOR) * (UINT8_MAX - 0)) /
                       (IE_VENDOR_THREAD_MARGIN_CEIL - IE_VENDOR_THREAD_MARGIN_FLOOR));

    return scaled;
}

/**
 * @brief Writes link metrics to previously prepared addresses in a frame.
 *
 * @param[inout]  p_written  Flag set to true if link metrics IE was written. If link metrics IE is
 *                           not written, the flag is not modified.
 */
static void link_metrics_ie_write_commit(bool * p_written)
{
    if ((mp_lm_rssi_addr != NULL) || (mp_lm_margin_addr != NULL))
    {
        int8_t rssi = (uint8_t)nrf_802154_core_last_frame_rssi_get();

        if (mp_lm_rssi_addr != NULL)
        {
            *mp_lm_rssi_addr = rssi_scale(rssi);
            *p_written       = true;
        }

        if (mp_lm_margin_addr != NULL)
        {
            *mp_lm_margin_addr = margin_scale((int16_t)rssi - ED_MIN_DBM);
            *p_written         = true;
        }
    }

    if (mp_lm_lqi_addr != NULL)
    {
        *mp_lm_lqi_addr = (uint8_t)nrf_802154_core_last_frame_lqi_get();
        *p_written      = true;
    }
}

/**
 * @brief Finds and prepare memory addresses where link metrics will be written.
 *
 * @param[in]  p_iterator  Information Element parser iterator.
 *
 * @retval  true  The write prepare operation for link metrics was successful.
 * @retval  false An improperly formatted link metrics IE was detected.
 */
static bool link_metrics_ie_write_prepare(const uint8_t * p_iterator)
{
    assert(p_iterator != NULL);

    // Initialize the iterator at the start of IE content
    uint8_t * p_content_iterator =
        (uint8_t *)nrf_802154_frame_parser_ie_vendor_thread_data_addr_get(p_iterator);
    uint8_t * ie_end = (uint8_t *)nrf_802154_frame_parser_ie_iterator_next(p_iterator);

    if (nrf_802154_frame_parser_ie_length_get(p_iterator) < IE_VENDOR_THREAD_ACK_SIZE_MIN ||
        nrf_802154_frame_parser_ie_length_get(p_iterator) > IE_VENDOR_THREAD_ACK_SIZE_MAX)
    {
        return false;
    }

    while (p_content_iterator != ie_end)
    {
        switch (*p_content_iterator)
        {
            case IE_VENDOR_THREAD_RSSI_TOKEN:
                if (mp_lm_rssi_addr != NULL)
                {
                    return false;
                }
                mp_lm_rssi_addr = p_content_iterator;
                break;

            case IE_VENDOR_THREAD_MARGIN_TOKEN:
                if (mp_lm_margin_addr != NULL)
                {
                    return false;
                }
                mp_lm_margin_addr = p_content_iterator;
                break;

            case IE_VENDOR_THREAD_LQI_TOKEN:
                if (mp_lm_lqi_addr != NULL)
                {
                    return false;
                }
                mp_lm_lqi_addr = p_content_iterator;
                break;

            default:
                return false;
        }

        p_content_iterator++;
    }

    return true;
}

/**
 * @brief Resets the prepared addresses for injecting link metrics into a frame.
 */
static void link_metrics_ie_write_reset(void)
{
    mp_lm_rssi_addr   = NULL;
    mp_lm_margin_addr = NULL;
    mp_lm_lqi_addr    = NULL;
}

/**
 * @brief Resets IE writer to pristine state.
 */
static void ie_writer_reset(void)
{
    m_writer_state = IE_WRITER_RESET;

    csl_ie_write_reset();
    link_metrics_ie_write_reset();
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
        switch (nrf_802154_frame_parser_ie_id_get(p_iterator))
        {
            case IE_VENDOR_ID:
                if (nrf_802154_frame_parser_ie_length_get(p_iterator) >= IE_VENDOR_SIZE_MIN &&
                    nrf_802154_frame_parser_ie_vendor_oui_get(p_iterator) == IE_VENDOR_THREAD_OUI)
                {
                    if (nrf_802154_frame_parser_ie_length_get(p_iterator) >=
                        IE_VENDOR_THREAD_SIZE_MIN &&
                        nrf_802154_frame_parser_ie_vendor_thread_subtype_get(p_iterator) ==
                        IE_VENDOR_THREAD_ACK_PROBING_ID)
                    {
                        result = link_metrics_ie_write_prepare(p_iterator);
                    }
                }
                break;

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
 *
 * @param[inout]  p_written  Flag set to true if IE was written. If IE was not written, the flag is
 *                           not modified.
 */
static void ie_writer_commit(bool * p_written)
{
    assert(m_writer_state == IE_WRITER_PREPARE);
    m_writer_state = IE_WRITER_COMMIT;

    csl_ie_write_commit(p_written);
    link_metrics_ie_write_commit(p_written);
}

void nrf_802154_ie_writer_prepare(uint8_t * p_ie_header, const uint8_t * p_end_addr)
{
    assert(p_ie_header != NULL);
    assert(p_ie_header < p_end_addr);

    ie_writer_reset();
    ie_writer_prepare(p_ie_header, p_end_addr);
}

bool nrf_802154_ie_writer_tx_setup(
    uint8_t                                 * p_frame,
    nrf_802154_transmit_params_t            * p_params,
    nrf_802154_transmit_failed_notification_t notify_function)
{
    (void)notify_function;

    if (p_params->frame_props.dynamic_data_is_set)
    {
        // The dynamic data in the frame is already set. Pass.
        return true;
    }

    const uint8_t * p_mfr_addr;
    uint8_t       * p_ie_header;

    nrf_802154_frame_parser_data_t frame_data;

    bool result = nrf_802154_frame_parser_data_init(p_frame,
                                                    p_frame[PHR_OFFSET] + PHR_SIZE,
                                                    PARSE_LEVEL_FULL,
                                                    &frame_data);

    assert(result);
    (void)result;

    p_ie_header = (uint8_t *)nrf_802154_frame_parser_ie_header_get(&frame_data);
    p_mfr_addr  = nrf_802154_frame_parser_mfr_get(&frame_data);

    if (p_ie_header == NULL)
    {
        return true;
    }

    nrf_802154_ie_writer_prepare(p_ie_header, p_mfr_addr);

    return true;
}

bool nrf_802154_ie_writer_tx_started_hook(uint8_t * p_frame)
{
    (void)p_frame;

    if (m_writer_state != IE_WRITER_PREPARE)
    {
        return true;
    }

    bool written = false;

    ie_writer_commit(&written);
    ie_writer_reset();

    if (written)
    {
        nrf_802154_tx_work_buffer_is_dynamic_data_updated_set();
    }

    return true;
}

void nrf_802154_ie_writer_tx_ack_started_hook(uint8_t * p_ack)
{
    (void)p_ack;

    if (m_writer_state != IE_WRITER_PREPARE)
    {
        return;
    }

    bool written = false;

    ie_writer_commit(&written);
    ie_writer_reset();

    if (written)
    {
        nrf_802154_tx_work_buffer_is_dynamic_data_updated_set();
    }
}

#if NRF_802154_DELAYED_TRX_ENABLED

void nrf_802154_ie_writer_csl_period_set(uint16_t period)
{
    m_csl_period = period;
}

#endif // NRF_802154_DELAYED_TRX_ENABLED

#endif // NRF_802154_IE_WRITER_ENABLED
