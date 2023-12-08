/*
 * Copyright (c) 2020, Nordic Semiconductor ASA
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
 * @file
 * @defgroup nrf_802154_sl_debug_logging Debug logging service
 * @{
 * @ingroup nrf_802154
 * @brief Execution backtrace and internal events logging service.
 *
 * To obtain unambiguous output of debug logs, it is required to use a unique identifier
 * in each @c .c module. See the following distribution of IDs for debug logs in component libraries:
 * - ranges for .c modules:
 *   - [  1 .. 31 ] for 802.15.4 Driver modules (NRF_802154_DRV_MODULE_ID_..)
 *   - [ 32 .. 39 ] for 802.15.4 MPSL modules (NRF_802154_MPSL_MODULE_ID_..)
 *   - [ 40 .. 63 ] for 802.15.4 SL modules (NRF_802154_SL_MODULE_ID_..)
 * - ranges for global events (NRF_802154_LOG_GLOBAL_EVENT_ID_..) :
 *   - [  1 .. 31 ] for global events in 802.15.4 Driver
 *   - [ 32 .. 39 ] for global events in 802.15.4 MPSL
 *   - [ 40 .. 63 ] for global events in 802.15.4 SL
 *
 */

#ifndef NRF_802154_SL_LOG_H__
#define NRF_802154_SL_LOG_H__

#include <stdint.h>

/**@def NRF_802154_SL_DEBUG_LOG_BUFFER_LEN
 * @brief Configures capacity of debug log buffer.
 *
 * The value of this macro determines how many entries can be stored in the log buffer.
 * If the buffer capacity is exceeded, the oldest entries are overwritten.
 *
 * @note This value must be power of 2.
 */
#ifndef NRF_802154_SL_DEBUG_LOG_BUFFER_LEN
#define NRF_802154_SL_DEBUG_LOG_BUFFER_LEN 1024U
#endif

/**@def NRF_802154_SL_DEBUG_LOG_BLOCKS_INTERRUPTS
 * @brief Configures if writing to log buffer is performed with interrupts disabled.
 *
 * Setting this macro to 1 has following consequences:
 * - Interrupts are automatically disabled during write to log buffer. This ensures
 *   thread-safety and always coherent log.
 * - Higher priority interrupts may be delayed, so logging has impact on timing.
 *
 * Setting this macro to 0 has following consequences:
 * - Interrupts are NOT disabled during write to log buffer. This may lead to missing
 *   logs if higher priority interrupt preempts current write log operation.
 * - Logging does not introduce delay to execution of higher priority interrupts.
 */
#ifndef NRF_802154_SL_DEBUG_LOG_BLOCKS_INTERRUPTS
#define NRF_802154_SL_DEBUG_LOG_BLOCKS_INTERRUPTS 0
#endif

/**@def NRF_802154_SL_LOG_VERBOSITY
 * @brief Defines the verbosity level of generated logs.
 *
 * Define this macro in your @c .c file before inclusion of @c nrf_802154_debug_log.h to
 * set verbosity level per-module basis.
 */
#ifndef NRF_802154_SL_LOG_VERBOSITY
#define NRF_802154_SL_LOG_VERBOSITY   NRF_802154_LOG_VERBOSITY_LOW
#endif

#define NRF_802154_LOG_VERBOSITY_NONE 0
#define NRF_802154_LOG_VERBOSITY_LOW  1
#define NRF_802154_LOG_VERBOSITY_HIGH 2

#if (NRF_802154_SL_DEBUG_LOG_BLOCKS_INTERRUPTS)

#include "nrf_802154_sl_utils.h"

/**@brief Declares a variable for storing interrupts state before disabling interrupts. */
#define nrf_802154_sl_debug_log_saved_interrupt_st_variable(var_name) \
    nrf_802154_sl_mcu_critical_state_t var_name

#define nrf_802154_sl_debug_log_disable_interrupts(var_name) \
    nrf_802154_sl_mcu_critical_enter(var_name)               \

#define nrf_802154_sl_debug_log_restore_interrupts(var_name) \
    nrf_802154_sl_mcu_critical_exit(var_name)                \

#else

#define nrf_802154_sl_debug_log_saved_interrupt_st_variable(var_name) /* empty macro */

#define nrf_802154_sl_debug_log_disable_interrupts(var_name) \
    do                                                       \
    {                                                        \
    }                                                        \
    while (0)

#define nrf_802154_sl_debug_log_restore_interrupts(var_name) \
    do                                                       \
    {                                                        \
    }                                                        \
    while (0)

