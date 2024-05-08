/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */
/* PURPOSE: Header to combine osif dependent files with stack
*/

#ifndef ZB_OSIF_PLATFORM_H__
#define ZB_OSIF_PLATFORM_H__

#define ZB_INTERRUPT_SAFE_CALLBACKS
#define ZB_USE_NVRAM
#define ZB_NVRAM_RADIO_OFF_DURING_TRANSACTION

/* trace */
#define ZB_NRF_TRACE

#ifdef ZB_TRACE_LEVEL
#define ZB_TRACE_OVER_USART
#define ZB_TRACE_TO_PORT
#define ZB_BINARY_TRACE
#define ZB_TRAFFIC_DUMP_ON
#define ZB_TRAF_DUMP_V2
#endif  /* ZB_TRACE_LEVEL */

#if defined ZB_TRACE_OVER_USART && defined ZB_TRACE_LEVEL
#define ZB_SERIAL_FOR_TRACE
#endif /* ZB_TRACE_OVER_USART && ZB_TRACE_LEVEL */

#ifdef ZB_SERIAL_FOR_TRACE
void zb_osif_serial_flush(void);
#if defined(ZB_NRF_TRACE) && (defined CONFIG_ZB_ASYNC_TRACE_CONTROL || defined ZB_MULTI_TEST)
#define ZB_OSIF_SERIAL_FLUSH()          zb_osif_serial_flush()
#elif !defined(ZBOSS_BUILD)
#define ZB_OSIF_SERIAL_FLUSH()          zb_osif_serial_flush()
#else /* ZB_NRF_TRACE && (CONFIG_ZB_ASYNC_TRACE_CONTROL || defined ZB_MULTI_TEST) */
#define ZB_OSIF_SERIAL_FLUSH()
#endif /* ZB_NRF_TRACE && (CONFIG_ZB_ASYNC_TRACE_CONTROL || defined ZB_MULTI_TEST) */
#else /* ZB_SERIAL_FOR_TRACE */
#define ZB_OSIF_SERIAL_FLUSH()
#endif /* ZB_SERIAL_FOR_TRACE */


#define ZB_SOFT_SECURITY
#define ZB_HW_ZB_AES128

#ifdef ZB_HW_ZB_AES128
void zb_osif_aes128_hw_encrypt(zb_uint8_t *key, zb_uint8_t *msg, zb_uint8_t *c);
#define hw_aes128                       zb_osif_aes128_hw_encrypt
#endif /* ZB_HW_ZB_AES128 */


void zb_osif_init(void);
#define ZB_PLATFORM_INIT()              zb_osif_init()

void zb_osif_abort(void);
#define ZB_ABORT                        zb_osif_abort

#define ZB_GO_IDLE()

/* Read the NCP firmware version. This function may be redefined by the application. */
zb_uint32_t zb_osif_get_fw_version(void);
#define NCP_FW_VERSION                  zb_osif_get_fw_version()

/* Read the ZBOSS stack version in NCP firmware. This value is defined in zb_vendor.h. */
#define NCP_STACK_VERSION               ZB_STACK_VERSION

/* Read the NCP protocol version. This function may be redefined by the application. */
zb_uint32_t zb_osif_get_ncp_protocol_version(void);
#define NCP_PROTOCOL_VERSION            zb_osif_get_ncp_protocol_version()


void zb_osif_timer_init(void);
zb_uint32_t zb_osif_timer_get(void);

void zb_osif_timer_stop(void);
#define ZB_STOP_HW_TIMER()              zb_osif_timer_stop()

void zb_osif_timer_start(void);
#define ZB_START_HW_TIMER()             zb_osif_timer_start()

zb_bool_t zb_osif_timer_is_on(void);
#define ZB_CHECK_TIMER_IS_ON()          zb_osif_timer_is_on()


void zb_osif_enable_all_inter(void);
void zb_osif_disable_all_inter(void);

/* Redefine ZBOSS scheduler API only inside Zephyr application. */
#ifndef ZBOSS_BUILD
#define ZB_SCHEDULE_APP_CALLBACK(func, param) \
	zigbee_schedule_callback(func, param)

