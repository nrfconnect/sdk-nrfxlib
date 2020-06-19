/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2020 DSR Corporation, Denver CO, USA.
 * http://www.dsr-zboss.com
 * http://www.dsr-corporation.com
 * All rights reserved.
 *
 *
 * Use in source and binary forms, redistribution in binary form only, with
 * or without modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 2. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 3. This software, with or without modification, must only be used with a Nordic
 *    Semiconductor ASA integrated circuit.
 *
 * 4. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*  PURPOSE: ZBOSS core API header. Time, scheduler and memory buffers API.
*/

#ifndef ZB_ZBOSS_API_CORE_H
#define ZB_ZBOSS_API_CORE_H 1

#include "zb_vendor.h"
#include "zb_config.h"
#include "zb_types.h"
#include "zb_channel_page.h"
#include "zb_errors.h"

/* zb_callback_t is used in osif if we have serial API */
/*! \addtogroup sched */
/*! @{ */

/**
 *   Callback function typedef.
 *   Callback is function planned to execute by another function.
 *
 *   @note Callback must be declared as reentrant for sdcc.
 *
 *   @param param - callback parameter - usually, but not always, ref to packet buf
 *
 *   See any sample
 */
typedef void (ZB_CODE * zb_callback_t)(zb_uint8_t param);
/*! @} */


#include "zb_osif.h"
#include "zb_debug.h"
#include "zb_trace.h"

/*! \addtogroup time */
/*! @{ */

/**
   \par Timer functionality.

   The idea is: platform has some timer which can be stopped or run.
   When run, it increments (or decrements - depends on platform) some counter
   until counter overflow (underflow), then issues interrupt - wakeups main loop
   if it sleeping.
   Time stored in ticks; time resolution is platform dependent, its usual value
   is 15.36 usec - 1 beacon interval.
   @note Time type has limited capacity (usually 16 bits) and can overflow.
   Macros which works with time handles overflow. It is supposed that time values will
   not differ to more then 1/2 of the maximum time value.

   All that timer macros will not be used directly by the application code - it
   is scheduler internals. The only API for timer is ZB_SCHEDULE_ALARM() call.
 */


/**
   Timer type.
 */

#ifdef ZB_TIMER_32
  typedef zb_uint32_t zb_time_t;
/**
 * Maximum timer value, if 32-bit timer is used.
 */
  #define ZB_MAX_TIME_VAL ZB_UINT32_MAX
/**
 * Minimum timer value, if 32-bit timer is used.
 */
  #define ZB_MIN_TIME_VAL ZB_UINT32_MIN
#else
/* 16 bits for 8051 - it will be hw timer value. */
  typedef zb_uint16_t zb_time_t;
/**
 * Maximum timer value, if 16-bit timer is used.
 */
  #define ZB_MAX_TIME_VAL ZB_UINT16_MAX
/**
 * Minimum timer value, if 16-bit timer is used.
 */
 #define ZB_MIN_TIME_VAL ZB_UINT16_MIN
#endif

/**
 * A half of defined maximum timer value.
 */
#define ZB_HALF_MAX_TIME_VAL (ZB_MAX_TIME_VAL / 2)

/** @cond internals_doc */
/**
 * @copydoc ZB_TIMER_GET()
 *
 * @note Don't call directly, use @ref ZB_TIMER_GET() instead
 */
zb_time_t zb_timer_get(void);
/** @endcond */ /* internals_doc */

/**
 * @return Get current timer value (beacon intervals)
 */
#define ZB_TIMER_GET() (zb_timer_get())

/**
   Time subtraction: subtract 'b' from 'a'

   Take overflow into account: change sign (subtraction order) if result >
   values_diapason/2.
   Suppose a always >= b, so result is never negative.
   This macro will be used to calculate, for example, amount of time to sleep
   - it is positive by definition.
   Do not use it to compare time values! Use ZB_TIME_GE() instead.
   @note Both a and b is of type @ref zb_time_t. Can't decrease time (subtract
   constant from it) using this macro.

   @param a - time to subtract from
   @param b - time to subtract
   @return subtraction result
 */
#define ZB_TIME_SUBTRACT(a, b) ((zb_time_t)((a) - (b)) < ZB_HALF_MAX_TIME_VAL ? (zb_time_t)((a) - (b)) : (zb_time_t)((b) - (a)))

/**
   Time add: add 'a' to 'b'

   Overflow is possible, but this is ok - it handled by subtraction and compare macros.

   @param a - time to add to
   @param b - value to add
   @return addition result
 */
#define ZB_TIME_ADD(a, b) (zb_time_t)((a) + (b))

