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

/**@brief Maximum length of IPv4 in string form, including null-termination character. */
#define NRF_INET_ADDRSTRLEN     16

/**@brief Maximum length of IPv6 in string form, including null-termination character. */
#define NRF_INET6_ADDRSTRLEN    46

/**@defgroup nrf_socket_families Values for nrf_socket_family_t
 * @ingroup nrf_socket
 * @{
 */
#define NRF_AF_LOCAL            1   /**< Family to identify protocols/operations local to Nordic device. */
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

/**@defgroup nrf_socket_mgmt_types Nordic specific extensions of nrf_socket_type_t
 * @ingroup nrf_socket
 * @{
 */
#define NRF_SOCK_MGMT           512 /**< Management socket. Used for system or link management. */
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
#define NRF_PROTO_PDN          514  /**< Identifies socket protocol for PDN management. */
#define NRF_PROTO_DFU          515  /**< Identifies socket protocol to be DFU. */
#define NRF_PROTO_GNSS         516  /**< Identifies socket protocol to be GNSS. */

/**@} */

/**@defgroup nrf_socket_options_secure_sockets Values for Secure Socket options
 * @ingroup nrf_socket
 * @{
 */
#define NRF_SO_SEC_ROLE                 1    /**< Identifies the option used to get and/or set the TLS role on the socket. @ref nrf_sec_role_t for details. */
#define NRF_SO_SEC_TAG_LIST             2    /**< Identifies the option used to get and/or set the security tags to be used on the TLS socket. @ref nrf_sec_tag_t for details. */
#define NRF_SO_SEC_SESSION_CACHE        3    /**< Identifies the option used to get and/or set the choice of session caching used on the TLS socket. @ref nrf_sec_session_cache_t for details. */
#define NRF_SO_SEC_PEER_VERIFY          4    /**< Identifies the option used to get and/or set the choice of peer verification on the TLS socket. @ref nrf_sec_peer_verify_t for details. */
#define NRF_SO_HOSTNAME                 5    /**< Identifies the option used to get and/or set the host name of the peer used for peer verification. Host name is provided a null-terminated string. */
#define NRF_SO_CIPHERSUITE_LIST         6    /**< Identifies the option used to get and/or set the TLS cipher suite on the socket. See @ref nrf_sec_cipher_t for details. */
#define NRF_SO_CIPHER_IN_USE            7    /**< Identifies the option used to get the TLS cipher selected for the session on the socket. See @ref nrf_sec_cipher_t for details. */
/**@} */

/**@defgroup nrf_socket_options_pdn_sockets Values for PDN Socket options
 * @ingroup nrf_socket
 * @{
 */
#define NRF_SO_PDN_AF                   1    /**< Identifies the option used to get/set supported address families on the PDN. @ref nrf_pdn_af_list_t for details. */
#define NRF_SO_PDN_CONTEXT_ID           2    /**< Identifies the option used to get the context id on the PDN. @ref nrf_pdn_context_id_t for details. */
#define NRF_SO_PDN_STATE                3    /**< Identifies the option used to get the PDN state. */
/**@} */

/**@defgroup nrf_socket_options_dfu_sockets Values for DFU Socket options
 * @ingroup nrf_socket
 * @{
 */
#define NRF_SO_DFU_FW_VERSION           1    /**< Identifies the option used to get firmware version. @ref nrf_dfu_fw_version_t for details. */
#define NRF_SO_DFU_RESOURCES            2    /**< Identifies the option used to get resources available for DFU. @ref nrf_dfu_resources_t for details. */
#define NRF_SO_DFU_TIMEO                3    /**< Identifies the option used to get and/or set the timeout to send a DFU fragment. Not implemented. */
#define NRF_SO_DFU_APPLY                4    /**< Identifies the option to set(execute) firmware upgrade. This option has no parameters. */
#define NRF_SO_DFU_REVERT               5    /**< Identifies the option to set(execute) revert the upgraded firmware to the old one. This option has no parameters. */
#define NRF_SO_DFU_BACKUP_DELETE        6    /**< Identifies the option to set(execute) delete any backup firmware. This option has no parameters. */
#define NRF_SO_DFU_OFFSET               7    /**< Identifies the option to get and/or set offset of the downloaded firmware. */
#define NRF_SO_DFU_ERROR                20   /**< Fetch latest DFU error. */
/**@} */

