/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @brief File containing API definitions for the
 * FMAC IF Layer of the Wi-Fi driver.
 */

#include "host_rpu_umac_if.h"
#include "fmac_api.h"
#include "hal_api.h"
#include "fmac_structs.h"
#include "fmac_api.h"
#include "fmac_util.h"
#include "fmac_peer.h"
#include "fmac_vif.h"
#include "fmac_tx.h"
#include "fmac_rx.h"
#include "fmac_cmd.h"
#include "fmac_event.h"
#include "fmac_bb.h"
#include "util.h"

#define RADIO_CMD_STATUS_TIMEOUT 5000



static enum nrf_wifi_status nrf_wifi_fmac_fw_init_rt(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
						     struct nrf_wifi_phy_rf_params *rf_params,
						     bool rf_params_valid,
#ifdef CONFIG_NRF_WIFI_LOW_POWER
						     int sleep_type,
#endif /* CONFIG_NRF_WIFI_LOW_POWER */
						     unsigned int phy_calib,
						     enum op_band op_band,
						     bool beamforming,
						     struct nrf_wifi_tx_pwr_ctrl_params *tx_pwr_ctrl,
						     struct nrf_wifi_board_params *board_params)
{
	unsigned long start_time_us = 0;
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;

	if (!fmac_dev_ctx) {
		nrf_wifi_osal_log_err("%s: Invalid device context",
				      __func__);
		goto out;
	}

	status = umac_cmd_init(fmac_dev_ctx,
			       rf_params,
			       rf_params_valid,
#ifdef CONFIG_NRF_WIFI_LOW_POWER
			       sleep_type,
#endif /* CONFIG_NRF_WIFI_LOW_POWER */
			       phy_calib,
			       op_band,
			       beamforming,
			       tx_pwr_ctrl,
			       board_params);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: UMAC init failed",
				      __func__);
		goto out;
	}
	start_time_us = nrf_wifi_osal_time_get_curr_us();
	while (!fmac_dev_ctx->fw_init_done) {
		nrf_wifi_osal_sleep_ms(1);
#define MAX_INIT_WAIT (5 * 1000 * 1000)
		if (nrf_wifi_osal_time_elapsed_us(start_time_us) >= MAX_INIT_WAIT) {
			break;
		}
	}

	if (!fmac_dev_ctx->fw_init_done) {
		nrf_wifi_osal_log_err("%s: UMAC init timed out",
				      __func__);
		status = NRF_WIFI_STATUS_FAIL;
		goto out;
	}

	status = NRF_WIFI_STATUS_SUCCESS;

out:
	return status;
}

static void nrf_wifi_fmac_fw_deinit_rt(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx)
{
}

void nrf_wifi_fmac_dev_rem_rt(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx)
{
	nrf_wifi_hal_dev_rem(fmac_dev_ctx->hal_dev_ctx);

	nrf_wifi_osal_mem_free(fmac_dev_ctx);
}


enum nrf_wifi_status nrf_wifi_fmac_dev_init_rt(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
#ifdef CONFIG_NRF_WIFI_LOW_POWER
					    int sleep_type,
#endif /* CONFIG_NRF_WIFI_LOW_POWER */
					    unsigned int phy_calib,
					    enum op_band op_band,
					    bool beamforming,
					    struct nrf_wifi_tx_pwr_ctrl_params *tx_pwr_ctrl_params,
					    struct nrf_wifi_tx_pwr_ceil_params *tx_pwr_ceil_params,
					    struct nrf_wifi_board_params *board_params)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_fmac_otp_info otp_info;
	struct nrf_wifi_phy_rf_params phy_rf_params;

	if (!fmac_dev_ctx) {
		nrf_wifi_osal_log_err("%s: Invalid device context",
				      __func__);
		goto out;
	}

	status = nrf_wifi_hal_dev_init(fmac_dev_ctx->hal_dev_ctx);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: nrf_wifi_hal_dev_init failed",
				      __func__);
		goto out;
	}

	fmac_dev_ctx->tx_pwr_ceil_params = nrf_wifi_osal_mem_alloc(sizeof(*tx_pwr_ceil_params));
	nrf_wifi_osal_mem_cpy(fmac_dev_ctx->tx_pwr_ceil_params,
			      tx_pwr_ceil_params,
			      sizeof(*tx_pwr_ceil_params));

	nrf_wifi_osal_mem_set(&otp_info,
			      0xFF,
			      sizeof(otp_info));

	status = nrf_wifi_hal_otp_info_get(fmac_dev_ctx->hal_dev_ctx,
					   &otp_info.info,
					   &otp_info.flags);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: Fetching of RPU OTP information failed",
				      __func__);
		goto out;
	}

	status = nrf_wifi_fmac_rf_params_get(fmac_dev_ctx,
		                             &phy_rf_params);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: RF parameters get failed",
				     __func__);
		goto out;
	}

	status = nrf_wifi_fmac_fw_init_rt(fmac_dev_ctx,
				          &phy_rf_params,
				          true,
#ifdef CONFIG_NRF_WIFI_LOW_POWER
					  sleep_type,
#endif /* CONFIG_NRF_WIFI_LOW_POWER */
					  phy_calib,
					  op_band,
					  beamforming,
					  tx_pwr_ctrl_params,
					  board_params);

	if (status == NRF_WIFI_STATUS_FAIL) {
		nrf_wifi_osal_log_err("%s: nrf_wifi_fmac_fw_init failed",
				      __func__);
		goto out;
	}
