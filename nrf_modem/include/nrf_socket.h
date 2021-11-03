/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**@file nrf_socket.h
 *
 */
#ifndef NRF_SOCKET_H__
#define NRF_SOCKET_H__

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(__GNUC__) || (__GNUC__ == 0)
typedef int32_t ssize_t;
#else
#include <sys/types.h>
#ifdef __SES_ARM
typedef int32_t ssize_t;
#endif
#endif

/**@addtogroup nrf_socket_address_resolution
 *@{
 */

/**@brief Host to network byte-orders on half word. */
#define NRF_HTONS(val) ((uint16_t)((((val)&0xff00) >> 8) | ((((val)&0x00ff) << 8))))

/**@brief Host to network byte-orders on full word. */
#define NRF_HTONL(val) ((((uint32_t)(val)&0xff000000) >> 24) | \
			(((uint32_t)(val)&0x00ff0000) >> 8) |  \
			(((uint32_t)(val)&0x0000ff00) << 8) |  \
			(((uint32_t)(val)&0x000000ff) << 24))

/**@brief Network to host byte-orders on half word. */
#define NRF_NTOHS(val) NRF_HTONS(val)

/**@brief Network to host byte-orders on full word. */
#define NRF_NTOHL(val) NRF_HTONL(val)

/** Convert byte order from host to network (short). */
#define nrf_htons(x) NRF_HTONS(x)

/** Convert byte order from host to network (long). */
#define nrf_htonl(x) NRF_HTONL(x)

/** Convert byte order from network to host (short). */
#define nrf_ntohs(x) NRF_NTOHS(x)

/** Convert byte order from network to host (long). */
#define nrf_ntohl(x) NRF_NTOHL(x)

/**@brief Maximum length of IPv4 in string form, including null-termination character. */
#define NRF_INET_ADDRSTRLEN 16

/**@brief Maximum length of IPv6 in string form, including null-termination character. */
#define NRF_INET6_ADDRSTRLEN 46

/**@}*/

/**@defgroup nrf_socket_api_enumerators Socket enumerators.
 * @brief Enumerated values that is used as input arguments to multiple socket functions.
 * @{
 */

/**@defgroup nrf_socket_families Socket family.
 * @{
 */
/** Unspecified address family */
#define NRF_AF_UNSPEC 0
/** Family to identify protocols/operations local to Nordic device.
 *  @deprecated since v1.3.0.
 */
#define NRF_AF_LOCAL 1
/** IPv4 socket family. */
#define NRF_AF_INET 2
/** Raw packet family. */
#define NRF_AF_PACKET 5
/** IPv6 socket family. */
#define NRF_AF_INET6 10
/** Nordic proprietary LTE socket family.
 *  @deprecated since v1.3.0.
 */
#define NRF_AF_LTE 102
/**@} */

/**@defgroup nrf_socket_types Socket type.
 * @{
 */
/** TCP socket type. */
#define NRF_SOCK_STREAM 1
/** UDP socket type. */
#define NRF_SOCK_DGRAM 2
/** RAW socket type. */
#define NRF_SOCK_RAW 3

/** Nordic specific management socket. Used for system or link management.
 *  @deprecated since v1.1.0.
 */
#define NRF_SOCK_MGMT 512
/**@} */

/**@defgroup nrf_socket_protocols Socket protocols.
 * @{
 */

/** TCP protocol. */
#define NRF_IPPROTO_TCP 1
/** UDP protocol. */
#define NRF_IPPROTO_UDP 2

/** TLS1v2 protocol. */
#define NRF_SPROTO_TLS1v2 260
/** TLS1v3 protocol. */
#define NRF_SPROTO_TLS1v3 261
/** DTLS1v2 protocol. */
#define NRF_SPROTO_DTLS1v2 270

/** AT command protocol.
 *  @deprecated since v1.3.0.
 */
#define NRF_PROTO_AT 513
/** DFU protocol.
 *  @deprecated since v1.3.0.
 */
#define NRF_PROTO_DFU 515
/**@} */

/**
 * @defgroup nrf_fd_set_api Descriptor sets API
 * @brief Types and macros used to manipulate the input data argument to the nrf_select() function.
 * @details File descriptor sets are used as input to the nrf_select() function for doing I/O
 *          multiplexing. The maximum number of descriptors contained in a set is defined by
 *          NRF_FD_SETSIZE.
 *
 * @{
 */
typedef uint32_t nrf_fd_set;
/** Clear the entire set. */
#define NRF_FD_ZERO(set) (*(set) = 0)
/** Set a bit in the set. */
#define NRF_FD_SET(fd, set) (*(set) |= (1u << (fd)))
/** Clear a bit in the set. */
#define NRF_FD_CLR(fd, set) (*(set) &= ~(1u << (fd)))
/** Check if a bit in the set is set. */
#define NRF_FD_ISSET(fd, set) (*(set) & (1u << (fd)))
/** The max size of a set. */
#define NRF_FD_SETSIZE sizeof(nrf_fd_set)

