/*
 * Copyright (c) 2020, Nordic Semiconductor ASA
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

    /**
     * Vendor property for nrf_802154_ack_data_set serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ACK_DATA_SET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 32,

    /**
     * Vendor property for nrf_802154_ack_data_clear serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ACK_DATA_CLEAR =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 33,

    /**
     * Vendor property for nrf_802154_time_get serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TIME_GET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 34,

    /**
     * Vendor property for nrf_802154_cca_cfg_get serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA_CFG_GET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 35,

    /**
     * Vendor property for nrf_802154_cca_cfg_set serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA_CFG_SET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 36,

    /**
     * Vendor property for nrf_802154_transmit_raw_at serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_RAW_AT =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 37,

    /**
     * Vendor property for nrf_802154_transmit_at_cancel serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_AT_CANCEL =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 38,

    /**
     * Vendor property for nrf_802154_receive_at serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE_AT =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 39,

    /**
     * Vendor property for nrf_802154_receive_at_cancel serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE_AT_CANCEL =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 40,

    /**
     * Vendor property for nrf_802154_security_global_frame_counter_set serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SECURITY_GLOBAL_FRAME_COUNTER_SET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 41,

    /**
     * Vendor property for nrf_802154_security_key_store serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SECURITY_KEY_STORE =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 42,

    /**
     * Vendor property for nrf_802154_security_key_remove serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SECURITY_KEY_REMOVE =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 43,

    /**
     * Vendor property for nrf_802154_csl_writer_period_set serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSL_WRITER_PERIOD_SET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 44,

    /**
     * Vendor property for nrf_802154_csma_ca_min_be_set serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MIN_BE_SET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 45,

    /**
     * Vendor property for nrf_802154_csma_ca_min_be_get serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MIN_BE_GET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 46,

    /**
     * Vendor property for nrf_802154_csma_ca_max_be_set serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MAX_BE_SET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 47,

    /**
     * Vendor property for nrf_802154_csma_ca_max_be_get serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MAX_BE_GET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 48,

    /**
     * Vendor property for nrf_802154_csma_ca_max_backoffs_set serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MAX_BACKOFFS_SET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 49,

    /**
     * Vendor property for nrf_802154_csma_ca_max_backoffs_get serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MAX_BACKOFFS_GET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 50,

    /**
     * Vendor property for nrf_802154_stat_timestamps_get serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_STAT_TIMESTAMPS_GET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 51,

    /**
     * Vendor property for nrf_802154_continuous_carrier serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CONTINUOUS_CARRIER =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 52,

    /**
     * Vendor property for nrf_802154_modulated_carrier serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_MODULATED_CARRIER =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 53,

    /**
     * Vendor property for nrf_802154_sleep_if_idle serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SLEEP_IF_IDLE =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 54,

    /**
     * Vendor property for nrf_802154_test_mode_csmaca_backoff_set serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TEST_MODE_CSMACA_BACKOFF_SET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 55,

    /**
     * Vendor property for nrf_802154_test_mode_csmaca_backoff_get serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TEST_MODE_CSMACA_BACKOFF_GET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 56,

    /**
     * Vendor property for nrf_802154_ifs_mode_set serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MODE_SET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 57,

    /**
     * Vendor property for nrf_802154_ifs_mode_get serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MODE_GET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 58,

    /**
     * Vendor property for nrf_802154_ifs_min_sifs_period_set serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MIN_SIFS_PERIOD_SET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 59,

    /**
     * Vendor property for nrf_802154_ifs_min_sifs_period_get serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MIN_SIFS_PERIOD_GET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 60,

    /**
     * Vendor property for nrf_802154_ifs_min_lifs_period_set serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MIN_LIFS_PERIOD_SET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 61,

    /**
     * Vendor property for nrf_802154_ifs_min_lifs_period_get serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MIN_LIFS_PERIOD_GET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 62,

    /**
     * Vendor property for nrf_802154_csl_writer_anchor_time_set serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSL_WRITER_ANCHOR_TIME_SET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 63,

    /**
     * Vendor property for nrf_802154_security_global_frame_counter_set_if_larger serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SECURITY_GLOBAL_FRAME_COUNTER_SET_IF_LARGER =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 64,

    /**
     * Vendor property for nrf_802154_pan_coord_get serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PAN_COORD_GET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 65,

    /**
     * Vendor property for nrf_802154_security_key_remove_all serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SECURITY_KEY_REMOVE_ALL =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 66,

    /**
     * Vendor property for nrf_802154_rx_on_when_idle_set serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RX_ON_WHEN_IDLE_SET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 67,

    /**
     * Vendor property for nrf_802154_ack_data_remove_all serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ACK_DATA_REMOVE_ALL =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 68,

    /**
     * Vendor property for nrf_802154_receive_at_scheduled_cancel serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE_AT_SCHEDULED_CANCEL =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 69,

    /**
     * Vendor property for nrf_802154_alternate_short_address_set serialization.
     */
    SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ALTERNATE_SHORT_ADDRESS_SET =
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154__BEGIN + 70,
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
 * @brief Spinel data type description for nrf_802154_transmitted_frame_props_t.
 */
#define SPINEL_DATATYPE_NRF_802154_TRANSMITTED_FRAME_PROPS_S \
    SPINEL_DATATYPE_BOOL_S /* is_secured */                  \
    SPINEL_DATATYPE_BOOL_S /* dynamic_data_is_set */

/**
 * @brief Encodes an instance of @ref SPINEL_DATATYPE_NRF_802154_TRANSMITTED_FRAME_PROPS_S data type.
 */
#define NRF_802154_TRANSMITTED_FRAME_PROPS_ENCODE(frame_props) \
    ((frame_props).is_secured), ((frame_props).dynamic_data_is_set)

/**
 * @brief Decodes an instance of @ref SPINEL_DATATYPE_NRF_802154_TRANSMITTED_FRAME_PROPS_S data type.
 */
#define NRF_802154_TRANSMITTED_FRAME_PROPS_DECODE(frame_props) \
    (&(frame_props).is_secured), (&(frame_props).dynamic_data_is_set)

/**
 * @brief Spinel data type description for nrf_802154_tx_power_metadata_t.
 */
