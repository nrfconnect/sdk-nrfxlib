/*
 * Copyright (c) 2017 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: BSD-5-Clause-Nordic
 */

/**@file ipc/bsd_platform.h
 *
 * @defgroup bsd_platform_ipc BSD Platform
 * @ingroup bsd_platform
 * @{
 * @brief Platform Initialization and system resource usage.
 *
 * @details This module provides:
 * -# A common entry point for platform specific system initializations.
 * -# Initialization of modules, in right order and with platform specific tuning.
 * -# Any system level configurations. These configurations include:
 *         -1. Interrupts and their priorities.
 *         -2. Any reserved memory.
 *         -3. Any other reserved resources in the system for the purpose.
 */
#ifndef BSD_PLATFORM_H__
#define BSD_PLATFORM_H__

#ifdef __cplusplus
extern "C" {
#endif

/**@defgroup bsd_version Version number.
 * @{
 * @brief  BSD Library version number.
 */

/**@brief Major version number. */
#define BSD_VERSION_MAJOR_NUMBER               0x0001

/**@brief Minor version number. */
#define BSD_VERSION_MINOR_NUMBER               0x0000

/**@} */

/**@defgroup bsd_reserved_memory Memory reserved for the BSD library Memory
 * @{
 * @brief  Memory reserved by the BSD library for communication with the application and the
 *         network layer.
 */

/**@brief Memory start address reserved by the BSD library. */
#define BSD_RESERVED_MEMORY_ADDRESS            0x21000000

/**
 * @brief Memory size reserved by the BSD library.
 * This value has to match with the application RAM start address defined in the linker script.
 */
#define BSD_RESERVED_MEMORY_SIZE               0x8000

/**@} */

/**@defgroup BSD_reserved_interrupts Reserved Interrupt and Priorities
 * @{
 * @brief  Interrupts and priorities reserved by the BSD library for communication with the
 *         application and the network layer.
 */

/**@brief Interrupt used for communication with the network layer. */
#define BSD_NETWORK_IRQ                        IPC_IRQn

/**@brief Interrupt priority used on interrupt for communication with the network layer. */
#define BSD_NETWORK_IRQ_PRIORITY               0

#if defined(NRF9120_XXAA)

/**@brief Interrupt used for communication with the application layer. */
#define BSD_APPLICATION_IRQ                    EGU1_IRQn

/** Interrupt handler used for communication with the application layer. */
#define BSD_APPLICATION_IRQ_HANDLER            EGU1_IRQHandler

/**@brief Interrupt priority used on interrupt for communication with the application layer. */
#define BSD_APPLICATION_IRQ_PRIORITY           6

#else // defined(NRF9120_XXAA)

/**@brief Interrupt used for communication with the application layer. */
#define BSD_APPLICATION_IRQ                    NFCT_IRQn

/** Interrupt handler used for communication with the application layer. */
#define BSD_APPLICATION_IRQ_HANDLER            NFCT_IRQHandler

/**@brief Interrupt priority used on interrupt for communication with the application layer. */
#define BSD_APPLICATION_IRQ_PRIORITY           6

#endif // defined(NRF9120_XXAA)

/**@brief Maximum number of client that can be registered with RPC. */
#define RPC_MAX_CLIENTS                        10

/**@brief Maximum transport instances supported for RPC. */
#define RPC_MAX_TRANSPORT_INSTANCES            1

/**@} */

#ifdef __cplusplus
}
#endif

#endif // BSD_PLATFORM_H__
/**@} */
