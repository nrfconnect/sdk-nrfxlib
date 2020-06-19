/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2020 DSR Corporation, Denver CO, USA.
 * http://www.dsr-zboss.com
 * http://www.dsr-corporation.com
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

#if ! defined ZB_ZCL_DIAGNOSTICS_H
#define ZB_ZCL_DIAGNOSTICS_H

#if defined(ZB_ZCL_SUPPORT_CLUSTER_DIAGNOSTICS) || defined(DOXYGEN)

#include "zcl/zb_zcl_common.h"
#include "zcl/zb_zcl_commands.h"

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

/*! @brief Structure saved all diagnostic attribute */
typedef struct zb_zcl_diagnostics_data_s
{
  /*! @brief numberOfResets, Zigbee Diagnostic Cluster spec 1.2.2.1.1 */
  zb_uint16_t numberOfResets;
  /*! @brief MacRxBcast, HA spec 9.3.2.2.2 */
  zb_uint16_t macRxBcast;
  /*! @brief MacTxBcast, HA spec 9.3.2.2.2 */
  zb_uint16_t macTxBcast;
  /*! @brief MacTxUcast, HA spec 9.3.2.2.2 */
  zb_uint16_t macTxUcast;
  /*! @brief APSTxBcast, HA spec 9.3.2.2.2 */
  zb_uint16_t apsTxBcast;
  /*! @brief APSTxUcastSuccess, HA spec 9.3.2.2.2 */
  zb_uint16_t apsTxUcastSuccess;
  /*! @brief APSTxUcastzb_zcl_diagnostics_data_t diagnostics_data;Fail, HA spec 9.3.2.2.2 */
  zb_uint16_t apsTxUcastFail;
  /*! @brief JoinIndication, Zigbee Diagnostic Cluster spec 1.2.2.2.17 */
  zb_uint16_t joinIndication;
  /*! @brief AverageMACRetryPerAPSMessageSent, HA spec 9.3.2.2.2.27 */
  zb_uint16_t averageMACRetryPerAPSMessageSent;
  /*! @brief JoinIndication, Zigbee Diagnostic Cluster spec 1.2.2.2.24  */
  zb_uint16_t packetBufferAllocateFeatures;
  /*! @brief LastMessageLQI, HA spec 9.3.2.2.2. */
  zb_uint8_t lastMessageLQI;
  /*! @brief LastMessageRSSI, HA spec 9.3.2.2.2. */
  zb_int8_t lastMessageRSSI;
} zb_zcl_diagnostics_data_t;




extern zb_zcl_diagnostics_data_t diagnostics_data;


