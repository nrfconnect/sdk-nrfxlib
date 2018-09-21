/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: BSD-5-Clause-Nordic
 */

#include <stdbool.h>
#include <string.h>
#include "errno.h"

#include <hal_nfc_t2t.h>
#include <nfc_t2t_lib.h>
#include <nfc_fixes.h>

#include <nrf_clock.h>
#include <clock_control.h>
#include <device.h>
#include <irq.h>

#include <logging/log.h>

#define LOG_MODULE_NAME hal_nfc_t2t
#define LOG_LEVEL CONFIG_HAL_NFC_LOG_LEVEL
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

/* NFC library version history:
 * first experimental version intended for nRF52 IC rev. Engineering A
 * (PCA10036, part of nRF52 Preview Development Kit)
 * #define NFC_LIB_VERSION          0x00
 * experimental version intended for nRF52 IC rev. Engineering B (PCA10040,
 * part of nRF52 Development Kit)
 * #define NFC_LIB_VERSION          0x01
 * experimental version intended for fix IC-12826 and fix: not released HFCLK
 * in SENSE mode
 * #define NFC_LIB_VERSION          0x02
 * experimental version intended for support logging module
 * #define NFC_LIB_VERSION          0x03
 * experimental version intended for nRF52840 IC rev. Engineering A (PCA10056,
 * part of nRF52840 Preview Development Kit). Removed PCA10036 support.
 * #define NFC_LIB_VERSION          0x04
 */

/** Internal: current NFC lib. version  */
#define NFC_LIB_VERSION             0x03u

/** Number of internal bytes defined by Type 2 Tag Operation Technical
 *   Specification
 */
#define T2T_INTERNAL_BYTES_NR       10u
/** Constant defined by ISO/EIC 14443-3 */
#define CASCADE_TAG_BYTE            0x88u

/** Shift value for NFC ID byte 2 */
#define NFCID1_3RD_LAST_BYTE2_SHIFT 16u
/** Shift value for NFC ID byte 1 */
#define NFCID1_3RD_LAST_BYTE1_SHIFT 8u
/** Shift value for NFC ID byte 0 */
#define NFCID1_3RD_LAST_BYTE0_SHIFT 0u
/** Shift value for NFC ID byte 2 */
#define NFCID1_2ND_LAST_BYTE2_SHIFT 16u
/** Shift value for NFC ID byte 1 */
#define NFCID1_2ND_LAST_BYTE1_SHIFT 8u
/** Shift value for NFC ID byte 0 */
#define NFCID1_2ND_LAST_BYTE0_SHIFT 0u
/** Shift value for NFC ID byte 3 */
#define NFCID1_LAST_BYTE3_SHIFT     24u
/** Shift value for NFC ID byte 2 */
#define NFCID1_LAST_BYTE2_SHIFT     16u
/** Shift value for NFC ID byte 1 */
#define NFCID1_LAST_BYTE1_SHIFT     8u
/** Shift value for NFC ID byte 0 */
#define NFCID1_LAST_BYTE0_SHIFT     0u

/** Length of single size NFCID1 */
#define NFCID1_SINGLE_SIZE          4u
/** Length of double size NFCID1 */
#define NFCID1_DOUBLE_SIZE          7u
/** Length of triple size NFCID1 */
#define NFCID1_TRIPLE_SIZE          10u
/** Length of NFCID1 if user does not provide one */
#define NFCID1_DEFAULT_LENGHT       NFCID1_DOUBLE_SIZE
/** Maximum length of NFCID1 */
#define NFCID1_MAX_LENGHT           NFCID1_TRIPLE_SIZE

/** NFC Rx data buffer size */
#define NFC_RX_BUFFER_SIZE          16u
/** Type 2 Tag Read command identifier */
#define T2T_READ_CMD                0x30u
/** NFC SLP_REQ command identifier */
#define NFC_SLP_REQ_CMD             0x50u
/** CRC size in bytes */
#define NFC_CRC_SIZE                2u

#ifdef HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND
/** Mask for clearing all error flags in NFCT_ERRORSTATUS register */
#define NRF_NFCT_ERRORSTATUS_ALL    (NFCT_ERRORSTATUS_FRAMEDELAYTIMEOUT_Msk)
#else
/** Mask for clearing all error flags in NFCT_ERRORSTATUS register */
#define NRF_NFCT_ERRORSTATUS_ALL    (NFCT_ERRORSTATUS_NFCFIELDTOOWEAK_Msk   | \
				     NFCT_ERRORSTATUS_NFCFIELDTOOSTRONG_Msk | \
				     NFCT_ERRORSTATUS_FRAMEDELAYTIMEOUT_Msk)
#endif /* HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND */

/** Mask for clearing all flags in NFCT_FRAMESTATUS_RX register */
#define NRF_NFCT_FRAMESTATUS_RX_MSK (NFCT_FRAMESTATUS_RX_OVERRUN_Msk      | \
				     NFCT_FRAMESTATUS_RX_PARITYSTATUS_Msk | \
				     NFCT_FRAMESTATUS_RX_CRCERROR_Msk)
/** Mask for checking FIELDPRESENT register for state: FIELD ON. */
#define NFC_FIELD_ON_MASK	     NFCT_FIELDPRESENT_LOCKDETECT_Msk
/** Mask for checking FIELDPRESENT register for state: FIELD OFF. */
#define NFC_FIELD_OFF_MASK	     NFCT_FIELDPRESENT_FIELDPRESENT_Msk

