.. _crypto_changelog_nrf_cc3xx_platform:

Changelog - nrf_cc3xx_platform
##############################

.. contents::
   :local:
   :depth: 2

All notable changes to this project are documented in this file.

nrf_cc3xx_platform - 0.9.17
***************************

New version of the library with the following features:

* Removed mbedtls_hardware_poll limitation on buffer size, it now accepts any buffer size.
* Removed Mbed TLS platform functions from library.

Library built against Mbed TLS version 3.3.0.

Added
=====

Added a new build of nrf_cc3xx_platform libraries for nRF9160, nRF52840, and nRF5340.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

+* nrf_cc312_platform, nRF5340 variants
+  * ``crypto/nrf_cc312_platform/lib/cortex-m33/**/libnrf_cc312_psa_crypto_0.9.17.a``
+
+* nrf_cc310_platform, nRF9160 variants
+  * ``crypto/nrf_cc310_platform/lib/cortex-m33/**/libnrf_cc310_platform_0.9.17.a``
+
+* nrf_cc310_platform, nRF52840 variants
+  * ``crypto/nrf_cc310_platform/lib/cortex-m4/**/libnrf_cc310_platform_0.9.17.a``

nrf_cc3xx_platform - 0.9.16
***************************

New version of the library with the following features:

* Added code-size optimized API for SHA-256 that has no limitation on where the input is stored (flash/RAM).
* Fixed a bug where platform mutexes ended up unallocated if a context holding them was not zeroized.

Library built against Mbed TLS version 3.1.0.

Added
=====

Added a new build of nrf_cc3xx_platform libraries for nRF9160, nRF52840, and nRF5340.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

+* nrf_cc312_platform, nRF5340 variants
+  * ``crypto/nrf_cc312_platform/lib/cortex-m33/**/libnrf_cc312_psa_crypto_0.9.16.a``
+
+* nrf_cc310_platform, nRF9160 variants
+  * ``crypto/nrf_cc310_platform/lib/cortex-m33/**/libnrf_cc310_platform_0.9.16.a``
+
+* nrf_cc310_mbedcrypto, nRF52840 variants
+  * ``crypto/nrf_cc310_platform/lib/cortex-m4/**/libnrf_cc310_platform_0.9.16.a``


nrf_cc3xx_platform - 0.9.15
***************************

New version of the library with the following features:

* Added new shadow key APIs for key derivation using KMU, which is compatible with multi-part operations.
* Ensured that random seeds (for EITS and attestation) are generated only once during boot.
* Minor bug fixes and improvements.

Library built against Mbed TLS version 3.1.0.

Added
=====

Added a new build of nrf_cc3xx_platform libraries for nRF9160, nRF52840, and nRF5340.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc312_platform, nRF5340 variants

  * ``cortex-m33/hard-float/libnrf_cc312_platform_0.9.15.a``
  * ``cortex-m33/soft-float/libnrf_cc312_platform_0.9.15.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc312_platform_0.9.15.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc312_platform_0.9.15.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc312_platform_0.9.15.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc312_platform_0.9.15.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.15.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.15.a``


* nrf_cc310_platform, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_platform_0.9.15.a``
  * ``cortex-m33/soft-float/libnrf_cc310_platform_0.9.15.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_platform_0.9.15.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_platform_0.9.15.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_platform_0.9.15.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_platform_0.9.15.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.15.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.15.a``

* nrf_cc310_platform, nRF52840 variants

  * ``cortex-m4/soft-float/libnrf_cc310_platform_0.9.15.a``
  * ``cortex-m4/hard-float/libnrf_cc310_platform_0.9.15.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_platform_0.9.15.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_platform_0.9.15.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_platform_0.9.15.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_platform_0.9.15.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.15.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.15.a``

nrf_cc3xx_platform - 0.9.14
***************************

New version of the library with the following features:

* Internal restructure of the library.
* Added APIs for storing encrypted identity key in KMU.
* Added APIs for retrieving boot generated RNG seed and nonce seed.

Library built against Mbed TLS version 3.0.0.

Added
=====

