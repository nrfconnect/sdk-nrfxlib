/*$$$LICENCE_NORDIC_STANDARD<2016>$$$*/

#include <stdbool.h>
#include <string.h>
#include "errno.h"

#include <nfc/hal_nfc_t4t.h>
#include <nfc/nfc_t4t_lib.h>
#include <nfc/nfc_fixes.h>

#include <nrf_clock.h>
#include <clock_control.h>
#include <device.h>
#include <irq.h>

#define LOG_MODULE_NAME hal_nfc_t4t
#define LOG_LEVEL CONFIG_HAL_NFC_LOG_LEVEL

#include <logging/log.h>
LOG_MODULE_REGISTER();

/* NFC library version history:
 * #define NFC_LIB_VERSION          0x00
 * first experimental version intended for nRF52840 IC rev. Engineering A
 * (PCA10056, part of nRF52840 Preview Development Kit)
 */

#define NFC_LIB_VERSION             0x00u
/**< Internal: current NFC lib. version  */

#define CASCADE_TAG_BYTE            0x88u
/**< Constant defined by ISO/EIC 14443-3 */
#define NFCID1_3RD_LAST_BYTE2_SHIFT 16u
/**< Shift value for NFC ID byte 2 */
#define NFCID1_3RD_LAST_BYTE1_SHIFT 8u
/**< Shift value for NFC ID byte 1 */
#define NFCID1_3RD_LAST_BYTE0_SHIFT 0u
/**< Shift value for NFC ID byte 0 */
#define NFCID1_2ND_LAST_BYTE2_SHIFT 16u
/**< Shift value for NFC ID byte 2 */
#define NFCID1_2ND_LAST_BYTE1_SHIFT 8u
/**< Shift value for NFC ID byte 1 */
#define NFCID1_2ND_LAST_BYTE0_SHIFT 0u
/**< Shift value for NFC ID byte 0 */
#define NFCID1_LAST_BYTE3_SHIFT     24u
/**< Shift value for NFC ID byte 3 */
#define NFCID1_LAST_BYTE2_SHIFT     16u
/**< Shift value for NFC ID byte 2 */
#define NFCID1_LAST_BYTE1_SHIFT     8u
/**< Shift value for NFC ID byte 1 */
#define NFCID1_LAST_BYTE0_SHIFT     0u
/**< Shift value for NFC ID byte 0 */
#define NFCID1_SINGLE_SIZE          4u
/**< Length of single size NFCID1 */
#define NFCID1_DOUBLE_SIZE          7u
/**< Length of double size NFCID1 */
#define NFCID1_TRIPLE_SIZE          10u
/**< Length of triple size NFCID1 */
#define NFCID1_DEFAULT_LENGHT       NFCID1_DOUBLE_SIZE
/**< Length of NFCID1 if user does not provide one */
#define NFCID1_MAX_LENGHT           NFCID1_TRIPLE_SIZE
/**< Maximum length of NFCID1 */
#define NFC_RX_BUFFER_SIZE          256u
/**< NFC Rx data buffer size */
#define NFC_SLP_REQ_CMD             0x50u
/**< NFC SLP_REQ command identifier */
#define NFC_CRC_SIZE                2u
/**< CRC size in bytes */
#define NFC_T4T_SELRES_PROTOCOL     1u
/**< Type 4A Tag PROTOCOL bit setup (b7:b6) for SEL_RES Response frame */
#define NFC_T4T_SELRES_PROTOCOL_MSK 0x03u
/**< PROTOCOL bits mask for SEL_RES Response frame */

#ifdef CONFIG_SOC_NRF52840
#define NFC_T4T_FWI_MAX         7u /**< Maximum FWI parameter value for 52840 */
#else
#define NFC_T4T_FWI_MAX         4u /**< Maximum FWI parameter value */
#endif /* CONFIG_SOC_NRF52840 */

