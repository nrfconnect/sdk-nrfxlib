/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file nrf_socket.h
 *
 * @defgroup nrf_socket nRF BSD Socket interface
 * @ingroup nrf_sdk_socket
 * @{
 * @brief Nordic socket interface.
 *
 * @details This module provides the socket interface for writing nRF applications. The API is
 *          designed to be compatible with the POSIX/BSD socket interface for the purpose of
 *          making porting easy.
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

/**@brief Host to network byte-orders on half word. */
//lint -emacro((572),HTONS) // Suppress warning 572 "Excessive shift value"
#define NRF_HTONS(val)  ((uint16_t)((((val) & 0xff00) >> 8) | ((((val) & 0x00ff) << 8))))

/**@brief Host to network byte-orders on full word. */
//lint -emacro((572),HTONL) // Suppress warning 572 "Excessive shift value"
#define NRF_HTONL(val)  ((((uint32_t) (val) & 0xff000000) >> 24)  |                                    \
                         (((uint32_t) (val) & 0x00ff0000) >> 8)   |                                    \
                         (((uint32_t) (val) & 0x0000ff00) << 8)   |                                    \
                         (((uint32_t) (val) & 0x000000ff) << 24))

/**@brief Network to host byte-orders on half word. */
#define NRF_NTOHS(val)  NRF_HTONS(val)

/**@brief Network to host byte-orders on full word. */
#define NRF_NTOHL(val)  NRF_HTONL(val)


#define nrf_htons(x) NRF_HTONS(x)   /**< Convert byte order from host to network (short). */

#define nrf_htonl(x) NRF_HTONL(x)   /**< Convert byte order from host to network (long). */

#define nrf_ntohs(x) NRF_NTOHS(x)   /**< Convert byte order from network to host (short). */

#define nrf_ntohl(x) NRF_NTOHL(x)   /**< Convert byte order from network to host (long). */

/**@defgroup nrf_socket_families Values for nrf_socket_family_t
 * @ingroup nrf_socket
 * @{
 */
#define NRF_AF_INET             2   /**< IPv4 socket family. */
#define NRF_AF_INET6            10  /**< IPv6 socket family. */
#define NRF_AF_LTE              102 /**< Nordic proprietary LTE socket family. */
/**@} */

/**@defgroup nrf_socket_types Values for nrf_socket_type_t
 * @ingroup nrf_socket
 * @{
 */
#define NRF_SOCK_STREAM         1   /**< TCP socket type. */
#define NRF_SOCK_DGRAM          2   /**< UDP socket type. */
/**@} */

/**@defgroup nrf_socket_protocols Values for nrf_socket_protocol_t
 * @ingroup nrf_socket
 * @{
 */
#define NRF_IPPROTO_TCP         1   /**< Use TCP as transport protocol. */
#define NRF_IPPROTO_UDP         2   /**< Use UDP as transport protocol. */
/**@} */

/**@defgroup nrf_socket_secure_protocols Values for Security Protocols
 * @ingroup nrf_socket_protocols
 * @{
 */
#define NRF_SPROTO_TLS1v2      260  /**< Use TLS1v2 as secure transport. */
#define NRF_SPROTO_TLS1v3      261  /**< Use TLS1v3 as secure transport. */
#define NRF_SPROTO_DTLS1v2     270  /**< Use DTLS1v2 as as secure transport. */
/**@} */

/**@defgroup nrf_socket_proprietary_protocols Values for Nordic Proprietary Protocols
 * @ingroup nrf_socket_protocols
 * @{
 */
#define NRF_PROTO_AT           513  /**< Identifies socket protocol to be AT commands. */
/**@} */

/**@defgroup nrf_socket_options_secure_sockets Values for Secure Socket options
 * @ingroup nrf_socket
 * @{
 */
#define NRF_SO_SEC_ROLE                 1    /**< Identifies the option used to get and/or set the TLS role on the socket. @ref nrf_sec_role_t for details. */
#define NRF_SO_SEC_TAG_LIST             2    /**< Identifies the option used to get and/or set the security tags to be used on the TLS socket. @ref nrf_sec_tag_t for details. */
#define NRF_SO_SEC_SESSION_CACHE        3    /**< Identifies the option used to get and/or set the choice of session caching used on the TLS socket. @ref nrf_sec_session_cache_t for details. */
#define NRF_SO_SEC_PEER_VERIFY          4    /**< Identifies the option used to get and/or set the choice of peer verification on the TLS socket. @ref nrf_sec_peer_verify_t for details. */
#define NRF_SO_HOSTNAME                 5    /**< Identifies the option used to get and/or set the host name of the peer used for peer verification. Host name is provided a null terminated string. */
#define NRF_SO_CIPHERSUITE_LIST         6    /**< Identifies the option used to get and/or set the TLS cipher suite on the socket. See @ref nrf_sec_cipher_t for details. */
#define NRF_SO_CIPHER_IN_USE            7    /**< Identifies the option used to get the TLS cipher selected for the session on the socket. See @ref nrf_sec_cipher_t for details. */
/**@} */

