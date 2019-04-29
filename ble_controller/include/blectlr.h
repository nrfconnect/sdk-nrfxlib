/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#ifndef BLECTLR_H__
#define BLECTLR_H__

#include <stdint.h>

/** @defgroup blectlr
 * @{
 **/


/** @brief Host signal handler function pointer type. */
typedef void (*host_signal_t)(void);

/**
 * @brief     Function prototype for the blectlr assertion handler. To be implemented
 *            by the application.
 *
 * @note      The implementation of this function should not return.
 *            This function may be called from different interrupt priorities
 *            and should be preemptive or disable interrupts.
 *
 * @param[in] file                  The filename where the assertion occurred
 * @param[in] line                  The line number where the assertion occurred
 */
void blectlr_assertion_handler(const char * const file, const uint32_t line);

/**
 * @brief      Initialize the BLE Controller
 *
 * @param[in]  host_signal  The host signal handler, this will be called by the BLE controller
 *                          when data or event is available.
 *
 * @retval ::NRF_SUCCESS             Initialized successfully
 * @retval ::NRF_ERROR_NO_MEM        Not enough memory to support the current configuration.
 */
uint32_t blectlr_init(host_signal_t host_signal);

/**
 * @brief      Process all pending signals in the BLE Controller
 *
 * @note       The higher priority interrupts will notify that the signals
 *             are ready to be processed by setting the SWI5 IRQ pending.
 */
void blectlr_signal(void);

/**
 * @brief      BLE Controller RADIO interrupt handler
 *
 * @note       This handler should be called from the ISR.
 *             The interrupt priority level should be priority 0.
 */
void C_RADIO_Handler(void);

/**
 * @brief      BLE Controller RTC0 interrupt handler
 *
 * @note       This handler should be called from the ISR.
 *             The interrupt priority level should be priority 0
 */
void C_RTC0_Handler(void);

/**
 * @brief      BLE Controller TIMER0 interrupt handler.
 *
 * @note       This handler should be called from the ISR
 *             The interrupt priority level should be priority 0
 */
void C_TIMER0_Handler(void);

/**
 * @brief      BLE Controller RNG interrupt handler
 *
 * @note       This handler should be called from the ISR.
 *             The interrupt priority level should be lower than priority 0.
 */
void C_RNG_Handler(void);

/**
 * @brief      BLE Controller POWER_CLOCK interrupt handler
 *
 * @note       This handler should be called from the ISR.
 *             The interrupt priority level should be lower than priority 0.
 */
void C_POWER_CLOCK_Handler(void);

/** @} **/

#endif
