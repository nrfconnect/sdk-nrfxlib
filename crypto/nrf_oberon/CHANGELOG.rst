.. _crypto_changelog_oberon:

Changelog - nRF Oberon
######################

.. contents::
   :local:
   :depth: 2

All notable changes to this project are documented in this file.

nrf_oberon - 3.0.13
*******************

New version of the nrf_oberon library with the following changes.

Added
=====

* Added Oberon PSA core, a heavily code-size optimized and efficient implementation of PSA core licensed for use on Nordic Semiconductor devices.
* Added ocrypto APIs for PBKDF2 support (CMAC and HMAC using SHA-1, SHA-256).
* Added ocrypto APIs for SPAKE2+ using ECC curve type secp256r1.
* Added Oberon PSA crypto drivers as source distribution with extensive support:
  - AEAD (AES CCM, AES GCM, ChaCha20/Poly1305)
  - Cipher (Chacha20, AES CTR, AES CBC, AES CCM* and AES ECB)
  - EC J-PAKE using ECC curve type secp256r1
  - ECDH using ECC curve types secp224r1, secp256r1, secp384r1
  - X25519
  - ECDSA using ECC curve types secp224r1, secp256r1, secp384r1
  - Ed25519
  - HASH (SHA-1, SHA-224, SHA-256, SHA-384 and SHA-512)
  - HKDF
  - PBKDF2 using CMAC and AES-128
  - PBKDF2 using HMAC and SHA-1 or SHA-256
  - TLS 1.2 PRF functions
  - HMAC, CMAC and CBC MAC
  - RSA PKCS#1 1.5 and 2.1
* Added custom Oberon PSA crypto drivers as source distribution:
  - CTR_DRBG
  - HMAC_DRBG
  - SPAKE2+ using ECC curve type secp256r1
  - SRP

Removed
=======

* Removed binary distribution of Oberon PSA crypto drivers.

Library built against Mbed TLS version 3.3.0.

Added the following Oberon crypto libraries for nRF9160, nRF53, nRF52, and nRF51 architectures.

.. note::
   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_oberon, nRF9160 and nRF53 application core variants

  * ``cortex-m33/hard-float/liboberon_3.0.13.a``
  * ``cortex-m33/hard-float/liboberon_mbedtls_3.0.13.a``
  * ``cortex-m33/soft-float/liboberon_3.0.13.a``
  * ``cortex-m33/soft-float/liboberon_mbedtls_3.0.13.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/liboberon_3.0.13.a``
    * ``cortex-m33/hard-float/short-wchar/liboberon_mbedtls_3.0.13.a``
    * ``cortex-m33/soft-float/short-wchar/liboberon_3.0.13.a``
    * ``cortex-m33/soft-float/short-wchar/liboberon_mbedtls_3.0.13.a``

  * Keil

    * ``cortex-m33/hard-float/short-wchar/oberon_3.0.13.lib``
    * ``cortex-m33/hard-float/short-wchar/oberon_mbedtls_3.0.13.lib``
    * ``cortex-m33/soft-float/short-wchar/oberon_3.0.13.lib``
    * ``cortex-m33/soft-float/short-wchar/oberon_mbedtls_3.0.13.lib``

* nrf_oberon, nRF53 network core variants

  * ``cortex-m33+nodsp/soft-float/liboberon_3.0.13.a``
  * ``cortex-m33+nodsp/soft-float/liboberon_mbedtls_3.0.13.a``

  * short-wchar

    * ``cortex-m33+nodsp/soft-float/short-wchar/liboberon_3.0.13.a``
    * ``cortex-m33+nodsp/soft-float/short-wchar/liboberon_mbedtls_3.0.13.a``

  * Keil

    * ``cortex-m33/soft-float/short-wchar/oberon_3.0.13.lib``
    * ``cortex-m33/soft-float/short-wchar/oberon_mbedtls_3.0.13.lib``

* nrf_oberon, nRF52 variants

  * ``cortex-m4/hard-float/liboberon_3.0.13.a``
  * ``cortex-m4/hard-float/liboberon_mbedtls_3.0.13.a``
  * ``cortex-m4/soft-float/liboberon_3.0.13.a``
  * ``cortex-m4/soft-float/liboberon_mbedtls_3.0.13.a.a``

  * short-wchar

    * ``cortex-m4/hard-float/short-wchar/liboberon_3.0.13.a``
    * ``cortex-m4/hard-float/short-wchar/liboberon_mbedtls_3.0.13.a``
    * ``cortex-m4/soft-float/short-wchar/liboberon_3.0.13.a``
    * ``cortex-m4/soft-float/short-wchar/liboberon_mbedtls_3.0.13.a``

  * Keil

    * ``cortex-m4/soft-float/short-wchar/oberon_3.0.13.lib``
    * ``cortex-m4/soft-float/short-wchar/oberon_mbedtls_3.0.13.lib``
    * ``cortex-m4/hard-float/short-wchar/oberon_3.0.13.lib``
    * ``cortex-m4/hard-float/short-wchar/oberon_mbedtls_3.0.13.lib``

