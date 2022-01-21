/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2021 DSR Corporation, Denver CO, USA.
 * www.dsr-zboss.com
 * www.dsr-corporation.com
 * All rights reserved.
 *
 *
 * Use in source and binary forms, redistribution in binary form only, with
 * or without modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 2. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 3. This software, with or without modification, must only be used with a Nordic
 *    Semiconductor ASA integrated circuit.
 *
 * 4. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/* PURPOSE: Macros for hash calculation
*/

#ifndef ZB_HASH_H
#define ZB_HASH_H 1

/*! @cond internals_doc */
/*! \addtogroup ZB_BASE */
/*! @{ */

/* DJB2 algorithm magic number */
#define ZB_HASH_MAGIC_VAL 5381UL

/**
 * DJB2 hash function step
 *
 * See: [Hash Functions](www.cse.yorku.ca/~oz/hash.html)
 */
#define ZB_HASH_FUNC_STEP(hash_var, v) ((((hash_var) << 5) + (hash_var)) + (v))
#define ZB_4INT_HASH_FUNC(v1, v2, v3, v4) (ZB_HASH_FUNC_STEP(ZB_HASH_FUNC_STEP( ZB_HASH_FUNC_STEP( ZB_HASH_FUNC_STEP(ZB_HASH_MAGIC_VAL, (v1)), (v2)), (v3)), (v4)) & ZB_INT_MASK)
#define ZB_2INT_HASH_FUNC(v1, v2)      (ZB_HASH_FUNC_STEP(ZB_HASH_FUNC_STEP(ZB_HASH_MAGIC_VAL, (v1)), (v2)) & ZB_INT_MASK)

#define ZB_1INT_HASH_FUNC(v1)          (ZB_HASH_FUNC_STEP(ZB_HASH_MAGIC_VAL, (v1)) & ZB_INT_MASK)

/* Calculate a hash using DJB2 hash function */
zb_uint32_t zb_64bit_hash(zb_uint8_t *data);

/**
 * Calculate Fowler-Noll-Vo hash.
 *
 * See: [FNV Hash](www.isthe.com/chongo/tech/comp/fnv/)
 */
zb_uint32_t zb_fnv_32a_uint16(zb_uint16_t v);

/*! @} */
/*! @endcond */

#endif /* ZB_HASH_H */
