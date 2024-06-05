/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @brief File containing API definitions for the
 * FMAC IF Layer of the Wi-Fi driver.
 */

#include "host_rpu_umac_if.h"
#include "fmac_api.h"
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

struct nrf_wifi_proc {
	const enum RPU_PROC_TYPE type;
	const char *name;
	bool is_patch_present;
};

struct nrf_wifi_proc wifi_proc[] = {
	{RPU_PROC_TYPE_MCU_LMAC, "LMAC", true},
	{RPU_PROC_TYPE_MCU_UMAC, "UMAC", true},
};

static int nrf_wifi_patch_version_compat(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
				const unsigned int version)
{
	unsigned int family, major, minor, patch;

	family = (version >> 24) & 0xff;
	major = (version >> 16) & 0xff;
	minor = (version >> 8) & 0xff;
	patch = (version >> 0) & 0xff;

	if (family != RPU_FAMILY) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
			"Incompatible RPU version: %d, expected: %d",
			family, RPU_FAMILY);
		return -1;
	}

	if (major != RPU_MAJOR_VERSION) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
			"Incompatible RPU major version: %d, expected: %d",
			major, RPU_MAJOR_VERSION);
		return -1;
	}

	/* TODO: Allow minor version to be different */
	if (minor != RPU_MINOR_VERSION) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
			"Incompatible RPU minor version: %d, expected: %d",
			minor, RPU_MINOR_VERSION);
		return -1;
	}

	/* TODO: Allow patch version to be different */
	if (patch != RPU_PATCH_VERSION) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
			"Incompatible RPU patch version: %d, expected: %d",
			patch, RPU_PATCH_VERSION);
		return -1;
	}

	return 0;
}

static int nrf_wifi_patch_feature_flags_compat(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
				const unsigned int feature_flags)
{
#ifdef CONFIG_NRF700X_RADIO_TEST
	if (!(feature_flags & NRF70_FEAT_RADIO_TEST)) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
			"Radio test feature flag not set");
		return -1;
	}
#elif defined(CONFIG_NRF700X_SCAN_ONLY)
	if (!(feature_flags & NRF70_FEAT_SCAN_ONLY)) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
			"Scan only feature flag not set");
		return -1;
	}
#elif defined(CONFIG_NRF700X_SYSTEM_MODE)
	if (!(feature_flags & NRF70_FEAT_SYSTEM_MODE)) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
			"System mode feature flag not set");
		return -1;
	}
#elif defined(CONFIG_NRF700X_SYSTEM_WITH_RAW_MODES)
	if (!(feature_flags & NRF70_FEAT_SYSTEM_WITH_RAW_MODES)) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
			"System with raw modes feature flag not set");
		return -1;
	}
#else
	nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
		"Invalid feature flags: 0x%x or build configuration",
		feature_flags);
#endif

	return 0;
}

enum nrf_wifi_status nrf_wifi_validate_fw_header(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
						 struct nrf70_fw_image_info *info)
{

	nrf_wifi_osal_log_dbg(fmac_dev_ctx->fpriv->opriv,
		"Signature: 0x%x", info->signature);
	nrf_wifi_osal_log_dbg(fmac_dev_ctx->fpriv->opriv,
		"num_images: %d", info->num_images);
	nrf_wifi_osal_log_dbg(fmac_dev_ctx->fpriv->opriv,
		"version: 0x%x", info->version);
	nrf_wifi_osal_log_dbg(fmac_dev_ctx->fpriv->opriv,
		"feature_flags: %d", info->feature_flags);

	if (info->signature != NRF_WIFI_PATCH_SIGNATURE) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
			"Invalid patch signature: 0x%x, expected: 0x%x",
			info->signature, NRF_WIFI_PATCH_SIGNATURE);
		return NRF_WIFI_STATUS_FAIL;
	}

	if (info->num_images != NRF_WIFI_PATCH_NUM_IMAGES) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
			"Invalid number of images, expected %d, got %d",
			NRF_WIFI_PATCH_NUM_IMAGES, info->num_images);
		return NRF_WIFI_STATUS_FAIL;
	}

	if (nrf_wifi_patch_version_compat(fmac_dev_ctx, info->version) != 0) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
			"Incompatible patch");
		return NRF_WIFI_STATUS_FAIL;
	}

	if (nrf_wifi_patch_feature_flags_compat(fmac_dev_ctx, info->feature_flags) != 0) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
			"Incompatible feature flags");
		return NRF_WIFI_STATUS_FAIL;
	}

	return NRF_WIFI_STATUS_SUCCESS;
}

enum nrf_wifi_status nrf_wifi_fmac_fw_parse(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					   const void *fw_data,
					   unsigned int fw_size,
					   struct nrf_wifi_fmac_fw_info *fw_info)
{
	struct nrf70_fw_image_info *info = (struct nrf70_fw_image_info *)fw_data;
	unsigned int offset;
	unsigned int image_id;

	if (!fw_data || !fw_size || !fw_info) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
			"Invalid parameters");
		return NRF_WIFI_STATUS_FAIL;
	}

	if (fw_size < sizeof(struct nrf70_fw_image_info)) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
			"Invalid fw_size: %d, minimum size: %d",
			fw_size, sizeof(struct nrf70_fw_image_info));
		return NRF_WIFI_STATUS_FAIL;
	}


	if (nrf_wifi_validate_fw_header(fmac_dev_ctx, info) != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
			"Invalid fw header");
		return NRF_WIFI_STATUS_FAIL;
	}

	offset = sizeof(struct nrf70_fw_image_info);

	nrf_wifi_osal_log_dbg(fmac_dev_ctx->fpriv->opriv, "====");
	for (image_id = 0; image_id < info->num_images; image_id++) {
		struct nrf70_fw_image *image =
			(struct nrf70_fw_image *)((char *)fw_data + offset);
		const void *data = (char *)fw_data + offset + sizeof(struct nrf70_fw_image);

		if (offset + sizeof(struct nrf70_fw_image) + image->len > fw_size) {
			nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				"Invalid fw_size: %d for image[%d] len: %d",
				fw_size, image_id, image->len);
			return NRF_WIFI_STATUS_FAIL;
		}

		nrf_wifi_osal_log_dbg(fmac_dev_ctx->fpriv->opriv,
			"image[%d] type: %d", image_id, image->type);
		nrf_wifi_osal_log_dbg(fmac_dev_ctx->fpriv->opriv,
			"image[%d] len: %d", image_id, image->len);
		nrf_wifi_osal_log_dbg(fmac_dev_ctx->fpriv->opriv,
			"====");

		switch (image_id) {
		case NRF70_IMAGE_LMAC_PRI:
			fw_info->lmac_patch_pri.data = data;
			fw_info->lmac_patch_pri.size = image->len;
			break;
		case NRF70_IMAGE_LMAC_SEC:
			fw_info->lmac_patch_sec.data = data;
			fw_info->lmac_patch_sec.size = image->len;
			break;
		case NRF70_IMAGE_UMAC_PRI:
			fw_info->umac_patch_pri.data = data;
			fw_info->umac_patch_pri.size = image->len;
			break;
		case NRF70_IMAGE_UMAC_SEC:
			fw_info->umac_patch_sec.data = data;
			fw_info->umac_patch_sec.size = image->len;
			break;
		default:
			nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				"Invalid image id: %d", image_id);
			break;
		}

		offset += sizeof(struct nrf70_fw_image) + image->len;
	}

	return NRF_WIFI_STATUS_SUCCESS;
}

