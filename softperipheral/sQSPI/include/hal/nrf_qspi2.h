/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef NRF_QSPI2_H__
#define NRF_QSPI2_H__

#include <nrfx.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SOFTPERIPHERAL_QSPI (1)
#if SOFTPERIPHERAL_QSPI
#include <nrf_sp_qspi.h>
#define NRF_QSPI2_Type NRF_SP_QSPI_Type
#else
#define NRF_QSPI2_Type NRF_QSPI_Type
#endif

/**
 * @defgroup nrf_qspi2_hal QSPI2 HAL
 * @{
 * @brief   Hardware access layer for managing the QSPI2 peripheral.
 */

/** @brief QSPI tasks. */
typedef enum
{
    NRF_QSPI2_TASK_START = offsetof(NRF_QSPI2_Type, TASKS_START), /**< Activate the QSPI interface. */
    NRF_QSPI2_TASK_RESET = offsetof(NRF_QSPI2_Type,
                                    TASKS_RESET),                 /**< Start transfer from external flash memory to internal RAM. */
} nrf_qspi2_task_t;

/** @brief QSPI events. */
typedef enum
{
    NRF_QSPI2_EVENT_DMA_DONE    = offsetof(NRF_QSPI2_Type, EVENTS_DMA.DONE),
    NRF_QSPI2_EVENT_DMA_DONEJOB = offsetof(NRF_QSPI2_Type, EVENTS_DMA.EVENTS_DONE.JOB),
    NRF_QSPI2_EVENT_DMA_ABORTED = offsetof(NRF_QSPI2_Type, EVENTS_DMA.ABORTED),
} nrf_qspi2_event_t;

typedef union
{
    struct
    {
        uint32_t dfs        : 5; // RW
        uint32_t reserved0  : 1; // R
        uint32_t frf        : 2; // RW
        uint32_t scph       : 1; // RW
        uint32_t scpol      : 1; // RW
        uint32_t tmod       : 2; // RW
        uint32_t slvoe      : 1; // RW
        uint32_t srl        : 1; // RW
        uint32_t sste       : 1; // RW
        uint32_t reserved1  : 1; // R
        uint32_t cfs        : 4; // RW
        uint32_t reserved2  : 2; // R
        uint32_t spifrf     : 2; // RW
        uint32_t hyperbusen : 1; // RW
        uint32_t spidwsen   : 1; // R
        uint32_t clkloopen  : 1; // R
        uint32_t reserved3  : 4; // R
        uint32_t sqspiismst : 1; // R
    };

    uint32_t raw;
} nrf_qspi2_core_ctrlr0_t;

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

typedef union
{
    struct
    {
        uint32_t transtype     : 2; // RW
        uint32_t addrl         : 4; // RW
        uint32_t reserved0     : 1; // R
        uint32_t xipmdbiten    : 1; // R
        uint32_t instl         : 2; // RW
        uint32_t reserved1     : 1; // R
        uint32_t waitcycles    : 5; // RW
        uint32_t spiddren      : 1; // R
        uint32_t instddren     : 1; // R
        uint32_t spirxdsen     : 1; // R
        uint32_t xipdfshc      : 1; // R
        uint32_t xipinsten     : 1; // R
        uint32_t xipcontxferen : 1; // R
        uint32_t reserved2     : 1; // R
        uint32_t rxdsvlen      : 1; // R
        uint32_t spidmen       : 1; // R
        uint32_t spirxdssigen  : 1; // R
        uint32_t xipmbl        : 2; // R
        uint32_t reserved3     : 1; // R
        uint32_t xipprefetchen : 1; // R
        uint32_t clkstretchen  : 1; // RW
        uint32_t reserved4     : 1; // R
    };

    uint32_t raw;
} nrf_qspi2_core_spictrlr0_t;

typedef struct
{
    uint32_t dfs;
    uint32_t bpp;
    uint32_t pixels;
    uint32_t cilen;
    uint32_t bitorder;
    uint32_t padding;
} nrf_qspi2_format_t;

NRF_STATIC_INLINE void nrf_qspi2_task_trigger(NRF_QSPI2_Type * p_reg, nrf_qspi2_task_t task);

