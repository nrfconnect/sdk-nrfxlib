/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
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
#define ZB_TRACE_FROM_INTR
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
#else /* ZB_NRF_TRACE && (CONFIG_ZB_ASYNC_TRACE_CONTROL || defined ZB_MULTI_TEST) */
#define ZB_OSIF_SERIAL_FLUSH()
#endif /* ZB_NRF_TRACE && (CONFIG_ZB_ASYNC_TRACE_CONTROL || defined ZB_MULTI_TEST) */
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



#ifdef ZB_MACSPLIT_TRANSPORT_USERIAL
#define ZB_TRANSPORT_NONBLOCK_ITERATION() (zb_macsplit_transport_recv_bytes(),0)
#else /* ZB_MACSPLIT_TRANSPORT_USERIAL */
#define ZB_TRANSPORT_NONBLOCK_ITERATION() 0
#endif /* ZB_MACSPLIT_TRANSPORT_USERIAL */


#ifdef CONFIG_ZB_HAVE_SERIAL
#define ZB_HAVE_SERIAL
#define ZB_HAVE_ASYNC_SERIAL
#endif  /* CONFIG_ZB_HAVE_SERIAL */


/** @addtogroup special_nordic_functions
 * @{
 */

/**
 * @brief Read IEEE long address from FICR registers.
 *
 * @param[out] ieee_eui64  Pointer to a long address structure to be filled.
 */
void zb_osif_get_ieee_eui64(zb_ieee_addr_t ieee_eui64);

#ifdef CONFIG_RADIO_STATISTICS
typedef struct zb_osif_radio_stats_s
{
  zb_uint32_t rx_successful;            /* Success: nrf_802154_received_timestamp_raw() calls count. */
  zb_uint32_t rx_err_none;              /* Error Code: 0x00 */
  zb_uint32_t rx_err_invalid_frame;     /* Error Code: 0x01 */
  zb_uint32_t rx_err_invalid_fcs;       /* Error Code: 0x02 */
  zb_uint32_t rx_err_invalid_dest_addr; /* Error Code: 0x03 */
  zb_uint32_t rx_err_runtime;           /* Error Code: 0x04 */
  zb_uint32_t rx_err_timeslot_ended;    /* Error Code: 0x05 */
  zb_uint32_t rx_err_aborted;           /* Error Code: 0x06 */

  zb_uint32_t tx_successful;            /* Success: nrf_802154_transmitted_raw() calls count. */
  zb_uint32_t tx_err_none;              /* Error Code: 0x00 */
  zb_uint32_t tx_err_busy_channel;      /* Error Code: 0x01 */
  zb_uint32_t tx_err_invalid_ack;       /* Error Code: 0x02 */
  zb_uint32_t tx_err_no_mem;            /* Error Code: 0x03 */
  zb_uint32_t tx_err_timeslot_ended;    /* Error Code: 0x04 */
  zb_uint32_t tx_err_no_ack;            /* Error Code: 0x05 */
  zb_uint32_t tx_err_aborted;           /* Error Code: 0x06 */
  zb_uint32_t tx_err_timeslot_denied;   /* Error Code: 0x07 */

} zb_osif_radio_stats_t;

zb_osif_radio_stats_t *zb_osif_get_radio_stats(void);
#endif /* CONFIG_RADIO_STATISTICS */

/**
 * @}
 */

#endif /* ZB_OSIF_PLATFORM_H__ */