out:
	return status;
}

void nrf_wifi_fmac_dev_deinit_rt(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx)
{
	nrf_wifi_osal_mem_free(fmac_dev_ctx->tx_pwr_ceil_params);
	nrf_wifi_fmac_fw_deinit_rt(fmac_dev_ctx);
}

struct nrf_wifi_fmac_priv *nrf_wifi_fmac_init_rt(void)
{
	struct nrf_wifi_fmac_priv *fpriv = NULL;
	struct nrf_wifi_hal_cfg_params hal_cfg_params;

	fpriv = nrf_wifi_osal_mem_zalloc(sizeof(*fpriv));

	if (!fpriv) {
		nrf_wifi_osal_log_err("%s: Unable to allocate fpriv",
				      __func__);
		goto out;
	}


	nrf_wifi_osal_mem_set(&hal_cfg_params,
			      0,
			      sizeof(hal_cfg_params));


	hal_cfg_params.max_cmd_size = MAX_NRF_WIFI_UMAC_CMD_SIZE;
	hal_cfg_params.max_event_size = MAX_EVENT_POOL_LEN;

	fpriv->hpriv = nrf_wifi_hal_init(&hal_cfg_params,
					 &nrf_wifi_fmac_event_callback,
					 NULL);

	if (!fpriv->hpriv) {
		nrf_wifi_osal_log_err("%s: Unable to do HAL init",
				      __func__);
		nrf_wifi_osal_mem_free(fpriv);
		fpriv = NULL;
		goto out;
	}
out:
	return fpriv;
}


void nrf_wifi_fmac_deinit_rt(struct nrf_wifi_fmac_priv *fpriv)
{
	nrf_wifi_hal_deinit(fpriv->hpriv);

	nrf_wifi_osal_mem_free(fpriv);
}

enum nrf_wifi_status wait_for_radio_cmd_status(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					       unsigned int timeout)
{
	unsigned int count = 0;
	enum nrf_wifi_radio_test_err_status radio_cmd_status;
	struct nrf_wifi_fmac_dev_ctx_rt *rt_dev_ctx = NULL;

	rt_dev_ctx = wifi_dev_priv(fmac_dev_ctx);
	do {
		nrf_wifi_osal_sleep_ms(1);
		count++;
	} while ((!rt_dev_ctx->radio_cmd_done) &&
		 (count < timeout));

	if (count == timeout) {
		nrf_wifi_osal_log_err("%s: Timed out (%d secs)",
				      __func__,
					 timeout / 1000);
		goto out;
	}

	radio_cmd_status = rt_dev_ctx->radio_cmd_status;

	if (radio_cmd_status != NRF_WIFI_UMAC_CMD_SUCCESS) {
		nrf_wifi_osal_log_err("%s: Radio test command failed with status %d",
				      __func__,
				      radio_cmd_status);
		goto out;
	}
	return NRF_WIFI_STATUS_SUCCESS;

out:
	return NRF_WIFI_STATUS_FAIL;
}

