/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2021 DSR Corporation, Denver CO, USA.
 * www.dsr-zboss.com
 * www.dsr-corporation.com
 * All rights reserved.
 *
 *
 * Use in source and binary forms, redistribution in binary form only, with
 * or without modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 2. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 3. This software, with or without modification, must only be used with a Nordic
 *    Semiconductor ASA integrated circuit.
 *
 * 4. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/* PURPOSE: Diagnostics cluster definitions
*/

#ifndef ZDO_DIAGNOSTICS_H
#define ZDO_DIAGNOSTICS_H 1

#if defined(ZDO_DIAGNOSTICS) || defined(DOXYGEN)

#include "zboss_api_internal.h"

/** @cond DOXYGEN_ZCL_SECTION */

/** @addtogroup ZB_ZCL_DIAGNOSTICS
 *  @{
 *    @details
 *    ZCL Diagnostics cluster definitions
 */


/* Cluster ZB_ZCL_CLUSTER_ID_DIAGNOSTICS */

/*! @name Diagnostics cluster attributes
    @{
*/

typedef ZB_PACKED_PRE struct zdo_diagnostics_ctx_s
{
  zdo_diagnostics_info_t diagnostics_info;
  zb_callback_t get_stats_cb;
} ZB_PACKED_STRUCT zdo_diagnostics_ctx_t;

typedef ZB_PACKED_PRE struct zdo_diagnostics_full_stats_s
{
  zb_mac_diagnostic_info_t mac_stats;
  zdo_diagnostics_info_t zdo_stats;
  zb_uint8_t status;
} ZB_PACKED_STRUCT zdo_diagnostics_full_stats_t;

/*! @brief Diagnostics cluster attribute identifiers
    @see HA spec, Diagnostics Cluster 9.3.2.2.2
*/
typedef enum zdo_diagnostics_counter_id_e
{
  /** @brief number_of_resets, Zigbee Diagnostic Cluster spec 1.2.2.1.1 */
  ZDO_DIAGNOSTICS_NUMBER_OF_RESETS_ID = 0,
  /** This attribute keeps track of the number of writes to persistent memory. */
  ZDO_DIAGNOSTICS_PERSISTENT_MEMORY_WRITES_ID = 1,
  /** APSRxBcast Attribute A counter that is incremented each time
   *  the APS layer receives a broadcast. */
  ZDO_DIAGNOSTICS_APS_RX_BCAST_ID = 2,
  /** @brief aps_tx_bcast, HA spec 9.3.2.2.2 */
  ZDO_DIAGNOSTICS_APS_TX_BCAST_ID = 3,
  /** APSRxUcast Attribute A counter that is incremented each time
   *  the APS layer receives a unicast. */
  ZDO_DIAGNOSTICS_APS_RX_UCAST_ID = 4,
  /** @brief aps_tx_ucast_success, HA spec 9.3.2.2.2 */
  ZDO_DIAGNOSTICS_APS_TX_UCAST_SUCCESS_ID = 5,
  /** APSTxUcastRetry Attribute A counter that is incremented each time
   *  the APS layer retries the sending of a unicast. */
  ZDO_DIAGNOSTICS_APS_TX_UCAST_RETRY_ID = 6,
  /** @brief aps_tx_ucast_fail, HA spec 9.3.2.2.2 */
  ZDO_DIAGNOSTICS_APS_TX_UCAST_FAIL_ID = 7,
  /** RouteDiscInitiated Attribute. A counter that is incremented each time the
   *  network layer submits a route discovery message to the MAC. HA spec 9.2.2.2.2.13 */
  ZDO_DIAGNOSTICS_ROUTE_DISC_INITIATED_ID = 8,
  /** NeighborAdded Attribute A counter that is incremented each time
   *  an entry is added to the neighbor table. */
  ZDO_DIAGNOSTICS_NEIGHBOR_ADDED_ID = 9,
  /** NeighborRemoved Attribute A counter that is incremented each time
   *  an entry is removed from the neighbor table. */
  ZDO_DIAGNOSTICS_NEIGHBOR_REMOVED_ID = 10,
  /** A counter that is incremented each time a neighbor table entry becomes stale
   *  because the neighbor has not been heard from. */
  ZDO_DIAGNOSTICS_NEIGHBOR_STALE_ID = 11,
  /** @brief join_indication, HA spec 1.2.2.2.17 */
  ZDO_DIAGNOSTICS_JOIN_INDICATION_ID = 12,
  /** A counter that is incremented each time an entry is removed from the child table. */
  ZDO_DIAGNOSTICS_CHILD_MOVED_ID = 13,
  /** A counter that is incremented each time a message is dropped at the network
   *  layer because the APS frame counter was not higher than the last message seen
   *  from that source. */
  ZDO_DIAGNOSTICS_NWKFC_FAILURE_ID = 14,
  /** A counter that is incremented each time a message is dropped at the APS layer
   *  because the APS frame counter was not higher than the last message seen from
   *  that source. */
  ZDO_DIAGNOSTICS_APSFC_FAILURE_ID = 15,
  /** A counter that is incremented each time a message is dropped at the APS layer
   *  because it had APS encryption but the key associated with the sender has
   *  not been authenticated, and thus the key is not authorized for use
   *  in APS data messages. */
  ZDO_DIAGNOSTICS_APS_UNAUTHORIZED_KEY_ID = 16,
  /** A counter that is incremented each time a NWK encrypted message was received
   *  but dropped because decryption failed. */
  ZDO_DIAGNOSTICS_NWK_DECRYPT_FAILURES_ID = 17,
  /** A counter that is incremented each time an APS encrypted message was received
   *  but dropped because decryption failed. */
  ZDO_DIAGNOSTICS_APS_DECRYPT_FAILURES_ID = 18,
  /** @brief average_mac_retry_per_aps_message_sent, HA spec 9.3.2.2.2.27 */
  ZDO_DIAGNOSTICS_PACKET_BUFFER_ALLOCATE_FAILURES_ID = 19,
  /** A counter that is incremented each time a unicast packet is relayed. */
  ZDO_DIAGNOSTICS_RELAYED_UCAST_ID = 20,
  ZDO_DIAGNOSTICS_PHYTOMACQUEUELIMITREACHED_ID = 21,
  /** A counter that is incremented each time a packet was dropped due to a packet
   *  validation error. This could be due to length or other formatting problems
   *  in the packet. */
  ZDO_DIAGNOSTICS_PACKET_VALIDATEDROPCOUNT_ID = 22,
  /** @brief PacketBufferAllocateFeatures, Zigbee Diagnostic Cluster spec 1.2.2.2.24 */
  ZDO_DIAGNOSTICS_AVERAGE_MAC_RETRY_PER_APS_ID = 23,
#ifdef ZB_ENABLE_NWK_RETRANSMIT
  /** A counter that is incremented on the NWK layer
   *  each time tries number of a packet resending are gone.
   *
   * @note It's a non-standard counter that depends on ZB_ENABLE_NWK_RETRANSMIT and
   *       will be zero always when the macro isn't set. */
  ZDO_DIAGNOSTICS_NWK_RETRY_OVERFLOW_ID = 24,
#endif /* ZB_ENABLE_NWK_RETRANSMIT */
  /** A non-standard counter of the number of times the NWK broadcast was dropped
   *  because the broadcast table was full. */
  ZDO_DIAGNOSTICS_NWK_BCAST_TABLE_FULL = 25
} zdo_diagnostics_counter_id_t;

