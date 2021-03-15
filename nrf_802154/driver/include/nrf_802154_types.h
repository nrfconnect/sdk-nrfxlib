/*
 * Copyright (c) 2018 - 2021, Nordic Semiconductor ASA
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

#ifndef NRF_802154_TYPES_H__
#define NRF_802154_TYPES_H__

#include <stdint.h>

/**
 * Avoid including nrfx dependencies in a unit test build.
 */
#if !defined(UNIT_TEST)
    #include "hal/nrf_radio.h"
#else
typedef uint8_t nrf_radio_cca_mode_t;

#endif

/**
 * @defgroup nrf_802154_types Type definitions used in the 802.15.4 driver
 * @{
 * @ingroup nrf_802154
 * @brief Definitions of types used in the 802.15.4 driver.
 */

/**
 * @brief States of the driver.
 */
typedef uint8_t nrf_802154_state_t;

#define NRF_802154_STATE_INVALID            0x01 // !< Radio in an invalid state.
#define NRF_802154_STATE_SLEEP              0x02 // !< Radio in the sleep state.
#define NRF_802154_STATE_RECEIVE            0x03 // !< Radio in the receive state.
#define NRF_802154_STATE_TRANSMIT           0x04 // !< Radio in the transmit state.
#define NRF_802154_STATE_ENERGY_DETECTION   0x05 // !< Radio in the energy detection state.
#define NRF_802154_STATE_CCA                0x06 // !< Radio in the CCA state.
#define NRF_802154_STATE_CONTINUOUS_CARRIER 0x07 // !< Radio in the continuous carrier state.
#define NRF_802154_STATE_MODULATED_CARRIER  0x08 // !< Radio in the modulated carrier state.

/**
 * @brief Errors reported during the frame transmission.
 */
typedef uint8_t nrf_802154_tx_error_t;

#define NRF_802154_TX_ERROR_NONE            0x00 // !< There is no transmit error.
#define NRF_802154_TX_ERROR_BUSY_CHANNEL    0x01 // !< CCA reported busy channel before the transmission.
#define NRF_802154_TX_ERROR_INVALID_ACK     0x02 // !< Received ACK frame is other than expected.
#define NRF_802154_TX_ERROR_NO_MEM          0x03 // !< No receive buffer is available to receive an ACK.
#define NRF_802154_TX_ERROR_TIMESLOT_ENDED  0x04 // !< Radio timeslot ended during the transmission procedure.
#define NRF_802154_TX_ERROR_NO_ACK          0x05 // !< ACK frame was not received during the timeout period.
#define NRF_802154_TX_ERROR_ABORTED         0x06 // !< Procedure was aborted by another operation.
#define NRF_802154_TX_ERROR_TIMESLOT_DENIED 0x07 // !< Transmission did not start due to a denied timeslot request.

/**
 * @brief Possible errors during the frame reception.
 */
typedef uint8_t nrf_802154_rx_error_t;

#define NRF_802154_RX_ERROR_NONE                    0x00 // !< There is no receive error.
#define NRF_802154_RX_ERROR_INVALID_FRAME           0x01 // !< Received a malformed frame.
#define NRF_802154_RX_ERROR_INVALID_FCS             0x02 // !< Received a frame with an invalid checksum.
#define NRF_802154_RX_ERROR_INVALID_DEST_ADDR       0x03 // !< Received a frame with a mismatched destination address.
#define NRF_802154_RX_ERROR_RUNTIME                 0x04 // !< Runtime error occurred (for example, CPU was held for too long).
#define NRF_802154_RX_ERROR_TIMESLOT_ENDED          0x05 // !< Radio timeslot ended during the frame reception.
#define NRF_802154_RX_ERROR_ABORTED                 0x06 // !< Procedure was aborted by another operation.
#define NRF_802154_RX_ERROR_DELAYED_TIMESLOT_DENIED 0x07 // !< Delayed reception request was rejected due to a denied timeslot request.
#define NRF_802154_RX_ERROR_DELAYED_TIMEOUT         0x08 // !< Delayed reception timeslot ended.
#define NRF_802154_RX_ERROR_INVALID_LENGTH          0x09 // !< Received a frame with invalid length.
#define NRF_802154_RX_ERROR_DELAYED_ABORTED         0x0A // !< Delayed operation in the ongoing state was aborted by other request.

/**
 * @brief Possible errors during the energy detection.
 */
typedef uint8_t nrf_802154_ed_error_t;

#define NRF_802154_ED_ERROR_ABORTED 0x01 // !< Procedure was aborted by another operation.

/**
 * @brief Possible errors during the CCA procedure.
 */
typedef uint8_t nrf_802154_cca_error_t;

#define NRF_802154_CCA_ERROR_ABORTED 0x01 // !< Procedure was aborted by another operation.

/**
 * @brief Possible errors during sleep procedure call.
 */
typedef uint8_t nrf_802154_sleep_error_t;