/**@defgroup nrf_socket_options_gnss_sockets Values for GNSS Socket options
 * @ingroup nrf_socket
 * @{
 */
#define NRF_SO_GNSS_FIX_INTERVAL        1    /**< Identifies the option used to set the GNSS fix interval. */
#define NRF_SO_GNSS_FIX_RETRY           2    /**< Identifies the option used to set the GNSS fix retry interval. */
#define NRF_SO_GNSS_SYSTEM              3    /**< Identifies the option used to set and/or get the GNSS system used. See @ref nrf_gnss_system_t for details. */
#define NRF_SO_GNSS_NMEA_MASK           4    /**< Identifies the option used to select the data format of the received data. */
#define NRF_SO_GNSS_ELEVATION_MASK      5    /**< Indicates at which elevation the GPS should stop tracking a satellite. */
#define NRF_SO_GNSS_USE_CASE            6    /**< Indicates the targeted start performance: 0 = single cold start performance targeted, 1 = multiple hot start performance targeted. */
#define NRF_SO_GNSS_START               7    /**< Identifies the option to start the GPS (with no payload). */
#define NRF_SO_GNSS_STOP                8    /**< Identifies the option to stop the GPS (with no payload). */
#define NRF_SO_GNSS_POWER_SAVE_MODE     9    /**< Identifies the option to set power save mode. */
/**@} */

/**@defgroup nrf_gnss_pvt_flags Set of GNSS flags (as bitmask) indicating additional information about the fix
 * @ingroup nrf_socket
 * @{
 */
#define NRF_GNSS_PVT_FLAG_FIX_VALID_BIT      1 /**< Identifies if we have a valid fix. */
#define NRF_GNSS_PVT_FLAG_LEAP_SECOND_VALID  2 /**< Identifies the validity of leap second. */
/**@} */

/**@defgroup nrf_gnss_sv_flags Set of GNSS satellite flags (as bitmask) indicating additional information about satellites being tracked
 * @ingroup nrf_socket
 * @{
 */
#define NRF_GNSS_SV_FLAG_USED_IN_FIX  2 /**< Indicate that the satellite is used in the position calculation. */
#define NRF_GNSS_SV_FLAG_UNHEALTHY    8 /**< Indicate that the satellite is unhealthy. */
/**@} */

/**@defgroup nrf_gnss_agps_data_type Flags indicating which AGPS assistance data set is written to the GPS module
 * @ingroup nrf_socket
 * @{
 */
#define NRF_GNSS_AGPS_UTC_PARAMETERS                   1 /**< GPS UTC assistance AGPS parameters. */
#define NRF_GNSS_AGPS_EPHEMERIDES                      2 /**< GPS ephemeris assistance AGPS parameters. */
#define NRF_GNSS_AGPS_ALMANAC                          3 /**< GPS almanac assistance AGPS parameters. */
#define NRF_GNSS_AGPS_KLOBUCHAR_IONOSPHERIC_CORRECTION 4 /**< GPS ionospheric assistance AGPS parameters, Klobuchar model. */
#define NRF_GNSS_AGPS_NEQUICK_IONOSPHERIC_CORRECTION   5 /**< GPS ionospheric assistance AGPS parameters, NeQuick model. */
#define NRF_GNSS_AGPS_GPS_SYSTEM_CLOCK_AND_TOWS        6 /**< GPS system time and SV TOW assistance AGPS parameter. */
/** @} */

/**@defgroup nrf_nmea_str_mask Set of values (as bitmask) to enable NMEA output strings
 * @ingroup nrf_socket
 * @{
 */
