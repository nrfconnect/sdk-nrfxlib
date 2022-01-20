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
/* PURPOSE: Test profile API
*/

#ifndef ZB_TEST_PROFILE_H
#define ZB_TEST_PROFILE_H 1

#ifdef ZB_TEST_PROFILE

/*! @cond internals_doc */
/*! \addtogroup zb_test_profile */
/*! @{ */

#ifdef ZB_TEST_PROFILE

#define ZB_TEST_CUSTOM_LINK_STATUS
#define USE_RESP_FOR_COUNTED_BUFS
#define USE_COUNTER_RES_REQ

#define ZB_TEST_PROFILE_ID                            0x7f01U /*!< test profile id */
#define ZB_TEST_PROFILE_EP                            0xF0U   /*!< test profile endpoint number (hardcoded) */

#define TP_TRANSMIT_COUNTED_PACKETS_REQ_CLID          0x0001U
/* Only for testing purpose (incorrect clid for test profile) */
#define TP_TRANSMIT_COUNTED_PACKETS_RES_CLID          0xABCDU

#define TP_BUFFER_TEST_REQUEST_CLID                   0x001CU
#define TP_BUFFER_TEST_RESPONSE_CLID                  0x0054U

#define TP_BUFFER_TEST_REQUEST_CLID2                  0x001DU

#define TP_RESET_PACKET_COUNT_CLID                    0x0002U /* Reset packet count */
#define TP_RETRIEVE_PACKET_COUNT_CLID                 0x0003U /* Retrieve packet count */
#define TP_PACKET_COUNT_RESPONSE_CLID                 0x0004U /* Packet count response */

#define MAX_NWK_RADIUS                                0x1EU

#if 0
/**
*  Cluster ID definitions for Test Profile 2
 */
#define TP_TRANSMIT_COUNTED_PACKETS_CLID               0x0001 /*Transmit counted packets */
#define TP_RESET_PACKET_COUNT_CLID                     0x0002 /* Reset packet count */
#define TP_RETRIEVE_PACKET_COUNT_CLID                  0x0003 /* Retrieve packet count */
#define TP_PACKET_COUNT_RESPONSE_CLID                  0x0004 /* Packet count response */
#define TP_TEST_REQUEST_CLID                           0x001c /* Buffer test request */
#define TP_TEST_GROUP_REQUEST_CLID                     0x001d /* Buffer test group request */
#define TP_TEST_RESPONSE_CLID                          0x0054 /* Buffer test response */
#define TP_TEST_GROUP_RESPONSE_CLID                    0x0055 /* Buffer test group response */
#define TP_DISCOVERY_REQUEST_CLID                      0x1000 /* Route discovery request */
#define TP_ROUTE_DISCOVERY_RESPONSE_CLID               0x1001 /* Route discovery response */
#define TP_FREEFORM_REQUEST_CLID                       0xa0a8 /* Freeform request */
#define TP_FREEFORM_RESOPONSE_CLID                     0xe000 /* Freeform response */
#define TP_FREEFORM_NO_DATA_RESPONSE_CLID              0xe001 /* Freeform no data response */
#define TP_BROADCAST_REQUEST_CLID                      0xf000 /* Broadcast request */
#define TP_BROADCAST_ALL_DEVICE_RESPONSE_CLID          0xf001 /* Broadcast to all devices response */
#define TP_BROADCAST_TO_ALL_IDLE_DEVICES_RESPONSE_CLID 0xf00a /* Broadcast to all RxOnWhenIdle devices response */
#define TP_BROACAST_TO_ALL_RC_RESPONSE_CLID            0xf00e /* Broadcast to all routers and coordinator response */
#endif

/* Check cluster id for membership in Test Profile 2 */

#define IS_CLUSTERID_TEST_PROFILE_BASE( _p )  ( ( _p == TP_BUFFER_TEST_REQUEST_CLID)            \
    	                                          || (_p == TP_BUFFER_TEST_REQUEST_CLID2)         \
    	                                          || (_p == TP_BUFFER_TEST_RESPONSE_CLID) )