/** The default state of NFCT. */
#define NRF_NFCT_DEFAULTSTATESLEEP     (*(u32_t volatile *)(0x40005420))
/** Mask for checking the default state of NFCT. */
#define NRF_NFCT_DEFAULTSTATESLEEP_MSK 0x1UL

#ifdef HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND
/** Number of required conditions to activate NFCT. */
#define NRF_NFCT_ACTIVATE_CONDS_THR    2
/** Minimal delay in us between NFC field detection and activation of NFCT. */
#define NRF_NFCT_ACTIVATE_DELAY        1000
#endif /* HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND */

enum nfct_field_sense_state {
	/** Initial value indicating no NFCT Field events. */
	NFC_FIELD_STATE_NONE,
	/** NFCT FIELDLOST Event has been set. */
	NFC_FIELD_STATE_OFF,
	/** NFCT FIELDDETECTED Event has been set. */
	NFC_FIELD_STATE_ON,
	/** Both NFCT Field Events have been set - ambiguous state. */
	NFC_FIELD_STATE_UNKNOWN
};

/* Static function declarations */
static inline void nrf_nfct_event_clear(volatile u32_t *event);
static inline void nrf_nfct_field_event_handler(
			volatile enum nfct_field_sense_state field_state);
static void hal_nfc_nfcid1_default_bytes(void);
static void hal_nfc_nfcid1_registers_setup(void);
static void hal_nfc_internal_bytes_setup(void);
static void NFCT_IRQHandler(void);
#if defined(HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND) || \
	defined(HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND)
static void TIMER4_IRQHandler(void);
#endif /* defined(HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND) || \
	* defined(HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND)
	*/
/* Static data */
/** Callback to nfc_lib layer */
static hal_nfc_callback_t m_nfc_lib_callback = (hal_nfc_callback_t) NULL;
/** Callback execution context */
static void *m_nfc_lib_context;
/** Buffer for NFC Rx data */
static volatile u8_t m_nfc_rx_buffer[NFC_RX_BUFFER_SIZE] = { 0 };
/** Flag indicating that SLP_REQ Command was received */
static volatile bool m_slp_req_received;
/** Flag indicating that NFC Tag field is present */
static volatile bool m_field_on;
/** Pointer to HF Clock device */
static struct device *hf_clk = (struct device *) NULL;
/** Length of NFCID1 provided by user or 0 if not initialized yet */
static u8_t m_nfcid1_length;
/** Content of NFCID1 */
static u8_t m_nfcid1_data[NFCID1_MAX_LENGHT] = { 0 };

#ifdef HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND
/** Flag indicating that field events are ignored. */
static volatile bool m_nfc_fieldevents_filter_active;
/** Number of activation conditions that are met. */
static volatile u32_t m_nfc_activate_conditions;
#endif /* HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND */

#ifndef HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND
/** Mask used for NFC Field polling in NFCT_FIELDPRESENT register */
static volatile u32_t m_nfc_fieldpresent_mask = NFC_FIELD_OFF_MASK;
#endif /* HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND */

#ifdef HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND

#define NRF_NFCT_POWER  (*(u32_t volatile *)(0x40005FFC))

#define NFC_HAL_FIELDPRESENT_MASK    (NFCT_FIELDPRESENT_LOCKDETECT_Msk | \
				      NFCT_FIELDPRESENT_FIELDPRESENT_Msk)

#define NFC_HAL_FIELDPRESENT_IS_LOST ((NFCT_FIELDPRESENT_FIELDPRESENT_NoField  \
				      << NFCT_FIELDPRESENT_FIELDPRESENT_Pos) | \
				      (NFCT_FIELDPRESENT_LOCKDETECT_NotLocked  \
				      << NFCT_FIELDPRESENT_LOCKDETECT_Pos))

#ifndef HAL_NFC_FIELD_TIMER_PERIOD
#define HAL_NFC_FIELD_TIMER_PERIOD    100  /* unit - us */
#endif

static inline void hal_nfc_re_setup(void);
static void hal_nfc_field_check(void);

#endif /* HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND */

#ifdef HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND

static void hal_nfc_activate_check(void)
{
	static bool is_field_validation_pending;

	if (is_field_validation_pending) {
		is_field_validation_pending = false;
		m_nfc_fieldevents_filter_active = false;

		/* Check the field status with FIELDPRESENT and take action if
		 * field is lost.
		 */
		nrf_nfct_field_event_handler(NFC_FIELD_STATE_UNKNOWN);
		return;
	}

	m_nfc_activate_conditions++;
	if (m_nfc_activate_conditions == NRF_NFCT_ACTIVATE_CONDS_THR) {
		m_nfc_activate_conditions = 0;

		NRF_NFCT->TASKS_ACTIVATE = 1;
		is_field_validation_pending = true;

		/* Start the timer second time to validate if tag has locked to
		 * the field
		 */
		NRF_TIMER4->TASKS_CLEAR = 1;
		NRF_TIMER4->TASKS_START = 1;
	}
}
#endif /* HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND */

#if defined(HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND) || \
	defined(HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND)

