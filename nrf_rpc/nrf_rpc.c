/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */
#define NRF_RPC_LOG_MODULE NRF_RPC
#include <nrf_rpc_log.h>

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "nrf_rpc.h"
#include "nrf_rpc_tr.h"
#include "nrf_rpc_os.h"

#if defined(__GNUC__)
/* Content of "NRF_RPC_AUTO_ARR" arrays are added by the linker script,
 * so the compiler should ignore any out of bounds warnings.
 */
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif

/* A pointer value to pass information that response */
#define RESPONSE_HANDLED_PTR ((uint8_t *)1)

/* Internal definition used in the macros. */
#define NRF_RPC_HEADER_SIZE 5

/* Protocol version field size inside initialization packet. */
#define NRF_RPC_PROTOCOL_VERSION_FIELD_SIZE sizeof(struct protocol_version)

/* Maximum possible protocol version. */
#define NRF_RPC_MAXIMUM_PROTOCOL_VERSION 15

/* Context holding state of the command execution.
 * Context contains data required to receive response to the command and
 * receive recursice commands. When a thread is waiting for a response
 * this context is associated with that thread to make sure that consecutive
 * commands from that thread will reuse this context.
 */
struct nrf_rpc_cmd_ctx {

	uint8_t id;		   /* Context id (index in cmd_ctx_pool array)*/
	uint8_t remote_id;	   /* Context id on the remote side which is
				    * associated with this context or
				    * NRF_RPC_ID_UNKNOWN if it was not
				    * associated yet.
				    */
	uint8_t use_count;	   /* Context usage counter. It increases
				    * each time context is reused.
				    */
	nrf_rpc_handler_t handler; /* Response handler provided be the user. */
	void *handler_data;	   /* Pointer for the response handler. */
	struct nrf_rpc_os_msg recv_msg;
				   /* Message passing between transport
				    * receive callback and a thread that waits
				    * for a response or a recursive commands.
				    */
};

/* Structure holding header information to encode or decode it. */
struct header {
	uint8_t dst;
	uint8_t src;
	uint8_t type;
	uint8_t id;
	uint8_t src_group_id;
	uint8_t dst_group_id;
};

/* Helper structure for encoding and decoding protocol version range from the
 * initialization packet.
 */
struct protocol_version {
	uint8_t min_version:4; /* Minimum supported protocol version. */
	uint8_t max_version:4; /* Maximum supported protocol version. */
} __packed;

/* Group initialization packet data. */
struct init_packet_data {
	uint8_t min_version; /* Remote minimum supported protocol version. */
	uint8_t max_version; /* Remote maximum supported protocol version. */
	size_t strid_len;    /* Length of the unique identifier of the remote group. */
	const char *strid;   /* Remote group unique identifier. */
};

/* Pool of statically allocated command contexts. */
static struct nrf_rpc_cmd_ctx cmd_ctx_pool[CONFIG_NRF_RPC_CMD_CTX_POOL_SIZE];

static struct nrf_rpc_os_event groups_init_event;

/* Number of groups */
static uint8_t group_count;
static uint8_t waiting_group_count;

static uint8_t initialized_group_count;

/* nRF RPC initialization status. */
static bool is_initialized;

/* Error handler provided to the init function. */
static nrf_rpc_err_handler_t global_err_handler;

/* Array with all defiend groups */
NRF_RPC_AUTO_ARR(nrf_rpc_groups_array, "grp");

/* Check the nRF RPC protocol version. */
NRF_RPC_STATIC_ASSERT(NRF_RPC_PROTOCOL_VERSION <= NRF_RPC_MAXIMUM_PROTOCOL_VERSION,
		      "nRF RPC protocol version must be smaller or equal to "
		      NRF_RPC_STRINGIFY(NRF_RPC_MAXIMUM_PROTOCOL_VERSION));

/* ======================== Common utilities ======================== */

static void free_rx_buf(const struct nrf_rpc_group *group, const uint8_t *packet)
{
	/* This is an optional transport layer functionality. */
	if (group->transport->api->rx_buf_free) {

		if (!group->data->transport_initialized) {
			NRF_RPC_ERR("Transport is not initialized");
			return;
		}

		group->transport->api->rx_buf_free(group->transport, (void *)packet);
	}
}

static int send(const struct nrf_rpc_group *group, const uint8_t *data, size_t length)
{
	if (!group->data->transport_initialized) {
		NRF_RPC_ERR("Transport is not initialized");
		return -NRF_ENODEV;
	}

	if (!group->transport->api->send) {
		NRF_RPC_ASSERT(false);
		return -NRF_EIO;
	}

	return group->transport->api->send(group->transport, data, length);
}

static inline bool auto_free_rx_buf(const struct nrf_rpc_tr *transport)
{
	return (transport->api->rx_buf_free == NULL);
}

static struct nrf_rpc_cmd_ctx *cmd_ctx_alloc(void)
{
	struct nrf_rpc_cmd_ctx *ctx;
	uint32_t index;

