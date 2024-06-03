/*
 * Copyright (c) 2017-2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file nrf_modem.h
 *
 * @defgroup nrf_modem Modem library
 * @{
 * @brief Application interface offered for management of the Modem library.
 */
#ifndef NRF_MODEM_H__
#define NRF_MODEM_H__

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup nrf_modem_limits Limits of the Modem library.
 * @ingroup nrf_modem
 * @{
 * @brief Modem library constants.
 */

/** @brief Maximum size in bytes of shared modem and application memory. */
#define NRF_MODEM_MAX_SHMEM_SIZE (128 << 10) /* 128KiB */

/** @} */

/**
 * @defgroup nrf_modem_dfu Modem DFU
 *           @c nrf_modem_init() return values when executing Modem firmware updates.
 *
 * @ingroup nrf_modem
 * @{
 */

/** Modem firmware update successful.
 *  The modem is running the updated modem firmware.
 */
#define NRF_MODEM_DFU_RESULT_OK 0x5500001u
/** Modem firmware update failed.
 *  The modem encountered a fatal internal error during firmware update.
 */
#define NRF_MODEM_DFU_RESULT_INTERNAL_ERROR 0x4400001u
/** Modem firmware update failed.
 *  The modem encountered a fatal hardware error during firmware update.
 */
#define NRF_MODEM_DFU_RESULT_HARDWARE_ERROR 0x4400002u
/** Modem firmware update failed, due to an authentication error.
 *  The modem is running the previous (non-updated) firmware.
 */
#define NRF_MODEM_DFU_RESULT_AUTH_ERROR 0x4400003u
/** Modem firmware update failed, due to UUID mismatch.
 *  The modem is running the previous (non-updated) firmware.
 */
#define NRF_MODEM_DFU_RESULT_UUID_ERROR 0x4400004u
/** Modem firmware update not executed due to low voltage.
 *  The modem will retry the update on reboot.
 */
#define NRF_MODEM_DFU_RESULT_VOLTAGE_LOW 0x4400005u

/** @} */

/**
 * @defgroup nrf_modem_shmem_config Shared memory configuration.
 *           Only the lower 128k of RAM may be shared with the modem core.
 *
 * @ingroup nrf_modem
 * @{
 */

/**
 * @brief Shared memory control region size for Cellular flavour.
 */
#define NRF_MODEM_CELLULAR_SHMEM_CTRL_SIZE 0x4e8
/**
 * @brief Shared memory control region size for DECT PHY flavour.
 */
#define NRF_MODEM_DECT_PHY_SHMEM_CTRL_SIZE 0x728

/** @brief Shared memory configuration in normal operation mode. */
struct nrf_modem_shmem_cfg {
	/** Control memory, used for control structures.
	 *  The size of this area is build constant, and must be equal to
	 *  @c NRF_MODEM_CELLULAR_SHMEM_CTRL_SIZE or @c NRF_MODEM_DECT_PHY_SHMEM_CTRL_SIZE.
	 */
	struct {
		uint32_t base;
		uint32_t size;
	} ctrl;
	/** TX memory, used to send data to the modem.
	 *  The base address must be word-aligned (4 bytes).
	 */
	struct {
		uint32_t base;
		uint32_t size;
	} tx;
	/** RX memory, used to receive data from the modem.
	 *  The base address must be word-aligned (4 bytes).
	 */
	struct {
		uint32_t base;
		uint32_t size;
	} rx;
	/** Trace memory, used to receive traces from the modem.
	 *  The base address must be word-aligned (4 bytes).
	 */
	struct {
		uint32_t base;
		uint32_t size;
	} trace;
};

/** @brief Bootloader shared memory size
 *
 *  @note: The size of this area is build constant.
 */
#define NRF_MODEM_SHMEM_BOOTLOADER_SIZE 0x2018

/** @brief Shared memory configuration in bootloader mode.
 *  The base address must be word-aligned (4 bytes).
 */
struct nrf_modem_bootloader_shmem_cfg {
	uint32_t base;
	uint32_t size;
};

/** @} */

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

/**
 * @brief Modem DFU handler.
 */
typedef void (*nrf_modem_dfu_handler_t)(uint32_t dfu_result);

/** @} */

/** @brief Modem library initialization parameters. */
struct nrf_modem_init_params {
	/** Shared memory configuration */
	struct nrf_modem_shmem_cfg shmem;
	/** IPC IRQ priority */
	uint32_t ipc_irq_prio;
	/** Modem fault handler */
	nrf_modem_fault_handler_t fault_handler;
	/** Modem DFU handler */
	nrf_modem_dfu_handler_t dfu_handler;
};

/** @brief Modem library bootloader initialization parameters. */
struct nrf_modem_bootloader_init_params {
	/** Shared memory configuration */
	struct nrf_modem_bootloader_shmem_cfg shmem;
	/** IPC IRQ priority */
	uint32_t ipc_irq_prio;
	/** Modem fault handler */
	nrf_modem_fault_handler_t fault_handler;
};

/**
 * @brief Return a statically allocated string identifying the Modem library build.
 *
 * @retval Version string if defined.
 * @retval Empty string if version string is not defined.
 */
char *nrf_modem_build_version(void);

/**
 * @brief Initialize the Modem library and turn on the modem.
 *
 * @note
 * To switch between bootloader and normal modes, @c nrf_modem_shutdown must be
 * called in between.
 *
 * @param[in] init_params Initialization parameters.
 *
 * @retval Zero on success.
 *
 * @retval -NRF_EPERM The Modem library is already initialized.
 * @retval -NRF_EFAULT @c init_params is @c NULL.
 * @retval -NRF_ENOLCK Not enough semaphores.
 * @retval -NRF_ENOMEM Not enough shared memory.
 * @retval -NRF_EINVAL Control region size is incorrect or missing handlers in @c init_params.
 * @retval -NRF_ENOTSUPP RPC version mismatch.
 * @retval -NRF_ETIMEDOUT Operation timed out.
 * @retval -NRF_EAGAIN Modem firmware update not executed due to insufficient voltage, try again.
 * @retval -NRF_EIO Modem firmware update failure or modem fault during initialization.
 */
int nrf_modem_init(const struct nrf_modem_init_params *init_params);

/**
 * @brief Initialize the Modem library and turn on the modem in bootloader mode.
 *
 * @note
 * To switch between bootloader and normal modes, @c nrf_modem_shutdown must be
 * called in between.
 *
 * @param[in] init_params Bootloader initialization parameters.
 *
 * @retval Zero on success.
 *
 * @retval -NRF_EPERM The Modem library is already initialized.
 * @retval -NRF_EFAULT @c init_params is @c NULL.
 * @retval -NRF_ENOLCK Not enough semaphores.
 * @retval -NRF_ENOMEM Not enough shared memory.
 * @retval -NRF_EINVAL Missing handler in @c init_params.
 * @retval -NRF_EACCES Bad root digest.
 * @retval -NRF_ETIMEDOUT Operation timed out.
 * @retval -NRF_EIO Bootloader fault.
 * @retval -NRF_ENOSYS Operation not available.
 */
int nrf_modem_bootloader_init(const struct nrf_modem_bootloader_init_params *init_params);

/**
 * @brief Check whether the modem is initialized.
 *
 * @retval true If the modem is initialized.
 * @retval false If the modem is uninitialized or in a fault state.
 */
bool nrf_modem_is_initialized(void);

/**
 * @brief De-initialize the Modem library and turn off the modem.
 *
 * @note
 * If the modem is initialized in normal mode, it must be put into offline mode (CFUN=0)
 * before shutting it down.
 *
 * Resources reserved by the library are freed when the library is shutdown.
 *
 * @retval Zero on success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 */
int nrf_modem_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif /* NRF_MODEM_H__ */
/** @} */
