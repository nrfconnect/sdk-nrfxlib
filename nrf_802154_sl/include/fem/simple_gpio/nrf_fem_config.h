/*
 * Copyright (c) 2020, Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef NRF_FEM_SIMPLE_GPIO_CONFIG_H_
#define NRF_FEM_SIMPLE_GPIO_CONFIG_H_

#include <stdint.h>
#include <stdbool.h>
#include "../nrf_fem_common_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief The ID of "simpe gpio" FEM type (e.g. for run-time selection)  */
#define NRF_FEM_IF_TYPE_SIMPLE_GPIO 1

/**
 * @brief Configuration parameters for the PA/LNA interface in the "simple gpio" variant
 */
typedef struct
{
    struct
    {
        uint32_t pa_time_gap_us;                /* Time between the activation of the PA pin and the start of the radio transmission. */
        uint32_t lna_time_gap_us;               /* Time between the activation of the LNA pin and the start of the radio reception. */
        int8_t   pa_gain_db;                    /* Configurable PA gain. Ignored if the amplifier is not supporting this feature. */
        int8_t   lna_gain_db;                   /* Configurable LNA gain. Ignored if the amplifier is not supporting this feature. */
    }                           fem_config;

    nrf_fem_gpiote_pin_config_t pa_pin_config;  /* Power Amplifier pin configuration. */
    nrf_fem_gpiote_pin_config_t lna_pin_config; /* Low Noise Amplifier pin configuration. */

    uint8_t                     ppi_channels[2];

} nrf_fem_simple_gpio_interface_config_t;

/**
 * @brief Configures the PA and LNA device interface.
 *
 * This function sets device interface parameters for the PA/LNA module.
 * The module can then be used to control a power amplifier or a low noise amplifier (or both) through the given interface and resources.
 *
 * The function also sets the PPI and GPIOTE channels to be configured for the PA/LNA interface.
 *
 * @param[in] p_config Pointer to the interface parameters for the PA/LNA device.
 *
 * @retval   ::NRFX_SUCCESS                 PA/LNA control successfully configured.
 * @retval   ::NRFX_ERROR_NOT_SUPPORTED     PA/LNA is not available.
 *
 */
int32_t nrf_fem_simple_gpio_interface_config_set(
    nrf_fem_simple_gpio_interface_config_t const * const p_config);

/**
 * @brief Retrieves the configuration of PA and LNA device interface.
 *
 * This function gets device interface parameters for the PA/LNA module.
 * The module can then be used to control a power amplifier or a low noise amplifier (or both) through the given interface and resources.
 *
 *
 * @param[in] p_config Pointer to the interface parameters for the PA/LNA device to be populated.
 *
 * @retval   ::NRFX_SUCCESS                 PA/LNA control successfully configured.
 * @retval   ::NRFX_ERROR_NOT_SUPPORTED     PA/LNA is not available.
 * @retval   ::NRFX_ERROR_FORBIDDEN         The "simpe gpio" is not currently selected as the FEM type.
 *
 */
int32_t nrf_fem_simple_gpio_interface_config_get(nrf_fem_simple_gpio_interface_config_t * p_config);

/**
 * @section Timings.
 */

/** Time in microseconds when PA GPIO is activated before the radio is ready for transmission. */
#define NRF_FEM_SIMPLE_GPIO_PA_TIME_IN_ADVANCE_US  23

/** Time in microseconds when LNA GPIO is activated before the radio is ready for reception. */
#define NRF_FEM_SIMPLE_GPIO_LNA_TIME_IN_ADVANCE_US 5

/**
 * @section Gains.
 */

#define NRF_FEM_SIMPLE_GPIO_PA_GAIN_DB  20
#define NRF_FEM_SIMPLE_GPIO_LNA_GAIN_DB 20

#ifdef __cplusplus
}
#endif

#endif /* NRF_FEM_SIMPLE_GPIO_CONFIG_H_ */