Added a new build of nrf_cc3xx_platform libraries for nRF9160, nRF52840, and nRF5340.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc312_platform, nRF5340 variants

  * ``cortex-m33/hard-float/libnrf_cc312_platform_0.9.14.a``
  * ``cortex-m33/soft-float/libnrf_cc312_platform_0.9.14.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc312_platform_0.9.14.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc312_platform_0.9.14.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc312_platform_0.9.14.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc312_platform_0.9.14.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.14.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.14.a``


* nrf_cc310_platform, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_platform_0.9.14.a``
  * ``cortex-m33/soft-float/libnrf_cc310_platform_0.9.14.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_platform_0.9.14.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_platform_0.9.14.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_platform_0.9.14.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_platform_0.9.14.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.14.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.14.a``

* nrf_cc310_platform, nRF52840 variants

  * ``cortex-m4/soft-float/libnrf_cc310_platform_0.9.14.a``
  * ``cortex-m4/hard-float/libnrf_cc310_platform_0.9.14.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_platform_0.9.14.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_platform_0.9.14.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_platform_0.9.14.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_platform_0.9.14.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.14.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.14.a``


nrf_cc3xx_platform - 0.9.13
***************************

New version of the library with the following features:

* Internal restructure of the library.
* Updated to the Mbed TLS version 3.0.0.

Library built against Mbed TLS version 3.0.0.

Added
=====

Added a new build of nrf_cc3xx_platform libraries for nRF9160, nRF52840, and nRF5340.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc312_platform, nRF5340 variants

  * ``cortex-m33/hard-float/libnrf_cc312_platform_0.9.13.a``
  * ``cortex-m33/soft-float/libnrf_cc312_platform_0.9.13.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc312_platform_0.9.13.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc312_platform_0.9.13.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc312_platform_0.9.13.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc312_platform_0.9.13.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.13.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.13.a``


* nrf_cc310_platform, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_platform_0.9.13.a``
  * ``cortex-m33/soft-float/libnrf_cc310_platform_0.9.13.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_platform_0.9.13.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_platform_0.9.13.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_platform_0.9.13.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_platform_0.9.13.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.13.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.13.a``

* nrf_cc310_platform, nRF52840 variants

  * ``cortex-m4/soft-float/libnrf_cc310_platform_0.9.13.a``
  * ``cortex-m4/hard-float/libnrf_cc310_platform_0.9.13.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_platform_0.9.13.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_platform_0.9.13.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_platform_0.9.13.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_platform_0.9.13.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.13.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.13.a``

nrf_cc3xx_platform - 0.9.12
***************************

New version of the library with bug fixes and added features:

* Fixed issue with KMU loading for nRF9160 devices.
  The issue is only present in certain builds, but it is highly recommended to update to this version of the library if you are using nRF9160.

Library built against Mbed TLS version 2.26.0.

Added
=====

Added a new build of nrf_cc3xx_platform libraries for nRF9160, nRF52840, and nRF5340.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc312_platform, nRF5340 variants

  * ``cortex-m33/hard-float/libnrf_cc312_platform_0.9.12.a``
  * ``cortex-m33/soft-float/libnrf_cc312_platform_0.9.12.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc312_platform_0.9.12.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc312_platform_0.9.12.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc312_platform_0.9.12.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc312_platform_0.9.12.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.12.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.12.a``


* nrf_cc310_platform, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_platform_0.9.12.a``
  * ``cortex-m33/soft-float/libnrf_cc310_platform_0.9.12.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_platform_0.9.12.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_platform_0.9.12.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_platform_0.9.12.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_platform_0.9.12.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.12.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.12.a``


* nrf_cc310_platform, nRF52840 variants

  * ``cortex-m4/soft-float/libnrf_cc310_platform_0.9.12.a``
  * ``cortex-m4/hard-float/libnrf_cc310_platform_0.9.12.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_platform_0.9.12.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_platform_0.9.12.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_platform_0.9.12.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_platform_0.9.12.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.12.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.12.a``


nrf_cc3xx_platform - 0.9.11
***************************

New version of the library with bug fixes and added features:

* Modified the KMU APIs to remove the reservation of slots 0 and 1.
  These slots can be used freely now.
* Fixed an issue where the global CTR_DRBG context would get stuck
  when it reached the reseed interval.
* Fixed an issue where building with the derived key APIs would not be possible.