#define ZB_SCHEDULE_APP_CALLBACK2(func, param, user_param) \
	zigbee_schedule_callback2(func, param, user_param)

#define ZB_SCHEDULE_APP_ALARM(func, param, timeout_bi) \
	zigbee_schedule_alarm(func, param, timeout_bi)

#define ZB_SCHEDULE_APP_ALARM_CANCEL(func, param) \
	zigbee_schedule_alarm_cancel(func, param)

#define zb_buf_get_out_delayed(func) \
	zigbee_get_out_buf_delayed(func)

#define zb_buf_get_in_delayed(func) \
	zigbee_get_in_buf_delayed(func)

#define zb_buf_get_out_delayed_ext(func, param, max_size) \
	zigbee_get_out_buf_delayed_ext(func, param, max_size)

#define zb_buf_get_in_delayed_ext(func, param, max_size) \
	zigbee_get_in_buf_delayed_ext(func, param, max_size)

/**@brief Schedule single-param callback execution.
 *
 * This API is thread- and ISR- safe.
 * It performs all necessary actions:
 *  - Forwards request from ISR to thread context
 *  - Schedules the callback in ZBOSS scheduler queue
 *  - Wakes up the Zigbee task.
 *
 * @param func    function to execute
 * @param param - callback parameter - usually ref to packet buffer
 *
 * @return RET_OK or RET_OVERFLOW.
 */
zb_ret_t zigbee_schedule_callback(zb_callback_t func, zb_uint8_t param);

/**@brief Schedule two-param callback execution.
 *
 * This API is thread- and ISR- safe.
 * It performs all necessary actions:
 *  - Forwards request from ISR to thread context
 *  - Schedules the callback in ZBOSS scheduler queue
 *  - Wakes up the Zigbee task.
 *
 * @param func        function to execute
 * @param param       zb_uint8_t callback parameter - usually,
 *                    ref to packet buffer
 * @param user_param  zb_uint16_t additional user parameter
 *
 * @return RET_OK or RET_OVERFLOW.
 */
zb_ret_t zigbee_schedule_callback2(zb_callback2_t func, zb_uint8_t param,
				   zb_uint16_t user_param);


/**@brief Schedule alarm - callback to be executed after timeout.
 *
 * Function will be called via scheduler after timeout expired, with possible
 * delays due to locking/unlocking and scheduling mechanism.
 * Timer resolution depends on implementation and is limited to a single
 * Beacon Interval (15.36 msec).
 * Same callback can be scheduled for execution more then once.
 *
 * This API is thread- and ISR- safe.
 * It performs all necessary actions:
 *  - Forwards request from ISR to thread context
 *  - Schedules the callback in ZBOSS scheduler queue
 *  - Wakes up the Zigbee task.
 *
 * @param func       function to call via scheduler
 * @param param      parameter to pass to the function
 * @param timeout_bi timeout, in beacon intervals
 *
 * @return RET_OK or RET_OVERFLOW
 */
zb_ret_t zigbee_schedule_alarm(zb_callback_t func, zb_uint8_t param,
			       zb_time_t run_after);

/**@brief Cancel previously scheduler alarm.
 *
 * This API cancels alarms scheduled via zigbee_schedule_alarm() API
 *
 * This API is thread- and ISR- safe.
 * It performs all necessary actions:
 *  - Forwards request from ISR to thread context
 *  - Schedules the callback in ZBOSS scheduler queue
 *  - Wakes up the Zigbee task.
 *
 * @param func - function to call via scheduler
 * @param param - parameter to pass to the function
 *
 * @return RET_OK or RET_OVERFLOW
 */
zb_ret_t zigbee_schedule_alarm_cancel(zb_callback_t func, zb_uint8_t param);

/**@brief Allocate OUT buffer, call a callback when the buffer is available.
 *
 * Use default buffer size _func(alloc single standard buffer).
 * If buffer is available, schedules callback for execution immediately.
 * If no buffers are available now, schedule callback later,
 * when buffer will be available.
 *
 * This API is thread- and ISR- safe.
 * It performs all necessary actions:
 *  - Forwards request from ISR to thread context
 *  - Schedules the callback in ZBOSS scheduler queue
 *  - Wakes up the Zigbee task.
 *
 * @param func - function to execute.
 * @return RET_OK or RET_OVERFLOW
 */