static void field_timer_with_callback_config(void)
{
	NRF_TIMER4->MODE = TIMER_MODE_MODE_Timer << TIMER_MODE_MODE_Pos;
	NRF_TIMER4->BITMODE = TIMER_BITMODE_BITMODE_16Bit <<
					TIMER_BITMODE_BITMODE_Pos;
	NRF_TIMER4->PRESCALER = 4 << TIMER_PRESCALER_PRESCALER_Pos;
#ifdef HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND
	NRF_TIMER4->CC[0] = HAL_NFC_FIELD_TIMER_PERIOD << TIMER_CC_CC_Pos;
#else
	NRF_TIMER4->CC[0] = NRF_NFCT_ACTIVATE_DELAY << TIMER_CC_CC_Pos;
#endif
	NRF_TIMER4->SHORTS = TIMER_SHORTS_COMPARE0_CLEAR_Enabled <<
					TIMER_SHORTS_COMPARE0_CLEAR_Pos;
	NRF_TIMER4->INTENSET = TIMER_INTENSET_COMPARE0_Set <<
					TIMER_INTENSET_COMPARE0_Pos;

	NVIC_ClearPendingIRQ(TIMER4_IRQn);
	IRQ_DIRECT_CONNECT(TIMER4_IRQn,
			   CONFIG_NFCT_IRQ_PRIORITY,
			   TIMER4_IRQHandler,
			   0);
	irq_enable(TIMER4_IRQn);
}

void TIMER4_IRQHandler(void)
{
#ifdef HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND
	hal_nfc_field_check();
#else
	NRF_TIMER4->TASKS_SHUTDOWN = 1;
	hal_nfc_activate_check();
#endif
	NRF_TIMER4->EVENTS_COMPARE[0] = 0;
}

#endif /* defined(HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND) ||
	* defined(HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND)
	*/

/**
 * @brief Common part of setup used for NFCT initialization and
 * reinitialization.
 */
#ifdef HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND
static void hal_nfc_common_hw_setup(void)
#else
static inline void hal_nfc_common_hw_setup(void)
#endif /* HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND */
{
#ifdef HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND
	if (type_52840_sample_check()) {
		/* Begin: Bugfix for FTPAN-98 */
		*(volatile u32_t *) 0x4000568C = 0x00038148;
		/* End: Bugfix for FTPAN-98 */
		/* Begin: Bugfix for FTPAN-144 */
		*(volatile u32_t *) 0x4000561c = 0x01;
		*(volatile u32_t *) 0x4000562c = 0x3F;
		*(volatile u32_t *) 0x4000563c = 0x0;
		/* End: Bugfix for FTPAN-144 */
	}
#endif /* HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND */

#ifdef HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND
	NRF_NFCT->INTENSET = (NFCT_INTENSET_FIELDDETECTED_Enabled <<
					NFCT_INTENSET_FIELDDETECTED_Pos);
#else
	NRF_NFCT->INTENSET = (NFCT_INTENSET_FIELDDETECTED_Enabled <<
					NFCT_INTENSET_FIELDDETECTED_Pos) |
			     (NFCT_INTENSET_FIELDLOST_Enabled <<
					NFCT_INTENSET_FIELDLOST_Pos);
#endif

	NRF_NFCT->INTENSET = (NFCT_INTENSET_ERROR_Enabled <<
					NFCT_INTENSET_ERROR_Pos) |
			     (NFCT_INTENSET_SELECTED_Enabled <<
					NFCT_INTENSET_SELECTED_Pos);

	/* Use Window Grid frame delay mode. */
	NRF_NFCT->FRAMEDELAYMODE =
			(NFCT_FRAMEDELAYMODE_FRAMEDELAYMODE_WindowGrid <<
				      NFCT_FRAMEDELAYMODE_FRAMEDELAYMODE_Pos) &
			 NFCT_FRAMEDELAYMODE_FRAMEDELAYMODE_Msk;

	hal_nfc_nfcid1_registers_setup();
}

/** @brief Setup NRF_NFCT->NFCID1 and NRF_NFCT->SENSRES registers based on
 *  m_nfcid1_data and m_nfcid1_length variables.
 */
