/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef _NRF_RPC_H_
#define _NRF_RPC_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <nrf_rpc_errno.h>
#include <nrf_rpc_common.h>
#include <nrf_rpc_tr.h>

/**
 * @defgroup nrf_rpc nRF RPC (Remote Procedure Calls) module.
 * @{
 * @brief Module to call procedures on a remote processor.
 */

#ifdef __cplusplus
extern "C" {
#endif

/* Internal definition used in the macros. */
#define _NRF_RPC_HEADER_SIZE 4

/** @brief Special value to indicate that ID is unknown or irrelevant. */
#define NRF_RPC_ID_UNKNOWN 0xFF

/* Forward declaration. */
struct nrf_rpc_err_report;

/** @brief Type of packet.
 *
 * Used by @ref nrf_rpc_err_report to indicate which packet caused the problem.
 */
enum nrf_rpc_packet_type {
	NRF_RPC_PACKET_TYPE_EVT  = 0x00, /**< @brief Event */
	NRF_RPC_PACKET_TYPE_RSP  = 0x01, /**< @brief Response */
	NRF_RPC_PACKET_TYPE_ACK  = 0x02, /**< @brief Event acknowledge */
	NRF_RPC_PACKET_TYPE_ERR  = 0x03, /**< @brief Error report from remote */
	NRF_RPC_PACKET_TYPE_INIT = 0x04, /**< @brief Initialization packet */
	NRF_RPC_PACKET_TYPE_CMD  = 0x80, /**< @brief Command */
};

/** @brief Error source.
 *
 * Used by @ref nrf_rpc_err_report to indicate when the problem occurred.
 */
enum nrf_rpc_err_src {
	NRF_RPC_ERR_SRC_RECV,   /**< @brief Error during receiving */
	NRF_RPC_ERR_SRC_SEND,   /**< @brief Error during sending */
	NRF_RPC_ERR_SRC_REMOTE, /**< @brief Error reported be the remote */
};

/** @brief Callback that handles decoding of commands, events and responses.
 *
 * @param packet       Packet data.
 * @param len          Length of the packet.
 * @param handler_data Opaque pointer provided by the user.
 */
typedef void (*nrf_rpc_handler_t)(const uint8_t *packet, size_t len,
				  void *handler_data);

/** @brief Callback called when ACK was received.
 *
 * @see NRF_RPC_GROUP_DEFINE
 *
 * @param id           Id of the event that was completed.
 * @param handler_data Opaque pointer provided by the user.
 */
typedef void (*nrf_rpc_ack_handler_t)(uint8_t id, void *handler_data);

/** @brief Callback to report error that cannot be returned by the API call.
 *
 * This callback will be called when received packet cannot be correctly
 * interpreted. Second case is when one of `_no_err` sending functions failed.
 *
 * Callback can be assigned to specific group. If group in which error occurred
 * is known associated error handler is called. Later global error handler
 * provided in @ref nrf_rpc_init is called.
 *
 * @see NRF_RPC_GROUP_DEFINE
 * @see nrf_rpc_init
 *
 * @param report Error report.
 */
typedef void (*nrf_rpc_err_handler_t)(const struct nrf_rpc_err_report *report);

/* Structure used internally to define registered command or event decoder. */
struct _nrf_rpc_decoder {
	uint8_t id;
	nrf_rpc_handler_t handler;
	void *handler_data;
};

/** @brief Defines a group of commands and events.
 *
 * Created by @ref NRF_RPC_GROUP_DEFINE.
 *
 * Fields of this structure are used internally by nRF RPC and not intended to
 * be used by the user.
 */
struct nrf_rpc_group {
	uint8_t *group_id;
	const void *cmd_array;
	const void *evt_array;
	nrf_rpc_ack_handler_t ack_handler;
	void *ack_handler_data;
	const char *strid;
	nrf_rpc_err_handler_t err_handler;
};

/** @brief Error report.
 */
struct nrf_rpc_err_report {

	/** @brief Error code. */
	int code;

	/** @brief Group where the error occurred or NULL if it is unknown. */
	const struct nrf_rpc_group *group;

	/** @brief Source of the error. */
	enum nrf_rpc_err_src src;

	/** @brief Command or event id or @ref NRF_RPC_ID_UNKNOWN. */
	uint8_t id;

	/** @brief Type of packet. Value may be outside defined enum values
	 * if packet is malformed.
	 */
	enum nrf_rpc_packet_type packet_type;
};

/** @brief Define a group of commands and events.
 *
 * @param _name        Symbol name of the group.
 * @param _strid       String containing unique identifier of the group. Naming
 *                     conventions the same as C symbol name. Groups on local
 *                     and remote must have the same unique identifier.
 * @param _ack_handler Handler of type @ref nrf_rpc_ack_handler_t called when
 *                     ACK was received after event completion. Can be NULL if
 *                     group does not want to receive ACK notifications.
 * @param _ack_data    Opaque pointer for the `_ack_handler`.
 * @param _err_handler Handler of type @ref nrf_rpc_err_handler_t called when
 *                     error occurred in context of this group. Can be NULL if
 *                     group does not want to receive error notifications.
 */
#define NRF_RPC_GROUP_DEFINE(_name, _strid, _ack_handler, _ack_data,	       \
			     _err_handler)				       \
	NRF_RPC_AUTO_ARR(NRF_RPC_CONCAT(_name, _cmd_array),		       \
			 "cmd_" NRF_RPC_STRINGIFY(_name));		       \
	NRF_RPC_AUTO_ARR(NRF_RPC_CONCAT(_name, _evt_array),		       \
			 "evt_" NRF_RPC_STRINGIFY(_name));		       \
	static uint8_t NRF_RPC_CONCAT(_name, _group_id);		       \
	NRF_RPC_AUTO_ARR_ITEM(const struct nrf_rpc_group, _name, "grp",	       \
			      _strid) = {				       \
		.group_id = &NRF_RPC_CONCAT(_name, _group_id),		       \
		.cmd_array = &NRF_RPC_CONCAT(_name, _cmd_array),	       \
		.evt_array = &NRF_RPC_CONCAT(_name, _evt_array),	       \
		.ack_handler = _ack_handler,				       \
		.ack_handler_data = _ack_data,				       \
		.strid = _strid,					       \
		.err_handler = _err_handler,				       \
	}