#define SPINEL_DATATYPE_NRF_802154_TX_POWER_METADATA_S \
    SPINEL_DATATYPE_BOOL_S /* use_metadata_value */    \
    SPINEL_DATATYPE_INT8_S /* power */

/**
 * @brief Encodes an instance of @ref SPINEL_DATATYPE_NRF_802154_TX_POWER_METADATA_S data type.
 */
#define NRF_802154_TX_POWER_METADATA_ENCODE(tx_power_metadata) \
    ((tx_power_metadata).use_metadata_value), ((tx_power_metadata).power)

/**
 * @brief Decodes an instance of @ref SPINEL_DATATYPE_NRF_802154_TX_POWER_METADATA_S data type.
 */
#define NRF_802154_TX_POWER_METADATA_DECODE(tx_power_metadata) \
    (&(tx_power_metadata).use_metadata_value), (&(tx_power_metadata).power)

/**
 * @brief Spinel data type description for nrf_802154_tx_channel_metadata_t.
 */
#define SPINEL_DATATYPE_NRF_802154_TX_CHANNEL_METADATA_S \
    SPINEL_DATATYPE_BOOL_S  /* use_metadata_value */     \
    SPINEL_DATATYPE_UINT8_S /* channel */

/**
 * @brief Encodes an instance of @ref SPINEL_DATATYPE_NRF_802154_TX_CHANNEL_METADATA_S data type.
 */
#define NRF_802154_TX_CHANNEL_METADATA_ENCODE(tx_channel_metadata) \
    ((tx_channel_metadata).use_metadata_value), ((tx_channel_metadata).channel)

/**
 * @brief Decodes an instance of @ref SPINEL_DATATYPE_NRF_802154_TX_CHANNEL_METADATA_S data type.
 */
#define NRF_802154_TX_CHANNEL_METADATA_DECODE(tx_channel_metadata) \
    (&(tx_channel_metadata).use_metadata_value), (&(tx_channel_metadata).channel)

/**
 * @brief Spinel data type description for nrf_802154_transmit_metadata_t.
 */
#define SPINEL_DATATYPE_NRF_802154_TRANSMIT_METADATA_S                     \
    SPINEL_DATATYPE_NRF_802154_TRANSMITTED_FRAME_PROPS_S /* frame_props */ \
    SPINEL_DATATYPE_BOOL_S                               /* cca */         \
    SPINEL_DATATYPE_NRF_802154_TX_POWER_METADATA_S       /* tx_power */    \
    SPINEL_DATATYPE_NRF_802154_TX_CHANNEL_METADATA_S     /* tx_channel */

/**
 * @brief Encodes an instance of @ref SPINEL_DATATYPE_NRF_802154_TRANSMIT_METADATA_S data type.
 */
#define NRF_802154_TRANSMIT_METADATA_ENCODE(tx_metadata)                  \
    NRF_802154_TRANSMITTED_FRAME_PROPS_ENCODE((tx_metadata).frame_props), \
    ((tx_metadata).cca),                                                  \
    NRF_802154_TX_POWER_METADATA_ENCODE((tx_metadata).tx_power),          \
    NRF_802154_TX_CHANNEL_METADATA_ENCODE((tx_metadata).tx_channel)

/**
 * @brief Decodes an instance of @ref SPINEL_DATATYPE_NRF_802154_TRANSMIT_METADATA_S data type.
 */
#define NRF_802154_TRANSMIT_METADATA_DECODE(tx_metadata)                  \
    NRF_802154_TRANSMITTED_FRAME_PROPS_DECODE((tx_metadata).frame_props), \
    (&(tx_metadata).cca),                                                 \
    NRF_802154_TX_POWER_METADATA_DECODE((tx_metadata).tx_power),          \
    NRF_802154_TX_CHANNEL_METADATA_DECODE((tx_metadata).tx_channel)

/**
 * @brief Spinel data type description for nrf_802154_transmit_csma_ca_metadata_t.
 */
#define SPINEL_DATATYPE_NRF_802154_TRANSMIT_CSMA_CA_METADATA_S             \
    SPINEL_DATATYPE_NRF_802154_TRANSMITTED_FRAME_PROPS_S /* frame_props */ \
    SPINEL_DATATYPE_NRF_802154_TX_POWER_METADATA_S       /* tx_power */    \
    SPINEL_DATATYPE_NRF_802154_TX_CHANNEL_METADATA_S     /* tx_channel */

/**
 * @brief Encodes an instance of @ref SPINEL_DATATYPE_NRF_802154_TRANSMIT_CSMA_CA_METADATA_S data type.
 */
#define NRF_802154_TRANSMIT_CSMA_CA_METADATA_ENCODE(tx_metadata)          \
    NRF_802154_TRANSMITTED_FRAME_PROPS_ENCODE((tx_metadata).frame_props), \
    NRF_802154_TX_POWER_METADATA_ENCODE((tx_metadata).tx_power),          \
    NRF_802154_TX_CHANNEL_METADATA_ENCODE((tx_metadata).tx_channel)

/**
 * @brief Decodes an instance of @ref SPINEL_DATATYPE_NRF_802154_TRANSMIT_CSMA_CA_METADATA_S data type.
 */
#define NRF_802154_TRANSMIT_CSMA_CA_METADATA_DECODE(tx_metadata)          \
    NRF_802154_TRANSMITTED_FRAME_PROPS_DECODE((tx_metadata).frame_props), \
    NRF_802154_TX_POWER_METADATA_DECODE((tx_metadata).tx_power),          \
    NRF_802154_TX_CHANNEL_METADATA_DECODE((tx_metadata).tx_channel)

/**
 * @brief Spinel data type description for nrf_802154_csma_ca_min_be_set.
 */
#define SPINEL_DATATYPE_NRF_802154_CSMA_CA_MIN_BE_SET               SPINEL_DATATYPE_UINT8_S

/**
 * @brief Spinel data type description for nrf_802154_csma_ca_min_be_set result.
 */
#define SPINEL_DATATYPE_NRF_802154_CSMA_CA_MIN_BE_SET_RET           SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_csma_ca_min_be_get.
 */
#define SPINEL_DATATYPE_NRF_802154_CSMA_CA_MIN_BE_GET               SPINEL_DATATYPE_NULL_S

/**
 * @brief Spinel data type description for nrf_802154_csma_ca_min_be_get result.
 */
