/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
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
