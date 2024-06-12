/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @brief nRF70 patch version.
 */
#ifndef _PATCH_INFO_H
#define _PATCH_INFO_H

#include "pack_def.h"

#ifndef BIT
#define BIT(x) (1 << (x))
#endif /* BIT */

#define NRF_WIFI_PATCH_SIGNATURE 0xDEAD1EAF
/* SHA256 hash length */
#define NRF_WIFI_PATCH_HASH_LEN 32
/* 2 - LMAC and 2 - UMAC */
#define NRF_WIFI_PATCH_NUM_IMAGES (2 + 2)

enum nrf70_feature_flags {
	NRF70_FEAT_SYSTEM_MODE = BIT(0),
	NRF70_FEAT_RADIO_TEST = BIT(1),
	NRF70_FEAT_SCAN_ONLY = BIT(2),
	NRF70_FEAT_SYSTEM_WITH_RAW_MODES = BIT(3),
};

enum nrf70_image_ids {
	NRF70_IMAGE_UMAC_PRI = 0,
	NRF70_IMAGE_UMAC_SEC,
	NRF70_IMAGE_LMAC_PRI,
	NRF70_IMAGE_LMAC_SEC,
};

struct nrf70_fw_image {
	unsigned int type;
	unsigned int len;
	/* Data follows */
	unsigned char data[];
} __NRF_WIFI_PKD;

struct nrf70_fw_image_info {
	unsigned int signature;
	unsigned int num_images;
	unsigned int version;
	unsigned int feature_flags;
	unsigned int len;
	/* Protects against image corruption */
	unsigned char hash[NRF_WIFI_PATCH_HASH_LEN];
	unsigned char data[];
} __NRF_WIFI_PKD;


#define RPU_FAMILY         (1)
#define RPU_MAJOR_VERSION   (2)
#define RPU_MINOR_VERSION   (11)
#define RPU_PATCH_VERSION   (0)

#endif /* _PATCH_INFO_H */
