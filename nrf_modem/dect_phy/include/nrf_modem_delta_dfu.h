/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file nrf_modem_delta_dfu.h
 *
 * @defgroup nrf_modem_delta_dfu Delta DFU API
 * @{
 * @brief API for Delta DFU.
 */
#ifndef NRF_MODEM_DELTA_DFU_H__
#define NRF_MODEM_DELTA_DFU_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup nrf_modem_delta_dfu_errors DFU errors
 * @brief    Delta DFU errors.
 * @{
 */
#define NRF_MODEM_DELTA_DFU_NO_ERROR 0			/**< No error */
#define NRF_MODEM_DELTA_DFU_RECEIVER_OUT_OF_MEMORY 1	/**< Modem out of memory */
#define NRF_MODEM_DELTA_DFU_RECEIVER_BLOCK_TOO_LARGE 2	/**< Block too large */
#define NRF_MODEM_DELTA_DFU_INVALID_HEADER_DATA 3	/**< Invalid header */
#define NRF_MODEM_DELTA_DFU_ERROR_INTERNAL_00 4		/**< Internal error */
#define NRF_MODEM_DELTA_DFU_INVALID_DATA 5		/**< Invalid patch */
#define NRF_MODEM_DELTA_DFU_ERROR_INTERNAL_01 6		/**< Internal error */
#define NRF_MODEM_DELTA_DFU_ERROR_INTERNAL_02 7		/**< Internal error */
#define NRF_MODEM_DELTA_DFU_ERROR_INTERNAL_03 8		/**< Internal error */
#define NRF_MODEM_DELTA_DFU_INVALID_UUID 9		/**< Wrong patch for current modem */
#define NRF_MODEM_DELTA_DFU_INVALID_ADDRESS 10		/**< Invalid address */
#define NRF_MODEM_DELTA_DFU_AREA_NOT_BLANK 11		/**< Scratch area not erased */
#define NRF_MODEM_DELTA_DFU_WRITE_ERROR 12		/**< Failed to write */
#define NRF_MODEM_DELTA_DFU_ERASE_ERROR 13		/**< Failed to erase */
#define NRF_MODEM_DELTA_DFU_INVALID_FILE_OFFSET 14	/**< Invalid offset */
#define NRF_MODEM_DELTA_DFU_PROGRESS_LOG_INVALID 15	/**< Operation log corrupt */
#define NRF_MODEM_DELTA_DFU_INVALID_RESUME_ATTEMPT 16	/**< Invalid resume attempt */
#define NRF_MODEM_DELTA_DFU_ERASE_PENDING 17		/**< Erase operation is pending */
#define NRF_MODEM_DELTA_DFU_OPERATION_NOT_ALLOWED 18	/**< Operation not allowed */
#define NRF_MODEM_DELTA_DFU_INCOMPLETE_DATA 19		/**< Patch is incomplete */
#define NRF_MODEM_DELTA_DFU_INTERRUPTED_WRITE 20	/**< Write was interrupted */
#define NRF_MODEM_DELTA_DFU_INVALID_OPERATION 21	/**< Invalid operation */
#define NRF_MODEM_DELTA_DFU_ERROR_INTERNAL_04 22	/**< Internal error */
#define NRF_MODEM_DELTA_DFU_NOT_ALLOWED_POFWARN_ON 23	/**< Refused because of POFWARN */
/** @} */


/** @brief Modem UUID length. */
#define NRF_MODEM_DELTA_DFU_UUID_LEN   36

/** @brief Modem UUID. */
struct nrf_modem_delta_dfu_uuid {
	uint8_t data[NRF_MODEM_DELTA_DFU_UUID_LEN];
};

/**
 * @brief Offset value that indicates that the scratch area is dirty.
 *
 * When the scratch area is dirty, it must be erased before a
 * new firmware delta update can take place.
 *
 */
#define NRF_MODEM_DELTA_DFU_OFFSET_DIRTY 2621440

/**
 * @brief Retrieve the modem firmware UUID.
 *
 * @param modem_uuid Modem firmware UUID.
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 * @retval -NRF_EFAULT If @c modem_uuid is NULL.
 * @retval -NRF_ESHUTDOWN Modem was shut down.
 * @returns A positive error code from @ref nrf_modem_delta_dfu_errors,
 *	    if the modem refused the operation.
 */
int nrf_modem_delta_dfu_uuid(struct nrf_modem_delta_dfu_uuid *modem_uuid);

/**
 * @brief Retrieve the offset of the firmware image in the modem DFU area.
 *
 * The offset will be zero if no image is in modem DFU area.
 * The offset will be @c NRF_MODEM_DELTA_DFU_OFFSET_DIRTY if the modem DFU area needs
 * to be erased before a new firmware update can be received.
 *
 * @param off Offset of the firmware image.
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 * @retval -NRF_EFAULT If @c off is NULL.
 * @retval -NRF_ESHUTDOWN Modem was shut down.
 * @returns A positive error code from @ref nrf_modem_delta_dfu_errors,
 *	    if the modem refused the operation.
 */
int nrf_modem_delta_dfu_offset(size_t *off);

/**
 * @brief Retrieve the size of the modem DFU area.
 *
 * The size of the modem DFU area is the size of the FLASH area
 * that the modem can use to receive an incoming firmware update.
 *
 * @param size Size of the modem DFU area.
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 * @retval -NRF_EFAULT If @c size is NULL.
 * @retval -NRF_ESHUTDOWN Modem was shut down.
 * @returns A positive error code from @ref nrf_modem_delta_dfu_errors,
 *	    if the modem refused the operation.
 */
int nrf_modem_delta_dfu_area(size_t *size);

/**
 * @brief Ready the modem to receive a firmware update.
 *
 * Ready the modem to receive a firmware update, acquiring the necessary resources.
 * When a firmware update is not in progress, call @c nrf_modem_delta_dfu_write_done
 * to release all resources.
 *
 * @note All functions of this interface, except @c nrf_modem_delta_dfu_write,
 *	 are available regardless of the initialization status.
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EALREADY Write already initialized.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN Modem was shut down.
 * @returns A positive error code from @ref nrf_modem_delta_dfu_errors,
 *	    if the modem refused the operation.
 */
int nrf_modem_delta_dfu_write_init(void);

/**
 * @brief Write bytes from a delta patch to the modem DFU area.
 *
 * @param src Firmware update bytes.
 * @param len Length of data being written.
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EFAULT If @c src is NULL.
 * @retval -NRF_EINVAL If @c len is zero.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN Modem was shut down.
 * @returns A positive error code from @ref nrf_modem_delta_dfu_errors,
 *	    if the modem refused the operation.
 */
int nrf_modem_delta_dfu_write(const void *src, size_t len);

/**
 * @brief Pause receiving a modem firmware update and release resources.
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN Modem was shut down.
 * @returns A positive error code from @ref nrf_modem_delta_dfu_errors,
 *	    if the modem refused the operation.
 */
int nrf_modem_delta_dfu_write_done(void);

/**
 * @brief Erase the modem DFU area.
 *
 * The modem DFU area must be erased before receiving a new firmware update.
 * This function will return after commencing the erase operation, but it will
 * not wait for its completion. Further calls to the delta DFU APIs will
 * return @c MODEM_DELTA_DFU_ERASE_PENDING as long as the erase operation is
 * pending or ongoing.
 *
 * The application may poll the offset until it returns zero, to determine
 * if the operation has completed.
 *
 * @note The erase operation may take longer when registered to the network
 *	 than when not registered, i.e. in airplane mode.
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN Modem was shut down.
 * @returns A positive error code from @ref nrf_modem_delta_dfu_errors,
 *	    if the modem refused the operation.
 */
int nrf_modem_delta_dfu_erase(void);

/**
 * @brief Schedule execution of firmware update.
 *
 * The modem needs to reboot to run the new firmware.
 * To reboot the modem, uninitialize the Modem library and reinitialize it
 * using the @c nrf_modem_deinit and @c nrf_modem_init respectively.
 *
 * @note The final result of the update operation is reported via
 *	 @c nrf_modem_init.
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN Modem was shut down.
 * @returns A positive error code from @ref nrf_modem_delta_dfu_errors,
 *	    if the modem refused the operation.
 */
int nrf_modem_delta_dfu_update(void);

/**
 * @brief Schedule a rollback to the previous firmware.
 *
 * The modem needs to reboot to run the previous firmware.
 * To reboot the modem, uninitialize the Modem library and reinitialize it
 * using the @c nrf_modem_deinit and @c nrf_modem_init respectively.
 *
 * @note The final result of the rollback operation is reported via
 *	 @c nrf_modem_init.
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN Modem was shut down.
 * @returns A positive error code from @ref nrf_modem_delta_dfu_errors,
 *	    if the modem refused the operation.
 */
int nrf_modem_delta_dfu_rollback(void);

#ifdef __cplusplus
}
#endif

#endif /* NRF_MODEM_DELTA_DFU_H__ */
/** @} */
