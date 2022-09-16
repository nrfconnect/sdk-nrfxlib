/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef NRF_GAI_ERRORS_H__
#define NRF_GAI_ERRORS_H__
/**
 * @file nrf_modem/include/nrf_gai_errors.h
 * @brief Defines integer values for getaddrinfo errors.
 *
 * @defgroup nrf_gai_errors Integer values for getaddrinfo errors
 */
#ifdef __cplusplus
extern "C" {
#endif

#define NRF_EAI_ADDRFAMILY      257 /**< Address family for NAME not supported. UNUSED */
#define NRF_EAI_AGAIN           258 /**< Temporary failure in name resolution UNUSED */
#define NRF_EAI_BADFLAGS        259 /**< Invalid value for `ai_flags' field. UNUSED */
#define NRF_EAI_FAIL            260 /**< Non-recoverable failure in name res. UNUSED */
#define NRF_EAI_FAMILY          270 /**< `ai_family' not supported */
#define NRF_EAI_MEMORY          271 /**< Memory allocation failure */
#define NRF_EAI_NODATA          272 /**< No address associated with NAME. UNUSED */
#define NRF_EAI_NONAME          273 /**< NAME or SERVICE is unknown */
#define NRF_EAI_SERVICE         274 /**< SRV not supported for `ai_socktype' */
#define NRF_EAI_SOCKTYPE        275 /**< `ai_socktype' not supported */
#define NRF_EAI_INPROGRESS      276 /**< Processing request in progress */
#define NRF_EAI_SYSTEM          277 /**< System error returned in `errno' */

#ifdef __cplusplus
}
#endif

#endif /* NRF_GAI_ERRORS_H__ */
