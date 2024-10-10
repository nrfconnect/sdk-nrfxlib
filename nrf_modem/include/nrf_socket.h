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

/** Convert byte order from host to network (short). */
#define nrf_htons(x) NRF_HTONS(x)

/** Convert byte order from host to network (long). */
#define nrf_htonl(x) NRF_HTONL(x)

/** Convert byte order from network to host (short). */
#define nrf_ntohs(x) NRF_NTOHS(x)

/** Convert byte order from network to host (long). */
#define nrf_ntohl(x) NRF_NTOHL(x)

/** @brief Maximum length of IPv4 in string form, including null-termination character. */
#define NRF_INET_ADDRSTRLEN 16

/** @brief Maximum length of IPv6 in string form, including null-termination character. */
#define NRF_INET6_ADDRSTRLEN 46

/** @} */

/**
 * @defgroup nrf_socket_api_enumerators Socket enumerators.
 * @brief Enumerated values that is used as input arguments to multiple socket functions.
 * @{
 */

/**
 * @defgroup nrf_socket_families Socket family.
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
 * @defgroup nrf_socket_types Socket type.
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
 * @defgroup nrf_socket_protocols Socket protocols.
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

/** @brief
 * Maximum TLS message size in bytes.
 */
#define NRF_SOCKET_TLS_MAX_MESSAGE_SIZE 2048

/** @brief
 * Maximum number of security tags that can be associated with a socket.
 */
#define NRF_SOCKET_TLS_MAX_SEC_TAG_LIST_SIZE  7

/** @brief
 * Write-only socket option to select the security tags to be used.
 * This option accepts a list of @sa nrf_sec_tag_t.
 */
#define NRF_SO_SEC_TAG_LIST 1

/** @brief
 * Socket option to set the hostname used for peer verification.
 * This option accepts a string containing the hostname, and its length.
 * The length may be set to zero to disable hostname verification.
 */
#define NRF_SO_SEC_HOSTNAME 2

/** @brief
 * Write-only socket option to select which ciphersuites to use.
 * This option accepts a prioritized array of selected cipher suites.
 * See @ref nrf_socket_tls_cipher_suites for a list of allowed values.
 */
#define NRF_SO_SEC_CIPHERSUITE_LIST 3

/** @brief
 * Socket option to retrieve the cipher suite used during the TLS/DTLS handshake.
 *
 * @note This socket option is only supported with Modem firmware 2.0.0 and newer.
 */
#define NRF_SO_SEC_CIPHERSUITE_USED 4

/** @brief
 * Socket option to set peer verification level.
 * See @ref nrf_socket_sec_peer_verify_options for a list of allowed values.
 */
#define NRF_SO_SEC_PEER_VERIFY 5

/** @brief
 * Write-only socket option to set role for the connection.
 * See @ref nrf_socket_sec_roles for a list of allowed values.
 */
#define NRF_SO_SEC_ROLE 6

/** @brief
 * Socket option to control TLS session caching.
 * See @ref nrf_socket_session_cache_options for a list of allowed values.
 */
#define NRF_SO_SEC_SESSION_CACHE 12

/** @brief
 * Socket option to purge session cache immediately.
 * This option accepts any value.
 */
#define NRF_SO_SEC_SESSION_CACHE_PURGE 13

/** @brief
 * Socket option to set DTLS handshake timeout value.
 * See @ref nrf_socket_so_sec_handshake_timeouts for allowed values.
 */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEO 14

/** @brief
 * Socket option to enable/disable the connection ID.
 * See @ref nrf_so_sec_dtls_cid_settings for allowed values.
 *
 * @note This socket option is only supported with Modem firmware v1.3.5 and newer.
 */
#define NRF_SO_SEC_DTLS_CID 15

/** @brief
 * Socket option to get the connection ID status.
 * See @ref nrf_so_sec_dtls_cid_statuses for allowed values.
 *
 * @note This socket option is only supported with Modem firmware v1.3.5 and newer.
 */
#define NRF_SO_SEC_DTLS_CID_STATUS 16

/** @brief
 * Socket option to save DTLS connection.
 *
 * Serializes the socket and compresses it. After the socket option is successfully called, you must
 * call @c NRF_SO_SEC_DTLS_CONN_LOAD before continuing to communicate on the socket.
 *
 * @note This socket option is only supported with Modem firmware v1.3.5 and newer.
 */
#define NRF_SO_SEC_DTLS_CONN_SAVE 17

/** @brief
 * Socket option to load DTLS connection.
 *
 * @note This socket option is only supported with Modem firmware v1.3.5 and newer.
 */