/** @brief Extern declaration of a group.
 *
 * Can be used in a header file.
 *
 * @param _name  Symbol name of the group.
 */
#define NRF_RPC_GROUP_DECLARE(_name)					       \
	extern const struct nrf_rpc_group _name

/** @brief Register a command decoder.
 *
 * @param _group   Group that the decoder will belong to, created with a
 *                 @ref NRF_RPC_GROUP_DEFINE().
 * @param _name    Name of the decoder.
 * @param _cmd     Command id. Can be from 0 to 254.
 * @param _handler Handler function of type @ref nrf_rpc_handler_t.
 * @param _data    Opaque pointer for the `_handler`.
 */
#define NRF_RPC_CMD_DECODER(_group, _name, _cmd, _handler, _data)	       \
	NRF_RPC_STATIC_ASSERT(_cmd <= 0xFE, "Command out of range");	       \
	NRF_RPC_AUTO_ARR_ITEM(const struct _nrf_rpc_decoder,		       \
			      NRF_RPC_CONCAT(_name, _cmd_dec),		       \
			      "cmd_" NRF_RPC_STRINGIFY(_group),		       \
			      NRF_RPC_STRINGIFY(_name)) = {		       \
		.id = _cmd,						       \
		.handler = _handler,					       \
		.handler_data = _data,					       \
	}

/** @brief Register an event decoder.
 *
 * @param _group   Group that the decoder will belong to, created with a
 *                 @ref NRF_RPC_GROUP_DEFINE().
 * @param _name    Name of the decoder.
 * @param _evt     Event id. Can be from 0 to 254.
 * @param _handler Handler function of type @ref nrf_rpc_handler_t.
 * @param _data    Opaque pointer for the `_handler`.
 */
#define NRF_RPC_EVT_DECODER(_group, _name, _evt, _handler, _data)	       \
	NRF_RPC_STATIC_ASSERT(_evt <= 0xFE, "Event out of range");	       \
	NRF_RPC_AUTO_ARR_ITEM(const struct _nrf_rpc_decoder,		       \
			       NRF_RPC_CONCAT(_name, _evt_dec),		       \
			       "evt_" NRF_RPC_STRINGIFY(_group),	       \
			       NRF_RPC_STRINGIFY(_name)) = {		       \
		.id = _evt,						       \
		.handler = _handler,					       \
		.handler_data = _data,					       \
	}

/** @brief Allocates memory for a packet.
 *
 * Macro may allocate some variables on stack, so it should be used at top level
 * of a function.
 *
 * Memory is automatically deallocated when it is passed to any of the send
 * functions. If not @ref NRF_RPC_DISCARD() can be used.
 *
 * @param[out] _packet Variable of type `uint8_t *` that will hold pointer to
 *                     a newly allocated packet buffer.
 * @param[in]  _len    Requested length of the packet.
 */
#define NRF_RPC_ALLOC(_packet, _len)					       \
	nrf_rpc_tr_alloc_tx_buf(&(_packet), _NRF_RPC_HEADER_SIZE + (_len));    \
	*(uint8_t **)&(_packet) += _NRF_RPC_HEADER_SIZE

/** @brief Deallocate memory for a packet.
 *
 * This macro should be used if memory was allocated, but it will not be send
 * with any of the send functions.
 *
 * @param _packet Packet that was previously allocated.
 */
