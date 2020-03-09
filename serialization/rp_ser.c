/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <rp_ser.h>
#include <rp_trans.h>
#include <rp_os.h>
#include <rp_errors.h>

#define RP_LOG_MODULE SER_CORE
#include <rp_log.h>

#define RP_SER_RSP_INITIAL_ARRAY_SIZE 2
#define RP_SER_CMD_EVT_INITIAL_ARRAY_SIZE 3

static uint8_t endpoint_cnt;

static uint8_t *buf_tail_get(struct rp_ser_buf *rp_buf)
{
	return rp_buf->buf + rp_buf->packet_size;
}

static size_t buf_free_space_get(struct rp_ser_buf *rp_buf)
{
	return rp_buf->size - rp_buf->packet_size;
}

static cmd_handler_t cmd_handler_get(struct rp_ser *rp, uint8_t cmd)
{
	cmd_handler_t cmd_handler = NULL;

	for (const struct rp_ser_cmd *iter = rp->decoders->cmd_begin;
	     iter <  rp->decoders->cmd_end; iter++) {
		if (cmd == iter->cmd) {
			cmd_handler = iter->func;
			break;
		}
	}

	return cmd_handler;
}

static rp_err_t cmd_execute(struct rp_ser *rp, uint8_t cmd, CborValue *it)
{
	rp_err_t err;
	cmd_handler_t cmd_handler;

	cmd_handler = cmd_handler_get(rp, cmd);

	if (cmd_handler) {
		err = cmd_handler(it);
	} else {
		RP_LOG_ERR("Unsupported command received");
		err = RP_ERROR_NOT_SUPPORTED;
	}

	return err;
}

static rp_err_t event_parse(struct rp_ser *rp, uint8_t evt, CborValue *it)
{
	rp_err_t err;
	evt_handler_t evt_handler = NULL;

	for (const struct rp_ser_evt *iter = rp->decoders->evt_begin;
	     iter <  rp->decoders->evt_end; iter++) {
		if (evt == iter->evt) {
			evt_handler = iter->func;
			break;
		}
	}

	if (evt_handler) {
		err = evt_handler(evt, it);
	} else {
		RP_LOG_ERR("Unsupported event received");
		err = RP_ERROR_NOT_SUPPORTED;
	}

	return err;
}

static rp_err_t response_parse(struct rp_ser *rp, const uint8_t *data,
			       size_t len)
{
	rp_err_t err;
	CborParser parser;
	CborValue value;

	if (len > 0) {
		if (cbor_parser_init(data, len, 0, &parser, &value) !=
		    CborNoError) {
			return RP_ERROR_INTERNAL;
		    }

		/* Extend paraser to handle more than one item. */
		value.remaining = UINT32_MAX;
	}

	if (rp->rsp_handler) {
		err = rp->rsp_handler(&value);

		rp->rsp_handler = NULL;

		if (err) {
			return err;
		}
	}

	return rp_os_response_signal(rp);
}

static rp_err_t event_dispatch(struct rp_ser *rp, const uint8_t *data,
			       size_t len)
{
	uint8_t evt;
	CborParser parser;
	CborValue value;
	uint32_t index = 0;

	if (len < 1) {
		return RP_ERROR_INVALID_PARAM;
	}

	evt = data[index];
	index++;
	len -= index;

	if (len > 0) {
		if (cbor_parser_init(&data[index], len, 0, &parser, &value) !=
		    CborNoError) {
			return RP_ERROR_INTERNAL;
		    }

		value.remaining = UINT32_MAX;
	}

	return event_parse(rp, evt, &value);
}

static rp_err_t cmd_dispatch(struct rp_ser *rp, const uint8_t *data,
			     size_t len)
{
	rp_err_t err;
	uint8_t cmd;
	CborParser parser;
	CborValue value;
	uint32_t index = 0;

	if (len < 1) {
		return RP_ERROR_INVALID_PARAM;
	}

	cmd = data[index];
	index++;
	len -= index;

	if (len > 0) {
		if (cbor_parser_init(&data[index], len, 0, &parser, &value) !=
		    CborNoError) {
			return RP_ERROR_INTERNAL;
		    }

		value.remaining = UINT32_MAX;
	}

	err = cmd_execute(rp, cmd, &value);
	if (err) {
		return err;
	}

	RP_LOG_DBG("Received command 0x%02x", cmd);

	return RP_SUCCESS;
}

static rp_err_t received_data_parse(struct rp_ser *rp,
				    const uint8_t *data, size_t len)
{
	rp_err_t err;
	uint8_t packet_type;
	uint32_t index = 0;

	if (len  < 1) {
		return RP_ERROR_INVALID_PARAM;
	}

	packet_type = data[index];
	index++;
	len -= index;

	switch (packet_type) {
	case RP_SER_PACKET_TYPE_CMD:
		RP_LOG_DBG("Command received");
		err = cmd_dispatch(rp, &data[index], len);

		break;

	case RP_SER_PACKET_TYPE_EVENT:
		RP_LOG_DBG("Event received");
		err = event_dispatch(rp, &data[index], len);

		break;

	case RP_SER_PACKET_TYPE_RSP:
		RP_LOG_DBG("Response received");
		err = response_parse(rp, &data[index], len);

		break;

	default:
		RP_LOG_ERR("Unknown packet received");
		return RP_ERROR_NOT_SUPPORTED;
	}

	if (err) {
		return err;
	}

	return RP_SUCCESS;
}

