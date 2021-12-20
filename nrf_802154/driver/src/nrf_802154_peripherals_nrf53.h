/*
 * Copyright (c) 2019 - 2021, Nordic Semiconductor ASA
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
 * @brief Module that defines the 802.15.4 driver peripheral usage for nRF53 family.
 *
 */

#ifndef NRF_802154_PERIPHERALS_NRF53_H__
#define NRF_802154_PERIPHERALS_NRF53_H__

#include <nrf.h>
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
 * Id of the EGU instance used by the driver to synchronize DPPIs and for requests and
 * notifications if SWI is in use.
 *
 */
#ifndef NRF_802154_EGU_INSTANCE_NO
#define NRF_802154_EGU_INSTANCE_NO 0
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
#define NRF_802154_EGU_INSTANCE NRFX_CONCAT_2(NRF_EGU, NRF_802154_EGU_INSTANCE_NO)

/**
 * @def NRF_802154_EGU_IRQ_HANDLER
 *
 * The EGU IRQ handler used by the driver for requests and notifications if SWI is in use.
 *
 * @note This option is used when the driver uses SWI to process requests and notifications.
 *
 */
#define NRF_802154_EGU_IRQ_HANDLER \
    NRFX_CONCAT_3(EGU, NRF_802154_EGU_INSTANCE_NO, _IRQHandler)

/**
 * @def NRF_802154_EGU_IRQN
 *
 * The SWI EGU IRQ number used by the driver for requests and notifications if SWI is in use.
 *
 * @note This option is used when the driver uses SWI to process requests and notifications.
 *
 */
#define NRF_802154_EGU_IRQN \
    NRFX_CONCAT_3(EGU, NRF_802154_EGU_INSTANCE_NO, _IRQn)

/**
 * @def NRF_802154_EGU_USED_MASK
 *
 * Bit mask of instances of SWI/EGU peripherals used by the 802.15.4 driver.
 */
#ifndef NRF_802154_EGU_USED_MASK
#define NRF_802154_EGU_USED_MASK (1 << NRF_802154_EGU_INSTANCE_NO)
#endif

/**
 * @def NRF_802154_RTC_INSTANCE_NO
 *
 * Number of the RTC instance used in the standalone timer driver implementation.
 *
 */
#ifndef NRF_802154_RTC_INSTANCE_NO
#define NRF_802154_RTC_INSTANCE_NO 2
#endif

/**
 * @def NRF_802154_DPPI_RADIO_DISABLED
 *
 * The DPPI channel that publishes RADIO_DISABLED event.
 *
 * @note This option is used by the core module regardless of the driver configuration.
 *
 */
#ifndef NRF_802154_DPPI_RADIO_DISABLED
#define NRF_802154_DPPI_RADIO_DISABLED 7U
#endif

/**
 * @def NRF_802154_DPPI_RADIO_READY
 *
 * The DPPI channel that publishes RADIO_READY event.
 *
 * @note This option is used by the core module regardless of the driver configuration.
 *
 */
#ifndef NRF_802154_DPPI_RADIO_READY
#define NRF_802154_DPPI_RADIO_READY 4U
#endif

/**
 * @def NRF_802154_DPPI_RADIO_ADDRESS
 *
 * The DPPI channel that publishes RADIO_ADDRESS event.
 *
 * @note This option is used by the core module regardless of the driver configuration.
 *
 */
#ifndef NRF_802154_DPPI_RADIO_ADDRESS
#define NRF_802154_DPPI_RADIO_ADDRESS 5U
#endif

/**
 * @def NRF_802154_DPPI_RADIO_END
 *
 * The DPPI channel that publishes RADIO_END event.
 *
 * @note This option is used by the core module regardless of the driver configuration.
 *
 */
#ifndef NRF_802154_DPPI_RADIO_END
#define NRF_802154_DPPI_RADIO_END 6U
#endif

/**
 * @def NRF_802154_DPPI_RADIO_PHYEND
 *
 * The DPPI channel that publishes RADIO_PHYEND event.
 *
 * @note This option is used by the core module regardless of the driver configuration.
 *
 */
#ifndef NRF_802154_DPPI_RADIO_PHYEND
#define NRF_802154_DPPI_RADIO_PHYEND 12U
#endif

/**
 * @def NRF_802154_DPPI_EGU_TO_RADIO_RAMP_UP
 *
 * The DPPI channel that connects EGU event to RADIO_TXEN or RADIO_RXEN task.
 *
 * @note This option is used by the core module regardless of the driver configuration.
 *       The peripheral is shared with @ref NRF_802154_DPPI_EGU_TO_RADIO_RAMP_UP.
 *
 */