zb_ret_t zigbee_get_out_buf_delayed(zb_callback_t func);

/**@brief Allocate IN buffer, call a callback when the buffer is available.
 *
 * Use default buffer size _func(alloc single standard buffer).
 * If buffer is available, schedules callback for execution immediately.
 * If no buffers are available now, schedule callback later,
 * when buffer will be available.
 *
 * This API is thread- and ISR- safe.
 * It performs all necessary actions:
 *  - Forwards request from ISR to thread context
 *  - Schedules the callback in ZBOSS scheduler queue
 *  - Wakes up the Zigbee task.
 *
 * @param func - function to execute.
 * @return RET_OK or RET_OVERFLOW
 */
zb_ret_t zigbee_get_in_buf_delayed(zb_callback_t func);

/**@brief Allocate OUT buffer, call a callback when the buffer is available.
 *
 * If buffer is available, schedules callback for execution immediately.
 * If no buffers are available now, schedule callback later,
 * when buffer will be available.
 *
 * This API is thread- and ISR- safe.
 * It performs all necessary actions:
 *  - Forwards request from ISR to thread context
 *  - Schedules the callback in ZBOSS scheduler queue
 *  - Wakes up the Zigbee task.
 *
 * @param func     function to execute.
 * @param param    second parameter to pass to the function
 * @param max_size required maximum buffer payload size (in bytes).
 *                 It can be bigger or smaller than the default buffer size.
 *                 Depending on the specific value, the buffer pool may decide
 *                 to use a fraction of buffer or long buffers.
 *                 Special value 0 means "single default buffer".
 * @return RET_OK or RET_OVERFLOW
 */
zb_ret_t zigbee_get_out_buf_delayed_ext(zb_callback2_t func, zb_uint16_t param,
					zb_uint16_t max_size);

/**@brief Allocate IN buffer, call a callback when the buffer is available.
 *
 * If buffer is available, schedules callback for execution immediately.
 * If no buffers are available now, schedule callback later,
 * when buffer will be available.
 *
 * @param func     function to execute.
 * @param param    second parameter to pass to the function
 * @param max_size required maximum buffer payload size (in bytes).
 *                 It can be bigger or smaller than the default buffer size.
 *                 Depending on the specific value, the buffer pool may decide
 *                 to use a fraction of buffer or long buffers.
 *                 Special value 0 means "single default buffer".
 * @return RET_OK or error code.
 */
zb_ret_t zigbee_get_in_buf_delayed_ext(zb_callback2_t func, zb_uint16_t param,
				   zb_uint16_t max_size);

#endif /* ZBOSS_BUILD */

#ifdef ZB_STACK_REGRESSION_TESTING_API

#define ZB_ENABLE_ALL_INTER()                          \
  ZB_REGRESSION_TESTS_API().osif_interrupts_balance++; \
  zb_osif_enable_all_inter()
#define ZB_DISABLE_ALL_INTER()                         \
  ZB_REGRESSION_TESTS_API().osif_interrupts_balance--; \
  zb_osif_disable_all_inter()

#else /* ZB_STACK_REGRESSION_TESTING_API */

#define ZB_ENABLE_ALL_INTER()           zb_osif_enable_all_inter()
#define ZB_DISABLE_ALL_INTER()          zb_osif_disable_all_inter()

#endif  /* ZB_STACK_REGRESSION_TESTING_API */

#define ZB_OSIF_GLOBAL_LOCK()           ZB_DISABLE_ALL_INTER()
#define ZB_OSIF_GLOBAL_UNLOCK()         ZB_ENABLE_ALL_INTER()


#ifdef ZB_USE_SLEEP
void zb_osif_sleep_init(void);
zb_uint32_t zb_osif_sleep(zb_uint32_t sleep_tmo);
zb_bool_t zb_osif_is_sleeping(void);
#endif /* ZB_USE_SLEEP */


#ifdef ZB_MACSPLIT_DEVICE
#if defined (NRFX_WDT_ENABLED) && (NRFX_WDT_ENABLED == 1)
#define ZB_KICK_HW_WATCHDOG()           zb_osif_wdt_feed()
#else /* ZB_MACSPLIT_DEVICE */
#define ZB_KICK_HW_WATCHDOG()
#endif
#endif /* ZB_MACSPLIT_DEVICE */

