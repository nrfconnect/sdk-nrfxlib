/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <nrfx.h>

//#if NRFX_CHECK(NRF_SQSPI_ENABLED)
#if defined(NRF_SQSPI_ENABLED) && (NRF_SQSPI_ENABLED == 1)
#include <hal/nrf_gpio.h>
#include <hal/nrf_qspi2.h>
#include <hal/nrf_vpr.h>
#include <nrf_sqspi.h>

#if defined (NRF54L20_ENGA_XXAA) || defined (NRF54LM20A_ENGA_XXAA)

#include <hal/nrf_egu.h>
#endif

#ifndef EXCLUDE_SP_FW
#define EXCLUDE_SP_FW 0
#endif
#if EXCLUDE_SP_FW
static void * nvm_fw_addr = (uint8_t *)SP_VPR_FIRMWARE_ADDRESS;
#else
#include <sqspi_firmware.h>
const void * nvm_fw_addr = (const void *)sqspi_firmware_bin;
#endif

#define NRFX_LOG_MODULE QSPI
#include <nrfx_log.h>
#include <softperipheral_meta.h>
#include <softperipheral_regif.h>

/******************************* NRF_HAL_QSPI_H START ************************/

#define SOFTPERIPHERAL_QSPI (1)
#if SOFTPERIPHERAL_QSPI
#define NRF_QSPI2_Type      NRF_SP_QSPI_Type
#else
#define NRF_QSPI2_Type      NRF_QSPI_Type
#endif

#define NRF_QSPI2_CORE_CTRLR0_DEFAULT_CONF                      \
    {.dfs = QSPI_CORE_CORE_CTRLR0_DFS_DFS08BIT,                 \
     .frf = QSPI_CORE_CORE_CTRLR0_FRF_SPI,                      \
     .scph = QSPI_CORE_CORE_CTRLR0_SCPH_MIDDLEBIT,              \
     .scpol = QSPI_CORE_CORE_CTRLR0_SCPOL_INACTIVEHIGH,         \
     .tmod = QSPI_CORE_CORE_CTRLR0_TMOD_TXONLY,                 \
     .slvoe = QSPI_CORE_CORE_CTRLR0_SLVOE_ENABLED,              \
     .srl = QSPI_CORE_CORE_CTRLR0_SRL_NORMALMODE,               \
     .sste = QSPI_CORE_CORE_CTRLR0_SSTE_TOGGLEDISABLE,          \
     .cfs = QSPI_CORE_CORE_CTRLR0_CFS_SIZE08BIT,                \
     .spifrf = QSPI_CORE_CORE_CTRLR0_SPIFRF_SPISTANDARD,        \
     .hyperbusen = QSPI_CORE_CORE_CTRLR0_SPIHYPERBUSEN_DISABLE, \
     .spidwsen = QSPI_CORE_CORE_CTRLR0_SPIDWSEN_DISABLE,        \
     .clkloopen = QSPI_CORE_CORE_CTRLR0_CLKLOOPEN_DISABLE,      \
     .sqspiismst = QSPI_CORE_CORE_CTRLR0_SQSPIISMST_CONTROLLER}

#define NRF_QSPI2_CORE_SPICTRLR0_DEFAULT_CONF             \
    {.transtype = QSPI_CORE_CORE_SPICTRLR0_TRANSTYPE_TT0, \
     .addrl = QSPI_CORE_CORE_SPICTRLR0_ADDRL_ADDRL0,      \
     .instl = QSPI_CORE_CORE_SPICTRLR0_INSTL_INSTL8,      \
     .waitcycles = 0,                                     \
     .clkstretchen = 0}

static uint32_t m_task_count = 1;

typedef struct
{
    nrf_sqspi_t          qspi;
    nrf_sqspi_callback_t handler;
    void *               p_context;
    nrf_sqspi_evt_t      evt;
    nrfx_drv_state_t     state;

    volatile bool        transfer_in_progress;
    volatile bool        prepared_pending;
    bool                 skip_gpio_cfg : 1;
    bool                 skip_pmux_cfg : 1;

    union
    {
        struct
        {
            uint32_t sck;
            uint32_t io[4];
            uint32_t ss;
        };

        uint32_t ios[6];
    }                    pins;

    struct
    {
        nrf_qspi2_core_ctrlr0_t    ctrlr0;
        nrf_qspi2_core_spictrlr0_t spictrlr0;
        nrf_qspi2_format_t         format;
    }                    conf;
} qspi2_control_block_t;

