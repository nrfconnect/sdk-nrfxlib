/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_fem_config_nrf2220.h
 *
 * @defgroup mpsl_fem_nrf2220 MPSL nRF2220 Front End Module Configuration
 * @ingroup  mpsl_fem
 *
 * @{
 */

#ifndef MPSL_FEM_CONFIG_NRF2220_H__
#define MPSL_FEM_CONFIG_NRF2220_H__

#include <stdint.h>
#include <stdbool.h>

#include "mpsl_fem_config_common.h"
#include "mpsl_fem_nrf22xx_twi_config_common.h"
#include "nrf.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Configuration structure of the nRF2220 Front End Module.
 */
typedef struct
{
    /** Output power in dBm supported by the Front End Module. */
    int8_t output_power_dbm;
    /** Gain of the Front End Module's bypass in dB. This parameter is usually negative
     *  to express attenuation. */
    int8_t bypass_gain_db;
} mpsl_fem_nrf2220_config_t;

/** @brief Configuration parameters for the interface of the nRF2220 Front End Module.
 */
typedef struct
{
    /** nRF2220 parameters configuration. */
    mpsl_fem_nrf2220_config_t     fem_config;
    /** Chip Select pin configuration. Configuration where @ref cs_pin_config.enable is false is unsupported. */
    mpsl_fem_gpiote_pin_config_t  cs_pin_config;
    /** Mode pin configuration. Configuration where @ref md_pin_config.enable is false is unsupported. */
    mpsl_fem_gpiote_pin_config_t  md_pin_config;
    /** TWI interface. */
    mpsl_fem_twi_if_t             twi_if;

#if defined(NRF52_SERIES)
    /** Array of PPI channels which need to be provided to Front End Module to operate. */
    uint8_t                       ppi_channels[3];
#else
    /** Array of DPPI channels which need to be provided to Front End Module to operate. */
    uint8_t                       dppi_channels[2];
#endif

    /** TWI register initialization map. */
    mpsl_fem_twi_reg_map_t        twi_regs_init_map;

    /** TWI communication fault handler.
     *
     *  This handler is called when TWI communication with nRF2200 device fails.
     *  When this pointer is NULL, the behavior is equivalent to an empty handler.
     */
    void (*p_on_twi_fault)(void);

} mpsl_fem_nrf2220_interface_config_t;

/** @brief Configures the nRF2220 Front End Module interface.
 *
 * This function sets device interface parameters for the nRF2220.
 * It also sets the PPI and GPIOTE channels to be configured for the interface.
 *
 * @param[in] p_config Pointer to the interface parameters for the nRF2220 device.
 *
 * @retval   0             nRF2220 control interface successfully configured.
 * @retval   -NRF_EPERM    PA/LNA is not available.
 *
 */
int32_t mpsl_fem_nrf2220_interface_config_set(mpsl_fem_nrf2220_interface_config_t const * const p_config);

#ifdef __cplusplus
}
#endif

#endif // MPSL_FEM_CONFIG_NRF2220_H__

/**@} */
