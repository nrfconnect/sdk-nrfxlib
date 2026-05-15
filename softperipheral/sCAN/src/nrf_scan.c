/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <nrfx.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "nrf_config_scan.h"

#if defined(NRF_SCAN_ENABLED) && (NRF_SCAN_ENABLED == 1)

#include <hal/nrf_can.h>
#include <hal/nrf_gpio.h>
#include <hal/nrf_vpr.h>
#include <nrf_scan.h>

#ifndef EXCLUDE_SP_FW
#define EXCLUDE_SP_FW 0
#endif
#if EXCLUDE_SP_FW
static void * nvm_fw_addr = (uint8_t *)SP_VPR_FIRMWARE_ADDRESS;
#else
#include <scan_firmware.h>
const void * nvm_fw_addr = (const void *)scan_firmware_bin;
#endif

#define NRFX_LOG_MODULE CAN
#include <nrfx_log.h>
#include <softperipheral_meta.h>
#include <softperipheral_regif.h>

/******************************* NRF_HAL_CAN_H START ************************/

#define SOFTPERIPHERAL_CAN (1)
#if SOFTPERIPHERAL_CAN
#define NRF_CAN_Type       NRF_SP_CAN_Type
#else
#define NRF_CAN_Type       NRF_CAN_Type
#endif

static uint32_t       m_task_count = 1;
nrf_scan_rx_mailbox_t m_scan_rx_mailbox[NRF_SCAN_RXFILTER_MAX_BUFFER_SIZE];

typedef struct
{
    void *                   p_hw_instance;
    nrf_scan_event_handler_t handler;
    void *                   p_context;
    nrf_scan_event_type_t    evt;
    nrfx_drv_state_t         state;

    volatile bool            transfer_in_progress;
    volatile bool            prepared_pending;
    volatile bool            timing_configured;

    struct
    {
        nrf_can_config_t config;
        nrf_can_frame_t  tx_frame;
    }                        conf;
} can_control_block_t;

typedef struct
{
    const nrf_scan_timing_t * p_config;
    nrf_scan_frame_t *        p_tx_frame;
    uint8_t                   drv_inst_idx;
} nrf_scan_request_data_t;

#define NRF_SCAN_ENABLED_COUNT (1)
static can_control_block_t m_cb[NRF_SCAN_ENABLED_COUNT] =
{{.state = NRFX_DRV_STATE_UNINITIALIZED}};
static volatile nrf_scan_request_data_t m_current_request;

NRF_STATIC_INLINE void sp_handshake_set(void * p_reg, uint32_t val, uint8_t idx)
{
    nrf_can_handshake_set((NRF_CAN_Type *)p_reg, val, idx);
}

NRF_STATIC_INLINE uint32_t sp_handshake_get(void * p_reg, uint8_t idx)
{
    return nrf_can_handshake_get((NRF_CAN_Type const *)p_reg, idx);
}

nrf_scan_error_t nrf_scan_init(nrf_scan_t const *       p_scan,
                               nrf_scan_event_handler_t handler,
                               void *                   p_context)
{
    NRFX_ASSERT(p_scan);

    can_control_block_t * p_cb = &m_cb[p_scan->drv_inst_idx];

    if (p_cb->state != NRFX_DRV_STATE_UNINITIALIZED)
    {
        return NRF_SCAN_ERROR_BUSY;
    }

    p_cb->transfer_in_progress = false;
    p_cb->prepared_pending     = false;

    const softperipheral_metadata_t * meta = (const softperipheral_metadata_t *)nvm_fw_addr;
#ifndef UNIT_TEST
    memset((void *)p_scan->p_reg, 0, sizeof(NRF_SP_CAN_Type));

    // Set ENABLE to 1, expect it to become 0 when ready.
    nrf_can_enable((NRF_CAN_Type *)p_scan->p_reg);
#endif
    uint32_t vpr_init_pc = (uint32_t)p_scan->p_reg - meta->fw_shared_ram_addr_offset -
                           (meta->fw_code_size << 4);
    // Copy firmware and start VPR.
    if (meta->self_boot == 0)
    {
#ifndef UNIT_TEST
        memcpy((void *)vpr_init_pc, nvm_fw_addr, meta->fw_code_size << 4);
#endif
        nrf_vpr_initpc_set(NRF_VPR, vpr_init_pc);
    }
    else
    {
        nrf_vpr_initpc_set(NRF_VPR, vpr_init_pc);
    }

    nrf_vpr_cpurun_set(NRF_VPR, true);
#ifndef UNIT_TEST
    while (nrf_can_enable_check((NRF_CAN_Type *)p_scan->p_reg) != false)
    {
        // Wait for peripheral to become ready to receive communication.
    }
#endif

    // Store hardware instance and configuration
    p_cb->p_hw_instance = p_scan->p_reg;

    // Store handler and context
    p_cb->handler   = handler;
    p_cb->p_context = p_context;

    NRFX_IRQ_PRIORITY_SET(SP_VPR_IRQn, 1);

    nrf_can_int_enable((NRF_CAN_Type *)p_scan->p_reg,
                       (NRF_CAN_INT_TXCOMPLETE_MASK | NRF_CAN_INT_RXCOMPLETE_MASK |
                        NRF_CAN_INT_ERRORDETECTED_MASK | NRF_CAN_INT_STATECHANGED_MASK));

    p_cb->state = NRFX_DRV_STATE_INITIALIZED;

    return NRF_SCAN_SUCCESS;
}