	index = nrf_rpc_os_ctx_pool_reserve();

	NRF_RPC_ASSERT(index < CONFIG_NRF_RPC_CMD_CTX_POOL_SIZE);

	ctx = &cmd_ctx_pool[index];
	ctx->handler = NULL;
	ctx->remote_id = NRF_RPC_ID_UNKNOWN;
	ctx->use_count = 1;

	nrf_rpc_os_tls_set(ctx);

	NRF_RPC_DBG("Command context %d allocated", ctx->id);

	return ctx;
}

static void cmd_ctx_free(struct nrf_rpc_cmd_ctx *ctx)
{
	nrf_rpc_os_tls_set(NULL);
	nrf_rpc_os_ctx_pool_release(ctx->id);
}

static struct nrf_rpc_cmd_ctx *cmd_ctx_reserve(void)
{
	struct nrf_rpc_cmd_ctx *ctx = nrf_rpc_os_tls_get();

	if (ctx == NULL) {
		return cmd_ctx_alloc();
	}

	ctx->use_count++;

	return ctx;
}

static void cmd_ctx_release(struct nrf_rpc_cmd_ctx *ctx)
{
	ctx->use_count--;
	if (ctx->use_count == 0) {
		cmd_ctx_free(ctx);
	}
}

static struct nrf_rpc_cmd_ctx *cmd_ctx_get_by_id(uint8_t id)
{
	if (id >= CONFIG_NRF_RPC_CMD_CTX_POOL_SIZE) {
		return NULL;
	}
	return &cmd_ctx_pool[id];
}

static struct nrf_rpc_cmd_ctx *cmd_ctx_get_current()
{
	struct nrf_rpc_cmd_ctx *ctx = nrf_rpc_os_tls_get();

	NRF_RPC_ASSERT(ctx != NULL);

	return ctx;
}

static inline int header_decode(const uint8_t *packet, size_t len,
				struct header *hdr)
{
	if (len < NRF_RPC_HEADER_SIZE) {
		hdr->src = NRF_RPC_ID_UNKNOWN;
		hdr->type = NRF_RPC_ID_UNKNOWN;
		hdr->id = NRF_RPC_ID_UNKNOWN;
		hdr->dst = NRF_RPC_ID_UNKNOWN;
		hdr->dst_group_id = NRF_RPC_ID_UNKNOWN;
		return -NRF_EBADMSG;
	}

	if (packet[0] & 0x80) {
		hdr->src = packet[0] & 0x7F;
		hdr->type = packet[0] & 0x80;
	} else {
		hdr->src = NRF_RPC_ID_UNKNOWN;
		hdr->type = packet[0];
	}

	hdr->id = packet[1];
	hdr->dst = packet[2];
	hdr->src_group_id = packet[3];
	hdr->dst_group_id = packet[4];

	return 0;
}

static inline void header_encode(uint8_t *packet, struct header *hdr)
{
	packet[0] = hdr->type;
	packet[1] = hdr->id;
	packet[2] = hdr->dst;
	packet[3] = hdr->src_group_id;
	packet[4] = hdr->dst_group_id;
}

static inline void header_cmd_encode(uint8_t *packet, struct header *hdr)
{
	packet[0] = NRF_RPC_PACKET_TYPE_CMD | hdr->src;
	packet[1] = hdr->id;
	packet[2] = hdr->dst;
	packet[3] = hdr->src_group_id;
	packet[4] = hdr->dst_group_id;
}

/* Function simplifying sending a short packets */
static int simple_send(const struct nrf_rpc_group *group, uint8_t dst, uint8_t type, uint8_t id,
		       uint8_t group_id, uint8_t dst_group_id, const uint8_t *packet, size_t len)
{
	struct header hdr;
	uint8_t *tx_buf;

	hdr.dst = dst;
	hdr.type = type;
	hdr.id = id;
	hdr.src_group_id = group_id;
	hdr.dst_group_id = dst_group_id;

	nrf_rpc_alloc_tx_buf(group, &tx_buf, len);
	if (tx_buf == NULL) {
		return -NRF_ENOMEM;
	}

	tx_buf -= NRF_RPC_HEADER_SIZE;

	header_encode(tx_buf, &hdr);

	if (packet != NULL && len > 0) {
		memcpy(&tx_buf[NRF_RPC_HEADER_SIZE], packet, len);
	}

	return send(group, tx_buf, NRF_RPC_HEADER_SIZE + len);
}

