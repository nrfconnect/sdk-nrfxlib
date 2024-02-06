/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef _NRF_RPC_CBOR_H_
#define _NRF_RPC_CBOR_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <zcbor_common.h>
#include <zcbor_decode.h>
#include <zcbor_encode.h>

#include <nrf_rpc.h>

/* Max ZCBOR states: 2 means that no backups are available but we can use constant state
 * to enable stop_on_error.
 */
#define NRF_RPC_ZCBOR_STATES	(2 + CONFIG_NRF_RPC_ZCBOR_BACKUPS)

/**
 * @defgroup nrf_rpc_cbor zcbor serialization layer for nRF RPC.
 * @{
 * @ingroup nrf_rpc
 *
 * @brief A module that simplifies the usage of zcbor for serializing nRF RPC.
 */

#ifdef __cplusplus
extern "C" {
#endif

struct nrf_rpc_cbor_ctx;

/** @brief Callback that handles decoding of commands, events and responses.
 *
 * @param group        nRF RPC group.
 * @param ctx          CBOR decoding context
 * @param handler_data Custom handler data.
 */
typedef void (*nrf_rpc_cbor_handler_t)(const struct nrf_rpc_group *group,
				       struct nrf_rpc_cbor_ctx *ctx, void *handler_data);

/* Structure used internally to define TinCBOR command or event decoder. */
struct _nrf_rpc_cbor_decoder {
	nrf_rpc_cbor_handler_t handler;
	void *handler_data;
	bool decoding_done_required;
};

/** @brief Context for encoding and sending commands, events and responses.
 *
 * Initialize it with @ref NRF_RPC_CBOR_ALLOC macro. Only `encoder` field is
 * significant for the API, other fields are internal.
 */
struct nrf_rpc_cbor_ctx {
	zcbor_state_t zs[NRF_RPC_ZCBOR_STATES];
	union {
		uint8_t *out_packet;
		const uint8_t *in_packet;
	};
};

/** @brief Register a command decoder.
 *
 * @param _group   Group that the decoder will belong to, created with a
 *                 @ref NRF_RPC_GROUP_DEFINE().
 * @param _name    Name of the decoder.
 * @param _cmd     Command id. Can be from 0 to 254.
 * @param _handler Handler function of type @ref nrf_rpc_cbor_handler_t.
 * @param _data    Opaque pointer for the `_handler`.
 */
#define NRF_RPC_CBOR_CMD_DECODER(_group, _name, _cmd, _handler, _data)	       \
	static const							       \
	struct _nrf_rpc_cbor_decoder NRF_RPC_CONCAT(_name, _cbor_data) = {     \
		.handler = _handler,					       \
		.handler_data = _data,					       \
		.decoding_done_required = true,				       \
	};								       \
	NRF_RPC_CMD_DECODER(_group, _name, _cmd, _nrf_rpc_cbor_proxy_handler,  \
			    (void *)&NRF_RPC_CONCAT(_name, _cbor_data))

/** @brief Register an event decoder.
 *
 * @param _group   Group that the decoder will belong to, created with a
 *                 @ref NRF_RPC_GROUP_DEFINE().
 * @param _name    Name of the decoder.
 * @param _evt     Event id. Can be from 0 to 254.
 * @param _handler Handler function of type @ref nrf_rpc_cbor_handler_t.
 * @param _data    Opaque pointer for the `_handler`.
 */
#define NRF_RPC_CBOR_EVT_DECODER(_group, _name, _evt, _handler, _data)	       \
	static const							       \
	struct _nrf_rpc_cbor_decoder NRF_RPC_CONCAT(_name, _cbor_data) = {     \
		.handler = _handler,					       \
		.handler_data = _data,					       \
		.decoding_done_required = true,				       \
	};								       \
	NRF_RPC_EVT_DECODER(_group, _name, _evt, _nrf_rpc_cbor_proxy_handler,  \
			    (void *)&NRF_RPC_CONCAT(_name, _cbor_data))

/** @brief Allocates memory for a packet.
 *
 * Macro may allocate some variables on stack, so it should be used at top level
 * of a function.
 *
 * Memory is automatically deallocated when it is passed to any of the send
 * functions. If not @ref NRF_RPC_CBOR_DISCARD() can be used.
 *
 * @param[in]  _group nRF RPC group.
 * @param[out] _ctx   Variable of type @ref nrf_rpc_cbor_ctx or
 *                    @ref nrf_rpc_cbor_rsp_ctx that will hold newly allocated
 *                    resources to encode and send a packet.
 * @param[in]  _len   Requested length of the packet.
 */
#define NRF_RPC_CBOR_ALLOC(_group, _ctx, _len)                                           \
	do {                                                                             \
		nrf_rpc_alloc_tx_buf(_group, &((_ctx).out_packet), (_len) + 1);		 \
		_nrf_rpc_cbor_prepare((struct nrf_rpc_cbor_ctx *)(&(_ctx)), (_len) + 1); \
	} while (0)

/** @brief Deallocate memory for a packet.
 *
 * This macro should be used if memory was allocated, but it will not be sent
 * with any of the send functions.
 *
 * @param _group nRF RPC group, used for allocation.
 * @param _ctx Packet that was previously allocated.
 */
#define NRF_RPC_CBOR_DISCARD(_group, _ctx)            \
	nrf_rpc_free_tx_buf(_group, (_ctx).out_packet)

/** @brief Check that the memory for a packet has been allocated.
 *
 * @param ctx  Context allocated by @ref NRF_RPC_CBOR_ALLOC.
 *
 * @return     true if memory is valid otherwise false.
 */
bool nrf_rpc_cbor_is_alloc(struct nrf_rpc_cbor_ctx *ctx);

/** @brief Send a command and provide callback to handle response.
 *
 * @param group        Group that command belongs to.
 * @param cmd          Command id.
 * @param ctx          Context allocated by @ref NRF_RPC_CBOR_ALLOC.
 * @param handler      Callback that handles the response. In case of error
 *                     (e.g. malformed response packet was received) it is
 *                     undefined if the handler will be called.
 * @param handler_data Opaque pointer that will be passed to `handler`.
 *
 * @return             0 on success or negative error code if a transport layer
 *                     reported a sending error.
 */
int nrf_rpc_cbor_cmd(const struct nrf_rpc_group *group, uint8_t cmd,
		     struct nrf_rpc_cbor_ctx *ctx,
		     nrf_rpc_cbor_handler_t handler, void *handler_data);

/** @brief Send a command and get response as an output parameter.
 *
 * This variant of command send function outputs response as an output
 * parameter. Caller is responsible to call @ref nrf_rpc_cbor_decoding_done
 * just after response packet was decoded and can be deallocated.
 * `ctx->value` can be used to decode the response.
 *
 * @param group  Group that command belongs to.
 * @param cmd    Command id.
 * @param ctx    Context allocated by @ref NRF_RPC_CBOR_ALLOC.
 *
 * @return       0 on success or negative error code if a transport
 *               layer reported a sending error.
 */
int nrf_rpc_cbor_cmd_rsp(const struct nrf_rpc_group *group, uint8_t cmd,
			 struct nrf_rpc_cbor_ctx *ctx);

/** @brief Send a command, provide callback to handle response and pass any
 * error to an error handler.
 *
 * This variant of command send function returns `void`, so sending error
 * returned from the transport layer is passed to the error handler.
 * Source of error is @ref NRF_RPC_ERR_SRC_SEND.
 *
 * @param group        Group that command belongs to.
 * @param cmd          Command id.
 * @param ctx          Context allocated by @ref NRF_RPC_CBOR_ALLOC.
 * @param handler      Callback that handles the response. In case of error
 *                     (e.g. malformed response packet was received) it is
 *                     undefined if the handler will be called.
 * @param handler_data Opaque pointer that will be passed to `handler`.
 */
void nrf_rpc_cbor_cmd_no_err(const struct nrf_rpc_group *group, uint8_t cmd,
			     struct nrf_rpc_cbor_ctx *ctx,
			     nrf_rpc_cbor_handler_t handler,
			     void *handler_data);

/** @brief Send a command, get response as an output parameter and pass any
 * error to an error handler.
 *
 * See both @ref nrf_rpc_cbor_cmd_rsp and @ref nrf_rpc_cbor_cmd_no_err for more
 * details on this variant of command send function.
 *
 * @param group  Group that command belongs to.
 * @param cmd    Command id.
 * @param ctx    Context allocated by @ref NRF_RPC_CBOR_ALLOC.
 */
void nrf_rpc_cbor_cmd_rsp_no_err(const struct nrf_rpc_group *group,
				 uint8_t cmd, struct nrf_rpc_cbor_ctx *ctx);

/** @brief Send an event.
 *
 * @param group  Group that event belongs to.
 * @param evt    Event id.
 * @param ctx    Context allocated by @ref NRF_RPC_CBOR_ALLOC.
 *
 * @return       0 on success or negative error code if a transport layer
 *               reported a sending error.
 */
int nrf_rpc_cbor_evt(const struct nrf_rpc_group *group, uint8_t evt,
		     struct nrf_rpc_cbor_ctx *ctx);

/** @brief Send an event and pass any error to an error handler.
 *
 * This variant of event send function returns `void`, so sending error
 * returned from the transport layer is passed to the error handler.
 * Source of error is @ref NRF_RPC_ERR_SRC_SEND.
 *
 * @param group  Group that event belongs to.
 * @param evt    Event id.
 * @param ctx    Context allocated by @ref NRF_RPC_CBOR_ALLOC.
 */
void nrf_rpc_cbor_evt_no_err(const struct nrf_rpc_group *group, uint8_t evt,
			     struct nrf_rpc_cbor_ctx *ctx);

/** @brief Send a response.
 *
 * @param group  Group that response belongs to.
 * @param ctx    Context allocated by @ref NRF_RPC_CBOR_ALLOC.
 *
 * @return       0 on success or negative error code if a transport layer
 *               reported a sending error.
 */
int nrf_rpc_cbor_rsp(const struct nrf_rpc_group *group, struct nrf_rpc_cbor_ctx *ctx);

/** @brief Send a response and pass any error to an error handler.
 *
 * This variant of response send function returns `void`, so sending error
 * returned from the transport layer is passed to the error handler.
 * Source of error is @ref NRF_RPC_ERR_SRC_SEND.
 *
 * @param group  Group that response belongs to.
 * @param ctx    Context allocated by @ref NRF_RPC_CBOR_ALLOC.
 */
void nrf_rpc_cbor_rsp_no_err(const struct nrf_rpc_group *group, struct nrf_rpc_cbor_ctx *ctx);

/** @brief Indicate that decoding of the input packet is done.
 *
 * This function must be called as soon as the input packet was parsed and can
 * be deallocated. It must be called in command decoder, event decoder and after
 * @ref nrf_rpc_cbor_cmd_rsp or @ref nrf_rpc_cbor_cmd_rsp_no_err. Packet is
 * automatically deallocated after completion of the response handler
 * function, so this `nrf_rpc_cbor_decoding_done` is not needed in response
 * handler.
 *
 * @param group  Group that response belongs to.
 * @param ctx    Context allocated by @ref NRF_RPC_CBOR_ALLOC.
 */
void nrf_rpc_cbor_decoding_done(const struct nrf_rpc_group *group, struct nrf_rpc_cbor_ctx *ctx);

/* Functions used internally by the macros, not intended to be used directly. */
void _nrf_rpc_cbor_prepare(struct nrf_rpc_cbor_ctx *ctx, size_t len);
void _nrf_rpc_cbor_proxy_handler(const struct nrf_rpc_group *group, const uint8_t *packet,
				 size_t len, void *handler_data);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* _NRF_RPC_CBOR_H_ */