/**
   Compare times a and b - check that a >= b

   Taking into account overflow and unsigned values arithmetic and supposing
   difference between a and b can't be > 1/2 of the overall time values
   diapason,
   a >= b only if a - b < values_diapason/2

   @param a - first time value to compare
   @param b - second time value to compare
   @return 1 is a >= b, 0 otherwise
 */
#define ZB_TIME_GE(a, b) ((zb_time_t)((a) - (b)) < ZB_HALF_MAX_TIME_VAL)


/**
 One second timeout
*/
#define ZB_TIME_ONE_SECOND ZB_MILLISECONDS_TO_BEACON_INTERVAL(1000)


#ifdef ZB_TIMER_32
/**
  Convert time from milliseconds to beacon intervals (32-bit platforms).
*/
#define ZB_MILLISECONDS_TO_BEACON_INTERVAL(ms) (((zb_time_t)(ms) * 1000) / ZB_BEACON_INTERVAL_USEC)
#else
/**
  Convert time from milliseconds to beacon intervals
  Try to not cause overflow in 16-bit arithmetic (with some precision lost...)
*/
#define ZB_MILLISECONDS_TO_BEACON_INTERVAL(ms) (((10l * (zb_time_t)(ms) + 0) / (ZB_BEACON_INTERVAL_USEC / 100)))
#endif

/**
 * Beacon interval in microseconds
 *
 * 1 beacon interval = aBaseSuperframeDuration * symbol duration
 *
 * 1 symbol = 16e-6 sec (mac spec 6.5.3.2 Symbol rate) for 2.4GHz ZB.
 * 1 beacon interval = 15.360 ms.
 */
#define ZB_BEACON_INTERVAL_USEC (ZB_SYMBOL_DURATION_USEC * ZB_ABASE_SUPERFRAME_DURATION)


#ifdef ZB_TIMER_32
/**
  Convert time from beacon intervals to milliseconds (32-bit platform).
*/
#define ZB_TIME_BEACON_INTERVAL_TO_MSEC(t) ((ZB_BEACON_INTERVAL_USEC * (zb_time_t)(t)) / 1000)
#else
/**
  Convert time from beacon intervals to milliseconds

  Try to not cause overflow in 16-bit arithmetic (with some precision lost...)
*/
#define ZB_TIME_BEACON_INTERVAL_TO_MSEC(t) ((ZB_BEACON_INTERVAL_USEC / 100 * (t) - 0) / 10)
#endif

/**
  Convert time from beacon intervals to microseconds

*/
#define ZB_TIME_BEACON_INTERVAL_TO_USEC(t) ((zb_uint64_t)ZB_BEACON_INTERVAL_USEC * (t))

/**
 Quarterseconds timeout
*/
#define ZB_TIME_QUARTERECONDS(n)  (ZB_TIME_BEACON_INTERVAL_TO_MSEC((n)) / 250)

/**
 Convert from quarterseconds to msec
*/
#define ZB_QUARTERECONDS_TO_MSEC(n)  250*(n)

/**
 Convert from quarterseconds to beacon
*/
#define ZB_QUARTERECONDS_TO_BEACON_INTERVAL(qsec) ZB_MILLISECONDS_TO_BEACON_INTERVAL(250*qsec)

/**
 * Convert from seconds to milliseconds
 */
#define ZB_SECONDS_TO_MILLISECONDS(_s) (1000l*(_s))
/**
 Convert from milliseconds to microseconds
*/
#define ZB_MILLISECONDS_TO_USEC(ms) ((ms) * (1000))

/**
 Convert from microseconds to milliseconds
*/
#define ZB_USECS_TO_MILLISECONDS(usec) ((usec) / (1000))

/*! @} */

/*! \addtogroup sched */
/*! @{ */
                                                        

typedef zb_ret_t (ZB_CODE * zb_ret_callback_t)(zb_uint8_t param);

typedef void (*zb_callback_void_t)(void);


/**
 *   Callback function with 2 parameters typedef.
 *   Callback is function planned to execute by another function.
 *
 *   @note Callback must be declared as reentrant for sdcc.
 *
 *   @param param - callback parameter - usually, but not always, ref to packet buf
 *   @param cb_param - additional 2-byte callback parameter, user data.
 *
 *   See any sample
 */
typedef void (ZB_CODE * zb_callback2_t)(zb_uint8_t param, zb_uint16_t cb_param);
/** @cond internals_doc */
/** Schedule single-param callback execution.
    (use ZB_SCHEDULE_APP_CALLBACK() macro instead of this function).

    Schedule execution of function `func' in the main scheduler loop.

    @param func - function to execute
    @param param - callback parameter - usually, but not always ref to packet buffer
    @param use_2_param - ZB_TRUE whether additional parameter should be used
    @param user_param - additional parameter which will be used if use_2_param is ZB_TRUE
    @param is_prior - ZB_TRUE is callback is high priority

    @return RET_OK or error code.

    See sched sample
*/
zb_ret_t zb_schedule_app_callback(zb_callback_t func, zb_uint8_t param,
                                  zb_bool_t use_2_param, zb_uint16_t user_param, zb_bool_t is_prior);
