/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */


/**
 * @file sdc.h
 *
 * @defgroup sdc SoftDevice Controller API
 *
 * The main APIs needed to configure, enable, and use the SoftDevice Controller.
 * @{
 */


#ifndef SDC_H__
#define SDC_H__


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>
#include "nrf.h"
#include "nrf_errno.h"

/** @brief Default resource configuration tag. */
#define SDC_DEFAULT_RESOURCE_CFG_TAG  0

/** @brief Default maximum number of concurrent advertisers. */
#define SDC_DEFAULT_ADV_COUNT     1

/** @brief Default maximum number of concurrent periodic advertisers. */
#define SDC_DEFAULT_PERIODIC_ADV_COUNT 0

/** @brief Default maximum number of concurrent synchronizations
 * to periodic advertisers. */
#define SDC_DEFAULT_PERIODIC_SYNC_COUNT 0

/** @brief Default maximum number of concurrent periodic advertisers with responses. */
#define SDC_DEFAULT_PERIODIC_ADV_RSP_COUNT 0

/** @brief Default number of buffers for subevent data.*/
#define SDC_DEFAULT_PERIODIC_ADV_RSP_TX_BUFFER_COUNT 1

/** @brief Default maximum data size for subevent data. */
#define SDC_DEFAULT_PERIODIC_ADV_RSP_MAX_TX_DATA 73

/** @brief Default number of buffers for response reports. */
#define SDC_DEFAULT_PERIODIC_ADV_RSP_RX_BUFFER_COUNT 0

/** @brief Default number of buffers for response data.*/
#define SDC_DEFAULT_PERIODIC_SYNC_RSP_TX_BUFFER_COUNT 1

/** @brief Default maximum number of concurrent peripheral links. */
#define SDC_DEFAULT_PERIPHERAL_COUNT   1

/** @brief Default maximum number of concurrent central links. */
#define SDC_DEFAULT_CENTRAL_COUNT  1

/** @brief Default maximum Link Layer TX packet size. */
#define SDC_DEFAULT_TX_PACKET_SIZE 27

/** @brief Default maximum Link Layer RX packet size. */
#define SDC_DEFAULT_RX_PACKET_SIZE 27

/** @brief Default maximum Link Layer TX packet count per connection.
 *
 * With the default count, the application is able to refill the buffers during a connection event.
 */
#define SDC_DEFAULT_TX_PACKET_COUNT 3

/** @brief Default maximum Link Layer RX packet count per connection.
 *
 * With the default count, the application is able to empty the buffers during a connection event.
 */
#define SDC_DEFAULT_RX_PACKET_COUNT 2

/** @brief Default maximum size of the Filter Accept List. */
#define SDC_DEFAULT_FAL_SIZE 8

/** @brief Default maximum number of advertising reports available in the scanner.
 *
 * The default buffer configuration allows the scanner to continue scanning
 * with a pending advertising and scan response report.
 */
#define SDC_DEFAULT_SCAN_BUFFER_COUNT 3

/** @brief Default maximum number of periodic advertising reports available. */
#define SDC_DEFAULT_PERIODIC_SYNC_BUFFER_COUNT 2

/** @brief Default advertising data size when legacy advertising is supported. */
#define SDC_DEFAULT_ADV_BUF_SIZE 31

/** @brief Default number of devices that can be added to the periodic advertiser list. */
#define SDC_DEFAULT_PERIODIC_ADV_LIST_SIZE 0

/** @brief Default maximum number of concurrent Connected Isochronous Groups. */
#define SDC_DEFAULT_CIG_COUNT 0

/** @brief Default maximum number of concurrent Connected Isochronous Streams. */
#define SDC_DEFAULT_CIS_COUNT 0

/** @brief Default maximum number of concurrent Broadcast Isochronous Groups. */
#define SDC_DEFAULT_BIG_COUNT 0

/** @brief Default maximum number of concurrent Sink Broadcast Isochronous Streams. */
#define SDC_DEFAULT_BIS_SINK_COUNT 0

/** @brief Default maximum number of concurrent Source Broadcast Isochronous Streams. */
#define SDC_DEFAULT_BIS_SOURCE_COUNT 0

/** @brief Default ISO RX PDU buffer per stream count. */
#define SDC_DEFAULT_ISO_RX_PDU_BUFFER_PER_STREAM_COUNT 0

/** @brief Default ISO RX SDU buffer count. */
#define SDC_DEFAULT_ISO_RX_SDU_BUFFER_COUNT 0

/** @brief Default ISO RX SDU buffer size. */
#define SDC_DEFAULT_ISO_RX_SDU_BUFFER_SIZE 251

/** @brief Default ISO SDU TX buffer count. */
#define SDC_DEFAULT_ISO_TX_SDU_BUFFER_COUNT 0

/** @brief Default ISO SDU TX buffer size. */
#define SDC_DEFAULT_ISO_TX_SDU_BUFFER_SIZE 247

/** @brief Default ISO TX PDU buffer per stream count. */
#define SDC_DEFAULT_ISO_TX_PDU_BUFFER_PER_STREAM_COUNT 0

/** @brief Size of build revision array in bytes. */
#define SDC_BUILD_REVISION_SIZE 20

/**
 * @defgroup sdc_mem_defines Memory requirement defines
 *
 * The SoftDevice Controller memory requirement defines may be used to determine the dynamic memory usage
 * at compile time. The defines specify an upper limit, therefore the actual memory required
 * may be less.
 *
 * @note The values of the memory requirement defines may change between minor releases.
 * @{
 */

/** @brief Auxiliary defines, not to be used outside of this file. */
#define __MEM_MINIMAL_CENTRAL_LINK_SIZE 1043
#define __MEM_MINIMAL_PERIPHERAL_LINK_SIZE 1164
#define __MEM_TX_BUFFER_OVERHEAD_SIZE 15
#define __MEM_RX_BUFFER_OVERHEAD_SIZE 14

#define __MEM_ADDITIONAL_LINK_SIZE(tx_size, rx_size, tx_count, rx_count) \
    ((tx_count) * ((tx_size) + __MEM_TX_BUFFER_OVERHEAD_SIZE) - \
     (SDC_DEFAULT_TX_PACKET_SIZE + __MEM_TX_BUFFER_OVERHEAD_SIZE) + \
     (rx_count) * ((rx_size) + __MEM_RX_BUFFER_OVERHEAD_SIZE) - \
     (SDC_DEFAULT_RX_PACKET_SIZE + __MEM_RX_BUFFER_OVERHEAD_SIZE))

/** @brief Maximum memory required per central link.
 *
 * @param[in] tx_size Link Layer TX packet size.
 * @param[in] rx_size Link Layer RX packet size.
 * @param[in] tx_count Link Layer TX packet count.
 * @param[in] rx_count Link Layer RX packet count.
 */