#endif

/**@brief Checks if the provided @p verbosity parameter has a value that allows the module to record a log. */
#define nrf_802154_sl_log_verbosity_allows(verbosity) \
    (((verbosity) > 0) && ((verbosity) <= NRF_802154_SL_LOG_VERBOSITY))

#if !defined(CU_TEST) && (NRF_802154_SL_ENABLE_DEBUG_LOG)

extern volatile uint32_t g_nrf_802154_sl_log_buffer[NRF_802154_SL_DEBUG_LOG_BUFFER_LEN];
extern volatile uint32_t gp_nrf_802154_sl_log_ptr;

/**@brief Writes one word into debug log buffer. */
#define nrf_802154_sl_debug_log_write_raw(value)                                                \
    do                                                                                          \
    {                                                                                           \
        uint32_t nrf_802154_sl_debug_log_wr_raw_value = (value);                                \
                                                                                                \
        nrf_802154_sl_debug_log_saved_interrupt_st_variable(nrf_802154_sl_debug_log_wr_raw_sv); \
        nrf_802154_sl_debug_log_disable_interrupts(nrf_802154_sl_debug_log_wr_raw_sv);          \
                                                                                                \
        uint32_t nrf_802154_sl_debug_log_write_raw_ptr = gp_nrf_802154_sl_log_ptr;              \
                                                                                                \
        g_nrf_802154_sl_log_buffer[nrf_802154_sl_debug_log_write_raw_ptr] =                     \
            nrf_802154_sl_debug_log_wr_raw_value;                                               \
        nrf_802154_sl_debug_log_write_raw_ptr += 1U;                                            \
        nrf_802154_sl_debug_log_write_raw_ptr &= (NRF_802154_SL_DEBUG_LOG_BUFFER_LEN - 1U);     \
        gp_nrf_802154_sl_log_ptr               = nrf_802154_sl_debug_log_write_raw_ptr;         \
                                                                                                \
        nrf_802154_sl_debug_log_restore_interrupts(nrf_802154_sl_debug_log_wr_raw_sv);          \
    }                                                                                           \
    while (0)

#else // !defined(CU_TEST) && (NRF_802154_SL_ENABLE_DEBUG_LOG)

#define nrf_802154_sl_debug_log_write_raw(value) \
    do                                           \
    {                                            \
    }                                            \
    while (0)

#endif // !defined(CU_TEST) && (NRF_802154_SL_ENABLE_DEBUG_LOG)

/**@name nrf_802154_sl_debug_log_entry_types
 * @brief Types of log entries.
 *
 * Value 0 is reserved and can't be used (reserved for empty log entry).
 * Allowed values are in range [ 1 .. 15 ].
 *
 * @{
 */
#define NRF_802154_LOG_TYPE_FUNCTION_ENTER 1U
#define NRF_802154_LOG_TYPE_FUNCTION_EXIT  2U
#define NRF_802154_LOG_TYPE_LOCAL_EVENT    3U
#define NRF_802154_LOG_TYPE_GLOBAL_EVENT   4U
/**
 *@}
 **/

/**@brief Bit shift of field "log type" in log word. */
#define NRF_802154_SL_DEBUG_LOG_TYPE_BITPOS      28

/**@brief Bit shift of field "module id" in log word. */
#define NRF_802154_SL_DEBUG_LOG_MODULE_ID_BITPOS 22

/**@brief Bit shift of field "event id" in log word. */
#define NRF_802154_SL_DEBUG_LOG_EVENT_ID_BITPOS  16

/**@brief Records log about entry to a function.
 *
 * @param[in] verbosity     Verbosity level required to emit log.
 */
#define nrf_802154_sl_log_function_enter(verbosity)                                             \
    do                                                                                          \
    {                                                                                           \
        if (nrf_802154_sl_log_verbosity_allows(verbosity))                                      \
        {                                                                                       \
            nrf_802154_sl_debug_log_write_raw(                                                  \
                ((NRF_802154_LOG_TYPE_FUNCTION_ENTER) << NRF_802154_SL_DEBUG_LOG_TYPE_BITPOS) | \
                ((NRF_802154_MODULE_ID) << NRF_802154_SL_DEBUG_LOG_MODULE_ID_BITPOS) |          \
                ((uint32_t)((uintptr_t)(__func__) & 0x000FFFFFUL) << 0));                       \
        }                                                                                       \
    }                                                                                           \
    while (0)

