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
 @brief This file contains the error definitions of the CryptoCell HKDF APIs.
 */

/*!
 @defgroup cc_hkdf_error Specific errors of the HKDF key-derivation APIs
 @brief Contains the CryptoCell HKDF-API error definitions. See mbedtls_cc_hkdf_error.h.
 @{
 @ingroup cc_hkdf
 @}
 */

#ifndef _MBEDTLS_CC_HKDF_ERROR_H
#define _MBEDTLS_CC_HKDF_ERROR_H

#include "cc_error.h"


#ifdef __cplusplus
extern "C"
{
#endif

/************************ Defines *******************************/

/* The base address for the CryptoCell HKDF module errors - 0x00F01100. */
/*! Invalid argument. */
#define CC_HKDF_INVALID_ARGUMENT_POINTER_ERROR      (CC_HKDF_MODULE_ERROR_BASE + 0x0UL)
/*! Invalid argument size. */
#define CC_HKDF_INVALID_ARGUMENT_SIZE_ERROR         (CC_HKDF_MODULE_ERROR_BASE + 0x1UL)
/*! Illegal hash mode. */
#define CC_HKDF_INVALID_ARGUMENT_HASH_MODE_ERROR        (CC_HKDF_MODULE_ERROR_BASE + 0x3UL)
/*! HKDF not supported. */
#define CC_HKDF_IS_NOT_SUPPORTED                              (CC_HKDF_MODULE_ERROR_BASE + 0xFFUL)

/************************ Enums *********************************/

/************************ Typedefs  *****************************/

/************************ Structs  ******************************/

/************************ Public Variables **********************/

/************************ Public Functions **********************/




#ifdef __cplusplus
}
#endif

#endif //_MBEDTLS_CC_HKDF_ERROR_H