/**@} */

/**@defgroup nrf_socket_tls TLS socket
 * @brief TLS socket API
 * @{
 */

/**@brief
 * Socket option to set role for the connection.
 * Accepts an nrf_sec_role_t with values:
 *  - 0 - Client role.
 *  - 1 - Server role.
 */
#define NRF_SO_SEC_ROLE 1

/**@brief
 * Socket option to select the security tags to be used.
 * @sa nrf_sec_tag_t.
 */
#define NRF_SO_SEC_TAG_LIST 2

/**@brief
 * Socket option to control TLS session caching.
 * Accepts an nrf_sec_session_cache_t with values:
 *  - 0 - Disabled.
 *  - 1 - Enabled.
 * @sa nrf_sec_session_cache_t.
 */
#define NRF_SO_SEC_SESSION_CACHE 3

/**@brief
 * Socket option to set peer verification level.
 * This option accepts an nrf_sec_peer_verify_t with values:
 *  - 0 - None
 *  - 1 - Optional
 *  - 2 - Required
 * @sa nrf_sec_peer_verify_t.
 */
#define NRF_SO_SEC_PEER_VERIFY 4

/**@brief
 * Socket option to set the hostname used for peer verification.
 * This option accepts a string containing the hostname, and its length.
 * The length may be set to zero to disable hostname verification.
 */
#define NRF_SO_HOSTNAME 5

/**@brief
 * Socket option to select which ciphersuites to use.
 * @sa nrf_sec_cipher_t.
 */
#define NRF_SO_CIPHERSUITE_LIST 6

/**@brief
 * Socket option to retrieve the ciphersuites used during the handshake.
 * Currently unsupported.
 * @sa nrf_sec_cipher_t.
 */
#define NRF_SO_CIPHER_IN_USE 7

/**@brief
 * Socket option to set DTLS handshake timeout value.
 * Please see @ref nrf_socket_tls_dtls_handshake_timeouts for allowed values.
 */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEO 8

/**@brief
 * Socket option to purge session cache immediately.
 * This option accepts any value.
 */
#define NRF_SO_SEC_SESSION_CACHE_PURGE 9
/**@} */

/**@defgroup nrf_socket_dfu DFU socket
 * @brief DFU socket API
 * @deprecated since v1.3.0.
 * @{
 */
/**@brief
 * Socket option to read the modem firmware version (UUID).
 * @sa nrf_dfu_fw_version_t.
 *
 * @deprecated since v1.3.0.
 */
#define NRF_SO_DFU_FW_VERSION 1

/**@brief
 * Socket option to retrieve the size of the largest firmware image
 * that can be transferred to the modem for firmware updates.
 * @sa nrf_dfu_resources_t.
 *
 * @deprecated since v1.3.0.
 */
#define NRF_SO_DFU_RESOURCES 2

/**@brief
 * Socket option to control the timeout to send a firmware fragment.
 * @note Not implemented.
 *
 * @deprecated since v1.3.0.
 */
#define NRF_SO_DFU_TIMEO 3

/**@brief
 * Socket option to schedule a modem firmware update at next boot.
 * The result of the update is returned by nrf_modem_init, at next boot.
 * The modem needs to be reset once more to run the updated firmware.
 *
 * @deprecated since v1.3.0.
 */
#define NRF_SO_DFU_APPLY 4

/**@brief
 * Socket option to schedule a rollback of a firmware update at next boot.
 *
 * @deprecated since v1.3.0.
 */
#define NRF_SO_DFU_REVERT 5

/**@brief
 * Socket option to delete a modem firmware image from the modem's scratch area.
 * This option removes the possibility to rollback to a previous version,
 * and is necessary to receive new firmware images.
 *
 * @deprecated since v1.3.0.
 */
#define NRF_SO_DFU_BACKUP_DELETE 6

/**@brief
 * Socket option read and write the offset of the downloaded firmware image
 * in the modem's scratch area. This option is used to determine whether
 * a firmware image exists in the modem's scratch area and its size.
 * A value of 2.5 megabytes (2621440 bytes) is returned if the scratch area
 * is dirty, and needs erasing (via NRF_SO_DFU_BACKUP_DELETE).
 * If non-zero and different from 2.5 megabytes, the value indicates the size
 * of the firmware image received so far.
 *
 * @deprecated since v1.3.0.
 */
#define NRF_SO_DFU_OFFSET 7

/**@brief
 * Socket option to retrieve the latest DFU error, see @ref nrf_dfu_errors.
 * Read-only.
 * @deprecated since v1.3.0.
 */
#define NRF_SO_DFU_ERROR 20

/** @} */

/**@defgroup nrf_socket_options_sockets Generic socket options
 * @brief Socket options used with both AT and IP sockets
 * @ingroup nrf_socket
 * @{
 */
