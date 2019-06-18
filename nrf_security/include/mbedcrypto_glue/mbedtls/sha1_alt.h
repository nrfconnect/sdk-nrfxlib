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

#ifndef MBEDTLS_SHA1_ALT_H
#define MBEDTLS_SHA1_ALT_H

#include <stddef.h>
#include <stdint.h>

#if defined (MBEDTLS_CONFIG_FILE)
#include MBEDTLS_CONFIG_FILE
#endif

#if defined (MBEDTLS_SHA1_ALT)

#define MBEDTLS_ERR_SHA1_HW_ACCEL_FAILED                -0x0035  /**< SHA-1 hardware accelerator failed */

/*! The Size of the hash context for SHA1.*/
#define MBEDTLS_SHA1_CONTEXT_SIZE_IN_WORDS              (60)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          SHA-1 context structure
 */
typedef struct mbedtls_sha1_context {
        /*! Internal buffer */
        uint32_t buff[MBEDTLS_SHA1_CONTEXT_SIZE_IN_WORDS]; // defined in cc_hash_defs_proj.h
} mbedtls_sha1_context;

#ifdef __cplusplus
}
#endif

#endif /*  MBEDTLS_SHA1_ALT  */

#endif /* MBEDTLS_SHA1_ALT_H */