#define SDC_MEM_PER_CENTRAL_LINK(tx_size, rx_size, tx_count, rx_count) \
    (__MEM_MINIMAL_CENTRAL_LINK_SIZE + \
     __MEM_ADDITIONAL_LINK_SIZE(tx_size, rx_size, tx_count, rx_count))

/** @brief Maximum memory required per peripheral link.
 *
 * @param[in] tx_size Link Layer TX packet size.
 * @param[in] rx_size Link Layer RX packet size.
 * @param[in] tx_count Link Layer TX packet count.
 * @param[in] rx_count Link Layer RX packet count.
 */
#define SDC_MEM_PER_PERIPHERAL_LINK(tx_size, rx_size, tx_count, rx_count) \
    (__MEM_MINIMAL_PERIPHERAL_LINK_SIZE + \
     __MEM_ADDITIONAL_LINK_SIZE(tx_size, rx_size, tx_count, rx_count))

/** Maximum shared memory required for central links. */
#define SDC_MEM_CENTRAL_LINKS_SHARED 17

/** Maximum shared memory required for peripheral links. */
#define SDC_MEM_PERIPHERAL_LINKS_SHARED  17

/** @brief Maximum memory required when supporting LE Power Control.
 *
 * @param[in] num_links Total number of peripheral and central links supported.
 */
#define SDC_MEM_LE_POWER_CONTROL(num_links) ((num_links) > 0 ? (12 + (num_links) * 26) : 0)

/** Memory required for Quality of Service (QoS) channel survey module. */
#define SDC_MEM_QOS_CHANNEL_SURVEY (40)

/** Memory required for the scanner when only supporting legacy scanning. */
#define SDC_MEM_SCAN(buffer_count) (340 + (buffer_count) * 96)

/** Memory required for the scanner when supporting extended scanning. */
#define SDC_MEM_SCAN_EXT(buffer_count) (340 + (buffer_count) * 312)

/** Additional memory required for the initiator when supporting scanning
 *  and initiating at the same time.
 */
#define SDC_MEM_INITIATOR (312)

/** Memory required for the Filter Accept List */
#define SDC_MEM_FAL(max_num_entries) ((max_num_entries) > 0 ? (4 + (max_num_entries) * 8) : 0)

/** @brief Auxiliary defines, not to be used outside of this file. */
#define __MEM_PER_ADV_SET_LOW(max_adv_data) ((4833+(max_adv_data)*18)/10)
#define __MEM_PER_ADV_SET_HIGH(max_adv_data) (671+(max_adv_data))
#define __MEM_PER_PERIODIC_ADV_SET_LOW(max_adv_data) ((2658+(max_adv_data)*18)/10)
#define __MEM_PER_PERIODIC_ADV_SET_HIGH(max_adv_data) (457+(max_adv_data))

/** @brief Maximum required memory for a given advertising buffer size.
 *
 * @param[in] max_adv_data The desired advertising data size.
 */
#define SDC_MEM_PER_ADV_SET(max_adv_data) ((max_adv_data<255)?\
    (__MEM_PER_ADV_SET_LOW(max_adv_data)):\
    (__MEM_PER_ADV_SET_HIGH(max_adv_data)))

/** @brief Additional memory required for periodic advertising.
 *
 * @note The max_adv_data must be the same as for advertising in @ref SDC_MEM_PER_ADV_SET.
 *
 * @param[in] max_adv_data The desired periodic advertising data size.
 */
#define SDC_MEM_PER_PERIODIC_ADV_SET(max_adv_data) ((max_adv_data<255)?\
     (__MEM_PER_PERIODIC_ADV_SET_LOW(max_adv_data)):\
     (__MEM_PER_PERIODIC_ADV_SET_HIGH(max_adv_data)))

/** Memory required per periodic sync when periodic sync with responses is not supported.
 *
 * @param[in] buffer_count The number of periodic synchronization receive buffers.
 */
#define SDC_MEM_PER_PERIODIC_SYNC(buffer_count) (221 + (buffer_count) * 279)

/** Memory required per periodic sync when periodic sync with responses is supported.
 *
 * @param[in] tx_buffer_count The number of buffers for sending data. Minimum of 1.
 * @param[in] rx_buffer_count The number of buffers for receiving data.
 */
#define SDC_MEM_PER_PERIODIC_SYNC_RSP(tx_buffer_count, rx_buffer_count) \
    (650 + (tx_buffer_count - 1) * 257 + (rx_buffer_count) * 278)

/** Memory required for the periodic adv list.
 *
 * @param[in] list_size The number of entries the list can fit.
 */
#define SDC_MEM_PERIODIC_ADV_LIST(list_size) ((list_size) * 8)

/** @brief Auxiliary defines, not to be used outside of this file */
#define __MEM_PER_PERIODIC_ADV_RSP_TX_BUFFER(max_tx_data_size) ((max_tx_data_size) + 5)
#define __MEM_PER_PERIODIC_ADV_RSP_RX_BUFFER (282)
#define __MEM_MINIMAL_PERIODIC_ADV_RSP_SET_SIZE_WITH_RX (465)
#define __MEM_MINIMAL_PERIODIC_ADV_RSP_SET_SIZE_WITHOUT_RX (166)
#define __MEM_FOR_PERIODIC_ADV_RSP_FAILURE_REPORTING (224)
#define __MEM_PER_ISO_PDU_POOL(count) ((count) > 0 ? (16 + (count) * 288) : 0)
#define __MEM_PER_ISO_TX_HCI_BUFFER(count) ((count) > 0 ? (12 + (count) * 300) : 0)

/** Memory required per periodic advertising with responses set.
 *
 * @param[in] max_adv_data The maximum size of data whcih can be sent in chains.
 * @param[in] tx_buffer_count The number of buffers for sending data. Minimum of 1.
 * @param[in] rx_buffer_count The number of buffers for receiving data.
 * @param[in] max_tx_data_size The maximum size of data which can be sent in subevents.
 * @param[in] failure_reporting_enabled Whether failure reporting is enabled.
 */
#define SDC_MEM_PER_PERIODIC_ADV_RSP_SET(max_adv_data, tx_buffer_count, rx_buffer_count, max_tx_data_size, failure_reporting_enabled) \
     (SDC_MEM_PER_PERIODIC_ADV_SET(max_adv_data) \
     + ((rx_buffer_count) > 0 ? __MEM_MINIMAL_PERIODIC_ADV_RSP_SET_SIZE_WITH_RX : \
                             __MEM_MINIMAL_PERIODIC_ADV_RSP_SET_SIZE_WITHOUT_RX ) \
     + (tx_buffer_count) * __MEM_PER_PERIODIC_ADV_RSP_TX_BUFFER(max_tx_data_size) \
     + (rx_buffer_count) * __MEM_PER_PERIODIC_ADV_RSP_RX_BUFFER \
     + ((failure_reporting_enabled) ? __MEM_FOR_PERIODIC_ADV_RSP_FAILURE_REPORTING : 0))

