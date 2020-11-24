/*
 * Copyright (c) 2017 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**
 * @file nrf_modem_platform.h
 * @defgroup nrf_modem_platform_ipc Platform
 * @ingroup nrf_modem_platform
 * @{
 */
#ifndef NRF_MODEM_PLATFORM_H__
#define NRF_MODEM_PLATFORM_H__

#ifdef __cplusplus
extern "C" {
#endif


/**@defgroup nrf_modem_reserved_memory Memory reserved for the library Memory
 * @{
 * @brief  Memory reserved by the library for communication with the application and the
 *         network layer.
 */

/**@brief Memory start address reserved by the library. */
#define NRF_MODEM_RESERVED_MEMORY_ADDRESS 0x20010000

/**
 * @brief Memory size reserved by the library.
 * This value has to match with the application RAM start address defined in the linker script.
 */
#define NRF_MODEM_RESERVED_MEMORY_SIZE 0xc020

/**
 * @brief Memory size reserved by the library when trace will always be disabled.
 */
#define NRF_MODEM_RESERVED_MEMORY_SIZE_TRACE_DISABLED 0x8020

/**@} */

/**@defgroup nrf_modem_reserved_interrupts Reserved Interrupt and Priorities
 * @{
 * @brief  Interrupts and priorities reserved by the library for communication with the
 *         application and the network layer.
 */

/**@brief Interrupt used for communication with the network layer. */
#define NRF_MODEM_NETWORK_IRQ IPC_IRQn

/**@brief Interrupt priority used on interrupt for communication with the network layer. */
#define NRF_MODEM_NETWORK_IRQ_PRIORITY 0

/**@brief Interrupt used for communication with the application layer. */
#define NRF_MODEM_APPLICATION_IRQ EGU1_IRQn

/** Interrupt handler used for communication with the application layer. */
#define NRF_MODEM_APPLICATION_IRQ_HANDLER EGU1_IRQHandler

/**@brief Interrupt priority used on interrupt for communication with the application layer. */
#define NRF_MODEM_APPLICATION_IRQ_PRIORITY 6

/**@brief Maximum number of client that can be registered with RPC. */
#define RPC_MAX_CLIENTS 10

/**@brief Enable IPC based transport */
#define RPC_TRANSPORT_IPC

/**@} */

#ifdef __cplusplus
}
#endif

#endif /* NRF_MODEM_PLATFORM_H__ */
/**@} */
