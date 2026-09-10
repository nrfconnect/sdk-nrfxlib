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
 * @brief Module that contains calculations of the duration of the 802.15.4 radio driver procedures.
 *
 */

#ifndef NRF_802154_PROCEDURES_DURATION_H_
#define NRF_802154_PROCEDURES_DURATION_H_

#include <stdbool.h>
#include <stdint.h>
#include "nrfx.h"

#include "nrf_802154_assert.h"
#include "nrf_802154_const.h"
#include "nrf_802154_utils.h"

#ifdef __STATIC_INLINE__
#undef __STATIC_INLINE__
#endif

#ifdef NRF_802154_PROCEDURES_DURATION_DECLARE_ONLY
#define __STATIC_INLINE__
#else
#define __STATIC_INLINE__                __STATIC_INLINE
#endif

#define TX_RAMP_UP_TIME                  40 // us
#define RX_RAMP_UP_TIME                  40 // us
#define RX_RAMP_DOWN_TIME                0  // us
#define MAX_RAMP_DOWN_TIME               6  // us
#if defined(NRF54L_SERIES)
#define RX_TX_TURNAROUND_TIME_HW         15 // us
#else
#define RX_TX_TURNAROUND_TIME_HW         20 // us
#endif

#define RX_TX_TURNAROUND_TIME            (RX_TX_TURNAROUND_TIME_HW + \
                                          NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US)

#define RX_PHYEND_EVENT_LATENCY_US_OQPSK 23 ///< Latency in us between the last bit on air and the PHYEND event for O-QPSK PHY.
#define RX_PHYEND_EVENT_LATENCY_US_GFSK  6  ///< Latency in us between the last bit on air and the PHYEND event for GFSK PHY.
#define RSSI_SETTLE_TIME_US              15 ///< Time required for RSSI measurements to become valid after signal level change.

/**@brief Duration of the SHR in microseconds for the given PHY. */
__STATIC_INLINE__ uint16_t nrf_802154_phy_shr_us_get(nrf_802154_phy_t phy);

/**@brief On-air duration in microseconds of the given number of octets for the given PHY. */
__STATIC_INLINE__ uint16_t nrf_802154_phy_octets_to_us(uint16_t octets, nrf_802154_phy_t phy);

/**@brief On-air duration in microseconds of a frame, optionally including SHR and PHR, for the given PHY. */
__STATIC_INLINE__ uint16_t nrf_802154_frame_duration_get(uint8_t          psdu_length,
                                                         bool             shr,
                                                         bool             phr,
                                                         nrf_802154_phy_t phy);

/**@brief Total duration in microseconds of a TX procedure, optionally with a preceding CCA and a following ACK wait. */
__STATIC_INLINE__ uint16_t nrf_802154_tx_duration_get(uint8_t          psdu_length,
                                                      bool             cca,
                                                      bool             ack_requested,
                                                      nrf_802154_phy_t phy);

/**@brief Duration in microseconds of a CCA followed by the RX-to-TX turnaround. */
__STATIC_INLINE__ uint16_t nrf_802154_cca_before_tx_duration_get(void);

/**@brief Total duration in microseconds of an RX procedure, optionally including the subsequent ACK transmission. */
__STATIC_INLINE__ uint16_t nrf_802154_rx_duration_get(uint8_t          psdu_length,
                                                      bool             ack_requested,
                                                      nrf_802154_phy_t phy);

/**@brief Total duration in microseconds of a standalone CCA procedure. */
__STATIC_INLINE__ uint16_t nrf_802154_cca_duration_get(void);

/**@brief On-air duration in microseconds of an immediate ACK frame (SHR + PHR + ACK PSDU) for the given PHY. */
__STATIC_INLINE__ uint16_t nrf_802154_imm_ack_duration_get(nrf_802154_phy_t phy);

/**@brief MAC ACK wait duration in microseconds for the given PHY. */
__STATIC_INLINE__ uint16_t nrf_802154_mac_imm_ack_wait_duration_get(nrf_802154_phy_t phy);

