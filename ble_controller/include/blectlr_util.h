/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: BSD-5-Clause-Nordic
 */

#ifndef BLECTLR_UTIL_H__
#define BLECTLR_UTIL_H__

#include <stdint.h>
#include <stdbool.h>

void blectlr_set_default_evt_length(void);

void cal_init(void);

void soc_rand_prio_low_vector_get_blocking(uint8_t * p_buff, uint8_t length);

void ll_util_revcpy(uint8_t *dest, const uint8_t *src, uint8_t size);

void ll_util_block_encrypt(const uint8_t key[16],
                           const uint8_t plaintext[16],
                           bool is_result_le,
                           uint8_t ciphertext[16]);

#endif