/** @brief Maximum memory required per CIG. */
#define SDC_MEM_PER_CIG(count) ((count) > 0 ? (13 + (count) * 123) : 0)

/** @brief Maximum memory required per CIS. Buffer and CIG memory comes in addition. */
#define SDC_MEM_PER_CIS(count) ((count) > 0 ? (13 + (count) * 548) : 0)

/** @brief Maximum memory required per BIG. */
#define SDC_MEM_PER_BIG(count) ((count) > 0 ? (13 + (count) * 284) : 0)

/** @brief Maximum memory required per BIS. Buffer and BIG memory comes in addition. */
#define SDC_MEM_PER_BIS(count) ((count) > 0 ? (13 + (count) * 268) : 0)

/** @brief Maximum memory required for the ISO RX PDU pool per stream.
 *  @param[in] rx_pdu_buffer_per_stream_count Number of RX PDU buffers allocated for each BIS or CIS stream. Minimum of 1.
 *                                            For BIS, this value determines the number of pretransmission that can be stored.
 *  @param[in] cis_count The number of supported CIS streams.
 *  @param[in] bis_sink_count The number of supported sink BIS streams. */
#define SDC_MEM_ISO_RX_PDU_POOL_PER_STREAM_SIZE(rx_pdu_buffer_per_stream_count, cis_count, bis_sink_count)     \
      (__MEM_PER_ISO_PDU_POOL(rx_pdu_buffer_per_stream_count) * ((cis_count) + (bis_sink_count)))

/** @brief Maximum memory required for the ISO RX path SDUs.
 * @param[in] count Number of shared SDUs allocated for the RX path.
 * @param[in] size  Maximum size of SDUs being used. */
#define SDC_MEM_ISO_RX_SDU_POOL_SIZE(count, size) ((count) > 0 ? (8 + (count) * ((size) + 13)) : 0)

/** @brief Maximum memory required for the ISO TX PDU pool.
 *  @param[in] tx_pdu_buffer_per_stream_count Number of TX PDU buffers allocated for each BIS or CIS stream.
 *                                            For BIS, this value determines the number of pretransmission that can be stored.
 *  @param[in] cis_count The number of supported CIS streams.
 *  @param[in] bis_source_count The number of supported source BIS streams. */
#define SDC_MEM_ISO_TX_PDU_POOL_SIZE(tx_pdu_buffer_per_stream_count, cis_count, bis_source_count)     \
      ((tx_pdu_buffer_per_stream_count) > 0 ? \
       __MEM_PER_ISO_PDU_POOL(tx_pdu_buffer_per_stream_count) * ((cis_count) + (bis_source_count)) : 0)

/** @brief Maximum memory required for the ISO TX path SDUs.
 * @param[in] count Number of shared SDUs allocated for the TX path.
 * @param[in] size  Maximum size of SDUs being used. */
#define SDC_MEM_ISO_TX_SDU_POOL_SIZE(count, size) ((count) > 0 ? (12 + (count) * ((size) + 49)) : 0)

/** @} end of sdc_mem_defines */

/** @brief Function prototype for the fault handler.
 *
 * @note The SoftDevice Controller will disable all interrupts prior to calling the
 *       fault handler. The SoftDevice Controller will reset the chip if the
 *       application returns from this function.
 *
 * @param[in] file  The filename where the assertion occurred.
 * @param[in] line  The line number where the assertion occurred.
 */
typedef void (*sdc_fault_handler_t)(const char * file, const uint32_t line);


/** @brief Function prototype for the SoftDevice Controller callback.
 *
 *  See also @ref sdc_enable().
 */
typedef void (*sdc_callback_t)(void);


enum sdc_cfg_type
{
    /** No configuration update. */
    SDC_CFG_TYPE_NONE = 0,
    /** See @ref sdc_cfg_t::central_count. */
    SDC_CFG_TYPE_CENTRAL_COUNT,
    /** See @ref sdc_cfg_t::peripheral_count. */
    SDC_CFG_TYPE_PERIPHERAL_COUNT,
    /** See @ref sdc_cfg_t::buffer_cfg. */
    SDC_CFG_TYPE_BUFFER_CFG,
    /** See @ref sdc_cfg_t::adv_count. */
    SDC_CFG_TYPE_ADV_COUNT,
    /** See @ref sdc_cfg_t::scan_buffer_cfg. */
    SDC_CFG_TYPE_SCAN_BUFFER_CFG,
    /** See @ref sdc_cfg_t::adv_buffer_cfg. */
    SDC_CFG_TYPE_ADV_BUFFER_CFG,
    /** See @ref sdc_cfg_t::fal_size */
    SDC_CFG_TYPE_FAL_SIZE,
    /** See @ref sdc_cfg_t::periodic_adv_count. */
    SDC_CFG_TYPE_PERIODIC_ADV_COUNT,
    /** See @ref sdc_cfg_t::periodic_sync_count. */
    SDC_CFG_TYPE_PERIODIC_SYNC_COUNT,
    /** See @ref sdc_cfg_t::periodic_sync_buffer_cfg. */
    SDC_CFG_TYPE_PERIODIC_SYNC_BUFFER_CFG,
    /** See @ref sdc_cfg_t::periodic_adv_list_size. */
    SDC_CFG_TYPE_PERIODIC_ADV_LIST_SIZE,
    /** See @ref sdc_cfg_t::periodic_adv_rsp_count. */
    SDC_CFG_TYPE_PERIODIC_ADV_RSP_COUNT,
    /** See @ref sdc_cfg_t::periodic_adv_rsp_buffer_cfg. */
    SDC_CFG_TYPE_PERIODIC_ADV_RSP_BUFFER_CFG,
    /** See @ref sdc_cfg_t::periodic_adv_rsp_failure_reporting_cfg. */
    SDC_CFG_TYPE_PERIODIC_ADV_RSP_FAILURE_REPORTING_CFG,
    /** See @ref sdc_cfg_t::periodic_sync_rsp_tx_buffer_cfg. */
    SDC_CFG_TYPE_PERIODIC_SYNC_RSP_TX_BUFFER_CFG,
    /** See @ref sdc_cfg_t::cig_count. */
    SDC_CFG_TYPE_CIG_COUNT,
    /** See @ref sdc_cfg_t::cis_count. */
    SDC_CFG_TYPE_CIS_COUNT,
    /** See @ref sdc_cfg_t::big_count. */
    SDC_CFG_TYPE_BIG_COUNT,
    /** See @ref sdc_cfg_t::bis_sink_count. */
    SDC_CFG_TYPE_BIS_SINK_COUNT,
    /** See @ref sdc_cfg_t::bis_source_count. */
    SDC_CFG_TYPE_BIS_SOURCE_COUNT,
    /** See @ref sdc_cfg_t::iso_buffer_cfg. */
    SDC_CFG_TYPE_ISO_BUFFER_CFG,
};