#define NRF_SO_SEC_DTLS_CONN_LOAD 18

/** @brief
 * Socket option to get end status of last completed TLS/DTLS handshake procedure.
 * See @ref nrf_so_sec_handshake_statuses for allowed values.
 *
 * @note This socket option is only supported with Modem firmware 2.0.0 and newer.
 */
#define NRF_SO_SEC_HANDSHAKE_STATUS 19
/** @} */

/**
 * @defgroup nrf_socket_options_sockets Generic socket options
 * @brief Socket options used with IP sockets.
 * @ingroup nrf_socket
 * @{
 */
/** Enable reuse of server addresses */
#define NRF_SO_REUSEADDR 2
/** Read and clear socket error status (read only). */
#define NRF_SO_ERROR 4
/** Receive timeout. */
#define NRF_SO_RCVTIMEO 20
/** Send timeout. */
#define NRF_SO_SNDTIMEO 21
/** Disable ICMP echo replies on both IPv4 and IPv6.
 *  Set to 1 to enable, or to 0 to disable. Default is 0, disabled.
 */
#define NRF_SO_SILENCE_ALL 30
/** Enable ICMP echo reply. Set to 1 to enable, or to 0 to disable. Default is 1, enabled. */
#define NRF_SO_IP_ECHO_REPLY 31
/** Enable ICMPv6 echo reply. Set to 1 to enable, or to 0 to disable. Default is 1, enabled. */
#define NRF_SO_IPV6_ECHO_REPLY 32
/** Send data related to an exceptional event. */
#define NRF_SO_EXCEPTIONAL_DATA 33
/** Keep socket open when its PDN connection is lost. */
#define NRF_SO_KEEPOPEN 34
/** Bind a socket to a Packet Data Network ID. */
#define NRF_SO_BINDTOPDN 40
/** Configurable TCP server session timeout in minutes.
 *  Range is 0 to 135. 0 is no timeout and 135 is 2 h 15 min. Default is 0 (no timeout).
 */
#define NRF_SO_TCP_SRV_SESSTIMEO 55
/** Set a callback for poll events */
#define NRF_SO_POLLCB 60
/** Release Assistance Indication (RAI).
 *  See @ref nrf_socket_options_rai for allowed values.
 */
#define NRF_SO_RAI 61

/** Release Assistance Indication.
 *  Indicate that the application does not intend to send more data.
 *  This socket option applies immediately and lets the modem exit connected mode more quickly.
 *
 *  @note This socket option requires the socket to be connected.
 *
 *  @deprecated since v2.6.0, use @ref NRF_SO_RAI with value @ref NRF_RAI_NO_DATA instead.
 */
#define NRF_SO_RAI_NO_DATA 50

/** Release Assistance Indication.
 *  Indicate that the application does not intend to send more data
 *  after the next call to send() or sendto().
 *  This lets the modem exit connected mode more quickly after sending the data.
 *
 *  @deprecated since v2.6.0, use @ref NRF_SO_RAI with value @ref NRF_RAI_LAST instead.
 */
#define NRF_SO_RAI_LAST 51

/** Release Assistance Indication.
 *  Indicate that the application is expecting to receive just one data packet
 *  after the next call to send() or sendto().
 *  This lets the modem exit connected mode more quickly after having received the data.
 *
 *  @deprecated since v2.6.0, use @ref NRF_SO_RAI with value @ref NRF_RAI_ONE_RESP instead.
 */
#define NRF_SO_RAI_ONE_RESP 52

/** Release Assistance Indication.
 *  Indicate that the socket is in active use by a client application.
 *  This lets the modem stay in connected mode longer.
 *
 *  @deprecated since v2.6.0, use @ref NRF_SO_RAI with value @ref NRF_RAI_ONGOING instead.
 */
#define NRF_SO_RAI_ONGOING 53

/** Release Assistance Indication.
 *  Indicate that the socket is in active use by a server application.
 *  This lets the modem stay in connected mode longer.
 *
 *  @deprecated since v2.6.0, use @ref NRF_SO_RAI with value @ref NRF_RAI_WAIT_MORE instead.
 */
