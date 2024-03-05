/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */
/* PURPOSE: MAC platform definitions for nRF SoC.
*/

#ifndef ZB_MAC_PLATFORM_H__
#define ZB_MAC_PLATFORM_H__

#include "zb_macll.h"


#define ZB_MAC_HARDWARE_PB_MATCHING
#define ZB_MAC_MANUAL_DUPS
#define ZB_MAC_SINGLE_PACKET_IN_FIFO
#define ZB_OPTIONAL_MAC_FEATURES

#define ZB_MAC_PENDING_BIT_SOURCE_MATCHING
#define ZB_AUTO_ACK_TX
#define ZB_MAC_AUTO_ACK_RECV
#define ZB_MANUAL_ACK
#define ZB_MANUAL_ACK_TIMEOUT
#define MAC_ACK_PARSE_BY_INTR

/* gpTxOffset should be calibrated for each HW/SW variant.
 * The reason is that ZB_GPD_TX_OFFSET_US is used from ZGP stub during Green Power frame transmitting,
 * and the actual TX time is not known at this level due to delays of MAC layer or transceiver.
 */
#define ZB_GPD_TX_OFFSET_US                              (ZB_MILLISECONDS_TO_USEC(ZB_GPD_RX_OFFSET_MS))
#define MAC_AUTO_GPDF_RETX

#define MAC_TIMER_QUANT_US                               4

#define ZB_TRANSCEIVER_START_CHANNEL_NUMBER              11
#define ZB_TRANSCEIVER_MAX_CHANNEL_NUMBER                26
#ifndef ZB_TRANSCEIVER_ALL_CHANNELS_MASK
#define ZB_TRANSCEIVER_ALL_CHANNELS_MASK                 0x07FFF800 /* 0000.0111 1111.1111 1111.1000 0000.0000*/
#endif /* ZB_TRANSCEIVER_ALL_CHANNELS_MASK */


/* Type definition required by ZBOSS transceiver API */
typedef struct zb_transceiver_ctx_s
{
  zb_uint_t tx_status; // Required and modified by zb_mac.c
}
zb_transceiver_ctx_t;



/**
   Tail size for mac packet
*/
#define ZB_TAIL_SIZE_FOR_RECEIVED_MAC_FRAME              (ZB_MAC_EXTRA_DATA_SIZE) /* 2 */

/**
   Definitions for extra information size in RX fifo packet. Packet format:
   1 byte packet length + N bytes of RX fifo data + M bytes of extra information
   (LQI, RSSI, at some platforms Frame timer, Superframe counter etc)
*/
#define ZB_MAC_PACKET_LENGTH_SIZE                        ZB_MACLL_PACKET_LENGTH_SIZE
#define ZB_MAC_EXTRA_DATA_SIZE                           ZB_MACLL_EXTRA_DATA_SIZE /* rssi & lqi - see zb_macll_metadata_t */

#define ZB_TRANSCEIVER_INIT_RADIO()
#define ZB_TRANSCEIVER_DEINIT_RADIO()

/* According to BZ#14383 and J#ZB-72 this workaround is not needed, if it is needed to put
 * transceiver in idle it should be done automatically in the MAC logic. */
#define ZB_MAC_TRANSCEIVER_GO_IDLE() /* not needed */
#define ZB_MAC_TRANSCEIVER_RESTORE() /* not needed */
#define ZB_TRANSCEIVER_RADIO_ON_OFF()                    zb_macll_get_radio_on_off()

#define zb_mac_transport_init                            zb_macll_init
#define ZB_RADIO_INT_DISABLE()                           ZB_OSIF_GLOBAL_LOCK()
#define ZB_RADIO_INT_ENABLE()                            ZB_OSIF_GLOBAL_UNLOCK()
#define ZB_IS_TRANSPORT_BUSY()                           ZB_MAC_GET_TRANS_INT_FLAG()

#define ZB_MAC_SET_TRANS_INT()                           zb_macll_set_trans_int()
#define ZB_MAC_CLEAR_TRANS_INT()                         zb_macll_clear_trans_int()
#define ZB_MAC_GET_TRANS_INT_FLAG()                      zb_macll_get_trans_int()