static int group_init_send(const struct nrf_rpc_group *group)
{
	struct protocol_version *protocol_version;
	struct header hdr;
	uint8_t *tx_buf;
	uint8_t *packet;
	size_t len;

	hdr.dst = NRF_RPC_ID_UNKNOWN;
	hdr.type = NRF_RPC_PACKET_TYPE_INIT;
	hdr.id = 0;
	hdr.src_group_id = group->data->src_group_id;
	hdr.dst_group_id = group->data->dst_group_id;

	len = strlen(group->strid) + NRF_RPC_PROTOCOL_VERSION_FIELD_SIZE;

	/* Allocates place for packet data and nRF RPC header.
	 * Returned pointer is set to the first position after header data.
	 */
	nrf_rpc_alloc_tx_buf(group, &tx_buf, len);
	if (tx_buf == NULL) {
		return -NRF_ENOMEM;
	}

	packet = tx_buf;
	/* Calculate the start address of the nRF RPC payload buffer. Pointer returned by
	 * @ref nrf_rpc_alloc_tx_buf is set to first byte after nRF RPC packet header so
	 * it is necessary here to subtract the header size.
	 */
	tx_buf -= NRF_RPC_HEADER_SIZE;

	header_encode(tx_buf, &hdr);

	/* Encode supported protocol version range. */
	protocol_version = (struct protocol_version *)packet;

	protocol_version->min_version = NRF_RPC_PROTOCOL_VERSION;
	protocol_version->max_version = NRF_RPC_PROTOCOL_VERSION;

	packet += NRF_RPC_PROTOCOL_VERSION_FIELD_SIZE;

	memcpy(packet, group->strid, strlen(group->strid));

	return send(group, tx_buf, NRF_RPC_HEADER_SIZE + len);
}

static inline bool packet_validate(const uint8_t *packet)
{
	uintptr_t addr = (uintptr_t)packet;
	/* Checking NULL may be sometimes not enough, because pointer can be
	 * shifted by size of headers.
	 */

	return (addr > NRF_RPC_HEADER_SIZE) &&
	       (addr < (uintptr_t)0 - (uintptr_t)NRF_RPC_HEADER_SIZE);
}

static int transport_init(nrf_rpc_tr_receive_handler_t receive_cb)
{
	int err = 0;
	void *iter;
	const struct nrf_rpc_group *group;

	for (NRF_RPC_AUTO_ARR_FOR(iter, group, &nrf_rpc_groups_array,
				 const struct nrf_rpc_group)) {
		const struct nrf_rpc_tr *transport = group->transport;
		struct nrf_rpc_group_data *data = group->data;

		NRF_RPC_ASSERT(transport != NULL);

		err = transport->api->init(transport, receive_cb, NULL);
		if (err) {
			NRF_RPC_ERR("Failed to initialize transport, err: %d", err);
			continue;
		}

		if (auto_free_rx_buf(transport)) {
			err = nrf_rpc_os_event_init(&data->decode_done_event);
			if (err < 0) {
				continue;
			}
		}

		group->data->transport_initialized = true;

		if (group->flags & NRF_RPC_FLAGS_INITIATOR) {
			err = group_init_send(group);
			if (err) {
				NRF_RPC_ERR("Failed to send group init packet for group id: %d strid: %s err: %d",
					data->src_group_id, group->strid, err);
				continue;
			}
		}
	}

	/* Group initialization errors are not propagated to the caller. */
	err = 0;

	if (waiting_group_count > 0) {
		err = nrf_rpc_os_event_wait(&groups_init_event, CONFIG_NRF_RPC_GROUP_INIT_WAIT_TIME);
		if (err) {
			NRF_RPC_ERR("Not all groups are ready to use.");
		}
	}

	return err;
}

/* ======================== Receiving Packets ======================== */

/* Find in array and execute command or event handler */
static void handler_execute(uint8_t id, const uint8_t *packet, size_t len,
			    const void *array,
			    const struct nrf_rpc_group *group)
{
	void *iter;
	const struct _nrf_rpc_decoder *decoder;

	NRF_RPC_ASSERT(packet_validate(packet));
	NRF_RPC_ASSERT(array != NULL);

	for (NRF_RPC_AUTO_ARR_FOR(iter, decoder, array,
				 const struct _nrf_rpc_decoder)) {

		if (id == decoder->id) {
			decoder->handler(group, packet, len, decoder->handler_data);
			return;
		}
	}

	nrf_rpc_decoding_done(group, packet);

	NRF_RPC_ERR("Unknown command or event received");

	nrf_rpc_err(-NRF_ENOENT, NRF_RPC_ERR_SRC_RECV, group, id,
		    (group->evt_array == array ? NRF_RPC_PACKET_TYPE_EVT :
		     NRF_RPC_PACKET_TYPE_CMD));
}

/* Search for a group based on group_id */
static const struct nrf_rpc_group *group_from_id(uint8_t group_id)
{
	if (group_id >= group_count) {
		return NULL;
	}

	return &NRF_RPC_AUTO_ARR_GET(&nrf_rpc_groups_array, group_id,
				    const struct nrf_rpc_group);
}