/*! @} */ /* Diagnostics cluster attributes */


/** @cond internals_doc */
/*! @brief Increment diagnostic attribute
    @param attr_id - attribute Id, see @ref zdo_diagnostics_counter_id_e
*/
#ifdef  ZDO_DIAGNOSTICS_DEBUG_TRACE
void zdo_diagnostics_inc(zdo_diagnostics_counter_id_t counter_id, zb_uint_t trace_file_id, zb_uint_t trace_line);
#else
void zdo_diagnostics_inc(zdo_diagnostics_counter_id_t counter_id);
#endif

/*! @brief Increment ZDO_DIAGNOSTICS_ROUTE_DISC_INITIATED_ID diagnostics counter
    @param param - buffer Id, verifies that it contains the NWK Command
                   and the NWK Command == ZB_NWK_CMD_ROUTE_REQUEST
*/
void zdo_diagnostics_route_req_inc(zb_uint8_t param);

/**
 * @brief Get full statistics from ZDO and MAC. @see zdo_diagnostics_full_stats_t
 *
 * @note Don't call this function directly!
 *       Use @ZDO_DIAGNOSTICS_GET_STATS or @ZDO_DIAGNOSTICS_GET_AND_CLEANUP_STATS
 *
 * @param cb - callback that will be called with the users parameter.
 * @param pib_attr - MAC PIB attribute to get stats from MAC.
 * @return RET_BUSY if another request is active; RET_OK otherwise.
 */
zb_ret_t zdo_diagnostics_get_stats(zb_callback_t cb, zb_uint8_t pib_attr);

/**
 * @brief Initialize diagnostics context
 */
void zdo_diagnostics_init(void);

#ifdef  ZDO_DIAGNOSTICS_DEBUG_TRACE
#define ZDO_DIAGNOSTICS_INC(counter_id) zdo_diagnostics_inc(counter_id, ZB_TRACE_FILE_ID, __LINE__)
#else
#define ZDO_DIAGNOSTICS_INC(counter_id) zdo_diagnostics_inc(counter_id)
#endif

#define ZDO_DIAGNOSTICS_ROUTE_REQ_INC(param) zdo_diagnostics_route_req_inc(param)

#define ZDO_DIAGNOSTICS_INIT() zdo_diagnostics_init()

#define ZDO_DIAGNOSTICS_GET_STATS(cb)                                   \
  zdo_diagnostics_get_stats((cb), ZB_PIB_ATTRIBUTE_IEEE_DIAGNOSTIC_INFO)

#define ZDO_DIAGNOSTICS_GET_AND_CLEANUP_STATS(cb)                       \
  zdo_diagnostics_get_stats((cb), ZB_PIB_ATTRIBUTE_GET_AND_CLEANUP_DIAG_INFO)

/*! @}
 *  @endcond */ /* internals_doc */

/*! @} */ /* ZCL Diagnostics cluster definitions */

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

#else /* defined(ZDO_DIAGNOSTICS) || defined(DOXYGEN) */

#define ZDO_DIAGNOSTICS_INC(counter_id)
#define ZDO_DIAGNOSTICS_ROUTE_REQ_INC(param)
#define ZDO_DIAGNOSTICS_INIT()

#define ZDO_DIAGNOSTICS_GET_STATS(cb)
#define ZDO_DIAGNOSTICS_GET_AND_CLEANUP_STATS(cb)

#endif /* defined(ZDO_DIAGNOSTICS) || defined(DOXYGEN) */

#endif /* ZDO_DIAGNOSTICS_H */
