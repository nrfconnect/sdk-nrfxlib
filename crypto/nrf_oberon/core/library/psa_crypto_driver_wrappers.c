/*
 *  Functions to delegate cryptographic operations to an available
 *  and appropriate accelerator.
 */
/*  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * NOTICE: This file has been modified by Oberon microsystems AG.
 */

#include "common.h"
#include "psa/crypto.h"
#include "psa_crypto_core.h"
#include "psa_crypto_driver_wrappers.h"

#if defined(MBEDTLS_PSA_CRYPTO_C)

/* Include test driver definition when running tests */
#ifdef PSA_NEED_OBERON_AEAD_DRIVER
#include "oberon_aead.h"
#endif
#ifdef PSA_NEED_OBERON_CIPHER_DRIVER
#include "oberon_cipher.h"
#endif
#ifdef PSA_NEED_OBERON_ECDSA_DRIVER
#include "oberon_ecdsa.h"
#endif
#ifdef PSA_NEED_OBERON_ECDH_DRIVER
#include "oberon_ecdh.h"
#endif
#ifdef PSA_NEED_OBERON_HASH_DRIVER
#include "oberon_hash.h"
#endif
#ifdef PSA_NEED_OBERON_KEY_PAIR_DRIVER
#include "oberon_key_pair.h"
#endif
#ifdef PSA_NEED_OBERON_MAC_DRIVER
#include "oberon_mac.h"
#endif
#ifdef PSA_NEED_OBERON_KDF_DRIVER
#include "oberon_kdf.h"
#endif
#ifdef PSA_NEED_OBERON_CTR_DRBG_DRIVER
#include "oberon_ctr_drbg.h"
#endif
#ifdef PSA_NEED_OBERON_HMAC_DRBG_DRIVER
#include "oberon_hmac_drbg.h"
#endif
#ifdef PSA_NEED_OBERON_RSA_DRIVER
#include "oberon_rsa.h"
#endif

#ifdef PSA_NEED_ENTROPY_DEMO_DRIVER
#include "demo_entropy.h"
#endif
#ifdef PSA_NEED_HARDWARE_DEMO_DRIVER
#include "demo_hardware.h"
#endif
#ifdef PSA_NEED_OPAQUE_DEMO_DRIVER
#include "demo_opaque.h"
#endif


/* Unique driver ids */
#define OBERON_DRIVER_ID         1
#define OPAQUE_DEMO_DRIVER_ID    3
#define HARDWARE_DEMO_DRIVER_ID  4

/* PAKE driver ids */
#define OBERON_JPAKE_DRIVER_ID   1
#define OBERON_SPAKE_DRIVER_ID   2
#define OBERON_SRP_DRIVER_ID     3


psa_status_t psa_driver_wrapper_init()
{
    psa_status_t status;

#ifdef PSA_NEED_OPAQUE_DEMO_DRIVER
    status = demo_opaque_init();
    if (status != PSA_SUCCESS) return status;
#endif /* PSA_NEED_OPAQUE_DEMO_DRIVER */

    (void)status;
    return PSA_SUCCESS;
}

void psa_driver_wrapper_free()
{
#ifdef PSA_NEED_OPAQUE_DEMO_DRIVER
    demo_opaque_free();
#endif /* PSA_NEED_OPAQUE_DEMO_DRIVER */
}

psa_status_t psa_driver_wrapper_sign_message(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg,
    const uint8_t *input, size_t input_length,
    uint8_t *signature, size_t signature_size, size_t *signature_length)
{
    psa_status_t status;

    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
    case PSA_KEY_LOCATION_LOCAL_STORAGE:
        /* Add cases for transparent drivers here */
#ifdef PSA_NEED_OBERON_ECDSA_DRIVER
        status = oberon_sign_message(
            attributes, key_buffer, key_buffer_size,
            alg,
            input, input_length,
            signature, signature_size, signature_length);
        if (status != PSA_ERROR_NOT_SUPPORTED) return status;
#endif /* PSA_NEED_OBERON_ECDSA_DRIVER */
        break;

        /* Add cases for opaque drivers here */
#ifdef PSA_NEED_OPAQUE_DEMO_DRIVER
    case OBERON_DEMO_DRIVER_LOCATION:
        return demo_opaque_signature_sign_message(
            attributes, key_buffer, key_buffer_size,
            alg,
            input, input_length,
            signature, signature_size, signature_length);
#endif /* PSA_NEED_OPAQUE_DEMO_DRIVER */

    default:
        /* Key is declared with a lifetime not known to us */
        (void)status;
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Use generic fallback */
    return psa_sign_message_builtin(
        attributes, key_buffer, key_buffer_size,
        alg,
        input, input_length,
        signature, signature_size, signature_length);
}

psa_status_t psa_driver_wrapper_verify_message(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg,
    const uint8_t *input, size_t input_length,
    const uint8_t *signature, size_t signature_length)
{
    psa_status_t status;

    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
    case PSA_KEY_LOCATION_LOCAL_STORAGE:
        /* Add cases for transparent drivers here */
#ifdef PSA_NEED_OBERON_ECDSA_DRIVER
        status = oberon_verify_message(
            attributes, key_buffer, key_buffer_size,
            alg,
            input, input_length,
            signature, signature_length);
        if (status != PSA_ERROR_NOT_SUPPORTED) return status;
#endif /* PSA_NEED_OBERON_ECDSA_DRIVER */
        break;

        /* Add cases for opaque drivers here */
#ifdef PSA_NEED_OPAQUE_DEMO_DRIVER
    case OBERON_DEMO_DRIVER_LOCATION:
        return demo_opaque_signature_verify_message(
            attributes, key_buffer, key_buffer_size,
            alg,
            input, input_length,
            signature, signature_length);
#endif /* PSA_NEED_OPAQUE_DEMO_DRIVER */

    default:
        /* Key is declared with a lifetime not known to us */
        (void)status;
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Use generic fallback */
    return psa_verify_message_builtin(
        attributes, key_buffer, key_buffer_size,
        alg,
        input, input_length,
        signature, signature_length);
}

psa_status_t psa_driver_wrapper_sign_hash(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg, const uint8_t *hash, size_t hash_length,
    uint8_t *signature, size_t signature_size, size_t *signature_length)
{
    psa_status_t status;

    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
    case PSA_KEY_LOCATION_LOCAL_STORAGE:
        /* Add cases for transparent drivers here */
#ifdef PSA_NEED_OBERON_ECDSA_DRIVER
        status = oberon_sign_hash(
            attributes, key_buffer, key_buffer_size,
            alg,
            hash, hash_length,
            signature, signature_size, signature_length);
        if (status != PSA_ERROR_NOT_SUPPORTED) return status;
#endif /* PSA_NEED_OBERON_ECDSA_DRIVER */
#ifdef PSA_NEED_OBERON_RSA_SIGN
        return oberon_rsa_sign_hash(
            attributes, key_buffer, key_buffer_size,
            alg,
            hash, hash_length,
            signature, signature_size, signature_length);
#endif /* PSA_NEED_OBERON_RSA_SIGN */
        return PSA_ERROR_NOT_SUPPORTED;

        /* Add cases for opaque drivers here */
#ifdef PSA_NEED_OPAQUE_DEMO_DRIVER
    case OBERON_DEMO_DRIVER_LOCATION:
        return demo_opaque_signature_sign_hash(
            attributes, key_buffer, key_buffer_size,
            alg,
            hash, hash_length,
            signature, signature_size, signature_length);
#endif /* PSA_NEED_OPAQUE_DEMO_DRIVER */

    default:
        /* Key is declared with a lifetime not known to us */
        (void)key_buffer;
        (void)key_buffer_size;
        (void)alg;
        (void)hash;
        (void)hash_length;
        (void)signature;
        (void)signature_size;
        (void)signature_length;
        (void)status;
        return PSA_ERROR_INVALID_ARGUMENT;
    }
}

psa_status_t psa_driver_wrapper_verify_hash(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg, const uint8_t *hash, size_t hash_length,
    const uint8_t *signature, size_t signature_length)
{
    psa_status_t status;

    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
    case PSA_KEY_LOCATION_LOCAL_STORAGE:
        /* Add cases for transparent drivers here */
#ifdef PSA_NEED_OBERON_ECDSA_DRIVER
        status = oberon_verify_hash(
            attributes, key_buffer, key_buffer_size,
            alg,
            hash, hash_length,
            signature, signature_length);
        if (status != PSA_ERROR_NOT_SUPPORTED) return status;
#endif /* PSA_NEED_OBERON_ECDSA_DRIVER */
#ifdef PSA_NEED_OBERON_RSA_SIGN
        return oberon_rsa_verify_hash(
            attributes, key_buffer, key_buffer_size,
            alg,
            hash, hash_length,
            signature, signature_length);
#endif /* PSA_NEED_OBERON_RSA_SIGN */
        return PSA_ERROR_NOT_SUPPORTED;

        /* Add cases for opaque driver here */
#ifdef PSA_NEED_OPAQUE_DEMO_DRIVER
    case OBERON_DEMO_DRIVER_LOCATION:
        return demo_opaque_signature_verify_hash(
            attributes, key_buffer, key_buffer_size,
            alg,
            hash, hash_length,
            signature, signature_length);
#endif /* PSA_NEED_OPAQUE_DEMO_DRIVER */

    default:
        /* Key is declared with a lifetime not known to us */
        (void)key_buffer;
        (void)key_buffer_size;
        (void)alg;
        (void)hash;
        (void)hash_length;
        (void)signature;
        (void)signature_length;
        (void)status;
        return PSA_ERROR_INVALID_ARGUMENT;
    }
}