typedef struct
{
    nrf_sqspi_xfer_dir_t dir;
    uint8_t *            p_dest;
    uint32_t             dest_len;
    uint8_t              drv_inst_idx;
} nrf_sqspi_transaction_data_t;

#define NRF_SQSPI_ENABLED_COUNT (1)
static qspi2_control_block_t m_cb[NRF_SQSPI_ENABLED_COUNT] =
{{.state = NRFX_DRV_STATE_UNINITIALIZED}};
static volatile nrf_sqspi_transaction_data_t m_current_xfer;

nrfx_err_t nrf_sqspi_init(const nrf_sqspi_t * p_qspi, const nrf_sqspi_cfg_t * p_config)
{
    NRFX_ASSERT(p_qspi);
    NRFX_ASSERT(p_config);

    qspi2_control_block_t * p_cb = &m_cb[p_qspi->drv_inst_idx];

    if (p_cb->state != NRFX_DRV_STATE_UNINITIALIZED)
    {
        return NRFX_ERROR_ALREADY;
    }

    p_cb->transfer_in_progress = false;
    p_cb->prepared_pending     = false;

    //Formatting default values
    p_cb->conf.format.pixels   = 0;
    p_cb->conf.format.dfs      = 7;
    p_cb->conf.ctrlr0.dfs      = 7;
    p_cb->conf.format.bpp      = 8;
    p_cb->conf.format.padding  = 0;
    p_cb->conf.format.bitorder = 0;
    p_cb->conf.format.cilen    = 0;

    // Set all pins to unused.
    for (int i = 0; i < 6; i++)
    {
        p_cb->pins.ios[i] = NRF_SQSPI_PINS_UNUSED;
    }

    const softperipheral_metadata_t * meta = (const softperipheral_metadata_t *)nvm_fw_addr;
#ifndef UNIT_TEST
    memset((void *)p_qspi->p_reg, 0, sizeof(NRF_SP_QSPI_Type));

    // Set ENABLE to 1, expect it to become 0 when ready.
    nrf_qspi2_enable(p_qspi->p_reg);
#endif
    uint32_t vpr_init_pc = (uint32_t)p_qspi->p_reg - meta->fw_shared_ram_addr_offset -
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
    while (nrf_qspi2_enable_check(p_qspi->p_reg) != false)
    {
        // Wait for peripheral to becore ready to receive communication.
    }
#endif
    // book-keep for dev_cfg and deactivate / uninit.
    p_cb->skip_gpio_cfg = p_config->skip_gpio_cfg;
    p_cb->skip_pmux_cfg = p_config->skip_pmux_cfg;

    if (p_config->skip_gpio_cfg == false)
    {
        if (p_config->pins.sck != NRF_SQSPI_PINS_UNUSED)
        {
            p_cb->pins.sck = p_config->pins.sck;
#if defined(NRF54L_SERIES) || defined(NRF54H20_XXAA)
            nrf_gpio_cfg(p_config->pins.sck, NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT,
                         NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);
#endif
        }
        else
        {
            return NRFX_ERROR_INVALID_PARAM; // We need SCK.
        }

        for (int i = 0; i < NRF_SQSPI_MAX_NUM_DATA_LINES; i++)
        {
            if (p_config->pins.io[i] != NRF_SQSPI_PINS_UNUSED)
            {
                p_cb->pins.io[i] = p_config->pins.io[i];
#if defined(NRF54L_SERIES) || defined(NRF54H20_XXAA)
                nrf_gpio_cfg(p_config->pins.io[i], NRF_GPIO_PIN_DIR_OUTPUT,
                             NRF_GPIO_PIN_INPUT_CONNECT,
                             NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);
#endif
            }
        }
    }
    if (p_config->skip_pmux_cfg == false)
    {
        if (p_cb->pins.sck != NRF_SQSPI_PINS_UNUSED)
        {
#if defined(NRF54L_SERIES)
            nrf_gpio_pin_control_select(p_config->pins.sck, NRF_GPIO_PIN_SEL_VPR);
#elif defined(NRF54H20_XXAA)
            // CTRLSEL setting of GPIO must be done by SecDom through UICR configuration.
#else
#pragma warning "Unknown platform"
#endif
        }
        else
        {
            return NRFX_ERROR_INVALID_PARAM; // We need SCK.
        }
        for (int i = 0; i < NRF_SQSPI_MAX_NUM_DATA_LINES; i++)
        {
            if (p_cb->pins.io[i] != NRF_SQSPI_PINS_UNUSED)
            {
#if defined(NRF54L_SERIES)
                nrf_gpio_pin_control_select(p_config->pins.io[i], NRF_GPIO_PIN_SEL_VPR);
#elif defined(NRF54H20_XXAA)
                // CTRLSEL setting of GPIO must be done by SecDom through UICR configuration.
#else
#pragma warning "Unknown platform"
#endif
            }
        }
    }

    nrf_qspi2_format_dfs(p_qspi->p_reg, (uint8_t)(p_cb->conf.format.dfs));
    nrf_qspi2_core_dr_x(p_qspi->p_reg, (uint32_t)(32 - p_cb->conf.format.padding), 22); //Hijacking DR[22] as red_dfs
    nrf_qspi2_format_bpp(p_qspi->p_reg, (uint8_t)(p_cb->conf.format.bpp));
    nrf_qspi2_format_bitorder(p_qspi->p_reg, (int)p_cb->conf.format.bitorder,
                              (int)p_cb->conf.format.bitorder);

    NRFX_IRQ_PRIORITY_SET(SP_VPR_IRQn, 1);

    nrf_qspi2_int_enable(p_qspi->p_reg, (QSPI_INTEN_DMADONE_Enabled << QSPI_INTEN_DMADONE_Pos));
    nrf_qspi2_int_enable(p_qspi->p_reg,
                         (QSPI_INTEN_DMAABORTED_Enabled << QSPI_INTEN_DMAABORTED_Pos));
    nrf_qspi2_int_enable(p_qspi->p_reg,
                         (QSPI_INTEN_DMADONEJOB_Enabled << QSPI_INTEN_DMADONEJOB_Pos));

    p_cb->state = NRFX_DRV_STATE_INITIALIZED;

    return NRFX_SUCCESS;
}

