/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @brief File containing interrupt handler specific definitions for the
 * HAL Layer of the Wi-Fi driver.
 */

#include "queue.h"
#include "hal_reg.h"
#include "hal_mem.h"
#include "hal_common.h"



enum nrf_wifi_status hal_rpu_irq_enable(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	unsigned int val = 0;

	/* First enable the blockwise interrupt for the relevant block in the
	 * master register
	 */
	status = hal_rpu_reg_read(hal_dev_ctx,
				  &val,
				  RPU_REG_INT_FROM_RPU_CTRL);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: Reading from Root interrupt register failed",
				      __func__);
		goto out;
	}

	val |= (1 << RPU_REG_BIT_INT_FROM_RPU_CTRL);

	status = hal_rpu_reg_write(hal_dev_ctx,
				   RPU_REG_INT_FROM_RPU_CTRL,
				   val);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: Enabling Root interrupt failed",
				      __func__);
		goto out;
	}

	/* Now enable the relevant MCU interrupt line */
	val = (1 << RPU_REG_BIT_INT_FROM_MCU_CTRL);

	status = hal_rpu_reg_write(hal_dev_ctx,
				   RPU_REG_INT_FROM_MCU_CTRL,
				   val);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s:Enabling MCU interrupt failed",
				      __func__);
		goto out;
	}

out:
	return status;
}


enum nrf_wifi_status hal_rpu_irq_disable(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	unsigned int val = 0;

	status = hal_rpu_reg_read(hal_dev_ctx,
				  &val,
				  RPU_REG_INT_FROM_RPU_CTRL);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: Reading from Root interrupt register failed",
				      __func__);
		goto out;
	}

	val &= ~((unsigned int)(1 << RPU_REG_BIT_INT_FROM_RPU_CTRL));

	status = hal_rpu_reg_write(hal_dev_ctx,
				   RPU_REG_INT_FROM_RPU_CTRL,
				   val);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: Disabling Root interrupt failed",
				      __func__);
		goto out;
	}

	val = ~((unsigned int)(1 << RPU_REG_BIT_INT_FROM_MCU_CTRL));

	status = hal_rpu_reg_write(hal_dev_ctx,
				   RPU_REG_INT_FROM_MCU_CTRL,
				   val);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: Disabling MCU interrupt failed",
				      __func__);
		goto out;
	}

out:
	return status;
}


static enum nrf_wifi_status hal_rpu_irq_ack(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	unsigned int val = 0;

	val = (1 << RPU_REG_BIT_INT_FROM_MCU_ACK);

	status = hal_rpu_reg_write(hal_dev_ctx,
				   RPU_REG_INT_FROM_MCU_ACK,
				   val);

	return status;
}


static bool hal_rpu_irq_wdog_chk(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	unsigned int val = 0;
	bool ret = false;

	status = hal_rpu_reg_read(hal_dev_ctx,
				  &val,
				  RPU_REG_MIPS_MCU_UCCP_INT_STATUS);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: Reading from interrupt status register failed",
				      __func__);
		goto out;
	}

	if (val & (1 << RPU_REG_BIT_MIPS_WATCHDOG_INT_STATUS)) {
		ret = true;
	}
out:
	return ret;

}


static enum nrf_wifi_status hal_rpu_irq_wdog_ack(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;

	status = hal_rpu_reg_write(hal_dev_ctx,
				   RPU_REG_MIPS_MCU_UCCP_INT_CLEAR,
				   1 << RPU_REG_BIT_MIPS_WATCHDOG_INT_CLEAR);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: Acknowledging watchdog interrupt failed",
				      __func__);
		goto out;
	}

out:
	return status;

}

static enum nrf_wifi_status hal_rpu_irq_wdog_rearm(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;

	status = hal_rpu_reg_write(hal_dev_ctx,
				  RPU_REG_MIPS_MCU_TIMER,
				  RPU_REG_MIPS_MCU_TIMER_RESET_VAL);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: Rearming watchdog interrupt failed",
				      __func__);
		goto out;
	}
out:
	return status;
}


static enum nrf_wifi_status hal_rpu_event_free(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx,
					       unsigned int event_addr)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;

	status = hal_rpu_hpq_enqueue(hal_dev_ctx,
				     &hal_dev_ctx->rpu_info.hpqm_info.event_avl_queue,
				     event_addr);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: Enqueueing of event failed",
				      __func__);
		goto out;
	}