Library built against Mbed TLS version 2.26.0.

Added
=====

Added a new build of nrf_cc3xx_platform libraries for nRF9160, nRF52840, and nRF5340.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc312_platform, nRF5340 variants

  * ``cortex-m33/hard-float/libnrf_cc312_platform_0.9.11.a``
  * ``cortex-m33/soft-float/libnrf_cc312_platform_0.9.11.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc312_platform_0.9.11.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc312_platform_0.9.11.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc312_platform_0.9.11.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc312_platform_0.9.11.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.11.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.11.a``


* nrf_cc310_platform, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_platform_0.9.11.a``
  * ``cortex-m33/soft-float/libnrf_cc310_platform_0.9.11.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_platform_0.9.11.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_platform_0.9.11.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_platform_0.9.11.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_platform_0.9.11.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.11.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.11.a``


* nrf_cc310_platform, nRF52840 variants

  * ``cortex-m4/soft-float/libnrf_cc310_platform_0.9.11.a``
  * ``cortex-m4/hard-float/libnrf_cc310_platform_0.9.11.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_platform_0.9.11.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_platform_0.9.11.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_platform_0.9.11.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_platform_0.9.11.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.11.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.11.a``


nrf_cc3xx_platform - 0.9.10
***************************

New version of the library with an added feature:

* Added HMAC_DRBG APIs in :file:`nrf_cc3xx_platform_hmac_drbg.h`

Library built against Mbed TLS version 2.26.0.

Added
=====

Added a new build of nrf_cc3xx_platform libraries for nRF9160, nRF52840, and nRF5340.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc312_platform, nRF5340 variants

  * ``cortex-m33/hard-float/libnrf_cc312_platform_0.9.10.a``
  * ``cortex-m33/soft-float/libnrf_cc312_platform_0.9.10.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc312_platform_0.9.10.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc312_platform_0.9.10.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc312_platform_0.9.10.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc312_platform_0.9.10.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.10.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.10.a``


* nrf_cc310_platform, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_platform_0.9.10.a``
  * ``cortex-m33/soft-float/libnrf_cc310_platform_0.9.10.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_platform_0.9.10.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_platform_0.9.10.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_platform_0.9.10.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_platform_0.9.10.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.10.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.10.a``


* nrf_cc310_platform, nRF52840 variants

  * ``cortex-m4/soft-float/libnrf_cc310_platform_0.9.10.a``
  * ``cortex-m4/hard-float/libnrf_cc310_platform_0.9.10.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_platform_0.9.10.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_platform_0.9.10.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_platform_0.9.10.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_platform_0.9.10.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.10.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.10.a``


nrf_cc3xx_platform - 0.9.9
**************************

New version of the library with bug fixes and added features:

* Added support for using ChaCha20 keys directly from the KMU on nRF9160 and nRF5340 devices.
* Added APIs for key derivation in nrf_cc3xx_platform_derived_key.h.
* Added support for using derived keys for ChaCha20 encryption/decryption.
* Modified CTR_DRBG APIs to use internal context when the context argument is NULL.
* Added new API for storing keys in the KMU.

Library built against Mbed TLS version 2.25.0.

Added
=====

Added a new build of nrf_cc3xx_platform libraries for nRF9160, nRF52840, and nRF5340.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc312_platform, nRF5340 variants

  * ``cortex-m33/hard-float/libnrf_cc312_platform_0.9.9.a``
  * ``cortex-m33/soft-float/libnrf_cc312_platform_0.9.9.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc312_platform_0.9.9.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc312_platform_0.9.9.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc312_platform_0.9.9.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc312_platform_0.9.9.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.9.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.9.a``


* nrf_cc310_platform, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_platform_0.9.9.a``
  * ``cortex-m33/soft-float/libnrf_cc310_platform_0.9.9.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_platform_0.9.9.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_platform_0.9.9.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_platform_0.9.9.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_platform_0.9.9.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.9.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.9.a``


* nrf_cc310_platform, nRF52840 variants

  * ``cortex-m4/soft-float/libnrf_cc310_platform_0.9.9.a``
  * ``cortex-m4/hard-float/libnrf_cc310_platform_0.9.9.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_platform_0.9.9.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_platform_0.9.9.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_platform_0.9.9.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_platform_0.9.9.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.9.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.9.a``