bool nrf_sqspi_init_check(const nrf_sqspi_t * p_qspi)
{
    qspi2_control_block_t * p_cb = &m_cb[p_qspi->drv_inst_idx];

    return (p_cb->state != NRFX_DRV_STATE_UNINITIALIZED);
}

nrfx_err_t nrf_sqspi_reconfigure(const nrf_sqspi_t * p_qspi, const nrf_sqspi_cfg_t * p_config)
{
    (void)p_qspi;
    (void)p_config;
    return NRFX_ERROR_NOT_SUPPORTED;
}

void nrf_sqspi_uninit(const nrf_sqspi_t * p_qspi)
{
    qspi2_control_block_t * p_cb = &m_cb[p_qspi->drv_inst_idx];

    //Turn off VPR
    if (p_cb->state == NRFX_DRV_STATE_POWERED_ON)
    {
        nrf_sqspi_deactivate(p_qspi);
    }

    NRFX_ASSERT(p_cb->state != NRFX_DRV_STATE_UNINITIALIZED);

    // Decouple pins from VPR using GPIO.CTRLSEL back to GPIO.
    // Disconnect pins.
    for (uint8_t i = 0; i < 6; i++)
    {
        if (p_cb->pins.ios[i] != NRF_SQSPI_PINS_UNUSED)
        {
            nrf_gpio_cfg_default(p_cb->pins.ios[i]);
            if (p_cb->skip_pmux_cfg == false)
            {
#if defined(NRF54L_SERIES)
                nrf_gpio_pin_control_select(p_cb->pins.ios[i], NRF_GPIO_PIN_SEL_GPIO);
#endif
            }
        }
        p_cb->pins.ios[i] = NRF_SQSPI_PINS_UNUSED;
    }

    // Stop VPR.
    nrf_vpr_cpurun_set(NRF_VPR, false);

    // Reset VPR.
    nrf_vpr_debugif_dmcontrol_mask_set(NRF_VPR,
                                       (VPR_DEBUGIF_DMCONTROL_NDMRESET_Active
                                        << VPR_DEBUGIF_DMCONTROL_NDMRESET_Pos |
                                        VPR_DEBUGIF_DMCONTROL_DMACTIVE_Enabled
                                        << VPR_DEBUGIF_DMCONTROL_DMACTIVE_Pos));
    nrf_vpr_debugif_dmcontrol_mask_set(NRF_VPR,
                                       (VPR_DEBUGIF_DMCONTROL_NDMRESET_Inactive
                                        << VPR_DEBUGIF_DMCONTROL_NDMRESET_Pos |
                                        VPR_DEBUGIF_DMCONTROL_DMACTIVE_Disabled
                                        << VPR_DEBUGIF_DMCONTROL_DMACTIVE_Pos));

    p_cb->state = NRFX_DRV_STATE_UNINITIALIZED;
}