#define NRF_SO_ERROR		       4
#define NRF_SO_RCVTIMEO		       20
#define NRF_SO_SNDTIMEO		       21
#define NRF_SO_BINDTODEVICE	       25
#define NRF_SO_SILENCE_ALL	       30
/** Boolean control for ICMP echo reply disable/enable (0/1). Default is 1 (enabled). */
#define NRF_SO_IP_ECHO_REPLY	       31
/** Boolean control for ICMPv6 echo reply disable/enable (0/1). Default is 1 (enabled). */
#define NRF_SO_IPV6_ECHO_REPLY	       32
#define NRF_SO_REUSEADDR	       40
/** Release Assistance Indication feature: This will indicate that the next call to send/sendto will
 * be the last one for some time, which means that the modem can get out of connected mode quicker
 * when this data is sent.
 */
#define NRF_SO_RAI_LAST                    50
/** Release Assistance Indication feature: This will indicate that the application will not send any
 * more data. This socket option will apply immediately, and does not require a call to send
 * afterwards.
 */
#define NRF_SO_RAI_NO_DATA                 51
/** Release Assistance Indication feature: This will indicate that after the next call to
 * send/sendto, the application is expecting to receive one more data packet before this socket
 * will not be used again for some time.
 */
#define NRF_SO_RAI_ONE_RESP                52
/** Release Assistance Indication feature: If a client application expects to use the socket more
 * it can indicate that by setting this socket option before the next send call which will keep
 * the modem in connected mode longer.
 */
#define NRF_SO_RAI_ONGOING                 53
/** Release Assistance Indication feature: If a server application expects to use the socket more
 * it can indicate that by setting this socket option before the next send call.
 */
#define NRF_SO_RAI_WAIT_MORE               54
/** Configurable TCP server session timeout in minutes.
 * Range is 0 to 135. 0 is no timeout and 135 is 2 h 15 min. Default is 0 (no timeout).
 */
#define NRF_SO_TCP_SRV_SESSTIMEO           55
/**@} */

/**@defgroup nrf_socket_options_levels Socket option levels enumerator
 * @ingroup nrf_socket_api_enumerators
 * @{
 */
#define NRF_SOL_SOCKET 1
#define NRF_SOL_SECURE 282
/** @deprecated since v1.3.0. */
#define NRF_SOL_DFU    515
/**@} */

/**@defgroup nrf_socket_send_recv_flags Socket send/recv flags.
 *@ingroup nrf_socket_api_enumerators
 * @{
 */
/** Send only to hosts on directly connected networks. */
#define NRF_MSG_DONTROUTE 0x01
/** Enables non-blocking operation. */
#define NRF_MSG_DONTWAIT 0x02
/** Sends out-of-band data on sockets that support this. */
#define NRF_MSG_OOB 0x04
/** Return data from the beginning of receive queue without removing data from the queue. */
#define NRF_MSG_PEEK 0x08
/** Request a blocking operation until the request is satisfied. */
#define NRF_MSG_WAITALL 0x10
/** Control the data truncation. */
#define NRF_MSG_TRUNC 0x20
/**@} */

/**@defgroup nrf_fcnt_commands Descriptor manipulate API
 * @brief API used to manipulate the behaviour of AT and IP sockets using nrf_fcntl().
 * @ingroup nrf_socket
 * @{
 */
/** Set flag. */
#define NRF_F_SETFL 1
/** Get flag. */
#define NRF_F_GETFL 2

/** Use non-blocking I/O. */
#define NRF_O_NONBLOCK 0x01
/**@} */

/**@defgroup nrf_socket_tls_dtls_handshake_timeouts DTLS handshake timeout values
 * @brief Allowed timeout values for DTLS handshake timeout socket option according
 *        to RFC6347 section 4.2.4.1. Default is 123 seconds.
 *        (https://tools.ietf.org/html/rfc6347#section-4.2.4.1)
 * @ingroup nrf_socket_tls
 * @{
 */
/** 1 second */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_1S	1
/** 1s + 2s */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_3S	3
/** 1s + 2s + 4s */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_7S	7
/** 1s + 2s + 4s + 8s */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_15S	15
/** 1s + 2s + 4s + 8s + 16s */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_31S	31
/** 1s + 2s + 4s + 8s + 16s + 32s */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_63S	63
/** 1s + 2s + 4s + 8s + 16s + 32s + 60s */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_123S	123
/**@} */

/**@addtogroup nrf_socket_api
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
/**@} */

/**@addtogroup nrf_socket_address_resolution
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
typedef int nrf_socket_family_t;
typedef nrf_socket_family_t nrf_sa_family_t;

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
 * @details Contains the address and port of the host, as well as other socket options. All fields
 *          in this structure are compatible with the POSIX variant for API compatibility.
 */