/**@brief On-air duration in microseconds of the longest possible 802.15.4 frame for the given PHY. */
__STATIC_INLINE__ uint16_t nrf_802154_max_phy_frame_time_us_get(nrf_802154_phy_t phy);

/**@brief Latency in microseconds between the last bit on air and the RADIO.PHYEND event on RX for the given PHY. */
__STATIC_INLINE__ uint16_t nrf_802154_phy_rx_end_event_latency_us_get(nrf_802154_phy_t phy);

/**@brief Delay in microseconds from the RADIO.ADDRESS event on TX to the first bit of MHR on air for the given PHY. */
__STATIC_INLINE__ uint16_t nrf_802154_phy_tx_address_to_mhr_us(nrf_802154_phy_t phy);

/**@brief Delay in microseconds from the RADIO.ADDRESS event on TX to the first bit of PHR on air for the given PHY. */
__STATIC_INLINE__ uint16_t nrf_802154_phy_tx_address_to_phr_us(nrf_802154_phy_t phy);

#ifndef NRF_802154_PROCEDURES_DURATION_DECLARE_ONLY

__STATIC_INLINE__ uint16_t nrf_802154_phy_shr_us_get(nrf_802154_phy_t phy)
{
    if (NRF_802154_GFSK_2MBPS_PHY_ENABLED && phy == NRF_802154_PHY_EXP1_GFSK_2MBPS)
    {
        return PHY_GFSK_SHR_SYMBOLS * PHY_GFSK_US_PER_OCTET / PHY_GFSK_SYMBOLS_PER_OCTET;
    }
    else
    {
        return PHY_OQPSK_SHR_SYMBOLS * PHY_OQPSK_US_PER_SYMBOL;
    }
}

__STATIC_INLINE__ uint16_t nrf_802154_phy_octets_to_us(uint16_t octets, nrf_802154_phy_t phy)
{
    if (NRF_802154_GFSK_2MBPS_PHY_ENABLED && phy == NRF_802154_PHY_EXP1_GFSK_2MBPS)
    {
        return octets * PHY_GFSK_US_PER_OCTET;
    }
    else
    {
        return octets * PHY_OQPSK_SYMBOLS_PER_OCTET * PHY_OQPSK_US_PER_SYMBOL;
    }
}

__STATIC_INLINE__ uint16_t nrf_802154_frame_duration_get(uint8_t          psdu_length,
                                                         bool             shr,
                                                         bool             phr,
                                                         nrf_802154_phy_t phy)
{
    uint16_t us_time = nrf_802154_phy_octets_to_us(psdu_length, phy);

    if (phr)
    {
        us_time += nrf_802154_phy_octets_to_us(PHR_SIZE, phy);
    }

    if (shr)
    {
        us_time += nrf_802154_phy_shr_us_get(phy);
    }

    return us_time;
}

__STATIC_INLINE__ uint16_t nrf_802154_tx_duration_get(uint8_t          psdu_length,
                                                      bool             cca,
                                                      bool             ack_requested,
                                                      nrf_802154_phy_t phy)
{
    // ramp down
    // if CCA: + RX ramp up + CCA + RX ramp down
    // + TX ramp up + SHR + PHR + PSDU
    // if ACK: + macAckWaitDuration
    uint16_t us_time = MAX_RAMP_DOWN_TIME + TX_RAMP_UP_TIME + nrf_802154_frame_duration_get(
        psdu_length,
        true,
        true,
        phy);

    if (ack_requested)
    {
        us_time += nrf_802154_mac_imm_ack_wait_duration_get(phy);
    }

    if (cca)
    {
        us_time += RX_RAMP_UP_TIME + RX_RAMP_DOWN_TIME + CCA_TIME;
    }

    return us_time;
}

__STATIC_INLINE__ uint16_t nrf_802154_cca_before_tx_duration_get(void)
{
    // CCA + turnaround time
    uint16_t us_time = CCA_TIME + RX_TX_TURNAROUND_TIME;

    return us_time;
}