enum nrf_wifi_status nrf_wifi_fmac_fw_reset(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	int i = 0;

	for (i = 0; i < ARRAY_SIZE(wifi_proc); i++) {
		status = nrf_wifi_hal_proc_reset(fmac_dev_ctx->hal_dev_ctx,
						 wifi_proc[i].type);

		if (status != NRF_WIFI_STATUS_SUCCESS) {
			nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
					      "%s: %s processor reset failed\n",
					      __func__, wifi_proc[i].name);
			return NRF_WIFI_STATUS_FAIL;
		}
	}

	return NRF_WIFI_STATUS_SUCCESS;
}

enum nrf_wifi_status nrf_wifi_fmac_fw_boot(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	int i = 0;

	for (i = 0; i < ARRAY_SIZE(wifi_proc); i++) {
		status = nrf_wifi_hal_fw_patch_boot(fmac_dev_ctx->hal_dev_ctx,
						    wifi_proc[i].type,
						    wifi_proc[i].is_patch_present);

		if (status != NRF_WIFI_STATUS_SUCCESS) {
			nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
					      "%s: %s processor ROM boot failed\n",
					      __func__, wifi_proc[i].name);
			return NRF_WIFI_STATUS_FAIL;
		}

		status = nrf_wifi_hal_fw_chk_boot(fmac_dev_ctx->hal_dev_ctx,
						  wifi_proc[i].type);

		if (status != NRF_WIFI_STATUS_SUCCESS) {
			nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
					      "%s: %s processor ROM boot check failed\n",
					      __func__, wifi_proc[i].name);
			return NRF_WIFI_STATUS_FAIL;
		}
	}

	return NRF_WIFI_STATUS_SUCCESS;
}


enum nrf_wifi_status nrf_wifi_fmac_fw_chunk_load(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					   enum RPU_PROC_TYPE rpu_proc,
					   struct nrf_wifi_fmac_fw_chunk_info *fw_chunk)
{
	return hal_fw_patch_chunk_load(fmac_dev_ctx->hal_dev_ctx,
				       rpu_proc,
				       fw_chunk->dest_addr,
				       fw_chunk->data,
				       fw_chunk->size);
}

enum nrf_wifi_status nrf_wifi_fmac_fw_load(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					   struct nrf_wifi_fmac_fw_info *fmac_fw)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;

	status = nrf_wifi_fmac_fw_reset(fmac_dev_ctx);
	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: FW reset failed\n",
				      __func__);
		goto out;
	}

	/* Load the UMAC patches if available */
	if (fmac_fw->umac_patch_pri.data && fmac_fw->umac_patch_pri.size &&
	    fmac_fw->umac_patch_sec.data && fmac_fw->umac_patch_sec.size) {
		status = nrf_wifi_hal_fw_patch_load(fmac_dev_ctx->hal_dev_ctx,
						    RPU_PROC_TYPE_MCU_UMAC,
						    fmac_fw->umac_patch_pri.data,
						    fmac_fw->umac_patch_pri.size,
						    fmac_fw->umac_patch_sec.data,
						    fmac_fw->umac_patch_sec.size);

		if (status != NRF_WIFI_STATUS_SUCCESS) {
			nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
					      "%s: UMAC patch load failed\n",
					      __func__);
			goto out;
		} else {
			nrf_wifi_osal_log_dbg(fmac_dev_ctx->fpriv->opriv,
					      "%s: UMAC patches loaded\n",
					      __func__);
		}
	} else {
		wifi_proc[1].is_patch_present = false;
	}

	/* Load the LMAC patches if available */
	if (fmac_fw->lmac_patch_pri.data && fmac_fw->lmac_patch_pri.size &&
	    fmac_fw->lmac_patch_sec.data && fmac_fw->lmac_patch_sec.size) {
		status = nrf_wifi_hal_fw_patch_load(fmac_dev_ctx->hal_dev_ctx,
						    RPU_PROC_TYPE_MCU_LMAC,
						    fmac_fw->lmac_patch_pri.data,
						    fmac_fw->lmac_patch_pri.size,
						    fmac_fw->lmac_patch_sec.data,
						    fmac_fw->lmac_patch_sec.size);

		if (status != NRF_WIFI_STATUS_SUCCESS) {
			nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
					      "%s: LMAC patch load failed\n",
					      __func__);
			goto out;
		} else {
			nrf_wifi_osal_log_dbg(fmac_dev_ctx->fpriv->opriv,
					      "%s: LMAC patches loaded\n",
					      __func__);
		}
	} else {
		wifi_proc[0].is_patch_present = false;
	}

	status = nrf_wifi_fmac_fw_boot(fmac_dev_ctx);
	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: FW boot failed\n",
				      __func__);
		goto out;
	}

	fmac_dev_ctx->fw_boot_done = true;

out:
	return status;
}


struct nrf_wifi_fmac_dev_ctx *nrf_wifi_fmac_dev_add(struct nrf_wifi_fmac_priv *fpriv,
						    void *os_dev_ctx)
{
	struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx = NULL;
#ifndef CONFIG_NRF700X_RADIO_TEST
	struct nrf_wifi_fmac_dev_ctx_def *fmac_dev_priv = NULL;
#else
	struct nrf_wifi_fmac_dev_ctx_rt *fmac_dev_priv = NULL;
#endif
#ifdef CONFIG_NRF700X_DATA_TX
	struct nrf_wifi_fmac_priv_def *def_priv = NULL;
#endif /* CONFIG_NRF700X_DATA_TX */

	if (!fpriv || !os_dev_ctx) {
		return NULL;
	}

	fmac_dev_ctx = nrf_wifi_osal_mem_zalloc(fpriv->opriv,
						sizeof(*fmac_dev_ctx) + sizeof(*fmac_dev_priv));

	if (!fmac_dev_ctx) {
		nrf_wifi_osal_log_err(fpriv->opriv,
				      "%s: Unable to allocate fmac_dev_ctx",
				      __func__);
		goto out;
	}

	fmac_dev_ctx->fpriv = fpriv;
	fmac_dev_ctx->os_dev_ctx = os_dev_ctx;

	fmac_dev_ctx->hal_dev_ctx = nrf_wifi_hal_dev_add(fpriv->hpriv,
							 fmac_dev_ctx);

	if (!fmac_dev_ctx->hal_dev_ctx) {
		nrf_wifi_osal_log_err(fpriv->opriv,
				      "%s: nrf_wifi_hal_dev_add failed",
				      __func__);

		nrf_wifi_osal_mem_free(fpriv->opriv,
				       fmac_dev_ctx);
		fmac_dev_ctx = NULL;
		goto out;
	}
#ifdef CONFIG_NRF700X_DATA_TX

	def_priv = wifi_fmac_priv(fpriv);
	fpriv->hpriv->cfg_params.max_ampdu_len_per_token = def_priv->max_ampdu_len_per_token;
#endif /* CONFIG_NRF700X_DATA_TX */
out:
	return fmac_dev_ctx;
}

