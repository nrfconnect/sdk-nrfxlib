/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2022 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: MAC and trace transport API

*/

#ifndef MAC_TRANSPORT_H
#define MAC_TRANSPORT_H 1

/*! @cond internals_doc */
/*! \addtogroup ZB_MAC_TRANSPORT */
/*! @{ */


/*
   That file is mostly historical.
   Initially it described some serial protocol.
   Now it describes traffic dump format.
*/

/**
   Packet types.

   This enum describes possible packet types to be transferred through pipes.
 */
typedef enum zb_mac_transport_type_e
{
  ZB_MAC_TRANSPORT_TYPE_CONSOLE  = 0x00, /*!< Data packet type */
  ZB_MAC_TRANSPORT_TYPE_DUMP  = 0x01, /*!< Dump packet type */
  ZB_MAC_TRANSPORT_TYPE_TRACE = 0x02, /*!< Trace packet type */
  ZB_MAC_TRANSPORT_TYPE_CRASH_DATA = 0x03, /*!< Crash dump */
  ZB_MAC_TRANSPORT_TYPE_MAC_SPLIT_DATA = 0x04, /*!< MAC split data */
  ZB_MAC_TRANSPORT_TYPE_OTA_PROTOCOL = 0x05, /*!< OTA protocol used over MACSPLIT */
  ZBNCP_TRANSPORT_TYPE_DUMP = 0x06, /* NCP dump packet type */
  ZB_MAC_TRANSPORT_TYPE_NUM
}
zb_mac_transport_type_t;

/* Available interface types */
typedef enum zb_dump_iface_type_e
{
  ZB_DUMP_IFACE_ZIGBEE      = 0,
  ZB_DUMP_IFACE_SUBGHZ      = 1,
  ZB_DUMP_IFACE_BLE_DIRECT  = 2,
  ZB_DUMP_IFACE_NCP         = 3,
  ZB_DUMP_IFACE_MAX
} zb_dump_iface_type_t;

/**
   Legacy packet header. Now used in ZBOSS internal traffic dump.

 */
typedef ZB_PACKED_PRE struct zb_mac_transport_hdr_s
{
  zb_uint8_t len;  /*!< Packet length, including hdr */
  zb_uint8_t type; /*!< Packet type, @see zb_mac_transport_type_t */
  zb_uint16_t  time;             /*!< time label */
} ZB_PACKED_STRUCT
zb_mac_transport_hdr_t;


typedef ZB_PACKED_PRE struct zb_dump_hdr_v3_s
{
  zb_uint8_t version;
  zb_uint8_t len;
  zb_uint8_t page;
  zb_uint8_t channel;
  zb_uint32_t trace_cnt;
} ZB_PACKED_STRUCT
zb_dump_hdr_v3_t;

typedef ZB_PACKED_PRE struct zb_dump_hdr_v4_s
{
  zb_uint8_t version;
  zb_uint8_t len;
  zb_uint8_t page;
  zb_uint8_t channel;
  zb_uint32_t trace_cnt;
  zb_uint8_t interface_type : 4;
  zb_uint8_t interface_id   : 4;
  zb_int8_t rssi;
  zb_uint8_t lqi;
} ZB_PACKED_STRUCT
zb_dump_hdr_v4_t;

typedef ZB_PACKED_PRE struct zb_dump_hdr_s
{
  zb_uint32_t trace_cnt;
  zb_uint8_t channel;
  zb_uint8_t len;
} ZB_PACKED_STRUCT
zb_dump_hdr_t;

#if defined ZB_TRAFFIC_DUMP_ON
void zb_dump_put_2buf(zb_uint8_t *buf1, zb_uint_t len1, const zb_uint8_t *buf2, zb_uint_t len2, zb_mac_transport_type_t dump_type, zb_bool_t is_w);
#endif /* ZB_TRAFFIC_DUMP_ON */


/*! @} */
/*! @endcond */

#endif /* MAC_TRANSPORT_H */
