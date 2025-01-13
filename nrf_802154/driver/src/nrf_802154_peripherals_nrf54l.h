/*
 * Copyright (c) 2024, Nordic Semiconductor ASA
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
 * @brief Module that defines the 802.15.4 driver peripheral usage for nRF54L family.
 *
 */

#ifndef NRF_802154_PERIPHERALS_NRF54L_H__
#define NRF_802154_PERIPHERALS_NRF54L_H__

#include <nrfx.h>
#include "nrf_802154_config.h"
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
#define NRF_802154_EGU_INSTANCE_NO 10
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
 * @def NRF_802154_EGU_USED_MASK
 *
 * Bit mask of instances of SWI/EGU peripherals used by the 802.15.4 driver.
 */
#ifndef NRF_802154_EGU_USED_MASK
#define NRF_802154_EGU_USED_MASK (1 << NRF_802154_EGU_INSTANCE_NO)
#endif

#if (NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US != 0) || defined(__DOXYGEN__)

/**
 * @def NRF_802154_EGU_TIMER_START_CHANNEL_NO
 *
 * The channel number of the @ref NRF_802154_EGU_INSTANCE used for starting the TIMER.
 * Used only when @ref NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US is non-zero.
 */
#define NRF_802154_EGU_TIMER_START_CHANNEL_NO 14

#define NRF_802154_EGU_TIMER_START_USED_CHANNELS_MASK \
    (1U << NRF_802154_EGU_TIMER_START_CHANNEL_NO)

/**
 * @def NRF_802154_EGU_TIMER_START2_CHANNEL_NO
 *
 * The channel number of the @ref NRF_802154_EGU_INSTANCE used for starting the TIMER (second source).
 * Used only when @ref NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US is non-zero.
 */
#define NRF_802154_EGU_TIMER_START2_CHANNEL_NO 13

#define NRF_802154_EGU_TIMER_START2_USED_CHANNELS_MASK \
    (1U << NRF_802154_EGU_TIMER_START2_CHANNEL_NO)

#endif /* (NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US != 0) || defined(__DOXYGEN__) */

/**
 * @def NRF_802154_CCM_INSTANCE_NO
 *
 * Id of the CCM instance used by the driver to encrypt outgoing frames.
 *
 * @note This option is used when @ref NRF_802154_ENCRYPTION_ENABLED is set.
 *
 */
#define NRF_802154_CCM_INSTANCE_NO 00

/**
 * @def NRF_802154_CCM_INSTANCE
 *
 * The CCM instance used by the driver to encrypt outgoing frames.
 *
 * @note This option is used when @ref NRF_802154_ENCRYPTION_ENABLED is set.
 *
 */
#define NRF_802154_CCM_INSTANCE    NRFX_CONCAT_2(NRF_CCM, \
                                                 NRF_802154_CCM_INSTANCE_NO)

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
 * @def NRF_802154_DPPIC_INSTANCE_NO
 *
 * Id of the DPPIC instance used by the driver to connect peripherals to radio.
 *
 */
#define NRF_802154_DPPIC_INSTANCE_NO 10

/**
 * @def NRF_802154_DPPIC_INSTANCE
 *
 * The DPPIC instance used by the driver to connect peripherals to radio.
 *
 */
#define NRF_802154_DPPIC_INSTANCE    NRFX_CONCAT_2(NRF_DPPIC, NRF_802154_DPPIC_INSTANCE_NO)

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
#define NRF_802154_DPPI_RADIO_PHYEND 8U
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
#define NRF_802154_DPPI_EGU_TO_RADIO_RAMP_UP 23U
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
#define NRF_802154_DPPI_TIMER_COMPARE_TO_RADIO_TXEN 23U
#endif

/**
 * @def NRF_802154_DPPI_RADIO_SYNC_TO_EGU_SYNC
 *
 * The DPPI channel that connects RADIO_SYNC event to EGU_SYNC task.
 * EGU_SYNC task belongs to one of EGU channels
 *
 */