#ifdef USE_RESP_FOR_COUNTED_BUFS
#define IS_CLUSTER_TEST_PROFILE_COUNTED( _p )  ( (_p == TP_TRANSMIT_COUNTED_PACKETS_REQ_CLID) || (_p == TP_TRANSMIT_COUNTED_PACKETS_RES_CLID) )
#else
#define IS_CLUSTER_TEST_PROFILE_COUNTED( _p )  ( ZB_FALSE )
#endif

#ifdef USE_COUNTER_RES_REQ
#define IS_CLUSTER_TEST_PROFILE_COUNT_RETRIVE( _p ) ( (_p == TP_RESET_PACKET_COUNT_CLID)          \
                                                      || (_p == TP_RETRIEVE_PACKET_COUNT_CLID)    \
                                                      || (_p == TP_PACKET_COUNT_RESPONSE_CLID) )
#else
#define IS_CLUSTER_TEST_PROFILE_COUNT_RETRIVE( _p ) ( ZB_FALSE )
#endif

#define IS_CLUSTERID_TEST_PROFILE2( _p ) (  IS_CLUSTERID_TEST_PROFILE_BASE(_p) || USE_COUNTER_RES_REQ(_p) || IS_CLUSTER_TEST_PROFILE_COUNT_RETRIVE(_p) )

/**
   Transmit counted packets request
 */
typedef ZB_PACKED_PRE struct zb_tp_transmit_counted_packets_req_s
{
  zb_uint8_t len;           /*!< Length (in bytes) of the packet */
  zb_uint16_t counter;      /*!< Octet sequence counter  */
  /* #AT */
  zb_uint8_t req_data[1];   /*!< Data for transmit */
}
ZB_PACKED_STRUCT
zb_tp_transmit_counted_packets_req_t;

/**
   Transmit counted packets parameters
 */
typedef struct zb_tp_transmit_counted_packets_param_s
{
  zb_uint8_t  len;            /*!< Length (in bytes) of the packet */
  zb_uint16_t packets_number; /*!< packets number to send */
  zb_uint16_t idle_time;      /*!< time before next packet is sent */
  zb_uint16_t dst_addr;       /*!< destination address */
  zb_uint8_t  addr_mode;      /*!< address type (group or device address) */
  zb_uint8_t  src_ep;         /*!< source endpoint id */
  zb_uint8_t  dst_ep;         /*!< destination endpoint id */
  zb_uint8_t  radius;         /*!< radius (set to 0 to use default) */
}
zb_tp_transmit_counted_packets_param_t;

/**
   Buffer test request
 */
typedef ZB_PACKED_PRE struct zb_buffer_test_req_s
{
  zb_uint8_t len;               /*!< Length (in bytes) of the packet */
  /* #AT */
  zb_uint8_t req_data[1];       /*!< Data for request */
}
ZB_PACKED_STRUCT
zb_buffer_test_req_t;

/**
   Buffer test request parameters
 */
typedef struct zb_buffer_test_req_param_s
{
  zb_uint8_t len;           /*!< Length (in bytes) of the packet */
  zb_uint16_t dst_addr;     /*!< destination address */
  zb_uint8_t addr_mode;     /*!< address type (group or device address) */
  zb_uint8_t src_ep;        /*!< Source endpoint */
  zb_uint8_t dst_ep;        /*!< Destination endpoint */
  zb_uint8_t radius;        /*!< Radius */
  zb_uint8_t tx_options;    /*!< TX Options */
  zb_uint16_t profile_id;    /*!< profile id */
}
zb_buffer_test_req_param_t;

/**
   Set default data
   length         - 10;
   address        - coordinator;
   address mode   - device address;
   src and dst ep - 240;
 */
