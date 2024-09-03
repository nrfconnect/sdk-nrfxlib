/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file Module that defines the peripherals used by the nRF 802.15.4 SL.
 *
 */

#ifndef NRF_802154_SL_PERIPHS_H__
#define NRF_802154_SL_PERIPHS_H__

#include <nrfx.h>

#if NRF_802154_USE_INTERNAL_INCLUDES
#include "nrf_802154_sl_periphs_internal.h"
#endif

#define NRF_802154_SL_PPI_CHANNELS_USED_MASK 0U
#define NRF_802154_SL_PPI_GROUPS_USED_MASK   0U
#define NRF_802154_SL_EGU_USED_CHANNELS_MASK 0U

#endif // NRF_802154_SL_PERIPHS_H__
