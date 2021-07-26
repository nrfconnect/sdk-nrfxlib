/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef NRF_RPC_TR_TMPL_H_
#define NRF_RPC_TR_TMPL_H_

/*
 * THIS IS A TEMPLATE FILE.
 * This is a template for nrf_rpc_tr API. It can be user as a starting point to
 * implement a new transport. All transports must have API compatible to
 * this one. Small changes are allowed that does not break compatibility, e.g.
 * convert some function to function-like macros.
 *
 * It also contains documentation of the nrf_rpc_tr API. Transports do not need
 * to duplicate this documentation.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @defgroup nrf_rpc_tr nRF RPC transport layer
 * @{
 * @ingroup nrf_rpc
 *
 * @brief nRF RPC transport layer API.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Defines maximum size of a header that transport layer can add to
 * a packet.
 */
#define NRF_RPC_TR_MAX_HEADER_SIZE 0

/** @brief Defines whether receive buffer is automatically dellocated when
 * @ref nrf_rpc_tr_receive_handler_t exits.
 *
 * If defined as `1` receive handler will wait until parsing thread done parsing
 * and allows deallocating of the buffer. It will not call
 * @ref nrf_rpc_tr_free_rx_buf.
 *
 * If defined as `0` receive handler will exit as soon as possible.
 * @ref nrf_rpc_tr_free_rx_buf will be called in the handler if possible or
 * by the thread that is parsing the buffer after it completes.
 */
#define NRF_RPC_TR_AUTO_FREE_RX_BUF 0

/** @brief Packet receive handler callback.
 *
 * @param packet Received packet.
 * @param len    Length of the `packet`.
 */
typedef void (*nrf_rpc_tr_receive_handler_t)(const uint8_t *packet, size_t len);

/** @brief nRF RPC transport layer initialization.
 *
 * @param callback Receive handler callback that will be called on each received
 *                 packet.
 *
 * @return         0 on success or negative error code.
 */
int nrf_rpc_tr_init(nrf_rpc_tr_receive_handler_t callback);

/** @brief Deallocates received packet.
 *
 * If @ref NRF_RPC_TR_AUTO_FREE_RX_BUF is set to `1` this function will never be
 * called.
 *
 * @param packet Packet to deallocate previously received by
 *               @ref nrf_rpc_tr_receive_handler_t.
 */
void nrf_rpc_tr_free_rx_buf(const uint8_t *packet);

/** @brief Allocate memory for packet sending.
 *
 * This function must always successfully return. If there is not enough memory,
 * but it is possible to wait then this function should wait. If requested
 * length is too big then this function should fail (e.g. with ASSERT).
 *
 * This can be macro that allocates memory on the stack as a local variable.
 *
 * Memory is deallocated by @ref nrf_rpc_tr_send or @ref nrf_rpc_tr_free_tx_buf.
 *
 * @param[out] buf  Buffer containing allocated memory.
 * @param[in]  len  Requested buffer length to allocate.
 */
void nrf_rpc_tr_alloc_tx_buf(uint8_t **buf, size_t len);

/** @brief Deallocate memory.
 *
 * This is only called when allocated packet was not send, because normally
 * @ref nrf_rpc_tr_send deallocates packet when it is no longer needed.
 */
void nrf_rpc_tr_free_tx_buf(uint8_t *buf);

/** @brief Send a packet.
 *
 * @param buf  Buffer allocated by @ref nrf_rpc_tr_alloc_tx_buf.
 * @param len  Length of the packet. Can be smaller than allocated.
 *
 * @return     0 on success or negative error code.
 */
int nrf_rpc_tr_send(uint8_t *buf, size_t len);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* NRF_RPC_TR_TMPL_H_ */