NRF_STATIC_INLINE void nrf_qspi2_enable(NRF_QSPI2_Type * p_reg);

NRF_STATIC_INLINE void nrf_qspi2_disable(NRF_QSPI2_Type * p_reg);

NRF_STATIC_INLINE bool nrf_qspi2_enable_check(NRF_QSPI2_Type const * p_reg);

NRF_STATIC_INLINE void nrf_qspi2_event_clear(NRF_QSPI2_Type * p_reg, nrf_qspi2_event_t event);

NRF_STATIC_INLINE bool nrf_qspi2_event_check(NRF_QSPI2_Type const * p_reg, nrf_qspi2_event_t event);

NRF_STATIC_INLINE uint32_t nrf_qspi2_event_address_get(NRF_QSPI2_Type const * p_reg,
                                                       nrf_qspi2_event_t      event);

NRF_STATIC_INLINE void nrf_qspi2_core_enable(NRF_QSPI2_Type * p_reg);

NRF_STATIC_INLINE void nrf_qspi2_core_disable(NRF_QSPI2_Type * p_reg);

NRF_STATIC_INLINE void nrf_qspi2_core_dr_0(NRF_QSPI2_Type * p_reg, uint32_t val);

NRF_STATIC_INLINE void nrf_qspi2_core_dr_x(NRF_QSPI2_Type * p_reg, uint32_t val, uint8_t idx);

NRF_STATIC_INLINE uint32_t nrf_qspi2_core_dr_x_get(NRF_QSPI2_Type * p_reg, uint8_t idx);

NRF_STATIC_INLINE void nrf_qspi2_core_baudr(NRF_QSPI2_Type * p_reg, uint16_t sckdiv);

NRF_STATIC_INLINE void nrf_qspi2_core_ctrlr1_ndf(NRF_QSPI2_Type * p_reg, uint16_t ndf);

NRF_STATIC_INLINE void nrf_qspi2_core_rx_sample_delay(NRF_QSPI2_Type * p_reg, uint8_t sclk);

NRF_STATIC_INLINE void nrf_qspi2_int_enable(NRF_QSPI2_Type * p_reg, uint32_t mask);

NRF_STATIC_INLINE uint32_t nrf_qspi2_int_enable_check(NRF_QSPI2_Type const * p_reg, uint32_t mask);

NRF_STATIC_INLINE void nrf_qspi2_int_disable(NRF_QSPI2_Type * p_reg, uint32_t mask);

NRF_STATIC_INLINE void nrf_qspi2_format_bpp(NRF_QSPI2_Type * p_reg, uint8_t val);

NRF_STATIC_INLINE void nrf_qspi2_format_dfs(NRF_QSPI2_Type * p_reg, uint8_t val);

NRF_STATIC_INLINE void nrf_qspi2_format_cilen(NRF_QSPI2_Type * p_reg, uint8_t val);

NRF_STATIC_INLINE void nrf_qspi2_format_bitorder(NRF_QSPI2_Type * p_reg, int cmd_val, int data_val);

NRF_STATIC_INLINE void nrf_qspi2_format_pixels(NRF_QSPI2_Type * p_reg, uint32_t val);

NRF_STATIC_INLINE void nrf_qspi2_core_ctrlr0_set(NRF_QSPI2_Type *        p_reg,
                                                 nrf_qspi2_core_ctrlr0_t conf);

NRF_STATIC_INLINE void nrf_qspi2_core_spictrlr0_set(NRF_QSPI2_Type *           p_reg,
                                                    nrf_qspi2_core_spictrlr0_t conf);

#ifndef NRF_DECLARE_ONLY

NRF_STATIC_INLINE void nrf_qspi2_task_trigger(NRF_QSPI2_Type * p_reg, nrf_qspi2_task_t task)
{
    *((volatile uint32_t *)((uint8_t *)p_reg + (uint32_t)task)) = 0x1UL;
}

NRF_STATIC_INLINE void nrf_qspi2_enable(NRF_QSPI2_Type * p_reg) { p_reg->ENABLE = 1; }

NRF_STATIC_INLINE void nrf_qspi2_disable(NRF_QSPI2_Type * p_reg) { p_reg->ENABLE = 0; }