/*! @brief Diagnostics cluster attribute identifiers
    @see HA spec, Diagnostics Cluster 9.3.2.2.2
*/
enum zb_zcl_diagnostics_attr_e
{
  /** @brief NumberOfResets, Zigbee Diagnostic Cluster spec 1.2.2.1.1 */
  ZB_ZCL_ATTR_DIAGNOSTICS_NUMBER_OF_RESETS_ID                = 0x0000,
  /** This attribute keeps track of the number of writes to persistent memory. */
  ZB_ZCL_ATTR_DIAGNOSTICS_PERSISTENT_MEMORY_WRITES_ID        = 0x0001,
  /** @brief MacRxBcast, HA spec 9.3.2.2.2 */
  ZB_ZCL_ATTR_DIAGNOSTICS_MAC_RX_BCAST_ID                    = 0x0100,
  /** @brief MacTxBcast, HA spec 9.3.2.2.2 */
  ZB_ZCL_ATTR_DIAGNOSTICS_MAC_TX_BCAST_ID                    = 0x0101,
  /** MacRxUcast Attribute A counter that is incremented each time the MAC
   *  layer receives a unicast. */
  ZB_ZCL_ATTR_DIAGNOSTICS_MAC_RX_UCAST_ID                    = 0x0102,
  /** @brief MacTxUcast, HA spec 9.3.2.2.2 */
  ZB_ZCL_ATTR_DIAGNOSTICS_MAC_TX_UCAST_ID                    = 0x0103,
  /** MacTxUcastRetry Attribute A counter that is incremented each time
   *  the MAC layer retries a unicast. */
  ZB_ZCL_ATTR_DIAGNOSTICS_MAC_TX_UCAST_RETRY_ID              = 0x0104,
  /** MacTxUcastFail Attribute A counter that is incremented each time
   *  the MAC layer fails to send a unicast. */
  ZB_ZCL_ATTR_DIAGNOSTICS_MAC_TX_UCAST_FAIL_ID               = 0x0105,
  /** APSRxBcast Attribute A counter that is incremented each time
   *  the APS layer receives a broadcast. */
  ZB_ZCL_ATTR_DIAGNOSTICS_APS_RX_BCAST_ID                    = 0x0106,
  /** @brief APSTxBcast, HA spec 9.3.2.2.2 */
  ZB_ZCL_ATTR_DIAGNOSTICS_APS_TX_BCAST_ID                    = 0x0107,
  /** APSRxUcast Attribute A counter that is incremented each time
   *  the APS layer receives a unicast. */
  ZB_ZCL_ATTR_DIAGNOSTICS_APS_RX_UCAST_ID                    = 0x0108,
  /** @brief APSTxUcastSuccess, HA spec 9.3.2.2.2 */
  ZB_ZCL_ATTR_DIAGNOSTICS_APS_TX_UCAST_SUCCESS_ID            = 0x0109,
  /** APSTxUcastRetry Attribute A counter that is incremented each time
   *  the APS layer retries the sending of a unicast. */
  ZB_ZCL_ATTR_DIAGNOSTICS_APS_TX_UCAST_RETRY_ID              = 0x010A,
  /** @brief APSTxUcastFail, HA spec 9.3.2.2.2 */
  ZB_ZCL_ATTR_DIAGNOSTICS_APS_TX_UCAST_FAIL_ID               = 0x010b,
  /** RouteDiscInitiated Attribute A counter that is incremented each time
   *  a route request is initiated . */
  ZB_ZCL_ATTR_DIAGNOSTICS_ROUTE_DISC_INITIATED_ID            = 0x010C,
  /** NeighborAdded Attribute A counter that is incremented each time
   *  an entry is added to the neighbor table. */
  ZB_ZCL_ATTR_DIAGNOSTICS_NEIGHBOR_ADDED_ID                  = 0x010D,
  /** NeighborRemoved Attribute A counter that is incremented each time
   *  an entry is removed from the neighbor table. */
  ZB_ZCL_ATTR_DIAGNOSTICS_NEIGHBOR_REMOVED_ID                = 0x010E,
  /** A counter that is incremented each time a neighbor table entry becomes stale
   *  because the neighbor has not been heard from. */
  ZB_ZCL_ATTR_DIAGNOSTICS_NEIGHBOR_STALE_ID                  = 0x010F,
  /** @brief JoinIndication, HA spec 1.2.2.2.17 */
  ZB_ZCL_ATTR_DIAGNOSTICS_JOIN_INDICATION_ID                 = 0x0110,
  /** A counter that is incremented each time an entry is removed from the child table. */
  ZB_ZCL_ATTR_DIAGNOSTICS_CHILD_MOVED_ID                     = 0x0111,
  /** A counter that is incremented each time a message is dropped at the network
   *  layer because the APS frame counter was not higher than the last message seen
   *  from that source. */
  ZB_ZCL_ATTR_DIAGNOSTICS_NWKFC_FAILURE_ID                   = 0x0112,
  /** A counter that is incremented each time a message is dropped at the APS layer
   *  because the APS frame counter was not higher than the last message seen from
   *  that source. */
  ZB_ZCL_ATTR_DIAGNOSTICS_APSFC_FAILURE_ID                   = 0x0113,
  /** A counter that is incremented each time a message is dropped at the APS layer
   *  because it had APS encryption but the key associated with the sender has
   *  not been authenticated, and thus the key is not authorized for use
   *  in APS data messages. */
  ZB_ZCL_ATTR_DIAGNOSTICS_APS_UNAUTHORIZED_KEY_ID            = 0x0114,
  /** A counter that is incremented each time a NWK encrypted message was received
   *  but dropped because decryption failed. */
  ZB_ZCL_ATTR_DIAGNOSTICS_NWK_DECRYPT_FAILURES_ID            = 0x0115,
  /** A counter that is incremented each time an APS encrypted message was received
   *  but dropped because decryption failed. */
  ZB_ZCL_ATTR_DIAGNOSTICS_APS_DECRYPT_FAILURES_ID            = 0x0116,
  /** @brief AverageMACRetryPerAPSMessageSent, HA spec 9.3.2.2.2.27 */
  ZB_ZCL_ATTR_DIAGNOSTICS_PACKET_BUFFER_ALLOCATE_FAILURES_ID = 0x0117,
  /** A counter that is incremented each time a unicast packet is relayed. */
  ZB_ZCL_ATTR_DIAGNOSTICS_RELAYED_UCAST_ID                   = 0x0118,
  ZB_ZCL_ATTR_DIAGNOSTICS_PHYTOMACQUEUELIMITREACHED_ID       = 0x0119,
  /** A counter that is incremented each time a packet was dropped due to a packet
   *  validation error. This could be due to length or other formatting problems
   *  in the packet. */
  ZB_ZCL_ATTR_DIAGNOSTICS_PACKET_VALIDATEDROPCOUNT_ID        = 0x011A,
  /** @brief PacketBufferAllocateFeatures, Zigbee Diagnostic Cluster spec 1.2.2.2.24 */
  ZB_ZCL_ATTR_DIAGNOSTICS_AVERAGE_MAC_RETRY_PER_APS_ID       = 0x011b,
  /** @brief LastMessageLQI, HA spec 9.3.2.2.2. */
  ZB_ZCL_ATTR_DIAGNOSTICS_LAST_LQI_ID                        = 0x011c,
  /** @brief LastMessageRSSI, HA spec 9.3.2.2.2. */
  ZB_ZCL_ATTR_DIAGNOSTICS_LAST_RSSI_ID                       = 0x011d,
};