#define NFC_T4T_FWI_52840S_MAX      4u
/**< Maximum FWI parameter value for first sample of 52840 */
#define NFCT_FRAMEDELAYMAX_52840S   (0xFFFFUL)
/**< Bit mask of FRAMEDELAYMAX field for first sample of 52840 */
#define NFC_T4T_FWI_DEFAULT         4u
/**< Default FWI parameter value */
#define NFC_T4T_RATS_CMD            0xE0u
/**< RATS Command Byte */
#define NFC_T4T_RATS_DID_MASK       0x0Fu
/**< Mask of DID field inside RATS Parameter Byte. */
#define NFC_T4T_RATS_DID_RFU        0x0Fu
/**< Invalid value of DID - RFU. */
#define NFC_T4T_S_DESELECT          0xC2u
/**< S(DESELECT) Block identifier */
#define NFC_T4T_S_BLOCK_MSK         0xF7u
/**< S-Block Mask */
#define NFCT_INTEN_MSK              0x1C5CFFu
/**< Mask for all NFCT interrupts */

#ifdef HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND
#define NRF_NFCT_ERRORSTATUS_ALL (NFCT_ERRORSTATUS_FRAMEDELAYTIMEOUT_Msk)
/**< Mask for clearing all error flags in NFCT_ERRORSTATUS register */
#else
#define NRF_NFCT_ERRORSTATUS_ALL (NFCT_ERRORSTATUS_NFCFIELDTOOWEAK_Msk   | \
				  NFCT_ERRORSTATUS_NFCFIELDTOOSTRONG_Msk | \
				  NFCT_ERRORSTATUS_FRAMEDELAYTIMEOUT_Msk)
/**< Mask for clearing all error flags in NFCT_ERRORSTATUS register */
#endif /* HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND */

#define NRF_NFCT_FRAMESTATUS_RX_MSK (NFCT_FRAMESTATUS_RX_OVERRUN_Msk      | \
				     NFCT_FRAMESTATUS_RX_PARITYSTATUS_Msk | \
				     NFCT_FRAMESTATUS_RX_CRCERROR_Msk)
/**< Mask for clearing all flags in NFCT_FRAMESTATUS_RX register */
#define NFC_FIELD_ON_MASK            NFCT_FIELDPRESENT_LOCKDETECT_Msk
/**< Mask for checking FIELDPRESENT register for state: FIELD ON. */
#define NFC_FIELD_OFF_MASK           NFCT_FIELDPRESENT_FIELDPRESENT_Msk
/**< Mask for checking FIELDPRESENT register for state: FIELD OFF. */

#define NFC_T4T_FWI_TO_FWT(FWI)      (256u * 16u * (1 << (FWI)))
/**< Macro for calculating FWT (in number of NFC carrier periods) from FWI
 *   parameter.
 */

#ifdef HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND
#define NRF_NFCT_ACTIVATE_CONDS_THR    2
/**< Number of required conditions to activate NFCT. */
#define NRF_NFCT_ACTIVATE_DELAY        1000
/**< Minimal delay in us between NFC field detection and activation of NFCT. */
#endif /* HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND */

enum nfct_field_sense_state {
	NFC_FIELD_STATE_NONE,
	/**< Initial value indicating no NFCT Field events. */
	NFC_FIELD_STATE_OFF,
	/**< NFCT FIELDLOST Event has been set. */
	NFC_FIELD_STATE_ON,
	/**< NFCT FIELDDETECTED Event has been set. */
	NFC_FIELD_STATE_UNKNOWN
	/**< Both NFCT Field Events have been set - ambiguous state. */
};

/* Static function declarations */
static inline void nrf_nfct_event_clear(volatile u32_t *event);
static inline void nrf_nfct_field_event_handler(
			volatile enum nfct_field_sense_state field_state);
