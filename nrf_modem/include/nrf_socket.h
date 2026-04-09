/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file nrf_socket.h
 *
 * @defgroup nrf_socket Modem library socket API
 * @{
 * @brief Application interface for nRF sockets.
 */

#ifndef NRF_SOCKET_H__
#define NRF_SOCKET_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
	#ifdef __GNUC__
		/* GNU C++ compiler supports restrict as __restrict__ */
		#define restrict __restrict__
	#else
		/* Other C++ compilers may not support restrict */
		#define restrict
	#endif
#endif

/* ssize_t is not part of the C library, but is defined by POSIX in <sys/types.h> */
#ifdef __has_include
	#if __has_include(<sys/types.h>)
		#include <sys/types.h>
	#else
		/* <sys/types.h> missing, define ssize_t here */
		typedef int32_t ssize_t;
	#endif
#else
	/* __has_include() not supported, rely on build-system macro */
	#ifdef NRF_MODEM_HAS_SYS_TYPES
		#include <sys/types.h>
	#else
		typedef int32_t ssize_t;
	#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Maximum number of IP sockets.
 * @addtogroup nrf_modem_limits
 */
#define NRF_MODEM_MAX_SOCKET_COUNT 8

/**
 * @addtogroup nrf_socket_address_resolution
 * @{
 */

/** @brief Host to network byte-orders on half word. */
#define NRF_HTONS(x) ((uint16_t) ((((x) & 0xff00) >> 8) | (((x) & 0x00ff) << 8)))

/** @brief Host to network byte-orders on full word. */
#define NRF_HTONL(x) ((uint32_t) ((((x) & 0xff000000) >> 24)	| \
				  (((x) & 0x00ff0000) >> 8)	| \
				  (((x) & 0x0000ff00) << 8)	| \
				  (((x) & 0x000000ff) << 24)))

/** @brief Network to host byte-orders on half word. */
#define NRF_NTOHS(x) NRF_HTONS(x)

/** @brief Network to host byte-orders on full word. */
#define NRF_NTOHL(x) NRF_HTONL(x)

/** @brief Convert byte order from host to network (short). */
#define nrf_htons(x) NRF_HTONS(x)

/** @brief Convert byte order from host to network (long). */
#define nrf_htonl(x) NRF_HTONL(x)

/** @brief Convert byte order from network to host (short). */
#define nrf_ntohs(x) NRF_NTOHS(x)

/** @brief Convert byte order from network to host (long). */
#define nrf_ntohl(x) NRF_NTOHL(x)

/** @brief Maximum length of IPv4 in string form, including null-termination character. */
#define NRF_INET_ADDRSTRLEN 16

/** @brief Maximum length of IPv6 in string form, including null-termination character. */
#define NRF_INET6_ADDRSTRLEN 46

/** @} */

/**
 * @defgroup nrf_socket_api_enumerators Socket enumerators
 * @brief Enumerated values that is used as input arguments to multiple socket functions.
 * @{
 */

/**
 * @defgroup nrf_socket_families Socket family
 * @{
 */
/** Unspecified address family */
#define NRF_AF_UNSPEC 0
/** IPv4 socket family. */
#define NRF_AF_INET 1
/** IPv6 socket family. */
#define NRF_AF_INET6 2
/** Raw packet family. */
#define NRF_AF_PACKET 3
/** @} */

/**
 * @defgroup nrf_socket_types Socket type
 * @{
 */
/** TCP socket type. */
#define NRF_SOCK_STREAM 1
/** UDP socket type. */
#define NRF_SOCK_DGRAM 2
/** RAW socket type. */
#define NRF_SOCK_RAW 3
/** @} */

/**
 * @defgroup nrf_socket_protocols Socket protocols
 * @{
 * @brief Protocol numbers from IANA/BSD.
 */

/** IP protocol (pseudo-val for setsockopt()) */
#define NRF_IPPROTO_IP 0
/** TCP protocol. */
#define NRF_IPPROTO_TCP 6
/** UDP protocol. */
#define NRF_IPPROTO_UDP 17
/** IPv6 protocol (pseudo-val for setsockopt()) */
#define NRF_IPPROTO_IPV6 41
/** Raw IP packets */
#define NRF_IPPROTO_RAW 255
/** IPv4 and IPv6 protocol (pseudo-val for setsockopt()) */
#define NRF_IPPROTO_ALL 512

/** TLS1v2 protocol. */
#define NRF_SPROTO_TLS1v2 258
/** DTLS1v2 protocol. */
#define NRF_SPROTO_DTLS1v2 273

/** @} */

/** @} */ /* nrf_socket_api_enumerators */

/**
 * @defgroup nrf_socket_tls TLS socket
 * @brief TLS socket API.
 * @{
 */

/**
 * @brief Maximum TLS message size in bytes.
 */
#define NRF_SOCKET_TLS_MAX_MESSAGE_SIZE 2048

/**
 * @brief Maximum number of security tags that can be associated with a socket.
 */
#define NRF_SOCKET_TLS_MAX_SEC_TAG_LIST_SIZE  7

/**
 * @brief Select an array of security tags to use for credentials when connecting.
 *
 * This option accepts a list of @ref nrf_sec_tag_t values.
 * The option length is the size in bytes of the array of security tags.
 * Passing \c NULL as an option value and \c 0 as an option length removes all security
 * tags associated with a socket.
 *
 * By default, no security tags are associated with a socket.
 * The maximum number of security tags is given by the @ref NRF_SOCKET_TLS_MAX_SEC_TAG_LIST_SIZE
 * macro.
 */
#define NRF_SO_SEC_TAG_LIST 1

/**
 * @brief Configure the hostname used for peer verification.
 *
 * Configure a hostname to check against during TLS handshakes.
 *
 * The option value is a null-terminated string containing the host name to verify against.
 * Passing \c NULL as an option value and \c 0 as an option length disables peer hostname
 * verification.
 *
 * By default, peer hostname verification is disabled.
 */
#define NRF_SO_SEC_HOSTNAME 2

/**
 * @brief Select which cipher suites are allowed to be used during the TLS handshake.
 *
 * This option accepts a prioritized array of selected cipher suites. The cipher suites are
 * identified by their IANA assigned values. See @ref nrf_socket_tls_cipher_suites for a list of
 * allowed values. By default, all supported cipher suites are allowed.
 *
 * For more information, see the release notes for the respective modem firmware products available
 * in the downloads sections, for example, the [nRF9151 downloads section]
 * (https://www.nordicsemi.com/Products/nRF9151/Download?lang=en#infotabs)
 *
 */
#define NRF_SO_SEC_CIPHERSUITE_LIST 3

/**
 * @brief Retrieve the cipher suite used during the TLS/DTLS handshake (Read-only).
 *
 * @note This is only supported by the following modem firmware:
 *       - mfw_nrf91x1
 *       - mfw_nrf9151-ntn
 */
#define NRF_SO_SEC_CIPHERSUITE_USED 4

/**
 * @brief Configure peer verification level.
 *
 * See @ref nrf_socket_sec_peer_verify_options for a list of allowed values.
 *
 * The default value is @ref NRF_SO_SEC_PEER_VERIFY_OPTIONAL (optional).
 *
 * @note If a TLS/DTLS protocol is used, there must be at least one root CA in the modem credential
 *       storage, regardless if the @ref NRF_SO_SEC_PEER_VERIFY value is
 *       @ref NRF_SO_SEC_PEER_VERIFY_NONE or @ref NRF_SO_SEC_PEER_VERIFY_OPTIONAL.
 */
#define NRF_SO_SEC_PEER_VERIFY 5