nrfx_err_t nrf_sqspi_dev_cfg(const nrf_sqspi_t *         p_qspi,
                             const nrf_sqspi_dev_cfg_t * p_config,
                             nrf_sqspi_callback_t        callback,
                             void *                      p_context)
{
    qspi2_control_block_t * p_cb = &m_cb[p_qspi->drv_inst_idx];

    if (p_cb->state == NRFX_DRV_STATE_UNINITIALIZED)
    {
        return NRFX_ERROR_INVALID_STATE;
    }

    p_cb->handler           = callback;
    p_cb->p_context         = p_context;
    p_cb->qspi.p_reg        = p_qspi->p_reg;
    p_cb->qspi.drv_inst_idx = p_qspi->drv_inst_idx;

    nrf_qspi2_core_ctrlr0_t    default_ctrlr0    = NRF_QSPI2_CORE_CTRLR0_DEFAULT_CONF;
    nrf_qspi2_core_spictrlr0_t default_spictrlr0 = NRF_QSPI2_CORE_SPICTRLR0_DEFAULT_CONF;

    p_cb->conf.ctrlr0    = default_ctrlr0;
    p_cb->conf.spictrlr0 = default_spictrlr0;

    uint32_t clkdiv = 0;
    if (p_config->sck_freq_khz > 0)
    {
        clkdiv = SP_VPR_BASE_FREQ_HZ / (p_config->sck_freq_khz * 1000);
    }

    nrf_qspi2_core_baudr(p_qspi->p_reg, (uint16_t)(clkdiv));

    if (p_cb->skip_gpio_cfg == false)
    {
        if (p_config->csn_pin != NRF_SQSPI_PINS_UNUSED)
        {
            p_cb->pins.ss = p_config->csn_pin;
#if defined(NRF54L_SERIES) || defined(NRF54H20_XXAA)
            nrf_gpio_cfg(p_config->csn_pin, NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT,
                         NRF_GPIO_PIN_NOPULL,
                         NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);
#endif
        }
        else
        {
            return NRFX_ERROR_INVALID_PARAM;
        }
    }

    if (p_cb->skip_pmux_cfg == false)
    {
        if (p_cb->pins.ss != NRF_SQSPI_PINS_UNUSED)
        {
#if defined(NRF54L_SERIES)
            nrf_gpio_pin_control_select(p_config->csn_pin, NRF_GPIO_PIN_SEL_VPR);
#elif defined(NRF54H20_XXAA)
            // CTRLSEL setting of GPIO must be done by SecDom through UICR configuration.
#else
#pragma warning "Unknown platform"
#endif
        }
        else
        {
            return NRFX_ERROR_INVALID_PARAM;
        }
    }

    if (p_config->protocol == NRF_SQSPI_PROTO_SPI_C)
    {
        // Set Frameformat SPI.
        p_cb->conf.ctrlr0.frf = QSPI_CORE_CORE_CTRLR0_FRF_SPI;

        // Set controller.
        p_cb->conf.ctrlr0.sqspiismst = QSPI_CORE_CORE_CTRLR0_SQSPIISMST_CONTROLLER;
    }
    else
    {
        return NRFX_ERROR_NOT_SUPPORTED;
    }

    if (p_config->sample_sync == NRF_SQSPI_SAMPLE_SYNC_DELAY)
    {
        nrf_qspi2_core_rx_sample_delay(p_qspi->p_reg, (uint8_t)p_config->sample_delay_cyc);
    }
    if (p_config->spi_cpolpha == NRF_SQSPI_SPI_CPOLPHA_0)
    {
        p_cb->conf.ctrlr0.scph  = QSPI_CORE_CORE_CTRLR0_SCPH_MIDDLEBIT;
        p_cb->conf.ctrlr0.scpol = QSPI_CORE_CORE_CTRLR0_SCPOL_INACTIVEHIGH;
    }

    if (p_config->mspi_lines == NRF_SQSPI_SPI_LINES_SINGLE)
    {
        p_cb->conf.ctrlr0.spifrf       = QSPI_CORE_CORE_CTRLR0_SPIFRF_SPISTANDARD;
        p_cb->conf.spictrlr0.transtype =
            QSPI_CORE_CORE_SPICTRLR0_TRANSTYPE_TT0;                              // single inst, single addr, single data.
    }
    else if (p_config->mspi_lines == NRF_SQSPI_SPI_LINES_DUAL_1_1_2)
    {
        p_cb->conf.ctrlr0.spifrf       = QSPI_CORE_CORE_CTRLR0_SPIFRF_SPIDUAL;
        p_cb->conf.spictrlr0.transtype = QSPI_CORE_CORE_SPICTRLR0_TRANSTYPE_TT0; // single inst, single addr, dual data.
    }
    else if (p_config->mspi_lines == NRF_SQSPI_SPI_LINES_DUAL_1_2_2)
    {
        p_cb->conf.ctrlr0.spifrf       = QSPI_CORE_CORE_CTRLR0_SPIFRF_SPIDUAL;
        p_cb->conf.spictrlr0.transtype = QSPI_CORE_CORE_SPICTRLR0_TRANSTYPE_TT1; // single inst, double addr, dual data.
    }
    else if (p_config->mspi_lines == NRF_SQSPI_SPI_LINES_QUAD_1_1_4)
    {
        p_cb->conf.ctrlr0.spifrf       = QSPI_CORE_CORE_CTRLR0_SPIFRF_SPIQUAD;
        p_cb->conf.spictrlr0.transtype = QSPI_CORE_CORE_SPICTRLR0_TRANSTYPE_TT0; // single inst, single addr, quad data.
    }
    else if (p_config->mspi_lines == NRF_SQSPI_SPI_LINES_QUAD_1_4_4)
    {
        p_cb->conf.ctrlr0.spifrf       = QSPI_CORE_CORE_CTRLR0_SPIFRF_SPIQUAD;
        p_cb->conf.spictrlr0.transtype = QSPI_CORE_CORE_SPICTRLR0_TRANSTYPE_TT1; // single inst, quad addr, quad data.
    }
    else
    {
        return NRFX_ERROR_INVALID_PARAM;
    }

    return NRFX_SUCCESS;
}

