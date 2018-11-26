/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * SHA-1 is a cryptographic hash function designed by the NSA.
 *
 * A fixed-sized message digest is computed from input data with arbitrary
 * length. The function is practically impossible to revert, and small changes
 * in the input message lead to major changes in the message digest.
 *
 * SHA-1 is no longer considered secure against well-funded opponents;
 * replacement by SHA-2 or SHA-3 is recommended.
 */

#ifndef OCC_SHA1_H
#define OCC_SHA1_H

#include <stdint.h>
#include <stddef.h>


/**
 * Length of SHA-1 hash.
 */
#define occ_sha1_BYTES (20)


/**
 * SHA-1 hash.
 *
 * The SHA-1 hash of a given input message @p in is computed and put into @p r.
 *
 * **Example**
 * @include occ_sha1.c
 *
 * @param[out] r      Generated hash.
 * @param      in     Input data.
 * @param      in_len Length of @p in.
 */
void occ_sha1(uint8_t r[occ_sha1_BYTES],
              const uint8_t *in, size_t in_len);

#endif
