/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef CC3XX_PSA_KEY_GENERATION_H
#define CC3XX_PSA_KEY_GENERATION_H

/** \file cc3xx_psa_key_generation.h
 *
 * This file contains the declaration of the entry points associated to the
 * key generation (i.e. random generation and extraction of public keys) as
 * described by the PSA Cryptoprocessor Driver interface specification
 *
 */

#include "psa/crypto.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Generate a random key
 *
 * \param[in]  attributes        Attributes of the key to use
 * \param[out] key_buffer        Buffer to hold the generated key
 * \param[in]  key_buffer_size   Size in bytes of the key_buffer buffer
 * \param[out] key_buffer_length Size in bytes of the generated key
 *
 * \retval  PSA_SUCCESS on success. Error code from psa_status_t on
 *          failure
 */
psa_status_t cc3xx_generate_key(const psa_key_attributes_t *attributes,
                                uint8_t *key_buffer, size_t key_buffer_size,
                                size_t *key_buffer_length);
/*!
 * \brief Export the public key from a private key.
 *
 * \param[in]  attributes      Attributes of the key to use
 * \param[in]  key_buffer      Buffer to hold the generated key
 * \param[in]  key_buffer_size Size in bytes of the key_buffer buffer
 * \param[out] data            Buffer to hold the extracted public key
 * \param[in]  data_size       Size in bytes of the data buffer
 * \param[out] data_length     Size in bytes of the extracted public key
 *
 * \retval  PSA_SUCCESS on success. Error code from psa_status_t on
 *          failure
 */
psa_status_t cc3xx_export_public_key(const psa_key_attributes_t *attributes,
                                     const uint8_t *key_buffer,
                                     size_t key_buffer_size, uint8_t *data,
                                     size_t data_size, size_t *data_length);

/*!
 * \brief Import a key.
 *
 * \param[in]  attributes        Attributes of the key to use
 * \param[in]  data              Buffer to hold the imported key
 * \param[in]  data_length       Size in bytes of the key_buffer buffer
 * \param[out] key_buffer        Internal buffer to hold the key
 * \param[in]  key_buffer_size   Size in bytes of the internal buffer
 * \param[out] key_buffer_length Length in bytes of the imported key
 * \param[out] key_bits          Length in bits the imported key
 *
 * \retval  PSA_SUCCESS on success. Error code from psa_status_t on
 *          failure
 */
psa_status_t cc3xx_import_key(const psa_key_attributes_t *attributes,
                              const uint8_t *data, size_t data_length,
                              uint8_t *key_buffer, size_t key_buffer_size,
                              size_t *key_buffer_length, size_t *key_bits );



/**
 * \brief Generate an ECC Weierstrass key pair
 *
 * \param[in]  attributes        Attributes of the key to use
 * \param[out] key_buffer        Buffer to hold the generated key
 * \param[in]  key_buffer_size   Size in bytes of the key_buffer buffer
 * \param[out] key_buffer_length Size in bytes of the generated key
 *
 * \retval  PSA_SUCCESS on success. Error code from psa_status_t on failure
 */
psa_status_t
cc3xx_internal_gen_ecc_wstr_keypair(const psa_key_attributes_t *attributes,
                                    uint8_t *key_buffer, size_t key_buffer_size,
                                    size_t *key_buffer_length);

/**
 * \brief Generate an ECC Edwards key pair
 *
 * \param[in]  attributes        Attributes of the key to use
 * \param[out] key_buffer        Buffer to hold the generated key
 * \param[in]  key_buffer_size   Size in bytes of the key_buffer buffer
 *
 * \retval  PSA_SUCCESS on success. Error code from psa_status_t on failure
 */
psa_status_t
cc3xx_internal_gen_ecc_edwards_keypair(const psa_key_attributes_t *attributes,
                                       uint8_t *key_buffer, size_t key_buffer_size);

/**
 * \brief Generate an ECC Montgomery key pair
 *
 * \param[in]  attributes        Attributes of the key to use
 * \param[out] key_buffer        Buffer to hold the generated key
 * \param[in]  key_buffer_size   Size in bytes of the key_buffer buffer
 *
 * \retval  PSA_SUCCESS on success. Error code from psa_status_t on failure
 */
psa_status_t
cc3xx_internal_gen_ecc_mont_keypair(const psa_key_attributes_t *attributes,
                                    uint8_t *key_buffer, size_t key_buffer_size);


/**
 * \brief Generate an RSA key pair
 *
 * \param[in]  attributes        Attributes of the key to use
 * \param[out] key_buffer        Buffer to hold the generated key
 * \param[in]  key_buffer_size   Size in bytes of the key_buffer buffer
 * \param[out] key_buffer_length Size in bytes of the generated key
 *
 * \retval  PSA_SUCCESS on success. Error code from psa_status_t on failure
 */
