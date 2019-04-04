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
 @brief This file contains the platform-dependent definitions of the CryptoCell PKA APIs.
 */

/*!
 @defgroup cc_pka_hw_plat_defs CryptoCell PKA-API platform-dependent types and definitions
 @brief Contains the platform-dependent definitions of the CryptoCell PKA APIs. See cc_pka_hw_plat_defs.h.

 @{
 @ingroup cc_pka
 @}
 */

#ifndef _CC_PKA_HW_PLAT_DEFS_H
#define _CC_PKA_HW_PLAT_DEFS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "cc_pal_types.h"

/*! The size of the PKA engine word. */
#define CC_PKA_WORD_SIZE_IN_BITS            64
/*! The maximal supported size of modulus in bits. */
#define CC_SRP_MAX_MODULUS_SIZE_IN_BITS       3072
/*! The maximal supported size of modulus in RSA in bits. */
#define CC_RSA_MAX_VALID_KEY_SIZE_VALUE_IN_BITS         2048
/*! The maximal supported size of key-generation in RSA in bits. */
#define CC_RSA_MAX_KEY_GENERATION_HW_SIZE_BITS         2048

/*! The maximal supported size of modulus in RSA in words. */
#define CC_RSA_MAX_VALID_KEY_SIZE_VALUE_IN_WORDS        CC_RSA_MAX_VALID_KEY_SIZE_VALUE_IN_BITS / CC_BITS_IN_32BIT_WORD

/*! The size of the Secure Boot or Secure Debug certificate RSA public modulus key in bits. */
#define SB_CERT_RSA_KEY_SIZE_IN_BITS     3072UL
/*! The size of the Secure Boot or Secure Debug certificate RSA public modulus key in Bytes. */
#define SB_CERT_RSA_KEY_SIZE_IN_BYTES    (SB_CERT_RSA_KEY_SIZE_IN_BITS/CC_BITS_IN_BYTE)
/*! The size of the Secure Boot or Secure Debug certificate RSA public modulus key in words. */
#define SB_CERT_RSA_KEY_SIZE_IN_WORDS    (SB_CERT_RSA_KEY_SIZE_IN_BITS/CC_BITS_IN_32BIT_WORD)

/*! The maximal count of extra bits in PKA operations. */
#define PKA_EXTRA_BITS  8
/*! The number of memory registers in PKA operations. */
#define PKA_MAX_COUNT_OF_PHYS_MEM_REGS  32


#ifdef __cplusplus
}
#endif

#endif //_CC_PKA_HW_PLAT_DEFS_H
