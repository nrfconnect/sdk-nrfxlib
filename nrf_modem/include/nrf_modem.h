/*
 * Copyright (c) 2017-2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
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

/** @brief Shared memory configuration.
 *
 *  @note: Only the lower 128k of RAM may be shared with the modem core.
 */
struct nrf_modem_shmem_cfg {
	/** Control memory, used for control structures.
	 *  The size of this area is build constant, and must be equal to
	 *  @c NRF_MODEM_SHMEM_CTRL_SIZE in @file nrf_modem_platform.h
	 */
	struct {
		uint32_t base;
		uint32_t size;
	} ctrl;
	/** TX memory, used to send data to the modem */
	struct {
		uint32_t base;
		uint32_t size;
	} tx;
	/** RX memory, used to receive data from the modem */
	struct {
		uint32_t base;
		uint32_t size;
	} rx;
	/** Trace memory, used to receive traces from the modem */
	struct {
		uint32_t base;
		uint32_t size;
	} trace;
};

/** @brief Modem library initialization parameters. */
typedef struct {
	/** Shared memory configuration */
	struct nrf_modem_shmem_cfg shmem;
	/** IPC IRQ priority */
	uint32_t ipc_irq_prio;
} nrf_modem_init_params_t;

/**@brief Modem library mode */
enum nrf_modem_mode_t {
	/** Normal operation mode */
	NORMAL_MODE,
	/** DFU mode */
	FULL_DFU_MODE,
};

/**
 * @brief Return version string of the Modem library build.
 *
 * @retval Version string if defined.
 * @retval Empty string if version string is not defined.
 */
char *nrf_modem_build_version(void);

/**
 * @brief Initialize the Modem library.
 *
 * Once initialized, the library uses the resources defined in nrf_modem_platform.h.
 *
 * Library has two operation modes, normal and DFU.
 * In normal operation mode, the DFU functionality is disabled.
 *
 * Library can alternatively be initialized in DFU mode, which means that
 * all shared memory regions are now reserved for DFU operation,
 * and therefore no other modem functionality can be used.
 *
 * To switch between DFU and normal modes, nrf_modem_shutdown() should be
 * called in between.
 *
 * @param[in] init_params Initialization parameters.
 *                        Cannot be NULL.
 * @param[in] mode Library mode.
 *
 * @retval Zero on success.
 * @retval A positive value from @ref nrf_modem_dfu when executing
 *         Modem firmware updates.
 *
 * @retval -1 on error.
 */
int nrf_modem_init(const nrf_modem_init_params_t *init_params,
		   enum nrf_modem_mode_t mode);

/**
 * @brief Shutdown the Modem library.
 *
 * @note
 * The modem must be put into offline (CFUN=0) mode before shutting it down.
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
