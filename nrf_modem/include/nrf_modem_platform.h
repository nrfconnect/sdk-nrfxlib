/*
 * Copyright (c) 2017 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
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


/**@defgroup nrf_modem_shmem_cfg Modem library shared memory configuration.
 * @{
 * Memory requirements for control structures.
 */
#define NRF_MODEM_SHMEM_CTRL_SIZE 0x4e8
/**@} */

/**@defgroup nrf_modem_reserved_interrupts Reserved Interrupt and Priorities
 * @{
 * @brief  Interrupts and priorities reserved by the library for communication with the
 *         application and the network layer.
 */

/**@brief Interrupt used for communication with the network layer. */
#define NRF_MODEM_NETWORK_IRQ IPC_IRQn

/**@brief Interrupt priority used on interrupt for communication with the network layer. */
#define NRF_MODEM_NETWORK_IRQ_PRIORITY CONFIG_NRF_MODEM_NETWORK_IRQ_PRIORITY

/**@brief Interrupt used for communication with the application layer. */
#define NRF_MODEM_APPLICATION_IRQ EGU1_IRQn

/**@brief Interrupt priority used on interrupt for communication with the application layer. */
#define NRF_MODEM_APPLICATION_IRQ_PRIORITY CONFIG_NRF_MODEM_APPLICATION_IRQ_PRIORITY

/**@} */

#ifdef __cplusplus
}
#endif

#endif /* NRF_MODEM_PLATFORM_H__ */
/**@} */
