/*
 * Copyright (c) 2017 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file bsd.h
 *
 * @defgroup bsd BSD Library Management
 * @ingroup bsd_library
 * @{
 * @brief Application interface offered for management of BSD Library.
 */
#ifndef BSD_H__
#define BSD_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup bsd_modem_dfu BSD Modem DFU
 *           @c bsd_init() return values when executing Modem firmware updates.
 *
 * @ingroup bsd_library
 * @{
 */

/** Modem firmware update successful.
  * The modem will run the updated firmware on reboot.
  */
#define MODEM_DFU_RESULT_OK 0x5500001u
/** Modem firmware update failed.
  * The modem encoutered a fatal internal error during firmware update.
  */
#define MODEM_DFU_RESULT_INTERNAL_ERROR 0x4400001u
/** Modem firmware update failed.
  * The modem encoutered a fatal hardware error during firmware update.
  */
#define MODEM_DFU_RESULT_HARDWARE_ERROR 0x4400002u
/** Modem firmware update failed, due to an authentication error.
  * The modem will automatically run the previous (non-updated)
  * firmware on reboot.
  */
#define MODEM_DFU_RESULT_AUTH_ERROR 0x4400003u
/** Modem firmware update failed, due to UUID mismatch.
  * The modem will automatically run the previous (non-updated)
  * firmware on reboot.
  */
#define MODEM_DFU_RESULT_UUID_ERROR 0x4400004u

/**@} */

/**@brief Structure for bsd library initialization parameters. */
typedef struct
{
    bool       trace_on;           /**< Indicates the traces are in use. */
    uint32_t   bsd_memory_address; /**< Defines the bsd library memory address. */
    uint32_t   bsd_memory_size;    /**< Defines the bsd library memory size. */
} bsd_init_params_t;

/**
 * @brief Initialize the library.
 *
 * Once initialized, the library uses the resources defined in bsd_platform.h.
 *
 * @param[in] p_init_params Parameters to initialize bsd library.
 *                          Cannot be NULL.
 *
 * @retval Zero on success.
 * @retval A positive value from @ref bsd_modem_dfu when executing
 *         Modem firmware updates.
 *
 * @retval -1 on error.
 */
int bsd_init(const bsd_init_params_t * p_init_params);


/**
 * @brief Shutdown the library.
 *
 * Resources reserved by the library in bsd_platform.h are freed when
 * the library is shutdown.
 *
 * @retval Zero on success.
 * @retval -1 on error.
 */
int bsd_shutdown(void);


/**
 * @brief Handler for BSD library errors.
 *
 * @param[in] error The error reason.
 */
extern void bsd_recoverable_error_handler(uint32_t error);

#ifdef __cplusplus
}
#endif

#endif // BSD_H__

/**@} */