out:
	return status;
}


static enum nrf_wifi_status hal_rpu_event_get(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx,
					      unsigned int event_addr)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_hal_msg *event = NULL;
	struct host_rpu_msg_hdr *rpu_msg_hdr = NULL;
	unsigned int rpu_msg_len = 0;
	unsigned int event_data_size = 0;
	/* QSPI : avoid global vars as they can be unaligned */
	unsigned char event_data_typical[RPU_EVENT_COMMON_SIZE_MAX];

	nrf_wifi_osal_mem_set(event_data_typical,
			      0,
			      sizeof(event_data_typical));

	if (!hal_dev_ctx->event_data_pending) {
		/* Copy data worth the maximum size of frequently occurring events from
		 * the RPU to a local buffer
		 */
		status = hal_rpu_mem_read(hal_dev_ctx,
					  event_data_typical,
					  event_addr,
					  sizeof(event_data_typical));

		if (status != NRF_WIFI_STATUS_SUCCESS) {
			nrf_wifi_osal_log_err("%s: Reading of the event failed",
					      __func__);
			goto out;
		}

		rpu_msg_hdr = (struct host_rpu_msg_hdr *)event_data_typical;

		rpu_msg_len = rpu_msg_hdr->len;

		/* Allocate space to assemble the entire event */
		hal_dev_ctx->event_data = nrf_wifi_osal_mem_zalloc(rpu_msg_len);

		if (!hal_dev_ctx->event_data) {
			nrf_wifi_osal_log_err("%s: Unable to alloc buff for event data",
					      __func__);
			goto out;
		} else {
			hal_dev_ctx->event_data_curr = hal_dev_ctx->event_data;
		}

		hal_dev_ctx->event_data_len = rpu_msg_len;
		hal_dev_ctx->event_data_pending = rpu_msg_len;
		hal_dev_ctx->event_resubmit = rpu_msg_hdr->resubmit;

		/* Fragmented event */
		if (rpu_msg_len > hal_dev_ctx->hpriv->cfg_params.max_event_size) {
			status = hal_rpu_mem_read(hal_dev_ctx,
						  hal_dev_ctx->event_data_curr,
						  event_addr,
						  hal_dev_ctx->hpriv->cfg_params.max_event_size);


			if (status != NRF_WIFI_STATUS_SUCCESS) {
				nrf_wifi_osal_log_err("%s: Reading of first event fragment failed",
						      __func__);
				nrf_wifi_osal_mem_free(hal_dev_ctx->event_data);
				hal_dev_ctx->event_data = NULL;
				goto out;
			}

			/* Free up the event in the RPU if necessary */
			if (hal_dev_ctx->event_resubmit) {
				status = hal_rpu_event_free(hal_dev_ctx,
							    event_addr);

				if (status != NRF_WIFI_STATUS_SUCCESS) {
					nrf_wifi_osal_log_err("%s: Freeing up of the event failed",
							      __func__);
					nrf_wifi_osal_mem_free(hal_dev_ctx->event_data);
					hal_dev_ctx->event_data = NULL;
					goto out;
				}
			}

			hal_dev_ctx->event_data_pending -=
			       hal_dev_ctx->hpriv->cfg_params.max_event_size;
			hal_dev_ctx->event_data_curr +=
			       hal_dev_ctx->hpriv->cfg_params.max_event_size;
		} else {
			/* If this is not part of a fragmented event check if we need to
			 * copy any additional data i.e. if the event is a corner case
			 * event of large size.
			 */
			if (rpu_msg_len > RPU_EVENT_COMMON_SIZE_MAX) {
				status = hal_rpu_mem_read(hal_dev_ctx,
							  hal_dev_ctx->event_data_curr,
							  event_addr,
							  rpu_msg_len);

				if (status != NRF_WIFI_STATUS_SUCCESS) {
					nrf_wifi_osal_log_err("%s: Reading of large event failed",
							      __func__);
					nrf_wifi_osal_mem_free(hal_dev_ctx->event_data);
					hal_dev_ctx->event_data = NULL;
					goto out;
				}
			} else {
				nrf_wifi_osal_mem_cpy(hal_dev_ctx->event_data_curr,
						      event_data_typical,
						      rpu_msg_len);
			}

			/* Free up the event in the RPU if necessary */
			if (hal_dev_ctx->event_resubmit) {
				status = hal_rpu_event_free(hal_dev_ctx,
							    event_addr);

				if (status != NRF_WIFI_STATUS_SUCCESS) {
					nrf_wifi_osal_log_err("%s: Freeing up of the event failed",
							      __func__);
					nrf_wifi_osal_mem_free(hal_dev_ctx->event_data);
					hal_dev_ctx->event_data = NULL;
					goto out;
				}
			}

			hal_dev_ctx->event_data_pending -= rpu_msg_len;
			hal_dev_ctx->event_data_curr += rpu_msg_len;

		}
	} else {
		event_data_size = (hal_dev_ctx->event_data_pending >
				   hal_dev_ctx->hpriv->cfg_params.max_event_size) ?
				  hal_dev_ctx->hpriv->cfg_params.max_event_size :
				  hal_dev_ctx->event_data_pending;

		if (hal_dev_ctx->event_data) {
			status = hal_rpu_mem_read(hal_dev_ctx,
						  hal_dev_ctx->event_data_curr,
						  event_addr,
						  event_data_size);

			if (status != NRF_WIFI_STATUS_SUCCESS) {
				nrf_wifi_osal_log_err("%s: Reading of large event failed",
						      __func__);
				nrf_wifi_osal_mem_free(hal_dev_ctx->event_data);
				hal_dev_ctx->event_data = NULL;
				goto out;
			}
		}

		/* Free up the event in the RPU if necessary */
		if (hal_dev_ctx->event_resubmit) {
			status = hal_rpu_event_free(hal_dev_ctx,
						    event_addr);

			if (status != NRF_WIFI_STATUS_SUCCESS) {
				nrf_wifi_osal_log_err("%s: Freeing up of the event failed",
						      __func__);
				nrf_wifi_osal_mem_free(hal_dev_ctx->event_data);
				hal_dev_ctx->event_data = NULL;
				goto out;
			}
		}

		hal_dev_ctx->event_data_pending -= event_data_size;
		hal_dev_ctx->event_data_curr += event_data_size;
	}

	/* This is either a unfragmented event or the last fragment of a
	 * fragmented event
	 */
	if (!hal_dev_ctx->event_data_pending) {
		event = nrf_wifi_osal_mem_zalloc(sizeof(*event) + hal_dev_ctx->event_data_len);

		if (!event) {
			nrf_wifi_osal_log_err("%s: Unable to alloc HAL msg for event",
					      __func__);
			nrf_wifi_osal_mem_free(hal_dev_ctx->event_data);
			hal_dev_ctx->event_data = NULL;
			goto out;
		}

		nrf_wifi_osal_mem_cpy(event->data,
				      hal_dev_ctx->event_data,
				      hal_dev_ctx->event_data_len);

		event->len = hal_dev_ctx->event_data_len;

		status = nrf_wifi_utils_q_enqueue(hal_dev_ctx->event_q,
						  event);

		if (status != NRF_WIFI_STATUS_SUCCESS) {
			nrf_wifi_osal_log_err("%s: Unable to queue event",
					      __func__);
			nrf_wifi_osal_mem_free(event);
			event = NULL;
			nrf_wifi_osal_mem_free(hal_dev_ctx->event_data);
			hal_dev_ctx->event_data = NULL;
			goto out;
		}

		/* Reset the state variables */
		nrf_wifi_osal_mem_free(hal_dev_ctx->event_data);
		hal_dev_ctx->event_data = NULL;
		hal_dev_ctx->event_data_curr = NULL;
		hal_dev_ctx->event_data_len = 0;
		hal_dev_ctx->event_resubmit = 0;
	}
