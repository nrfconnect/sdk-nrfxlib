/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @brief Header containing patch loader specific declarations for the
 * HAL Layer of the Wi-Fi driver.
 */

#ifndef __HAL_FW_PATCH_LOADER_H__
#define __HAL_FW_PATCH_LOADER_H__

#include "hal_structs.h"

enum nrf_wifi_fw_patch_type {
	NRF_WIFI_FW_PATCH_TYPE_PRI,
	NRF_WIFI_FW_PATCH_TYPE_SEC,
	NRF_WIFI_FW_PATCH_TYPE_MAX
};


/* Loads a firmware patch chunk into RPU memory. */
enum nrf_wifi_status hal_fw_patch_chunk_load(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx,
						enum RPU_PROC_TYPE rpu_proc,
						unsigned int dest_addr,
						const void *fw_chunk_data,
						unsigned int fw_chunk_size);
/*
 * Downloads a firmware patch into RPU memory.
 */
enum nrf_wifi_status nrf_wifi_hal_fw_patch_load(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx,
						enum RPU_PROC_TYPE rpu_proc,
						const void *fw_pri_patch_data,
						unsigned int fw_pri_patch_size,
						const void *fw_sec_patch_data,
						unsigned int fw_sec_patch_size);

enum nrf_wifi_status nrf_wifi_hal_fw_patch_boot(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx,
						enum RPU_PROC_TYPE rpu_proc,
						bool is_patch_present);
#endif /* __HAL_FW_PATCH_LOADER_H__ */
