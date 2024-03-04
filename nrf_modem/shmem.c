/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */
#include <nrf_modem.h>

#if CONFIG_NRF_MODEM_LINK_BINARY_CELLULAR
#if (CONFIG_NRF_MODEM_SHMEM_CTRL_SIZE != NRF_MODEM_CELLULAR_SHMEM_CTRL_SIZE)
#error The control region size for Cellular variant must be updated
#endif
#elif CONFIG_NRF_MODEM_LINK_BINARY_DECT_PHY
#if (CONFIG_NRF_MODEM_SHMEM_CTRL_SIZE != NRF_MODEM_DECT_PHY_SHMEM_CTRL_SIZE)
#error The control region size for DECT PHY variant must be updated
#endif
#endif
