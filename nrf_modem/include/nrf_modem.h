/*
 * Copyright (c) 2017-2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file nrf_modem.h
 *
 * @defgroup nrf_modem Modem library
 * @{
 * @brief Application interface offered for management of the Modem library.
 */
#ifndef NRF_MODEM_H__
#define NRF_MODEM_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup nrf_modem_dfu Modem DFU
 *           @c nrf_modem_init() return values when executing Modem firmware updates.
 *
 * @ingroup nrf_modem
 * @{
 */

/** Modem firmware update successful.
  * The modem will run the updated firmware on reboot.
  */
#define MODEM_DFU_RESULT_OK 0x5500001u
/** Modem firmware update failed.
  * The modem encountered a fatal internal error during firmware update.
  */
#define MODEM_DFU_RESULT_INTERNAL_ERROR 0x4400001u
/** Modem firmware update failed.
  * The modem encountered a fatal hardware error during firmware update.
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

/**@brief Modem library initialization parameters. */
typedef struct {
	/** Indicates the traces are in use. */
	bool trace_on;
	/** Defines the library memory address. */
	uint32_t memory_address;
	/** Defines the library memory size. */
	uint32_t memory_size;
} nrf_modem_init_params_t;

/**
 * @brief Initialize the Modem library.
 *
 * Once initialized, the library uses the resources defined in nrf_modem_platform.h.
 *
 * @param[in] p_init_params Initialization parameters.
 *                          Cannot be NULL.
 *
 * @retval Zero on success.
 * @retval A positive value from @ref nrf_modem_dfu when executing
 *         Modem firmware updates.
 *
 * @retval -1 on error.
 */
int nrf_modem_init(const nrf_modem_init_params_t *p_init_params);

/**
 * @brief Shutdown the Modem library.
 *
 * Resources reserved by the library in nrf_modem_platform.h are freed when
 * the library is shutdown.
 *
 * @retval Zero on success.
 * @retval -1 on error.
 */
int nrf_modem_shutdown(void);

/**
 * @brief Handler for library errors.
 *
 * @param[in] error The error reason.
 */
extern void nrf_modem_recoverable_error_handler(uint32_t error);

#ifdef __cplusplus
}
#endif

#endif /* NRF_MODEM_H__ */

/**@} */
