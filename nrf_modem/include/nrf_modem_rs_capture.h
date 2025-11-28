/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file nrf_modem_rs_capture.h
 *
 * @defgroup nrf_modem_rs_capture RS capture API
 * @{
 * @brief API for accessing the RS capture module on the modem.
 *
 * @note This is only supported by the following modem firmware:
 * - mfw_nrf91x1 >= v2.0.3
 * - mfw_nrf9151-ntn
 */
#ifndef NRF_MODEM_RS_CAPTURE_H__
#define NRF_MODEM_RS_CAPTURE_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Maximum number of data samples in 'data' member in @ref nrf_modem_rs_capture_prs_sample. */
#define NRF_MODEM_RS_CAPTURE_MAX_DATA_SAMPLES 1536

/**
 * @defgroup nrf_modem_rs_capture_status RS capture status values.
 * @brief    RS capture status values.
 * @{
 */
/** Success. */
#define NRF_MODEM_RS_CAPTURE_STATUS_SUCCESS              0x00
/** Success, cell available. */
#define NRF_MODEM_RS_CAPTURE_STATUS_CELL_AVAILABLE       0x00
/** Memory not available. */
#define NRF_MODEM_RS_CAPTURE_STATUS_NO_MEM               0x01
/** Cell not available. */
#define NRF_MODEM_RS_CAPTURE_STATUS_NO_CELL              0x03
/** Modem or capture session not active. */
#define NRF_MODEM_RS_CAPTURE_STATUS_NOT_ACTIVE           0x04
/** Modem temporarily busy. */
#define NRF_MODEM_RS_CAPTURE_STATUS_BUSY                 0x05
/** Operation not supported. */
#define NRF_MODEM_RS_CAPTURE_STATUS_NOT_SUPPORTED        0x06
/** Operation failed due to a timeout. */
#define NRF_MODEM_RS_CAPTURE_STATUS_TIMEOUT              0x07
/** Session stopped. */
#define NRF_MODEM_RS_CAPTURE_STATUS_STOPPED              0x08
/** @} */

/** Event IDs. */
enum nrf_modem_rs_capture_event_id {
	/** PRS sample notification.
	 *
	 * Event data is of type @ref nrf_modem_rs_capture_prs_sample
	 * in @ref nrf_modem_rs_capture_event_data.
	 */
	NRF_MODEM_RS_CAPTURE_EVT_PRS_SAMPLE = 1,
	/** Cell status notification.
	 *
	 * Event data is of type @ref nrf_modem_rs_capture_cell_status
	 * in @ref nrf_modem_rs_capture_event_data.
	 */
	NRF_MODEM_RS_CAPTURE_EVT_CELL_STATUS,
	/** RS capture session aborted.
	 *
	 * There is no associated event data.
	 */
	NRF_MODEM_RS_CAPTURE_EVT_ABORT,
};

/** RS capture session start request parameters. */
struct nrf_modem_rs_capture_session_start_req {
	/** RS capture session timeout in milliseconds. */
	uint32_t timeout;
};

/** RS capture session start response parameters. */
struct nrf_modem_rs_capture_session_start_resp {
	/** PLMN ID.
	 *
	 * Bytes encoded as follows:
	 *     mcc2_mcc1, mnc3_mcc3, mnc2_mnc1
	 */
	uint8_t plmn[3];
	/** 28bit cell id. */
	uint32_t cell_id;
	/** Mobile Country Code. Range: 0...999. */
	uint16_t mcc;
	/** Mobile Network Code. Range: 0...999. */
	uint16_t mnc;
};

/** PRS capture configure request parameters. */
struct nrf_modem_rs_capture_prs_config {
	/** Set of gain offset values, in dB, used for RS capture. Range: -127...127.
	 *
	 * Offsets are relative to the normal LTE gain settings for the serving cell.
	 *
	 * This pattern is repeated one after the other in the RS capture procedure.
	 * The table must be filled entirely, but the same value can be set into several indices.
	 * The first value is applied in the first PRS occasion after SFN0.
	 */
	int8_t gain_offset[16];
	/** Pattern of narrowband values used for RS capture. Range for each value is 0...16.
	 *
	 * Narrowband numbering is as in 3GPP TS 36.211 added by one.
	 * Narrowband 0 is reserved for the central narrowband exactly in the middle of system BW.
	 *
	 * This pattern is repeated one after the other in the RS capture procedure.
	 * The table must be filled entirely, but the same value can be set into several indices.
	 * The first value is applied in the first PRS occasion after SFN0.
	 */
	uint8_t nb_pattern[16];
	/** PRS configuration index specified in 3GPP TS 36.211. Range is 0...4095. */
	uint16_t i_prs;
	/** Timing offset. Range: -2192...2192.
	 *
	 * Offset of FFT window compared to own cell timing, in 30.72MHz samples.
	 */
	int16_t timing_offset;
	/** Index of the configuration. Range: 0...255
	 *
	 * Returned in @ref nrf_modem_rs_capture_prs_sample in
	 * @ref NRF_MODEM_RS_CAPTURE_EVT_PRS_SAMPLE.
	 */
	uint8_t index;
};

/** PRS sample. */
struct nrf_modem_rs_capture_prs_sample {
	/** PLMN ID.
	 *
	 * Bytes encoded as follows:
	 *     mcc2_mcc1, mnc3_mcc3, mnc2_mnc1
	 */
	uint8_t plmn[3];
	/** 28bit cell id. */
	uint32_t cell_id;
	/** Mobile Country Code. Range: 0...999. */
	uint16_t mcc;
	/** Mobile Network Code. Range: 0...999. */
	uint16_t mnc;
	/** Index of the configuration used to capture the sample. Range: 0 ... 255
	 *
	 * Index matches the one given in @ref nrf_modem_rs_capture_prs_config.
	 */
	uint8_t index;
	/** Narrowband. Range: 0...16.
	 *
	 * Narrowband numbering is as in 3GPP TS 36.211 added by one.
	 * Narrowband 0 is reserved for the central narrowband exactly in the middle of system BW.
	 */
	uint8_t nb;
	/** System frame number. Range: 0...1023. */
	uint16_t sfn;
	/** Number of captured 16bit frequency domain samples, I and Q separately.
	 *
	 * 0 or 1536.
	 */
	uint16_t num_samples;
	/** Timing offset. Range: -2192...2192.
	 *
	 * Offset of FFT window compared to own cell timing, in 30.72MHz samples.
	 */
	int16_t timing_offset;
	/** RSSI in dBm. Range: -20...-120. */
	int8_t rssi;
	/** Gain offset in dB. Range: -127...127.
	 *
	 * Offset is relative to the normal LTE gain settings for the serving cell.
	 */
	int8_t gain_offset;

	/** Data samples.
	 *
	 * Number of data samples is indicated in @ref num_samples variable.
	 *
	 * Frequency domain samples, 16bit signed I & Q, I first.
	 *
	 * 8 PRB's (96 subcarriers) are stored per OFDM symbol, in frequency increasing order
	 * (subcarrier #0 first, #95 last).
	 *
	 * Only OFDM symbols containing PRS are stored except in 4 TX antenna configuration, where
	 * every eight symbols is invalid.
	 *
	 * If present based on @ref num_samples variable, number of samples are
	 * 2 (I+Q) * 8 PRS symbols * 8 PRBs * 12 subcarriers = 1536 samples.
	 *
	 * Centermost 6 PRB's (#1..#6, from #0..#7) within these 8 PRB's always correspond to
	 * the configured LTE-M narrowband (@ref nb variable).
	 *
	 * If either or both PRB's on the edge (PRB #0 and/or #7) are outside of the
	 * LTE cell bandwidth, they are still returned but do not contain LTE signal.
	 * This happens if cell bandwidth is 1.4MHz, or cell bandwidth is 5MHz and first or
	 * last narrowband is configured.
	 *
	 * When LTE-M narrowband reception is configured (@ref nb variable is 1..16):
	 *   - UE RF center frequency is set at the first subcarrier of PRB #4 (subcarrier #48),
	 *     except when physical cell ID mod 3 == 0, when it is at the second subcarrier of
	 *     PRB #4 (subcarrier #49).
	 *   - This subcarrier is zeroed in the data.
	 *   - Phase offset caused by subcarrier offset between UE RF center frequency and
	 *     cell center frequency is compensated in the data.
	 *   - If empty subcarrier at the cell center is within the 8 PRB reception window,
	 *     it is removed from the data.
	 *
	 * When LTE cell center frequency reception is configured (@ref nb variable is zero):
	 *   - UE RF center frequency is set at the empty subcarrier at cell center.
	 *     This subcarrier is removed from the data.
	 */
	int16_t *data;
};

/** Cell status. */
struct nrf_modem_rs_capture_cell_status {
	/** PLMN ID.
	 *
	 * Bytes encoded as follows:
	 *     mcc2_mcc1, mnc3_mcc3, mnc2_mnc1
	 */
	uint8_t plmn[3];
	/** 28bit cell id. */
	uint32_t cell_id;
	/** Mobile Country Code. Range: 0...999. */
	uint16_t mcc;
	/** Mobile Network Code. Range: 0...999. */
	uint16_t mnc;
};

/** RS capture event data. */
struct nrf_modem_rs_capture_event_data {
	/** Event ID. */
	enum nrf_modem_rs_capture_event_id event_id;

	/** Status from @ref nrf_modem_rs_capture_status.
	 *
	 * Possible values for @ref NRF_MODEM_RS_CAPTURE_EVT_PRS_SAMPLE event:
	 *   - NRF_MODEM_RS_CAPTURE_STATUS_SUCCESS Success.
	 *   - NRF_MODEM_RS_CAPTURE_STATUS_NO_MEM No memory for sample.
	 *
	 * Possible values for @ref NRF_MODEM_RS_CAPTURE_EVT_CELL_STATUS event:
	 *   - NRF_MODEM_RS_CAPTURE_STATUS_CELL_AVAILABLE: Cell available.
	 *   - NRF_MODEM_RS_CAPTURE_STATUS_NO_CELL: Cell lost.
	 *
	 * Possible values for @ref NRF_MODEM_RS_CAPTURE_EVT_ABORT event:
	 *   - NRF_MODEM_RS_CAPTURE_STATUS_NO_CELL: Cell lost.
	 *   - NRF_MODEM_RS_CAPTURE_STATUS_STOPPED: Session duration exceeded or
	 *     higher priority activity stopped the session.
	 */
	int32_t status;

	/** Event specific data. */
	union {
		/** Data for @ref NRF_MODEM_RS_CAPTURE_EVT_PRS_SAMPLE event. */
		struct nrf_modem_rs_capture_prs_sample prs_sample;
		/** Data for @ref NRF_MODEM_RS_CAPTURE_EVT_CELL_STATUS event. */
		struct nrf_modem_rs_capture_cell_status cell_status;
	};
};

/** @brief Event handler prototype.
 *
 * @param[in] event_data Event data.
 */
typedef void (*nrf_modem_rs_capture_event_handler_type_t)(
	const struct nrf_modem_rs_capture_event_data *event_data);

/** @brief Version negotiation for the RS capture API.
 *
 * @details Request desired RS capture version and get the selected version.
 *          Selected version is decided by the modem based on its support and requested version.
 *
 * @param[in] version Requested version.
 *
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EBADMSG The modem responded with wrong response.
 * @retval -NRF_ENOMEM There is not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 * @returns Positive value indicates selected version.
 */
int32_t nrf_modem_rs_capture_version(uint8_t version);

/** @brief Starts RS capture session.
 *
 * @details This function prepares RS capture sampling session but the actual sampling
 * is not started until @ref nrf_modem_rs_capture_prs_configure function is called.
 * An event handler is also given to this function to receive the samples.
 *
 * @note The event handler is executed in interrupt context so a lot of processing inside the
 *       handler may have performance implications and side effects. It's recommended to use the
 *       handler only for signaling or re-scheduling processing to a separate thread.
 *
 * @param[in] req Session start request parameters.
 * @param[out] resp Session start response parameters if 0 is returned.
 * @param[in] callback Event handler function.
 *
 * @retval 0 on success.
 * @retval -NRF_EBUSY Another RS capture session is ongoing.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EBADMSG The modem responded with wrong response.
 * @retval -NRF_EFAULT Input parameter is NULL.
 * @retval -NRF_ENOMEM There is not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 * @retval NRF_MODEM_RS_CAPTURE_STATUS_NO_CELL Cell not found.
 * @retval NRF_MODEM_RS_CAPTURE_STATUS_BUSY Resources not available or session already started.
 * @retval NRF_MODEM_RS_CAPTURE_STATUS_NOT_SUPPORTED Capture not supported in current LTE system.
 * @retval NRF_MODEM_RS_CAPTURE_STATUS_TIMEOUT Timeout.
 * @retval NRF_MODEM_RS_CAPTURE_STATUS_STOPPED Stop requested during start.
 */
int32_t nrf_modem_rs_capture_session_start(
	const struct nrf_modem_rs_capture_session_start_req *req,
	struct nrf_modem_rs_capture_session_start_resp *resp,
	nrf_modem_rs_capture_event_handler_type_t callback);

/** @brief Stops RS capture.
 *
 * @retval 0 on success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EBADMSG The modem responded with wrong response.
 * @retval -NRF_ENOMEM There is not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 */
int32_t nrf_modem_rs_capture_session_stop(void);

/** @brief Configure PRS capture.
 *
 * @details This function can be used to start PRS sampling, or reconfigure an ongoing sampling.
 *
 * Given configuration is applied immediately and index variable in
 * @ref nrf_modem_rs_capture_prs_sample indicates when exactly this happened.
 *
 * @param[in] req PRS configure request parameters.
 *
 * @retval 0 on success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EBADMSG The modem responded with wrong response.
 * @retval -NRF_EFAULT Input parameter is NULL.
 * @retval -NRF_ENOMEM There is not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 * @retval NRF_MODEM_RS_CAPTURE_STATUS_NOT_ACTIVE Session not active.
 * @retval NRF_MODEM_RS_CAPTURE_STATUS_NOT_SUPPORTED Parameter values not supported.
 */
int32_t nrf_modem_rs_capture_prs_configure(const struct nrf_modem_rs_capture_prs_config *req);

#ifdef __cplusplus
}
#endif

#endif /* NRF_MODEM_RS_CAPTURE_H__ */
/** @} */
