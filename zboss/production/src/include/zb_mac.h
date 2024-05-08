/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2024 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: Mac layer API. Platform-independent or default.
*/

#ifndef ZB_MAC_API_INCLUDED
#define ZB_MAC_API_INCLUDED

#include "zb_types.h"
#include "zb_debug.h"
#include "zb_bufpool.h"
#include "zb_config_common.h"

/* Include platform-specific MAC stuff from the separate repo */
#include "mac_platform.h"

#ifndef ZB_TRANSCEIVER_START_CHANNEL_NUMBER
  #define ZB_TRANSCEIVER_START_CHANNEL_NUMBER 11U
#endif

#ifndef ZB_TRANSCEIVER_MAX_CHANNEL_NUMBER
  #define ZB_TRANSCEIVER_MAX_CHANNEL_NUMBER   26U
#endif

#define ZB_MAC_IS_ADDRESS_BROADCAST(addr) ( ((addr) & 0xFFF0U) == 0xFFF0U )


/** @cond internals_doc */
/**
   @addtogroup ZB_MAC
   @{
*/

#define ZB_MAC_MIN_HEADER_SIZE  (2U  /* fcf */ + 1U /* seq num */ + 2U /* dst short addr */ + \
  2U  /* pan */ + 2U /*src_short_addr*/)
/**
   Return maximal mac header size
*/
#define ZB_MAC_MAX_HEADER_SIZE(dst_short_addr, src_short_addr)          \
  2U  /* fcf */ + 1U /* seq num */ + ((dst_short_addr) ? 2U : 8U) /* dst addr */ + \
  2U  /* pan */ + ((src_short_addr) ? 2U : 8U) /* src addr */

/**
   offset in bytes from the start of the mac frame to sequence number
   field
*/
#define SEQ_NUMBER_OFFSET           2U
/* check specification IEEE 802.15 item 7.2.1 */

/**
   offset in bytes from the start of the mac frame to destination PAN
   identifier field
*/
#define ZB_MAC_DST_PANID_OFFSET     3U
/* check specification IEEE 802.15 item 7.2.1 */


#define ZB_MAC_ACK_FRAME_LEN ZB_MAC_DST_PANID_OFFSET
/**
   Length of ACK frame to transmit
 */
#define ZB_MAC_TX_ACK_FRAME_LEN ZB_MAC_ACK_FRAME_LEN

/**
   Offset in bytes from the start of the mac frame to destination address
   field if field destination Pan ID is presented in given mac
   frame
*/
#define DST_ADDR_OFFSET             5U
/* check specification IEEE 802.15 item 7.2.1 */

/**
   length in bytes Sequence Number
*/
#define SEQUENCE_NUMBER_LENGTH      2U
/* check specification IEEE 802.15 item 7.2.2.1 */

/**
   length in bytes SuperFrame Specification
*/
#define SUREPFRAME_SPEC_LENGTH      2U
/* check specification IEEE 802.15 item 7.2.2.1 */

/**
   length in bytes GTS Specification
*/
#define GTS_SPEC_LENGTH             1U
/* check specification IEEE 802.15 item 7.3.1.4 */

/**
   length in bytes GTS Directions
*/
#define GTS_DIRECTIONS_LENGTH       1U
/* check specification IEEE 802.15 item 7.3.1.4 */

/**
   length in bytes GTS Descriptor
*/
#define GTS_DESCRIPTOR_LENGTH       3U
/* check specification IEEE 802.15 item 7.3.1.4 */

/**
   length in bytes Pending Address Specification
*/
#define PENDING_ADDRESS_SPEC_LENGTH 1U
/* check specification IEEE 802.15 item 7.3.1.5 */

/**
   length in bytes of and field containing Pan ID
*/
#define PAN_ID_LENGTH               2U
/* check specification IEEE 802.15 item 7.2.1 */

/**
   All supported channels mask
*/
#define ZB_MAC_ALL_CHANNELS_MASK    ZB_TRANSCEIVER_ALL_CHANNELS_MASK

/**
   Maximal pending addresses number
*/
#define MAX_PENDING_ADDRESSES          7U

/**
   MAC overhead for unicast frame with Pan ID compression (normal case when
   sending via nwk), including FCS bytes
*/
#define MAX_MAC_OVERHEAD_SHORT_ADDRS  11U
/**
   Maximal beacon overhead
*/
#define MAX_BCN_OVERHEAD              75U
/**
   Maximal beacon payload size
*/
#define MAX_BCN_PAYLOAD               (MAX_PHY_FRM_SIZE-MAX_BCN_OVERHEAD)
/**
   Minimal CAP length
*/
#define MAC_MIN_CAP_LENGTH 440U  /* mac spec 7.4.1 MAC constants */

/* By default packet 1b length is before data, 2b packet trailer with rssi, lqi. Platform can redefine it. */
#ifndef ZB_MAC_PACKET_LENGTH_SIZE
#define ZB_MAC_PACKET_LENGTH_SIZE 1U
#endif
#ifndef ZB_MAC_EXTRA_DATA_SIZE
#define ZB_MAC_EXTRA_DATA_SIZE    2U /* rssi & lqi - see ns_pkt_trailer_t */
#endif

/**
   Maximal size of MAC header
*/
#define ZB_MAC_MAX_MHR_SIZE  25U

/**
   MAC footer size
*/
#define ZB_MAC_MFR_SIZE  2U

/**
   Maximal size available for NWK layer
*/
#define ZB_MAC_MAX_PAYLOAD_SIZE  (MAX_PHY_FRM_SIZE - (ZB_MAC_MAX_MHR_SIZE + ZB_MAC_MFR_SIZE))

/**
   Is used for macBeaconOrder and macSuperframeOrder
*/
#define ZB_TURN_OFF_ORDER  15U
/**
   Short address value
*/
#define ZB_MAC_SHORT_ADDR_NO_VALUE      0xFFFFU
/**
   Not allocated short address value
*/
#define ZB_MAC_SHORT_ADDR_NOT_ALLOCATED 0xFFFEU
/**
   Broadcast Pan ID value
*/
#define ZB_BROADCAST_PAN_ID 0xFFFFU

/* 7.4 MAC constants and PIB attributes */
/**
   MAC base slot duration value
*/
#define ZB_MAC_BASE_SLOT_DURATION 60U
/**
   Superframe slots number
*/
#define ZB_MAC_NUM_SUPERFRAME_SLOTS 16U
/**
   MAC base superframe duration value
*/
#define ZB_MAC_BASE_SUPERFRAME_DURATION ZB_MAC_BASE_SLOT_DURATION * ZB_MAC_NUM_SUPERFRAME_SLOTS /* = 960 */

/*
  NOTE: 1 beacon interval = ZB_MAC_BASE_SUPERFRAME_DURATION * 1 symbol (16e-6 sec) = 15.36 milliseconds
  Beacon interval is used to measure MAC timeouts
*/

/**
 * @name IEEE 802.15.4 frame version
 * @anchor mac_frame_version
 *
 * Note: These values were members of `enum mac_frame_version_e` type but were converted to a
 * set of macros due to MISRA violations.
 */
/** @{ */
#define MAC_FRAME_IEEE_802_15_4_2003 0U
#define MAC_FRAME_IEEE_802_15_4      1U
#define MAC_FRAME_IEEE_802_15_4_2015 2U
/** @} */

/**
 * @name MAC transmission flags
 * @anchor mac_tx_options_bits
 * @see IEEE standard 802.15
 *
 * Note: These values were members of `enum mac_tx_options_bits_e` type but were converted to a
 * set of macros due to MISRA violations.
 */
/** @{ */
/** 1 - acknowledged transmission, 0 - unacknowledged transmission */
#define MAC_TX_OPTION_ACKNOWLEDGED_BIT                0x01U
/** 1 - GTS transmission, 0 - CAP transmission */
#define MAC_TX_OPTION_GTS_CAP_BIT                     0x02U
/** 1 - indirect transmission, 0 - direct transmission */
#define MAC_TX_OPTION_INDIRECT_TRANSMISSION_BIT       0x04U
#define MAC_TX_OPTION_NO_CSMA_CA                      0x08U /*!< non standard option, need it for GreenPower */
/** temporary flag means ok to transmit indirect. Used with some non-DSR MAC only  */
#define MAC_TX_OPTION_OK_TO_INDIRECT_TRANSMISSION_NOW 0x10U
/** @} */

/**
 * @name MAC frame types
 * @anchor mac_frame_type
 *
 * Note: These values were members of `enum mac_frame_type_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define MAC_FRAME_BEACON         0U
#define MAC_FRAME_DATA           1U
#define MAC_FRAME_ACKNOWLEDGMENT 2U
#define MAC_FRAME_COMMAND        3U
#define MAC_FRAME_RESERVED1      4U
#define MAC_FRAME_RESERVED2      5U
#define MAC_FRAME_RESERVED3      6U
#define MAC_FRAME_RESERVED4      7U
/** @} */

/**
 * @name MAC command frame id
 * @anchor mac_command_frame_id
 *
 * Note: These values were members of `enum mac_command_frame_id_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define MAC_CMD_ASSOCIATION_REQUEST          1U
#define MAC_CMD_ASSOCIATION_RESPONSE         2U
#define MAC_CMD_DISASSOCIATION_NOTIFICATION  3U
#define MAC_CMD_DATA_REQUEST                 4U
#define MAC_CMD_PAN_ID_CONFLICT_NOTIFICATION 5U
#define MAC_CMD_ORPHAN_NOTIFICATION          6U
#define MAC_CMD_BEACON_REQUEST               7U
#define MAC_CMD_COORDINATOR_REALIGNMENT      8U
#define MAC_CMD_GTS_REQUEST                  9U
/** @} */


/* Frame control field macroses */

/**
   Sets frame type subfield in frame control field ( FCF )
   Valid values are defined in @ref mac_frame_type.


   @param p_fcf - pointer to 16bit FCF field.
   @param frame_type - @ref mac_frame_type.
*/


#define ZB_FCF_SET_FRAME_TYPE( p_fcf, frame_type )                      \
  do                                                                    \
  {                                                                     \
    ZB_ASSERT( (zb_uint8_t) ( frame_type ) < MAC_FRAME_RESERVED1 );     \
    ( ( ( zb_uint8_t* ) ( p_fcf ) )[ZB_PKT_16B_ZERO_BYTE] ) &= 0xf8U;   \
    ( ( ( zb_uint8_t* ) ( p_fcf ) )[ZB_PKT_16B_ZERO_BYTE] ) |= (zb_uint8_t) ( frame_type ); \
  } while( 0 )


/**
   Gets frame type subfield in frame control field ( FCF )
   Return values are in range of @see @ref mac_frame_type.

   @param p_fcf - pointer to 16bit FCF field.
*/

#define ZB_FCF_GET_FRAME_TYPE( p_fcf ) ( ( ( ( const zb_uint8_t* ) ( p_fcf ) )[ZB_PKT_16B_ZERO_BYTE] ) & 0x07U )


/**
   Gets security bit subfield in frame control field ( FCF )
   Return values can be 0 or 1.

   @param p_fcf - pointer to 16bit FCF field.
*/

#define ZB_FCF_GET_SECURITY_BIT( p_fcf ) ( ( ( ( const zb_uint8_t* ) ( p_fcf ) )[ZB_PKT_16B_ZERO_BYTE] ) & 0x08U )


/**
   Sets security bit subfield in frame control field ( FCF )
   Input values can be 0 or 1.

   @param p_fcf     - pointer to 16bit FCF field.
   @param bit_value - 0 or 1.
*/

#define ZB_FCF_SET_SECURITY_BIT( p_fcf, bit_value )                     \
  do                                                                    \
  {                                                                     \
    ZB_ASSERT( ( bit_value ) == 0U || ( bit_value ) == 1U );            \
    ( ( ( zb_uint8_t* ) ( p_fcf ) )[ZB_PKT_16B_ZERO_BYTE] ) &= 0xF7U;   \
    ( ( ( zb_uint8_t* ) ( p_fcf ) )[ZB_PKT_16B_ZERO_BYTE] ) |= (bit_value) << 3U; \
  } while( 0 )

/**
   Gets frame pending bit subfield in frame control field ( FCF )
   Return values can be 0 or 1.

   @param p_fcf - pointer to 16bit FCF field.
*/

#define ZB_FCF_GET_FRAME_PENDING_BIT( p_fcf ) (( ( ( const zb_uint8_t* ) ( p_fcf ) )[ZB_PKT_16B_ZERO_BYTE] ) & 0x10U )


/**
   Sets security bit subfield in frame control field ( FCF )

   @param p_fcf     - pointer to 16bit FCF field.
   @param bit_value - 0 or 1.
*/

#define ZB_FCF_SET_FRAME_PENDING_BIT( p_fcf, bit_value )                \
  do                                                                    \
  {                                                                     \
    ZB_ASSERT( ( bit_value ) == 0U || ( bit_value ) == 1U );            \
    ( ( ( zb_uint8_t* ) ( p_fcf ) )[ZB_PKT_16B_ZERO_BYTE] ) &= 0xEFU;   \
    ( ( ( zb_uint8_t* ) ( p_fcf ) )[ZB_PKT_16B_ZERO_BYTE] ) |= (bit_value) << 4U; \
  } while( 0 )

/**
   Gets ack request bit subfield in frame control field ( FCF )
   Return values can be 0 or 1.

   @param p_fcf - pointer to 16bit FCF field.
*/

#define ZB_FCF_GET_ACK_REQUEST_BIT( p_fcf ) ( ( ( ( const zb_uint8_t* ) ( p_fcf ) )[ZB_PKT_16B_ZERO_BYTE] ) & 0x20U )


/**
   Sets ack request bit subfield in frame control field ( FCF )

   @param p_fcf     - pointer to 16bit FCF field.
   @param bit_value - 0 or 1.
*/

#define ZB_FCF_SET_ACK_REQUEST_BIT(p_fcf, bit_value)                                               \
  do                                                                                               \
  {                                                                                                \
    ZB_ASSERT((bit_value) == 0U || (bit_value) == 1U);                                             \
    (((zb_uint8_t *)(p_fcf))[ZB_PKT_16B_ZERO_BYTE]) &= 0xDFU;                                      \
    (((zb_uint8_t *)(p_fcf))[ZB_PKT_16B_ZERO_BYTE]) |= ((bit_value) << 5U);                        \
  } while (ZB_FALSE)


/**
   Get sequence number from MAC FCF
*/
#define ZB_FCF_GET_SEQ_NUMBER(p_fcf) ((const zb_uint8_t*)(p_fcf))[2]


/**
   Gets Pan ID compression bit subfield in frame control field ( FCF )
   Return values can be 0 or 1.

   @param p_fcf - pointer to 16bit FCF field.
*/

#define ZB_FCF_GET_PANID_COMPRESSION_BIT( p_fcf ) (( ( ( ( const zb_uint8_t* ) ( p_fcf ) )[ZB_PKT_16B_ZERO_BYTE] ) & 0x40U ))


/**
   Sets ack request bit subfield in frame control field ( FCF )

   @param p_fcf     - pointer to 16bit FCF field.
   @param bit_value - 0 or 1.
*/

#define ZB_FCF_SET_PANID_COMPRESSION_BIT(p_fcf, bit_value)                                         \
  do                                                                                               \
  {                                                                                                \
    ZB_ASSERT((bit_value) == 0U || (bit_value) == 1U);                                             \
    (((zb_uint8_t *)(p_fcf))[ZB_PKT_16B_ZERO_BYTE]) &= 0xBFU;                                      \
    (((zb_uint8_t *)(p_fcf))[ZB_PKT_16B_ZERO_BYTE]) |= ((bit_value) << 6U);                        \
  } while (ZB_FALSE)


/**
   Gets destination addressing mode subfield in frame control field ( FCF )
   Return values is one value from @ref address_modes.

   @param p_fcf - pointer to 16bit FCF field.
*/

