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

/**
 * @defgroup nrf_modem_fault_handling Modem fault handling
 *
 * @ingroup nrf_modem
 * @{
 */

/** Undefined fault (e.g. undefined interrupt) */
#define NRF_MODEM_FAULT_UNDEFINED 0x001
/** HW WD reset detected */
#define NRF_MODEM_FAULT_HW_WD_RESET 0x002
/** Hard fault */
#define NRF_MODEM_FAULT_HARDFAULT 0x003
/** Memory management fault */
#define NRF_MODEM_FAULT_MEM_MANAGE 0x004
/** Bus fault */
#define NRF_MODEM_FAULT_BUS 0x005
/** Usage fault */
#define NRF_MODEM_FAULT_USAGE 0x006
/** Modem has been reset due to secure ctrl */
#define NRF_MODEM_FAULT_SECURE_RESET 0x007
/** Error handler has crashed */
#define NRF_MODEM_FAULT_PANIC_DOUBLE 0x008
/** Modem startup has failed four times in row */
#define NRF_MODEM_FAULT_PANIC_RESET_LOOP 0x009
/** Assert */
#define NRF_MODEM_FAULT_ASSERT 0x010
/** Unconditional SW reset */
#define NRF_MODEM_FAULT_PANIC 0x011
/** Flash erase fault */
#define NRF_MODEM_FAULT_FLASH_ERASE 0x012
/** Flash write fault */
#define NRF_MODEM_FAULT_FLASH_WRITE 0x013
/** Undervoltage fault */
#define NRF_MODEM_FAULT_POFWARN 0x014
/** Overtemperature fault */
#define NRF_MODEM_FAULT_THWARN 0x015


/** @brief Modem fault info struct. */
struct nrf_modem_fault_info {
	uint32_t reason;
	uint32_t program_counter;
};

/**
 * @brief Modem fault handler.
 */
typedef void (*nrf_modem_fault_handler_t)(struct nrf_modem_fault_info *fault_info);

/**@} */

/** @brief Modem library initialization parameters. */
struct nrf_modem_init_params {
	/** Shared memory configuration */
	struct nrf_modem_shmem_cfg shmem;
	/** IPC IRQ priority */
	uint32_t ipc_irq_prio;
	/** Modem fault handler */
	nrf_modem_fault_handler_t fault_handler;
};

/** @brief Modem library mode */
enum nrf_modem_mode {
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
 * @param[in] mode Library mode.
 *
 * @retval Zero on success.
 * @retval A positive value from @ref nrf_modem_dfu when executing
 *         Modem firmware updates.
 *
 * @retval -NRF_EFAULT @c init_params is @c NULL.
 * @retval -NRF_ENOMEM Not enough shared memory for this operation.
 * @retval -NRF_EPERM The Modem library is already initialized.
 * @retval -NRF_ETIMEDOUT Operation timed out.
 * @retval -NRF_EINVAL RPC control region size is incorrect.
 * @retval -NRF_EOPNOTSUPP RPC version mismatch.
 * @retval -NRF_EIO IPC State fault or missing root digest.
 */
int nrf_modem_init(const struct nrf_modem_init_params *init_params,
		   enum nrf_modem_mode mode);

/**
 * @brief Check whether the modem is initialized.
 *
 * @retval true If the modem is initialized.
 * @retval false If the modem is uninitialized or in a fault state.
 */
bool nrf_modem_is_initialized(void);

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
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_ENOMEM Not enough shared memory for this operation.
 */
int nrf_modem_shutdown(void);

/**
 * @brief Application IRQ handler in the modem library.
 *
 * Call this function when handling the Application IRQ.
 */
void nrf_modem_application_irq_handler(void);

#ifdef __cplusplus
}
#endif

#endif /* NRF_MODEM_H__ */

/**@} */