/**
 * @brief Configure the role for the connection.
 *
 * See @ref nrf_socket_sec_roles for a list of allowed values.
 *
 * The default value is @ref NRF_SO_SEC_ROLE_CLIENT (client).
 */
#define NRF_SO_SEC_ROLE 6

/**
 * @brief Enable/disable TLS session caching.
 *
 * See @ref nrf_socket_session_cache_options for a list of allowed values.
 *
 * The default value is @ref NRF_SO_SEC_SESSION_CACHE_DISABLED (disabled)
 *
 * @note Session cache may not be used if the peer does not support it.
 */
#define NRF_SO_SEC_SESSION_CACHE 12

/**
 * @brief Purge TLS session cache (write-only).
 *
 * This socket option accepts any value and immediately deletes the TLS session cache.
 */
#define NRF_SO_SEC_SESSION_CACHE_PURGE 13

/**
 * @brief Configure the DTLS handshake timeout value.
 *
 * See @ref nrf_socket_so_sec_handshake_timeouts for allowed values.
 *
 * The default value is @ref NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_NONE (no timeout).
 */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEO 14

/**
 * @brief Enable/disable the DTLS connection ID.
 *
 * This socket option decides whether the modem requests or accepts a DTLS connection ID when
 * performing the server handshake.
 *
 * See @ref nrf_so_sec_dtls_cid_settings for allowed values.
 *
 * The default value is @ref NRF_SO_SEC_DTLS_CID_DISABLED (disabled).
 *
 * @note This is only supported by the following modem firmware:
 *       - mfw_nrf9160 v1.3.5 or later
 *       - mfw_nrf91x1
 *       - mfw_nrf9151-ntn
 */
#define NRF_SO_SEC_DTLS_CID 15

/**
 * @brief Get the DTLS connection ID status (read-only).
 *
 * The status tells whether the connection ID is used in the current connection and in which
 * direction it is used.
 *
 * See @ref nrf_so_sec_dtls_cid_statuses for allowed values.
 *
 * @note This is only supported by the following modem firmware:
 *       - mfw_nrf9160 v1.3.5 or later
 *       - mfw_nrf91x1
 *       - mfw_nrf9151-ntn
 */
#define NRF_SO_SEC_DTLS_CID_STATUS 16

/**
 * @brief Save DTLS connection (Write-only).
 *
 * You can use this socket option to pause a session that is not frequently used by the
 * application. Saving the session frees up memory in the modem (The modem serializes and
 * compresses the socket). This frees up memory that can be used for other connections.
 *
 * If the socket is closed, the saved DTLS data is cleaned and the connection with the server is
 * lost.
 *
 * This option require a DTLS v1.2 connection with renegotiation disabled.
 *
 * After the socket option is successfully called, you must call @ref NRF_SO_SEC_DTLS_CONN_LOAD
 * before continuing to communicate on the socket.
 *
 * This option can fail with different errno values:
 *
 * | errno         | Condition                               | Notes                               |
 * |---------------|-----------------------------------------|-------------------------------------|
 * | \c NRF_EAGAIN | Error during SSL context serialization. | The SSL context is still present    |
 * |               | This can happen if the modem cannot     | in the socket, so data sending      |
 * |               | allocate enough memory or if the socket | is still possible.                  |
 * |               | is busy sending or receiving data.      |                                     |
 * |               |                                         |                                     |
 * | \c NRF_EINVAL | The socket option is not supported      | Examples: DTLS handshake not        |
 * |               | with the current configuration.         | completed; connection is not a      |
 * |               |                                         | DTLS 1.2 connection with            |
 * |               |                                         | renegotiation disabled; or          |
 * |               |                                         | connection does not use an AEAD     |
 * |               |                                         | cipher suite (AES-CCM or AES-GCM).  |
 * | \c NRF_ENOMEM | The number of saved connections         |                                     |
 * |               | exceeds four.                           |                                     |
 *
 * @note This is only supported by the following modem firmware:
 *       - mfw_nrf9160 v1.3.5 or later
 *       - mfw_nrf91x1
 *       - mfw_nrf9151-ntn
 */
#define NRF_SO_SEC_DTLS_CONN_SAVE 17

/**
 * @brief load DTLS connection (Write-only)
 *
 * This socket option can fail with different errno values:
 *
 * | errno         | Condition                                 | Notes                             |
 * |---------------|-------------------------------------------|-----------------------------------|
 * | \c NRF_EAGAIN | Error during SSL context deserialization. | This can happen if the modem      |
 * |               |                                           | cannot allocate enough memory     |
 * |               |                                           | or the connection is not saved.   |
 * | \c NRF_EINVAL | The socket option is not supported with   |                                   |
 * |               | the current configuration.                |                                   |
 *
 * @note This is only supported by the following modem firmware:
 *       - mfw_nrf9160 v1.3.5 or later
 *       - mfw_nrf91x1
 *       - mfw_nrf9151-ntn
 */
#define NRF_SO_SEC_DTLS_CONN_LOAD 18

/**
 * @brief Get the end status of the last completed TLS/DTLS handshake procedure (Read-only).
 *
 * See @ref nrf_so_sec_handshake_statuses for allowed values.
 *
 * The default value is @ref NRF_SO_SEC_HANDSHAKE_STATUS_FULL (full handshake).
 *
 * @note This is only supported by the following modem firmware:
 *       - mfw_nrf91x1
 *       - mfw_nrf9151-ntn
 */
#define NRF_SO_SEC_HANDSHAKE_STATUS 19

/**
 * @brief Enable/disable the DTLS fragmentation extension.
 *
 * See @ref nrf_so_sec_dtls_frag_ext_options for allowed values.
 *
 * The default value is @ref NRF_SO_SEC_DTLS_FRAG_EXT_DISABLED (disabled).
 *
 * @note This is only supported by the following modem firmware:
 *       - mfw_nrf91x1 v2.0.4 or later
 *       - mfw_nrf9151-ntn
 */
#define NRF_SO_SEC_DTLS_FRAG_EXT 20
/** @} */

/**
 * @defgroup nrf_socket_options_sockets Generic socket options
 * @brief Socket options used with IP sockets.
 * @ingroup nrf_socket
 * @{
 */

/**
 * @brief Enable reuse of server addresses (write-only).
 *
 * This socket options permits the reuse of local addresses by using the @ref nrf_bind function
 * (protocol-specific).
 *
 * Set to any non-zero value to enable, or to \c 0 to disable.
 * The default value is \c 0 (OFF).
 */
#define NRF_SO_REUSEADDR 2

/**
 * @brief Request and clear pending error information on the socket (read-only).
 *
 * When this option is specified, @ref nrf_getsockopt returns any pending errors on the socket and
 * clears the error status.
 * It returns a \c 0 value if there is no pending error.
 *
 * This option can be used to check for asynchronous errors on connected connectionless-mode
 * sockets or for other types of asynchronous errors.
 *
 * This option has no default value.
 */
#define NRF_SO_ERROR 4

/**
 * @brief Timeout value for a socket receive and accept operations.
 *
 * Set a timeout value for the @ref nrf_recv, @ref nrf_recvfrom and @ref nrf_accept operations.
 * This option accepts an @ref nrf_timeval structure with a number of seconds and microseconds
 * specifying the limit on how long to wait for an input operation to complete.
 *
 * If a receive operation has blocked for this much time without receiving additional data, it
 * returns with a partial count, or \c errno is set to @ref NRF_EAGAIN or @ref NRF_EWOULDBLOCK if
 * no data were received.
 *
 * If an accept operation has blocked for this much time without receiving an incoming connection,
 * it returns \c -1 and \c errno is set to @ref NRF_EAGAIN.
 *
 * The default value is \c 0 (no timeout).
 *
 * @note The minimum supported resolution is 1 millisecond.
 */