* nrf_oberon, nRF51 variants

  * ``cortex-m0/soft-float/liboberon_3.0.13.a``
  * ``cortex-m0/soft-float/liboberon_mbedtls_3.0.13.a``

  * short-wchar

    * ``cortex-m0/soft-float/short-wchar/liboberon_3.0.13.a``
    * ``cortex-m0/soft-float/short-wchar/liboberon_mbedtls_3.0.13.a``

  * Keil

    * ``cortex-m0/soft-float/short-wchar/oberon_3.0.13.lib``
    * ``cortex-m0/soft-float/short-wchar/oberon_mbedtls_3.0.13.lib``

nrf_oberon - 3.0.12
*******************

New version of the nrf_oberon library with the following changes.

Added
=====

* Added incremental ocrypto HMAC API.
* Reduced SHA-1 stack size.
* Improved ECDSA performance.
* Changed the API for PSA Cipher for nrf_oberon PSA Crypto driver (now includes ``iv`` and ``iv_length`` parameters).

Library built against Mbed TLS version 3.1.0.

Added the following Oberon crypto libraries for nRF9160, nRF53, nRF52, and nRF51 architectures.

.. note::
   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_oberon, nRF9160 and nRF53 application core variants

  * ``cortex-m33/hard-float/liboberon_3.0.12.a``
  * ``cortex-m33/hard-float/liboberon_psa_3.0.12.a``
  * ``cortex-m33/hard-float/liboberon_mbedtls_3.0.12.a``
  * ``cortex-m33/soft-float/liboberon_3.0.12.a``
  * ``cortex-m33/soft-float/liboberon_psa_3.0.12.a``
  * ``cortex-m33/soft-float/liboberon_mbedtls_3.0.12.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/liboberon_3.0.12.a``
    * ``cortex-m33/hard-float/short-wchar/liboberon_psa_3.0.12.a``
    * ``cortex-m33/hard-float/short-wchar/liboberon_mbedtls_3.0.12.a``
    * ``cortex-m33/soft-float/short-wchar/liboberon_3.0.12.a``
    * ``cortex-m33/soft-float/short-wchar/liboberon_psa_3.0.12.a``
    * ``cortex-m33/soft-float/short-wchar/liboberon_mbedtls_3.0.12.a``

  * Keil

    * ``cortex-m33/hard-float/short-wchar/oberon_3.0.12.lib``
    * ``cortex-m33/hard-float/short-wchar/oberon_psa_3.0.12.lib``
    * ``cortex-m33/hard-float/short-wchar/oberon_mbedtls_3.0.12.lib``
    * ``cortex-m33/soft-float/short-wchar/oberon_3.0.12.lib``
    * ``cortex-m33/soft-float/short-wchar/oberon_psa_3.0.12.lib``
    * ``cortex-m33/soft-float/short-wchar/oberon_mbedtls_3.0.12.lib``

* nrf_oberon, nrf53 network core variants

  * ``cortex-m33+nodsp/soft-float/liboberon_3.0.12.a``
  * ``cortex-m33+nodsp/soft-float/liboberon_psa_3.0.12.a``
  * ``cortex-m33+nodsp/soft-float/liboberon_mbedtls_3.0.12.a``

  * short-wchar

    * ``cortex-m33+nodsp/soft-float/short-wchar/liboberon_3.0.12.a``
    * ``cortex-m33+nodsp/soft-float/short-wchar/liboberon_psa_3.0.12.a``
    * ``cortex-m33+nodsp/soft-float/short-wchar/liboberon_mbedtls_3.0.12.a``

  * Keil

    * ``cortex-m33/soft-float/short-wchar/oberon_3.0.12.lib``
    * ``cortex-m33/soft-float/short-wchar/oberon_psa_3.0.12.lib``
    * ``cortex-m33/soft-float/short-wchar/oberon_mbedtls_3.0.12.lib``

