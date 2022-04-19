/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef NRF_RPC_TR_H_
#define NRF_RPC_TR_H_

#include <nrf_rpc.h>

/* Helper nRF transport structure declaration. */
struct nrf_rpc_tr;

/** @brief Packet receive handler callback.
 *
 * @param transport nRF RPC transport instance.
 * @param packet Received packet.
 * @param len    Length of the @p packet.
 * @param context User data.
 */
typedef void (*nrf_rpc_tr_receive_handler_t)(const struct nrf_rpc_tr *transport,
					     const uint8_t *packet, size_t len,
					     void *context);

/** @brief Unified nRF RPC transport API structure.
 */
struct nrf_rpc_tr_api {
	/** @brief Function for initialization the nRF RPC transport interface.
	 *
	 * @param[in] transport nRF RPC transport instance.
	 * @param[in] receive_cb Data received callback. It is called every time when transport
	 *                       receives data.
	 * @param[in] context User context.
	 *
	 * @retval 0 If the operation was successful.
	 *         Otherwise, a (negative) error code is returned.
	 */
	int (*init)(const struct nrf_rpc_tr *transport, nrf_rpc_tr_receive_handler_t receive_cb,
		    void *context);

	/** @brief Function for sending data over transport interface.
	 *
	 * @param[in] transport nRF RPC transport instance.
	 * @param[in] data nRF RPC data to send.
	 * @param[in] length nRF RPC data length.
	 *
	 * @retval 0 If the operation was successful.
	 *         Otherwise, a (negative) error code is returned.
	 */
	int (*send)(const struct nrf_rpc_tr *transport, const uint8_t *data, size_t length);

	/** @brief Function for allocation Tx buffer for packet sending.
	 *
	 * This function must successfully return. If there is not enough memory,
	 * but it is possible to wait, then this function should wait.
	 * If the requested length is too long then this function should fail
	 * (for example with ASSERT).
	 *
	 * Memory is deallocated by @p send function or @p tx_buf_free function.
	 *
	 * @param[in] transport nRF RPC transport instance.
	 * @param[in, out] size Requested buffer size as input. Allocated buffer size as output.
	 *
	 * @retval Allocated buffer or NULL if buffer allocation is not possible.
	 */
	void *(*tx_buf_alloc)(const struct nrf_rpc_tr *transport, size_t *size);

	/** @brief Function for free allocated Tx buffer.
	 *
	 * This is only called when allocated packet was not sent, because normally
	 * @p send function deallocates packet when it is no longer needed.
	 *
	 * @param[in] transport nRF RPC transport instance.
	 * @param[in] buf Tx buffer to free.
	 */
	void (*tx_buf_free)(const struct nrf_rpc_tr *transport, void *buf);

	/** @brief Function for free allocated Rx buffer.
	 *
	 * @note This function is optional. It is used only when transport requires to explicitly
	 *       free Rx buffer.
	 *
	 * @param[in] transport nRF RPC transport instance.
	 * @param[in] buf Rx buffer to free.
	 */
	void (*rx_buf_free)(const struct nrf_rpc_tr *transport, void *buf);
};

/** @brief nRF RPC transport structure.
 */
struct nrf_rpc_tr {
	/** nRF RPC transport API. */
	const struct nrf_rpc_tr_api *api;

	/** nRF RPC transport context. Used for keeping transport specific data. */
	void *ctx;
};

#endif  /* NRF_RPC_TR_H_ */