#define ZB_FCF_GET_FRAME_VERSION( p_fcf )  ( (( ( ( ( const zb_uint8_t* ) ( p_fcf ) )[ZB_PKT_16B_FIRST_BYTE] ) & 0x30U )) >> 4U )


/**
   Sets frame version subfield in frame control field ( FCF )
   Return values is one value defined in @ref mac_frame_version.

   @param p_fcf     - pointer to 16bit FCF field.
   @param frame_version -
*/
#define ZB_FCF_SET_FRAME_VERSION( p_fcf, frame_version )                \
  do                                                                    \
  {                                                                     \
    ZB_ASSERT( (frame_version) == MAC_FRAME_IEEE_802_15_4_2003		\
               || (frame_version) == MAC_FRAME_IEEE_802_15_4		\
               || (frame_version) == MAC_FRAME_IEEE_802_15_4_2015 );	\
                                                                        \
    ( ( ( zb_uint8_t* ) ( p_fcf ) )[ZB_PKT_16B_FIRST_BYTE] ) &= 0xCFU;  \
    ( ( ( zb_uint8_t* ) ( p_fcf ) )[ZB_PKT_16B_FIRST_BYTE] ) |= ( frame_version ) << 4U; \
  } while( 0 )


/**
   Gets IE list present bit subfield in frame control field ( FCF )
   Return values can be 0 or 1.

   @param p_fcf - pointer to 16bit FCF field.
*/
#define ZB_FCF_GET_IE_LIST_PRESENT_BIT( p_fcf ) ((( ( ( ( const zb_uint8_t* ) ( p_fcf ) )[ZB_PKT_16B_FIRST_BYTE] ) & 0x2U )) >> 1U)


/**
   Sets IE list present bit subfield in frame control field ( FCF )

   @param p_fcf     - pointer to 16bit FCF field.
   @param bit_value - 0 or 1.
*/
#define ZB_FCF_SET_IE_LIST_PRESENT_BIT(p_fcf, bit_value)                                           \
  do                                                                                               \
  {                                                                                                \
    ZB_ASSERT((bit_value) == 0U || (bit_value) == 1U);                                             \
    (((zb_uint8_t *)(p_fcf))[ZB_PKT_16B_FIRST_BYTE]) &= 0xFDU;                                     \
    (((zb_uint8_t *)(p_fcf))[ZB_PKT_16B_FIRST_BYTE]) |= ((bit_value) << 1U);                       \
  } while (ZB_FALSE)


/**
   Gets source addressing mode subfield in frame control field ( FCF )
   Return values is one value from @ref address_modes.

   @param p_fcf - pointer to 16bit FCF field.
*/

#define ZB_FCF_GET_SRC_ADDRESSING_MODE( p_fcf )  ( (( ( ( ( const zb_uint8_t* ) ( p_fcf ) )[ZB_PKT_16B_FIRST_BYTE] ) & 0xC0U )) >> 6U )

/**
   Sets src addressing subfield in frame control field ( FCF )

   @param p_fcf     - pointer to 16bit FCF field.
   @param addr_mode - 0 or 1.
*/
/* implemented as function zb_fcf_set_src_addressing_mode()
   #define ZB_FCF_SET_SRC_ADDRESSING_MODE( p_fcf, addr_mode )                \
   do                                                                        \
   {                                                                         \
   ZB_ASSERT( ( addr_mode ) == 0 ||                                        \
   ( addr_mode ) == 1 ||                                        \
   ( addr_mode ) == 2 ||                                        \
   ( addr_mode ) == 3    );                                     \
                                                                          \
                                                                          ( ( ( zb_uint8_t* ) ( p_fcf ))[ZB_PKT_16B_FIRST_BYTE] ) &= 0x3F;               \
                                                                          ( ( ( zb_uint8_t* ) ( p_fcf ))[ZB_PKT_16B_FIRST_BYTE] ) |= ( addr_mode ) << 6; \
                                                                          } while( 0 )
*/
void zb_fcf_set_src_addressing_mode(zb_uint8_t *p_fcf, zb_uint8_t addr_mode);
#define ZB_FCF_SET_SRC_ADDRESSING_MODE zb_fcf_set_src_addressing_mode

/**
   Gets source addressing mode subfield in frame control field ( FCF )
   Return values is one value from @ref address_modes.

   @param p_fcf - pointer to 16bit FCF field.
*/

#define ZB_FCF_GET_DST_ADDRESSING_MODE( p_fcf )  (((( ( ( const zb_uint8_t* ) ( p_fcf ) )[ZB_PKT_16B_FIRST_BYTE] ) & 0x0CU) ) >> 2U )


/**
   Sets dst addressing subfield in frame control field ( FCF )

   @param p_fcf     - pointer to 16bit FCF field.
   @param addr_mode - 0 or 1.
*/
/* implemented as function zb_fcf_set_dst_addressing_mode()
   #define ZB_FCF_SET_DST_ADDRESSING_MODE( p_fcf, addr_mode )                    \
   do                                                                            \
   {                                                                             \
   ZB_ASSERT( ( addr_mode ) == 0 ||                                            \
   ( addr_mode ) == 1 ||                                            \
   ( addr_mode ) == 2 ||                                            \
   ( addr_mode ) == 3    );                                         \
                                                                              \
                                                                              ( ( ( ( zb_uint8_t* ) ( p_fcf ) )[ZB_PKT_16B_FIRST_BYTE]) ) &= 0xF3;               \
                                                                              ( ( ( ( zb_uint8_t* ) ( p_fcf ) )[ZB_PKT_16B_FIRST_BYTE]) ) |= ( addr_mode ) << 2; \
                                                                              } while( 0 )
*/
void zb_fcf_set_dst_addressing_mode(zb_uint8_t *p_fcf, zb_uint8_t addr_mode);
#define ZB_FCF_SET_DST_ADDRESSING_MODE zb_fcf_set_dst_addressing_mode

/**
   Gets memory address of FCF field inside MAC frame

   @param p_buf     - pointer to beginning of the MAC header r
   @return            - pointer to 16-bit FCF field
*/

#define ZB_MAC_GET_FCF_PTR( p_buf ) ( ( zb_uint8_t* ) ( p_buf ) )


/**
   Gets length in bytes of source address field inside MAC frame

   @param p_buf     - pointer to beginning of the MAC header
   @param out_len     - name of variable of any int type which has been declared before.
   stored value is in bytes, so following values are possible [ 0, 2, 8 ]

   @b Example
   @code
   int z;
   ZB_FCF_GET_DST_ADDR_LENGTH( p_buf, z );
   @endcode
*/

/**
   Tail size for mac packet.

   This is default value. Platform can redefine it.
*/
#ifndef ZB_TAIL_SIZE_FOR_RECEIVED_MAC_FRAME
#define ZB_TAIL_SIZE_FOR_RECEIVED_MAC_FRAME (ZB_MAC_EXTRA_DATA_SIZE)
#endif

#ifndef ZB_MAC_GET_LQI

/**
   Get LQI value
   @param packet - pointer to buffer
*/
#define ZB_MAC_GET_LQI(packet) *((zb_uint8_t*)zb_buf_end(packet) - ZB_MAC_EXTRA_DATA_SIZE)
#endif

#ifndef ZB_MAC_SET_LQI
/**
   Set LQI value
   @param packet - pointer to buffer
   @param v - LQI value
*/

#define ZB_MAC_SET_LQI(packet, v) *((zb_uint8_t*)zb_buf_end(packet) - ZB_MAC_EXTRA_DATA_SIZE) = (v)
#endif


#define ZB_MAC_ACK_LEN_BYTES   5U

#ifndef ZB_MAC_GET_RSSI
/**
   Get RSSI value
   @param packet - pointer to buffer
*/
#define ZB_MAC_GET_RSSI(packet) *((zb_int8_t*)zb_buf_end(packet) - ZB_MAC_EXTRA_DATA_SIZE + 1U)
#endif

#ifndef ZB_MAC_SET_RSSI
/**
   Set RSSI value
   @param packet - pointer to buffer
   @param v - RSSI value
*/
#define ZB_MAC_SET_RSSI(packet, v) (*((zb_int8_t*)zb_buf_end(packet) - ZB_MAC_EXTRA_DATA_SIZE + 1U) = (v))
#endif

/**
   Remove MAC header and trailer from the packet.

   To be used when passing packet up in NWK.
   Save hdr offset in internal buf structure to be able to get mac hdr data from
   upper layers, i.e. from nwk route discovery

   @param packet - packet to proceed
   @param hlen - length MAC header
   @param ptr    - pointer to the NWK begin
*/
#define ZB_MAC_CUT_HDR(packet, hlen, ptr)                               \
  do                                                                    \
  {                                                                     \
    /* Remove RSSI etc. */                                              \
    zb_buf_cut_right(packet, ZB_TAIL_SIZE_FOR_RECEIVED_MAC_FRAME);      \
    ptr = zb_buf_cut_left(packet, hlen);                                 \
  } while (0)


/*
  NS build adds FCS to the outgoing packet end while HW build does not
*/
#define ZB_TAIL_SIZE_FOR_SENDER_MAC_FRAME 2U

/**
   Gets Beacon Payload offset from the beginning of the Beacon frame

   @param beacon - pointer to beginning of the Beacon frame
   @return beacon payload offset value
*/
zb_ushort_t zb_mac_get_beacon_payload_offset(zb_uint8_t *beacon);

#define ZB_BEACON_PAYLOAD_TAIL (ZB_TAIL_SIZE_FOR_RECEIVED_MAC_FRAME)
#define ZB_BEACON_PAYLOAD_LENGTH_MIN    2U

/* MAC security */
#ifdef ZB_MAC_SECURITY
#define MAC_SECUR_LEV5_KEYID1_AUX_HDR_SIZE 6U
#else
#define MAC_SECUR_LEV5_KEYID1_AUX_HDR_SIZE 0U
#endif

#if defined ZB_MAC_TESTING_MODE
#define MAC_SECUR_CERT_AUX_HDR_SIZE    5U
#define MAC_SECUR_CERT_FRAME_COUNTER   4U
#define MAC_SECUR_CERT_KEY_SEQ_COUNTER 5U
#endif

/**
   Remove MAC header from the packet.

   To be used when passing packet up in NWK.
   Save hdr offset in internal buf structure to be able to get mac hdr data from
   upper layers, i.e. from nwk route discovery

   @param _packet - packet to proceed
   @param _ptr    - pointer to the NWK begin
*/
#define ZB_MAC_CUT_HDR_WITHOUT_TRAILER(_packet, _ptr)                                              \
  do                                                                                               \
  {                                                                                                \
    zb_ushort_t hlen;                                                                              \
    void *mac_hdr = zb_buf_begin(_packet);                                                         \
                                                                                                   \
    /* Save hdr offset */                                                                          \
    hlen = zb_mac_calculate_mhr_length(ZB_FCF_GET_SRC_ADDRESSING_MODE(mac_hdr),                    \
                                       ZB_FCF_GET_DST_ADDRESSING_MODE(mac_hdr),                    \
                                       ZB_U2B(ZB_FCF_GET_PANID_COMPRESSION_BIT(mac_hdr)));         \
    _ptr = zb_buf_cut_left(                                                                        \
        _packet, hlen + MAC_SECUR_LEV5_KEYID1_AUX_HDR_SIZE * ZB_FCF_GET_SECURITY_BIT(mac_hdr));    \
                                                                                                   \
                                                                                                   \
  } while (ZB_FALSE)


/**
   Get full MAC header size
*/
#define ZB_MAC_HDR_FULL_SIZE(_mac_hdr)                                                             \
  (zb_mac_calculate_mhr_length(ZB_FCF_GET_SRC_ADDRESSING_MODE(_mac_hdr),                           \
                               ZB_FCF_GET_DST_ADDRESSING_MODE(_mac_hdr),                           \
                               ZB_U2B(ZB_FCF_GET_PANID_COMPRESSION_BIT(_mac_hdr)))                 \
   + MAC_SECUR_LEV5_KEYID1_AUX_HDR_SIZE * ZB_FCF_GET_SECURITY_BIT(_mac_hdr))

/**
   Holds status of purge operation.
   @note This struct does not contain msduHandle, because it must be in NWK
   header
   @param status - The status of the request to be purged an MSDU
   from the transaction queue ( one of values of mac_status_e )
*/

typedef ZB_PACKED_PRE struct zb_mac_purge_confirm_s
{
  zb_uint8_t status;
} ZB_PACKED_STRUCT
zb_mac_purge_confirm_t;


#define ZB_MAC_CAP_GET_ALTERNATE_PAN_COORDINATOR(cap) ((cap) & 1U)
#define ZB_MAC_CAP_SET_ALTERNATE_PAN_COORDINATOR(cap, v) (cap) |= (v)

#define ZB_MAC_CAP_GET_DEVICE_TYPE(cap) (((cap) >> 1U) & 1U)
#define ZB_MAC_CAP_SET_DEVICE_TYPE(cap, v)               \
  (                                                      \
    (cap) &= 0xFDU, /* clear bit 1 */                    \
    (cap) |= ((v) << 1U) /* set bit if it is == 1 */     \
    )

#define ZB_MAC_CAP_GET_POWER_SOURCE(cap) (((cap) >> 2U) & 1U)
#define ZB_MAC_CAP_SET_POWER_SOURCE(cap, v) ((cap) |= ((v) << 2U))

#define ZB_MAC_CAP_GET_RX_ON_WHEN_IDLE(cap) (((cap) >> 3U) & 1U)
#define ZB_MAC_CAP_SET_RX_ON_WHEN_IDLE(cap, v) ((cap) |= ((v) << 3U))

/**
   Set capabilities typical for router: device type - R, power source - main, rx
   on when idle.
*/
#define ZB_MAC_CAP_SET_ROUTER_CAPS(cap) ((cap) |= (1U << 1U) | (1U << 2U) | (1U << 3U))

#define ZB_MAC_CAP_GET_SECURITY(cap) (((cap) >> 6U) & 1U)
#define ZB_MAC_CAP_SET_SECURITY(cap, v) ((cap) |= ((v) << 6U))

#define ZB_MAC_CAP_GET_ALLOCATE_ADDRESS(cap) (((cap) >> 7U) & 1U)
#define ZB_MAC_CAP_SET_ALLOCATE_ADDRESS(cap, v) ((cap) |= ((v) << 7U))







/**
   Builds MLME disassociate request. The MLME-DISASSOCIATE.request primitive is
   used by an associated device to notify the coordinator of its intent to leave
    PAN. It is also used by the coordinator to instruct an associated device to leave the
   PAN.

   @param buf - pointer ot zb_buf_t container

   @param device_addr_mode - ( valid values  ZB_ADDR_16BIT_DEV_OR_BROADCAST or

   @param device_pan_id -  Pan ID of the device to which the
          disassociation notification command is sent.

   @param device_addr - pointer to the address of the device to which the
          disassociation notification command is sent.

   @param disassociate_reason - one of values of mac_status_e enum

   @param tx_indirect - 1 if the disassociation notification command is to be
   sent indirectly, otherwise must be 0.

   @param out_len - out integer variable to receive length

*/
#define ZB_MLME_BUILD_DISASSOCIATE_REQUEST( buf,                        \
                                            device_addr_mode,           \
                                            device_pan_id,              \
                                            device_addr,                \
                                            disassociate_reason,        \
                                            tx_indirect,                \
                                            out_len )                   \
  do                                                                    \
  {                                                                     \
  } while( 0 );


/**
   Defines struct for MLME disassociate indication primitive.

   @param device_address - the address of the device requesting disassociation.

   @param disassociate_reason - the status of the association attempt. ( value from   mac_status_e enum )

*/
typedef ZB_PACKED_PRE struct zb_mac_disassociate_indication_s
{
  zb_ieee_addr_t device_address;
  zb_uint8_t     disassociate_reason;
} ZB_PACKED_STRUCT
zb_mac_disassociate_indication_t;

/**
   Returns pointer to zb_mac_disassociate_indication_t structure
   located somewhere in buf.

   @param buf    - pointer ot zb_buf_t container
   @param outptr - out pointer to zb_mac_associate_confirm_t struct located
   somewhere inside buf
*/