enum nrf_wifi_status nrf_wifi_fmac_stats_get(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					     enum rpu_op_mode op_mode,
					     struct rpu_op_stats *stats)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	unsigned char count = 0;
	int stats_type;
#ifndef CONFIG_NRF700X_RADIO_TEST
	struct nrf_wifi_fmac_dev_ctx_def *def_dev_ctx = NULL;
#endif /* CONFIG_NRF700X_RADIO_TEST */

	#ifdef CONFIG_NRF700X_RADIO_TEST
		stats_type = RPU_STATS_TYPE_PHY;
	#else
		stats_type = RPU_STATS_TYPE_ALL;
	#endif /* CONFIG_NRF700X_RADIO_TEST */


	if ((stats_type == RPU_STATS_TYPE_ALL) ||
	    (stats_type == RPU_STATS_TYPE_UMAC) ||
	    (stats_type == RPU_STATS_TYPE_LMAC) ||
	    (stats_type == RPU_STATS_TYPE_PHY)) {
		if (fmac_dev_ctx->stats_req == true) {
			nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
					      "%s: Stats request already pending",
					      __func__);
			goto out;
		}

		fmac_dev_ctx->stats_req = true;
		fmac_dev_ctx->fw_stats = &stats->fw;

		status = umac_cmd_prog_stats_get(fmac_dev_ctx,
		#ifdef CONFIG_NRF700X_RADIO_TEST
						 op_mode,
		#endif /* CONFIG_NRF700X_RADIO_TEST */
						 stats_type);

		if (status != NRF_WIFI_STATUS_SUCCESS) {
			goto out;
		}

		do {
			nrf_wifi_osal_sleep_ms(fmac_dev_ctx->fpriv->opriv,
					       1);
			count++;
		} while ((fmac_dev_ctx->stats_req == true) &&
			 (count < NRF_WIFI_FMAC_STATS_RECV_TIMEOUT));

		if (count == NRF_WIFI_FMAC_STATS_RECV_TIMEOUT) {
			nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
					      "%s: Timed out",
					      __func__);
			goto out;
		}
	}


#ifndef CONFIG_NRF700X_RADIO_TEST
	def_dev_ctx = wifi_dev_priv(fmac_dev_ctx);
	if ((stats_type == RPU_STATS_TYPE_ALL) ||
	    (stats_type == RPU_STATS_TYPE_HOST)) {
		nrf_wifi_osal_mem_cpy(fmac_dev_ctx->fpriv->opriv,
				      &stats->host,
				      &def_dev_ctx->host_stats,
				      sizeof(def_dev_ctx->host_stats));
	}
#endif

	status = NRF_WIFI_STATUS_SUCCESS;
out:
	return status;
}

enum nrf_wifi_status nrf_wifi_fmac_ver_get(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					  unsigned int *fw_ver)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;

	status = hal_rpu_mem_read(fmac_dev_ctx->hal_dev_ctx,
				  fw_ver,
				  RPU_MEM_UMAC_VER,
				  sizeof(*fw_ver));

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: Unable to read UMAC ver",
				      __func__);
		goto out;
	}

out:
	return status;
}

enum nrf_wifi_status nrf_wifi_fmac_conf_ltf_gi(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					       unsigned char he_ltf,
					       unsigned char he_gi,
					       unsigned char enabled)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;

	status = umac_cmd_he_ltf_gi(fmac_dev_ctx, he_ltf, he_gi, enabled);

	return status;
}

enum nrf_wifi_status nrf_wifi_fmac_conf_srcoex(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					       void *cmd, unsigned int cmd_len)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;

	status = umac_cmd_srcoex(fmac_dev_ctx, cmd, cmd_len);

	return status;
}


enum nrf_wifi_status nrf_wifi_fmac_otp_mac_addr_get(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
						    unsigned char vif_idx,
						    unsigned char *mac_addr)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_fmac_otp_info otp_info;
	unsigned char *otp_mac_addr = NULL;
	unsigned int otp_mac_addr_flag_mask = 0;

	if (!fmac_dev_ctx || !mac_addr || (vif_idx >= MAX_NUM_VIFS)) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: Invalid parameters",
				      __func__);
		goto out;
	}

	nrf_wifi_osal_mem_set(fmac_dev_ctx->fpriv->opriv,
			      &otp_info,
			      0xFF,
			      sizeof(otp_info));

	status = nrf_wifi_hal_otp_info_get(fmac_dev_ctx->hal_dev_ctx,
					   &otp_info.info,
					   &otp_info.flags);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: Fetching of RPU OTP information failed",
				      __func__);
		goto out;
	}

	if (vif_idx == 0) {
		otp_mac_addr = (unsigned char *)otp_info.info.mac_address0;
		otp_mac_addr_flag_mask = (~MAC0_ADDR_FLAG_MASK);

	} else if (vif_idx == 1) {
		otp_mac_addr = (unsigned char *)otp_info.info.mac_address1;
		otp_mac_addr_flag_mask = (~MAC1_ADDR_FLAG_MASK);
	}

	/* Check if a valid MAC address has been programmed in the OTP */

	if (otp_info.flags & otp_mac_addr_flag_mask) {
		nrf_wifi_osal_log_info(fmac_dev_ctx->fpriv->opriv,
				       "%s: MAC addr not programmed in OTP",
				       __func__);

	} else {
		nrf_wifi_osal_mem_cpy(fmac_dev_ctx->fpriv->opriv,
				      mac_addr,
				      otp_mac_addr,
				      NRF_WIFI_ETH_ADDR_LEN);

		if (!nrf_wifi_utils_is_mac_addr_valid(fmac_dev_ctx->fpriv->opriv,
						      (const char *)mac_addr)) {
			nrf_wifi_osal_log_info(fmac_dev_ctx->fpriv->opriv,
					       "%s:  Invalid OTP MAC addr: %02X%02X%02X%02X%02X%02X",
					       __func__,
					       (*(mac_addr + 0)),
					       (*(mac_addr + 1)),
					       (*(mac_addr + 2)),
					       (*(mac_addr + 3)),
					       (*(mac_addr + 4)),
					       (*(mac_addr + 5)));

		}
	}
out:
	return status;
}