#define NRF_802154_SLEEP_ERROR_NONE 0x00 // !< There is no error.
#define NRF_802154_SLEEP_ERROR_BUSY 0x01 // !< The driver cannot enter the sleep state due to the ongoing operation.

/**
 * @brief Termination level selected for a particular request.
 *
 * Each request can terminate an ongoing operation. This type selects which operation should be
 * aborted by a given request.
 */
typedef uint8_t nrf_802154_term_t;

#define NRF_802154_TERM_NONE   0x00 // !< Request is skipped if another operation is ongoing.
#define NRF_802154_TERM_802154 0x01 // !< Request terminates the ongoing 802.15.4 operation.

/**
 * @brief Structure for configuring CCA.
 */
typedef struct
{
    nrf_radio_cca_mode_t mode;           // !< CCA mode.
    uint8_t              ed_threshold;   // !< Busy threshold of the CCA energy. Not used in @ref NRF_RADIO_CCA_MODE_CARRIER.
    uint8_t              corr_threshold; // !< Busy threshold of the CCA correlator. Not used in @ref NRF_RADIO_CCA_MODE_ED.
    uint8_t              corr_limit;     // !< Limit of occurrences above the busy threshold of the CCA correlator. Not used in @ref NRF_RADIO_CCA_MODE_ED.
} nrf_802154_cca_cfg_t;

/**
 * @brief Types of data that can be set in an ACK message.
 */
typedef uint8_t nrf_802154_ack_data_t;

#define NRF_802154_ACK_DATA_PENDING_BIT 0x00
#define NRF_802154_ACK_DATA_IE          0x01

/**
 * @brief Methods of source address matching.
 *
 * You can use one of the following methods that can be set during the initialization phase
 * by calling @ref nrf_802154_src_matching_method:
 *   - For Thread: @ref NRF_802154_SRC_ADDR_MATCH_THREAD -- The pending bit is set only for the addresses found in the list.
 *   - For Zigbee: @ref NRF_802154_SRC_ADDR_MATCH_ZIGBEE -- The pending bit is cleared only for the short addresses found in the list.\n
 *     This method does not set pending bit in non-command and non-data-request frames.
 *   - For standard-compliant implementation: @ref NRF_802154_SRC_ADDR_MATCH_ALWAYS_1 -- The pending bit is always set to 1.\n
 *     This requires an empty data frame with AR set to 0 to be transmitted immediately afterwards.
 */
typedef uint8_t nrf_802154_src_addr_match_t;

#define NRF_802154_SRC_ADDR_MATCH_THREAD   0x00 // !< Implementation for the Thread protocol.
#define NRF_802154_SRC_ADDR_MATCH_ZIGBEE   0x01 // !< Implementation for the Zigbee protocol.
#define NRF_802154_SRC_ADDR_MATCH_ALWAYS_1 0x02 // !< Standard compliant implementation.

/**
 * @brief RSSI measurement results.
 */

#define NRF_802154_RSSI_INVALID INT8_MAX

/**
 * @brief Mode of triggering receive request to Coex arbiter.
 *
 * Possible values:
 * - @ref NRF_802154_COEX_RX_REQUEST_MODE_ENERGY_DETECTION,
 * - @ref NRF_802154_COEX_RX_REQUEST_MODE_PREAMBLE,
 * - @ref NRF_802154_COEX_RX_REQUEST_MODE_DESTINED
 */
typedef uint8_t nrf_802154_coex_rx_request_mode_t;

#define NRF_802154_COEX_RX_REQUEST_MODE_ENERGY_DETECTION 0x01 // !< Coex requests to arbiter in receive mode upon energy detected.
#define NRF_802154_COEX_RX_REQUEST_MODE_PREAMBLE         0x02 // !< Coex requests to arbiter in receive mode upon preamble reception.
#define NRF_802154_COEX_RX_REQUEST_MODE_DESTINED         0x03 // !< Coex requests to arbiter in receive mode upon detection that frame is addressed to this device.

/**
 * @brief Mode of triggering transmit request to Coex arbiter.
 *
 * Possible values:
 * - @ref NRF_802154_COEX_TX_REQUEST_MODE_FRAME_READY,
 * - @ref NRF_802154_COEX_TX_REQUEST_MODE_CCA_START,
 * - @ref NRF_802154_COEX_TX_REQUEST_MODE_CCA_DONE
 */
typedef uint8_t nrf_802154_coex_tx_request_mode_t;

#define NRF_802154_COEX_TX_REQUEST_MODE_FRAME_READY 0x01 // !< Coex requests to arbiter in transmit mode when the frame is ready to be transmitted.
#define NRF_802154_COEX_TX_REQUEST_MODE_CCA_START   0x02 // !< Coex requests to arbiter in transmit mode before CCA is started.
#define NRF_802154_COEX_TX_REQUEST_MODE_CCA_DONE    0x03 // !< Coex requests to arbiter in transmit mode after CCA is finished.