struct nrf_sockaddr_in6 {
	/** Length of this data structure. */
	uint8_t sin6_len;
	/** Socket family. */
	nrf_sa_family_t sin6_family;
	/** Port, in network byte order. */
	nrf_in_port_t sin6_port;
	/** IPv6 flow info parameters. Not used. */
	uint32_t sin6_flowinfo;
	/** IPv6 address. */
	struct nrf_in6_addr sin6_addr;
	/** IPv6 scope ID. Not used. */
	uint32_t sin6_scope_id;
};

/**
 * @brief Address record for IPv4 addresses.
 *
 * @details Contains the address and port of the host. All fields
 *          in this structure are compatible with the POSIX variant for API compatibility.
 */
struct nrf_sockaddr_in {
	/** Length of this data structure. */
	uint8_t sin_len;
	/** Socket family. */
	nrf_sa_family_t sin_family;
	/** Port, in network byte order. */
	nrf_in_port_t sin_port;
	/** IPv4 address. */
	struct nrf_in_addr sin_addr;
};

typedef struct nrf_sockaddr nrf_sockaddr_t;
typedef struct nrf_sockaddr_in6 nrf_sockaddr_in6_t;
typedef struct nrf_in6_addr nrf_in6_addr;
typedef struct nrf_in6_addr nrf_in6_addr_t;
typedef struct nrf_sockaddr_in nrf_sockaddr_in_t;

/**
 * @brief Socket module size type.
 */
typedef uint32_t nrf_socklen_t;

/**
 * @brief Generic socket address.
 *
 * @details Only provided for API compatibility.
 */
typedef struct nrf_sockaddr {
	/** Socket address length */
	uint8_t sa_len;
	/** Socket address family */
	int sa_family;
	/** Socket address */
	char sa_data[];
} nrf_sockaddr;

/* Flags for getaddrinfo() hints. */

/** Assume `service` (port) is numeric.
 *  When specified together with the NRF_AI_PDNSERV flag,
 *  `service` shall be formatted as follows: "port:pdn_id"
 *  where "port" is the port number and "pdn_id" is the PDN ID.
 *  Example: "8080:1", port 8080 PDN ID 1.
 *  Example: "42:0", port 42 PDN ID 0.
 */
#define NRF_AI_NUMERICSERV 0x400
/** Assume `service` contains a Packet Data Network (PDN) ID.
 *  When specified together with the NRF_AI_NUMERICSERV flag,
 *  `service` shall be formatted as follows: "port:pdn_id"
 *  where "port" is the port number and "pdn_id" is the PDN ID.
 *  Example: "8080:1", port 8080 PDN ID 1.
 *  Example: "42:0", port 42 PDN ID 0.
 */
#define NRF_AI_PDNSERV 0x1000

/**@brief Address information. */
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

/**@} */

/**@addtogroup nrf_socket_tls
 * @{
 */

/**@brief
 * TLS role for the connection.
 *  - 0 - TLS client role.
 *  - 1 - TLS server role.
 */
typedef uint32_t nrf_sec_role_t;

/**@brief
 * Security tags used on the TLS socket.
 *
 * More than one security tags may be used on a socket.
 * If more than one tag is used on the socket, pass an array of security tags.
 *
 * A maximum of 8 tags can be set per socket.
 */
typedef uint32_t nrf_sec_tag_t;

/**@brief
 * Session cache configuration for the TLS connection.
 *  - 0 - Disabled.
 *  - 1 - Enabled.
 *
 * By default, the session cache is enabled.
 * @note Session cache, may not be used if the peer does not support it.
 */
typedef uint8_t nrf_sec_session_cache_t;

/**@brief
 * Peer verification level for the TLS connection.
 *  - 0 - None.
 *  - 1 - Optional.
 *  - 2 - Required.
 *
 * By default, peer verification is optional.
 */
typedef uint32_t nrf_sec_peer_verify_t;

/**@brief
 * A IANA cipher suite identifier.
 */
typedef uint32_t nrf_sec_cipher_t;

/**@brief Data type to combine all security configuration parameters. */
typedef struct {
	/** Local role to be played. See nrf_sec_role_t for details. */
	nrf_sec_role_t role;
	/** Indicates the preference for peer verification. See nrf_sec_peer_verify_t for details. */
	nrf_sec_peer_verify_t peer_verify;
	/** Indicates the preference for session caching. See nrf_sec_session_cache_t for details. */
	nrf_sec_session_cache_t session_cache;
	/** Indicates the number of entries in the cipher list. */
	uint32_t cipher_count;
	/** Indicates the list of ciphers to be used for the session. See nrf_sec_cipher_t for details. */
	nrf_sec_cipher_t *p_cipher_list;
	/** Indicates the number of entries in the sec tag list. */
	uint32_t sec_tag_count;
	/** Indicates the list of security tags to be used for the session. See nrf_sec_tag_t for details. */
	nrf_sec_tag_t *p_sec_tag_list;
} nrf_sec_config_t;

/**
 * @brief Maximum network interface name size.
 * @deprecated since v1.1.0.
 */
#define NRF_IFNAMSIZ 64