/**@defgroup nrf_socket_options_sockets Values for Socket options
 * @ingroup nrf_socket
 * @{
 */
#define NRF_SO_ERROR                    4
#define NRF_SO_RCVTIMEO                 20
#define NRF_SO_BINDTODEVICE             25
/**@} */

/**@defgroup nrf_socket_options_levels Values for Socket option levels
 * @ingroup nrf_socket
 * @{
 */
#define NRF_SOL_SOCKET                  1
#define NRF_SOL_SECURE                  282
/**@} */

/**@defgroup nrf_socket_send_recv_flags Socket send/recv flags
 * @ingroup nrf_socket
 * @{
 */
#define NRF_MSG_DONTROUTE       0x01    /**< Send only to hosts on directly connected networks. */
#define NRF_MSG_DONTWAIT        0x02    /**< Enables non-blocking operation. */
#define NRF_MSG_OOB             0x04    /**< Sends out-of-band data on sockets that support this. */
#define NRF_MSG_PEEK            0x08    /**< Return data from the beginning of receive queue without removing data from the queue. */
#define NRF_MSG_WAITALL         0x10    /**< Request a blocking operation until the request is satisfied. */
/**@} */

/**@defgroup nrf_fcnt_commands fcntl commands
 * @ingroup nrf_socket
 * @{
 */
#define NRF_F_SETFL             1       /**< Set flag. */
#define NRF_F_GETFL             2       /**< Get flag. */
/**@} */

/**@defgroup nrf_fcnt_flags fcntl flags
 * @ingroup nrf_socket
 * @{
 */
#define NRF_O_NONBLOCK          0x01    /**< Use non-blocking I/O. */
/**@} */

/**
 * @brief Socket module size type.
 */
typedef uint32_t nrf_socklen_t;

/**
 * @brief Socket port type.
 */
typedef uint16_t nrf_in_port_t;

/**
 * @brief Structure specifying time interval.
 */
struct nrf_timeval
{
    uint32_t tv_sec;    /**< Time interval seconds. */
    uint32_t tv_usec;   /**< Time interval microseconds. */
};

/**
 * @brief Socket families.
 *
 * @details For a list of valid values, refer to @ref nrf_socket_families.
 */
typedef int nrf_socket_family_t;
typedef nrf_socket_family_t nrf_sa_family_t;

/**
 * @brief Generic socket address.
 *
 * @details Only provided for API compatibility.
 */
struct nrf_sockaddr
{
    uint8_t         sa_len;
    int             sa_family;
    char            sa_data[];
};

/**
 * @brief IPv6 address.
 */
struct nrf_in6_addr
{
    uint8_t s6_addr[16];
};

/**
 * @brief IPv4 address.
 */
typedef uint32_t nrf_in_addr_t;

/**
 * @brief IPv4 address structure.
 */
