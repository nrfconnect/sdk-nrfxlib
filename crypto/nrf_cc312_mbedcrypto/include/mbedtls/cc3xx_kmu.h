/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**@file
 * @defgroup nrf_cc3xx_mbedcrypto nrf_cc3xx_mbedcrypto APIs
 * @{
 * @brief nrf_cc3xx_mbedcrypto  nrf_cc3xx_mbedcrypto library containing cc3xx
 * APIs for the KMU or KDR peripherals. Further documentation can be found on : https://tls.mbed.org
 * @}
 *
 * @defgroup nrf_cc3xx_mbedcrypto_kmu nrf_cc3xx_mbedcrypto KMU APIs
 * @ingroup nrf_cc3xx_mbedcrypto
 * @{
 * @brief The nrf_cc3xx_mbedcrypto_kmu APIs can be utilized to directly use or derive keys
 *        from KMU or KDR in ARM CryptoCell devices
 */
#ifndef CC3XX_KMU_H__
#define CC3XX_KMU_H__

#include <stdint.h>

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "nrf_cc3xx_platform_defines.h"


#define MBEDTLS_SHADOW_KEY_KDF_MAX_LABEL_SIZE_IN_BYTES      (64)    //!< KDF input "label" can be 0 to 64 bytes.
#define MBEDTLS_SHADOW_KEY_KDF_MAX_CONTEXT_SIZE_IN_BYTES    (64)    //!< KDF input "context" can be 0 to 64 bytes.
#define MBEDTLS_SHADOW_KEY_KDF_MAX_DERIVED_SIZE_IN_BYTES    (4080)  //!< KDF max length for derived material.


#define MBEDTLS_ERR_SHADOW_KEY_KEY_OK               (0)     //!< The shadow key operation was succesful.
#define MBEDTLS_ERR_SHADOW_KEY_INVALID_SLOT         (-1)    //!< The shadow key operation used an invalid slot.
#define MBEDTLS_ERR_SHADOW_KEY_INVALID_SIZE         (-2)    //!< The shadow key was of invalid size.
#define MBEDTLS_ERR_SHADOW_KEY_KDF_INVALID_LABEL    (-3)    //!< The KDF input label is invalid
#define MBEDTLS_ERR_SHADOW_KEY_KDF_INVALID_CONTEXT  (-4)    //!< The KDF input context is invalid
#define MBEDTLS_ERR_SHADOW_KEY_KDF_INVALID_INPUT    (-5)    //!< The KDF input is invalid
#define MBEDTLS_ERR_SHADOW_KEY_INTERNAL_ERROR       (-6)    //!< KMU/KDF internal error.

#if defined(MBEDTLS_AES_C)

#include "mbedtls/aes.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** @brief Function to configure AES to use one or more KMU key slot for
 *         encryption
 *
 * @note    A shadow key is not directly accessible, only reference information
 *          is stored in the context type
 *
 * @note    Replaces the API mbedtls_aes_setkey_enc.
 *
 * @note    Using this API enforces raw key usage of keys in the KMU slots.
 *          If derived key usage is intended, please use the API
 *          nrf_cc3xx_platform_kmu_aes_setkey_enc_shadow_key_derived.
 *
 * @param   ctx         AES context to set the key by KMU slot
 * @param   slot_id     Identifier of the key slot (0 - 127)
 * @param   keybits     Key size in bits
 *
 * @returns 0 on success, otherwise a negative number.
 */
int mbedtls_aes_setkey_enc_shadow_key(
    mbedtls_aes_context * const ctx,
    uint32_t slot_id,
    unsigned int keybits);


/** @brief Function to configure AES to use one or more KMU key slot for
 *         decryption
 *
 * @note    A shadow key is not directly accessible, only reference information
 *          is stored in the context type
 *
 * @note    Replaces the API mbedtls_aes_setkey_dec.
 *
 * @note    Using this API enforces raw key usage of keys in the KMU slots.
 *          If derived key usage is intended, please use the API
 *          nrf_cc3xx_platform_kmu_aes_setkey_dec_shadow_key_derived.
 *
 * @param   ctx         AES context to set the key by KMU slot.
 * @param   slot_id     Identifier of the key slot (0 - 127).
 * @param   keybits     Key size in bits.
 *
 * @returns 0 on success, otherwise a negative number.
 */
int mbedtls_aes_setkey_dec_shadow_key(
    mbedtls_aes_context * const ctx,
    uint32_t slot_id,
    unsigned int keybits);


/** @brief Function to configure AES to use a key derived from one or more
 *         slots in KMU for encryption.
 *
 * @details See mbedtls_derive_kmu_key for details on the KDF function.
 *
 * @note    Replaces the API mbedtls_aes_setkey_dec.
 *
 * @note    The key derivation is executed before each request to encrypt.
 *          this function only configures the context to use a derived key.
 *
 * @note    When deriving the key from KMU registers, the derived keys exist
 *          in SRAM for a brief period of time, before being loaded into the
 *          write-only CryptoCell HW registers for AES keys before encryption.
 *
 * @param   ctx             AES context to set the decryption key by KMU slot.
 * @param   slot_id         Identifier of the key slot  (0 - 127).
 * @param   keybits         Key size in bits.
 * @param   label           Label to use for KDF.
 * @param   label_size      Size of the label to use for KDF.
 * @param   context         Context info to use for KDF.
 * @param   context_size    Context info size to use for KDF.
 *
 * @returns 0 on success, otherwise a negative number.
 */
int mbedtls_aes_setkey_enc_shadow_key_derived(
    mbedtls_aes_context * const ctx,
    uint32_t slot_id,
    unsigned int keybits,
    uint8_t const * label,
    size_t label_size,
    uint8_t const * context,
    size_t context_size);


/** @brief Function to configure AES to use a key derived from one or more
 *         slots in KMU for decryption.
 *
 * @details See mbedtls_derive_kmu_key for details on the KDF function.
 *
 * @note    A shadow key is not directly accessible, only reference information
 *          is stored in the context type
 * @note    Replaces the API mbedtls_aes_setkey_enc.
 *
 * @note    The key derivation is executed before each request to decrypt.
 *          This function only configures the context to use a derived key.
 *
 * @note    When deriving the key from KMU registers, the derived keys exist
 *          in SRAM for a brief period of time, before being loaded into the
 *          write-only CryptoCell HW registers for AES keys before decryption.
 *
 * @param   ctx             AES context to set the decryption key by KMU slot.
 * @param   slot_id         Identifier of the key slot  (0 - 127).
 * @param   keybits         Key size in bits.
 * @param   label           Label to use for KDF.
 * @param   label_size      Size of the label to use for KDF.
 * @param   context         Context info to use for KDF.
 * @param   context_size    Context info size to use for KDF.
 *
 * @returns 0 on success, otherwise a negative number.
 */
int mbedtls_aes_setkey_dec_shadow_key_derived(
    mbedtls_aes_context * const ctx,
    uint32_t slot_id,
    unsigned int keybits,
    uint8_t const * label,
    size_t label_size,
    uint8_t const * context,
    size_t context_size);

#ifdef __cplusplus
}
#endif