static void hal_nfc_nfcid1_registers_setup(void)
{
	/* Value that will be written to NRF_NFCT->SENSRES */
	u32_t sens_res_size;
	/* Points to the first byte of m_nfcid1_data remaining to write
	 * to NRF_NFCT->NFCID1 registers
	 */
	u8_t *nfcid_remaining_data;

	nfcid_remaining_data = m_nfcid1_data;

	if (m_nfcid1_length == NFCID1_SINGLE_SIZE) {
		sens_res_size = NFCT_SENSRES_NFCIDSIZE_NFCID1Single;
	} else {
		if (m_nfcid1_length == NFCID1_DOUBLE_SIZE) {
			sens_res_size = NFCT_SENSRES_NFCIDSIZE_NFCID1Double;
		} else {
			/* then m_nfcid1_length == NFCID1_TRIPLE_SIZE */
			/* MSB of NFCID1_3RD_LAST register is not used -
			 * always 0
			 */
			NRF_NFCT->NFCID1_3RD_LAST =
					((u32_t) nfcid_remaining_data[0] <<
						NFCID1_3RD_LAST_BYTE2_SHIFT) |
					((u32_t) nfcid_remaining_data[1] <<
						NFCID1_3RD_LAST_BYTE1_SHIFT) |
					((u32_t) nfcid_remaining_data[2] <<
						NFCID1_3RD_LAST_BYTE0_SHIFT);
			nfcid_remaining_data += 3;
			sens_res_size = NFCT_SENSRES_NFCIDSIZE_NFCID1Triple;
		}
		/* MSB of NFCID1_2ND_LAST register is not used - always 0 */
		NRF_NFCT->NFCID1_2ND_LAST =
				((u32_t) nfcid_remaining_data[0] <<
						NFCID1_2ND_LAST_BYTE2_SHIFT) |
				((u32_t) nfcid_remaining_data[1] <<
						NFCID1_2ND_LAST_BYTE1_SHIFT) |
				((u32_t) nfcid_remaining_data[2] <<
						NFCID1_2ND_LAST_BYTE0_SHIFT);
		nfcid_remaining_data += 3;
	}

	NRF_NFCT->NFCID1_LAST = ((u32_t) nfcid_remaining_data[0] <<
						NFCID1_LAST_BYTE3_SHIFT) |
				((u32_t) nfcid_remaining_data[1] <<
						NFCID1_LAST_BYTE2_SHIFT) |
				((u32_t) nfcid_remaining_data[2] <<
						NFCID1_LAST_BYTE1_SHIFT) |
				((u32_t) nfcid_remaining_data[3] <<
						NFCID1_LAST_BYTE0_SHIFT);

	/* Begin: Bugfix for FTPAN-25 (IC-9929) */
	/* Workaround for wrong SENSRES values require using SDD00001, but here
	 * SDD00100 is used because it's required to operate with Windows Phone
	 */
	NRF_NFCT->SENSRES = (sens_res_size << NFCT_SENSRES_NFCIDSIZE_Pos) |
			    (NFCT_SENSRES_BITFRAMESDD_SDD00100 <<
						NFCT_SENSRES_BITFRAMESDD_Pos);
	/* End:   Bugfix for FTPAN-25 (IC-9929)*/
}

/** @brief Call nfc_t2t_internal_set() with data generated based on
 *  m_nfcid1_data and m_nfcid1_length variables.
 */
static void hal_nfc_internal_bytes_setup(void)
{
	if (m_nfcid1_length == NFCID1_TRIPLE_SIZE) {
		(void) nfc_t2t_internal_set(m_nfcid1_data,
					    T2T_INTERNAL_BYTES_NR);
	} else {
		u8_t nfc_internal[T2T_INTERNAL_BYTES_NR];

		if (m_nfcid1_length == NFCID1_DOUBLE_SIZE) {
			/* CASCADE TAG send over the air, but not included in
			 * internal data
			 */
			/* UID0 = Manufacturer ID */
			nfc_internal[0] = m_nfcid1_data[0];
			/* UID1 */
			nfc_internal[1] = m_nfcid1_data[1];
			/* UID2 */
			nfc_internal[2] = m_nfcid1_data[2];
			/* BCC0 = XOR over the 4 previous bytes */
			nfc_internal[3] = (u8_t)CASCADE_TAG_BYTE ^
						nfc_internal[0] ^
						nfc_internal[1] ^
						nfc_internal[2];
			/* UID3 */
			nfc_internal[4] = m_nfcid1_data[3];
			/* UID4 */
			nfc_internal[5] = m_nfcid1_data[4];
			/* UID5 */
			nfc_internal[6] = m_nfcid1_data[5];
			/* UID6 */
			nfc_internal[7] = m_nfcid1_data[6];
			/* BCC1 = XOR over the 4 previous bytes */
			nfc_internal[8] = nfc_internal[4] ^ nfc_internal[5] ^
					  nfc_internal[6] ^ nfc_internal[7];
			/* For internal use */
			nfc_internal[9] = NFC_LIB_VERSION;
		} else {
			/* then m_nfcid1_length == NFCID1_SINGLE_SIZE */

			/* UID0 = Manufacturer ID */
			nfc_internal[0] = m_nfcid1_data[0];
			/* UID1 */
			nfc_internal[1] = m_nfcid1_data[1];
			/* UID2 */
			nfc_internal[2] = m_nfcid1_data[2];
			/* UID3 */
			nfc_internal[3] = m_nfcid1_data[3];
			/* BCC0 = XOR over the 4 previous bytes */
			nfc_internal[4] = nfc_internal[0] ^ nfc_internal[1] ^
					  nfc_internal[2] ^ nfc_internal[3];
			/* 4-byte padding */
			memset(&nfc_internal[5], 0xFF, 4);
			/* For internal use */
			nfc_internal[9] = NFC_LIB_VERSION;
		}

		(void) nfc_t2t_internal_set(nfc_internal,
					    T2T_INTERNAL_BYTES_NR);
	}
}

int hal_nfc_setup(hal_nfc_callback_t callback, void *context)
{
	m_nfc_lib_callback = callback;
	m_nfc_lib_context  = context;

	if (m_nfcid1_length == 0) {
		m_nfcid1_length = NFCID1_DEFAULT_LENGHT;
		hal_nfc_nfcid1_default_bytes();
	}

	hal_nfc_common_hw_setup();
	hal_nfc_internal_bytes_setup();

	/* Clock workaround. Clock module initialization and start of HFCLK */
	hf_clk = device_get_binding(CONFIG_CLOCK_CONTROL_NRF5_M16SRC_DRV_NAME);
	if (!hf_clk) {
		LOG_ERR("NRF5 HF Clock device not found!");
		return -ENODEV;
	}

	int err = clock_control_on(hf_clk, (void *)true);

	if (err < 0) {
		LOG_ERR("NRF5 HF clock start fail");
	}
	/*TODO: HFCLK driver configuration */

#if defined(HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND) || \
	defined(HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND)
#if defined(HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND)
	if (!type_52840_sample_check())
#endif /* defined(HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND) */
	{
		field_timer_with_callback_config();
	}
#endif /* defined(HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND) ||
	* defined(HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND)
	*/

	return err;
}

