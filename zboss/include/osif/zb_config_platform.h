/* ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2018 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: Compiler and CPU specific configuration.
*/

#ifndef ZB_CONFIG_PLATFORM_H__
#define ZB_CONFIG_PLATFORM_H__

#include <string.h>             /* memcpy */
#include <stdint.h>


#ifdef __IAR_SYSTEMS_ICC__
#ifndef ZB_IAR
#define ZB_IAR
#endif
#endif

/* The MAIN() macro is meant to be used only by applications and tests implemented within ZBOSS source code repository. */
#define MAIN() int zboss_app_main(void)
#define FAKE_ARGV
#define ARGV_UNUSED
#define MAIN_RETURN(v) return (v)

#ifdef KEIL
#define ZB_MEMCPY(dst, src, size)   memcpy((void*)(dst), (void*)(src), (size))
#define ZB_MEMSET(dst, val, size)   memset((void*)(dst), (val), (size))
#define ZB_MEMMOVE(dst, src, size)  memmove((void*)(dst), (void*)(src), (size))
#define ZB_MEMCMP(ptr1, ptr2, size)	memcmp((void*)(ptr1), (void*)(ptr2), (size))

#else /* KEIL */
#define ZB_MEMCPY   memcpy
#define ZB_MEMSET   memset
#define ZB_MEMMOVE  memmove
#define ZB_MEMCMP   memcmp
#endif /* KEIL */


/* Enable default parameters, specific for nRF52 platform. */
#define ZB_CONFIG_NRF52
#define ZB_PLATFORM_NRF52

#define ZB_LITTLE_ENDIAN
#define ZB_NEED_ALIGN
#define ZB_VOLATILE
#define ZB_CALLBACK
#define ZB_SDCC_BANKED
#define ZB_KEIL_REENTRANT

#define ZB_BZERO(s,l)  ZB_MEMSET((char*)(s), 0, (l))
#define ZB_BZERO2(s)   ZB_BZERO(s, 2)
#define ZVUNUSED(v)    (void)v


/* At ARM all types from 1 to 4 bytes are passed to vararg with casting to 4 bytes */
typedef uint32_t zb_minimal_vararg_t;

#endif /* ZB_CONFIG_PLATFORM_H__ */
