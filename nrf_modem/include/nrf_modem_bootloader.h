/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file nrf_modem_bootloader.h
 *
 * @defgroup nrf_modem_bootloader Bootloader API
 * @{
 * @brief API for modem bootloader (Full DFU).
 */
#ifndef NRF_BOOTLOADER_H__
#define NRF_BOOTLOADER_H__

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Digest buffer length. */
#define NRF_MODEM_BOOTLOADER_DIGEST_LEN 8
/** @brief UUID buffer length. */
#define NRF_MODEM_BOOTLOADER_UUID_LEN   36

/** @brief Storage for 256-bit digest/hash replies. */
struct nrf_modem_bootloader_digest {
	uint32_t data[NRF_MODEM_BOOTLOADER_DIGEST_LEN];
};

/** @brief Modem UUID response. */
struct nrf_modem_bootloader_uuid {
	uint8_t data[NRF_MODEM_BOOTLOADER_UUID_LEN];
};

/** @brief Bootloader segment */
struct nrf_modem_bootloader_fw_segment {
	uint32_t start_addr;
	uint32_t end_addr;
};

/**
 * @brief Write a segment of the bootloader to the modem.
 *
 * Call after nrf_modem_bootloader_init() to upload the modem bootloader segments.
 * Bootloader segments are appended together, so the bootloader
 * may be uploaded with one or more call to this function.
 * Once the entire bootloader is uploaded, call nrf_modem_bootloader_update().
 *
 * Firmware segments can be uploaded after successful bootloader upload.
 *
 * @param[in]   src    Pointer to the buffer where chunk data is stored.
 * @param[in]   len    Length of data to be written.
 *
 * @retval 0 on success.
 * @retval -NRF_EINVAL When src pointer is NULL or user tries to upload too big bootloader.
 * @retval -NRF_EOPNOTSUPP If the modem is not in a state to receive.
 */
int nrf_modem_bootloader_bl_write(void *src, uint32_t len);

/**
 * @brief Write a segment of the firmware to the modem.
 *
 * Call after the modem bootloader have been written and updated
 * to upload the modem firmware segments.
 * Firmware segments are appended together, so the firmware
 * may be uploaded with one or more call to this function.
 * Once the entire firmware is uploaded, call nrf_modem_bootloader_update().
 *
 * @param[in]   addr   Address to write the data to.
 * @param[in]   src    Pointer to the start of the buffer where the data to write is stored.
 * @param[in]   len    Length of the data to be written.
 *
 * @retval 0 on success.
 * @retval -NRF_EINVAL When src pointer is NULL, or user tries to upload too big
 *                 bootloader, or addr parameter is zero after a bootloader
 *                 upload.
 * @retval -NRF_EOPNOTSUPP If the modem is not in a state to receive.
 * @retval -NRF_EPERM When modem did not accept the flash programming request.
 * @retval -NRF_ENOEXEC When flash programming failed.
 * @retval -NRF_ETIMEDOUT When modem did not respond.
 * @retval -NRF_EIO When incorrect response received from modem.
 */
int nrf_modem_bootloader_fw_write(uint32_t addr, void *src, uint32_t len);

/**
 * @brief Complete bootloader or firmware update.
 *
 * This call ensures that all parts from internal update buffers have been written
 * to the modem. Call after the final bootloader segment or the final firmware segment
 * has been written.
 *
 * @retval 0 on success.
 * @retval -NRF_EOPNOTSUPP If the modem is not in a state to receive.
 * @retval -NRF_EPERM When modem did not accept the flash programming request.
 * @retval -NRF_ENOEXEC When flash programming failed.
 * @retval -NRF_ETIMEDOUT When modem did not respond.
 * @retval -NRF_EIO When incorrect response received from modem.
 */
int nrf_modem_bootloader_update(void);

/**
 * @brief Read a digest hash data from the modem.
 *
 * @param[in]   segments        Firmware segments.
 * @param[in]   num_segments    Number of firmware segments.
 * @param[out]  digest_buffer   Pointer to the buffer to store digest hash data.
 *
 * @retval 0 on success.
 * @retval -NRF_EINVAL When digest_buffer pointer is NULL.
 * @retval -NRF_EOPNOTSUPP If bootloader is not programmed.
 * @retval -NRF_EPERM When modem did not accept RPC command.
 * @retval -NRF_ENOEXEC When RPC command failed.
 * @retval -NRF_ETIMEDOUT When modem did not respond.
 * @retval -NRF_EIO When incorrect response received from modem.
 */
int nrf_modem_bootloader_digest(struct nrf_modem_bootloader_fw_segment *segments,
				size_t num_segments,
				struct nrf_modem_bootloader_digest *digest_buffer);

/**
 * @brief Read modem UUID data.
 *
 * @param[out]  modem_uuid      Pointer to the buffer to store uuid data.
 *
 * @retval 0 on success.
 * @retval -NRF_EINVAL When modem_uuid pointer is NULL.
 * @retval -NRF_EOPNOTSUPP If bootloader is not programmed.
 * @retval -NRF_EPERM When modem did not accept RPC command.
 * @retval -NRF_ENOEXEC When RPC command failed.
 * @retval -NRF_ETIMEDOUT When modem did not respond.
 * @retval -NRF_EIO When incorrect response received from modem.
 */
int nrf_modem_bootloader_uuid(struct nrf_modem_bootloader_uuid *modem_uuid);

/**
 * @brief Verify the modem' firmware signature.
 *
 * @param[in] data Pointer to the buffer where the signature is stored.
 * @param[in] len  Size of the signature.
 *
 * @retval 0 on success.
 * @retval -NRF_EINVAL When data pointer is NULL.
 * @retval -NRF_EOPNOTSUPP If bootloader is not programmed.
 * @retval -NRF_EPERM When modem did not accept RPC command.
 * @retval -NRF_ENOEXEC When RPC command failed.
 * @retval -NRF_ETIMEDOUT When modem did not respond.
 * @retval -NRF_EIO When incorrect response received from modem.
 */
int nrf_modem_bootloader_verify(const void *data, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* NRF_BOOTLOADER_H__ */
/** @} */