/** @brief Default value for NumberOfResets attribute */
#define ZB_ZCL_DIAGNOSTICS_NUMBER_OF_RESETS_DEFAULT_VALUE ((zb_uint16_t)0x00000000)

/** @brief Default value for PersistentMemoryWrites attribute */
#define ZB_ZCL_DIAGNOSTICS_PERSISTENT_MEMORY_WRITES_DEFAULT_VALUE ((zb_uint16_t)0x00000000)

/** @brief Default value for MacRxBcast attribute */
#define ZB_ZCL_DIAGNOSTICS_MAC_RX_BCAST_DEFAULT_VALUE ((zb_uint32_t)0)

/** @brief Default value for MacTxBcast attribute */
#define ZB_ZCL_DIAGNOSTICS_MAC_TX_BCAST_DEFAULT_VALUE ((zb_uint32_t)0)

/** @brief Default value for MacRxUcast attribute */
#define ZB_ZCL_DIAGNOSTICS_MAC_RX_UCAST_DEFAULT_VALUE ((zb_uint32_t)0)

/** @brief Default value for MacTxUcast attribute */
#define ZB_ZCL_DIAGNOSTICS_MAC_TX_UCAST_DEFAULT_VALUE ((zb_uint32_t)0)

/** @brief Default value for MacTxUcastRetry attribute */
#define ZB_ZCL_DIAGNOSTICS_MAC_TX_UCAST_RETRY_DEFAULT_VALUE ((zb_uint16_t)0)

