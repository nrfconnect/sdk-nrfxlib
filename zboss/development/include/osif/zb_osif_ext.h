/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */
/* PURPOSE: ZBOSS OSIF extensions/adaption layer for nRF Connect SDK
*/

#ifndef ZB_OSIF_EXT_H
#define ZB_OSIF_EXT_H 1

#include "zboss_api.h"

/** Increase ZBOSS timer value. */
void zb_osif_zboss_timer_tick(void);

/** Filter out NVRAM datasets, handled by NCP host. */
void zb_osif_ncp_set_nvram_filter(void);

#endif /* ZB_OSIF_EXT_H */
