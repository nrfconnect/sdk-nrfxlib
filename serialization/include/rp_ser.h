/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef RP_SER_H_
#define RP_SER_H_

#include <stdint.h>

#include <cbor.h>

#include <rp_errors.h>
#include <rp_common.h>
#include <rp_trans.h>

/**
 * @file
 * @defgroup rp_ser Remote Procedures Serialization core
 * @{
 * @brief Remote Procedures Serialization core API.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**@brief Serialization packet type.*/
enum rp_ser_packet_type {
	/** Serialization command packet. */
	RP_SER_PACKET_TYPE_CMD          = 0x01,

	/** Serialization event packet. */
	RP_SER_PACKET_TYPE_EVENT,

	/** Serialization command response packet. */
	RP_SER_PACKET_TYPE_RSP,

	/** Serialization transport reserved packet. */
	RP_SER_PACKET_TRANSPORT_RESERVED = 128,

	/** Serialization upper bound. */
	RP_SER_PACKER_TYPE_MAX           = 255
};

/**@brief Command and response decoder handler type. */
typedef rp_err_t (*cmd_handler_t)(CborValue *it);

/**@brief Event decoder handler type. */
typedef rp_err_t (*evt_handler_t)(uint8_t evt, CborValue *it);

/**@brief Encoder buffer structure. */
struct rp_ser_buf {
	/** Encoder buffer. */
	uint8_t *buf;

	/** Encoder buffer size. */
	size_t size;

	/** Current packet size. */
	size_t packet_size;
};

/**@brief Command decoder structure. */
struct rp_ser_cmd {
	/** Command code. */
	uint8_t cmd;

	/** Command decoder. */
	cmd_handler_t func;
};

/**@brief Event decoder structure. */
struct rp_ser_evt {
	/** Event code. */
	uint8_t evt;

	/** Event decoder. */
	evt_handler_t func;
};

/**@brief Commands and events decoder structure. */
struct rp_ser_decoders {
	/** Command section start address. */
	const struct rp_ser_cmd *cmd_begin;

	/** Command section end address. */
	const struct rp_ser_cmd *cmd_end;

	/** Event section start address. */
	const struct rp_ser_evt *evt_begin;

	/** Event section end address. */
	const struct rp_ser_evt *evt_end;
};

/**@brief Helper macro for creating command decoder. All comands decoders have to be assigned
 *        to proper Remote Procedure Serialization instance. After receiving a command, the
 *        command decoder function is searching in command data memory section based on command
 *        number and calls the actual decoder.
 *
 * @param[in] _rp_inst Remote Procedure Serialization instance.
 * @param[in] _name Command decoder name.
 * @param[in] _cmd Command number.
 * @param[in] _handler Command decoder function @ref cmd_handler_t.
 */
#define RP_SER_CMD_DECODER(_rp_inst, _name, _cmd, _handler)         \
	RP_STATIC_ASSERT(&_rp_inst != NULL, "Invalid instance");    \
	RP_STATIC_ASSERT(_cmd <= 0xFF, "Command out of range");     \
	const struct rp_ser_cmd RP_CONCAT(_name, _cmd_dec) __used   \
	__attribute__((__section__("." RP_STRINGIFY(rp_ser_decoder) \
				   "." "cmd"                        \
				   "." RP_STRINGIFY(_rp_inst)       \
				   "." RP_STRINGIFY(_name)))) = {   \
		.cmd = _cmd,					    \
		.func = _handler				    \
	}

/**@brief Helper macro for creating event decoder. All events decoders have to be assigned
 *        to proper Remote Procedure Serialization instance. After receiving a event the
 *        event decoder function is searching in command data memory section based on command
 *        number and called.
 *
 * @param[in] _rp_inst Remote Procedure Serialization instance.
 * @param[in] _name Event decoder name.
 * @param[in] _cmd Event number.
 * @param[in] _handler Event decoder function @ref cmd_handler_t.
 */