#define NRF_SO_RAI_WAIT_MORE 54
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
 * @defgroup nrf_socket_send_recv_flags Socket send/recv flags.
 * @ingroup nrf_socket_api_enumerators
 * @{
 */
/** Return data from the beginning of receive queue without removing data from the queue. */
#define NRF_MSG_PEEK 0x02
/** Enables non-blocking operation. */
#define NRF_MSG_DONTWAIT 0x40
/** Request a blocking read operation until the request is satisfied. */
#define NRF_MSG_WAITALL 0x100
/** Request a blocking send operation until the request is acknowledged.
 *  When used in @c nrf_send() or @c nrf_sendto(), the operation will block until the data has been
 *  sent on-air and acknowledged by the peer, if required by the network protocol.
 */
#define NRF_MSG_WAITACK 0x200
/** @} */

/**
 * @defgroup nrf_fcnt_commands File descriptor control option commands.
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
 * @defgroup nrf_fcnt_flags File descriptor control option flags.
 * @brief Flags used to control the behaviour of IP sockets using nrf_fcntl().
 * @ingroup nrf_socket
 * @{
 */
/** Use non-blocking I/O. */
#define NRF_O_NONBLOCK 0x01
/** @} */

/**
 * @defgroup nrf_socket_options_rai Socket option values for RAI
 * @brief Release Assistance Indication (RAI).
 * @ingroup nrf_socket
 * @{
 */
/** Indicate that the application does not intend to send more data.
 *  This applies immediately and lets the modem exit connected mode more
 *  quickly.
 *
 *  @note This requires the socket to be connected.
 */
#define NRF_RAI_NO_DATA 1
/** Indicate that the application does not intend to send more data
 *  after the next call to send() or sendto().
 *  This lets the modem exit connected mode more quickly after sending the data.
 */
#define NRF_RAI_LAST 2
/** Indicate that the application is expecting to receive just one data packet
 *  after the next call to send() or sendto().
 *  This lets the modem exit connected mode more quickly after having received the data.
 */
#define NRF_RAI_ONE_RESP 3
/** Indicate that the socket is in active use by a client application.
 *  This lets the modem stay in connected mode longer.
 */
#define NRF_RAI_ONGOING 4
/** Indicate that the socket is in active use by a server application.
 *  This lets the modem stay in connected mode longer.
 */
#define NRF_RAI_WAIT_MORE 5
/** @} */

/**
 * @defgroup nrf_socket_sec_peer_verify_options TLS peer verification options
 * @brief Allowed TLS peer verification options. By default, peer verification is optional.
 *
 * @ingroup nrf_socket_tls
 * @{
 */
/** None */
#define NRF_SO_SEC_PEER_VERIFY_NONE 0
/** Optional */
#define NRF_SO_SEC_PEER_VERIFY_OPTIONAL 1
/** Required */
#define NRF_SO_SEC_PEER_VERIFY_REQUIRED 2
/** @} */

/**
 * @defgroup nrf_socket_sec_roles Role for the socket connection
 * @brief Allowed roles for the socket connection.
 *
 * @ingroup nrf_socket_tls
 * @{
 */
/** Client */
#define NRF_SO_SEC_ROLE_CLIENT 0
/** Server */
#define NRF_SO_SEC_ROLE_SERVER 1
/** @} */

/**
 * @defgroup nrf_socket_session_cache_options TLS session cache options
 * @brief Allowed options for the TLS session cache. By default, the session cache is enabled.
 * @note Session cache may not be used if the peer does not support it.
 *
 * @ingroup nrf_socket_tls
 * @{
 */
/** Disabled */
#define NRF_SO_SEC_SESSION_CACHE_DISABLED 0
/** Enabled */
#define NRF_SO_SEC_SESSION_CACHE_ENABLED 1
/** @} */

/**
 * @defgroup nrf_socket_so_sec_handshake_timeouts DTLS handshake timeout values
 * @brief Allowed timeout values for DTLS handshake timeout socket option according
 *        to RFC6347 section 4.2.4.1. Default is 123 seconds.
 *        (https://tools.ietf.org/html/rfc6347#section-4.2.4.1)
 * @ingroup nrf_socket_tls
 * @{
 */
/** No timeout */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_NONE 0
/** 1 second */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_1S 1
/** 1s + 2s */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_3S 3
/** 1s + 2s + 4s */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_7S 7
/** 1s + 2s + 4s + 8s */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_15S 15
/** 1s + 2s + 4s + 8s + 16s */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_31S 31
/** 1s + 2s + 4s + 8s + 16s + 32s */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_63S 63
/** 1s + 2s + 4s + 8s + 16s + 32s + 60s */
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
/** The following cipher suites are only supported with
 * Modem firmware v1.3.x >= 1,
 * Modem firmware v1.2.x >= 7 and
 * Modem firmware v1.1.x >= 5.
 */
#define NRF_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 0xC02B /**< TLS 1.2 */
#define NRF_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384 0xC030 /**< TLS 1.2 */
#define NRF_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256 0xC02F /**< TLS 1.2 */
/** @} */

/**
 * @defgroup nrf_so_sec_dtls_cid_settings DTLS Connection ID settings
 * @brief Allowed values for DTLS connection ID setting socket option.
 * @ingroup nrf_socket_tls
 * @{
 */

/**
 * Disabled - The connection ID extension is not included in the client hello, so the
 * DTLS connection ID is not used.
 */
#define NRF_SO_SEC_DTLS_CID_DISABLED 0
/**
 * Supported - The connection ID extension with a zero-length CID is included in the client hello,
 * so the modem will accept a DTLS connection ID from the server.
 */
#define NRF_SO_SEC_DTLS_CID_SUPPORTED 1
/**
 * Enabled - The connection ID extension with a valid CID is included in the client hello, so the
 * modem will request DTLS connection ID support.
 */
#define NRF_SO_SEC_DTLS_CID_ENABLED 2
/** @} */

/**
 * @defgroup nrf_so_sec_dtls_cid_statuses DTLS Connection ID statuses
 * @brief Allowed values for DTLS connection ID status socket option.
 * @ingroup nrf_socket_tls
 * @{
 */

/**
 * Disabled - The DTLS connection ID is not included in DTLS records sent to and from the modem.
 * This status is returned before the DTLS handshake is complete.
 */
#define NRF_SO_SEC_DTLS_CID_STATUS_DISABLED 0
/**
 * Downlink - The DTLS connection ID is included only in DTLS records sent to the modem.
 */
#define NRF_SO_SEC_DTLS_CID_STATUS_DOWNLINK 1
/**
 * Uplink - The DTLS connection ID is included only in DTLS records sent from the modem.
 */
#define NRF_SO_SEC_DTLS_CID_STATUS_UPLINK 2
/**
 * Bidirectional - The DTLS connection ID is included in DTLS records sent to and from the modem.
 */
#define NRF_SO_SEC_DTLS_CID_STATUS_BIDIRECTIONAL 3
/** @} */

/**
 * @defgroup nrf_so_sec_handshake_statuses TLS/DTLS Handshake statuses
 * @brief Allowed values for DTLS connection ID status socket option.
 * @ingroup nrf_socket_tls
 * @{
 */
/** full */
#define NRF_SO_SEC_HANDSHAKE_STATUS_FULL 0
/** cached */
#define NRF_SO_SEC_HANDSHAKE_STATUS_CACHED 1
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
 * @note Supported by modem firmware v2.0.0 or later.
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
 * @details For a list of valid values, refer to nrf_socket_families.
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

/** @brief Address information. */
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

/** @brief Interface address information. */
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

/** @brief
 * TLS role for the connection.
 *  - 0 - TLS client role.
 *  - 1 - TLS server role.
 *
 * @deprecated since v2.6.0, use type int instead.
 */
typedef uint32_t nrf_sec_role_t;

/** @brief
 * Security tags used on the TLS socket.
 *
 * More than one security tags may be used on a socket.
 * If more than one tag is used on the socket, pass an array of security tags.
 *
 * A maximum of @ref NRF_SOCKET_TLS_MAX_SEC_TAG_LIST_SIZE tags can be set per socket.
 */
typedef uint32_t nrf_sec_tag_t;

/** @brief
 * Session cache configuration for the TLS connection.
 *  - 0 - Disabled.
 *  - 1 - Enabled.
 *
 * By default, the session cache is enabled.
 * @note Session cache, may not be used if the peer does not support it.
 *
 * @deprecated since v2.6.0, use type int instead.
 */
typedef uint32_t nrf_sec_session_cache_t;

/** @brief
 * Peer verification level for the TLS connection.
 *  - 0 - None.
 *  - 1 - Optional.
 *  - 2 - Required.
 *
 * By default, peer verification is optional.
 *
 * @deprecated since v2.6.0, use type int instead.
 */
typedef uint32_t nrf_sec_peer_verify_t;

/** @brief
 * An IANA cipher suite identifier.
 *
 * @deprecated since v2.6.0, use type int instead.
 */
typedef uint32_t nrf_sec_cipher_t;

/** @} */

/**
 * @defgroup nrf_socket_api nRF Socket interface
 * @{
 */

/**
 * @brief Create a network socket.
 *
 * @details
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
 * @details
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
 * @details
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
 * @details
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
 * @details
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/send.html">
 * POSIX.1-2017 article</a> for normative description.
 *
 * In addition, the function may return -1 and set the following errno:
 * - [NRF_ESHUTDOWN] Modem was shut down.
 * - [NRF_ECANCELED] Operation canceled because of APN rate control.
 * - [NRF_ENOMEM] TCP stream interrupted because of no heap memory.
 * - [NRF_EPROTO] Request failed because DTLS context was serialized.
 */
ssize_t nrf_send(int socket, const void *buffer, size_t length, int flags);

/**
 * @brief Send a message on a socket.
 *
 * @details
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/sendto.html">
 * POSIX.1-2017 article</a> for normative description.
 *
 * In addition, the function may return -1 and set the following errno:
 * - [NRF_ESHUTDOWN] Modem was shut down.
 * - [NRF_ECANCELED] Operation canceled because of APN rate control.
 * - [NRF_ENOMEM] TCP stream interrupted because of no heap memory.
 * - [NRF_EPROTO] Request failed because DTLS context was serialized.
 */
ssize_t nrf_sendto(int socket, const void *message, size_t length, int flags,
		   const struct nrf_sockaddr *dest_addr, nrf_socklen_t dest_len);

/**
 * @brief Receive a message from a connected socket.
 *
 * @details
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
 * @details
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

/** Data other than high-priority data may be read without blocking */
#define NRF_POLLIN 0x1
/** Data may be written without blocking */
#define NRF_POLLOUT 0x4
/** An error has occurred (revents only) */
#define NRF_POLLERR 0x8
/** Device has been disconnected (revents only) */
#define NRF_POLLHUP 0x10
/** Invalid fd member (revents only) */
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
 * @details
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
 * @details
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/setsockopt.html">
 * POSIX.1-2017 article</a> for normative description.
 *
 * @note In Modem library this function supports a subset
 *       of socket options described by POSIX, but also some additional options.
 *
 * In addition, the function may return -1 and set the following errno:
 * - [NRF_EAGAIN] The option could not be set when requested, try again.
 * - [NRF_EDESTADDRREQ] The socket option @ref NRF_SO_RAI_NO_DATA or the socket option
 *                      @ref NRF_SO_RAI with value @ref NRF_RAI_NO_DATA cannot be set
 *                      on a socket that is not connected.
 * - [NRF_EOPNOTSUPP] The option is not supported with the current socket configuration.
 * - [NRF_ESHUTDOWN] Modem was shut down.
 */
int nrf_setsockopt(int socket, int level, int option_name,
		   const void *option_value, nrf_socklen_t option_len);

/**
 * @brief Get the socket options.
 *
 * @details
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
 * @details
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
 * @details
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
 * @details
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
 * @details
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/inet_pton.html">
 * POSIX.1-2017 article</a> for normative description.
 */
int nrf_inet_pton(int af, const char *restrict src, void *restrict dst);

/**
 * @brief Convert IPv4 and IPv6 addresses between binary and text form.
 *
 * @details
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/inet_ntop.html">
 * POSIX.1-2017 article</a> for normative description.
 */
const char *nrf_inet_ntop(int af, const void *restrict src, char *restrict dst, nrf_socklen_t size);

/**
 * @brief Get address information.
 *
 * @details
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
 * @details
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
 * @details
 * Free a linked list of nrf_ifaddrs structures.
 *
 * @param ifa First item in the linked list of interface addresses.
 */
void nrf_freeifaddrs(struct nrf_ifaddrs *ifa);

/**
 * @brief Set a secondary DNS address.
 *
 * @details
 * The secondary DNS address is only used in case the primary DNS address is unreachable,
 * or if no DNS address is provided by the operator. The secondary DNS address does not
 * override the primary DNS address.
 *
 * @note
 * It is not possible to unset a secondary DNS address set using this function.
 *
 * @param family    Address family.
 * @param in_addr   An IPv4 or IPv6 address encoded in a nrf_in_addr or
 *                  nrf_in6_addr structure, respectively.
 * @param in_size   Size of the structure pointed to by in_addr.
 *
 * @retval 0  On success
 * @retval -1 On error, and set @c errno to indicate the reason.
 *
 * The function may return -1 and set the following errno:
 * - [NRF_EPERM] The Modem library is not initialized.
 * - [NRF_EAFNOSUPPORT] The implementation does not support the specified address family.
 * - [NRF_EINVAL] Invalid parameters.
 * - [NRF_ENOBUFS] Not enough shared memory for this request.
 * - [NRF_ESHUTDOWN] Modem was shut down.
 */
int nrf_setdnsaddr(int family, const void *in_addr, nrf_socklen_t in_size);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* NRF_SOCKET_H__ */
/** @} */