/** @brief Default value for MacTxUcastFail attribute */
#define ZB_ZCL_DIAGNOSTICS_MAC_TX_UCAST_FAIL_DEFAULT_VALUE ((zb_uint16_t)0)

/** @brief Default value for APSRxBcast attribute */
#define ZB_ZCL_DIAGNOSTICS_APS_RX_BCAST_DEFAULT_VALUE ((zb_uint16_t)0)

/** @brief Default value for APSTxBcast attribute */
#define ZB_ZCL_DIAGNOSTICS_APS_TX_BCAST_DEFAULT_VALUE ((zb_uint16_t)0)

/** @brief Default value for APSRxUcast attribute */
#define ZB_ZCL_DIAGNOSTICS_APS_RX_UCAST_DEFAULT_VALUE ((zb_uint16_t)0)

/** @brief Default value for APSTxUcastSuccess attribute */
#define ZB_ZCL_DIAGNOSTICS_APS_TX_UCAST_SUCCESS_DEFAULT_VALUE ((zb_uint16_t)0)

/** @brief Default value for APSTxUcastRetry attribute */
#define ZB_ZCL_DIAGNOSTICS_APS_TX_UCAST_RETRY_DEFAULT_VALUE ((zb_uint16_t)0)

/** @brief Default value for APSTxUcastFail attribute */
#define ZB_ZCL_DIAGNOSTICS_APS_TX_UCAST_FAIL_DEFAULT_VALUE ((zb_uint16_t)0)

/** @brief Default value for RouteDiscInitiated attribute */
#define ZB_ZCL_DIAGNOSTICS_ROUTE_DISC_INITIATED_DEFAULT_VALUE ((zb_uint16_t)0)

/** @brief Default value for NeighborAdded attribute */
#define ZB_ZCL_DIAGNOSTICS_NEIGHBOR_ADDED_DEFAULT_VALUE ((zb_uint16_t)0)

/** @brief Default value for NeighborRemoved attribute */
#define ZB_ZCL_DIAGNOSTICS_NEIGHBOR_REMOVED_DEFAULT_VALUE ((zb_uint16_t)0)

/** @brief Default value for NeighborStale attribute */
#define ZB_ZCL_DIAGNOSTICS_NEIGHBOR_STALE_DEFAULT_VALUE ((zb_uint16_t)0)

/** @brief Default value for JoinIndication attribute */
#define ZB_ZCL_DIAGNOSTICS_JOIN_INDICATION_DEFAULT_VALUE ((zb_uint16_t)0)

/** @brief Default value for ChildMoved attribute */
#define ZB_ZCL_DIAGNOSTICS_CHILD_MOVED_DEFAULT_VALUE ((zb_uint16_t)0)

/** @brief Default value for NWKFCFailure attribute */
#define ZB_ZCL_DIAGNOSTICS_NWKFC_FAILURE_DEFAULT_VALUE ((zb_uint16_t)0)

/** @brief Default value for APSFCFailure attribute */
#define ZB_ZCL_DIAGNOSTICS_APSFC_FAILURE_DEFAULT_VALUE ((zb_uint16_t)0)

/** @brief Default value for APSUnauthorizedKey attribute */
#define ZB_ZCL_DIAGNOSTICS_APS_UNAUTHORIZED_KEY_DEFAULT_VALUE ((zb_uint16_t)0)

/** @brief Default value for NWKDecryptFailures attribute */
#define ZB_ZCL_DIAGNOSTICS_NWK_DECRYPT_FAILURES_DEFAULT_VALUE ((zb_uint16_t)0)

/** @brief Default value for APSDecryptFailures attribute */
#define ZB_ZCL_DIAGNOSTICS_APS_DECRYPT_FAILURES_DEFAULT_VALUE ((zb_uint16_t)0)

/** @brief Default value for PacketBufferAllocateFailures attribute */
#define ZB_ZCL_DIAGNOSTICS_PACKET_BUFFER_ALLOCATE_FAILURES_DEFAULT_VALUE ((zb_uint16_t)0)

