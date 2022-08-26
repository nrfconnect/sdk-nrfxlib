/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file
 * @brief Gazell Link Layer constants and default values.
 *
 * NOTE! Changing values here has no effect. They are only provided as a reference.
 */

#ifndef NRF_GZLL_CONSTANTS_H__
#define NRF_GZLL_CONSTANTS_H__

#include "nrf_gzll_glue.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @defgroup gzll_constants Gazell Link Layer constants
 * @{
 */


/*****************************************************************************/
/** @name Hardware resources used by Gazell
 *  @{ */
/*****************************************************************************/
#define NRF_GZLL_SWI_IRQn           nrf_gzll_swi_irqn               ///< Software interrupt # used for callback functions.
#define NRF_GZLL_SWI_IRQ_HANDLER    nrf_gzll_swi_irq_handler        ///< Software interrupt handler used for callback functions.
#define NRF_GZLL_TIMER              nrf_gzll_timer                  ///< Timer to be used as flywheel timer.
#define NRF_GZLL_TIMER_IRQn         nrf_gzll_timer_irqn             ///< Interrupt # for the timer.
#define NRF_GZLL_TIMER_IRQ_HANDLER  nrf_gzll_timer_irq_handler      ///< Interrupt handler for the timer.

// In addition, Gazell uses the radio peripheral and radio interrupts.

/*
 * PPI configuration
 */
#if defined(PPI_PRESENT) || defined(DOXYGEN)
#define NRF_GZLL_PPI_EEP0 (*nrf_gzll_ppi_eep0)                      ///< Gazell PPI event endpoint 0
#define NRF_GZLL_PPI_TEP0 (*nrf_gzll_ppi_tep0)                      ///< Gazell PPI task endpoint 0
#define NRF_GZLL_PPI_EEP1 (*nrf_gzll_ppi_eep1)                      ///< Gazell PPI event endpoint 1
#define NRF_GZLL_PPI_TEP1 (*nrf_gzll_ppi_tep1)                      ///< Gazell PPI task endpoint 1
#define NRF_GZLL_PPI_EEP2 (*nrf_gzll_ppi_eep2)                      ///< Gazell PPI event endpoint 2
#define NRF_GZLL_PPI_TEP2 (*nrf_gzll_ppi_tep2)                      ///< Gazell PPI task endpoint 2

#define NRF_GZLL_PPI_CHEN_MSK_0_AND_1 (nrf_gzll_ppi_chen_msk_0_and_1) ///< Channel enable/disable mask for PPI endpoint 0 and 1.
#define NRF_GZLL_PPI_CHEN_MSK_2       (nrf_gzll_ppi_chen_msk_2)       ///< Channel enable/disable mask for PPI endpoint 2.
#endif

#if defined(DPPI_PRESENT) || defined(DOXYGEN)
#define NRF_GZLL_DPPI_CH0 (nrf_gzll_dppi_ch0)                       ///< Gazell DPPI channel 0
#define NRF_GZLL_DPPI_CH1 (nrf_gzll_dppi_ch1)                       ///< Gazell DPPI channel 1
#define NRF_GZLL_DPPI_CH2 (nrf_gzll_dppi_ch2)                       ///< Gazell DPPI channel 2

#define NRF_GZLL_DPPI_CHEN_MSK_0_AND_1 (nrf_gzll_dppi_chen_msk_0_and_1) ///< Channel enable/disable mask for DPPI channel 0 and 1.
#define NRF_GZLL_DPPI_CHEN_MSK_2       (nrf_gzll_dppi_chen_msk_2)       ///< Channel enable/disable mask for DPPI channel 2.
#endif
/** @} */

/*****************************************************************************/
/** @name Constant pipe and FIFO configuration
 *  @{ */
/*****************************************************************************/
#define NRF_GZLL_CONST_PIPE_COUNT            8                      ///< Number of TX pipes (at least one for each Device-Host pairs).
#define NRF_GZLL_CONST_FIFO_LENGTH           3                      ///< Maximum number of packets allowed in a TX or RX FIFO.
#define NRF_GZLL_CONST_MAX_TOTAL_PACKETS     6                      ///< Maximum number of packets available for reservation at any one time.
#define NRF_GZLL_CONST_MAX_PAYLOAD_LENGTH    32                     ///< Maximum allowed payload length in bytes.
#define NRF_GZLL_CONST_CALLBACK_QUEUE_LENGTH 10                     ///< Maximum number of notifications allowed in the callback queue.
/** @} */

/*****************************************************************************/
/** @name Default radio configuration
 *  @{ */
/*****************************************************************************/
#define NRF_GZLL_DEFAULT_TX_POWER NRF_GZLL_TX_POWER_0_DBM           ///< Default TX power.
#define NRF_GZLL_DEFAULT_DATARATE NRF_GZLL_DATARATE_2MBIT           ///< Default data rate.

#define NRF_GZLL_DEFAULT_CHANNEL_TABLE        {4, 25, 42, 63, 77}   ///< Default channel table.
#define NRF_GZLL_DEFAULT_CHANNEL_TABLE_SIZE   5                     ///< Default channel table size.
#define NRF_GZLL_CONST_MAX_CHANNEL_TABLE_SIZE 16                    ///< Maximum channel table size allowed by Gazell.
/** @} */

/*****************************************************************************/
/** @name Default Address configuration
 *  @{ */
/*****************************************************************************/

/*
Corresponds to Legacy nRFgo SDK Gazell config:
#define GZLL_DEFAULT_ADDRESS_PIPE0 {0x01, 0x04, 0x07, 0x0A, 0x0D}
#define GZLL_DEFAULT_ADDRESS_PIPE1 {0x02, 0x05, 0x08, 0x0B, 0x0E}
#define GZLL_DEFAULT_ADDRESS_PIPE2 3
#define GZLL_DEFAULT_ADDRESS_PIPE3 4
#define GZLL_DEFAULT_ADDRESS_PIPE4 5
#define GZLL_DEFAULT_ADDRESS_PIPE5 6
*/

#define NRF_GZLL_DEFAULT_FULL_ADDRESS_PIPE0  {0x01, 0x04, 0x07, 0x0A, 0x0D}   ///< Corresponding legacy Gazell pipe 0 address.
#define NRF_GZLL_DEFAULT_BASE_ADDRESS_0      0x0D0A0704                       ///< Default base address 0.
#define NRF_GZLL_DEFAULT_BASE_ADDRESS_1      0x0E0B0805                       ///< Default base address 1.
#define NRF_GZLL_DEFAULT_PREFIX_BYTE_0       1                                ///< Default prefix address pipe 0.
#define NRF_GZLL_DEFAULT_PREFIX_BYTE_1       2                                ///< Default prefix address pipe 1.
#define NRF_GZLL_DEFAULT_PREFIX_BYTE_2       3                                ///< Default prefix address pipe 2.
#define NRF_GZLL_DEFAULT_PREFIX_BYTE_3       4                                ///< Default prefix address pipe 3.
#define NRF_GZLL_DEFAULT_PREFIX_BYTE_4       5                                ///< Default prefix address pipe 4.
#define NRF_GZLL_DEFAULT_PREFIX_BYTE_5       6                                ///< Default prefix address pipe 5.
#define NRF_GZLL_DEFAULT_PREFIX_BYTE_6       7                                ///< Default prefix address pipe 6.
#define NRF_GZLL_DEFAULT_PREFIX_BYTE_7       8                                ///< Default prefix address pipe 7.
#define NRF_GZLL_DEFAULT_BASE_ADDRESS_LENGTH NRF_GZLL_BASE_ADDRESS_LENGTH_4B  ///< Default on-air base address length.
#define NRF_GZLL_DEFAULT_RX_PIPES_ENABLED    0x000000FF                       ///< Enabled Rx pipes. See nrf_gzll_set_rx_pipes_enabled().
/** @} */

/*****************************************************************************/
/** @name Default timeslot and synchronization configuration
 *  @{ */
/*****************************************************************************/
#define NRF_GZLL_DEFAULT_TIMESLOT_PERIOD                 600                                                      ///< Default timeslot period.
#define NRF_GZLL_DEFAULT_TIMESLOTS_PER_CHANNEL           2                                                        ///< Timeslots use by the Host and by the Device when communication is in sync.
#define NRF_GZLL_DEFAULT_DEVICE_CHANNEL_SELECTION_POLICY NRF_GZLL_DEVICE_CHANNEL_SELECTION_POLICY_USE_SUCCESSFUL  ///< Default channel Gazell Device channel selection policy
#define NRF_GZLL_DEFAULT_MAX_TX_ATTEMPTS                 0                                                        ///< Default maximum TX attempts for each packet. A value of zero implies the infinite number of TX attempts.
#define NRF_GZLL_DEFAULT_XOSC_CTL                        NRF_GZLL_XOSC_CTL_AUTO                                   ///< Deafult setting for controlling the XOSC
#define NRF_GZLL_DEFAULT_TIMESLOTS_PER_CHANNEL_WHEN_DEVICE_OUT_OF_SYNC  15                                        ///< Timeslots use by the Device before communication is in sync.
#define NRF_GZLL_DEFAULT_SYNC_LIFETIME (3 * NRF_GZLL_DEFAULT_CHANNEL_TABLE_SIZE * NRF_GZLL_DEFAULT_TIMESLOTS_PER_CHANNEL) ///< Number of timeslots to keep the timer running so that communication remains synchronized.
/** @} */

#define NRF_GZLL_PA_LNA_MAX_RAMP_UP_TIME    129                               ///< Max external front end ramp up time [us].
#define NRF_GZLL_PA_LNA_PPI_CHANNELS_NUM    4                                 ///< Number of PPI channels used for front end control.

/** @} */

#ifdef __cplusplus
}
#endif

#endif
