/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_fem_config_nrf21540_common.h
 *
 * @defgroup mpsl_fem_nrf21540_common MPSL nRF21540 common Front End Module Configuration
 * @ingroup  mpsl_fem
 *
 * @{
 */

#ifndef MPSL_FEM_CONFIG_NRF21540_COMMON_H__
#define MPSL_FEM_CONFIG_NRF21540_COMMON_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* nRF21540 Front End Module Timings
 */

/** @brief Time in microseconds when PA GPIO is activated before the radio is ready for transmission. */
#define MPSL_FEM_NRF21540_DEFAULT_PA_TIME_IN_ADVANCE_US  13

/** @brief Time in microseconds when LNA GPIO is activated before the radio is ready for reception. */
#define MPSL_FEM_NRF21540_DEFAULT_LNA_TIME_IN_ADVANCE_US 13

/** @brief Time in microseconds between activating the PDN and asserting the RX_EN/TX_EN. */
#define MPSL_FEM_NRF21540_DEFAULT_PDN_SETTLE_US          18

/** @brief Time in microseconds between deasserting the RX_EN/TX_EN and deactivating PDN. */
#define MPSL_FEM_NRF21540_DEFAULT_TRX_HOLD_US            5

/* nRF21540 Front End Module Gains
 *
 * The provided gains are the default gains. The actual gain may depend on the
 * temperature and the configuration of the Front End Module.
 * See the Objective Product Specification for more details.
 */

/** @brief Gain of the PA in dB when using POUTA. */
#define MPSL_FEM_NRF21540_PA_POUTA_DEFAULT_GAIN_DB   20

/** @brief Gain of the PA in dB when using POUTB. */
#define MPSL_FEM_NRF21540_PA_POUTB_DEFAULT_GAIN_DB   10

/** @brief Default gain of the PA in dB. */
#define MPSL_FEM_NRF21540_PA_DEFAULT_GAIN_DB         MPSL_FEM_NRF21540_PA_POUTA_DEFAULT_GAIN_DB

/** @brief Gain of the LNA in dB. */
#define MPSL_FEM_NRF21540_LNA_DEFAULT_GAIN_DB        20

/**
 * @brief Configuration structure of the nRF21540 Front End Module.
 */
typedef struct
{
    /** Time in microseconds between the activation of the PA pin and the start of the radio transmission.
     *  Should be no bigger than Radio Ramp-Up time. */
    uint32_t pa_time_gap_us;
    /** Time in microseconds between the activation of the LNA pin and the start of the radio reception.
     *  Should be no bigger than Radio Ramp-Up time. */
    uint32_t lna_time_gap_us;
    /** Time in microseconds between activating the PDN pin and activating the PA/LNA pin. */
    uint32_t pdn_settle_us;
    /** Time in microseconds between deactivating the PA/LNA pin and deactivating the PDN pin. */
    uint32_t trx_hold_us;
    /** Default PA gain in dB. Ignored if the amplifier is not supporting this feature. */
    int8_t   pa_gain_db;
    /** Available PA gains in dB. @c pa_gains_db[0] corresponds to POUTA and @c pa_gains_db[1] to POUTB. */
    int8_t   pa_gains_db[2];
    /** Configurable LNA gain in dB. Ignored if the amplifier is not supporting this feature. */
    int8_t   lna_gain_db;
} mpsl_fem_nrf21540_config_t;

#ifdef __cplusplus
}
#endif

#endif // MPSL_FEM_CONFIG_NRF21540_COMMON_H__

/**@} */
