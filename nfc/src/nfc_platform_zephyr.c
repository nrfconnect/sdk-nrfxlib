/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */
#include <clock_control.h>
#include <device.h>
#include <nrfx_nfct.h>
#include <nrfx_timer.h>

#include <logging/log.h>

#define LOG_MODULE_NAME nfc_platform
#define LOG_LEVEL CONFIG_NFC_PLATFORM_LOG_LEVEL
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

struct device *clock;

nrfx_err_t nfc_platform_setup(void)
{
	int err;

	clock = device_get_binding(DT_NORDIC_NRF_CLOCK_0_LABEL "_16M");
	__ASSERT_NO_MSG(clock);

	err = clock_control_on(clock, (void *)1);
	__ASSERT_NO_MSG(!err);

	IRQ_DIRECT_CONNECT(NFCT_IRQn, CONFIG_NFCT_IRQ_PRIORITY,
			   nrfx_nfct_irq_handler, 0);
	IRQ_DIRECT_CONNECT(TIMER4_IRQn, CONFIG_NFCT_IRQ_PRIORITY,
			   nrfx_timer_4_irq_handler, 0);

	LOG_DBG("NFC platform initialized");
	return NRFX_SUCCESS;
}


void nfc_platform_event_handler(nrfx_nfct_evt_t const *event)
{
	switch (event->evt_id) {
	case NRFX_NFCT_EVT_FIELD_DETECTED:
		LOG_DBG("Field detected");
		/* Activate NFCT only when HFXO is running */
		nrfx_nfct_state_force(NRFX_NFCT_STATE_ACTIVATED);
		break;

	case NRFX_NFCT_EVT_FIELD_LOST:
		LOG_DBG("Field lost");
		break;

	default:
		/* No implementation required */
		break;
	}
}