#endif /* defined(MBEDTLS_AES_C) */


#if defined(MBEDTLS_CCM_C)

#include "mbedtls/ccm.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** @brief Function to configure AES CCM to use one or more KMU key slot as
 *         encryption key.
 *
 * @note    A shadow key is not directly accessible, only reference information
 *          is stored in the context type
 *
 * @note    Replaces the API mbedtls_ccm_setkey.
 *
 * @note    Using this API enforces raw key usage of keys in the KMU slots.
 *          If derived key usage is intended, please use the API
 *          nrf_cc3xx_platform_kmu_aes_setkey_enc_shadow_key_derived.
 *
 * @param   ctx         AES context to set the key by KMU slot.
 * @param   cipher      Cipher id to use.
 * @param   slot_id     Identifier of the key slot  (0 - 127).
 * @param   keybits     Key size in bits.
 *
 * @returns 0 on success, otherwise a negative number.
 */
int mbedtls_ccm_setkey_shadow_key(
    mbedtls_ccm_context * const ctx,
    mbedtls_cipher_id_t cipher,
    uint32_t slot_id,
    unsigned int keybits
);

/** @brief Function to configure AES CCM to use a key derived from one or more
 *         slots in KMU for encryption.
 *
 * @details See mbedtls_derive_kmu_key for details on the KDF function.
 *
 * @note    A shadow key is not directly accessible, only reference information
 *          is stored in the context type
 *
 * @note    Replaces the API mbedtls_ccm_setkey.
 *
 * @note    The key derivation is executed before each request to decrypt.
 *          This function only configures the context to use a derived key.
 *
 * @note    When deriving the key from KMU registers, the derived keys exist
 *          in SRAM for a brief period of time, before being loaded into the
 *          write-only CryptoCell HW registers for AES keys before decryption.
 *
 * @param   ctx             AES context to set the decryption key by KMU slot.
 * @param   cipher          Cipher id to use.
 * @param   slot_id         Identifier of the key slot (0 - 127).
 * @param   keybits         Key size in bits.
 * @param   label           Label to use for KDF.
 * @param   label_size      Size of the label to use for KDF.
 * @param   context         Context info to use for KDF.
 * @param   context_size    Context info size to use for KDF.
 *
 * @returns 0 on success, otherwise a negative number.
 */
int mbedtls_ccm_setkey_shadow_key_derived(
    mbedtls_ccm_context * const ctx,
    mbedtls_cipher_id_t cipher,
    uint32_t slot_id,
    unsigned int keybits,
    uint8_t const * label,
    size_t label_size,
    uint8_t const * context,
    size_t context_size
);