/**@brief Data type for network interface.
 * @deprecated since v1.1.0.
 */
struct nrf_ifreq {
	char ifr_name[NRF_IFNAMSIZ]; /* Interface name */
};
/**@} */

/**@addtogroup nrf_socket_dfu
 * @{
 */

/**@brief
 * Universally unique identifier of the modem firmware version.
 * The UUID format is defined by RFC 4122.
 * @deprecated since v1.3.0.
 */
typedef uint8_t nrf_dfu_fw_version_t[36];

/**@brief
 * Maximum size for a firmware image, in bytes.
 * @deprecated since v1.3.0.
 */
typedef uint32_t nrf_dfu_resources_t;

/**@brief
 * Size of the firmware image stored in flash, in bytes.
 * @deprecated since v1.3.0.
 */
typedef uint32_t nrf_dfu_fw_offset_t;

/**@defgroup nrf_dfu_errors DFU errors
 * @brief    DFU socket errors.
 * @deprecated since v1.3.0.
 * @{
 */

/**@brief DFU socket error.
 * @deprecated since v1.3.0.
 */
typedef int32_t nrf_dfu_err_t;

#define DFU_NO_ERROR		     0
#define DFU_RECEIVER_OUT_OF_MEMORY   -1
#define DFU_RECEIVER_BLOCK_TOO_LARGE -2
#define DFU_INVALID_HEADER_DATA	     -3
#define DFU_ERROR_INTERNAL_00	     -4
#define DFU_INVALID_DATA	     -5
#define DFU_ERROR_INTERNAL_01	     -6
#define DFU_ERROR_INTERNAL_02	     -7
#define DFU_ERROR_INTERNAL_03	     -8
#define DFU_INVALID_UUID	     -9
#define DFU_INVALID_ADDRESS	     -10
#define DFU_AREA_NOT_BLANK	     -11
#define DFU_WRITE_ERROR		     -12
#define DFU_ERASE_ERROR		     -13
#define DFU_INVALID_FILE_OFFSET	     -14
#define DFU_PROGRESS_LOG_INVALID     -15
#define DFU_INVALID_RESUME_ATTEMPT   -16
#define DFU_ERASE_PENDING	     -17
#define DFU_OPERATION_NOT_ALLOWED    -18
#define DFU_INCOMPLETE_DATA	     -19
#define DFU_INTERRUPTED_WRITE	     -20

/** @} */
/** @} */

/**@defgroup nrf_socket_api nRF Socket interface
 * @{
 */

/**
 * @brief Function for creating a socket.
 *
 * @details API to create a socket that can be used for network communication independently
 *          of lower protocol layers.
 *
 * @param[in] family    The protocol family of the network protocol to use.
 * @param[in] type      The protocol type to use for this socket.
 * @param[in] protocol  The transport protocol to use for this socket.
 *
 * @return A non-negative socket descriptor on success.
 *         -1 on error, and errno indicates the reason for failure.
 */
int nrf_socket(int family, int type, int protocol);

/**
 * @brief Function for closing a socket and freeing any resources held by it.
 *
 * @details If the socket is already closed, this function does nothing.
 *
 * @param[in] sock  The socket to close.
 *
 * @return 0 on success.
 *         -1 on error, and errno indicates the reason for failure.
 */
int nrf_close(int sock);

/**
 * @brief Function for controlling file descriptor options.
 *
 * @details Set or get file descriptor options or flags. For a list of supported commands, refer
 *          to @ref nrf_fcnt_commands.
 *          For a list of supported flags, refer to nrf_fcnt_flags.
 *
 * @param[in] fd    The descriptor to set options on.
 * @param[in] cmd   The command class for options.
 * @param[in] flags The flags to set.
 *
 * @return Value dependent on command class:
 *         NRF_F_GETFL - Value of file status flags.
 *         -1 on error, and errno indicates the reason for failure.
 *         0 otherwise.
 */
int nrf_fcntl(int fd, int cmd, int flags);

/**
 * @brief Function for connecting to an endpoint with a given address.
 *
 * @details The socket handle must be a valid handle that has not yet been connected. Running
 *          connect on a connected handle will return an error.
 *
 * @param[in] sock          The socket to use for connection.
 * @param[in] p_servaddr    The address of the server to connect to.
 * @param[in] addrlen       The size of the p_servaddr argument.
 *
 * @return 0 on success.
 *         -1 on error, and errno indicates the reason for failure.
 */
int nrf_connect(int sock, const void *p_servaddr, nrf_socklen_t addrlen);

/**
 * @brief Function for sending data through a socket.
 *
 * @details By default, this function will block unless the NRF_O_NONBLOCK
 *          socket option has been set, OR NRF_MSG_DONTWAIT is passed as a flag. In that case, the
 *          method will return immediately.
 *
 * @param[in] sock     The socket to write data to.
 * @param[in] p_buff   Buffer containing the data to send.
 * @param[in] nbytes   Size of data contained on p_buff.
 * @param[in] flags    Flags to control send behavior.
 *
 * @return The number of bytes that were sent on success.
 *         -1 on error, and errno indicates the reason for failure.
 */
