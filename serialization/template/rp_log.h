/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef RP_LOG_H_
#define RP_LOG_H_

/*
THIS IS A TEMPLATE FILE.
It should be copied to a suitable location within the host environment into
which Remote Procedure serialization is integrated, and the following macros should be provided with
appropriate implementations.
And this comment should be removed from the customized file.
*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Macro for logging a message with the severity level ERR.
 *
 * @param format printf-style format string, optionally followed by arguments
 *               to be formatted and inserted in the resulting string.
 */
#define RP_LOG_ERR(format, ...)

/**
 * @brief Macro for logging a message with the severity level WRN.
 *
 * @param format printf-style format string, optionally followed by arguments
 *               to be formatted and inserted in the resulting string.
 */
#define RP_LOG_WRN(format, ...)

/**
 * @brief Macro for logging a message with the severity level INF.
 *
 * @param format printf-style format string, optionally followed by arguments
 *               to be formatted and inserted in the resulting string.
 */
#define RP_LOG_INF(format, ...)

/**
 * @brief Macro for logging a message with the severity level DBG.
 *
 * @param format printf-style format string, optionally followed by arguments
 *               to be formatted and inserted in the resulting string.
 */
#define RP_LOG_DBG(format, ...)


/**
 * @brief Macro for logging a memory dump with the severity level ERR.
 *
 * @param[in] p_memory Pointer to the memory region to be dumped.
 * @param[in] length   Length of the memory region in bytes.
 */
#define RP_LOG_HEXDUMP_ERR(p_memory, length)

/**
 * @brief Macro for logging a memory dump with the severity level WRN.
 *
 * @param[in] p_memory Pointer to the memory region to be dumped.
 * @param[in] length   Length of the memory region in bytes.
 */
#define RP_LOG_HEXDUMP_WRN(p_memory, length)

/**
 * @brief Macro for logging a memory dump with the severity level INF.
 *
 * @param[in] p_memory Pointer to the memory region to be dumped.
 * @param[in] length   Length of the memory region in bytes.
 */
#define RP_LOG_HEXDUMP_INF(p_memory, length)

/**
 * @brief Macro for logging a memory dump with the severity level DBG.
 *
 * @param[in] p_memory Pointer to the memory region to be dumped.
 * @param[in] length   Length of the memory region in bytes.
 */
#define RP_LOG_HEXDUMP_DBG(p_memory, length)


/**
 * @brief Macro for getting the textual representation of a given error code.
 *
 * @param[in] error_code Error code.
 *
 * @return String containing the textual representation of the error code.
 */
#define RP_LOG_ERROR_STRING_GET(error_code)

/**
 *@}
 */

#ifdef __cplusplus
}
#endif

#endif /* RP_LOG_H_ */