#define SPINEL_DATATYPE_NRF_802154_CSMA_CA_MIN_BE_GET_RET           SPINEL_DATATYPE_UINT8_S

/**
 * @brief Spinel data type description for nrf_802154_csma_ca_max_be_set.
 */
#define SPINEL_DATATYPE_NRF_802154_CSMA_CA_MAX_BE_SET               SPINEL_DATATYPE_UINT8_S

/**
 * @brief Spinel data type description for nrf_802154_csma_ca_max_be_set result.
 */
#define SPINEL_DATATYPE_NRF_802154_CSMA_CA_MAX_BE_SET_RET           SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_csma_ca_max_be_get.
 */
#define SPINEL_DATATYPE_NRF_802154_CSMA_CA_MAX_BE_GET               SPINEL_DATATYPE_NULL_S

/**
 * @brief Spinel data type description for nrf_802154_csma_ca_max_be_get result.
 */
#define SPINEL_DATATYPE_NRF_802154_CSMA_CA_MAX_BE_GET_RET           SPINEL_DATATYPE_UINT8_S

/**
 * @brief Spinel data type description for nrf_802154_csma_ca_max_backoffs_set.
 */
#define SPINEL_DATATYPE_NRF_802154_CSMA_CA_MAX_BACKOFFS_SET         SPINEL_DATATYPE_UINT8_S

/**
 * @brief Spinel data type description for nrf_802154_csma_ca_max_backoffs_get.
 */
#define SPINEL_DATATYPE_NRF_802154_CSMA_CA_MAX_BACKOFFS_GET         SPINEL_DATATYPE_NULL_S

/**
 * @brief Spinel data type description for nrf_802154_csma_ca_max_backoffs_get result.
 */
#define SPINEL_DATATYPE_NRF_802154_CSMA_CA_MAX_BACKOFFS_GET_RET     SPINEL_DATATYPE_UINT8_S

/**
 * @brief Spinel data type description for nrf_802154_test_mode_csmaca_backoff_set.
 */
#define SPINEL_DATATYPE_NRF_802154_TEST_MODE_CSMACA_BACKOFF_SET     SPINEL_DATATYPE_UINT8_S

/**
 * @brief Spinel data type description for nrf_802154_test_mode_csmaca_backoff_get.
 */
#define SPINEL_DATATYPE_NRF_802154_TEST_MODE_CSMACA_BACKOFF_GET     SPINEL_DATATYPE_NULL_S

/**
 * @brief Spinel data type description for nrf_802154_test_mode_csmaca_backoff_get result.
 */
#define SPINEL_DATATYPE_NRF_802154_TEST_MODE_CSMACA_BACKOFF_GET_RET SPINEL_DATATYPE_UINT8_S

/**
 * @brief Spinel data type description for nrf_802154_transmit_at_metadata_t.
 */
#define SPINEL_DATATYPE_NRF_802154_TRANSMIT_AT_METADATA_S                  \
    SPINEL_DATATYPE_NRF_802154_TRANSMITTED_FRAME_PROPS_S /* frame_props */ \
    SPINEL_DATATYPE_BOOL_S                               /* cca */         \
    SPINEL_DATATYPE_UINT8_S                              /* channel */     \
    SPINEL_DATATYPE_NRF_802154_TX_POWER_METADATA_S       /* tx_power */    \
    SPINEL_DATATYPE_UINT8_S                              /* extra_cca_attempts */

/**
 * @brief Encodes an instance of @ref SPINEL_DATATYPE_NRF_802154_TRANSMIT_AT_METADATA_S data type.
 */
#define NRF_802154_TRANSMIT_AT_METADATA_ENCODE(tx_at_metadata)               \
    NRF_802154_TRANSMITTED_FRAME_PROPS_ENCODE((tx_at_metadata).frame_props), \
    ((tx_at_metadata).cca),                                                  \
    ((tx_at_metadata).channel),                                              \
    NRF_802154_TX_POWER_METADATA_ENCODE((tx_at_metadata).tx_power),          \
    ((tx_at_metadata).extra_cca_attempts)

/**
 * @brief Decodes an instance of @ref SPINEL_DATATYPE_NRF_802154_TRANSMIT_AT_METADATA_S data type.
 */
#define NRF_802154_TRANSMIT_AT_METADATA_DECODE(tx_at_metadata)               \
    NRF_802154_TRANSMITTED_FRAME_PROPS_DECODE((tx_at_metadata).frame_props), \
    (&(tx_at_metadata).cca),                                                 \
    (&(tx_at_metadata).channel),                                             \
    NRF_802154_TX_POWER_METADATA_DECODE((tx_at_metadata).tx_power),          \
    (&(tx_at_metadata).extra_cca_attempts)

/**
 * @brief Spinel data type description for nrf_802154_cca_cfg_t.
 */
#define SPINEL_DATATYPE_NRF_802154_CCA_CFG_S     \
    SPINEL_DATATYPE_UINT8_S /* mode */           \
    SPINEL_DATATYPE_INT8_S  /* ed_threshold */   \
    SPINEL_DATATYPE_UINT8_S /* corr_threshold */ \
    SPINEL_DATATYPE_UINT8_S /* corr_limit */

/**
 * @brief Encodes an instance of @ref SPINEL_DATATYPE_NRF_802154_CCA_CFG_S data type.
 */
#define NRF_802154_CCA_CFG_ENCODE(cca_cfg) \
    ((cca_cfg).mode), ((cca_cfg).ed_threshold), ((cca_cfg).corr_threshold), ((cca_cfg).corr_limit)

/**
 * @brief Decodes an instance of @ref SPINEL_DATATYPE_NRF_802154_CCA_CFG_S data type.
 */
#define NRF_802154_CCA_CFG_DECODE(cca_cfg) \
    (&(cca_cfg).mode),                     \
    (&(cca_cfg).ed_threshold),             \
    (&(cca_cfg).corr_threshold),           \
    (&(cca_cfg).corr_limit)

/**
 * @brief Spinel data type description for nrf_802154_stat_timestamps_t
 */
#define SPINEL_DATATYPE_NRF_802154_STAT_TIMESTAMPS_S \
    SPINEL_DATATYPE_UINT64_S                         \
    SPINEL_DATATYPE_UINT64_S                         \
    SPINEL_DATATYPE_UINT64_S                         \
    SPINEL_DATATYPE_UINT64_S                         \
    SPINEL_DATATYPE_UINT64_S                         \
    SPINEL_DATATYPE_UINT64_S