#define ZB_MLME_GET_DISASSOCIATE_INDICATION_PTR( buf, outptr )  \
  do                                                            \
  {                                                             \
  } while( 0 );

/**
   Defines struct which describes MLME-DISASSOCIATE.confirm primitive.

   @param status - the status of the disassociation attempt. ( one of the
   values list from the mac_status_e enum )

   @param device_addr_mode - ( valid values  ZB_ADDR_16BIT_DEV_OR_BROADCAST or
   ZB_ADDR_64BIT_DEV ). The addressing mode of the device that has either requested
   disassociation or been instructed to disassociate by its coordinator.

 @param device_pan_id - Pan ID of the device that has either requested disassociation or
   been instructed to disassociate by its coordinator.

   @param device_addr - the address of the device that has either requested disassociation or
   been instructed to disassociate by its coordinator.

*/

typedef ZB_PACKED_PRE struct zb_mac_disassociate_confirm_s
{
  zb_uint8_t      status;
  zb_uint8_t      device_addr_mode;
  zb_uint16_t     device_pan_id;
  zb_addr_u device_addr;
} ZB_PACKED_STRUCT
zb_mac_disassociate_confirm_t;


/**
   Returns pointer to zb_mac_disassociate_confirm_t structure
   located somewhere in buf.

   @param buf    - pointer ot zb_buf_t container
   @param outptr - out pointer to zb_mac_associate_confirm_t struct located
   somewhere inside buf
*/

#define ZB_MAC_GET_DISASSOCIATE_CONFIRM_PTR( buf, outptr )      \
  do                                                            \
  {                                                             \
  } while( 0 )

/**
   Define the Superframe Specification field

   @param beacon_order - shall specify the transmission interval of the beacon.
   ( see 7.5.1.1 )

   @param superframe_order - shall specify the length of time during which the
   superframe is active (i.e., receiver enabled),
   including the beacon frame transmission time. See
   7.5.1.1 for explanation of the relationship between
   the superframe order and the superframe duration.

   @param final_cap_slot - specifies the final superframe slot utilized by the CAP.
   The duration of the CAP, as implied by this subfield,
   shall be greater than or equal to the value specified by
   aMinCAPLength. However, an exception is allowed for
   the accommodation of the temporary increase in the
   beacon frame length needed to perform GTS maintenance
   (see 7.2.2.1.3).

   @param battery_life_extension - shall be set to one if frames transmitted to
   the beaconing device during its CAP are
   required to start in or before
   macBattLifeExtPeriods full backoff periods
   after the IFS period following the beacon.
   Otherwise, the BLE subfield shall be set to zero.

   @param reserved - reserved for future needs.

   @param pan_coordinator - shall be set to one if the beacon frame is being
   transmitted by the PAN coordinator. Otherwise, the
   PAN Coordinator subfield shall be set to zero.

   @param associate_permit - shall be set to one if macAssociationPermit is set
   to TRUE ( i.e., the coordinator is accepting
   association to the PAN ). The association permit bit
   shall be set to zero if the coordinator is currently
   not accepting association requests on its network.

*/

typedef ZB_PACKED_PRE struct zb_super_frame_spec_s
{
#ifdef ZB_LITTLE_ENDIAN
  zb_bitfield_t beacon_order:4;
  zb_bitfield_t superframe_order:4;

  zb_bitfield_t final_cap_slot:4;
  zb_bitfield_t battery_life_extension:1;
  zb_bitfield_t reserved:1;
  zb_bitfield_t pan_coordinator:1;
  zb_bitfield_t associate_permit:1;
#else
  zb_bitfield_t final_cap_slot:4;
  zb_bitfield_t battery_life_extension:1;
  zb_bitfield_t reserved:1;
  zb_bitfield_t pan_coordinator:1;
  zb_bitfield_t associate_permit:1;

  zb_bitfield_t beacon_order:4;
  zb_bitfield_t superframe_order:4;
#endif
} ZB_PACKED_STRUCT
zb_super_frame_spec_t;

/* zb_super_frame_spec_t structure is MSBF/LSBF specific, use the
 * following macro to make code MSBF/LSBF portable */

/**
   Sets superframe beacon order
   @param superframe - pointer to superframe
   @param beacon_order - beacon order value
*/
#define ZB_SUPERFRAME_SET_BEACON_ORDER(superframe, beacon_order)        \
  do                                                                    \
  {                                                                     \
    ((zb_uint8_t*)(superframe))[ZB_PKT_16B_ZERO_BYTE] &= 0xF0U;         \
    ((zb_uint8_t*)(superframe))[ZB_PKT_16B_ZERO_BYTE] |= (beacon_order); \
  } while(0)

/**
   Sets superframe order
   @param superframe - pointer to superframe
   @param superframe_order - superframe order value
*/
#define ZB_SUPERFRAME_SET_SUPERFRAME_ORDER(superframe, superframe_order)                           \
  do                                                                                               \
  {                                                                                                \
    ((zb_uint8_t *)(superframe))[ZB_PKT_16B_ZERO_BYTE] &= 0x0FU;                                   \
    ((zb_uint8_t *)(superframe))[ZB_PKT_16B_ZERO_BYTE] |= (zb_uint8_t)((superframe_order) << 4U);  \
  } while (0)

/**
   Sets superframe fcs
   @param superframe - pointer to superframe
   @param fcs - fcs value
*/
#define ZB_SUPERFRAME_SET_FINAL_CAP_SLOT(superframe, fcs)               \
  do                                                                    \
  {                                                                     \
    ((zb_uint8_t*)(superframe))[ZB_PKT_16B_FIRST_BYTE] &= 0xF0U; /*1111.0000*/ \
    ((zb_uint8_t*)(superframe))[ZB_PKT_16B_FIRST_BYTE] |= (fcs);        \
  } while(0)

/**
   Sets superframe ble
   @param superframe - pointer to superframe
   @param ble - superframe ble value
*/
#define ZB_SUPERFRAME_SET_BLE(superframe, ble)                          \
  do                                                                    \
  {                                                                     \
    ((zb_uint8_t*)(superframe))[ZB_PKT_16B_FIRST_BYTE] &= 0xEFU; /*1110.1111*/ \
    ((zb_uint8_t*)(superframe))[ZB_PKT_16B_FIRST_BYTE] |= (ble) << 4U;  \
  } while(0)

/**
   Sets superframe Pan coordinator flag
   @param superframe - pointer to superframe
   @param pan_coord - Pan coordinator flag value
*/
#define ZB_SUPERFRAME_SET_PAN_COORD(superframe, pan_coord)              \
  do                                                                    \
  {                                                                     \
    ((zb_uint8_t*)(superframe))[ZB_PKT_16B_FIRST_BYTE] &= 0xBFU; /*1011.1111*/ \
    ((zb_uint8_t*)(superframe))[ZB_PKT_16B_FIRST_BYTE] |= (pan_coord) << 6U; \
  } while(0)

/**
   Sets superframe association permit flag
   @param superframe - pointer to superframe
   @param ass_permit - association permit flag value
*/
#define ZB_SUPERFRAME_SET_ASS_PERMIT(superframe, ass_permit)            \
  do                                                                    \
  {                                                                     \
    ((zb_uint8_t*)(superframe))[ZB_PKT_16B_FIRST_BYTE] &= 0x7FU; /*0111.1111*/ \
    ((zb_uint8_t*)(superframe))[ZB_PKT_16B_FIRST_BYTE] |= (ass_permit) << 7U; \
  } while(0)


/**
   Define the Beacon Payload field

   see figure 3.49

   @param protocol_id - This field identifies the network layer protocols in use and,
   for purposes of this specification, shall always be set to 0,
   indicating the Zigbee protocols.
   The value 0xff shall also be reserved for future use by
   the Zigbee Alliance.
   ( see Zigbee 3.6.7 )

   @param stack_profile - ZBOSS profile identifier. ( see Zigbee 3.6.7 )

   @param protocol_version - The version of the Zigbee protocol. ( see Zigbee 3.6.7 )

   @param reserved - reserved for future needs.

   @param router_capacity - This value is set to TRUE if this device is capable
   of accepting join requests from router-capable devices
   and is set to FALSE otherwise. ( see Zigbee 3.6.7 )

   @param device_depth - The network depth of this device. A value of 0x00 indicates
   that this device is the Zigbee coordinator for the network.
   ( see Zigbee 3.6.7 )

   @param end_device_capacity - This value is set to TRUE if the device is capable
   of accepting join requests from end devices seeking
   to join the network and is set to FALSE otherwise.
   ( see Zigbee 3.6.7 )

  @param nwkExtendedPANId - The globally unique ID for the PAN for which the beaconing
   device is a member. By default, this is the 64-bit IEEE address
                            of the ZigBee coordinator that formed the network,
   but other values are possible and
   there is no required structure to the address.
                            ( see zigbee 3.6.7 )

   @param txoffset - This value indicates the difference in time, measured in symbols,
   between the beacon transmission time of the device and
   the beacon transmission time of its parent;
   This offset may be subtracted from the beacon transmission time
   of the device to calculate the beacon transmission time of the parent.
   This parameter is set to the default value of 0xFFFFFF
   in beaconless networks.
   ( see Zigbee 3.6.7 )

   @param nwkUpdateId - This field reflects the value of nwkUpdateId from the NIB.
   ( see Zigbee 3.6.7 )

*/
typedef ZB_PACKED_PRE struct  zb_mac_beacon_payload_s
{
  zb_uint8_t protocol_id;       /**< for purposes of this specification,
                                   shall always be set to 0,
                                   indicating the Zigbee protocols.
                                */
#ifdef ZB_LITTLE_ENDIAN
  zb_bitfield_t stack_profile:4;
  zb_bitfield_t protocol_version:4;

  /* Used by Amazon WWAH, reserved otherwise */
  zb_bitfield_t long_uptime:1;
  zb_bitfield_t tc_connectivity:1;

  zb_bitfield_t router_capacity:1;
  zb_bitfield_t device_depth:4;
  zb_bitfield_t end_device_capacity:1;

#else /* #ifdef ZB_LITTLE_ENDIAN */

  zb_bitfield_t long_uptime:1;
  zb_bitfield_t tc_connectivity:1;

  zb_bitfield_t router_capacity:1;
  zb_bitfield_t device_depth:4;
  zb_bitfield_t end_device_capacity:1;

  zb_bitfield_t stack_profile:4;
  zb_bitfield_t protocol_version:4;
#endif /* #else ZB_LITTLE_ENDIAN */

  zb_ext_pan_id_t extended_panid;
  zb_uint8_t txoffset[3];         /**< This parameter is set to the
                                     default value of 0xFFFFFF in
                                     a beaconless networks.  */
  zb_uint8_t nwk_update_id;       /*!< This field reflects the value of
                                    nwkUpdateId from the NIB. */
} ZB_PACKED_STRUCT
zb_mac_beacon_payload_t;


/**
   Gets Superframe specification from beacon frame

   @param mhr         - pointer to beginning of the MAC header
   @param mhr_len     - MAC header length (must be already known - use \see zb_parse_mhr
   @param superframe  - out pointer to superframe specification

   @b Example
   @code
   zb_uint8_t *mac_hdr;
   ZB_GET_SUPERFRAME( mhr, mhr_len, superframe );
   @endcode
*/
#define ZB_GET_SUPERFRAME(mhr, mhr_len, superframe)             \
  ZB_LETOH16(superframe, ((const zb_uint8_t*)(mhr) + (mhr_len)))


/**
   Gets GTS fields from beacon frame

   @param mhr         - pointer to beginning of the MAC header
   @param mhr_len     - MAC header length (must be already known - use \see zb_parse_mhr
   @param gts_fields_p  - out pointer to GTS fields

   @b Example
   @code
   zb_mac_gts_fields_t *beacon_payload;
   ZB_MAC_GET_GTS_FIELDS( p_buf, mhr_len, gts_fields );
   @endcode
*/
#define ZB_MAC_GET_GTS_FIELDS(mhr, mhr_len, gts_fields_p )              \
  (gts_fields_p) = (void *)(((zb_uint8_t *)mhr) + mhr_len + SUREPFRAME_SPEC_LENGTH)


/**
   Get superframe spec from the Beacon packet.

   @param mhr         - pointer to beginning of the MAC header
   @param mhr_len     - MAC header length (must be already known - use \see zb_parse_mhr
   @param superframe_p - out pointer to Superframe

*/
#define ZB_MAC_GET_SUPERFRAME_SPEC(mhr, mhr_len, superframe_p)  \
  superframe_p = (void *)(((zb_uint8_t *)mhr) + mhr_len);



/**
   Gets Beacon Order subfield in Superframe Specification field ( SFS )

   @param p_sfs - pointer to 16bit SFS field.
*/

#define ZB_MAC_GET_BEACON_ORDER( p_sfs ) ((zb_uint_t)((((zb_uint8_t*)(p_sfs))[ZB_PKT_16B_ZERO_BYTE]) & 0x07U))

/**
   Gets Association Permit subfield in Superframe Specification field ( SFS )

   @param p_sfs - pointer to 16bit SFS field.
*/

#define ZB_MAC_GET_ASSOCIATION_PERMIT( p_sfs ) ((zb_uint_t)((((zb_uint8_t*)(p_sfs))[ZB_PKT_16B_FIRST_BYTE] >> 7U) & 1U))

/**
   Gets Superframe Order subfield in Superframe Specification field ( SFS )

   @param p_sfs - pointer to 16bit SFS field.
*/

#define ZB_MAC_GET_SUPERFRAME_ORDER( p_sfs ) ((zb_uint_t)((((zb_uint8_t*)(p_sfs))[ZB_PKT_16B_ZERO_BYTE] >> 4U) & 0xFU))

/**
   Gets Pan Coordinator subfield in Superframe Specification field ( SFS )

   @param p_sfs - pointer to 16bit SFS field.
*/

#define ZB_MAC_GET_SUPERFRAME_PAN_COORDINATOR( p_sfs ) ((zb_uint_t)((((zb_uint8_t*) (p_sfs))[ZB_PKT_16B_FIRST_BYTE] >> 6U) & 1U))


/**
   Defines Pan descriptor structure

   @param coord_addr_mode - the coordinator addressing mode corresponding
   to the received beacon frame.
   ( valid values  ZB_ADDR_16BIT_DEV_OR_BROADCAST or
   ZB_ADDR_64BIT_DEV ).

   @param coord_pan_id - Pan ID of the coordinator as specified
   in the received beacon frame.

   @param coord_address - pointer to 16-bit or 64-bit IEEE address. This value
   depends on coord_addr_mode.

   @param channel_page - the current channel page occupied by the network

   @param logical_channel - the current logical channel occupied by the
   network.

   @param super_frame_spec - the superframe specification as specified in the
   received beacon frame.

   @param gts_permit - TRUE if the beacon is from the Pan coordinator that
   is accepting GTS requests.

   @param LinkQuality - the LQI at which the network beacon was received.
*/


typedef ZB_PACKED_PRE struct zb_mac_pan_descriptor_s    // 7.1.5.1.1 table-41
{
  zb_uint8_t            coord_addr_mode;
  zb_uint16_t           coord_pan_id;
  zb_addr_u       coord_address;    /* pointer to 16-bit or 64-bit address, this
                                           * depends on value of coord_addr_mode  */
  zb_uint8_t            channel_page;
  zb_uint8_t            logical_channel;
  zb_super_frame_spec_t super_frame_spec;
  zb_uint8_t            gts_permit;
  zb_uint8_t            link_quality;
  /* Zigbee does not use security and uses beaconless mode, so skip other Pan descriptor
     fields - for timestamp and security  */
#if defined ZB_MAC_TESTING_MODE
  zb_time_t             timestamp; /* Optional Timestamp field */
#endif
#if defined ZB_ENHANCED_BEACON_SUPPORT
  zb_uint16_t           enh_beacon_nwk_addr; /* the field is required for enhanced beacons handling */
#endif /* ZB_ENHANCED_BEACON_SUPPORT */
} ZB_PACKED_STRUCT
zb_pan_descriptor_t;

