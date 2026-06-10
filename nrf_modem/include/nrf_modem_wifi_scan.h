/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file nrf_modem_wifi_scan.h
 *
 * @defgroup nrf_modem_wifi_scan Wi-Fi scan API
 * @{
 * @brief API for modem Wi-Fi scan functionality.
 */
#ifndef NRF_MODEM_WIFI_SCAN_H__
#define NRF_MODEM_WIFI_SCAN_H__

#include <stdint.h>
#include <stddef.h>
#include <nrf_modem_toolchain.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Size of the MAC address. */
#define NRF_MODEM_WIFI_SCAN_MAC_ADDRESS_LEN 6
/** Length of the SSID buffer including null-termination character. */
#define NRF_MODEM_WIFI_SCAN_SSID_BUF_LEN 33
/** Received signal strength indicator not measured. */
#define NRF_MODEM_WIFI_SCAN_RSSI_NOT_MEASURED 0
/** Bitmask for all Wi-Fi channels. */
#define NRF_MODEM_WIFI_SCAN_WIFI_ALL_CHANNELS 0xFFFF
/** Shortest allowed scan time in milliseconds. */
#define NRF_MODEM_WIFI_SCAN_TIME_MS_MIN 100
/** Default desired scan time in milliseconds. */
#define NRF_MODEM_WIFI_SCAN_TIME_DESIRED_MS_DEFAULT 1000

/**
 * @brief Wi-Fi scan status values.
 */
enum nrf_modem_wifi_scan_status {
	/** Success. */
	NRF_MODEM_WIFI_SCAN_STATUS_SUCCESS = 0,
	/** Scan was aborted. */
	NRF_MODEM_WIFI_SCAN_STATUS_ABORTED = 2,
	/** Scan could not be performed within suitable time. */
	NRF_MODEM_WIFI_SCAN_STATUS_TIMEOUT = 3,
	/** Scan failure happened during the scan. */
	NRF_MODEM_WIFI_SCAN_STATUS_FAILURE = 4,
	/** Modem ran out of memory during the scan. */
	NRF_MODEM_WIFI_SCAN_STATUS_NO_MEMORY = 6,
};

/** Event IDs. */
enum nrf_modem_wifi_scan_event_id {
	/**
	 * Wi-Fi scan result event.
	 *
	 * This event is sent to provide scan results after a successful call to
	 * the @ref nrf_modem_wifi_scan_start function.
	 *
	 * This event is also sent if an ongoing scan is cancelled with the
	 * @ref nrf_modem_wifi_scan_cancel function, and to report an error during the scan.
	 *
	 * The associated payload is the @c nrf_modem_wifi_scan_event.result member of type
	 * @ref nrf_modem_wifi_scan_result in the event.
	 *
	 * Multiple events may be sent for a single scan request. The last event always has the
	 * @c finished field set to true to indicate that the scan has been finished.
	 * When the @c finished field is true, the scan results in the event are not valid.
	 *
	 * In case of an error, possible scan results until the error are still reported.
	 * In this case, the @c status field in the events with valid scan results is set to
	 * @ref NRF_MODEM_WIFI_SCAN_STATUS_SUCCESS, while the last event with @c finished set to
	 * true has the @c status field set to the error status from the modem.
	 */
	NRF_MODEM_WIFI_SCAN_EVT_RESULT = 1,
};

 /** @brief Data for an @ref NRF_MODEM_WIFI_SCAN_EVT_RESULT event. */
struct nrf_modem_wifi_scan_result {
	/**
	 * If false, indicates that the scan is not finished and the results in the event are
	 * valid. The event is sent with this field set to false for each access point found
	 * during the scan.
	 *
	 * For each scan request, the last event always has this field set to true to indicate
	 * that the scan has been finished. When true, the results in the event are not valid.
	 */
	bool finished;

	/**
	 * Received signal strength indicator (RSSI) of the access point in dBm.
	 *
	 * @ref NRF_MODEM_WIFI_SCAN_RSSI_NOT_MEASURED indicates that
	 * the value is not measured.
	 */
	int16_t rssi;

	/**
	 * Channel number.
	 *
	 * Range: 1...14.
	 */
	uint8_t channel;

	/** MAC address. */
	uint8_t mac_address[NRF_MODEM_WIFI_SCAN_MAC_ADDRESS_LEN];

	/** SSID string including null-termination character. */
	char ssid[NRF_MODEM_WIFI_SCAN_SSID_BUF_LEN];
};

/** @brief Wi-Fi scan event. */
struct nrf_modem_wifi_scan_event {
	/** Event ID. */
	enum nrf_modem_wifi_scan_event_id id;
	/** Status. */
	enum nrf_modem_wifi_scan_status status;
	union {
		/** Data for an @ref NRF_MODEM_WIFI_SCAN_EVT_RESULT event. */
		struct nrf_modem_wifi_scan_result result;
	};
};

/** @defgroup nrf_modem_wifi_scan_actions Wi-Fi scan actions.
 *
 * @brief Bitmap definitions of scanning behaviour related special features.
 * @{
 */

/** Ignore probe responses and scan only beacons. */
#define NRF_MODEM_WIFI_SCAN_ACTIONS_IGNORE_PROBES 0x01
/** Ignore local MAC addresses. */
#define NRF_MODEM_WIFI_SCAN_ACTIONS_IGNORE_LOCAL_ADDR 0x02
/**
 * Increases the maximum RF gain in the Wi-Fi receiver.
 *
 * This action enhances the sensitivity of the Wi-Fi receiver by increasing
 * the maximum RF gain, allowing for detection of weaker signals.
 *
 * @note While this improves the reception range, it may also:
 *       - increase the current consumption
 *       - increase susceptibility to interference
 */
#define NRF_MODEM_WIFI_SCAN_ACTIONS_INCREASE_MAX_GAIN 0x04
/**
 * Enable offline processing of beacon or probe response frames that
 * cannot be processed in real-time due to cycle budget constraints.
 */
#define NRF_MODEM_WIFI_SCAN_ACTIONS_ENABLE_OFFLINE_PROCESSING 0x08
/** @} */

/**
 * @brief Wi-Fi scan start parameters.
 */
struct nrf_modem_wifi_scan_start_params {
	/**
	 * Scan related special actions.
	 *
	 * This is a bitmap of the requested actions @ref nrf_modem_wifi_scan_actions.
	 */
	uint32_t actions;

	/**
	 * Bitmap of the Wi-Fi channels to be scanned.
	 *
	 * Channels: bit0 = channel1 ... bit13 = channel14
	 *
	 * To scan for all channels, use @ref NRF_MODEM_WIFI_SCAN_WIFI_ALL_CHANNELS.
	 */
	uint16_t channels_bitmap;

	/**
	 * Desired scanning time per channel in milliseconds.
	 *
	 * This can be used to define the desired AP listening time but the modem may shorten it,
	 * e.g., when LTE idle DRX period will not allow desired scanning time.
	 *
	 * Zero means default value.
	 *
	 * Default value is @ref NRF_MODEM_WIFI_SCAN_TIME_DESIRED_MS_DEFAULT.
	 *
	 * Minimum value is @ref NRF_MODEM_WIFI_SCAN_TIME_MS_MIN.
	 */
	uint16_t scan_time_desired_ms;

	/**
	 * Maximum number of reported access points.
	 *
	 * Zero means unlimited number of APs, that is, as many as can be found.
	 */
	uint8_t ap_max_count;
};

/**
 * @brief Application handler prototype for Wi-Fi scan events.
 *
 * @note The handler is executed in an interrupt service routine (ISR).
 *	 Take care to offload any processing as appropriate.
 *
 * @param[in] evt Event data.
 */
typedef void (*nrf_modem_wifi_scan_event_handler_t)(const struct nrf_modem_wifi_scan_event *evt);

/**
 * @brief Set the application event handler for Wi-Fi scan events.
 *
 * The application must set the handler for events coming from the Wi-Fi scan before attempting
 * other operations.
 *
 * @note The handler is executed in an interrupt service routine (ISR).
 *	 Take care to offload any processing as appropriate.
 *
 * @param handler Event handler.
 *
 * @return 0 Success.
 * @retval -NRF_EFAULT @p handler is @c NULL.
 */
int nrf_modem_wifi_scan_event_handler_set(nrf_modem_wifi_scan_event_handler_t handler);

/**
 * @brief Start Wi-Fi scan operation.
 *
 * @details If the function returns zero, the results are sent via one or more
 *          @ref NRF_MODEM_WIFI_SCAN_EVT_RESULT events.
 *
 * @param[in] params Scan operation input parameters.
 *
 * @retval 0 Success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_ENOMEM There is not enough shared memory for this request.
 * @retval -NRF_EACCES Wi-Fi scan is not enabled in the functional mode.
 * @retval -NRF_EFAULT Input parameter is NULL or event handler is not set.
 * @retval -NRF_EINVAL Input parameter has an invalid value.
 * @retval -NRF_EBADMSG The modem responded with wrong response.
 * @retval -NRF_EOPNOTSUPP Wi-Fi scan API is not supported by the modem.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 * @retval -NRF_EALREADY Wi-Fi scan is already in progress.
 */
int nrf_modem_wifi_scan_start(struct nrf_modem_wifi_scan_start_params const *params);

/**
 * @brief Cancel the ongoing Wi-Fi scan operation.
 *
 * @details If there was an ongoing scan operation before this function is called,
 *          the result of the operation is sent via the @ref NRF_MODEM_WIFI_SCAN_EVT_RESULT event.
 *
 * @retval 0 Success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 * @retval -NRF_EACCES Wi-Fi scan is not enabled in the functional mode.
 * @retval -NRF_EFAULT Event handler is not set.
 * @retval -NRF_EBADMSG The modem responded with wrong response.
 * @retval -NRF_EOPNOTSUPP Wi-Fi scan API is not supported by the modem.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 * @retval -NRF_EALREADY No Wi-Fi scan is in progress.
 */
int nrf_modem_wifi_scan_cancel(void);

#ifdef __cplusplus
}
#endif

#endif /* NRF_MODEM_WIFI_SCAN_H__ */

/**@} */
