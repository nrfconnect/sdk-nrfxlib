/**
 * @file hal_mem.h
 *
 * @brief Header containing memory read/write specific declarations for the
 * HAL Layer of the Wi-Fi driver.
 */

#ifndef __HAL_MEM_H__
#define __HAL_MEM_H__

#include "hal_api.h"

/**
 * Enum defining the types of RPU memory.
 */
enum HAL_RPU_MEM_TYPE {
	/** GRAM memory type */
	HAL_RPU_MEM_TYPE_GRAM,
	/** PKTRAM memory type */
	HAL_RPU_MEM_TYPE_PKTRAM,
	/** Core ROM memory type */
	HAL_RPU_MEM_TYPE_CORE_ROM,
	/** Core RET memory type */
	HAL_RPU_MEM_TYPE_CORE_RET,
	/** Core SCRATCH memory type */
	HAL_RPU_MEM_TYPE_CORE_SCRATCH,
	/** Maximum number of memory types */
	HAL_RPU_MEM_TYPE_MAX
};

/**
 * @brief Read from the RPU memory.
 *
 * This function reads a specified number of bytes from the RPU memory and
 * copies them to the host memory.
 *
 * @param hal_ctx       Pointer to HAL context.
 * @param host_addr     Pointer to the host memory where the contents read from
 *                      the RPU memory are to be copied.
 * @param rpu_mem_addr  Absolute value of the RPU memory address from which the
 *                      contents are to be read.
 * @param len           The length (in bytes) of the contents to be read from
 *                      the RPU memory.
 *
 * @return Status
 *         - Pass: NRF_WIFI_STATUS_SUCCESS
 *         - Error: NRF_WIFI_STATUS_FAIL
 */
enum nrf_wifi_status hal_rpu_mem_read(struct nrf_wifi_hal_dev_ctx *hal_ctx,
									  void *host_addr,
									  unsigned int rpu_mem_addr,
									  unsigned int len);

/**
 * @brief Write to the RPU memory.
 *
 * This function writes a specified number of bytes to the RPU memory from the
 * host memory.
 *
 * @param hal_ctx       Pointer to HAL context.
 * @param rpu_mem_addr  Absolute value of the RPU memory address where the
 *                      contents are to be written.
 * @param host_addr     Pointer to the host memory from where the contents are
 *                      to be copied to the RPU memory.
 * @param len           The length (in bytes) of the contents to be copied to
 *                      the RPU memory.
 *
 * @return Status
 *         - Pass: NRF_WIFI_STATUS_SUCCESS
 *         - Error: NRF_WIFI_STATUS_FAIL
 */
enum nrf_wifi_status hal_rpu_mem_write(struct nrf_wifi_hal_dev_ctx *hal_ctx,
		unsigned int rpu_mem_addr,
		void *host_addr,
		unsigned int len);

/**
 * @brief Clear contents of RPU memory.
 *
 * This function fills the RPU memory with zeros.
 *
 * @param hal_ctx   Pointer to HAL context.
 * @param rpu_proc  The RPU processor for which the memory is to be cleared.
 * @param mem_type  The type of the RPU memory to be cleared.
 *
 * @return Status
 *         - Pass: NRF_WIFI_STATUS_SUCCESS
 *         - Error: NRF_WIFI_STATUS_FAIL
 */
enum nrf_wifi_status hal_rpu_mem_clr(struct nrf_wifi_hal_dev_ctx *hal_ctx,
		enum RPU_PROC_TYPE rpu_proc,
		enum HAL_RPU_MEM_TYPE mem_type);

#endif /* __HAL_MEM_H__ */
