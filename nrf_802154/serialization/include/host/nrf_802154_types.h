/*
 * Copyright (c) 2020 - 2021, Nordic Semiconductor ASA
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

#include <stdbool.h>
#include <stdint.h>

/**
 * @defgroup nrf_802154_types Type definitions used in the 802.15.4 driver
 * @{
 * @ingroup nrf_802154
 * @brief Definitions of types used in the 802.15.4 driver.
 */

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
#define NRF_802154_TX_ERROR_TIMEOUT         0x08 // !< Timeout specified for a transmission has been reached.

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
 */
typedef uint8_t nrf_802154_ed_error_t;

#define NRF_802154_ED_ERROR_ABORTED 0x01 // !< Procedure was aborted by another operation.

/**
 * @brief Possible errors during the CCA procedure.
 */
typedef uint8_t nrf_802154_cca_error_t;

#define NRF_802154_CCA_ERROR_ABORTED 0x01 // !< Procedure was aborted by another operation.

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
 * @brief Capabilites of nrf 802.15.4 radio driver
 *
 * Possible values:
 * - @ref NRF_802154_CAPABILITY_CSMA,
 * - @ref NRF_802154_CAPABILITY_DELAYED_TX,
 * - @ref NRF_802154_CAPABILITY_DELAYED_RX,
 * - @ref NRF_802154_CAPABILITY_ACK_TIMEOUT,
 * - @ref NRF_802154_CAPABILITY_ANT_DIVERSITY,
 * - @ref NRF_802154_CAPABILITY_IFS,
 * - @ref NRF_802154_CAPABILITY_TIMESTAMP
 * - @ref NRF_802154_CAPABILITY_SECURITY
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
#define NRF_802154_CAPABILITY_SECURITY      (1UL << 7UL) // !< Frame security supported

/**
 * @brief Structure with frame properties associated with the transmission operation.
 *
 * @note When using to request transmission, parameters contained here influence whether or not
 *       the security related data transformation will be performed. In particular, the driver may:
 *        - update frame counter field if the dynamic parts of the frame are not yet updated
 *        - secure a non-secured frame on-the-fly
 *       If performed, the above operations are configured by the IEEE 802.15.4 Auxiliary Security
 *       Header present in the transmitted frame.
 *
 * @note It is recommended to use values defined in @ref NRF_802154_TRANSMITTED_FRAME_PROPS_DEFAULT_INIT
 *       to perform the first transmission attempt. Using other values may result in transmitting
 *       malformed or incorrect frames and creating security breaches.
 *
 * @note The combination with is_secured = true and dynamic_data_is_set = false is not allowed.
 *       An attempt to transmit a frame with such parameters will fail unconditionally.
 */
typedef struct
{
    bool is_secured;          // !< If the frame to be transmitted is already secured (in the sense of IEEE 802.15.4 security operations).
    bool dynamic_data_is_set; // !< If dynamic data of the frame frame to be transmitted is set.
} nrf_802154_transmitted_frame_props_t;

/**
 * @brief Default initializer for nrf_802154_transmitted_frame_props_t.
 */
#define NRF_802154_TRANSMITTED_FRAME_PROPS_DEFAULT_INIT \
    (nrf_802154_transmitted_frame_props_t){             \
        .is_secured          = false,                   \
        .dynamic_data_is_set = false                    \
    }

/**
 * @brief Structure with transmit request metadata for simple transmission request.
 */
typedef struct
{
    nrf_802154_transmitted_frame_props_t frame_props; // !< Properties of the frame to be transmitted.
    bool                                 cca;         // !< If the driver is to perform a CCA procedure before transmission.
} nrf_802154_transmit_metadata_t;

/**
 * @brief Structure with transmit request metadata for transmission preceded by CSMA-CA procedure.
 */
typedef struct
{
    nrf_802154_transmitted_frame_props_t frame_props; // !< Properties of the frame to be transmitted.
} nrf_802154_transmit_csma_ca_metadata_t;

/**
 * @brief Structure that holds transmission result metadata.
 */
typedef struct
{
    nrf_802154_transmitted_frame_props_t frame_props; // !< Properties of the returned frame.

    union
    {
        struct
        {
            uint8_t * p_ack; // !< If NRF_802154_USE_RAW_API is disabled, p_ack is a pointer to a buffer that contains only the received ACK payload (PSDU excluding FCS).
                             // If NRF_802154_USE_RAW_API is enabled, p_ack is a pointer to a buffer that contains PHR and PSDU of the received ACK. The first byte
                             // in the buffer is the length of the frame (PHR). The following bytes contain the ACK frame itself (PSDU). The length byte
                             // (PHR) includes FCS. FCS is already verified by the hardware and may be modified by the hardware.
                             // If ACK was not requested or requested but not received, @ref p_ack is set to NULL.
            uint8_t  length; // !< Length of the received ACK payload or 0 if @ref p_ack is NULL.
            int8_t   power;  // !< RSSI of the received frame or 0 if @ref p_ack is NULL.
            uint8_t  lqi;    // !< LQI of the received frame or 0 if @ref p_ack is NULL.
            uint32_t time;   // !< Timestamp taken when the last symbol of ACK is received. If @ref p_ack is NULL, this field is set to 0, but is considered invalid.
        } transmitted;       // !< Result values for a successful frame transmission.
    } data;                  // !< Result values that are valid only for successful operations.
} nrf_802154_transmit_done_metadata_t;

/**
 *@}
 **/

#endif // NRF_802154_TYPES_H__
