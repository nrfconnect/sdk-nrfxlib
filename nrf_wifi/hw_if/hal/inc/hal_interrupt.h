/**
 * @file hal_interrupt.h
 *
 * @brief Header containing interrupt handler specific declarations for the
 * HAL Layer of the Wi-Fi driver.
 */

#ifndef __HAL_INTERRUPT_H__
#define __HAL_INTERRUPT_H__

#include "hal_api.h"

/**
 * @brief Enable interrupts from the RPU.
 *
 * @param hal_dev_ctx Pointer to HAL context.
 *
 * @return Status
 *         - Pass: NRF_WIFI_STATUS_SUCCESS
 *         - Error: NRF_WIFI_STATUS_FAIL
 */
enum nrf_wifi_status hal_rpu_irq_enable(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx);


/**
 * @brief Disable interrupts from the RPU.
 *
 * @param hal_dev_ctx Pointer to HAL context.
 *
 * @return Status
 *         - Pass: NRF_WIFI_STATUS_SUCCESS
 *         - Error: NRF_WIFI_STATUS_FAIL
 */
enum nrf_wifi_status hal_rpu_irq_disable(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx);


/**
 * @brief Process an interrupt from the RPU.
 *
 * @param hal_dev_ctx Pointer to HAL context.
 * @param do_rpu_recovery Pointer to a boolean variable that indicates if the RPU recovery
 *                       is required.
 *
 * @return Status
 *         - Pass: NRF_WIFI_STATUS_SUCCESS
 *         - Error: NRF_WIFI_STATUS_FAIL
 */
enum nrf_wifi_status hal_rpu_irq_process(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx,
    bool *do_rpu_recovery);
#endif /* __HAL_INTERRUPT_H__ */