bool nrf_scan_init_check(const nrf_scan_t * p_scan)
{
    NRFX_ASSERT(p_scan);
    can_control_block_t * p_cb = &m_cb[p_scan->drv_inst_idx];

    return (p_cb->state != NRFX_DRV_STATE_UNINITIALIZED);
}

nrf_scan_error_t nrf_scan_enable(nrf_scan_t const * p_scan)
{
    NRFX_ASSERT(p_scan);
    can_control_block_t * p_cb = &m_cb[p_scan->drv_inst_idx];

    if (p_cb->state != NRFX_DRV_STATE_INITIALIZED)
    {
        return NRF_SCAN_ERROR_INVALID_STATE;
    }

    nrf_can_enable((NRF_CAN_Type *)p_cb->p_hw_instance);

    __ASB(p_cb->p_hw_instance);

    nrf_can_event_clear((NRF_CAN_Type *)p_cb->p_hw_instance, NRF_CAN_EVENT_TXCOMPLETE);
    nrf_can_event_clear((NRF_CAN_Type *)p_cb->p_hw_instance, NRF_CAN_EVENT_RXCOMPLETE);
    nrf_can_event_clear((NRF_CAN_Type *)p_cb->p_hw_instance, NRF_CAN_EVENT_ERRORDETECTED);
    nrf_can_event_clear((NRF_CAN_Type *)p_cb->p_hw_instance, NRF_CAN_EVENT_STATECHANGED);

    NRFX_IRQ_ENABLE(SP_VPR_IRQn);

    p_cb->state = NRFX_DRV_STATE_POWERED_ON;

    return NRF_SCAN_SUCCESS;
}

nrf_scan_error_t nrf_scan_abort(nrf_scan_t const * p_scan)
{
    NRFX_ASSERT(p_scan);
    can_control_block_t * p_cb = &m_cb[p_scan->drv_inst_idx];

    if (p_cb->state == NRFX_DRV_STATE_UNINITIALIZED)
    {
        return NRF_SCAN_ERROR_INVALID_STATE;
    }

    p_cb->prepared_pending     = false;
    p_cb->timing_configured    = false;
    p_cb->transfer_in_progress = false;
    __SSB(p_cb->p_hw_instance);

    return NRF_SCAN_SUCCESS;
}

nrf_scan_error_t nrf_scan_disable(nrf_scan_t const * p_scan)
{
    NRFX_ASSERT(p_scan);
    can_control_block_t * p_cb = &m_cb[p_scan->drv_inst_idx];

    if (p_cb->state != NRFX_DRV_STATE_POWERED_ON)
    {
        return NRF_SCAN_ERROR_INVALID_STATE;
    }

    nrf_scan_error_t err = nrf_scan_abort(p_scan);
    if (err != NRF_SCAN_SUCCESS)
    {
        return err;
    }

    // while (p_cb->transfer_in_progress) {}

    NRFX_IRQ_DISABLE(SP_VPR_IRQn);

    nrf_can_disable((NRF_CAN_Type *)p_cb->p_hw_instance);

    __ASB(p_cb->p_hw_instance);

    p_cb->state = NRFX_DRV_STATE_INITIALIZED;

    return NRF_SCAN_SUCCESS;
}