/* Parse incoming packet and execute if needed. */
static uint8_t parse_incoming_packet(struct nrf_rpc_cmd_ctx *cmd_ctx,
				     const uint8_t *packet, size_t len)
{
	int err;
	struct header hdr;
	const struct nrf_rpc_group *group = NULL;
	struct nrf_rpc_cmd_ctx *allocated_ctx = NULL;

	/* Validate required parameters */
	NRF_RPC_ASSERT(packet_validate(packet));

	err = header_decode(packet, len, &hdr);

	/* It was already validated in receive handler, so ASSERT is enough. */
	NRF_RPC_ASSERT(err >= 0);

	if (hdr.type == NRF_RPC_PACKET_TYPE_RSP) {
		/* Receive handler will not pass a response if cmd context is
		 * unknown, so ASSERT is enough.
		 */
		NRF_RPC_ASSERT(cmd_ctx != NULL);
		NRF_RPC_DBG("Response received");
		return hdr.type;
	}

	group = group_from_id(hdr.dst_group_id);

	/* It was already validated in receive handler, so ASSERT is enough. */
	NRF_RPC_ASSERT(group != NULL);

	if (hdr.type == NRF_RPC_PACKET_TYPE_CMD) {

		if (cmd_ctx == NULL) {
			allocated_ctx = cmd_ctx_alloc();
			cmd_ctx = allocated_ctx;
		}
		cmd_ctx->remote_id = hdr.src;
		NRF_RPC_DBG("Executing command 0x%02X from group 0x%02X",
			    hdr.id, group->data->src_group_id);
		handler_execute(hdr.id, &packet[NRF_RPC_HEADER_SIZE],
				len - NRF_RPC_HEADER_SIZE, group->cmd_array,
				group);
		if (allocated_ctx != NULL) {
			cmd_ctx_free(allocated_ctx);
		}

	} else if (hdr.type == NRF_RPC_PACKET_TYPE_EVT) {

		NRF_RPC_ASSERT(cmd_ctx == NULL);
		NRF_RPC_DBG("Executing event 0x%02X from group 0x%02X", hdr.id,
			    group->data->src_group_id);
		handler_execute(hdr.id, &packet[NRF_RPC_HEADER_SIZE],
				len - NRF_RPC_HEADER_SIZE, group->evt_array,
				group);
		err = simple_send(group, NRF_RPC_ID_UNKNOWN, NRF_RPC_PACKET_TYPE_ACK,
				  hdr.id, group->data->src_group_id, group->data->dst_group_id,
				  NULL, 0);
		if (err < 0) {
			NRF_RPC_ERR("ACK send error");
			nrf_rpc_err(err, NRF_RPC_ERR_SRC_SEND, group, hdr.id,
				    hdr.type);
		}

	} else {

		/* It was already validated in receive handler. */
		NRF_RPC_ASSERT(0);

	}

	return hdr.type;
}

/* Thread pool callback */
static void execute_packet(const uint8_t *packet, size_t len)
{
	parse_incoming_packet(NULL, packet, len);
}

static bool protocol_version_check(const struct init_packet_data *init_data)
{
	uint8_t protocol_version = NRF_RPC_PROTOCOL_VERSION;

	if ((protocol_version >= init_data->min_version) &&
	    (protocol_version <= init_data->max_version)) {
		return true;
	}

	return false;
}
static const struct nrf_rpc_group *group_from_strid(const char *strid, size_t strid_len)
{
	void *iter;
	const struct nrf_rpc_group *group;

	for (NRF_RPC_AUTO_ARR_FOR(iter, group, &nrf_rpc_groups_array,
				 const struct nrf_rpc_group)) {
		if ((strid_len == strlen(group->strid)) &&
		    (memcmp(strid, group->strid, strid_len) == 0)) {
			return group;
		}
	}

	return NULL;
}

static int init_packet_parse(struct init_packet_data *init_data, const uint8_t *packet, size_t len)
{
	struct protocol_version *protocol_version;

	if (len < (NRF_RPC_HEADER_SIZE + NRF_RPC_PROTOCOL_VERSION_FIELD_SIZE)) {
		NRF_RPC_ERR("To small initialization packet");

		return -NRF_EBADMSG;
	}

	packet += NRF_RPC_HEADER_SIZE;

	protocol_version = (struct protocol_version *)packet;

	init_data->min_version = protocol_version->min_version;
	init_data->max_version = protocol_version->max_version;

	packet += NRF_RPC_PROTOCOL_VERSION_FIELD_SIZE;

	init_data->strid_len = len - (NRF_RPC_HEADER_SIZE + NRF_RPC_PROTOCOL_VERSION_FIELD_SIZE);

	if (init_data->strid_len == 0) {
		NRF_RPC_ERR("Initialization packet does not contain remote group strid");

		return -NRF_EBADMSG;
	}

	init_data->strid = (char *)packet;

	return 0;
}