#define NRF_SO_RCVTIMEO 20

/**
 * @brief Timeout value for a socket send operation.
 *
 * Set a timeout value for the @ref nrf_send, @ref nrf_sendto and @ref nrf_accept operations.
 * This option accepts an @ref nrf_timeval structure with a number of seconds and microseconds
 * specifying the limit on how long to wait for an input operation to complete.
 *
 * If a send operation has blocked for this much time without sending additional data, it returns
 * with a partial count, or \c errno is set to @ref NRF_EAGAIN or @ref NRF_EWOULDBLOCK if no data
 * were sent.
 *
 * The default value is \c 0 (no timeout).
 *
 * @note The minimum supported resolution is 1 millisecond.
 */
#define NRF_SO_SNDTIMEO 21

/**
 * @brief Disable ICMP echo replies on both IPv4 and IPv6.
 *
 * Set to \c 1 to enable, or to \c 0 to disable.
 *
 * The default value is \c 0 (OFF).
 */
#define NRF_SO_SILENCE_ALL 30

/**
 * @brief Enable ICMP echo replies on IPv4.
 *
 * Set to \c 1 to enable, or to \c 0 to disable.
 *
 * The default is \c 1 (ON).
 */
#define NRF_SO_IP_ECHO_REPLY 31

/**
 * @brief Enable ICMP echo replies on IPv6.
 *
 * Set to \c 1 to enable, or to \c 0 to disable.
 *
 * The default is \c 1 (ON).
 */
#define NRF_SO_IPV6_ECHO_REPLY 32
/**
 * @brief Send data on a socket as part of an exceptional event.
 *
 * Before using this socket option, the PDN associated with the socket must be configured to
 * allow exceptional events by using the \c AT%EXCEPTIONALDATA AT command.
 * For more information about the \c AT%EXCEPTIONALDATA AT command, see the
 * [nRF91x1 AT Commands Reference Guide]
 * (https://docs.nordicsemi.com/bundle/ref_at_commands_nrf91x1).
 *
 * @note Exceptional events are described in the 3GPP Release 14 specification. This feature
 *       requires network support.
 *
 * @note This is only supported by the following modem firmware:
 *       - mfw_nrf91x1
 *       - mfw_nrf9151-ntn
 */
#define NRF_SO_EXCEPTIONAL_DATA 33
/**
 * @brief Keep the socket open when its PDN connection is lost, or the device is set to flight
 *        mode.
 *
 * This option keeps a socket from being closed upon PDN disconnection or reactivation events,
 * or when the device is set to flight mode (\c +CFUN=4). Until the PDN connection is
 * reestablished, the socket is not functional.
 *
 * Output operations, such as the functions @ref nrf_send and @ref nrf_connect return an error and
 * set \c errno to @ref NRF_ENETUNREACH.
 *
 * Input operations, such as the functions @ref nrf_recv and @ref nrf_accept block, since no data
 * can be received, or return an error if the socket or the operation is non-blocking.
 *
 * Upon PDN connection reestablishment, the socket behavior depends on the socket type and protocol
 * and on the IP address of the socket's newly established PDN connection, as shown in the
 * following table:
 *
 * | Socket type     | Socket protocol      | Socket is functional (no errors)                     |
 * |-----------------|----------------------|------------------------------------------------------|
 * | NRF_SOCK_DGRAM  | NRF_IPPROTO_UDP      | Always                                               |
 * | NRF_SOCK_DGRAM  | NRF_SPROTO_DTLS1v2   | If using DTLS connection ID                          |
 * | NRF_SOCK_STREAM | NRF_IPPROTO_TCP      | If the socket PDN IP address has not changed         |
 * | NRF_SOCK_STREAM | NRF_SPROTO_TLS1v2    | If the socket PDN IP address has not changed         |
 * | NRF_SOCK_RAW    | Any                  | Always                                               |
 *
 * If the conditions to keep the socket open after PDN connection reestablishment are not met, the
 * socket will report an error (and set \c errno to @ref NRF_ENETDOWN), and must be closed by the
 * application. Otherwise, the socket is functional, and the application can use it.
 *
 * For further information on how to detect and handle these errors, see
 * [Handling PDN errors on sockets]
 * (https://docs.nordicsemi.com/bundle/ncs-latest/page/nrfxlib/nrf_modem/doc/sockets/pdn.html
 * #handling_pdn_errors_on_sockets)
 *
 * @note Putting the device into functional mode \c 0 (\c +CFUN=0) always forces all sockets to be
 *       closed, regardless of the @ref NRF_SO_KEEPOPEN socket option.
 *
 * @note This is only supported by the following modem firmware:
 *       - mfw_nrf91x1 v2.0.1 or later
 *       - mfw_nrf9151-ntn
 */
#define NRF_SO_KEEPOPEN 34

/**
 * @brief Bind a socket to a Packet Data Network (PDN) ID (write-only)
 *
 * The passed option is an integer specifying the PDN ID.
 * If a socket is bound to an interface, only packets received from that particular interface are
 * processed by the socket.
 */
#define NRF_SO_BINDTOPDN 40

/**
 * @brief Configure the TCP server session inactivity timeout for a socket.
 *
 *  The timeout value is specified in minutes.
 *  The allowed range is \c 0 (no timeout) to \c 135 (2 h 15 min).
 *
 *  The default value is \c 0 (no timeout).
 *
 * @note This option must be set on the listening socket, but it can be overridden on the accepting
 *       socket afterwards.
 */
#define NRF_SO_TCP_SRV_SESSTIMEO 55

/**
 * @brief Set a callback for @ref nrf_poll events on sockets (write-only).
 *
 * Set a callback for events occurring on this socket such as @ref NRF_POLLIN and @ref NRF_POLLOUT.
 * The @ref nrf_modem_pollcb::callback function is invoked every time any of the events specified
 * by the @ref nrf_modem_pollcb::events bitmask field occurs.
 *
 * In addition, the @ref NRF_POLLHUP and @ref NRF_POLLERR events will also trigger the callback,
 * regardless of whether they are set in the @ref nrf_modem_pollcb::events bitmask field.
 * The callback receives a @ref nrf_pollfd structure, populated in the same way as it would be
 * populated by the @ref nrf_poll function.
 *
 * If the @ref nrf_modem_pollcb::oneshot field is set to \c true, the callback will be invoked only
 * once, and it is automatically unset afterwards.
 *
 * @note The callback is executed in an interrupt context. Take care to offload any processing as
 *       appropriate.
 */
#define NRF_SO_POLLCB 60

/**
 * @brief Release Assistance Indication (RAI) (write-only).
 *
 *  See @ref nrf_socket_options_rai for allowed values.
 */
#define NRF_SO_RAI 61

/**
 * @brief Delay IPv6 address refresh during power saving mode.
 *
 * By default, if the lifetime of an IPv6 address expires during power saving mode (PSM) or eDRX
 * sleep, the device wakes up solely to refresh the address.
 *
 * If this option is enabled, the IPv6 address refresh
 * is delayed until the next time the device wakes up from PSM or eDRX sleep.
 * This avoids unnecessary wake-ups and optimizes the power usage.
 * The option value is an integer, a \c 1 value enables delayed IPv6 address refresh on IPv6.
 *
 * The default value is \c 0 (OFF).
 *
 * @note This is only supported by the following modem firmware:
 *       - mfw_nrf9160 v1.3.7 or later
 *       - mfw_nrf91x1 v2.0.2 or later
 *       - mfw_nrf9151-ntn
 */
#define NRF_SO_IPV6_DELAYED_ADDR_REFRESH 62