* nrf_oberon, nRF52 variants

  * ``cortex-m4/hard-float/liboberon_3.0.12.a``
  * ``cortex-m4/hard-float/liboberon_psa_3.0.12.a``
  * ``cortex-m4/hard-float/liboberon_mbedtls_3.0.12.a``
  * ``cortex-m4/soft-float/liboberon_3.0.12.a``
  * ``cortex-m4/soft-float/liboberon_psa_3.0.12.a``
  * ``cortex-m4/soft-float/liboberon_mbedtls_3.0.12.a.a``

  * short-wchar

    * ``cortex-m4/hard-float/short-wchar/liboberon_3.0.12.a``
    * ``cortex-m4/hard-float/short-wchar/liboberon_psa_3.0.12.a``
    * ``cortex-m4/hard-float/short-wchar/liboberon_mbedtls_3.0.12.a``
    * ``cortex-m4/soft-float/short-wchar/liboberon_3.0.12.a``
    * ``cortex-m4/soft-float/short-wchar/liboberon_psa_3.0.12.a``
    * ``cortex-m4/soft-float/short-wchar/liboberon_mbedtls_3.0.12.a``

  * Keil

    * ``cortex-m4/soft-float/short-wchar/oberon_3.0.12.lib``
    * ``cortex-m4/soft-float/short-wchar/oberon_psa_3.0.12.lib``
    * ``cortex-m4/soft-float/short-wchar/oberon_mbedtls_3.0.12.lib``
    * ``cortex-m4/hard-float/short-wchar/oberon_3.0.12.lib``
    * ``cortex-m4/hard-float/short-wchar/oberon_psa_3.0.12.lib``
    * ``cortex-m4/hard-float/short-wchar/oberon_mbedtls_3.0.12.lib``

* nrf_oberon, nRF51 variants

  * ``cortex-m0/soft-float/liboberon_3.0.12.a``
  * ``cortex-m0/soft-float/oberon_psa_3.0.12.lib``
  * ``cortex-m0/soft-float/liboberon_mbedtls_3.0.12.a``

  * short-wchar

    * ``cortex-m0/soft-float/short-wchar/liboberon_3.0.12.a``
    * ``cortex-m0/soft-float/short-wchar/liboberon_psa_3.0.12.a``
    * ``cortex-m0/soft-float/short-wchar/liboberon_mbedtls_3.0.12.a``

  * Keil

    * ``cortex-m0/soft-float/short-wchar/oberon_3.0.12.lib``
    * ``cortex-m0/soft-float/short-wchar/oberon_psa_3.0.12.lib``
    * ``cortex-m0/soft-float/short-wchar/oberon_mbedtls_3.0.12.lib``


nrf_oberon - 3.0.11
*******************

New version of the nrf_oberon library with the following changes.

Added
=====

* Added support for in-place encryption in PSA Crypto, needed for TLS/DTLS
* Added PKCS#7 padding for CBC
* Added support for 16 bytes IV for GCM in PSA Crypto APIs


Added the following Oberon crypto libraries for nRF9160, nRF53, nRF52, and nRF51 architectures.

.. note::
   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_oberon, nRF9160 and nRF53 application core variants

  * ``cortex-m33/hard-float/liboberon_3.0.11.a``
  * ``cortex-m33/hard-float/liboberon_psa_3.0.11.a``
  * ``cortex-m33/hard-float/liboberon_mbedtls_3.0.11.a``
  * ``cortex-m33/soft-float/liboberon_3.0.11.a``
  * ``cortex-m33/soft-float/liboberon_psa_3.0.11.a``
  * ``cortex-m33/soft-float/liboberon_mbedtls_3.0.11.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/liboberon_3.0.11.a``
    * ``cortex-m33/hard-float/short-wchar/liboberon_psa_3.0.11.a``
    * ``cortex-m33/hard-float/short-wchar/liboberon_mbedtls_3.0.11.a``
    * ``cortex-m33/soft-float/short-wchar/liboberon_3.0.11.a``
    * ``cortex-m33/soft-float/short-wchar/liboberon_psa_3.0.11.a``
    * ``cortex-m33/soft-float/short-wchar/liboberon_mbedtls_3.0.11.a``

  * Keil

    * ``cortex-m33/hard-float/short-wchar/oberon_3.0.11.lib``
    * ``cortex-m33/hard-float/short-wchar/oberon_psa_3.0.11.lib``
    * ``cortex-m33/hard-float/short-wchar/oberon_mbedtls_3.0.11.lib``
    * ``cortex-m33/soft-float/short-wchar/oberon_3.0.11.lib``
    * ``cortex-m33/soft-float/short-wchar/oberon_psa_3.0.11.lib``
    * ``cortex-m33/soft-float/short-wchar/oberon_mbedtls_3.0.11.lib``

