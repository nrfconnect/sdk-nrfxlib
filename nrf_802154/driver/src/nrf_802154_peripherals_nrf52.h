/*
 * Copyright (c) 2019, Nordic Semiconductor ASA
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
 * @brief Module that defines the 802.15.4 driver peripheral usage for nRF52 family.
 *
 */

#ifndef NRF_802154_PERIPHERALS_NRF52_H__
#define NRF_802154_PERIPHERALS_NRF52_H__

#include <nrfx.h>
#include "nrf_802154_config.h"
#include "nrf_802154_debug.h"
#include "nrf_802154_sl_periphs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @def NRF_802154_EGU_INSTANCE_NO
 *
 * Number of the EGU instance used by the driver to synchronize PPIs and for requests and
 * notifications if SWI is in use.
 *
 */
#ifndef NRF_802154_EGU_INSTANCE_NO

#define NRF_802154_EGU_INSTANCE_NO 0

#endif // NRF_802154_EGU_INSTANCE_NO

/**
 * @def NRF_802154_EGU_INSTANCE
 *
 * The EGU instance used by the driver to synchronize PPIs and for requests and notifications if
 * SWI is in use.
 *
 * @note This option is used by the core module regardless of the driver configuration.
 *
 */
#define NRF_802154_EGU_INSTANCE NRFX_CONCAT_2(NRF_EGU, NRF_802154_EGU_INSTANCE_NO)

/**
 * @def NRF_802154_EGU_IRQ_HANDLER
 *
 * The SWI EGU IRQ handler used by the driver for requests and notifications if SWI is in use.
 *
 * @note This option is used when the driver uses SWI to process requests and notifications.
 *
 */
#define NRF_802154_EGU_IRQ_HANDLER                                      \
    NRFX_CONCAT_3(NRFX_CONCAT_3(SWI, NRF_802154_EGU_INSTANCE_NO, _EGU), \
                  NRF_802154_EGU_INSTANCE_NO,                           \
                  _IRQHandler)

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
 * @def NRF_802154_PPI_RADIO_RAMP_UP_TRIGG
 *
 * The PPI channel that connects ramp up triggering event to EGU task.
 *
 * @note The peripheral is shared with @ref NRF_802154_PPI_RADIO_DISABLED_TO_EGU in such a way
 *       that a receive or transmit request causes that ppi to be configured for ramp up
 *       triggering purpose, while in the EVENT_READY isr the ppi is reconfigured to follow the
 *       @ref NRF_802154_PPI_RADIO_DISABLED_TO_EGU description.
 *
 * @note This option is used regardless of the driver configuration.
 *
 */
#ifndef NRF_802154_PPI_RADIO_RAMP_UP_TRIGG
#define NRF_802154_PPI_RADIO_RAMP_UP_TRIGG 6U
#endif

/**
 * @def NRF_802154_PPI_RADIO_DISABLED_TO_EGU
 *
 * The PPI channel that connects RADIO_DISABLED event to EGU task.
 *
 * @note This option is used by the core module regardless of the driver configuration.
 *       The peripheral is shared with @ref NRF_802154_PPI_RADIO_RAMP_UP_TRIGG.
 *
 */
#ifndef NRF_802154_PPI_RADIO_DISABLED_TO_EGU
#define NRF_802154_PPI_RADIO_DISABLED_TO_EGU 6U
#endif

/**
 * @def NRF_802154_PPI_EGU_TO_RADIO_RAMP_UP
 *
 * The PPI channel that connects EGU event to RADIO_TXEN or RADIO_RXEN task.
 *
 * @note This option is used by the core module regardless of the driver configuration.
 *
 */
#ifndef NRF_802154_PPI_EGU_TO_RADIO_RAMP_UP
#define NRF_802154_PPI_EGU_TO_RADIO_RAMP_UP 7U
#endif

/**
 * @def NRF_802154_PPI_EGU_TO_TIMER_START
 *
 * The PPI channel that connects EGU event to TIMER_START task.
 *
 * @note This option is used by the core module regardless of the driver configuration.
 *
 */
#ifndef NRF_802154_PPI_EGU_TO_TIMER_START
#define NRF_802154_PPI_EGU_TO_TIMER_START 8U
#endif

/**
 * @def NRF_802154_PPI_RADIO_CCAIDLE_TO_FEM_GPIOTE
 *
 * The PPI channel that connects RADIO_CCAIDLE event to the GPIOTE tasks used by the Frontend.
 *
 * @note This option is used by the core module regardless of the driver configuration.
 *       The peripheral is shared with @ref NRF_802154_PPI_TIMER_COMPARE_TO_RADIO_TXEN.
 *
 */
#ifndef NRF_802154_PPI_RADIO_CCAIDLE_TO_FEM_GPIOTE
#define NRF_802154_PPI_RADIO_CCAIDLE_TO_FEM_GPIOTE 9U
#endif