/** Calculate the key buffer size required to store the key material of a key
 *  associated with an opaque driver from input key data.
 *
 * \param[in] attributes        The key attributes
 * \param[in] data              The input key data.
 * \param[in] data_length       The input data length.
 * \param[out] key_buffer_size  Minimum buffer size to contain the key material.
 *
 * \retval #PSA_SUCCESS
 * \retval #PSA_ERROR_INVALID_ARGUMENT
 * \retval #PSA_ERROR_NOT_SUPPORTED
 */
psa_status_t psa_driver_wrapper_get_key_buffer_size_from_key_data(
    const psa_key_attributes_t *attributes,
    const uint8_t *data, size_t data_length, size_t *key_buffer_size)
{
    psa_key_type_t key_type = attributes->core.type;

    *key_buffer_size = 0;
    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
#ifdef PSA_NEED_OPAQUE_DEMO_DRIVER
    case OBERON_DEMO_DRIVER_LOCATION:
        *key_buffer_size = demo_opaque_size_function(
            key_type, PSA_BYTES_TO_BITS(data_length));
        return *key_buffer_size != 0 ? PSA_SUCCESS : PSA_ERROR_NOT_SUPPORTED;
#endif /* PSA_NEED_OPAQUE_DEMO_DRIVER */

    default:
        (void)key_type;
        (void)data;
        (void)data_length;
        return PSA_ERROR_INVALID_ARGUMENT;
    }
}

/** Get the key buffer size required to store the key material of a key
 *  associated with an opaque driver.
 *
 * \param[in] attributes  The key attributes.
 * \param[out] key_buffer_size  Minimum buffer size to contain the key material
 *
 * \retval #PSA_SUCCESS
 *         The minimum size for a buffer to contain the key material has been
 *         returned successfully.
 * \retval #PSA_ERROR_NOT_SUPPORTED
 *         The type and/or the size in bits of the key or the combination of
 *         the two is not supported.
 * \retval #PSA_ERROR_INVALID_ARGUMENT
 *         The key is declared with a lifetime not known to us.
 */
psa_status_t psa_driver_wrapper_get_key_buffer_size(
    const psa_key_attributes_t *attributes,
    size_t *key_buffer_size)
{
    psa_key_type_t key_type = attributes->core.type;
    size_t key_bits = attributes->core.bits;

    *key_buffer_size = 0;
    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
#ifdef PSA_NEED_OPAQUE_DEMO_DRIVER
    case OBERON_DEMO_DRIVER_LOCATION:
        *key_buffer_size = demo_opaque_size_function(
            key_type, key_bits);
        return *key_buffer_size != 0 ? PSA_SUCCESS : PSA_ERROR_NOT_SUPPORTED;
#endif /* PSA_NEED_OPAQUE_DEMO_DRIVER */

    default:
        (void)key_type;
        (void)key_bits;
        return PSA_ERROR_INVALID_ARGUMENT;
    }
}

psa_status_t psa_driver_wrapper_generate_key(
    const psa_key_attributes_t *attributes,
    uint8_t *key_buffer, size_t key_buffer_size, size_t *key_buffer_length)
{
    psa_status_t status;

    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
    case PSA_KEY_LOCATION_LOCAL_STORAGE:
        /* Add cases for transparent drivers here */
#ifdef PSA_NEED_OBERON_KEY_PAIR_DRIVER
        status = oberon_generate_key(
            attributes, key_buffer, key_buffer_size,
            key_buffer_length);
        if (status != PSA_ERROR_NOT_SUPPORTED) return status;
#endif /* PSA_NEED_OBERON_KEY_PAIR_DRIVER */
        break;

        /* Add cases for opaque drivers here */
#ifdef PSA_NEED_OPAQUE_DEMO_DRIVER
    case OBERON_DEMO_DRIVER_LOCATION:
        return demo_opaque_generate_key(
            attributes, key_buffer, key_buffer_size, key_buffer_length);
#endif /* PSA_NEED_OPAQUE_DEMO_DRIVER */

    default:
        /* Key is declared with a lifetime not known to us */
        (void)status;
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Use generic fallback for basic key types */
    return psa_generate_key_internal(
        attributes, key_buffer, key_buffer_size, key_buffer_length);
}

psa_status_t psa_driver_wrapper_import_key(
    const psa_key_attributes_t *attributes,
    const uint8_t *data, size_t data_length,
    uint8_t *key_buffer, size_t key_buffer_size, size_t *key_buffer_length,
    size_t *bits)
{
    psa_status_t status;

    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
    case PSA_KEY_LOCATION_LOCAL_STORAGE:
        /* Add cases for transparent drivers here */
#ifdef PSA_NEED_OBERON_KEY_PAIR_DRIVER
        status = oberon_import_key(
            attributes, data, data_length,
            key_buffer, key_buffer_size,
            key_buffer_length, bits);
        if (status != PSA_ERROR_NOT_SUPPORTED) return status;
#endif /* PSA_NEED_OBERON_KEY_PAIR_DRIVER */
#ifdef PSA_NEED_OBERON_RSA_DRIVER
        status = oberon_import_rsa_key(
            attributes, data, data_length,
            key_buffer, key_buffer_size,
            key_buffer_length, bits);
        if (status != PSA_ERROR_NOT_SUPPORTED) return status;
#endif /* PSA_NEED_OBERON_RSA_DRIVER */
        break;

        /* Add cases for opaque drivers here */
#ifdef PSA_NEED_OPAQUE_DEMO_DRIVER
    case OBERON_DEMO_DRIVER_LOCATION:
        return demo_opaque_import_key(
            attributes, data, data_length,
            key_buffer, key_buffer_size,
            key_buffer_length, bits);
#endif /* PSA_NEED_OPAQUE_DEMO_DRIVER */

    default:
        (void)status;
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Use generic fallback for simple cases */
    return psa_import_key_into_slot(
        attributes, data, data_length,
        key_buffer, key_buffer_size,
        key_buffer_length, bits);
}

psa_status_t psa_driver_wrapper_export_key(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    uint8_t *data, size_t data_size, size_t *data_length)

{
    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
    case PSA_KEY_LOCATION_LOCAL_STORAGE:
        return psa_export_key_internal(
            attributes, key_buffer, key_buffer_size,
            data, data_size, data_length);

        /* Add cases for opaque drivers here */
#ifdef PSA_NEED_OPAQUE_DEMO_DRIVER
    case OBERON_DEMO_DRIVER_LOCATION:
        return demo_opaque_export_key(
            attributes, key_buffer, key_buffer_size,
            data, data_size, data_length);
#endif /* PSA_NEED_OPAQUE_DEMO_DRIVER */

    default:
        /* Key is declared with a lifetime not known to us */
        (void)key_buffer;
        (void)key_buffer_size;
        (void)data;
        (void)data_size;
        (void)data_length;
        return PSA_ERROR_INVALID_ARGUMENT;
    }
}

psa_status_t psa_driver_wrapper_export_public_key(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    uint8_t *data, size_t data_size, size_t *data_length)

{
    psa_status_t status;

    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
    case PSA_KEY_LOCATION_LOCAL_STORAGE:
        /* Add cases for transparent drivers here */
#ifdef PSA_NEED_OBERON_KEY_PAIR_DRIVER
        status = oberon_export_public_key(
            attributes, key_buffer, key_buffer_size,
            data, data_size, data_length);
        if (status != PSA_ERROR_NOT_SUPPORTED) return status;
#endif /* PSA_NEED_OBERON_KEY_PAIR_DRIVER */
#ifdef PSA_NEED_OBERON_RSA_DRIVER
        status = oberon_export_rsa_public_key(
            attributes, key_buffer, key_buffer_size,
            data, data_size, data_length);
        if (status != PSA_ERROR_NOT_SUPPORTED) return status;
#endif /* PSA_NEED_OBERON_RSA_DRIVER */
        break;

        /* Add cases for opaque drivers here */
#ifdef PSA_NEED_OPAQUE_DEMO_DRIVER
    case OBERON_DEMO_DRIVER_LOCATION:
        return demo_opaque_export_public_key(
            attributes, key_buffer, key_buffer_size,
            data, data_size, data_length);
#endif /* PSA_NEED_OPAQUE_DEMO_DRIVER */

    default:
        /* Key is declared with a lifetime not known to us */
        (void)status;
        (void)key_buffer;
        (void)key_buffer_size;
        (void)data;
        (void)data_size;
        (void)data_length;
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Use generic fallback for simple cases */
    return psa_export_public_key_internal(
        attributes, key_buffer, key_buffer_size,
        data, data_size, data_length);
}