out:
	return status;
}


static unsigned int hal_rpu_event_get_all(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	unsigned int num_events = 0;
	unsigned int event_addr = 0;

	while (1) {
		event_addr = 0;

		/* First get the event address */
		status = hal_rpu_hpq_dequeue(hal_dev_ctx,
					     &hal_dev_ctx->rpu_info.hpqm_info.event_busy_queue,
					     &event_addr);

		if (status != NRF_WIFI_STATUS_SUCCESS) {
			nrf_wifi_osal_log_err("%s: Failed to get event addr",
					      __func__);
			goto out;
		}

		/* No more events to read. Sometimes when low power mode is enabled
		 * we see a wrong address, but it work after a while, so, add a
		 * check for that.
		 */
		if (!event_addr || event_addr == 0xAAAAAAAA) {
			break;
		}

		/* Now get the event for further processing */
		status = hal_rpu_event_get(hal_dev_ctx,
					   event_addr);

		if (status != NRF_WIFI_STATUS_SUCCESS) {
			nrf_wifi_osal_log_err("%s: Failed to queue event",
					      __func__);
			goto out;
		}

		num_events++;
	}

out:
	return num_events;
}

static enum nrf_wifi_status hal_rpu_process_wdog(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx,
						  bool *do_rpu_recovery)
{
	enum nrf_wifi_status nrf_wifi_status = NRF_WIFI_STATUS_FAIL;
	bool rpu_recovery = false;
#ifdef CONFIG_NRF_WIFI_LOW_POWER
	unsigned long flags = 0;
#endif

	nrf_wifi_osal_log_dbg("Processing watchdog interrupt");

	/* Check if host has asserted WAKEUP_NOW */
#ifdef CONFIG_NRF_WIFI_LOW_POWER
	nrf_wifi_osal_spinlock_irq_take(hal_dev_ctx->rpu_ps_lock,
					&flags);

	if (hal_dev_ctx->rpu_ps_state == RPU_PS_STATE_AWAKE) {
		nrf_wifi_osal_spinlock_irq_rel(hal_dev_ctx->rpu_ps_lock,
					       &flags);
		nrf_wifi_osal_log_dbg("Host has asserted WAKEUP_NOW, ignoring watchdog interrupt");
		goto out;
	}

	rpu_recovery = true;
	nrf_wifi_osal_spinlock_irq_rel(hal_dev_ctx->rpu_ps_lock,
				       &flags);
#endif /* CONFIG_NRF_WIFI_LOW_POWER */

	if (!rpu_recovery) {
		hal_rpu_irq_wdog_rearm(hal_dev_ctx);
		goto out;
	}

	nrf_wifi_osal_log_dbg("Host has not asserted WAKEUP_NOW, start RPU recovery");
out:
	nrf_wifi_status = NRF_WIFI_STATUS_SUCCESS;
	*do_rpu_recovery = rpu_recovery;

	return nrf_wifi_status;
}

