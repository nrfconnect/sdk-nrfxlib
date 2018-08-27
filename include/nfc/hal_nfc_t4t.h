/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: BSD-5-Clause-Nordic
 */

#ifndef HAL_NFC_H__
#define HAL_NFC_H__

/** @file
 * @defgroup nfc_t4t_hal NFC Type 4 Tag HAL
 * @{
 * @ingroup nfc_t4t
 * @brief @tagAPI52 Hardware abstraction layer for the NFC Type 4 Tag library.
 *
 * @note Before the NFCT peripheral enters ACTIVATED state, the HFXO must be
 *  running.
 */

#include <zephyr/types.h>
#include <stdbool.h>
#include <string.h>
#include "errno.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Events passed to the upper-layer callback function. */
enum hal_nfc_event {
	HAL_NFC_EVENT_FIELD_ON,          /**< Field is detected. */
	HAL_NFC_EVENT_FIELD_OFF,         /**< Field is lost. */
	HAL_NFC_EVENT_DATA_RECEIVED,     /**< Data is received. */
	HAL_NFC_EVENT_DATA_TRANSMITTED,  /**< Data is transmitted. */
	HAL_NFC_EVENT_DATA_TX_STARTED    /**< Data transmission has started. */
};

/** @brief Parameter IDs for the set/get function. */
enum hal_nfc_param_id {
	HAL_NFC_PARAM_ID_TESTING,	/**< Used for unit tests. */
	HAL_NFC_PARAM_FWI,		/**< Frame Wait Time parameter. */
	HAL_NFC_PARAM_SELRES,
	/**< Parameter for setting the 'Protocol' bits for SEL_RES packet. */
	HAL_NFC_PARAM_NFCID1,
	/**< NFCID1 value, data can be 4, 7, or 10 bytes long (simple, double,
	 *   or triple size). To use default NFCID1 of specific length pass
	 *   one byte containing requested length. Default 7-byte NFCID1 will
	 *   be used if this parameter was not set before nfc_t4t_setup().
	 */
	HAL_NFC_PARAM_ID_UNKNOWN
};

/** @brief Callback from HAL_NFC layer into the upper layer.
 *
 * If event == HAL_NFC_EVENT_DATA_RECEIVED:
 * data points to the received packet. The memory belongs to the HAL_NFC layer
 * and is guaranteed to be valid only until the callback returns.
 *
 * If event == HAL_NFC_EVENT_DATA_TRANSMITTED:
 * data points to the transmitted packet. The memory belongs to the application.
 *
 * If event == \<Other event\>:
 * data definition is event-specific (to be defined).
 *
 * param context Context for callback execution.
 * param event The event that occurred.
 * param data Received/transmitted data or NULL.
 * param data_length Size of the received/transmitted packet.
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
 * param callback Pointer to the callback function.
 * param context Context of callback.
 *
 * @retval 0 If the NFC layer was initialized successfully. If one
 * of the arguments was invalid, an error code is returned.
 */
int hal_nfc_setup(hal_nfc_callback_t callback, void *context);

/** @brief Function for setting a HAL_NFC parameter.
 *
 * This function allows to set any parameter defined as available by HAL_NFC.
 *
 * param id ID of the parameter to set.
 * param data Pointer to the buffer containing the data to set.
 * param data_length Size of the buffer containing the data to set.
 *
 * @retval 0 If the parameter was set successfully. If one of the arguments
 * was invalid (for example, wrong data length), an error code
 * is returned.
 */
int hal_nfc_parameter_set(enum hal_nfc_param_id id,
			  void *data,
			  size_t data_length);

/** @brief Function for querying a HAL_NFC parameter value.
 *
 * The queried value will be placed into the passed data buffer. If the buffer
 * is too small, max_data_length will contain the required buffer size.
 *
 * param id ID of the parameter to query.
 * @param data Pointer to a buffer receiving the queried data.
 * @param max_data_length Size of the buffer. It receives the required size if
 * buffer is too small.
 *
 * @retval 0 If the parameter was received successfully. If one of the arguments
 * was invalid (for example, the buffer was too small), an error code
 * is returned.
 */
int hal_nfc_parameter_get(enum hal_nfc_param_id id,
			  void *data,
			  size_t *max_data_length);

/** @brief Function for starting the NFC subsystem.
 *
 * After this function completes, NFC readers will be able to detect the chip.
 *
 * @retval 0 If the NFC subsystem was started successfully. If the NFC
 * subsystem could not be started, an error code is returned.
 */
int hal_nfc_start(void);

/** @brief Function for sending a packet to the connected NFC reader.
 *
 * The provided data buffer belongs to the caller and is guaranteed to be
 * valid until the HAL_NFC_EVENT_DATA_TRANSMITTED event is received by the
 * callback.
 *
 * param data The data packet to send.
 * param data_length Size of the packet in bytes.
 * param delayed_mode The data packet transfer is postponed till the end of
 * the FWT window.
 *
 * @retval 0 If the packet was sent. Otherwise, an error code is returned.
 */
int hal_nfc_send(const u8_t *data, size_t data_length, bool delayed_mode);

/** @brief Function for stopping the NFC subsystem.
 *
 * After this function returns, NFC readers will no longer be able to connect
 * to the chip.
 *
 * @retval 0 If the NFC subsystem was stopped. Otherwise, an error code
 * is returned.
 */
int hal_nfc_stop(void);

/** @brief Function for releasing resources.
 *
 * After this call returns, the callback is considered invalid and no more
 * events will be posted to it.
 *
 * @retval 0 This function always succeeds.
 */
int hal_nfc_done(void);

#ifdef __cplusplus
}
#endif

/** @} */
#endif /* HAL_NFC_H__ */