ssize_t nrf_send(int sock, const void *p_buff, size_t nbytes, int flags);

/**
 * @brief Function for sending datagram through a socket.
 *
 * @details By default, this function will block if the lower layers are not able to process the
 *          packet, unless the NRF_O_NONBLOCK socket option has been set, OR NRF_MSG_DONTWAIT is passed as a flag.
 *          In that case, the method will return immediately.
 *
 * @param[in] sock          The socket to write data to.
 * @param[in] p_buff        Buffer containing the data to send.
 * @param[in] nbytes        Size of data contained in p_buff.
 * @param[in] flags         Flags to control send behavior.
 * @param[in] p_servaddr    The address of the server to send to.
 * @param[in] addrlen       The size of the p_servaddr argument.
 *
 * @return The number of bytes that were sent on success.
 *         -1 on error, and errno indicates the reason for failure.
 */
ssize_t nrf_sendto(int sock,
		   const void *p_buff,
		   size_t nbytes,
		   int flags,
		   const void *p_servaddr,
		   nrf_socklen_t addrlen);

/**
 * @brief Function for writing data to a socket. See \ref nrf_send() for details.
 *
 * @param[in] sock     The socket to write data to.
 * @param[in] p_buff   Buffer containing the data to send.
 * @param[in] nbytes   Size of data contained in p_buff.
 *
 * @return The number of bytes that were sent on success.
 *         -1 on error, and errno indicates the reason for failure.
 */
ssize_t nrf_write(int sock, const void *p_buff, size_t nbytes);

/**
 * @brief Function for receiving data on a socket.
 *
 * @details API for receiving data from a socket. By default, this function will block, unless the
 *          NRF_O_NONBLOCK socket option has been set, or NRF_MSG_DONTWAIT is passed as a flag.
 *
 * @param[in]  sock     The socket to receive data from.
 * @param[out] p_buff   Buffer to hold the data to be read.
 * @param[in]  nbytes   Number of bytes to read. Should not be larger than the size of p_buff.
 * @param[in]  flags    Flags to control receive behavior.
 *
 * @return The number of bytes that were read.
 *         -1 on error, and errno indicates the reason for failure.
 */
ssize_t nrf_recv(int sock, void *p_buff, size_t nbytes, int flags);

/**
 * @brief Function for receiving datagram on a socket.
 *
 * @details API for receiving data from a socket. By default, this function will block, unless the
 *          NRF_O_NONBLOCK socket option has been set, or NRF_MSG_DONTWAIT is passed as a flag.
 *
 * @param[in]    sock         The socket to receive data from.
 * @param[out]   p_buff       Buffer to hold the data to be read.
 * @param[in]    nbytes       Number of bytes to read. Should not be larger than the size of p_buff.
 * @param[in]    flags        Flags to control receive behavior.
 * @param[out]   p_cliaddr    Socket address that will be set to the client's address.
 * @param[inout] p_addrlen    The size of the p_cliaddr passed. Might be modified by the function.
 *
 * @return The number of bytes that were read.
 *         -1 on error, and errno indicates the reason for failure.
 */
ssize_t nrf_recvfrom(int sock,
		     void *p_buff,
		     size_t nbytes,
		     int flags,
		     void *p_cliaddr,
		     nrf_socklen_t *p_addrlen);

/**
 * @brief Function for reading data from a socket. See \ref nrf_recv() for details.
 *
 * @param[in]  sock     The socket to receive data from.
 * @param[out] p_buff   Buffer to hold the data to be read.
 * @param[in]  nbytes   Number of bytes to read. Should not be larger than the size of p_buff.
 *
 * @return The number of bytes that were read.
 *         -1 on error, and errno indicates the reason for failure.
 */
ssize_t nrf_read(int sock, void *p_buff, size_t nbytes);

/**
 * @brief Function for waiting for read, write, or exception events on a socket.
 *
 * @details Wait for a set of socket descriptors to be ready for reading, writing, or having
 *          exceptions. The set of socket descriptors is configured before calling this function.
 *          This function will block until any of the descriptors in the set has any of the required
 *          events. This function is mostly useful when using NRF_O_NONBLOCK or NRF_MSG_DONTWAIT
 *          options to enable asynchronous operation.
 *
 * @param[in]    nfds          The highest socket descriptor value contained in the sets.
 * @param[inout] p_readset     The set of descriptors for which to wait for read events. Set to NULL
 *                             if not used.
 * @param[inout] p_writeset    The set of descriptors for which to wait for write events. Set to NULL
 *                             if not used.
 * @param[inout] p_exceptset   The set of descriptors for which to wait for exception events. Set to
 *                             NULL if not used.
 * @param[in]    p_timeout     The timeout to use for select call. Set to NULL if waiting forever.
 *
 * @return The number of ready descriptors contained in the descriptor sets on success.
 *         -1 on error, and errno indicates the reason for failure.
 */
