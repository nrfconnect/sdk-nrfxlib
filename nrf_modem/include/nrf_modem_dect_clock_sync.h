/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file nrf_modem_dect_clock_sync.h
 *
 * @defgroup nrf_modem_dect_clock_sync DECT clock sync interface
 * @{
 * @brief DECT clock sync API.
 *
 */
#ifndef NRF_MODEM_DECT_CLOCK_SYNC_H__
#define NRF_MODEM_DECT_CLOCK_SYNC_H__


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief DECT CS errors.
 */
enum nrf_modem_dect_clock_sync_err {
	/**
	 * @brief Success.
	 */
	NRF_MODEM_DECT_CLOCK_SYNC_NO_ERROR = 0,
	/**
	 * @brief Clock synchronization not active.
	 */
	NRF_MODEM_DECT_CLOCK_SYNC_NOT_ACTIVE = 0xFFFE,
	/**
	 * @brief Error.
	 */
	NRF_MODEM_DECT_CLOCK_SYNC_ERROR = 0xFFFF
};

/**
 * @brief Clock synchronization enable event.
 */
struct nrf_modem_dect_clock_sync_enable_event {
	/**
	 * @brief Operation result.
	 *
	 * Can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_CLOCK_SYNC_NO_ERROR
	 * - @ref NRF_MODEM_DECT_CLOCK_SYNC_ERROR
	 */
	enum nrf_modem_dect_clock_sync_err err;
};

/**
 * @brief Clock synchronization pulse info event.
 */
struct nrf_modem_dect_clock_sync_pulse_info_event {
	/**
	 * @brief Operation result.
	 *
	 * Can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_CLOCK_SYNC_NO_ERROR
	 * - @ref NRF_MODEM_DECT_CLOCK_SYNC_NOT_ACTIVE
	 * - @ref NRF_MODEM_DECT_CLOCK_SYNC_ERROR
	 */
	enum nrf_modem_dect_clock_sync_err err;

	/** @brief Queried pulse direction (copied from request) */
	uint16_t pulse_direction;

	/** Padding */
	uint8_t pad[2];

	/** @brief Pulse occurrence time in modem time units */
	uint64_t pulse_time;
};

/**
 * @brief Clock synchronization disable event.
 */
struct nrf_modem_dect_clock_sync_disable_event {
	/**
	 * @brief Operation result.
	 *
	 * Can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_CLOCK_SYNC_NO_ERROR
	 * - @ref NRF_MODEM_DECT_CLOCK_SYNC_ERROR
	 */
	enum nrf_modem_dect_clock_sync_err err;
};

/**
 * @brief Clock synchronization state event.
 */
struct nrf_modem_dect_clock_sync_state_event {
	/**
	 * @brief Operation result.
	 *
	 * Can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_CLOCK_SYNC_NO_ERROR
	 * - @ref NRF_MODEM_DECT_CLOCK_SYNC_ERROR
	 */
	enum nrf_modem_dect_clock_sync_err err;

	/**
	 * @brief Clock synchronization status.
	 *
	 * Indicates whether the modem clock is synchronized to the external clock.
	 * - true - synchronized
	 * - false - not synchronized
	 */
	bool synchronized;

	/**
	 * @brief Clock synchronization error standard deviation.
	 *
	 * Standard deviation of the clock error in parts per million (ppm) when synchronized.
	 * Only valid when synchronized is true.
	 *
	 * Value is encoded in Q3.12 fixed-point format:
	 * - Bit 15: Sign bit (0 = positive, 1 = negative)
	 * - Bits 14-12: Integer part (3 bits)
	 * - Bits 11-0: Fractional part (12 bits)
	 *
	 * To convert to floating-point ppm: value / 4096.0
	 *
	 * Example: 0x1000 (4096 decimal) = 1.0 ppm standard deviation
	 */
	int16_t clock_std_ppm;
};

/**
 * @brief Clock synchronization ref output enable event.
 */
struct nrf_modem_dect_clock_sync_enable_ref_output_event {
	/**
	 * @brief Operation result.
	 *
	 * Can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_CLOCK_SYNC_NO_ERROR
	 * - @ref NRF_MODEM_DECT_CLOCK_SYNC_ERROR
	 */
	enum nrf_modem_dect_clock_sync_err err;
};

/**
 * @brief Clock synchronization ref output disable event.
 */
struct nrf_modem_dect_clock_sync_disable_ref_output_event {
	/**
	 * @brief Operation result.
	 *
	 * Can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_CLOCK_SYNC_NO_ERROR
	 * - @ref NRF_MODEM_DECT_CLOCK_SYNC_ERROR
	 */
	enum nrf_modem_dect_clock_sync_err err;
};


enum nrf_modem_dect_clock_sync_event_id {
	/**
	 * @brief Event to indicate the completion of clock synchronization enable
	 * operation.
	 */
	NRF_MODEM_DECT_CLOCK_SYNC_EVT_ENABLE,
	/**
	 * @brief Event to indicate the completion of clock synchronization disable
	 * operation.
	 */
	NRF_MODEM_DECT_CLOCK_SYNC_EVT_DISABLE,
	/**
	 * @brief Event carrying synchronization signal information.
	 */
	NRF_MODEM_DECT_CLOCK_SYNC_EVT_PULSE_INFO,
	/**
	 * @brief Event carrying clock synchronization state information.
	 */
	NRF_MODEM_DECT_CLOCK_SYNC_EVT_STATE,
	/**
	 * @brief Event to indicate the completion of clock synchronization ref output
	 * enable operation.
	 */
	NRF_MODEM_DECT_CLOCK_SYNC_EVT_ENABLE_REF_OUTPUT,
	/**
	 * @brief Event to indicate the completion of clock synchronization ref output
	 * disable operation.
	 */
	NRF_MODEM_DECT_CLOCK_SYNC_EVT_DISABLE_REF_OUTPUT,
};

/**
 * @brief Clock synchronization event.
 */
struct nrf_modem_dect_clock_sync_event {
	/**
	 * @brief Event ID.
	 */
	enum nrf_modem_dect_clock_sync_event_id id;
	/**
	 * @brief Event data.
	 */
	union {
		struct nrf_modem_dect_clock_sync_enable_event clock_sync_enable;
		struct nrf_modem_dect_clock_sync_disable_event clock_sync_disable;
		struct nrf_modem_dect_clock_sync_pulse_info_event clock_sync_pulse_info;
		struct nrf_modem_dect_clock_sync_state_event clock_sync_state;
		struct nrf_modem_dect_clock_sync_enable_ref_output_event
			clock_sync_enable_ref_output;
		struct nrf_modem_dect_clock_sync_disable_ref_output_event
			clock_sync_disable_ref_output;
	};
};

/**
 * @brief Application handler prototype for clock sync events.
 */
typedef void (*nrf_modem_dect_clock_sync_event_handler_t)(
	const struct nrf_modem_dect_clock_sync_event *event);

/**
 * @brief Set the application event handler for clock sync events.
 *
 * The application must set the handler for events coming for the DECT clock sync before attempting
 * other clock synchronization operations.
 *
 * @param handler Event handler.
 *
 * @return 0 On success.
 * @retval -NRF_EFAULT @p handler is @c NULL.
 */
int nrf_modem_dect_clock_sync_event_handler_set(nrf_modem_dect_clock_sync_event_handler_t handler);

/**
 * @brief Clock synchronization pulse direction.
 */
enum nrf_modem_dect_clock_sync_pulse_direction {
	/**
	 * @brief Received/incoming sync pulse time.
	 */
	NRF_MODEM_DECT_CLOCK_SYNC_PULSE_INCOMING = 0,
	/**
	 * @brief Transmitted/outgoing sync pulse time.
	 */
	NRF_MODEM_DECT_CLOCK_SYNC_PULSE_OUTGOING = 1,
};

/**
 * @brief Enable external clock synchronization.
 *
 * The external clock signal is expected to be a 1 PPS (pulse-per-second) signal. When
 * synchronization is enabled, the modem's clock rate aligns with the external signal,
 * ensuring that exactly NRF_MODEM_DECT_MODEM_TIME_TICK_RATE_KHZ clock cycles occur
 * between consecutive external clock pulses.
 *
 * Synchronization accuracy (jitter) is 0.001 ppm (parts per million).
 *
 * This operation is performed asynchronously.
 * Completion of this operation is indicated by the @ref NRF_MODEM_DECT_CLOCK_SYNC_EVT_ENABLE
 * event.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT No event handler is set.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_clock_sync_enable(void);

/**
 * @brief Disable external clock synchronization.
 *
 * Once complete, the modem's clock will not be synchronized to the external clock signal anymore.
 *
 * This operation is performed asynchronously.
 * Completion of this operation is indicated by the @ref NRF_MODEM_DECT_CLOCK_SYNC_EVT_DISABLE
 * event.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT No event handler is set.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_clock_sync_disable(void);

/**
 * @brief Query the modem for the next synchronization signal.
 *
 * Can be used to query when the next synchronization signal is expected to happen.
 *
 * This operation is performed asynchronously.
 * Completion of this operation is indicated by the
 * @ref NRF_MODEM_DECT_CLOCK_SYNC_EVT_PULSE_INFO event.
 *
 * @param direction Direction of the pulse to query information about.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT No event handler is set.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_clock_sync_get_pulse_info(
	enum nrf_modem_dect_clock_sync_pulse_direction direction);

/**
 * @brief Query the modem for the synchronization state.
 *
 * This operation is performed asynchronously.
 * Completion of this operation is indicated by the
 * @ref NRF_MODEM_DECT_CLOCK_SYNC_EVT_STATE event.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT No event handler is set.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_clock_sync_get_state(void);

/**
 * @brief Enable external clock synchronization reference output.
 *
 * When reference output is enabled, the modem outputs a signal on COEX1
 * pin. The output signal is sent once per second as seen in modem internal
 * time. The output signal has 100 ms pulse width.
 *
 * This operation is performed asynchronously.
 * Completion of this operation is indicated by the
 * @ref NRF_MODEM_DECT_CLOCK_SYNC_EVT_ENABLE_REF_OUTPUT event.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT No event handler is set.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_clock_sync_enable_ref_output(void);

/**
 * @brief Disable external clock synchronization reference output.
 *
 * Once complete, the modem will not output the reference signal on COEX1 pin
 * anymore.
 *
 * This operation is performed asynchronously.
 * Completion of this operation is indicated by the
 * @ref NRF_MODEM_DECT_CLOCK_SYNC_EVT_DISABLE_REF_OUTPUT event.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT No event handler is set.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_clock_sync_disable_ref_output(void);

#ifdef __cplusplus
}
#endif

#endif /* NRF_MODEM_DECT_CLOCK_SYNC_H__ */
/** @} */