#define NRF_RPC_DISCARD(_packet) nrf_rpc_tr_free_tx_buf((_packet))

/** @brief Initialize the nRF RPC
 *
 * @param err_handler Error handler that will be called to report error in
 *                    nRF RPC.
 *
 * @return            0 on success or negative error code.
 */
int nrf_rpc_init(nrf_rpc_err_handler_t err_handler);

/** @brief Send a command and provide callback to handle response.
 *
 * @param group        Group that command belongs to.
 * @param cmd          Command id.
 * @param packet       Packet allocated by @ref NRF_RPC_ALLOC and filled with
 *                     an encoded data.
 * @param len          Length of the packet. Can be smaller than allocated.
 * @param handler      Callback that handles the response. In case of error
 *                     (e.g. malformed response packet was received) it is
 *                     undefined if the handler will be called.
 * @param handler_data Opaque pointer that will be passed to `handler`.
 *
 * @return             0 on success or negative error code if a transport layer
 *                     reported a sendig error.
 */
static inline int nrf_rpc_cmd(const struct nrf_rpc_group *group, uint8_t cmd,
			      uint8_t *packet, size_t len,
			      nrf_rpc_handler_t handler, void *handler_data);

/** @brief Send a command and get response as an output parameter.
 *
 * This variant of command send function outputs response as an output
 * parameter. Caller is responsible to call @ref nrf_rpc_decoding_done with
 * a response packet just after response packet was decoded and can be
 * deallocated.
 *
 * @param[in]  group      Group that command belongs to.
 * @param[in]  cmd        Command id.
 * @param[in]  packet     Packet allocated by @ref NRF_RPC_ALLOC and filled
 *                        with an encoded data.
 * @param[in]  len        Length of the packet. Can be smaller than allocated.
 * @param[out] rsp_packet Packet containing the response or NULL on error.
 * @param[out] rsp_len    Length of `rsp_packet`.
 *
 * @return                0 on success or negative error code if a transport
 *                        layer reported a sendig error.
 */
static inline int nrf_rpc_cmd_rsp(const struct nrf_rpc_group *group,
				  uint8_t cmd, uint8_t *packet, size_t len,
				  const uint8_t **rsp_packet, size_t *rsp_len);

/** @brief Send a command, provide callback to handle response and pass any
 * error to an error handler.
 *
 * This variant of command send function returns `void`, so sending error
 * returned from the transport layer is passed to the error handler.
 * Source of error is @ref NRF_RPC_ERR_SRC_SEND.
 *
 * @param group        Group that command belongs to.
 * @param cmd          Command id.
 * @param packet       Packet allocated by @ref NRF_RPC_ALLOC and filled with
 *                     an encoded data.
 * @param len          Length of the packet. Can be smaller than allocated.
 * @param handler      Callback that handles the response. In case of error
 *                     (e.g. malformed response packet was received) it is
 *                     undefined if the handler will be called.
 * @param handler_data Opaque pointer that will be passed to `handler`.
 */
static inline void nrf_rpc_cmd_no_err(const struct nrf_rpc_group *group,
				      uint8_t cmd, uint8_t *packet, size_t len,
				      nrf_rpc_handler_t handler,
				      void *handler_data);

/** @brief Send a command, get response as an output parameter and pass any
 * error to an error handler.
 *
 * See both @ref nrf_rpc_cmd_rsp and @ref nrf_rpc_cmd_no_err for more
 * details on this variant of command send function.
 *
 * @param[in]  group      Group that command belongs to.
 * @param[in]  cmd        Command id.
 * @param[in]  packet     Packet allocated by @ref NRF_RPC_ALLOC and filled
 *                        with an encoded data.
 * @param[in]  len        Length of the packet. Can be smaller than allocated.
 * @param[out] rsp_packet Packet containing the response or NULL on error.
 * @param[out] rsp_len    Length of `rsp_packet`.
 */
static inline void nrf_rpc_cmd_rsp_no_err(const struct nrf_rpc_group *group,
					  uint8_t cmd, uint8_t *packet,
					  size_t len,
					  const uint8_t **rsp_packet,
					  size_t *rsp_len);

/** @brief Send an event.
 *
 * @param group  Group that event belongs to.
 * @param evt    Event id.
 * @param packet Packet allocated by @ref NRF_RPC_ALLOC and filled with
 *               an encoded data.
 * @param len    Length of the packet. Can be smaller than allocated.
 *
 * @return       0 on success or negative error code if a transport layer
 *               reported a sendig error.
 */
int nrf_rpc_evt(const struct nrf_rpc_group *group, uint8_t evt, uint8_t *packet,
		size_t len);