/**
   Defines pending address specification field.

   @param num_of_short_addr - indicates the number of 16-bit short addresses
   contained in the Address List field of the beacon frame.
   @param rvd0 - reserved bit.
   @param num_of_long_addr -indicates the number of IEEE-addresses
   contained in the Address List field of the beacon frame.
   @param rvd1 - reserved bit.
*/
typedef ZB_PACKED_PRE struct  zb_pending_address_spec_s
{
  zb_bitfield_t num_of_short_addr:3;
  zb_bitfield_t rvd0:1;
  zb_bitfield_t num_of_long_addr:3;
  zb_bitfield_t rvd1:1;
} ZB_PACKED_STRUCT
zb_pending_address_spec_t;

/**

   Parameters for MLME-SCAN.request primitive

   @param buf      - pointer ot zb_buf_t container
   @param type     - one value of @ref mac_scan_type

   @param _channel_page - the channel page on which to perform the scan

   @param _channels - bitmap. Only first 27 bits are used.
   Bits (b0, b1,... b26) indicate which channels are to be scanned
   ( 1 = scan, 0 = do not scan) for each of the 27 channels supported by hardware.
   UBEK transciever maps 16 IEEE 802.15.4 2.4GHz band channels to following interval [ 11, 26 ].
   So values of all others bits except bits from 11 to 26 are ignored

   @param duration - a value used to calculate the length of time to spend scanning each channel for ED,
   active, and passive scans. This parameter is ignored for orphan scans. The time spent scanning
   each channel is [ aBaseSuperframeDuration * ( 2n + 1 ) ] symbols, where n is the value of the
   ScanDuration parameter.

   @param mac_iface_id - index of interface that will be used for scanning
   TODO [Multi-MAC] will it be better to just extract iface_id from _channel_page and _channels?
*/
#define ZB_MLME_BUILD_SCAN_REQUEST( buf, _channel_page, _channels, type, duration, mac_iface_id)     \
do                                                                                                   \
{                                                                                                    \
  zb_mlme_scan_params_t *_p = zb_buf_alloc_tail((buf), sizeof(zb_mlme_scan_params_t));               \
  _p->channel_page = _channel_page;                                                                  \
  _p->channels = _channels;                                                                          \
  _p->scan_type = type;                                                                              \
  _p->scan_duration = duration;                                                                      \
  _p->iface_id = mac_iface_id;                                                                       \
} while( 0 )

#define ZB_MLME_SCAN_REQUEST_IE_SIZES_HDR_LEN 2U

/**
 * Write total header IE and payload IE sizes into buffer for mlme-scan.req
 * After these two bytes HIE and PIE themselves follow
 */
#define ZB_MLME_SCAN_REQUEST_SET_IE_SIZES_HDR(ptr, hie_len, pie_len)    \
  do                                                                    \
  {                                                                     \
    (ptr)[ZB_PKT_16B_ZERO_BYTE] = (hie_len) & 0xFFU;                    \
    (ptr)[ZB_PKT_16B_FIRST_BYTE] = (pie_len) & 0xFFU;                   \
  } while( 0 )



/**
   Defines structure for MAC-MLME.confirm primitive

   @param status - one of values of \ref mac_status

   @param scan_type - one of values of @ref mac_scan_type

   @param channel_page - the channel page on which the scan was performed

   @param result_list_size - the number of elements returned in the appropriate
                             result lists. This value is zero for the result of
                             an orphan scan.

   @param unscanned_channels - indicates which channels given in the
                               request were not scanned
                               ( 1 = not scanned, 0 = scanned or not requested).
                               This parameter is not valid for ED scans.

   @param energy_detect - the array of energy measurements, one for each channel
                          searched during an ED scan. This parameter is null for
                          active, passive, and orphan scans.

   @param pan_descriptor - The list of Pan descriptors, one for each beacon
                           found during an active or passive scan if
                           macAutoRequest is set to TRUE. This parameter is null
                           for ED and orphan scans or when macAutoRequest is set
                           to FALSE during an active or passive scan.
*/


typedef ZB_PACKED_PRE struct zb_mac_scan_confirm_s
{
  zb_uint8_t  iface_id;
  zb_uint8_t  status;
  zb_uint8_t  scan_type;
  zb_uint8_t  channel_page;
  zb_uint8_t  result_list_size;
  zb_uint32_t unscanned_channels;

  ZB_PACKED_PRE union
  {
    zb_uint8_t           energy_detect[ ZB_MAC_SUPPORTED_CHANNELS ];
  } ZB_PACKED_STRUCT list;
} ZB_PACKED_STRUCT
zb_mac_scan_confirm_t;


/** @} */
/** @endcond */

/**
 *  @addtogroup mac_pib
 *  @{
 */

/**
 * @name MAC PIB attributes
 * @anchor mac_pib_attr
 */
/** @{ */
/* PHY PIB */
#define ZB_PHY_PIB_CURRENT_CHANNEL 0x00U
#define ZB_PHY_PIB_CURRENT_PAGE    0x04U

/* MAC PIB */
#define ZB_PIB_ATTRIBUTE_ACK_WAIT_DURATION            0x40U
#define ZB_PIB_ATTRIBUTE_ASSOCIATION_PERMIT           0x41U
#define ZB_PIB_ATTRIBUTE_AUTO_REQUEST                 0x42U
#define ZB_PIB_ATTRIBUTE_BATT_LIFE_EXT                0x43U
#define ZB_PIB_ATTRIBUTE_BATT_LIFE_EXT_PERIODS        0x44U
#define ZB_PIB_ATTRIBUTE_BEACON_PAYLOAD               0x45U
#define ZB_PIB_ATTRIBUTE_BEACON_PAYLOAD_LENGTH        0x46U
#define ZB_PIB_ATTRIBUTE_BEACON_ORDER                 0x47U
#define ZB_PIB_ATTRIBUTE_BEACON_TX_TIME               0x48U
#define ZB_PIB_ATTRIBUTE_BSN                          0x49U
#define ZB_PIB_ATTRIBUTE_COORD_EXTEND_ADDRESS         0x4aU
#define ZB_PIB_ATTRIBUTE_COORD_SHORT_ADDRESS          0x4bU
#define ZB_PIB_ATTRIBUTE_DSN                          0x4cU
#define ZB_PIB_ATTRIBUTE_GTS_PERMIT                   0x4dU
#define ZB_PIB_ATTRIBUTE_MAX_CSMA_BACKOFFS            0x4eU
#define ZB_PIB_ATTRIBUTE_MIN_BE                       0x4fU
#define ZB_PIB_ATTRIBUTE_PANID                        0x50U
#define ZB_PIB_ATTRIBUTE_PROMISCUOUS_MODE             0x51U
#define ZB_PIB_ATTRIBUTE_RX_ON_WHEN_IDLE              0x52U
#define ZB_PIB_ATTRIBUTE_SHORT_ADDRESS                0x53U
#define ZB_PIB_ATTRIBUTE_SUPER_FRAME_ORDER            0x54U
#define ZB_PIB_ATTRIBUTE_TRANSACTION_PERSISTENCE_TIME 0x55U
#define ZB_PIB_ATTRIBUTE_ASSOCIATED_PAN_COORD         0x56U
#define ZB_PIB_ATTRIBUTE_MAX_BE                       0x57U
#define ZB_PIB_ATTRIBUTE_MAX_FRAME_TOTAL_WAIT_TIME    0x58U
#define ZB_PIB_ATTRIBUTE_MAX_FRAME_RETRIES            0x59U
#define ZB_PIB_ATTRIBUTE_RESPONSE_WAIT_TIME           0x5aU
#define ZB_PIB_ATTRIBUTE_SYNC_SYMBOL_OFFSET           0x5bU
#define ZB_PIB_ATTRIBUTE_TIMESTAMP_SUPPORTED          0x5cU
#define ZB_PIB_ATTRIBUTE_SECURITY_ENABLED             0x5dU

/* Non-standard values */
#define ZB_PIB_ATTRIBUTE_EXTEND_ADDRESS                 0x81U /*!< long MAC address */
#define ZB_PIB_ATTRIBUTE_NO_AUTO_ACK                    0x82U /*!< for toggling auto ack on platform */
/** data indication callback when in promisc mode */
#define ZB_PIB_ATTRIBUTE_PROMISCUOUS_MODE_CB            0x83U
#define ZB_PIB_ATTRIBUTE_PROMISCUOUS_RX_ON              0x84U /*!< switch rx on/off when in sniffer mode */
#define ZB_PIB_ATTRIBUTE_SRC_MATCH                      0x85U /*!< Source match for MAC ACK pending bit */
#define ZB_PIB_ATTRIBUTE_JOINING_IEEE_LIST              0x86U /*!< mibJoiningIeeeList, r22 */
#define ZB_PIB_ATTRIBUTE_JOINING_IEEE_LIST_LENGTH       0x8AU /*!< mibJoiningIeeeList length, r22 */
#define ZB_PIB_ATTRIBUTE_JOINING_POLICY                 0x87U /*!< mibJoiningPolicy, r22 */
#define ZB_PIB_ATTRIBUTE_IEEE_EXPIRY_INTERVAL           0x88U /*!< mibIeeeExpiryInterval, r22 */
#define ZB_PIB_ATTRIBUTE_IEEE_EXPIRY_INTERVAL_COUNTDOWN 0x89U /*!< mibIeeeExpiryIntervalCountdown */
#define ZB_PIB_ATTRIBUTE_SKIP_ALL_GPF                   0x8AU /*!< zgp_skip_all_packets */

/* ZBOSS MAC custom attributes */
#define ZB_PIB_ATTRIBUTE_IEEE_DIAGNOSTIC_INFO      0x8FU /*!< Get diagnostics counters. */
/** Get and clear diagnostics counters. Only ZCL counters will be cleared. */
#define ZB_PIB_ATTRIBUTE_GET_AND_CLEANUP_DIAG_INFO 0x90U
/** @} */

/**
 * @brief Type for MAC PIB attributes.
 *
 * @deprecated holds one of @ref mac_pib_attr. Kept only for backward compatibility as
 * @ref mac_pib_attr were declared previously as enum. Can be removed in future releases.
 */
typedef zb_uint8_t zb_mac_pib_attr_t;

/** @} */ /* mac_pib */

/** \addtogroup mac_api */
/** @{ */

/**
 *  @typedef zb_mac_capability_info_t
 *  @brief Defines capability information field as a set of bitfields.
 *  @li bit 0 alternate_PAN_coordinator - the bit is set if the device is capable
 *  of becoming the Pan coordinator. Otherwise the subfield is set to zero.
 *  @li bit 1 device_type - the bit is set if the device is FFD. The zero value indicates that
 *  device is RFD.
 *  @li bit 2 power_source - the bit is set if the device is receiving power from the alternating
 *  current mains. Otherwise, the subfield is set to zero.
 *  @li bit 3 receive_on_when_idle - the bit is set if the device does not disable its receiver to
 *  conserve power during idle periods. Otherwise the bit shall be set to zero.
 *  @li bit 6 security_capability - the bit is set if the device is capable of sending and
 *  receiving cryptographically protected MAC frames, Otherwise shall be set to zero .
 *  @li bit 7 allocate_address - the bit is set if the device wishes the coordinator to allocate
 *  16bit short address as a result of the association procedure. Otherwise it shall be set  to
 *  zero.
 */
typedef zb_uint8_t zb_mac_capability_info_t;

/**
 *  @name MAC common constants and types.
 *  @{
 */

/**
 * @name MAC status
 * @anchor mac_status
 * @brief MAC status (see table 78 in MAC 2006 specification).
 */
/** @{ */
#define MAC_SUCCESS                 0x00U /*!< Transaction was successful */
#define MAC_PAN_AT_CAPACITY         0x01U
#define MAC_PAN_ACCESS_DENIED       0x02U
#define MAC_BEACON_LOSS             0xe0U /*!< Beacon was lost (used in beacon'd networks) */
#define MAC_CHANNEL_ACCESS_FAILURE  0xe1U /*!< Unable to transmit due to channel being busy */
#define MAC_COUNTER_ERROR           0xdBU /*!< Frame counter of received frame is invalid */
#define MAC_DENIED                  0xe2U /*!< GTS request denied */
#define MAC_DISABLE_TRX_FAILURE     0xe3U /*!< Failed to disable the transceiver */
#define MAC_SECURITY_ERROR          0xe4U /*!< Frame failed decryption */
#define MAC_FRAME_TOO_LONG          0xe5U /*!< Frame exceeded maximum size */
#define MAC_IMPROPER_KEY_TYPE       0xdcU /*!< Key not allowed to be used with this frame type */
#define MAC_IMPROPER_SECURITY_LEVEL 0xddU /*!< Frame does not meet min security level expected */
#define MAC_INVALID_ADDRESS         0xf5U /*!< Data request failed because no src or dest address */
#define MAC_INVALID_GTS             0xe6U /*!< Invalid timeslot requested (beacon'd networks) */
#define MAC_INVALID_HANDLE          0xe7U /*!< Invalid frame data handle */
#define MAC_INVALID_INDEX           0xf9U /*!< Invalid index when trying to write MAC PIB */
#define MAC_INVALID_PARAMETER       0xe8U /*!< Invalid parameter passed to service */
#define MAC_LIMIT_REACHED           0xfaU /*!< Scan terminated because max Pan descriptors reached */
#define MAC_NO_ACK                  0xe9U /*!< ACK not received after tx with ack_req flag set */
#define MAC_NO_BEACON               0xeaU /*!< Beacon not returned after beacon request */
#define MAC_NO_DATA                 0xebU /*!< Data frame not returned after data request (indirect poll) */
/** No short address allocated to this device (due to lack of address space) */
#define MAC_NO_SHORT_ADDRESS        0xecU
/** Rx enable request failed. Spec'd number of symbols longer than beacon interval */
#define MAC_ON_TIME_TOO_LONG        0xf6U
/** Association failed due to lack of capacity (no nbor tbl entry or no address) */
#define MAC_OUT_OF_CAP              0xedU
/** Different networks within listening range have identical Pan IDs */
#define MAC_PAN_ID_CONFLICT         0xeeU
/** Rx enable failed. Too late for current superframe and unable to be deferred */
#define MAC_PAST_TIME               0xf7U
#define MAC_READ_ONLY               0xfbU /*!< PIB attribute is read only */
#define MAC_REALIGNMENT             0xefU /*!< Coordinator realignment received */
/** Request to perform scan failed because scan already in progress */
#define MAC_SCAN_IN_PROGRESS        0xfcU
/** Start time of beacon overlapped transmission time of coordinator beacon */
#define MAC_SUPERFRAME_OVERLAP      0xfdU
/** Device not tracking beacons but instructed to send beacons based on tracked beacons */
#define MAC_TRACKING_OFF            0xf8U
#define MAC_TRANSACTION_EXPIRED     0xf0U  /*!< Frame buffered in indirect queue expired */
#define MAC_TRANSACTION_OVERFLOW    0xf1U  /*!< Exceeded maximum amount of entries in indirect queue */
#define MAC_TX_ACTIVE               0xf2U /*!< Transmission in progress */
#define MAC_UNAVAILABLE_KEY         0xf3U /*!< Security key unavailable */
#define MAC_UNSUPPORTED_ATTRIBUTE   0xf4U /*!< Requested PIB attribute is not supported */
/** 802.15.4 2003 security on frame, but not supported by device */
#define MAC_UNSUPPORTED_LEGACY      0xdeU
#define MAC_UNSUPPORTED_SECURITY    0xdfU /*!< Security on received frame is not supported */
#define MAC_PURGED                  0xdaU /*!< Custom status: the packet has been purged */
#define MAC_INTERRUPTED             0xd0U /*!< Custom status: the operation has been interrupted */
/** @} */

/**
 * @brief Type for MAC status.
 *
 * Holds one of @ref mac_status.
 */
typedef zb_uint8_t zb_mac_status_t;


/**
 *  @brief PHY enumerations descriptions
 */