#define RP_SER_EVT_DECODER(_rp_inst, _name, _evt, _handler)         \
	RP_STATIC_ASSERT(&_rp_inst != NULL, "Invalid instance");    \
	RP_STATIC_ASSERT(_evt <= 0xFF, "Event out of range");       \
	const struct rp_ser_evt RP_CONCAT(_name, _evt_dec) __used   \
	__attribute__((__section__("." RP_STRINGIFY(rp_ser_decoder) \
				   "." "evt"                        \
				   "." RP_STRINGIFY(_rp_inst)       \
				   "." RP_STRINGIFY(_name)))) = {   \
		.evt = _evt                                         \
		.func = _handler			            \
	}

/**@brief Endpoint configuration structure. */
struct rp_ser_endpoint {
	/** Endpoint number. */
	int number;
};

/**@brief Remote Procedure Serialization instance. */
struct rp_ser {
	/** Transport endpoint instance. */
	struct rp_trans_endpoint endpoint;

	/** Decoders section addresses. */
	const struct rp_ser_decoders *decoders;

	/** Transport endpoint initial configuration. */
	const struct rp_ser_endpoint *ep_conf;

	/** Pointer to OS signal mechanism. */
	void *rp_sem;

	/** Current processing command response decoder. */
	cmd_handler_t rsp_handler;
};

/**@brief Macro for defining the Remote Procedure Serialization instance.
 *
 * @param[in] _name Instance name.
 * @param[in] _lock_type OS lock data type. For example in Zephyr struct k_sem can be used.
 * @param[in] _endpoint_num Endpoint number, used for transport endpoint identification.
 * @param[in] _endpoint_stack_size Endpoint thread stack size.
 * @param[in] _endpoint_thread_prio Endpoint thread priority.
 */
#define RP_SER_DEFINE(_name, _lock_type, _endpoint_num,                           \
		      _endpoint_stack_size, _endpoint_thread_prio)                \
	/* Helper variables used to specify start and end addresses of specific   \
	 * subsection instance event and command decoders data. The section must  \
	 * be sorted in alphabetical order to ensure the valid value.             \
	 */                                                                       \
	const struct rp_ser_cmd RP_CONCAT(_name, _cmd_begin) __used               \
	__attribute__((__section__("." RP_STRINGIFY(rp_ser_decoder)               \
				   "." "cmd" "." RP_STRINGIFY(_name) ".")));      \
		const struct rp_ser_cmd RP_CONCAT(_name, _cmd_end) __used         \
	__attribute__((__section__("." RP_STRINGIFY(rp_ser_decoder)               \
				   "." "cmd" "." RP_STRINGIFY(_name) "." "}")));  \
		const struct rp_ser_evt RP_CONCAT(_name, _evt_begin) __used       \
	__attribute__((__section__("." RP_STRINGIFY(rp_ser_decoder)               \
				   "." "evt" "." RP_STRINGIFY(_name) ".")));      \
		const struct rp_ser_evt RP_CONCAT(_name, _evt_end) __used         \
	__attribute__((__section__("." RP_STRINGIFY(rp_ser_decoder)               \
				   "." "evt" "." RP_STRINGIFY(_name) "." "}")));  \
                                                                                  \
	static _lock_type RP_CONCAT(_name, _sem);                                 \
	static const struct rp_ser_endpoint RP_CONCAT(_name, _ep) = {             \
		.number = _endpoint_num,                                          \
	};                                                                        \
										  \
	RP_TRANS_ENDPOINT_PREPARE(RP_CONCAT(_name, _ep),                          \
					    _endpoint_stack_size,                 \
					    _endpoint_thread_prio);               \
										  \
	const struct rp_ser_decoders RP_CONCAT(_name, _decoders) = {		  \
		.cmd_begin = (&RP_CONCAT(_name, _cmd_begin) + 1),		  \
		.cmd_end = &RP_CONCAT(_name, _cmd_end),			          \
		.evt_begin = (&RP_CONCAT(_name, _evt_begin) + 1),		  \
		.evt_end = &RP_CONCAT(_name, _evt_end)				  \
	};									  \
										  \
	struct rp_ser _name = {							  \
		.endpoint = RP_TRANS_ENDPOINT_INITIALIZER(RP_CONCAT(_name, _ep)), \
		.decoders = &RP_CONCAT(_name, _decoders),			  \
		.ep_conf = &RP_CONCAT(_name, _ep),				  \
		.rp_sem = (void *)&RP_CONCAT(_name, _sem),			  \
	}

