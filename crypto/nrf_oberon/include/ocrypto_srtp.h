/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup nrf_oberon_srpt SRPT - Secure Real-Time Transport Protocol APIs
 * @ingroup nrf_oberon
 * @{
 * @brief Type declarations and APIs for SRTP - Secure Real-time Transport Protocol.
 */

#ifndef OCRYPTO_SRTP_H
#define OCRYPTO_SRTP_H

#include <stddef.h>
#include <stdint.h>

#include "ocrypto_aes_key.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * SRTP Authentication Key Size.
 */
#define ocrypto_srtp_AuthKeySize  (20)

/**
 * SRTP Salt Size.
 */
#define ocrypto_srtp_SaltSize     (14)

/**
 * SRTP Maximum Key Size.
 */
#define ocrypto_srtp_MaxKeySize   (ocrypto_aes256_KEY_BYTES)

/**
 * SRTP Context.
 */
typedef struct {
    /**
     * Key size [bytes].
     */
    uint32_t keySize;

    /**
     * Tag size [bytes].
     */
    uint32_t tagSize;

    /**
     * Session encryption key (max 256 bits).
     */
    uint8_t encrKey[ocrypto_srtp_MaxKeySize];

    /**
     * Session authentication key
     * 160 bits.
     */
    uint8_t authKey[ocrypto_srtp_AuthKeySize];

    /**
     * Session salt
     * 112 bits.
     */
    uint8_t saltKey[ocrypto_srtp_SaltSize];
} ocrypto_srtp_context;

/**
 * Setup SRTP contexts.
 *
 * @param[out] srtpContext          SRTP context to be setup.
 * @param[out] srtcpContext         SRTCP context to be setup.
 * @param      key                  Master key.
 * @param      keySize              Size of the master key (16, 24, or 32 bytes)
 * @param      salt                 Master salt.
 * @param      tagSize              Size of the authentication tag.
 * @param      ssrc                 Synchronization source.
 */
void ocrypto_srtp_setupContext(
    ocrypto_srtp_context *srtpContext,
    ocrypto_srtp_context *srtcpContext,
    const uint8_t *key,
    uint32_t keySize,
    const uint8_t *salt,
    uint32_t tagSize,
    uint32_t ssrc);

/**
 * Encrypt SRTP packet.
 *
 * The final packet consists of @p numHeaderBytes encrypted in place, followed
 * by @p numDataBytes copied from @p dataBytes during encryption.
 *
 * @param         srtpContext          SRTP context.
 * @param[in,out] packet               Encrypted packet.
 * @param         dataBytes            Data bytes to be encrypted.
 * @param         numHeaderBytes       Number of header bytes.
 * @param         numDataBytes         Number of data bytes.
 * @param         index                Packet index.
 */
void ocrypto_srtp_encrypt(
    const ocrypto_srtp_context *srtpContext,
    uint8_t *packet,
    const uint8_t *dataBytes,
    size_t numHeaderBytes,
    size_t numDataBytes,
    uint32_t index);

/**
 * Decrypt SRTP packet.
 *
 * @param      srtpContext          SRTP context.
 * @param[out] data                 Decrypted data.
 * @param      packetBytes          Packet bytes.
 * @param      numPacketBytes       Number of packet bytes.
 * @param      index                Packet index.
 */
void ocrypto_srtp_decrypt(
    const ocrypto_srtp_context *srtpContext,
    uint8_t *data,
    const uint8_t *packetBytes,
    size_t numPacketBytes,
    uint32_t index);

/**
 * Generate SRTP authentication tag from bytes and index.
 *
 * @param      context              SRTP context.
 * @param[out] tag                  Authentication tag generated.
 * @param      bytes                Byte buffer.
 * @param      numBytes             Number of bytes in buffer.
 * @param      index                Index.
 */
void ocrypto_srtp_authenticate(
    const ocrypto_srtp_context *context,
    uint8_t *tag,
    const uint8_t *bytes,
    size_t numBytes,
    uint32_t index);

/**
 * Check SRTP authentication tag against bytes and index.
 *
 * @param      context              SRTP context.
 * @param      tag                  Tag.
 * @param      bytes                Byte buffer.
 * @param      numBytes             Number of bytes in buffer.
 * @param      index                Index.
 *
 * @retval 1 If the tag is valid.
 * @retval 0 Otherwise.
 */
int ocrypto_srtp_verifyAuthentication(
    const ocrypto_srtp_context *context,
    const uint8_t *tag,
    const uint8_t *bytes,
    size_t numBytes,
    uint32_t index);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef OCRYPTO_SRTP_H */

/** @} */
