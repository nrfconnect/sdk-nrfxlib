/*
 * Copyright (c) 2025, Nordic Semiconductor ASA
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 * @brief Module that contains utilities for manipulating
 *        hardware registers.
 *
 * @details
 */

#ifndef NRF_802154_HW_UTILS_H_
#define NRF_802154_HW_UTILS_H_

#include <nrfx.h>

#ifdef __STATIC_INLINE__
#undef __STATIC_INLINE__
#endif /* __STATIC_INLINE__ */

#ifdef NRF_802154_HW_UTILS_DECLARE_ONLY
#define __STATIC_INLINE__
#else /* NRF_802154_HW_UTILS_DECLARE_ONLY */
#define __STATIC_INLINE__          __STATIC_INLINE
#endif /* NRF_802154_HW_UTILS_DECLARE_ONLY */

#define RADIO_BASE                 ((uintptr_t)NRF_RADIO)
#define FICR_BASE                  ((uintptr_t)NRF_FICR)

#define FICR_REG_RESET_VALUE       0xFFFFFFFFUL
#define IS_FICR_REG_SET(_ficr_reg) ((_ficr_reg) != FICR_REG_RESET_VALUE)

/**
 * @brief Read peripheral register at a given offset.
 *
 * @param[in]  base_addr Base addres of a peripheral instance.
 * @param[in]  offset    Offset within the peripheral. Must be a multiple of four.
 *
 * @returns Value of the peripheral register.
 */
__STATIC_INLINE__ uint32_t nrf_802154_hw_offset_read(uintptr_t base_addr, uint32_t offset);

/**
 * @brief Write to peripheral register at a given offset.
 *
 * @param[in]  base_addr Base addres of a peripheral instance.
 * @param[in]  offset    Offset within the peripheral. Must be a multiple of four.
 * @param[in]  value     Value to write.
 */
__STATIC_INLINE__ void nrf_802154_hw_offset_write(uintptr_t base_addr,
                                                  uint32_t  offset,
                                                  uint32_t  value);

#ifndef RADIO_POWER_POWER_Msk
/**
 * @brief Reset radio PUBLISH, SUBSCRIBE, EVENTS and INTEN registers.
 */
__STATIC_INLINE__ void nrf_802154_hw_reset_radio_tasks_events(void);
#endif /* RADIO_POWER_POWER_Msk */

#if NRF53_ERRATA_158_ENABLE_WORKAROUND
/**
 * @brief This is a workaround for an issue reported in YOPAN-158.
 *
 * After RADIO peripheral reset with RADIO.POWER register the trim-values, loaded from FICR at
 * network core boot time by MDK, are lost. The trim-values are not preserved and re-applied by
 * hardware.
 *
 * Only selected trim-values are restored, those that apply to RADIO peripheral. The check
 * is done based on destination address.
 */
__STATIC_INLINE__ void yopan_158_workaround(void);
#endif /* NRF53_ERRATA_158_ENABLE_WORKAROUND */

#if NRF53_ERRATA_117_ENABLE_WORKAROUND
/**
 * @brief Applies ERRATA-117
 *
 * Shall be called after setting RADIO mode to NRF_RADIO_MODE_IEEE802154_250KBIT.
 */
__STATIC_INLINE__ void errata_117_apply(void);
#endif /* NRF53_ERRATA_117_ENABLE_WORKAROUND */

#if NRF54L_ERRATA_6_ENABLE_WORKAROUND
/**
 * @brief Applies MLTPAN-6
 *
 * Shall be called after setting RADIO mode to NRF_RADIO_MODE_IEEE802154_250KBIT.
 */
__STATIC_INLINE__ void mltpan_6_apply(void);
#endif /* NRF54L_ERRATA_6_ENABLE_WORKAROUND */

#if NRF52_CONFIGURATION_254_ENABLE
/**
 * @brief Applies DEVICE-CONFIG-254.
 *
 * Shall be called after every RADIO peripheral reset.
 */
__STATIC_INLINE__ void device_config_254_apply_tx(void);
#endif /* NRF52_CONFIGURATION_254_ENABLE */

#ifndef NRF_802154_HW_UTILS_DECLARE_ONLY

__STATIC_INLINE__ uint32_t nrf_802154_hw_offset_read(uintptr_t base_addr, uint32_t offset)
{
    return *(volatile uint32_t *)(base_addr + offset);
}

__STATIC_INLINE__ void nrf_802154_hw_offset_write(uintptr_t base_addr,
                                                  uint32_t  offset,
                                                  uint32_t  value)
{
    volatile uint32_t * p_register_address = (volatile uint32_t *)(base_addr + offset);

    *p_register_address = value;
}

#ifndef RADIO_POWER_POWER_Msk