static int init_packet_handle(struct header *hdr, const struct nrf_rpc_group **group,
			      const uint8_t *packet, size_t len)
{
	int err;
	struct init_packet_data init_data = {0};
	struct nrf_rpc_group_data *group_data;
	bool first_init;
	bool wait_on_init;

	*group = NULL;

	err = init_packet_parse(&init_data, packet, len);
	if (err) {
		return err;
	}

	/* Check the protocol version */
	if (!protocol_version_check(&init_data)) {
		NRF_RPC_ERR("Unsupported protocol version. Local version: %d, "
			    "remote protocol version range: %d - %d",
			    NRF_RPC_PROTOCOL_VERSION, init_data.min_version,
			    init_data.max_version);

		return -NRF_EPROTONOSUPPORT;
	}

	/* Check for the corresponding local group and initialize it. */
	*group = group_from_strid(init_data.strid, init_data.strid_len);
	if (*group == NULL) {
		NRF_RPC_ERR("Remote group does not match local group");
		NRF_RPC_ASSERT(false);
		return -NRF_EFAULT;
	}

	group_data = (*group)->data;
	first_init = group_data->dst_group_id == NRF_RPC_ID_UNKNOWN;
	group_data->dst_group_id = hdr->src_group_id;
	wait_on_init = (*group)->flags & NRF_RPC_FLAGS_WAIT_ON_INIT;
	nrf_rpc_group_bound_handler_t bound_handler = (*group)->bound_handler;

	NRF_RPC_DBG("Found corresponding local group. Remote id: %d, Local id: %d",
		    hdr->src_group_id, group_data->src_group_id);

	if (bound_handler != NULL) {
		bound_handler(*group);
	}

	if (first_init && wait_on_init) {
		++initialized_group_count;

		if (initialized_group_count == waiting_group_count) {
			/* All groups are initialized. */
			nrf_rpc_os_event_set(&groups_init_event);
		}
	}

	if (hdr->dst_group_id == NRF_RPC_ID_UNKNOWN) {
		/*
		 * If remote processor does not know our group id, send an init packet back,
		 * since it might have missed our original init packet.
		 */
		err = group_init_send(*group);
		if (err) {
			NRF_RPC_ERR("Failed to send group init packet for group id: %d strid: %s",
				    group_data->src_group_id, (**group).strid);
		}
	}

	return err;
}

/* Callback from transport layer that handles incoming. */
static void receive_handler(const struct nrf_rpc_tr *transport, const uint8_t *packet, size_t len,
			    void *context)
{
	int err;
	int remote_err;
	struct header hdr;
	struct nrf_rpc_cmd_ctx *cmd_ctx;
	const struct nrf_rpc_group *group = NULL;

	err = header_decode(packet, len, &hdr);
	if (err < 0) {
		NRF_RPC_ERR("Packet too small");
		goto cleanup_and_exit;
	}

	if (hdr.type == NRF_RPC_PACKET_TYPE_CMD ||
	    hdr.type == NRF_RPC_PACKET_TYPE_EVT ||
	    hdr.type == NRF_RPC_PACKET_TYPE_ACK ||
	    hdr.type == NRF_RPC_PACKET_TYPE_RSP ||
	    hdr.type == NRF_RPC_PACKET_TYPE_ERR) {

		group = group_from_id(hdr.dst_group_id);
		if (group == NULL) {
			NRF_RPC_ERR("Invalid group id");
			err = -NRF_EBADMSG;
			goto cleanup_and_exit;
		}
	}

	NRF_RPC_DBG("Received %d bytes packet from %d to %d, type 0x%02X, "
		    "cmd/evt/cnt 0x%02X, grp %d (%s)", len, hdr.src, hdr.dst,
		    hdr.type, hdr.id, hdr.src_group_id,
		    (group != NULL) ? group->strid : "unknown");

	if (hdr.type == NRF_RPC_PACKET_TYPE_CMD &&
	    hdr.dst == NRF_RPC_ID_UNKNOWN) {
		/* In this place command behaves almost the same as an event if
		 * destination in unknown. Local change of type avoids code
		 * duplication.
		 */
		hdr.type = NRF_RPC_PACKET_TYPE_EVT;
	}

	switch (hdr.type) {

	case NRF_RPC_PACKET_TYPE_CMD: /* with known destination */
	case NRF_RPC_PACKET_TYPE_RSP:

		cmd_ctx = cmd_ctx_get_by_id(hdr.dst);

		if (cmd_ctx == NULL) {
			NRF_RPC_ERR("Invalid ctx id in received packet");
			err = -NRF_EBADMSG;
			goto cleanup_and_exit;
		}
		if (cmd_ctx->handler != NULL &&
		    hdr.type == NRF_RPC_PACKET_TYPE_RSP &&
		    auto_free_rx_buf(transport)) {
			cmd_ctx->handler(group, &packet[NRF_RPC_HEADER_SIZE],
					 len - NRF_RPC_HEADER_SIZE,
					 cmd_ctx->handler_data);
			nrf_rpc_os_msg_set(&cmd_ctx->recv_msg,
					   RESPONSE_HANDLED_PTR, 0);
			goto cleanup_and_exit;

		} else {
			nrf_rpc_os_msg_set(&cmd_ctx->recv_msg, packet, len);

			if (auto_free_rx_buf(transport)) {
				nrf_rpc_os_event_wait(&group->data->decode_done_event,
						      NRF_RPC_OS_WAIT_FOREVER);
			}

		}
		return;

	case NRF_RPC_PACKET_TYPE_EVT:
		/* or NRF_RPC_PACKET_TYPE_CMD with unknown destination. */
		nrf_rpc_os_thread_pool_send(packet, len);

		if (auto_free_rx_buf(transport)) {
			nrf_rpc_os_event_wait(&group->data->decode_done_event,
					      NRF_RPC_OS_WAIT_FOREVER);
		}

		return;

	case NRF_RPC_PACKET_TYPE_ACK:
		if (group->ack_handler != NULL) {
			group->ack_handler(hdr.id, group->ack_handler_data);
		}
		break;

	case NRF_RPC_PACKET_TYPE_ERR:
		remote_err = -NRF_EBADMSG;
		if (len >= NRF_RPC_HEADER_SIZE + sizeof(int)) {
			remote_err = *(int *)(&packet[NRF_RPC_HEADER_SIZE]);
		}
		nrf_rpc_err(remote_err, NRF_RPC_ERR_SRC_REMOTE, group, hdr.id,
			    hdr.src);
		break;

	case NRF_RPC_PACKET_TYPE_INIT:
		err = init_packet_handle(&hdr, &group, packet, len);
		break;

	default:
		NRF_RPC_ERR("Invalid type of packet received");
		err = -NRF_EBADMSG;
		break;
	}

cleanup_and_exit:
	if (!auto_free_rx_buf(transport)) {
		transport->api->rx_buf_free(transport, (void *)packet);
	}

	if (err < 0) {
		nrf_rpc_err(err, NRF_RPC_ERR_SRC_RECV, group, hdr.id,
			    hdr.type);
	}
}

