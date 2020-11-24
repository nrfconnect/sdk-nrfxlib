/*
 * Copyright (c) 2017 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file nrf_modem_limits.h
 *
 * @defgroup nrf_modem_limits Limits of the Modem library.
 * @ingroup nrf_modem
 * @{
 * @brief Upper and lower bound limits of the Modem library.
 */
#ifndef NRF_MODEM_LIMITS_H__
#define NRF_MODEM_LIMITS_H__

#ifdef __cplusplus
extern "C" {
#endif

/**@brief Maximum number of AT and IP sockets available at the same time. */
#define NRF_MODEM_MAX_SOCKET_COUNT 8

/**@brief Maximum AT Command Size in bytes. */
#define NRF_MODEM_AT_MAX_CMD_SIZE 4096

/**@brief Maximum IP message size in bytes. */
#define NRF_MODEM_IP_MAX_MESSAGE_SIZE 4096

/**@brief Maximum TLS message size in bytes. */
#define NRF_MODEM_TLS_MAX_MESSAGE_SIZE 2048

/**
 * @brief Maximum number of PDN connections that can be created.
 */
#define NRF_MODEM_MAX_PDN_COUNT 10

#ifdef __cplusplus
}
#endif

#endif /* NRF_MODEM_LIMITS_H__ */

/**@} */