enum nrf_wifi_status nrf_wifi_fmac_rf_params_get(
		struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
		struct nrf_wifi_phy_rf_params *phy_rf_params)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_fmac_otp_info otp_info;
	unsigned int ft_prog_ver;
	int ret = -1;
	/* If package_info is not written to OTP then the default value will be 0xFF. */
	unsigned int package_info = 0xFFFFFFFF;
	struct nrf_wifi_tx_pwr_ceil_params *tx_pwr_ceil_params;
	unsigned char backoff_2g_dsss = 0, backoff_2g_ofdm = 0;
	unsigned char backoff_5g_lowband = 0, backoff_5g_midband = 0, backoff_5g_highband = 0;

	if (!fmac_dev_ctx || !phy_rf_params) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: Invalid parameters",
				      __func__);
		goto out;
	}

	tx_pwr_ceil_params = fmac_dev_ctx->tx_pwr_ceil_params;

	nrf_wifi_osal_mem_set(fmac_dev_ctx->fpriv->opriv,
			      &otp_info,
			      0xFF,
			      sizeof(otp_info));

	status = nrf_wifi_hal_otp_info_get(fmac_dev_ctx->hal_dev_ctx,
					   &otp_info.info,
					   &otp_info.flags);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: Fetching of RPU OTP information failed",
				      __func__);
		goto out;
	}

	status = nrf_wifi_hal_otp_ft_prog_ver_get(fmac_dev_ctx->hal_dev_ctx,
						  &ft_prog_ver);
	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: Fetching of FT program version failed",
				      __func__);
		goto out;
	}

	status = nrf_wifi_hal_otp_pack_info_get(fmac_dev_ctx->hal_dev_ctx,
						&package_info);
	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: Fetching of Package info failed",
				      __func__);
		goto out;
	}

	ret = nrf_wifi_phy_rf_params_init(fmac_dev_ctx->fpriv->opriv,
				    	  phy_rf_params,
				    	  package_info,
				    	  NRF_WIFI_DEF_RF_PARAMS);

	if (ret == -1) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: Initialization of RF params with default values failed",
				      __func__);
		status = NRF_WIFI_STATUS_FAIL;
		goto out;
	}
	if (!(otp_info.flags & (~CALIB_XO_FLAG_MASK))) {
		nrf_wifi_osal_mem_cpy(fmac_dev_ctx->fpriv->opriv,
				      &phy_rf_params->xo_offset.xo_freq_offset,
				      (char *)otp_info.info.calib + OTP_OFF_CALIB_XO,
				      OTP_SZ_CALIB_XO);

	}

	ft_prog_ver = (ft_prog_ver & FT_PROG_VER_MASK) >> 16;

	if (ft_prog_ver == FT_PROG_VER1) {
		backoff_2g_dsss = FT_PROG_VER1_2G_DSSS_TXCEIL_BKOFF;
		backoff_2g_ofdm = FT_PROG_VER1_2G_OFDM_TXCEIL_BKOFF;
		backoff_5g_lowband = FT_PROG_VER1_5G_LOW_OFDM_TXCEIL_BKOFF;
		backoff_5g_midband = FT_PROG_VER1_5G_MID_OFDM_TXCEIL_BKOFF;
		backoff_5g_highband = FT_PROG_VER1_5G_HIGH_OFDM_TXCEIL_BKOFF;
	} else if (ft_prog_ver == FT_PROG_VER2) {
		backoff_2g_dsss = FT_PROG_VER2_2G_DSSS_TXCEIL_BKOFF;
		backoff_2g_ofdm = FT_PROG_VER2_2G_OFDM_TXCEIL_BKOFF;
		backoff_5g_lowband = FT_PROG_VER2_5G_LOW_OFDM_TXCEIL_BKOFF;
		backoff_5g_midband = FT_PROG_VER2_5G_MID_OFDM_TXCEIL_BKOFF;
		backoff_5g_highband = FT_PROG_VER2_5G_HIGH_OFDM_TXCEIL_BKOFF;
	} else if (ft_prog_ver == FT_PROG_VER3) {
		backoff_2g_dsss = FT_PROG_VER3_2G_DSSS_TXCEIL_BKOFF;
		backoff_2g_ofdm = FT_PROG_VER3_2G_OFDM_TXCEIL_BKOFF;
		backoff_5g_lowband = FT_PROG_VER3_5G_LOW_OFDM_TXCEIL_BKOFF;
		backoff_5g_midband = FT_PROG_VER3_5G_MID_OFDM_TXCEIL_BKOFF;
		backoff_5g_highband = FT_PROG_VER3_5G_HIGH_OFDM_TXCEIL_BKOFF;
	}
	phy_rf_params->max_pwr_ceil.max_dsss_pwr =
	MIN(tx_pwr_ceil_params->max_pwr_2g_dsss, phy_rf_params->max_pwr_ceil.max_dsss_pwr)
       	- backoff_2g_dsss;
	phy_rf_params->max_pwr_ceil.max_lb_mcs7_pwr =
	MIN(tx_pwr_ceil_params->max_pwr_2g_mcs7, phy_rf_params->max_pwr_ceil.max_lb_mcs7_pwr)
       	- backoff_2g_ofdm;
	phy_rf_params->max_pwr_ceil.max_lb_mcs0_pwr =
	MIN(tx_pwr_ceil_params->max_pwr_2g_mcs0, phy_rf_params->max_pwr_ceil.max_lb_mcs0_pwr)
       	- backoff_2g_ofdm;
#ifndef CONFIG_NRF70_2_4G_ONLY
	phy_rf_params->max_pwr_ceil.max_hb_low_chan_mcs7_pwr =
	MIN(tx_pwr_ceil_params->max_pwr_5g_low_mcs7, 
		phy_rf_params->max_pwr_ceil.max_hb_low_chan_mcs7_pwr) - backoff_5g_lowband;
	phy_rf_params->max_pwr_ceil.max_hb_mid_chan_mcs7_pwr =
	MIN(tx_pwr_ceil_params->max_pwr_5g_mid_mcs7,
		phy_rf_params->max_pwr_ceil.max_hb_mid_chan_mcs7_pwr) - backoff_5g_midband;
	phy_rf_params->max_pwr_ceil.max_hb_high_chan_mcs7_pwr =
	MIN(tx_pwr_ceil_params->max_pwr_5g_high_mcs7,
		phy_rf_params->max_pwr_ceil.max_hb_high_chan_mcs7_pwr) - backoff_5g_highband;
	phy_rf_params->max_pwr_ceil.max_hb_low_chan_mcs0_pwr =
	MIN(tx_pwr_ceil_params->max_pwr_5g_low_mcs0,
		phy_rf_params->max_pwr_ceil.max_hb_low_chan_mcs0_pwr) - backoff_5g_lowband;
	phy_rf_params->max_pwr_ceil.max_hb_mid_chan_mcs0_pwr =
	MIN(tx_pwr_ceil_params->max_pwr_5g_mid_mcs0,
	        phy_rf_params->max_pwr_ceil.max_hb_mid_chan_mcs0_pwr) - backoff_5g_midband;
	phy_rf_params->max_pwr_ceil.max_hb_high_chan_mcs0_pwr =
	MIN(tx_pwr_ceil_params->max_pwr_5g_high_mcs0,
	        phy_rf_params->max_pwr_ceil.max_hb_high_chan_mcs0_pwr) - backoff_5g_highband;
#endif /* CONFIG_NRF70_2_4G_ONLY */

	status = NRF_WIFI_STATUS_SUCCESS;
out:
	return status;
}