enum nrf_wifi_status nrf_wifi_fmac_radio_test_init(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
						   struct rpu_conf_params *params)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_radio_test_init_info init_params;
	struct nrf_wifi_fmac_dev_ctx_rt *rt_dev_ctx = NULL;

	rt_dev_ctx = wifi_dev_priv(fmac_dev_ctx);

	nrf_wifi_osal_mem_set(&init_params,
			      0,
			      sizeof(init_params));

	nrf_wifi_osal_mem_cpy(init_params.rf_params,
			      params->rf_params,
			      NRF_WIFI_RF_PARAMS_SIZE);

	nrf_wifi_osal_mem_cpy(&init_params.chan,
			      &params->chan,
			      sizeof(init_params.chan));

	init_params.phy_threshold = params->phy_threshold;
	init_params.phy_calib = params->phy_calib;

	rt_dev_ctx->radio_cmd_done = false;
	status = umac_cmd_prog_init(fmac_dev_ctx,
				    &init_params);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: Unable to init radio test",
				      __func__);
		goto out;
	}

	status = wait_for_radio_cmd_status(fmac_dev_ctx,
					   RADIO_CMD_STATUS_TIMEOUT);
	if (status != NRF_WIFI_STATUS_SUCCESS) {
		goto out;
	}

out:
	return status;
}


enum nrf_wifi_status nrf_wifi_fmac_radio_test_prog_tx(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
						      struct rpu_conf_params *params)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_fmac_dev_ctx_rt *rt_dev_ctx = NULL;

	rt_dev_ctx = wifi_dev_priv(fmac_dev_ctx);

	rt_dev_ctx->radio_cmd_done = false;
	status = umac_cmd_prog_tx(fmac_dev_ctx,
				  params);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: Unable to program radio test TX",
				      __func__);
		goto out;
	}

	status = wait_for_radio_cmd_status(fmac_dev_ctx,
					   RADIO_CMD_STATUS_TIMEOUT);
	if (status != NRF_WIFI_STATUS_SUCCESS) {
		goto out;
	}
out:
	return status;
}


enum nrf_wifi_status nrf_wifi_fmac_radio_test_prog_rx(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
						      struct rpu_conf_params *params)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct rpu_conf_rx_radio_test_params rx_params;
	struct nrf_wifi_fmac_dev_ctx_rt *rt_dev_ctx = NULL;

	rt_dev_ctx = wifi_dev_priv(fmac_dev_ctx);

	nrf_wifi_osal_mem_set(&rx_params,
			      0,
			      sizeof(rx_params));

	rx_params.nss = params->nss;

	nrf_wifi_osal_mem_cpy(rx_params.rf_params,
			      params->rf_params,
			      NRF_WIFI_RF_PARAMS_SIZE);

	nrf_wifi_osal_mem_cpy(&rx_params.chan,
			      &params->chan,
			      sizeof(rx_params.chan));

	rx_params.phy_threshold = params->phy_threshold;
	rx_params.phy_calib = params->phy_calib;
	rx_params.rx = params->rx;

	rt_dev_ctx->radio_cmd_done = false;
	status = umac_cmd_prog_rx(fmac_dev_ctx,
				  &rx_params);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: Unable to program radio test RX",
				      __func__);
		goto out;
	}

	status = wait_for_radio_cmd_status(fmac_dev_ctx,
					   RADIO_CMD_STATUS_TIMEOUT);
	if (status != NRF_WIFI_STATUS_SUCCESS) {
		goto out;
	}
out:
	return status;
}