/** @brief Default value for RelayedUcast attribute */
#define ZB_ZCL_DIAGNOSTICS_RELAYED_UCAST_DEFAULT_VALUE ((zb_uint16_t)0)

/** @brief Default value for PhytoMACqueuelimitreached attribute */
#define ZB_ZCL_DIAGNOSTICS_PHYTO_MA_CQUEUELIMITREACHED_DEFAULT_VALUE ((zb_uint16_t)0)

/** @brief Default value for PacketValidatedropcount attribute */
#define ZB_ZCL_DIAGNOSTICS_PACKET_VALIDATEDROPCOUNT_DEFAULT_VALUE ((zb_uint16_t)0)

/** @brief Default value for AverageMACRetryPerAPS attribute */
#define ZB_ZCL_DIAGNOSTICS_AVERAGE_MAC_RETRY_PER_APS_DEFAULT_VALUE ((zb_uint16_t)0)

/** @brief Default value for LastLQI attribute */
#define ZB_ZCL_DIAGNOSTICS_LAST_LQI_DEFAULT_VALUE ((zb_uint8_t)0)

/** @brief Default value for LastRSSI attribute */
#define ZB_ZCL_DIAGNOSTICS_LAST_RSSI_DEFAULT_VALUE ((zb_int8_t)0)

/** @cond internals_doc */
/*! @internal @name Diagnostics cluster internals
    Internal structures for attribute representation in cluster definitions.
    @{
*/

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIAGNOSTICS_NUMBER_OF_RESETS_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIAGNOSTICS_NUMBER_OF_RESETS_ID,                  \
  ZB_ZCL_ATTR_TYPE_U16,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIAGNOSTICS_PACKET_BUFFER_ALLOCATE_FAILURES_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIAGNOSTICS_PACKET_BUFFER_ALLOCATE_FAILURES_ID,   \
  ZB_ZCL_ATTR_TYPE_U16,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIAGNOSTICS_JOIN_INDICATION_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIAGNOSTICS_JOIN_INDICATION_ID,                   \
  ZB_ZCL_ATTR_TYPE_U16,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}


#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIAGNOSTICS_MAC_RX_BCAST_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIAGNOSTICS_MAC_RX_BCAST_ID,                      \
  ZB_ZCL_ATTR_TYPE_U32,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIAGNOSTICS_MAC_TX_BCAST_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIAGNOSTICS_MAC_TX_BCAST_ID,                      \
  ZB_ZCL_ATTR_TYPE_U32,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIAGNOSTICS_MAC_TX_UCAST_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIAGNOSTICS_MAC_TX_UCAST_ID,                      \
  ZB_ZCL_ATTR_TYPE_U32,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIAGNOSTICS_APS_TX_BCAST_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIAGNOSTICS_APS_TX_BCAST_ID,                      \
  ZB_ZCL_ATTR_TYPE_U16,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIAGNOSTICS_APS_TX_UCAST_SUCCESS_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIAGNOSTICS_APS_TX_UCAST_SUCCESS_ID,              \
  ZB_ZCL_ATTR_TYPE_U16,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIAGNOSTICS_APS_TX_UCAST_FAIL_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIAGNOSTICS_APS_TX_UCAST_FAIL_ID,                 \
  ZB_ZCL_ATTR_TYPE_U16,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIAGNOSTICS_AVERAGE_MAC_RETRY_PER_APS_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIAGNOSTICS_AVERAGE_MAC_RETRY_PER_APS_ID,         \
  ZB_ZCL_ATTR_TYPE_U16,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIAGNOSTICS_LAST_LQI_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIAGNOSTICS_LAST_LQI_ID,                          \
  ZB_ZCL_ATTR_TYPE_U8,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIAGNOSTICS_LAST_RSSI_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIAGNOSTICS_LAST_RSSI_ID,                         \
  ZB_ZCL_ATTR_TYPE_S8,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}

