/*
 * Copyright (c) 2025, Nordic Semiconductor ASA
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

#ifndef NRF_802154_COMPILER_H__
#define NRF_802154_COMPILER_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup nrf_802154_config_compiler Compiler compatibility macros
 * @{
 * @ingroup nrf_802154
 * @brief Compiler compatibility macros for the 802.15.4 radio driver.
 */

/**
 * @def SWITCH_CASE_FALLTHROUGH
 *
 * Portable fallthrough annotation for switch case statements.
 * 
 * This macro provides a portable way to annotate intentional fallthrough
 * between switch case labels across different C standards and compilers:
 * - C23: Uses [[fallthrough]] attribute
 * - GCC 7+/Clang: Uses __attribute__((fallthrough))
 * - Older compilers: Falls back to comment-based annotation
 * 
 * Usage:
 * @code
 * switch (value) {
 *     case 1:
 *         do_something();
 *         SWITCH_CASE_FALLTHROUGH;
 *     case 2:
 *         do_something_else();
 *         break;
 * }
 * @endcode
 */
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 202311L)
    /* C23 and later */
    #define SWITCH_CASE_FALLTHROUGH [[fallthrough]]
#elif defined(__has_attribute)
    /* Clang and GCC with __has_attribute support */
    #if __has_attribute(fallthrough)
        #define SWITCH_CASE_FALLTHROUGH __attribute__((fallthrough))
    #else
        #define SWITCH_CASE_FALLTHROUGH /* fallthrough */
    #endif
#elif defined(__GNUC__) && (__GNUC__ >= 7)
    /* GCC 7+ */
    #define SWITCH_CASE_FALLTHROUGH __attribute__((fallthrough))
#elif defined(__clang__)
    /* Clang (older versions) */
    #if defined(__clang_major__) && (__clang_major__ >= 10)
        #define SWITCH_CASE_FALLTHROUGH __attribute__((fallthrough))
    #else
        #define SWITCH_CASE_FALLTHROUGH /* fallthrough */
    #endif
#else
    /* Fallback for other compilers */
    #define SWITCH_CASE_FALLTHROUGH /* fallthrough */
#endif

/** @} */

#ifdef __cplusplus
}
#endif

#endif // NRF_802154_COMPILER_H__
