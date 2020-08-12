/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#ifndef BLE_CONTROLLER_SOC_H__
#define BLE_CONTROLLER_SOC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "sdc_soc.h"

#define BLE_CONTROLLER_FLASH_CMD_STATUS         SDC_SOC_FLASH_CMD_STATUS
#define BLE_CONTROLLER_FLASH_CMD_STATUS_SUCCESS SDC_SOC_FLASH_CMD_STATUS_SUCCESS
#define BLE_CONTROLLER_FLASH_CMD_STATUS_TIMEOUT SDC_SOC_FLASH_CMD_STATUS_TIMEOUT

#define ble_controller_flash_callback_t sdc_soc_flash_callback_t

#define ble_controller_flash_write              sdc_soc_flash_write_async
#define ble_controller_flash_page_erase         sdc_soc_flash_page_erase_async
#define ble_controller_rand_vector_get          sdc_soc_rand_vector_poll
#define ble_controller_rand_vector_get_blocking sdc_soc_rand_vector_get
#define ble_controller_ecb_block_encrypt        sdc_soc_ecb_block_encrypt

#ifdef __cplusplus
}
#endif

#endif /* BLE_CONTROLLER_SOC_H__ */
