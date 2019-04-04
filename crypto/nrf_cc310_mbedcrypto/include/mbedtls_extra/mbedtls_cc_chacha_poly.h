/**************************************************************************************
* Copyright (c) 2016-2018, Arm Limited (or its affiliates). All rights reserved       *
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

/*!
 @file
 @brief This file contains all of the CryptoCell ChaCha-POLY APIs, their enums and definitions.
 */

/*!
 @defgroup cc_chacha_poly CryptoCell ChaCha-POLY APIs
 @brief Contains CryptoCell ChaCha-POLY APIs. See mbedtls_cc_chacha_poly.h.

 @{
 @ingroup cryptocell_api
 @}
 */

#ifndef _MBEDTLS_CC_CHACHA_POLY_H
#define _MBEDTLS_CC_CHACHA_POLY_H

#include "cc_pal_types.h"
#include "cc_error.h"
#include "mbedtls_cc_chacha.h"
#include "mbedtls_cc_poly.h"


#ifdef __cplusplus
extern "C"
{
#endif


/*!
  @brief This function performs the ChaCha-POLY encryption and authentication operation.

  @return \c CC_OK on success.
  @return A non-zero value on failure as defined in mbedtls_cc_chacha_poly_error.h.
*/
CIMPORT_C CCError_t  mbedtls_chacha_poly(
        mbedtls_chacha_nonce             pNonce,             /*!< [in] A pointer to a buffer containing the nonce value. */
        mbedtls_chacha_key               pKey,               /*!< [in] A pointer to the key buffer of the user. */
        mbedtls_chacha_encrypt_mode_t    encryptDecryptFlag, /*!< [in] A flag specifying which operation the ChaCha-POLY module should perform: encrypt or decrypt. */
        uint8_t                          *pAddData,          /*!< [in] A pointer to the additional data input buffer to the POLY module.
                                                                       This pointer does not need to be aligned. Must not be null. */
        size_t                           addDataSize,        /*!< [in] The size of the input data. Must not be zero. */
        uint8_t                          *pDataIn,           /*!< [in] A pointer to the input-data buffer to the ChaCha engine.
                                                                       This pointer does not need to be aligned. Must not be null. */
        size_t                           dataInSize,         /*!< [in] The size of the input data. Must not be zero. */
        uint8_t                          *pDataOut,          /*!< [out] A pointer to the output-data buffer from the ChaCha engine.
                                                                        This pointer does not need to be aligned. Must not be null. */
        mbedtls_poly_mac                 macRes              /*!< [in/out] A pointer to the MAC result buffer.*/
);



#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MBEDTLS_CC_CHACHA_POLY_H */