/**@brief Macro for declaring a serialization instance (not creating it)
 *
 * Serialization which are split up over multiple files must have exactly
 * one file use @ref RP_SER_DEFINE to create module-specific state
 * and register the decoders data section.
 *
 * The other serialization files which could share the same instance should
 * use this macro instead to creating the new one.
 *
 * @param[in] _name Exiting instance name.
 */
#define RP_SER_DECLARE(_name) \
	extern struct rp_ser _name

/**@brief Function for initializing the Remote Procedure Serialization instance.
 *
 * This function initializes Remote Procedure Serialization instance, and creates a new
 * tranport endpoint for it. Multiple instances can be initialized during runtime. This ensures
 * parallel remote function calls.
 *
 * @param[in] rp Remote procedure instance.
 *
 * @retval RP_SUCCESS Initialization was successful.
 * @retval RP_ERROR_NULL A parameter was NULL.
 */
rp_err_t rp_ser_init(struct rp_ser *rp);

/**@brief Function for uninitializing the Remote Procedure Serialization
 *        instance.
 *
 * This function uninitializes the Remote Procedure instance.
 *
 * @param[in] rp Remote Procedure instance.
 */
void rp_ser_uninit(struct rp_ser *rp);

/**@brief Function for sending command(function call) to the Remote processor.
 *
 * This function sends a procedure call to the remote processor and waits for response
 * * a specified amount of time if rsp is not NULL. After receiving command response
 * the response decoder is called and received data can be returned.
 *
 * @param[in] rp Remote Procedure Serialization instance.
 * @param[in] encoder Remote Procedure instance encoder.
 * @param[in] rsp Command response decoder. If not NULL, function waits for response and
 *                decodes it using this handler.
 *
 * @retval RP_SUCCESS Command send was successful.
 * @retval RP_ERROR_NULL A parameter was NULL.
 * @retval RP_ERROR_INVALID_PARAM A serialization packet length was 0.
 */
rp_err_t rp_ser_cmd_send(struct rp_ser *rp,
			 struct rp_ser_buf *rp_buf,
			 CborEncoder *encoder,
			 cmd_handler_t rsp);

/**@brief Function for sending event to the Remote processor.
 *
 * This function sends an event to the remote processor. Event is asynchronous and
 * receiving it is not confirmed by the Remote processor. Event can be used in case
 * when remote function call doesn't return any data or the remote processor gets an
 * event which should be transported to the local processor it is needed to pass it
 * to other processor.
 *
 * @param[in] rp Remote Procedure Serialization instance.
 * @param[in] encoder Event encoder.
 *
 * @retval RP_SUCCESS Command send was successful.
 * @retval RP_ERROR_NULL A parameter was NULL.
 * @retval RP_ERROR_INVALID_PARAM A serialization packet length was 0.
 */
rp_err_t rp_ser_evt_send(struct rp_ser *rp, struct rp_ser_buf *rp_buf,
			 CborEncoder *encoder);

/**@brief Function for sending command response to the Remote processor.
 *
 * This function sends response after the received command was processed.
 *
 * @param[in] rp Remote Procedure Serialization instance.
 * @param[in] encoder Response encoder.
 *
 * @retval RP_SUCCESS Command send was successfull.
 * @retval RP_ERROR_NULL A parameter was NULL.
 * @retval RP_ERROR_INVALID_PARAM A serialization packet length was 0.
 */
rp_err_t rp_ser_rsp_send(struct rp_ser *rp, struct rp_ser_buf *rp_buf,
			 CborEncoder *encoder);

