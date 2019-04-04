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

#ifndef _MBEDTLS_CC_EC_MONT_EDW_ERROR_H
#define _MBEDTLS_CC_EC_MONT_EDW_ERROR_H


/*!
@file
@brief This file contains the definitions of the CryptoCell ECC-25519 errors.
@defgroup cc_ecmontedw_error CryptoCell ECC-25519 errors
@{
@ingroup cryptocell_api
*/

#include "cc_error.h"

#ifdef __cplusplus
extern "C"
{
#endif


/************************ Defines ******************************/

/**********************************************************************************************************
 * CryptoCell ECC-25519 MODULE ERRORS    base address - 0x00F02100                *
 **********************************************************************************************************/
/*! Illegal input pointer */
#define CC_EC_EDW_INVALID_INPUT_POINTER_ERROR                 (CC_EC_MONT_EDW_MODULE_ERROR_BASE + 0x00UL)
/*! Illegal input size */
#define CC_EC_EDW_INVALID_INPUT_SIZE_ERROR                (CC_EC_MONT_EDW_MODULE_ERROR_BASE + 0x01UL)
/*! Illegal scalar size */
#define CC_EC_EDW_INVALID_SCALAR_SIZE_ERROR               (CC_EC_MONT_EDW_MODULE_ERROR_BASE + 0x02UL)
/*! Illegal scalar data */
#define CC_EC_EDW_INVALID_SCALAR_DATA_ERROR               (CC_EC_MONT_EDW_MODULE_ERROR_BASE + 0x03UL)
/*! Invalid RND context pointer */
#define CC_EC_EDW_RND_CONTEXT_PTR_INVALID_ERROR              (CC_EC_MONT_EDW_MODULE_ERROR_BASE + 0x04UL)
/*! Invalid RND generate vector functions pointer */
#define CC_EC_EDW_RND_GEN_VECTOR_FUNC_ERROR               (CC_EC_MONT_EDW_MODULE_ERROR_BASE + 0x05UL)
/*! Signing or verification operation failed */
#define CC_EC_EDW_SIGN_VERIFY_FAILED_ERROR                (CC_EC_MONT_EDW_MODULE_ERROR_BASE + 0x20UL)
/*! Illegal input pointer */
#define CC_EC_MONT_INVALID_INPUT_POINTER_ERROR              (CC_EC_MONT_EDW_MODULE_ERROR_BASE + 0x30UL)
/*! Illegal input size */
#define CC_EC_MONT_INVALID_INPUT_SIZE_ERROR                 (CC_EC_MONT_EDW_MODULE_ERROR_BASE + 0x31UL)
/*! Illegal domain id */
#define CC_EC_MONT_INVALID_DOMAIN_ID_ERROR                  (CC_EC_MONT_EDW_MODULE_ERROR_BASE + 0x32UL)
/*! Internal PKI error */
#define CC_ECEDW_INTERNAL_ERROR                             (CC_EC_MONT_EDW_MODULE_ERROR_BASE + 0x33UL)
/*! Internal PKI error */
#define CC_ECMONT_INTERNAL_ERROR                            (CC_EC_MONT_EDW_MODULE_ERROR_BASE + 0x34UL)


/************************************************************************************************************
 *    NOT SUPPORTED MODULES ERROR IDs                                                                       *
 ************************************************************************************************************/
/*! EC montgomery is not supported */
#define CC_EC_MONT_IS_NOT_SUPPORTED                         (CC_ECPKI_MODULE_ERROR_BASE + 0xFEUL)
/*! EC edwards is not supported */
#define CC_EC_EDW_IS_NOT_SUPPORTED                          (CC_ECPKI_MODULE_ERROR_BASE + 0xFFUL)



/************************ Enums ********************************/

/************************ Typedefs  ****************************/

/************************ Structs  ******************************/

/************************ Public Variables **********************/

/************************ Public Functions **********************/

#ifdef __cplusplus
}
#endif
/**
@}
*/
#endif//_MBEDTLS_CC_EC_MONT_EDW_ERROR_H