nrfx_err_t nrf_sqspi_dev_data_fmt_set(const nrf_sqspi_t *    p_qspi,
                                      nrf_sqspi_data_fmt_t * p_data_fmt)
{
    if (p_data_fmt->addr_bit_order != p_data_fmt->cmd_bit_order)
    {
        return NRFX_ERROR_INVALID_PARAM;
    }
    if (p_data_fmt->data_bit_reorder_unit != p_data_fmt->data_swap_unit)
    {
        return NRFX_ERROR_INVALID_PARAM;
    }
    if (p_data_fmt->data_padding != 0)
    {
        if (p_data_fmt->data_container + p_data_fmt->data_padding != 32)
        {
            return NRFX_ERROR_INVALID_PARAM;
        }
    }

    qspi2_control_block_t * p_cb = &m_cb[p_qspi->drv_inst_idx];
    p_cb->conf.format.padding  = p_data_fmt->data_padding;
    p_cb->conf.format.bitorder = ((uint32_t)(p_data_fmt->cmd_bit_order) & 1U) |
                                 (((uint32_t)(p_data_fmt->data_bit_order) & 1U) << 1U);

    p_cb->conf.format.dfs = (uint8_t)((p_data_fmt->data_container - 1) & 0x1F);

    p_cb->conf.format.bpp = p_data_fmt->data_swap_unit;
    nrf_qspi2_format_bitorder(p_qspi->p_reg, p_data_fmt->cmd_bit_order, p_data_fmt->data_bit_order);
    //NOTE: data_bit_reorder_unit is ignored
    nrf_qspi2_format_dfs(p_qspi->p_reg, p_data_fmt->data_container - 1);
    nrf_qspi2_core_dr_x(p_qspi->p_reg, (uint32_t)(32 - p_data_fmt->data_padding), 22); //Hijacking DR[22] as red_dfs
    nrf_qspi2_format_bpp(p_qspi->p_reg, p_data_fmt->data_swap_unit);

    p_cb->conf.ctrlr0.dfs = (uint8_t)((p_data_fmt->data_container - 1) & 0x1F);
    return NRFX_SUCCESS;
}

