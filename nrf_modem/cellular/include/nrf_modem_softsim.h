/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file nrf_modem_softsim.h
 *
 * @defgroup nrf_modem_softsim SoftSIM API
 * @{
 * @brief API to mediate SoftSIM exchanges between Modem and application.
 */
#ifndef NRF_MODEM_SOFTSIM_H__
#define NRF_MODEM_SOFTSIM_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief SoftSIM request command type. */
enum nrf_modem_softsim_cmd {
	/** @brief Initialization command.*/
	NRF_MODEM_SOFTSIM_INIT = 1,
	/** @brief Transport command, issued by the Modem to request data from the application. */
	NRF_MODEM_SOFTSIM_APDU = 3,
	/** @brief Deinitialization command. */
	NRF_MODEM_SOFTSIM_DEINIT = 4,
	/** @brief Reset command, issued by the Modem when a request becomes unresponsive. */
	NRF_MODEM_SOFTSIM_RESET = 5
};

/** @brief Handle a SoftSIM request.
 *
 * @details This handler lets the application process a SoftSIM request.
 *          The application must then call @ref nrf_modem_softsim_res with the response data
 *          requested.
 *
 * @note This handler is executed in an interrupt service routine.
 *       Offload any intensive operations as necessary.
 *
 * @param cmd SoftSIM request command.
 * @param req_id Request ID used to match request and response.
 * @param[in] data Pointer to the data of the SoftSIM request.
 * @param data_len Length of the data of the SoftSIM request.
 */
typedef void (*nrf_modem_softsim_req_handler_t)(enum nrf_modem_softsim_cmd cmd, uint16_t req_id,
						void *data, uint16_t data_len);

/**
 * @brief Set a handler function for SoftSIM requests.
 *
 * @note The handler is executed in an interrupt service routine.
 *	 Take care to offload any processing as appropriate.
 *
 * @param handler The SoftSIM request handler. Use @c NULL to unset handler.
 *
 * @retval 0 On success.
 */
int nrf_modem_softsim_req_handler_set(nrf_modem_softsim_req_handler_t handler);

/**
 * @brief Sends a SoftSIM response to a request.
 *
 * @details This function is used to respond to the Modem with the data requested by a specific
 *          request.
 *
 * @param cmd SoftSIM response command.
 * @param req_id Request ID used to match request and response.
 * @param[in] data Pointer to the data of the SoftSIM response.
 * @param data_len Length of the data of the SoftSIM response.
 *
 * @retval 0 on success.
 * @retval -NRF_EINVAL If input data is invalid.
 * @retval -NRF_ENOMEM If memory allocation failed.
 */
int nrf_modem_softsim_res(enum nrf_modem_softsim_cmd cmd, uint16_t req_id, void *data,
			  uint16_t data_len);

/**
 * @brief Sends a SoftSIM error to a request.
 *
 * @details This function is used to inform the Modem that an error occurred for a specific request.
 *
 * @param cmd SoftSIM response command.
 * @param req_id Request ID used to match request and response.
 *
 * @retval 0 on success.
 * @retval -NRF_ENOMEM If memory allocation failed.
 */
int nrf_modem_softsim_err(enum nrf_modem_softsim_cmd cmd, uint16_t req_id);

/**
 * @brief Free the data of a SoftSIM request.
 *
 * @param data Data to free.
 */
void nrf_modem_softsim_data_free(void *data);

#ifdef __cplusplus
}
#endif

#endif /* NRF_MODEM_SOFTSIM_H__ */
/** @} */
