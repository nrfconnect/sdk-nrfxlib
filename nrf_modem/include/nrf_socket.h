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

typedef int32_t ssize_t;

/**@addtogroup nrf_socket_address_resolution
 *@{
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
/** IPv4 socket family. */
#define NRF_AF_INET 1
/** IPv6 socket family. */
#define NRF_AF_INET6 2
/** Raw packet family. */
#define NRF_AF_PACKET 5
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
/**@} */

/**@defgroup nrf_socket_protocols Socket protocols.
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

/**@} */

/**@defgroup nrf_socket_tls TLS socket
 * @brief TLS socket API.
 * @{
 */

/** @brief
 * Write-only socket option to set role for the connection.
 * Accepts an nrf_sec_role_t with values:
 *  - 0 - Client role.
 *  - 1 - Server role.
 */
#define NRF_SO_SEC_ROLE 1

/** @brief
 * Write-only socket option to select the security tags to be used.
 * @sa nrf_sec_tag_t.
 */
#define NRF_SO_SEC_TAG_LIST 2

/** @brief
 * Socket option to control TLS session caching.
 * Accepts an nrf_sec_session_cache_t with values:
 *  - 0 - Disabled.
 *  - 1 - Enabled.
 * @sa nrf_sec_session_cache_t.
 */
#define NRF_SO_SEC_SESSION_CACHE 3

/** @brief
 * Socket option to set peer verification level.
 * This option accepts an nrf_sec_peer_verify_t with values:
 *  - 0 - None
 *  - 1 - Optional
 *  - 2 - Required
 * @sa nrf_sec_peer_verify_t.
 */
#define NRF_SO_SEC_PEER_VERIFY 4

/** @brief
 * Socket option to set the hostname used for peer verification.
 * This option accepts a string containing the hostname, and its length.
 * The length may be set to zero to disable hostname verification.
 */
#define NRF_SO_SEC_HOSTNAME 5

/** @brief
 * Write-only socket option to select which ciphersuites to use.
 * @sa nrf_sec_cipher_t.
 */
#define NRF_SO_SEC_CIPHERSUITE_LIST 6

/** @brief
 * Socket option to retrieve the ciphersuites used during the handshake.
 * Currently unsupported.
 * @sa nrf_sec_cipher_t.
 */
#define NRF_SO_SEC_CIPHER_IN_USE 7

/** @brief
 * Socket option to set DTLS handshake timeout value.
 * Please see @ref nrf_socket_tls_dtls_handshake_timeouts for allowed values.
 */
#define NRF_SO_SEC_DTLS_HANDSHAKE_TIMEO 8

/** @brief
 * Socket option to purge session cache immediately.
 * This option accepts any value.
 */
#define NRF_SO_SEC_SESSION_CACHE_PURGE 9
/**@} */

/**@defgroup nrf_socket_options_sockets Generic socket options
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
/** Bind a socket to network interface identified by a Packet Data Network ID. */
#define NRF_SO_BINDTODEVICE 25
/** Disable ICMP echo replies on both IPv4 and IPv6.
 *  Set to 1 to enable, or to 0 to disable. Default is 0, disabled.
 */
#define NRF_SO_SILENCE_ALL 30
/** Enable ICMP echo reply. Set to 1 to enable, or to 0 to disable. Default is 1, enabled. */
#define NRF_SO_IP_ECHO_REPLY 31
/** Enable ICMPv6 echo reply. Set to 1 to enable, or to 0 to disable. Default is 1, enabled. */
#define NRF_SO_IPV6_ECHO_REPLY 32
/** Configurable TCP server session timeout in minutes.
 *  Range is 0 to 135. 0 is no timeout and 135 is 2 h 15 min. Default is 0 (no timeout).
 */
#define NRF_SO_TCP_SRV_SESSTIMEO 55

/** Release Assistance Indication.
 *  Indicate that the application does not intend to send more data.
 *  This socket option applies immediately and lets the modem exit connected mode more quickly.
 */
#define NRF_SO_RAI_NO_DATA 50

/** Release Assistance Indication.
 *  Indicate that the application does not intend to send more data
 *  after the next call to send() or sendto().
 *  This lets the modem exit connected mode more quickly after sending the data.
 */
#define NRF_SO_RAI_LAST 51

