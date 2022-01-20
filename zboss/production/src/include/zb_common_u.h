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
/* PURPOSE: Common utilities include file for ZigBee
*/
#ifndef ZB_COMMON_U_H
#define ZB_COMMON_U_H 1

#include "zboss_api_core.h"

/**
   Generate random value between 0 to 0xffffffff (32 bit) for jitters
 */
zb_uint32_t zb_random_jitter(void);

/**
   Used only for jitters. Need to add a premonition to randoms
 */
#define ZB_RANDOM_JTR(max_value) (zb_random_jitter() / (ZB_RAND_MAX / (zb_uint32_t)(max_value)))

/*! @} */

/*! \internal \addtogroup ZB_BASE */
/*! @{ */

/**
   Generate CRC8 hash
 */
zb_uint8_t zb_crc8(const zb_uint8_t *p, zb_uint8_t crc, zb_uint_t len);

/**
   Generate CRC16 hash for string
 */
zb_uint16_t zb_crc16(const zb_uint8_t *p, zb_uint16_t crc, zb_uint_t len);


/**
   Generate CRC32 hash
 */
zb_uint32_t zb_crc32(const zb_uint8_t *p, zb_uint_t len);

zb_uint32_t zb_crc32_next(zb_uint32_t prev_crc, zb_uint8_t *p, zb_uint_t len);

zb_uint32_t zb_crc32_next_v2(zb_uint32_t prev_crc, zb_uint8_t *p, zb_uint16_t len);

/**
 * Calculates number of '1' in 16bit bitmask
 */
zb_uint8_t zb_bit_cnt16(zb_uint16_t a);

void zb_inverse_bytes(zb_uint8_t *ptr, zb_uint32_t len);

zb_uint_t zb_high_bit_number(zb_uint32_t mask);

zb_uint_t zb_low_bit_number(zb_uint32_t mask);

/*! @} */

#ifdef ZB_TOOL
void zb_init_tool(void);
#endif

#endif /* ZB_COMMON_U_H */