/**
 * @brief Callback to be called when a send request is acknowledged.
 *
 * Set a callback to be called when a send request is acknowledged by the network and
 * the data has been acknowledged by the peer, if required by the network protocol, or until the
 * timeout, given by the @ref NRF_SO_SNDTIMEO socket option, is reached. Valid timeout values are
 * 1 to 600 seconds.
 * See @ref nrf_modem_sendcb_t for the callback function type.
 *
 * @note The callback is executed in an interrupt context.
 *	     Take care to offload any processing as appropriate.
 *
 * @note This is only supported by the following modem firmware:
 *       - mfw_nrf9151-ntn
 *
 * This socket option cannot be used along with the @ref NRF_MSG_WAITACK send flag.
 */
#define NRF_SO_SENDCB 63
/** @} */

/**
 * @defgroup nrf_socket_options_sendcb_params Socket send callback option parameters
 * @ingroup nrf_socket
 * @{
 */
/**
 * @brief This structure is used to describe the parameters to a send callback function.
 */
struct nrf_modem_sendcb_params {
	/** Socket handle */
	int fd;
	/** Status. Can be one of the following values:
	 *  - 0 Data sent successfully.
	 *  - [NRF_EAGAIN] The socket's @ref NRF_SO_SNDTIMEO timeout was exceeded.
	 *  - [NRF_EBADF] The socket argument is not a valid file descriptor.
	 *  - [NRF_ECONNRESET] A connection was forcibly closed by a peer.
	 *  - [NRF_ECOMM] Data transmission failed. Sending may be retried.
	 */
	int status;
	/** Number of bytes that was sent. */
	size_t bytes_sent;
};

/** Callback for send events */
typedef void (*nrf_modem_sendcb_t)(const struct nrf_modem_sendcb_params *params);

/** @c nrf_send() and @c nrf_sendto() callback */
struct nrf_modem_sendcb {
	/** Callback function */
	nrf_modem_sendcb_t callback;
};
/** @} */

/**
 * @defgroup nrf_socket_options_levels Socket option levels enumerator
 * @ingroup nrf_socket_api_enumerators
 * @{
 */
#define NRF_SOL_SOCKET 1
#define NRF_SOL_SECURE 282
/** @} */

/**
 * @defgroup nrf_socket_send_recv_flags Socket send/recv flags
 * @ingroup nrf_socket_api_enumerators
 * @{
 */
/**
 * @brief Return data from the beginning of receive queue without removing data from the input
 *        queue.
 *
 * This flag is only supported for the @ref nrf_recv and @ref nrf_recvfrom functions.
 * When this flag is used, the data is copied into the provided buffer but remains in the socket's
 * input queue for future reads. Subsequent calls to @ref nrf_recv or @ref nrf_recvfrom returns
 * the same data until it is read without the @ref NRF_MSG_PEEK flag. This flag is useful when an
 * application needs to inspect incoming data without consuming it, allowing for multiple reads of
 * the same data. The default behavior is to remove the data from the input queue after it has been
 * read.
 */
#define NRF_MSG_PEEK 0x02

/**
 * @brief Enables non-blocking operation for this specific function call.
 *
 * If the operation blocks, the function returns immediately with \c -1 and sets \c errno to
 * @ref NRF_EAGAIN or @ref NRF_EWOULDBLOCK. This flag has no effect if the socket is already set to
 * non-blocking mode using @ref nrf_fcntl. This flag is useful when an application wants to perform
 * a single non-blocking operation on a socket that is otherwise in blocking mode. The default
 * behavior is to follow the socket's blocking mode.
 */
#define NRF_MSG_DONTWAIT 0x40

/**
 * @brief Requests that the function blocks read operation until the full amount of data requested
 *        has been received.
 *
 * This flag is only supported for the @ref nrf_recv and @ref nrf_recvfrom functions.
 *
 * This flag is not valid for datagram sockets.
 *
 * If this flag is used, the function continues to block and wait for more data until either the
 * requested number of bytes has been received or an error occurs. If the connection is closed
 * before all requested data is received, the function returns the number of bytes that were
 * actually received.
 *
 * This flag is useful when an application needs to ensure that it receives a complete message or
 * data block in a single operation. The default behavior is to return as soon as any data is
 * available, even if it is less than the requested amount.
 */
#define NRF_MSG_WAITALL 0x100

/**
 * @brief Request a blocking send operation until the request is acknowledged.
 *
 * @note This flag is only supported for the @ref nrf_send and @ref nrf_sendto functions.
 *
 * When this flag is used, write operations on datagram sockets are blocked until the data has
 * been sent on-air, and write operations on stream sockets are blocked until data reception is
 * acknowledged by the peer.
 *
 * The operation timeout can be configured using the @ref NRF_SO_SNDTIMEO socket option. The
 * valid timeout values are 1 to 600 seconds.
 *
 * For the @ref NRF_SOCK_STREAM socket type, the operation is blocked until the data is
 * acknowledged by the peer, and for the @ref NRF_SOCK_DGRAM socket type, until the data is sent
 * on-air by the modem. This flag is useful in scenarios where confirmation of receipt is critical.
 *
 * The default behavior is to return immediately after queuing the data for sending, without
 * waiting for acknowledgment.
 *
 * @note This send flag cannot be used along with the @ref NRF_SO_SENDCB socket option.
 *
 *
 * @note This is only supported by the following modem firmware:
 *       - mfw_nrf91x1
 *       - mfw_nrf9151-ntn
 */
#define NRF_MSG_WAITACK 0x200
/** @} */

/**
 * @defgroup nrf_fcnt_commands File descriptor control option commands
 * @brief API commands used to control the behaviour of IP sockets using nrf_fcntl().
 * @ingroup nrf_socket
 * @{
 */
/** Get flag. */
#define NRF_F_GETFL 3
/** Set flag. */
#define NRF_F_SETFL 4
/** @} */

/**
 * @defgroup nrf_fcnt_flags File descriptor control option flags
 * @brief Flags used to control the behaviour of IP sockets using nrf_fcntl().
 * @ingroup nrf_socket
 * @{
 */
/** Use non-blocking I/O. */
#define NRF_O_NONBLOCK 0x01
/** @} */

/**
 * @defgroup nrf_socket_options_rai Socket option values for RAI
 * @brief Release Assistance Indication (RAI) @ref NRF_SO_RAI.
 * @ingroup nrf_socket
 * @{
 */
/**
 * @brief Indicate that the application does not intend to send more data.
 *
 * This applies immediately and lets the modem exit connected mode more quickly.
 * (Immediately enter RRC idle mode for this socket. Does not require a following output
 * operation).
 *
 * @note This requires the socket to be connected.
 */
#define NRF_RAI_NO_DATA 1
/**
 * @brief Indicate that the application does not intend to send more data after the next call to
 *        @ref nrf_send() or @ref nrf_sendto().
 *
 * This lets the modem exit connected mode more quickly after sending the data.
 * (Enter RRC idle mode after the next output operation on this socket is complete).
 */
#define NRF_RAI_LAST 2
/**
 * @brief Indicate that the application is expecting to receive just one data packet
 *        after the next call to @ref nrf_send() or @ref nrf_sendto().
 *
 * This lets the modem exit connected mode more quickly after having received the data. (After the
 * next output operation is complete, wait for one more packet to be received from the network on
 * this socket before entering RRC idle mode).
 */
#define NRF_RAI_ONE_RESP 3
/**
 * @brief Indicate that the socket is in active use by a client application.
 *
 * This lets the modem stay in connected mode longer (Keep RRC in connected mode after the next
 * output operation on this socket (client side)).
 */
#define NRF_RAI_ONGOING 4
/**
 * @brief Indicate that the socket is in active use by a server application.
 *
 * This lets the modem stay in connected mode longer (Keep RRC in connected mode after the next
 * output operation on this socket (server side)).
 */
