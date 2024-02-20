/*
 * Copyright (c) 2020, Nordic Semiconductor ASA
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
 * @file Module that defines the 802.15.4 driver peripheral usage.
 *
 */

#ifndef NRF_802154_SL_PERIPHS_H__
#define NRF_802154_SL_PERIPHS_H__

#include <nrfx.h>

#if NRF_802154_USE_INTERNAL_INCLUDES
#include "nrf_802154_sl_periphs_internal.h"
#endif

/**
 * @def NRF_802154_EGU_INSTANCE_NO
 *
 * Id of the EGU instance used by the driver to synchronize DPPIs and for requests and
 * notifications if SWI is in use.
 *
 */
#ifndef NRF_802154_EGU_INSTANCE_NO
#if defined(NRF52_SERIES) || defined(NRF5340_XXAA)
#define NRF_802154_EGU_INSTANCE_NO 0
#elif defined(NRF54L_SERIES)
#define NRF_802154_EGU_INSTANCE_NO 10
#endif
#endif

/**
 * @def NRF_802154_EGU_INSTANCE
 *
 * The EGU instance used by the driver to synchronize PPIs and for requests and notifications if
 * SWI is in use.
 *
 * @note This option is used by the core module regardless of the driver configuration.
 *
 */
#define NRF_802154_EGU_INSTANCE          NRFX_CONCAT_2(NRF_EGU, NRF_802154_EGU_INSTANCE_NO)

/**
 * @def NRF_802154_EGU_TIMESTAMP_CHANNEL
 */
#define NRF_802154_EGU_TIMESTAMP_CHANNEL 4

/**
 * @def NRF_802154_RTC_INSTANCE_NO
 *
 * Number of the RTC instance used in the standalone timer driver implementation.
 *
 */
#ifndef NRF_802154_RTC_INSTANCE_NO

#define NRF_802154_RTC_INSTANCE_NO 2

#endif // NRF_802154_RTC_INSTANCE_NO

/**
 * @def NRF_802154_RTC_INSTANCE
 *
 * The RTC instance used in the standalone timer driver implementation.
 *
 * @note This configuration is only applicable for the Low Power Timer Abstraction Layer
 *       implementation in nrf_802154_lp_timer_nodrv.c.
 *
 */
#define NRF_802154_RTC_INSTANCE    NRFX_CONCAT_2(NRF_RTC, NRF_802154_RTC_INSTANCE_NO)

/**
 * @def NRF_802154_RTC_IRQ_HANDLER
 *
 * The RTC interrupt handler name used in the standalone timer driver implementation.
 *
 * @note This configuration is only applicable for Low Power Timer Abstraction Layer implementation
 *       in nrf_802154_lp_timer_nodrv.c.
 *
 */
#define NRF_802154_RTC_IRQ_HANDLER NRFX_CONCAT_3(RTC, NRF_802154_RTC_INSTANCE_NO, _IRQHandler)

/**
 * @def NRF_802154_RTC_IRQN
 *
 * The RTC Interrupt number used in the standalone timer driver implementation.
 *
 * @note This configuration is only applicable for the Low Power Timer Abstraction Layer implementation
 *       in nrf_802154_lp_timer_nodrv.c.
 *
 */
#define NRF_802154_RTC_IRQN        NRFX_CONCAT_3(RTC, NRF_802154_RTC_INSTANCE_NO, _IRQn)

/**
 * @def NRF_802154_HIGH_PRECISION_TIMER_INSTANCE_NO
 *
 * Number of the timer instance used for precise frame timestamps and synchronous radio operations.
 *
 */
#ifndef NRF_802154_HIGH_PRECISION_TIMER_INSTANCE_NO
#define NRF_802154_HIGH_PRECISION_TIMER_INSTANCE_NO 1
#endif

/**
 * @def NRF_802154_HIGH_PRECISION_TIMER_INSTANCE
 *
 * The timer instance used for precise frame timestamps and synchronous radio operations.
 *
 */