#define NRF_GNSS_NMEA_GGA_MASK     1  /**< Enables Global Positioning System Fix Data. */
#define NRF_GNSS_NMEA_GLL_MASK     2  /**< Enables Geographic Position Latitude/Longitude and time. */
#define NRF_GNSS_NMEA_GSA_MASK     4  /**< Enables DOP and active satellites. */
#define NRF_GNSS_NMEA_GSV_MASK     8  /**< Enables Satellites in view. */
#define NRF_GNSS_NMEA_RMC_MASK     16 /**< Enables Recommended minimum specific GPS/Transit data. */
/** @} */

/**@defgroup nrf_gnss_psm_modes Values indicating the power saving mode used in the GNSS module
 * @ingroup nrf_socket
 * @{
 */
#define NRF_GNSS_PSM_DISABLED                 0 /** No power save mode is enabled. */
#define NRF_GNSS_PSM_DUTY_CYCLING_PERFORMANCE 1 /** Enables duty-cycling performance policy power save mode. */
#define NRF_GNSS_PSM_DUTY_CYCLING_POWER       2 /** Enables duty-cycling power policy power save mode. */
/** @} */

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
#define NRF_SOL_PDN                     514
#define NRF_SOL_DFU                     515
#define NRF_SOL_GNSS                    516
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

/**@defgroup nrf_socket_dns BSD Socket Address Functions (DNS)
 * @ingroup nrf_socket
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

/**@defgroup nrf_socketopt_tls TLS socket option
 * @ingroup nrf_socket
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

#define NRF_IFNAMSIZ 64

/**@brief Data type for network interface. */
struct nrf_ifreq
{
   char ifr_name[NRF_IFNAMSIZ]; /* Interface name */
};
/**@} */

/**@defgroup nrf_socketopt_pdn PDN socket option
 * @ingroup nrf_socket
 * @brief Data types defined to set and get socket options on a PDN socket.
 * @{
 */
/**@brief Defines the format of the address family(ies) for the PDN.
 * @note the size of the list is provided in the optlen field.
 */
typedef nrf_sa_family_t * nrf_pdn_af_list_t;

/**@brief Defines the format for Address family for the PDN. */
typedef uint32_t nrf_pdn_class_t;

/**@brief Defines the format for Context ID for the PDN. */
typedef uint8_t nrf_pdn_context_id_t;

/**@brief Defines the format for the PDN state:
 *        1 means active, 0 otherwise.
 */
typedef uint8_t nrf_pdn_state_t;
/**@} */

/**@defgroup nrf_socketopt_dfu DFU socket option
 * @ingroup nrf_socket
 * @brief Data types defined to set and get socket options on a DFU socket.
 * @{
 */
/**@brief Defines the format of resources available for firmware upgrade.
 */

/**@brief Defines the firmware version.
 *
 * @details The firmware version is as defined by the RFC 4122.
 */
typedef uint8_t nrf_dfu_fw_version_t[36];

/**@brief Defines the format for resources available for firmware upgrade,
 * i.e. available flash size in bytes.
 */
typedef  uint32_t nrf_dfu_resources_t;

/**@brief Defines the format to get/set offset for firmware download. */
typedef uint32_t nrf_dfu_fw_offset_t;

/**@brief DFU socket errors. */
#define DFU_NO_ERROR                      0
#define DFU_RECEIVER_OUT_OF_MEMORY       -1
#define DFU_RECEIVER_BLOCK_TOO_LARGE     -2
#define DFU_INVALID_HEADER_DATA          -3
#define DFU_ERROR_INTERNAL_00            -4
#define DFU_INVALID_DATA                 -5
#define DFU_ERROR_INTERNAL_01            -6
#define DFU_ERROR_INTERNAL_02            -7
#define DFU_ERROR_INTERNAL_03            -8
#define DFU_INVALID_UUID                 -9
#define DFU_INVALID_ADDRESS              -10
#define DFU_AREA_NOT_BLANK               -11
#define DFU_WRITE_ERROR                  -12
#define DFU_ERASE_ERROR                  -13
#define DFU_INVALID_FILE_OFFSET          -14
#define DFU_PROGRESS_LOG_INVALID         -15
#define DFU_INVALID_RESUME_ATTEMPT       -16
#define DFU_ERASE_PENDING                -17
#define DFU_OPERATION_NOT_ALLOWED        -18
#define DFU_INCOMPLETE_DATA              -19
#define DFU_INTERRUPTED_WRITE            -20

