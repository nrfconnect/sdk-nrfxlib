/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * AES (advanced encryption standard) is a symmetric encryption algorithm standardized by NIST.
 * AES transfers a 128 bit block of data into an encrypted block of same size.
 *
 * AES-EAX (encrypt-then-authenticate-then-translate) is an AES mode which effectively turn the
 * block cipher into a stream cipher. The AES block cipher primitive is used in CTR mode for
 * encryption and as OMAC for authentication over each block.
 */

#ifndef OCRYPTO_AES_EAX_H
#define OCRYPTO_AES_EAX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>


/**
 * AES-EAX encryption.
 *
 * @param[out] ct     Cyphertext.
 * @param[out] tag    Authentication tag.
 * @param      pt     Plaintext.
 * @param      pt_len Plaintext length.
 * @param      key    AES key.
 * @param      size   Key size (16, 24, or 32).
 * @param      iv     Initial vector.
 * @param      iv_len Initial vector length.
 * @param      aa     Additional authentication data.
 * @param      aa_len Additional authentication data length.
 *
 * @remark @p ct and @p pt can point to the same address.
 */
void ocrypto_aes_eax_encrypt (
    uint8_t* ct, uint8_t tag[16], const uint8_t* pt, size_t pt_len,  const uint8_t *key, size_t size,
    const uint8_t* iv, size_t iv_len, const uint8_t *aa, size_t aa_len);

/**
 * AES-EAX decryption.
 *
 * @param[out] pt     Plaintext.
 * @param      tag    Authentication tag.
 * @param      ct     Cyphertext.
 * @param      ct_len Cyphertext length.
 * @param      key    AES key.
 * @param      size   Key size (16, 24, or 32).
 * @param      iv     Initial vector.
 * @param      iv_len Initial vector length.
 * @param      aa     Additional authentication data.
 * @param      aa_len Additional authentication data length.
 *
 * @remark @p ct and @p pt can point to the same address.
 *
 * @returns 0  If @p tag is valid.
 * @returns -1 Otherwise.
 */
int ocrypto_aes_eax_decrypt (
    uint8_t* pt, const uint8_t tag[16], const uint8_t* ct, size_t ct_len, const uint8_t *key, size_t size,
    const uint8_t* iv, size_t iv_len, const uint8_t *aa, size_t aa_len);


#ifdef __cplusplus
}
#endif

#endif
