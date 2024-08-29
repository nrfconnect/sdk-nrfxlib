/*
 * Copyright (c) Nordic Semiconductor ASA
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
#include <nrf.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief GPIO pin and port to be used by Front End Module.
 */
typedef struct
{
  NRF_GPIO_Type * p_port;    /**< GPIO port register address. */
  uint8_t         port_no;   /**< GPIO port number. */
  uint8_t         port_pin;  /**< GPIO pin number relative to the port. */
} mpsl_fem_pin_t;

/** @brief Configuration parameters for pins that control Front End Module through a GPIOTE peripheral.
 */
typedef struct
{
    /** GPIO port number, register address and pin number relative to the port.
     * 
     *  For nRF54L series devices only the pins within a power domain equipped with a GPIOTE can be used.
     *  For nRF54L15 device pins from GPIO P0 or GPIO P1 can be used, GPIO P2 can not be used.
    */
    mpsl_fem_pin_t  gpio_pin;

    /* Enable toggling for this pin. */
    bool            enable;

    /* If true, the pin will be active high. Otherwise, the pin will be active low. */
    bool            active_high;

    /** The GPIOTE channel used for toggling the pin.
     *
     * For nRF54L series devices it is assumed that the GPIOTE channel belongs to that GPIOTE instance
     * that allows to control the pin given by @c gpio_pin.
     * The field @c p_gpiote must also be set.
     */
    uint8_t         gpiote_ch_id;

#if defined(NRF54L_SERIES)
    /** The EGU channel numbers (belonging to EGU10 in Radio Power Domain) that need to be provided.
     *
     *  It is a responsibility of an user of the FEM API to provide the cross-domain DPPI/PPIB connectios that
     *  fulfills the following requirements:
     *  - The egu_channels[0] should trigger the TASKS_CLR[gpiote_ch_id] of the GPIOTE.
     *  - The egu_channels[1] should trigger the TASKS_SET[gpiote_ch_id] of the GPIOTE.
     */
    uint8_t         egu_channels[2];

    /** The pointer to the GPIOTE instance used to toggle the GPIO pin.
     *
     *  It is a responsibility of an user of the FEM API to provide proper instance
     *  allowing to toggle the pin given by @c gpio_pin field.
     */
    NRF_GPIOTE_Type * p_gpiote;
#endif
} mpsl_fem_gpiote_pin_config_t;

/** @brief Configuration parameters for pins that control Front End Module.
 */
typedef struct
{

    mpsl_fem_pin_t  gpio_pin;       /**< GPIO port number, register address and pin number relative to the port. */
    bool            enable;         /**< Enable toggling for this pin. */
    bool            active_high;    /**< If true, the pin will be active high. Otherwise, the pin will be active low. */
} mpsl_fem_gpio_pin_config_t;

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