#ifndef NRF_802154_DPPI_RADIO_SYNC_TO_EGU_SYNC
#define NRF_802154_DPPI_RADIO_SYNC_TO_EGU_SYNC 22U
#endif

#if (NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US != 0) || defined(__DOXYGEN__)
/**
 * @def NRF_802154_DPPI_TIMER_START
 *
 * The DPPI channel that is used to trigger TIMER's START task.
 * Used only when @ref NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US is non-zero.
 *
 */
#ifndef NRF_802154_DPPI_TIMER_START
#define NRF_802154_DPPI_TIMER_START 21U
#endif

/**
 * @def NRF_802154_DPPI_RADIO_TXEN
 *
 * The DPPI channel that is used to trigger RADIO's TXEN task
 * in the scenario where short CCAIDLE_TXEN is not used.
 * Used only when @ref NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US is non-zero.
 *
 */
#ifndef NRF_802154_DPPI_RADIO_TXEN
#define NRF_802154_DPPI_RADIO_TXEN       20U
#endif

#define NRF_802154_DPPI_TIMER_START_MASK (1U << NRF_802154_DPPI_TIMER_START)
#define NRF_802154_DPPI_RADIO_TXEN_MASK  (1U << NRF_802154_DPPI_RADIO_TXEN)

#else

#define NRF_802154_DPPI_TIMER_START_MASK 0U
#define NRF_802154_DPPI_RADIO_TXEN_MASK  0U

#endif /* (NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US != 0) || defined(__DOXYGEN__) */

/**
 * @def NRF_802154_DPPI_RADIO_CCAIDLE
 *
 * The DPPI channel that RADIO.CCAIDLE event publishes to
 */
#ifndef NRF_802154_DPPI_RADIO_CCAIDLE
#define NRF_802154_DPPI_RADIO_CCAIDLE 14U
#endif

/**
 * @def NRF_802154_DPPI_RADIO_CCABUSY
 *
 * The DPPI channel that RADIO.CCABUSY event publishes to
 */
#ifndef NRF_802154_DPPI_RADIO_CCABUSY
#define NRF_802154_DPPI_RADIO_CCABUSY 3U
#endif

/**
 * @def NRF_802154_DPPI_RADIO_HW_TRIGGER
 *
 * The DPPI channel that triggers radio.
 */
#ifndef NRF_802154_DPPI_RADIO_HW_TRIGGER
#define NRF_802154_DPPI_RADIO_HW_TRIGGER 10U
#endif

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
        (NRF_802154_DPPI_TIMER_START_MASK) |                   \
        (NRF_802154_DPPI_RADIO_TXEN_MASK) |                    \
        (1UL << NRF_802154_DPPI_RADIO_CCAIDLE) |               \
        (1UL << NRF_802154_DPPI_RADIO_CCABUSY) |               \
        (1UL << NRF_802154_DPPI_RADIO_HW_TRIGGER) |            \
        NRF_802154_SL_PPI_CHANNELS_USED_MASK)
#endif // NRF_802154_DPPI_CHANNELS_USED_MASK

/**
 * @def NRF_802154_DPPI_GROUPS_USED_MASK
 *
 * Bit mask of DPPI groups identifiers used by the 802.15.4 driver.
 */
#ifndef NRF_802154_DPPI_GROUPS_USED_MASK
#define NRF_802154_DPPI_GROUPS_USED_MASK 0UL
#endif // NRF_802154_DPPI_GROUPS_USED_MASK

/**
 * @def NRF_802154_TIMER_INSTANCE_NO
 *
 * Number of the timer instance used both by the driver for ACK IFS and by the FEM module.
 *
 */
#define NRF_802154_TIMER_INSTANCE_NO 10

#ifdef __cplusplus
}
#endif

#endif // NRF_802154_PERIPHERALS_NRF54L_H__