enum nrf_wifi_status nrf_wifi_fmac_set_reg(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					   struct nrf_wifi_fmac_reg_info *reg_info)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_cmd_req_set_reg *set_reg_cmd = NULL;
	unsigned int count = 0, max_count = NRF_WIFI_FMAC_REG_SET_TIMEOUT_MS / 20;
	enum nrf_wifi_reg_initiator exp_initiator = NRF_WIFI_REGDOM_SET_BY_USER;
	enum nrf_wifi_reg_type exp_reg_type = NRF_WIFI_REGDOM_TYPE_COUNTRY;
	char exp_alpha2[NRF_WIFI_COUNTRY_CODE_LEN] = {0};
	struct nrf_wifi_fmac_reg_info cur_reg_info = {0};
	struct nrf_wifi_event_regulatory_change *reg_change = NULL;

	if (!fmac_dev_ctx || !reg_info) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: Invalid parameters",
				      __func__);
		goto out;
	}

	/* No change event from UMAC for same regd */
	status = nrf_wifi_fmac_get_reg(fmac_dev_ctx, &cur_reg_info);
	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: Failed to get current regulatory information",
				      __func__);
		goto out;
	}

	if (nrf_wifi_osal_mem_cmp(fmac_dev_ctx->fpriv->opriv,
				  cur_reg_info.alpha2,
				  reg_info->alpha2,
				  NRF_WIFI_COUNTRY_CODE_LEN) == 0) {
		nrf_wifi_osal_log_dbg(fmac_dev_ctx->fpriv->opriv,
				       "%s: Regulatory domain already set to %c%c",
				       __func__,
				       reg_info->alpha2[0],
				       reg_info->alpha2[1]);
		status = NRF_WIFI_STATUS_SUCCESS;
		goto out;
	}

	set_reg_cmd = nrf_wifi_osal_mem_zalloc(fmac_dev_ctx->fpriv->opriv,
					       sizeof(*set_reg_cmd));

	if (!set_reg_cmd) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: Unable to allocate memory",
				      __func__);
		goto out;
	}

	set_reg_cmd->umac_hdr.cmd_evnt = NRF_WIFI_UMAC_CMD_REQ_SET_REG;
	set_reg_cmd->umac_hdr.ids.valid_fields = 0;

	nrf_wifi_osal_mem_cpy(fmac_dev_ctx->fpriv->opriv,
			      set_reg_cmd->nrf_wifi_alpha2,
			      reg_info->alpha2,
			      NRF_WIFI_COUNTRY_CODE_LEN);

	exp_alpha2[0] = reg_info->alpha2[0];
	exp_alpha2[1] = reg_info->alpha2[1];

	if (reg_info->alpha2[0] == '0' && reg_info->alpha2[1] == '0') {
		exp_reg_type = NRF_WIFI_REGDOM_TYPE_WORLD;
	}

	set_reg_cmd->valid_fields = NRF_WIFI_CMD_REQ_SET_REG_ALPHA2_VALID;

	/* New feature in rev B patch */
	if (reg_info->force) {
		set_reg_cmd->valid_fields |= NRF_WIFI_CMD_REQ_SET_REG_USER_REG_FORCE;
	}

	status = umac_cmd_cfg(fmac_dev_ctx,
			      set_reg_cmd,
			      sizeof(*set_reg_cmd));
	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: Failed to set regulatory information",
				      __func__);
		goto out;
	}

	fmac_dev_ctx->reg_set_status = false;
	while (!fmac_dev_ctx->reg_set_status && count++ <= max_count) {
		nrf_wifi_osal_sleep_ms(fmac_dev_ctx->fpriv->opriv,
				       100);
	}

	if (!fmac_dev_ctx->reg_set_status) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: Failed to set regulatory information",
				      __func__);
		status = NRF_WIFI_STATUS_FAIL;
		goto out;
	}

	reg_change = fmac_dev_ctx->reg_change;

	if (reg_change->intr != exp_initiator) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: Regulatory domain change not initiated by user: exp: %d, got: %d",
				      __func__,
					  exp_initiator,
					  reg_change->intr);
		status = NRF_WIFI_STATUS_FAIL;
		goto out;
	}

	if (reg_change->regulatory_type != exp_reg_type) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: Regulatory domain change not to expected type: exp: %d, got: %d",
				      __func__,
					  exp_reg_type,
					  reg_change->regulatory_type);
		status = NRF_WIFI_STATUS_FAIL;
		goto out;
	}

	if ((reg_change->regulatory_type == NRF_WIFI_REGDOM_TYPE_COUNTRY) &&
		 nrf_wifi_osal_mem_cmp(fmac_dev_ctx->fpriv->opriv,
				  reg_change->nrf_wifi_alpha2,
				  exp_alpha2,
				  NRF_WIFI_COUNTRY_CODE_LEN) != 0) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: Regulatory domain change not to expected alpha2: exp: %c%c, got: %c%c",
				      __func__,
					  exp_alpha2[0],
					  exp_alpha2[1],
					  reg_change->nrf_wifi_alpha2[0],
					  reg_change->nrf_wifi_alpha2[1]);
		status = NRF_WIFI_STATUS_FAIL;
		goto out;
	}

out:
	if (set_reg_cmd) {
		nrf_wifi_osal_mem_free(fmac_dev_ctx->fpriv->opriv,
				       set_reg_cmd);
	}

	if (reg_change) {
		nrf_wifi_osal_mem_free(fmac_dev_ctx->fpriv->opriv,
				       reg_change);
		fmac_dev_ctx->reg_change = NULL;
	}

	return status;
}

enum nrf_wifi_status nrf_wifi_fmac_get_reg(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					   struct nrf_wifi_fmac_reg_info *reg_info)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_umac_cmd_get_reg *get_reg_cmd = NULL;
	unsigned int count = 0;

	if (!fmac_dev_ctx || !reg_info) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: Invalid parameters",
				      __func__);
		goto err;
	}

	get_reg_cmd = nrf_wifi_osal_mem_zalloc(fmac_dev_ctx->fpriv->opriv,
					       sizeof(*get_reg_cmd));

	if (!get_reg_cmd) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: Unable to allocate memory",
				      __func__);
		goto err;
	}

	get_reg_cmd->umac_hdr.cmd_evnt = NRF_WIFI_UMAC_CMD_GET_REG;
	get_reg_cmd->umac_hdr.ids.valid_fields = 0;

	fmac_dev_ctx->alpha2_valid = false;
	fmac_dev_ctx->reg_chan_info = reg_info->reg_chan_info;

	status = umac_cmd_cfg(fmac_dev_ctx,
			      get_reg_cmd,
			      sizeof(*get_reg_cmd));

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: Failed to get regulatory information",	__func__);
		goto err;
	}

	do {
		nrf_wifi_osal_sleep_ms(fmac_dev_ctx->fpriv->opriv,
				       100);
	} while (count++ < 100 && !fmac_dev_ctx->alpha2_valid);

	if (!fmac_dev_ctx->alpha2_valid) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: Failed to get regulatory information",
				      __func__);
		goto err;
	}

	nrf_wifi_osal_mem_cpy(fmac_dev_ctx->fpriv->opriv,
		   reg_info->alpha2,
	       fmac_dev_ctx->alpha2,
	       sizeof(reg_info->alpha2));

	reg_info->reg_chan_count = fmac_dev_ctx->reg_chan_count;

	return NRF_WIFI_STATUS_SUCCESS;
