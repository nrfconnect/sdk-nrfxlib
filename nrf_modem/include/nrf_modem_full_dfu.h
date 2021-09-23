/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**@file nrf_modem_full_dfu.h
 * @defgroup nrf_modem_full_dfu Full DFU API
 * @{
 * @brief API for Full DFU.
 */

#ifndef NRF_FULL_DFU_H__
#define NRF_FULL_DFU_H__

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(__GNUC__) || (__GNUC__ == 0)
typedef int32_t ssize_t;
#else
#include <sys/types.h>
#ifdef __SES_ARM
typedef int32_t ssize_t;
#endif
#endif



/**@brief Digest buffer length. */
#define NRF_MODEM_FULL_DFU_DIGEST_LEN 32
/**@brief UUID buffer length. */
#define NRF_MODEM_FULL_DFU_UUID_LEN   36

/**@brief Required shared memory length */
#define NRF_MODEM_FULL_DFU_SHM_LEN 0x201c

/**@brief Storage for 256-bit digest/hash replies.
 * Endianness not converted.
 */
struct nrf_modem_full_dfu_digest {
	uint8_t data[NRF_MODEM_FULL_DFU_DIGEST_LEN];
};

/**@brief Modem UUID response. */
struct nrf_modem_full_dfu_uuid {
	uint8_t data[NRF_MODEM_FULL_DFU_UUID_LEN];
};

/**
 * @brief Set modem in full DFU mode.
 *
 * Reset the modem and set it into a state ready for full DFU operation.
 * Before initialization, the modem library must be shut down and
 * reinitialized with FULL_DFU_MODE.
 *
 * Call once before DFU operations. If the modem goes to a error state,
 * this can be called again to re-initialize.
 * The root key digest response of the modem is put in the digest_buffer
 * structure.
 *
 * If success, the modem will be in a state to wait for a bootloader.
 * Follow up with a call to nrf_modem_full_dfu_bl_write().
 *
 * @param[out]  digest_buffer       Pointer to the buffer to store digest hash.
 *                                  If NULL, digest skipped.
 *
 * @retval 0 if the procedure succeeds.
 * @retval -NRF_EFAULT When modem responded with error codes.
 * @retval -NRF_ETIMEDOUT When modem did not respond.
 * @retval -NRF_EIO When incorrect response received after modem reset.
 */
int nrf_modem_full_dfu_init(struct nrf_modem_full_dfu_digest *digest_buffer);

/**
 * @brief Write a booloader chunk to the modem.
 *
 * Call after nrf_modem_full_dfu_init() to upload modem bootloader segments.
 * Bootloader segments are appended together, so the full bootloader
 * may be uploaded with one or many call to this function.
 * Once the full bootloader is uploaded, call nrf_modem_full_dfu_apply().
 *
 * Firmware segments can be uploaded after successful bootloader upload.
 *
 * @param[in]   len    Length of data to be written.
 * @param[in]   src    Pointer to the buffer where chunk data is stored.
 *
 * @retval 0 if the procedure succeeds.
 * @retval -NRF_EINVAL When src pointer is NULL or user tries to upload too big bootloader.
 * @retval -NRF_EOPNOTSUPP If the modem is not in a state to receive.
 * @retval -NRF_EFAULT When modem responded with error codes.
 * @retval -NRF_EPERM When modem did not accept the flash programming request.
 * @retval -NRF_ENOEXEC When flash programming failed.
 * @retval -NRF_ETIMEDOUT When modem did not respond.
 * @retval -NRF_EIO When incorrect response received from modem.
 */
int nrf_modem_full_dfu_bl_write(uint32_t len, void *src);

/**
 * @brief Write a memory chunk to the modem.
 *
 * Call after bootloader have been upload to the modem.
 *
 * DFU library internally buffers all writes to a full page, so you
 * may call split writes to any size preferred. When addr points to an area
 * that is not continuing from previous write, the previous buffer is first
 * written to a modem. After all firmware segments have been written, call
 * nrf_modem_full_dfu_apply() to complete writing of the last buffer.
 *
 * @param[in]   addr   Address where the data to be written.
 * @param[in]   len    Length of data to be written.
 * @param[in]   src    Pointer to the buffer where chunk data is stored.
 *
 * @retval 0 if the procedure succeeds.
 * @retval -NRF_EINVAL When src pointer is NULL, or user tries to upload too big
 *                 bootloader, or addr parameter is zero after a bootloader
 *                 upload.
 * @retval -NRF_EOPNOTSUPP If the modem is not in a state to receive.
 * @retval -NRF_EFAULT When modem responded with error codes.
 * @retval -NRF_EPERM When modem did not accept the flash programming request.
 * @retval -NRF_ENOEXEC When flash programming failed.
 * @retval -NRF_ETIMEDOUT When modem did not respond.
 * @retval -NRF_EIO When incorrect response received from modem.
 */
int nrf_modem_full_dfu_fw_write(uint32_t addr, uint32_t len, void *src);

/**
 * @brief Complete previous write cycle.
 *
 * This call ensures that all parts from internal DFU buffers have been written
 * to the modem. Call after final bootloader chunk have been written as well
 * as when final firmware segment have been written.
 *
 * @retval 0 if the procedure succeeds.
 * @retval -NRF_EOPNOTSUPP If the modem is not in a state to receive.
 * @retval -NRF_EFAULT When modem responded with error codes.
 * @retval -NRF_EPERM When modem did not accept the flash programming request.
 * @retval -NRF_ENOEXEC When flash programming failed.
 * @retval -NRF_ETIMEDOUT When modem did not respond.
 * @retval -NRF_EIO When incorrect response received from modem.
 */
int nrf_modem_full_dfu_apply(void);

/**
 * @brief Read a digest hash data from the modem.
 *
 * @param[in]   addr            Start address.
 * @param[in]   size            Size of hash data.
 * @param[out]  digest_buffer   Pointer to the buffer to store digest hash data.
 *
 * @retval 0 if the procedure succeeds.
 * @retval -NRF_EINVAL When digest_buffer pointer is NULL.
 * @retval -NRF_EOPNOTSUPP If bootloader is not programmed.
 * @retval -NRF_EFAULT When modem responded with error codes.
 * @retval -NRF_EPERM When modem did not accept RPC command.
 * @retval -NRF_ENOEXEC When RPC command failed.
 * @retval -NRF_ETIMEDOUT When modem did not respond.
 * @retval -NRF_EIO When incorrect response received from modem.
 */
int nrf_modem_full_dfu_digest(uint32_t addr, uint32_t size,
			     struct nrf_modem_full_dfu_digest *digest_buffer);

/**
 * @brief Read an uuid data from the modem.
 *
 * @param[out]  modem_uuid      Pointer to the buffer to store uuid data.
 *
 * @retval 0 if the procedure succeeds.
 * @retval -NRF_EINVAL When modem_uuid pointer is NULL.
 * @retval -NRF_EOPNOTSUPP If bootloader is not programmed.
 * @retval -NRF_EFAULT When modem responded with error codes.
 * @retval -NRF_EPERM When modem did not accept RPC command.
 * @retval -NRF_ENOEXEC When RPC command failed.
 * @retval -NRF_ETIMEDOUT When modem did not respond.
 * @retval -NRF_EIO When incorrect response received from modem.
 */
int nrf_modem_full_dfu_uuid(struct nrf_modem_full_dfu_uuid *modem_uuid);

/**
 * @brief Verify the modem firmware signature.
 *
 * @param[in]  bytes      Size of signature.
 * @param[in]  data       Pointer to the buffer where signature is stored.
 *
 * @retval 0 if the procedure succeeds.
 * @retval -NRF_EINVAL When data pointer is NULL.
 * @retval -NRF_EOPNOTSUPP If bootloader is not programmed.
 * @retval -NRF_EFAULT When modem responded with error codes.
 * @retval -NRF_EPERM When modem did not accept RPC command.
 * @retval -NRF_ENOEXEC When RPC command failed.
 * @retval -NRF_ETIMEDOUT When modem did not respond.
 * @retval -NRF_EIO When incorrect response received from modem.
 */
int nrf_modem_full_dfu_verify(uint32_t bytes, const void *data);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* NRF_FULL_DFU_H__ */