nrf_cc3xx_platform - 0.9.8
**************************

New version of the library with improvements and bug fixes:

* Decreased stack usage for PRNG using `CTR_DRBG`.
* Fixed issue with `CTR_DRBG` usage on the platform library when multiple backends are enabled in nordic security.
* Fixed issues in the entropy module.
* Added APIs for key derivation in nrf_cc3xx_platform_kmu.h

Library built against Mbed TLS version 2.24.0.

Added
=====

Added a new build of nrf_cc3xx_platform libraries for nRF9160, nRF52840, and nRF5340.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc312_platform, nRF5340 variants

  * ``cortex-m33/hard-float/libnrf_cc312_platform_0.9.8.a``
  * ``cortex-m33/soft-float/libnrf_cc312_platform_0.9.8.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc312_platform_0.9.8.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc312_platform_0.9.8.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc312_platform_0.9.8.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc312_platform_0.9.8.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.8.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.8.a``


* nrf_cc310_platform, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_platform_0.9.8.a``
  * ``cortex-m33/soft-float/libnrf_cc310_platform_0.9.8.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_platform_0.9.8.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_platform_0.9.8.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_platform_0.9.8.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_platform_0.9.8.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.8.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.8.a``


* nrf_cc310_platform, nRF52840 variants

  * ``cortex-m4/soft-float/libnrf_cc310_platform_0.9.8.a``
  * ``cortex-m4/hard-float/libnrf_cc310_platform_0.9.8.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_platform_0.9.8.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_platform_0.9.8.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_platform_0.9.8.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_platform_0.9.8.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.8.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.8.a``


nrf_cc3xx_platform - 0.9.7
**************************

New version of the library with a bug fix:

* Fixed an issue with mutex slab allocation in Zephyr RTOS platform file.

Library built against Mbed TLS version 2.24.0.

Added
=====

Added a new build of nrf_cc3xx_platform libraries for nRF9160, nRF52840, and nRF5340.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc312_platform, nRF5340 variants

  * ``cortex-m33/hard-float/libnrf_cc312_platform_0.9.7.a``
  * ``cortex-m33/soft-float/libnrf_cc312_platform_0.9.7.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc312_platform_0.9.7.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc312_platform_0.9.7.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc312_platform_0.9.7.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc312_platform_0.9.7.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.7.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.7.a``


* nrf_cc310_platform, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_platform_0.9.7.a``
  * ``cortex-m33/soft-float/libnrf_cc310_platform_0.9.7.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_platform_0.9.7.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_platform_0.9.7.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_platform_0.9.7.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_platform_0.9.7.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.7.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.7.a``


* nrf_cc310_platform, nRF52840 variants

  * ``cortex-m4/soft-float/libnrf_cc310_platform_0.9.7.a``
  * ``cortex-m4/hard-float/libnrf_cc310_platform_0.9.7.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_platform_0.9.7.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_platform_0.9.7.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_platform_0.9.7.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_platform_0.9.7.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.7.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.7.a``


nrf_cc3xx_platform - 0.9.6
**************************

New version of the library with Mbed TLS sources ctr_drbg.c and entropy.c built in
Library built against Mbed TLS version 2.24.0.

Added
=====

Added a new build of nrf_cc3xx_platform libraries for nRF9160, nRF52840, and nRF5340.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc312_platform, nRF5340 variants

  * ``cortex-m33/hard-float/libnrf_cc312_platform_0.9.6.a``
  * ``cortex-m33/soft-float/libnrf_cc312_platform_0.9.6.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc312_platform_0.9.6.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc312_platform_0.9.6.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc312_platform_0.9.6.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc312_platform_0.9.6.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.6.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.6.a``


* nrf_cc310_platform, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_platform_0.9.6.a``
  * ``cortex-m33/soft-float/libnrf_cc310_platform_0.9.6.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_platform_0.9.6.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_platform_0.9.6.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_platform_0.9.6.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_platform_0.9.6.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.6.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.6.a``


