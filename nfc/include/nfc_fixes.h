/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: BSD-5-Clause-Nordic
 */

#ifndef NFC_FIXES_H__
#define NFC_FIXES_H__

#include <zephyr/types.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @file
 * @defgroup nfc_fixes NFC fixes and workarounds
 * @{
 * @ingroup nfc_t2t
 * @brief @tagAPI52 Fixes for hardware-related anomalies.
 *
 * If you are using the nRF52832 chip,
 * you must define the macro HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND to apply
 * workarounds for the following anomalies:
 * - 79. NFCT: A false EVENTS_FIELDDETECTED event occurs after the field
 *	 is lost.
 * - 116. NFCT does not release HFCLK when switching from ACTIVATED to SENSE
 *	  mode.
 *
 * If you are using the IC revision Engineering A of the nRF52840 chip,
 * you must define the macro HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND to
 * apply workarounds for the following anomalies:
 * - 98. NFCT: The NFCT is not able to communicate with the peer.
 * - 116. NFCT does not release HFCLK when switching from ACTIVATED to SENSE
 *	  mode.
 * - 144. NFCT: Not optimal NFC performance
 *
 * If you are using the IC revision 1, Engineering B, or Engineering C of the
 * nRF52840 chip, you must define the macro
 * HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND to apply workarounds for the
 * following anomalies:
 * - 190. NFCT: Event FIELDDETECTED may be generated too early.
 *
 * The use of implemented workarounds for the nRF52840 chip is determined at
 * runtime and depends on the chip variant.
 *
 * The current code contains a patch for anomaly 25 (NFCT: Reset value of
 * SENSRES register is incorrect), so that it now works on Windows Phone.
 */

#ifdef CONFIG_SOC_NRF52832
/* assume nRF52832 chip in IC rev. Engineering B or Engineering C */
	#define HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND
#elif defined(CONFIG_SOC_NRF52840)
/* assume nRF52840 chip in IC rev. Engineering A, B or C */
	#define HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND
#endif


/**
 * @brief Internal auxiliary function to check if the program is running on
 * NRF52840 chip
 * @retval true  It is NRF52480 chip
 * @retval false It is other chip
 */
static inline bool type_52840_check(void)
{
	return ((((*(u32_t *)0xF0000FE0) & 0xFF) == 0x08) &&
		(((*(u32_t *)0xF0000FE4) & 0x0F) == 0x0));
}

/**
 * @brief Internal auxiliary function to check if the program is running on
 * first sample of NRF52840 chip
 * @retval true  It is NRF52480 chip and it is first sample version
 * @retval false It is other chip
 */
static inline bool type_52840_sample_check(void)
{
	return (type_52840_check() &&
		(((*(u32_t *)0xF0000FE8) & 0xF0) == 0x00) &&
		(((*(u32_t *)0xF0000FEC) & 0xF0) == 0x00));
}

/**
 * @brief Internal auxiliary function to check if the program is running on
 * final version of NRF52840 chip
 * @retval true  It is NRF52480 chip and it is final version
 * @retval false It is other chip
 */
static inline bool type_52840_final_check(void)
{
	return (type_52840_check() &&
		((((*(u32_t *)0xF0000FE8) & 0xF0) != 0x00) ||
		 (((*(u32_t *)0xF0000FEC) & 0xF0) != 0x00)));
}

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* NFC_FIXES_H__ */