/** @endcond */ /* internals_doc */
/**
   Schedule single-param callback execution.

   Schedule execution of function `func' in the main scheduler loop.

   @param func - function to execute
   @param param - callback parameter - usually, but not always ref to packet buffer

   @return RET_OK or RET_OVERFLOW.

   See sched sample
 */
#ifndef ZB_SCHEDULE_APP_CALLBACK
#define ZB_SCHEDULE_APP_CALLBACK(func, param) zb_schedule_app_callback(func, param, ZB_FALSE, 0, ZB_FALSE)
#endif /* ZB_SCHEDULE_APP_CALLBACK */

/**
   Schedule two-param callback execution.
   Schedule execution of function `func' in the main scheduler loop.

   @param func - function to execute
   @param param - zb_uint8_t callback parameter - usually, but not always ref to
   packet buffer
   @param user_param - zb_uint16_t user parameter - usually, but not
   always short address

   @return RET_OK or RET_OVERFLOW.
   See sched sample
 */
#ifndef ZB_SCHEDULE_APP_CALLBACK2
#define ZB_SCHEDULE_APP_CALLBACK2(func, param, user_param) zb_schedule_app_callback((zb_callback_t)(func),  param, ZB_TRUE, user_param, ZB_FALSE)
#endif /* ZB_SCHEDULE_APP_CALLBACK2 */

/** @cond internals_doc */
zb_ret_t zb_schedule_app_alarm(zb_callback_t func, zb_uint8_t param, zb_time_t run_after);
/** @endcond */ /* internals_doc */

/**
   Schedule alarm - callback to be executed after timeout.

   Function will be called via scheduler after timeout expired (maybe, plus some
   additional time).
   Timer resolution depends on implementation.
   Same callback can be scheduled for execution more then once.

   @param func - function to call via scheduler
   @param param - parameter to pass to the function
   @param timeout_bi - timeout, in beacon intervals
   @return RET_OK or RET_OVERFLOW

   See any sample
 */
#ifndef ZB_SCHEDULE_APP_ALARM
#define ZB_SCHEDULE_APP_ALARM(func, param, timeout_bi) zb_schedule_app_alarm(func, param, timeout_bi)
#endif /* ZB_SCHEDULE_APP_ALARM */

/**
   Special parameter for zb_schedule_alarm_cancel(): cancel alarm once without
   parameter check

   Cancel only one alarm without check for parameter
 */
#define ZB_ALARM_ANY_PARAM (zb_uint8_t)(-1)

/**
   Special parameter for zb_schedule_alarm_cancel(): cancel alarm for all
   parameters
 */
#define ZB_ALARM_ALL_CB (zb_uint8_t)(-2)
/** @cond internals_doc */
/**
   Cancel scheduled alarm (use macro ZB_SCHEDULE_APP_ALARM_CANCEL() or
   ZB_SCHEDULE_APP_ALARM_CANCEL_AND_GET_BUF() instead of this function).

   This function cancel previously scheduled alarm. Function is identified by
   the pointer.

   @param func - function to cancel
   @param param - parameter to cancel. \see ZB_ALARM_ANY_PARAM. \see ZB_ALARM_ALL_CB
   @param p_param - [out] pointer of ref buffer from cancelled flag: free buffer if its alarm will be cancel
   @return RET_OK or error code

   See reporting_srv sample
 */
zb_ret_t zb_schedule_alarm_cancel(zb_callback_t func, zb_uint8_t param, zb_uint8_t *p_param);
/** @endcond */ /* internals_doc */
/**
   Cancel scheduled alarm.

   This function cancel previously scheduled alarm. Function is identified by
   the pointer.

   @param func - function to cancel
   @param param - parameter to cancel. \see ZB_ALARM_ANY_PARAM. \see ZB_ALARM_ALL_CB
   @return RET_OK or RET_OVERFLOW

   See reporting_srv sample
 */
#ifndef ZB_SCHEDULE_APP_ALARM_CANCEL
#define ZB_SCHEDULE_APP_ALARM_CANCEL(func, param) zb_schedule_alarm_cancel((func), (param), NULL)
#endif /* ZB_SCHEDULE_APP_ALARM_CANCEL */