/**
 * @brief Encodes an instance of @ref SPINEL_DATATYPE_NRF_802154_STAT_TIMESTAMPS_S data type.
 */
#define NRF_802154_STAT_TIMESTAMPS_ENCODE(stat_timestamps) \
    ((stat_timestamps).last_csmaca_start_timestamp),       \
    ((stat_timestamps).last_cca_start_timestamp),          \
    ((stat_timestamps).last_cca_idle_timestamp),           \
    ((stat_timestamps).last_tx_end_timestamp),             \
    ((stat_timestamps).last_ack_end_timestamp),            \
    ((stat_timestamps).last_rx_end_timestamp)

/**
 * @brief Decodes an instance of @ref SPINEL_DATATYPE_NRF_802154_STAT_TIMESTAMPS_S data type.
 */
#define NRF_802154_STAT_TIMESTAMPS_DECODE(stat_timestamps) \
    (&(stat_timestamps).last_csmaca_start_timestamp),      \
    (&(stat_timestamps).last_cca_start_timestamp),         \
    (&(stat_timestamps).last_cca_idle_timestamp),          \
    (&(stat_timestamps).last_tx_end_timestamp),            \
    (&(stat_timestamps).last_ack_end_timestamp),           \
    (&(stat_timestamps).last_rx_end_timestamp)

/**
 * @brief Spinel data type description for SPINEL_PROP_LAST_STATUS.
 */
#define SPINEL_DATATYPE_SPINEL_PROP_LAST_STATUS      SPINEL_DATATYPE_UINT_PACKED_S

/**
 * @brief Spinel data type description for nrf_802154_sleep.
 */
#define SPINEL_DATATYPE_NRF_802154_SLEEP             SPINEL_DATATYPE_NULL_S

/**
 * @brief Spinel data type description for nrf_802154_sleep result.
 */
#define SPINEL_DATATYPE_NRF_802154_SLEEP_RET         SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_sleep_if_idle.
 */
#define SPINEL_DATATYPE_NRF_802154_SLEEP_IF_IDLE     SPINEL_DATATYPE_NULL_S

/**
 * @brief Spinel data type description for nrf_802154_sleep_if_idle result.
 */
#define SPINEL_DATATYPE_NRF_802154_SLEEP_IF_IDLE_RET SPINEL_DATATYPE_UINT8_S

/**
 * @brief Spinel data type description for nrf_802154_receive.
 */
#define SPINEL_DATATYPE_NRF_802154_RECEIVE           SPINEL_DATATYPE_NULL_S

/**
 * @brief Spinel data type description for nrf_802154_receive result.
 */
#define SPINEL_DATATYPE_NRF_802154_RECEIVE_RET       SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_receive_at.
 */
#define SPINEL_DATATYPE_NRF_802154_RECEIVE_AT \
    SPINEL_DATATYPE_UINT64_S /* rx_time */    \
    SPINEL_DATATYPE_UINT32_S /* timeout */    \
    SPINEL_DATATYPE_UINT8_S  /* channel */    \
    SPINEL_DATATYPE_UINT32_S /* window id */  \

/**
 * @brief Spinel data type description for nrf_802154_receive_at result.
 */
#define SPINEL_DATATYPE_NRF_802154_RECEIVE_AT_RET                  SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_receive_at_cancel.
 */
#define SPINEL_DATATYPE_NRF_802154_RECEIVE_AT_CANCEL               SPINEL_DATATYPE_UINT32_S

/**
 * @brief Spinel data type description for nrf_802154_receive_at_cancel result.
 */
#define SPINEL_DATATYPE_NRF_802154_RECEIVE_AT_CANCEL_RET           SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_receive_at_scheduled_cancel.
 */
#define SPINEL_DATATYPE_NRF_802154_RECEIVE_AT_SCHEDULED_CANCEL     SPINEL_DATATYPE_UINT32_S

/**
 * @brief Spinel data type description for nrf_802154_receive_at_scheduled_cancel result.
 */
#define SPINEL_DATATYPE_NRF_802154_RECEIVE_AT_SCHEDULED_CANCEL_RET SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_pan_id_set.
 */
#define SPINEL_DATATYPE_NRF_802154_PAN_ID_SET                      SPINEL_DATATYPE_DATA_S

/**
 * @brief Spinel data type description for nrf_802154_short_address_set.
 */
#define SPINEL_DATATYPE_NRF_802154_SHORT_ADDRESS_SET               SPINEL_DATATYPE_DATA_S

/**
 * @brief Spinel data type description for nrf_802154_short_address_set.
 */
#define SPINEL_DATATYPE_NRF_802154_ALTERNATE_SHORT_ADDRESS_SET\
    SPINEL_DATATYPE_BOOL_S /* Data valid flag */              \
    SPINEL_DATATYPE_DATA_S /* Alternative short address */    \

/**
 * @brief Spinel data type description for nrf_802154_extended_address_set.
 */
#define SPINEL_DATATYPE_NRF_802154_EXTENDED_ADDRESS_SET    SPINEL_DATATYPE_DATA_S

/**
 * @brief Spinel data type description for nrf_802154_pan_coord_set.
 */
#define SPINEL_DATATYPE_NRF_802154_PAN_COORD_SET           SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_pan_coord_get result.
 */
#define SPINEL_DATATYPE_NRF_802154_PAN_COORD_GET_RET       SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_pan_coord_get.
 */
#define SPINEL_DATATYPE_NRF_802154_PAN_COORD_GET           SPINEL_DATATYPE_NULL_S

/**
 * @brief Spinel data type description for nrf_802154_promiscuous_set.
 */
#define SPINEL_DATATYPE_NRF_802154_PROMISCUOUS_SET         SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_rx_on_when_idle_set.
 */
#define SPINEL_DATATYPE_NRF_802154_RX_ON_WHEN_IDLE_SET     SPINEL_DATATYPE_BOOL_S

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
#define SPINEL_DATATYPE_NRF_802154_ENERGY_DETECTED         SPINEL_DATATYPE_INT8_S

/**
 * @brief Spinel data type description for nrf_802154_energy_detection_failed.
 */
