/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/*!
 * @brief Poly1305 is a message authentication code created by Daniel J.
 *        Bernstein. It can be used to verify the data integrity and the
 *        authenticity of a message.
 */
/**@file
 * Poly1305 takes a one-time key to produce an authentication tag for a message.
 * Since a key can only be used to authenticate a single message, a new key
 * needs to be derived for each message.
 *
 * @see [RFC 7539 - ChaCha20 and Poly1305 for IETF Protocols](http://tools.ietf.org/html/rfc7539)
 * @see [Poly1305-AES: a state-of-the-art message-authentication code](http://cr.yp.to/mac.html)
 */

#ifndef OCC_POLY1305_H
#define OCC_POLY1305_H

#include <stdint.h>
#include <stddef.h>


/**
 * Key length.
 */
#define occ_poly1305_KEY_BYTES (32)

/**
 * Authenticator length.
 */
#define occ_poly1305_BYTES (16)


/**@cond */
typedef struct {
    uint32_t h[5];
} occ_poly1305_ctx;
/**@endcond */


/**@name Incremental Poly1305 generator.
 * 
 * This group of functions can be used to incrementally compute the Poly1305
 * authenticator for a given message and key.
 *
 * **Example**
 * @include occ_poly1305_incremental.c
 */
/**@{*/
/**
 * Poly1305 generator initialize.
 *
 * The generator state @p ctx is initialized by this function.
 *
 * @param[out] ctx Generator state.
 */
void occ_poly1305_init(occ_poly1305_ctx *ctx);

/**
 * Poly1305 generator.
 *
 * The generator state @p ctx is updated to authenticate a message chunk @p in
 * with a key @p k.
 * 
 * This function can be called repeatedly until the whole message has been
 * processed.
 *
 * @param[in,out] ctx    Generator state.
 * @param         in     Input data.
 * @param         in_len Length of @p in.
 * @param         k      Encryption key.
 *
 * @remark Initialization of the generator state @p ctx through
 *         @c occ_poly1305_init is required before this function can be called.
 *
 * @remark The same key @p k needs to be supplied for all message chunks.
 */
void occ_poly1305_update(occ_poly1305_ctx *ctx,
                         const uint8_t *in, size_t in_len,
                         const uint8_t k[occ_poly1305_KEY_BYTES]);

/**
 * Poly1305 generator output.
 *
 * The generator state @p ctx is updated to finalize the authenticator for the
 * previously processed message chunks with key @p k. The authentication tag is
 * put into @p r.
 *
 * @param[out]    r   Generated authentication tag.
 * @param[in,out] ctx Generator state.
 * @param         k   Encryption key.
 *
 * @remark Initialization of the generator state @p ctx through
 *         @c occ_poly1305_init is required before this function can be called.
 *
 * @remark The same key @p k needs to be supplied that was used in previous
 *         @c occ_poly1305_update invocations.
 *
 * @remark After return, the generator state @p ctx must no longer be used with
 *         @c occ_poly1305_update and @c occ_poly1305_final unless it is
 *         reinitialized using @c occ_poly1305_init.
 */
void occ_poly1305_final(uint8_t r[occ_poly1305_BYTES],
                        occ_poly1305_ctx *ctx,
                        const uint8_t k[occ_poly1305_KEY_BYTES]);
/**@}*/

/**
 * Poly1305 message authentication tag.
 *
 * The Poly1305 authentication of a given input message @p in is computed and
 * put into @p r.
 *
 * **Example**
 * @include occ_poly1305.c
 *
 * @param[out] r      Generated authentication tag.
 * @param      in     Input data.
 * @param      in_len Length of @p in.
 * @param      k      Encryption key.
 */
void occ_poly1305(uint8_t r[occ_poly1305_BYTES],
                  const uint8_t *in, size_t in_len,
                  const uint8_t k[occ_poly1305_KEY_BYTES]);

#endif