void nrf_scan_uninit(nrf_scan_t const * p_scan)
{
    NRFX_ASSERT(p_scan);

    can_control_block_t * p_cb = &m_cb[p_scan->drv_inst_idx];

    //Turn off VPR
    if (p_cb->state == NRFX_DRV_STATE_POWERED_ON)
    {
        nrf_scan_disable(p_scan);
    }

    NRFX_ASSERT(p_cb->state != NRFX_DRV_STATE_UNINITIALIZED);

    // Stop VPR.
    nrf_vpr_cpurun_set(NRF_VPR, false);

    // Reset VPR.
    nrf_vpr_debugif_dmcontrol_mask_set(
        NRF_VPR, (VPR_DEBUGIF_DMCONTROL_NDMRESET_Active << VPR_DEBUGIF_DMCONTROL_NDMRESET_Pos |
                  VPR_DEBUGIF_DMCONTROL_DMACTIVE_Enabled << VPR_DEBUGIF_DMCONTROL_DMACTIVE_Pos));
    nrf_vpr_debugif_dmcontrol_mask_set(
        NRF_VPR, (VPR_DEBUGIF_DMCONTROL_NDMRESET_Inactive << VPR_DEBUGIF_DMCONTROL_NDMRESET_Pos |
                  VPR_DEBUGIF_DMCONTROL_DMACTIVE_Disabled << VPR_DEBUGIF_DMCONTROL_DMACTIVE_Pos));

    p_cb->state = NRFX_DRV_STATE_UNINITIALIZED;
}

#ifndef MAX
#define MAX(x, y)         \
  ({                      \
    __auto_type _x = (x); \
    __auto_type _y = (y); \
    _x > _y ? _x : _y;    \
  })
#endif

#ifndef MIN
#define MIN(x, y)         \
  ({                      \
    __auto_type _x = (x); \
    __auto_type _y = (y); \
    _x < _y ? _x : _y;    \
  })
#endif

#define TIMING_PACK(prescaler, phase_seg1, phase_seg2, rsync_jw)                                    \
    (((uint32_t)(prescaler) << 24) | ((uint32_t)(phase_seg2) << 16) | ((uint32_t)(phase_seg1) << 8) \
     | ((uint32_t)(rsync_jw)))

nrf_scan_error_t nrf_scan_timing(nrf_scan_t const *        p_scan,
                                 nrf_scan_timing_t const * p_timing)
{
    NRFX_ASSERT(p_scan);

    can_control_block_t * p_cb = &m_cb[p_scan->drv_inst_idx];

    if (p_cb->state != NRFX_DRV_STATE_POWERED_ON)
    {
        return NRF_SCAN_ERROR_INVALID_STATE;
    }

    if (!p_timing)
    {
        return NRF_SCAN_ERROR_INVALID_PARAM;
    }

    p_cb->conf.config.request = NRF_CAN_REQUEST_RX;

    if (p_timing->prescaler < 8)
    {
        return NRF_SCAN_ERROR_INVALID_PARAM;
    }

    uint16_t ts1 = p_timing->prop_seg + p_timing->phase_seg1;
    uint16_t ts2 = p_timing->phase_seg2;
    if ((ts1 < 2) || (ts2 < 1) || (ts1 > 16) || (ts2 > p_timing->phase_seg1))
    {
        return NRF_SCAN_ERROR_INVALID_PARAM;
    }
    if ((p_timing->sjw < 1) || (p_timing->sjw > MIN(p_timing->phase_seg1, 4)))
    {
        return NRF_SCAN_ERROR_INVALID_PARAM;
    }
    p_cb->conf.config.timing = TIMING_PACK(p_timing->prescaler, ts1, ts2, p_timing->sjw);

    //Write to regif
    nrf_can_config_set((NRF_CAN_Type *)p_cb->p_hw_instance, &(p_cb->conf.config));
    nrf_can_set_parsing_timing((NRF_CAN_Type *)p_cb->p_hw_instance);

    __CSB(p_cb->p_hw_instance);

    p_cb->timing_configured        = true;
    m_current_request.drv_inst_idx = p_scan->drv_inst_idx;
    m_current_request.p_config     = p_timing;

    return NRF_SCAN_SUCCESS;
}