/**
   Cancel scheduled alarm and get buffer.

   This function cancel previously scheduled alarm and returns buffer ref associated with alarm.
   Function is identified by the pointer.

   @param func - function to cancel
   @param param - parameter to cancel. \see ZB_ALARM_ANY_PARAM. \see ZB_ALARM_ALL_CB
   @param p_param - [out] pointer of ref buffer from cancelled flag: free buffer if its alarm will be cancel
   @return RET_OK or error code

   @b Example:
   @code
   {
     zb_uint8_t cancelled_param;

     ZB_SCHEDULE_ALARM_CANCEL_AND_GET_BUF(my_func1, ZB_ALARM_ALL_CB, &cancelled_param);
     my_func1(cancelled_param);
   }
   @endcode

   See reporting_srv sample
 */
#define ZB_SCHEDULE_APP_ALARM_CANCEL_AND_GET_BUF(func, param, p_param) zb_schedule_alarm_cancel((func), (param), p_param)

/** @cond internals_doc */
/**
   Get Schedule alarm time - seek alarm and return alarm time

   @param func - function to call via scheduler
   @param param - parameter to pass to the function
   @param timeout_bi - pointer on alarm timeout, in beacon intervals
   @return RET_OK or error code
 */
zb_ret_t zb_schedule_get_alarm_time(zb_callback_t func, zb_uint8_t param, zb_time_t *timeout_bi);
/** @endcond */ /* internals_doc */

/**
   Get Schedule alarm time - seek alarm and return alarm time

   @param func - function to call via scheduler
   @param param - parameter to pass to the function
   @param timeout_bi - pointer on alarm timeout, in beacon intervals
   @return RET_OK or error code
*/
#define ZB_SCHEDULE_GET_ALARM_TIME(func, param, timeout_bi) zb_schedule_get_alarm_time(func, param, timeout_bi)

/*! @} */


/*! \addtogroup rng */
/*! @{ */

/**
 * Maximal possible value for randomly generated (16-bit) value
 */
#define ZB_RAND_MAX ((zb_uint16_t)0xffff)

/**
 * Generate random value
 *
 * @return random value between 0 to 0xffff (16 bit)
 */
zb_uint16_t zb_random(void);


/**
   Generate random value between 0 to max_value (16 bit)
 */
#define ZB_RANDOM_VALUE(max_value) (((zb_uint16_t)(max_value)) ? (zb_random() / (ZB_RAND_MAX / (zb_uint16_t)(max_value))) : 0)

/*! @} */

/** @cond internals_doc */
/**
   Copy 8 byte array (i.e. long address).
 */
void zb_memcpy8(zb_uint8_t *ptr, zb_uint8_t *src);
/** @endcond */ /* internals_doc */

/**
  Return absolute value
*/
#define ZB_ABS(a) ((a) < 0 ? -(a) : (a))

/** @cond internals_doc */
/**
 * @note Pointer should be aligned.
 * Cast is intended to suppress warnings "cast increases required alignment of target type [-Wcast-align]"
 * when you perform pointers cast.
 */
#define ZB_SAFE_PTR_CAST(ptr) (void*)(ptr)
/** @endcond */ /* internals_doc */

/**
 * Set of ZB_BITSx() macros return value with bits set in provided positions.
 * ZB_BITS1(0)     = 1
 * ZB_BITS2(2,3)   = 12
 * ZB_BITS3(0,1,2) = 7
 */
#define ZB_BITS1(_b) (1ul << (_b))
#define ZB_BITS2(_b1, _b2) ((1ul << (_b1)) | (1ul << (_b2)))
#define ZB_BITS3(_b1, _b2, _b3) ((1ul << (_b1)) | (1ul << (_b2)) | (1ul << (_b3)))
#define ZB_BITS4(_b1, _b2, _b3, _b4) ((1ul << (_b1)) | (1ul << (_b2)) | (1ul << (_b3)) | (1ul << (_b4)))
#define ZB_BITS5(_b1, _b2, _b3, _b4, _b5) ((1ul << (_b1)) | (1ul << (_b2)) | (1ul << (_b3)) | \
                                           (1ul << (_b4)) | (1ul << (_b5)))

#define ZB_SET_BIT_IN_BIT_VECTOR(vector, nbit) ( (vector)[ (nbit) / 8 ] |= ( 1 << ( (nbit) % 8 )) )
#define ZB_CLR_BIT_IN_BIT_VECTOR(vector, nbit) ( (vector)[ (nbit) / 8 ] &= ~( 1 << ( (nbit) % 8 )) )
#define ZB_CHECK_BIT_IN_BIT_VECTOR(vector, nbit) ( (vector)[ (nbit) / 8 ] & ( 1 << ( (nbit) % 8 )) )
#define ZB_SIZE_OF_BIT_VECTOR(bit_count) (bit_count / 8 + !!(bit_count % 8 != 0))

#endif /* ZB_ZBOSS_API_CORE_H */