void nrf_rpc_decoding_done(const struct nrf_rpc_group *group, const uint8_t *packet)
{
	const uint8_t *full_packet = &packet[-NRF_RPC_HEADER_SIZE];

	if (packet != NULL) {
		if (auto_free_rx_buf(group->transport)) {
			nrf_rpc_os_event_set(&group->data->decode_done_event);
		} else {
			free_rx_buf(group, (void *)full_packet);
		}
	}
}

/* ======================== Command sending ======================== */

/** Wait for response after sending a command.
 *
 * Setting @a rsp_packet and @a rsp_len to NULL informs that the response packet
 * should be handled from filter function before it gets here.
 *
 * @param      local_ep   Endpoint that will receive the response.
 * @param[out] rsp_packet If not NULL contains response packet data.
 * @param[out] rsp_len    If not NULL contains response packet length.
 * @return 0 on success or negative error code.
 */
static void wait_for_response(const struct nrf_rpc_group *group, struct nrf_rpc_cmd_ctx *cmd_ctx,
			      const uint8_t **rsp_packet, size_t *rsp_len)
{
	size_t len;
	const uint8_t *packet;
	uint8_t type;

	NRF_RPC_ASSERT(cmd_ctx != NULL);

	NRF_RPC_DBG("Waiting for a response");

	do {
		nrf_rpc_os_msg_get(&cmd_ctx->recv_msg, &packet, &len);

		NRF_RPC_ASSERT(packet != NULL);

		if (packet == RESPONSE_HANDLED_PTR) {
			return;
		}

		type = parse_incoming_packet(cmd_ctx, packet, len);

	} while (type != NRF_RPC_PACKET_TYPE_RSP);

	if (rsp_packet != NULL) {

		NRF_RPC_ASSERT(rsp_len != NULL);
		*rsp_packet = &packet[NRF_RPC_HEADER_SIZE];
		*rsp_len = len - NRF_RPC_HEADER_SIZE;

	} else if (!auto_free_rx_buf(group->transport) && cmd_ctx->handler != NULL) {

		cmd_ctx->handler(group,
				 &packet[NRF_RPC_HEADER_SIZE],
				 len - NRF_RPC_HEADER_SIZE,
				 cmd_ctx->handler_data);

		nrf_rpc_decoding_done(group, &packet[NRF_RPC_HEADER_SIZE]);
	}
}