/**@brief Get the duration of the ACK frame along with turnaround in microseconds. */
__STATIC_INLINE__ uint16_t nrf_802154_ack_duration_with_turnaround_get(nrf_802154_phy_t phy)
{
    // aTurnaroundTime + ACK frame duration
    return TURNAROUND_TIME +
           nrf_802154_phy_shr_us_get(phy) +
           nrf_802154_phy_octets_to_us(IMM_ACK_LENGTH + PHR_SIZE, phy);
}

__STATIC_INLINE__ uint16_t nrf_802154_rx_duration_get(uint8_t          psdu_length,
                                                      bool             ack_requested,
                                                      nrf_802154_phy_t phy)
{
    // SHR + PHR + PSDU
    // if ACK: + aTurnaroundTime + ACK frame duration
    uint16_t us_time = nrf_802154_frame_duration_get(psdu_length, true, true, phy);

    if (ack_requested)
    {
        us_time += nrf_802154_ack_duration_with_turnaround_get(phy);
    }

    return us_time;
}

__STATIC_INLINE__ uint16_t nrf_802154_cca_duration_get(void)
{
    // ramp down + rx ramp up + CCA
    uint16_t us_time = MAX_RAMP_DOWN_TIME +
                       RX_RAMP_UP_TIME +
                       CCA_TIME;

    return us_time;
}

__STATIC_INLINE__ uint16_t nrf_802154_imm_ack_duration_get(nrf_802154_phy_t phy)
{
    // SHR + PHR + ACK frame
    return nrf_802154_phy_shr_us_get(phy) +
           nrf_802154_phy_octets_to_us(PHR_SIZE + IMM_ACK_LENGTH, phy);
}

__STATIC_INLINE__ uint16_t nrf_802154_mac_imm_ack_wait_duration_get(nrf_802154_phy_t phy)
{
    return UNIT_BACKOFF_PERIOD + TURNAROUND_TIME + nrf_802154_imm_ack_duration_get(phy);
}

__STATIC_INLINE__ uint16_t nrf_802154_max_phy_frame_time_us_get(nrf_802154_phy_t phy)
{
    uint16_t max_packet_size = nrf_802154_max_psdu_size_get(phy);

    return nrf_802154_phy_shr_us_get(phy) +
           nrf_802154_phy_octets_to_us(PHR_SIZE + max_packet_size, phy);
}

__STATIC_INLINE__ uint16_t nrf_802154_phy_rx_end_event_latency_us_get(nrf_802154_phy_t phy)
{
    if (NRF_802154_GFSK_2MBPS_PHY_ENABLED && phy == NRF_802154_PHY_EXP1_GFSK_2MBPS)
    {
        return RX_PHYEND_EVENT_LATENCY_US_GFSK;
    }
    else
    {
        return RX_PHYEND_EVENT_LATENCY_US_OQPSK;
    }
}

__STATIC_INLINE__ uint16_t nrf_802154_phy_tx_address_to_mhr_us(nrf_802154_phy_t phy)
{
    if (NRF_802154_GFSK_2MBPS_PHY_ENABLED && phy == NRF_802154_PHY_EXP1_GFSK_2MBPS)
    {
        return PHY_GFSK_US_PER_OCTET;
    }
    else
    {
        return 2U * PHY_OQPSK_SYMBOLS_PER_OCTET * PHY_OQPSK_US_PER_SYMBOL;
    }
}

__STATIC_INLINE__ uint16_t nrf_802154_phy_tx_address_to_phr_us(nrf_802154_phy_t phy)
{
    if (NRF_802154_GFSK_2MBPS_PHY_ENABLED && phy == NRF_802154_PHY_EXP1_GFSK_2MBPS)
    {
        return 0U;
    }
    else
    {
        return PHY_OQPSK_SYMBOLS_PER_OCTET * PHY_OQPSK_US_PER_SYMBOL;
    }
}

#endif /* NRF_802154_PROCEDURES_DURATION_DECLARE_ONLY */

#endif /* NRF_802154_PROCEDURES_DURATION_H_ */