nrfx_err_t nrf_sqspi_activate(const nrf_sqspi_t * p_qspi)
{
    qspi2_control_block_t * p_cb = &m_cb[p_qspi->drv_inst_idx];

    if (p_cb->state != NRFX_DRV_STATE_INITIALIZED)
    {
        return NRFX_ERROR_INVALID_STATE;
    }

    nrf_qspi2_enable(p_qspi->p_reg);

    __ASB(p_qspi->p_reg);

    nrf_qspi2_event_clear(p_qspi->p_reg, NRF_QSPI2_EVENT_DMA_DONE);
    nrf_qspi2_event_clear(p_qspi->p_reg, NRF_QSPI2_EVENT_DMA_ABORTED);
    nrf_qspi2_event_clear(p_qspi->p_reg, NRF_QSPI2_EVENT_DMA_DONEJOB);

    NRFX_IRQ_ENABLE(SP_VPR_IRQn);

    p_cb->state = NRFX_DRV_STATE_POWERED_ON;

    return NRFX_SUCCESS;
}

nrfx_err_t nrf_sqspi_deactivate(const nrf_sqspi_t * p_qspi)
{
    qspi2_control_block_t * p_cb = &m_cb[p_qspi->drv_inst_idx];

    if (p_cb->state != NRFX_DRV_STATE_POWERED_ON)
    {
        return NRFX_ERROR_INVALID_STATE;
    }

    p_cb->prepared_pending = false;

    if (p_cb->transfer_in_progress)
    {
        __SSB(p_qspi->p_reg);
    }

    while (p_cb->transfer_in_progress)
    {}
    ;

    NRFX_IRQ_DISABLE(SP_VPR_IRQn);

    nrf_qspi2_disable(p_qspi->p_reg);

    __ASB(p_qspi->p_reg);

    p_cb->state = NRFX_DRV_STATE_INITIALIZED;

    return NRFX_SUCCESS;
}