NRF_STATIC_INLINE bool nrf_qspi2_enable_check(NRF_QSPI2_Type const * p_reg)
{
    return (bool)p_reg->ENABLE;
}

NRF_STATIC_INLINE void nrf_qspi2_event_clear(NRF_QSPI2_Type * p_reg, nrf_qspi2_event_t event)
{
    *((volatile uint32_t *)((uint8_t *)p_reg + (uint32_t)event)) = 0x0UL;
    nrf_event_readback((uint8_t *)p_reg + (uint32_t)event);
}

NRF_STATIC_INLINE bool nrf_qspi2_event_check(NRF_QSPI2_Type const * p_reg, nrf_qspi2_event_t event)
{
    return nrf_event_check(p_reg, event);
}

NRF_STATIC_INLINE uint32_t nrf_qspi2_event_address_get(NRF_QSPI2_Type const * p_reg,
                                                       nrf_qspi2_event_t      event)
{
    return nrf_task_event_address_get(p_reg, event);
}

NRF_STATIC_INLINE void nrf_qspi2_core_enable(NRF_QSPI2_Type * p_reg)
{
    p_reg->CORE.CORE.SQSPIENR = ((QSPI_CORE_CORE_SQSPIENR_SQSPICEN_ENABLED
                                  << QSPI_CORE_CORE_SQSPIENR_SQSPICEN_Pos) &
                                 QSPI_CORE_CORE_SQSPIENR_SQSPICEN_Msk);
}

NRF_STATIC_INLINE void nrf_qspi2_core_disable(NRF_QSPI2_Type * p_reg)
{
    p_reg->CORE.CORE.SQSPIENR = ((QSPI_CORE_CORE_SQSPIENR_SQSPICEN_DISABLE
                                  << QSPI_CORE_CORE_SQSPIENR_SQSPICEN_Pos) &
                                 QSPI_CORE_CORE_SQSPIENR_SQSPICEN_Msk);
}

NRF_STATIC_INLINE void nrf_qspi2_core_dr_0(NRF_QSPI2_Type * p_reg, uint32_t val)
{
    p_reg->CORE.CORE.DR[0] = val;
}

NRF_STATIC_INLINE void nrf_qspi2_core_dr_x(NRF_QSPI2_Type * p_reg, uint32_t val, uint8_t idx)
{
    p_reg->CORE.CORE.DR[idx] = val;
}

NRF_STATIC_INLINE uint32_t nrf_qspi2_core_dr_x_get(NRF_QSPI2_Type * p_reg, uint8_t idx)
{
    return p_reg->CORE.CORE.DR[idx];
}

NRF_STATIC_INLINE void nrf_qspi2_core_baudr(NRF_QSPI2_Type * p_reg, uint16_t sckdiv)
{
    p_reg->CORE.CORE.BAUDR =
        /* Reserved bit 0 */ ((0
                               << QSPI_CORE_CORE_BAUDR_RSVDBAUDR0_Pos) &
                              QSPI_CORE_CORE_BAUDR_RSVDBAUDR0_Msk)
        /* SCKDV - SQSPI Clock Divider */
        | ((sckdiv << QSPI_CORE_CORE_BAUDR_SCKDV_Pos) & QSPI_CORE_CORE_BAUDR_SCKDV_Msk)
        /* Reserved */
        | ((0 << QSPI_CORE_CORE_BAUDR_RSVDBAUDR1631_Pos) & QSPI_CORE_CORE_BAUDR_RSVDBAUDR1631_Msk);
}

NRF_STATIC_INLINE void nrf_qspi2_core_ctrlr1_ndf(NRF_QSPI2_Type * p_reg, uint16_t ndf)
{
    p_reg->CORE.CORE.CTRLR1 =
        /* NDF - Number of Data Frames */ ((ndf
                                            << QSPI_CORE_CORE_CTRLR1_NDF_Pos) &
                                           QSPI_CORE_CORE_CTRLR1_NDF_Msk)
        /* Reserved */
        | ((0 << QSPI_CORE_CORE_CTRLR1_RSVDCTRLR1_Pos) & QSPI_CORE_CORE_CTRLR1_RSVDCTRLR1_Msk);
}