typedef enum zb_phy_status_e
{
  PHY_BUSY                  = 0x00,
  PHY_BUSY_RX               = 0x01,
  PHY_BUSY_TX               = 0x02,
  PHY_FORCE_TRX_OFF         = 0x03,
  PHY_IDLE                  = 0x04,
  PHY_INVALID_PARAMETER     = 0x05,
  PHY_RX_ON                 = 0x06,
  PHY_SUCCESS               = 0x07,
  PHY_TRX_OFF               = 0x08,
  PHY_TX_ON                 = 0x09,
  PHY_UNSUPPORTED_ATTRIBUTE = 0x0a,
  PHY_READ_ONLY             = 0x0b
} zb_phy_status_t;

/**
 *  @name Address Modes
 *  @anchor address_modes
 *  Address modes for different protocol layers.
 *  Hope can use same constants without re-assign.
 */
/** @{ */
/** 802.15: 0x00 = no address (addressing fields omitted, see 7.2.1.1.8). */
#define ZB_ADDR_NO_ADDR                  0x00U
/** 802.15: 0x01 = reserved.
 *  NWK:    0x01 = 16-bit multicast group address. */
#define ZB_ADDR_16BIT_MULTICAST          0x01U
/** APS:    0x01 = 16-bit group address for DstAddress. */
#define ZB_ADDR_16BIT_GROUP  ZB_ADDR_16BIT_MULTICAST
/** 802.15: 0x02 = 16-bit short address.
 *  NWK:    0x02 = 16-bit network address of a device or a 16-bit broadcast address. */
#define ZB_ADDR_16BIT_DEV_OR_BROADCAST   0x02U
/** 802.15: 0x03 = 64-bit extended address. */
#define ZB_ADDR_64BIT_DEV                0x03U
/** @} */

/**
 * @brief Type for address modes (different protocol layers).
 *
 * Designed to hold one of @ref address_modes.
 */
typedef zb_uint8_t zb_addr_mode_t;

/** @} */ /* MAC common constants and types. */

/**
 *  @name MAC data service constants and API.
 *  @{
 */

/* Structure is used for both NWK and ZGP data requests. For ZGP data request,
 * source address is not used, but tx_at field matters.
 * The structure is used for memory consumption optimization.
 */
#if defined (__ICCARM__)
/* Probably IAR version needs to refine */
#if (__VER__ > 6040002)  /* If iccarm version is fresh than 6.40.2 ---------- */
ZB_PACKED_PRE
#endif
#elif __CC_ARM
ZB_PACKED_PRE
#endif
union zb_addr_time_u
{
  zb_uint16_t  addr_short;
  zb_ieee_addr_t addr_long;
  zb_time_t tx_at;              /**< No src addr. Transmit ZGPDF at given time  */
};

/** @brief Parameters for data request. */
typedef ZB_PACKED_PRE struct zb_mcps_data_req_params_s
{
#ifndef ZB_MAC_EXT_DATA_REQ
  /* Quite old, from 2007: short addresses only */
  zb_uint16_t src_addr;             /**< Source address */
  zb_uint16_t dst_addr;             /**< Destination address */
#else
  /* In Zigbee 2007 never need to send data to/from long address, but need it for
   * MAC certification testing and for interpan (ZLL). So actually we are here always. */
  union zb_addr_time_u src_addr;         /**< Source address */
  zb_addr_u      dst_addr;         /**< Destination address */
  zb_uint16_t    dst_pan_id;       /**< Destination Pan ID */
#endif  /* ZB_MAC_EXT_DATA_REQ */
  zb_bitfield_t tx_options:5;      /**< Options. @see mac_tx_options_bits */
  zb_bitfield_t src_addr_mode:2;      /**< Source address mode, one of @ref address_modes */
  zb_bitfield_t reserved1:1;
  zb_bitfield_t mhr_len:5;         /* mhr length, filled by MAC and used internally.
                                    * Can't be >= 32 */
  zb_bitfield_t dst_addr_mode:2;      /**< Destination address mode, one of @ref address_modes */
  zb_bitfield_t reserved2:1;
#ifdef ZB_ENABLE_NWK_RETRANSMIT
  zb_uint8_t    nwk_retry_cnt;
#endif
  zb_uint8_t    msdu_handle;          /**< */
#ifdef ZB_MAC_SECURITY
  zb_uint8_t      security_level;   /**< */
  zb_uint8_t      key_id_mode;      /**< */
  zb_uint8_t      key_source[8];    /**< */
  zb_uint8_t      key_index;        /**< */
#endif
  zb_uint8_t iface_id;
#ifdef ZB_MAC_TESTING_MODE
  ZB_PACKED_PRE struct
  {
    zb_bitfield_t invalid_fcs: 1;                 /**< Invalid FCS for TP/154/MAC/FRAME-VALIDATION-01 */
    zb_bitfield_t reserved_frame_type: 1;         /**< Reserved frame type for TP/154/MAC/FRAME-VALIDATION-02 */
    zb_bitfield_t security_enabled: 1;            /**< Security enabled frame for TP/154/MAC/FRAME-VALIDATION-03 */
    zb_bitfield_t delay_frame_transmission: 1;    /* Delay to be able to send packet at the end of
                                                   * tx window (for TP/154/MAC/DATA-04) */
    zb_bitfield_t reserved: 4;                    /**< Reserved bits */
  } ZB_PACKED_STRUCT cert_hacks;
#endif /* ZB_MAC_TESTING_MODE */
} ZB_PACKED_STRUCT
zb_mcps_data_req_params_t;

/* Assert that MCPS Data Request parameters will fit inside the buffer with the full length MAC frame (TP/154/MAC/DATA-01.h)
 *
 * IEEE 802.15.4 frame header (9 bytes):
 *  - FCS: 2 bytes
 *  - Seq: 1 byte
 *  - DST PAN: 2 bytes
 *  - DST short: 2 bytes
 *  - SRC short: 2 bytes
 *
 * IEEE 802.15.4 frame payload (116 bytes)
 */
ZB_ASSERT_COMPILE_DECL(ZB_IO_BUF_SIZE > (9 + 0x74 + (sizeof(zb_mcps_data_req_params_t) + ZB_BUF_ALLOC_ALIGN - 1) / ZB_BUF_ALLOC_ALIGN * ZB_BUF_ALLOC_ALIGN));


/*!< MSDU handle for GPDFs sent from ZGP EP in Direct mode  */
#define ZB_MAC_DIRECT_GPDF_MSDU_HANDLE   0xE1U


/** @brief Parameters for data confirm call. */
typedef ZB_PACKED_PRE struct zb_mcps_data_confirm_params_s
{
  /* Put some data_req parameters into confirm. Confirm call always removes MAC hdr now. */
  zb_addr_u     src_addr;         /**< Source address */
  zb_addr_u     dst_addr;         /**< Destination address */
  zb_uint16_t   dst_pan_id;       /**< Destination Pan ID */
  zb_mac_status_t status;
  zb_bitfield_t tx_options:4;      /**< Options. @see mac_tx_options_bits. Note: no space for MAC_TX_OPTION_OK_TO_INDIRECT_TRANSMISSION_NOW used internally in non-DSR MAC adopter only */
  zb_bitfield_t src_addr_mode:2;   /**< Source address mode, one of @ref address_modes */
  zb_bitfield_t dst_addr_mode:2;   /**< Destination address mode, one of @ref address_modes */

#ifdef ZB_ENABLE_NWK_RETRANSMIT
  zb_uint8_t nwk_retry_cnt;
#endif
  zb_uint8_t msdu_handle;   /**< MSDU handle value. */
#if defined ZB_MAC_TESTING_MODE
  zb_time_t timestamp;      /**< Timestamp of TX done */
#endif
  zb_uint8_t iface_id;
} ZB_PACKED_STRUCT zb_mcps_data_confirm_params_t;

/**
 *  @brief MCPS-DATA.request.
 *  Accepts data request.
 *  @param param - reference to buffer, contains upper layer data to send.
 */
void zb_mcps_data_request(zb_uint8_t param);

/**
 *  @brief MCPS-DATA.indication primitive.
 *  This function called via scheduler by the MAC layer to pass incoming data packet to the NWK
 *  layer.
 *  @note This function must be defined in the NWK layer!  MAC layer just calls this
 *  function.
 *  @param param - reference to buffer, conatains nsdu, the set of octets comprising the NSDU to be
 *  transferred (with length).
 *
 *  Other fields got from MAC nsdu by macros
 */
void zb_mcps_data_indication(zb_uint8_t param);

/**
 *  @brief MCPS-DATA.confirm primitive.
 *  This function called via scheduler by the MAC layer to pass incoming data packet to the NWK
 *  layer.
 *  @note zb_mcps_data_indication() must be defined in the NWK layer! MAC layer just
 *  calls this function.
 *  @param param - reference to buffer, contains nsdu - the set of octets comprising the NSDU to be
 *  transferred (with length).
 *
 *  Other fields got from MAC nsdu by macros.
 */
void zb_mcps_data_confirm(zb_uint8_t param);

/** @} */ /* MAC data service constants ans API */

/**
 *  @name MAC management service.
 *  @{
 */

/**
 *  @brief MCPS-POLL.indication primitive.
 *  This function called via scheduler by the MAC layer to pass information from
 *  incoming data poll to the NWK layer.
 *  @note This function must be defined in the NWK layer!  MAC layer just calls this function.
 *  @param param - reference to buffer, conatains nsdu, the set of octets comprising the NSDU to be
 *  transferred (with length).
 *
 *  Other fields got from MAC nsdu by macros
 */
void zb_mcps_poll_indication(zb_uint8_t param);


/** @brief Defines MLME-POLL.indication primitive. */
typedef ZB_PACKED_PRE struct zb_mlme_poll_indication_s
{
  zb_uint16_t         addr_mode;
  zb_uint16_t         dev_addr;
} ZB_PACKED_STRUCT
zb_mlme_poll_indication_t;


/**
 * Union for constant sized callback type.
 * We need such a type in case of architecture split.
 */
#if defined (__ICCARM__)
/* Probably IAR version needs to refine */
#if (__VER__ > 6040002)  /* If iccarm version is fresh than 6.40.2 ---------- */
ZB_PACKED_PRE
#endif
#elif __CC_ARM
ZB_PACKED_PRE
#endif
union zb_cb_align64_u
{
  zb_uint64_t u64;
  zb_callback_t cb;
};

/** @brief Defines MLME-GET.request primitive. */
typedef ZB_PACKED_PRE struct zb_mlme_get_request_s
{
  zb_uint8_t            pib_attr;
  zb_uint8_t            iface_id;
  union zb_cb_align64_u confirm_cb_u;
} ZB_PACKED_STRUCT zb_mlme_get_request_t;

/** @brief Defines MLME-GET.confirm primitive. */
typedef ZB_PACKED_PRE struct zb_mlme_get_confirm_s
{
  zb_mac_status_t    status;
  zb_uint8_t  pib_attr;
  zb_uint8_t         iface_id;
  zb_uint8_t         pib_length;
} ZB_PACKED_STRUCT
zb_mlme_get_confirm_t;

/** @brief Defines MLME-SET.request primitive. */
typedef ZB_PACKED_PRE struct zb_mlme_set_request_s
{
  zb_uint8_t  pib_attr;
  zb_uint8_t         iface_id;
  zb_uint8_t         pib_length;
  union zb_cb_align64_u    confirm_cb_u;
} ZB_PACKED_STRUCT
zb_mlme_set_request_t;

/** @brief Defines MLME-SET.confirm primitive. */
typedef ZB_PACKED_PRE struct zb_mlme_set_confirm_s
{
  zb_mac_status_t status;
  zb_uint8_t pib_attr;
  zb_uint8_t iface_id; /*!< MAC Interface #  */
  ZB_PACKED_PRE union zb_cb_align64_u confirm_cb_u;
} ZB_PACKED_STRUCT zb_mlme_set_confirm_t;

#if defined ZB_JOINING_LIST_SUPPORT
/* @brief Custom structure for MLME-GET.req when fetching ZB_PIB_ATTRIBUTE_JOINING_IEEE_LIST
 * Must be located after zb_mlme_get_request_t.
 * At 08/16/17 passing buffer params to MLME-GET.req will cause problems with mac split
 */
typedef ZB_PACKED_PRE struct zb_mlme_get_ieee_joining_list_req_s
{
  zb_uint8_t         start_index;
  zb_uint8_t         count;
} ZB_PACKED_STRUCT
zb_mlme_get_ieee_joining_list_req_t;

/* @brief to be used as payload for zb_mlme_get_confirm_t */
typedef ZB_PACKED_PRE struct zb_mlme_get_ieee_joining_list_res_s
{
  zb_uint8_t         start_index;
  zb_uint8_t         total_length;
  zb_uint8_t         joining_policy;
  zb_uint8_t         addr_count;
} ZB_PACKED_STRUCT
zb_mlme_get_ieee_joining_list_res_t;

/* @brief */
typedef enum zb_mlme_set_ieee_joining_list_req_type_e
{
  ZB_MLME_SET_IEEE_JL_REQ_RANGE = 0,
  ZB_MLME_SET_IEEE_JL_REQ_INSERT,
  ZB_MLME_SET_IEEE_JL_REQ_ERASE,
  ZB_MLME_SET_IEEE_JL_REQ_CLEAR,
} zb_mlme_set_ieee_joining_list_req_type_t;

typedef ZB_PACKED_PRE struct zb_mlme_set_ieee_joining_list_range_s
{
  zb_uint8_t         start_index;    /* the first element to be replaced */
  zb_uint8_t         items_count;
  zb_uint8_t         joining_policy;
  zb_uint8_t         list_size;      /* list size to set */
} ZB_PACKED_STRUCT
zb_mlme_set_ieee_joining_list_range_t;

typedef ZB_PACKED_PRE struct zb_mlme_set_ieee_joining_list_clear_s
{
  zb_uint8_t         joining_policy; /* policy to set on clearing */
} ZB_PACKED_STRUCT
zb_mlme_set_ieee_joining_list_clear_t;

/* @brief to be used as payload for zb_mlme_set_request_t */
typedef ZB_PACKED_PRE struct zb_mlme_set_ieee_joining_list_req_s
{
  zb_uint8_t         op_type;   /* operation type */
  ZB_PACKED_PRE union
  {
    zb_mlme_set_ieee_joining_list_range_t range_params;
    zb_mlme_set_ieee_joining_list_clear_t clear_params;
    zb_ieee_addr_t ieee_value;
  } ZB_PACKED_STRUCT param;
} ZB_PACKED_STRUCT
zb_mlme_set_ieee_joining_list_req_t;

#endif /* ZB_JOINING_LIST_SUPPORT */

/**
 *  @brief MLME-GET.request primitive.
 *  @param param - reference to the buffer containing request data.
 *
 *  @snippet aib_nib_pib_test/start_ze.c zb_mlme_get_request
 *
 */
void zb_mlme_get_request(zb_uint8_t param);

#ifdef MAC_DIRECT_PIB_ACCESS
/**
 *  @brief MLME-GET.confirm primitive.
 *  @param param - reference to the buffer containing operation status information.
 */
void zb_mlme_get_confirm(zb_uint8_t param);
#endif /* MAC_DIRECT_PIB_ACCESS */

/**
 *  @brief MLME-SET.request primitive.
 *  @param param - reference to the buffer containing request data.
 *
 *  @snippet aib_nib_pib_test/start_ze.c zb_mlme_set_request
 *
 */
void zb_mlme_set_request(zb_uint8_t param);

#if (defined ZB_ZGPD_ROLE && defined ZB_ENABLE_ZGP) || defined MAC_DIRECT_PIB_ACCESS
/**
 *  @brief MLME-SET.confirm primitive.
 *  @param param - reference to the buffer containing operation status information.
 */
void zb_mlme_set_confirm(zb_uint8_t param);
#endif /* (ZB_ZGPD_ROLE && ZB_ENABLE_ZGP) || MAC_DIRECT_PIB_ACCESS */