/**@brief DFU socket error. */
typedef int nrf_dfu_err_t;

/**@} */

/**@defgroup nrf_socket_gnss_data_frame GNSS data frames
 * @ingroup  nrf_socket
 * @brief    GNSS Data frame formats.
 * @{
 */
typedef struct
{
    uint16_t year;    /**< 4-digit representation (Gregorian calendar). */
    uint8_t  month;   /**< 1...12 */
    uint8_t  day;     /**< 1...31 */
    uint8_t  hour;    /**< 0...23 */
    uint8_t  minute;  /**< 0...59 */
    uint8_t  seconds; /**< 0...59 */
    uint16_t ms;      /**< 0...999 */
} nrf_gnss_datetime_t;

#define NRF_GNSS_MAX_SATELLITES 12

typedef struct
{
    uint16_t sv;        /**< SV number 1...32 for GPS. */
    uint8_t  signal;    /**< Signal type. 0: invalid, 1: GPS L1C/A, other values are reserved for other GNSSes or signals. */
    uint16_t cn0;       /**< 0.1 dB/Hz. */
    int16_t  elevation; /**< SV elevation angle in degrees. */
    int16_t  azimuth;   /**< SV azimuth angle in degrees. */
    uint8_t  flags;     /**< Bit mask of measurement and position computation flags. */
} nrf_gnss_sv_t;

typedef struct
{
    double              latitude; /**< Latitude in degrees. */
    double              longitude;/**< Longitude in degrees. */
    float               altitude; /**< Altitude above WGS-84 ellipsoid in meters. */
    float               accuracy; /**< Accuracy (2D 1-sigma) in meters. */
    float               speed;    /**< Horizontal speed in meters. */
    float               heading;  /**< Heading of user movement in degrees. */
    nrf_gnss_datetime_t datetime;
    float               pdop;     /**< Position dilution of precision. */
    float               hdop;     /**< Horizontal dilution of precision. */
    float               vdop;     /**< Vertical dilution of precision. */
    float               tdop;     /**< Time dilution of precision. */
    uint8_t             flags;    /**< Bit 0 (LSB): fix validity. Bit 1: Leap second validity. Bit 2: If set, the GNSS operation is blocked, for example, by LTE. */
    nrf_gnss_sv_t       sv[NRF_GNSS_MAX_SATELLITES]; /**< Describes up to 12 of the space vehicles used for the measurement. */
} nrf_gnss_pvt_data_frame_t;

#define NRF_GNSS_NMEA_MAX_LEN     83

typedef char nrf_gnss_nmea_data_frame_t[NRF_GNSS_NMEA_MAX_LEN];

#define NRF_GNSS_PVT_DATA_ID  1
#define NRF_GNSS_NMEA_DATA_ID 2

typedef struct
{
    uint8_t data_id;
    union
    {
        nrf_gnss_pvt_data_frame_t  pvt;
        nrf_gnss_nmea_data_frame_t nmea;
    };
} nrf_gnss_data_frame_t;

/** @} */

/**@} */

/**@defgroup nrf_socket_gnss_data_agps AGPS data types
 * @ingroup  nrf_socket
 * @brief    AGPS Data types.
 * @{
 */

/**@brief Type used to select which AGPS data is written to the GPS module.
 * @details Goes into the @c p_servaddr parameter in the @c nrf_sendto function prototype. Possible values:
 *          - @c NRF_GNSS_AGPS_UTC_PARAMETERS
 *          - @c NRF_GNSS_AGPS_EPHEMERIDE
 *          - @c NRF_GNSS_AGPS_ALMANAC
 *          - @c NRF_GNSS_AGPS_KLOBUCHAR_IONOSPHERIC_CORRECTION
 *          - @c NRF_GNSS_AGPS_NEQUICK_IONOSPHERIC_CORRECTION
 *          - @c NRF_GNSS_AGPS_GPS_SYSTEM_CLOCK_AND_TOWS
 */
