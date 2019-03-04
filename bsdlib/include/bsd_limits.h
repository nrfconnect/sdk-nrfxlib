/*
 * Copyright (c) 2017 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file bsd_limits.h
 *
 * @defgroup bsd_limits Limits of the BSD library.
 * @ingroup bsd_library
 * @{
 * @brief Upper and lower bound limits of BSD library.
 *
 * @details This module provides:
 * -# A common entry point for limits specific system initializations.
 * -# Limits specific parameters:
 *         -# Maximum number of concurrent sockets in the system.
 *         -# Maximum number of concurrent secure sockets in the system.
 *         -# Maximum number AT sockets and subscriptions for AT events in the system.
 *         -# ..
 */
#ifndef BSD_LIMITS_H__
#define BSD_LIMITS_H__

#ifdef __cplusplus
extern "C" {
#endif

/**@brief Maximum number of sockets of IP sockets. */
#define BSD_MAX_IP_SOCKET_COUNT                8

/**@brief Maximum AT sockets. */
#define BSD_MAX_AT_SOCKET_COUNT                8

/**
 * @brief Maximum number of sockets available in the system.
 *
 * Maximum number of sockets that can be opened using the BSD library.
 * The socket may be of any type. IPv4, IPV6, AT etc. This upper bound applies
 * irrespective of which type of socket was in use.
 *
 * In short, this value is, MAX(BSD_MAX_IP_SOCKET_COUNT, BSD_MAX_AT_SOCKET_COUNT).
 */
#define BSD_MAX_SOCKET_COUNT                   8

/**@brief Maximum AT Command Size in bytes. */
#define BSD_AT_MAX_CMD_SIZE                    4096

/**
 * @brief Maximum number of PDN connections that can be created.
 *
 * Maximum number of concurrent PDN connections that can be created and managed on the system.
 * This number include any default PDNs.
 *
 */
#define BSD_MAX_PDN_COUNT                      10


#ifdef __cplusplus
}
#endif

#endif // BSD_LIMITS_H__

/**@} */