#define SPINEL_DATATYPE_NRF_802154_ENERGY_DETECTION_FAILED SPINEL_DATATYPE_UINT8_S

/**
 * @brief Spinel data type description for nrf_802154_continuous_carrier.
 */
#define SPINEL_DATATYPE_NRF_802154_CONTINUOUS_CARRIER      SPINEL_DATATYPE_NULL_S

/**
 * @brief Spinel data type description for nrf_802154_nrf_802154_continuous_carrier result.
 */
#define SPINEL_DATATYPE_NRF_802154_CONTINUOUS_CARRIER_RET  SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_modulated_carrier.
 */
#define SPINEL_DATATYPE_NRF_802154_MODULATED_CARRIER       SPINEL_DATATYPE_DATA_S

/**
 * @brief Spinel data type description for nrf_802154_nrf_802154_modulated_carrier result.
 */
#define SPINEL_DATATYPE_NRF_802154_MODULATED_CARRIER_RET   SPINEL_DATATYPE_BOOL_S

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
    SPINEL_DATATYPE_UINT64_S           /* timestamp */

/**
 * @brief Spinel data type description for nrf_802154_receive_failed
 */
#define SPINEL_DATATYPE_NRF_802154_RECEIVE_FAILED \
    SPINEL_DATATYPE_UINT8_S  /* error */          \
    SPINEL_DATATYPE_UINT32_S /* id */

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
#define SPINEL_DATATYPE_NRF_802154_TRANSMIT_RAW    \
    SPINEL_DATATYPE_NRF_802154_TRANSMIT_METADATA_S \
    SPINEL_DATATYPE_NRF_802154_HDATA_S /* Frame to transmit with its handle */

/**
 * @brief Spinel data type description for return value of nrf_802154_transmit_raw
 */
#define SPINEL_DATATYPE_NRF_802154_TRANSMIT_RAW_RET               SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for return value of nrf_802154_transmit_csma_ca_raw
 */
#define SPINEL_DATATYPE_NRF_802154_TRANSMIT_CSMA_CA_RAW_RET       SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for return value of nrf_802154_transmit_raw_at
 */
#define SPINEL_DATATYPE_NRF_802154_TRANSMIT_RAW_AT_RET            SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for return value of nrf_802154_transmit_at_cancel
 */
#define SPINEL_DATATYPE_NRF_802154_TRANSMIT_AT_CANCEL_RET         SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_auto_pending_bit_set.
 */
#define SPINEL_DATATYPE_NRF_802154_AUTO_PENDING_BIT_SET           SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_pending_bit_for_addr_set.
 *
 * SPINEL_DATATYPE_ARRAY_S encoding is not implemented, SPINEL_DATATYPE_DATA_S has to be used instead.
 */
#define SPINEL_DATATYPE_NRF_802154_PENDING_BIT_FOR_ADDR_SET       SPINEL_DATATYPE_DATA_S

/**
 * @brief Spinel data type description for nrf_802154_pending_bit_for_addr_set return value.
 */
#define SPINEL_DATATYPE_NRF_802154_PENDING_BIT_FOR_ADDR_SET_RET   SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_pending_bit_for_addr_clear.
 *
 * SPINEL_DATATYPE_ARRAY_S encoding is not implemented, SPINEL_DATATYPE_DATA_S has to be used instead.
 */
#define SPINEL_DATATYPE_NRF_802154_PENDING_BIT_FOR_ADDR_CLEAR     SPINEL_DATATYPE_DATA_S

/**
 * @brief Spinel data type description for nrf_802154_pending_bit_for_addr_set return value.
 */
#define SPINEL_DATATYPE_NRF_802154_PENDING_BIT_FOR_ADDR_CLEAR_RET SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_pending_bit_for_addr_reset.
 */
#define SPINEL_DATATYPE_NRF_802154_PENDING_BIT_FOR_ADDR_RESET     SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_src_addr_matching_method_set.
 */
#define SPINEL_DATATYPE_NRF_802154_SRC_ADDR_MATCHING_METHOD_SET   SPINEL_DATATYPE_UINT8_S

/**
 * @brief Spinel data type description for nrf_802154_ack_data_set.
 *
 * SPINEL_DATATYPE_ARRAY_S encoding is not implemented, SPINEL_DATATYPE_DATA_S has to be used instead.
 */
#define SPINEL_DATATYPE_NRF_802154_ACK_DATA_SET        \
    SPINEL_DATATYPE_DATA_WLEN_S /* Address */          \
    SPINEL_DATATYPE_DATA_WLEN_S /* Data to be set */   \
    SPINEL_DATATYPE_UINT8_S     /* Type of the data */ \

/**
 * @brief Spinel data type description for nrf_802154_ack_data_set return value.
 */
#define SPINEL_DATATYPE_NRF_802154_ACK_DATA_SET_RET SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_ack_data_clear.
 *
 * SPINEL_DATATYPE_ARRAY_S encoding is not implemented, SPINEL_DATATYPE_DATA_S has to be used instead.
 */
#define SPINEL_DATATYPE_NRF_802154_ACK_DATA_CLEAR \
    SPINEL_DATATYPE_DATA_WLEN_S /* Address */     \
    SPINEL_DATATYPE_UINT8_S     /* Type of the data */

/**
 * @brief Spinel data type description for nrf_802154_ack_data_clear return value.
 */
#define SPINEL_DATATYPE_NRF_802154_ACK_DATA_CLEAR_RET SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_ack_data_remove_all.
 */
#define SPINEL_DATATYPE_NRF_802154_ACK_DATA_REMOVE_ALL   \
    SPINEL_DATATYPE_BOOL_S  /* Extended/short address */ \
    SPINEL_DATATYPE_UINT8_S /* Type of the data */

/**
 * @brief Spinel data type description for nrf_802154_transmit_csma_ca_raw.
 */
#define SPINEL_DATATYPE_NRF_802154_TRANSMIT_CSMA_CA_RAW    \
    SPINEL_DATATYPE_NRF_802154_TRANSMIT_CSMA_CA_METADATA_S \
    SPINEL_DATATYPE_NRF_802154_HDATA_S /* Frame to transmit with its handle */

/**
 * @brief Spinel data type description for nrf_802154_transmit_raw_at
 */
