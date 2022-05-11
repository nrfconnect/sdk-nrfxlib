/*
 * Copyright (c) 2017 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
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

/**@brief Maximum size in bytes of shared modem and application memory. */
#define NRF_MODEM_MAX_SHMEM_SIZE (128 << 10) /* 128KiB */

/**@brief Maximum TLS message size in bytes. */
#define NRF_MODEM_TLS_MAX_MESSAGE_SIZE 2048

#ifdef __cplusplus
}
#endif

#endif /* NRF_MODEM_LIMITS_H__ */

/**@} */