static nrfx_err_t xfer_common(qspi2_control_block_t *  p_cb,
                              const nrf_sqspi_t *      p_qspi,
                              nrf_sqspi_xfer_t const * p_xfer,
                              size_t                   xfer_count)
{
    if (xfer_count > NRF_SQSPI_TRANSFERS_PER_REQUEST)
    {
        return NRFX_ERROR_NOT_SUPPORTED;
    }

    if (p_cb->state != NRFX_DRV_STATE_POWERED_ON)
    {
        return NRFX_ERROR_INVALID_STATE;
    }

    // Set addrl
    if ((p_xfer->addr_length % 4 == 0) && (p_xfer->addr_length <= 60))
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
        p_cb->conf.spictrlr0.addrl = ((p_xfer->addr_length / 4) & 0xF); // Max 4 bits.
#pragma GCC diagnostic pop
    }
    else
    {
        return NRFX_ERROR_INVALID_PARAM;
    }

    // Set instl
    if ((p_xfer->cmd_length % 8 == 0) && (p_xfer->cmd_length <= 16))
    {
        switch (p_xfer->cmd_length)
        {
            case 4: p_cb->conf.spictrlr0.instl  = 1; break;
            case 8: p_cb->conf.spictrlr0.instl  = 2; break;
            case 16: p_cb->conf.spictrlr0.instl = 3; break;
            default: p_cb->conf.spictrlr0.instl = 0; break;
        }
    }
    else
    {
        return NRFX_ERROR_INVALID_PARAM;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
    p_cb->conf.spictrlr0.waitcycles = (p_xfer->dummy_length & 0x1F); // Max 5 bits.
#pragma GCC diagnostic pop
    uint32_t pixels         = 0;
    uint16_t ndf            = 0;
    uint8_t  command_length = 0;
    if (p_cb->conf.format.bpp != 0)
    {
        pixels = (8 * p_xfer->data_length) / p_cb->conf.format.bpp;
        ndf    = (uint16_t)((pixels * (p_cb->conf.format.dfs + 1) + p_cb->conf.format.bpp - 1) /
                            p_cb->conf.format.bpp);
    }
    else
    {
        pixels = (8 * p_xfer->data_length) /
                 ((p_cb->conf.format.dfs + 1) + p_cb->conf.format.padding);
        ndf = (uint16_t)pixels;
    }
    command_length          = (uint8_t)((p_xfer->cmd_length + 31) / 32);
    p_cb->conf.format.cilen = command_length;
    nrf_qspi2_format_pixels(p_qspi->p_reg, pixels);
    nrf_qspi2_core_ctrlr1_ndf(p_qspi->p_reg, ndf);
    nrf_qspi2_core_dr_x(p_qspi->p_reg, (uint32_t)(p_xfer->data_length), 23); //Hijacking DR[23] as red_data_nr_bytes
    nrf_qspi2_format_cilen(p_qspi->p_reg, command_length);

    p_cb->conf.ctrlr0.tmod = p_xfer->dir;

    nrf_qspi2_core_ctrlr0_set(p_qspi->p_reg, p_cb->conf.ctrlr0);
    nrf_qspi2_core_spictrlr0_set(p_qspi->p_reg, p_cb->conf.spictrlr0);

    static uint32_t m_inst_addr[3];
    m_inst_addr[0] = p_xfer->cmd;
    uint64_t * p_addr = (uint64_t *)&m_inst_addr[1];
    *p_addr = p_xfer->address;

    // Calculate total size of the inst_cmd dma write.
    uint8_t cmd_addr_size = 0;
    if (p_xfer->cmd_length > 0)
    {
        cmd_addr_size = 4;
    }

    if (p_xfer->addr_length > 0)
    {
        if (p_xfer->addr_length > 32)
        {
            cmd_addr_size += 8;
        }
        else
        {
            cmd_addr_size += 4;
        }
    }

    nrf_qspi2_core_dr_x(p_qspi->p_reg, p_xfer->cmd,                                        0);
    nrf_qspi2_core_dr_x(p_qspi->p_reg, ((uint32_t)p_xfer->address & 0xFFFFFFFF),           1);
    nrf_qspi2_core_dr_x(p_qspi->p_reg, ((uint32_t)((p_xfer->address >> 31) & 0xFFFFFFFF)), 2);

    nrf_qspi2_core_dr_x(p_qspi->p_reg, ((uint32_t)p_xfer->p_data),      3);
    nrf_qspi2_core_dr_x(p_qspi->p_reg, ((uint32_t)p_xfer->data_length), 4);

    __CSB(p_qspi->p_reg);

    // Cache transaction data.
    m_current_xfer.drv_inst_idx = p_qspi->drv_inst_idx;
    m_current_xfer.dir          = p_xfer->dir;

    if ((p_xfer->dir == NRF_SQSPI_XFER_DIR_RX) || (p_xfer->dir == NRF_SQSPI_XFER_DIR_TXRX))
    {
        m_current_xfer.p_dest   = p_xfer->p_data;
        m_current_xfer.dest_len = p_xfer->data_length;
    }

    return NRFX_SUCCESS;
}

nrfx_err_t nrf_sqspi_xfer(const nrf_sqspi_t *      p_qspi,
                          const nrf_sqspi_xfer_t * p_xfer,
                          size_t                   xfer_count,
                          uint32_t                 flags)
{
    if (flags == NRF_SQSPI_FLAG_HOLD_XFER)
    {
        return nrf_sqspi_xfer_prepare(p_qspi, p_xfer, xfer_count);
    }

    qspi2_control_block_t * p_cb = &m_cb[p_qspi->drv_inst_idx];

    if (p_cb->transfer_in_progress || p_cb->prepared_pending)
    {
        return NRFX_ERROR_BUSY;
    }

    nrfx_err_t retval;

    retval = xfer_common(p_cb, p_qspi, p_xfer, xfer_count);

    if (retval == NRFX_SUCCESS)
    {
        nrf_qspi2_core_enable(p_qspi->p_reg);
        __ASB(p_qspi->p_reg);

        p_cb->transfer_in_progress = true;

        nrf_vpr_task_trigger(NRF_VPR,
                             offsetof(NRF_VPR_Type, TASKS_TRIGGER[SP_VPR_TASK_DPPI_0_IDX]));
    }

    return retval;
}