#define NRF_RAI_WAIT_MORE 5
/** @} */

/**
 * @defgroup nrf_socket_sec_peer_verify_options TLS peer verification options
 * @brief Allowed TLS peer verification options @ref NRF_SO_SEC_PEER_VERIFY.
 *
 * @ingroup nrf_socket_tls
 * @{
 */
/** No peer verification */
#define NRF_SO_SEC_PEER_VERIFY_NONE 0
/** Optional peer verification */
#define NRF_SO_SEC_PEER_VERIFY_OPTIONAL 1
/** Peer verification is required */
#define NRF_SO_SEC_PEER_VERIFY_REQUIRED 2
/** @} */

/**
 * @defgroup nrf_socket_sec_roles Role for the socket connection
 * @brief Allowed roles for the socket connection @ref NRF_SO_SEC_ROLE.
 *
 * For TLS, the choice is implicit from the usage of @ref nrf_listen, @ref nrf_accept, and
 * @ref nrf_connect.
 *
 * @ingroup nrf_socket_tls
 * @{
 */
/** Client role */
#define NRF_SO_SEC_ROLE_CLIENT 0
/** Server role */
#define NRF_SO_SEC_ROLE_SERVER 1
/** @} */

/**
 * @defgroup nrf_socket_session_cache_options TLS session cache options
 * @brief Allowed options for the TLS session cache @ref NRF_SO_SEC_SESSION_CACHE.
 *
 * @ingroup nrf_socket_tls
 * @{
 */
/** Disable TLS session caching. */
#define NRF_SO_SEC_SESSION_CACHE_DISABLED 0
/** Enable TLS session caching. */
#define NRF_SO_SEC_SESSION_CACHE_ENABLED 1
/** @} */

/**
 * @defgroup nrf_socket_so_sec_handshake_timeouts DTLS handshake timeout values
 * @brief Allowed timeout values for DTLS handshake timeout socket option according
 *        to RFC6347 section 4.2.4.1. Default is 123 seconds.
 *        (https://tools.ietf.org/html/rfc6347#section-4.2.4.1)
 *
 * @ingroup nrf_socket_tls
 * @{
 */
/** No timeout */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_NONE 0
/** 1 second */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_1S 1
/** 3 seconds (1s + 2s) */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_3S 3
/** 7 seconds (1s + 2s + 4s) */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_7S 7
/** 15 seconds (1s + 2s + 4s + 8s) */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_15S 15
/** 31 seconds (1s + 2s + 4s + 8s + 16s) */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_31S 31
/** 63 seconds (1s + 2s + 4s + 8s + 16s + 32s) */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_63S 63
/** 123 seconds (1s + 2s + 4s + 8s + 16s + 32s + 60s) */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_123S 123
/** @} */

/**
 * @defgroup nrf_socket_tls_cipher_suites TLS Cipher suites
 * @brief Allowed IANA cipher suites for the nRF modem.
 * @ingroup nrf_socket_tls
 * @{
 */
#define NRF_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384 0xC024 /**< TLS 1.2 */
#define NRF_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA  0xC00A /**< Not in SSL3! */
#define NRF_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256 0xC023 /**< TLS 1.2 */
#define NRF_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA 0xC009 /**< Not in SSL3! */
#define NRF_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA 0xC014 /**< Not in SSL3! */
#define NRF_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256 0xC027 /**< TLS 1.2 */
#define NRF_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA 0xC013 /**< Not in SSL3! */
#define NRF_TLS_PSK_WITH_AES_256_CBC_SHA 0x008D
#define NRF_TLS_PSK_WITH_AES_128_CBC_SHA256 0x00AE
#define NRF_TLS_PSK_WITH_AES_128_CBC_SHA 0x008C
#define NRF_TLS_PSK_WITH_AES_128_CCM_8 0xC0A8 /**< TLS 1.2 */
#define NRF_TLS_EMPTY_RENEGOTIATIONINFO_SCSV 0x00FF
#define NRF_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 0xC02B /**< TLS 1.2 */
#define NRF_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384 0xC030 /**< TLS 1.2 */
#define NRF_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256 0xC02F /**< TLS 1.2 */
/** @} */

/**
 * @defgroup nrf_so_sec_dtls_cid_settings DTLS Connection ID settings
 * @brief Allowed values for DTLS connection ID setting socket option @ref NRF_SO_SEC_DTLS_CID.
 *
 * @ingroup nrf_socket_tls
 * @{
 */

/**
 * @brief Disabled - The connection ID extension is not included in the Client Hello, so the
 *        DTLS connection ID is not used.
 */
#define NRF_SO_SEC_DTLS_CID_DISABLED 0
/**
 * @brief Supported - The connection ID extension with a zero-length CID is included in the
 *        Client Hello, so the modem will accept a DTLS connection ID from the server.
 */
#define NRF_SO_SEC_DTLS_CID_SUPPORTED 1
/**
 * @brief Enabled - The connection ID extension with a valid CID is included in the Client Hello,
 *        so the modem will request DTLS connection ID support.
 */
#define NRF_SO_SEC_DTLS_CID_ENABLED 2
/** @} */

/**
 * @defgroup nrf_so_sec_dtls_cid_statuses DTLS Connection ID statuses
 * @brief Allowed values for DTLS connection ID status socket option
 *        @ref NRF_SO_SEC_DTLS_CID_STATUS.
 * @ingroup nrf_socket_tls
 * @{
 */

/**
 * @brief Disabled - The DTLS connection ID is not included in DTLS records sent to and from the
 *        modem. This status is returned before the DTLS handshake is complete.
 */
#define NRF_SO_SEC_DTLS_CID_STATUS_DISABLED 0
/**
 * @brief Downlink - The DTLS connection ID is included only in DTLS records sent to the modem.
 */
#define NRF_SO_SEC_DTLS_CID_STATUS_DOWNLINK 1
/**
 * @brief Uplink - The DTLS connection ID is included only in DTLS records sent from the modem.
 */
#define NRF_SO_SEC_DTLS_CID_STATUS_UPLINK 2
/**
 * @brief Bidirectional - The DTLS connection ID is included in DTLS records sent to and from the
 *        modem.
 */
#define NRF_SO_SEC_DTLS_CID_STATUS_BIDIRECTIONAL 3
/** @} */

/**
 * @defgroup nrf_so_sec_handshake_statuses TLS/DTLS Handshake statuses
 * @brief Allowed values for TLS/DTLS handshake status socket option
 *        @ref NRF_SO_SEC_HANDSHAKE_STATUS.
 *
 * @ingroup nrf_socket_tls
 * @{
 */
/**
 * @brief Full handshake
 *
 * TLS/DTLS attach/negotiation procedure was made with a full handshake, and session cache data was
 * not used or it was not accepted by the server.
 */
#define NRF_SO_SEC_HANDSHAKE_STATUS_FULL 0
/** Cached handshake
 *
 * @brief The latest TLS/DTLS attach negotiation was completed successfully with cached session
 *        data.
 */
#define NRF_SO_SEC_HANDSHAKE_STATUS_CACHED 1
/** @} */

/**
 * @defgroup nrf_so_sec_dtls_frag_ext_options DTLS Fragmentation extension options
 * @brief Allowed values for DTLS Fragmentation extension socket option
 *        @ref NRF_SO_SEC_DTLS_FRAG_EXT.
 *
 * @ingroup nrf_socket_tls
 * @{
 */
/**
 * @brief Disabled - The DTLS fragmentation extension is not included in the Client Hello.
 */
#define NRF_SO_SEC_DTLS_FRAG_EXT_DISABLED 0

