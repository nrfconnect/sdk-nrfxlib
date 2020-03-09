/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * This is a template for rp_tran API. It can be user as a starting point to
 * declare a new transport. All transports must have API compatible to
 * this one. Small changes are allowed that do not break compatibility, e.g.
 * convert some function to function-like macros.
 *
 * It also contains documentation of the rp_trans API. Transports do not need
 * to duplicate this documentation.
 */

#ifndef RP_TRANS_TMPL_H_
#define RP_TRANS_TMPL_H_

#include <stdint.h>
#include <stddef.h>


/**
 * @file
 * @defgroup rp_transport Remote procedures transport
 * @{
 * @brief Remote procedures serialization transport API.
 */


#ifdef __cplusplus
extern "C" {
#endif


/** @brief Structure to hold and identify transport endpoint.
 *
 * Content is implementation specific.
 */
struct rp_trans_endpoint {
	void *_implementation_specific;
};

/** @brief Prepares data for transport endpoint
 *
 * @param _name       Name of the transport endpoint
 * @param _stack_size Stack size of the thread controling the endpoint (ignored
 *                    if implementation does not create a thread stack)
 * @param _prio       Priority of the thread controling the endpoint (ignored
 *                    if implementation does not set a thread priority)
 */
#define RP_TRANS_ENDPOINT_PREPARE(_name, _stack_size, _prio)

/** @brief Initializer for rp_trans_endpoint structure.
 *
 * Data for this initialized had to be prepared before with @a RP_TRANS_ENDPOINT
 * macro.
 * @param _name       Name of the transport endpoint
 */
#define RP_TRANS_ENDPOINT_INITIALIZER(_name)

/** @brief Callback called from endpoint's rx thread when a new packet arrived.
 *
 * @param endpoint endpoint at which a new packet arrived
 * @param buf      data buffer with the packet
 * @param length   size of the packet
 */
typedef void (*rp_trans_receive_handler)(struct rp_trans_endpoint *endpoint,
	const uint8_t *buf, size_t length);


/** @brief Initializes RP transport layer
 *
 * @param callback A callback called from endpoint's rx thread with newly
 *                 received packet.
 */
int rp_trans_init(rp_trans_receive_handler callback);


/** @brief Uninitializes RP transport layer
 */
void rp_trans_uninit(void);


/** @brief Initialize an endpoint
 *
 * @param endpoint        endpoint structure to initialize
 * @param endpoint_number endpoint number the same endpoint on both sides
 */
int rp_trans_endpoint_init(struct rp_trans_endpoint *endpoint,
	int endpoint_number);


/** @brief Uninitialize the endpoint
 *
 * @param endpoint endpoint structure to uninitialize
 */
void rp_trans_endpoint_uninit(struct rp_trans_endpoint *endpoint);


/** @brief Allocates a buffer to transmit packet.
 *
 * Allocated memory must be released by exactly one of the two functions:
 * rp_trans_send or rp_trans_free_tx_buf.
 *
 * Transport implementation may decide to allocate data on the stack, so
 * buffer may be lost after return from the caller.
 *
 * @param         endpoint Endpoint where packet will be send
 * @param[out]    buf      Pointer to start of the buffer
 * @param[in,out] length   Requested buffer length as input, size pf actual allocated
 *                         length as output which can be greater than
 *                         requested.
 */
void rp_trans_alloc_tx_buf(struct rp_trans_endpoint *endpoint, uint8_t **buf,
	size_t *length);


/**  @brief Free allocated transmit buffer in case it was not sent.
 *
 * @param buf      Pointer to start of the buffer
 */
void rp_trans_free_tx_buf(struct rp_trans_endpoint *endpoint, uint8_t *buf);


/**  @brief Sends a packet via specified endpoint.
 *
 * @param endpoint Endpoint where packet will be send
 * @param buf      Pointer to start of the buffer
 * @param buf_len  Length of @a buf, may be less than allocated
 */
int rp_trans_send(struct rp_trans_endpoint *endpoint, const uint8_t *buf,
	size_t buf_len);


#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* RP_TRANS_TMPL_H_ */