typedef uint16_t nrf_gnss_agps_data_type_t;

typedef struct
{
    int32_t a1;         /**< First order term of polynomial (sec/sec). Scale factor 2^-50. Range -8388608...8388607 (25 bits). */
    int32_t a0;         /**< Constant term of polynomial (sec). Scale factor 2^-30. */
    uint8_t tot;        /**< UTC reference GPS time-of-week (sec). Scale factor 2^12. Range 0..147. */
    uint8_t wn_t;       /**< UTC reference GPS week number modulo 256. */
    int8_t  delta_tls;  /**< Current or past leap second count (sec). */
    uint8_t wn_lsf;     /**< Leap second reference GPS week number modulo 256. */
    int8_t  dn;         /**< Leap second reference GPS day-of-week (day). Range 0...6. */
    int8_t  delta_tlsf; /**< Current or future leap second count (sec) (total size of the type-specific assistance data). */
} nrf_gnss_agps_data_utc_t;

typedef struct
{
    uint8_t  sv_id;     /**< Satellite ID (dimensionless). Range 1...32. */
    uint8_t  health;    /**< Satellite health (dimensionless). */
    uint16_t iodc;      /**< Issue of data, clock parameters (dimensionless). Range 0...2047 (11 bits). */
    uint16_t toc;       /**< Clock parameters reference GPS time-of-week (sec). Scale factor 2^4. Range 0...37799. */
    int8_t   af2;       /**< Clock drift rate (sec/sec2). Scale factor 2^-55. */
    int16_t  af1;       /**< Clock drift (sec/sec). Scale factor 2^-43. */
    int32_t  af0;       /**< Clock bias (sec). Scale factor 2^-31. Range -2097152...2097151 (22 bit) */
    int8_t   tgd;       /**< Group delay (sec). Scale factor 2^-31. */
    uint8_t  ura;       /**< URA index (dimensionless). Range 0...15. */
    uint8_t  fit_int;   /**< Curve fit interval indication. Range 0...1. */
    uint16_t toe;       /**< Ephemeris parameters reference GPS time-of-week (sec). Scale factor 2^4. Range 0...37799. */
    int32_t  w;         /**< Argument of perigee (semi-circle). Scale factor 2^-31. */
    int16_t  delta_n;   /**< Mean motion difference (semi-circle/sec). Scale factor 2^-43. */
    int32_t  m0;        /**< Mean anomaly at reference time (semi-circle). Scale factor 2^-31. */
    int32_t  omega_dot; /**< Rate of right ascension (semi-circle/sec). Scale factor 2^-43. Range -8388608...8388607 (24 bits). */
    uint32_t e;         /**< Eccentricity (dimensionless). Scale factor 2^-33. */
    int16_t  idot;      /**< Rate of inclination angle (semi-circle/sec). Scale factor 2-43. Range -8192...8191 (14 bits). */
    uint32_t sqrt_a;    /**< Square root of semi-major axis (m). Scale factor 2^-19. */
    int32_t  i0;        /**< Inclination angle at reference time (semi-circle). Scale factor 2^-31. */
    int32_t  omega0;    /**< Longitude of ascending node at weekly epoch (semi-circle). Scale factor 2^-31. */
    int16_t  crs;       /**< Orbit radius, sine harmonic amplitude (m). Scale factor 2^-5. */
    int16_t  cis;       /**< Inclination angle, sine harmonic amplitude (rad). Scale factor 2^-29. */
    int16_t  cus;       /**< Argument of latitude, sine harmonic amplitude (rad). Scale factor 2^-29. */
    int16_t  crc;       /**< Orbit radius, cosine harmonic amplitude (m). Scale factor 2^-5. */
    int16_t  cic;       /**< Inclination angle, cosine harmonic amplitude (rad). Scale factor 2^-29. */
    int16_t  cuc;       /**< Argument of latitude, cosine harmonic amplitude (rad). Scale factor 2^-29. */
} nrf_gnss_agps_data_ephemeris_t;