nrf_scan_error_t nrf_scan_mode(nrf_scan_t const * p_scan, nrf_scan_mode_type_t * p_mode)
{
    NRFX_ASSERT(p_scan);

    can_control_block_t * p_cb = &m_cb[p_scan->drv_inst_idx];

    if (p_cb->state != NRFX_DRV_STATE_POWERED_ON)
    {
        return NRF_SCAN_ERROR_INVALID_STATE;
    }

    if (!p_mode)
    {
        return NRF_SCAN_ERROR_INVALID_PARAM;
    }

    p_cb->conf.config.mode = (nrf_can_mode_t)*p_mode;
    nrf_can_config_set((NRF_CAN_Type *)p_cb->p_hw_instance, &(p_cb->conf.config));
    nrf_can_set_parsing_mode((NRF_CAN_Type *)p_cb->p_hw_instance);

    __CSB(p_cb->p_hw_instance);

    return NRF_SCAN_SUCCESS;
}

nrf_scan_error_t nrf_scan_set_rx_filter(nrf_scan_t const *           p_scan,
                                        // nrf_scan_config_t const * p_config,
                                        nrf_scan_rx_filter_t const * p_rxfilter,
                                        uint8_t                      index)
{
    NRFX_ASSERT(p_scan);

    can_control_block_t * p_cb = &m_cb[p_scan->drv_inst_idx];

    if (index >= NRF_SCAN_RXFILTER_MAX_BUFFER_SIZE)
    {
        return NRF_SCAN_ERROR_INVALID_PARAM;
    }

    if ((p_cb->state != NRFX_DRV_STATE_POWERED_ON))
    {
        return NRF_SCAN_ERROR_INVALID_STATE;
    }

    //Filter overrides a previous one
    memset(&m_scan_rx_mailbox[index], 0, sizeof(m_scan_rx_mailbox[index]));
    m_scan_rx_mailbox[index].rx_filter = *p_rxfilter;

    nrf_can_rxfilter_t rxfilter;
    rxfilter.filter  = p_rxfilter->id;
    rxfilter.id_mask = p_rxfilter->mask;

    //Write to regif
    nrf_can_rxfilter_set((NRF_CAN_Type *)p_cb->p_hw_instance, &rxfilter, index);
    nrf_can_rxfilter_usefilter_set((NRF_CAN_Type *)p_cb->p_hw_instance, index, 1);
    nrf_can_rxfilter_filtermatched_set((NRF_CAN_Type *)p_cb->p_hw_instance, index, 0);
    nrf_can_rxfilter_inmailbox_set((NRF_CAN_Type *)p_cb->p_hw_instance, index, 0);
    nrf_can_set_parsing_rxfilter((NRF_CAN_Type *)p_cb->p_hw_instance);

    __CSB(p_cb->p_hw_instance);

    m_current_request.drv_inst_idx = p_scan->drv_inst_idx;

    return NRF_SCAN_SUCCESS;
}

nrf_scan_error_t nrf_scan_disable_rx_filter(nrf_scan_t const * p_scan, uint8_t index)
{
    NRFX_ASSERT(p_scan);

    can_control_block_t * p_cb = &m_cb[p_scan->drv_inst_idx];

    if (index >= NRF_SCAN_RXFILTER_MAX_BUFFER_SIZE)
    {
        return NRF_SCAN_ERROR_INVALID_PARAM;
    }

    if ((p_cb->state != NRFX_DRV_STATE_POWERED_ON))
    {
        return NRF_SCAN_ERROR_INVALID_STATE;
    }

    //Clear filter usefilter bit to disable application at regif
    nrf_can_rxfilter_usefilter_set((NRF_SP_CAN_Type *)p_cb->p_hw_instance, index, 0);
    nrf_can_set_parsing_rxfilter((NRF_CAN_Type *)p_cb->p_hw_instance);

    __CSB(p_cb->p_hw_instance);

    return NRF_SCAN_SUCCESS;
}