* nrf_cc310_platform, nRF52840 variants

  * ``cortex-m4/soft-float/libnrf_cc310_platform_0.9.6.a``
  * ``cortex-m4/hard-float/libnrf_cc310_platform_0.9.6.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_platform_0.9.6.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_platform_0.9.6.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_platform_0.9.6.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_platform_0.9.6.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.6.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.6.a``


nrf_cc3xx_platform - 0.9.5
**************************

Added correct TRNG categorization values for nRF5340 devices.

Added
=====

Added a new build of nrf_cc3xx_platform libraries for nRF9160, nRF52840, and nRF5340.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc312_platform, nRF5340 variants

  * ``cortex-m33/hard-float/libnrf_cc312_platform_0.9.5.a``
  * ``cortex-m33/soft-float/libnrf_cc312_platform_0.9.5.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc312_platform_0.9.5.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc312_platform_0.9.5.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc312_platform_0.9.5.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc312_platform_0.9.5.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.5.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.5.a``


* nrf_cc310_platform, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_platform_0.9.5.a``
  * ``cortex-m33/soft-float/libnrf_cc310_platform_0.9.5.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_platform_0.9.5.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_platform_0.9.5.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_platform_0.9.5.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_platform_0.9.5.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.5.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.5.a``


* nrf_cc310_platform, nRF52840 variants

  * ``cortex-m4/soft-float/libnrf_cc310_platform_0.9.5.a``
  * ``cortex-m4/hard-float/libnrf_cc310_platform_0.9.5.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_platform_0.9.5.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_platform_0.9.5.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_platform_0.9.5.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_platform_0.9.5.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.5.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.5.a``


nrf_cc3xx_platform - 0.9.4
**************************

Added API to push KMU slot 0 on nRF9160 devices into CryptoCell KDR registers.
See :file:`include/nrf_cc3xx_platform_kmu.h`.

Added API to load a key from an address into CryptoCell KDR registers on nRF52840 devices.
See :file:`include/nrf_cc3xx_platform_kmu.h`.

Added
=====

Added a new build of nrf_cc3xx_platform libraries for nRF9160, nRF52840, and nRF5340.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc312_platform, nRF5340 variants

  * ``cortex-m33/hard-float/libnrf_cc312_platform_0.9.4.a``
  * ``cortex-m33/soft-float/libnrf_cc312_platform_0.9.4.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc312_platform_0.9.4.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc312_platform_0.9.4.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc312_platform_0.9.4.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc312_platform_0.9.4.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.4.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.4.a``


* nrf_cc310_platform, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_platform_0.9.4.a``
  * ``cortex-m33/soft-float/libnrf_cc310_platform_0.9.4.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_platform_0.9.4.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_platform_0.9.4.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_platform_0.9.4.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_platform_0.9.4.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.4.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.4.a``


* nrf_cc310_platform, nRF52840 variants

  * ``cortex-m4/soft-float/libnrf_cc310_platform_0.9.4.a``
  * ``cortex-m4/hard-float/libnrf_cc310_platform_0.9.4.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_platform_0.9.4.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_platform_0.9.4.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_platform_0.9.4.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_platform_0.9.4.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.4.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.4.a``

nrf_cc3xx_platform - 0.9.3
**************************

Added experimental support for devices with Arm CryptoCell CC312 (nRF5340).

Changed name of configurations from CC310 to CC3XX. This is reflected in the header file and APIs as well, where nrf_cc310_xxxx is renamed to nrf_cc3xx_xxxx.

Added new version of libraries nrf_cc310_platform/nrf_cc312_platform built with Mbed TLS version 2.23.0.

Added APIs for storing keys in the KMU peripheral (nRF9160, nRF5340).
See :file:`include/nrf_cc3xx_platform_kmu.h`.

Added APIs for generating CSPRNG using CTR_DRBG.
See :file:`include/nrf_cc3xx_platform_ctr_drbg.h`.

This version also adds experimental support for interrupts in selected versions of the library (the libraries that do not support interrupts can be found in the ``no-interrupts`` folders).

Added
=====

Added a new build of nrf_cc3xx_platform libraries for nRF9160, nRF52840, and nRF5340.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc312_platform, nRF5340 variants

  * ``cortex-m33/hard-float/libnrf_cc312_platform_0.9.3.a``
  * ``cortex-m33/soft-float/libnrf_cc312_platform_0.9.3.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc312_platform_0.9.3.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc312_platform_0.9.3.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc312_platform_0.9.3.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc312_platform_0.9.3.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.3.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc312_platform_0.9.3.a``


