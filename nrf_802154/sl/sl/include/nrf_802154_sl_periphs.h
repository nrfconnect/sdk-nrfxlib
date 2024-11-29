/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file Module that defines the peripherals used by the nRF 802.15.4 SL.
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
#elif defined(NRF54H_SERIES)
#define NRF_802154_EGU_INSTANCE_NO 020
#endif
#endif

#if defined(NRF53_SERIES)
/**
 * @def NRF_802154_EGU_INSTANCE
 *
 * The EGU instance used by the nRF 802.15.4 SL.
 *
 * @note This option is used on nRF53 series SoCs only.
 *
 */
#define NRF_802154_EGU_INSTANCE                        NRFX_CONCAT_2(NRF_EGU, \
                                                                     NRF_802154_EGU_INSTANCE_NO)

/**
 * @def NRF_802154_EGU_TIMESTAMP_CHANNEL
 *
 * The EGU channel number of the @ref NRF_802154_EGU_INSTANCE instance used by the time stamping feature.
 *
 * @note This option is used on nRF53 series SoCs only.
 *
 */
#define NRF_802154_EGU_TIMESTAMP_CHANNEL               4

#define NRF_802154_SL_EGU_TIMESTAMP_USED_CHANNELS_MASK (1U << NRF_802154_EGU_TIMESTAMP_CHANNEL)

#else
#define NRF_802154_SL_EGU_TIMESTAMP_USED_CHANNELS_MASK 0U
#endif

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
#define NRF_802154_PPI_RTC_COMPARE_TO_TIMER_CAPTURE 13U
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
#define NRF_802154_PPI_TIMESTAMP_EVENT_TO_TIMER_CAPTURE 14U
#endif
#endif

/**
 * @def NRF_802154_SL_PPI_CHANNELS_USED_MASK
 *
 * Mask of fixed (D)PPI channels used by the nRF 802.15.4 SL.
 */
#define NRF_802154_SL_PPI_CHANNELS_USED_MASK              \
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
#define NRF_802154_PPI_TIMESTAMP_GROUP 1U
#else
#define NRF_802154_PPI_TIMESTAMP_GROUP 2U
#endif
#endif

/**
 * @def NRF_802154_SL_PPI_GROUPS_USED_MASK
 *
 * Mask of fixed (D)PPI groups used by the nRF 802.15.4 SL.
 */
#define NRF_802154_SL_PPI_GROUPS_USED_MASK \
    (1U << NRF_802154_PPI_TIMESTAMP_GROUP)

/**
 * @def NRF_802154_SL_EGU_USED_CHANNELS_MASK
 *
 * Mask of fixed EGU channels used by the nRF 802.15.4 SL.
 */
#define NRF_802154_SL_EGU_USED_CHANNELS_MASK \
    NRF_802154_SL_EGU_TIMESTAMP_USED_CHANNELS_MASK

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

#if defined(NRF54H_SERIES)

/**
 * @def NRF_802154_SL_DPPIC_INSTANCE_NO
 *
 * Id of the DPPIC instance used by the driver to connect peripherals to radio.
 *
 */
#ifndef NRF_802154_SL_DPPIC_INSTANCE_NO
#define NRF_802154_SL_DPPIC_INSTANCE_NO 020
#endif

/**
 * @def NRF_802154_SL_DPPIC_INSTANCE
 *
 * The DPPIC instance used by the driver to connect peripherals to radio.
 *
 */
#define NRF_802154_SL_DPPIC_INSTANCE NRFX_CONCAT_2(NRF_DPPIC, NRF_802154_SL_DPPIC_INSTANCE_NO)

#endif // defined(NRF54H_SERIES)

#endif // NRF_802154_SL_PERIPHS_H__