nrf_scan_error_t nrf_scan_enable_rx_filter(nrf_scan_t const * p_scan, uint8_t index)
{
    NRFX_ASSERT(p_scan);

    can_control_block_t * p_cb = &m_cb[p_scan->drv_inst_idx];

    if (index >= NRF_SCAN_RXFILTER_MAX_BUFFER_SIZE)
    {
        return NRF_SCAN_ERROR_INVALID_PARAM;
    }

    if ((p_cb->state != NRFX_DRV_STATE_POWERED_ON))
    {
        return NRF_SCAN_ERROR_INVALID_STATE;
    }

    //Clear filter usefilter bit to disable application at regif
    nrf_can_rxfilter_usefilter_set((NRF_SP_CAN_Type *)p_cb->p_hw_instance, index, 1);
    nrf_can_set_parsing_rxfilter((NRF_CAN_Type *)p_cb->p_hw_instance);

    __CSB(p_cb->p_hw_instance);

    return NRF_SCAN_SUCCESS;
}

nrf_scan_error_t nrf_scan_unlock_rx_mailbox(nrf_scan_t const * p_scan, uint8_t index)
{
    NRFX_ASSERT(p_scan);

    can_control_block_t * p_cb = &m_cb[p_scan->drv_inst_idx];

    if (index >= NRF_SCAN_RXFILTER_MAX_BUFFER_SIZE)
    {
        return NRF_SCAN_ERROR_INVALID_PARAM;
    }

    if ((p_cb->state != NRFX_DRV_STATE_POWERED_ON))
    {
        return NRF_SCAN_ERROR_INVALID_STATE;
    }

    //clear ready
    m_scan_rx_mailbox[index].ready = false;

    //clear API's mailbox
    memset(&m_scan_rx_mailbox[index].rx_frame, 0, sizeof(m_scan_rx_mailbox[index].rx_frame));
    //Clear filter inmailbox bit to allow re-populating m_scan_rx_mailbox at index
    nrf_can_rxfilter_inmailbox_set((NRF_SP_CAN_Type *)p_cb->p_hw_instance, index, 0);
    //Clear filter matched bit to allow overriding at regif
    nrf_can_rxfilter_filtermatched_set((NRF_CAN_Type *)p_cb->p_hw_instance, index, 0);

    nrf_can_set_parsing_rxfilter((NRF_CAN_Type *)p_cb->p_hw_instance);

    __CSB(p_cb->p_hw_instance);

    return NRF_SCAN_SUCCESS;
}

nrf_scan_error_t nrf_scan_send(nrf_scan_t const * p_scan,
                               nrf_scan_frame_t * p_tx_frame)
{
    NRFX_ASSERT(p_scan);

    can_control_block_t * p_cb = &m_cb[p_scan->drv_inst_idx];

    if (p_cb->prepared_pending)
    {
        return NRF_SCAN_ERROR_BUSY;
    }

    if ((p_cb->state != NRFX_DRV_STATE_POWERED_ON) || !p_cb->timing_configured)
    {
        return NRF_SCAN_ERROR_INVALID_STATE;
    }

    if (!p_tx_frame)
    {
        return NRF_SCAN_ERROR_INVALID_PARAM;
    }

    nrf_scan_status_t status = nrf_scan_get_status(p_scan);
    if ((p_cb->conf.config.mode == NRF_CAN_MODE_LISTENONLY) ||
        (status.state == NRF_SCAN_STATE_BUS_OFF))
    {
        return NRF_SCAN_ERROR_INVALID_PARAM;
    }

    p_cb->conf.config.request      = NRF_CAN_REQUEST_TX;
    p_cb->conf.tx_frame.identifier = p_tx_frame->identifier;
    p_cb->conf.tx_frame.crc        = 0; //Just zero out, will be calculated on the fly, may not need after all
    if (p_tx_frame->data_length > 8)
    {
        return NRF_SCAN_ERROR_INVALID_PARAM;
    }
    p_cb->conf.tx_frame.length = p_tx_frame->data_length;

    memcpy(p_cb->conf.tx_frame.data, p_tx_frame->data, sizeof(p_cb->conf.tx_frame.data));
    p_cb->conf.tx_frame.extended_format = p_tx_frame->ide;
    p_cb->conf.tx_frame.remote_request  = p_tx_frame->rtr;

    //Write to regif
    nrf_can_config_set((NRF_CAN_Type *)p_cb->p_hw_instance, &p_cb->conf.config);
    nrf_can_txframe_set((NRF_CAN_Type *)p_cb->p_hw_instance, 0, &p_cb->conf.tx_frame);

    // Store transaction data
    m_current_request.p_tx_frame = p_tx_frame;

    p_cb->transfer_in_progress = true;

    nrf_vpr_task_trigger(NRF_VPR, offsetof(NRF_VPR_Type, TASKS_TRIGGER[SP_VPR_TASK_DPPI_0_IDX]));

    return NRF_SCAN_SUCCESS;
}