#define SPINEL_DATATYPE_NRF_802154_TRANSMIT_RAW_AT    \
    SPINEL_DATATYPE_NRF_802154_TRANSMIT_AT_METADATA_S \
    SPINEL_DATATYPE_UINT64_S           /* tx_time */  \
    SPINEL_DATATYPE_NRF_802154_HDATA_S /* Frame to transmit with its handle */

/**
 * @brief Spinel data type description for nrf_802154_sleep.
 */
#define SPINEL_DATATYPE_NRF_802154_TRANSMIT_AT_CANCEL SPINEL_DATATYPE_NULL_S

/**
 * @brief Spinel data type description for nrf_802154_transmit_done_metadata.
 */
#define SPINEL_DATATYPE_NRF_802154_TRANSMIT_DONE_METADATA_S                \
    SPINEL_DATATYPE_NRF_802154_TRANSMITTED_FRAME_PROPS_S /* Frame props */ \
    SPINEL_DATATYPE_UINT8_S                              /* Length */      \
    SPINEL_DATATYPE_INT8_S                               /* Power */       \
    SPINEL_DATATYPE_UINT8_S                              /* LQI */         \
    SPINEL_DATATYPE_UINT64_S                             /* Timestamp */   \
    SPINEL_DATATYPE_NRF_802154_HDATA_S                   /* Ack frame with its handle */

/**
 * @brief Encodes an instance of @ref SPINEL_DATATYPE_NRF_802154_TRANSMIT_DONE_METADATA_S data type.
 *
 * @param[in]  metadata    Transmit done metadata structure to be decoded.
 * @param[in]  ack_handle  Variable containing handle to remote ack frame.
 */
#define NRF_802154_TRANSMIT_DONE_METADATA_ENCODE(metadata, ack_handle) \
    NRF_802154_TRANSMITTED_FRAME_PROPS_ENCODE((metadata).frame_props), \
    (metadata).data.transmitted.length,                                \
    (metadata).data.transmitted.power,                                 \
    (metadata).data.transmitted.lqi,                                   \
    (metadata).data.transmitted.time,                                  \
    NRF_802154_HDATA_ENCODE(ack_handle,                                \
                            (metadata).data.transmitted.p_ack,         \
                            ((metadata).data.transmitted.p_ack) ?      \
                            ((metadata).data.transmitted.p_ack)[0] + 1 : 0)

/**
 * @brief Decodes an instance of @ref SPINEL_DATATYPE_NRF_802154_TRANSMIT_DONE_METADATA_S data type.
 *
 * @param[out]  metadata    Transmit done metadata structure to which store decoded data.
 * @param[out]  ack_handle  Pointer variable to which store handle of received ACK frame.
 * @param[out]  ack_length  Variable to which store length of the decoded ACK with a handle.
 */
#define NRF_802154_TRANSMIT_DONE_METADATA_DECODE(metadata, ack_handle, ack_length) \
    NRF_802154_TRANSMITTED_FRAME_PROPS_DECODE((metadata).frame_props),             \
    &(metadata).data.transmitted.length,                                           \
    &(metadata).data.transmitted.power,                                            \
    &(metadata).data.transmitted.lqi,                                              \
    &(metadata).data.transmitted.time,                                             \
    NRF_802154_HDATA_DECODE(ack_handle, (metadata).data.transmitted.p_ack, ack_length)

/**
 * @brief Spinel data type description for nrf_802154_transmit_failed_metadata.
 */
#define SPINEL_DATATYPE_NRF_802154_TRANSMIT_FAILED_METADATA_S \
    SPINEL_DATATYPE_NRF_802154_TRANSMITTED_FRAME_PROPS_S /* Frame props */

/**
 * @brief Encodes an instance of @ref SPINEL_DATATYPE_NRF_802154_TRANSMIT_FAILED_METADATA_S data type.
 *
 * @param[in]  metadata    Transmit failed metadata structure to be encoded.
 */
#define NRF_802154_TRANSMIT_FAILED_METADATA_ENCODE(metadata) \
    NRF_802154_TRANSMITTED_FRAME_PROPS_ENCODE((metadata).frame_props)

/**
 * @brief Decodes an instance of @ref SPINEL_DATATYPE_NRF_802154_TRANSMIT_FAILED_METADATA_S data type.
 *
 * @param[out]  metadata    Transmit failed metadata structure to which store decoded data.
 */
#define NRF_802154_TRANSMIT_FAILED_METADATA_DECODE(metadata) \
    NRF_802154_TRANSMITTED_FRAME_PROPS_DECODE((metadata).frame_props)

/**
 * @brief Spinel data type description for nrf_802154_transmitted_raw.
 */
#define SPINEL_DATATYPE_NRF_802154_TRANSMITTED_RAW                                            \
    SPINEL_DATATYPE_NRF_802154_HDATA_S                  /* Transmitted frame and its handle*/ \
    SPINEL_DATATYPE_NRF_802154_TRANSMIT_DONE_METADATA_S /* Transmit done metadata */

/**
 * @brief Encodes an instance of @ref SPINEL_DATATYPE_NRF_802154_TRANSMITTED_RAW data type.
 *
 * @param[in]  frame_handle Variable containing remote frame handle.
 * @param[in]  frame_data   Pointer to contents of frame.
 * @param[in]  metadata     Transmit done metadata structure to be encoded.
 * @param[in]  ack_handle   Variable containing handle to received Ack.
 */
#define NRF_802154_TRANSMITTED_RAW_ENCODE(frame_handle, frame_data, metadata, ack_handle) \
    NRF_802154_HDATA_ENCODE(frame_handle, frame_data, frame_data[0] + 1),                 \
    NRF_802154_TRANSMIT_DONE_METADATA_ENCODE(metadata, ack_handle)

/**
 * @brief Decodes an instance of @ref SPINEL_DATATYPE_NRF_802154_TRANSMITTED_RAW data type.
 *
 * @param[out]  frame_handle  Pointer variable to which store handle of transmitted frame.
 * @param[out]  frame_data    Pointer to memory to which store contents of transmitted frame.
 * @param[out]  frame_length  Variable to which store length of the decoded frame.
 * @param[out]  metadata      Transmit done metadata structure to which store decoded data.
 * @param[out]  ack_handle    Pointer variable to which store handle of ACK frame.
 * @param[out]  ack_length    Variable to which store length of the decoded ACK frame.
 */