/** @brief Role count. */
typedef struct
{
    uint8_t count;   /**< Max number of concurrent roles. */
} sdc_cfg_role_count_t;


/** @brief Buffer configuration. */
typedef struct
{
    uint8_t tx_packet_size;   /**< Link Layer TX packet size. Valid range: 27-251.
                                   Default: @ref SDC_DEFAULT_TX_PACKET_SIZE. */
    uint8_t rx_packet_size;   /**< Link Layer RX packet size. Valid range: 27-251.
                                   Default: @ref SDC_DEFAULT_RX_PACKET_SIZE. */
    uint8_t tx_packet_count;  /**< Link Layer TX packet count per link. Minimum of 1.
                                   Default: @ref SDC_DEFAULT_TX_PACKET_COUNT. */
    uint8_t rx_packet_count;  /**< Link Layer RX packet count per link. Minimum of 1.
                                   Default: @ref SDC_DEFAULT_RX_PACKET_COUNT. */
} sdc_cfg_buffer_cfg_t;


/** @brief Buffer count configuration. */
typedef struct
{
    uint8_t count;   /**< Number of buffers. */
} sdc_cfg_buffer_count_t;


typedef struct
{
    /** The maximum advertising data length that can be used by an advertiser
     *
     * If set to 31, the controller will support 31 bytes of advertising data and scan response data.
     * Setting a value larger than 31 bytes is only useful when supporting extended advertising.
     *
     * Default: @ref SDC_DEFAULT_ADV_BUF_SIZE.
     */
    uint16_t max_adv_data;
} sdc_cfg_adv_buffer_cfg_t;


typedef struct
{
    /** Configures the size of the buffer pool allocated to each periodic
     * advertising set with responses for subevent data.
     *
     * Default: @ref SDC_DEFAULT_PERIODIC_ADV_RSP_TX_BUFFER_COUNT.
     */
    uint8_t tx_buffer_count;
    /** Configures the maximum amount of data which can be sent in a PAwR subevent.
     *
     * Default: @ref SDC_DEFAULT_PERIODIC_ADV_RSP_MAX_TX_DATA.
     */
    uint8_t max_tx_data_size;
    /** Configures the size of the buffer pool allocated to each periodic
     * advertising set with responses for response reports.
     *
     * The value can be set to 0 to disable listening for responses.
     *
     * Default: @ref SDC_DEFAULT_PERIODIC_ADV_RSP_RX_BUFFER_COUNT.
     */
    uint8_t rx_buffer_count;
} sdc_cfg_periodic_adv_rsp_buffer_cfg_t;


typedef struct
{
    /** Configures the number of shared SDU TX buffers allocated for ISO.
     *
     * Default: @ref SDC_DEFAULT_ISO_TX_SDU_BUFFER_COUNT.
     */
    uint8_t tx_sdu_buffer_count;
    /** Configures the size of shared SDU TX buffers allocated for ISO.
     *
     * Default: @ref SDC_DEFAULT_ISO_TX_SDU_BUFFER_SIZE.
     */
    uint16_t tx_sdu_buffer_size;
    /** Configures the number of TX PDU buffers allocated per ISO stream.
     *
     * This is the number of maximum size (251 bytes) PDU buffers. When PDU size is smaller
     * than the maximum size, the pool will be repartitioned into a larger number of adjusted PDUs.
     *
     * For BIS, this value is used to determine the furthest PDU that can be stored in the buffers.
     * Therefore, limiting the number of pretransmission subevents and maximum pretransmission offset (PTO)
     * that the controller can use.
     *
     * Default: @ref SDC_DEFAULT_ISO_TX_PDU_BUFFER_PER_STREAM_COUNT.
     */
    uint8_t tx_pdu_buffer_per_stream_count;
    /** Configures the number of RX PDU buffers allocated per ISO stream.
     *
     * This is the number of maximum size (251 bytes) PDU buffers. When PDU size is smaller
     * than the maximum size, the pool will be repartitioned into a larger number of adjusted PDUs.
     *
     * For BIS, this value determines how many pretransmissions can be stored and utilized.
     *
     * Default: @ref SDC_DEFAULT_ISO_RX_PDU_BUFFER_PER_STREAM_COUNT.
     */
    uint8_t rx_pdu_buffer_per_stream_count;
    /** Configures the number of RX SDU buffers allocated for ISO.
     *
     * Default: @ref SDC_DEFAULT_ISO_RX_SDU_BUFFER_COUNT.
     */
    uint8_t rx_sdu_buffer_count;
    /** Configures the size of RX SDU buffers allocated for ISO.
     *
     * Default: @ref SDC_DEFAULT_ISO_RX_SDU_BUFFER_SIZE.
     */
    uint16_t rx_sdu_buffer_size;
} sdc_cfg_iso_buffer_cfg_t;