nrf_scan_error_t nrf_scan_status_to_err(nrf_scan_t const * p_scan)
{
    can_control_block_t * p_cb      = &m_cb[p_scan->drv_inst_idx];
    nrf_scan_error_t      err       = NRF_SCAN_ERROR_PROTOCOL;
    uint32_t              raw_error = nrf_can_error_get(p_cb->p_hw_instance);

    if (raw_error == 0x0)
    {
        err = NRF_SCAN_SUCCESS;
        return err;
    }

    switch (raw_error)
    {
        case SP_CAN_STATUS_STATUS_BITSTUFFINGERROR_Msk:
        {
            err = NRF_SCAN_ERROR_BIT_STUFFING_ERROR;
        } break;
        case SP_CAN_STATUS_STATUS_BITERROR_Msk | SP_CAN_STATUS_STATUS_BIT0ERROR_Msk:
        {
            err = NRF_SCAN_ERROR_BIT0_ERROR;
        } break;
        case SP_CAN_STATUS_STATUS_BITERROR_Msk | SP_CAN_STATUS_STATUS_BIT1ERROR_Msk:
        {
            err = NRF_SCAN_ERROR_BIT1_ERROR;
        } break;
        case SP_CAN_STATUS_STATUS_FORMERROR_Msk:
        {
            err = NRF_SCAN_ERROR_FORM_ERROR;
        } break;
        case SP_CAN_STATUS_STATUS_ACKERROR_Msk:
        {
            err = NRF_SCAN_ERROR_ACK_ERROR;
        } break;
        case SP_CAN_STATUS_STATUS_CRCERROR_Msk:
        {
            err = NRF_SCAN_ERROR_CRC_ERROR;
        } break;
        default:
        {
            err = NRF_SCAN_ERROR_PROTOCOL;
        } break;
    }

    return err;
}

nrf_scan_state_t nrf_scan_status_to_state(nrf_scan_t const * p_scan)
{
    can_control_block_t * p_cb      = &m_cb[p_scan->drv_inst_idx];
    nrf_scan_state_t      state     = NRF_SCAN_STATE_ERROR_ACTIVE;
    uint32_t              raw_state = nrf_can_state_get(p_cb->p_hw_instance);

    switch (raw_state)
    {
        case NRF_CAN_STATE_ERRORACTIVE:
        {
            state = NRF_SCAN_STATE_ERROR_ACTIVE;
        } break;
        case NRF_CAN_STATE_ERRORPASSIVE:
        {
            state = NRF_SCAN_STATE_ERROR_PASSIVE;
        } break;
        case NRF_CAN_STATE_ERRORWARNING:
        {
            state = NRF_SCAN_STATE_ERROR_WARNING;
        } break;
        case NRF_CAN_STATE_STOPPED:
        {
            state = NRF_SCAN_STATE_STOPPED;
        } break;
        case NRF_CAN_STATE_BUSOFF:
        {
            state = NRF_SCAN_STATE_BUS_OFF;
        } break;
        default:
        {
            state = NRF_SCAN_STATE_ERROR_ACTIVE;
        } break;
    }

    return state;
}

nrf_scan_status_t nrf_scan_get_status(nrf_scan_t const * p_scan)
{
    NRFX_ASSERT(p_scan);
    nrf_scan_status_t status;
    status.state = nrf_scan_status_to_state(p_scan);
    status.error = nrf_scan_status_to_err(p_scan);
    return status;
}

