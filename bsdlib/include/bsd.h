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
 * @brief Application Interface offered for management of BSD Library.
 */
#ifndef BSD_H__
#define BSD_H__

#include <stdint.h>

#include "bsd_limits.h"

#ifdef __cplusplus
extern "C" {
#endif


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

/**
 * @brief Method to initialize BSD library.
 *
 * @details This method shall be called before using any of the other methods of the application.
 *          In case the initialization fails, the call results in a hard fault.
 *
 *          This method shall be called once. Calling this method again with a shutdown results
 *          in undefined behavior.
 *
 *          Initializing the library results in reserving resources defined in bsd_platform.h on
 *          the system. The application shall not use any of the resources identified in
 *          bsd_platform.h.
 *
 * @return Zero on success or an error code otherwise.
 */
int bsd_init(void);


/**
 * @brief Method to gracefully shutdown the BSD library.
 *
 * @details This method used to shutdown the library. Resources reserved by the system may be reused
 *          once the library is gracefully shutdown.
 */
void bsd_shutdown(void);


/**
 * @brief Handler for recoverable BSD library errors.
 *
 * @note  It can be overwritten by the application.
 *        The default will hard fault.
 *
 * @param[in] error Indicates the error that occurred.
 */
extern void bsd_recoverable_error_handler(uint32_t error);


/**
 * @brief Handler for irrecoverable BSD library errors.
 *
 * @note  It can be overwritten by the application.
 *        The default will hard fault.
 *
 * @param[in] error Indicates the error that occurred.
 */
extern void bsd_irrecoverable_error_handler(uint32_t error);

#ifdef __cplusplus
}
#endif

#endif // BSD_H__
/**@} */