/**
 * @def NRF_802154_PPI_TIMER_COMPARE_TO_RADIO_TXEN
 *
 * The PPI channel that connects TIMER_COMPARE event to RADIO_TXEN task.
 *
 * @note This option is used by the core module regardless of the driver configuration.
 *       The peripheral is shared with @ref NRF_802154_PPI_RADIO_CCAIDLE_TO_FEM_GPIOTE.
 *
 */
#ifndef NRF_802154_PPI_TIMER_COMPARE_TO_RADIO_TXEN
#define NRF_802154_PPI_TIMER_COMPARE_TO_RADIO_TXEN 9U
#endif

/**
 * @def NRF_802154_PPI_RADIO_CCABUSY_TO_RADIO_CCASTART
 *
 * The PPI channel that connects RADIO_CCABUSY event to RADIO_CCASTART task. This PPI is used to
 * perform multiple back-to-back CCA attempts before a transmission.
 *
 * @note This option is used by the core module regardless of the driver configuration.
 *
 */
#ifndef NRF_802154_PPI_RADIO_CCABUSY_TO_RADIO_CCASTART
#define NRF_802154_PPI_RADIO_CCABUSY_TO_RADIO_CCASTART 10U
#endif

/**
 * @def NRF_802154_PPI_RADIO_SYNC_TO_EGU_SYNC
 *
 * The PPI channel that connects RADIO_SYNC event to EGU_SYNC task.
 * EGU_SYNC task belongs to one of EGU channels
 *
 */
#ifndef NRF_802154_PPI_RADIO_SYNC_TO_EGU_SYNC
#define NRF_802154_PPI_RADIO_SYNC_TO_EGU_SYNC 11U
#endif

#define NRF_802154_DISABLE_BCC_MATCHING_PPI_CHANNELS_USED_MASK \
    (1 << NRF_802154_PPI_RADIO_SYNC_TO_EGU_SYNC)

/**
 * @def NRF_802154_PPI_CORE_GROUP
 *
 * The PPI channel group used to disable self-disabling PPIs used by the core module.
 *
 * @note This option is used by the core module regardless of the driver configuration.
 *
 */
#ifndef NRF_802154_PPI_CORE_GROUP
#define NRF_802154_PPI_CORE_GROUP 0U
#endif

/**
 * @def NRF_802154_PPI_ABORT_GROUP
 *
 * The PPI channel group used to break PPI connections related with FEM, when the abort condition occurs.
 *
 */
#ifndef NRF_802154_PPI_ABORT_GROUP
#define NRF_802154_PPI_ABORT_GROUP 1U
#endif

/**
 * @def NRF_802154_EGU_USED_MASK
 *
 * Bit mask of instances of SWI/EGU peripherals used by the 802.15.4 driver.
 */
#ifndef NRF_802154_EGU_USED_MASK
#define NRF_802154_EGU_USED_MASK (1 << NRF_802154_EGU_INSTANCE_NO)
#endif

/**
 * @def NRF_802154_PPI_CHANNELS_USED_MASK
 *
 * Bit mask of PPI channels used by the 802.15.4 driver.
 */
#ifndef NRF_802154_PPI_CHANNELS_USED_MASK
#define NRF_802154_PPI_CHANNELS_USED_MASK ((1 << NRF_802154_PPI_RADIO_DISABLED_TO_EGU) |            \
                                           (1 << NRF_802154_PPI_RADIO_RAMP_UP_TRIGG) |              \
                                           (1 << NRF_802154_PPI_EGU_TO_RADIO_RAMP_UP) |             \
                                           (1 << NRF_802154_PPI_EGU_TO_TIMER_START) |               \
                                           (1 << NRF_802154_PPI_RADIO_CCAIDLE_TO_FEM_GPIOTE) |      \
                                           (1 << NRF_802154_PPI_TIMER_COMPARE_TO_RADIO_TXEN) |      \
                                           (1 << NRF_802154_PPI_RADIO_CCABUSY_TO_RADIO_CCASTART) |  \
                                           NRF_802154_DISABLE_BCC_MATCHING_PPI_CHANNELS_USED_MASK | \
                                           NRF_802154_SL_PPI_CHANNELS_USED_MASK |                   \
                                           NRF_802154_DEBUG_PPI_CHANNELS_USED_MASK)
#endif // NRF_802154_PPI_CHANNELS_USED_MASK

/**
 * @def NRF_802154_PPI_GROUPS_USED_MASK
 *
 * Bit mask of PPI groups identifiers used by the 802.15.4 driver.
 */
#ifndef NRF_802154_PPI_GROUPS_USED_MASK
#define NRF_802154_PPI_GROUPS_USED_MASK ((1 << NRF_802154_PPI_CORE_GROUP) | \
                                         (1 << NRF_802154_PPI_ABORT_GROUP)  )
#endif // NRF_802154_PPI_GROUPS_USED_MASK

#ifdef __cplusplus
}
#endif

#endif // NRF_802154_PERIPHERALS_NRF52_H__