/**
 * @brief Mode of handling Interframe spacing.
 *
 * Possible values:
 * - @ref NRF_802154_IFS_MODE_DISABLED,
 * - @ref NRF_802154_IFS_MODE_MATCHING_ADDRESSES,
 * - @ref NRF_802154_IFS_MODE_ALWAYS
 */
typedef uint8_t nrf_802154_ifs_mode_t;

#define NRF_802154_IFS_MODE_DISABLED           0x00 // !< Interframe spacing is never inserted.
#define NRF_802154_IFS_MODE_MATCHING_ADDRESSES 0x01 // !< Interframe spacing is inserted only on matching addresses.
#define NRF_802154_IFS_MODE_ALWAYS             0x02 // !< Interframe spacing is always inserted.

/**
 * @brief Capabilities of nrf 802.15.4 radio driver
 *
 * Possible values:
 * - @ref NRF_802154_CAPABILITY_CSMA,
 * - @ref NRF_802154_CAPABILITY_DELAYED_TX,
 * - @ref NRF_802154_CAPABILITY_DELAYED_RX,
 * - @ref NRF_802154_CAPABILITY_ACK_TIMEOUT,
 * - @ref NRF_802154_CAPABILITY_ANT_DIVERSITY,
 * - @ref NRF_802154_CAPABILITY_IFS,
 * - @ref NRF_802154_CAPABILITY_TIMESTAMP
 *
 */
typedef uint32_t nrf_802154_capabilities_t;

#define NRF_802154_CAPABILITY_CSMA          (1UL << 0UL) // !< CSMA-CA supported
#define NRF_802154_CAPABILITY_DELAYED_TX    (1UL << 1UL) // !< TX at specified time supported
#define NRF_802154_CAPABILITY_DELAYED_RX    (1UL << 2UL) // !< RX at specified time supported
#define NRF_802154_CAPABILITY_ACK_TIMEOUT   (1UL << 3UL) // !< ACK timeout supported
#define NRF_802154_CAPABILITY_ANT_DIVERSITY (1UL << 4UL) // !< Antenna diversity supported
#define NRF_802154_CAPABILITY_IFS           (1UL << 5UL) // !< Inter-frame spacing supported
#define NRF_802154_CAPABILITY_TIMESTAMP     (1UL << 6UL) // !< Frame timestamping supported

/**
 * @brief Type of structure holding statistic counters.
 *
 * This structure holds counters of @c uint32_t type only.
 */
typedef struct
{
    /**@brief Number of failed CCA attempts. */
    uint32_t cca_failed_attempts;
    /**@brief Number of frames received with correct CRC and with filtering passing. */
    uint32_t received_frames;
    /**@brief Number of times energy was detected in receive mode.*/
    uint32_t received_energy_events;
    /**@brief Number of times a preamble was received in receive mode. */
    uint32_t received_preambles;
    /**@brief Number of coex requests issued to coex arbiter. */
    uint32_t coex_requests;
    /**@brief Number of coex requests issued to coex arbiter that have been granted. */
    uint32_t coex_granted_requests;
    /**@brief Number of coex requests issued to coex arbiter that have been denied. */
    uint32_t coex_denied_requests;
    /**@brief Number of coex grant activations that have been not requested. */
    uint32_t coex_unsolicited_grants;
} nrf_802154_stat_counters_t;

/**
 * @brief Type of structure holding time stamps of certain events.
 */
typedef struct
{
    /**@brief Time stamp of last CSMA/CA procedure started. */
    uint32_t last_csmaca_start_timestamp;
    /**@brief Time stamp of last CCA start attempt. */
    uint32_t last_cca_start_timestamp;
    /**@brief Time stamp of last CCA attempt finished with CCA IDLE (channel was free to transmit). */
    uint32_t last_cca_idle_timestamp;
    /**@brief Time stamp when last bit of transmitted frame was sent on the air. */
    uint32_t last_tx_end_timestamp;
    /**@brief Time stamp when last bit of acknowledge frame was received */
    uint32_t last_ack_end_timestamp;
    /**@brief Time stamp when last bit of received frame was received. */
    uint32_t last_rx_end_timestamp;
} nrf_802154_stat_timestamps_t;

/**
 * @brief Type of structure holding total times spent in certain states.
 *
 * This structure holds fields of @c uint64_t type only.
 */
typedef struct
{
    /**@brief Total time in microseconds spent with receiver turned on, but not actually receiving any frames. */
    uint64_t total_listening_time;
    /**@brief Total time in microseconds spent with receiver turned on and actually receiving frames. */
    uint64_t total_receive_time;
    /**@brief Total time in microseconds spent on transmission. */
    uint64_t total_transmit_time;
} nrf_802154_stat_totals_t;

/**
 * @brief Type of structure holding statistics about the Radio Driver behavior.
 */
typedef struct
{
    /**@brief Statistic counters */
    nrf_802154_stat_counters_t   counters;

    /**@brief Time stamps of events */
    nrf_802154_stat_timestamps_t timestamps;
} nrf_802154_stats_t;

/**
 *@}
 **/

#endif // NRF_802154_TYPES_H__