psa_status_t
cc3xx_internal_gen_rsa_keypair(const psa_key_attributes_t *attributes,
                               uint8_t *key_buffer, size_t key_buffer_size,
                               size_t *key_buffer_length);


/**
 * \brief Export the public key from an ECC Weierstrass private key
 *
 * \param[in]  attributes      Attributes of the key to use
 * \param[in]  key_buffer      Buffer holding the private key
 * \param[in]  key_buffer_size Size in bytes of the key_buffer buffer
 * \param[out] data            Buffer to hold the extracted public key
 * \param[in]  data_size       Size in bytes of the data buffer
 * \param[out] data_length     Size in bytes of the extracted public key
 *
 * \retval  PSA_SUCCESS on success. Error code from psa_status_t on failure
 */
psa_status_t
cc3xx_internal_export_ecc_wrst_public_key(const psa_key_attributes_t *attributes,
                                           const uint8_t *key_buffer,
                                           size_t key_buffer_size, uint8_t *data,
                                           size_t data_size, size_t *data_length);

/**
 * \brief Export the public key from an ECC Edwards private key
 *
 * \param[in]  attributes      Attributes of the key to use
 * \param[in]  key_buffer      Buffer holding the private key
 * \param[in]  key_buffer_size Size in bytes of the key_buffer buffer
 * \param[out] data            Buffer to hold the extracted public key
 * \param[in]  data_size       Size in bytes of the data buffer
 * \param[out] data_length     Size in bytes of the extracted public key
 *
 * \retval  PSA_SUCCESS on success. Error code from psa_status_t on failure
 */
psa_status_t
cc3xx_internal_export_ecc_edwards_public_key(const psa_key_attributes_t *attributes,
                                           const uint8_t *key_buffer,
                                           size_t key_buffer_size, uint8_t *data,
                                           size_t data_size, size_t *data_length);

/**
 * \brief Export the public key from an ECC Montgomery private key
 *
 * \param[in]  attributes      Attributes of the key to use
 * \param[in]  key_buffer      Buffer holding the private key
 * \param[in]  key_buffer_size Size in bytes of the key_buffer buffer
 * \param[out] data            Buffer to hold the extracted public key
 * \param[in]  data_size       Size in bytes of the data buffer
 * \param[out] data_length     Size in bytes of the extracted public key
 *
 * \retval  PSA_SUCCESS on success. Error code from psa_status_t on failure
 */
psa_status_t cc3xx_internal_export_ecc_mont_public_key(const psa_key_attributes_t *attributes,
                                          const uint8_t *key_buffer,
                                          size_t key_buffer_size, uint8_t *data,
                                          size_t data_size, size_t *data_length);

/**
 * \brief Check validity of ECC Montgomery key
 *
 * \param[in]  key_type        Type of the key
 * \param[in]  key_buff        Buffer holding the key
 * \param[in]  key_buff_size   Size in bytes of the key buffer
 *
 * \retval  PSA_SUCCESS on success. Error code from psa_status_t on failure
 */
psa_status_t cc3xx_internal_check_mont_key(psa_key_type_t key_type,
                                           const uint8_t *key_buff,
                                           size_t key_buff_size);

/**
 * \brief Check validity of ECC Edwards key
 *
 * \param[in]  key_type        Type of the key
 * \param[in]  key_buff        Buffer holding the key
 * \param[in]  key_buff_length Size in bytes of the key buffer
 *
 * \retval  PSA_SUCCESS on success. Error code from psa_status_t on failure
 */
psa_status_t cc3xx_internal_check_edw_key(psa_key_type_t key_type,
                                          const uint8_t *key_buff,
                                          size_t key_buff_length);

/**
 * \brief Check validity of ECC Weierstrass key
 *
 * \param[in]  curve           ECC curve family
 * \param[in]  curve_bits      Number of bits in the curve
 * \param[in]  key_type        Type of the key
 * \param[in]  key_buff        Buffer holding the key
 * \param[in]  key_buff_length Size in bytes of the key buffer
 *
 * \retval  PSA_SUCCESS on success. Error code from psa_status_t on failure
 */
psa_status_t cc3xx_internal_check_wrst_key(psa_ecc_family_t curve,
                                           size_t curve_bits,
                                           psa_key_type_t key_type,
                                           const uint8_t *key_buff,
                                           size_t key_buff_length);

#ifdef __cplusplus
}
#endif
#endif /* CC3XX_PSA_KEY_GENERATION_H */
