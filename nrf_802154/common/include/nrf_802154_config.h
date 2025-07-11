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

#ifndef NRF_802154_CONFIG_H__
#define NRF_802154_CONFIG_H__

#ifdef NRF_802154_PROJECT_CONFIG
#include NRF_802154_PROJECT_CONFIG
#endif

#include <nrfx.h>
#include "nrf_802154_nrfx_addons.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup nrf_802154_config 802.15.4 driver configuration
 * @{
 * @ingroup nrf_802154
 * @brief Configuration of the 802.15.4 radio driver for nRF SoCs.
 */

/**
 * @defgroup nrf_802154_config_radio Radio driver configuration
 * @{
 */

/**
 * @def NRF_802154_CCA_MODE_DEFAULT
 *
 * CCA mode used by the driver.
 *
 */
#ifndef NRF_802154_CCA_MODE_DEFAULT
#define NRF_802154_CCA_MODE_DEFAULT NRF_RADIO_CCA_MODE_ED
#endif

/**
 * @def NRF_802154_CCA_ED_THRESHOLD_DBM_DEFAULT
 *
 * Energy detection threshold in dBm used in the CCA procedure.
 *
 * Note: NRF_802154_CCA_ED_THRESHOLD_DEFAULT is deprecated.
 *
 */
#ifdef NRF_802154_CCA_ED_THRESHOLD_DEFAULT
#undef NRF_802154_CCA_ED_THRESHOLD_DBM_DEFAULT
#define NRF_802154_CCA_ED_THRESHOLD_DBM_DEFAULT (ED_RSSIOFFS + NRF_802154_CCA_ED_THRESHOLD_DEFAULT)
#else
#ifndef NRF_802154_CCA_ED_THRESHOLD_DBM_DEFAULT
#define NRF_802154_CCA_ED_THRESHOLD_DBM_DEFAULT (-75)
#endif
#endif

/**
 * @def NRF_802154_CCA_CORR_THRESHOLD_DEFAULT
 *
 * Correlator threshold used in the CCA procedure.
 *
 */
#ifndef NRF_802154_CCA_CORR_THRESHOLD_DEFAULT
#define NRF_802154_CCA_CORR_THRESHOLD_DEFAULT 0x14
#endif

/**
 * @def NRF_802154_CCA_CORR_LIMIT_DEFAULT
 *
 * Correlator limit used in the CCA procedure.
 *
 */
#ifndef NRF_802154_CCA_CORR_LIMIT_DEFAULT
#define NRF_802154_CCA_CORR_LIMIT_DEFAULT 0x02
#endif

/**
 * @def NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US
 *
 * Additional time in microseconds that delays triggering of @c TXEN after the
 * @c CCAIDLE event occurred. Default value for most use-cases is @c 0,
 * In this scenario, the short between the @c CCAIDLE event and the
 * @c TXEN task is used. If this value is non-zero, the short is not used.
 * The triggering of @c TXEN occurs through (D)PPI and TIMER.
 * A non-zero value may be necessary to ensure enough switching time for
 * use with some Front-End Modules.
 *
 * This option is supported for the nRF53 Series and the nRF54L Series only.
 */
#ifndef NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US
#define NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US 0U
#endif

/**
 * @def NRF_802154_INTERNAL_RADIO_IRQ_HANDLING
 *
 * If the driver is expected to internally handle the RADIO IRQ.
 * If the driver is used in an OS, the RADIO IRQ can be handled by the OS and passed to
 * the driver by @ref nrf_802154_radio_irq_handler.
 * In this case, the internal handling must be disabled.
 *
 */
#ifndef NRF_802154_INTERNAL_RADIO_IRQ_HANDLING
#define NRF_802154_INTERNAL_RADIO_IRQ_HANDLING 0
#endif

/**
 * @def NRF_802154_INTERNAL_SWI_IRQ_HANDLING
 *
 * If the driver is expected to internally handle the SWI IRQ.
 * If the driver is used in an OS, the SWI IRQ can be handled by the OS and passed to
 * the driver by @ref nrf_802154_swi_irq_handler.
 * In this case, the internal handling must be disabled.
 *
 */
#ifndef NRF_802154_INTERNAL_SWI_IRQ_HANDLING
#define NRF_802154_INTERNAL_SWI_IRQ_HANDLING 1
#endif

/**
 * @def NRF_802154_IRQ_PRIORITY
 *
 * Interrupt priority for RADIO peripheral.
 * Keep the IRQ priority high (low number) to prevent losing frames due to preemption.
 *
 */
#ifndef NRF_802154_IRQ_PRIORITY
#define NRF_802154_IRQ_PRIORITY 0
#endif

/**
 * @def NRF_802154_SWI_PRIORITY
 *
 * The priority of software interrupt used for requests and notifications.
 *
 */
#ifndef NRF_802154_SWI_PRIORITY
#define NRF_802154_SWI_PRIORITY 4
#endif

/**
 * @def NRF_802154_ECB_PRIORITY
 *
 * Interrupt priority for ECB peripheral used for frame encryption.
 *
 */
#ifndef NRF_802154_ECB_PRIORITY
#define NRF_802154_ECB_PRIORITY 3
#endif

/**
 * @def NRF_802154_PENDING_SHORT_ADDRESSES
 *
 * The number of slots containing short addresses of nodes for which the pending data is stored.
 *
 */
#ifndef NRF_802154_PENDING_SHORT_ADDRESSES
#define NRF_802154_PENDING_SHORT_ADDRESSES 10
#endif

/**
 * @def NRF_802154_PENDING_EXTENDED_ADDRESSES
 *
 * The number of slots containing extended addresses of nodes for which the pending data is stored.
 *
 */
#ifndef NRF_802154_PENDING_EXTENDED_ADDRESSES
#define NRF_802154_PENDING_EXTENDED_ADDRESSES 10
#endif

/**
 * @def NRF_802154_RX_BUFFERS
 *
 * The number of buffers in the receive queue.
 *
 */
#ifndef NRF_802154_RX_BUFFERS
#define NRF_802154_RX_BUFFERS 16
#endif

/**
 * @def NRF_802154_MAX_DISREGARDABLE_NOTIFICATIONS
 *
 * The number of slots in the driver's notification queue intended for notifications that can be
 * disregarded with no impact on the driver, for instance failed reception notifications.
 *
 * @note Setting this macro to 0 will cause the driver to not issue any disregardable notifications.
 */
#ifndef NRF_802154_MAX_DISREGARDABLE_NOTIFICATIONS
#define NRF_802154_MAX_DISREGARDABLE_NOTIFICATIONS 4
#endif

/**
 * @def NRF_802154_NOTIFY_CRCERROR
 *
 * With this flag set to 1, the CRC errors are notified to upper layers. This requires an interrupt
 * handler to be used.
 *
 */
#ifndef NRF_802154_NOTIFY_CRCERROR
#define NRF_802154_NOTIFY_CRCERROR 1
#endif

/**
 * @def NRF_802154_FRAME_TIMESTAMP_ENABLED
 *
 * If timestamps are to be added to the frames received.
 * Enabling this feature enables the function @ref nrf_802154_received_timestamp_raw,
 * which add timestamps to the frames received.
 * Enables also proper value of frame timestamp
 * ( @c nrf_802154_transmit_done_metadata_t::data.transmitted.time ) passed as metadata to
 * @ref nrf_802154_transmitted_raw .
 * This option also enables timestamping in stats.
 *
 */
#ifndef NRF_802154_FRAME_TIMESTAMP_ENABLED
#define NRF_802154_FRAME_TIMESTAMP_ENABLED 1
#endif

/**
 * @def NRF_802154_DELAYED_TRX_ENABLED
 *
 * If the delayed transmission and the receive window features are available.
 *
 */
#if !defined(CONFIG_NRF_802154_SL_OPENSOURCE)
#ifndef NRF_802154_DELAYED_TRX_ENABLED
#define NRF_802154_DELAYED_TRX_ENABLED 1
#endif
#endif

/**
 * @def NRF_802154_TEST_MODES_ENABLED
 *
 * Enables test modes. Test modes are normally disabled in end products.
 * When @ref NRF_802154_TEST_MODES_ENABLED is set to 1, nRF 802.15.4 Radio Driver
 * provides additional API to enable certain test modes. Setting
 * @ref NRF_802154_TEST_MODES_ENABLED to 1 without using test mode API does not
 * change any behavior of the Radio Driver.
 */
#ifndef NRF_802154_TEST_MODES_ENABLED
#define NRF_802154_TEST_MODES_ENABLED 0
#endif

/**
 * @def NRF_802154_PAN_COORD_GET_ENABLED
 *
 * Enables the @ref nrf_802154_pan_coord_get function.
 */
#ifndef NRF_802154_PAN_COORD_GET_ENABLED
#define NRF_802154_PAN_COORD_GET_ENABLED 0
#endif

/**
 * @}
 * @defgroup nrf_802154_config_csma CSMA/CA procedure configuration
 * @{
 */

/**
 * @def NRF_802154_CSMA_CA_ENABLED
 *
 * If CSMA-CA is to be enabled by the driver. Disabling CSMA-CA improves
 * the driver performance.
 *
 */
#if !defined(CONFIG_NRF_802154_SL_OPENSOURCE)
#ifndef NRF_802154_CSMA_CA_ENABLED
#define NRF_802154_CSMA_CA_ENABLED 1
#endif
#endif

/**
 * @def NRF_802154_CSMA_CA_MIN_BE_DEFAULT
 *
 * The default minimum value of the backoff exponent (BE) in the CSMA-CA algorithm
 * (see IEEE 802.15.4-2015: 6.2.5.1).
 *
 * @note The minimum value of the backoff exponent may be changed from default by calling the
 *       @ref nrf_802154_csma_ca_min_be_set function.
 *
 */
#ifdef NRF_802154_CSMA_CA_MIN_BE
#error "NRF_802154_CSMA_CA_MIN_BE was replaced with NRF_802154_CSMA_CA_MIN_BE_DEFAULT"
#endif
#ifndef NRF_802154_CSMA_CA_MIN_BE_DEFAULT
#define NRF_802154_CSMA_CA_MIN_BE_DEFAULT 3
#endif

/**
 * @def NRF_802154_CSMA_CA_MAX_BE_DEFAULT
 *
 * The default maximum value of the backoff exponent, BE, in the CSMA-CA algorithm
 * (see IEEE 802.15.4-2015: 6.2.5.1).
 *
 * @note The maximum value of the backoff exponent may be changed from default by calling the
 *       @ref nrf_802154_csma_ca_max_be_set function.
 *
 */
#ifdef NRF_802154_CSMA_CA_MAX_BE
#error "NRF_802154_CSMA_CA_MAX_BE was replaced with NRF_802154_CSMA_CA_MAX_BE_DEFAULT"
#endif
#ifndef NRF_802154_CSMA_CA_MAX_BE_DEFAULT
#define NRF_802154_CSMA_CA_MAX_BE_DEFAULT 5
#endif

/**
 * @def NRF_802154_CSMA_CA_MAX_CSMA_BACKOFFS_DEFAULT
 *
 * The default maximum number of backoffs that the CSMA-CA algorithm will attempt before declaring
 * a channel access failure.
 *
 * @note The maximum number of backoffs may be changed from default by calling the
 *       @ref nrf_802154_csma_ca_max_backoffs_set function.
 *
 */
#ifdef NRF_802154_CSMA_CA_MAX_CSMA_BACKOFFS
#error \
    "NRF_802154_CSMA_CA_MAX_CSMA_BACKOFFS was replaced with NRF_802154_CSMA_CA_MAX_CSMA_BACKOFFS_DEFAULT"
#endif
#ifndef NRF_802154_CSMA_CA_MAX_CSMA_BACKOFFS_DEFAULT
#define NRF_802154_CSMA_CA_MAX_CSMA_BACKOFFS_DEFAULT 4
#endif

/**
 * @def NRF_802154_CSMA_CA_WAIT_FOR_TIMESLOT
 *
 * Indicates whether the CSMA-CA algorithm waits for an available timeslot before it performs
 * the CCA procedure.
 *
 * @note When this option is enabled, the CSMA-CA procedure can be synchronized with timeslots of
 *       other protocols. This decreases the robustness of the CSMA-CA procedure. On the other hand,
 *       enabling this function increases the performance of multi-protocol applications, especially
 *       when protocols other than 802.15.4 use much of the radio time (like the BLE scanning
 *       operation).
 *
 */
#ifndef NRF_802154_CSMA_CA_WAIT_FOR_TIMESLOT
#define NRF_802154_CSMA_CA_WAIT_FOR_TIMESLOT 1
#endif

/**
 * @}
 * @defgroup nrf_802154_config_timeout ACK timeout feature configuration
 * @{
 */

/**
 * @def NRF_802154_ACK_TIMEOUT_ENABLED
 *
 * Indicates whether the ACK timeout feature is to be enabled in the driver.
 *
 */
#ifndef NRF_802154_ACK_TIMEOUT_ENABLED
#define NRF_802154_ACK_TIMEOUT_ENABLED 1
#endif

/**
 * @def NRF_802154_PRECISE_ACK_TIMEOUT_DEFAULT_TIMEOUT
 *
 * The default timeout in microseconds (us) for the precise ACK timeout feature.
 *
 */
#ifndef NRF_802154_PRECISE_ACK_TIMEOUT_DEFAULT_TIMEOUT
#define NRF_802154_PRECISE_ACK_TIMEOUT_DEFAULT_TIMEOUT 210
#endif

/**
 * @def NRF_802154_MAX_ACK_IE_SIZE
 *
 * The maximum supported size of the 802.15.4-2015 IE header and content fields in an Enh-Ack.
 *
 */
#ifndef NRF_802154_MAX_ACK_IE_SIZE
#define NRF_802154_MAX_ACK_IE_SIZE 16
#endif

/**
 * @}
 * @defgroup nrf_802154_config_ifs Interframe spacing feature configuration
 * @{
 */

/**
 * @def NRF_802154_IFS_ENABLED
 *
 * Indicates whether the Short/Long Interframe spacing feature is to be enabled in the driver.
 * This is an experimental feature.
 *
 */
#if !defined(CONFIG_NRF_802154_SL_OPENSOURCE)
#ifndef NRF_802154_IFS_ENABLED
#define NRF_802154_IFS_ENABLED 0
#endif
#endif

/**
 * @}
 * @defgroup nrf_802154_config_transmission Transmission start notification feature configuration
 * @{
 */

/**
 * @}
 * @defgroup nrf_802154_config_stats Statistics configuration
 * @{
 */

/**
 * @def NRF_802154_STATS_COUNT_ENERGY_DETECTED_EVENTS
 *
 * Configures if energy detected events will be counted in receive mode.
 * When this option is enabled additional interrupts on energy detected events will occur
 * increasing power consumption. The events counter is stored in
 * @ref nrf_802154_stat_counters_t::received_energy_events field and can be retrieved by
 * a call to @ref nrf_802154_stats_get or @ref nrf_802154_stat_counters_get.
 */
#ifndef NRF_802154_STATS_COUNT_ENERGY_DETECTED_EVENTS
#define NRF_802154_STATS_COUNT_ENERGY_DETECTED_EVENTS 1
#endif

/**
 * @def NRF_802154_STATS_COUNT_RECEIVED_PREAMBLES
 *
 * Configures if number of received preambles will be counted in receive mode.
 * When this option is enabled additional interrupts on preamble reveived will occur
 * increasing power consumption. The events counter is stored in
 * @ref nrf_802154_stat_counters_t::received_preambles field and can be retrieved by
 * a call to @ref nrf_802154_stats_get or @ref nrf_802154_stat_counters_get.
 */
#ifndef NRF_802154_STATS_COUNT_RECEIVED_PREAMBLES
#define NRF_802154_STATS_COUNT_RECEIVED_PREAMBLES 1
#endif

/**
 * @}
 * @defgroup nrf_802154_config_security Security configuration
 * @{
 */

/**
 * @def NRF_802154_SECURITY_KEY_STORAGE_SIZE
 *
 * Configures the number of keys which are available in the Key Storage.
 * This configuration is implementation-independent.
 */
#ifndef NRF_802154_SECURITY_KEY_STORAGE_SIZE
#define NRF_802154_SECURITY_KEY_STORAGE_SIZE 3
#endif

/**
 * @def NRF_802154_SECURITY_WRITER_ENABLED
 *
 * Enables the Security Writer module. The module parses the frame being transmitted, validates
 * frame's security header and injects frame counter associated with the used key.
 */
#ifndef NRF_802154_SECURITY_WRITER_ENABLED
#define NRF_802154_SECURITY_WRITER_ENABLED 1
#endif

/**
 * @def NRF_802154_ENCRYPTION_ENABLED
 *
 * Enables the frame encryption module. The module uses AES-CCM* algorithm to secure frames
 * transmitted by the driver.
 */
#ifndef NRF_802154_ENCRYPTION_ENABLED
#define NRF_802154_ENCRYPTION_ENABLED 1
#endif

/**
 * @def NRF_802154_ENCRYPTION_ACCELERATOR_ECB
 *
 * Enables ECB peripheral to be used as hardware accelerator for on-the-fly AES-CCM* encryption.
 */
#if !defined(NRF_802154_ENCRYPTION_ACCELERATOR_ECB) || defined(__DOXYGEN__)
#if defined(NRF52_SERIES) || defined(NRF5340_XXAA) || defined(__DOXYGEN__)
#define NRF_802154_ENCRYPTION_ACCELERATOR_ECB 1
#elif defined(NRF54H_SERIES) || defined(NRF54L_SERIES)
#define NRF_802154_ENCRYPTION_ACCELERATOR_ECB 0
#endif
#endif

/**
 * @}
 * @defgroup nrf_802154_ie Information Elements configuration
 * @{
 */

/**
 * @def NRF_802154_IE_WRITER_ENABLED
 *
 * Enables the Information Element writer module. The module parses frames being transmitted
 * for known Information Element IDs. If such elements are found, the writer module shall
 * fill the elements with appropriate data.
 */
#ifndef NRF_802154_IE_WRITER_ENABLED
#define NRF_802154_IE_WRITER_ENABLED 1
#endif

/**
 * @def NRF_802154_CARRIER_FUNCTIONS_ENABLED
 *
 * Enables functions used for test purposes: nrf_802154_continuous_carrier and
 * nrf_802154_modulated_carrier
 */
#ifndef NRF_802154_CARRIER_FUNCTIONS_ENABLED
#define NRF_802154_CARRIER_FUNCTIONS_ENABLED 1
#endif

/**
 * @def NRF_802154_NOTIFICATION_IMPL_DIRECT
 *
 * Use nrf_802154_notification_direct.c implementation.
 * See @ref NRF_802154_NOTIFICATION_IMPL.
 */
#define NRF_802154_NOTIFICATION_IMPL_DIRECT 0

/**
 * @def NRF_802154_NOTIFICATION_IMPL_SWI
 *
 * Use nrf_802154_notification_swi.c implementation.
 * See @ref NRF_802154_NOTIFICATION_IMPL.
 */
#define NRF_802154_NOTIFICATION_IMPL_SWI  1

/**
 * @def NRF_802154_NOTIFICATION_IMPL
 *
 * Selects implementation of internal "notification" module.
 * Possible values:
 *   @ref NRF_802154_NOTIFICATION_IMPL_DIRECT,
 *   @ref NRF_802154_NOTIFICATION_IMPL_SWI
 */
#ifndef NRF_802154_NOTIFICATION_IMPL
#define NRF_802154_NOTIFICATION_IMPL NRF_802154_NOTIFICATION_IMPL_SWI
#endif

/**
 * @def NRF_802154_REQUEST_IMPL_DIRECT
 *
 * Use nrf_802154_request_direct.c implementation.
 * See @ref NRF_802154_NOTIFICATION_IMPL.
 */
#define NRF_802154_REQUEST_IMPL_DIRECT 0

/**
 * @def NRF_802154_REQUEST_IMPL_DIRECT
 *
 * Use nrf_802154_request_swi.c implementation.
 * See @ref NRF_802154_REQUEST_IMPL.
 */
#define NRF_802154_REQUEST_IMPL_SWI    1

/**
 * @def NRF_802154_REQUEST_IMPL
 *
 * Selects implementation of internal "request" module.
 * Possible values:
 *   @ref NRF_802154_REQUEST_IMPL_DIRECT,
 *   @ref NRF_802154_REQUEST_IMPL_SWI
 */
#ifndef NRF_802154_REQUEST_IMPL
#define NRF_802154_REQUEST_IMPL NRF_802154_REQUEST_IMPL_SWI
#endif

/** @} */

/**
 *@}
 **/

#ifdef __cplusplus
}
#endif

#endif // NRF_802154_CONFIG_H__
