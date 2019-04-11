/**************************************************************************************
* Copyright (c) 2016-2018, ARM Limited or its affiliates. All rights reserved         *
*                                                                                     *
* This file and the related binary are licensed under the following license:          *
*                                                                                     *
* ARM Object Code and Header Files License, v1.0 Redistribution.                      *
*                                                                                     *
* Redistribution and use of object code, header files, and documentation, without     *
* modification, are permitted provided that the following conditions are met:         *
*                                                                                     *
* 1) Redistributions must reproduce the above copyright notice and the                *
*    following disclaimer in the documentation and/or other materials                 *
*    provided with the distribution.                                                  *
*                                                                                     *
* 2) Unless to the extent explicitly permitted by law, no reverse                     *
*    engineering, decompilation, or disassembly of is permitted.                      *
*                                                                                     *
* 3) Redistribution and use is permitted solely for the purpose of                    *
*    developing or executing applications that are targeted for use                   *
*    on an ARM-based product.                                                         *
*                                                                                     *
* DISCLAIMER. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND                  *
* CONTRIBUTORS "AS IS." ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT             *
* NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT,        *
* AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE          *
* COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,   *
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED            *
* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR              *
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF              *
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING                *
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS                  *
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                        *
**************************************************************************************/

#ifndef MBEDTLS_PLATFORM_ALT_H
#define MBEDTLS_PLATFORM_ALT_H

#include <stddef.h>
#include <stdint.h>

#if defined(MBEDTLS_CONFIG_FILE)
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_PLATFORM_SETUP_TEARDOWN_ALT)

#define MBEDTLS_RND_CONTEXT_INTERNAL_SIZE       (5)
#define MBEDTLS_RND_WORKBUFFER_INTERNAL_SIZE    (1528)

// These error codes are subject to change
#define MBEDTLS_ERR_PLATFORM_PARAM_NULL	        -0x7001
#define MBEDTLS_ERR_PLATFORM_INTERNAL	        -0x7002
#define MBEDTLS_ERR_PLATFORM_RNG_INIT_FAILED	-0x7003
#define MBEDTLS_ERR_PLATFORM_VERSION_FAILED	-0x7004
#define MBEDTLS_ERR_PLATFORM_PARAM_WRITE_FAILED	-0x7005

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          Internal RNG context structure
 */
typedef struct mbedtls_rng_context_internal
{
        uint32_t buff[MBEDTLS_RND_CONTEXT_INTERNAL_SIZE];

} mbedtls_rng_context_internal;

/**
 * \brief          Internal RNG work buffer structure
 */
typedef struct mbedtls_rng_workbuf_internal {
        /*! Internal buffer */
        uint32_t buff[MBEDTLS_RND_WORKBUFFER_INTERNAL_SIZE];
} mbedtls_rng_workbuf_internal;


/**
 * \brief   The platform context structure.
 *
 * \note    This structure may be used to assist platform-specific
 *          setup or teardown operations.
 *
 * \note    If p_rnd_workbuff is set to NULL, the required memory will
 *          be temporarily allocated.
 *
 */
typedef struct {
    mbedtls_rng_context_internal    rnd_context;
    mbedtls_rng_workbuf_internal  * p_rnd_workbuf;
}
mbedtls_platform_context;


#ifdef __cplusplus
}
#endif

#endif  /* MBEDTLS_PLATFORM_SETUP_TEARDOWN_ALT */

#endif  /* MBEDTLS_PLATFORM_ALT_H */