/**
 * @brief Enabled - The DTLS fragmentation extension is included in the Client Hello with the
 *        fragment size of 512 bytes.
 *
 * @note The user data size in send requests also becomes limited to a maximum of 512 bytes.
 */
#define NRF_SO_SEC_DTLS_FRAG_EXT_512_ENABLED 1

/**
 * @brief Enabled - The DTLS fragmentation extension is included in the Client Hello with the
 *        fragment size of 1024 bytes.
 *
 * @note The user data size in send requests also becomes limited to a maximum of 1024 bytes.
 */
#define NRF_SO_SEC_DTLS_FRAG_EXT_1024_ENABLED 2
 /** @} */

/**
 * @defgroup nrf_so_sec_tag_tls_decrypt Security tags for decrypting TLS traffic
 * @brief Reserved security tags used for decrypting TLS traffic using Nordic tools.
 * @ingroup nrf_socket_tls
 *
 * Transport Layer Security (TLS) traffic can be decrypted with Nordic tools if the TLS
 * session is created using certificates stored to security tags @ref NRF_SEC_TAG_TLS_DECRYPT_0 to
 * @ref NRF_SEC_TAG_TLS_DECRYPT_19.
 * These security tags must be used only for test and development purposes.
 *
 * @note This is only supported by the following modem firmware:
 *       - mfw_nrf91x1
 *       - mfw_nrf9151-ntn
 * @{
 */
#define NRF_SEC_TAG_TLS_DECRYPT_BASE 2147483648
#define NRF_SEC_TAG_TLS_DECRYPT_0  (NRF_SEC_TAG_TLS_DECRYPT_BASE + 0)
#define NRF_SEC_TAG_TLS_DECRYPT_1  (NRF_SEC_TAG_TLS_DECRYPT_BASE + 1)
#define NRF_SEC_TAG_TLS_DECRYPT_2  (NRF_SEC_TAG_TLS_DECRYPT_BASE + 2)
#define NRF_SEC_TAG_TLS_DECRYPT_3  (NRF_SEC_TAG_TLS_DECRYPT_BASE + 3)
#define NRF_SEC_TAG_TLS_DECRYPT_4  (NRF_SEC_TAG_TLS_DECRYPT_BASE + 4)
#define NRF_SEC_TAG_TLS_DECRYPT_5  (NRF_SEC_TAG_TLS_DECRYPT_BASE + 5)
#define NRF_SEC_TAG_TLS_DECRYPT_6  (NRF_SEC_TAG_TLS_DECRYPT_BASE + 6)
#define NRF_SEC_TAG_TLS_DECRYPT_7  (NRF_SEC_TAG_TLS_DECRYPT_BASE + 7)
#define NRF_SEC_TAG_TLS_DECRYPT_8  (NRF_SEC_TAG_TLS_DECRYPT_BASE + 8)
#define NRF_SEC_TAG_TLS_DECRYPT_9  (NRF_SEC_TAG_TLS_DECRYPT_BASE + 9)
#define NRF_SEC_TAG_TLS_DECRYPT_10 (NRF_SEC_TAG_TLS_DECRYPT_BASE + 10)
#define NRF_SEC_TAG_TLS_DECRYPT_11 (NRF_SEC_TAG_TLS_DECRYPT_BASE + 11)
#define NRF_SEC_TAG_TLS_DECRYPT_12 (NRF_SEC_TAG_TLS_DECRYPT_BASE + 12)
#define NRF_SEC_TAG_TLS_DECRYPT_13 (NRF_SEC_TAG_TLS_DECRYPT_BASE + 13)
#define NRF_SEC_TAG_TLS_DECRYPT_14 (NRF_SEC_TAG_TLS_DECRYPT_BASE + 14)
#define NRF_SEC_TAG_TLS_DECRYPT_15 (NRF_SEC_TAG_TLS_DECRYPT_BASE + 15)
#define NRF_SEC_TAG_TLS_DECRYPT_16 (NRF_SEC_TAG_TLS_DECRYPT_BASE + 16)
#define NRF_SEC_TAG_TLS_DECRYPT_17 (NRF_SEC_TAG_TLS_DECRYPT_BASE + 17)
#define NRF_SEC_TAG_TLS_DECRYPT_18 (NRF_SEC_TAG_TLS_DECRYPT_BASE + 18)
#define NRF_SEC_TAG_TLS_DECRYPT_19 (NRF_SEC_TAG_TLS_DECRYPT_BASE + 19)
/** @} */

/**
 * @defgroup nrf_so_sec_tag_tls_invalid Invalid security tag
 * @brief Designated value that represents an invalid @sa nrf_sec_tag_t.
 * @ingroup nrf_socket_tls
 * @{
 */
#define NRF_SEC_TAG_TLS_INVALID 0xFFFFFFFF
/** @} */

/**
 * @addtogroup nrf_socket_api
 * @{
 */

/**
 * @brief Structure specifying time interval.
 */
struct nrf_timeval {
	/** Time interval seconds. */
	uint32_t tv_sec;
	/** Time interval microseconds. */
	uint32_t tv_usec;
};
/** @} */

/**
 * @addtogroup nrf_socket_address_resolution
 * @{
 */

/**
 * @brief Socket port type.
 */
typedef uint16_t nrf_in_port_t;

/**
 * @brief Socket families.
 *
 * @details For a list of valid values, refer to @ref nrf_socket_families.
 */
typedef unsigned short int nrf_sa_family_t;

/**
 * @brief IPv6 address.
 */
struct nrf_in6_addr {
	uint8_t s6_addr[16];
};

/**
 * @brief IPv4 address.
 */
typedef uint32_t nrf_in_addr_t;

/**
 * @brief IPv4 address structure.
 */
struct nrf_in_addr {
	nrf_in_addr_t s_addr;
};

/**
 * @brief Global IPv6 any-address.
 */
extern const struct nrf_in6_addr nrf_in6addr_any;

/**
 * @brief Global IPv4 any-address.
 */
extern const struct nrf_in_addr nrf_inaddr_any;

/**
 * @brief Address record for IPv6 addresses.
 *
 * Contains the address and port of the host.
 */
struct nrf_sockaddr_in6 {
	/** Socket family. */
	nrf_sa_family_t sin6_family;
	/** Port, in network byte order. */
	nrf_in_port_t sin6_port;
	/** IPv6 address. */
	struct nrf_in6_addr sin6_addr;
	/** IPv6 scope ID. Not used. */
	uint8_t sin6_scope_id;
};

/**
 * @brief Address record for IPv4 addresses.
 *
 * Contains the address and port of the host.
 */
struct nrf_sockaddr_in {
	/** Socket family. */
	nrf_sa_family_t sin_family;
	/** Port, in network byte order. */
	nrf_in_port_t sin_port;
	/** IPv4 address. */
	struct nrf_in_addr sin_addr;
};

typedef uint32_t nrf_socklen_t;
typedef uint32_t nrf_nfds_t;

/**
 * @brief Generic socket address.
 *
 * @details Only provided for API compatibility.
 */
struct nrf_sockaddr {
	/** Socket address family */
	nrf_sa_family_t sa_family;
	/** Socket address */
	char sa_data[];
};

/* Flags for getaddrinfo() hints. */

/** Fill in ai_canonname */
#define NRF_AI_CANONNAME 0x2
/** Assume `service` (port) is numeric. */
#define NRF_AI_NUMERICSERV 0x400
/** Assume `service` contains a Packet Data Network (PDN) ID.
 *  When specified together with the NRF_AI_NUMERICSERV flag,
 *  `service` must be formatted as follows: "port:pdn_id"
 *  where "port" is the port number and "pdn_id" is the PDN ID.
 *  Example: "8080:1", port 8080 PDN ID 1.
 *  Example: "42:0", port 42 PDN ID 0.
 */