static void hal_nfc_nfcid1_default_bytes(void);
static void hal_nfc_nfcid1_registers_setup(void);
void NFCT_IRQHandler(void);
/* Static data */
static hal_nfc_callback_t m_nfc_lib_callback = (hal_nfc_callback_t) NULL;
/**< Callback to nfc_lib layer */
static void *m_nfc_lib_context;
/**< Callback execution context */
static volatile u8_t m_nfc_rx_buffer[NFC_RX_BUFFER_SIZE] = {0};
/**< Buffer for NFC Rx data */
static volatile bool m_slp_req_received;
/**< Flag indicating that SLP_REQ Command was received */
static volatile bool m_field_on;
/**< Flag indicating that NFC Tag field is present */
static struct device *hf_clk = (struct device *) NULL;
/**< Pointer to HF Clock device */
static volatile u8_t m_fwi = NFC_T4T_FWI_DEFAULT;
/**< FWI parameter */
static volatile bool m_deselect;
/**< Flag indicating reception of DESELECT command */
static volatile u8_t m_t4t_selres = NFC_T4T_SELRES_PROTOCOL;
/**< Protocol bits setup in SEL_RES frame - can be modified using the library
 *   API
 */
static u8_t m_nfcid1_length;
/**< Length of NFCID1 provided by user or 0 if not initialized yet */
static u8_t m_nfcid1_data[NFCID1_MAX_LENGHT] = {0};
/**< Content of NFCID1 */
static volatile u8_t m_t4t_active;
/**< Indicates if NFC Tag is in 4A state (on reception of correct RATS command).
 */

#ifdef HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND
static volatile bool m_nfc_fieldevents_filter_active;
/**< Flag indicating that field events are ignored. */
static volatile u32_t m_nfc_activate_conditions;
/**< Number of activation conditions that are met. */
#endif /* HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND */

#ifndef HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND
static volatile u32_t m_nfc_fieldpresent_mask = NFC_FIELD_OFF_MASK;
/**< Mask used for NFC Field polling in NFCT_FIELDPRESENT register */
#endif /* HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND */

#ifdef HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND

static inline void hal_nfc_re_setup(void);
static void hal_nfc_field_check(void);

#define NFC_HAL_FIELDPRESENT_MASK    (NFCT_FIELDPRESENT_LOCKDETECT_Msk | \
				      NFCT_FIELDPRESENT_FIELDPRESENT_Msk)

#define NFC_HAL_FIELDPRESENT_IS_LOST (					       \
				(NFCT_FIELDPRESENT_FIELDPRESENT_NoField <<     \
					 NFCT_FIELDPRESENT_FIELDPRESENT_Pos) | \
				(NFCT_FIELDPRESENT_LOCKDETECT_NotLocked <<     \
					 NFCT_FIELDPRESENT_LOCKDETECT_Pos))

#ifndef HAL_NFC_FIELD_TIMER_PERIOD
#define HAL_NFC_FIELD_TIMER_PERIOD    100  /* unit - us */
#endif

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

#if defined(HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND) ||			\
		defined(HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND)

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



#endif /* defined(HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND) ||
	*	defined(HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND)
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

	/* Begin: Bugfix for FTPAN-17 */
	/* fixed by avoiding usage of FIELDLOST event */
#ifdef HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND
	NRF_NFCT->INTENSET = (NFCT_INTENSET_FIELDDETECTED_Enabled <<
					NFCT_INTENSET_FIELDDETECTED_Pos);
#else
	NRF_NFCT->INTENSET = (NFCT_INTENSET_FIELDDETECTED_Enabled <<
					NFCT_INTENSET_FIELDDETECTED_Pos) |
			     (NFCT_INTENSET_FIELDLOST_Enabled <<
					NFCT_INTENSET_FIELDLOST_Pos);
#endif /* HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND */
	/* End:   Bugfix for FTPAN-17 */

	NRF_NFCT->INTENSET = (NFCT_INTENSET_TXFRAMESTART_Enabled <<
					NFCT_INTENSET_TXFRAMESTART_Pos);

	NRF_NFCT->INTENSET = (NFCT_INTENSET_ERROR_Enabled <<
					NFCT_INTENSET_ERROR_Pos) |
			     (NFCT_INTENSET_SELECTED_Enabled <<
					NFCT_INTENSET_SELECTED_Pos);

	hal_nfc_nfcid1_registers_setup();

	/* Set FRAMEDELAYMAX to default setting */
	int err = hal_nfc_parameter_set(HAL_NFC_PARAM_FWI,
					     (void *) &m_fwi,
					     sizeof(m_fwi));
	if (err < 0) {
		LOG_ERR("Cannot set HAL's parameter");
	}

	/* Set PROTOCOL bits for Type 4A Tag */
	NRF_NFCT->SELRES =
	(m_t4t_selres << NFCT_SELRES_PROTOCOL_Pos) & NFCT_SELRES_PROTOCOL_Msk;
}