int nrf_rpc_cmd_common(const struct nrf_rpc_group *group, uint32_t cmd,
			uint8_t *packet, size_t len, void *ptr1, void *ptr2)
{
	int err;
	struct header hdr;
	nrf_rpc_handler_t old_handler;
	void *old_handler_data;
	uint8_t *full_packet = &packet[-NRF_RPC_HEADER_SIZE];
	nrf_rpc_handler_t handler = NULL;
	void *handler_data = NULL;
	const uint8_t **rsp_packet = NULL;
	size_t *rsp_len = NULL;
	struct nrf_rpc_cmd_ctx *cmd_ctx;

	NRF_RPC_ASSERT(group != NULL);
	NRF_RPC_ASSERT((cmd & 0xFF) != NRF_RPC_ID_UNKNOWN);
	NRF_RPC_ASSERT(packet_validate(packet));
	NRF_RPC_ASSERT(ptr1 != NULL);

	if (cmd & 0x10000) {
		NRF_RPC_ASSERT(ptr2 != NULL);
		rsp_packet = ptr1;
		rsp_len = ptr2;
		*rsp_packet = NULL;
		*rsp_len = 0;
	} else {
		handler = ptr1;
		handler_data = ptr2;
	}

	cmd_ctx = cmd_ctx_reserve();

	hdr.dst = cmd_ctx->remote_id;
	hdr.src = cmd_ctx->id;
	hdr.id = cmd & 0xFF;
	hdr.src_group_id = group->data->src_group_id;
	hdr.dst_group_id = group->data->dst_group_id;
	header_cmd_encode(full_packet, &hdr);

	old_handler = cmd_ctx->handler;
	old_handler_data = cmd_ctx->handler_data;
	cmd_ctx->handler = handler;
	cmd_ctx->handler_data = handler_data;

	NRF_RPC_DBG("Sending command 0x%02X from group 0x%02X", cmd,
		    group->data->src_group_id);

	err = send(group, full_packet, len + NRF_RPC_HEADER_SIZE);

	if (err >= 0) {
		wait_for_response(group, cmd_ctx, rsp_packet, rsp_len);
	}

	cmd_ctx->handler = old_handler;
	cmd_ctx->handler_data = old_handler_data;

	cmd_ctx_release(cmd_ctx);

	return err;
}

void nrf_rpc_cmd_common_no_err(const struct nrf_rpc_group *group, uint32_t cmd,
			       uint8_t *packet, size_t len, void *ptr1,
			       void *ptr2)
{
	int err;

	err = nrf_rpc_cmd_common(group, cmd, packet, len, ptr1, ptr2);
	if (err < 0) {
		NRF_RPC_ERR("Unhandled command send error %d", err);
		nrf_rpc_err(err, NRF_RPC_ERR_SRC_SEND, group, cmd,
			    NRF_RPC_PACKET_TYPE_CMD);
	}
}

/* ======================== Event sending ======================== */

int nrf_rpc_evt(const struct nrf_rpc_group *group, uint8_t evt, uint8_t *packet,
		size_t len)
{
	int err;
	struct header hdr;
	uint8_t *full_packet = &packet[-NRF_RPC_HEADER_SIZE];

	NRF_RPC_ASSERT(group != NULL);
	NRF_RPC_ASSERT(evt != NRF_RPC_ID_UNKNOWN);
	NRF_RPC_ASSERT(packet_validate(packet));

	hdr.dst = NRF_RPC_ID_UNKNOWN;
	hdr.type = NRF_RPC_PACKET_TYPE_EVT;
	hdr.id = evt;
	hdr.src_group_id = group->data->src_group_id;
	hdr.dst_group_id = group->data->dst_group_id;
	header_encode(full_packet, &hdr);

	NRF_RPC_DBG("Sending event 0x%02X from group 0x%02X", evt,
		    group->data->src_group_id);

	err = send(group, full_packet, len + NRF_RPC_HEADER_SIZE);

	return err;
}

void nrf_rpc_evt_no_err(const struct nrf_rpc_group *group, uint8_t evt,
			uint8_t *packet, size_t len)
{
	int err;

	err = nrf_rpc_evt(group, evt, packet, len);
	if (err < 0) {
		NRF_RPC_ERR("Unhandled event send error %d", err);
		nrf_rpc_err(err, NRF_RPC_ERR_SRC_SEND, group, evt,
			    NRF_RPC_PACKET_TYPE_EVT);
	}
}

/* ======================== Response sending ======================== */

int nrf_rpc_rsp(const struct nrf_rpc_group *group, uint8_t *packet, size_t len)
{
	int err;
	struct header hdr;
	struct nrf_rpc_cmd_ctx *cmd_ctx;
	uint8_t *full_packet = &packet[-NRF_RPC_HEADER_SIZE];

	NRF_RPC_ASSERT(packet_validate(packet));

	cmd_ctx = cmd_ctx_get_current();

	hdr.dst = cmd_ctx->remote_id;
	hdr.type = NRF_RPC_PACKET_TYPE_RSP;
	hdr.id = NRF_RPC_ID_UNKNOWN;
	hdr.src_group_id = group->data->src_group_id;
	hdr.dst_group_id = group->data->dst_group_id;
	header_encode(full_packet, &hdr);

	NRF_RPC_DBG("Sending response");

	err = send(group, full_packet, len + NRF_RPC_HEADER_SIZE);

	return err;
}

void nrf_rpc_rsp_no_err(const struct nrf_rpc_group *group, uint8_t *packet, size_t len)
{
	int err;

	err = nrf_rpc_rsp(group, packet, len);
	if (err < 0) {
		NRF_RPC_ERR("Unhandled response send error %d", err);
		nrf_rpc_err(err, NRF_RPC_ERR_SRC_SEND, group, NRF_RPC_ID_UNKNOWN,
			    NRF_RPC_PACKET_TYPE_RSP);
	}
}