#ifdef MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS
psa_status_t psa_driver_wrapper_get_builtin_key(
    psa_drv_slot_number_t slot_number,
    psa_key_attributes_t *attributes,
    uint8_t *key_buffer, size_t key_buffer_size, size_t *key_buffer_length)
{
    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
    default:
        (void)slot_number;
        (void)key_buffer;
        (void)key_buffer_size;
        (void)key_buffer_length;
        return PSA_ERROR_DOES_NOT_EXIST;
    }
}
#endif /* MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS */

psa_status_t psa_driver_wrapper_copy_key(
    psa_key_attributes_t *attributes,
    const uint8_t *source_key, size_t source_key_length,
    uint8_t *target_key_buffer, size_t target_key_buffer_size,
    size_t *target_key_buffer_length)
{
    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
#ifdef PSA_NEED_OPAQUE_DEMO_DRIVER
    case OBERON_DEMO_DRIVER_LOCATION:
        return demo_opaque_copy_key(
            attributes, source_key, source_key_length,
            target_key_buffer, target_key_buffer_size, target_key_buffer_length);
#endif /* PSA_NEED_OPAQUE_DEMO_DRIVER */

    default:
        (void)source_key;
        (void)source_key_length;
        (void)target_key_buffer;
        (void)target_key_buffer_size;
        (void)target_key_buffer_length;
        return PSA_ERROR_INVALID_ARGUMENT;
    }
}

/*
 * Cipher functions
 */
psa_status_t psa_driver_wrapper_cipher_encrypt(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg,
    const uint8_t *iv, size_t iv_length,
    const uint8_t *input, size_t input_length,
    uint8_t *output, size_t output_size, size_t *output_length)
{
    psa_status_t status;

    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
    case PSA_KEY_LOCATION_LOCAL_STORAGE:
        /* Add cases for transparent drivers here */
#ifdef PSA_NEED_HARDWARE_DEMO_DRIVER
        status = demo_hardware_cipher_encrypt(
            attributes, key_buffer, key_buffer_size,
            alg,
            iv, iv_length,
            input, input_length,
            output, output_size, output_length);
        if (status != PSA_ERROR_NOT_SUPPORTED) return status;
#endif /* PSA_NEED_HARDWARE_DEMO_DRIVER */
#ifdef PSA_NEED_OBERON_CIPHER_DRIVER
        return oberon_cipher_encrypt(
            attributes, key_buffer, key_buffer_size,
            alg,
            iv, iv_length,
            input, input_length,
            output, output_size, output_length);
#endif /* PSA_NEED_OBERON_CIPHER_DRIVER */
        return PSA_ERROR_NOT_SUPPORTED;

        /* Add cases for opaque drivers here */
#ifdef PSA_NEED_OPAQUE_DEMO_DRIVER
    case OBERON_DEMO_DRIVER_LOCATION:
        return demo_opaque_cipher_encrypt(
            attributes, key_buffer, key_buffer_size,
            alg,
            iv, iv_length,
            input, input_length,
            output, output_size, output_length);
#endif /* PSA_NEED_OPAQUE_DEMO_DRIVER */

    default:
        /* Key is declared with a lifetime not known to us */
        (void)key_buffer;
        (void)key_buffer_size;
        (void)alg;
        (void)iv;
        (void)iv_length;
        (void)input;
        (void)input_length;
        (void)output;
        (void)output_size;
        (void)output_length;
        (void)status;
        return PSA_ERROR_INVALID_ARGUMENT;
    }
}

psa_status_t psa_driver_wrapper_cipher_decrypt(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg,
    const uint8_t *input, size_t input_length,
    uint8_t *output, size_t output_size, size_t *output_length)
{
    psa_status_t status;

    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
    case PSA_KEY_LOCATION_LOCAL_STORAGE:
        /* Add cases for transparent drivers here */
#ifdef PSA_NEED_HARDWARE_DEMO_DRIVER
        status = demo_hardware_cipher_decrypt(
            attributes, key_buffer, key_buffer_size,
            alg,
            input, input_length,
            output, output_size, output_length);
        if (status != PSA_ERROR_NOT_SUPPORTED) return status;
#endif /* PSA_NEED_HARDWARE_DEMO_DRIVER */
#ifdef PSA_NEED_OBERON_CIPHER_DRIVER
        return oberon_cipher_decrypt(
            attributes, key_buffer, key_buffer_size,
            alg,
            input, input_length,
            output, output_size, output_length);
#endif /* PSA_NEED_OBERON_CIPHER_DRIVER */
        return PSA_ERROR_NOT_SUPPORTED;

        /* Add cases for opaque drivers here */
#ifdef PSA_NEED_OPAQUE_DEMO_DRIVER
    case OBERON_DEMO_DRIVER_LOCATION:
        return demo_opaque_cipher_decrypt(
            attributes, key_buffer, key_buffer_size,
            alg,
            input, input_length,
            output, output_size, output_length);
#endif /* PSA_NEED_OPAQUE_DEMO_DRIVER */

    default:
        /* Key is declared with a lifetime not known to us */
        (void)key_buffer;
        (void)key_buffer_size;
        (void)alg;
        (void)input;
        (void)input_length;
        (void)output;
        (void)output_size;
        (void)output_length;
        (void)status;
        return PSA_ERROR_INVALID_ARGUMENT;
    }
}

psa_status_t psa_driver_wrapper_cipher_encrypt_setup(
    psa_cipher_operation_t *operation,
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg)
{
    psa_status_t status;

    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
    case PSA_KEY_LOCATION_LOCAL_STORAGE:
        /* Add cases for transparent drivers here */
#ifdef PSA_NEED_HARDWARE_DEMO_DRIVER
        status = demo_hardware_cipher_encrypt_setup(
            &operation->ctx.oberon_hw_cipher_ctx,
            attributes, key_buffer, key_buffer_size,
            alg);
        if (status == PSA_SUCCESS) operation->id = HARDWARE_DEMO_DRIVER_ID;
        if (status != PSA_ERROR_NOT_SUPPORTED) return status;
#endif /* PSA_NEED_HARDWARE_DEMO_DRIVER */
#ifdef PSA_NEED_OBERON_CIPHER_DRIVER
        status = oberon_cipher_encrypt_setup(
            &operation->ctx.oberon_cipher_ctx,
            attributes, key_buffer, key_buffer_size,
            alg);
        if (status == PSA_SUCCESS) operation->id = OBERON_DRIVER_ID;
        return status;
#endif /* PSA_NEED_OBERON_CIPHER_DRIVER */
        return PSA_ERROR_NOT_SUPPORTED;

        /* Add cases for opaque drivers here */
#ifdef PSA_NEED_OPAQUE_DEMO_DRIVER
    case OBERON_DEMO_DRIVER_LOCATION:
        status = demo_opaque_cipher_encrypt_setup(
            &operation->ctx.opaque_cipher_ctx,
            attributes, key_buffer, key_buffer_size,
            alg);
        if (status == PSA_SUCCESS) operation->id = OPAQUE_DEMO_DRIVER_ID;
        return status;
#endif /* PSA_NEED_OPAQUE_DEMO_DRIVER */

    default:
        /* Key is declared with a lifetime not known to us */
        (void)status;
        (void)operation;
        (void)key_buffer;
        (void)key_buffer_size;
        (void)alg;
        return PSA_ERROR_INVALID_ARGUMENT;
    }
}

psa_status_t psa_driver_wrapper_cipher_decrypt_setup(
    psa_cipher_operation_t *operation,
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg)
{
    psa_status_t status;

    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
    case PSA_KEY_LOCATION_LOCAL_STORAGE:
        /* Add cases for transparent drivers here */
#ifdef PSA_NEED_HARDWARE_DEMO_DRIVER
        status = demo_hardware_cipher_decrypt_setup(
            &operation->ctx.oberon_hw_cipher_ctx,
            attributes, key_buffer, key_buffer_size,
            alg);
        if (status == PSA_SUCCESS) operation->id = HARDWARE_DEMO_DRIVER_ID;
        if (status != PSA_ERROR_NOT_SUPPORTED) return status;
#endif /* PSA_NEED_HARDWARE_DEMO_DRIVER */
#ifdef PSA_NEED_OBERON_CIPHER_DRIVER
        status = oberon_cipher_decrypt_setup(
            &operation->ctx.oberon_cipher_ctx,
            attributes, key_buffer, key_buffer_size,
            alg);
        if (status == PSA_SUCCESS) operation->id = OBERON_DRIVER_ID;
        return status;
#endif /* PSA_NEED_OBERON_CIPHER_DRIVER */
        return PSA_ERROR_NOT_SUPPORTED;

        /* Add cases for opaque drivers here */
#ifdef PSA_NEED_OPAQUE_DEMO_DRIVER
    case OBERON_DEMO_DRIVER_LOCATION:
        status = demo_opaque_cipher_decrypt_setup(
            &operation->ctx.opaque_cipher_ctx,
            attributes, key_buffer, key_buffer_size,
            alg);
        if (status == PSA_SUCCESS) operation->id = OPAQUE_DEMO_DRIVER_ID;
        return status;
#endif /* PSA_NEED_OPAQUE_DEMO_DRIVER */

    default:
        /* Key is declared with a lifetime not known to us */
        (void)status;
        (void)operation;
        (void)key_buffer;
        (void)key_buffer_size;
        (void)alg;
        return PSA_ERROR_INVALID_ARGUMENT;
    }
}

