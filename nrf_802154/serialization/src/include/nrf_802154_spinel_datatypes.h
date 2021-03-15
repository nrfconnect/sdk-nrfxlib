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

/**
 * @defgroup nrf_802154_spinel_serialization
 * 802.15.4 radio driver spinel serialization
 * @{
 *
 */

#ifndef NRF_802154_SPINEL_DATATYPES_H_
#define NRF_802154_SPINEL_DATATYPES_H_

#include "../spinel_base/spinel.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enum containing spinel custom vendor properties for nrf_802154 serialization.
 */
typedef enum
{
    /**
     * Vendor property group for Nordic begin.
     */
    SPINEL_PROP_VENDOR_NORDIC__BEGIN = SPINEL_PROP_VENDOR__BEGIN,

    /**
     * Vendor property group for Nordic nrf_802154 begin.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN = SPINEL_PROP_VENDOR_NORDIC__BEGIN,

    /**
     * Vendor property for nrf_802154_init serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_INIT =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 0,

    /**
     * Vendor property for nrf_802154_sleep serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SLEEP =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 1,

    /**
     * Vendor property for nrf_802154_receive serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 2,

    /**
     * Vendor property for nrf_802154_channel_get serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CHANNEL_GET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 3,

    /**
     * Vendor property for nrf_802154_channel_set serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CHANNEL_SET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 4,

    /**
     * Vendor property for nrf_802154_tx_power_get serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TX_POWER_GET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 5,

    /**
     * Vendor property for nrf_802154_tx_power_set serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TX_POWER_SET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 6,

    /**
     * Vendor property for nrf_802154_auto_pending_bit_set serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_AUTO_PENDING_BIT_SET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 7,

    /**
     * Vendor property for nrf_802154_pending_bit_for_addr_set serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PENDING_BIT_FOR_ADDR_SET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 8,

    /**
     * Vendor property for nrf_802154_pending_bit_for_addr_clear serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PENDING_BIT_FOR_ADDR_CLEAR =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 9,

    /**
     * Vendor property for nrf_802154_pending_bit_for_addr_reset serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PENDING_BIT_FOR_ADDR_RESET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 10,

    /**
     * Vendor property for nrf_802154_src_addr_matching_method_set serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SRC_ADDR_MATCHING_METHOD_SET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 11,

    /**
     * Vendor property for nrf_802154_pan_id_set serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PAN_ID_SET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 12,

    /**
     * Vendor property for nrf_802154_short_address_set serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SHORT_ADDRESS_SET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 13,

    /**
     * Vendor property for nrf_802154_extended_address_set serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_EXTENDED_ADDRESS_SET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 14,

    /**
     * Vendor property for nrf_802154_pan_coord_set serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PAN_COORD_SET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 15,

    /**
     * Vendor property for nrf_802154_promiscuous_set serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PROMISCUOUS_SET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 16,

    /**
     * Vendor property for nrf_802154_cca serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 17,

    /**
     * Vendor property for nrf_802154_cca_done serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA_DONE =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 18,

    /**
     * Vendor property for nrf_802154_cca_failed serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA_FAILED =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 19,

    /**
     * Vendor property for nrf_802154_energy_detection serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ENERGY_DETECTION =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 20,

    /**
     * Vendor property for nrf_802154_energy_detected serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ENERGY_DETECTED =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 21,

    /**
     * Vendor property for nrf_802154_energy_detection_failed serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ENERGY_DETECTION_FAILED =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 22,

    /**
     * Vendor property for nrf_802154_received_timestamp_raw serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVED_TIMESTAMP_RAW =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 23,

    /**
     * Vendor property for nrf_802154_receive_failed serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE_FAILED =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 24,

    /**
     * Vendor property for nrf_802154_tx_ack_started serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TX_ACK_STARTED =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 25,

    /**
     * Vendor property for nrf_802154_buffer_free_raw serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_BUFFER_FREE_RAW =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 26,

    /**
     * Vendor property for nrf_802154_transmit_raw serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_RAW =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 27,

    /**
     * Vendor property for nrf_802154_transmit_csma_ca_raw serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_CSMA_CA_RAW =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 28,

    /**
     * Vendor property for nrf_802154_transmitted_raw serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMITTED_RAW =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 29,

    /**
     * Vendor property for nrf_802154_transmit_failed serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_FAILED =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 30,

    /**
     * Vendor property for nrf_802154_capabilities_get serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CAPABILITIES_GET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 31,
} spinel_prop_vendor_key_t;

/**
 * @brief Spinel data type description for nrf_802154_channel_get
 */
#define SPINEL_DATATYPE_NRF_802154_CHANNEL_GET     SPINEL_DATATYPE_NULL_S

/**
 * @brief Spinel data type description for returning channel value.
 */
#define SPINEL_DATATYPE_NRF_802154_CHANNEL_GET_RET SPINEL_DATATYPE_UINT8_S

/**
 * @brief Spinel data type description for nrf_802154_channel_set
 */
#define SPINEL_DATATYPE_NRF_802154_CHANNEL_SET     SPINEL_DATATYPE_UINT8_S

/**
 * @brief Length of @ref SPINEL_DATATYPE_NRF_802154_HDATA_S data type.
 */
#define NRF_802154_HDATA_LENGTH(data_len)          ((data_len) + sizeof(uint32_t))

/**
 * @brief Encodes an instance of @ref SPINEL_DATATYPE_NRF_802154_HDATA_S data type.
 */
#define NRF_802154_HDATA_ENCODE(handle, data, length) \
    handle, data, NRF_802154_HDATA_LENGTH(length)

/**
 * @brief Decodes an instance of @ref SPINEL_DATATYPE_NRF_802154_HDATA_S data type.
 */
#define NRF_802154_HDATA_DECODE(handle, data, length) \
    (&handle), &data, &length

/**
 * @brief Calculates length of data stored by @ref SPINEL_DATATYPE_NRF_802154_HDATA_S data type.
 */
#define NRF_802154_DATA_LEN_FROM_HDATA_LEN(hdata_len) ((hdata_len) - sizeof(uint32_t))

/**
 * @brief Spinel data type description for a generic data with a handle.
 *
 * This type is used when data buffers are passed through serialization but
 * still need to be referenced e.g. in response processing to match response
 * data to a request. To serve this data handle field is used.
 */
#define SPINEL_DATATYPE_NRF_802154_HDATA_S          \
    SPINEL_DATATYPE_STRUCT_S(                       \
        SPINEL_DATATYPE_UINT32_S /* Data handle */  \
        SPINEL_DATATYPE_DATA_S   /* Data content */ \
                            )

/**
 * @brief Spinel data type description for SPINEL_PROP_LAST_STATUS.
 */
#define SPINEL_DATATYPE_SPINEL_PROP_LAST_STATUS            SPINEL_DATATYPE_UINT_PACKED_S

/**
 * @brief Spinel data type description for nrf_802154_sleep.
 */
#define SPINEL_DATATYPE_NRF_802154_SLEEP                   SPINEL_DATATYPE_NULL_S

/**
 * @brief Spinel data type description for nrf_802154_sleep result.
 */
#define SPINEL_DATATYPE_NRF_802154_SLEEP_RET               SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_receive.
 */
#define SPINEL_DATATYPE_NRF_802154_RECEIVE                 SPINEL_DATATYPE_NULL_S

/**
 * @brief Spinel data type description for nrf_802154_receive result.
 */
#define SPINEL_DATATYPE_NRF_802154_RECEIVE_RET             SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_pan_id_set.
 */
#define SPINEL_DATATYPE_NRF_802154_PAN_ID_SET              SPINEL_DATATYPE_DATA_S

/**
 * @brief Spinel data type description for nrf_802154_short_address_set.
 */
#define SPINEL_DATATYPE_NRF_802154_SHORT_ADDRESS_SET       SPINEL_DATATYPE_DATA_S

/**
 * @brief Spinel data type description for nrf_802154_extended_address_set.
 */
#define SPINEL_DATATYPE_NRF_802154_EXTENDED_ADDRESS_SET    SPINEL_DATATYPE_DATA_S

/**
 * @brief Spinel data type description for nrf_802154_pan_coord_set.
 */
#define SPINEL_DATATYPE_NRF_802154_PAN_COORD_SET           SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_promiscuous_set.
 */
#define SPINEL_DATATYPE_NRF_802154_PROMISCUOUS_SET         SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_cca.
 */
#define SPINEL_DATATYPE_NRF_802154_CCA                     SPINEL_DATATYPE_NULL_S

/**
 * @brief Spinel data type description for nrf_802154_cca result.
 */
#define SPINEL_DATATYPE_NRF_802154_CCA_RET                 SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_cca_done.
 */
#define SPINEL_DATATYPE_NRF_802154_CCA_DONE                SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_cca_failed.
 */
#define SPINEL_DATATYPE_NRF_802154_CCA_FAILED              SPINEL_DATATYPE_UINT8_S

/**
 * @brief Spinel data type description for nrf_802154_energy_detection.
 */
#define SPINEL_DATATYPE_NRF_802154_ENERGY_DETECTION        SPINEL_DATATYPE_UINT32_S

/**
 * @brief Spinel data type description for nrf_802154_energy_detection result.
 */
#define SPINEL_DATATYPE_NRF_802154_ENERGY_DETECTION_RET    SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_energy_detected.
 */
#define SPINEL_DATATYPE_NRF_802154_ENERGY_DETECTED         SPINEL_DATATYPE_UINT8_S

/**
 * @brief Spinel data type description for nrf_802154_energy_detection_failed.
 */
#define SPINEL_DATATYPE_NRF_802154_ENERGY_DETECTION_FAILED SPINEL_DATATYPE_UINT8_S

/**
 * @brief Spinel data type description for nrf_802154_tx_power_get.
 */
#define SPINEL_DATATYPE_NRF_802154_TX_POWER_GET            SPINEL_DATATYPE_NULL_S

/**
 * @brief Spinel data type description for nrf_802154_tx_power_get result.
 */
#define SPINEL_DATATYPE_NRF_802154_TX_POWER_GET_RET        SPINEL_DATATYPE_INT8_S

/**
 * @brief Spinel data type description for nrf_802154_tx_power_set.
 */
#define SPINEL_DATATYPE_NRF_802154_TX_POWER_SET            SPINEL_DATATYPE_INT8_S

/**
 * @brief Spinel data type description for nrf_802154_received_timestamp_raw
 */
#define SPINEL_DATATYPE_NRF_802154_RECEIVED_TIMESTAMP_RAW   \
    SPINEL_DATATYPE_NRF_802154_HDATA_S /* Received frame */ \
    SPINEL_DATATYPE_INT8_S             /* Power */          \
    SPINEL_DATATYPE_UINT8_S            /* lqi */            \
    SPINEL_DATATYPE_UINT32_S           /* timestamp */

/**
 * @brief Spinel data type description for nrf_802154_receive_failed
 */
#define SPINEL_DATATYPE_NRF_802154_RECEIVE_FAILED \
    SPINEL_DATATYPE_UINT8_S /* error */           \

/**
 * @brief Spinel data type description for nrf_802154_tx_ack_started
 */
#define SPINEL_DATATYPE_NRF_802154_TX_ACK_STARTED \
    SPINEL_DATATYPE_DATA_S /* Transmitted ACK */

/**
 * @brief Spinel data type description for nrf_802154_buffer_free_raw
 */
#define SPINEL_DATATYPE_NRF_802154_BUFFER_FREE_RAW \
    SPINEL_DATATYPE_UINT32_S /* Handle to buffer to free */

/**
 * @brief Spinel data type description for nrf_802154_transmit_raw
 */
#define SPINEL_DATATYPE_NRF_802154_TRANSMIT_RAW                                \
    SPINEL_DATATYPE_NRF_802154_HDATA_S /* Frame to transmit with its handle */ \
    SPINEL_DATATYPE_BOOL_S             /* CCA */

/**
 * @brief Spinel data type description for return value of nrf_802154_transmit_raw
 */
#define SPINEL_DATATYPE_NRF_802154_TRANSMIT_RAW_RET               SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type desription for nrf_802154_auto_pending_bit_set.
 */
#define SPINEL_DATATYPE_NRF_802154_AUTO_PENDING_BIT_SET           SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type desription for nrf_802154_pending_bit_for_addr_set.
 *
 * SPINEL_DATATYPE_ARRAY_S encoding is not implemented, SPINEL_DATATYPE_DATA_S has to be used instead.
 */
#define SPINEL_DATATYPE_NRF_802154_PENDING_BIT_FOR_ADDR_SET       SPINEL_DATATYPE_DATA_S

/**
 * @brief Spinel data type desription for nrf_802154_pending_bit_for_addr_set return value.
 */
#define SPINEL_DATATYPE_NRF_802154_PENDING_BIT_FOR_ADDR_SET_RET   SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type desription for nrf_802154_pending_bit_for_addr_clear.
 *
 * SPINEL_DATATYPE_ARRAY_S encoding is not implemented, SPINEL_DATATYPE_DATA_S has to be used instead.
 */
#define SPINEL_DATATYPE_NRF_802154_PENDING_BIT_FOR_ADDR_CLEAR     SPINEL_DATATYPE_DATA_S

/**
 * @brief Spinel data type desription for nrf_802154_pending_bit_for_addr_set return value.
 */
#define SPINEL_DATATYPE_NRF_802154_PENDING_BIT_FOR_ADDR_CLEAR_RET SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type desription for nrf_802154_pending_bit_for_addr_reset.
 */
#define SPINEL_DATATYPE_NRF_802154_PENDING_BIT_FOR_ADDR_RESET     SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type desription for nrf_802154_src_addr_matching_method_set.
 */
#define SPINEL_DATATYPE_NRF_802154_SRC_ADDR_MATCHING_METHOD_SET   SPINEL_DATATYPE_UINT8_S

/**
 * @brief Spinel data type description for nrf_802154_transmit_csma_ca_raw.
 */
#define SPINEL_DATATYPE_NRF_802154_TRANSMIT_CSMA_CA_RAW \
    SPINEL_DATATYPE_NRF_802154_HDATA_S /* Frame to transmit with its handle */

/**
 * @brief Spinel data type description for nrf_802154_transmitted_raw.
 */
#define SPINEL_DATATYPE_NRF_802154_TRANSMITTED_RAW                       \
    SPINEL_DATATYPE_UINT32_S           /* Handle to transmitted frame */ \
    SPINEL_DATATYPE_NRF_802154_HDATA_S /* Ack frame with its handle */   \
    SPINEL_DATATYPE_INT8_S             /* Power */                       \
    SPINEL_DATATYPE_UINT8_S            /* LQI */

/**
 * @brief Spinel data type description for nrf_802154_transmit_failed.
 */
#define SPINEL_DATATYPE_NRF_802154_TRANSMIT_FAILED             \
    SPINEL_DATATYPE_UINT32_S /* Handle to transmitted frame */ \
    SPINEL_DATATYPE_UINT8_S  /* Error code */

/**
 * @brief Spinel data type description for nrf_802154_capabilities_get.
 */
#define SPINEL_DATATYPE_NRF_802154_CAPABILITIES_GET     SPINEL_DATATYPE_NULL_S

/**
 * @brief Spinel data type description for nrf_802154_capabilities_get_ret.
 */
#define SPINEL_DATATYPE_NRF_802154_CAPABILITIES_GET_RET SPINEL_DATATYPE_UINT32_S

#ifdef __cplusplus
}
#endif

#endif /* NRF_802154_SPINEL_DATATYPES_H_ */

/** @} */