/**@brief Function for clearing an event flag in NRF_NFCT registers.
 *
 * @param event Pointer to event register.
 *
 */
static inline void nrf_nfct_event_clear(volatile u32_t *event)
{
	*event = 0;

	/* Perform read to ensure clearing is effective */
	volatile u32_t dummy = *event;
	(void) dummy;
}

/**@brief Function for handling events from Clock Module.
 *
 * @param event Clock event.
 *
 */

void hal_nrf_clock_event_cb(nrf_clock_event_t event)
{
	switch (event) {
	case NRF_CLOCK_EVENT_HFCLKSTARTED:
		/* Activate NFCT only when HFXO is running */

#ifdef HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND
		if (type_52840_final_check()) {
			hal_nfc_activate_check();
		} else
#endif /* HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND */
		{
			NRF_NFCT->TASKS_ACTIVATE = 1;
		}
		break;

	default:
		/* No implementation required */
		break;
	}
}

#ifdef HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND
static inline void nrf_nfct_field_lost_hfclk_handle(void)
{
	/* Begin:   Bugfix for FTPAN-116 (IC-12886) */
	/* reset the NFC for release HFCLK */
	__DMB();
	(*(uint32_t volatile *)(0x40005FFC)) = 0UL;
	__DMB();
	(*(uint32_t volatile *)(0x40005FFC)) = 1UL;
	/* END:   Bugfix for FTPAN-116 (IC-12886) */

}
#endif /* HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND */

#ifndef HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND
/**@brief Function for evaluating and handling NFC field events.
 *
 * @param field_state Current field state.
 *
 */
void nrf_nfct_field_event_handler(
			volatile enum nfct_field_sense_state field_state)
{
#ifdef HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND
	bool is_first_sample = type_52840_sample_check();

	if ((!is_first_sample) && (m_nfc_fieldevents_filter_active)) {
		return;
	}
#endif /* HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND */

	if (field_state == NFC_FIELD_STATE_UNKNOWN) {
		/* Probe NFC field */
		u32_t field_present = NRF_NFCT->FIELDPRESENT;

		if (field_present & m_nfc_fieldpresent_mask) {
			field_state = NFC_FIELD_STATE_ON;
		} else {
			field_state = NFC_FIELD_STATE_OFF;
		}
	}

	/* Field event service */
	switch (field_state) {
	case NFC_FIELD_STATE_ON:
		if (!m_field_on) {
#ifdef HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND
			/* Begin: Bugfix for FTPAN-190 */
			if (!is_first_sample) {
				m_nfc_activate_conditions = 0;
				m_nfc_fieldevents_filter_active = true;

				NRF_TIMER4->TASKS_CLEAR = 1;
				NRF_TIMER4->TASKS_START = 1;
			}
			/* END: Bugfix for FTPAN-190 */
#endif /* HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND */
			/* Clock workaround for lacking clock driver.
			 * TODO: HFCLK clock request
			 */
			hal_nrf_clock_event_cb(NRF_CLOCK_EVENT_HFCLKSTARTED);
		}
		m_field_on = true;
		break;

	case NFC_FIELD_STATE_OFF:
#ifdef HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND
		/* Begin: Bugfix for FTPAN-116 (IC-12886) */
		if (is_first_sample) {
			*(volatile u32_t *)0x40005010 = 1;
		}
		/* END: Bugfix for FTPAN-116 (IC-12886) */
#endif /* HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND */

		NRF_NFCT->TASKS_SENSE = 1;
		/* TODO: HFCLK release */
		m_field_on = false;

		NRF_NFCT->INTENCLR = (NFCT_INTENCLR_RXFRAMEEND_Clear <<
						NFCT_INTENCLR_RXFRAMEEND_Pos) |
				     (NFCT_INTENCLR_RXERROR_Clear <<
						NFCT_INTENCLR_RXERROR_Pos);

		/* Change mask to FIELD_OFF state - trigger FIELD_ON even if HW
		 *  has not locked to the field
		 */
		m_nfc_fieldpresent_mask = NFC_FIELD_OFF_MASK;

		if (m_nfc_lib_callback != NULL) {
			m_nfc_lib_callback(m_nfc_lib_context,
					   HAL_NFC_EVENT_FIELD_OFF,
					   0,
					   0);
		}
		break;

	default:
		/* No implementation required */
		break;
	}
}
#endif /* HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND */

/** @brief Writes default values to m_nfcid1_data based on NRF_FICR->NFC
 *  registers.
 */