struct nrf_in_addr
{
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
struct nrf_sockaddr_in6
{
    uint8_t             sin6_len;             /**< Length of this data structure. */
    nrf_sa_family_t     sin6_family;          /**< Socket family. */
    nrf_in_port_t       sin6_port;            /**< Port, in network byte order. */
    uint32_t            sin6_flowinfo;        /**< IPv6 flow info parameters. Not used. */
    struct nrf_in6_addr sin6_addr;            /**< IPv6 address. */
    uint32_t            sin6_scope_id;        /**< IPv6 scope ID. Not used. */
};

/**
 * @brief Address record for IPv4 addresses.
 *
 * @details Contains the address and port of the host. All fields
 *          in this structure are compatible with the POSIX variant for API compatibility.
 */
struct nrf_sockaddr_in
{
    uint8_t            sin_len;               /**< Length of this data structure. */
    nrf_sa_family_t    sin_family;            /**< Socket family. */
    nrf_in_port_t      sin_port;              /**< Port, in network byte order. */
    struct nrf_in_addr sin_addr;              /**< IPv4 address. */
};

typedef struct nrf_sockaddr     nrf_sockaddr_t;
typedef struct nrf_sockaddr_in6 nrf_sockaddr_in6_t;
typedef struct nrf_in6_addr     nrf_in6_addr;
typedef struct nrf_in6_addr     nrf_in6_addr_t;
typedef struct nrf_sockaddr_in  nrf_sockaddr_in_t;

/**@defgroup iot_socket_dns BSD Socket Address Functions (DNS)
 * @ingroup iot_sdk_socket
 * @{
 */
/**@brief Address information. */
struct nrf_addrinfo
{
    int                  ai_flags;            /**< Input flags. */
    int                  ai_family;           /**< Address family of the socket. */
    int                  ai_socktype;         /**< Socket type. */
    int                  ai_protocol;         /**< Protocol of the socket. */
    nrf_socklen_t        ai_addrlen;          /**< Length of the socket address. */
    struct nrf_sockaddr  *ai_addr;            /**< Address of the socket. */
    char                 *ai_canonname;       /**< Canonical name of service location. */
    struct nrf_addrinfo  *ai_next;            /**< Pointer to next in list. */
};
/**@} */

/**@defgroup iot_socketopt_tls TLS socket option
 * @ingroup iot_sdk_socket
 * @brief Data types defined to set and get socket options on a TLS socket.
 * @{
 */

/**@brief Defines TLS role on the socket.
 *
 * @details Valid values for the role include:
 * 0 and 1.
 * 0 implies a TLS client role.
 * 1 implies a TLS server role.
 *
 * @note The default role is the TLS client role.
 */
typedef uint32_t nrf_sec_role_t;

/**@brief Defines the security tag to be used on the TLS socket.
 *
 * @details More than one security tags may be used on a socket.
 *          If more than one tag is used on the socket,
 *          set the array of security tags.
 *          A maximum of 8 tags can be set per socket.
 */
typedef uint32_t nrf_sec_tag_t;

/**@brief Defines session cache option on the TLS socket.
 *
 * @details Valid values used to set the choice of session cache include:
 * 0 and 1.
 * 0 implies session cache is enabled.
 * 1 implies session cache is disabled.
 *
 * @note By default, the session cache is disabled.
 * @note Session cache, even if enabled may not be used if the peer does not support it.
 */
typedef uint8_t nrf_sec_session_cache_t;

/**@brief Defines peer verification options on the TLS socket.
 *
 * @details Valid values for the peer verification option include:
 * 0, 1 and 2.
 * 0 implies no peer verification.
 * 1 implies peer verification is optional.
 * 2 implies peer verification is strict (mandatory).
 *
 * @note By default, peer verification is set to strict.
 */
typedef uint32_t nrf_sec_peer_verify_t;

/**@brief Defines the cipher suite.
 *
 * @details IANA defined values must be used to identify the cipher.
 *          See https://www.iana.org/assignments/tls-parameters/tls-parameters.xhtml#tls-parameters-4 for the list.
 *
 * @note Implementation support only a limited subset of the values.
 */
typedef uint32_t nrf_sec_cipher_t;


/**@brief Data type to combine all security configuration parameters. */
typedef struct
{
    nrf_sec_role_t          role;                    /**< Local role to be played. See @nrf_sec_role_t for details. */
    nrf_sec_peer_verify_t   peer_verify;             /**< Indicates the preference for peer verification. See @nrf_sec_peer_verify_t for details. */
    nrf_sec_session_cache_t session_cache;           /**< Indicates the preference for session caching. See @nrf_sec_session_cache_t for details. */
    uint32_t                cipher_count;            /**< Indicates the number of entries in the cipher list. */
    nrf_sec_cipher_t        *p_cipher_list;          /**< Indicates the list of ciphers to be used for the session. See @nrf_sec_cipher_t for details. */
    uint32_t                 sec_tag_count;          /**< Indicates the number of entries in the sec tag list. */
    nrf_sec_tag_t           *p_sec_tag_list;         /**< Indicates the list of security tags to be used for the session. See @nrf_sec_tag_t for details. */
} nrf_sec_config_t;

/**@} */



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
 * @return A non-negative socket descriptor on success, or -1 on error.
 */
int nrf_socket(int family, int type, int protocol);


/**
 * @brief Function for closing a socket and freeing any resources held by it.
 *
 * @details If the socket is already closed, this function does nothing.
 *
 * @param[in] sock  The socket to close.
 *
 * @return 0 on success, or -1 on error.
 */
int nrf_close(int sock);


/**
 * @brief Function for controlling file descriptor options.
 *
 * @details Set or get file descriptor options or flags. For a list of supported commands, refer
 *          to @ref nrf_fcnt_commands.
 *          For a list of supported flags, refer to @ref nrf_fcnt_flags.
 *
 * @param[in] fd    The descriptor to set options on.
 * @param[in] cmd   The command class for options.
 * @param[in] flags The flags to set.
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
 * @return 0 on success, or -1 on error.
 */
int nrf_connect(int sock, const void * p_servaddr, nrf_socklen_t addrlen);


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
 * @return The number of bytes that were sent on success, or -1 on error.
 */
ssize_t nrf_send(int sock, const void * p_buff, size_t nbytes, int flags);


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
 * @return The number of bytes that were sent on success, or -1 on error.
 */
ssize_t nrf_sendto(int             sock,
                   const void    * p_buff,
                   size_t          nbytes,
                   int             flags,
                   const void    * p_servaddr,
                   nrf_socklen_t   addrlen);


/**
 * @brief Function for writing data to a socket. See \ref nrf_send() for details.
 *
 * @param[in] sock     The socket to write data to.
 * @param[in] p_buff   Buffer containing the data to send.
 * @param[in] nbytes   Size of data contained in p_buff.
 *
 * @return The number of bytes that were sent on success, or -1 on error.
 */
ssize_t nrf_write(int sock, const void * p_buff, size_t nbytes);


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
 * @return The number of bytes that were read, or -1 on error.
 */
ssize_t nrf_recv(int sock, void * p_buff, size_t nbytes, int flags);


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
 * @return The number of bytes that were read, or -1 on error.
 */
ssize_t nrf_recvfrom(int             sock,
                     void          * p_buff,
                     size_t          nbytes,
                     int             flags,
                     void          * p_cliaddr,
                     nrf_socklen_t * p_addrlen);


/**
 * @brief Function for reading data from a socket. See \ref nrf_recv() for details.
 *
 * @param[in]  sock     The socket to receive data from.
 * @param[out] p_buff   Buffer to hold the data to be read.
 * @param[in]  nbytes   Number of bytes to read. Should not be larger than the size of p_buff.
 *
 * @return The number of bytes that were read, or -1 on error.
 */
ssize_t nrf_read(int sock, void * p_buff, size_t nbytes);


/**
 * @defgroup nrf_fd_set_api API for file descriptor set
 * @ingroup nrf_socket
 * @details File descriptor sets are used as input to the nrf_select() function for doing I/O
 *          multiplexing. The maximum number of descriptors contained in a set is defined by
 *          NRF_FD_SETSIZE.
 *
 * @{
 */
typedef uint32_t nrf_fd_set;
#define NRF_FD_ZERO(set)            (*(set) = 0)                   /**< Clear the entire set. */
#define NRF_FD_SET(fd, set)         (*(set) |= (1u << (fd)))       /**< Set a bit in the set. */
#define NRF_FD_CLR(fd, set)         (*(set) &= ~(1u << (fd)))      /**< Clear a bit in the set. */
#define NRF_FD_ISSET(fd, set)       (*(set) & (1u << (fd)))        /**< Check if a bit in the set is set. */
#define NRF_FD_SETSIZE              sizeof(nrf_fd_set)             /**< The max size of a set. */

/**@} */


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
 * @return The number of ready descriptors contained in the descriptor sets on success, or -1 on error.
 */
int nrf_select(int                        nfds,
               nrf_fd_set               * p_readset,
               nrf_fd_set               * p_writeset,
               nrf_fd_set               * p_exceptset,
               const struct nrf_timeval * p_timeout);


/**
 * @defgroup nrf_poll_api Interface for polling for socket events.
 * @ingroup nrf_socket
 * @details Necessary data types, methods and defines to poll for
 *          events on one or more sockets.
 *
 * @{
 */
 struct nrf_pollfd
 {
    int   handle;        /* Socket handle */
    short requested;     /* Requested events, shall be a mask of events defined in @ref nrf_poll_events. */
    short returned;      /* Returned events, is a mask of events defines in  @nrf_poll_events. */
};

/**
 * @defgroup nrf_poll_events Events defined to poll sockets.
 * @ingroup nrf_poll_api
 * Set of events defined as bit mask to poll on sockets.
 * More than one event can be polled on a socket at any given time.
 */
#define NRF_POLLIN       0x0001    /**< Event for data receive. Can be requested and returned. */
#define NRF_POLLOUT      0x0002    /**< Event for data send. Can be requested and returned. */
#define NRF_POLLERR      0x0004    /**< Event for error on the polled socket. Is set in returned events to indicate error on a polled socket. Ignored in requested events. */
#define NRF_POLLNVAL     0x0008    /**< Event to indicate the polled socket is not open. Is set in returned events to indicate error on a polled socket. Ignored in requested events. */
/**@} */


/**
 * @brief Method to poll for events on one or more sockets.
 *
 * @param[inout] An array of sockets, and respective for each socket that the caller polls for.
 *               The occurred events per socket is returned in the requested field of @ref struct nrf_pollfd structure.
 *               Shall not be NULL.
 * @param[in]    Positive number of sockets being polled for events.
 *               Shall not be more than @ref BSD_MAX_SOCKET_COUNT.
 *
 * @param[in]    Timeout in milliseconds.
 *               The methods waits for this time period for the events to occur on the sockets.
 *
 * @retval A positive number less than or equal to nfds indicating sockets on which events occurred.
 *         0 indicates the timed out occurred and no file descriptors were ready.
 *         -1 on error, and errno indicates the reason for failure.
 * @{
 */
int nrf_poll(struct nrf_pollfd * p_fds, uint32_t nfds, int timeout);
/**@} */


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
 * @return 0 on success, or -1 on error.
 */
int nrf_setsockopt(int             sock,
                   int             level,
                   int             optname,
                   const void    * p_optval,
                   nrf_socklen_t   optlen);


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
 * @param[inout]    p_optlen  The size of p_optval. Can be modified to the actual size of p_optval.
 *
 * @return 0 on success, or -1 on error.
 */
int nrf_getsockopt(int             sock,
                   int             level,
                   int             optname,
                   void          * p_optval,
                   nrf_socklen_t * p_optlen);


/**
 * @brief Function for binding a socket to an address and port.
 *
 * @details The provided address must be supported by the socket protocol family.
 *
 * @param[in] sock      The socket descriptor to bind.
 * @param[in] p_myaddr  The address to bind this socket to.
 * @param[in] addrlen   The size of p_myaddr.
 *
 * @return 0 on success, or -1 on error.
 */
int nrf_bind(int sock, const void * p_myaddr, nrf_socklen_t addrlen);


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
 * @return 0 on success, or -1 on error.
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
 * @return  A non-negative client descriptor on success, or -1 on error.
 */
int nrf_accept(int sock, void * p_cliaddr, nrf_socklen_t * p_addrlen);


/**
 * @brief Function for converting a human-readable IP address to a form usable by the socket API.
 *
 * @details This function will convert a string form of addresses and encode it into a byte array.
 *
 * @param[in]  family  Address family. Only NRF_AF_INET supported.
 * @param[in]  p_src   Null-terminated string containing the address to convert.
 * @param[out] p_dst   Pointer to a struct in6_addr where the address will be stored.
 *
 * @return 1 on success, 0 if p_src does not contain a valid address, -1 if family is not a valid address
 *         family.
 */
int nrf_inet_pton(int family, const char * p_src, void * p_dst);


/**@brief Function to resolve the host name into IPv4 and/or IPv6 addresses.
 *
 * @note The memory pointed to by @p pp_res must be freed using
 *       @ref nrf_freeaddrinfo when the address is no longer needed
 *       or before calling @ref nrf_getaddrinfo again.
 *
 * @param[in]  p_node     Host name to resolve.
 * @param[in]  p_service  Service to resolve.
 * @param[in]  p_hints    Any hints to be used for the resolution.
 * @param[out] pp_res     Pointer to the linked list of resolved addresses if the procedure
 *                        was successful.
 *
 * @return 0 if the procedure succeeds, else, an errno indicating the reason for failure.
 */
int nrf_getaddrinfo(const char                *  p_node,
                    const char                *  p_service,
                    const struct nrf_addrinfo *  p_hints,
                    struct nrf_addrinfo       ** pp_res);


/**@brief Function for freeing the memory allocated for the result of @ref nrf_getaddrinfo.
 *
 * When the linked list of resolved addresses created by @ref getaddrinfo
 * is no longer needed, call this function to free the allocated memory.
 *
 * @param[in] p_res  Pointer to the memory to be freed.
 */
void nrf_freeaddrinfo(struct nrf_addrinfo * p_res);

#ifdef __cplusplus
}
#endif

#endif // NRF_SOCKET_H__

/**@} */
