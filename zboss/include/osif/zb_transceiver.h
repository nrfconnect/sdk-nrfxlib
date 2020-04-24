/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#ifndef ZB_TRANSCEIVER_H__
#define ZB_TRANSCEIVER_H__

void zb_trans_hw_init(void);
void zb_trans_set_pan_id(zb_uint16_t pan_id);
void zb_trans_set_long_addr(zb_ieee_addr_t long_addr);
void zb_trans_set_short_addr(zb_uint16_t addr);
void zb_trans_start_get_rssi(zb_uint8_t scan_duration_bi);
void zb_trans_get_rssi(zb_uint8_t *rssi_value_p);
void zb_trans_set_channel(zb_uint8_t channel_number);
void zb_trans_set_tx_power(zb_int8_t power);
void zb_trans_get_tx_power(zb_int8_t *power);
void zb_trans_set_pan_coord(zb_bool_t pan_coord);
void zb_trans_set_auto_ack(zb_bool_t enabled);
void zb_trans_set_promiscuous_mode(zb_bool_t enabled);
void zb_trans_enter_receive(void);
void zb_trans_enter_sleep(void);
zb_bool_t zb_trans_is_receiving(void);
zb_bool_t zb_trans_is_active(void);
zb_bool_t zb_trans_transmit(zb_uint8_t wait_type, zb_time_t tx_at, zb_uint8_t *tx_buf, zb_uint8_t current_channel);
void zb_trans_buffer_free(zb_uint8_t *p_buf);
zb_bool_t zb_trans_set_pending_bit(zb_uint8_t *addr, zb_bool_t value, zb_bool_t extended);
void zb_trans_src_match_tbl_drop(void);
zb_time_t osif_sub_trans_timer(zb_time_t t2, zb_time_t t1);
zb_uint8_t zb_trans_get_next_packet(zb_bufid_t buf);

#endif /* ZB_TRANSCEIVER_H__ */
