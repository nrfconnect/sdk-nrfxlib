/*
 * Copyright (c) 2020 - 2022, Nordic Semiconductor ASA
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
 * @brief Possible errors during sleep procedure call.
 */
typedef uint8_t nrf_802154_sleep_error_t;

#define NRF_802154_SLEEP_ERROR_NONE 0x00 // !< There is no error.
#define NRF_802154_SLEEP_ERROR_BUSY 0x01 // !< The driver cannot enter the sleep state due to the ongoing operation.

/**
 * @brief Possible errors during the CCA procedure.
 */
typedef uint8_t nrf_802154_cca_error_t;

#define NRF_802154_CCA_ERROR_ABORTED      0x01 // !< Procedure was aborted by another operation.

/** @brief RADIO Clear Channel Assessment modes. */
#define NRF_RADIO_CCA_MODE_ED             0x00
#define NRF_RADIO_CCA_MODE_CARRIER        0x01
#define NRF_RADIO_CCA_MODE_CARRIER_AND_ED 0x02
#define NRF_RADIO_CCA_MODE_CARRIER_OR_ED  0x03

/**
 * @brief Structure for configuring CCA.
 */
typedef struct
{
    uint8_t mode;           // !< CCA mode.
    uint8_t ed_threshold;   // !< Busy threshold of the CCA energy. Not used in @ref NRF_RADIO_CCA_MODE_CARRIER.
    uint8_t corr_threshold; // !< Busy threshold of the CCA correlator. Not used in @ref NRF_RADIO_CCA_MODE_ED.
    uint8_t corr_limit;     // !< Limit of occurrences above the busy threshold of the CCA correlator. Not used in @ref NRF_RADIO_CCA_MODE_ED.
} nrf_802154_cca_cfg_t;

/**
 * @brief Types of data that can be set in an ACK message.
 */
typedef uint8_t nrf_802154_ack_data_t;

#define NRF_802154_ACK_DATA_PENDING_BIT 0x00
#define NRF_802154_ACK_DATA_IE          0x01

/**
 * @brief Type holding the CSMA/CA backoff control test mode
 *
 * Possible values:
 * - @ref NRF_802154_TEST_MODE_CSMACA_BACKOFF_RANDOM
 * - @ref NRF_802154_TEST_MODE_CSMACA_BACKOFF_ALWAYS_MAX
 * - @ref NRF_802154_TEST_MODE_CSMACA_BACKOFF_ALWAYS_MIN
 */
typedef uint8_t nrf_802154_test_mode_csmaca_backoff_t;

#define NRF_802154_TEST_MODE_CSMACA_BACKOFF_RANDOM     0x00 // !< The CSMA/CA uses random number of backoff periods (IEEE Std. 802.15.4 compliant)
#define NRF_802154_TEST_MODE_CSMACA_BACKOFF_ALWAYS_MAX 0x01 // !< The CSMA/CA uses always maximum backoff periods (Test mode, non-compliant to IEEE Std. 802.15.4)
#define NRF_802154_TEST_MODE_CSMACA_BACKOFF_ALWAYS_MIN 0x02 // !< The CSMA/CA uses always minimum backoff periods (Test mode, non-compliant to IEEE Std. 802.15.4)

/**
 * @brief Methods of source address matching.
 *
 * You can use one of the following methods that can be set during the initialization phase
 * by calling @ref nrf_802154_src_addr_matching_method_set:
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
 * @brief Type of structure holding time stamps of certain events.
 */