#ifdef __cplusplus
}
#endif

#endif /* defined(MBEDTLS_CCM_C) */

#if defined(MBEDTLS_GCM_C)

#include "mbedtls/gcm.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** @brief Function to configure AES GCM to use one or more KMU key slot as
 *         encryption key.
 *
 * @note    A shadow key is not directly accessible, only reference information
 *          is stored in the context type
 *
 * @note    Replaces the API mbedtls_gcm_setkey.
 *
 * @note    Using this API enforces raw key usage of keys in the KMU slots.
 *          If derived key usage is intended, please use the API
 *          nrf_cc3xx_platform_kmu_aes_setkey_enc_shadow_key_derived.
 *
 * @param   ctx         AES context to set the key by KMU slot.
 * @param   cipher      Cipher id to use.
 * @param   slot_id     Identifier of the key slot (0 - 127).
 * @param   keybits     Key size in bits.
 *
 * @returns 0 on success, otherwise a negative number.
 */
int mbedtls_gcm_setkey_shadow_key(
    mbedtls_gcm_context * const ctx,
    mbedtls_cipher_id_t cipher,
    uint32_t slot_id,
    unsigned int keybits
);

/** @brief Function to configure AES GCM to use a key derived from one or more
 *         slots in KMU for encryption.
 *
 * @details See mbedtls_derive_kmu_key for details on the KDF function.
 *
 * @note    A shadow key is not directly accessible, only reference information
 *          is stored in the context type
 *
 * @note    Replaces the API mbedtls_gcm_setkey.
 *
 * @note    The key derivation is executed before each request to decrypt.
 *          this function only configures the context to use a derived key.
 *
 * @note    When deriving the key from KMU registers, the derived keys exist
 *          in SRAM for a brief period of time, before being loaded into the
 *          write-only CryptoCell HW registers for AES keys before decryption.
 *
 * @param   ctx             AES context to set the decryption key by KMU slot.
 * @param   cipher          Cipher id to use.
 * @param   slot_id         Identifier of the key slot (0 - 127).
 * @param   keybits         Key size in bits.
 * @param   label           Label to use for KDF.
 * @param   label_size      Size of the label to use for KDF.
 * @param   context         Context info to use for KDF.
 * @param   context_size    Context info size to use for KDF.
 *
 * @returns 0 on success, otherwise a negative number.
 */
int mbedtls_gcm_setkey_shadow_key_derived(
    mbedtls_gcm_context * const ctx,
    mbedtls_cipher_id_t cipher,
    uint32_t slot_id,
    unsigned int keybits,
    uint8_t const * label,
    size_t label_size,
    uint8_t const * context,
    size_t context_size
);

#ifdef __cplusplus
}
#endif

#endif // defined(MBEDTLS_GCM_C)

#if defined(MBEDTLS_AES_C)

#include "mbedtls/aes.h"

#ifdef __cplusplus
extern "C"
{
#endif


/** @brief Function to use CMAC to derive a key stored in KMU/Kdr
 *
 * @details The KDF is using a PRF function described in the Special publication
 *          800-108: Recommendation for Key Derivation Using Pseudorandom Functions
 *          https://csrc.nist.gov/publications/detail/sp/800-108/final.
 *
 *          This algorithm is described in chapter 5.1 - KDF in Counter Mode
 *
 *          The format of the PRF (the input) is as follows:
 *          PRF (KI, i || Label || 0x00 || Context || L)
 *
 *          KI: The Key derivation key
 *          i : The counter value for each iteration of the PRF represented
 *              as one byte.
 *          label: A string identifying the purpose of the derived key
 *                 that is up to 64 bytes long.
 *          0x00: a single byte delimiter.
 *          Context: Fixed information about the derived keying material
 *                   that is up to 64 bytes long.
 *          L : The length of derived key material in bits represented as two
 *              bytes.
 *
 * @note On nRF52840 only slot_id == 0 is valid, pointing to the
 *       Kdr key (also known as a HUK key) loaded into the CryptoCell.
 *
 * @param   slot_id         Identifier of the key slot.
 * @param   keybits         Key size in bits.
 * @param   label           Label to use for KDF.
 * @param   label_size      Size of the label to use for KDF.
 * @param   context         Context info to use for KDF.
 * @param   context_size    Context info size to use for KDF.
 * @param   output          Output buffer.
 * @param   output_size     Size of output buffer in bytes.
 *
 * @returns 0 on success, otherwise a negative number.
 */
int mbedtls_shadow_key_derive(uint32_t slot_id,
                             unsigned int keybits,
                             uint8_t const * label,
                             size_t label_size,
                             uint8_t const * context,
                             size_t context_size,
                             uint8_t * output,
                             size_t output_size);

#ifdef __cplusplus
}
#endif

#endif // defined(MBEDTLS_AES_C)

#endif /* CC3XX_KMU_H__ */

/** @} */