/**@brief Records log about exit from a function.
 *
 * @param[in] verbosity     Verbosity level required to emit log.
 */
#define nrf_802154_sl_log_function_exit(verbosity)                                             \
    do                                                                                         \
    {                                                                                          \
        if (nrf_802154_sl_log_verbosity_allows(verbosity))                                     \
        {                                                                                      \
            nrf_802154_sl_debug_log_write_raw(                                                 \
                ((NRF_802154_LOG_TYPE_FUNCTION_EXIT) << NRF_802154_SL_DEBUG_LOG_TYPE_BITPOS) | \
                ((NRF_802154_MODULE_ID) << NRF_802154_SL_DEBUG_LOG_MODULE_ID_BITPOS) |         \
                ((uint32_t)((uintptr_t)(__func__) & 0x000FFFFFUL) << 0));                      \
        }                                                                                      \
    }                                                                                          \
    while (0)

/**@brief Records log about event (with parameter) related to current module.
 *
 * @param[in] verbosity         Verbosity level required to emit log.
 * @param[in] local_event_id    Event identifier whose meaning is defined within the scope
 *                              of the module. Possible values: [ 0 .. 63 ].
 * @param[in] param_u16         Additional parameter to be logged with event. Meaning
 *                              of the parameter is defined by the module in which
 *                              the log is recorded and @p local_event_id.
 */
#define nrf_802154_sl_log_local_event(verbosity, local_event_id, param_u16)                  \
    do                                                                                       \
    {                                                                                        \
        if (nrf_802154_sl_log_verbosity_allows(verbosity))                                   \
        {                                                                                    \
            nrf_802154_sl_debug_log_write_raw(                                               \
                ((NRF_802154_LOG_TYPE_LOCAL_EVENT) << NRF_802154_SL_DEBUG_LOG_TYPE_BITPOS) | \
                ((NRF_802154_MODULE_ID) << NRF_802154_SL_DEBUG_LOG_MODULE_ID_BITPOS) |       \
                (((uint32_t)(local_event_id)) << NRF_802154_SL_DEBUG_LOG_EVENT_ID_BITPOS) |  \
                ((uint16_t)(param_u16) << 0));                                               \
        }                                                                                    \
    }                                                                                        \
    while (0)

/**@brief Records log about event (with parameter) related to global resource.
 *
 * @param[in] verbosity     Verbosity level required to emit log.
 * @param[in] event_id      Event identifier whose meaning is defined globally. Possible values: [ 0 .. 63 ].
 * @param[in] param_u16     Additional parameter to be logged with event. Meaning
 *                          of the parameter is defined by value of @p global_event_id.
 */
#define nrf_802154_sl_log_global_event(verbosity, global_event_id, param_u16)                 \
    do                                                                                        \
    {                                                                                         \
        if (nrf_802154_sl_log_verbosity_allows(verbosity))                                    \
        {                                                                                     \
            nrf_802154_sl_debug_log_write_raw(                                                \
                ((NRF_802154_LOG_TYPE_GLOBAL_EVENT) << NRF_802154_SL_DEBUG_LOG_TYPE_BITPOS) | \
                ((NRF_802154_MODULE_ID) << NRF_802154_SL_DEBUG_LOG_MODULE_ID_BITPOS) |        \
                (((uint32_t)(global_event_id)) << NRF_802154_SL_DEBUG_LOG_EVENT_ID_BITPOS) |  \
                ((uint16_t)(param_u16) << 0));                                                \
        }                                                                                     \
    }                                                                                         \
    while (0)

/**@brief Helper macro for defining one element of the list of local events.
 *
 * @param[in] module_lbl    name of module
 * @param[in] event_lbl     name of local event
 * @param[in] event_id      numeric id of local event (unique in module @c module_lbl)
 * @param[in] event_enum    enum that defines the set of possible values of parameter
                            for event @c event_lbl
 */
#define NRF_802154_LOG_L_EVENT_DEFINE(module_lbl, event_lbl, event_id, event_enum) \
    NRF_802154_LOG_LOCAL_EVENT_ID_ ## module_lbl ## __ ## event_lbl = event_id,    \
    NRF_802154_LOG_LE ## event_id ## _PARAM_ENUM_ ## event_enum     = event_id,

/**
 * @brief Initializes the Debug Logs module.
 *
 */
void nrf_802154_sl_log_init(void);

/**
 *@}
 **/

#endif // NRF_802154_SL_LOG_H__