static void transport_handler(struct rp_trans_endpoint *endpoint,
			      const uint8_t *buf, size_t length)
{
	struct rp_ser *rp = RP_CONTAINER_OF(endpoint, struct rp_ser, endpoint);

	received_data_parse(rp, buf, length);
}

rp_err_t rp_ser_cmd_send(struct rp_ser *rp, struct rp_ser_buf *rp_buf,
			 CborEncoder *encoder, cmd_handler_t rsp)
{
	rp_err_t err;

	if (!rp || !rp_buf) {
		return RP_ERROR_NULL;
	}

	/* Encode NULL value to indicate packet end. */
	if (cbor_encode_null(encoder) != CborNoError) {
		return RP_ERROR_INTERNAL;
	}

	/* Calculate TinyCbor packet size */
	rp_buf->packet_size += cbor_encoder_get_buffer_size(encoder,
							    buf_tail_get(rp_buf));

	if (rp_buf->packet_size < 1) {
		return RP_ERROR_INVALID_PARAM;
	}

	if (rp->rsp_handler) {
		return RP_ERROR_BUSY;
	}

	err = rp_trans_send(&rp->endpoint, rp_buf->buf, rp_buf->packet_size);
	if (err) {
		return err;
	}

	RP_LOG_DBG("Command sent");

	if (rsp) {
		rp->rsp_handler = rsp;

		RP_LOG_DBG("Waiting for response");

		return rp_os_response_wait(rp);
	}

	return RP_SUCCESS;
}

rp_err_t rp_ser_evt_send(struct rp_ser *rp, struct rp_ser_buf *rp_buf,
			 CborEncoder *encoder)
{
	if (!rp || !rp_buf) {
		return RP_ERROR_NULL;
	}

	/* Encode NULL value to indicate packet end. */
	if (cbor_encode_null(encoder) != CborNoError) {
		return RP_ERROR_INTERNAL;
	}

	/* Calculate TinyCbor packet size */
	rp_buf->packet_size += cbor_encoder_get_buffer_size(encoder,
							    buf_tail_get(rp_buf));
	if (rp_buf->packet_size < 1) {
		return RP_ERROR_INVALID_PARAM;
	}

	return rp_trans_send(&rp->endpoint, rp_buf->buf, rp_buf->packet_size);
}

rp_err_t rp_ser_rsp_send(struct rp_ser *rp, struct rp_ser_buf *rp_buf,
			 CborEncoder *encoder)
{
	return rp_ser_evt_send(rp, rp_buf, encoder);
}

rp_err_t rp_ser_init(struct rp_ser *rp)
{
	rp_err_t err;

	if (!rp) {
		return RP_ERROR_NULL;
	}

	err = rp_os_signal_init(rp);
	if (err) {
		return err;
	}

	RP_LOG_DBG("OS signal initialized");

	if (!endpoint_cnt) {
		err = rp_trans_init(transport_handler);
		if (err) {
			return err;
		}

		endpoint_cnt++;
	}

	return rp_trans_endpoint_init(&rp->endpoint, rp->ep_conf->number);
}

void rp_ser_uninit(struct rp_ser *rp)
{
	rp_trans_endpoint_uninit(&rp->endpoint);

	if (endpoint_cnt > 0) {
		endpoint_cnt--;
	} else {
		RP_LOG_DBG("Uninitializing RP transport");
		rp_trans_uninit();
	}
}

rp_err_t rp_ser_cmd_init(struct rp_ser_buf *rp_buf, CborEncoder *encoder,
			 uint8_t cmd)
{
	uint8_t *data = rp_buf->buf;

	if (!rp_buf) {
		return RP_ERROR_NULL;
	}

	if (rp_buf->size < RP_SER_CMD_EVT_INITIAL_ARRAY_SIZE) {
		return RP_ERROR_NO_MEM;
	}

	*data = RP_SER_PACKET_TYPE_CMD;
	rp_buf->packet_size++;
	data++;

	*data = cmd;
	rp_buf->packet_size++;

	cbor_encoder_init(encoder, buf_tail_get(rp_buf),
			  buf_free_space_get(rp_buf), 0);

	return RP_SUCCESS;
}

rp_err_t rp_ser_evt_init(struct rp_ser_buf *rp_buf, CborEncoder *encoder,
			 uint8_t evt)
{
	uint8_t *data = rp_buf->buf;

	if (!rp_buf) {
		return RP_ERROR_NULL;
	}

	if (rp_buf->size < RP_SER_CMD_EVT_INITIAL_ARRAY_SIZE) {
		return RP_ERROR_NO_MEM;
	}

	*data = RP_SER_PACKET_TYPE_EVENT;
	rp_buf->packet_size++;
	data++;

	*data = evt;
	rp_buf->packet_size++;

	cbor_encoder_init(encoder, buf_tail_get(rp_buf),
			  buf_free_space_get(rp_buf), 0);

	return RP_SUCCESS;
}

rp_err_t rp_ser_rsp_init(struct rp_ser_buf *rp_buf, CborEncoder *encoder)
{
	uint8_t *data = rp_buf->buf;

	if (!rp_buf) {
		return RP_ERROR_NULL;
	}

	if (rp_buf->size < RP_SER_RSP_INITIAL_ARRAY_SIZE) {
		return RP_ERROR_NO_MEM;
	}

	*data = RP_SER_PACKET_TYPE_RSP;
	rp_buf->packet_size++;

	cbor_encoder_init(encoder, buf_tail_get(rp_buf),
			  buf_free_space_get(rp_buf), 0);

	return RP_SUCCESS;
}