/** @brief Setup NRF_NFCT->NFCID1 and NRF_NFCT->SENSRES registers based on
 *  m_nfcid1_data and m_nfcid1_length variables.
 */
static void hal_nfc_nfcid1_registers_setup(void)
{
	/* Value that will be written toNRF_NFCT->SENSRES */
	u32_t sens_res_size;
	/* Points to the first byte of m_nfcid1_data remaining to write to
	 *  NRF_NFCT->NFCID1 registers
	 */
	u8_t *nfcid_remaining_data;

	nfcid_remaining_data = m_nfcid1_data;

	if (m_nfcid1_length == NFCID1_SINGLE_SIZE) {
		sens_res_size = NFCT_SENSRES_NFCIDSIZE_NFCID1Single;
	} else {
		if (m_nfcid1_length == NFCID1_DOUBLE_SIZE) {
			sens_res_size = NFCT_SENSRES_NFCIDSIZE_NFCID1Double;
		} else { /* then m_nfcid1_length == NFCID1_TRIPLE_SIZE */

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

	NRF_NFCT->NFCID1_LAST =
				((u32_t) nfcid_remaining_data[0] <<
						NFCID1_LAST_BYTE3_SHIFT) |
				((u32_t) nfcid_remaining_data[1] <<
						NFCID1_LAST_BYTE2_SHIFT) |
				((u32_t) nfcid_remaining_data[2] <<
						NFCID1_LAST_BYTE1_SHIFT) |
				((u32_t) nfcid_remaining_data[3] <<
						NFCID1_LAST_BYTE0_SHIFT);

	/* Begin: Bugfix for FTPAN-25 (IC-9929) */
	/* Workaround for wrong SENSRES values require using SDD00001, but here
	 *  SDD00100 is used because it's required to operate with Windows Phone
	 */
	NRF_NFCT->SENSRES = (sens_res_size << NFCT_SENSRES_NFCIDSIZE_Pos) |
			    (NFCT_SENSRES_BITFRAMESDD_SDD00100 <<
					    NFCT_SENSRES_BITFRAMESDD_Pos);
	/* End:   Bugfix for FTPAN-25 (IC-9929)*/

	m_t4t_active = false;
}

int hal_nfc_setup(hal_nfc_callback_t callback, void *context)
{
	m_nfc_lib_callback = callback;
	m_nfc_lib_context = context;

	if (m_nfcid1_length == 0) {
		m_nfcid1_length = NFCID1_DEFAULT_LENGHT;
		hal_nfc_nfcid1_default_bytes();
	}

	hal_nfc_common_hw_setup();

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

#if defined(HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND) ||			      \
			defined(HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND)
#if defined(HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND)
	if (!type_52840_sample_check())
#endif /* defined(HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND) */
	{
		field_timer_with_callback_config();
	}
#endif /* defined(HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND) ||
	*		defined(HAL_NFC_NRF52840_ENGINEERING_ABC_WORKAROUND)
	*/

	return err;
}

/**@brief Function for clearing an event flag in NRF_NFCT registers.
 *
 * @param[in]  event  Pointer to event register.
 *
 */
static inline void nrf_nfct_event_clear(volatile u32_t *event)
{
	*event = 0;

	/* Perform read to ensure clearing is effective */
	volatile u32_t dummy = *event;
	(void)dummy;
}

/**@brief Function for handling events from Clock Module.
 *
 * @param[in]  event  Clock event.
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
	/* Begin: Bugfix for FTPAN-116 (IC-12886) */
	/* reset the NFC for release HFCLK */
	__DMB();
	(*(u32_t volatile *)(0x40005FFC)) = 0UL;
	__DMB();
	(*(u32_t volatile *)(0x40005FFC)) = 1UL;
	/* END:   Bugfix for FTPAN-116 (IC-12886) */

}
#endif /* HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND */

#ifndef HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND
/**@brief Function for evaluating and handling NFC field events.
 *
 * @param[in]  field_state  Current field state.
 *
 */
static inline void nrf_nfct_field_event_handler(
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

		/* Re-enable Auto Collision Resolution */
#if defined(NRF52832_XXAA) || defined(NRF52832_XXAB)
		(*(u32_t volatile *)(0x4000559C)) &= ~(1u);
#else
		NRF_NFCT->AUTOCOLRESCONFIG &= ~NFCT_AUTOCOLRESCONFIG_MODE_Msk;
#endif /* defined(NRF52832_XXAA) || defined(NRF52832_XXAB) */
		m_t4t_active = false;

		/* Go back to default frame delay mode, bugfix for tag locking.
		 *
		 */
		NRF_NFCT->FRAMEDELAYMODE =
				NFCT_FRAMEDELAYMODE_FRAMEDELAYMODE_WindowGrid <<
					NFCT_FRAMEDELAYMODE_FRAMEDELAYMODE_Pos;
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
	if (((*(u32_t volatile *)(0x40005420)) & 0x1UL) == (1UL))
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
	/* Parameter validation is done in upper-layer */

	if (id == HAL_NFC_PARAM_FWI) {
		/* Update Frame Wait Time setting; possible settings are limited
		 * by NFCT hardware
		 */
		m_fwi = *((u8_t *)data);

		if (data_length != sizeof(u8_t)) {
			return -EINVAL;
		}
		/* Frame Wait Time settings for first sample of 52840 */
		if (type_52840_sample_check()) {
			if (m_fwi > NFC_T4T_FWI_52840S_MAX) {
				return -EINVAL;
			}
			/* Set FRAMEDELAYTIME */
			if (m_fwi == NFC_T4T_FWI_52840S_MAX) {
				NRF_NFCT->FRAMEDELAYMAX =
						NFCT_FRAMEDELAYMAX_52840S;
			} else {
				NRF_NFCT->FRAMEDELAYMAX =
						NFC_T4T_FWI_TO_FWT(m_fwi);
			}
		} else {
			if (m_fwi > NFC_T4T_FWI_MAX) {
				return -EINVAL;
			}

			/* Set FRAMEDELAYTIME */
			if (m_fwi == NFC_T4T_FWI_MAX) {
				NRF_NFCT->FRAMEDELAYMAX =
					NFCT_FRAMEDELAYMAX_FRAMEDELAYMAX_Msk;
			} else {
				NRF_NFCT->FRAMEDELAYMAX =
					NFC_T4T_FWI_TO_FWT(m_fwi);
			}
		}
	} else if (id == HAL_NFC_PARAM_SELRES) {
		/* Update SEL_RES 'Protocol' bits setting */
		u8_t sel_res = *((u8_t *)data);

		if (data_length != sizeof(u8_t)) {
			return -EINVAL;
		}
		if (sel_res > NFC_T4T_SELRES_PROTOCOL_MSK) {
			return -EINVAL;
		}

		m_t4t_selres = sel_res;
		NRF_NFCT->SELRES = (m_t4t_selres << NFCT_SELRES_PROTOCOL_Pos) &
				   NFCT_SELRES_PROTOCOL_Msk;
	} else if (id == HAL_NFC_PARAM_NFCID1) {
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
	} else {
		/* No implementation needed */
	}

	return 0;
}

/* This function is used by nfc_lib for unit testing only */
int hal_nfc_parameter_get(enum hal_nfc_param_id id,
			  void *data,
			  size_t *max_data_length)
{
	if (*max_data_length < 1) {
		*max_data_length = 1;
		return -EINVAL;
	}

	if (id == HAL_NFC_PARAM_FWI) {
		*((u8_t *) data) = m_fwi;
		*max_data_length = sizeof(m_fwi);
	} else if (id == HAL_NFC_PARAM_SELRES) {
		/* Get SEL_RES 'Protocol' bits setting */
		*((u8_t *) data) = m_t4t_selres;
		*max_data_length = sizeof(m_t4t_selres);
	} else if (id == HAL_NFC_PARAM_NFCID1) {
		if (m_nfcid1_length == 0) {
			m_nfcid1_length = NFCID1_DEFAULT_LENGHT;
			hal_nfc_nfcid1_default_bytes();
		}

		if (*max_data_length < (size_t) m_nfcid1_length) {
			return -EINVAL;
		}

		*max_data_length = (size_t) m_nfcid1_length;
		memcpy(data, m_nfcid1_data, m_nfcid1_length);
	} else {
		/* No implementation needed */
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

int hal_nfc_send(const u8_t *data, size_t data_length, bool delayed_mode)
{
	if (data_length == 0) {
		return -EINVAL;
	}

	/* Ignore previous TX END events, SW takes care only for data frames
	 * which tranmission is triggered in this function
	 */
	nrf_nfct_event_clear(&NRF_NFCT->EVENTS_TXFRAMEEND);

	/* Moved to the end in T4T to avoid delaying TASKS_STARTX */
	NRF_NFCT->INTENSET = (NFCT_INTENSET_TXFRAMEEND_Enabled <<
					NFCT_INTENSET_TXFRAMEEND_Pos);
	NRF_NFCT->PACKETPTR = (u32_t) data;
	NRF_NFCT->TXD.AMOUNT =
			(data_length << NFCT_TXD_AMOUNT_TXDATABYTES_Pos) &
			NFCT_TXD_AMOUNT_TXDATABYTES_Msk;

	if (delayed_mode) {
		NRF_NFCT->FRAMEDELAYMODE =
				NFCT_FRAMEDELAYMODE_FRAMEDELAYMODE_ExactVal <<
					NFCT_FRAMEDELAYMODE_FRAMEDELAYMODE_Pos;
	} else {
		NRF_NFCT->FRAMEDELAYMODE =
				NFCT_FRAMEDELAYMODE_FRAMEDELAYMODE_WindowGrid <<
					NFCT_FRAMEDELAYMODE_FRAMEDELAYMODE_Pos;
		LOG_INF("Send");
	}
	NRF_NFCT->TASKS_STARTTX = 1;

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

	if (NRF_NFCT->EVENTS_RXFRAMEEND &&
	    (NRF_NFCT->INTEN & NFCT_INTEN_RXFRAMEEND_Msk)) {
		/* Take into account only number of whole bytes */
		u32_t rx_status = 0;
		u32_t rx_data_size =
		    ((NRF_NFCT->RXD.AMOUNT & NFCT_RXD_AMOUNT_RXDATABYTES_Msk) >>
				NFCT_RXD_AMOUNT_RXDATABYTES_Pos) - NFC_CRC_SIZE;
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

		/* Ignore all NFC-A data frames with Transmission Error */
		if (rx_status) {
			/* Go back to idle state if currently in the ACTIVE_A
			 * state
			 */
			if (!m_t4t_active) {
				nrf_nfct_default_state_reset();
			} else {
				/* Stay in the CARD_EMULATOR_4A state */
				/* Command with Transmission Error, so wait for
				 * next frame reception
				 */
				NRF_NFCT->TASKS_ENABLERXDATA = 1;
			}
		} else {
			/* Look for NFC-A Commands */
			if (!m_t4t_active) {
				/* 'NFC Forum Digital Protocol Technical
				 *  Specification 2.0, 14.6.1.13'
				 */
				if ((m_nfc_rx_buffer[0] == NFC_T4T_RATS_CMD) &&
				((m_nfc_rx_buffer[1] & NFC_T4T_RATS_DID_MASK) !=
						NFC_T4T_RATS_DID_RFU)) {
					/* Disable Auto Collision Resolution */
#if defined(NRF52832_XXAA) || defined(NRF52832_XXAB)
					(*(u32_t volatile *)(0x4000559C)) |=
									  (1u);
#else
					NRF_NFCT->AUTOCOLRESCONFIG |=
						NFCT_AUTOCOLRESCONFIG_MODE_Msk;
#endif /* defined(NRF52832_XXAA) || defined(NRF52832_XXAB) */
					m_t4t_active = true;
					LOG_DBG("RX: T4T Activate");
				} else if (m_nfc_rx_buffer[0] ==
							NFC_SLP_REQ_CMD) {
					/* Indicate that SLP_REQ was received
					 * - this will cause FRAMEDELAYTIMEOUT
					 * error
					 */

					/* disable RX here (will enable at
					 * SELECTED)
					 */
					m_slp_req_received = true;
					NRF_NFCT->INTENCLR =
					     NFCT_INTENCLR_RXFRAMEEND_Clear <<
						  NFCT_INTENCLR_RXFRAMEEND_Pos;
				} else {
					nrf_nfct_default_state_reset();
				}
			} else {
				/* Look for Tag 4 Type Commands */

				/* Assume that the ISO-DEP frame is incorrect or
				 * not directed to our tag.
				 */
				NRF_NFCT->TASKS_ENABLERXDATA = 1;
			}

			if (m_nfc_lib_callback != NULL) {
				/* This callback should trigger transmission of
				 * READ Response
				 */
				m_nfc_lib_callback(m_nfc_lib_context,
						  HAL_NFC_EVENT_DATA_RECEIVED,
						  (void *)m_nfc_rx_buffer,
						  rx_data_size);
			}

			/* Clear TXFRAMESTART EVENT so it can be checked in
			 * hal_nfc_send
			 */
			nrf_nfct_event_clear(&NRF_NFCT->EVENTS_TXFRAMESTART);
		}

		LOG_DBG("Rx fend");
	}

	if (NRF_NFCT->EVENTS_TXFRAMEEND &&
	    (NRF_NFCT->INTEN & NFCT_INTEN_TXFRAMEEND_Msk)) {
		nrf_nfct_event_clear(&NRF_NFCT->EVENTS_TXFRAMEEND);

		/* Disable TX END event to ignore frame transmission other than
		 * READ response
		 */
		NRF_NFCT->INTENCLR = (NFCT_INTENCLR_TXFRAMEEND_Clear <<
						NFCT_INTENCLR_TXFRAMEEND_Pos);

		if (m_deselect) {
			/* Re-enable Auto Collision Resolution */
#if defined(NRF52832_XXAA) || defined(NRF52832_XXAB)
			(*(u32_t volatile *)(0x4000559C)) &= ~(1u);
#else
			NRF_NFCT->AUTOCOLRESCONFIG &=
						~NFCT_AUTOCOLRESCONFIG_MODE_Msk;
#endif /* defined(NRF52832_XXAA) || defined(NRF52832_XXAB) */
			NRF_NFCT->TASKS_GOSLEEP = 1;
			/* Disable RX here (will be enabled at SELECTED) */
			NRF_NFCT->INTENCLR = NFCT_INTENCLR_RXFRAMEEND_Clear <<
						NFCT_INTENCLR_RXFRAMEEND_Pos;
			m_deselect = false;
			m_t4t_active = false;
		} else {
			/* Set up for reception */
			NRF_NFCT->PACKETPTR = (u32_t) m_nfc_rx_buffer;
			NRF_NFCT->MAXLEN = NFC_RX_BUFFER_SIZE;
			NRF_NFCT->TASKS_ENABLERXDATA = 1;
			NRF_NFCT->INTENSET =
					(NFCT_INTENSET_RXFRAMEEND_Enabled <<
						NFCT_INTENSET_RXFRAMEEND_Pos) |
					(NFCT_INTENSET_RXERROR_Enabled <<
						NFCT_INTENSET_RXERROR_Pos);
		}

		if (m_nfc_lib_callback != NULL) {
			m_nfc_lib_callback(m_nfc_lib_context,
					   HAL_NFC_EVENT_DATA_TRANSMITTED,
					   0,
					   0);
		}

		LOG_DBG("Tx fend");
	}

	if (NRF_NFCT->EVENTS_SELECTED &&
	    (NRF_NFCT->INTEN & NFCT_INTEN_SELECTED_Msk)) {
		nrf_nfct_event_clear(&NRF_NFCT->EVENTS_SELECTED);

		/* Clear also RX END and RXERROR events because SW does not take
		 * care of commands which were received before selecting the tag
		 */
		nrf_nfct_event_clear(&NRF_NFCT->EVENTS_RXFRAMEEND);
		nrf_nfct_event_clear(&NRF_NFCT->EVENTS_RXERROR);

		/* Set up registers for EasyDMA and start receiving packets */
		NRF_NFCT->PACKETPTR = (u32_t) m_nfc_rx_buffer;
		NRF_NFCT->MAXLEN = NFC_RX_BUFFER_SIZE;
		NRF_NFCT->TASKS_ENABLERXDATA = 1;

		NRF_NFCT->INTENSET = (NFCT_INTENSET_RXFRAMEEND_Enabled <<
						NFCT_INTENSET_RXFRAMEEND_Pos) |
				     (NFCT_INTENSET_RXERROR_Enabled <<
						NFCT_INTENSET_RXERROR_Pos);

		/* At this point any previous error status can be ignored */
		NRF_NFCT->FRAMESTATUS.RX = NRF_NFCT_FRAMESTATUS_RX_MSK;
		NRF_NFCT->ERRORSTATUS = NRF_NFCT_ERRORSTATUS_ALL;

#ifndef HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND
		/* Change mask to FIELD_ON state - trigger FIELD_ON only if HW
		 * has locked to the field
		 */
		m_nfc_fieldpresent_mask = NFC_FIELD_ON_MASK;
#endif /* HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND */

		if (m_nfc_lib_callback != NULL) {
			m_nfc_lib_callback(m_nfc_lib_context,
					   HAL_NFC_EVENT_FIELD_ON,
					   0,
					   0);
		}

		m_t4t_active = false;

		LOG_DBG("Selected");
	}

	if (NRF_NFCT->EVENTS_ERROR &&
			(NRF_NFCT->INTEN & NFCT_INTEN_ERROR_Msk)) {
		u32_t err_status = NRF_NFCT->ERRORSTATUS;

		nrf_nfct_event_clear(&NRF_NFCT->EVENTS_ERROR);

		/* Clear FRAMEDELAYTIMEOUT error (expected HW behaviour) when
		 *  SLP_REQ command was received
		 */
		if ((err_status & NFCT_ERRORSTATUS_FRAMEDELAYTIMEOUT_Msk) &&
		    m_slp_req_received) {
			NRF_NFCT->ERRORSTATUS =
					NFCT_ERRORSTATUS_FRAMEDELAYTIMEOUT_Msk;
			m_slp_req_received = false;

			LOG_DBG("RX: SLP_REQ");
		}
		/* Report any other error */
		err_status &= ~NFCT_ERRORSTATUS_FRAMEDELAYTIMEOUT_Msk;

		if (err_status) {
			LOG_DBG("Error (0x%x)",
				      (unsigned int) err_status);
		}

		/* Clear error status */
		NRF_NFCT->ERRORSTATUS = NRF_NFCT_ERRORSTATUS_ALL;
	}

	if (NRF_NFCT->EVENTS_TXFRAMESTART &&
	    (NRF_NFCT->INTEN & NFCT_INTEN_TXFRAMESTART_Msk)) {
		nrf_nfct_event_clear(&NRF_NFCT->EVENTS_TXFRAMESTART);

		if (m_t4t_active) {
			u8_t *tx_buf = (u8_t *) NRF_NFCT->PACKETPTR;
			u32_t tx_data_size = (NRF_NFCT->TXD.AMOUNT &
					      NFCT_TXD_AMOUNT_TXDATABYTES_Msk)
					     >> NFCT_TXD_AMOUNT_TXDATABYTES_Pos;

			if ((*tx_buf & NFC_T4T_S_BLOCK_MSK) ==
						NFC_T4T_S_DESELECT) {
				m_deselect = true;

				LOG_DBG("T4T Go to sleep");
			}

			if (m_nfc_lib_callback != NULL) {
				m_nfc_lib_callback(m_nfc_lib_context,
						  HAL_NFC_EVENT_DATA_TX_STARTED,
						  tx_buf,
						  tx_data_size);
			}
		}
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
		 * TODO: HF CLK clock request
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
