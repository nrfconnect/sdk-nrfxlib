/**
 * @file fmac_event.h
 *
 * @brief Header containing event specific declarations for the FMAC IF Layer
 *        of the Wi-Fi driver.
 */

#ifndef __FMAC_EVENT_H__
#define __FMAC_EVENT_H__

/**
 * @brief RPU event classifier and handler.
 *
 * This callback classifies and processes an event. This classification of the
 * event is based on whether it contains data or control messages
 * and invokes further handlers based on that.
 *
 * @param data Pointer to the device driver context.
 * @param event_data Pointer to event data.
 * @param len Length of event data pointed to by @p event_data.
 *
 * @return Status
 *         - Pass: NRF_WIFI_STATUS_SUCCESS
 *         - Fail: NRF_WIFI_STATUS_FAIL
 */
enum nrf_wifi_status nrf_wifi_fmac_event_callback(void *data,
		void *event_data,
		unsigned int len);
#endif /* __FMAC_EVENT_H__ */