/*! @} */ /* Diagnostics cluster internals */
/*! @}
 *  @endcond */ /* internals_doc */

/** @brief Declare attribute list for Diagnostics cluster - server side
    @param attr_list - attribute list name
*/
#define ZB_ZCL_DECLARE_DIAGNOSTICS_ATTRIB_LIST(attr_list)                                         \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                                     \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DIAGNOSTICS_NUMBER_OF_RESETS_ID,                               \
                       &(diagnostics_data.numberOfResets))                                        \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DIAGNOSTICS_MAC_RX_BCAST_ID, &(diagnostics_data.macRxBcast))   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DIAGNOSTICS_MAC_TX_BCAST_ID, &(diagnostics_data.macTxBcast))   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DIAGNOSTICS_MAC_TX_UCAST_ID, &(diagnostics_data.macTxUcast))   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DIAGNOSTICS_APS_TX_BCAST_ID, &(diagnostics_data.apsTxBcast))   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DIAGNOSTICS_APS_TX_UCAST_SUCCESS_ID,                           \
                                        &(diagnostics_data.apsTxUcastSuccess))                    \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DIAGNOSTICS_APS_TX_UCAST_FAIL_ID,                              \
                                        &(diagnostics_data.apsTxUcastFail))                       \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DIAGNOSTICS_PACKET_BUFFER_ALLOCATE_FAILURES_ID,                \
                                        &(diagnostics_data.packetBufferAllocateFeatures))         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DIAGNOSTICS_JOIN_INDICATION_ID,                                \
                                        &(diagnostics_data.joinIndication))                       \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DIAGNOSTICS_AVERAGE_MAC_RETRY_PER_APS_ID,                      \
                                        &(diagnostics_data.packetBufferAllocateFeatures))         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DIAGNOSTICS_LAST_LQI_ID, &(diagnostics_data.lastMessageLQI))   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DIAGNOSTICS_LAST_RSSI_ID, &(diagnostics_data.lastMessageRSSI)) \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*! @} */ /* Diagnostics cluster attributes */


/** @cond internals_doc */
/*! @brief Increment diagnostic attribute
    @param attr_id - attribute Id, see @ref zb_zcl_diagnostics_attr_e
*/
zb_void_t zb_zcl_diagnostics_inc(zb_uint16_t attr_id, zb_uint8_t value);

#define ZB_ZCL_DIAGNOSTICS_INC(attr_id)           zb_zcl_diagnostics_inc((attr_id), 0)
#define ZB_ZCL_DIAGNOSTICS_SAVE(attr_id, value)   zb_zcl_diagnostics_inc((attr_id), (value))
/*! @}
 *  @endcond */ /* internals_doc */

/*! @} */ /* ZCL Diagnostics cluster definitions */

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

zb_void_t zb_zcl_diagnostics_init_server(void);
zb_void_t zb_zcl_diagnostics_init_client(void);
#define ZB_ZCL_CLUSTER_ID_DIAGNOSTICS_SERVER_ROLE_INIT zb_zcl_diagnostics_init_server
#define ZB_ZCL_CLUSTER_ID_DIAGNOSTICS_CLIENT_ROLE_INIT zb_zcl_diagnostics_init_client

#else /* defined(ZB_ZCL_SUPPORT_CLUSTER_DIAGNOSTICS) */

#define ZB_ZCL_DIAGNOSTICS_INC(attr_id)
#define ZB_ZCL_DIAGNOSTICS_SAVE(attr_id, value)

#define ZB_ZCL_CLUSTER_ID_DIAGNOSTICS_SERVER_ROLE_INIT
#define ZB_ZCL_CLUSTER_ID_DIAGNOSTICS_CLIENT_ROLE_INIT

#endif /* defined(ZB_ZCL_SUPPORT_CLUSTER_DIAGNOSTICS) */

#endif /* ! defined ZB_ZCL_DIAGNOSTICS_H */
