/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef NRF_RPC_LOG_H_
#define NRF_RPC_LOG_H_

/*
 * THIS IS A TEMPLATE FILE.
 * It should be copied to a suitable location within the host environment into
 * which Remote Procedure serialization is integrated, and the following macros
 * should be provided with appropriate implementations.
 */

/**
 * @defgroup nrf_rpc_log Logging functionality for nRF PRC
 * @{
 * @ingroup nrf_rpc
 *
 * @brief Logging functionality for nRF PRC
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Macro for logging a message with the severity level ERR.
 *
 * @param ... printf-style format string, optionally followed by arguments
 *            to be formatted and inserted in the resulting string.
 */
#define NRF_RPC_ERR(...)

/**
 * @brief Macro for logging a message with the severity level WRN.
 *
 * @param ... printf-style format string, optionally followed by arguments
 *            to be formatted and inserted in the resulting string.
 */
#define NRF_RPC_WRN(...)

/**
 * @brief Macro for logging a message with the severity level INF.
 *
 * @param ... printf-style format string, optionally followed by arguments
 *            to be formatted and inserted in the resulting string.
 */
#define NRF_RPC_INF(...)

/**
 * @brief Macro for logging a message with the severity level DBG.
 *
 * @param ... printf-style format string, optionally followed by arguments
 *            to be formatted and inserted in the resulting string.
 */
#define NRF_RPC_DBG(...)

/**
 * @brief Macro for logging a memory dump with the severity level ERR.
 *
 * @param[in] memory Pointer to the memory region to be dumped.
 * @param[in] length Length of the memory region in bytes.
 * @param[in] text   Text describing the dump.
 */
#define NRF_RPC_DUMP_ERR(memory, length, text)

/**
 * @brief Macro for logging a memory dump with the severity level WRN.
 *
 * @param[in] memory Pointer to the memory region to be dumped.
 * @param[in] length Length of the memory region in bytes.
 * @param[in] text   Text describing the dump.
 */
#define NRF_RPC_DUMP_WRN(memory, length, text)

/**
 * @brief Macro for logging a memory dump with the severity level INF.
 *
 * @param[in] memory Pointer to the memory region to be dumped.
 * @param[in] length Length of the memory region in bytes.
 * @param[in] text   Text describing the dump.
 */
#define NRF_RPC_DUMP_INF(memory, length, text)

/**
 * @brief Macro for logging a memory dump with the severity level DBG.
 *
 * @param[in] memory Pointer to the memory region to be dumped.
 * @param[in] length Length of the memory region in bytes.
 * @param[in] text   Text describing the dump.
 */
#define NRF_RPC_DUMP_DBG(memory, length, text)

#ifdef __cplusplus
}
#endif

/**
 *@}
 */

#endif /* NRF_RPC_LOG_H_ */