typedef struct
{
    /**@brief Time stamp of last CSMA/CA procedure started. */
    uint64_t last_csmaca_start_timestamp;
    /**@brief Time stamp of last CCA start attempt. */
    uint64_t last_cca_start_timestamp;
    /**@brief Time stamp of last CCA attempt finished with CCA IDLE (channel was free to transmit). */
    uint64_t last_cca_idle_timestamp;
    /**@brief Time stamp when last bit of transmitted frame was sent on the air. */
    uint64_t last_tx_end_timestamp;
    /**@brief Time stamp when last bit of acknowledge frame was received */
    uint64_t last_ack_end_timestamp;
    /**@brief Time stamp when last bit of received frame was received. */
    uint64_t last_rx_end_timestamp;
} nrf_802154_stat_timestamps_t;

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
 * @brief Structure with transmit request metadata for scheduling transmission at a specific time.
 */
typedef struct
{
    nrf_802154_transmitted_frame_props_t frame_props; // !< Properties of the frame to be transmitted.
    bool                                 cca;         // !< If the driver is to perform a CCA procedure before transmission.
    uint8_t                              channel;     // !< Radio channel on which the frame is to be transmitted.
} nrf_802154_transmit_at_metadata_t;

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
            uint64_t time;   // !< Timestamp taken when the last symbol of ACK is received. If @ref p_ack is NULL, this field is set to 0, but is considered invalid.
        } transmitted;       // !< Result values for a successful frame transmission.
    } data;                  // !< Result values that are valid only for successful operations.
} nrf_802154_transmit_done_metadata_t;

/**
 * @brief Possible errors during key handling.
 */
typedef uint8_t nrf_802154_security_error_t;

#define NRF_802154_SECURITY_ERROR_NONE                   0x00 // !< There is no error.
#define NRF_802154_SECURITY_ERROR_STORAGE_FULL           0x01 // !< The key storage is full - removal of stored keys is needed.
#define NRF_802154_SECURITY_ERROR_KEY_NOT_FOUND          0x02 // !< The provided key was not found inside the storage.
#define NRF_802154_SECURITY_ERROR_ALREADY_PRESENT        0x03 // !< The storage already has the key of the same ID.
#define NRF_802154_SECURITY_ERROR_TYPE_NOT_SUPPORTED     0x04 // !< The provided key type is not supported.
#define NRF_802154_SECURITY_ERROR_MODE_NOT_SUPPORTED     0x05 // !< The provided key id mode is not supported.
#define NRF_802154_SECURITY_ERROR_FRAME_COUNTER_OVERFLOW 0x06 // !< The associated frame counter overflowed.

/**
 * @brief Types of keys which can be used with the nRF 802.15.4 Radio Driver.
 *
 * Possible values:
 * - @ref NRF_802154_KEY_CLEARTEXT,
 *
 */
typedef uint32_t nrf_802154_key_type_t;

#define NRF_802154_KEY_CLEARTEXT 0x00 // !< Key stored in clear text.

/**
 * @brief Type holding the value of Key Id Mode of the key stored in nRF 802.15.4 Radio Driver.
 */
typedef uint8_t nrf_802154_key_id_mode_t;

/**
 * @brief Type holding the value of Key Id for the keys stored in nRF 802.15.4 Radio Driver.
 */
typedef struct
{
    nrf_802154_key_id_mode_t mode;     // !< Key Id Mode (0..3)
    uint8_t                * p_key_id; // !< Pointer to the Key Id field
} nrf_802154_key_id_t;

/**
 * @brief Type of structure holding a 802.15.4 MAC Security Key.
 */
typedef struct
{
    union
    {
        uint8_t * p_cleartext_key;                  // !< Pointer to the cleartext representation of the key.
    }                     value;                    // !< Union holding different representations of the key.
    nrf_802154_key_id_t   id;                       // !< Key Id of the key.
    nrf_802154_key_type_t type;                     // !< @ref nrf_802154_key_type_t type of the key used.
    uint32_t              frame_counter;            // !< Frame counter to use in case @ref use_global_frame_counter is set to false.
    bool                  use_global_frame_counter; // !< Whether to use the global frame counter instead of the one defined in this structure.
} nrf_802154_key_t;

/**
 *@}
 **/

#endif // NRF_802154_TYPES_H__