/** @brief Parameters for start request. */
typedef ZB_PACKED_PRE struct zb_mlme_start_req_s
{
  zb_uint8_t  iface_id;
  zb_uint16_t pan_id;                               /**< Pan ID */
  zb_uint8_t  logical_channel;                      /**< Logical channel */
  zb_uint8_t  channel_page;                         /**< The channel page to use */
  zb_uint8_t  beacon_order;                         /**< Beacon order */
  zb_uint8_t  superframe_order;                     /**< Superframe order */
  zb_bitfield_t  pan_coordinator:1;                 /**< Is Coordinator */
  zb_bitfield_t  battery_life_extension:1;          /**< IS Battery life extension */
  zb_bitfield_t  coord_realignment:1;               /**< */
  zb_bitfield_t  reserved:5;                        /**< Reserved */
} ZB_PACKED_STRUCT
zb_mlme_start_req_t;

/**
 *  @brief Handles start request.
 *  @param param - reference to buffer, contains zb_mlme_start_req_t parameters for start.
 *
 *  @snippet tp_pro_bv_29_zc2.c zb_mlme_start_request
 *
 */
void zb_mlme_start_request(zb_uint8_t param);


/**
 *  @brief Confirms start procedure.
 *  @param param - reference to buffer.
 */
void zb_mlme_start_confirm(zb_uint8_t param);

/** @brief Parameter for the zb_mlme_reset_request(). */
typedef ZB_PACKED_PRE struct zb_mlme_reset_request_s
{
  zb_uint8_t           set_default_pib; /**< If TRUE, the MAC sublayer is reset, and all MAC PIB
                                           attributes are set to their default values. If FALSE, the
                                           MAC sublayer is reset, but all MAC PIB attributes retain
                                           their values prior to the generation of the
                                           MLME-RESET.request primitive.  */
  zb_uint8_t iface_id;
#ifdef ZB_MAC_TESTING_MODE
  ZB_PACKED_PRE struct
  {
    zb_bitfield_t allow_empty_beacon_payload:1;   /**< Allow sending/receiving empty Beacon payload */
    zb_bitfield_t allow_sending_empty_frames:1;   /**< Allow responding with empty frames to indirect transmission */
    zb_bitfield_t reset_init_only_radio:1;        /**< Only init radio during MLME-RESET.request (for TP_154_MAC_WARM_START_01) */
    zb_bitfield_t lbt_radio_busy_disabled: 1;     /**< Disable blocking radio by LBT while sending frame TP/154/MAC/CHANNEL-ACCESS-04 test procedure 2 */
    zb_bitfield_t reserved: 4;                    /**< Reserved bits */
  } ZB_PACKED_STRUCT cert_hacks;
#endif /* ZB_MAC_TESTING_MODE */
} ZB_PACKED_STRUCT
zb_mlme_reset_request_t;

/**
 *  @brief Handles MLME-RESET.request.
 *  @param param - parameter (packet buffer), see @ref zb_mlme_reset_request_s is on its tail.
 */
void zb_mlme_reset_request(zb_uint8_t param);

/** @brief Parameter for the zb_mlme_reset_confirm(). */
typedef ZB_PACKED_PRE struct zb_mlme_reset_confirm_s
{
  zb_uint8_t status; /**< Status */
  zb_uint8_t iface_id;
} ZB_PACKED_STRUCT
zb_mlme_reset_confirm_t;

/**
 *  @brief MLME-RESET.confirm.
 *  This function called by MAC layer via callback.
 *  @param param - packet buffer. Only its header.status is used.
 */
void zb_mlme_reset_confirm (zb_uint8_t param);

/**
 * @name Possible scan operations
 * @anchor mac_scan_type
 *
 * Note: These values were members of `enum zb_mac_scan_type_e` type but were converted
 * to a set of macros due to MISRA violations.
 */
/** @{ */
#define ED_SCAN              0U /**< ED scan (optional for RFD). */
#define ACTIVE_SCAN          1U /**< active scan (optional for RFD). */
#define PASSIVE_SCAN         2U /**< passive scan. */
#define ORPHAN_SCAN          3U /**< orphan scan.  */
#define ENHANCED_ACTIVE_SCAN 4U /**< enhanced active scan. */
/** @} */

/**
   Table 67 MLME-SCAN.request parameters

   Table 67defines more parameters but NWK sends only 3 parameters.
   Others should be set by MAC to some constants.

*/

typedef ZB_PACKED_PRE struct
{
  zb_uint8_t  scan_type;   /**< Indicates the type of scan performed. See @ref mac_scan_type
                              0x00 = ED scan (optional for RFD).
                              0x01 = active scan (optional for RFD).
                              0x02 = passive scan.
                              0x03 = orphan scan.
                              0x04 = enhanced active scan.
			   */
  zb_uint8_t  scan_duration; /**< A value used to calculate the length of time to
                                spend scanning each channel for ED, active,
                                and passive scans. This parameter is ignored for
                                orphan scans.
                                The time spent scanning each channel is
                                [aBaseSuperframeDuration * (2**n + 1)]
                                symbols, where n is the value of the
                                ScanDuration parameter.  */
  zb_uint8_t  channel_page;  /**< The channel page on which to perform the scan.  */
  zb_uint32_t    channels;   /**< The 27bits (b , b ,... b ) indicate which
                                channels are to be scanned (1=scan, 0=do not
                                scan) for each of the 27 channels supported by
                                the ChannelPage parameter.  */
  zb_uint8_t iface_id;      /**< MAC interface index */
} ZB_PACKED_STRUCT zb_mlme_scan_params_t;

/**
 *  @brief Handles scan request.
 *  @param param - reference to buffer, contains @ref zb_mlme_scan_params_t parameters for scan.
 *
 *  @snippet start_ze_s05.c zb_mlme_scan_request
 */
void zb_mlme_scan_request(zb_uint8_t param);


/**
 *  @brief Confirms scan procedure.
 *  @param param - reference to buffer.
 */
void zb_mlme_scan_confirm(zb_uint8_t param);


void zb_mac_cancel_scan(zb_bufid_t buf);

void zb_mac_cancel_scan_response(zb_bufid_t buf);

/**
 * @brief Beacon type
 */
typedef enum zb_mac_beacon_type_e
{
  ZB_MAC_BEACON_TYPE_BEACON,
  ZB_MAC_BEACON_TYPE_ENHANCED_BEACON
} zb_mac_beacon_type_t;

/**
 *  @brief Defines MLME-BEACON-NOTIFY.indication.
 *  @param bsn - the beacon sequence number (or enhanced beacon sequence number).
 *  @param pan_descriptor - the Pan Descriptor for the received.
 *  @param pend_addr_spec - the beacon pending address specification.
 *  @param addr_list - the list of addresses of the devices for which the beacon source has data.
 *  @param beacon_type - indicates whether a beacon or an enhanced beacon was received.
 *  @param sdu_length - the number of octets contained in the beacon payload of the beacon frame
 *  received by the MAC sublayer.
 *  @param total_hie_size - the total size of header IEs stored in sdu, in bytes.
 *  @param total_pie_size - the total size of payload IEs stored in sdu, in bytes.
 *
 *  @param rssi - custom field; used to execute Beacons Survey procedure in WWAH and r23
 *  @param sdu - depending on beacon_type:
 *
 *  1. For beacons: the set of octets comprising the beacon payload to be transferred
 *    from the MAC sublayer entity to the next higher layer.
 *
 *  2. For enhanced beacons: the set of octets containing all received IEs:
 *    first come header IEs - with the total size of total_hie_size
 *    then come payload IEs - with the total size of total_pie_size
 */
typedef ZB_PACKED_PRE struct zb_mac_beacon_notify_indication_s
{
  zb_uint8_t                iface_id;
  zb_uint8_t                bsn;
  zb_pan_descriptor_t       pan_descriptor;
  zb_pending_address_spec_t pend_addr_spec;
  zb_addr_u                 addr_list[ MAX_PENDING_ADDRESSES ];
  zb_uint8_t                ebsn;
  zb_mac_beacon_type_t      beacon_type;
  zb_uint8_t                sdu_length;
  zb_uint8_t                total_hie_size;
  zb_uint8_t                total_pie_size;
  zb_int8_t                 rssi;
  zb_uint8_t                sdu[1];
} ZB_PACKED_STRUCT
zb_mac_beacon_notify_indication_t;

/**
 *  @brief MLME-BEACON-NOTIFY.indication primitive.
 *  This function called via scheduler by the MAC layer to pass incoming beacon indication to the
 *  NWK layer.
 *  @note zb_mlme_beacon_notify_indication() must be defined in the NWK layer! MAC
 *  layer just calls this function.
 *  @param param - reference to buffer containing @ref zb_mac_beacon_notify_indication_s.
 */
void zb_mlme_beacon_notify_indication(zb_uint8_t param);

/**
 * @brief Data structure for mlme_associate_request.
 * Some fields are not set - (set explicitly by MAC):
 * @li CoordAddrMode - 3 (64-bit address)
 * @li SecurityLevel - ?
 * @li KeyIdMode     - ?
 * @li KeySource     - ?
 * @li KeyIndex      - ?
 */
typedef ZB_PACKED_PRE struct zb_mlme_associate_params_s
{
  zb_uint16_t     pan_id;
  zb_addr_u coord_addr;
  zb_uint8_t      coord_addr_mode;
  zb_uint8_t      channel_page;
  zb_uint8_t      logical_channel;
  zb_mac_capability_info_t capability;
} ZB_PACKED_STRUCT
  zb_mlme_associate_params_t;

/**
 *  @brief Builds MLME-ASSOCIATE request. This request allows a device to request an association
 *  with a coordinator.
 *  @param buf - pointer to @ref zb_buf_t.
 *  @param _channel_page - the channel page on which to attempt association.
 *  @param _logical_channel - selected from the available logical channels supported by the PHY.
 *  @param coord_PAN_id - the identifier of the PAN with which to associate.
 *  @param addr_mode - ZB_ADDR_16BIT_DEV_OR_BROADCAST or ZB_ADDR_64BIT_DEV. See @ref address_modes.
 *  @param p_address - pointer of the address of the coordinator with which to associate, Suppose
 *  64-bit address mode.
 *  @param _capability - zb_mac_capability_info_t struct. Specifies the operational capabilities of
 *  the associating device.
 *
 *  snippet association_04_rfd.c ZB_MLME_BUILD_ASSOCIATE_REQUEST
 */
#define ZB_MLME_BUILD_ASSOCIATE_REQUEST( buf,                           \
                                         _channel_page,                 \
                                         _logical_channel,              \
                                         coord_PAN_id,                  \
                                         addr_mode,                     \
                                         p_address,                     \
                                         _capability)                   \
do                                                                    \
{                                                                     \
  zb_mlme_associate_params_t *_p = (zb_mlme_associate_params_t *)ZB_BUF_GET_PARAM((buf), zb_mlme_associate_params_t); \
  _p->channel_page = (_channel_page);                                   \
  _p->logical_channel = (_logical_channel);                             \
  _p->pan_id = (coord_PAN_id);                                          \
  _p->coord_addr_mode = (addr_mode);                                    \
  if ((addr_mode) == ZB_ADDR_64BIT_DEV)                                 \
  {                                                                     \
    ZB_IEEE_ADDR_COPY(_p->coord_addr.addr_long, (p_address));           \
  }                                                                     \
  else                                                                  \
  {                                                                     \
    ZB_MEMCPY(&_p->coord_addr.addr_short, (p_address), sizeof(zb_uint16_t)); \
  }                                                                     \
  _p->capability = ( _capability );                                     \
} while( 0 )

/**
 *  @brief Defines struct for indication of associate request.
 *  @param device_address - IEEE 64bit address.
 *  @param capability - @ref zb_mac_capability_info_t struct. Specifies the operational
 *  capabilities of the associating device.
 */
typedef ZB_PACKED_PRE struct zb_mlme_associate_indication_s
{
  zb_uint8_t               iface_id;
  zb_ieee_addr_t           device_address;
  zb_mac_capability_info_t capability;
  zb_uint8_t               lqi; /* non-standard, but MAC has it and we really
                                 * need it */
} ZB_PACKED_STRUCT zb_mlme_associate_indication_t;

/**
 *  @brief MLME-COMM-STATUS.indication data structure.
 *  Not all fields included.
 *
 *  Currently used after MLME-ASSOCIATE.response only.
 *  When will be used for MLME-ORPHAN.response, maybe, will add some fields.
 */
typedef ZB_PACKED_PRE struct zb_mlme_associate_response_s
{
  zb_ieee_addr_t device_address;
  zb_uint16_t    short_address;
  zb_uint8_t     status;
} ZB_PACKED_STRUCT
zb_mlme_associate_response_t;

/**
 *  @brief Builds MLME-ASSOCIATE response.
 *  @param buf - pointer to @ref zb_buf_t.
 *  @param p_address - pointer to the extended 64-bit IEEE  address of the coordinator with which
 *  to associate, size of address depends on coord_addr_mode parameter.
 *  @param short_addr - 16-bit short device address allocated by the coordinator on successful
 *  association. This parameter is set to 0xffff if the association was unsuccessful.
 *  @param st - one of values of @ref mac_status.
 *
 *  @snippet data_01_ffd.c ZB_MLME_BUILD_ASSOCIATE_RESPONSE
 */
#define ZB_MLME_BUILD_ASSOCIATE_RESPONSE( buf,                          \
                                          p_address,                    \
                                          short_addr,                   \
                                          st)                           \
  do                                                                    \
  {                                                                     \
    zb_mlme_associate_response_t *p = (zb_mlme_associate_response_t *)ZB_BUF_GET_PARAM((buf), zb_mlme_associate_response_t); \
    ZB_IEEE_ADDR_COPY(p->device_address, (p_address));                  \
    p->short_address = (short_addr);                                    \
    p->status = (st);                                                   \
  }                                                                     \
  while( 0 )

/**
 *  @brief Defines structure for MLME-ASSOCIATE confirm primitive.
 *  @param assoc_short_addres - the short device address allocated by the coordinator on successful
 *  association. This parameter will be equal to 0xffff if the association attempt was
 *  unsuccessful.
 *  @param status - the status of the association attempt (value from @ref mac_status).
 */
typedef ZB_PACKED_PRE struct zb_mlme_associate_confirm_s
{
  zb_uint16_t assoc_short_address;
  zb_mac_status_t  status;
  /* more fields (will it be used?):
     SecurityLevel
     KeyIdMode
     KeySource
     KeyIndex
  */
  zb_ieee_addr_t parent_address; /* non-standard field, but we really need it */
} ZB_PACKED_STRUCT zb_mlme_associate_confirm_t;

/**
 *  @brief Returns pointer to @ref zb_mlme_associate_confirm_s structure located somewhere in buf.
 *  @param buf    - pointer to @ref zb_buf_t container.
 *  @param outptr - out pointer to @ref zb_mlme_associate_confirm_s struct located somewhere inside
 *  buf.
 */
#define ZB_MLME_GET_ASSOCIATE_CONFIRM_PTR( buf, outptr )        \
  do                                                            \
  {                                                             \
  } while( 0 )

/**
 *  @brief Handle MLME-ASSOCIATION.request.
 *  @param param - reference to buffer, contains @ref zb_mlme_associate_params_t parameters.
 *
 *  @snippet association_04_rfd.c ZB_MLME_BUILD_ASSOCIATE_REQUEST
 */
void zb_mlme_associate_request(zb_uint8_t param);

/**
 *  @brief Associate responce - coordinator side.
 *  Send response to device.
 *  @param param - reference to buffer.
 *
 *  @snippet data_01_ffd.c ZB_MLME_BUILD_ASSOCIATE_RESPONSE
 */
void zb_mlme_associate_response(zb_uint8_t param);

void zb_mlme_associate_indication(zb_uint8_t param);

void zb_mlme_associate_confirm(zb_uint8_t param);

/** @brief Parameters for poll indication. */
typedef ZB_PACKED_PRE struct zb_mcps_poll_indication_param_s
{
  zb_uint8_t               iface_id;
} ZB_PACKED_STRUCT zb_mcps_poll_indication_param_t;

