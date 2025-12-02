/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <nrfx.h>
#include <stdint.h>

#if defined(NRF_SEMMC_ENABLED) && (NRF_SEMMC_ENABLED == 1)
#include <hal/nrf_emmc.h>
#include <hal/nrf_gpio.h>
#include <hal/nrf_vpr.h>
#include <nrf_semmc.h>

#ifndef EXCLUDE_SP_FW
#define EXCLUDE_SP_FW 0
#endif
#if EXCLUDE_SP_FW
static void * nvm_fw_addr = (uint8_t *)SP_VPR_FIRMWARE_ADDRESS;
#else
#include <semmc_firmware.h>
const void * nvm_fw_addr = (const void *)semmc_firmware_bin;
#endif

#define NRFX_LOG_MODULE EMMC
#include <nrfx_log.h>
#include <softperipheral_meta.h>
#include <softperipheral_regif.h>

/******************************* NRF_HAL_EMMC_H START ************************/

#define SOFTPERIPHERAL_EMMC (1)
#if SOFTPERIPHERAL_EMMC
#define NRF_EMMC_Type       NRF_SP_EMMC_Type
#else
#define NRF_EMMC_Type       NRF_EMMC_Type
#endif

static uint32_t m_task_count = 1;

typedef struct
{
    void *                    p_hw_instance;
    nrf_semmc_event_handler_t handler;
    void *                    p_context;
    nrf_semmc_event_t         evt;
    nrfx_drv_state_t          state;
    nrf_semmc_error_t         xfer_err;

    volatile bool             transfer_in_progress;
    volatile bool             prepared_pending;

    struct
    {
        nrf_emmc_config_t config;
    }                         conf;
} emmc_control_block_t;

typedef struct
{
    nrf_semmc_cmd_desc_t *      p_cmd;
    nrf_semmc_transfer_desc_t * p_transfer;
    uint8_t                     drv_inst_idx;
} nrf_semmc_transaction_data_t;

#define NRF_SEMMC_ENABLED_COUNT (1)
static emmc_control_block_t m_cb[NRF_SEMMC_ENABLED_COUNT] =
{{.state = NRFX_DRV_STATE_UNINITIALIZED}};
static volatile nrf_semmc_transaction_data_t m_current_xfer;

NRF_STATIC_INLINE void sp_handshake_set(void * p_reg, uint32_t val, uint8_t idx)
{
    nrf_emmc_handshake_set((NRF_EMMC_Type *)p_reg, val, idx);
}

NRF_STATIC_INLINE uint32_t sp_handshake_get(void * p_reg, uint8_t idx)
{
    return nrf_emmc_handshake_get((NRF_EMMC_Type const *)p_reg, idx);
}

nrf_semmc_error_t nrf_semmc_init(nrf_semmc_t const *       p_semmc,
                                 nrf_semmc_event_handler_t handler,
                                 void *                    p_context)
{
    NRFX_ASSERT(p_semmc);

    emmc_control_block_t * p_cb = &m_cb[p_semmc->drv_inst_idx];

    if (p_cb->state != NRFX_DRV_STATE_UNINITIALIZED)
    {
        return NRF_SEMMC_ERROR_BUSY;
    }

    p_cb->transfer_in_progress = false;
    p_cb->prepared_pending     = false;

    const softperipheral_metadata_t * meta = (const softperipheral_metadata_t *)nvm_fw_addr;
#ifndef UNIT_TEST
    memset((void *)p_semmc->p_reg, 0, sizeof(NRF_SP_EMMC_Type));

    // Set ENABLE to 1, expect it to become 0 when ready.
    nrf_emmc_enable((NRF_EMMC_Type *)p_semmc->p_reg);
#endif
    uint32_t vpr_init_pc = (uint32_t)p_semmc->p_reg - meta->fw_shared_ram_addr_offset -
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
    while (nrf_emmc_enable_check((NRF_EMMC_Type *)p_semmc->p_reg) != false)
    {
        // Wait for peripheral to become ready to receive communication.
    }
#endif

    // Store hardware instance and configuration
    p_cb->p_hw_instance                 = p_semmc->p_reg;
    p_cb->conf.config.ready_to_transfer = false;

    // Store handler and context
    p_cb->handler   = handler;
    p_cb->p_context = p_context;

    NRFX_IRQ_PRIORITY_SET(SP_VPR_IRQn, 1);

    nrf_emmc_int_enable((NRF_EMMC_Type *)p_semmc->p_reg,
                        (NRF_EMMC_INT_XFERCOMPLETE_MASK | NRF_EMMC_INT_ABORTED_MASK |
                         NRF_EMMC_INT_READYTOTRANSFER_MASK));

    p_cb->state = NRFX_DRV_STATE_INITIALIZED;

    return NRF_SEMMC_SUCCESS;
}