/** @brief SoftDevice Controller configuration.  */
typedef union
{
    /** Max number of concurrent central connections.
     *  Default: @ref SDC_DEFAULT_CENTRAL_COUNT.
     */
    sdc_cfg_role_count_t   central_count;
    /** Max number of concurrent peripheral connections.
     *  Default: @ref SDC_DEFAULT_PERIPHERAL_COUNT.
     */
    sdc_cfg_role_count_t   peripheral_count;
    /** Configures the number and size of the data buffers available per link.
     *  Default: See @ref sdc_cfg_buffer_cfg_t.
     */
    sdc_cfg_buffer_cfg_t   buffer_cfg;
    /** Max number of concurrent advertisers.
     *  Must be more than or equal to @ref sdc_cfg_t::periodic_adv_count.
     *  Default: @ref SDC_DEFAULT_ADV_COUNT.
     */
    sdc_cfg_role_count_t   adv_count;
    /** Configures the maximum number of advertising reports available in the scanner.
     *
     *  The minimum allowed number of buffers is 2.
     *
     *  It is recommended to support at least three buffers,
     *  otherwise the scan response report will likely not be generated.
     *
     *  Default: @ref SDC_DEFAULT_SCAN_BUFFER_COUNT.
     */
    sdc_cfg_buffer_count_t scan_buffer_cfg;
    /** Configures the maximum advertising data per advertising set.
     *  Default: See @ref sdc_cfg_adv_buffer_cfg_t.
     */
    sdc_cfg_adv_buffer_cfg_t adv_buffer_cfg;
    /** Configures the maximum size of the Filter Accept List.
     *
     * Default: @ref SDC_DEFAULT_FAL_SIZE.
     */
    uint16_t fal_size;
    /** Configures the maximum number of concurrent periodic advertisers.
     *  Must be less than or equal to @ref sdc_cfg_t::adv_count.
     *  Default: @ref SDC_DEFAULT_PERIODIC_ADV_COUNT.
     */
    sdc_cfg_role_count_t periodic_adv_count;
    /** Configures the maximum number of concurrent synchronizations
     *  to periodic advertisers.
     *  Default: @ref SDC_DEFAULT_PERIODIC_SYNC_COUNT.
     */
    sdc_cfg_role_count_t periodic_sync_count;
    /** Configures the maximum number of periodic advertising reports available
     *  for each synchronization to a periodic advertiser.
     *
     *  Each synchronization to a periodic advertiser allocates its own buffer
     *  pool.
     *
     *  When periodic sync with responses is not supported,
     *  the minimum allowed number of buffers is 2.
     *
     *  When periodic sync with responses is supported,
     *  the minimum allowed number of buffers is 1.
     *
     *  No reports for a new advertising event containing chained PDUs will be
     *  enqueued until the host has emptied the previous event's buffers.
     *
     *  Default: @ref SDC_DEFAULT_PERIODIC_SYNC_BUFFER_COUNT.
     */
    sdc_cfg_buffer_count_t periodic_sync_buffer_cfg;
    /** Configures the size of the periodic advertiser list.
     *
     * The maximum number of supported devices is 8.
     *
     * Default: @ref SDC_DEFAULT_PERIODIC_ADV_LIST_SIZE.
     */
    uint8_t periodic_adv_list_size;
    /** Configures the maximum number of concurrent periodic advertising sets with responses.
     *
     * Default: @ref SDC_DEFAULT_PERIODIC_ADV_RSP_COUNT.
     */
    sdc_cfg_role_count_t periodic_adv_rsp_count;
    /** Configures the number and size of the data buffers available per periodic advertising set with responses.
     *  Default: See @ref sdc_cfg_periodic_adv_rsp_buffer_cfg_t.
     */
    sdc_cfg_periodic_adv_rsp_buffer_cfg_t periodic_adv_rsp_buffer_cfg;
    /** Enables/disables failure response reports for Periodic Advertising with Responses - Advertiser.
     *
     * Set to 1 to enable RX failure reporting, 0 to disable.
     *
     * Default: disabled.
     */
    uint8_t periodic_adv_rsp_failure_reporting_cfg;
    /** Configures the maximum number of responses that can be stored in the controller
     *  when synchronized to a periodic advertiser with responses.
     *
     * The minimum number of buffers is 1.
     *
     * Default: @ref SDC_DEFAULT_PERIODIC_SYNC_RSP_TX_BUFFER_COUNT.
     */
    sdc_cfg_buffer_count_t periodic_sync_rsp_tx_buffer_cfg;
    /** Configures the maximum number of concurrent CIGs.
     *
     * Default: @ref SDC_DEFAULT_CIG_COUNT.
     */
    sdc_cfg_role_count_t cig_count;
    /** Configures the maximum number of concurrent CISs.
     *
     * Default: @ref SDC_DEFAULT_CIS_COUNT.
     */
    sdc_cfg_role_count_t cis_count;
    /** Configures the maximum number of concurrent BIGs.
     *
     * Default: @ref SDC_DEFAULT_BIG_COUNT.
     */
    sdc_cfg_role_count_t big_count;
    /** Configures the maximum number of concurrent sink BISs.
     *
     * Default: @ref SDC_DEFAULT_BIS_SINK_COUNT.
     */
    sdc_cfg_role_count_t bis_sink_count;
    /** Configures the maximum number of concurrent source BISs.
     *
     * Default: @ref SDC_DEFAULT_BIS_SOURCE_COUNT.
     */
    sdc_cfg_role_count_t bis_source_count;
    /** Configures the number and size of buffers allocated for ISO.
     *
     * Default: See @ref sdc_cfg_iso_buffer_cfg_t.
     */
    sdc_cfg_iso_buffer_cfg_t iso_buffer_cfg;
} sdc_cfg_t;


/** @brief Initialize the SoftDevice Controller
 *
 * After this function is called, the application may use SoC APIs.
 *
 * @param[in]  fault_handler  The fault handler will be executed when there is an
 *                            internal error in the SoftDevice Controller.
 *
 * @retval 0            Success
 * @retval -NRF_EINVAL  Invalid argument provided
 * @retval -NRF_EPERM   Unable to initialize because
 *                        - MPSL is not initialized
 *                        - MPSL needs to be configured with a LFCLK accuracy
 *                          of 500 ppm or better.
 */
int32_t sdc_init(sdc_fault_handler_t fault_handler);


/** @brief Change or add a SoftDevice Controller configuration
 *
 * To change the default configuration, update @ref
 * SDC_DEFAULT_RESOURCE_CFG_TAG. To create or update a new
 * configuration, provide another resource_cfg_tag.
 *
 * @note The application can set config_type to @ref
 *       SDC_CFG_TYPE_NONE to obtain the required memory size for the
 *       current configuration in bytes.
 *
 * @note Resource configuration can only be performed prior to calling @ref
 *       sdc_enable(). However, the current configuration may be
 *       changed after enabling the SoftDevice Controller.
 *
 * @param[in]  config_tag       Configuration tag.
 * @param[in]  config_type      Configuration type. See also @ref sdc_cfg_type.
 * @param[in]  p_resource_cfg   Configuration to be changed.
 *
 * @returns Required memory size for the current configuration in bytes.
 * @retval -NRF_EOPNOTSUPP    Unsupported configuration.
 * @retval -NRF_ENOMEM        Configuration required over 64 kB of RAM that is
                              not supported currently.
 * @retval -NRF_EINVAL        Invalid argument provided.
 * @retval -NRF_EPERM         This API was called after @ref sdc_enable().
 */
int32_t sdc_cfg_set(uint8_t config_tag,
                    uint8_t config_type,
                    sdc_cfg_t const * p_resource_cfg);