#define ZB_MAC_SET_RX_INT_STATUS_BIT()                   zb_macll_set_rx_flag()
#define ZB_MAC_CLEAR_RX_INT_STATUS_BIT()                 zb_macll_clear_rx_flag()
#define ZB_MAC_GET_RX_INT_STATUS_BIT()                   zb_macll_get_rx_flag()

#define ZB_MAC_SET_TX_INT_STATUS_BIT()                   zb_macll_set_tx_flag()
#define ZB_MAC_CLEAR_TX_INT_STATUS_BIT()                 (zb_macll_clear_tx_flag(), ZB_TRANS_SET_TX_STATUS(0))
#define ZB_MAC_GET_TX_INT_STATUS_BIT()                   zb_macll_get_tx_flag()

#define ZB_MAC_READ_INT_STATUS_REG()     /* already done by int handler */

#define ZB_TRANS_SET_TX_STATUS(status)                   (TRANS_CTX().tx_status = (status))
#define ZB_TRANS_CHECK_CHANNEL_BUSY_ERROR()              (TRANS_CTX().tx_status == ZB_TRANS_CHANNEL_BUSY_ERROR)
#define ZB_TRANS_CHECK_TX_RETRY_COUNT_EXCEEDED_ERROR()   0
#define ZB_TRANS_CHECK_NO_ACK()                          (TRANS_CTX().tx_status == ZB_TRANS_NO_ACK)

#define ZB_TRANSCEIVER_SET_COORD_SHORT_ADDR(addr)
#define ZB_TRANSCEIVER_SET_COORD_EXT_ADDR(addr)

#define ZB_TRANSCEIVER_SET_PAN_COORDINATOR(pan_coord)    zb_macll_set_pan_coord((zb_bool_t)pan_coord)


#ifdef ZB_MAC_HARDWARE_PB_MATCHING

/* MAC source matching logic implemented in radio-driver v1.6.0:
 * 1) Add device address in the driver's pending table if there is no indirect data for it (clear pending bit);
 * 2) Remove device address from he driver's pending table if there is an indirect data for it (set pending bit);
*/

#define ZB_TRANSCEIVER_SRC_MATCH_SHORT_SET_PENDING_BIT(short_addr, value) \
  zb_macll_src_match_set_pending_bit((zb_uint8_t *)&(short_addr), (zb_bool_t)(value), ZB_FALSE)

#define ZB_TRANSCEIVER_SRC_MATCH_ADD_SHORT_ADDR(index, short_addr) \
  zb_macll_src_match_set_pending_bit((zb_uint8_t *)&(short_addr), ZB_FALSE, ZB_FALSE)

#define ZB_TRANSCEIVER_SRC_MATCH_DELETE_SHORT_ADDR(index, short_addr) \
  zb_macll_src_match_set_pending_bit((zb_uint8_t *)&(short_addr), ZB_TRUE, ZB_FALSE)

#define ZB_TRANSCEIVER_SRC_MATCH_IEEE_SET_PENDING_BIT(ieee_addr, value) \
  zb_macll_src_match_set_pending_bit((zb_uint8_t *)(ieee_addr), (zb_bool_t)(value), ZB_TRUE)

#define ZB_TRANSCEIVER_SRC_MATCH_ADD_IEEE_ADDR(index, ieee_addr) \
  zb_macll_src_match_set_pending_bit((zb_uint8_t *)(ieee_addr), ZB_FALSE, ZB_TRUE)

#define ZB_TRANSCEIVER_SRC_MATCH_DELETE_IEEE_ADDR(index, ieee_addr) \
  zb_macll_src_match_set_pending_bit((zb_uint8_t *)(ieee_addr), ZB_TRUE, ZB_TRUE)

#define ZB_TRANSCEIVER_SRC_MATCH_TBL_DROP()              zb_macll_src_match_tbl_drop()


#else  /* !ZB_MAC_HARDWARE_PB_MATCHING */
#error Only one Pending Bit handling method is supported.

#endif  /* !ZB_MAC_HARDWARE_PB_MATCHING */


#define ZB_TRANSCEIVER_SET_SHORT_ADDR(addr)              zb_macll_set_short_addr(addr)
#define ZB_TRANSCEIVER_UPDATE_SHORT_ADDR()               ZB_TRANSCEIVER_SET_SHORT_ADDR(MAC_PIB().mac_short_address)