err:
	return NRF_WIFI_STATUS_FAIL;
}

int nrf_wifi_phy_rf_params_init(struct nrf_wifi_osal_priv *opriv,
				struct nrf_wifi_phy_rf_params *prf,
				unsigned int package_info,
				unsigned char *str)
{
	int ret = -1;
	unsigned int rf_param_offset = BAND_2G_LW_ED_BKF_DSSS_OFST - NRF_WIFI_RF_PARAMS_CONF_SIZE;
	/* Initilaize reserved bytes */
	nrf_wifi_osal_mem_set(opriv,
			      prf,
			      0x0,
			      sizeof(prf));
	/* Initialize PD adjust values for MCS7. Currently these 4 bytes are not being used */
	prf->pd_adjust_val.pd_adjt_lb_chan = PD_ADJUST_VAL;
	prf->pd_adjust_val.pd_adjt_hb_low_chan = PD_ADJUST_VAL;
	prf->pd_adjust_val.pd_adjt_hb_mid_chan = PD_ADJUST_VAL;
	prf->pd_adjust_val.pd_adjt_hb_high_chan = PD_ADJUST_VAL;

	/* Configure systematic offset value */
	prf->syst_tx_pwr_offset.syst_off_lb_chan = SYSTEM_OFFSET_LB;
	prf->syst_tx_pwr_offset.syst_off_hb_low_chan = SYSTEM_OFFSET_HB_CHAN_LOW;
	prf->syst_tx_pwr_offset.syst_off_hb_mid_chan = SYSTEM_OFFSET_HB_CHAN_MID;
	prf->syst_tx_pwr_offset.syst_off_hb_high_chan = SYSTEM_OFFSET_HB_CHAN_HIGH;

	/* RX Gain offsets */
	prf->rx_gain_offset.rx_gain_lb_chan = RX_GAIN_OFFSET_LB_CHAN;
	prf->rx_gain_offset.rx_gain_hb_low_chan = RX_GAIN_OFFSET_HB_LOW_CHAN;
	prf->rx_gain_offset.rx_gain_hb_mid_chan = RX_GAIN_OFFSET_HB_MID_CHAN;
	prf->rx_gain_offset.rx_gain_hb_high_chan = RX_GAIN_OFFSET_HB_HIGH_CHAN;

	if (package_info == CSP_PACKAGE_INFO) {
		prf->xo_offset.xo_freq_offset = CSP_XO_VAL;
		/* TX power ceiling */
		prf->max_pwr_ceil.max_dsss_pwr = CSP_MAX_TX_PWR_DSSS;
		prf->max_pwr_ceil.max_lb_mcs7_pwr = CSP_MAX_TX_PWR_LB_MCS7;
		prf->max_pwr_ceil.max_lb_mcs0_pwr = CSP_MAX_TX_PWR_LB_MCS0;
		prf->max_pwr_ceil.max_hb_low_chan_mcs7_pwr = CSP_MAX_TX_PWR_HB_LOW_CHAN_MCS7;
		prf->max_pwr_ceil.max_hb_mid_chan_mcs7_pwr = CSP_MAX_TX_PWR_HB_MID_CHAN_MCS7;
		prf->max_pwr_ceil.max_hb_high_chan_mcs7_pwr = CSP_MAX_TX_PWR_HB_HIGH_CHAN_MCS7;
		prf->max_pwr_ceil.max_hb_low_chan_mcs0_pwr = CSP_MAX_TX_PWR_HB_LOW_CHAN_MCS0;
		prf->max_pwr_ceil.max_hb_mid_chan_mcs0_pwr = CSP_MAX_TX_PWR_HB_MID_CHAN_MCS0;
		prf->max_pwr_ceil.max_hb_high_chan_mcs0_pwr = CSP_MAX_TX_PWR_HB_HIGH_CHAN_MCS0;

#ifndef CONFIG_NRF700X_RADIO_TEST
		/* Max-Min chip temperature, VT backoffs configuration */
		prf->temp_volt_backoff.max_chip_temp = CSP_MAX_CHIP_TEMP;
		prf->temp_volt_backoff.min_chip_temp = CSP_MIN_CHIP_TEMP;
		prf->temp_volt_backoff.lb_max_pwr_bkf_hi_temp =	CSP_LB_MAX_PWR_BKF_HI_TEMP;
		prf->temp_volt_backoff.lb_max_pwr_bkf_low_temp = CSP_LB_MAX_PWR_BKF_LOW_TEMP;
		prf->temp_volt_backoff.hb_max_pwr_bkf_hi_temp =	CSP_HB_MAX_PWR_BKF_HI_TEMP;
		prf->temp_volt_backoff.hb_max_pwr_bkf_low_temp = CSP_HB_MAX_PWR_BKF_LOW_TEMP;
		prf->temp_volt_backoff.lb_vbt_lt_vlow = CSP_LB_VBT_LT_VLOW;
		prf->temp_volt_backoff.hb_vbt_lt_vlow =	CSP_HB_VBT_LT_VLOW;
		prf->temp_volt_backoff.lb_vbt_lt_low = CSP_LB_VBT_LT_LOW;
		prf->temp_volt_backoff.hb_vbt_lt_low = CSP_HB_VBT_LT_LOW;
#endif /* CONFIG_NRF700X_RADIO_TEST */
	}
	else {
		/** If nothing is written to OTP field corresponding to package info byte
		 * or if the package info field is corrupted then the default package
		 * package is QFN.
		 */

		/* Initialize XO */
		prf->xo_offset.xo_freq_offset = QFN_XO_VAL;

		/* TX power ceiling */
		prf->max_pwr_ceil.max_dsss_pwr = QFN_MAX_TX_PWR_DSSS;
		prf->max_pwr_ceil.max_lb_mcs7_pwr = QFN_MAX_TX_PWR_LB_MCS7;
		prf->max_pwr_ceil.max_lb_mcs0_pwr = QFN_MAX_TX_PWR_LB_MCS0;
		prf->max_pwr_ceil.max_hb_low_chan_mcs7_pwr = QFN_MAX_TX_PWR_HB_LOW_CHAN_MCS7;
		prf->max_pwr_ceil.max_hb_mid_chan_mcs7_pwr = QFN_MAX_TX_PWR_HB_MID_CHAN_MCS7;
		prf->max_pwr_ceil.max_hb_high_chan_mcs7_pwr = QFN_MAX_TX_PWR_HB_HIGH_CHAN_MCS7;
		prf->max_pwr_ceil.max_hb_low_chan_mcs0_pwr = QFN_MAX_TX_PWR_HB_LOW_CHAN_MCS0;
		prf->max_pwr_ceil.max_hb_mid_chan_mcs0_pwr = QFN_MAX_TX_PWR_HB_MID_CHAN_MCS0;
		prf->max_pwr_ceil.max_hb_high_chan_mcs0_pwr = QFN_MAX_TX_PWR_HB_HIGH_CHAN_MCS0;

#ifndef CONFIG_NRF700X_RADIO_TEST
		/* Max-Min chip temperature, VT backoffs configuration */
		prf->temp_volt_backoff.max_chip_temp = QFN_MAX_CHIP_TEMP;
		prf->temp_volt_backoff.min_chip_temp = QFN_MIN_CHIP_TEMP;
		prf->temp_volt_backoff.lb_max_pwr_bkf_hi_temp =	QFN_LB_MAX_PWR_BKF_HI_TEMP;
		prf->temp_volt_backoff.lb_max_pwr_bkf_low_temp = QFN_LB_MAX_PWR_BKF_LOW_TEMP;
		prf->temp_volt_backoff.hb_max_pwr_bkf_hi_temp =	QFN_HB_MAX_PWR_BKF_HI_TEMP;
		prf->temp_volt_backoff.hb_max_pwr_bkf_low_temp = QFN_HB_MAX_PWR_BKF_LOW_TEMP;
		prf->temp_volt_backoff.lb_vbt_lt_vlow =	QFN_LB_VBT_LT_VLOW;
		prf->temp_volt_backoff.hb_vbt_lt_vlow =	QFN_HB_VBT_LT_VLOW;
		prf->temp_volt_backoff.lb_vbt_lt_low = QFN_LB_VBT_LT_LOW;
		prf->temp_volt_backoff.hb_vbt_lt_low = QFN_HB_VBT_LT_LOW;
#endif /* CONFIG_NRF700X_RADIO_TEST */
	}

	ret = nrf_wifi_utils_hex_str_to_val(opriv,
					(unsigned char *)&prf->phy_params,
					sizeof(prf->phy_params),
					str);

	prf->phy_params[rf_param_offset]  = CONFIG_NRF700X_BAND_2G_LOWER_EDGE_BACKOFF_DSSS;
	prf->phy_params[rf_param_offset + 1]  = CONFIG_NRF700X_BAND_2G_LOWER_EDGE_BACKOFF_HT;
	prf->phy_params[rf_param_offset + 2]  = CONFIG_NRF700X_BAND_2G_LOWER_EDGE_BACKOFF_HE;
	prf->phy_params[rf_param_offset + 3]  = CONFIG_NRF700X_BAND_2G_UPPER_EDGE_BACKOFF_DSSS;
	prf->phy_params[rf_param_offset + 4]  = CONFIG_NRF700X_BAND_2G_UPPER_EDGE_BACKOFF_HT;
	prf->phy_params[rf_param_offset + 5]  = CONFIG_NRF700X_BAND_2G_UPPER_EDGE_BACKOFF_HE;
	prf->phy_params[rf_param_offset + 6]  = CONFIG_NRF700X_BAND_UNII_1_LOWER_EDGE_BACKOFF_HT;
	prf->phy_params[rf_param_offset + 7]  = CONFIG_NRF700X_BAND_UNII_1_LOWER_EDGE_BACKOFF_HE;
	prf->phy_params[rf_param_offset + 8]  = CONFIG_NRF700X_BAND_UNII_1_UPPER_EDGE_BACKOFF_HT;
	prf->phy_params[rf_param_offset + 9]  = CONFIG_NRF700X_BAND_UNII_1_UPPER_EDGE_BACKOFF_HE;
	prf->phy_params[rf_param_offset + 10]  = CONFIG_NRF700X_BAND_UNII_2A_LOWER_EDGE_BACKOFF_HT;
	prf->phy_params[rf_param_offset + 11]  = CONFIG_NRF700X_BAND_UNII_2A_LOWER_EDGE_BACKOFF_HE;
	prf->phy_params[rf_param_offset + 12]  = CONFIG_NRF700X_BAND_UNII_2A_UPPER_EDGE_BACKOFF_HT;
	prf->phy_params[rf_param_offset + 13]  = CONFIG_NRF700X_BAND_UNII_2A_UPPER_EDGE_BACKOFF_HE;
	prf->phy_params[rf_param_offset + 14]  = CONFIG_NRF700X_BAND_UNII_2C_LOWER_EDGE_BACKOFF_HT;
	prf->phy_params[rf_param_offset + 15]  = CONFIG_NRF700X_BAND_UNII_2C_LOWER_EDGE_BACKOFF_HE;
	prf->phy_params[rf_param_offset + 16]  = CONFIG_NRF700X_BAND_UNII_2C_UPPER_EDGE_BACKOFF_HT;
	prf->phy_params[rf_param_offset + 17]  = CONFIG_NRF700X_BAND_UNII_2C_UPPER_EDGE_BACKOFF_HE;
	prf->phy_params[rf_param_offset + 18]  = CONFIG_NRF700X_BAND_UNII_3_LOWER_EDGE_BACKOFF_HT;
	prf->phy_params[rf_param_offset + 19]  = CONFIG_NRF700X_BAND_UNII_3_LOWER_EDGE_BACKOFF_HE;
	prf->phy_params[rf_param_offset + 20]  = CONFIG_NRF700X_BAND_UNII_3_UPPER_EDGE_BACKOFF_HT;
	prf->phy_params[rf_param_offset + 21]  = CONFIG_NRF700X_BAND_UNII_3_UPPER_EDGE_BACKOFF_HE;
	prf->phy_params[rf_param_offset + 22]  = CONFIG_NRF700X_BAND_UNII_4_LOWER_EDGE_BACKOFF_HT;
	prf->phy_params[rf_param_offset + 23]  = CONFIG_NRF700X_BAND_UNII_4_LOWER_EDGE_BACKOFF_HE;
	prf->phy_params[rf_param_offset + 24]  = CONFIG_NRF700X_BAND_UNII_4_UPPER_EDGE_BACKOFF_HT;
	prf->phy_params[rf_param_offset + 25]  = CONFIG_NRF700X_BAND_UNII_4_UPPER_EDGE_BACKOFF_HE;
	prf->phy_params[rf_param_offset + 26]  = CONFIG_NRF700X_ANT_GAIN_2G;
	prf->phy_params[rf_param_offset + 27]  = CONFIG_NRF700X_ANT_GAIN_5G_BAND1;
	prf->phy_params[rf_param_offset + 28]  = CONFIG_NRF700X_ANT_GAIN_5G_BAND2;
	prf->phy_params[rf_param_offset + 29]  = CONFIG_NRF700X_ANT_GAIN_5G_BAND3;
	prf->phy_params[rf_param_offset + 30]  = CONFIG_NRF700X_PCB_LOSS_2G;
	prf->phy_params[rf_param_offset + 31]  = CONFIG_NRF700X_PCB_LOSS_5G_BAND1;
	prf->phy_params[rf_param_offset + 32]  = CONFIG_NRF700X_PCB_LOSS_5G_BAND2;
	prf->phy_params[rf_param_offset + 33]  = CONFIG_NRF700X_PCB_LOSS_5G_BAND3;

	return(ret);
}

