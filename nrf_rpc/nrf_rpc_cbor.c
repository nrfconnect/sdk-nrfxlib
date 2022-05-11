/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */
#define NRF_RPC_LOG_MODULE NRF_RPC_CBOR
#include <nrf_rpc_log.h>

#include <stdint.h>
#include <stddef.h>

#include <nrf_rpc.h>
#include <nrf_rpc_cbor.h>
#include <nrf_rpc_common.h>

struct handler_proxy_ctx {
	CborValue value;
	//CborParser parser;
	zcbor_decoder_t parser;
	struct cbor_buf_reader reader;
	const uint8_t *in_packet;
};

inline static size_t nrf_rpc_cbor_data_len(const struct nrf_rpc_cbor_ctx *ctx)
{
	return ctx->zs->payload_mut - ctx->out_packet;
}

int nrf_rpc_cbor_cmd(const struct nrf_rpc_group *group, uint8_t cmd,
		     struct nrf_rpc_cbor_ctx *ctx,
		     nrf_rpc_cbor_handler_t handler, void *handler_data)
{
	size_t len;
	const struct _nrf_rpc_cbor_decoder cbor_handler = {
		.handler = handler,
		.handler_data = handler_data,
		.decoding_done_required = false,
	};

	if (!zcbor_nul_put(ctx->zs)) {
		NRF_RPC_CBOR_DISCARD(*ctx);
		return -NRF_ENOMEM;
	}

	len = nrf_rpc_cbor_data_len(ctx);

	return nrf_rpc_cmd(group, cmd, ctx->out_packet, len,
				&_nrf_rpc_cbor_proxy_handler,
				(void *)&cbor_handler);
}

int nrf_rpc_cbor_cmd_rsp(const struct nrf_rpc_group *group, uint8_t cmd,
			 struct nrf_rpc_cbor_ctx *ctx)
{
	int err;
	size_t len;
	size_t rsp_size;

	if (!zcbor_nul_put(ctx->zs)) {
		NRF_RPC_CBOR_DISCARD(*ctx);
		return -NRF_ENOMEM;
	}

	len = nrf_rpc_cbor_data_len(ctx);

	err = nrf_rpc_cmd_rsp(group, cmd, ctx->packet, len,
				&ctx->packet, &rsp_size);

	if (err < 0) {
	} else {
		zcbor_new_decode_state(ctx->sz, ARRAY_SIZE(ctx->zs). ctx->packet,
					rsp_size, 1);
	}

/*
	if (cbor_parser_init(&ctx->reader.r, 0, &ctx->parser, &ctx->value)
				!= CborNoError && err >= 0) {

		nrf_rpc_decoding_done(ctx->packet);
		err = -NRF_EBADMSG;
	}
	ctx->value.remaining = UINT32_MAX;
*/

	return err;
}

void nrf_rpc_cbor_cmd_no_err(const struct nrf_rpc_group *group, uint8_t cmd,
			     struct nrf_rpc_cbor_ctx *ctx,
			     nrf_rpc_cbor_handler_t handler,
			     void *handler_data)
{
	int err;

	err = nrf_rpc_cbor_cmd(group, cmd, ctx, handler, handler_data);
	if (err < 0) {
		NRF_RPC_ERR("Unhandled command send error %d", err);
		nrf_rpc_err(err, NRF_RPC_ERR_SRC_SEND, group, cmd,
			    NRF_RPC_PACKET_TYPE_CMD);
	}
}

void nrf_rpc_cbor_cmd_rsp_no_err(const struct nrf_rpc_group *group, uint8_t cmd,
				 struct nrf_rpc_cbor_ctx *ctx)
{
	int err;

	err = nrf_rpc_cbor_cmd_rsp(group, cmd, ctx);
	if (err < 0) {
		NRF_RPC_ERR("Unhandled command send error %d", err);
		nrf_rpc_err(err, NRF_RPC_ERR_SRC_SEND, group, cmd,
			    NRF_RPC_PACKET_TYPE_CMD);
	}
}

int nrf_rpc_cbor_evt(const struct nrf_rpc_group *group, uint8_t evt,
		     struct nrf_rpc_cbor_ctx *ctx)
{

	size_t len;

	if (!zcbor_nul_put(ctx->zs)) {
		NRF_RPC_CBOR_DISCARD(*ctx);
		return -NRF_ENOMEM;
	}

	len = nrf_rpc_cbor_data_len(ctx);

	return nrf_rpc_evt(group, evt, ctx->out_packet, len);
}

void nrf_rpc_cbor_evt_no_err(const struct nrf_rpc_group *group, uint8_t evt,
			     struct nrf_rpc_cbor_ctx *ctx)
{
	int err;

	err = nrf_rpc_cbor_evt(group, evt, ctx);
	if (err < 0) {
		NRF_RPC_ERR("Unhandled command send error %d", err);
		nrf_rpc_err(err, NRF_RPC_ERR_SRC_SEND, group, evt,
			    NRF_RPC_PACKET_TYPE_EVT);
	}

}

int nrf_rpc_cbor_rsp(struct nrf_rpc_cbor_ctx *ctx)
{

	size_t len;

	if (!zcbor_nul_put(ctx->zs)) {
		NRF_RPC_CBOR_DISCARD(*ctx);
		return -NRF_ENOMEM;
	}

	len = nrf_rpc_cbor_data_len(ctx);

	return nrf_rpc_rsp(ctx->out_packet, len);
}

void nrf_rpc_cbor_rsp_no_err(struct nrf_rpc_cbor_ctx *ctx)
{
	int err;

	err = nrf_rpc_cbor_rsp(ctx);
	if (err < 0) {
		NRF_RPC_ERR("Unhandled command send error %d", err);
		nrf_rpc_err(err, NRF_RPC_ERR_SRC_SEND, NULL, NRF_RPC_ID_UNKNOWN,
			    NRF_RPC_PACKET_TYPE_RSP);
	}

}

void nrf_rpc_cbor_decoding_done(CborValue *value)
{
	struct handler_proxy_ctx *ctx =
		NRF_RPC_CONTAINER_OF(value, struct handler_proxy_ctx, value);

	nrf_rpc_decoding_done(ctx->in_packet);
}

void _nrf_rpc_cbor_proxy_handler(const uint8_t *packet, size_t len,
				void *handler_data)
{
	struct handler_proxy_ctx ctx;

	ctx.in_packet = packet;

	struct _nrf_rpc_cbor_decoder *cbor_handler =
		(struct _nrf_rpc_cbor_decoder *)handler_data;

	cbor_buf_reader_init(&ctx.reader, packet, len);

	if (cbor_parser_init(&ctx.reader.r, 0, &ctx.parser, &ctx.value) !=
	    CborNoError) {

		if (cbor_handler->decoding_done_required) {
			nrf_rpc_decoding_done(packet);
		}
		nrf_rpc_err(-NRF_EBADMSG, NRF_RPC_ERR_SRC_RECV, NULL,
			    NRF_RPC_ID_UNKNOWN, NRF_RPC_PACKET_TYPE_CMD);
		return;
	}
	ctx.value.remaining = UINT32_MAX;

	return cbor_handler->handler(&ctx.value, cbor_handler->handler_data);
}

void _nrf_rpc_cbor_prepare(struct nrf_rpc_cbor_ctx *ctx, size_t len)
{
	zcbor_new_encode_state(ctx->zs, ARRAY_SIZE(ctx->zs), ctx->packet, len, 0);
	ctx->zs->constant_state->stop_on_error = true;
	ctx->error = 0;
}
