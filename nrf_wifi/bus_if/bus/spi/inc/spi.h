/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file spi.h
 *
 * @brief Header file for the Linux SPI bus layer specific structure declarations
 * of the Wi-Fi driver.
 */

#ifndef __LINUX_SPI_H__
#define __LINUX_SPI_H__

#include "osal_structs.h"
#include "bal_structs.h"

/**
 * @brief Structure to hold context information for the Linux SPI bus.
 */
struct nrf_wifi_bus_spi_priv {
	/** Pointer to the OS-specific SPI context. */
	void *os_spi_priv;

	/**
	 * @brief Interrupt callback function.
	 *
	 * @param hal_ctx The HAL context.
	 * @return The status of the interrupt callback function.
	 */
	enum nrf_wifi_status (*intr_callbk_fn)(void *hal_ctx);

	/**
	 * @brief Configuration parameters for the Linux SPI bus.
	 */
	struct nrf_wifi_bal_cfg_params cfg_params;
};

/**
 * @brief Structure to hold the device context for the Linux SPI bus.
 */
struct nrf_wifi_bus_spi_dev_ctx {
	/** Pointer to the SPI bus private context. */
	struct nrf_wifi_bus_spi_priv *spi_priv;
	/** Pointer to the BAL device context. */
	void *bal_dev_ctx;
	/** Pointer to the OS-specific SPI device context. */
	void *os_spi_dev_ctx;

	/** Base address of the host. */
	unsigned long host_addr_base;
	/** Base address of the packet RAM. */
	unsigned long addr_pktram_base;
};

#endif /* __LINUX_SPI_H__ */
