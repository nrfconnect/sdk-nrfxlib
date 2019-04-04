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
 @brief This file contains definitions and types of CryptoCell PAL platform-dependent APIs.
 */

/*!
 @defgroup cc_pal_types CryptoCell PAL platform-dependent definitions and types
 @brief Contains CryptoCell PAL platform-dependent definitions and types. See cc_pal_types.h.

 @{
 @ingroup cc_pal
 @}
*/

#ifndef CC_PAL_TYPES_H
#define CC_PAL_TYPES_H

#include "cc_pal_types_plat.h"

/*! Boolean types.*/
typedef enum {
    /*! Boolean false definition.*/
    CC_FALSE = 0,
    /*! Boolean true definition.*/
    CC_TRUE = 1
} CCBool;

/*! Success definition. */
#define CC_SUCCESS              0UL
/*! Failure definition. */
#define CC_FAIL         1UL

/*! Success (OK) definition. */
#define CC_OK   0

/*! Handles unused parameters in the code, to avoid compilation warnings.  */
#define CC_UNUSED_PARAM(prm)  ((void)prm)

/*! The maximal uint32 value.*/
#define CC_MAX_UINT32_VAL   (0xFFFFFFFF)


/* Minimal and Maximal macros */
#ifdef  min
/*! Definition for minimal calculation. */
#define CC_MIN(a,b) min( a , b )
#else
/*! Definition for minimal calculation. */
#define CC_MIN( a , b ) ( ( (a) < (b) ) ? (a) : (b) )
#endif

#ifdef max
/*! Definition for maximal calculation. */
#define CC_MAX(a,b) max( a , b )
#else
/*! Definition for maximal calculation.. */
#define CC_MAX( a , b ) ( ( (a) > (b) ) ? (a) : (b) )
#endif

/*! This macro calculates the number of full Bytes from bits, where seven bits are one Byte. */
#define CALC_FULL_BYTES(numBits)        ((numBits)/CC_BITS_IN_BYTE + (((numBits) & (CC_BITS_IN_BYTE-1)) > 0))
/*! This macro calculates the number of full 32-bit words from bits where 31 bits are one word. */
#define CALC_FULL_32BIT_WORDS(numBits)      ((numBits)/CC_BITS_IN_32BIT_WORD +  (((numBits) & (CC_BITS_IN_32BIT_WORD-1)) > 0))
/*! This macro calculates the number of full 32-bit words from Bytes where three Bytes are one word. */
#define CALC_32BIT_WORDS_FROM_BYTES(sizeBytes)  ((sizeBytes)/CC_32BIT_WORD_SIZE + (((sizeBytes) & (CC_32BIT_WORD_SIZE-1)) > 0))
/*! This macro calculates the number of full 32-bit words from 64-bits dwords. */
#define CALC_32BIT_WORDS_FROM_64BIT_DWORD(sizeWords)  (sizeWords * CC_32BIT_WORD_IN_64BIT_DWORD)
/*! This macro rounds up bits to 32-bit words. */
#define ROUNDUP_BITS_TO_32BIT_WORD(numBits)     (CALC_FULL_32BIT_WORDS(numBits) * CC_BITS_IN_32BIT_WORD)
/*! This macro rounds up bits to Bytes. */
#define ROUNDUP_BITS_TO_BYTES(numBits)      (CALC_FULL_BYTES(numBits) * CC_BITS_IN_BYTE)
/*! This macro rounds up Bytes to 32-bit words. */
#define ROUNDUP_BYTES_TO_32BIT_WORD(sizeBytes)  (CALC_32BIT_WORDS_FROM_BYTES(sizeBytes) * CC_32BIT_WORD_SIZE)
/*! Definition of 1 KB in Bytes. */
#define CC_1K_SIZE_IN_BYTES 1024
/*! Definition of number of bits in a Byte. */
#define CC_BITS_IN_BYTE     8
/*! Definition of number of bits in a 32-bits word. */
#define CC_BITS_IN_32BIT_WORD   32
/*! Definition of number of Bytes in a 32-bits word. */
#define CC_32BIT_WORD_SIZE  4
/*! Definition of number of 32-bits words in a 64-bits dword. */
#define CC_32BIT_WORD_IN_64BIT_DWORD 2


#endif