* nrf_oberon, nrf53 network core variants

  * ``cortex-m33+nodsp/soft-float/liboberon_3.0.11.a``
  * ``cortex-m33+nodsp/soft-float/liboberon_psa_3.0.11.a``
  * ``cortex-m33+nodsp/soft-float/liboberon_mbedtls_3.0.11.a``

  * short-wchar

    * ``cortex-m33+nodsp/soft-float/short-wchar/liboberon_3.0.11.a``
    * ``cortex-m33+nodsp/soft-float/short-wchar/liboberon_psa_3.0.11.a``
    * ``cortex-m33+nodsp/soft-float/short-wchar/liboberon_mbedtls_3.0.11.a``

  * Keil

    * ``cortex-m33/soft-float/short-wchar/oberon_3.0.11.lib``
    * ``cortex-m33/soft-float/short-wchar/oberon_psa_3.0.11.lib``
    * ``cortex-m33/soft-float/short-wchar/oberon_mbedtls_3.0.11.lib``

* nrf_oberon, nRF52 variants

  * ``cortex-m4/hard-float/liboberon_3.0.11.a``
  * ``cortex-m4/hard-float/liboberon_psa_3.0.11.a``
  * ``cortex-m4/hard-float/liboberon_mbedtls_3.0.11.a``
  * ``cortex-m4/soft-float/liboberon_3.0.11.a``
  * ``cortex-m4/soft-float/liboberon_psa_3.0.11.a``
  * ``cortex-m4/soft-float/liboberon_mbedtls_3.0.11.a.a``

  * short-wchar

    * ``cortex-m4/hard-float/short-wchar/liboberon_3.0.11.a``
    * ``cortex-m4/hard-float/short-wchar/liboberon_psa_3.0.11.a``
    * ``cortex-m4/hard-float/short-wchar/liboberon_mbedtls_3.0.11.a``
    * ``cortex-m4/soft-float/short-wchar/liboberon_3.0.11.a``
    * ``cortex-m4/soft-float/short-wchar/liboberon_psa_3.0.11.a``
    * ``cortex-m4/soft-float/short-wchar/liboberon_mbedtls_3.0.11.a``

  * Keil

    * ``cortex-m4/soft-float/short-wchar/oberon_3.0.11.lib``
    * ``cortex-m4/soft-float/short-wchar/oberon_psa_3.0.11.lib``
    * ``cortex-m4/soft-float/short-wchar/oberon_mbedtls_3.0.11.lib``
    * ``cortex-m4/hard-float/short-wchar/oberon_3.0.11.lib``
    * ``cortex-m4/hard-float/short-wchar/oberon_psa_3.0.11.lib``
    * ``cortex-m4/hard-float/short-wchar/oberon_mbedtls_3.0.11.lib``

* nrf_oberon, nRF51 variants

  * ``cortex-m0/soft-float/liboberon_3.0.11.a``
  * ``cortex-m0/soft-float/oberon_psa_3.0.11.lib``
  * ``cortex-m0/soft-float/liboberon_mbedtls_3.0.11.a``

  * short-wchar

    * ``cortex-m0/soft-float/short-wchar/liboberon_3.0.11.a``
    * ``cortex-m0/soft-float/short-wchar/liboberon_psa_3.0.11.a``
    * ``cortex-m0/soft-float/short-wchar/liboberon_mbedtls_3.0.11.a``


  * Keil

    * ``cortex-m0/soft-float/short-wchar/oberon_3.0.11.lib``
    * ``cortex-m0/soft-float/short-wchar/oberon_psa_3.0.11.lib``
    * ``cortex-m0/soft-float/short-wchar/oberon_mbedtls_3.0.11.lib``


nrf_oberon - 3.0.10
*******************

New version of the nrf_oberon library with the following changes.

Added
=====

* Fixed an issue with the ChaChaPoly PSA APIs where more IV sizes than supported by the APIs were accepted.
* Added support for the PSA APIs.

Added the following Oberon crypto libraries for nRF9160, nRF53, nRF52, and nRF51 architectures.

.. note::
   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_oberon, nRF9160 and nRF53 application core variants

  * ``cortex-m33/hard-float/liboberon_3.0.10.a``
  * ``cortex-m33/soft-float/liboberon_3.0.10.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/liboberon_3.0.10.a``
    * ``cortex-m33/soft-float/short-wchar/liboberon_3.0.10.a``

  * Keil

    * ``cortex-m33/hard-float/short-wchar/oberon_3.0.10.lib``
    * ``cortex-m33/soft-float/short-wchar/oberon_3.0.10.lib``