#define NRF_AI_PDNSERV 0x1000

/**
 * @brief Address information.
 */
struct nrf_addrinfo {
	/** Input flags. */
	int ai_flags;
	/** Address family of the socket. */
	int ai_family;
	/** Socket type. */
	int ai_socktype;
	/** Protocol of the socket. */
	int ai_protocol;
	/** Length of the socket address. */
	nrf_socklen_t ai_addrlen;
	/** Address of the socket. */
	struct nrf_sockaddr *ai_addr;
	/** Canonical name of service location. */
	char *ai_canonname;
	/** Pointer to next in list. */
	struct nrf_addrinfo *ai_next;
};

/**
 * @brief Interface address information.
 */
struct nrf_ifaddrs {
	/* Pointer to next struct */
	struct nrf_ifaddrs *ifa_next;
	/** Interface name. */
	char *ifa_name;
	/** Interface flags. */
	uint32_t ifa_flags;
	/** Interface address. */
	struct nrf_sockaddr *ifa_addr;
	/** Interface netmask. Not supported by the modem. */
	struct nrf_sockaddr *ifa_netmask;
	/** Interface broadcast address. Not supported by the modem.  */
	struct nrf_sockaddr *ifa_broadaddr;
	/** Interface destination address. Not supported by the modem.  */
	struct nrf_sockaddr *ifa_dstaddr;
	/** Address specific data. */
	void *ifa_data;
};

/** @} */

/**
 * @addtogroup nrf_socket_tls
 * @{
 */

/**
 * @brief Security tags used on the TLS socket.
 *
 * More than one security tag may be used on a socket.
 * If more than one tag is used on the socket, pass an array of security tags.
 *
 * A maximum of @ref NRF_SOCKET_TLS_MAX_SEC_TAG_LIST_SIZE tags can be set per socket.
 */
typedef uint32_t nrf_sec_tag_t;

/** @} */

/**
 * @defgroup nrf_socket_api nRF Socket interface
 * @{
 */

/**
 * @brief Create a network socket.
 *
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/socket.html">
 * POSIX.1-2017 article</a> for normative description.
 *
 * In addition, the function may return -1 and set the following errno:
 * - [NRF_ESHUTDOWN] Modem was shut down.
 */
int nrf_socket(int family, int type, int protocol);

/**
 * @brief Close a network socket.
 *
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/close.html">
 * POSIX.1-2017 article</a> for normative description.
 *
 * In addition, the function may return -1 and set the following errno:
 * - [NRF_ESHUTDOWN] Modem was shut down.
 */
int nrf_close(int fildes);

/**
 * @brief Function for controlling file descriptor options.
 *
 * Set or get file descriptor options or flags.
 * For a list of supported commands, refer to @ref nrf_fcnt_commands.
 * For a list of supported flags, refer to @ref nrf_fcnt_flags.
 *
 * @param fd    The descriptor to set options on.
 * @param cmd   The command class for options.
 * @param flags The flags to set.
 *
 * @return Value dependent on command class:
 *         NRF_F_GETFL - Value of file status flags.
 *         -1 on error, and errno indicates the reason for failure.
 *         0 otherwise.
 */
int nrf_fcntl(int fd, int cmd, int flags);

/**
 * @brief Connect a socket.
 *
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/connect.html">
 * POSIX.1-2017 article</a> for normative description.
 *
 * In addition, the function may return -1 and set the following errno:
 * - [NRF_ESHUTDOWN] Modem was shut down.
 *
 * In addition, an asynchronous connection attempt will fail and set NRF_SO_ERROR if:
 * - [NRF_EBUSY] Another TLS handshake was ongoing.
 */
int nrf_connect(int socket, const struct nrf_sockaddr *address, nrf_socklen_t address_len);

/**
 * @brief Send a message on a connected socket.
 *
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/send.html">
 * POSIX.1-2017 article</a> for normative description.
 *
 * In addition, the function may return -1 and set the following errno:
 * - [NRF_ESHUTDOWN] Modem was shut down.
 * - [NRF_ECANCELED] Operation canceled because of APN rate control.
 * - [NRF_ENOMEM] TCP stream interrupted because of no heap memory.
 * - [NRF_EPROTO] Request failed because DTLS context was serialized.
 * - [NRF_EBUSY] Earlier send request with the @ref NRF_SO_SENDCB socket option set is still
 *               ongoing.
 * - [NRF_ECOMM] Data transmission with the @ref NRF_MSG_WAITACK flag failed. Sending may be
 *               retried.
 */
ssize_t nrf_send(int socket, const void *buffer, size_t length, int flags);

/**
 * @brief Send a message on a socket.
 *
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/sendto.html">
 * POSIX.1-2017 article</a> for normative description.
 *
 * In addition, the function may return -1 and set the following errno:
 * - [NRF_ESHUTDOWN] Modem was shut down.
 * - [NRF_ECANCELED] Operation canceled because of APN rate control.
 * - [NRF_ENOMEM] TCP stream interrupted because of no heap memory.
 * - [NRF_EPROTO] Request failed because DTLS context was serialized.
 * - [NRF_EBUSY] Earlier send request with the @ref NRF_SO_SENDCB socket option set is still
 *               ongoing.
 * - [NRF_ECOMM] Data transmission with the @ref NRF_MSG_WAITACK flag failed. Sending may be
 *               retried.
 */
ssize_t nrf_sendto(int socket, const void *message, size_t length, int flags,
		   const struct nrf_sockaddr *dest_addr, nrf_socklen_t dest_len);

/**
 * @brief Receive a message from a connected socket.
 *
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/recv.html">
 * POSIX.1-2017 article</a> for normative description.
 *
 * In addition, the function may return -1 and set the following errno:
 * - [NRF_ESHUTDOWN] Modem was shut down.
 * - [NRF_ENOMEM] TCP stream interrupted because of no heap memory.
 */
ssize_t nrf_recv(int socket, void *buffer, size_t length, int flags);

/**
 * @brief Receive a message from a socket.
 *
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/recvfrom.html">
 * POSIX.1-2017 article</a> for normative description.
 *
 * In addition, the function may return -1 and set the following errno:
 * - [NRF_ESHUTDOWN] Modem was shut down.
 * - [NRF_ENOMEM] TCP stream interrupted because of no heap memory.
 */
ssize_t nrf_recvfrom(int socket, void *restrict buffer, size_t length, int flags,
		     struct nrf_sockaddr *restrict address, nrf_socklen_t *restrict address_len);

/** @} */

/**
 * @defgroup nrf_socket_api_poll Socket polling API
 * @brief Data types and defines for use with nrf_poll().
 * @{
 */

/**
 * @brief This structure is used to describe which events to poll for a given socket.
 */
struct nrf_pollfd {
	/** Socket handle */
	int fd;
	/** The input event flags */
	short events;
	/** The output event flags */
	short revents;
};

/** @brief Data other than high-priority data may be read without blocking */
#define NRF_POLLIN 0x1
/** @brief Data may be written without blocking */
#define NRF_POLLOUT 0x4
/** @brief An error has occurred (revents only) */
#define NRF_POLLERR 0x8
/** @brief Device has been disconnected (revents only) */
#define NRF_POLLHUP 0x10
/** @brief Invalid fd member (revents only) */
#define NRF_POLLNVAL 0x20

/** Callback for poll events */
typedef void (*nrf_modem_pollcb_t)(struct nrf_pollfd *pollfd);

/** @c nrf_poll() callback */
struct nrf_modem_pollcb {
	/** Callback function */
	nrf_modem_pollcb_t callback;
	/** Events mask */
	short events;
	/** Oneshot callback.
	 *  If @c true, unset the callback after invoking it.
	 */
	bool oneshot;
};

/** @} */

