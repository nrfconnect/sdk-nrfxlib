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

/*! @file
@brief This file defines bit-field operations macros.
*/

#ifndef _CC_BITOPS_H_
#define _CC_BITOPS_H_


/*! Defintion of number of 32bit maximum value. */
#define CC_32BIT_MAX_VALUE  (0xFFFFFFFFUL)

/*! Definition for bitmask */
#define BITMASK(mask_size) (((mask_size) < 32) ?    \
    ((1UL << (mask_size)) - 1) : 0xFFFFFFFFUL)
/*! Definition for bitmask in a given offset. */
#define BITMASK_AT(mask_size, mask_offset) (BITMASK(mask_size) << (mask_offset))

/*! Definition for getting bits value from a word. */
#define BITFIELD_GET(word, bit_offset, bit_size) \
    (((word) >> (bit_offset)) & BITMASK(bit_size))
/*! Definition for setting bits value from a word. */
#define BITFIELD_SET(word, bit_offset, bit_size, new_val)   do {    \
    word = ((word) & ~BITMASK_AT(bit_size, bit_offset)) |       \
        (((new_val) & BITMASK(bit_size)) << (bit_offset));  \
} while (0)

/*!Definition for is val aligned to "align" ("align" must be power of 2). */
#ifndef IS_ALIGNED
#define IS_ALIGNED(val, align)      \
    (((uintptr_t)(val) & ((align) - 1)) == 0)
#endif
/*!Definition swap endianity for 32 bits word. */
#define SWAP_ENDIAN(word)       \
    (((word) >> 24) | (((word) & 0x00FF0000) >> 8) | \
    (((word) & 0x0000FF00) << 8) | (((word) & 0x000000FF) << 24))

#ifdef BIG__ENDIAN
#define SWAP_TO_LE(word) SWAP_ENDIAN(word)
#define SWAP_TO_BE(word) word
#else
/*! Definition for swapping to LE. */
#define SWAP_TO_LE(word) word
/*! Definition for swapping to BE. */
#define SWAP_TO_BE(word) SWAP_ENDIAN(word)
#endif

/*!Align X to uint32_t size. */
#ifndef ALIGN_TO_4BYTES
#define ALIGN_TO_4BYTES(x)      (((unsigned long)(x) + (CC_32BIT_WORD_SIZE-1)) & ~(CC_32BIT_WORD_SIZE-1))
#endif



/*! Definition for is val a multiple of "mult" ("mult" must be power of 2). */
#define IS_MULT(val, mult)              \
    (((val) & ((mult) - 1)) == 0)

/*! Definition for is NULL address. */
#define IS_NULL_ADDR(adr)       \
    (!(adr))

#endif /*_CC_BITOPS_H_*/