/** @brief Enable the SoftDevice Controller
 *
 * After this function is called, the application may utilize HCI APIs.
 *
 * @param[in] callback  The callback will be executed when HCI data or and HCI
 *                      event is available. The callback will be executed in
 *                      the same context as @ref mpsl_low_priority_process.
 *                      See also @ref sdc_hci_get().
 * @param[in]  p_mem    Provide memory for the current resource configuration.
 *                      To obtain the required memory size, use the value
 *                      returned from @ref sdc_cfg_set().
 *                      The pointer must be 8 bytes aligned.
 *
 * @retval 0            Success
 * @retval -NRF_EINVAL  Invalid argument provided
 * @retval -NRF_EPERM   The entropy source is not configured. Call @ref sdc_rand_source_register() first.
 */
int32_t sdc_enable(sdc_callback_t callback,
                   uint8_t * p_mem);


/** @brief Disable the SoftDevice Controller
 *
 * This call is synchronous. After the SoftDevice Controller is disabled, Bluetooth LE
 * functionality is no longer available.
 *
 * @retval 0 Success
 */
int32_t sdc_disable(void);


/** @brief Obtain build revision string
 *
 * The application must provide a buffer that is at least @ref SDC_BUILD_REVISION_SIZE
 * bytes long. The SoftDevice Controller will copy the build revision string to the provided buffer.
 *
 *  @param[in,out] p_build_revision  Build revision.
 *
 * @retval 0            Success
 * @retval -NRF_EINVAL  Invalid argument provided
 */
int32_t sdc_build_revision_get(uint8_t * p_build_revision);

/** @brief Support Advertising State
 *
 * After this API is called, the controller will support the HCI commands
 * and events related to the Advertising State.
 * Only non-connectable advertising is supported. To support connectable
 * advertising, call @ref sdc_support_peripheral().
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  Advertising state is not supported.
 */
int32_t sdc_support_adv(void);

/** @brief Support extended advertising
 *
 * After this API is called, the controller will support the HCI commands
 * and events related to both legacy and extended advertising.
 *
 * To reduce the size of the final linked image, the application should
 * call either @ref sdc_support_adv() or @ref sdc_support_ext_adv().
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  Extended advertising state is not supported.
 */
int32_t sdc_support_ext_adv(void);

/** @brief Support Peripheral role
 *
 * After this API is called, the controller will support the HCI commands
 * and events related to the peripheral role.
 *
 * The application shall call either @ref sdc_support_adv() or
 * @ref sdc_support_ext_adv() to be able to support connection
 * establishment.
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  Peripheral role is not supported.
 */
int32_t sdc_support_peripheral(void);

/** @brief Support Scanning state
 *
 * After this API is called, the controller will support the HCI commands
 * and events related to the scanning state.
 *
 * To reduce the size of the final linked image, the application should
 * not call both @ref sdc_support_scan() and @ref sdc_support_ext_scan().
 *
 * This API shall not be called together with
 * @ref sdc_support_central() or @ref sdc_support_ext_central().
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  Scanning state is not supported.
 */
int32_t sdc_support_scan(void);

/** @brief Support scanning for extended advertising PDUs
 *
 * After this API is called, the controller will support the HCI commands
 * and events related to both legacy and extended scanning.
 *
 * To reduce the size of the final linked image, the application should
 * not call both @ref sdc_support_scan() and @ref sdc_support_ext_scan().
 *
 * This API shall not be called together with
 * @ref sdc_support_central() or @ref sdc_support_ext_central().
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  Extended scanning state is not supported.
 */
int32_t sdc_support_ext_scan(void);

/** @brief Support Scanner, Initiator, and Central role
 *
 * After this API is called, the controller will support the HCI commands
 * and events related to the scanner, initiator, and central role.
 *
 * To reduce the size of the final linked image, the application should
 * not call both @ref sdc_support_central() and @ref sdc_support_ext_central().
 *
 * This API shall not be called together with
 * @ref sdc_support_scan() or @ref sdc_support_ext_scan().
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  These features are not supported.
 */
int32_t sdc_support_central(void);

/** @brief Support Extended Scanner, Extended Initiator, and Central role
 *
 * After this API is called, the controller will support the HCI commands
 * and events related to the extended scanner, initiator, and central role.
 *
 * To reduce the size of the final linked image, the application should
 * not call both @ref sdc_support_central() and @ref sdc_support_ext_central().
 *
 * This API shall not be called together with
 * @ref sdc_support_scan() or @ref sdc_support_ext_scan().
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  These features are not supported.
 */
int32_t sdc_support_ext_central(void);

/** @brief Support for scanning and initiating at the same time.
 *
 * After this API is called, the controller will support:
 *   - Creating a connection while passive or active scanning is enabled
 *   - Enabling passive or active scanning while a connection attempt is ongoing
 *
 * This API should be called only when centrals are supported (the application should
 * call either @ref sdc_support_central() or @ref sdc_support_ext_central()).
 *
 * @retval 0               Success
 * @retval -NRF_EPERM      This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP Scanning and initiating in parallel is not supported.
 */
int32_t sdc_support_parallel_scanning_and_initiating(void);

/** @brief Support Data Length Extensions for a central device
 *
 * After this API is called, the controller will support data length extension in the central role.
 * That is:
 *  - DLE is marked supported in the LL Feature Exchange procedure.
 *  - All DLE HCI APIs are supported. The controller replies with LL_LENGTH_RSP
 *    when a LL_LENGTH_REQ is received.
 *
 * @note The application is required to call both @ref sdc_support_dle_central() and @ref sdc_support_dle_peripheral()
 *       if both central and peripheral roles are supported.
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  Data Length Extension or Central Role is not supported.
 */
int32_t sdc_support_dle_central(void);

/** @brief Support Data Length Extensions for a peripheral device
 *
 * After this API is called, the controller will support data length extension in the peripheral role.
 * That is:
 *  - DLE is marked supported in the LL Feature Exchange procedure.
 *  - All DLE HCI APIs are supported. The controller replies with LL_LENGTH_RSP
 *    when a LL_LENGTH_REQ is received.
 *
 * @note The application is required to call both @ref sdc_support_dle_central() and @ref sdc_support_dle_peripheral()
 *       if both central and peripheral roles are supported.
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  Data Length Extension or Peripheral Role is not supported.
 */
int32_t sdc_support_dle_peripheral(void);

/** @brief Support LE 2M PHY
 *
 * After this API is called, the controller will support LE 2M PHY. That is:
 *  - The controller can use 2M PHY in both the connected and non-connected state.
 *  - LE 2M PHY is marked supported in the LL Feature Exchange procedure.
 *
 * @note The application is required to call @ref sdc_support_phy_update_central() and/or @ref sdc_support_phy_update_peripheral()
 *       to enable the PHY update procedure.
 *
 * @retval 0           Success
 * @retval -NRF_EPERM  This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 */
int32_t sdc_support_le_2m_phy(void);