NRF_STATIC_INLINE void nrf_qspi2_core_rx_sample_delay(NRF_QSPI2_Type * p_reg, uint8_t sclk)
{
    p_reg->CORE.CORE.RXSAMPLEDELAY = sclk;
}

NRF_STATIC_INLINE void nrf_qspi2_int_enable(NRF_QSPI2_Type * p_reg, uint32_t mask)
{
    p_reg->INTEN = p_reg->INTEN | mask;
}

NRF_STATIC_INLINE uint32_t nrf_qspi2_int_enable_check(NRF_QSPI2_Type const * p_reg, uint32_t mask)
{
    return p_reg->INTENSET & mask;
}

NRF_STATIC_INLINE void nrf_qspi2_int_disable(NRF_QSPI2_Type * p_reg, uint32_t mask)
{
    p_reg->INTENCLR = mask;
}

NRF_STATIC_INLINE void nrf_qspi2_format_bpp(NRF_QSPI2_Type * p_reg, uint8_t val)
{
    p_reg->FORMAT.BPP = (uint32_t)val;
}

NRF_STATIC_INLINE void nrf_qspi2_format_dfs(NRF_QSPI2_Type * p_reg, uint8_t val)
{
    p_reg->FORMAT.DFS = (uint32_t)val;
}

NRF_STATIC_INLINE void nrf_qspi2_format_cilen(NRF_QSPI2_Type * p_reg, uint8_t val)
{
    p_reg->FORMAT.CILEN = (uint32_t)val;
}

NRF_STATIC_INLINE void nrf_qspi2_format_bitorder(NRF_QSPI2_Type * p_reg, int cmd_val, int data_val)
{
    p_reg->FORMAT.BITORDER = (uint32_t)(cmd_val << QSPI_FORMAT_BITORDER_COMMAND_Pos | data_val
                                        << QSPI_FORMAT_BITORDER_DATA_Pos);
}

NRF_STATIC_INLINE void nrf_qspi2_format_pixels(NRF_QSPI2_Type * p_reg, uint32_t val)
{
    p_reg->FORMAT.PIXELS = val;
}