#ifdef CONFIG_NRF700X_UTIL
enum nrf_wifi_status nrf_wifi_fmac_set_tx_rate(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					       unsigned char rate_flag,
					       int data_rate)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct host_rpu_msg *umac_cmd = NULL;
	struct nrf_wifi_cmd_fix_tx_rate *umac_cmd_data = NULL;
	int len = 0;

	len = sizeof(*umac_cmd_data);

	umac_cmd = umac_cmd_alloc(fmac_dev_ctx,
				  NRF_WIFI_HOST_RPU_MSG_TYPE_SYSTEM,
				  len);

	if (!umac_cmd) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: umac_cmd_alloc failed",
				      __func__);
		goto out;
	}

	umac_cmd_data = (struct nrf_wifi_cmd_fix_tx_rate *)(umac_cmd->msg);

	umac_cmd_data->sys_head.cmd_event = NRF_WIFI_CMD_TX_FIX_DATA_RATE;
	umac_cmd_data->sys_head.len = len;

	umac_cmd_data->rate_flags = rate_flag;
	umac_cmd_data->fixed_rate = data_rate;

	status = nrf_wifi_hal_ctrl_cmd_send(fmac_dev_ctx->hal_dev_ctx,
					    umac_cmd,
					    (sizeof(*umac_cmd) + len));
