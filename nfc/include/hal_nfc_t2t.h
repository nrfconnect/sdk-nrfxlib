/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: BSD-5-Clause-Nordic
 */

#ifndef HAL_NFC_H__
#define HAL_NFC_H__

/** @file
 * @defgroup nfc_t2t_hal NFC Type 2 Tag HAL
 * @{
 * @ingroup nfc_t2t
 * @brief @tagAPI52 Hardware abstraction layer for the NFC Type 2 Tag library.
 *
 * @note Before the NFCT peripheral enters ACTIVATED state, the HFXO must be
 * running.
 *
 * @note If you are using nRF52832 chip (in IC rev. Engineering B or
 * Engineering C) or if You are using nRF52840 chip (in IC rev. Engineering A,
 * B or C) library will use TIMER 4 to apply workarounds for the anomalies
 * listed in @ref nfc_fixes.h
 */

#include <zephyr/types.h>
#include <string.h>
#include <nrf_clock.h>
#include "errno.h"

#ifdef __cplusplus
extern "C" {
#endif


/** @brief Events passed to the upper-layer callback function. */
enum hal_nfc_event {
	HAL_NFC_EVENT_FIELD_ON,           /**< Field is detected. */
	HAL_NFC_EVENT_FIELD_OFF,          /**< Field is lost. */
	HAL_NFC_EVENT_DATA_RECEIVED,      /**< Data is received. */
	HAL_NFC_EVENT_DATA_TRANSMITTED    /**< Data is Transmitted. */
};

/** @brief Parameter IDs for set/get function. */
enum hal_nfc_param_id {
	/**  Used for unit tests. */
	HAL_NFC_PARAM_ID_TESTING,
	/** NFCID1 value, data can be 4, 7, or 10 bytes long (single, double,
	 *  or triple size). To use default NFCID1 of specific length pass
	 *  one byte containing requested length. Default 7-byte NFCID1 will
	 *  be used if this parameter was not set. This parameter can be
	 *  set before nfc_t2t_setup() to set initial NFCID1 and it can be
	 *  changed later.
	 */
	HAL_NFC_PARAM_ID_NFCID1,

	HAL_NFC_PARAM_ID_UNKNOWN
};

/** @brief Callback from HAL_NFC layer into the upper layer.
 *
 * If event == HAL_NFC_EVENT_DATA_RECEIVED:
 * data points to the received packet. The memory belongs to the HAL_NFC
 * layer and is guaranteed to be valid only until the callback returns.
 *
 * If event == HAL_NFC_EVENT_DATA_TRANSMITTED:
 * data points to the transmitted packet. The memory belongs to the
 * application.
 *
 * If event == \<Other event\>:
 * data definition is event-specific (to be defined).
 *
 * @param context Context for callback execution.
 * @param event The event that occurred.
 * @param data Received/transmitted data or NULL.
 * @param data_length Size of the received/transmitted packet.
 */
typedef void (*hal_nfc_callback_t)(void *context,
				   enum hal_nfc_event event,
				   const u8_t *data,
				   size_t data_length);


/** @brief Function for initializing the NFC layer.
 *
 * This function provides a pointer to a callback function and the callback
 * context to the NFC layer.
 *
 * @param callback Pointer to the callback function.
 * @param context Context of callback.
 *
 * @retval Zero on success or (negative) error code otherwise.
 */
int hal_nfc_setup(hal_nfc_callback_t callback, void *context);


/** @brief Function for setting a HAL_NFC parameter.
 *
 * This function allows to set any parameter defined as available by HAL_NFC.
 *
 * @param id ID of the parameter to set.
 * @param data Pointer to the buffer containing the data to set.
 * @param data_length Size of the buffer containing the data to set.
 *
 * @retval Zero on success or (negative) error code otherwise.
 */
int hal_nfc_parameter_set(enum hal_nfc_param_id id,
			  void *data,
			  size_t data_length);


/** @brief Function for querying a HAL_NFC parameter value.
 *
 * The queried value will be placed into the passed data buffer. If the buffer
 * is too small, maxDataLength will contain the required buffer size.
 *
 * @param id ID of the parameter to query.
 * @param data Pointer to a buffer receiving the queried data.
 * @param max_data_length Size of the buffer. It receives the required
 * size if buffer is too small.
 *
 * @retval Zero on success or (negative) error code otherwise.
 */
int hal_nfc_parameter_get(enum hal_nfc_param_id id,
			  void *data,
			  size_t *max_data_length);


/** @brief Function for starting the NFC subsystem.
 *
 * After this function completes, NFC readers will be able to detect the chip.
 *
 * @retval Zero on success or (negative) error code otherwise.
 */
int hal_nfc_start(void);


/** @brief Function for sending a packet to the connected NFC reader.
 *
 * The provided data buffer belongs to the caller and is guaranteed to be
 * valid until the HAL_NFC_EVENT_DATA_TRANSMITTED event is received by the
 * callback.
 *
 * @note Provided pointer must point to RAM region.
 *
 * @param data Pointer to the memory area in RAM containing data packet to
 * send.
 * @param data_length  Size of the packet in bytes.
 *
 * @retval Zero on success or (negative) error code otherwise.
 */
int hal_nfc_send(const u8_t *data, size_t data_length);


/** @brief Function for stopping the NFC subsystem.
 *
 * After this function returns, NFC readers will no longer be able to connect
 * to the chip.
 *
 * @retval Zero on success or (negative) error code otherwise.
 */
int hal_nfc_stop(void);


/** @brief Function for releasing resources.
 *
 * After this call returns, the callback is considered invalid and no more
 * events will be posted to it.
 *
 * @retval Zero. This function always succeeds.
 */
int hal_nfc_done(void);


/** @brief Function which is called when HFCLK started event occurs.
 *
 * @param event Clock event type.
 *
 */
void hal_nrf_clock_event_cb(nrf_clock_event_t event);


#ifdef __cplusplus
}
#endif

/** @} */
#endif /* HAL_NFC_H__ */
