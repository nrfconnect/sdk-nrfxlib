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
 @file mbedtls_cc_ecies.h

 @brief This file contains the CryptoCell Elliptic Curve Integrated Encryption Scheme (ECIES) APIs.
 */

/*!
 @defgroup cc_ecies CryptoCell ECIES APIs
 @brief Contains the CryptoCell Elliptic Curve Integrated Encryption Scheme (ECIES) APIs. See mbedtls_cc_ecies.h.
 @{

 @ingroup cc_ecc
 @}
*/

#ifndef _MBEDTLS_CC_ECIES_H
#define _MBEDTLS_CC_ECIES_H


#include "cc_ecpki_types.h"
#include "cc_pal_types_plat.h"
#include "cc_kdf.h"
#include "mbedtls_cc_hkdf.h"
#include "mbedtls/ecp.h"

#ifdef __cplusplus
extern "C"
{
#endif
/*! The maximal length of the ECIES cipher in Bytes. */
#define MBEDTLS_ECIES_MAX_CIPHER_LEN_BYTES  ((2*CC_ECPKI_MODUL_MAX_LENGTH_IN_WORDS + 1) * sizeof(int))
/*! The minimal length of the ECIES buffer in Bytes. */
#define MBEDTLS_ECIES_MIN_BUFF_LEN_BYTES    (sizeof(CCEciesTempData_t))

/*!
  @brief A macro for creating and encrypting a secret key.

  For a description of the parameters see ::mbedtls_ecies_kem_encrypt_full.
 */
#define  mbedtls_ecies_kem_encrypt(pGrp, pRecipPublKey, kdfDerivMode, kdfHashMode, \
                                   isSingleHashMode, pSecrKey, secrKeySize, \
                                   pCipherData, pCipherDataSize, pBuff, buffLen, \
                                   f_rng, p_rng) \
     mbedtls_ecies_kem_encrypt_full((pGrp), (pRecipPublKey), (kdfDerivMode), (kdfHashMode), \
                                    (isSingleHashMode), NULL, NULL, (pSecrKey), (secrKeySize), \
                                    (pCipherData), (pCipherDataSize), (pBuff), (buffLen), \
                                    f_rng, p_rng)



/*!
  @brief This function creates and encrypts (encapsulates) the secret key of required size,
  according to <em>ISO/IEC 18033-2:2006: Information technology -- Security techniques --
  Encryption algorithms -- Part 2: Asymmetric ciphers</em>, ECIES-KEM Encryption.

  To call this function in applications, the ::mbedtls_ecies_kem_encrypt macro definition must be used.
  The function itself has the additional input of the external ephemeral key pair, used only
  for testing purposes.

  @note Use KDF2 function mode for compliance with <em>X9.63-2011: Public Key Cryptography for the
  Financial Services Industry – Key Agreement and Key Transport Using Elliptic Curve Cryptograph</em>.\n

  @note <ul><li>The term "sender" indicates an entity that creates and encapsulates
  the secret key using this function. The term "recipient" indicates another
  entity which receives and decrypts the secret key.</li>
  <li>All public and private keys that are used must relate to the same EC Domain.</li>
  <li>The user must verify that the public key of the recipient is
  on the elliptic curve before it is used in this function.</li></ul>

  @return CCError_t \c 0 on success.
 */
