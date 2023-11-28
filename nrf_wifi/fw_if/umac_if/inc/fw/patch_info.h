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

/* 2 - LMAC and 2 - UMAC */
#define NRF_WIFI_PATCH_NUM_IMAGES (2 + 2)

enum nrf70_feature_flags {
	NRF70_FEAT_SYSTEM_MODE = BIT(0),
	NRF70_FEAT_RADIO_TEST = BIT(1),
	NRF70_FEAT_SCAN_ONLY = BIT(2),
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
	unsigned int num_images;
	unsigned int version;
	unsigned int feature_flags;
} __NRF_WIFI_PKD;


#define RPU_FAMILY         (1)
#define RPU_MAJOR_VERSION   (2)
#define RPU_MINOR_VERSION   (9)
#define RPU_PATCH_VERSION   (5)

#endif /* _PATCH_INFO_H */