nrfx_err_t nrf_sqspi_xfer_prepare(const nrf_sqspi_t *      p_qspi,
                                  nrf_sqspi_xfer_t const * p_xfer,
                                  size_t                   xfer_count)
{
    qspi2_control_block_t * p_cb = &m_cb[p_qspi->drv_inst_idx];

    if (p_cb->prepared_pending)
    {
        return NRFX_ERROR_BUSY;
    }

    nrfx_err_t retval;

    retval = xfer_common(p_cb, p_qspi, p_xfer, xfer_count);

    if (retval == NRFX_SUCCESS)
    {
        p_cb->prepared_pending = true;
        nrf_qspi2_core_enable(p_cb->qspi.p_reg);
        __ASB(p_cb->qspi.p_reg);
    }

    return retval;
}

void nrf_sqspi_irq_handler(void)
{
    if (nrf_vpr_event_check(NRF_VPR, offsetof(NRF_VPR_Type, EVENTS_TRIGGERED[SP_VPR_EVENT_IDX])))
    {
        nrf_vpr_event_clear(NRF_VPR, offsetof(NRF_VPR_Type, EVENTS_TRIGGERED[SP_VPR_EVENT_IDX]));

        qspi2_control_block_t * p_cb = &m_cb[m_current_xfer.drv_inst_idx];

        if (nrf_qspi2_event_check(p_cb->qspi.p_reg, NRF_QSPI2_EVENT_DMA_DONEJOB))
        {
            nrf_qspi2_event_clear(p_cb->qspi.p_reg, NRF_QSPI2_EVENT_DMA_DONEJOB);

            p_cb->evt.type           = NRF_SQSPI_EVT_XFER_STARTED;
            p_cb->evt.data.xfer_done = NRF_SQSPI_RESULT_OK;
            if ((p_cb->prepared_pending == true) && (p_cb->transfer_in_progress == false))
            {
                // Transition the pending into a transfer_in_progress to allow for a new xfer to be scheduled.
                p_cb->transfer_in_progress = true;
                p_cb->prepared_pending     = false;
            }

            // Indicate to the app that the prepared transaction has been triggered.
            p_cb->handler(&p_cb->qspi, &p_cb->evt, p_cb->p_context);
        }

        if (nrf_qspi2_event_check(p_cb->qspi.p_reg, NRF_QSPI2_EVENT_DMA_DONE))
        {
            nrf_qspi2_event_clear(p_cb->qspi.p_reg, NRF_QSPI2_EVENT_DMA_DONE);
            // End current transaction.
            nrf_qspi2_core_disable(p_cb->qspi.p_reg);
            __ASB(p_cb->qspi.p_reg);

            if ((p_cb->prepared_pending == true) && (p_cb->transfer_in_progress == true)) // There was an ongoing transfer and we have one on hold.
            {
                // Trigger the pending transaction.
                nrf_qspi2_core_enable(p_cb->qspi.p_reg);
                __ASB(p_cb->qspi.p_reg);
                // Continue transfer_in_progress, but clear the pending.
                p_cb->prepared_pending = false;
            }
            else
            {
                p_cb->transfer_in_progress = false;
            }

            p_cb->evt.type           = NRF_SQSPI_EVT_XFER_DONE;
            p_cb->evt.data.xfer_done = NRF_SQSPI_RESULT_OK;

            p_cb->handler(&p_cb->qspi, &p_cb->evt, p_cb->p_context);
        }

        if (nrf_qspi2_event_check(p_cb->qspi.p_reg, NRF_QSPI2_EVENT_DMA_ABORTED))
        {
            nrf_qspi2_event_clear(p_cb->qspi.p_reg, NRF_QSPI2_EVENT_DMA_ABORTED);

            p_cb->transfer_in_progress = false;
            p_cb->prepared_pending     = false;

            p_cb->evt.type           = NRF_SQSPI_EVT_XFER_DONE;
            p_cb->evt.data.xfer_done = NRF_SQSPI_RESULT_ABORTED;

            nrf_qspi2_core_disable(p_cb->qspi.p_reg);
            __ASB(p_cb->qspi.p_reg);

            p_cb->handler(&p_cb->qspi, &p_cb->evt, p_cb->p_context);
        }
    }
}

uint32_t * nrf_sqspi_start_task_address_get(nrf_sqspi_t const * p_qspi)
{
    (void)p_qspi;
    return (uint32_t *)(nrf_vpr_task_address_get(NRF_VPR,
                                                 (nrf_vpr_task_t)offsetof(NRF_VPR_Type,
                                                                          TASKS_TRIGGER[
                                                                              SP_VPR_TASK_DPPI_0_IDX])));
}

#endif // NRFX_CHECK(NRF_SQSPI_ENABLED)