void nrf_scan_irq_handler(void)
{
    if (nrf_vpr_event_check(NRF_VPR, offsetof(NRF_VPR_Type, EVENTS_TRIGGERED[SP_VPR_EVENT_IDX])))
    {
        nrf_vpr_event_clear(NRF_VPR, offsetof(NRF_VPR_Type, EVENTS_TRIGGERED[SP_VPR_EVENT_IDX]));

        can_control_block_t * p_cb = &m_cb[m_current_request.drv_inst_idx];

        // Check for TX completion event
        if (nrf_can_event_check(p_cb->p_hw_instance, NRF_CAN_EVENT_TXCOMPLETE))
        {
            nrf_can_event_clear(p_cb->p_hw_instance, NRF_CAN_EVENT_TXCOMPLETE);

            if ((p_cb->prepared_pending == true) && (p_cb->transfer_in_progress == true)) // There was an ongoing transfer and we have one on hold.
            {
                p_cb->prepared_pending = false;
            }
            else
            {
                p_cb->transfer_in_progress = false;
            }

            p_cb->evt = NRF_SCAN_EVT_TX_COMPLETE;
            p_cb->handler(&p_cb->evt, p_cb->p_context);
        }

        // Check for RX completion event
        if (nrf_can_event_check(p_cb->p_hw_instance, NRF_CAN_EVENT_RXCOMPLETE))
        {
            nrf_can_event_clear(p_cb->p_hw_instance, NRF_CAN_EVENT_RXCOMPLETE);

            for (uint8_t i = 0; i < NRF_SCAN_RXFILTER_MAX_BUFFER_SIZE; i++)
            {
              //get matched element index that is not in any mailbox
                bool filter_matched =
                    nrf_can_rxfilter_filtermatched_get((NRF_CAN_Type *)p_cb->p_hw_instance, i);
                bool in_mailbox =
                    nrf_can_rxfilter_inmailbox_get((NRF_SP_CAN_Type *)p_cb->p_hw_instance, i);
                if (filter_matched && !in_mailbox)
                {
                    memset(&(m_scan_rx_mailbox[i].rx_frame), 0,
                           sizeof(m_scan_rx_mailbox[i].rx_frame));
                    nrf_can_frame_t tmp;
                    nrf_can_rxframe_get(p_cb->p_hw_instance, i, &tmp);
                    m_scan_rx_mailbox[i].rx_frame.identifier  = tmp.identifier;
                    m_scan_rx_mailbox[i].rx_frame.ide         = tmp.extended_format;
                    m_scan_rx_mailbox[i].rx_frame.rtr         = tmp.remote_request;
                    m_scan_rx_mailbox[i].rx_frame.data_length = tmp.length;
                    memcpy(m_scan_rx_mailbox[i].rx_frame.data, tmp.data, tmp.length);
                    m_scan_rx_mailbox[i].ready = true;

                  // mark as in mailbox
                    nrf_can_rxfilter_inmailbox_set((NRF_SP_CAN_Type *)p_cb->p_hw_instance, i, 1);
                  // set context to index global variable
                    nrf_scan_context_t * cntxt;
                    cntxt                       = (nrf_scan_context_t *)(p_cb->p_context);
                    cntxt->last_updated_mailbox = i;
                  //Clear filter matched bit to allow overriding, use nrf_scan_unlock_rx_mailbox
                }
            }

            p_cb->evt = NRF_SCAN_EVT_RX_COMPLETE;
            p_cb->handler(&p_cb->evt, p_cb->p_context);
        }

        if (nrf_can_event_check(p_cb->p_hw_instance, NRF_CAN_EVENT_ERRORDETECTED))
        {
            nrf_can_event_clear(p_cb->p_hw_instance, NRF_CAN_EVENT_ERRORDETECTED);

            p_cb->evt = NRF_SCAN_EVT_ERROR;

            p_cb->handler(&p_cb->evt, p_cb->p_context);
        }

        if (nrf_can_event_check(p_cb->p_hw_instance, NRF_CAN_EVENT_STATECHANGED))
        {
            nrf_can_event_clear(p_cb->p_hw_instance, NRF_CAN_EVENT_STATECHANGED);

            p_cb->evt = NRF_SCAN_EVT_STATE_CHANGED;

            p_cb->handler(&p_cb->evt, p_cb->p_context);
        }
    }
}

#endif // NRFX_CHECK(NRF_SCAN_ENABLED)
