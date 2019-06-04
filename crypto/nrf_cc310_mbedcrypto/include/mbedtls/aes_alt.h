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

#ifndef MBEDTLS_AES_ALT_H
#define MBEDTLS_AES_ALT_H

#include <stddef.h>
#include <stdint.h>

#if defined(MBEDTLS_CONFIG_FILE)
#include MBEDTLS_CONFIG_FILE
#endif


#if defined(MBEDTLS_AES_ALT)


/* padlock.c and aesni.c rely on these values! */
#define MBEDTLS_AES_ENCRYPT     1 /**< AES encryption. */
#define MBEDTLS_AES_DECRYPT     0 /**< AES decryption. */

/* Error codes in range 0x0020-0x0022 */
#define MBEDTLS_ERR_AES_INVALID_KEY_LENGTH                -0x0020  /**< Invalid key length. */
#define MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH              -0x0022  /**< Invalid data input length. */

/* Error codes in range 0x0023-0x0025 */
#define MBEDTLS_ERR_AES_FEATURE_UNAVAILABLE               -0x0023  /**< Feature not available. For example, an unsupported AES key size. */
#define MBEDTLS_ERR_AES_HW_ACCEL_FAILED                   -0x0025  /**< AES hardware accelerator failed. */


/* The Size of the AES context.*/
#define MBEDTLS_AES_CONTEXT_SIZE_IN_WORDS           (24)


#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          AES context structure
 */
typedef struct
{
    uint32_t buf[MBEDTLS_AES_CONTEXT_SIZE_IN_WORDS];
} mbedtls_aes_context;

typedef struct mbedtls_aes_xts_context
{
    mbedtls_aes_context crypt; /*!< The AES context to use for AES block
                                        encryption or decryption. */
    mbedtls_aes_context tweak; /*!< The AES context used for tweak
                                        computation. */
} mbedtls_aes_xts_context;

#ifdef __cplusplus
}
#endif

#endif  /* MBEDTLS_AES_ALT */

#endif /* MBEDTLS_AES_ALT_H */