static void hal_nfc_nfcid1_default_bytes(void)
{
	u32_t nfc_tag_header0 = NRF_FICR->NFC.TAGHEADER0;
	u32_t nfc_tag_header1 = NRF_FICR->NFC.TAGHEADER1;
	u32_t nfc_tag_header2 = NRF_FICR->NFC.TAGHEADER2;

	m_nfcid1_data[0] = (u8_t) (nfc_tag_header0 >> 0);
	m_nfcid1_data[1] = (u8_t) (nfc_tag_header0 >> 8);
	m_nfcid1_data[2] = (u8_t) (nfc_tag_header0 >> 16);
	m_nfcid1_data[3] = (u8_t) (nfc_tag_header1 >> 0);
	m_nfcid1_data[4] = (u8_t) (nfc_tag_header1 >> 8);
	m_nfcid1_data[5] = (u8_t) (nfc_tag_header1 >> 16);
	m_nfcid1_data[6] = (u8_t) (nfc_tag_header1 >> 24);
	m_nfcid1_data[7] = (u8_t) (nfc_tag_header2 >> 0);
	m_nfcid1_data[8] = (u8_t) (nfc_tag_header2 >> 8);
	m_nfcid1_data[9] = (u8_t) (nfc_tag_header2 >> 16);

	/* Begin: Bugfix for FTPAN-181. */
	if (m_nfcid1_data[3] == 0x88) {
		m_nfcid1_data[3] |= 0x11;
	}
	/* End: Bugfix for FTPAN-181 */
}

/** @brief Resets NFCT peripheral to its default state before automatic
 *  collision resolution procedure.
 */
static inline void nrf_nfct_default_state_reset(void)
{
#if defined(NRF52832_XXAA) || defined(NRF52832_XXAB)
	if (((*(uint32_t volatile *)(0x40005420)) & 0x1UL) == (1UL))
#else
	if (NRF_NFCT->SLEEPSTATE & NFCT_SLEEPSTATE_SLEEPSTATE_Msk)
	/* Default state is SLEEP_A */
#endif /* defined(NRF52832_XXAA) || defined(NRF52832_XXAB) */
	{
		NRF_NFCT->TASKS_GOSLEEP = 1;
	} else { /* Default state is IDLE */
		NRF_NFCT->TASKS_GOIDLE = 1;
	}

	/* Disable RX here (will be enabled at SELECTED) */
	NRF_NFCT->INTENCLR = NFCT_INTENCLR_RXFRAMEEND_Clear <<
				NFCT_INTENCLR_RXFRAMEEND_Pos;
}

int hal_nfc_parameter_set(enum hal_nfc_param_id id,
			  void *data,
			  size_t data_length)
{
	if (id == HAL_NFC_PARAM_ID_NFCID1) {
		if (data_length == 1) {
			u8_t id_length = *((u8_t *) data);

			if (id_length == NFCID1_SINGLE_SIZE ||
			    id_length == NFCID1_DOUBLE_SIZE ||
			    id_length == NFCID1_TRIPLE_SIZE) {
				m_nfcid1_length = id_length;
				hal_nfc_nfcid1_default_bytes();
			} else {
				return -EINVAL;
			}
		} else if (data_length == NFCID1_SINGLE_SIZE ||
			   data_length == NFCID1_DOUBLE_SIZE ||
			   data_length == NFCID1_TRIPLE_SIZE) {
			m_nfcid1_length = (u8_t) data_length;
			memcpy(m_nfcid1_data, data, data_length);
		} else {
			return -EINVAL;
		}
		hal_nfc_nfcid1_registers_setup();
		hal_nfc_internal_bytes_setup();
	}

	return 0;
}

int hal_nfc_parameter_get(enum hal_nfc_param_id id,
			  void *data,
			  size_t *max_data_length)
{
	if (id == HAL_NFC_PARAM_ID_NFCID1) {
		if (m_nfcid1_length == 0) {
			m_nfcid1_length = NFCID1_DEFAULT_LENGHT;
			hal_nfc_nfcid1_default_bytes();
		}

		if (*max_data_length < (size_t) m_nfcid1_length) {
			return -EINVAL;
		}

		*max_data_length = (size_t) m_nfcid1_length;
		memcpy(data, m_nfcid1_data, m_nfcid1_length);
	}

	return 0;
}

int hal_nfc_start(void)
{
	NRF_NFCT->ERRORSTATUS = NRF_NFCT_ERRORSTATUS_ALL;
	NRF_NFCT->TASKS_SENSE = 1;

	NVIC_ClearPendingIRQ(NFCT_IRQn);
	IRQ_DIRECT_CONNECT(NFCT_IRQn,
			   CONFIG_NFCT_IRQ_PRIORITY,
			   NFCT_IRQHandler,
			   0);
	irq_enable(NFCT_IRQn);

	LOG_INF("Start");
	return 0;
}

int hal_nfc_send(const u8_t *data, size_t data_length)
{
	if (data_length == 0) {
		return -EINVAL;
	}

	/* Ignore previous TX END events, SW takes care only for data frames
	 * which tranmission is triggered in this function
	 */
	nrf_nfct_event_clear(&NRF_NFCT->EVENTS_TXFRAMEEND);

	NRF_NFCT->PACKETPTR = (u32_t) data;
	NRF_NFCT->TXD.AMOUNT = (data_length <<
					NFCT_TXD_AMOUNT_TXDATABYTES_Pos) &
			       NFCT_TXD_AMOUNT_TXDATABYTES_Msk;
	NRF_NFCT->INTENSET = (NFCT_INTENSET_TXFRAMEEND_Enabled <<
					NFCT_INTENSET_TXFRAMEEND_Pos);
	NRF_NFCT->TASKS_STARTTX = 1;

	LOG_INF("Send");
	return 0;
}