enum nrf_wifi_status nrf_wifi_fmac_rf_test_rx_cap(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
						  enum nrf_wifi_rf_test rf_test_type,
						  void *cap_data,
						  unsigned short int num_samples,
						  unsigned char lna_gain,
						  unsigned char bb_gain)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_rf_test_capture_params rf_test_cap_params;
	struct nrf_wifi_fmac_dev_ctx_rt *rt_dev_ctx = NULL;
	unsigned int count = 0;

	rt_dev_ctx = wifi_dev_priv(fmac_dev_ctx);

	nrf_wifi_osal_mem_set(&rf_test_cap_params,
			      0,
			      sizeof(rf_test_cap_params));

	rf_test_cap_params.test = rf_test_type;
	rf_test_cap_params.cap_len = num_samples;
	rf_test_cap_params.lna_gain = lna_gain;
	rf_test_cap_params.bb_gain = bb_gain;

	rt_dev_ctx->rf_test_type = rf_test_type;
	rt_dev_ctx->rf_test_cap_data = cap_data;
	rt_dev_ctx->rf_test_cap_sz = (num_samples * 3);

	status = umac_cmd_prog_rf_test(fmac_dev_ctx,
				       &rf_test_cap_params,
				       sizeof(rf_test_cap_params));

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: umac_cmd_prog_rf_test_cap failed",
				      __func__);

		goto out;
	}

	do {
		nrf_wifi_osal_sleep_ms(100);
		count++;
	} while ((rt_dev_ctx->rf_test_type != NRF_WIFI_RF_TEST_MAX) &&
		 (count < NRF_WIFI_FMAC_RF_TEST_EVNT_TIMEOUT));

	if (count == NRF_WIFI_FMAC_RF_TEST_EVNT_TIMEOUT) {
		nrf_wifi_osal_log_err("%s: Timed out",
				      __func__);
		rt_dev_ctx->rf_test_type = NRF_WIFI_RF_TEST_MAX;
		rt_dev_ctx->rf_test_cap_data = NULL;
		status = NRF_WIFI_STATUS_FAIL;
		goto out;
	}

out:
	return status;
}


enum nrf_wifi_status nrf_wifi_fmac_rf_test_tx_tone(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
						  unsigned char enable,
						  signed char tone_freq,
						  signed char tx_power)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_rf_test_tx_params rf_test_tx_params;
	struct nrf_wifi_fmac_dev_ctx_rt *rt_dev_ctx = NULL;
	unsigned int count = 0;

	rt_dev_ctx = wifi_dev_priv(fmac_dev_ctx);

	nrf_wifi_osal_mem_set(&rf_test_tx_params,
			      0,
			      sizeof(rf_test_tx_params));

	rf_test_tx_params.test = NRF_WIFI_RF_TEST_TX_TONE;
	rf_test_tx_params.tone_freq = tone_freq;
	rf_test_tx_params.tx_pow = tx_power;
	rf_test_tx_params.enabled = enable;

	rt_dev_ctx->rf_test_type = NRF_WIFI_RF_TEST_TX_TONE;
	rt_dev_ctx->rf_test_cap_data = NULL;
	rt_dev_ctx->rf_test_cap_sz = 0;

	status = umac_cmd_prog_rf_test(fmac_dev_ctx,
				       &rf_test_tx_params,
				       sizeof(rf_test_tx_params));

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: umac_cmd_prog_rf_test_tx_tone failed",
				      __func__);

		goto out;
	}

	do {
		nrf_wifi_osal_sleep_ms(100);
		count++;
	} while ((rt_dev_ctx->rf_test_type != NRF_WIFI_RF_TEST_MAX) &&
		 (count < NRF_WIFI_FMAC_RF_TEST_EVNT_TIMEOUT));

	if (count == NRF_WIFI_FMAC_RF_TEST_EVNT_TIMEOUT) {
		nrf_wifi_osal_log_err("%s: Timed out",
				      __func__);
		rt_dev_ctx->rf_test_type = NRF_WIFI_RF_TEST_MAX;
		rt_dev_ctx->rf_test_cap_data = NULL;
		status = NRF_WIFI_STATUS_FAIL;
		goto out;
	}

out:
	return status;
}


enum nrf_wifi_status nrf_wifi_fmac_rf_test_dpd(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					       unsigned char enable)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_rf_test_dpd_params rf_test_dpd_params;
	struct nrf_wifi_fmac_dev_ctx_rt *rt_dev_ctx = NULL;
	unsigned int count = 0;

	rt_dev_ctx = wifi_dev_priv(fmac_dev_ctx);

	nrf_wifi_osal_mem_set(&rf_test_dpd_params,
			      0,
			      sizeof(rf_test_dpd_params));

	rf_test_dpd_params.test = NRF_WIFI_RF_TEST_DPD;
	rf_test_dpd_params.enabled = enable;

	rt_dev_ctx->rf_test_type = NRF_WIFI_RF_TEST_DPD;
	rt_dev_ctx->rf_test_cap_data = NULL;
	rt_dev_ctx->rf_test_cap_sz = 0;

	status = umac_cmd_prog_rf_test(fmac_dev_ctx,
					   &rf_test_dpd_params,
					   sizeof(rf_test_dpd_params));

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: umac_cmd_prog_rf_test_dpd failed",
				      __func__);

		goto out;
	}

	do {
		nrf_wifi_osal_sleep_ms(100);
		count++;
	} while ((rt_dev_ctx->rf_test_type != NRF_WIFI_RF_TEST_MAX) &&
		 (count < NRF_WIFI_FMAC_RF_TEST_EVNT_TIMEOUT));

	if (count == NRF_WIFI_FMAC_RF_TEST_EVNT_TIMEOUT) {
		nrf_wifi_osal_log_err("%s: Timed out",
				      __func__);
		rt_dev_ctx->rf_test_type = NRF_WIFI_RF_TEST_MAX;
		rt_dev_ctx->rf_test_cap_data = NULL;
		status = NRF_WIFI_STATUS_FAIL;
		goto out;
	}