/** @brief Parameters for poll request. */
typedef ZB_PACKED_PRE struct zb_mlme_poll_request_s
{
  zb_uint8_t  coord_addr_mode;      /**< */
  zb_addr_u coord_addr;           /**< */
  zb_uint16_t     coord_pan_id;         /**< */
  zb_time_t       poll_rate;
} ZB_PACKED_STRUCT
  zb_mlme_poll_request_t;

/**
 *  @brief Handles MLME-poll.confirm.
 *  @param param - parameter (packet buffer), with poll status.
 */
void zb_mlme_poll_confirm(zb_uint8_t param);

void zb_mlme_poll_request(zb_uint8_t param);

/**
 *  @brief Sync loss reasons.
 *  The reason that synchronization was lost.
 */

#define ZB_SYNC_LOSS_REASON_PAN_ID_CONFLICT 0U
#define ZB_SYNC_LOSS_REASON_REALIGNMENT 1U
#define ZB_SYNC_LOSS_REASON_BEACON_LOST 2U

/** @brief Parameters for sync loss indication. */
typedef ZB_PACKED_PRE struct zb_mlme_sync_loss_ind_s
{
  zb_uint16_t pan_id; /* Pan ID with which the device lost
	                   * synchronization or to which it was realigned */
  zb_uint8_t loss_reason; /* Lost syncronization reason */
  zb_uint8_t logical_channel; /* Logical channel */
  zb_uint8_t channel_page; /* Channel page */
} ZB_PACKED_STRUCT
  zb_mlme_sync_loss_ind_t;

/**
 *  @brief MLME-COMM-STATUS.indication data structure.
 *  Not all fields included.
 *
 * Currently used after MLME-ASSOCIATE.response only.
 * When will be used for MLME-ORPHAN.response, maybe, will add some fields.
 */
typedef ZB_PACKED_PRE struct zb_mlme_comm_status_indication_s
{
  zb_addr_u src_addr;   /**< The individual device address of the entity from which the frame
                           causing the error originated.  */
  zb_uint8_t src_addr_mode;
  zb_addr_u dst_addr;   /**< The individual device address of the device for which the frame
                           was intended */
  zb_uint8_t dst_addr_mode;
  zb_mac_status_t  status;      /**< The communications status.
                                   SUCCESS,
                                   TRANSACTION_OVERFLOW,
                                   TRANSACTION_EXPIRED,
                                   CHANNEL_ACCESS_FAILURE,
                                   NO_ACK, COUNTER_ERROR,
                                   FRAME_TOO_LONG,
                                   IMPROPER_KEY_TYPE,
                                   IMPROPER_SECURITY_LEVEL,
                                   SECURITY_ERROR,
                                   UNAVAILABLE_KEY,
                                   UNSUPPORTED_LEGACY,
                                   UNSUPPORTED_SECURITY or
                                   INVALID_PARAMETER
                                */
  zb_uint16_t pan_id;           /* 16-bit Pan ID of the device from which the frame was
                                   received or to which the frame was being sent. */
  zb_bool_t is_orphaning;
} ZB_PACKED_STRUCT
zb_mlme_comm_status_indication_t;

void zb_mlme_comm_status_indication(zb_uint8_t param);

#ifdef ZB_MAC_DUTY_CYCLE_MONITORING

typedef zb_uint8_t zb_mac_duty_cycle_status_t;
#define ZB_MAC_DUTY_CYCLE_STATUS_NORMAL     0U
#define ZB_MAC_DUTY_CYCLE_STATUS_LIMITED    1U
#define ZB_MAC_DUTY_CYCLE_STATUS_CRITICAL   2U
#define ZB_MAC_DUTY_CYCLE_STATUS_SUSPENDED  3U

/** @brief Defines MLME-DUTY_CYCLE-MODE.indication primitive. */
typedef ZB_PACKED_PRE struct zb_mlme_duty_cycle_mode_indication_s
{
  zb_uint8_t status;    /**< Current MAC Duty Cycle mode: @ref
                         * zb_mac_duty_cycle_status_t value */
} ZB_PACKED_STRUCT zb_mlme_duty_cycle_mode_indication_t;


/** @brief MLME-DUTY-CYCLE-MODE.indication primitive.
 *  @details This function is called via scheduler by the MAC layer to pass Duty Cycle mode change
 *  to the NWK layer. Note that zb_mlme_duty_cycle_mode_indication() must be defined at the NWK layer!
 *  MAC layer just calls this function.
 *  @param  param - reference to the buffer containing Duty Cycle mode value.
 */
void zb_mlme_duty_cycle_mode_indication(zb_uint8_t param);

zb_uint_t zb_mac_duty_cycle_get_time_period_sec(void);

#endif  /* ZB_MAC_DUTY_CYCLE_MONITORING */

#if defined ZB_MAC_POWER_CONTROL
typedef ZB_PACKED_PRE struct zb_mac_power_ctrl_info_tbl_ent_s
{
  zb_uint16_t short_addr;
  zb_ieee_addr_t ieee_addr;
  zb_int8_t tx_power;           /* TX power level */
  zb_int8_t last_rssi;          /* Last RSSI level */
  zb_bool_t nwk_negotiated;     /* NWK negotiated */
} ZB_PACKED_STRUCT zb_mac_power_ctrl_info_tbl_ent_t;

typedef ZB_PACKED_PRE struct zb_mlme_get_power_info_tbl_req_s
{
  zb_uint16_t short_addr;
  zb_ieee_addr_t ieee_addr;
} ZB_PACKED_STRUCT zb_mlme_get_power_info_tbl_req_t;

/*!
*   Optimal signal level should be +20dB above the reference sensitivity
*   @ref ZB_EU_FSK_REFERENCE_SENSITIVITY
*   @ref ZB_NA_FSK_REFERENCE_SENSITIVITY
*
*   See reference document 20-66955-016 section D.9.2.4.2. Zigbee Specification
*/
#define ZB_MAC_POWER_CONTROL_OPT_SIGNAL_LEVEL(page) \
  ((ZB_LOGICAL_PAGE_IS_SUB_GHZ_NA_FSK(page) ? (ZB_NA_FSK_REFERENCE_SENSITIVITY) : (ZB_EU_FSK_REFERENCE_SENSITIVITY)) + 20)

/* [VK]: Get/set power information table req/conf functions
 *       were renamed not to trigger MISRAC2012-Rule-5.1:
 *       The external identifier 'func_name' clashes with other
 *       identifier(s) in the first 31 characters 1 time(s).
 */
void zb_mlme_get_power_info_table_request(zb_uint8_t param);

typedef ZB_PACKED_PRE struct zb_mlme_get_power_info_tbl_conf_s
{
  zb_uint8_t status;
  zb_mac_power_ctrl_info_tbl_ent_t ent;
} ZB_PACKED_STRUCT zb_mlme_get_power_info_tbl_conf_t;

void zb_mlme_get_power_info_table_confirm(zb_uint8_t param);

typedef ZB_PACKED_PRE struct zb_mlme_set_power_info_tbl_req_s
{
  zb_mac_power_ctrl_info_tbl_ent_t ent;
} ZB_PACKED_STRUCT zb_mlme_set_power_info_tbl_req_t;

void zb_mlme_set_power_info_table_request(zb_uint8_t param);

typedef ZB_PACKED_PRE struct zb_mlme_set_power_info_tbl_conf_s
{
  zb_uint8_t status;
} ZB_PACKED_STRUCT zb_mlme_set_power_info_tbl_conf_t;

void zb_mlme_set_power_info_table_confirm(zb_uint8_t param);

#endif  /* ZB_MAC_POWER_CONTROL */

/** @} */ /* MAC management service. */

/** @} */


/** @cond internals_doc */
/** \addtogroup ZB_MAC */
/** @{ */

/**
   Parameters for realignment command, mac spec 7.3.8 Coordinator
*/
typedef ZB_PACKED_PRE struct zb_coord_realignment_cmd_s
{
  /* MHR fields - are not described here */
  zb_uint8_t cmd_frame_id;  /**< Command Frame Identifier == 0x08*/
  zb_uint16_t pan_id;       /**< Pan ID */
  zb_uint16_t coord_short_addr; /**< Coordinator Short Address */
  zb_uint8_t logical_channel;    /**< Logical Channel */
  zb_uint16_t short_addr;   /**< Short Address */
#if (MAC_FRAME_VERSION > MAC_FRAME_IEEE_802_15_4_2003)
  zb_uint8_t  channel_page; /**< Channel page, may be omitted */
#endif
} ZB_PACKED_STRUCT
zb_coord_realignment_cmd_t;


/**
   Parser MAC header.

   mac spec 7.2.1 General MAC frame format
   @note Actually MHR is of variable length and this structure can't
   be used for physical data i/o, it is used only for function calls.
*/
typedef struct zb_mac_mhr_s
{
  zb_addr_u dst_addr;     /**< */
  zb_addr_u src_addr;     /**< */
  zb_uint16_t src_pan_id;       /**< */
  zb_uint16_t dst_pan_id;       /**< */
  zb_uint8_t frame_control[2];  /**< */
  zb_uint8_t seq_number;        /**< */
  zb_uint8_t mhr_len;        /**< */

#ifdef ZB_MAC_SECURITY
  zb_uint8_t      security_level;
  zb_uint8_t      key_id_mode;
  zb_uint8_t      key_source[8];
  zb_uint8_t      key_index;
  zb_uint32_t     frame_counter;
#endif

}
  zb_mac_mhr_t;

/**
   Parameters for sending beacon frame
*/
typedef struct zb_beacon_frame_params_s
{
  zb_uint8_t src_addr_mode;     /**< */
  zb_uint8_t security_enabled;  /**< */
  zb_uint8_t frame_pending;     /**< */
  zb_uint8_t ble;               /**< */
  zb_uint8_t beacon_order;      /**< */
  zb_uint8_t superframe_order;  /**< */
}
  zb_beacon_frame_params_t;

/**
   Parameters for sending data request
*/
typedef enum zb_callback_type_e
{
  MAC_ASS_CONFIRM_CALLBACK = 0,     /**< */
  MAC_POLL_REQUEST_CALLBACK = 1     /**< */
}
  zb_callback_type_t;

typedef struct zb_mlme_data_req_params_s
{
  zb_uint8_t src_addr_mode;         /**< Source address mode */
  zb_uint8_t dst_addr_mode;         /**< Destination address mode */
  zb_addr_u src_addr;         /**< Source address */
  zb_addr_u dst_addr;         /**< Destination address */
  zb_callback_type_t cb_type;       /**< Callback for mlme-data-request function */
}
  zb_mlme_data_req_params_t;

/* Moved zb_mac_pending_data_t to zboss_api_internal.h */

/**
   Parameters for orphan indication
*/
typedef struct zb_mac_orphan_ind_s
{
  zb_ieee_addr_t orphan_addr; /*<! The 64-bits address of the orphaned device */
  zb_uint8_t iface_id; /*<! MAC interface id */
}
  zb_mac_orphan_ind_t;

/**
   Parameters for orphan response
*/
typedef ZB_PACKED_PRE struct zb_mac_orphan_response_s
{
  zb_ieee_addr_t orphan_addr; /*<! The 64-bits address of the orphaned device */
  zb_uint16_t short_addr; /*<! The 16-bit short address allocated to the
                           * orphaned device if it is associated with this coordinator */
  zb_uint8_t associated; /*!< TRUE if the orphaned device is associated with this
                           coordinator or FALSE otherwise */
}
  ZB_PACKED_STRUCT
  zb_mac_orphan_response_t;


/**
   Initialize MAC layer for the work. Must be
   called during initialization of whole system.
*/
void zb_mac_init(void);


/**
   Pre-historic routine - now MAC pre-init.

   TODO: kill it.
 */
void zb_mac_transport_init(void);



/**
   Fill packed mac header

   @param ptr - pointer to out data
   @param mhr - structure with mhr data
*/
void zb_mac_fill_mhr(zb_uint8_t *ptr, zb_mac_mhr_t *mhr);




/**
   Calculates length of mac header (MHR) inside MAC frame

   @param src_addr_mode   - source addressing mode one of @ref address_modes
   @param dst_addr_mode   - destination addressing mode one of @ref address_modes
   @param equal_panid - is need alloc for source Pan ID Field (ZB_FALSE) or not (ZB_TRUE)
*/
zb_uint8_t zb_mac_calculate_mhr_length(zb_uint8_t src_addr_mode, zb_uint8_t dst_addr_mode, zb_bool_t equal_panid);


/**
   Parses packed mhr header, fills mhr structure
   @param mhr - out pointer to mhr structure
   @param ptr - pointer to packed mhr header buffer
   @return packed mhr buffer length
*/
zb_uint8_t zb_parse_mhr(zb_mac_mhr_t *mhr, zb_bufid_t buf);

zb_uint8_t zb_parse_mhr_ptr(zb_mac_mhr_t *mhr, zb_uint8_t *ptr);

void zb_mac_resp_by_empty_frame(zb_uint8_t param);

#if defined ZB_TRAFFIC_DUMP_ON || defined ZB_NSNG || defined ZB_MAC_TESTING_MODE
void zb_mac_fcs_add(zb_bufid_t buf);
#define MAC_ADD_FCS(buf) zb_mac_fcs_add(buf)
#else
#define MAC_ADD_FCS(buf)
#endif /* ZB_TRAFFIC_DUMP_ON || ZB_NSNG || ZB_MAC_TESTING_MODE */

void zb_mlme_orphan_indication(zb_uint8_t param);

/**
   Sends orphan request command
   @param param - parameters for sending data request,
   zb_mac_orphan_response_t
   @return noting
*/
void zb_mlme_orphan_response(zb_uint8_t param);


#ifdef ZB_LIMIT_VISIBILITY
void mac_add_visible_device(zb_ieee_addr_t long_addr);
void mac_add_invisible_short(zb_uint16_t addr);
void mac_remove_invisible_short(zb_uint16_t addr);
#define MAC_ADD_VISIBLE_LONG(long_addr) mac_add_visible_device(long_addr)
#define MAC_ADD_INVISIBLE_SHORT(addr) mac_add_invisible_short(addr)
#define MAC_REMOVE_INVISIBLE_SHORT(addr) mac_remove_invisible_short(addr)
#else
#define MAC_ADD_VISIBLE_LONG(long_addr) (void)(long_addr)
#define MAC_ADD_INVISIBLE_SHORT(addr) (void)(addr)
#define MAC_REMOVE_INVISIBLE_SHORT(addr) (void)addr
#endif

/**
   Main MAC logic iteration.

   To be called from the scheduler iteration before executing other callbacks.
   Called not using callbacks API to achieve higher priority then usual
   callbacks.

   @return RET_OK if success, RET_BLOCKED if MCU can go asleep
*/
zb_ret_t zb_multimac_mac_logic_iteration_proxy(void);
zb_ret_t zb_mac_logic_iteration(void);

#define ZB_MAC_LOGIC_ITERATION() zb_multimac_mac_logic_iteration_proxy()

#if !defined ZB_MACSPLIT_HOST && !defined NCP_MODE_HOST
/**
  Checks that MAC allows to perform blocking transport iteration.
*/
zb_bool_t zb_mac_allows_transport_iteration(void);

#define ZB_MAC_ALLOWS_TRANSPORT_ITERATION() (zb_mac_allows_transport_iteration())

#else

#define ZB_MAC_ALLOWS_TRANSPORT_ITERATION() ZB_TRUE

#endif

#ifdef ZB_MAC_SPECIFIC_GET_LQI_RSSI
/**
   Gets last packet's LQI and RSSI for device with the specified short address.
*/
void zb_mac_diag_data_get(zb_uint16_t short_address, zb_uint8_t *lqi, zb_int8_t *rssi);
#endif


/** @} */
/** @endcond */

#ifdef ZB_MAC_TESTING_MODE
/**
   Handles MLME-purge.request

   @param param - parameter (packet buffer), @see zb_mlme_purge_request_t is on its tail
*/
void zb_mlme_purge_request(zb_uint8_t param);

