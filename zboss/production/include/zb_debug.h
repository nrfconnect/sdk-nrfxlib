/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2024 DSR Corporation, Denver CO, USA.
 * www.dsr-zboss.com
 * www.dsr-corporation.com
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
/* PURPOSE: Tests and debug macros
*/
#ifndef ZB_DEBUG_H
#define ZB_DEBUG_H 1

/*! \addtogroup ZB_ASSERT_DYNAMIC */
/*! @{ */

#if (defined USE_ASSERT)
/**
   Trace current location, abort current program execution, with core dump if possible.

   @param caller_file - source file name
   @param caller_line - line in the source
 */
void zb_abort(char *caller_file, int caller_line);

#ifndef ZB_BINARY_TRACE
/** @cond internals_doc */
/**
   Assert: print diagnostic and force core dump

   @param file_name - source file name
   @param line_number - line in the source
*/
void zb_assert(const zb_char_t *file_name, zb_int_t line_number);
/** @endcond */
/**
   Check for expression in runtime and call zb_assert() if it is false.

   Compiled to nothing if USE_ASSERT is not defined.

   @param expr expression to check
*/
#define ZB_ASSERT(expr) {if(!(expr)){zb_assert(__FILE__, __LINE__);}}
/** @cond internals_doc */
#define ZB_INLINE_ASSERT_SIMPLE(expr)  ((expr) ? 1 : (zb_assert(__FILE__, __LINE__), 1))
/** @endcond */

#else  /* !ZB_BINARY_TRACE */
/**
   Assert: print diagnostic and force core dump

   @param file_id - source file id
   @param line_number - line in the source
*/
void zb_assert(zb_uint16_t file_id, zb_int_t line_number);
/**
   Check for expression in runtime and call zb_assert() if it is false.

   Compiled to nothing if USE_ASSERT is not defined.

   @param expr expression to check
*/
#define ZB_ASSERT(expr) {if(!(expr)) { zb_assert(ZB_TRACE_FILE_ID, __LINE__);} }
/** @cond internals_doc */
#define ZB_INLINE_ASSERT_SIMPLE(expr)  ((expr) ? 1 : (zb_assert(ZB_TRACE_FILE_ID, __LINE__), 1))
/** @endcond */

#endif  /* !ZB_BINARY_TRACE */

/** @cond internals_doc */
void lwip_zb_assert(zb_uint16_t file_id, zb_int_t line_number);
/** @endcond */

/**
   Assert which can be used inside an expression.

   @param expr - expression to check.
   @return always 1 (true)
 */
#define ZB_INLINE_ASSERT(expr)  ZB_INLINE_ASSERT_SIMPLE((expr)),

#else  /* release */

#define ZB_ASSERT(expr) ((void)(expr))
#define ZB_INLINE_ASSERT_SIMPLE(expr) -1
#define ZB_INLINE_ASSERT(expr)

#endif

/*! @} */

/** \addtogroup ZB_ASSERT_STATIC */
/** @{ */

/** @cond internals_doc */
/* Tricks to force preprocessor to substitute __LINE__ in macros */
#define ZB_ASSERT_CAT_(a, b) a##_##b
#define ZB_ASSERT_CAT(a, b) ZB_ASSERT_CAT_(a, b)
/** @endcond */

/**
   Check condition at compile time in the code (not declaration block)

   @param expr - expression to check.
 */
#if defined SDCC || defined KEIL
#define ZB_ASSERT_COMPILE_TIME(expr)
#else
#define ZB_ASSERT_COMPILE_TIME(expr) ((void)(zb_int_t (*)[(expr) ? 1 : -1])0)
#endif
/**
   Check condition at compile time in the code (not declaration block), return 0
   To be used inside an expressions.

   @param expr - expression to check.
 */
#define ZB_ASSERT_COMPILE_TIME_EXPR(expr) ((zb_int_t)((zb_int_t (*)[(expr) ? 1 : -1])0))

/**
   Check condition at compile time in the declaration block

   @param expr - expression to check.
 */
