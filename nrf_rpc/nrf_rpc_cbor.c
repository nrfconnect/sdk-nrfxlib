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
	CborParser parser;
	struct cbor_buf_reader reader;
	const uint8_t *in_packet;
};

/* All Context structures are independently defined for API user convenient, but
 * they need the same layout to allow casting between them. Following asserts
 * ensures that.
 */
NRF_RPC_STATIC_ASSERT(offsetof(struct nrf_rpc_cbor_ctx, encoder) ==
		      offsetof(struct nrf_rpc_cbor_rsp_ctx, encoder),
		      "Context structures fields does not match");
NRF_RPC_STATIC_ASSERT(offsetof(struct nrf_rpc_cbor_ctx, out_packet) ==
		      offsetof(struct nrf_rpc_cbor_rsp_ctx, out_packet),
		      "Context structures fields does not match");
NRF_RPC_STATIC_ASSERT(offsetof(struct nrf_rpc_cbor_ctx, writer) ==
		      offsetof(struct nrf_rpc_cbor_rsp_ctx, writer),
		      "Context structures fields does not match");

NRF_RPC_STATIC_ASSERT(offsetof(struct handler_proxy_ctx, value) ==
		      offsetof(struct nrf_rpc_cbor_rsp_ctx, value),
		      "Context structures fields does not match");
NRF_RPC_STATIC_ASSERT(offsetof(struct handler_proxy_ctx, parser) ==
		      offsetof(struct nrf_rpc_cbor_rsp_ctx, parser),
		      "Context structures fields does not match");
NRF_RPC_STATIC_ASSERT(offsetof(struct handler_proxy_ctx, reader) ==
		      offsetof(struct nrf_rpc_cbor_rsp_ctx, reader),
		      "Context structures fields does not match");
NRF_RPC_STATIC_ASSERT(offsetof(struct handler_proxy_ctx, in_packet) ==
		      offsetof(struct nrf_rpc_cbor_rsp_ctx, in_packet),
		      "Context structures fields does not match");

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

	if (cbor_encode_null(&ctx->encoder) != CborNoError) {
		NRF_RPC_CBOR_DISCARD(*ctx);
		return -NRF_ENOMEM;
	}

	len = cbor_buf_writer_buffer_size(&ctx->writer, ctx->out_packet);

	return nrf_rpc_cmd(group, cmd, ctx->out_packet, len,
				&_nrf_rpc_cbor_proxy_handler,
				(void *)&cbor_handler);
}

int nrf_rpc_cbor_cmd_rsp(const struct nrf_rpc_group *group, uint8_t cmd,
			 struct nrf_rpc_cbor_rsp_ctx *ctx)
{
	int err;
	size_t len;
	size_t rsp_size;

	if (cbor_encode_null(&ctx->encoder) != CborNoError) {
		NRF_RPC_CBOR_DISCARD(*ctx);
		return -NRF_ENOMEM;
	}

	len = cbor_buf_writer_buffer_size(&ctx->writer, ctx->out_packet);

	err = nrf_rpc_cmd_rsp(group, cmd, ctx->out_packet, len,
				&ctx->in_packet, &rsp_size);

	if (err < 0) {
		cbor_buf_reader_init(&ctx->reader, "", 0);
	} else {
		cbor_buf_reader_init(&ctx->reader, ctx->in_packet, rsp_size);
	}

	if (cbor_parser_init(&ctx->reader.r, 0, &ctx->parser, &ctx->value)
				!= CborNoError && err >= 0) {

		nrf_rpc_decoding_done(ctx->in_packet);
		err = -NRF_EBADMSG;
	}
	ctx->value.remaining = UINT32_MAX;

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
				 struct nrf_rpc_cbor_rsp_ctx *ctx)
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

	if (cbor_encode_null(&ctx->encoder) != CborNoError) {
		NRF_RPC_CBOR_DISCARD(*ctx);
		return -NRF_ENOMEM;
	}

	len = cbor_buf_writer_buffer_size(&ctx->writer, ctx->out_packet);

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

	if (cbor_encode_null(&ctx->encoder) != CborNoError) {
		NRF_RPC_CBOR_DISCARD(*ctx);
		return -NRF_ENOMEM;
	}

	len = cbor_buf_writer_buffer_size(&ctx->writer, ctx->out_packet);

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
	cbor_buf_writer_init(&ctx->writer, ctx->out_packet, len);
	cbor_encoder_init(&ctx->encoder, &ctx->writer.enc, 0);
}
