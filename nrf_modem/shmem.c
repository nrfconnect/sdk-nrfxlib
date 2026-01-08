/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */
#include <nrf_modem.h>

#if defined(CONFIG_NRF_MODEM_LINK_BINARY_CELLULAR)
#if (CONFIG_NRF_MODEM_SHMEM_CTRL_SIZE != NRF_MODEM_CELLULAR_SHMEM_CTRL_SIZE)
#error The control region size for Cellular variant must be updated
#endif
#elif defined(CONFIG_NRF_MODEM_LINK_BINARY_DECT_PHY) || defined(CONFIG_NRF_MODEM_LINK_BINARY_DECT)
#if (CONFIG_NRF_MODEM_SHMEM_CTRL_SIZE != NRF_MODEM_DECT_SHMEM_CTRL_SIZE)
#error The control region size for DECT variant must be updated
#endif
#endif