typedef struct
{
    uint8_t  sv_id;     /**< Satellite ID (dimensionless). Range 1...32. */
    uint8_t  wn;        /**< Almanac reference GPS week number modulo 256. */
    uint8_t  toa;       /**< Almanac reference GPS time-of-week (sec). Scale factor 2^12. Range 0...147. */
    uint8_t  ioda;      /**< Issue of data, almanac (dimensionless). Range 0...3  (2 bits). */
    uint16_t e;         /**< Eccentricity (dimensionless). Scale factor 2^-21. */
    int16_t  delta_i;   /**< Correction to inclination (semi-circle). Scale factor 2^-19. */
    int16_t  omega_dot; /**< Rate of right ascension (semi-circle/sec). Scale factor 2^-38. */
    uint8_t  sv_health; /**< Satellite health (dimensionless) */
    uint32_t sqrt_a;    /**< Square root of semi-major axis (m^(1/2)). Scale factor 2^-11. Range 0...16777215 (24 bit). */
    int32_t  omega0;    /**< Longitude of ascending node at weekly epoch (semi-circle). Scale factor 2^-23. Range -8388608...8388607  (24 bits). */
    int32_t  w;         /**< Argument of perigee (semi-circle). Scale factor 2^-23. */
    int32_t  m0;        /**< Mean anomaly at reference time (semi-circle). Scale factor 2^-23. Range -8388608...8388608 (24 bits). */
    int16_t  af0;       /**< Clock bias (sec). Scale factor 2^-20. Range -1024...1023 (11 bits). */
    int16_t  af1;       /**< Clock drift (sec/sec). Scale factor 2^-38. Range -1024...1023  (11 bits). */
} nrf_gnss_agps_data_almanac_t;

typedef struct
{
    int8_t alpha0; /**< Constant term (sec). Scale factor 2^-30. */
    int8_t alpha1; /**< First-order coefficient (sec/semi-circle). Scale factor 2^-27. */
    int8_t alpha2; /**< Second-order coefficient (sec/semi-circle^2). Scale factor 2^-24. */
    int8_t alpha3; /**< Third-order coefficient (sec/semi-circle^3). Scale factor 2^-24. */
    int8_t beta0;  /**< Constant term (sec). Scale factor 2^11. */
    int8_t beta1;  /**< First-order coefficient (sec/semi-circle). Scale factor 2^14. */
    int8_t beta2;  /**< Second-order coefficient (sec/semi-circle^2). Scale factor 2^16. */
    int8_t beta3;  /**< Third-order coefficient (sec/semi-circle^3). Scale factor 2^16. */
} nrf_gnss_agps_data_klobuchar_t;

typedef struct
{
    int16_t ai0;         /**< Effective ionisation level 1st order parameter (SFU). Scale factor 2^-2. Range 0...2047  (11 bits). */
    int16_t ai1;         /**< Effective ionisation level 2nd order parameter (SFU/deg). Scale factor 2^-8. Range -1024...1023 (11 bits). */
    int16_t ai2;         /**< Effective ionisation level 3rd order parameter (SFU/deg^2). Scale factor 2^-15. Range -8192...8191  (14 bits). */
    uint8_t storm_cond;  /**< Storm condition bit mask indicating the ionospheric storm condition for different regions. */
    uint8_t storm_valid; /**< Storm validity bit mask indicating for which regions the ionospheric storm condition bit is valid. */
} nrf_gnss_agps_data_nequick_t;

typedef struct
{
    uint16_t tlm;   /**< First two bits (MSB) represent the reserved bit and integrity status flag in the telemetry message (TLM) word.
                         The following 14 bits represent the TLM being broadcast by the satellite. */
    uint8_t  flags; /**< Bit 0 (LSB): anti-spoof flag. Bit 1: alert flag. */
} nrf_gnss_agps_data_tow_element_t;

#define NRF_GNSS_AGPS_MAX_SV_TOW 32