#define ZB_TRANSCEIVER_SET_PAN_ID(pan_id)                zb_macll_set_pan_id(pan_id)
#define ZB_TRANSCEIVER_UPDATE_PAN_ID()                   ZB_TRANSCEIVER_SET_PAN_ID(MAC_PIB().mac_pan_id)

#define ZB_TRANSCEIVER_SET_LONGMAC(long_mac)             zb_macll_set_long_addr(long_mac)
#define ZB_TRANSCEIVER_UPDATE_LONGMAC()                  ZB_TRANSCEIVER_SET_LONGMAC((zb_uint8_t *)ZB_PIB_EXTENDED_ADDRESS())


#define ZB_TRANS_SEND_FRAME(header_length, buf, wait_type)      \
  {                                                             \
    ZB_DUMP_OUTGOING_DATA(buf);                                 \
    (void)header_length;                                        \
    zb_macll_send_packet((buf), (wait_type));                   \
  }

#define ZB_TRANS_REPEAT_SEND_FRAME(header_length, buf, wait_type)       \
  (ZB_DUMP_OUTGOING_DATA(buf),                                          \
   (void)header_length, zb_macll_resend_packet(wait_type, 0))

#define ZB_TRANS_GET_TX_TIMESTAMP()                      zb_macll_get_tx_timestamp()

#ifdef MAC_RADIO_TX_WATCHDOG_ALARM
#error Not implemented feature
#define ZB_MAC_TX_ABORT()
#endif


#define ZB_TRANSCEIVER_SET_PROMISCUOUS(promiscuous_mode)

#define ZB_TRANSCEIVER_SET_CHANNEL(page, channel_number) zb_macll_set_channel(channel_number)

#define ZB_TRANSCEIVER_START_GET_RSSI(_scan_duration_bi) zb_macll_start_get_rssi(_scan_duration_bi);
#define ZB_TRANSCEIVER_GET_RSSI(rssi_value)              zb_macll_get_rssi(rssi_value)
#define ZB_TRANSCEIVER_GET_ENERGY_LEVEL(ed_level)        ZB_TRANSCEIVER_GET_RSSI(ed_level)

#define ZB_TRANSCEIVER_SET_RX_ON_OFF(_rx_on)             zb_macll_trans_set_rx_on_off(_rx_on)
#define ZB_TRANSCEIVER_GET_RX_ON_OFF()                   zb_macll_get_rx_on_off()

#define ZB_TRANS_RECV_PACKET(buf)                        zb_macll_get_next_packet(buf)
#define ZB_MAC_TRANS_CLEAR_PENDING_BIT()
#define ZB_MAC_TRANS_SET_PENDING_BIT()

/**
   Get LQI value
   @param packet - pointer to buffer
*/
#define ZB_MAC_GET_LQI(packet)                           zb_macll_metadata_get_lqi(packet)

/**
   Set LQI value
   @param packet - pointer to buffer
   @param v - LQI value
*/
#define ZB_MAC_SET_LQI(packet, v)                        zb_macll_metadata_set_lqi(packet, v)

/**
   Get RSSI value
   @param packet - pointer to buffer
*/
#define ZB_MAC_GET_RSSI(packet)                          zb_macll_metadata_get_power(packet)

/**
   Set RSSI value
   @param packet - pointer to buffer
   @param v - RSSI value
*/
#define ZB_MAC_SET_RSSI(packet, v)                       zb_macll_metadata_set_power(packet, v)


/*PHY tests specific API. Implement for PHY tests only.*/
#define ZB_TRANSCEIVER_PERFORM_CCA(rssi)                 zb_macll_cca(rssi)
#define ZB_TRANS_TX_CARRIER_DATA(channel, timeout_bi)    zb_macll_tx_carrier(channel, timeout_bi)

#define ZB_TRANSCEIVER_SET_TX_POWER(new_power)           zb_macll_set_tx_power(new_power)
#define ZB_TRANSCEIVER_GET_TX_POWER(power)               zb_macll_get_tx_power(power)

/**
   Cut transceiver-specific header (length byte)
*/
#define ZB_TRANS_CUT_SPECIFIC_HEADER(zb_buffer)

#define ZB_TRANSCEIVER_SET_AUTO_ACK_OFF()                zb_macll_set_auto_ack_off()

#endif /* ZB_MAC_PLATFORM_H__ */
