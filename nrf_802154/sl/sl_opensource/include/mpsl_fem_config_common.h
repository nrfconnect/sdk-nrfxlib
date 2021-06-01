/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_fem_config_common.h
 *
 * @defgroup mpsl_fem_config_common MPSL Front End Module Common Configuration parts
 * @ingroup  mpsl_fem
 *
 * The MPSL Front End Module Common Configuration defines structures common for every supported Front End Module.
 * @{
 */

#ifndef MPSL_FEM_CONFIG_COMMON_H__
#define MPSL_FEM_CONFIG_COMMON_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Configuration parameters for pins that enable or disable (or both) either Power Amplifier (PA) or Low Noise Amplifier (LNA).
 */
typedef struct
{
    bool    enable;       /**< Enable toggling for this pin. */
    bool    active_high;  /**< If true, the pin will be active high. Otherwise, the pin will be active low. */
    uint8_t gpio_pin;     /**< GPIO pin number for the pin. */
    uint8_t gpiote_ch_id; /**< The GPIOTE channel used for toggling this pin. */
} mpsl_fem_gpiote_pin_config_t;

/** @brief Sets flag which determines whether DEVICE-CONFIG-254 shall be applied.
 *
 * @note This function shall be called before initialization of any protocol stack.
 *
 * @param[in]   apply   Whether the DEVICE-CONFIG-254 option shall be applied.
 */
void mpsl_fem_device_config_254_apply_set(bool apply);

/** @brief Gets flag which determines whether DEVICE-CONFIG-254 shall be applied.
 *
 * @retval  true    DEVICE-CONFIG-254 shall be applied.
 * @retval  false   DEVICE-CONFIG-254 shall not be applied.
 */
bool mpsl_fem_device_config_254_apply_get(void);

#ifdef __cplusplus
}
#endif

#endif // MPSL_FEM_CONFIG_COMMON_H__

/**@} */
