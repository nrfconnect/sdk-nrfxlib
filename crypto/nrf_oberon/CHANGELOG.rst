.. _crypto_changelog_oberon:

Changelog - nRF Oberon
######################

All notable changes to this project are documented in this file.

nrf_oberon - 3.0.5

Added
=====

Added the following header files with ocrypto APIs

* ``include/ocrypto_aes_cbc.h``
* ``include/ocrypto_aes_ccm.h``
* ``include/ocrypto_aes_cmac.h``
* ``include/ocrypto_ecjpake_p256.h``
* ``include/ocrypto_hkdf_sha1.h``
* ``include/ocrypto_hmac_sha1.h``

Added the following header files with mbed TLS alternate APIs

* ``include/mbedtls/ecjpake_alt.h``
* ``include/mbedtls/sha1_alt.h``
* ``include/mbedtls/sha256_alt.h``

Added library-internal symbols for mbed TLS alternate APIs

* ECDSA generate key, sign, and verify (secp256r1)
* ECDH generate key, compute shared secret (secp256r1)

Added the following Oberon crypto libraries for nRF9160, nRF53, nRF52, and nRF51 architectures.

.. note::
   short-wchar: Those libraries are compiled with a wchar_t size of 16 bits.

* nrf_oberon, nRF9160 and nRF53 app core variants

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
