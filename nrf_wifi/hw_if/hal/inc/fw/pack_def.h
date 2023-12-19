/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @brief Toolchain agnostic packing definitions
 */

#ifndef __PACK_DEF__
#define __PACK_DEF__

#ifdef __ZEPHYR__
#include <zephyr/toolchain.h>
#elif __KERNEL__
#include <linux/compiler_attributes.h>
#endif

#define __NRF_WIFI_PKD __packed
#define __NRF_WIFI_ALIGN_4 __aligned(4)

#endif
