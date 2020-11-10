/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#ifndef SST_SETTINGS_H_
#define SST_SETTINGS_H_

#include <zephyr/types.h>

/**
 * Performs any initialization for the settings subsystem.
 *
 */
int sst_settings_init(void);

/// Fetches the value of a setting
/** This function fetches the value of the setting identified
 *  by key and write it to the memory pointed to by buff.
 *
 *  @param[in]     key       The key associated with the requested setting.
 *  @param[out]    buff      A pointer to where the value of the setting should be written.
 *  @param[inout]  buff_len  A pointer to the length of the buff. When called, this pointer should point to an
 *                           integer containing the maximum value size that can be written to buff.
 *
 *  @retval > 0: Number of bytes read. The given setting was found and fetched successfully.
 *  @retval <= 0: The given setting was not found in the setting store.
 */
int sst_settings_get(uint32_t key, void *buff, size_t buff_len);

/// Sets or replaces the value of a setting
/** This function sets or replaces the value of a setting
 *  identified by key.
 *
 *  @param[in]  key       The key associated with the setting to change.
 *  @param[in]  buff      A pointer to where the new value of the setting should be read from.
 *  @param[in]  buff_len  The length of the buffer pointed to by buff.
 *
 *  @retval 0   The given setting was set.
 *  @retval !=  The setting failed.
 */
int sst_settings_set(uint32_t key, const void *buff, size_t buff_len);

/// Removes a setting from the setting store
/** This function deletes a specific value from the
 *  setting identified by key from the settings store.
 *
 *  @param[in] key       The key associated with the requested setting.
 *
 *  @retval 0 on success.
 *  @retval non-zero on failure.
 */
int sst_settings_delete(uint32_t key);

/// Removes all settings from the setting store
/** This function deletes all settings from the settings
 *  store.
 */
int sst_settings_delete_all(void);

#endif // SST_SETTINGS_H_