#endif /* ZB_MAC_TESTING_MODE */

#if defined MAC_CERT_TEST_HACKS || defined ZB_MAC_TESTING_MODE

/**
   Handles MLME-purge.confirm

   @param param - parameter (packet buffer), with status and zb_mlme_purge_confirm_t
*/
void zb_mlme_purge_confirm(zb_uint8_t param);

/**
   MLME-purge.request parameter
*/
typedef struct zb_mlme_purge_request_s
{
  zb_uint8_t msdu_handle;
}
  zb_mlme_purge_request_t;

typedef zb_mlme_purge_request_t zb_mlme_purge_confirm_t;

#endif

#ifdef ZB_MAC_TESTING_MODE
/**
   Handles PLME-CCA.request

   @param param - parameter (packet buffer)
*/
void zb_plme_cca_request(zb_uint8_t param);

/**
   Handles PLME-CCA.confirm

   @param param - parameter (packet buffer), with status
*/
void zb_plme_cca_confirm(zb_uint8_t param);
#endif /* ZB_MAC_TESTING_MODE */

#if defined ZB_MAC_PENDING_BIT_SOURCE_MATCHING

/**
 * @name MAC source match commands
 * @anchor mac_src_match_commands
 *
 * Note: These values were members of `enum zb_mac_src_match_commands_e` type but were converted to a
 * set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_MAC_SRC_MATCH_CMD_ADD    0U
#define ZB_MAC_SRC_MATCH_CMD_DELETE 1U
#define ZB_MAC_SRC_MATCH_CMD_DROP   2U
/** @} */

typedef ZB_PACKED_PRE struct zb_mac_src_match_params_s
{
  zb_uint8_t cmd;               /**< @ref mac_src_match_commands */
  zb_uint8_t index;             /**< Index to synchronize with ZBOSS Neighbor table */
  zb_uint8_t addr_mode;         /**< Address mode. Valid values are ZB_ADDR_16BIT_DEV_OR_BROADCAST or
                                 * ZB_ADDR_64BIT_DEV */
  zb_addr_u addr;               /**< Address  */
#ifdef ZB_MAC_POLL_INDICATION_CALLS_REDUCED
  zb_uint16_t poll_timeout;     /**< Poll indication calls timeout (in sec) */
#endif /* ZB_MAC_POLL_INDICATION_CALLS_REDUCED */
} ZB_PACKED_STRUCT zb_mac_src_match_params_t;

#if defined(ZB_MAC_SOFTWARE_PB_MATCHING) || defined(ZB_MAC_CONTROLLABLE_PB_MATCHING)
#define MAC_PENDING_BITMAP_GET(idx) ZB_B2U(ZB_BIT_IS_SET(MAC_CTX().pending_bitmap[(idx) / 32U], ((zb_uint32_t)1U << ((zb_uint32_t)(idx) % (zb_uint32_t)32U))))
#define MAC_PENDING_BITMAP_SET(idx) MAC_CTX().pending_bitmap[(idx) / 32U] |= ((zb_uint32_t)1U << ((zb_uint32_t)(idx) % (zb_uint32_t)32U))
#define MAC_PENDING_BITMAP_CLR(idx) MAC_CTX().pending_bitmap[(idx) / 32U] &= ~((zb_uint32_t)1U << ((zb_uint32_t)(idx) % (zb_uint32_t)32U))

#ifdef ZB_MAC_POLL_INDICATION_CALLS_REDUCED
#define MAC_POLL_TIMESTAMP_GET(idx) (MAC_CTX().poll_timestamp_table[(idx)])
#define MAC_POLL_TIMESTAMP_UPD(idx) MAC_CTX().poll_timestamp_table[(idx)] = ZB_TIMER_GET()
#define MAC_POLL_TIMEOUT_GET(idx)   (MAC_CTX().poll_timeout_table[(idx)] * ZB_TIME_ONE_SECOND)
#define MAC_POLL_TIMEOUT_SET(idx, tmo) MAC_CTX().poll_timeout_table[(idx)] = tmo
#endif /* ZB_MAC_POLL_INDICATION_CALLS_REDUCED */

#endif /* ZB_MAC_SOFTWARE_PB_MATCHING || ZB_MAC_CONTROLLABLE_PB_MATCHING */

#endif  /* ZB_MAC_PENDING_BIT_SOURCE_MATCHING */


#if defined(ZB_MAC_INTERFACE_SINGLE)
  #if defined(ZB_MACSPLIT_HOST)
    #define ZB_MAC_CALL_INTERFACE(interface_id, primitive, param) ((void)interface_id, ZB_SCHEDULE_CALLBACK(zb_##primitive##_macsplit, param))
    #define ZB_MAC_CALL_INTERFACE_ALARM(interface_id, primitive, param, delay) ((void)interface_id, ZB_SCHEDULE_ALARM(zb_##primitive##_macsplit, param, delay))
  #elif defined(ZB_MACSPLIT_DEVICE)
    #define ZB_MAC_CALL_INTERFACE(interface_id, primitive, param) ((void)interface_id, ZB_SCHEDULE_CALLBACK(zb_##primitive, param))
    #define ZB_MAC_CALL_INTERFACE_ALARM(interface_id, primitive, param, delay) ((void)interface_id, ZB_SCHEDULE_ALARM(zb_##primitive, param, delay))
  #elif defined(ZB_MAC_MONOLITHIC)
    #define ZB_MAC_CALL_INTERFACE(interface_id, primitive, param) ((void)interface_id, ZB_SCHEDULE_CALLBACK(zb_##primitive, param))
    #define ZB_MAC_CALL_INTERFACE_ALARM(interface_id, primitive, param, delay) ((void)interface_id, ZB_SCHEDULE_ALARM(zb_##primitive, param, delay))
  #elif defined(ZB_MAC_BLE)
    #define ZB_MAC_CALL_INTERFACE(interface_id, primitive, param) ((void)interface_id, ZB_SCHEDULE_CALLBACK(zb_##primitive##_ble, param))
    #define ZB_MAC_CALL_INTERFACE_ALARM(interface_id, primitive, param, delay) ((void)interface_id, ZB_SCHEDULE_ALARM(zb_##primitive##ble, param, delay))
  #elif defined(ZB_MAC_SUBGHZ)
    #define ZB_MAC_CALL_INTERFACE(interface_id, primitive, param) ((void)interface_id, ZB_SCHEDULE_CALLBACK(zb_##primitive##_subghz, param))
    #define ZB_MAC_CALL_INTERFACE_ALARM(interface_id, primitive, param, delay) ((void)interface_id, ZB_SCHEDULE_ALARM(zb_##primitive##_subghz, param, delay))
  #elif defined(NCP_MODE_HOST)
    #define ZB_MAC_CALL_INTERFACE(interface_id, primitive, param) ((void)interface_id, (void)primitive, (void)param, ZB_ASSERT(ZB_FALSE))
    #define ZB_MAC_CALL_INTERFACE_ALARM(interface_id, primitive, param, delay) ((void)interface_id, (void)primitive, (void)param, (void)delay, ZB_ASSERT(ZB_FALSE))
  #else
    #error At least one of ZB_MAC_xxx directives must be defined!
  #endif

  #define ZB_MULTIMAC_IS_INTERFACE_ACTIVE(interface_id) (ZB_TRUE)
#else
  #define ZB_MAC_CALL_INTERFACE(interface_id, primitive, param)                                   \
    do {                                                                                          \
      if (ZB_U2B(ZB_NWK_MAC_IFACE_TBL_ENTRY(interface_id)->state))                                \
      {                                                                                           \
        ZB_SCHEDULE_CALLBACK(ZG->nwk.mac_interfaces[interface_id].primitive, param);              \
      }                                                                                           \
      else                                                                                        \
      {                                                                                           \
        ZB_ASSERT(ZB_FALSE);                                                                      \
      }                                                                                           \
    } while (0)

  #define ZB_MAC_CALL_INTERFACE_ALARM(interface_id, primitive, param, delay)                      \
    do {                                                                                          \
      if (ZB_U2B(ZB_NWK_MAC_IFACE_TBL_ENTRY(interface_id)->state))                                \
      {                                                                                           \
        ZB_SCHEDULE_ALARM(ZG->nwk.mac_interfaces[interface_id].primitive, param, delay);          \
      }                                                                                           \
      else                                                                                        \
      {                                                                                           \
        ZB_ASSERT(ZB_FALSE);                                                                      \
      }                                                                                           \
    } while (0)

    #define ZB_MULTIMAC_IS_INTERFACE_ACTIVE(interface_id) ZB_U2B(ZB_NWK_MAC_IFACE_TBL_ENTRY(interface_id)->state)
#endif

/**
 * @brief This parameter is added to buffer on MAC interface layer to
 *  determine MAC interface that is used for receiving the packet on upper stack layers
 */
typedef struct zb_mcps_data_indication_param_s
{
  zb_uint8_t iface_id;
  zb_time_t timestamp;
}
zb_mcps_data_indication_param_t;

/** @endcond */ /* DOXYGEN_MULTIMAC_SECTION */

/**
 * Defines internal constants for mac interface enumeration
 *
 */
typedef enum zb_mac_interface_type_e
{
  MAC_INTERFACE_TYPE_MONOLITHIC      = 0,
  MAC_INTERFACE_TYPE_MACSPLIT_HOST   = 1,
  MAC_INTERFACE_TYPE_MACSPLIT_DEVICE = 2,
  MAC_INTERFACE_TYPE_SUBGHZ          = 3,
  MAC_INTERFACE_TYPE_BLE             = 4,
  MAC_INTERFACE_TYPE_MAX
} zb_mac_interface_type_t;

typedef void (*zb_mac_interface_func_t)(zb_bufid_t param);
typedef struct zb_mac_interface_s {
    zb_mac_interface_func_t mcps_data_request;
    zb_mac_interface_func_t mcps_purge_indirect_queue_request;
    zb_mac_interface_func_t mlme_get_request;
    zb_mac_interface_func_t mlme_set_request;
    zb_mac_interface_func_t mlme_reset_request;
    zb_mac_interface_func_t mlme_scan_request;
    zb_mac_interface_func_t mlme_associate_request;
    zb_mac_interface_func_t mlme_associate_response;
    zb_mac_interface_func_t mlme_poll_request;
    zb_mac_interface_func_t mlme_orphan_response;
    zb_mac_interface_func_t mlme_start_request;
    zb_mac_interface_func_t mac_resp_by_empty_frame;
} zb_mac_interface_t;



typedef ZB_PACKED_PRE struct zb_mlme_multimac_start_req_s
{
  zb_mlme_start_req_t start_params;
#if ZB_NWK_MAC_IFACE_TBL_SIZE >= 2
  zb_uint8_t  logical_channel[ZB_NWK_MAC_IFACE_TBL_SIZE - 1]; /**< Logical channel */
  zb_uint8_t  channel_page[ZB_NWK_MAC_IFACE_TBL_SIZE - 1]; /**< The channel page to use */
#endif
} ZB_PACKED_STRUCT
zb_mlme_multimac_start_req_t;

#if defined ZB_ALIEN_MAC || defined ZB_NSNG

#define ZB_TRANSCEIVER_TIME_SUBTRACT ZB_TIME_SUBTRACT

#else

zb_time_t osif_sub_trans_timer(zb_time_t t2, zb_time_t t1);
#define ZB_TRANSCEIVER_TIME_SUBTRACT osif_sub_trans_timer

#endif

#ifdef ZB_ENABLE_ZGP
void zb_gp_mcps_data_indication(zb_uint8_t param);
#endif /* ZB_ENABLE_ZGP */

#ifdef ZB_MAC_CONFIGURABLE_TX_POWER

typedef zb_ret_t (*zb_tx_power_provider_t)(zb_uint8_t page, zb_uint8_t channel, zb_int8_t *power_dbm);

void zb_mac_set_tx_power_provider_function(zb_tx_power_provider_t new_provider);

#endif /* #ifdef ZB_MAC_CONFIGURABLE_TX_POWER */

/**
 * @name Type of indirect queue purge request
 * @anchor mcps_purge_indir_q_type
 *
 * Note: These values were members of `enum zb_mcps_purge_indir_q_type_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_MCPS_INDIR_Q_PURGE_TYPE_NONE        0U
#define ZB_MCPS_INDIR_Q_PURGE_TYPE_ASSOCIATION 1U
#define ZB_MCPS_INDIR_Q_PURGE_TYPE_REJOIN      2U
/** @} */

#if !defined ZB_LITE_NO_INDIRECT_QUEUE_PURGE

typedef ZB_PACKED_PRE struct zb_mcps_purge_indir_q_req_s
{
  zb_uint8_t type;
  zb_uint16_t short_addr;
  zb_ieee_addr_t ieee_addr;
} ZB_PACKED_STRUCT zb_mcps_purge_indir_q_req_t;

typedef ZB_PACKED_PRE struct zb_mcps_purge_indir_q_conf_s
{
  zb_mac_status_t status;
  zb_uint8_t type;
} ZB_PACKED_STRUCT zb_mcps_purge_indir_q_conf_t;

void zb_mcps_purge_indirect_queue_request(zb_uint8_t param);

void zb_mcps_purge_indirect_queue_confirm(zb_uint8_t param);

#endif

#ifdef ZB_MACSPLIT_HOST
/**
   Reset macsplit device
*/
void zb_mlme_dev_reset(zb_uint8_t param);
#endif /* ZB_MACSPLIT_HOST */

#if defined ZB_TRAFFIC_DUMP_ON && !defined ZB_TRANSPORT_OWN_TRAFFIC_DUMP_ON
void zb_mac_traffic_dump(zb_bufid_t buf, zb_bool_t is_w);
#define ZB_DUMP_INCOMING_DATA(buf) zb_mac_traffic_dump((buf), ZB_FALSE)
#define ZB_DUMP_OUTGOING_DATA(buf) zb_mac_traffic_dump((buf), ZB_TRUE)
#else
#define ZB_DUMP_INCOMING_DATA(buf) ZVUNUSED(buf)
#define ZB_DUMP_OUTGOING_DATA(buf) ZVUNUSED(buf)
#endif

void mac_clear_filters(void);

/**
   Check whether a frame is a broadcast frame
*/
zb_bool_t zb_mac_check_frame_is_broadcast(const zb_mac_mhr_t *mhr);

/**
   Check destination address of a frame
*/
zb_bool_t zb_mac_check_frame_dst_addr(const zb_mac_mhr_t *mhr);

/**
   Check destination PAN ID of a frame
*/
zb_bool_t zb_mac_check_frame_pan_id(const zb_mac_mhr_t *mhr);

#ifdef ZB_PHY_TESTING_MODE

/**
 * Validate received packet buffer in PHY Testing mode
 */
void zb_mac_phy_testing_rx_data_indication_cb(zb_uint8_t param);

/**
 * Handle PHY Testing mode request
 */
void zb_mac_phy_testing_mode_request(zb_bufid_t param);

/**
 * Handle PHY Testing mode statistics request
 */
void zb_mac_phy_testing_mode_statistics_request(zb_bufid_t param);

/**
 * Handle tx_done event in PHY Testing mode
 */
void zb_mac_phy_testing_tx_done(void);

/*
 * Notify upper layer that the transceiver entered the PHY testing mode
 * or that the test has finished.
*/
void zb_mac_phy_testing_mode_notification(zb_bufid_t param);

#endif

/**
    Parses association response from pkt body
     and writes result to `zb_mlme_associate_confirm_t` struct.

   @param mhr           - pointer to parsed mac header
   @param cmd_ptr       - pointer to mac command start
   @param assoc_confirm - pointer to zb_mlme_associate_confirm_t struct
                          to which result will be written
 */
void zb_mac_parse_associate_response(zb_mac_mhr_t *mhr, zb_uint8_t *cmd_ptr, zb_mlme_associate_confirm_t *assoc_confirm);

#endif  /* ZB_MAC_API_INCLUDED */