#ifndef NRF_802154_DPPI_EGU_TO_RADIO_RAMP_UP
#define NRF_802154_DPPI_EGU_TO_RADIO_RAMP_UP 10U
#endif

/**
 * @def NRF_802154_DPPI_TIMER_COMPARE_TO_RADIO_TXEN
 *
 * The DPPI channel that connects TIMER_COMPARE event to RADIO_TXEN task.
 *
 * @note This option is used by the core module regardless of the driver configuration.
 *       The peripheral is shared with @ref NRF_802154_DPPI_EGU_TO_RADIO_RAMP_UP.
 *
 */
#ifndef NRF_802154_DPPI_TIMER_COMPARE_TO_RADIO_TXEN
#define NRF_802154_DPPI_TIMER_COMPARE_TO_RADIO_TXEN 10U
#endif

/**
 * @def NRF_802154_DPPI_RADIO_SYNC_TO_EGU_SYNC
 *
 * The DPPI channel that connects RADIO_SYNC event to EGU_SYNC task.
 * EGU_SYNC task belongs to one of EGU channels
 *
 */
#ifndef NRF_802154_DPPI_RADIO_SYNC_TO_EGU_SYNC
#define NRF_802154_DPPI_RADIO_SYNC_TO_EGU_SYNC 8U
#endif

/**
 * @def NRF_802154_DPPI_RADIO_CCAIDLE
 *
 * The DPPI channel that RADIO.CCAIDLE event publishes to
 */
#ifndef NRF_802154_DPPI_RADIO_CCAIDLE
#define NRF_802154_DPPI_RADIO_CCAIDLE 9U
#endif

/**
 * @def NRF_802154_DPPI_TIMESTAMPS_USED_MASK
 *
 * Helper bit mask of DPPI channels used by the 802.15.4 driver for timestamping.
 */
#ifdef NRF_802154_FRAME_TIMESTAMP_ENABLED
#define NRF_802154_DPPI_TIMESTAMPS_USED_MASK                \
    ((1UL << NRF_802154_PPI_RTC_COMPARE_TO_TIMER_CAPTURE) | \
     (1UL << NRF_802154_PPI_TIMESTAMP_EVENT_TO_TIMER_CAPTURE))
#else // NRF_802154_FRAME_TIMESTAMP_ENABLED
#define NRF_802154_DPPI_TIMESTAMPS_USED_MASK 0U
#endif  // NRF_802154_FRAME_TIMESTAMP_ENABLED

/**
 * @def NRF_802154_DPPI_CHANNELS_USED_MASK
 *
 * Bit mask of DPPI channels used by the 802.15.4 driver.
 */
#ifndef NRF_802154_DPPI_CHANNELS_USED_MASK
#define NRF_802154_DPPI_CHANNELS_USED_MASK (                   \
        (1UL << NRF_802154_DPPI_RADIO_DISABLED) |              \
        (1UL << NRF_802154_DPPI_RADIO_READY) |                 \
        (1UL << NRF_802154_DPPI_RADIO_ADDRESS) |               \
        (1UL << NRF_802154_DPPI_RADIO_END) |                   \
        (1UL << NRF_802154_DPPI_RADIO_PHYEND) |                \
        (1UL << NRF_802154_DPPI_EGU_TO_RADIO_RAMP_UP) |        \
        (1UL << NRF_802154_DPPI_TIMER_COMPARE_TO_RADIO_TXEN) | \
        (1UL << NRF_802154_DPPI_RADIO_SYNC_TO_EGU_SYNC) |      \
        (1UL << NRF_802154_DPPI_RADIO_CCAIDLE) |               \
        NRF_802154_DPPI_TIMESTAMPS_USED_MASK)
#endif // NRF_802154_DPPI_CHANNELS_USED_MASK

/**
 * @def NRF_802154_DPPI_GROUPS_USED_MASK
 *
 * Bit mask of DPPI groups identifiers used by the 802.15.4 driver.
 */
#ifndef NRF_802154_DPPI_GROUPS_USED_MASK
#define NRF_802154_DPPI_GROUPS_USED_MASK 0UL
#endif // NRF_802154_DPPI_GROUPS_USED_MASK

#ifdef __cplusplus
}
#endif

#endif // NRF_802154_PERIPHERALS_NRF53_H__