/**@brief Function for initializing the Remote procedure command packet.
 *
 * This function initializes the command packet. Should be used
 * after allocation of the buffer for encoding the Remote procedure.
 * It creates the packet header data in provided buffer and initializes
 * serializator.
 *
 * @note Command packet needs three bytes from rp_buf for its
 *       internal data.
 *
 * @param[in, out] rp_buf Remote Procedure buffer
 * @param[in, out] encoder Remote Procedure TinyCBOR encoder data.
 * @param[in] cmd Command number.
 *
 * @retval RP_SUCCESS The Remote Procedure initialization was successful.
 * @retval RP_ERROR_NULL A parameter was NULL.
 * @retval RP_ERROR_NO_MEMORY Too small buffer. Packet header data cannot be encoded.
 */
rp_err_t rp_ser_cmd_init(struct rp_ser_buf *rp_buf, CborEncoder *encoder,
			 uint8_t cmd);

/**@brief Function for initializing the Remote procedure event packet.
 *
 * This function initializes the event packet. Should be used
 * after allocation of the buffer for encoding the Remote procedure.
 * It creates the packet header data in provided buffer and initializes
 * serializator.
 *
 * @note Event packet needs three bytes from rp_buf for its
 *       internal data.
 *
 * @param[in, out] rp_buf Remote Procedure buffer
 * @param[in, out] encoder Remote Procedure TinyCBOR encoder data.
 * @param[in] evt Event number.
 *
 * @retval RP_SUCCESS The Remote Procedure initialization was successful.
 * @retval RP_ERROR_NULL A parameter was NULL.
 * @retval RP_ERROR_NO_MEMORY Too small buffer. Packet header data cannot be encoded.
 */
rp_err_t rp_ser_evt_init(struct rp_ser_buf *rp_buf, CborEncoder *encoder,
			 uint8_t evt);

/**@brief Function for initializing the Remote procedure response packet.
 *
 * This function initializes the response packet. Should be used
 * after allocation of the buffer for encoding the Remote procedure.
 * It creates the packet header data in provided buffer and initializes
 * serializator.
 *
 * @note Response packet needs two bytes from rp_buf for its
 *       internal data.
 *
 * @param[in, out] rp_buf Remote Procedure buffer
 * @param[in, out] encoder Remote Procedure TinyCBOR encoder data.
 *
 * @retval RP_SUCCESS The Remote Procedure initialization was successful.
 * @retval RP_ERROR_NULL A parameter was NULL.
 * @retval RP_ERROR_NO_MEMORY Too small buffer. Packet header data cannot be encoded.
 */
rp_err_t rp_ser_rsp_init(struct rp_ser_buf *rp_buf, CborEncoder *encoder);

/**@brief Define the rp_ser_buf stack variable and allocate Remote Procedure
 *        buffer. Every remote procedure needs to alloc the buffer for
 *        encoded data.
 *
 * @param[in] rp The Remote Procedure serialization instance.
 * @param[in, out] encoder Remote Procedure encoder data.
 * @param[in, out] size Requested buffer size as input, allocated buffer size as output.
 *
 */
#define rp_ser_buf_alloc(_rp_buf_name, _rp, _size)			              \
	uint8_t *RP_CONCAT(_rp_buf_name, _buf);                                       \
	rp_trans_alloc_tx_buf(&_rp.endpoint, &RP_CONCAT(_rp_buf_name, _buf), &_size); \
	struct rp_ser_buf _rp_buf_name = {                                            \
		.buf = RP_CONCAT(_rp_buf_name, _buf),                                 \
		.size = _size,                                                        \
		.packet_size = 0                                                      \
	}

/**@brief Macro for releasing the allocated buffer.
 *        It can be used in case of error in the Remote Procedure Serialization.
 *
 * @param[in] rp The Remote Procedure Serialization instance.
 * @param[in] buf Pointer to currently used buffer.
 */
#define rp_ser_buf_free(rp, rp_buf) \
	rp_trans_free_tx_buf(rp.endpoint, rp_buf.buf)

#ifdef __cplusplus
}
#endif

/**
 *@}
 */

#endif /* RP
 * @brief Remote procedures OS specific API_SER_H_ */