/** @brief Support LE Coded PHY
 *
 * After this API is called, the controller will support LE Coded PHY. That is:
 *  - The controller can use LE Coded PHY in both the connected and non-connected state.
 *  - LE Coded PHY is marked supported in the LL Feature Exchange procedure.
 *
 * @note The application is required to call @ref sdc_support_phy_update_central() and/or @ref sdc_support_phy_update_peripheral()
 *       to enable the PHY update procedure.
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  LE Coded PHY is not supported.
 */
int32_t sdc_support_le_coded_phy(void);

/** @brief Support PHY Update Procedure for central role
 *
 * After this API is called, the controller will support PHY update procedure in central role. That is:
 *  - All HCI APIs for obtaining or changing PHYs are supported for central role.
 *
 * @note The application is required to call both @ref sdc_support_phy_update_central() and @ref sdc_support_phy_update_peripheral()
 *       if both central and peripheral roles are supported.
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  LE Coded PHY is not supported.
 */
int32_t sdc_support_phy_update_central(void);

/** @brief Support LE Coded PHY for peripheral role
 *
 * After this API is called, the controller will support PHY update procedure in peripheral role. That is:
 *  - All HCI APIs for obtaining or changing PHYs are supported for peripheral role.
 *
 * @note The application is required to call both @ref sdc_support_phy_update_central() and @ref sdc_support_phy_update_peripheral()
 *       if both central and peripheral roles are supported.
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  LE Coded PHY is not supported.
 */
int32_t sdc_support_phy_update_peripheral(void);

/** @brief Support LE Periodic Advertising in the Advertising state
 *
 * After this API is called, the controller will support the HCI commands
 * related to the Periodic Advertising State.
 *
 * The application shall also call @ref sdc_support_ext_adv() to enable
 * support for extended advertising before enabling support for periodic advertising.
 *
 * @note This API also enables support for ADI in periodic advertising packets.
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  LE Periodic advertising is not supported.
 */
int32_t sdc_support_le_periodic_adv(void);

/** @brief Support LE Periodic Advertising in the Synchronization state
 *
 * After this API is called, the controller will support the HCI commands
 * related to the Synchronization State.
 *
 * The application shall also call @ref sdc_support_ext_scan() to enable
 * support for scanning for periodic advertisers before enabling support for
 * this feature.
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  LE Periodic advertising is not supported.
 */
int32_t sdc_support_le_periodic_sync(void);

/** @brief Support LE Periodic Advertising with Responses in the Advertising state
 *
 * After this API is called, the controller will support the HCI commands
 * related to Periodic Advertising with Responses.
 *
 * The application shall also call @ref sdc_support_ext_adv(), @ref sdc_support_le_periodic_adv(),
 * and at least one of @ref sdc_support_periodic_adv_sync_transfer_sender_central()
 * and @ref sdc_support_periodic_adv_sync_transfer_sender_peripheral()
 * to enable support for the extended advertising, periodic advertising,
 * and sync transfer sender features before enabling support for
 * Periodic Advertising with Responses.
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  LE Periodic Advertising with Responses is not supported.
 */
int32_t sdc_support_le_periodic_adv_with_rsp(void);

/** @brief Support LE Periodic Advertising with Responses in the Synchronization state
 *
 * After this API is called, the controller will support the HCI commands
 * related to Periodic Sync with Responses.
 *
 * The application shall also call @ref sdc_support_ext_adv(), @ref sdc_support_le_periodic_sync(),
 * and at least one of @ref sdc_support_periodic_adv_sync_transfer_receiver_central and
 * @ref sdc_support_periodic_adv_sync_transfer_receiver_peripheral to enable support for extended
 * advertising, periodic advertising, and the sync transfer receiver features before enabling
 * support for Periodic Sync with Responses.
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  LE Periodic Sync with Responses is not supported.
 */
int32_t sdc_support_le_periodic_sync_with_rsp(void);

/** @brief Support LE Power Control for central role
 *
 * After this API is called, the controller will support the HCI commands
 * related to the LE Power Control.
 *
 * @note The application is required to call both @ref sdc_support_le_power_control_central() and @ref sdc_support_le_power_control_peripheral()
 *       if both central and peripheral roles are supported.
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  LE Power Control is not supported.
 */
int32_t sdc_support_le_power_control_central(void);

/** @brief Support LE Power Control for peripheral role
 *
 * After this API is called, the controller will support the HCI commands
 * related to the LE Power Control.
 *
 * @note The application is required to call both @ref sdc_support_le_power_control_central() and @ref sdc_support_le_power_control_peripheral()
 *       if both central and peripheral roles are supported.
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  LE Power Control is not supported.
 */
int32_t sdc_support_le_power_control_peripheral(void);

/** @brief Support Sleep Clock Accuracy (SCA) Update procedure for central role
 *
 * @note The application is required to call both @ref sdc_support_sca_central() and @ref sdc_support_sca_peripheral()
 *       if both central and peripheral roles are supported.
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  SCA Update procedure is not supported.
 */
int32_t sdc_support_sca_central(void);

/** @brief Support Sleep Clock Accuracy (SCA) Update procedure for peripheral role
 *
 * @note The application is required to call both @ref sdc_support_sca_central() and @ref sdc_support_sca_peripheral()
 *       if both central and peripheral roles are supported.
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  SCA Update procedure is not supported.
 */
int32_t sdc_support_sca_peripheral(void);

/** @brief Support LE Connection CTE response for central role
 *
 * After this API is called, the controller will support the HCI commands
 * related to the LE Connection CTE Response.
 *
 * @note The application is required to call both @ref sdc_support_le_conn_cte_rsp_central() and @ref sdc_support_le_conn_cte_rsp_peripheral()
 *       if both central and peripheral roles are supported.
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  LE Connection CTE Response is not supported.
 */
int32_t sdc_support_le_conn_cte_rsp_central(void);

/** @brief Support LE Connection CTE response for peripheral role
 *
 * After this API is called, the controller will support the HCI commands
 * related to the LE Connection CTE Response.
 *
 * @note The application is required to call both @ref sdc_support_le_conn_cte_rsp_central() and @ref sdc_support_le_conn_cte_rsp_peripheral()
 *       if both central and peripheral roles are supported.
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  LE Connection CTE Response is not supported.
 */
int32_t sdc_support_le_conn_cte_rsp_peripheral(void);

/** @brief Support for sending periodic advertising sync transfers as central role
 *
 * @note The application is required to call both @ref sdc_support_periodic_adv_sync_transfer_sender_central()
 *       and @ref sdc_support_periodic_adv_sync_transfer_sender_peripheral() if both central and peripheral roles are supported.
 *
 * @retval 0               Success
 * @retval -NRF_EPERM      This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP Sending periodic advertising sync transfers is not supported.
 */