enum nrf_wifi_status hal_rpu_irq_process(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx,
		bool *do_rpu_recovery)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	unsigned int num_events = 0;


	/* Get all the events in the queue. It is possible that there are no
	 * events in the queue. This is a valid scenario as per our present
	 * design (as discussed with LMAC team), since the RPU will raise
	 * interrupts for every event, irrespective of whether the host has
	 * acknowledged the previous event or not. This is being done to
	 * address an issue with FPGA platform where some interrupts are
	 * getting lost. Also RPU does not provide a register to check if
	 * the interrupt is from the RPU, so presently host cannot identify
	 * the interrupt source. This will be a problem in shared interrupt
	 * scenarios and has to be taken care by the SOC designers.
	 */
	num_events = hal_rpu_event_get_all(hal_dev_ctx);

	if (hal_rpu_irq_wdog_chk(hal_dev_ctx)) {
		nrf_wifi_osal_log_dbg("Received watchdog interrupt");

		status = hal_rpu_process_wdog(hal_dev_ctx, do_rpu_recovery);
		if (status == NRF_WIFI_STATUS_FAIL) {
			nrf_wifi_osal_log_err("%s: hal_rpu_process_wdog failed",
					      __func__);
			goto out;
		}

		status = hal_rpu_irq_wdog_ack(hal_dev_ctx);
		if (status == NRF_WIFI_STATUS_FAIL) {
			nrf_wifi_osal_log_err("%s: hal_rpu_irq_wdog_ack failed",
					      __func__);
			goto out;
		}
	}

	status = hal_rpu_irq_ack(hal_dev_ctx);

	if (status == NRF_WIFI_STATUS_FAIL) {
		nrf_wifi_osal_log_err("%s: hal_rpu_irq_ack failed",
				      __func__);
		goto out;
	}
out:
	return status;
}