#define NRF_802154_TRANSMITTED_RAW_DECODE(frame_handle,              \
                                          frame_data,                \
                                          frame_length,              \
                                          metadata,                  \
                                          ack_handle,                \
                                          ack_length)                \
    NRF_802154_HDATA_DECODE(frame_handle, frame_data, frame_length), \
    NRF_802154_TRANSMIT_DONE_METADATA_DECODE(metadata, ack_handle, ack_length)

/**
 * @brief Spinel data type description for nrf_802154_transmit_failed.
 */
#define SPINEL_DATATYPE_NRF_802154_TRANSMIT_FAILED                                                                        \
    SPINEL_DATATYPE_NRF_802154_HDATA_S                    /* Frame that was attempted to be transmitted and its handle */ \
    SPINEL_DATATYPE_UINT8_S                               /* Error code */                                                \
    SPINEL_DATATYPE_NRF_802154_TRANSMIT_FAILED_METADATA_S /* Transmit failed metadata */

/**
 * @brief Encodes an instance of @ref SPINEL_DATATYPE_NRF_802154_TRANSMIT_FAILED data type.
 *
 * @param[in]  frame_handle Variable containing remote frame handle.
 * @param[in]  frame_data   Pointer to contents of frame.
 * @param[in]  error_code   Error code to be encoded.
 * @param[in]  metadata     Transmit failed metadata structure to be encoded.
 */
#define NRF_802154_TRANSMIT_FAILED_ENCODE(frame_handle, frame_data, error_code, metadata) \
    NRF_802154_HDATA_ENCODE(frame_handle, frame_data, frame_data[0] + 1),                 \
    error_code,                                                                           \
    NRF_802154_TRANSMIT_FAILED_METADATA_ENCODE(metadata)

/**
 * @brief Decodes an instance of @ref SPINEL_DATATYPE_NRF_802154_TRANSMIT_FAILED data type.
 *
 * @param[out]  frame_handle    Pointer variable to which store handle of frame failed to transmit.
 * @param[out]  frame_data      Pointer to memory to which store contents of frame failed to transmit.
 * @param[out]  frame_length    Variable to which store length of the decoded frame.
 * @param[in]   error_code      Variable to which store decode error code.
 * @param[out]  metadata        Transmit failed metadata structure to which store decoded data.
 */
#define NRF_802154_TRANSMIT_FAILED_DECODE(frame_handle,              \
                                          frame_data,                \
                                          frame_length,              \
                                          error_code,                \
                                          metadata)                  \
    NRF_802154_HDATA_DECODE(frame_handle, frame_data, frame_length), \
    &error_code,                                                     \
    NRF_802154_TRANSMIT_FAILED_METADATA_DECODE(metadata)

/**
 * @brief Spinel data type description for nrf_802154_capabilities_get.
 */
#define SPINEL_DATATYPE_NRF_802154_CAPABILITIES_GET     SPINEL_DATATYPE_NULL_S

/**
 * @brief Spinel data type description for nrf_802154_capabilities_get_ret.
 */
#define SPINEL_DATATYPE_NRF_802154_CAPABILITIES_GET_RET SPINEL_DATATYPE_UINT32_S

/**
 * @brief Spinel data type description for nrf_802154_time_get.
 */
#define SPINEL_DATATYPE_NRF_802154_TIME_GET             SPINEL_DATATYPE_NULL_S

/**
 * @brief Spinel data type description for nrf_802154_time_get_ret.
 */
#define SPINEL_DATATYPE_NRF_802154_TIME_GET_RET         SPINEL_DATATYPE_UINT64_S

/**
 * @brief Spinel data type description for nrf_802154_cca_cfg_get.
 */
#define SPINEL_DATATYPE_NRF_802154_CCA_CFG_GET          SPINEL_DATATYPE_NULL_S

/**
 * @brief Spinel data type description for nrf_802154_cca_cfg_get_ret.
 */
#define SPINEL_DATATYPE_NRF_802154_CCA_CFG_GET_RET \
    SPINEL_DATATYPE_NRF_802154_CCA_CFG_S

/**
 * @brief Spinel data type description for nrf_802154_cca_cfg_get.
 */
#define SPINEL_DATATYPE_NRF_802154_CCA_CFG_SET \
    SPINEL_DATATYPE_NRF_802154_CCA_CFG_S

/**
 * @brief Spinel data type description for nrf_802154_security_global_frame_counter_set.
 */
#define SPINEL_DATATYPE_NRF_802154_SECURITY_GLOBAL_FRAME_COUNTER_SET SPINEL_DATATYPE_UINT32_S

/**
 * @brief Spinel data type description for nrf_802154_security_key_store.
 */
#define SPINEL_DATATYPE_NRF_802154_SECURITY_KEY_STORE \
    SPINEL_DATATYPE_STRUCT_S(                         \
        SPINEL_DATATYPE_DATA_S  /* Key value */       \
                            )                         \
    SPINEL_DATATYPE_STRUCT_S(                         \
        SPINEL_DATATYPE_UINT8_S /* Key mode */        \
        SPINEL_DATATYPE_DATA_S  /* Key ID */          \
                            )                         \
    SPINEL_DATATYPE_UINT32_S    /* Key type */        \
    SPINEL_DATATYPE_UINT32_S    /* Frame counter */   \
    SPINEL_DATATYPE_BOOL_S      /* Whether to use global frame counter */

/**
 * @brief Encodes an instance of @ref SPINEL_DATATYPE_NRF_802154_SECURITY_KEY_STORE data type.
 *
 * @param[in]  key          Key structure (of @ref nrf_802154_key_t type) to be encoded.
 * @param[in]  key_size     Size of the key data, i.e. cleartext size.
 * @param[in]  key_id_size  Size of the key id in @p key structure.
 */
#define NRF_802154_SECURITY_KEY_STORE_ENCODE(key, key_size, key_id_size) \
    (key).value.p_cleartext_key,                                         \
    (key_size),                                                          \
    (key).id.mode,                                                       \
    (key).id.p_key_id,                                                   \
    (key_id_size),                                                       \
    (key).type,                                                          \
    (key).frame_counter,                                                 \
    (key).use_global_frame_counter