* nrf_oberon, nrf53 network core variants

  * ``cortex-m33+nodsp/soft-float/liboberon_3.0.10.a``

  * short-wchar

    * ``cortex-m33+nodsp/soft-float/short-wchar/liboberon_3.0.10.a``

  * Keil

    * ``cortex-m33/soft-float/short-wchar/oberon_3.0.10.lib``

* nrf_oberon, nRF52 variants

  * ``cortex-m4/hard-float/liboberon_3.0.10.a``
  * ``cortex-m4/soft-float/liboberon_3.0.10.a``

  * short-wchar

    * ``cortex-m4/hard-float/short-wchar/liboberon_3.0.10.a``
    * ``cortex-m4/soft-float/short-wchar/liboberon_3.0.10.a``

  * Keil

    * ``cortex-m4/soft-float/short-wchar/oberon_3.0.10.lib``
    * ``cortex-m4/hard-float/short-wchar/oberon_3.0.10.lib``

* nrf_oberon, nRF51 variants

  * ``cortex-m0/soft-float/liboberon_3.0.10.a``

  * short-wchar

    * ``cortex-m0/soft-float/short-wchar/liboberon_3.0.10.a``

  * Keil

    * ``cortex-m0/soft-float/short-wchar/oberon_3.0.10.lib``

nrf_oberon - 3.0.9
******************

New version of the nrf_oberon library with the following changes.

Added
=====

* Added PSA API support.

Added the following Oberon crypto libraries for nRF9160, nRF53, nRF52, and nRF51 architectures.

.. note::
   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_oberon, nRF9160, and nRF53 application core variants

  * ``cortex-m33/hard-float/liboberon_3.0.9.a``
  * ``cortex-m33/soft-float/liboberon_3.0.9.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/liboberon_3.0.9.a``
    * ``cortex-m33/soft-float/short-wchar/liboberon_3.0.9.a``

  * Keil

    * ``cortex-m33/hard-float/short-wchar/oberon_3.0.9.lib``
    * ``cortex-m33/soft-float/short-wchar/oberon_3.0.9.lib``

* nrf_oberon, nrf53 network core variants

  * ``cortex-m33+nodsp/soft-float/liboberon_3.0.9.a``

  * short-wchar

    * ``cortex-m33+nodsp/soft-float/short-wchar/liboberon_3.0.9.a``

  * Keil

    * ``cortex-m33/soft-float/short-wchar/oberon_3.0.9.lib``

* nrf_oberon, nRF52 variants

  * ``cortex-m4/hard-float/liboberon_3.0.9.a``
  * ``cortex-m4/soft-float/liboberon_3.0.9.a``

  * short-wchar

    * ``cortex-m4/hard-float/short-wchar/liboberon_3.0.9.a``
    * ``cortex-m4/soft-float/short-wchar/liboberon_3.0.9.a``

  * Keil

    * ``cortex-m4/soft-float/short-wchar/oberon_3.0.9.lib``
    * ``cortex-m4/hard-float/short-wchar/oberon_3.0.9.lib``

* nrf_oberon, nRF51 variants

  * ``cortex-m0/soft-float/liboberon_3.0.9.a``

  * short-wchar

    * ``cortex-m0/soft-float/short-wchar/liboberon_3.0.9.a``

  * Keil

    * ``cortex-m0/soft-float/short-wchar/oberon_3.0.9.lib``


nrf_oberon - 3.0.8
******************

New version of the nrf_oberon library with the following changes.

Added
=====

* Added APIs for doing ECDH calculation using secp256r1 in incremental steps.
* Added ``ocrypto_`` APIs for SHA-224 and SHA-384.
* Added ``ocrypto_`` APIs for pbkdf2 for SHA-1 and SHA-256.

Added the following Oberon crypto libraries for nRF9160, nRF53, nRF52, and nRF51 architectures.

.. note::
   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_oberon, nRF9160 and nRF53 application core variants

  * ``cortex-m33/hard-float/liboberon_3.0.8.a``
  * ``cortex-m33/soft-float/liboberon_3.0.8.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/liboberon_3.0.8.a``
    * ``cortex-m33/soft-float/short-wchar/liboberon_3.0.8.a``

  * Keil

    * ``cortex-m33/hard-float/short-wchar/oberon_3.0.8.lib``
    * ``cortex-m33/soft-float/short-wchar/oberon_3.0.8.lib``

* nrf_oberon, nrf53 network core variants

  * ``cortex-m33+nodsp/soft-float/liboberon_3.0.8.a``

  * short-wchar

    * ``cortex-m33+nodsp/soft-float/short-wchar/liboberon_3.0.8.a``

  * Keil

    * ``cortex-m33/soft-float/short-wchar/oberon_3.0.8.lib``