__STATIC_INLINE__ void nrf_802154_hw_reset_radio_tasks_events(void)
{
    /* SUBSCRIBE registers */
    nrf_radio_subscribe_clear(NRF_RADIO, NRF_RADIO_TASK_TXEN);
    nrf_radio_subscribe_clear(NRF_RADIO, NRF_RADIO_TASK_RXEN);
    nrf_radio_subscribe_clear(NRF_RADIO, NRF_RADIO_TASK_START);
    nrf_radio_subscribe_clear(NRF_RADIO, NRF_RADIO_TASK_STOP);
    nrf_radio_subscribe_clear(NRF_RADIO, NRF_RADIO_TASK_DISABLE);
    nrf_radio_subscribe_clear(NRF_RADIO, NRF_RADIO_TASK_RSSISTART);
    nrf_radio_subscribe_clear(NRF_RADIO, NRF_RADIO_TASK_BCSTART);
    nrf_radio_subscribe_clear(NRF_RADIO, NRF_RADIO_TASK_BCSTOP);
    nrf_radio_subscribe_clear(NRF_RADIO, NRF_RADIO_TASK_EDSTART);
    nrf_radio_subscribe_clear(NRF_RADIO, NRF_RADIO_TASK_EDSTOP);
    nrf_radio_subscribe_clear(NRF_RADIO, NRF_RADIO_TASK_CCASTART);
    nrf_radio_subscribe_clear(NRF_RADIO, NRF_RADIO_TASK_CCASTOP);

    /* EVENT registers */
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_READY);
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_ADDRESS);
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_PAYLOAD);
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_END);
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_DISABLED);
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_DEVMATCH);
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_DEVMISS);
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_BCMATCH);
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_CRCOK);
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_CRCERROR);
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_FRAMESTART);
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_EDEND);
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_EDSTOPPED);
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_CCAIDLE);
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_CCABUSY);
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_CCASTOPPED);
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_RATEBOOST);
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_TXREADY);
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_RXREADY);
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_MHRMATCH);
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_PHYEND);
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_CTEPRESENT);

    /* PUBLISH registers */
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_READY);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_ADDRESS);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_PAYLOAD);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_END);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_DISABLED);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_DEVMATCH);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_DEVMISS);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_BCMATCH);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_CRCOK);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_CRCERROR);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_FRAMESTART);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_EDEND);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_EDSTOPPED);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_CCAIDLE);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_CCABUSY);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_CCASTOPPED);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_RATEBOOST);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_TXREADY);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_RXREADY);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_MHRMATCH);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_PHYEND);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_CTEPRESENT);

    /* INTEN registers */
    nrf_radio_int_disable(NRF_RADIO, 0xFFFFFFFFUL);
}

#endif /* RADIO_POWER_POWER_Msk */

#if NRF53_ERRATA_158_ENABLE_WORKAROUND

/** Implement the YOPAN-158 workaround. */
__STATIC_INLINE__ void yopan_158_workaround(void)
{
    const uint32_t radio_address_mask        = 0xFFFFF000UL;
    const uint32_t ficr_trim_registers_count = 32UL;

    /* Copy all the trimming values from FICR into the target addresses. Trim until one ADDR
       is not initialized. */
    for (uint32_t index = 0; index < ficr_trim_registers_count; index++)
    {
        if (((volatile uint32_t *)((volatile uintptr_t)NRF_FICR_NS->TRIMCNF[index].ADDR &
                                   (uintptr_t)radio_address_mask) == (uint32_t *)NRF_RADIO))
        {
            *((volatile uint32_t *)NRF_FICR_NS->TRIMCNF[index].ADDR) =
                NRF_FICR_NS->TRIMCNF[index].DATA;
        }
    }
}

#endif /* NRF53_ERRATA_158_ENABLE_WORKAROUND */

#if NRF53_ERRATA_117_ENABLE_WORKAROUND

__STATIC_INLINE__ void errata_117_apply(void)
{
#ifndef CONFIG_SOC_SERIES_BSIM_NRFXX

    /* Register at 0x01FF0084. */
    uint32_t ficr_reg = nrf_802154_hw_offset_read(FICR_BASE, 0x84UL);

    /* Register at 0x41008588. */
    nrf_802154_hw_offset_write(RADIO_BASE, 0x588UL, ficr_reg);

#endif /* CONFIG_SOC_SERIES_BSIM_NRFXX */
}

#endif /* NRF53_ERRATA_117_ENABLE_WORKAROUND */

#if NRF54L_ERRATA_6_ENABLE_WORKAROUND

__STATIC_INLINE__ void mltpan_6_apply(void)
{
#ifndef CONFIG_SOC_SERIES_BSIM_NRFXX

    /* Apply MLTPAN-6 */
    nrf_802154_hw_offset_write(RADIO_BASE, 0x810UL, 2);

#endif /* CONFIG_SOC_SERIES_BSIM_NRFXX */
}

#endif /* NRF54L_ERRATA_6_ENABLE_WORKAROUND */

#if NRF52_CONFIGURATION_254_ENABLE

/**
 * @brief Applies DEVICE-CONFIG-254.
 *
 * Shall be called after every RADIO peripheral reset.
 */
__STATIC_INLINE__ void device_config_254_apply_tx(void)
{
    /* Trim values for FTPAN-254 */
    uint32_t ficr_reg1 = nrf_802154_hw_offset_read(FICR_BASE, 0x0330UL);
    uint32_t ficr_reg2 = nrf_802154_hw_offset_read(FICR_BASE, 0x0334UL);
    uint32_t ficr_reg3 = nrf_802154_hw_offset_read(FICR_BASE, 0x0338UL);

    /* Check if the device is fixed by testing every FICR register's value separately. */
    if (IS_FICR_REG_SET(ficr_reg1))
    {
        nrf_802154_hw_offset_write(RADIO_BASE, 0x074cUL, ficr_reg1);
    }

    if (IS_FICR_REG_SET(ficr_reg2))
    {
        nrf_802154_hw_offset_write(RADIO_BASE, 0x0584UL, ficr_reg2);
    }

    if (IS_FICR_REG_SET(ficr_reg3))
    {
        nrf_802154_hw_offset_write(RADIO_BASE, 0x0588UL, ficr_reg3);
    }
}

#endif /* NRF52_CONFIGURATION_254_ENABLE */

#endif /* NRF_802154_HW_UTILS_DECLARE_ONLY */

#endif /* NRF_802154_HW_UTILS_H_ */
