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
 */
#ifndef BSD_LIMITS_H__
#define BSD_LIMITS_H__

#ifdef __cplusplus
extern "C" {
#endif

/**@brief Maximum number of AT and IP sockets available at the same time. */
#define BSD_MAX_SOCKET_COUNT                   8

/**@brief Maximum AT Command Size in bytes. */
#define BSD_AT_MAX_CMD_SIZE                    4096

/**@brief Maximum IP message size in bytes. */
#define BSD_IP_MAX_MESSAGE_SIZE                4096

/**@brief Maximum TLS message size in bytes. */
#define BSD_TLS_MAX_MESSAGE_SIZE               2048

/**
 * @brief Maximum number of PDN connections that can be created.
 */
#define BSD_MAX_PDN_COUNT                      10


#ifdef __cplusplus
}
#endif

#endif // BSD_LIMITS_H__

/**@} */