/** @brief Send an event and pass any error to an error handler.
 *
 * This variant of event send function returns `void`, so sending error
 * returned from the transport layer is passed to the error handler.
 * Source of error is @ref NRF_RPC_ERR_SRC_SEND.
 *
 * @param group  Group that event belongs to.
 * @param evt    Event id.
 * @param packet Packet allocated by @ref NRF_RPC_ALLOC and filled with
 *               an encoded data.
 * @param len    Length of the packet. Can be smaller than allocated.
 */
void nrf_rpc_evt_no_err(const struct nrf_rpc_group *group, uint8_t evt,
			uint8_t *packet, size_t len);

/** @brief Send a response.
 *
 * @param packet Packet allocated by @ref NRF_RPC_ALLOC and filled with
 *               encoded data.
 * @param len    Length of the packet. Can be smaller than allocated.
 *
 * @return       0 on success or negative error code if a transport layer
 *               reported a sendig error.
 */
int nrf_rpc_rsp(uint8_t *packet, size_t len);

/** @brief Send a response and pass any error to an error handler.
 *
 * This variant of response send function returns `void`, so sending error
 * returned from the transport layer is passed to the error handler.
 * Source of error is @ref NRF_RPC_ERR_SRC_SEND.
 *
 * @param packet Packet allocated by @ref NRF_RPC_ALLOC and filled with
 *               encoded data.
 * @param len    Length of the packet. Can be smaller than allocated.
 */
void nrf_rpc_rsp_no_err(uint8_t *packet, size_t len);

/** @brief Indicate that decoding of the input packet is done.
 *
 * This function must be called as soon as the input packet was parsed and can
 * be deallocated. It must be called in command decoder, event decoder and after
 * @ref nrf_rpc_cmd_rsp or @ref nrf_rpc_cmd_rsp_no_err. Packet is
 * automatically deallocated after completetion of the response handler
 * function, so this `nrf_rpc_decoding_done` is not needed in response handler.
 *
 * @param packet Packet which parsing has completed.
 */
void nrf_rpc_decoding_done(const uint8_t *packet);

/** @brief Report an error to nRP PRC error handler.
 *
 * Main intention for exposing this function is to allow serialization layer
 * (e.g. TinyCBOR) report error to the same error handler as entire nRF RPC.
 *
 * @param code        Negative error code.
 * @param src         Source of error.
 * @param group       Group where error happent or NULL if unknown.
 * @param id          Command or event id which caused the error or
 *                    @ref NRF_RPC_ID_UNKNOWN if unknown.
 * @param packet_type Type of packet related to this error.
 */
void nrf_rpc_err(int code, enum nrf_rpc_err_src src,
		 const struct nrf_rpc_group *group, uint8_t id,
		 uint8_t packet_type);

/* Inline definitions. */

static inline int nrf_rpc_cmd(const struct nrf_rpc_group *group, uint8_t cmd,
			      uint8_t *packet, size_t len,
			      nrf_rpc_handler_t handler, void *handler_data)
{
	int nrf_rpc_cmd_common(const struct nrf_rpc_group *group, uint32_t cmd,
			       uint8_t *packet, size_t len, void *ptr1,
			       void *ptr2);

	return nrf_rpc_cmd_common(group, cmd, packet, len, handler,
				  handler_data);
}

static inline int nrf_rpc_cmd_rsp(const struct nrf_rpc_group *group,
				  uint8_t cmd, uint8_t *packet, size_t len,
				  const uint8_t **rsp_packet, size_t *rsp_len)
{
	int nrf_rpc_cmd_common(const struct nrf_rpc_group *group, uint32_t cmd,
			       uint8_t *packet, size_t len, void *ptr1,
			       void *ptr2);

	return nrf_rpc_cmd_common(group, cmd | 0x10000, packet, len, rsp_packet,
				  rsp_len);
}

static inline void nrf_rpc_cmd_no_err(const struct nrf_rpc_group *group,
				      uint8_t cmd, uint8_t *packet, size_t len,
				      nrf_rpc_handler_t handler,
				      void *handler_data)
{
	void nrf_rpc_cmd_common_no_err(const struct nrf_rpc_group *group,
				       uint32_t cmd, uint8_t *packet,
				       size_t len, void *ptr1, void *ptr2);

	nrf_rpc_cmd_common_no_err(group, cmd, packet, len, handler,
				  handler_data);
}

static inline void nrf_rpc_cmd_rsp_no_err(const struct nrf_rpc_group *group,
					  uint8_t cmd, uint8_t *packet,
					  size_t len,
					  const uint8_t **rsp_packet,
					  size_t *rsp_len)
{
	void nrf_rpc_cmd_common_no_err(const struct nrf_rpc_group *group,
				       uint32_t cmd, uint8_t *packet,
				       size_t len, void *ptr1, void *ptr2);

	nrf_rpc_cmd_common_no_err(group, cmd | 0x10000, packet, len, rsp_packet,
				  rsp_len);
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* _NRF_RPC_H_ */