typedef struct
{
    uint16_t date_day;     /**< Day number since Jan 6th, 1980 00:00:00 UTC (USNO) */
    uint32_t time_full_s;  /**< Full seconds part of time-of-day (s). Range 0...86399. */
    uint16_t time_frac_ms; /**< Fraction of a second part of time-of-day (ms). Range 0...999. */
    uint32_t sv_mask;      /**< Bit mask indicating the satellite PRNs for which the satellite-specific TOW assistance data is valid. */
    nrf_gnss_agps_data_tow_element_t sv_tow[NRF_GNSS_AGPS_MAX_SV_TOW]; /**< TOW assistance data for PRN n */
} nrf_gnss_agps_data_system_time_and_sv_tow_t;

/** @} */

/**@defgroup nrf_socketopt_gnss GNSS socket option
 * @ingroup nrf_socket
 * @brief Data types defined to set and get socket options on GNSS sockets.
 * @{
 */

/**@brief Defines the interval between each fix in seconds.
 * @details The default interval is 1 second. 0 denotes a single fix.
 */
typedef uint16_t nrf_gnss_fix_interval_t;


/**@brief Defines how long (in seconds) the receiver should try to get a fix.
 * @details The default retry wait time is 60 seconds before it gives up.
            0 denotes an infinite limit.
 */
typedef uint16_t nrf_gnss_fix_retry_t;

/**@brief Defines which GNSS system to use.
 * @details 0 denotes GPS. Currently, only GPS is supported and any other value
 *          returns an error.
 */
typedef uint8_t  nrf_gnss_system_t;

/**@brief Defines which GNSS output format to use.
 * @details 0 denotes RAW GPS data defined in nrf_gnss_raw_data_frame_t.
 *          1 denotes NPEA formated data frames. Any other value returns
 *          an error.
 */
typedef uint8_t  nrf_gnss_data_format_t;

/**@brief Defines at which elevation the GPS should track a satellite.
 * @details This option is used to make the GPS stop tracking GPSes on a
 *          certain elevation, because the information sent from the GPS gets more
 *          inaccurate as it gets closer to the horizon. Acceptable values
 *          are between 0 and 30 degrees.
 */
typedef uint8_t  nrf_gnss_elevation_mask_t;

/**@brief Defines if NMEA frames should be added.
 */
typedef uint16_t nrf_gnss_nmea_mask_t;

/**@brief Defines which power mode policy to use for the GNSS module.
 * @details
 *          - @c NRF_GNSS_PSM_DISABLED for no power mode policy.
 *          - @c NRF_GNSS_PSM_DUTY_CYCLING_PERFORMANCE for low power mode with better performance.
 *          - @c NRF_GNSS_PSM_DUTY_CYCLING_POWER for low power mode with lower power consumption.
 */
typedef uint8_t nrf_gnss_power_save_mode_t;

/**@brief Defines a mask of non-volatile data types to delete.
 * @details
 *          - Bit 0 denotes ephemerides data.
 *          - Bit 1 denotes almanac data (excluding leap second and ionospheric correction parameters).
 *          - Bit 2 denotes ionospheric correction parameters data.
 *          - Bit 3 denotes last good fix (the last position) data.
 *          - Bit 4 denotes GPS time-of-week (TOW) data.
 *          - Bit 5 denotes GPS week number data.
 *          - Bit 6 denotes leap second (UTC parameters) data.
 *          - Bit 7 denotes local clock (TCXO) frequency offset data.
 */
typedef uint32_t nrf_gnss_delete_mask_t;
/** @} */


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
#define NRF_POLLHUP      0x0008    /**< Event to indicate that the polled socket has been closed by the peer. Ignored in requested events. Subsequent calls to read the socket will be possible until all outstanding data has been read, and return zero-length packets (EOF). */
#define NRF_POLLNVAL     0x0010    /**< Event to indicate the polled socket is not open. Is set in returned events to indicate error on a polled socket. Ignored in requested events. */
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
 * @return 1 on success, 0 if p_src does not contain a valid address,
 *                       -1 and errno set in case of error.
 */
int nrf_inet_pton(int family, const char * p_src, void * p_dst);

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
const char * nrf_inet_ntop(int             family,
                           const void    * p_src,
                           char          * p_dst,
                           nrf_socklen_t   size);

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