int nrf_select(int nfds,
	       nrf_fd_set *p_readset,
	       nrf_fd_set *p_writeset,
	       nrf_fd_set *p_exceptset,
	       const struct nrf_timeval *p_timeout);

/** @} */

/**
 * @defgroup nrf_socket_api_poll Socket polling API
 * @brief Data types and defines for use with nrf_poll().
 * @details Necessary data types and defines to poll for
 *          events on one or more sockets using nrf_poll().
 *
 * @{
 */

/**
 *  @details This structure is used to describe which events to poll for a given socket. Which is then given as argument
 *           to nrf_poll().
 */
struct nrf_pollfd {
	/** Socket handle. */
	int fd;
	/** Requested events, is a mask of events. */
	short events;
	/** Returned events, is a mask of events. */
	short revents;
};

/** Event for data receive. Can be requested and returned. */
#define NRF_POLLIN 0x0001
/** Event for data send. Can be requested and returned. */
#define NRF_POLLOUT 0x0002
/** Event for error on the polled socket. Is set in returned events to indicate error on a polled socket. Ignored in requested events. */
#define NRF_POLLERR 0x0004
/** Event to indicate that the polled socket has been closed by the peer. Ignored in requested events. Subsequent calls to read the socket will be possible until all outstanding data has been read, and return zero-length packets (EOF). */
#define NRF_POLLHUP 0x0008
/** Event to indicate the polled socket is not open. Is set in returned events to indicate error on a polled socket. Ignored in requested events. */
#define NRF_POLLNVAL 0x0010
/**@} */

/**@addtogroup nrf_socket_api
 * @{
 */

/**
 * @brief Method to poll for events on one or more sockets.
 *
 * @param[in,out] p_fds    An array of sockets, and respective for each socket that the caller polls for.
 *                         The occurred events per socket is returned in the revents field of nrf_pollfd structure.
 *                         Shall not be NULL.
 * @param[in]     nfds     Positive number of sockets being polled for events.
 *                         Shall not be more than NRF_MODEM_MAX_SOCKET_COUNT.
 *
 * @param[in]     timeout  Timeout in milliseconds.
 *                         The methods waits for this time period for the events to occur on the sockets.
 *
 * @return A positive number less than or equal to nfds indicating sockets on which events occurred.
 *         0 indicates the timed out occurred and no file descriptors were ready.
 *         -1 on error, and errno indicates the reason for failure.
 */
int nrf_poll(struct nrf_pollfd *p_fds, uint32_t nfds, int timeout);

/**
 * @brief Function for setting socket options for a given socket.
 *
 * @details The options are grouped by level, and the option value should be the expected for the
 *          given option, and the lifetime must be longer than that of the socket.
 *
 * @param[in] sock      The socket for which to set the option.
 * @param[in] level     The level or group to which the option belongs.
 * @param[in] optname   The name of the socket option.
 * @param[in] p_optval  The value to be stored for this option.
 * @param[in] optlen    The size of p_optval.
 *
 * @return 0 on success.
 *         -1 on error, and errno indicates the reason for failure.
 */
int nrf_setsockopt(int sock,
		   int level,
		   int optname,
		   const void *p_optval,
		   nrf_socklen_t optlen);

/**
 * @brief Function for getting socket options for a given socket.
 *
 * @details The options are grouped by level, and the option value is the value described by the
 *          option name.
 *
 * @param[in]       sock      The socket for which to set the option.
 * @param[in]       level     The level or group to which the option belongs.
 * @param[in]       optname   The name of the socket option.
 * @param[out]      p_optval  Pointer to the storage for the option value.
 * @param[in,out]   p_optlen  The size of p_optval. Can be modified to the actual size of p_optval.
 *
 * @return 0 on success.
 *         -1 on error, and errno indicates the reason for failure.
 */
int nrf_getsockopt(int sock,
		   int level,
		   int optname,
		   void *p_optval,
		   nrf_socklen_t *p_optlen);

/**
 * @brief Function for binding a socket to an address and port.
 *
 * @details The provided address must be supported by the socket protocol family.
 *
 * @param[in] sock      The socket descriptor to bind.
 * @param[in] p_myaddr  The address to bind this socket to.
 * @param[in] addrlen   The size of p_myaddr.
 *
 * @return 0 on success.
 *         -1 on error, and errno indicates the reason for failure.
 */
int nrf_bind(int sock, const void *p_myaddr, nrf_socklen_t addrlen);

/**
 * @brief Function to put the socket in listening mode for incoming connections.
 *
 * @details Once a socket is marked to be in the listening state, it remains a listening socket until closed.
 *          It is important to consider the backlog parameter, as it will affect how much memory your application will
 *          use in the worst case.
 *
 * @param[in] sock      The socket descriptor on which to set the listening options.
 * @param[in] backlog   The max length of the queue of pending connections. A value of 0 means
 *                      infinite.
 *
 * @return 0 on success.
 *         -1 on error, and errno indicates the reason for failure.
 */