int32_t sdc_support_periodic_adv_sync_transfer_sender_central(void);

/** @brief Support for sending periodic advertising sync transfers as peripheral role
 *
 * @note The application is required to call both @ref sdc_support_periodic_adv_sync_transfer_sender_central()
 *       and @ref sdc_support_periodic_adv_sync_transfer_sender_peripheral() if both central and peripheral roles are supported.
 *
 * @retval 0               Success
 * @retval -NRF_EPERM      This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP Sending periodic advertising sync transfers is not supported.
 */
int32_t sdc_support_periodic_adv_sync_transfer_sender_peripheral(void);

/** @brief Support for receiving periodic advertising sync transfers as central role
 *
 * @note The application is required to call both @ref sdc_support_periodic_adv_sync_transfer_receiver_central()
 *       and @ref sdc_support_periodic_adv_sync_transfer_receiver_peripheral() if both central and peripheral roles are supported.
 *
 * @retval 0               Success
 * @retval -NRF_EPERM      This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP Receiving periodic advertising sync transfers is not supported.
 */
int32_t sdc_support_periodic_adv_sync_transfer_receiver_central(void);

/** @brief Support for receiving periodic advertising sync transfers as peripheral role
 *
 * @note The application is required to call both @ref sdc_support_periodic_adv_sync_transfer_receiver_central()
 *       and @ref sdc_support_periodic_adv_sync_transfer_receiver_peripheral() if both central and peripheral roles are supported.
 *
 * @retval 0               Success
 * @retval -NRF_EPERM      This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP Receiving periodic advertising sync transfers is not supported.
 */
int32_t sdc_support_periodic_adv_sync_transfer_receiver_peripheral(void);

/** @brief Support Connected Isochronous streams in the peripheral role
 *
 * After this API is called, the controller will support the HCI commands
 * related to Connected Isochronous channels in the peripheral role
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  Connected Isochronous streams in the peripheral role is not supported.
 */
int32_t sdc_support_cis_peripheral(void);

/** @brief Support Connected Isochronous streams in the central role
 *
 * After this API is called, the controller will support the HCI commands
 * related to Connected Isochronous channels in the central role
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  Connected Isochronous streams in the central role is not supported.
 */
int32_t sdc_support_cis_central(void);

/** @brief Support Broadcast Isochronous streams as a source
 *
 * After this API is called, the controller will support the HCI commands
 * related to Broadcast Isochronous channels as a source
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  Broadcast Isochronous streams as a source is not supported.
 */
int32_t sdc_support_bis_source(void);

/** @brief Support Broadcast Isochronous streams as a sink
 *
 * After this API is called, the controller will support the HCI commands
 * related to Broadcast Isochronous channels as a sink
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  Broadcast Isochronous streams as a sink is not supported.
 */
int32_t sdc_support_bis_sink(void);

/** @brief Configure the controller to ignore HCI ISO data timestamps from the host
 *
 * The controller requires that timestamps coming from the host/application are based on the controller's clock.
 * This option can instruct the controller to ignore the timestamps, if the host sends timestamps that are not based on the controller's clock.
 *
 * @param ignore indicates if timestamps in HCI ISO data packets from the host should be ignored.
 *
 * @retval 0                Success
 * @retval -NRF_EOPNOTSUPP  Broadcast Isochronous streams as source and Connected Isochronous Channels are not supported.
 */
int32_t sdc_iso_host_timestamps_ignore(bool ignore);

/** @brief Support for Quality of Service (QoS) channel survey module
 *
 * After this API is called, the controller will support the @ref sdc_hci_cmd_vs_qos_channel_survey_enable HCI command
 *
 * @retval 0               Success
 * @retval -NRF_EPERM      This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP QoS channel survey is not supported.
 */
int32_t sdc_support_qos_channel_survey(void);

/** @brief Configure the coex advertising mode
 *
 * Configure how the advertiser behaves on denial of an advertising packet.
 * The advertiser can be configured to carry on with the advertisement after the denial or
 * to abort the advertising event, once an advertising packet gets denied.
 *
 * The default behavior is that the advertiser tries to carry on with the advertising event
 * after the denial.
 *
 * @note Setting @p adv_cont_on_denial to True is not supported for the
 *       @ref MPSL_COEX_1WIRE_GPIOTE_ID coex interface type.
 *       Using the @ref MPSL_COEX_1WIRE_GPIOTE_ID coex interface without using this
 *       API call with @p adv_cont_on_denial set to False will result in unexpected behavior.
 *
 * @retval 0                Success
 * @retval -NRF_EOPNOTSUPP  The coexistence feature is not supported.
 */
int32_t sdc_coex_adv_mode_configure(bool adv_cont_on_denial);

/** @brief Support for setting the default radio TX power level
 *
 * This API sets the default power level for radio TX activity in the controller.
 *
 * The power level set will be equal to or less than the one requested, based on the values
 * supported by the hardware used. Values outside the suported range will be set to the nearest
 * supported value.
 *
 * @p requested_power_level represents the actual power level fed to the antenna.
 * When a Front-End Module is used, gain values for the SoC and FEM are calculated
 * automatically to guarantee the closest possible match to the value requested by the user
 * at the RF output.
 *
 * @param[in] requested_power_level The power level in dBm to use.
 *
 * @retval 0           Success
 * @retval -NRF_EPERM  This API must be called before @ref sdc_enable().
 */
int32_t sdc_default_tx_power_set(int8_t requested_power_level);

/** @brief Support Connection Subrating for central role
 *
 * After this API is called, the controller will support the HCI commands
 * related to Connection Subrating.
 *
 * @note The application is required to call both @ref sdc_support_connection_subrating_central() and @ref sdc_support_connection_subrating_peripheral()
 *       if both central and peripheral roles are supported.
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  Connection Subrating is not supported.
 */
int32_t sdc_support_connection_subrating_central(void);

/** @brief Support Connection Subrating for peripheral role
 *
 * After this API is called, the controller will support the HCI commands
 * related to Connection Subrating.
 *
 * @note The application is required to call both @ref sdc_support_connection_subrating_central() and @ref sdc_support_connection_subrating_peripheral()
 *       if both central and peripheral roles are supported.
 *
 * @retval 0                Success
 * @retval -NRF_EPERM       This API must be called before @ref sdc_cfg_set() or @ref sdc_enable().
 * @retval -NRF_EOPNOTSUPP  Connection Subrating is not supported.
 */
int32_t sdc_support_connection_subrating_peripheral(void);

#ifdef __cplusplus
}
#endif

/** @} end of sdc */

#endif /* SDC_H__ */
