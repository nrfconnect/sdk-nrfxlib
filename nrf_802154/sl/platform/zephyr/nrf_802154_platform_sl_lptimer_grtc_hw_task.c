/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include "nrf_802154_platform_sl_lptimer_grtc_hw_task.h"

#include <haly/nrfy_dppi.h>
#include <haly/nrfy_grtc.h>
#include <hal/nrf_ppib.h>

#include <platform/nrf_802154_platform_sl_lptimer.h>
#include <zephyr/drivers/timer/nrf_grtc_timer.h>

#if defined(NRF54L_SERIES)

#include <helpers/nrfx_gppi.h>
#include <soc/interconnect/nrfx_gppi_d2ppi.h>

/* To trigger RADIO.TASKS_x with GRTC.EVENT_CAPTURE{?}, the following connection chain must be
 * created:
 *    - starting with the PERI domain (_P_):
 *        {a} DPPIC_20         <-- GRTC.CC
 *        {b} PPIB_21          <-- DPPIC_20
 *    - crossing domain boundaries
 *        {c} PPIB_11          <-- PPIB_21
 *    - ending with RADIO domain (_R_):
 *        {d} DPPIC_10         <-- PPIB_11
 *        {e} RADIO.TASK_{?}   <-- DPPIC_10
 */

#define INVALID_CHANNEL UINT8_MAX

static nrfx_gppi_handle_t peri_rad_handle;
static uint32_t ppib_chan;

void nrf_802154_platform_sl_lptimer_hw_task_cross_domain_connections_setup(uint32_t cc_channel)
{
	nrfx_gppi_resource_t resource = {
		.domain_id = NRFX_GPPI_DOMAIN_RAD,
		.channel = 0
	};
	uint32_t eep = z_nrf_grtc_timer_compare_evt_address_get(cc_channel);
	int err;

	err = nrfx_gppi_ext_conn_alloc(NRFX_GPPI_DOMAIN_PERI, NRFX_GPPI_DOMAIN_RAD,
				       &peri_rad_handle, &resource);
	__ASSERT_NO_MSG(err == 0);

	/* Add event endpoint (GRTC compare) to the previously configured connection. */
	err = nrfx_gppi_ep_attach(eep, peri_rad_handle);
	__ASSERT_NO_MSG(err == 0);

	/* Get PPIB channel used in the connection. */
	ppib_chan = nrfx_gppi_domain_channel_get(peri_rad_handle, NRFX_GPPI_NODE_PPIB11_21);
	__ASSERT_NO_MSG(ppib_chan >= 0);

	/* Disable PPIB for now until exact channel from RADIO domain is known. */
	nrf_ppib_publish_clear(NRF_PPIB11, nrf_ppib_receive_event_get(ppib_chan));

	nrfx_gppi_conn_enable(peri_rad_handle);
}

void nrf_802154_platform_sl_lptimer_hw_task_cross_domain_connections_clear(void)
{
	nrfx_gppi_conn_disable(peri_rad_handle);
	nrfx_gppi_domain_conn_free(peri_rad_handle);
}

void nrf_802154_platform_sl_lptimer_hw_task_local_domain_connections_setup(uint32_t dppi_ch,
									   uint32_t cc_channel)
{
	if (dppi_ch == NRF_802154_SL_HW_TASK_PPI_INVALID) {
		return;
	}

	/* Configure PPIB to forward GRTC event to the DPPI channel from the Radio domain. */
	nrf_ppib_publish_set(NRF_PPIB11, nrf_ppib_receive_event_get(ppib_chan), dppi_ch);
}

void nrf_802154_platform_sl_lptimer_hw_task_local_domain_connections_clear(void)
{
	nrf_ppib_publish_clear(NRF_PPIB11, nrf_ppib_receive_event_get(ppib_chan));
}

#endif
