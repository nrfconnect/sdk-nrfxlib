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

#undef nrf_rpc_cbor_cmd_no_err
#undef nrf_rpc_cbor_cmd_rsp_no_err
#undef nrf_rpc_cbor_evt_no_err
#undef nrf_rpc_cbor_rsp_no_err

/* Maximum RPC parameters that can be passed */
#define NRF_RPC_MAX_PARAMETERS 255

static inline size_t nrf_rpc_cbor_data_len(const struct nrf_rpc_cbor_ctx *ctx)
{
	return ctx->zs->payload_mut - ctx->out_packet;
}

bool nrf_rpc_cbor_is_alloc(struct nrf_rpc_cbor_ctx *ctx)
{
	return (ctx->out_packet != NULL);
}

int nrf_rpc_cbor_cmd(const struct nrf_rpc_group *group, uint8_t cmd,
		     struct nrf_rpc_cbor_ctx *ctx,
		     nrf_rpc_cbor_handler_t handler, void *handler_data)
{
	size_t len;
	const struct _nrf_rpc_cbor_decoder cbor_handler = {
		.handler = handler,
		.handler_data = handler_data,
	};

	if (!zcbor_nil_put(ctx->zs, NULL)) {
		NRF_RPC_CBOR_DISCARD(group, *ctx);
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


	if (!zcbor_nil_put(ctx->zs, NULL)) {
		NRF_RPC_CBOR_DISCARD(group, *ctx);
		return -NRF_ENOMEM;
	}

	len = nrf_rpc_cbor_data_len(ctx);

	err = nrf_rpc_cmd_rsp(group, cmd, ctx->out_packet, len, &ctx->in_packet,
			      &rsp_size);

	if (err >= 0) {
		zcbor_new_decode_state(ctx->zs, ZCBOR_ARRAY_SIZE(ctx->zs),
				       ctx->out_packet, rsp_size, NRF_RPC_MAX_PARAMETERS, NULL, 0);
		ctx->zs->constant_state->stop_on_error = true;
	}

	return err;
}

static void check_send_err(int err, const struct nrf_rpc_group *group, uint8_t id, uint8_t packet_type, const char *file, int line, const char *func)
{
	if (err < 0) {
		NRF_RPC_ERR("Unhandled command send error %d", err);
		nrf_rpc_err_impl(err, NRF_RPC_ERR_SRC_SEND, group, id, packet_type, file, line, func);
	}
}

void nrf_rpc_cbor_cmd_no_err_impl(const struct nrf_rpc_group *group, uint8_t cmd,
			     struct nrf_rpc_cbor_ctx *ctx,
			     nrf_rpc_cbor_handler_t handler,
			     void *handler_data,
			     const char *file, int line, const char *func)
{
	check_send_err(nrf_rpc_cbor_cmd(group, cmd, ctx, handler, handler_data),
		       group, cmd, NRF_RPC_PACKET_TYPE_CMD, file, line, func);
}

void nrf_rpc_cbor_cmd_no_err(const struct nrf_rpc_group *group, uint8_t cmd,
			     struct nrf_rpc_cbor_ctx *ctx,
			     nrf_rpc_cbor_handler_t handler,
			     void *handler_data)
{
	nrf_rpc_cbor_cmd_no_err_impl(group, cmd, ctx, handler, handler_data, NULL, 0, NULL);
}

void nrf_rpc_cbor_cmd_rsp_no_err_impl(const struct nrf_rpc_group *group, uint8_t cmd,
				 struct nrf_rpc_cbor_ctx *ctx,
				 const char *file, int line, const char *func)
{
	check_send_err(nrf_rpc_cbor_cmd_rsp(group, cmd, ctx),
		       group, cmd, NRF_RPC_PACKET_TYPE_CMD, file, line, func);
}

void nrf_rpc_cbor_cmd_rsp_no_err(const struct nrf_rpc_group *group, uint8_t cmd,
				 struct nrf_rpc_cbor_ctx *ctx)
{
	nrf_rpc_cbor_cmd_rsp_no_err_impl(group, cmd, ctx, NULL, 0, NULL);
}

int nrf_rpc_cbor_evt(const struct nrf_rpc_group *group, uint8_t evt,
		     struct nrf_rpc_cbor_ctx *ctx)
{

	size_t len;

	if (!zcbor_nil_put(ctx->zs, NULL)) {
		NRF_RPC_CBOR_DISCARD(group, *ctx);
		return -NRF_ENOMEM;
	}

	len = nrf_rpc_cbor_data_len(ctx);

	return nrf_rpc_evt(group, evt, ctx->out_packet, len);
}

void nrf_rpc_cbor_evt_no_err_impl(const struct nrf_rpc_group *group, uint8_t evt,
			     struct nrf_rpc_cbor_ctx *ctx,
			     const char *file, int line, const char *func)
{
	check_send_err(nrf_rpc_cbor_evt(group, evt, ctx),
		       group, evt, NRF_RPC_PACKET_TYPE_EVT, file, line, func);
}

void nrf_rpc_cbor_evt_no_err(const struct nrf_rpc_group *group, uint8_t evt,
			     struct nrf_rpc_cbor_ctx *ctx)
{
	nrf_rpc_cbor_evt_no_err_impl(group, evt, ctx, NULL, 0, NULL);
}

int nrf_rpc_cbor_rsp(const struct nrf_rpc_group *group, struct nrf_rpc_cbor_ctx *ctx)
{

	size_t len;

	if (!zcbor_nil_put(ctx->zs, NULL)) {
		NRF_RPC_CBOR_DISCARD(group, *ctx);
		return -NRF_ENOMEM;
	}

	len = nrf_rpc_cbor_data_len(ctx);

	return nrf_rpc_rsp(group, ctx->out_packet, len);
}

void nrf_rpc_cbor_rsp_no_err_impl(const struct nrf_rpc_group *group, struct nrf_rpc_cbor_ctx *ctx,
			     const char *file, int line, const char *func)
{
	check_send_err(nrf_rpc_cbor_rsp(group, ctx),
		       group, NRF_RPC_ID_UNKNOWN, NRF_RPC_PACKET_TYPE_RSP, file, line, func);
}

void nrf_rpc_cbor_rsp_no_err(const struct nrf_rpc_group *group, struct nrf_rpc_cbor_ctx *ctx)
{
	nrf_rpc_cbor_rsp_no_err_impl(group, ctx, NULL, 0, NULL);
}

void nrf_rpc_cbor_decoding_done(const struct nrf_rpc_group *group, struct nrf_rpc_cbor_ctx *ctx)
{
	nrf_rpc_decoding_done(group, ctx->out_packet);
}

void _nrf_rpc_cbor_proxy_handler(const struct nrf_rpc_group *group, const uint8_t *packet,
				 size_t len, void *handler_data)
{
	struct nrf_rpc_cbor_ctx ctx;

	ctx.in_packet = packet;

	struct _nrf_rpc_cbor_decoder *cbor_handler =
		(struct _nrf_rpc_cbor_decoder *)handler_data;

	zcbor_new_decode_state(ctx.zs, ZCBOR_ARRAY_SIZE(ctx.zs), ctx.out_packet, len,
			       NRF_RPC_MAX_PARAMETERS, NULL, 0);
	ctx.zs->constant_state->stop_on_error = true;

	return cbor_handler->handler(group, &ctx, cbor_handler->handler_data);
}

void _nrf_rpc_cbor_prepare(struct nrf_rpc_cbor_ctx *ctx, size_t len)
{
	zcbor_new_encode_state(ctx->zs, ZCBOR_ARRAY_SIZE(ctx->zs), ctx->out_packet,
			       len, 0);
	ctx->zs->constant_state->stop_on_error = true;
}