int hal_nfc_stop(void)
{
	NRF_NFCT->TASKS_DISABLE = 1;

	LOG_INF("Stop");
	return 0;
}

int hal_nfc_done(void)
{
	m_nfc_lib_callback = (hal_nfc_callback_t) NULL;

	return 0;
}

void NFCT_IRQHandler(void)
{
	enum nfct_field_sense_state current_field = NFC_FIELD_STATE_NONE;

	if (NRF_NFCT->EVENTS_FIELDDETECTED &&
	    (NRF_NFCT->INTEN & NFCT_INTEN_FIELDDETECTED_Msk)) {
		nrf_nfct_event_clear(&NRF_NFCT->EVENTS_FIELDDETECTED);
		current_field = NFC_FIELD_STATE_ON;
		LOG_DBG("Field detected");
	}

#ifndef HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND
	if (NRF_NFCT->EVENTS_FIELDLOST &&
			(NRF_NFCT->INTEN & NFCT_INTEN_FIELDLOST_Msk)) {
		nrf_nfct_event_clear(&NRF_NFCT->EVENTS_FIELDLOST);
		current_field = (current_field == NFC_FIELD_STATE_NONE) ?
				NFC_FIELD_STATE_OFF : NFC_FIELD_STATE_UNKNOWN;

		LOG_DBG("Field lost");
	}
#endif /* HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND */

	/* Perform actions if any FIELD event is active */
	if (current_field != NFC_FIELD_STATE_NONE) {
		nrf_nfct_field_event_handler(current_field);
	}

	if (NRF_NFCT->EVENTS_RXFRAMEEND
			&& (NRF_NFCT->INTEN & NFCT_INTEN_RXFRAMEEND_Msk)) {
		/* Take into account only number of whole bytes */
		u32_t rx_status = 0;
		u32_t rx_data_size = ((NRF_NFCT->RXD.AMOUNT &
					NFCT_RXD_AMOUNT_RXDATABYTES_Msk) >>
				      NFCT_RXD_AMOUNT_RXDATABYTES_Pos) -
				     NFC_CRC_SIZE;
		nrf_nfct_event_clear(&NRF_NFCT->EVENTS_RXFRAMEEND);

		if (NRF_NFCT->EVENTS_RXERROR &&
		    (NRF_NFCT->INTEN & NFCT_INTEN_RXERROR_Msk)) {
			rx_status = (NRF_NFCT->FRAMESTATUS.RX &
				     NRF_NFCT_FRAMESTATUS_RX_MSK);
			nrf_nfct_event_clear(&NRF_NFCT->EVENTS_RXERROR);

			LOG_DBG("Rx error (0x%x)", (unsigned int) rx_status);

			/* Clear rx frame status */
			NRF_NFCT->FRAMESTATUS.RX = NRF_NFCT_FRAMESTATUS_RX_MSK;
		}

		/* Go back to idle state if NFC-A frame with Transmission Errors
		 * occurs.
		 */
		if (rx_status) {
			nrf_nfct_default_state_reset();
		} else {
			/* Look for Tag 2 Type READ Command */
			if (m_nfc_rx_buffer[0] == T2T_READ_CMD) {
				if (m_nfc_lib_callback != NULL) {
					/* This callback should trigger
					 * transmission of READ Response
					 */
					m_nfc_lib_callback(m_nfc_lib_context,
						    HAL_NFC_EVENT_DATA_RECEIVED,
						    (void *) m_nfc_rx_buffer,
						    rx_data_size);
				}
			} else {
				/* Indicate that SLP_REQ was received - this
				 * will cause FRAMEDELAYTIMEOUT error
				 */
				if (m_nfc_rx_buffer[0] == NFC_SLP_REQ_CMD) {
					m_slp_req_received = true;
					/* No need to parse incoming frames,
					 * wait for SELECTED
					 */
					NRF_NFCT->INTENCLR =
					      NFCT_INTENCLR_RXFRAMEEND_Clear <<
					      NFCT_INTENCLR_RXFRAMEEND_Pos;
				} else {
					nrf_nfct_default_state_reset();
				}
			}
		}

		LOG_DBG("Rx fend");
	}

	if (NRF_NFCT->EVENTS_TXFRAMEEND
			&& (NRF_NFCT->INTEN & NFCT_INTEN_TXFRAMEEND_Msk)) {
		nrf_nfct_event_clear(&NRF_NFCT->EVENTS_TXFRAMEEND);

		/* Disable TX END event to ignore frame transmission other than
		 * READ response
		 */
		NRF_NFCT->INTENCLR = (NFCT_INTENCLR_TXFRAMEEND_Clear
				<< NFCT_INTENCLR_TXFRAMEEND_Pos);

		/* Set up for reception */
		NRF_NFCT->PACKETPTR = (u32_t) m_nfc_rx_buffer;
		NRF_NFCT->MAXLEN = NFC_RX_BUFFER_SIZE;
		NRF_NFCT->TASKS_ENABLERXDATA = 1;

		if (m_nfc_lib_callback != NULL) {
			m_nfc_lib_callback(m_nfc_lib_context,
					   HAL_NFC_EVENT_DATA_TRANSMITTED,
					   0,
					   0);
		}

		LOG_DBG("Tx fend");
	}

	if (NRF_NFCT->EVENTS_SELECTED
			&& (NRF_NFCT->INTEN & NFCT_INTEN_SELECTED_Msk)) {
		nrf_nfct_event_clear(&NRF_NFCT->EVENTS_SELECTED);
		/* Clear also RX END and RXERROR events because SW does not
		 * take care of commands which were received before selecting
		 * the tag
		 */
		nrf_nfct_event_clear(&NRF_NFCT->EVENTS_RXFRAMEEND);
		nrf_nfct_event_clear(&NRF_NFCT->EVENTS_RXERROR);

		/* Set up registers for EasyDMA and start receiving packets */
		NRF_NFCT->PACKETPTR = (u32_t) m_nfc_rx_buffer;
		NRF_NFCT->MAXLEN = NFC_RX_BUFFER_SIZE;
		NRF_NFCT->TASKS_ENABLERXDATA = 1;

		NRF_NFCT->INTENSET = (NFCT_INTENSET_RXFRAMEEND_Enabled
					<< NFCT_INTENSET_RXFRAMEEND_Pos) |
				     (NFCT_INTENSET_RXERROR_Enabled
					<< NFCT_INTENSET_RXERROR_Pos);

		/* At this point any previous error status can be ignored */
		NRF_NFCT->FRAMESTATUS.RX = NRF_NFCT_FRAMESTATUS_RX_MSK;
		NRF_NFCT->ERRORSTATUS = NRF_NFCT_ERRORSTATUS_ALL;

#ifndef HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND
		/* Change mask to FIELD_ON state - trigger FIELD_ON only if HW
		 * has locked to the field
		 */
		m_nfc_fieldpresent_mask = NFC_FIELD_ON_MASK;
#endif

		if (m_nfc_lib_callback != NULL) {
			m_nfc_lib_callback(m_nfc_lib_context,
					   HAL_NFC_EVENT_FIELD_ON,
					   0,
					   0);
		}

		LOG_DBG("Selected");
	}

	if (NRF_NFCT->EVENTS_ERROR
			&& (NRF_NFCT->INTEN & NFCT_INTEN_ERROR_Msk)) {
		u32_t err_status = NRF_NFCT->ERRORSTATUS;

		nrf_nfct_event_clear(&NRF_NFCT->EVENTS_ERROR);

		/* Clear FRAMEDELAYTIMEOUT error (expected HW behaviour) when
		 * SLP_REQ command was received
		 */
		if ((err_status & NFCT_ERRORSTATUS_FRAMEDELAYTIMEOUT_Msk)
				&& m_slp_req_received) {
			NRF_NFCT->ERRORSTATUS =
					NFCT_ERRORSTATUS_FRAMEDELAYTIMEOUT_Msk;
			m_slp_req_received = false;

			LOG_DBG("RX: SLP_REQ");
		}
		/* Report any other error */
		err_status &= ~NFCT_ERRORSTATUS_FRAMEDELAYTIMEOUT_Msk;
		if (err_status) {
			LOG_DBG("Error (0x%x)", (unsigned int) err_status);
		}

		/* Clear error status */
		NRF_NFCT->ERRORSTATUS = NRF_NFCT_ERRORSTATUS_ALL;
	}
}