#define BUFFER_TEST_REQ_SET_DEFAULT( _p ) _p->len = 0x0AU;                                     \
		                                      _p->dst_addr = 0x0000U;                             \
		                                      _p->addr_mode = ZB_APS_ADDR_MODE_16_ENDP_PRESENT;   \
		                                      _p->src_ep = ZB_TEST_PROFILE_EP;                    \
		                                      _p->dst_ep = ZB_TEST_PROFILE_EP;                    \
		                                      _p->radius = MAX_NWK_RADIUS;                        \
		                                      _p->tx_options = 0;                        \
                                                      _p->profile_id = ZB_TEST_PROFILE_ID

/**
   Set default data
   length         - 10;
   address        - coordinator;
   address mode   - device address;
   src and dst ep - 240;
 */
#define BUFFER_COUNTED_TEST_REQ_SET_DEFAULT( _p ) _p->len            = 0x0AU;                              \
                                                  _p->packets_number = 5U;                                 \
                                                  _p->idle_time      = 3U;                                 \
                                                  _p->dst_addr       = 0x0000;                             \
                                                  _p->addr_mode      = ZB_APS_ADDR_MODE_16_ENDP_PRESENT;   \
                                                  _p->src_ep         = ZB_TEST_PROFILE_EP;                 \
                                                  _p->dst_ep         = ZB_TEST_PROFILE_EP;                 \
                                                  _p->radius         = MAX_NWK_RADIUS;
/**
   Buffer test request parameters, send by endpoint and cluster id
 */
typedef struct zb_buffer_test_req_param_EP_s
{
  zb_uint8_t len;        /*!< Length (in bytes) of the packet */
  zb_uint8_t src_ep;     /*!< Source endpoint */
  zb_uint8_t dst_ep;     /*!< Destination endpoint */
}
zb_buffer_test_req_param_EP_t;


#define ZB_TP_BUFFER_TEST_OK   0x00U /* Successful buffer test */
#define ZB_TP_BUFFER_TEST_FAIL 0x01U /* Transmission failure on first attempt */

/**
   Transmit buffer test response
*/
typedef ZB_PACKED_PRE struct zb_buffer_test_response_s
{
  zb_uint8_t len;     /*!< Length (in bytes) of the packet */
  zb_uint8_t status;  /*!< Status of the response */
}
ZB_PACKED_STRUCT
zb_buffer_test_response_t;

/**
   Transmit buffer test response parameters
*/
typedef ZB_PACKED_PRE struct zb_buffer_test_response_param_s
{
  zb_uint8_t len;       /*!< Length (in bytes) of the packet */
  zb_uint8_t status;    /*!< Status of the response */
  zb_uint16_t dst_addr; /*!< Status of the response */
  zb_uint8_t addr_mode; /*!< address type (group or device address) */
  zb_uint8_t src_ep;    /*!< Source endpoint */
  zb_uint8_t dst_ep;    /*!< Destination endpoint */
  zb_uint8_t tx_options;/*!< TX Options */
}
ZB_PACKED_STRUCT
zb_buffer_test_response_param_t;

/**
   Test Profile indication primitive

   @param param - buffer with command data

   @snippet tp_aps_bv_21_i_zed2.c zb_test_profile_indication

 */
void zb_test_profile_indication(zb_uint8_t param);

/**
    Send counted packet
    Every next packet send by confirm send command

    @param param - buffer with command data
 */
void tp_send_counted_packet(zb_uint8_t param);


/**
   Send counted packet with ACK request field set as TRUE in APS frame
    Every next packet send by confirm send command

    @param param - buffer with command data
 */
void tp_send_counted_packet_ext(zb_uint8_t param);

/**
   Send packet by short address destination

   Difference from tp_send_req_by_short:
    - This function only copies its params into apsde_data_request.
    - APS_SECURITY set in tx_options - not by default
    - Network Radius always copied from param radius

   @param command_id - command ID
   @param param - buffer with command data
   @param profile_id - profile id (typically ZB_TEST_PROFILE_ID)
   @param addr - destination address
   @param addr_mode - destination address mode
   @param src_ep - source endpoint
   @param dst_ep - destination endpoint
   @param tx_options - any options (security, etc)
   @param radius - network radius

   @snippet doxygen_snippets.dox? tp_send_req_by_short_zdo_test_profile_c
 */