out:
	return status;
}

#ifdef CONFIG_NRF_WIFI_LOW_POWER
enum nrf_wifi_status nrf_wifi_fmac_get_host_rpu_ps_ctrl_state(void *dev_ctx,
							      int *rpu_ps_ctrl_state)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx = NULL;

	fmac_dev_ctx = dev_ctx;

	if (!fmac_dev_ctx || !rpu_ps_ctrl_state) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: Invalid parameters",
				      __func__);
		goto out;
	}


	status = nrf_wifi_hal_get_rpu_ps_state(fmac_dev_ctx->hal_dev_ctx,
					       rpu_ps_ctrl_state);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: Fetching of RPU PS state failed",
				      __func__);
		goto out;
	}
out:
	return status;
}
#endif /* CONFIG_NRF_WIFI_LOW_POWER */
#endif /* CONFIG_NRF700X_UTIL */

#ifdef CONFIG_NRF700X_SYSTEM_WITH_RAW_MODES
enum nrf_wifi_status nrf_wifi_fmac_set_mode(void *dev_ctx,
					    unsigned char if_idx,
					    unsigned char mode)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_cmd_raw_config_mode *umac_cmd_data = NULL;
	struct host_rpu_msg *umac_cmd = NULL;
	struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx = dev_ctx;
	int len = 0;

	if (!fmac_dev_ctx) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: Invalid parameters",
				      __func__);
		goto out;
	}

	len = sizeof(*umac_cmd_data);
	umac_cmd = umac_cmd_alloc(fmac_dev_ctx,
				  NRF_WIFI_HOST_RPU_MSG_TYPE_SYSTEM,
				  len);

	if (!umac_cmd) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: umac_cmd_alloc failed",
				      __func__);
		goto out;
	}

	umac_cmd_data = (struct nrf_wifi_cmd_raw_config_mode *)(umac_cmd->msg);
	umac_cmd_data->sys_head.cmd_event = NRF_WIFI_CMD_RAW_CONFIG_MODE;
	umac_cmd_data->sys_head.len = len;
	umac_cmd_data->if_index = if_idx;
	umac_cmd_data->op_mode = mode;

	status = nrf_wifi_hal_ctrl_cmd_send(fmac_dev_ctx->hal_dev_ctx,
					    umac_cmd,
					    (sizeof(*umac_cmd) + len));
out:
	return status;
}
#endif

#if defined(CONFIG_NRF700X_RAW_DATA_TX) || defined(CONFIG_NRF700X_RAW_DATA_RX)
enum nrf_wifi_status nrf_wifi_fmac_set_channel(void *dev_ctx,
					       unsigned char if_idx,
					       unsigned int channel)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_cmd_set_channel *umac_cmd_data = NULL;
	struct host_rpu_msg *umac_cmd = NULL;
	struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx = dev_ctx;
	int len = 0;

	if (!fmac_dev_ctx) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: Invalid parameters",
				      __func__);
		goto out;
	}

	len = sizeof(*umac_cmd_data);
	umac_cmd = umac_cmd_alloc(fmac_dev_ctx,
				  NRF_WIFI_HOST_RPU_MSG_TYPE_SYSTEM,
				  len);

	if (!umac_cmd) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: umac_cmd_alloc failed",
				      __func__);
		goto out;
	}

	umac_cmd_data = (struct nrf_wifi_cmd_set_channel *)(umac_cmd->msg);
	umac_cmd_data->sys_head.cmd_event = NRF_WIFI_CMD_CHANNEL;
	umac_cmd_data->sys_head.len = len;
	umac_cmd_data->if_index = if_idx;
	umac_cmd_data->chan.primary_num = channel;

	status = nrf_wifi_hal_ctrl_cmd_send(fmac_dev_ctx->hal_dev_ctx,
					    umac_cmd,
					    (sizeof(*umac_cmd) + len));
out:
	return status;
}
#endif /* CONFIG_NRF700X_RAW_DATA_TX || CONFIG_NRF700X_RAW_DATA_RX */

#if defined(CONFIG_NRF700X_RAW_DATA_RX) || defined(CONFIG_NRF700X_PROMISC_DATA_RX)
enum nrf_wifi_status nrf_wifi_fmac_set_packet_filter(void *dev_ctx, unsigned char filter,
						     unsigned char if_idx,
						     unsigned short buffer_size)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_cmd_raw_config_filter *umac_cmd_data = NULL;
	struct host_rpu_msg *umac_cmd = NULL;
	struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx = dev_ctx;
	int len = 0;

	if (!fmac_dev_ctx) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: Invalid parameters\n",
				      __func__);
		goto out;
	}

	len = sizeof(*umac_cmd_data);
	umac_cmd = umac_cmd_alloc(fmac_dev_ctx,
				  NRF_WIFI_HOST_RPU_MSG_TYPE_SYSTEM,
				  len);
	if (!umac_cmd) {
		nrf_wifi_osal_log_err(fmac_dev_ctx->fpriv->opriv,
				      "%s: umac_cmd_alloc failed\n",
				      __func__);
		goto out;
	}

	umac_cmd_data = (struct nrf_wifi_cmd_raw_config_filter *)(umac_cmd->msg);
	umac_cmd_data->sys_head.cmd_event = NRF_WIFI_CMD_RAW_CONFIG_FILTER;
	umac_cmd_data->sys_head.len = len;
	umac_cmd_data->if_index = if_idx;
	umac_cmd_data->filter = filter;
	umac_cmd_data->capture_len = buffer_size;

	status = nrf_wifi_hal_ctrl_cmd_send(fmac_dev_ctx->hal_dev_ctx,
					    umac_cmd,
					    (sizeof(*umac_cmd) + len));
out:
	return status;
}
#endif /* CONFIG_NRF700X_RAW_DATA_RX || CONFIG_NRF700X_PROMISC_DATA_RX */