out:
	return status;
}


enum nrf_wifi_status nrf_wifi_fmac_rf_get_temp(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_temperature_params rf_test_get_temperature;
	struct nrf_wifi_fmac_dev_ctx_rt *rt_dev_ctx = NULL;
	unsigned int count = 0;

	rt_dev_ctx = wifi_dev_priv(fmac_dev_ctx);

	nrf_wifi_osal_mem_set(&rf_test_get_temperature,
			      0,
			      sizeof(rf_test_get_temperature));

	rf_test_get_temperature.test = NRF_WIFI_RF_TEST_GET_TEMPERATURE;

	rt_dev_ctx->rf_test_type = NRF_WIFI_RF_TEST_GET_TEMPERATURE;
	rt_dev_ctx->rf_test_cap_data = NULL;
	rt_dev_ctx->rf_test_cap_sz = 0;

	status = umac_cmd_prog_rf_test(fmac_dev_ctx,
					   &rf_test_get_temperature,
					   sizeof(rf_test_get_temperature));

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: umac_cmd_prog_rf_get_temperature failed",
				      __func__);

		goto out;
	}

	do {
		nrf_wifi_osal_sleep_ms(100);
		count++;
	} while ((rt_dev_ctx->rf_test_type != NRF_WIFI_RF_TEST_MAX) &&
		 (count < NRF_WIFI_FMAC_RF_TEST_EVNT_TIMEOUT));

	if (count == NRF_WIFI_FMAC_RF_TEST_EVNT_TIMEOUT) {
		nrf_wifi_osal_log_err("%s: Timed out",
				      __func__);
		rt_dev_ctx->rf_test_type = NRF_WIFI_RF_TEST_MAX;
		rt_dev_ctx->rf_test_cap_data = NULL;
		status = NRF_WIFI_STATUS_FAIL;
		goto out;
	}

out:
	return status;
}

enum nrf_wifi_status nrf_wifi_fmac_rf_get_rf_rssi(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_rf_get_rf_rssi rf_get_rf_rssi_params;
	struct nrf_wifi_fmac_dev_ctx_rt *rt_dev_ctx = NULL;
	unsigned int count = 0;

	rt_dev_ctx = wifi_dev_priv(fmac_dev_ctx);

	nrf_wifi_osal_mem_set(&rf_get_rf_rssi_params,
			      0,
			      sizeof(rf_get_rf_rssi_params));

	rf_get_rf_rssi_params.test = NRF_WIFI_RF_TEST_RF_RSSI;
	rf_get_rf_rssi_params.lna_gain = 3;
	rf_get_rf_rssi_params.bb_gain = 10;

	rt_dev_ctx->rf_test_type = NRF_WIFI_RF_TEST_RF_RSSI;
	rt_dev_ctx->rf_test_cap_data = NULL;
	rt_dev_ctx->rf_test_cap_sz = 0;

	status = umac_cmd_prog_rf_test(fmac_dev_ctx,
					   &rf_get_rf_rssi_params,
					   sizeof(rf_get_rf_rssi_params));

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: umac_cmd_prog_rf_get_rf_rssi failed",
				      __func__);

		goto out;
	}

	do {
		nrf_wifi_osal_sleep_ms(100);
		count++;
	} while ((rt_dev_ctx->rf_test_type != NRF_WIFI_RF_TEST_MAX) &&
		 (count < NRF_WIFI_FMAC_RF_TEST_EVNT_TIMEOUT));

	if (count == NRF_WIFI_FMAC_RF_TEST_EVNT_TIMEOUT) {
		nrf_wifi_osal_log_err("%s: Timed out",
				      __func__);
		rt_dev_ctx->rf_test_type = NRF_WIFI_RF_TEST_MAX;
		rt_dev_ctx->rf_test_cap_data = NULL;
		status = NRF_WIFI_STATUS_FAIL;
		goto out;
	}