psa_status_t psa_driver_wrapper_cipher_set_iv(
    psa_cipher_operation_t *operation,
    const uint8_t *iv, size_t iv_length)
{
    switch (operation->id) {


#ifdef PSA_NEED_HARDWARE_DEMO_DRIVER
    case HARDWARE_DEMO_DRIVER_ID:
        return demo_hardware_cipher_set_iv(
            &operation->ctx.oberon_hw_cipher_ctx,
            iv, iv_length);
#endif /* PSA_NEED_HARDWARE_DEMO_DRIVER */

#ifdef PSA_NEED_OBERON_CIPHER_DRIVER
    case OBERON_DRIVER_ID:
        return oberon_cipher_set_iv(
            &operation->ctx.oberon_cipher_ctx,
            iv, iv_length);
#endif /* PSA_NEED_OBERON_CIPHER_DRIVER */

#ifdef PSA_NEED_OPAQUE_DEMO_DRIVER
    case OPAQUE_DEMO_DRIVER_ID:
        return demo_opaque_cipher_set_iv(
            &operation->ctx.opaque_cipher_ctx,
            iv, iv_length);
#endif /* PSA_NEED_OPAQUE_DEMO_DRIVER */

    default:
        (void)iv;
        (void)iv_length;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_cipher_update(
    psa_cipher_operation_t *operation,
    const uint8_t *input, size_t input_length,
    uint8_t *output, size_t output_size, size_t *output_length)
{
    switch (operation->id) {


#ifdef PSA_NEED_HARDWARE_DEMO_DRIVER
    case HARDWARE_DEMO_DRIVER_ID:
        return demo_hardware_cipher_update(
            &operation->ctx.oberon_hw_cipher_ctx,
            input, input_length,
            output, output_size, output_length);
#endif /* PSA_NEED_HARDWARE_DEMO_DRIVER */

#ifdef PSA_NEED_OBERON_CIPHER_DRIVER
    case OBERON_DRIVER_ID:
        return oberon_cipher_update(
            &operation->ctx.oberon_cipher_ctx,
            input, input_length,
            output, output_size, output_length);
#endif /* PSA_NEED_OBERON_CIPHER_DRIVER */

#ifdef PSA_NEED_OPAQUE_DEMO_DRIVER
    case OPAQUE_DEMO_DRIVER_ID:
        return demo_opaque_cipher_update(
            &operation->ctx.opaque_cipher_ctx,
            input, input_length,
            output, output_size, output_length);
#endif /* PSA_NEED_OPAQUE_DEMO_DRIVER */

    default:
        (void)input;
        (void)input_length;
        (void)output;
        (void)output_size;
        (void)output_length;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_cipher_finish(
    psa_cipher_operation_t *operation,
    uint8_t *output,
    size_t output_size,
    size_t *output_length)
{
    switch (operation->id) {


#ifdef PSA_NEED_HARDWARE_DEMO_DRIVER
    case HARDWARE_DEMO_DRIVER_ID:
        return demo_hardware_cipher_finish(
            &operation->ctx.oberon_hw_cipher_ctx,
            output, output_size, output_length);
#endif /* PSA_NEED_HARDWARE_DEMO_DRIVER */

#ifdef PSA_NEED_OBERON_CIPHER_DRIVER
    case OBERON_DRIVER_ID:
        return oberon_cipher_finish(
            &operation->ctx.oberon_cipher_ctx,
            output, output_size, output_length);
#endif /* PSA_NEED_OBERON_CIPHER_DRIVER */

#ifdef PSA_NEED_OPAQUE_DEMO_DRIVER
    case OPAQUE_DEMO_DRIVER_ID:
        return demo_opaque_cipher_finish(
            &operation->ctx.opaque_cipher_ctx,
            output, output_size, output_length);
#endif /* PSA_NEED_OPAQUE_DEMO_DRIVER */

    default:
        (void)output;
        (void)output_size;
        (void)output_length;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_cipher_abort(
    psa_cipher_operation_t *operation)
{
    switch (operation->id) {


#ifdef PSA_NEED_HARDWARE_DEMO_DRIVER
    case HARDWARE_DEMO_DRIVER_ID:
        return demo_hardware_cipher_abort(&operation->ctx.oberon_hw_cipher_ctx);
#endif /* PSA_NEED_HARDWARE_DEMO_DRIVER */

#ifdef PSA_NEED_OBERON_CIPHER_DRIVER
    case OBERON_DRIVER_ID:
        return oberon_cipher_abort(&operation->ctx.oberon_cipher_ctx);
#endif /* PSA_NEED_OBERON_CIPHER_DRIVER */

#ifdef PSA_NEED_OPAQUE_DEMO_DRIVER
    case OPAQUE_DEMO_DRIVER_ID:
        return demo_opaque_cipher_abort(&operation->ctx.opaque_cipher_ctx);
#endif /* PSA_NEED_OPAQUE_DEMO_DRIVER */

    default:
        return PSA_SUCCESS;
    }
}

/*
 * Hashing functions
 */
psa_status_t psa_driver_wrapper_hash_compute(
    psa_algorithm_t alg,
    const uint8_t *input, size_t input_length,
    uint8_t *hash, size_t hash_size, size_t *hash_length)
{
    psa_status_t status;

#ifdef PSA_NEED_HARDWARE_DEMO_DRIVER
    status = demo_hardware_hash_compute(
        alg, input, input_length, hash, hash_size, hash_length);
    if (status != PSA_ERROR_NOT_SUPPORTED) return status;
#endif /* PSA_NEED_HARDWARE_DEMO_DRIVER */
#ifdef PSA_NEED_OBERON_HASH_DRIVER
    return oberon_hash_compute(
        alg, input, input_length, hash, hash_size, hash_length);
#endif /* PSA_NEED_OBERON_HASH_DRIVER */

    (void)status;
    (void)alg;
    (void)input;
    (void)input_length;
    (void)hash;
    (void)hash_size;
    (void)hash_length;
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_driver_wrapper_hash_setup(
    psa_hash_operation_t *operation,
    psa_algorithm_t alg)
{
    psa_status_t status;

#ifdef PSA_NEED_HARDWARE_DEMO_DRIVER
    status = demo_hardware_hash_setup(
        &operation->ctx.oberon_hw_hash_ctx, alg);
    if (status == PSA_SUCCESS) operation->id = HARDWARE_DEMO_DRIVER_ID;
    if (status != PSA_ERROR_NOT_SUPPORTED) return status;
#endif /* PSA_NEED_HARDWARE_DEMO_DRIVER */
#ifdef PSA_NEED_OBERON_HASH_DRIVER
    status = oberon_hash_setup(
        &operation->ctx.oberon_hash_ctx, alg);
    if (status == PSA_SUCCESS) operation->id = OBERON_DRIVER_ID;
    return status;
#endif /* PSA_NEED_OBERON_HASH_DRIVER */

    (void)status;
    (void)operation;
    (void)alg;
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_driver_wrapper_hash_clone(
    const psa_hash_operation_t *source_operation,
    psa_hash_operation_t *target_operation)
{
    switch (source_operation->id) {

#ifdef PSA_NEED_HARDWARE_DEMO_DRIVER
    case HARDWARE_DEMO_DRIVER_ID:
        target_operation->id = HARDWARE_DEMO_DRIVER_ID;
        return demo_hardware_hash_clone(
            &source_operation->ctx.oberon_hw_hash_ctx,
            &target_operation->ctx.oberon_hw_hash_ctx);
#endif /* PSA_NEED_HARDWARE_DEMO_DRIVER */

#ifdef PSA_NEED_OBERON_HASH_DRIVER
    case OBERON_DRIVER_ID:
        target_operation->id = OBERON_DRIVER_ID;
        return oberon_hash_clone(
            &source_operation->ctx.oberon_hash_ctx,
            &target_operation->ctx.oberon_hash_ctx);
#endif /* PSA_NEED_OBERON_HASH_DRIVER */

    default:
        (void)target_operation;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_hash_update(
    psa_hash_operation_t *operation,
    const uint8_t *input, size_t input_length)
{
    switch (operation->id) {

#ifdef PSA_NEED_HARDWARE_DEMO_DRIVER
    case HARDWARE_DEMO_DRIVER_ID:
        return demo_hardware_hash_update(
            &operation->ctx.oberon_hw_hash_ctx,
            input, input_length);
#endif /* PSA_NEED_HARDWARE_DEMO_DRIVER */

#ifdef PSA_NEED_OBERON_HASH_DRIVER
    case OBERON_DRIVER_ID:
        return oberon_hash_update(
            &operation->ctx.oberon_hash_ctx,
            input, input_length);
#endif /* PSA_NEED_OBERON_HASH_DRIVER */

    default:
        (void)input;
        (void)input_length;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_hash_finish(
    psa_hash_operation_t *operation,
    uint8_t *hash,
    size_t hash_size,
    size_t *hash_length)
{
    switch (operation->id) {

#ifdef PSA_NEED_HARDWARE_DEMO_DRIVER
    case HARDWARE_DEMO_DRIVER_ID:
        return demo_hardware_hash_finish(
            &operation->ctx.oberon_hw_hash_ctx,
            hash, hash_size, hash_length);
#endif /* PSA_NEED_HARDWARE_DEMO_DRIVER */

#ifdef PSA_NEED_OBERON_HASH_DRIVER
    case OBERON_DRIVER_ID:
        return oberon_hash_finish(
            &operation->ctx.oberon_hash_ctx,
            hash, hash_size, hash_length);
#endif /* PSA_NEED_OBERON_HASH_DRIVER */

    default:
        (void)hash;
        (void)hash_size;
        (void)hash_length;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_hash_abort(
    psa_hash_operation_t *operation)
{
    switch (operation->id) {

#ifdef PSA_NEED_HARDWARE_DEMO_DRIVER
    case HARDWARE_DEMO_DRIVER_ID:
        return demo_hardware_hash_abort(&operation->ctx.oberon_hw_hash_ctx);
#endif /* PSA_NEED_HARDWARE_DEMO_DRIVER */

#ifdef PSA_NEED_OBERON_HASH_DRIVER
    case OBERON_DRIVER_ID:
        return oberon_hash_abort(&operation->ctx.oberon_hash_ctx);
#endif /* PSA_NEED_OBERON_HASH_DRIVER */

    default:
        return PSA_SUCCESS;
    }
}

psa_status_t psa_driver_wrapper_aead_encrypt(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg,
    const uint8_t *nonce, size_t nonce_length,
    const uint8_t *additional_data, size_t additional_data_length,
    const uint8_t *plaintext, size_t plaintext_length,
    uint8_t *ciphertext, size_t ciphertext_size, size_t *ciphertext_length)
{
    psa_status_t status;

    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
    case PSA_KEY_LOCATION_LOCAL_STORAGE:
        /* Add cases for transparent drivers here */
#ifdef PSA_NEED_OBERON_AEAD_DRIVER
        return oberon_aead_encrypt(
            attributes, key_buffer, key_buffer_size,
            alg,
            nonce, nonce_length,
            additional_data, additional_data_length,
            plaintext, plaintext_length,
            ciphertext, ciphertext_size, ciphertext_length);
#endif /* PSA_NEED_OBERON_AEAD_DRIVER */
        return PSA_ERROR_NOT_SUPPORTED;

        /* Add cases for opaque drivers here */

    default:
        /* Key is declared with a lifetime not known to us */
        (void)key_buffer;
        (void)key_buffer_size;
        (void)alg;
        (void)nonce;
        (void)nonce_length;
        (void)additional_data;
        (void)additional_data_length;
        (void)plaintext;
        (void)plaintext_length;
        (void)ciphertext;
        (void)ciphertext_size;
        (void)ciphertext_length;
        (void)status;
        return PSA_ERROR_INVALID_ARGUMENT;
    }
}

psa_status_t psa_driver_wrapper_aead_decrypt(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg,
    const uint8_t *nonce, size_t nonce_length,
    const uint8_t *additional_data, size_t additional_data_length,
    const uint8_t *ciphertext, size_t ciphertext_length,
    uint8_t *plaintext, size_t plaintext_size, size_t *plaintext_length)
{
    psa_status_t status;

    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
    case PSA_KEY_LOCATION_LOCAL_STORAGE:
        /* Add cases for transparent drivers here */
#ifdef PSA_NEED_OBERON_AEAD_DRIVER
        return oberon_aead_decrypt(
            attributes, key_buffer, key_buffer_size,
            alg,
            nonce, nonce_length,
            additional_data, additional_data_length,
            ciphertext, ciphertext_length,
            plaintext, plaintext_size, plaintext_length);
#endif /* PSA_NEED_OBERON_AEAD_DRIVER */
        return PSA_ERROR_NOT_SUPPORTED;

        /* Add cases for opaque drivers here */

    default:
        /* Key is declared with a lifetime not known to us */
        (void)key_buffer;
        (void)key_buffer_size;
        (void)alg;
        (void)nonce;
        (void)nonce_length;
        (void)additional_data;
        (void)additional_data_length;
        (void)ciphertext;
        (void)ciphertext_length;
        (void)plaintext;
        (void)plaintext_size;
        (void)plaintext_length;
        (void)status;
        return PSA_ERROR_INVALID_ARGUMENT;
    }
}

psa_status_t psa_driver_wrapper_aead_encrypt_setup(
    psa_aead_operation_t *operation,
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg)
{
    psa_status_t status;

    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
    case PSA_KEY_LOCATION_LOCAL_STORAGE:
        /* Add cases for transparent drivers here */
#ifdef PSA_NEED_OBERON_AEAD_DRIVER
        status = oberon_aead_encrypt_setup(
            &operation->ctx.oberon_aead_ctx,
            attributes, key_buffer, key_buffer_size,
            alg);
        if (status == PSA_SUCCESS) operation->id = OBERON_DRIVER_ID;
        return status;
#endif /* PSA_NEED_OBERON_AEAD_DRIVER */
        return PSA_ERROR_NOT_SUPPORTED;

        /* Add cases for opaque drivers here */

    default:
        /* Key is declared with a lifetime not known to us */
        (void)status;
        (void)operation;
        (void)key_buffer;
        (void)key_buffer_size;
        (void)alg;
        return PSA_ERROR_NOT_SUPPORTED;
    }
}

psa_status_t psa_driver_wrapper_aead_decrypt_setup(
    psa_aead_operation_t *operation,
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg)
{
    psa_status_t status;

    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
    case PSA_KEY_LOCATION_LOCAL_STORAGE:
        /* Add cases for transparent drivers here */
#ifdef PSA_NEED_OBERON_AEAD_DRIVER
        status = oberon_aead_decrypt_setup(
            &operation->ctx.oberon_aead_ctx,
            attributes, key_buffer, key_buffer_size,
            alg);
        if (status == PSA_SUCCESS) operation->id = OBERON_DRIVER_ID;
        return status;
#endif /* PSA_NEED_OBERON_AEAD_DRIVER */
        return PSA_ERROR_NOT_SUPPORTED;

        /* Add cases for opaque drivers here */

    default:
        /* Key is declared with a lifetime not known to us */
        (void)status;
        (void)operation;
        (void)key_buffer;
        (void)key_buffer_size;
        (void)alg;
        return PSA_ERROR_NOT_SUPPORTED;
    }
}

psa_status_t psa_driver_wrapper_aead_set_nonce(
    psa_aead_operation_t *operation,
    const uint8_t *nonce, size_t nonce_length)
{
    switch (operation->id) {


#ifdef PSA_NEED_OBERON_AEAD_DRIVER
    case OBERON_DRIVER_ID:
        return oberon_aead_set_nonce(
            &operation->ctx.oberon_aead_ctx,
            nonce, nonce_length);
#endif /* PSA_NEED_OBERON_AEAD_DRIVER */

    default:
        (void)nonce;
        (void)nonce_length;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_aead_set_lengths(
    psa_aead_operation_t *operation,
    size_t ad_length,
    size_t plaintext_length)
{
    switch (operation->id) {


#if PSA_NEED_OBERON_AEAD_DRIVER
    case OBERON_DRIVER_ID:
        return oberon_aead_set_lengths(
            &operation->ctx.oberon_aead_ctx,
            ad_length, plaintext_length);
#endif /* PSA_NEED_OBERON_AEAD_DRIVER */

    default:
        (void)ad_length;
        (void)plaintext_length;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_aead_update_ad(
    psa_aead_operation_t *operation,
    const uint8_t *input, size_t input_length)
{
    switch (operation->id) {


#ifdef PSA_NEED_OBERON_AEAD_DRIVER
    case OBERON_DRIVER_ID:
        return oberon_aead_update_ad(
            &operation->ctx.oberon_aead_ctx,
            input, input_length);
#endif /* PSA_NEED_OBERON_AEAD_DRIVER */

    default:
        (void)input;
        (void)input_length;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_aead_update(
    psa_aead_operation_t *operation,
    const uint8_t *input, size_t input_length,
    uint8_t *output, size_t output_size, size_t *output_length)
{
    switch (operation->id) {


#ifdef PSA_NEED_OBERON_AEAD_DRIVER
    case OBERON_DRIVER_ID:
        return oberon_aead_update(
            &operation->ctx.oberon_aead_ctx,
            input, input_length,
            output, output_size, output_length);
#endif /* PSA_NEED_OBERON_AEAD_DRIVER */

    default:
        (void)input;
        (void)input_length;
        (void)output;
        (void)output_size;
        (void)output_length;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_aead_finish(
    psa_aead_operation_t *operation,
    uint8_t *ciphertext, size_t ciphertext_size, size_t *ciphertext_length,
    uint8_t *tag, size_t tag_size, size_t *tag_length)
{
    switch (operation->id) {


#ifdef PSA_NEED_OBERON_AEAD_DRIVER
    case OBERON_DRIVER_ID:
        return oberon_aead_finish(
            &operation->ctx.oberon_aead_ctx,
            ciphertext, ciphertext_size, ciphertext_length,
            tag, tag_size, tag_length);
#endif /* PSA_NEED_OBERON_AEAD_DRIVER */

    default:
        (void)ciphertext;
        (void)ciphertext_size;
        (void)ciphertext_length;
        (void)tag;
        (void)tag_size;
        (void)tag_length;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_aead_verify(
    psa_aead_operation_t *operation,
    uint8_t *plaintext, size_t plaintext_size, size_t *plaintext_length,
    const uint8_t *tag, size_t tag_length)
{
    switch (operation->id) {


#ifdef PSA_NEED_OBERON_AEAD_DRIVER
    case OBERON_DRIVER_ID:
        return oberon_aead_verify(
            &operation->ctx.oberon_aead_ctx,
            plaintext, plaintext_size, plaintext_length,
            tag, tag_length);
#endif /* PSA_NEED_OBERON_AEAD_DRIVER */

    default:
        (void)plaintext;
        (void)plaintext_size;
        (void)plaintext_length;
        (void)tag;
        (void)tag_length;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_aead_abort(
    psa_aead_operation_t *operation)
{
    switch (operation->id) {


#ifdef PSA_NEED_OBERON_AEAD_DRIVER
    case OBERON_DRIVER_ID:
        return oberon_aead_abort(&operation->ctx.oberon_aead_ctx);
#endif /* PSA_NEED_OBERON_AEAD_DRIVER */

    default:
        return PSA_SUCCESS;
    }
}

/*
 * Asymmetric functions
 */
psa_status_t psa_driver_wrapper_asymmetric_encrypt(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg,
    const uint8_t *input, size_t input_length,
    const uint8_t *salt, size_t salt_length,
    uint8_t *output, size_t output_size, size_t *output_length)
{
    psa_status_t status;

    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
    case PSA_KEY_LOCATION_LOCAL_STORAGE:
        /* Add cases for transparent drivers here */
#ifdef PSA_NEED_OBERON_RSA_CRYPT
        return oberon_asymmetric_encrypt(
            attributes, key_buffer, key_buffer_size,
            alg,
            input, input_length,
            salt, salt_length,
            output, output_size, output_length);
#endif /* PSA_NEED_OBERON_RSA_CRYPT */
        return PSA_ERROR_NOT_SUPPORTED;

        /* Add cases for opaque driver here */

    default:
        /* Key is declared with a lifetime not known to us */
        (void)key_buffer;
        (void)key_buffer_size;
        (void)alg;
        (void)input;
        (void)input_length;
        (void)salt;
        (void)salt_length;
        (void)output;
        (void)output_size;
        (void)output_length;
        (void)status;
        return PSA_ERROR_INVALID_ARGUMENT;
    }
}

psa_status_t psa_driver_wrapper_asymmetric_decrypt(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg,
    const uint8_t *input, size_t input_length,
    const uint8_t *salt, size_t salt_length,
    uint8_t *output, size_t output_size, size_t *output_length)
{
    psa_status_t status;

    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
    case PSA_KEY_LOCATION_LOCAL_STORAGE:
        /* Add cases for transparent drivers here */
#ifdef PSA_NEED_OBERON_RSA_CRYPT
        return oberon_asymmetric_decrypt(
            attributes, key_buffer, key_buffer_size,
            alg,
            input, input_length,
            salt, salt_length,
            output, output_size, output_length);
#endif /* PSA_NEED_OBERON_RSA_CRYPT */
        return PSA_ERROR_NOT_SUPPORTED;

        /* Add cases for opaque driver here */

    default:
        /* Key is declared with a lifetime not known to us */
        (void)key_buffer;
        (void)key_buffer_size;
        (void)alg;
        (void)input;
        (void)input_length;
        (void)salt;
        (void)salt_length;
        (void)output;
        (void)output_size;
        (void)output_length;
        (void)status;
        return PSA_ERROR_INVALID_ARGUMENT;
    }
}

/*
 * MAC functions
 */
psa_status_t psa_driver_wrapper_mac_compute(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg,
    const uint8_t *input, size_t input_length,
    uint8_t *mac, size_t mac_size, size_t *mac_length)
{
    psa_status_t status;

    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
    case PSA_KEY_LOCATION_LOCAL_STORAGE:
        /* Add cases for transparent drivers here */
#ifdef PSA_NEED_OBERON_MAC_DRIVER
        return oberon_mac_compute(
            attributes, key_buffer, key_buffer_size,
            alg,
            input, input_length,
            mac, mac_size, mac_length);
#endif /* PSA_NEED_OBERON_MAC_DRIVER */
        return PSA_ERROR_NOT_SUPPORTED;

        /* Add cases for opaque driver here */

    default:
        /* Key is declared with a lifetime not known to us */
        (void)key_buffer;
        (void)key_buffer_size;
        (void)alg;
        (void)input;
        (void)input_length;
        (void)mac;
        (void)mac_size;
        (void)mac_length;
        (void)status;
        return PSA_ERROR_INVALID_ARGUMENT;
    }
}

psa_status_t psa_driver_wrapper_mac_sign_setup(
    psa_mac_operation_t *operation,
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg)
{
    psa_status_t status;

    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
    case PSA_KEY_LOCATION_LOCAL_STORAGE:
        /* Add cases for transparent drivers here */
#ifdef PSA_NEED_OBERON_MAC_DRIVER
        status = oberon_mac_sign_setup(
            &operation->ctx.oberon_mac_ctx,
            attributes, key_buffer, key_buffer_size,
            alg);
        if (status == PSA_SUCCESS) operation->id = OBERON_DRIVER_ID;
        return status;
#endif /* PSA_NEED_OBERON_MAC_DRIVER */
        return PSA_ERROR_NOT_SUPPORTED;

        /* Add cases for opaque driver here */

    default:
        /* Key is declared with a lifetime not known to us */
        (void)status;
        (void)operation;
        (void)key_buffer;
        (void)key_buffer_size;
        (void)alg;
        return PSA_ERROR_INVALID_ARGUMENT;
    }
}

psa_status_t psa_driver_wrapper_mac_verify_setup(
    psa_mac_operation_t *operation,
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg)
{
    psa_status_t status;

    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
    case PSA_KEY_LOCATION_LOCAL_STORAGE:
        /* Add cases for transparent drivers here */
#ifdef PSA_NEED_OBERON_MAC_DRIVER
        status = oberon_mac_verify_setup(
            &operation->ctx.oberon_mac_ctx,
            attributes, key_buffer, key_buffer_size,
            alg);
        if (status == PSA_SUCCESS) operation->id = OBERON_DRIVER_ID;
        return status;
#endif /* PSA_NEED_OBERON_MAC_DRIVER */
        return PSA_ERROR_NOT_SUPPORTED;

        /* Add cases for opaque driver here */

    default:
        /* Key is declared with a lifetime not known to us */
        (void)status;
        (void)operation;
        (void)key_buffer;
        (void)key_buffer_size;
        (void)alg;
        return PSA_ERROR_INVALID_ARGUMENT;
    }
}

psa_status_t psa_driver_wrapper_mac_update(
    psa_mac_operation_t *operation,
    const uint8_t *input, size_t input_length)
{
    switch (operation->id) {

#ifdef PSA_NEED_OBERON_MAC_DRIVER
    case OBERON_DRIVER_ID:
        return oberon_mac_update(
            &operation->ctx.oberon_mac_ctx,
            input, input_length);
#endif /* PSA_NEED_OBERON_MAC_DRIVER */

    default:
        (void)input;
        (void)input_length;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_mac_sign_finish(
    psa_mac_operation_t *operation,
    uint8_t *mac, size_t mac_size, size_t *mac_length)
{
    switch (operation->id) {

#ifdef PSA_NEED_OBERON_MAC_DRIVER
    case OBERON_DRIVER_ID:
        return oberon_mac_sign_finish(
            &operation->ctx.oberon_mac_ctx,
            mac, mac_size, mac_length);
#endif /* PSA_NEED_OBERON_MAC_DRIVER */

    default:
        (void)mac;
        (void)mac_size;
        (void)mac_length;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_mac_verify_finish(
    psa_mac_operation_t *operation,
    const uint8_t *mac, size_t mac_length)
{
    switch (operation->id) {

#ifdef PSA_NEED_OBERON_MAC_DRIVER
    case OBERON_DRIVER_ID:
        return oberon_mac_verify_finish(
            &operation->ctx.oberon_mac_ctx,
            mac, mac_length);
#endif /* PSA_NEED_OBERON_MAC_DRIVER */

    default:
        (void)mac;
        (void)mac_length;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_mac_abort(
    psa_mac_operation_t *operation)
{
    switch (operation->id) {

#ifdef PSA_NEED_OBERON_MAC_DRIVER
    case OBERON_DRIVER_ID:
        return oberon_mac_abort(&operation->ctx.oberon_mac_ctx);
#endif /* PSA_NEED_OBERON_MAC_DRIVER */

    default:
        return PSA_SUCCESS;
    }
}

/*
 * Key derivation functions
 */
psa_status_t psa_driver_wrapper_key_derivation_setup(
    psa_key_derivation_operation_t *operation,
    psa_algorithm_t alg)
{
    psa_status_t status;

#ifdef PSA_NEED_OBERON_KDF_DRIVER
    status = oberon_key_derivation_setup(
        &operation->ctx.oberon_kdf_ctx,
        alg);
    if (status == PSA_SUCCESS) operation->id = OBERON_DRIVER_ID;
    return status;
#endif /* PSA_NEED_OBERON_KDF_DRIVER */

    (void)status;
    (void)operation;
    (void)alg;
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_driver_wrapper_key_derivation_set_capacity(
    psa_key_derivation_operation_t *operation,
    size_t capacity)
{
    switch (operation->id) {
#ifdef PSA_NEED_OBERON_KDF_DRIVER
    case OBERON_DRIVER_ID:
        return oberon_key_derivation_set_capacity(
            &operation->ctx.oberon_kdf_ctx,
            capacity);
#endif /* PSA_NEED_OBERON_KDF_DRIVER */

    default:
        (void)capacity;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_key_derivation_input_bytes(
    psa_key_derivation_operation_t *operation,
    psa_key_derivation_step_t step,
    const uint8_t *data, size_t data_length)
{
    switch (operation->id) {
#ifdef PSA_NEED_OBERON_KDF_DRIVER
    case OBERON_DRIVER_ID:
        return oberon_key_derivation_input_bytes(
            &operation->ctx.oberon_kdf_ctx,
            step,
            data, data_length);
#endif /* PSA_NEED_OBERON_KDF_DRIVER */

    default:
        (void)step;
        (void)data;
        (void)data_length;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_key_derivation_input_integer(
    psa_key_derivation_operation_t *operation,
    psa_key_derivation_step_t step,
    uint64_t value)
{
    switch (operation->id) {
#ifdef PSA_NEED_OBERON_KDF_DRIVER
    case OBERON_DRIVER_ID:
        return oberon_key_derivation_input_integer(
            &operation->ctx.oberon_kdf_ctx,
            step,
            value);
#endif /* PSA_NEED_OBERON_KDF_DRIVER */

    default:
        (void)step;
        (void)value;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_key_derivation_output_bytes(
    psa_key_derivation_operation_t *operation,
    uint8_t *output, size_t output_length)
{
    switch (operation->id) {
#ifdef PSA_NEED_OBERON_KDF_DRIVER
    case OBERON_DRIVER_ID:
        return oberon_key_derivation_output_bytes(
            &operation->ctx.oberon_kdf_ctx,
            output, output_length);
#endif /* PSA_NEED_OBERON_KDF_DRIVER */

    default:
        (void)output;
        (void)output_length;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_key_derivation_abort(
    psa_key_derivation_operation_t *operation)
{
    switch (operation->id) {
#ifdef PSA_NEED_OBERON_KDF_DRIVER
    case OBERON_DRIVER_ID:
        return oberon_key_derivation_abort(&operation->ctx.oberon_kdf_ctx);
#endif /* PSA_NEED_OBERON_KDF_DRIVER */

    default:
        return PSA_SUCCESS;
    }
}

/*
 * Raw Key Agreement
 */
psa_status_t psa_driver_wrapper_key_agreement(
    const psa_key_attributes_t *attributes,
    const uint8_t *key, size_t key_length,
    psa_algorithm_t alg,
    const uint8_t *peer_key, size_t peer_key_length,
    uint8_t *output, size_t output_size, size_t *output_length)
{
    psa_status_t status;

    switch (PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime)) {
    case PSA_KEY_LOCATION_LOCAL_STORAGE:
        /* Add cases for transparent drivers here */
#ifdef PSA_NEED_OBERON_ECDH_DRIVER
        return oberon_key_agreement(
            attributes, key, key_length,
            alg,
            peer_key, peer_key_length,
            output, output_size, output_length);
#endif /* PSA_NEED_OBERON_ECDH_DRIVER */
        return PSA_ERROR_NOT_SUPPORTED;

    default:
        /* Key is declared with a lifetime not known to us */
        (void)alg;
        (void)key;
        (void)key_length;
        (void)peer_key;
        (void)peer_key_length;
        (void)output;
        (void)output_size;
        (void)output_length;
        (void)status;
        return PSA_ERROR_INVALID_ARGUMENT;
    }
}

/*
 * PAKE functions
 */
psa_status_t psa_driver_wrapper_pake_setup(
    psa_pake_operation_t *operation,
    const psa_pake_cipher_suite_t *cipher_suite)
{
    psa_status_t status;

#ifdef PSA_NEED_OBERON_JPAKE_DRIVER
    if (cipher_suite->algorithm == PSA_ALG_JPAKE) {
        status = oberon_jpake_setup(
            &operation->ctx.oberon_jpake_ctx, cipher_suite);
        if (status == PSA_SUCCESS) operation->id = OBERON_JPAKE_DRIVER_ID;
        return status;
    }
#endif /* PSA_NEED_OBERON_JPAKE_DRIVER */
#ifdef PSA_NEED_OBERON_SPAKE2P_DRIVER
    if (cipher_suite->algorithm == PSA_ALG_SPAKE2P) {
        status = oberon_spake2p_setup(
            &operation->ctx.oberon_spake2p_ctx, cipher_suite);
        if (status == PSA_SUCCESS) operation->id = OBERON_SPAKE_DRIVER_ID;
        return status;
    }
#endif /* PSA_NEED_OBERON_SPAKE2P_DRIVER */
#ifdef PSA_NEED_OBERON_SRP_DRIVER
    if (cipher_suite->algorithm == PSA_ALG_SRP_6) {
        status = oberon_srp_setup(
            &operation->ctx.oberon_srp_ctx, cipher_suite);
        if (status == PSA_SUCCESS) operation->id = OBERON_SRP_DRIVER_ID;
        return status;
    }
#endif /* PSA_NEED_OBERON_SRP_DRIVER */

    (void)status;
    (void)operation;
    (void)cipher_suite;
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_driver_wrapper_pake_set_password_key(
    psa_pake_operation_t *operation,
    const psa_key_attributes_t *attributes,
    const uint8_t *password, size_t password_length)
{
    switch (operation->id) {
#ifdef PSA_NEED_OBERON_JPAKE_DRIVER
    case OBERON_JPAKE_DRIVER_ID:
        return oberon_jpake_set_password_key(
            &operation->ctx.oberon_jpake_ctx,
            attributes, password, password_length);
#endif /* PSA_NEED_OBERON_JPAKE_DRIVER */
#ifdef PSA_NEED_OBERON_SPAKE2P_DRIVER
    case OBERON_SPAKE_DRIVER_ID:
        return oberon_spake2p_set_password_key(
            &operation->ctx.oberon_spake2p_ctx,
            attributes, password, password_length);
#endif /* PSA_NEED_OBERON_SPAKE2P_DRIVER */
#ifdef PSA_NEED_OBERON_SRP_DRIVER
    case OBERON_SRP_DRIVER_ID:
        return oberon_srp_set_password_key(
            &operation->ctx.oberon_srp_ctx,
            attributes, password, password_length);
#endif /* PSA_NEED_OBERON_SRP_DRIVER */

    default:
        (void)attributes;
        (void)password;
        (void)password_length;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_pake_set_user(
    psa_pake_operation_t *operation,
    const uint8_t *user_id, size_t user_id_len)
{
    switch (operation->id) {
#ifdef PSA_NEED_OBERON_JPAKE_DRIVER
    case OBERON_JPAKE_DRIVER_ID:
        return oberon_jpake_set_user(
            &operation->ctx.oberon_jpake_ctx,
            user_id, user_id_len);
#endif /* PSA_NEED_OBERON_JPAKE_DRIVER */
#ifdef PSA_NEED_OBERON_SPAKE2P_DRIVER
    case OBERON_SPAKE_DRIVER_ID:
        return oberon_spake2p_set_user(
            &operation->ctx.oberon_spake2p_ctx,
            user_id, user_id_len);
#endif /* PSA_NEED_OBERON_SPAKE2P_DRIVER */
#ifdef PSA_NEED_OBERON_SRP_DRIVER
    case OBERON_SRP_DRIVER_ID:
        return oberon_srp_set_user(
            &operation->ctx.oberon_srp_ctx,
            user_id, user_id_len);
#endif /* PSA_NEED_OBERON_SRP_DRIVER */

    default:
        (void)user_id;
        (void)user_id_len;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_pake_set_peer(
    psa_pake_operation_t *operation,
    const uint8_t *peer_id, size_t peer_id_len)
{
    switch (operation->id) {
#ifdef PSA_NEED_OBERON_JPAKE_DRIVER
    case OBERON_JPAKE_DRIVER_ID:
        return oberon_jpake_set_peer(
            &operation->ctx.oberon_jpake_ctx,
            peer_id, peer_id_len);
#endif /* PSA_NEED_OBERON_JPAKE_DRIVER */
#ifdef PSA_NEED_OBERON_SPAKE2P_DRIVER
    case OBERON_SPAKE_DRIVER_ID:
        return oberon_spake2p_set_peer(
            &operation->ctx.oberon_spake2p_ctx,
            peer_id, peer_id_len);
#endif /* PSA_NEED_OBERON_SPAKE2P_DRIVER */
#ifdef PSA_NEED_OBERON_SRP_DRIVER
    case OBERON_SRP_DRIVER_ID:
        return PSA_ERROR_NOT_SUPPORTED;
#endif /* PSA_NEED_OBERON_SRP_DRIVER */

    default:
        (void)peer_id;
        (void)peer_id_len;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_pake_set_role(
    psa_pake_operation_t *operation,
    psa_pake_role_t role)
{
    switch (operation->id) {
#ifdef PSA_NEED_OBERON_JPAKE_DRIVER
    case OBERON_JPAKE_DRIVER_ID:
        return oberon_jpake_set_role(
            &operation->ctx.oberon_jpake_ctx,
            role);
#endif /* PSA_NEED_OBERON_JPAKE_DRIVER */
#ifdef PSA_NEED_OBERON_SPAKE2P_DRIVER
    case OBERON_SPAKE_DRIVER_ID:
        return oberon_spake2p_set_role(
            &operation->ctx.oberon_spake2p_ctx,
            role);
#endif /* PSA_NEED_OBERON_SPAKE2P_DRIVER */
#ifdef PSA_NEED_OBERON_SRP_DRIVER
    case OBERON_SRP_DRIVER_ID:
        return oberon_srp_set_role(
            &operation->ctx.oberon_srp_ctx,
            role);
#endif /* PSA_NEED_OBERON_SRP_DRIVER */

    default:
        (void)role;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_pake_output(
    psa_pake_operation_t *operation,
    psa_pake_step_t step,
    uint8_t *output, size_t output_size, size_t *output_length)
{
    switch (operation->id) {
#ifdef PSA_NEED_OBERON_JPAKE_DRIVER
    case OBERON_JPAKE_DRIVER_ID:
        return oberon_jpake_output(
            &operation->ctx.oberon_jpake_ctx,
            step,
            output, output_size, output_length);
#endif /* PSA_NEED_OBERON_JPAKE_DRIVER */
#ifdef PSA_NEED_OBERON_SPAKE2P_DRIVER
    case OBERON_SPAKE_DRIVER_ID:
        return oberon_spake2p_output(
            &operation->ctx.oberon_spake2p_ctx,
            step,
            output, output_size, output_length);
#endif /* PSA_NEED_OBERON_SPAKE2P_DRIVER */
#ifdef PSA_NEED_OBERON_SRP_DRIVER
    case OBERON_SRP_DRIVER_ID:
        return oberon_srp_output(
            &operation->ctx.oberon_srp_ctx,
            step,
            output, output_size, output_length);
#endif /* PSA_NEED_OBERON_SRP_DRIVER */

    default:
        (void)step;
        (void)output;
        (void)output_size;
        (void)output_length;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_pake_input(
    psa_pake_operation_t *operation,
    psa_pake_step_t step,
    const uint8_t *input, size_t input_length)
{
    switch (operation->id) {
#ifdef PSA_NEED_OBERON_JPAKE_DRIVER
    case OBERON_JPAKE_DRIVER_ID:
        return oberon_jpake_input(
            &operation->ctx.oberon_jpake_ctx,
            step,
            input, input_length);
#endif /* PSA_NEED_OBERON_JPAKE_DRIVER */
#ifdef PSA_NEED_OBERON_SPAKE2P_DRIVER
    case OBERON_SPAKE_DRIVER_ID:
        return oberon_spake2p_input(
            &operation->ctx.oberon_spake2p_ctx,
            step,
            input, input_length);
#endif /* PSA_NEED_OBERON_SPAKE2P_DRIVER */
#ifdef PSA_NEED_OBERON_SRP_DRIVER
    case OBERON_SRP_DRIVER_ID:
        return oberon_srp_input(
            &operation->ctx.oberon_srp_ctx,
            step,
            input, input_length);
#endif /* PSA_NEED_OBERON_SRP_DRIVER */

    default:
        (void)step;
        (void)input;
        (void)input_length;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_pake_get_implicit_key(
    psa_pake_operation_t *operation,
    uint8_t *output, size_t output_size, size_t *output_length)
{
    switch (operation->id) {
#ifdef PSA_NEED_OBERON_JPAKE_DRIVER
    case OBERON_JPAKE_DRIVER_ID:
        return oberon_jpake_get_implicit_key(
            &operation->ctx.oberon_jpake_ctx,
            output, output_size, output_length);
#endif /* PSA_NEED_OBERON_JPAKE_DRIVER */
#ifdef PSA_NEED_OBERON_SPAKE2P_DRIVER
    case OBERON_SPAKE_DRIVER_ID:
        return oberon_spake2p_get_implicit_key(
            &operation->ctx.oberon_spake2p_ctx,
            output, output_size, output_length);
#endif /* PSA_NEED_OBERON_SPAKE2P_DRIVER */
#ifdef PSA_NEED_OBERON_SRP_DRIVER
    case OBERON_SRP_DRIVER_ID:
        return oberon_srp_get_implicit_key(
            &operation->ctx.oberon_srp_ctx,
            output, output_size, output_length);
#endif /* PSA_NEED_OBERON_SRP_DRIVER */

    default:
        (void)output;
        (void)output_size;
        (void)output_length;
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_driver_wrapper_pake_abort(
    psa_pake_operation_t *operation)
{
    switch (operation->id) {
#ifdef PSA_NEED_OBERON_JPAKE_DRIVER
    case OBERON_JPAKE_DRIVER_ID:
        return oberon_jpake_abort(&operation->ctx.oberon_jpake_ctx);
#endif /* PSA_NEED_OBERON_JPAKE_DRIVER */
#ifdef PSA_NEED_OBERON_SPAKE2P_DRIVER
    case OBERON_SPAKE_DRIVER_ID:
        return oberon_spake2p_abort(&operation->ctx.oberon_spake2p_ctx);
#endif /* PSA_NEED_OBERON_SPAKE2P_DRIVER */
#ifdef PSA_NEED_OBERON_SRP_DRIVER
    case OBERON_SRP_DRIVER_ID:
        return oberon_srp_abort(&operation->ctx.oberon_srp_ctx);
#endif /* PSA_NEED_OBERON_SRP_DRIVER */

    default:
        return PSA_SUCCESS;
    }
}

/*
 * Random
 */
psa_status_t psa_driver_wrapper_get_entropy(
    uint32_t flags,
    size_t *estimate_bits,
    uint8_t *output,
    size_t output_size)
{

#ifdef PSA_NEED_ENTROPY_DEMO_DRIVER
    return demo_get_entropy(flags, estimate_bits, output, output_size);
#endif /* PSA_NEED_ENTROPY_DEMO_DRIVER */

    (void)flags;
    (void)output;
    (void)output_size;
    *estimate_bits = 0;
    return PSA_ERROR_INSUFFICIENT_ENTROPY;
}

psa_status_t psa_driver_wrapper_init_random(
    psa_driver_random_context_t *context)
{
#ifdef PSA_NEED_ENTROPY_DEMO_DRIVER
    psa_status_t status;
    status = demo_init_entropy();
    if (status != PSA_SUCCESS) return status;
#endif /* PSA_NEED_ENTROPY_DEMO_DRIVER */
#ifdef PSA_NEED_OBERON_CTR_DRBG_DRIVER
    return oberon_ctr_drbg_init(&context->oberon_ctr_drbg_ctx);
#endif /* PSA_NEED_OBERON_CTR_DRBG_DRIVER */
#ifdef PSA_NEED_OBERON_HMAC_DRBG_DRIVER
    return oberon_hmac_drbg_init(&context->oberon_hmac_drbg_ctx);
#endif /* PSA_NEED_OBERON_HMAC_DRBG_DRIVER */

    (void)context;
    return PSA_SUCCESS;
}

psa_status_t psa_driver_wrapper_get_random(
    psa_driver_random_context_t *context,
    uint8_t *output, size_t output_size)
{
#ifdef PSA_NEED_OBERON_CTR_DRBG_DRIVER
    return oberon_ctr_drbg_get_random(&context->oberon_ctr_drbg_ctx, output, output_size);
#endif /* PSA_NEED_OBERON_CTR_DRBG_DRIVER */
#ifdef PSA_NEED_OBERON_HMAC_DRBG_DRIVER
    return oberon_hmac_drbg_get_random(&context->oberon_hmac_drbg_ctx, output, output_size);
#endif /* PSA_NEED_OBERON_HMAC_DRBG_DRIVER */

    (void)context;
    (void)output;
    (void)output_size;
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_driver_wrapper_free_random(
    psa_driver_random_context_t *context)
{
#ifdef PSA_NEED_OBERON_CTR_DRBG_DRIVER
    return oberon_ctr_drbg_free(&context->oberon_ctr_drbg_ctx);
#endif /* PSA_NEED_OBERON_CTR_DRBG_DRIVER */
#ifdef PSA_NEED_OBERON_HMAC_DRBG_DRIVER
    return oberon_hmac_drbg_free(&context->oberon_hmac_drbg_ctx);
#endif /* PSA_NEED_OBERON_HMAC_DRBG_DRIVER */

    (void)context;
    return PSA_SUCCESS;
}

#endif /* MBEDTLS_PSA_CRYPTO_C */