* nrf_cc310_platform, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_platform_0.9.3.a``
  * ``cortex-m33/soft-float/libnrf_cc310_platform_0.9.3.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_platform_0.9.3.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_platform_0.9.3.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_platform_0.9.3.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_platform_0.9.3.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.3.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.3.a``


* nrf_cc310_platform, nRF52840 variants

  * ``cortex-m4/soft-float/libnrf_cc310_platform_0.9.3.a``
  * ``cortex-m4/hard-float/libnrf_cc310_platform_0.9.3.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_platform_0.9.3.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_platform_0.9.3.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_platform_0.9.3.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_platform_0.9.3.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.3.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.3.a``


nrf_cc310_platform - 0.9.2
**************************

New version of nrf_cc310_platform library fixing power management issues with pending interrupts.

This version also adds experimental support for interrupts in selected versions of the library (the libraries that do not support interrupts can be found in the ``no-interrupts`` folders).

This version must match the version of nrf_cc310_mbedcrypto if it is also used.

Added
=====

Added a new build of nrf_cc310_platform library for nRF9160 and nRF52 architectures.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc310_platform, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_platform_0.9.2.a``
  * ``cortex-m33/soft-float/libnrf_cc310_platform_0.9.2.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_platform_0.9.2.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_platform_0.9.2.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_platform_0.9.2.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_platform_0.9.2.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.2.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.2.a``

* nrf_cc310_platform, nRF52 variants

  * ``cortex-m4/soft-float/libnrf_cc310_platform_0.9.2.a``
  * ``cortex-m4/hard-float/libnrf_cc310_platform_0.9.2.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_platform_0.9.2.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_platform_0.9.2.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_platform_0.9.2.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_platform_0.9.2.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.2.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.2.a``


nrf_cc310_platform - 0.9.1
**************************

New version of nrf_cc310_platform library containing Arm CC310 hardware initialization and entropy gathering APIs.

Added to match with the nrf_cc310_mbedcrypto v0.9.1 library.

.. note::

    The library version must match with nrf_cc310_mbedcrypto if this is also
    used

Added
=====

Added a new build of nrf_cc310_platform library for nRF9160 and nRF52 architectures.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc310_platform, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_platform_0.9.1.a``
  * ``cortex-m33/soft-float/libnrf_cc310_platform_0.9.1.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_platform_0.9.1.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_platform_0.9.1.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_platform_0.9.1.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_platform_0.9.1.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.1.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.1.a``

* nrf_cc310_platform, nRF52 variants

  * ``cortex-m4/soft-float/libnrf_cc310_platform_0.9.1.a``
  * ``cortex-m4/hard-float/libnrf_cc310_platform_0.9.1.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_platform_0.9.1.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_platform_0.9.1.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_platform_0.9.1.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_platform_0.9.1.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.1.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.1.a``


nrf_cc310_platform - 0.9.0
**************************

Initial, experimental release of nrf_cc310_platform library containing Arm CC310 hardware initialization and entropy gathering APIs.

The library also contains APIs and companion source-files to setup RTOS dependent mutex and abort functionality for the nrf_cc310_mbedcrypto library in Zephyr RTOS and FreeRTOS.

.. note::

    The library version must match with nrf_cc310_mbedcrypto if this is also
    used

Added
=====

Added a new build of nrf_cc310_platform library for nRF9160 and nRF52 architectures.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc310_platform, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_platform_0.9.0.a``
  * ``cortex-m33/soft-float/libnrf_cc310_platform_0.9.0.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_platform_0.9.0.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_platform_0.9.0.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_platform_0.9.0.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_platform_0.9.0.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.0.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.0.a``

* nrf_cc310_platform, nRF52 variants

  * ``cortex-m4/soft-float/libnrf_cc310_platform_0.9.0.a``
  * ``cortex-m4/hard-float/libnrf_cc310_platform_0.9.0.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_platform_0.9.0.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_platform_0.9.0.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_platform_0.9.0.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_platform_0.9.0.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.0.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.0.a``