out:
	return status;
}


enum nrf_wifi_status nrf_wifi_fmac_set_xo_val(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
						  unsigned char value)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_rf_test_xo_calib nrf_wifi_rf_test_xo_calib_params;
	struct nrf_wifi_fmac_dev_ctx_rt *rt_dev_ctx = NULL;
	unsigned int count = 0;

	rt_dev_ctx = wifi_dev_priv(fmac_dev_ctx);

	nrf_wifi_osal_mem_set(&nrf_wifi_rf_test_xo_calib_params,
			      0,
			      sizeof(nrf_wifi_rf_test_xo_calib_params));

	nrf_wifi_rf_test_xo_calib_params.test = NRF_WIFI_RF_TEST_XO_CALIB;
	nrf_wifi_rf_test_xo_calib_params.xo_val = value;


	rt_dev_ctx->rf_test_type = NRF_WIFI_RF_TEST_XO_CALIB;
	rt_dev_ctx->rf_test_cap_data = NULL;
	rt_dev_ctx->rf_test_cap_sz = 0;

	status = umac_cmd_prog_rf_test(fmac_dev_ctx,
					   &nrf_wifi_rf_test_xo_calib_params,
					   sizeof(nrf_wifi_rf_test_xo_calib_params));

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: umac_cmd_prog_set_xo_val failed",
				      __func__);

		goto out;
	}

	do {
		nrf_wifi_osal_sleep_ms(100);
		count++;
	} while ((rt_dev_ctx->rf_test_type != NRF_WIFI_RF_TEST_MAX) &&
		 (count < NRF_WIFI_FMAC_RF_TEST_EVNT_TIMEOUT));

	if (count == NRF_WIFI_FMAC_RF_TEST_EVNT_TIMEOUT) {
		nrf_wifi_osal_log_err("%s: Timed out",
				      __func__);
		rt_dev_ctx->rf_test_type = NRF_WIFI_RF_TEST_MAX;
		rt_dev_ctx->rf_test_cap_data = NULL;
		status = NRF_WIFI_STATUS_FAIL;
		goto out;
	}

out:
	return status;
}


enum nrf_wifi_status nrf_wifi_fmac_rf_test_compute_xo(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_rf_get_xo_value rf_get_xo_value_params;
	struct nrf_wifi_fmac_dev_ctx_rt *rt_dev_ctx = NULL;
	unsigned int count = 0;

	rt_dev_ctx = wifi_dev_priv(fmac_dev_ctx);

	nrf_wifi_osal_mem_set(&rf_get_xo_value_params,
			      0,
			      sizeof(rf_get_xo_value_params));

	rf_get_xo_value_params.test = NRF_WIFI_RF_TEST_XO_TUNE;

	rt_dev_ctx->rf_test_type = NRF_WIFI_RF_TEST_XO_TUNE;
	rt_dev_ctx->rf_test_cap_data = NULL;
	rt_dev_ctx->rf_test_cap_sz = 0;

	status = umac_cmd_prog_rf_test(fmac_dev_ctx,
					   &rf_get_xo_value_params,
					   sizeof(rf_get_xo_value_params));

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: umac_cmd_prog_rf_get_xo_value failed",
				      __func__);

		goto out;
	}

	do {
		nrf_wifi_osal_sleep_ms(100);
		count++;
	} while ((rt_dev_ctx->rf_test_type != NRF_WIFI_RF_TEST_MAX) &&
		 (count < NRF_WIFI_FMAC_RF_TEST_EVNT_TIMEOUT));

	if (count == NRF_WIFI_FMAC_RF_TEST_EVNT_TIMEOUT) {
		nrf_wifi_osal_log_err("%s: Timed out",
				      __func__);
		rt_dev_ctx->rf_test_type = NRF_WIFI_RF_TEST_MAX;
		rt_dev_ctx->rf_test_cap_data = NULL;
		status = NRF_WIFI_STATUS_FAIL;
		goto out;
	}

out:
	return status;
}