* nrf_oberon, nRF52 variants

  * ``cortex-m4/hard-float/liboberon_3.0.8.a``
  * ``cortex-m4/soft-float/liboberon_3.0.8.a``

  * short-wchar

    * ``cortex-m4/hard-float/short-wchar/liboberon_3.0.8.a``
    * ``cortex-m4/soft-float/short-wchar/liboberon_3.0.8.a``

  * Keil

    * ``cortex-m4/soft-float/short-wchar/oberon_3.0.8.lib``
    * ``cortex-m4/hard-float/short-wchar/oberon_3.0.8.lib``

* nrf_oberon, nRF51 variants

  * ``cortex-m0/soft-float/liboberon_3.0.8.a``

  * short-wchar

    * ``cortex-m0/soft-float/short-wchar/liboberon_3.0.8.a``

  * Keil

    * ``cortex-m0/soft-float/short-wchar/oberon_3.0.8.lib``

nrf_oberon - 3.0.7
******************

New version of the nrf_oberon library with the following changes.

Added
=====

Added the following header files with ocrypto APIs

* ``include/ocrypto_ecdh_p224.h``
* ``include/ocrypto_ecdsa_p224.h``

Added the following header files with Mbed TLS alternate APIs

* ``include/mbedtls/chacha20_alt.h``
* ``include/mbedtls/poly1305_alt.h``

Added library-internal symbols for Mbed TLS alternate APIs

* ECDSA generate key, sign, and verify (secp224r1, secp256r1, curve25519)
* ECDH generate key, compute shared secret (secp224r1, secp256r1, curve25519)

Added the following Oberon crypto libraries for nRF9160, nRF53, nRF52, and nRF51 architectures.

.. note::
   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_oberon, nRF9160 and nRF53 application core variants

  * ``cortex-m33/hard-float/liboberon_3.0.7.a``
  * ``cortex-m33/soft-float/liboberon_3.0.7.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/liboberon_3.0.7.a``
    * ``cortex-m33/soft-float/short-wchar/liboberon_3.0.7.a``

  * Keil

    * ``cortex-m33/hard-float/short-wchar/oberon_3.0.7.lib``
    * ``cortex-m33/soft-float/short-wchar/oberon_3.0.7.lib``

* nrf_oberon, nrf53 network core variants

  * ``cortex-m33+nodsp/soft-float/liboberon_3.0.7.a``

  * short-wchar

    * ``cortex-m33+nodsp/soft-float/short-wchar/liboberon_3.0.7.a``

  * Keil

    * ``cortex-m33/soft-float/short-wchar/oberon_3.0.7.lib``

* nrf_oberon, nRF52 variants

  * ``cortex-m4/hard-float/liboberon_3.0.7.a``
  * ``cortex-m4/soft-float/liboberon_3.0.7.a``

  * short-wchar

    * ``cortex-m4/hard-float/short-wchar/liboberon_3.0.7.a``
    * ``cortex-m4/soft-float/short-wchar/liboberon_3.0.7.a``

  * Keil

    * ``cortex-m4/soft-float/short-wchar/oberon_3.0.7.lib``
    * ``cortex-m4/hard-float/short-wchar/oberon_3.0.7.lib``

* nrf_oberon, nRF51 variants

  * ``cortex-m0/soft-float/liboberon_3.0.7.a``

  * short-wchar

    * ``cortex-m0/soft-float/short-wchar/liboberon_3.0.7.a``

  * Keil

    * ``cortex-m0/soft-float/short-wchar/oberon_3.0.7.lib``

nrf_oberon - 3.0.5
******************

Added
=====

Added the following header files with ocrypto APIs

* ``include/ocrypto_aes_cbc.h``
* ``include/ocrypto_aes_ccm.h``
* ``include/ocrypto_aes_cmac.h``
* ``include/ocrypto_ecjpake_p256.h``
* ``include/ocrypto_hkdf_sha1.h``
* ``include/ocrypto_hmac_sha1.h``

Added the following header files with Mbed TLS alternate APIs

* ``include/mbedtls/ecjpake_alt.h``
* ``include/mbedtls/sha1_alt.h``
* ``include/mbedtls/sha256_alt.h``

Added library-internal symbols for Mbed TLS alternate APIs

* ECDSA generate key, sign, and verify (secp256r1)
* ECDH generate key, compute shared secret (secp256r1)

Added the following Oberon crypto libraries for nRF9160, nRF53, nRF52, and nRF51 architectures.