#ifdef ZB_MACSPLIT_TRANSPORT_USERIAL
#define ZB_TRANSPORT_NONBLOCK_ITERATION() (zb_macsplit_transport_recv_bytes(),0)
#else /* ZB_MACSPLIT_TRANSPORT_USERIAL */
#define ZB_TRANSPORT_NONBLOCK_ITERATION() 0
#endif /* ZB_MACSPLIT_TRANSPORT_USERIAL */


#ifdef CONFIG_ZB_HAVE_SERIAL
#define ZB_HAVE_SERIAL
#define ZB_HAVE_ASYNC_SERIAL
#endif  /* CONFIG_ZB_HAVE_SERIAL */

#ifdef CONFIG_ZB_USE_LEDS
#define ZB_USE_LEDS
#define ZB_N_LEDS 4
#endif /* CONFIG_ZB_USE_LEDS */

#ifdef CONFIG_ZB_USE_DIMMABLE_LED
#define ZB_USE_DIMMABLE_LED
#endif /* CONFIG_ZB_USE_DIMMABLE_LED */

#ifdef CONFIG_ZB_USE_BUTTONS
#define ZB_USE_BUTTONS
#define ZB_N_BUTTONS 4
#endif /* CONFIG_ZB_USE_BUTTONS */


/** @addtogroup special_nordic_functions
 * @{
 */

/**
 * @brief Read IEEE long address from FICR registers.
 *
 * @param[out] ieee_eui64  Pointer to a long address structure to be filled.
 * 								@cond NORDIC_NCS_SPECIAL_FUNCTIONS
 * 								This structure is filled with the EUI-64 assigned by Nordic Semiconductor. Refer to the Configuring Zigbee in nRF Connect SDK ("IEEE 802.15.4 EUI-64 configuration" section) if you want to change it.
 * 								@endcond
 * 								@cond NORDIC_NRF5_SPECIAL_FUNCTIONS
 * 								The 24 oldest bits are filled in by the ZIGBEE_VENDOR_OUI macro and the remainder is taken from the FICR register.
 * 								@endcond
 */
void zb_osif_get_ieee_eui64(zb_ieee_addr_t ieee_eui64);

#if defined ZB_NRF_INTERNAL || defined NORDIC_NRF5_SPECIAL_FUNCTIONS

/**@brief Function which waits for event -- essential implementation of sleep on NRF52
 */
void zb_osif_wait_for_event(void);

/**@brief Function which tries to sleep down the MCU
 *
 * Function is defined as weak; to be redefined if someone wants to implement their own
 * going-to-sleep policy.
 */
void zb_osif_go_idle(void);

/**@brief Function which disables all Zigbee stack-related peripherals
 *
 * Function is defined as weak; to be redefined if someone wants to implement their own
 * going-to-deep-sleep policy/share resources between Zigbee stack and other components.
 */
void zb_nrf52_periph_disable(void);

/**@brief Function which enables back all Zigbee stack-related peripherals
 *
 * Function is defined as weak; to be redefined if someone wants to implement their own
 * going-to-deep-sleep policy/share resources between Zigbee stack and other components.
 */
void zb_nrf52_periph_enable(void);

#endif /* ZB_NRF_INTERNAL && NORDIC_NRF5_SPECIAL_FUNCTIONS */

/**@brief Function which tries to put the device into deep sleep mode, caused by an empty Zigbee stack scheduler queue.
 *
 * Function is defined as weak; to be redefined if someone wants to implement their own
 * going-to-deep-sleep policy.
 */
zb_uint32_t zb_osif_sleep(zb_uint32_t sleep_tmo);

/**@brief Function which is called after zb_osif_sleep
 *        finished and ZBOSS timer is reenabled.
 *
 * Function is defined as weak; to be redefined if someone
 * wants to implement their own going-to-deep-sleep policy/share resources
 * between Zigbee stack and other components.
 */
void zb_osif_wake_up(void);

/**
 * @}
 */

#endif /* ZB_OSIF_PLATFORM_H__ */
