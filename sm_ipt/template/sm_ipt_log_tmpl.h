/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef SM_IPT_LOG_H_
#define SM_IPT_LOG_H_

/*
 * THIS IS A TEMPLATE FILE.
 * It should be copied to a suitable location within the host environment into
 * which Remote Procedure serialization is integrated, and the following macros
 * should be provided with appropriate implementations.
 */

/**
 * @defgroup sm_ipt_log Logging functionality for SM IPT
 * @{
 * @ingroup sm_ipt
 *
 * @brief Logging functionality for SM IPT
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
#define SM_IPT_ERR(...)

/**
 * @brief Macro for logging a message with the severity level WRN.
 *
 * @param ... printf-style format string, optionally followed by arguments
 *            to be formatted and inserted in the resulting string.
 */
#define SM_IPT_WRN(...)

/**
 * @brief Macro for logging a message with the severity level INF.
 *
 * @param ... printf-style format string, optionally followed by arguments
 *            to be formatted and inserted in the resulting string.
 */
#define SM_IPT_INF(...)

/**
 * @brief Macro for logging a message with the severity level DBG.
 *
 * @param ... printf-style format string, optionally followed by arguments
 *            to be formatted and inserted in the resulting string.
 */
#define SM_IPT_DBG(...)


#ifdef __cplusplus
}
#endif

/**
 *@}
 */

#endif /* SM_IPT_LOG_H_ */