.. note::
   short-wchar: Those libraries are compiled with a wchar_t size of 16 bits.

* nrf_oberon, nRF9160 and nRF53 application core variants

  * ``cortex-m33/hard-float/liboberon_3.0.5.a``
  * ``cortex-m33/soft-float/liboberon_3.0.5.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/liboberon_3.0.5.a``
    * ``cortex-m33/soft-float/short-wchar/liboberon_3.0.5.a``

  * Keil

    * ``cortex-m33/hard-float/short-wchar/oberon_3.0.5.lib``
    * ``cortex-m33/soft-float/short-wchar/oberon_3.0.5.lib``

* nrf_oberon, nrf53 network core variants

  * ``cortex-m33+nodsp/soft-float/liboberon_3.0.5.a``

  * short-wchar

    * ``cortex-m33+nodsp/soft-float/short-wchar/liboberon_3.0.5.a``

  * Keil

    * ``cortex-m33/soft-float/short-wchar/oberon_3.0.5.lib``

* nrf_oberon, nRF52 variants

  * ``cortex-m4/hard-float/liboberon_3.0.5.a``
  * ``cortex-m4/soft-float/liboberon_3.0.5.a``

  * short-wchar

    * ``cortex-m4/hard-float/short-wchar/liboberon_3.0.5.a``
    * ``cortex-m4/soft-float/short-wchar/liboberon_3.0.5.a``

  * Keil

    * ``cortex-m4/soft-float/short-wchar/oberon_3.0.5.lib``
    * ``cortex-m4/hard-float/short-wchar/oberon_3.0.5.lib``

* nrf_oberon, nRF51 variants

  * ``cortex-m0/soft-float/liboberon_3.0.5.a``

  * short-wchar

    * ``cortex-m0/soft-float/short-wchar/liboberon_3.0.5.a``

  * Keil

    * ``cortex-m0/soft-float/short-wchar/oberon_3.0.5.lib``

Removed
=======


nrf_oberon - 3.0.3
******************

Added
=====

Added the following Oberon crypto libraries for nRF9160, nRF52, and nRF51 architectures.

Added Oberon ocrypto_poly1305.h and ocrypto_sc_p256.h headers.

.. note::
   short-wchar: Those libraries are compiled with a wchar_t size of 16 bits.


* nrf_oberon, nRF9160 variants

  * ``cortex-m33/hard-float/liboberon_3.0.3.a``
  * ``cortex-m33/soft-float/liboberon_3.0.3.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/liboberon_3.0.3.a``
    * ``cortex-m33/soft-float/short-wchar/liboberon_3.0.3.a``

  * Keil

    * ``cortex-m33/hard-float/short-wchar/oberon_3.0.3.lib``
    * ``cortex-m33/soft-float/short-wchar/oberon_3.0.3.lib``

* nrf_oberon, nRF52 variants

  * ``cortex-m4/hard-float/liboberon_3.0.3.a``
  * ``cortex-m4/soft-float/liboberon_3.0.3.a``

  * short-wchar

    * ``cortex-m4/hard-float/short-wchar/liboberon_3.0.3.a``
    * ``cortex-m4/soft-float/short-wchar/liboberon_3.0.3.a``

  * Keil

    * ``cortex-m4/soft-float/short-wchar/oberon_3.0.3.lib``
    * ``cortex-m4/hard-float/short-wchar/oberon_3.0.3.lib``

* nrf_oberon, nRF51 variants

  * ``cortex-m0/soft-float/liboberon_3.0.3.a``

  * short-wchar

    * ``cortex-m0/soft-float/short-wchar/liboberon_3.0.3.a``

  * Keil

    * ``cortex-m0/soft-float/short-wchar/oberon_3.0.3.lib``


Removed
=======

* All 3.0.2 versions of the library and old include files


nrf_oberon - 3.0.2
******************

Added
=====

Added the following Oberon crypto libraries for nRF9160, nRF52, and nRF51 architectures.

Added Oberon SRP, Secure Remote Password, ocrypto_srp functions.

.. note::
   short-wchar: Those libraries are compiled with a wchar_t size of 16 bits.


* nrf_oberon, nRF9160 variants

  * ``cortex-m33/hard-float/liboberon_3.0.2.a``
  * ``cortex-m33/soft-float/liboberon_3.0.2.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/liboberon_3.0.2.a``
    * ``cortex-m33/soft-float/short-wchar/liboberon_3.0.2.a``

  * Keil

    * ``cortex-m33/hard-float/short-wchar/oberon_3.0.2.lib``
    * ``cortex-m33/soft-float/short-wchar/oberon_3.0.2.lib``