#ifdef __cplusplus
#define ZB_ASSERT_COMPILE_DECL(expr) __extension__ static_assert(expr, "Assert at line __LINE__")
#elif (defined __GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6))
#define ZB_ASSERT_COMPILE_DECL(expr) __extension__ _Static_assert(expr, "Assert at line __LINE__")
#elif defined(__COUNTER__)
#define ZB_ASSERT_COMPILE_DECL(expr) typedef zb_int_t ZB_ASSERT_CAT(assert, ZB_ASSERT_CAT(__LINE__, __COUNTER__))[(expr) ? 1 : -1]
#else
#define ZB_ASSERT_COMPILE_DECL(expr)
#endif


/**
   Ensures, that size of type `type' is not greater than `limit'.  If it is not,
   compilation is aborted.

   @param type - type name
   @param limit - size limit
 */
#define ZB_ASSERT_TYPE_SIZE_NOT_GREATER(type, limit) ZB_ASSERT_COMPILE_DECL(sizeof(type) <= (limit))

/**
   Ensures, that size of type `type' is not less than `limit'.  If it is not,
   compilation is aborted.

   @param type - type name
   @param limit - size limit
 */
#define ZB_ASSERT_TYPE_SIZE_NOT_LESS(type, limit) ZB_ASSERT_COMPILE_DECL(sizeof(type) >= (limit))

/**
   Ensures, that size of type `type' is equal to the `limit'.  If it is not,
   compilation is aborted.

   @param type - type name
   @param limit - size limit
 */
#define ZB_ASSERT_TYPE_SIZE_EQ(type, limit) ZB_ASSERT_COMPILE_DECL(sizeof(type) == (limit))

/*! @} */

#ifdef ZB_ARRAYS_CHECK
/**
   Address all array elements. Use this macro to test that array really inited
   by valgrind in Linux
 */
#define ZB_CHK_ARR(arr, len)                       \
  do                                               \
  {                                                \
  zb_ushort_t _ii;                                 \
  zb_ushort_t n = 0;                               \
  for (_ii = 0 ; _ii < (len) ; ++_ii)              \
  {                                                \
    if ((arr)[_ii] < 2)                            \
    {                                              \
      n++;                                         \
    }                                              \
    else                                           \
    {                                              \
      n--;                                         \
    }                                              \
  }                                                \
  }                                                \
while (0)

#else
#define ZB_CHK_ARR(arr, len)
#endif  /* ZB_ARRAYS_CHECK */


#if !defined ZB_NS_BUILD && defined ZB8051 && defined C8051F120
#define ZB_P3_ON() LED1 = 0
#define ZB_P3_OFF() LED1 = 1
#define ZB_P4_ON() LED2 = 0
#define ZB_P4_OFF() LED2 = 1
#else
#define ZB_P3_ON()
#define ZB_P3_OFF()
#define ZB_P4_ON()
#define ZB_P4_OFF()
#endif

#if defined ZB_TRAFFIC_DUMP_ON
/**
   Dump array of byte

   @param buf - array of byte
   @param len - size of array
 */
void dump_traf(zb_uint8_t *buf, zb_ushort_t len);
#endif /* DEBUG */

#ifdef ZB_TRAFFIC_DUMP_ON
void dump_usb_traf(zb_uint8_t *buf, zb_ushort_t len);
#else
#define dump_usb_traf(buf, len)
#endif

#if (defined ZB_MAC_TESTING_MODE) && (defined ZB_TRAFFIC_DUMP_ON)
#define DUMP_TRAF(cmt, buf, len, total) TRACE_MSG(TRACE_MAC3, #cmt, (FMT__0)); dump_traf(buf, len)
#else
#define DUMP_TRAF(comment, buf, len, total)
#endif

#ifdef DEBUG
void dump_hex_data(zb_uint_t trace_mask, zb_uint8_t trace_level, zb_uint8_t *buf, zb_ushort_t len);

void trace_hex_data_func(zb_uint8_t *ptr, zb_short_t size, zb_bool_t format);
#define trace_8hex_per_line(ptr, size) trace_hex_data_func((ptr), (size), ZB_FALSE)
#define trace_16hex_per_line(ptr, size) trace_hex_data_func((ptr), (size), ZB_TRUE)
#else
#define dump_hex_data(trace_mask, trace_level, buf, len)

#define trace_8hex_per_line(ptr, size)
#define trace_16hex_per_line(ptr, size)
#endif  /* DEBUG */

#endif /* ZB_DEBUG_H */