#ifdef HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND
#ifdef HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND
	#error Wrong workaround combination
#endif

static u32_t field_state_cnt;
/**
 * @brief Function for evaluating and handling NFC fieldlost event.
 */
static void hal_nfc_field_check(void)
{
	u32_t nfc_fieldpresen_masked;

	nfc_fieldpresen_masked = NRF_NFCT->FIELDPRESENT &
				 NFC_HAL_FIELDPRESENT_MASK;

	if (nfc_fieldpresen_masked == NFC_HAL_FIELDPRESENT_IS_LOST) {
		++field_state_cnt;
		if (field_state_cnt > 7) {
			NRF_TIMER4->TASKS_SHUTDOWN = 1;
			/* TODO: HFCLK release */
			nrf_nfct_field_lost_hfclk_handle();

			if (m_nfc_lib_callback != NULL) {
				m_nfc_lib_callback(m_nfc_lib_context,
						   HAL_NFC_EVENT_FIELD_OFF,
						   0,
						   0);
			}
			m_field_on = false;

			/* Begin:   Bugfix for FTPAN-116 (IC-12886) */
			/* resume the NFCT to initialized state */
			hal_nfc_re_setup();
			/* End:   Bugfix for FTPAN-116 (IC-12886) */
		}

		return;
	}

	field_state_cnt = 0;
}

/**
 * @brief Function for enablinge hight precision clock and start eveluating
 * fieldlost event.
 */
static inline void nrf_nfct_field_event_handler(
			volatile enum nfct_field_sense_state field_state)
{
	if (!m_field_on) {
		/* Clock workaround for lacking clock driver.
		 * TODO: HFCLK clock request
		 */
		hal_nrf_clock_event_cb(NRF_CLOCK_EVENT_HFCLKSTARTED);

		NRF_TIMER4->TASKS_CLEAR = 1;
		NRF_TIMER4->TASKS_START = 1;
		field_state_cnt = 0;
	}
	m_field_on = true;
}

/**
 * @brief Function for resume the NFCT to initialized state after software's
 * reset.
 */
static inline void hal_nfc_re_setup(void)
{
	hal_nfc_common_hw_setup();

	LOG_INF("Reinitialize");
}
#endif /* HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND */
