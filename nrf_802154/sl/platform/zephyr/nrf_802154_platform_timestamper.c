/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include "platform/nrf_802154_platform_timestamper.h"

#include <haly/nrfy_dppi.h>
#include <haly/nrfy_grtc.h>
#include <hal/nrf_dppi.h>
#include <hal/nrf_ppib.h>

#include <assert.h>
#include <zephyr/drivers/timer/nrf_grtc_timer.h>

static int32_t m_timestamp_cc_channel;

#if defined(NRF54L_SERIES)

/* To trigger GRTC.TASKS_CAPTURE[#cc] with RADIO.EVENT_{?}, the following connection chain must be
 * created:
 *    - starting from RADIO domain (_R_):
 *        {a} RADIO.EVENT_{?}  --> DPPIC_10
 *        {b} DPPIC_10         --> PPIB_11
 *    - crossing domain boundaries
 *        {c} PPIB_11          --> PPIB_21
 *    - ending in the PERI domain (_P_):
 *        {d} PPIB_21          --> DPPIC_20
 *        {e} DPPIC_20         --> GRTC.CC
 */

#include <helpers/nrfx_gppi.h>
#include <soc/interconnect/nrfx_gppi_d2ppi.h>

static nrfx_gppi_handle_t rad_peri_handle;
static uint32_t ppib_chan;

void nrf_802154_platform_timestamper_cross_domain_connections_setup(void)
{
	nrfx_gppi_resource_t resource = {
		.domain_id = NRFX_GPPI_DOMAIN_RAD,
		.channel = 0
	};
	nrf_grtc_task_t capture_task =
		nrfy_grtc_sys_counter_capture_task_get(m_timestamp_cc_channel);
	uint32_t tep = nrfy_grtc_task_address_get(NRF_GRTC, capture_task);
	int err;

	err = nrfx_gppi_ext_conn_alloc(NRFX_GPPI_DOMAIN_RAD, NRFX_GPPI_DOMAIN_PERI,
				       &rad_peri_handle, &resource);
	__ASSERT_NO_MSG(err == 0);

	/* Add task endpoint (GRTC capture) to the previously configured connection. */
	err = nrfx_gppi_ep_attach(tep, rad_peri_handle);
	__ASSERT_NO_MSG(err == 0);

	/* Get PPIB channel used in the connection. */
	ppib_chan = nrfx_gppi_domain_channel_get(rad_peri_handle, NRFX_GPPI_NODE_PPIB11_21);
	__ASSERT_NO_MSG(ppib_chan >= 0);

	/* Disable PPIB for now until exact channel from RADIO domain is known. */
	nrf_ppib_subscribe_clear(NRF_PPIB11, nrf_ppib_send_task_get(ppib_chan));

	nrfx_gppi_conn_enable(rad_peri_handle);
}

void nrf_802154_platform_timestamper_local_domain_connections_setup(uint32_t dppi_ch)
{
	z_nrf_grtc_timer_capture_prepare(m_timestamp_cc_channel);
	/* Configure PPIB to forward provided DPPI channel from Radio domain and enable
	 * the connection.
	 */
	nrf_ppib_subscribe_set(NRF_PPIB11, nrf_ppib_send_task_get(ppib_chan), dppi_ch);
}

#endif

void nrf_802154_platform_timestamper_init(void)
{
	m_timestamp_cc_channel = z_nrf_grtc_timer_chan_alloc();
	assert(m_timestamp_cc_channel >= 0);
}

void nrf_802154_platform_timestamper_cross_domain_connections_clear(void)
{
	nrf_grtc_task_t capture_task =
		nrfy_grtc_sys_counter_capture_task_get(m_timestamp_cc_channel);

	NRF_DPPI_ENDPOINT_CLEAR(nrfy_grtc_task_address_get(NRF_GRTC, capture_task));
}

void nrf_802154_platform_timestamper_local_domain_connections_clear(uint32_t dppi_ch)
{
	/* Intentionally empty. */
}

bool nrf_802154_platform_timestamper_captured_timestamp_read(uint64_t *p_captured)
{
	/* @todo: check if this can be replaced with:
	 *
	 * z_nrf_grtc_timer_capture_read(m_timestamp_cc_channel, p_captured);
	 */
	if (nrf_grtc_sys_counter_cc_enable_check(NRF_GRTC, m_timestamp_cc_channel)) {
		return false;
	}

	*p_captured = nrfy_grtc_sys_counter_cc_get(NRF_GRTC, m_timestamp_cc_channel);
	return true;
}
