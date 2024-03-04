/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file nrf_modem/include/nrf_gai_errors.h
 *
 * @defgroup nrf_gai_errors Integer values for getaddrinfo errors
 * @{
 * @brief Defines integer values for getaddrinfo errors.
 */
#ifndef NRF_GAI_ERRORS_H__
#define NRF_GAI_ERRORS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define NRF_EAI_BADFLAGS        -1   /**< Invalid value for `ai_flags' field. */
#define NRF_EAI_NONAME          -2   /**< NAME or SERVICE is unknown */
#define NRF_EAI_AGAIN           -3   /**< Temporary failure in name resolution */
#define NRF_EAI_FAIL            -4   /**< Non-recoverable failure in name res. */
#define NRF_EAI_NODATA          -5   /**< No address associated with NAME. */
#define NRF_EAI_FAMILY          -6   /**< `ai_family' not supported */
#define NRF_EAI_SOCKTYPE        -7   /**< `ai_socktype' not supported */
#define NRF_EAI_SERVICE         -8   /**< SRV not supported for `ai_socktype' */
#define NRF_EAI_ADDRFAMILY      -9   /**< Address family for NAME not supported. */
#define NRF_EAI_MEMORY          -10  /**< Memory allocation failure */
#define NRF_EAI_SYSTEM          -11  /**< System error returned in `errno' */
#define NRF_EAI_INPROGRESS      -100 /**< Processing request in progress */

#ifdef __cplusplus
}
#endif

#endif /* NRF_GAI_ERRORS_H__ */
/** @} */