/** Release Assistance Indication.
 *  Indicate that the application is expecting to receive just one data packet
 *  after the next call to send() or sendto().
 *  This lets the modem exit connected mode more quickly after having received the data.
 */
#define NRF_SO_RAI_ONE_RESP 52

/** Release Assistance Indication.
 *  Indicate that the socket is in active use by a client application.
 *  This lets the modem stay in connected mode longer.
 */
#define NRF_SO_RAI_ONGOING 53

/** Release Assistance Indication.
 *  Indicate that the socket is in active use by a server application.
 *  This lets the modem stay in connected mode longer.
 */
#define NRF_SO_RAI_WAIT_MORE 54
/**@} */

/**@defgroup nrf_socket_options_levels Socket option levels enumerator
 * @ingroup nrf_socket_api_enumerators
 * @{
 */
#define NRF_SOL_SOCKET 1
#define NRF_SOL_SECURE 282
/**@} */

/**@defgroup nrf_socket_send_recv_flags Socket send/recv flags.
 *@ingroup nrf_socket_api_enumerators
 * @{
 */
/** Return data from the beginning of receive queue without removing data from the queue. */
#define NRF_MSG_PEEK 0x02
/** Enables non-blocking operation. */
#define NRF_MSG_DONTWAIT 0x40
/** Request a blocking operation until the request is satisfied. */
#define NRF_MSG_WAITALL 0x100
/**@} */

/**@defgroup nrf_fcnt_commands Descriptor manipulate API
 * @brief API used to manipulate the behaviour of IP sockets using nrf_fcntl().
 * @ingroup nrf_socket
 * @{
 */
/** Get flag. */
#define NRF_F_GETFL 3
/** Set flag. */
#define NRF_F_SETFL 4

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
 * Contains the address and port of the host.
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
 * Contains the address and port of the host.
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

typedef uint32_t nrf_socklen_t;
typedef uint32_t nrf_nfds_t;

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

/** Fill in ai_canonname */
#define NRF_AI_CANONNAME 0x2
/** Assume `service` (port) is numeric. */
#define NRF_AI_NUMERICSERV 0x400
/** Assume `service` contains a Packet Data Network (PDN) ID.
 *  When specified together with the NRF_AI_NUMERICSERV flag,
 *  `service` shall be formatted as follows: "port:pdn_id"
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

/**@} */

/**@addtogroup nrf_socket_tls
 * @{
 */

/** @brief
 * TLS role for the connection.
 *  - 0 - TLS client role.
 *  - 1 - TLS server role.
 */
typedef uint32_t nrf_sec_role_t;

/** @brief
 * Security tags used on the TLS socket.
 *
 * More than one security tags may be used on a socket.
 * If more than one tag is used on the socket, pass an array of security tags.
 *
 * A maximum of 8 tags can be set per socket.
 */
typedef uint32_t nrf_sec_tag_t;

/** @brief
 * Session cache configuration for the TLS connection.
 *  - 0 - Disabled.
 *  - 1 - Enabled.
 *
 * By default, the session cache is enabled.
 * @note Session cache, may not be used if the peer does not support it.
 */
typedef uint32_t nrf_sec_session_cache_t;

/** @brief
 * Peer verification level for the TLS connection.
 *  - 0 - None.
 *  - 1 - Optional.
 *  - 2 - Required.
 *
 * By default, peer verification is optional.
 */
typedef uint32_t nrf_sec_peer_verify_t;

/** @brief
 * An IANA cipher suite identifier.
 */
typedef uint32_t nrf_sec_cipher_t;

/**
 * @brief Maximum network interface name size.
 * @deprecated since v1.1.0.
 */
#define NRF_IFNAMSIZ 64

/** @brief Data type for network interface.
 * @deprecated since v1.1.0.
 */
struct nrf_ifreq {
	char ifr_name[NRF_IFNAMSIZ]; /* Interface name */
};
/**@} */

/**@defgroup nrf_socket_api nRF Socket interface
 * @{
 */

/**
 * @brief Create a network socket.
 *
 * @details
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/socket.html">
 * POSIX.1-2017 article</a> for normative description.
 *
 * In addition, the function shall return -1 and set the following errno:
 * [NRF_ESHUTDOWN] Modem was shut down.
 */
int nrf_socket(int family, int type, int protocol);