* nrf_oberon, nRF52 variants

  * ``cortex-m4/hard-float/liboberon_3.0.2.a``
  * ``cortex-m4/soft-float/liboberon_3.0.2.a``

  * short-wchar

    * ``cortex-m4/hard-float/short-wchar/liboberon_3.0.2.a``
    * ``cortex-m4/soft-float/short-wchar/liboberon_3.0.2.a``

  * Keil

    * ``cortex-m4/soft-float/short-wchar/oberon_3.0.2.lib``
    * ``cortex-m4/hard-float/short-wchar/oberon_3.0.2.lib``

* nrf_oberon, nRF51 variants

  * ``cortex-m0/soft-float/liboberon_3.0.2.a``

  * short-wchar

    * ``cortex-m0/soft-float/short-wchar/liboberon_3.0.2.a``

  * Keil

    * ``cortex-m0/soft-float/short-wchar/oberon_3.0.2.lib``


Removed
=======

* All 3.0.0 versions of the library and old include files


nrf_oberon - 3.0.0
******************

Added
=====

Added the following Oberon crypto libraries for nRF9160, nRF52, and nRF51 architectures.

.. note::
   The include files and APIs have changed the prefix from ``occ_`` to ``ocrypto_``.

.. note::
   short-wchar: Those libraries are compiled with a wchar_t size of 16 bits.


* nrf_oberon, nRF9160 variants

  * ``cortex-m33/hard-float/liboberon_3.0.0.a``
  * ``cortex-m33/soft-float/liboberon_3.0.0.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/liboberon_3.0.0.a``
    * ``cortex-m33/soft-float/short-wchar/liboberon_3.0.0.a``

  * Keil

    * ``cortex-m33/hard-float/short-wchar/oberon_3.0.0.lib``
    * ``cortex-m33/soft-float/short-wchar/oberon_3.0.0.lib``

* nrf_oberon, nRF52 variants

  * ``cortex-m4/hard-float/liboberon_3.0.0.a``
  * ``cortex-m4/soft-float/liboberon_3.0.0.a``

  * short-wchar

    * ``cortex-m4/hard-float/short-wchar/liboberon_3.0.0.a``
    * ``cortex-m4/soft-float/short-wchar/liboberon_3.0.0.a``

  * Keil

    * ``cortex-m4/soft-float/short-wchar/oberon_3.0.0.lib``
    * ``cortex-m4/hard-float/short-wchar/oberon_3.0.0.lib``

* nrf_oberon, nRF51 variants

  * ``cortex-m0/soft-float/liboberon_3.0.0.a``

  * short-wchar

    * ``cortex-m0/soft-float/short-wchar/liboberon_3.0.0.a``

  * Keil

    * ``cortex-m0/soft-float/short-wchar/oberon_3.0.0.lib``


Removed
=======

* All 2.0.7 versions of the library and old include files


nrf_oberon - 2.0.7
******************

Initial release.

Added
=====

Added the following Oberon crypto libraries for nRF9160, nRF52, and nRF51 architectures.

.. note::
   short-wchar: Those libraries are compiled with a wchar_t size of 16 bits.

* nrf_oberon, nrf9160 variants

  * ``cortex-m33/hard-float/liboberon_2.0.7.a``
  * ``cortex-m33/soft-float/liboberon_2.0.7.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/liboberon_2.0.7.a``
    * ``cortex-m33/soft-float/short-wchar/liboberon_2.0.7.a``

  * Keil

    * ``cortex-m33/hard-float/short-wchar/oberon_2.0.7.lib``
    * ``cortex-m33/soft-float/short-wchar/oberon_2.0.7.lib``

* nrf_oberon, nrf52 variants

  * ``cortex-m4/hard-float/liboberon_2.0.7.a``
  * ``cortex-m4/soft-float/liboberon_2.0.7.a``

  * short-wchar

    * ``cortex-m4/hard-float/short-wchar/liboberon_2.0.7.a``
    * ``cortex-m4/soft-float/short-wchar/liboberon_2.0.7.a``

  * Keil

    * ``cortex-m4/soft-float/short-wchar/oberon_2.0.7.lib``
    * ``cortex-m4/hard-float/short-wchar/oberon_2.0.7.lib``

* nrf_oberon, nrf51 variants

  * ``cortex-m0/soft-float/liboberon_2.0.7.a``

  * short-wchar

    * ``cortex-m0/soft-float/short-wchar/liboberon_2.0.7.a``

  * Keil

    * ``cortex-m0/soft-float/short-wchar/oberon_2.0.7.lib``