/* ======================== Common API functions ======================== */

int nrf_rpc_init(nrf_rpc_err_handler_t err_handler)
{
	int err;
	int i;
	void *iter;
	const struct nrf_rpc_group *group;
	uint8_t group_id = 0;
	uint8_t wait_count = 0;

	NRF_RPC_DBG("Initializing nRF RPC module");

	if (is_initialized) {
		return 0;
	}

	global_err_handler = err_handler;

	for (NRF_RPC_AUTO_ARR_FOR(iter, group, &nrf_rpc_groups_array,
				 const struct nrf_rpc_group)) {
		struct nrf_rpc_group_data *data = group->data;

		if (group_id >= 0xFF) {
			return -NRF_ENOMEM;
		}

		NRF_RPC_DBG("Group '%s' has id %d", group->strid, group_id);
		data->src_group_id = group_id;
		group_id++;
		if (group->flags & NRF_RPC_FLAGS_WAIT_ON_INIT) {
			wait_count++;
		}
	}

	group_count = group_id;
	waiting_group_count = wait_count;

	memset(&cmd_ctx_pool, 0, sizeof(cmd_ctx_pool));

	err = nrf_rpc_os_init(execute_packet);
	if (err < 0) {
		return err;
	}

	err = nrf_rpc_os_event_init(&groups_init_event);
	if (err < 0) {
		return err;
	}

	for (i = 0; i < CONFIG_NRF_RPC_CMD_CTX_POOL_SIZE; i++) {
		cmd_ctx_pool[i].id = i;
		err = nrf_rpc_os_msg_init(&cmd_ctx_pool[i].recv_msg);
		if (err < 0) {
			return err;
		}
	}

	err = transport_init(receive_handler);
	if (err < 0) {
		return err;
	}

	is_initialized = true;
	NRF_RPC_DBG("Done initializing nRF RPC module");

	return err;
}

/** Report an error that cannot be reported as a function return value */
void nrf_rpc_err(int code, enum nrf_rpc_err_src src,
		 const struct nrf_rpc_group *group, uint8_t id,
		 uint8_t packet_type)
{
	struct nrf_rpc_err_report report;
	uint8_t src_group_id = (group != NULL) ? group->data->src_group_id :
				NRF_RPC_ID_UNKNOWN;
	uint8_t dst_group_id = (group != NULL) ? group->data->dst_group_id :
				NRF_RPC_ID_UNKNOWN;

	NRF_RPC_ERR("Error %s reported %d, group %s, id 0x%02X, type 0x%02X",
		    (src == NRF_RPC_ERR_SRC_RECV) ? "on receive" :
		    (src == NRF_RPC_ERR_SRC_SEND) ? "on send" : "from remote",
		    code, (group != NULL) ? group->strid : "unknown", id,
		    packet_type);

	if ((src == NRF_RPC_ERR_SRC_RECV) && group) {
		simple_send(group, packet_type, NRF_RPC_PACKET_TYPE_ERR, id, src_group_id,
			    dst_group_id, (const uint8_t *)&code, sizeof(code));
	}

	report.code = code;
	report.src = src;
	report.group = group;
	report.id = id;
	report.packet_type = packet_type;

	if (group != NULL && group->err_handler != NULL) {
		group->err_handler(&report);
	}

	if (global_err_handler != NULL) {
		global_err_handler(&report);
	}
}

void nrf_rpc_alloc_tx_buf(const struct nrf_rpc_group *group, uint8_t **buf, size_t len)
{
	size_t req_size = len + NRF_RPC_HEADER_SIZE;
	uint8_t *packet;

	*buf = NULL;

	if (!group->data->transport_initialized) {
		NRF_RPC_ERR("Transport is not initialized");
		return;
	}

	if (!group->transport->api->tx_buf_alloc) {
		NRF_RPC_ASSERT(false);
		return;
	}

	packet = group->transport->api->tx_buf_alloc(group->transport, &req_size);

	if (packet == NULL) {
		NRF_RPC_ASSERT(false);

		return;
	}

	if (req_size < (len + NRF_RPC_HEADER_SIZE)) {
		NRF_RPC_ASSERT(false);

		return;
	}

	*buf = packet + NRF_RPC_HEADER_SIZE;
}

void nrf_rpc_free_tx_buf(const struct nrf_rpc_group *group, uint8_t *buf)
{
	if (!group->transport->api->tx_buf_free) {
		NRF_RPC_ASSERT(false);

		return;
	}

	if (!group->data->transport_initialized) {
		NRF_RPC_ERR("Transport is not initialized");
		return;
	}

	/* We need to subtract packet header placeholder. */
	buf -= NRF_RPC_HEADER_SIZE;

	group->transport->api->tx_buf_free(group->transport, buf);
}