NRF_STATIC_INLINE void nrf_qspi2_core_ctrlr0_set(NRF_QSPI2_Type *        p_reg,
                                                 nrf_qspi2_core_ctrlr0_t conf)
{
    p_reg->CORE.CORE.CTRLR0 =
        /* DFS - Data Frame Size*/ ((conf.dfs
                                     << QSPI_CORE_CORE_CTRLR0_DFS_Pos) &
                                    QSPI_CORE_CORE_CTRLR0_DFS_Msk)
        /* Reserved */
        | ((0 << QSPI_CORE_CORE_CTRLR0_RSVDCTRLR05_Pos) & QSPI_CORE_CORE_CTRLR0_RSVDCTRLR05_Msk)
        /* FRF - Frame Format */
        | ((conf.frf << QSPI_CORE_CORE_CTRLR0_FRF_Pos) & QSPI_CORE_CORE_CTRLR0_FRF_Msk)
        /* SCPH */
        | ((conf.scph << QSPI_CORE_CORE_CTRLR0_SCPH_Pos) & QSPI_CORE_CORE_CTRLR0_SCPH_Msk)
        /* SCPOL */
        | ((conf.scpol << QSPI_CORE_CORE_CTRLR0_SCPOL_Pos) & QSPI_CORE_CORE_CTRLR0_SCPOL_Msk)
        /* TMOD - Transfer Mode */
        | ((conf.tmod << QSPI_CORE_CORE_CTRLR0_TMOD_Pos) & QSPI_CORE_CORE_CTRLR0_TMOD_Msk)
        /* SLVOE - Slave Output Enable */
        | ((conf.slvoe << QSPI_CORE_CORE_CTRLR0_SLVOE_Pos) & QSPI_CORE_CORE_CTRLR0_SLVOE_Msk)
        /* SRL - Shift Register Loop */
        | ((conf.srl << QSPI_CORE_CORE_CTRLR0_SRL_Pos) & QSPI_CORE_CORE_CTRLR0_SRL_Msk)
        /* SSTE - Slave Select Toggle Enable */
        | ((conf.sste << QSPI_CORE_CORE_CTRLR0_SSTE_Pos) & QSPI_CORE_CORE_CTRLR0_SSTE_Msk)
        /* Reserved */
        | ((0 << QSPI_CORE_CORE_CTRLR0_RSVDCTRLR015_Pos) & QSPI_CORE_CORE_CTRLR0_RSVDCTRLR015_Msk)
        /* CFS - Control Frame Size */
        | ((conf.cfs << QSPI_CORE_CORE_CTRLR0_CFS_Pos) & QSPI_CORE_CORE_CTRLR0_CFS_Msk)
        /* Reserved */
        | ((0
            << QSPI_CORE_CORE_CTRLR0_RSVDCTRLR02021_Pos) & QSPI_CORE_CORE_CTRLR0_RSVDCTRLR02021_Msk)
        /* SPIFRF - SPI Frame Format */
        | ((conf.spifrf << QSPI_CORE_CORE_CTRLR0_SPIFRF_Pos) & QSPI_CORE_CORE_CTRLR0_SPIFRF_Msk)
        /* SPIHYPERBUSEN */
        | ((conf.hyperbusen
            << QSPI_CORE_CORE_CTRLR0_SPIHYPERBUSEN_Pos) & QSPI_CORE_CORE_CTRLR0_SPIHYPERBUSEN_Msk)
        /* SPIDWSEN - Dynamic wait state SPI */
        | ((conf.spidwsen
            << QSPI_CORE_CORE_CTRLR0_SPIDWSEN_Pos) & QSPI_CORE_CORE_CTRLR0_SPIDWSEN_Msk)
        /* CLKLOOPEN - Clock loop back enable */
        | ((conf.clkloopen
            << QSPI_CORE_CORE_CTRLR0_CLKLOOPEN_Pos) & QSPI_CORE_CORE_CTRLR0_CLKLOOPEN_Msk)
        /* Reserved */
        | ((0
            << QSPI_CORE_CORE_CTRLR0_RSVDCTRLR02730_Pos) & QSPI_CORE_CORE_CTRLR0_RSVDCTRLR02730_Msk)
        /* SQSPIISMST - Master or Slave */
        | ((conf.sqspiismst
            << QSPI_CORE_CORE_CTRLR0_SQSPIISMST_Pos) & QSPI_CORE_CORE_CTRLR0_SQSPIISMST_Msk);
}