CCError_t mbedtls_ecies_kem_encrypt_full(
        mbedtls_ecp_group *pGrp, /*!< [in] The ECP group to use. */
        mbedtls_ecp_point *pRecipUzPublKey, /*!< [in] A pointer to the public key of the recipient. */
        CCKdfDerivFuncMode_t kdfDerivMode, /*!< [in] The KDF function mode to use: KDF1 or KDF2. For more information, see CCKdfDerivFuncMode_t() in cc_kdf.h. */
        mbedtls_hkdf_hashmode_t kdfHashMode, /*!< [in] The used hash function. */
        uint32_t isSingleHashMode, /*!< [in] The specific ECIES mode, according to <em>ISO/IEC 18033-2:2006: Information technology -- Security techniques -- Encryption algorithms -- Part 2: Asymmetric ciphers</em> - section 10.2: 0: Not-single hash, or 1: Single hash. */
        mbedtls_ecp_point *pExtEphUzPublicKey, /*!< [in] A pointer to the ephemeral public key related to the private key. Must be set to NULL if \p pExtEphUzPrivateKey = NULL. */
        mbedtls_mpi *pExtEphUzPrivateKey, /*!< [in] The pointer to the external ephemeral private key. This key is used only for testing the function. In regular use, the pointer should be set to NULL and then the random key-pair should be generated internally. */
        uint8_t *pSecrKey, /*!< [in] A pointer to the buffer for the secret-key data to be generated. */
        size_t secrKeySize, /*!< [in] The size of the secret-key data in Bytes. */
        uint8_t *pCipherData, /*!< [in] A pointer to the encrypted cipher text. */
        size_t *pCipherDataSize, /*!< [in/out] In: A pointer to the size of the buffer for output of the CipherData, or Out: The size of the buffer for output of the CipherData in Bytes. */
        void *pBuff, /*!< [in] A pointer to the temporary buffer. */
        size_t buffLen, /*!< [in] The size of the buffer pointed by \p pBuff. Must not be less than #MBEDTLS_ECIES_MIN_BUFF_LEN_BYTES. */
        int (*f_rng)(void *, unsigned char *, size_t),  /*!< [in] The RNG function required for generating a key pair when \p pExtEphUzPublicKey and \p pExtEphUzPrivateKey are NULL */
        void *p_rng /*!< [in] The RNG parameter. */
        );



/*!
  @brief This function decrypts the encapsulated secret key passed by the sender, according to
  <em>ISO/IEC 18033-2:2006: Information technology -- Security techniques -- Encryption algorithms
  -- Part 2: Asymmetric ciphers</em>, sec. 10.2.4 - ECIES-KEM Decryption.

  @note The KDF2 function mode must be used for compliance with <em>X9.63-2011: Public Key Cryptography for the
  Financial Services Industry – Key Agreement and Key Transport Using Elliptic Curve Cryptograph</em>.\n

  @note <ul><li>The term "sender" indicates an entity that creates and encapsulates
  the secret key using this function. The term "recipient" indicates another
  entity which receives and decrypts the secret key.</li>
  <li>All public and private keys that are used must relate to the same EC Domain.</li>

  @return CCError_t \c 0 on success.
 */
CCError_t mbedtls_ecies_kem_decrypt(
        mbedtls_ecp_group *pGrp, /*!< [in] The ECP group to use. */
        mbedtls_mpi *pRecipUzPrivKey, /*!< [in] A pointer to the private key of the recipient. */
        CCKdfDerivFuncMode_t kdfDerivMode, /*!< [in] The KDF function mode to use: KDF1 or KDF2. For more information, see CCKdfDerivFuncMode_t() in cc_kdf.h. */
        mbedtls_hkdf_hashmode_t kdfHashMode, /*!< [in] The used hash function. */
        uint32_t isSingleHashMode, /*!< [in] The specific ECIES mode definition: 0,1, according to <em>ISO/IEC 18033-2:2006: Information technology -- Security techniques -- Encryption algorithms -- Part 2: Asymmetric ciphers</em> - section 10.2. */
        uint8_t *pCipherData, /*!< [in] A pointer to the received encrypted cipher data. */
        size_t cipherDataSize, /*!< [in] The size of the cipher data in Bytes. */
        uint8_t *pSecrKey, /*!< [in] A pointer to the buffer for the secret-key data to be generated. */
        size_t secrKeySize, /*!< [in] The size of the secret-key data in Bytes. */
        void *pBuff, /*!< [in] A pointer to the temporary buffer. */
        size_t buffLen /*!< [in] The size of the buffer pointed by \p pBuff. Must not be less than #MBEDTLS_ECIES_MIN_BUFF_LEN_BYTES. */
        );


#ifdef __cplusplus
}
#endif

#endif


