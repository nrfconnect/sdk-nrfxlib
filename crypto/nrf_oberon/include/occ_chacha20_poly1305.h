/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/*!
 * @brief ChaCha20-Poly1305 is an authenticated encryption algorithm with
 *        optional additional authenticated data developed by Daniel J.
 *        Bernstein.
 */
/**@file
 * The ChaCha20 stream cipher is combined with the Poly1305 authenticator.
 *
 * @see [RFC 7539 - ChaCha20 and Poly1305 for IETF Protocols](http://tools.ietf.org/html/rfc7539)
 */

#ifndef OCC_CHACHA20_POLY1305_H
#define OCC_CHACHA20_POLY1305_H

#include <stdint.h>
#include <stddef.h>


/**
 * Length of the encryption key.
 */
#define occ_chacha20_poly1305_KEY_BYTES (32)

/**
 * Maximum length of the nonce.
 */
#define occ_chacha20_poly1305_NONCE_BYTES_MAX (12)

/**
 * Length of the authentication tag.
 */
#define occ_chacha20_poly1305_TAG_BYTES (16)


/**@{*/
/**
 * AEAD ChaCha20-Poly1305 encrypt.
 *
 * The message @p m is encrypted using a ChaCha20 cipher stream derived from the
 * encryption key @p k and the nonce @p n. The resulting ciphertext has the same
 * length @p m_len as the input message @p m and is put into @p c.
 *
 * Additionally, the ciphertext @p c is authenticated with a tag that is
 * generated with Poly1305 using a unique subkey derived from @p k and @p n, and
 * then put into @p tag.
 *
 * **Example**
 * @include occ_chacha20_poly1305_encrypt.c
 *
 * @param[out] tag   Generated authentication tag.
 * @param[out] c     Generated ciphertext. Same length as input message.
 * @param      m     Input message.
 * @param      m_len Length of @p m and @p c.
 * @param      n     Nonce.
 * @param      n_len Length of @p n. 0 <= @p n_len <= @c occ_chacha20_poly1305_NONCE_BYTES_MAX.
 * @param      k     Encryption key.
 *
 * @remark @p c and @p m can point to the same address.
 *
 * @remark When reusing an encryption key @p k for a different message @p m, a
 *         different nonce @p n must be used.
 */
void occ_chacha20_poly1305_encrypt(uint8_t tag[occ_chacha20_poly1305_TAG_BYTES],
                                   uint8_t *c,
                                   const uint8_t *m, size_t m_len,
                                   const uint8_t *n, size_t n_len,
                                   const uint8_t k[occ_chacha20_poly1305_KEY_BYTES]);

/**
 * AEAD ChaCha20-Poly1305 encrypt with AAD.
 *
 * The message @p m is encrypted using a ChaCha20 cipher stream derived from the
 * encryption key @p k and the nonce @p n. The resulting ciphertext has the same
 * length @p m_len as the input message @p m and is put into @p c.
 *
 * Additionally, the ciphertext @p c as well as the additional authenticated
 * data @p a is authenticated with a tag that is generated with Poly1305 using a
 * unique subkey derived from @p k and @p n, and then put into @p tag.
 *
 * **Example**
 * @include occ_chacha20_poly1305_encrypt_aad.c
 *
 * @param[out] tag   Generated authentication tag.
 * @param[out] c     Generated ciphertext. Same length as input message.
 * @param      m     Input message.
 * @param      m_len Length of @p m and @p c.
 * @param      a     Additional authenticated data.
 * @param      a_len Length of @p a.
 * @param      n     Nonce.
 * @param      n_len Length of @p n. 0 <= @p n_len <= @c occ_chacha20_poly1305_NONCE_BYTES_MAX.
 * @param      k     Encryption key.
 *
 * @remark @p c and @p m can point to the same address.
 *
 * @remark When reusing an encryption key @p k for a different message @p m or
 *         different additional authenticated data @p a, a different nonce @p n
 *         must be used.
 */
void occ_chacha20_poly1305_encrypt_aad(uint8_t tag[occ_chacha20_poly1305_TAG_BYTES],
                                       uint8_t *c,
                                       const uint8_t *m, size_t m_len,
                                       const uint8_t *a, size_t a_len,
                                       const uint8_t *n, size_t n_len,
                                       const uint8_t k[occ_chacha20_poly1305_KEY_BYTES]);
/**@}*/

/**@{*/
/**
 * AEAD ChaCha20-Poly1305 decrypt.
 *
 * If the authentication tag @p tag is valid for the ciphertext @p c, the
 * encryption key @p k and the nonce @p n, the ciphertext is decrypted and put
 * into @p m. The decrypted message @p m has the same length @p c_len as the
 * original ciphertext.
 *
 * **Example**
 * @include occ_chacha20_poly1305_decrypt.c
 *
 * @param      tag   Received authentication tag.
 * @param[out] m     Decoded message. Same length as received ciphertext.
 * @param      c     Received ciphertext.
 * @param      c_len Length of @p c and @p m.
 * @param      n     Nonce.
 * @param      n_len Length of @p n. 0 <= @p n_len <= @c occ_chacha20_poly1305_NONCE_BYTES_MAX.
 * @param      k     Encryption key.
 *
 * @returns 0 If @p tag is valid.
 * @returns 1 Otherwise.
 *
 * @remark @p m and @p c can point to the same address.
 */
int occ_chacha20_poly1305_decrypt(const uint8_t tag[occ_chacha20_poly1305_TAG_BYTES],
                                  uint8_t *m,
                                  const uint8_t *c, size_t c_len,
                                  const uint8_t *n, size_t n_len,
                                  const uint8_t k[occ_chacha20_poly1305_KEY_BYTES]);

/**
 * AEAD ChaCha20-Poly1305 decrypt with AAD.
 *
 * If the authentication tag @p tag is valid for the ciphertext @p c, the
 * additional authenticated data @p a, the encryption key @p k and the nonce
 * @p n, the ciphertext is decrypted and put into @p m. The decrypted message
 * @p m has the same length @p c_len as the original ciphertext.
 *
 * **Example**
 * @include occ_chacha20_poly1305_decrypt_aad.c
 *
 * @param      tag   Received authentication tag.
 * @param[out] m     Decoded message. Same length as received ciphertext.
 * @param      c     Received ciphertext.
 * @param      c_len Length of @p c and @p m.
 * @param      a     Received additional authenticated data.
 * @param      a_len Length of @p a.
 * @param      n     Nonce.
 * @param      n_len Length of @p n. 0 <= @p n_len <= @c occ_chacha20_poly1305_NONCE_BYTES_MAX.
 * @param      k     Encryption key.
 *
 * @returns 0 If @p tag is valid.
 * @returns 1 Otherwise.
 *
 * @remark @p m and @p c can point to the same address.
 */
int occ_chacha20_poly1305_decrypt_aad(const uint8_t tag[occ_chacha20_poly1305_TAG_BYTES],
                                      uint8_t *m,
                                      const uint8_t *c, size_t c_len,
                                      const uint8_t *a, size_t a_len,
                                      const uint8_t *n, size_t n_len,
                                      const uint8_t k[occ_chacha20_poly1305_KEY_BYTES]);
/**@}*/

#endif