NRF_STATIC_INLINE void nrf_qspi2_core_spictrlr0_set(NRF_QSPI2_Type *           p_reg,
                                                    nrf_qspi2_core_spictrlr0_t conf)
{
    p_reg->CORE.CORE.SPICTRLR0 =
        /* TRANSTYPE */ ((conf.transtype << QSPI_CORE_CORE_SPICTRLR0_TRANSTYPE_Pos) &
                         QSPI_CORE_CORE_SPICTRLR0_TRANSTYPE_Msk)
        /* ADDRL */
        | ((conf.addrl << QSPI_CORE_CORE_SPICTRLR0_ADDRL_Pos) & QSPI_CORE_CORE_SPICTRLR0_ADDRL_Msk)
        /* Reserved */
        | ((0
            << QSPI_CORE_CORE_SPICTRLR0_RSVDSPICTRLR06_Pos) &
           QSPI_CORE_CORE_SPICTRLR0_RSVDSPICTRLR06_Msk)
        /* XIPMDBITEN */
        | ((0 << QSPI_CORE_CORE_SPICTRLR0_XIPMDBITEN_Pos) & QSPI_CORE_CORE_SPICTRLR0_XIPMDBITEN_Msk)
        /* INSTL */
        | ((conf.instl << QSPI_CORE_CORE_SPICTRLR0_INSTL_Pos) & QSPI_CORE_CORE_SPICTRLR0_INSTL_Msk)
        /* Reserved */
        | ((0
            << QSPI_CORE_CORE_SPICTRLR0_RSVDSPICTRLR010_Pos) &
           QSPI_CORE_CORE_SPICTRLR0_RSVDSPICTRLR010_Msk)
        /* WAITCYCLES */
        | ((conf.waitcycles
            << QSPI_CORE_CORE_SPICTRLR0_WAITCYCLES_Pos) & QSPI_CORE_CORE_SPICTRLR0_WAITCYCLES_Msk)
        /* SPIDDREN - Dual data rate enable */
        | ((0 << QSPI_CORE_CORE_SPICTRLR0_SPIDDREN_Pos) & QSPI_CORE_CORE_SPICTRLR0_SPIDDREN_Msk)
        /* INSTDDREN */
        | ((0 << QSPI_CORE_CORE_SPICTRLR0_INSTDDREN_Pos) & QSPI_CORE_CORE_SPICTRLR0_INSTDDREN_Msk)
        /* SPIRXDSEN - Read data strobe */
        | ((0 << QSPI_CORE_CORE_SPICTRLR0_SPIRXDSEN_Pos) & QSPI_CORE_CORE_SPICTRLR0_SPIRXDSEN_Msk)
        /* XIPDFSHC */
        | ((0 << QSPI_CORE_CORE_SPICTRLR0_XIPDFSHC_Pos) & QSPI_CORE_CORE_SPICTRLR0_XIPDFSHC_Msk)
        /* XIPINSTEN */
        | ((0 << QSPI_CORE_CORE_SPICTRLR0_XIPINSTEN_Pos) & QSPI_CORE_CORE_SPICTRLR0_XIPINSTEN_Msk)
        /* SQSPICXIPCONTXFEREN */
        | ((0
            << QSPI_CORE_CORE_SPICTRLR0_SQSPICXIPCONTXFEREN_Pos) &
           QSPI_CORE_CORE_SPICTRLR0_SQSPICXIPCONTXFEREN_Msk)
        /* Reserved */
        | ((0
            << QSPI_CORE_CORE_SPICTRLR0_RSVDSPICTRLR022_Pos) &
           QSPI_CORE_CORE_SPICTRLR0_RSVDSPICTRLR022_Msk)
        /* RXDSVLEN - RXDS variable latncy en */
        | ((0 << QSPI_CORE_CORE_SPICTRLR0_RXDSVLEN_Pos) & QSPI_CORE_CORE_SPICTRLR0_RXDSVLEN_Msk)
        /* SPIDMEN - SPI data mask bit */
        | ((0 << QSPI_CORE_CORE_SPICTRLR0_SPIDMEN_Pos) & QSPI_CORE_CORE_SPICTRLR0_SPIDMEN_Msk)
        /* SPIRXDSSIGEN (hyperbus) */
        | ((0
            << QSPI_CORE_CORE_SPICTRLR0_SPIRXDSSIGEN_Pos) &
           QSPI_CORE_CORE_SPICTRLR0_SPIRXDSSIGEN_Msk)
        /* XIPMBL - XIP Mode bits length */
        | ((0 << QSPI_CORE_CORE_SPICTRLR0_XIPMBL_Pos) & QSPI_CORE_CORE_SPICTRLR0_XIPMBL_Msk)
        /* Reserved */
        | ((0
            << QSPI_CORE_CORE_SPICTRLR0_RSVDSPICTRLR028_Pos) &
           QSPI_CORE_CORE_SPICTRLR0_RSVDSPICTRLR028_Msk)
        /* XIPPREFETCHEN */
        | ((0
            << QSPI_CORE_CORE_SPICTRLR0_XIPPREFETCHEN_Pos) &
           QSPI_CORE_CORE_SPICTRLR0_XIPPREFETCHEN_Msk)
        /* CLKSTRETCHEN */
        | ((conf.clkstretchen
            << QSPI_CORE_CORE_SPICTRLR0_CLKSTRETCHEN_Pos) &
           QSPI_CORE_CORE_SPICTRLR0_CLKSTRETCHEN_Msk)
        /* Reserved */
        | ((0
            << QSPI_CORE_CORE_SPICTRLR0_RSVDSPICTRLR0_Pos) &
           QSPI_CORE_CORE_SPICTRLR0_RSVDSPICTRLR0_Msk);
}

#endif // NRF_DECLARE_ONLY

/** @} */

#ifdef __cplusplus
}
#endif

#endif // NRF_UARTE_H__