int nrf_listen(int sock, int backlog);

/**
 * @brief Function for waiting for the next client to connect.
 *
 * @details This function will block if there are no clients attempting to connect.
 *
 * @param[in]  sock         The socket descriptor to use for waiting on client connections.
 * @param[out] p_cliaddr    Socket address that will be set to the client's address.
 * @param[out] p_addrlen    The size of the p_cliaddr passed. Might be modified by the function.
 *
 * @return A non-negative client descriptor on success.
 *         -1 on error, and errno indicates the reason for failure.
 */
int nrf_accept(int sock, void *p_cliaddr, nrf_socklen_t *p_addrlen);

/**@} */

/**@} */

/**@defgroup nrf_socket_address_resolution Socket address resolution API
 * @brief Address resolution utility functions.
 * @details Utility functions and macros for resolving host name and converting address information between
 *          human readable and a format the library expect.
 * @ingroup nrf_socket
 * @{
 */

/**
 * @brief Function for converting a human-readable IP address to a form usable by the socket API.
 *
 * @details This function will convert a string form of addresses and encode it into a byte
 *          array in network byte order.
 *
 * @note    Currently not supporting mixed IPv4 and IPv6 format strings.
 *
 * @param[in]  family  Address family. NRF_AF_INET or NRF_AF_INET6.
 * @param[in]  p_src   Null-terminated string containing the address to convert.
 * @param[out] p_dst   Pointer to a struct nrf_in_addr or nrf_in6_addr where the address will
 *                     be stored.
 *
 * @return 1 on success
 *         0 if p_src does not contain a valid address,
 *         -1 on error, and errno indicates the reason for failure.
 */
int nrf_inet_pton(int family, const char *p_src, void *p_dst);

/**
 * @brief Function for converting an IP address to a human-readable string form.
 *
 * @details This function will decode the IP bytes from network byte order and convert
 *          it to a string form of the address.
 *
 * @note    Currently not supporting mixed IPv4 and IPv6 format strings.
 *
 * @param[in]  family  Address family. NRF_AF_INET or NRF_AF_INET6.
 * @param[in]  p_src   Pointer to a struct nrf_in_addr or nrf_in6_addr containing the address to convert.
 * @param[out] p_dst   Pointer to a buffer where the string representation of the address will be stored.
 * @param[in]  size    Size of the provided buffer in p_dst.
 *
 * @return Pointer to p_dst on success, NULL and errno set in case of error.
 */
const char *nrf_inet_ntop(int family,
			  const void *p_src,
			  char *p_dst,
			  nrf_socklen_t size);

/**
 * @brief Function to resolve the host name into IPv4 and/or IPv6 addresses.
 *
 * @note The memory pointed to by @p pp_res must be freed using
 *       nrf_freeaddrinfo when the address is no longer needed
 *       or before calling nrf_getaddrinfo again.
 *
 * @param[in]  p_node     Host name to resolve.
 * @param[in]  p_service  Service to resolve.
 * @param[in]  p_hints    Any hints to be used for the resolution.
 * @param[out] pp_res     Pointer to the linked list of resolved addresses if the procedure
 *                        was successful.
 *
 * @return 0 if the procedure succeeds.
 *		   Positive eai error on failiure
 */
int nrf_getaddrinfo(const char *p_node,
		    const char *p_service,
		    const struct nrf_addrinfo *p_hints,
		    struct nrf_addrinfo **pp_res);

/**
 * @brief Function for freeing the memory allocated for the result of nrf_getaddrinfo.
 *
 * @details When the linked list of resolved addresses created by nrf_getaddrinfo
 *          is no longer needed, call this function to free the allocated memory.
 *
 * @param[in] p_res  Pointer to the memory to be freed.
 */
void nrf_freeaddrinfo(struct nrf_addrinfo *p_res);

/**
 * @brief Set a secondary DNS address.
 *
 * @details The secondary DNS address is used automatically in case the primary DNS
 *          address is unreachable, or if no DNS address is provided by the operator.
 *          The secondary DNS address does not override the primary DNS address.
 *
 * @param[in] family    Address family.
 * @param[in] in_addr   An IPv4 or IPv6 address encoded in a nrf_in_addr or
 *                      nrf_in6_addr structure, respectively.
 *                      Pass @c NULL to unset the secondary DNS address.
 * @param[in] in_size   Size of the structure pointed to by in_addr.
 *
 * @return 0 on success,
 *		   -1 on error, and errno indicates the reason for failure.
 */
int nrf_setdnsaddr(int family, const void *in_addr, nrf_socklen_t in_size);

#ifdef __cplusplus
}
#endif

#endif /* NRF_SOCKET_H__ */

/**@} */
