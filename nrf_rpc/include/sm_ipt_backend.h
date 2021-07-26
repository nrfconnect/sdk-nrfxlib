/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef SM_IPT_BACKEND_H
#define SM_IPT_BACKEND_H

#include <sm_ipt.h>

#ifdef __cplusplus
extern "C" {
#endif

extern struct sm_ipt_ctx sm_ipt_context;

static inline int nrf_rpc_tr_init(sm_ipt_receive_handler_t callback)
{
	return sm_ipt_init(&sm_ipt_context, callback);
}

static inline void nrf_rpc_tr_free_rx_buf(const uint8_t *packet)
{
	sm_ipt_free_rx_buf(&sm_ipt_context, packet);
}

static inline void nrf_rpc_tr_alloc_tx_buf(uint8_t **buf, size_t len)
{
	sm_ipt_alloc_tx_buf(&sm_ipt_context, buf, len);
}

static inline void nrf_rpc_tr_free_tx_buf(uint8_t *buf)
{
	sm_ipt_free_tx_buf(&sm_ipt_context, buf);
}

static inline int nrf_rpc_tr_send(uint8_t *buf, size_t len)
{
	return sm_ipt_send(&sm_ipt_context, buf, len);
}

#ifdef __cplusplus
}
#endif

#endif