/**
 * @brief Decodes an instance of @ref SPINEL_DATATYPE_NRF_802154_SECURITY_KEY_STORE data type.
 *
 * @param[out]  key          Key structure (of @ref nrf_802154_key_t type) to which store decoded data.
 * @param[out]  key_size     Variable to which store size of the decoded key, i.e. cleartext size.
 * @param[out]  key_id_size  Variable to which store size of the decoded key ID.
 */
#define NRF_802154_SECURITY_KEY_STORE_DECODE(key, key_size, key_id_size) \
    & (key).value.p_cleartext_key,                                       \
    &(key_size),                                                         \
    &(key).id.mode,                                                      \
    &(key).id.p_key_id,                                                  \
    &(key_id_size),                                                      \
    &(key).type,                                                         \
    &(key).frame_counter,                                                \
    &(key).use_global_frame_counter

/**
 * @brief Spinel data type description for nrf_802154_security_key_remove.
 */
#define SPINEL_DATATYPE_NRF_802154_SECURITY_KEY_REMOVE \
    SPINEL_DATATYPE_STRUCT_S(                          \
        SPINEL_DATATYPE_UINT8_S /* Key mode */         \
        SPINEL_DATATYPE_DATA_S  /* Key ID */           \
                            )

/**
 * @brief Encodes an instance of @ref SPINEL_DATATYPE_NRF_802154_SECURITY_KEY_REMOVE data type.
 *
 * @param[in]  key_id       Key ID structure (of @ref nrf_802154_key_id_t type) to be encoded.
 * @param[in]  key_id_size  Size of the key id in @p key_id structure.
 */
#define NRF_802154_SECURITY_KEY_REMOVE_ENCODE(key_id, key_id_size) \
    (key_id).mode,                                                 \
    (key_id).p_key_id,                                             \
    (key_id_size)

/**
 * @brief Decodes an instance of @ref SPINEL_DATATYPE_NRF_802154_SECURITY_KEY_STORE data type.
 *
 * @param[out]  key_id       Key ID structure (of @ref nrf_802154_key_id_t type) to which store decoded data.
 * @param[out]  key_id_size  Variable to which store size of the decoded key ID.
 */
#define NRF_802154_SECURITY_KEY_REMOVE_DECODE(key_id, key_id_size) \
    & (key_id).mode,                                               \
    &(key_id).p_key_id,                                            \
    &(key_id_size)

/**
 * @brief Spinel data type description for nrf_802154_security_key_remove_all.
 */
#define SPINEL_DATATYPE_NRF_802154_SECURITY_KEY_REMOVE_ALL    SPINEL_DATATYPE_NULL_S

/**
 * @brief Spinel data type description for return type for security commands.
 */
#define SPINEL_DATATYPE_NRF_802154_SECURITY_ERROR_RET         SPINEL_DATATYPE_UINT8_S

/**
 * @brief Spinel data type description for nrf_802154_csl_writer_period_set.
 */
#define SPINEL_DATATYPE_NRF_802154_CSL_WRITER_PERIOD_SET      SPINEL_DATATYPE_UINT16_S

/**
 * @brief Spinel data type description for nrf_802154_csl_writer_anchor_time_set.
 */
#define SPINEL_DATATYPE_NRF_802154_CSL_WRITER_ANCHOR_TIME_SET SPINEL_DATATYPE_UINT64_S

/**
 * @brief Spinel data type description for nrf_802154_stat_timestamps_get
 */
#define SPINEL_DATATYPE_NRF_802154_STAT_TIMESTAMPS_GET        SPINEL_DATATYPE_NULL_S

/**
 * @brief Spinel data type description for nrf_802154_stat_timestamps_get_ret.
 */
#define SPINEL_DATATYPE_NRF_802154_STAT_TIMESTAMPS_GET_RET \
    SPINEL_DATATYPE_NRF_802154_STAT_TIMESTAMPS_S

/**
 * @brief Spinel data type description for nrf_802154_ifs_mode_get
 */
#define SPINEL_DATATYPE_NRF_802154_IFS_MODE_GET                SPINEL_DATATYPE_NULL_S

/**
 * @brief Spinel data type description for returning ifs_mode value.
 */
#define SPINEL_DATATYPE_NRF_802154_IFS_MODE_GET_RET            SPINEL_DATATYPE_UINT8_S

/**
 * @brief Spinel data type description for nrf_802154_ifs_mode_set
 */
#define SPINEL_DATATYPE_NRF_802154_IFS_MODE_SET                SPINEL_DATATYPE_UINT8_S

/**
 * @brief Spinel data type description for nrf_802154_ifs_mode_set result.
 */
#define SPINEL_DATATYPE_NRF_802154_IFS_MODE_SET_RET            SPINEL_DATATYPE_BOOL_S

/**
 * @brief Spinel data type description for nrf_802154_ifs_min_sifs_period_get
 */
#define SPINEL_DATATYPE_NRF_802154_IFS_MIN_SIFS_PERIOD_GET     SPINEL_DATATYPE_NULL_S

/**
 * @brief Spinel data type description for returning min_sifs_period value.
 */
#define SPINEL_DATATYPE_NRF_802154_IFS_MIN_SIFS_PERIOD_GET_RET SPINEL_DATATYPE_UINT16_S

/**
 * @brief Spinel data type description for nrf_802154_ifs_min_sifs_period_set
 */
#define SPINEL_DATATYPE_NRF_802154_IFS_MIN_SIFS_PERIOD_SET     SPINEL_DATATYPE_UINT16_S

/**
 * @brief Spinel data type description for nrf_802154_ifs_min_lifs_period_get
 */
#define SPINEL_DATATYPE_NRF_802154_IFS_MIN_LIFS_PERIOD_GET     SPINEL_DATATYPE_NULL_S

/**
 * @brief Spinel data type description for returning min_lifs_period value.
 */
#define SPINEL_DATATYPE_NRF_802154_IFS_MIN_LIFS_PERIOD_GET_RET SPINEL_DATATYPE_UINT16_S

/**
 * @brief Spinel data type description for nrf_802154_ifs_min_lifs_period_set
 */
#define SPINEL_DATATYPE_NRF_802154_IFS_MIN_LIFS_PERIOD_SET     SPINEL_DATATYPE_UINT16_S

#ifdef __cplusplus
}
#endif

#endif /* NRF_802154_SPINEL_DATATYPES_H_ */

/** @} */
