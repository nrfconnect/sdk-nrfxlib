/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef _NRF_GZLL_GLUE_H_
#define _NRF_GZLL_GLUE_H_

/**
 * @file
 * @brief Gazell Link Layer glue
 *
 * File defines a set of functions and variables called by Gazell Link Layer.
 */

#include <nrf.h>
#include <nrf_peripherals.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup gzll_glue Gazell Link Layer glue
 * @{
 */

#ifdef DOXYGEN
#ifndef __IOM
#define __IOM
#endif
#endif

/** Timer instance. */
extern NRF_TIMER_Type * const nrf_gzll_timer;
/** Interrupt number for the timer. */
extern IRQn_Type        const nrf_gzll_timer_irqn;
/** Interrupt number for the software interrupt. */
extern IRQn_Type        const nrf_gzll_swi_irqn;

#if defined(PPI_PRESENT) || defined(DOXYGEN)
/** Gazell PPI event endpoint 0 register address. */
extern __IOM uint32_t * nrf_gzll_ppi_eep0;
/** Gazell PPI task endpoint 0 register address. */
extern __IOM uint32_t * nrf_gzll_ppi_tep0;
/** Gazell PPI event endpoint 1 register address. */
extern __IOM uint32_t * nrf_gzll_ppi_eep1;
/** Gazell PPI task endpoint 1 register address. */
extern __IOM uint32_t * nrf_gzll_ppi_tep1;
/** Gazell PPI event endpoint 2 register address. */
extern __IOM uint32_t * nrf_gzll_ppi_eep2;
/** Gazell PPI task endpoint 2 register address. */
extern __IOM uint32_t * nrf_gzll_ppi_tep2;
/** Channel enable/disable mask for PPI endpoint 0 and 1. */
extern       uint32_t   nrf_gzll_ppi_chen_msk_0_and_1;
/** Channel enable/disable mask for PPI endpoint 2. */
extern       uint32_t   nrf_gzll_ppi_chen_msk_2;
#endif

#if defined(DPPI_PRESENT) || defined(DOXYGEN)
/** Gazell DPPI channel index 0. */
extern uint8_t nrf_gzll_dppi_ch0;
/** Gazell DPPI channel index 1. */
extern uint8_t nrf_gzll_dppi_ch1;
/** Gazell DPPI channel index 2. */
extern uint8_t nrf_gzll_dppi_ch2;
/** Channel enable/disable mask for DPPI channel 0 and 1. */
extern uint32_t nrf_gzll_dppi_chen_msk_0_and_1;
/** Channel enable/disable mask for DPPI channel 2. */
extern uint32_t nrf_gzll_dppi_chen_msk_2;
#endif


/**
 * @brief Microseconds delay.
 *
 * @param usec_to_wait  Number of microseconds to delay.
 */
void nrf_gzll_delay_us(uint32_t usec_to_wait);

/**
 * @brief Request external oscillator (XOSC).
 */
void nrf_gzll_request_xosc(void);

/**
 * @brief Release external oscillator (XOSC).
 */
void nrf_gzll_release_xosc(void);

/**
 * @brief Radio interrupt handler.
 */
void nrf_gzll_radio_irq_handler(void);

/**
 * @brief Timer interrupt handler.
 */
void nrf_gzll_timer_irq_handler(void);

/**
 * @brief Software interrupt handler.
 */
void nrf_gzll_swi_irq_handler(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* _NRF_GZLL_GLUE_H_ */