bool nrf_semmc_init_check(const nrf_semmc_t * p_semmc)
{
    NRFX_ASSERT(p_semmc);
    emmc_control_block_t * p_cb = &m_cb[p_semmc->drv_inst_idx];

    return (p_cb->state != NRFX_DRV_STATE_UNINITIALIZED);
}

void nrf_semmc_uninit(nrf_semmc_t const * p_semmc)
{
    NRFX_ASSERT(p_semmc);

    emmc_control_block_t * p_cb = &m_cb[p_semmc->drv_inst_idx];

    //Turn off VPR
    if (p_cb->state == NRFX_DRV_STATE_POWERED_ON)
    {
        nrf_semmc_disable(p_semmc);
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

nrf_semmc_error_t nrf_semmc_enable(nrf_semmc_t const * p_semmc)
{
    NRFX_ASSERT(p_semmc);
    emmc_control_block_t * p_cb = &m_cb[p_semmc->drv_inst_idx];

    if (p_cb->state != NRFX_DRV_STATE_INITIALIZED)
    {
        return NRF_SEMMC_ERROR_INVALID_STATE;
    }

    nrf_emmc_enable((NRF_EMMC_Type *)p_cb->p_hw_instance);

    __ASB(p_cb->p_hw_instance);

    nrf_emmc_event_clear((NRF_EMMC_Type *)p_cb->p_hw_instance, NRF_EMMC_EVENT_XFERCOMPLETE);
    nrf_emmc_event_clear((NRF_EMMC_Type *)p_cb->p_hw_instance, NRF_EMMC_EVENT_ABORTED);
    nrf_emmc_event_clear((NRF_EMMC_Type *)p_cb->p_hw_instance, NRF_EMMC_EVENT_READYTOTRANSFER);

    NRFX_IRQ_ENABLE(SP_VPR_IRQn);

    p_cb->state = NRFX_DRV_STATE_POWERED_ON;

    return NRF_SEMMC_SUCCESS;
}

nrf_semmc_error_t nrf_semmc_abort(nrf_semmc_t const * p_semmc)
{
    NRFX_ASSERT(p_semmc);
    emmc_control_block_t * p_cb = &m_cb[p_semmc->drv_inst_idx];

    if (p_cb->state == NRFX_DRV_STATE_UNINITIALIZED)
    {
        return NRF_SEMMC_ERROR_INVALID_STATE;
    }

    p_cb->prepared_pending = false;
    if (p_cb->transfer_in_progress)
    {
        __SSB(p_cb->p_hw_instance);
    }

    return NRF_SEMMC_SUCCESS;
}

nrf_semmc_error_t nrf_semmc_disable(nrf_semmc_t const * p_semmc)
{
    NRFX_ASSERT(p_semmc);
    emmc_control_block_t * p_cb = &m_cb[p_semmc->drv_inst_idx];

    if (p_cb->state != NRFX_DRV_STATE_POWERED_ON)
    {
        return NRF_SEMMC_ERROR_INVALID_STATE;
    }

    nrf_semmc_error_t err = nrf_semmc_abort(p_semmc);
    if (err != NRF_SEMMC_SUCCESS)
    {
        return err;
    }

    while (p_cb->transfer_in_progress)
    {}

    NRFX_IRQ_DISABLE(SP_VPR_IRQn);

    nrf_emmc_disable((NRF_EMMC_Type *)p_cb->p_hw_instance);

    __ASB(p_cb->p_hw_instance);

    p_cb->state = NRFX_DRV_STATE_INITIALIZED;

    return NRF_SEMMC_SUCCESS;
}

nrf_semmc_error_t nrf_semmc_cmd_common(emmc_control_block_t *      p_cb,
                                       nrf_semmc_t const *         p_semmc,
                                       nrf_semmc_cmd_desc_t *      p_cmd,
                                       nrf_semmc_config_t const *  p_config,
                                       nrf_semmc_transfer_desc_t * p_transfer,
                                       size_t                      cmd_count
                                       )
{
    if (cmd_count > NRF_SEMMC_TRANSFERS_PER_REQUEST)
    {
        return NRF_SEMMC_ERROR_UNSUPPORTED;
    }

    if (p_cb->state != NRFX_DRV_STATE_POWERED_ON)
    {
        return NRF_SEMMC_ERROR_INVALID_STATE;
    }

    if (!p_cmd)
    {
        return NRF_SEMMC_ERROR_INVALID_PARAM;
    }
    if (!p_transfer)
    {
        return NRF_SEMMC_ERROR_INVALID_PARAM;
    }

    // sEMMC config
    uint32_t clkdiv = (uint32_t)SP_VPR_BASE_FREQ_HZ / p_config->clk_freq_hz;
    //clkdiv must be an even number
    if (clkdiv & 0x1)
    {
        return NRF_SEMMC_ERROR_INVALID_PARAM;
    }
    if (clkdiv < 4)
    {
        return NRF_SEMMC_ERROR_INVALID_PARAM;
    }
    p_cb->conf.config.clkfreqhz = p_config->clk_freq_hz;
    if ((nrf_emmc_bus_width_t)p_config->bus_width > NRF_EMMC_BUS_WIDTH_4_LANES)
    {
        return NRF_SEMMC_ERROR_INVALID_PARAM;
    }
    else
    {
        p_cb->conf.config.bus_width = (nrf_emmc_bus_width_t)p_config->bus_width;
    }
    if (p_config->read_delay >= clkdiv)
    {
        return NRF_SEMMC_ERROR_INVALID_PARAM;
    }
    p_cb->conf.config.read_delay = p_config->read_delay;
    if (p_config->spis_instance != NULL)
    {
        return NRF_SEMMC_ERROR_UNSUPPORTED;
    }
    nrf_emmc_config_set((NRF_EMMC_Type *)p_cb->p_hw_instance, &(p_cb->conf.config));

    // Configure EMMC command
    nrf_emmc_command_t emmc_cmd = {0};
    emmc_cmd.idx = p_cmd->cmd;
    emmc_cmd.arg = p_cmd->arg;

    // Map response types
    switch (p_cmd->resp_type)
    {
        case NRF_SEMMC_RESP_NONE: emmc_cmd.resp_type = NRF_EMMC_RESPONSE_NONE; break;
        case NRF_SEMMC_RESP_R1: emmc_cmd.resp_type   = NRF_EMMC_RESPONSE_R1; break;
        case NRF_SEMMC_RESP_R1B: emmc_cmd.resp_type  = NRF_EMMC_RESPONSE_R1B; break;
        case NRF_SEMMC_RESP_R2: emmc_cmd.resp_type   = NRF_EMMC_RESPONSE_R2; break;
        case NRF_SEMMC_RESP_R3: emmc_cmd.resp_type   = NRF_EMMC_RESPONSE_R3; break;
        default: return NRF_SEMMC_ERROR_INVALID_PARAM;
    }

    emmc_cmd.resp_proc = p_config->process_response;
    emmc_cmd.resp_addr = (uint32_t)p_cmd->resp_buffer;
    // Set command in hardware
    nrf_emmc_command_set((NRF_EMMC_Type *)p_cb->p_hw_instance, &emmc_cmd);

    nrf_emmc_data_t emmc_data = {0};
    emmc_data.buffer_addr = (uint32_t)p_transfer->buffer;
    emmc_data.block_size  = p_transfer->block_size;
    emmc_data.block_num   = p_transfer->num_blocks;

    // Set data configuration in hardware
    nrf_emmc_data_set((NRF_EMMC_Type *)p_cb->p_hw_instance, &emmc_data);

    __CSB(p_cb->p_hw_instance);

    // Store transaction data
    m_current_xfer.drv_inst_idx = p_semmc->drv_inst_idx;
    m_current_xfer.p_cmd        = p_cmd;
    m_current_xfer.p_transfer   = p_transfer;

    return NRF_SEMMC_SUCCESS;
}

nrf_semmc_error_t nrf_semmc_cmd(nrf_semmc_t const *         p_semmc,
                                nrf_semmc_cmd_desc_t *      p_cmd,
                                nrf_semmc_config_t const *  p_config,
                                nrf_semmc_transfer_desc_t * p_transfer,
                                size_t                      cmd_count,
                                uint32_t                    flags)
{
    if (flags == NRF_SEMMC_FLAG_HOLD_XFER)
    {
        return nrf_semmc_cmd_prepare(p_semmc, p_cmd, p_config, p_transfer, cmd_count);
    }

    NRFX_ASSERT(p_semmc);
    emmc_control_block_t * p_cb = &m_cb[p_semmc->drv_inst_idx];

    if (!p_cmd)
    {
        return NRF_SEMMC_ERROR_INVALID_PARAM;
    }
    if ((p_cb->transfer_in_progress || p_cb->prepared_pending) &&
        (p_cmd->cmd != NRF_SEMMC_CMD12_STOP_TRANSMISSION))
    {
        return NRF_SEMMC_ERROR_BUSY;
    }

    if (!p_config)
    {
        return NRF_SEMMC_ERROR_INVALID_PARAM;
    }

    if (!p_transfer)
    {
        return NRF_SEMMC_ERROR_INVALID_PARAM;
    }

    nrf_semmc_error_t retval;
    retval = nrf_semmc_cmd_common(p_cb, p_semmc, p_cmd, p_config, p_transfer, cmd_count);
    if (retval == NRF_SEMMC_SUCCESS)
    {
        p_cb->conf.config.ready_to_transfer = true;
        nrf_emmc_config_set_ready_to_transfer(p_cb->p_hw_instance, &(p_cb->conf.config));
        __ASB(p_cb->p_hw_instance);
        p_cb->transfer_in_progress = true;

        nrf_vpr_task_trigger(NRF_VPR,
                             offsetof(NRF_VPR_Type, TASKS_TRIGGER[SP_VPR_TASK_DPPI_0_IDX]));
    }

    return retval;
}

nrf_semmc_error_t nrf_semmc_cmd_prepare(nrf_semmc_t const *         p_semmc,
                                        nrf_semmc_cmd_desc_t *      p_cmd,
                                        nrf_semmc_config_t const *  p_config,
                                        nrf_semmc_transfer_desc_t * p_transfer,
                                        size_t                      cmd_count)
{
    NRFX_ASSERT(p_semmc);
    emmc_control_block_t * p_cb = &m_cb[p_semmc->drv_inst_idx];

    if (!p_cmd)
    {
        return NRF_SEMMC_ERROR_INVALID_PARAM;
    }
    if (p_cb->prepared_pending)
    {
        return NRF_SEMMC_ERROR_BUSY;
    }

    if (!p_config)
    {
        return NRF_SEMMC_ERROR_INVALID_PARAM;
    }

    if (!p_transfer)
    {
        return NRF_SEMMC_ERROR_INVALID_PARAM;
    }

    nrf_semmc_error_t retval;
    retval = nrf_semmc_cmd_common(p_cb, p_semmc, p_cmd, p_config, p_transfer, cmd_count);
    if (retval == NRF_SEMMC_SUCCESS)
    {
        p_cb->prepared_pending              = true;
        p_cb->conf.config.ready_to_transfer = true;
        nrf_emmc_config_set_ready_to_transfer(p_cb->p_hw_instance, &(p_cb->conf.config));
        __ASB(p_cb->p_hw_instance);
    }
    return retval;
}

bool nrf_semmc_is_busy(nrf_semmc_t const * p_semmc)
{
    NRFX_ASSERT(p_semmc);
    emmc_control_block_t * p_cb = &m_cb[p_semmc->drv_inst_idx];

    return p_cb->transfer_in_progress || p_cb->prepared_pending;
}

nrf_semmc_error_t nrf_semmc_status_to_err(nrf_semmc_t const * p_semmc)
{
    emmc_control_block_t * p_cb       = &m_cb[p_semmc->drv_inst_idx];
    nrf_semmc_error_t      err        = NRF_SEMMC_ERROR_ILLEGAL_CMD;
    uint32_t               raw_status = nrf_emmc_status_get(p_cb->p_hw_instance);

    switch (raw_status)
    {
        case SP_EMMC_STATUS_STATUS_CMDTIMEOUT_Msk:
        {
            err = NRF_SEMMC_ERROR_CMDTIMEOUT;
        } break;
        case SP_EMMC_STATUS_STATUS_CMDCRCERROR_Msk:
        {
            err = NRF_SEMMC_ERROR_CMDCRCERROR;
        } break;
        case SP_EMMC_STATUS_STATUS_DATACRCERROR_Msk:
        {
            err = NRF_SEMMC_ERROR_DATACRCERROR;
        } break;
        case SP_EMMC_STATUS_STATUS_PROTOCOLERR_Msk:
        {
            err = NRF_SEMMC_ERROR_INTERNAL_OR_PROTOCOL;
        } break;
        default:
        {
            err = NRF_SEMMC_SUCCESS;
        } break;
    }
    return err;
}

void nrf_semmc_get_status_response_after_xfer(nrf_semmc_t const *    p_semmc,
                                              nrf_semmc_cmd_desc_t * p_cmd)
{
    NRFX_ASSERT(p_semmc);
    emmc_control_block_t * p_cb = &m_cb[p_semmc->drv_inst_idx];

    p_cb->xfer_err = nrf_semmc_status_to_err(p_semmc);
    p_cmd->err     = p_cb->xfer_err;
  //Get the response if we're expecting one
    if (p_cmd->resp_type != NRF_SEMMC_RESP_NONE)
    {
        nrf_emmc_command_get_response(p_cb->p_hw_instance, p_cmd->response);
    }
}

void nrf_semmc_irq_handler(void)
{
    if (nrf_vpr_event_check(NRF_VPR, offsetof(NRF_VPR_Type, EVENTS_TRIGGERED[SP_VPR_EVENT_IDX])))
    {
        nrf_vpr_event_clear(NRF_VPR, offsetof(NRF_VPR_Type, EVENTS_TRIGGERED[SP_VPR_EVENT_IDX]));

        emmc_control_block_t * p_cb = &m_cb[m_current_xfer.drv_inst_idx];

        if (nrf_emmc_event_check(p_cb->p_hw_instance, NRF_EMMC_EVENT_READYTOTRANSFER))
        {
            nrf_emmc_event_clear(p_cb->p_hw_instance, NRF_EMMC_EVENT_READYTOTRANSFER);
            p_cb->evt.type           = NRF_SEMMC_EVT_XFER_STARTED;
            p_cb->evt.data.xfer_done = NRF_SEMMC_RESULT_OK;
            if ((p_cb->prepared_pending == true) && (p_cb->transfer_in_progress == false))
            {
                // Transition the pending into a transfer_in_progress to allow for a new xfer to be scheduled.
                p_cb->transfer_in_progress = true;
                p_cb->prepared_pending     = false;
            }

            // Indicate to the app that the prepared transaction has been triggered.
            p_cb->handler(&p_cb->evt, p_cb->p_context);
        }

        // Check for completion event
        if (nrf_emmc_event_check(p_cb->p_hw_instance, NRF_EMMC_EVENT_XFERCOMPLETE))
        {
            nrf_emmc_event_clear(p_cb->p_hw_instance, NRF_EMMC_EVENT_XFERCOMPLETE);

            //End current transaction
            p_cb->conf.config.ready_to_transfer = false;
            nrf_emmc_config_set_ready_to_transfer(p_cb->p_hw_instance, &(p_cb->conf.config));
            __ASB(p_cb->p_hw_instance);

            if ((p_cb->prepared_pending == true) && (p_cb->transfer_in_progress == true)) // There was an ongoing transfer and we have one on hold.
            {
                // Trigger the pending transaction.
                p_cb->conf.config.ready_to_transfer = true;
                nrf_emmc_config_set_ready_to_transfer(p_cb->p_hw_instance, &(p_cb->conf.config));
                __ASB(p_cb->p_hw_instance);
                // Continue transfer_in_progress, but clear the pending.
                p_cb->prepared_pending = false;
            }
            else
            {
                p_cb->transfer_in_progress = false;
            }

            p_cb->evt.type           = NRF_SEMMC_EVT_XFER_DONE;
            p_cb->evt.data.xfer_done = NRF_SEMMC_RESULT_OK;

            nrf_semmc_get_status_response_after_xfer(p_cb->p_hw_instance, m_current_xfer.p_cmd);
            nrf_emmc_data_t emmc_data = {0};
            nrf_emmc_get_num_blocks(p_cb->p_hw_instance, &emmc_data);
            m_current_xfer.p_transfer->num_blocks = emmc_data.block_num;

            p_cb->handler(&p_cb->evt, p_cb->p_context);
        }

        // Check for abort event
        if (nrf_emmc_event_check(p_cb->p_hw_instance, NRF_EMMC_EVENT_ABORTED))
        {
            nrf_emmc_event_clear(p_cb->p_hw_instance, NRF_EMMC_EVENT_ABORTED);

            p_cb->transfer_in_progress = false;
            p_cb->prepared_pending     = false;

            p_cb->evt.type           = NRF_SEMMC_EVT_XFER_DONE;
            p_cb->evt.data.xfer_done = NRF_SEMMC_RESULT_ABORTED;

            p_cb->conf.config.ready_to_transfer = false;
            nrf_emmc_config_set_ready_to_transfer(p_cb->p_hw_instance, &(p_cb->conf.config));
            __ASB(p_cb->p_hw_instance);

            p_cb->xfer_err            = NRF_SEMMC_ERROR_TIMEOUT;
            m_current_xfer.p_cmd->err = p_cb->xfer_err;
            nrf_emmc_command_get_response(p_cb->p_hw_instance, m_current_xfer.p_cmd->response);

            nrf_emmc_data_t emmc_data = {0};
            nrf_emmc_get_num_blocks(p_cb->p_hw_instance, &emmc_data);
            m_current_xfer.p_transfer->num_blocks = emmc_data.block_num;

            p_cb->handler(&p_cb->evt, p_cb->p_context);
        }
    }
}

uint32_t * nrf_semmc_start_task_address_get(nrf_semmc_t const * p_semmc)
{
    (void)p_semmc;
    return (uint32_t *)(nrf_vpr_task_address_get(NRF_VPR,
                                                 (nrf_vpr_task_t)offsetof(NRF_VPR_Type,
                                                                          TASKS_TRIGGER[
                                                                              SP_VPR_TASK_DPPI_0_IDX])));
}

#endif // NRFX_CHECK(NRF_SEMMC_ENABLED)