void tp_send_req_by_short_ext(zb_uint16_t command_id,
                            zb_uint8_t param,
                            zb_uint16_t profile_id,
                            zb_uint16_t addr,
                            zb_uint8_t addr_mode,
                            zb_uint8_t src_ep,
                            zb_uint8_t dst_ep,
                            zb_uint8_t tx_options,
                            zb_uint8_t radius);

/**
    Send packet and set callback on confirm

    @param param - buffer with command data
    @param cb - callback *.confirm

    @snippet tp_r21_bv-25_gzr_c.c zb_tp_transmit_counted_packets_req_ext
    @snippet tp_r21_bv-25_gzr_r.c zb_tp_transmit_counted_packets_req_ext

 */
void zb_tp_transmit_counted_packets_req_ext(zb_uint8_t param, zb_callback_t cb);


/**
   Send packet by short address destination
   @param command_id - command ID
   @param param - buffer with command data
   @param profile_id - profile id (typically ZB_TEST_PROFILE_ID)
   @param addr - destination address
   @param addr_mode - destination address mode
   @param src_ep - source endpoint
   @param dst_ep - destination endpoint
   @param tx_options - any options (security, etc)
   @param radius - network radius

   @snippet doxygen_snippets.dox tp_send_req_by_short_zdo_test_profile_c
 */
void tp_send_req_by_short(zb_uint16_t command_id,
                          zb_uint8_t param,
                          zb_uint16_t profile_id,
                          zb_uint16_t addr,
                          zb_uint8_t addr_mode,
                          zb_uint8_t src_ep,
                          zb_uint8_t dst_ep,
                          zb_uint8_t tx_options,
                          zb_uint8_t radius);

/**
    Send packet and set callback on confirm

    @param param - buffer with command data
    @param cb - callback *.confirm

    @snippet tp_aps_bv_09_zc.c zb_tp_transmit_counted_packets_req

 */
void zb_tp_transmit_counted_packets_req(zb_uint8_t param, zb_callback_t cb);

/**
    Send ACK

    @param param - buffer for send ack data

    Internal
 */
void tp_packet_ack(zb_uint8_t param);

/**
    Send packet

    @param param - buffer with data see zb_tp_buffer_test_request_s
    @param cb - callback *.confirm

    @snippet TP_PRO_BV-46/tp_pro_bv-46_zc.c tp_pro_bv-46_zc

 */
void zb_tp_buffer_test_request(zb_uint8_t param, zb_callback_t cb);

/**
    Send packet

    @param param - buffer with data see zb_tp_buffer_test_request_s
    @param cb - callback *.confirm

    @snippet tp_zdo_bv_12_zed1.c zb_tp_buffer_test_request_EP

 */
void zb_tp_buffer_test_request_EP(zb_uint8_t param, zb_callback_t cb);

/**
    Send packet broadcast

    @param param - buffer with data see zb_tp_buffer_test_request_s
    @param cb - callback *.confirm

    Not implemented
 */
void zb_tp_buffer_test_request_EP_brdcast(zb_uint8_t param, zb_callback_t cb);

/**
    Send Device Announce APS command

    @param param - buffer for send request

    @code
    void func1(zb_uint8_t param)
    {
      zb_tp_device_announce(param);
    }
    @endcode
 */
void zb_tp_device_announce(zb_uint8_t param);

#ifdef USE_COUNTER_RES_REQ
void zb_tp_retrive_packet_count(zb_uint8_t param);
#endif

#if defined ZB_PRO_STACK && defined ZB_TEST_CUSTOM_LINK_STATUS && defined ZB_ROUTER_ROLE
/* Custom link status command for testing purpose only
 * (allow to send cmd from some predefined adddress)
 */
zb_bool_t zb_tp_send_link_status_command(zb_uint8_t param, zb_uint16_t short_addr);
#endif

#endif /* #ifdef ZB_TEST_PROFILE */
/*! @} */
/*! @endcond */
#endif /* ZB_TEST_PROFILE */

#endif /* ZB_TEST_PROFILE_H */