#define NRF_802154_HIGH_PRECISION_TIMER_INSTANCE \
    NRFX_CONCAT_2(NRF_TIMER, NRF_802154_HIGH_PRECISION_TIMER_INSTANCE_NO)

/**
 * @def NRF_802154_PPI_RTC_COMPARE_TO_TIMER_CAPTURE
 *
 * The PPI channel that connects LP timer's COMPARE event to HP timer's TIMER_CAPTURE task.
 *
 * @note This option is used only when the timestamping feature is enabled
 *       (see @ref NRF_802154_SL_TIMESTAMP_ENABLED).
 *
 */
#ifndef NRF_802154_PPI_RTC_COMPARE_TO_TIMER_CAPTURE
#if defined(DPPI_PRESENT)
#define NRF_802154_PPI_RTC_COMPARE_TO_TIMER_CAPTURE 13U
#else
#define NRF_802154_PPI_RTC_COMPARE_TO_TIMER_CAPTURE NRF_PPI_CHANNEL13
#endif
#endif

/**
 * @def NRF_802154_PPI_TIMESTAMP_EVENT_TO_TIMER_CAPTURE
 *
 * The PPI channel that connects provided event to HP timer's TIMER_CAPTURE task.
 *
 * @note This option is used only when the timestamping feature is enabled
 *       (see @ref NRF_802154_SL_TIMESTAMP_ENABLED).
 *
 */
#ifndef NRF_802154_PPI_TIMESTAMP_EVENT_TO_TIMER_CAPTURE
#if defined(DPPI_PRESENT)
#define NRF_802154_PPI_TIMESTAMP_EVENT_TO_TIMER_CAPTURE 11U
#else
#define NRF_802154_PPI_TIMESTAMP_EVENT_TO_TIMER_CAPTURE NRF_PPI_CHANNEL14
#endif
#endif

/**
 * @def NRF_802154_TIMESTAMP_PPI_CHANNELS_USED_MASK
 *
 * Helper bit mask of PPI channels used by the 802.15.4 driver for timestamping.
 */
#define NRF_802154_TIMESTAMP_PPI_CHANNELS_USED_MASK       \
    ((1 << NRF_802154_PPI_RTC_COMPARE_TO_TIMER_CAPTURE) | \
     (1 << NRF_802154_PPI_TIMESTAMP_EVENT_TO_TIMER_CAPTURE))

/**
 * @def NRF_802154_PPI_TIMESTAMP_GROUP
 *
 * The PPI channel group used to control PPIs used for timestamping.
 *
 * @note This option is used only when the timestamping feature is enabled
 *       (see @ref NRF_802154_SL_TIMESTAMP_ENABLED).
 *
 */
#ifndef NRF_802154_PPI_TIMESTAMP_GROUP
#if defined(DPPI_PRESENT)
#define NRF_802154_PPI_TIMESTAMP_GROUP NRF_DPPI_CHANNEL_GROUP1
#else
#define NRF_802154_PPI_TIMESTAMP_GROUP NRF_PPI_CHANNEL_GROUP2
#endif
#endif

#if defined(NRF54L_SERIES)
/**
 * @def NRF_802154_SL_DPPIC_INSTANCE_NO
 *
 * Id of the DPPIC instance used by the driver to connect peripherals to the radio.
 *
 */
#ifndef NRF_802154_SL_DPPIC_INSTANCE_NO
#define NRF_802154_SL_DPPIC_INSTANCE_NO 10
#endif

/**
 * @def NRF_802154_SL_DPPIC_INSTANCE
 *
 * The DPPIC instance used by the driver to connect peripherals to the radio.
 *
 */
#define NRF_802154_SL_DPPIC_INSTANCE NRFX_CONCAT_2(NRF_DPPIC, NRF_802154_SL_DPPIC_INSTANCE_NO)

#endif // defined(NRF54L_SERIES)

#endif // NRF_802154_SL_PERIPHS_H__