/**
 * @brief Close a network socket.
 *
 * @details
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/close.html">
 * POSIX.1-2017 article</a> for normative description.
 *
 * In addition, the function shall return -1 and set the following errno:
 * [NRF_ESHUTDOWN] Modem was shut down.
 */
int nrf_close(int fildes);

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
 * @brief Connect a socket.
 *
 * @details
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/connect.html">
 * POSIX.1-2017 article</a> for normative description.
 *
 * In addition, the function shall return -1 and set the following errno:
 * [NRF_ESHUTDOWN] Modem was shut down.
 */
int nrf_connect(int socket, const struct nrf_sockaddr *address, nrf_socklen_t address_len);

/**
 * @brief Send a message on a connected socket.
 *
 * @details
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/send.html">
 * POSIX.1-2017 article</a> for normative description.
 *
 * In addition, the function shall return -1 and set the following errno:
 * [NRF_ESHUTDOWN] Modem was shut down.
 */
ssize_t nrf_send(int socket, const void *buffer, size_t length, int flags);

/**
 * @brief Send a message on a socket.
 *
 * @details
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/sendto.html">
 * POSIX.1-2017 article</a> for normative description.
 *
 * In addition, the function shall return -1 and set the following errno:
 * [NRF_ESHUTDOWN] Modem was shut down.
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
 * In addition, the function shall return -1 and set the following errno:
 * [NRF_ESHUTDOWN] Modem was shut down.
 */
ssize_t nrf_recv(int socket, void *buffer, size_t length, int flags);

/**
 * @brief Receive a message from a socket.
 *
 * @details
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/recvfrom.html">
 * POSIX.1-2017 article</a> for normative description.
 *
 * In addition, the function shall return -1 and set the following errno:
 * [NRF_ESHUTDOWN] Modem was shut down.
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
/**@} */

/**@addtogroup nrf_socket_api
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
 * In addition, the function shall return -1 and set the following errno:
 * [NRF_ESHUTDOWN] Modem was shut down.
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
 * In addition, the function shall return -1 and set the following errno:
 * [NRF_ESHUTDOWN] Modem was shut down.
 */
int nrf_bind(int socket, const struct nrf_sockaddr *address, nrf_socklen_t address_len);

/**
 * @brief Listen for socket connections and limit the queue of incoming connections.
 *
 * @details
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/listen.html">
 * POSIX.1-2017 article</a> for normative description.
 *
 * In addition, the function shall return -1 and set the following errno:
 * [NRF_ESHUTDOWN] Modem was shut down.
 */
int nrf_listen(int sock, int backlog);

/**
 * @brief Accept a new connection a socket.
 *
 * @details
 * See <a href="https://pubs.opengroup.org/onlinepubs/9699919799/functions/accept.html">
 * POSIX.1-2017 article</a> for normative description.
 *
 * In addition, the function shall return -1 and set the following errno:
 * [NRF_ESHUTDOWN] Modem was shut down.
 */
int nrf_accept(int socket, struct nrf_sockaddr *restrict address,
	       nrf_socklen_t *restrict address_len);

/**@} */

/**@} */

/**@defgroup nrf_socket_address_resolution Socket address resolution API
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
 * In addition, the function shall return -1 and set the following errno:
 * [NRF_ESHUTDOWN] Modem was shut down.
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
 * @param[in] family    Address family.
 * @param[in] in_addr   An IPv4 or IPv6 address encoded in a nrf_in_addr or
 *                      nrf_in6_addr structure, respectively.
 * @param[in] in_size   Size of the structure pointed to by in_addr.
 *
 * @retval 0  On success
 * @retval -1 On error, and set @c errno to indicate the reason.
 *
 * The function shall return -1 and set the following errno:
 * [NRF_EPERM] The Modem library is not initialized.
 * [NRF_EAFNOSUPPORT] The implementation does not support the specified address family.
 * [NRF_EINVAL] Invalid parameters.
 * [NRF_ENOBUFS] Not enough shared memory for this request.
 * [NRF_ESHUTDOWN] Modem was shut down.
 */
int nrf_setdnsaddr(int family, const void *in_addr, nrf_socklen_t in_size);

#ifdef __cplusplus
}
#endif

#endif /* NRF_SOCKET_H__ */

/**@} */