/**
 * @addtogroup nrf_socket_api
 * @{
 */

/**
 * @brief Poll multiple sockets for events.
 *
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/poll.html">
 * POSIX.1-2017 article</a> for normative description.
 *
 * @note In Modem library, this function works only with Modem library sockets,
 *       not arbitrary file descriptors.
 */
int nrf_poll(struct nrf_pollfd fds[], nrf_nfds_t nfds, int timeout);

/**
 * @brief Set the socket options.
 *
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/setsockopt.html">
 * POSIX.1-2017 article</a> for normative description.
 *
 * @note In Modem library this function supports a subset
 *       of socket options described by POSIX, but also some additional options.
 *
 * In addition, the function may return -1 and set the following errno:
 * - [NRF_EAGAIN] The option could not be set when requested, try again.
 * - [NRF_EDESTADDRREQ] The socket option @ref NRF_SO_RAI with value @ref NRF_RAI_NO_DATA cannot be
 *                      set on a socket that is not connected.
 * - [NRF_EOPNOTSUPP] The option is not supported with the current socket configuration.
 * - [NRF_ESHUTDOWN] Modem was shut down.
 */
int nrf_setsockopt(int socket, int level, int option_name,
		   const void *option_value, nrf_socklen_t option_len);

/**
 * @brief Get the socket options.
 *
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/getsockopt.html">
 * POSIX.1-2017 article</a> for normative description.
 *
 * In Modem library this function supports a subset
 * of socket options described by POSIX, but also some additional options.
 */
int nrf_getsockopt(int socket, int level, int option_name,
		   void *restrict option_value, nrf_socklen_t *restrict option_len);

/**
 * @brief Bind a name to a socket.
 *
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/bind.html">
 * POSIX.1-2017 article</a> for normative description.
 *
 * In addition, the function may return -1 and set the following errno:
 * - [NRF_ESHUTDOWN] Modem was shut down.
 */
int nrf_bind(int socket, const struct nrf_sockaddr *address, nrf_socklen_t address_len);

/**
 * @brief Listen for socket connections and limit the queue of incoming connections.
 *
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/listen.html">
 * POSIX.1-2017 article</a> for normative description.
 *
 * In addition, the function may return -1 and set the following errno:
 * - [NRF_ESHUTDOWN] Modem was shut down.
 */
int nrf_listen(int sock, int backlog);

/**
 * @brief Accept a new connection a socket.
 *
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/accept.html">
 * POSIX.1-2017 article</a> for normative description.
 *
 * In addition, the function may return -1 and set the following errno:
 * - [NRF_EAGAIN] The socket's NRF_SO_RCVTIMEO timeout was exceeded.
 * - [NRF_ESHUTDOWN] Modem was shut down.
 */
int nrf_accept(int socket, struct nrf_sockaddr *restrict address,
	       nrf_socklen_t *restrict address_len);

/** @} */

/**
 * @defgroup nrf_socket_address_resolution Socket address resolution API
 * @brief Address resolution utility functions.
 * @ingroup nrf_socket
 * @{
 */

/**
 * @brief Convert IPv4 and IPv6 addresses between binary and text form.
 *
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/inet_pton.html">
 * POSIX.1-2017 article</a> for normative description.
 */
int nrf_inet_pton(int af, const char *restrict src, void *restrict dst);

/**
 * @brief Convert IPv4 and IPv6 addresses between binary and text form.
 *
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/inet_ntop.html">
 * POSIX.1-2017 article</a> for normative description.
 */
const char *nrf_inet_ntop(int af, const void *restrict src, char *restrict dst, nrf_socklen_t size);

/**
 * @brief Get address information.
 *
 * See <a href="http://pubs.opengroup.org/onlinepubs/9699919799/functions/getaddrinfo.html">
 * POSIX.1-2017 article</a> for normative description.
 *
 * @note
 * When using address family @c NRF_AF_UNSPEC the implementation attempts to resolve the IPv6
 * address first. If successful, the resolved address is returned. If not, the implementation
 * attempts to resolve and return the IPv4 family address. This applies regardless of the PDN
 * supporting IPv6 or not.
 *
 * In addition, the function may return -1 and set the following errno:
 * - [NRF_ESHUTDOWN] Modem was shut down.
 */
int nrf_getaddrinfo(const char *restrict nodename,
		    const char *restrict servname,
		    const struct nrf_addrinfo *restrict hints,
		    struct nrf_addrinfo **restrict res);

/**
 * @brief Free address information returned by @ref nrf_getaddrinfo().
 *
 * @details
 * See <a href="http://pubs.opengroup.org/onlinepubs/9699919799/functions/freeaddrinfo.html">
 * POSIX.1-2017 article</a> for normative description.
 */
void nrf_freeaddrinfo(struct nrf_addrinfo *ai);

/**
 * @brief Get interface address information.
 *
 * Create a linked list of nrf_ifaddrs structures describing the network interfaces
 * and store the address of the first item of the list in @p *ifa.
 *
 * The data returned by this function is dynamically allocated and
 * must be freed using @ref nrf_freeifaddrs() when no longer needed.
 *
 * @param[in, out] ifa First item in the linked list of interface addresses.
 *
 * @retval 0 on success.
 * @retval -1 on error, and set @c errno to indicate the reason.
 */
int nrf_getifaddrs(struct nrf_ifaddrs **ifa);

/**
 * @brief Free address information returned by @ref nrf_getifaddrs().
 *
 * Free a linked list of nrf_ifaddrs structures.
 *
 * @param ifa First item in the linked list of interface addresses.
 */
void nrf_freeifaddrs(struct nrf_ifaddrs *ifa);

/**
 * @brief Set or unset a fallback DNS address.
 *
 * The fallback DNS address is used only when the network-provided DNS addresses are
 * missing or unreachable. The fallback DNS does not override the network-provided DNS.
 *
 * @param family    Address family, either NRF_AF_INET or NRF_AF_INET6.
 * @param in_addr   An IPv4 or IPv6 address encoded in a nrf_in_addr or nrf_in6_addr structure,
 *                  respectively. To unset the fallback DNS address, @p in_addr must be NULL.
 * @param in_size   Size of the structure pointed to by @p in_addr. To unset the fallback DNS
 *                  address, @p in_size must be 0 (when @p in_addr is NULL).
 *
 * @retval 0  On success
 * @retval -1 On error, and set @c errno to indicate the reason.
 *
 * The function may return -1 and set the following errno:
 * - [NRF_EPERM] The Modem library is not initialized.
 * - [NRF_EINVAL] Invalid parameters.
 * - [NRF_EAFNOSUPPORT] The implementation does not support the specified address family.
 * - [NRF_ENOBUFS] Not enough shared memory for this request.
 * - [NRF_ESHUTDOWN] Modem was shut down.
 */
int nrf_setdnsaddr(int family, const void *in_addr, nrf_socklen_t in_size);

/**
 * @brief Enable or disable data traffic through the socket interface.
 *
 * This function can be used to disable all data traffic through the socket interface.
 *
 * When enabled is set to false, all sockets are closed and no new sockets can be opened.
 * DNS operations are also disabled. The affected functions indicate an error and set @c errno
 * to NRF_EACCES when data is disabled.
 *
 * @param enabled Whether to enable or disable data.
 *
 * @retval 0 on success.
 * @retval -1 on error, and set @c errno to indicate the reason.
 */
int nrf_socket_data_enabled_set(bool enabled);

/**
 * @brief Get the current data enabled state.
 *
 * This function is used to get the current data enabled state.
 *
 * @retval true if data is enabled.
 * @retval false if data is disabled.
 */
bool nrf_socket_data_enabled_get(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* NRF_SOCKET_H__ */
/** @} */
