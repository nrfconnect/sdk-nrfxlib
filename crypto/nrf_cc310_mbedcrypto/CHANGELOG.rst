.. _crypto_changelog_nrf_cc3xx_mbedcrypto:

Changelog - nrf_cc3xx_mbedcrypto
################################

.. contents::
   :local:
   :depth: 2

All notable changes to this project are documented in this file.

nrf_cc3xx_mbedcrypto - 0.9.7
****************************
New version of the runtime library with bugfixes:
* Fixed issues where mbedtls_rsa_complete was not able to deduce missing parameters.
* Fixed an issue with calculating the correct salt length for certain combinations of RSA key and digest sizes.
* Adding missing function mbedtls_ecp_write_key.

Library built against mbed TLS version 2.24.0.

This version is dependent on the nrf_cc310_platform or nrf_cc312_platform library for low-level initialization of the system and proper RTOS integration.

Added
=====

Added a new build of nRF_cc3xx_mbedcrypto libraries for nRF9160, nRF52840, and nRF5340.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc312_mbedcrypto, nRF5340 variants

  * ``cortex-m33/hard-float/libnrf_cc312_mbedcrypto_0.9.7.a``
  * ``cortex-m33/soft-float/libnrf_cc312_mbedcrypto_0.9.7.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc312_mbedcrypto_0.9.7.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc312_mbedcrypto_0.9.7.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc312_mbedcrypto_0.9.7.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc312_mbedcrypto_0.9.7.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc312_mbedcrypto_0.9.7.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc312_mbedcrypto_0.9.7.a``


* nrf_cc310_mbedcrypto, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc312_mbedcrypto_0.9.7.a``
  * ``cortex-m33/soft-float/libnrf_cc310_mbedcrypto_0.9.7.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.7.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.7.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.7.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.7.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.7.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.7.a``


* nrf_cc310_mbedcrypto, nRF52840 variants

  * ``cortex-m4/soft-float/libnrf_cc310_mbedcrypto_0.9.7.a``
  * ``cortex-m4/hard-float/libnrf_cc310_mbedcrypto_0.9.7.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.7.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.7.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.7.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.7.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.7.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.7.a``


nrf_cc3xx_mbedcrypto - 0.9.6
****************************

New version of the runtime library fixing a regression in derived keys for ECB, CCM, and GCM
Library built against mbed TLS version 2.24.0.

This version is dependent on the nrf_cc310_platform or nrf_cc312_platform library for low-level initialization of the system and proper RTOS integration.

Added
=====

Added a new build of nRF_cc3xx_mbedcrypto libraries for nRF9160, nRF52840, and nRF5340.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc312_mbedcrypto, nRF5340 variants

  * ``cortex-m33/hard-float/libnrf_cc312_mbedcrypto_0.9.6.a``
  * ``cortex-m33/soft-float/libnrf_cc312_mbedcrypto_0.9.6.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc312_mbedcrypto_0.9.6.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc312_mbedcrypto_0.9.6.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc312_mbedcrypto_0.9.6.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc312_mbedcrypto_0.9.6.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc312_mbedcrypto_0.9.6.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc312_mbedcrypto_0.9.6.a``


* nrf_cc310_mbedcrypto, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc312_mbedcrypto_0.9.6.a``
  * ``cortex-m33/soft-float/libnrf_cc310_mbedcrypto_0.9.6.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.6.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.6.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.6.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.6.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.6.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.6.a``


* nrf_cc310_mbedcrypto, nRF52840 variants

  * ``cortex-m4/soft-float/libnrf_cc310_mbedcrypto_0.9.6.a``
  * ``cortex-m4/hard-float/libnrf_cc310_mbedcrypto_0.9.6.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.6.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.6.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.6.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.6.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.6.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.6.a``


nrf_cc3xx_mbedcrypto - 0.9.5
****************************

New version built against nrf_cc3xx_platform adding correct TRNG categorization for nRF5340 devices.

This version is dependent on the nrf_cc310_platform or nrf_cc312_platform library for low-level initialization of the system and proper RTOS integration.

Added
=====

Added anew build of nRF_cc3xx_mbedcrypto libraries for nRF9160, nRF52840, and nRF5340.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc312_mbedcrypto, nRF5340 variants

  * ``cortex-m33/hard-float/libnrf_cc312_mbedcrypto_0.9.5.a``
  * ``cortex-m33/soft-float/libnrf_cc312_mbedcrypto_0.9.5.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc312_mbedcrypto_0.9.5.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc312_mbedcrypto_0.9.5.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc312_mbedcrypto_0.9.5.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc312_mbedcrypto_0.9.5.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc312_mbedcrypto_0.9.5.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc312_mbedcrypto_0.9.5.a``


* nrf_cc310_mbedcrypto, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc312_mbedcrypto_0.9.5.a``
  * ``cortex-m33/soft-float/libnrf_cc310_mbedcrypto_0.9.5.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.5.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.5.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.5.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.5.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.5.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.5.a``


* nrf_cc310_mbedcrypto, nRF52840 variants

  * ``cortex-m4/soft-float/libnrf_cc310_mbedcrypto_0.9.5.a``
  * ``cortex-m4/hard-float/libnrf_cc310_mbedcrypto_0.9.5.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.5.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.5.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.5.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.5.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.5.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.5.a``



nrf_cc3xx_mbedcrypto - 0.9.4
****************************

Fixed bugs in KDR/KMU key derivation functions exposed in mbedtls/cc3xx_kmu.h.

This version is dependent on the nrf_cc310_platform or nrf_cc312_platform library for low-level initialization of the system and proper RTOS integration.


Added
=====

Added a new build of nrf_cc3xx_mbedcrypto libraries for nRF9160, nRF52840, and nRF5340.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc312_mbedcrypto, nRF5340 variants

  * ``cortex-m33/hard-float/libnrf_cc312_mbedcrypto_0.9.4.a``
  * ``cortex-m33/soft-float/libnrf_cc312_mbedcrypto_0.9.4.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc312_mbedcrypto_0.9.4.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc312_mbedcrypto_0.9.4.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc312_mbedcrypto_0.9.4.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc312_mbedcrypto_0.9.4.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc312_mbedcrypto_0.9.4.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc312_mbedcrypto_0.9.4.a``


* nrf_cc310_mbedcrypto, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc312_mbedcrypto_0.9.4.a``
  * ``cortex-m33/soft-float/libnrf_cc310_mbedcrypto_0.9.4.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.4.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.4.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.4.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.4.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.4.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.4.a``


* nrf_cc310_mbedcrypto, nRF52840 variants

  * ``cortex-m4/soft-float/libnrf_cc310_mbedcrypto_0.9.4.a``
  * ``cortex-m4/hard-float/libnrf_cc310_mbedcrypto_0.9.4.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.4.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.4.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.4.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.4.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.4.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.4.a``


nrf_cc3xx_mbedcrypto - 0.9.3
****************************

Added experimental support for devices with Arm CryptoCell CC312 (nRF5340).

Added APIs for key derivation of keys stored in the KMU peripheral (nRF9160, nRF5340).
See :file:`include/mbedlts/cc3xx_kmu.h`.

Added APIs for direct usage of keys stored in the KMU peripheral (nRF9160, nRF5340).
See :file:`include/mbedtls/cc3xx_kmu.h`.

Added APIs for key derivation from KDR key loaded into CryptoCell on boot (nRF52840, nRF9160).
See :file:`include/mbedtls/cc3xx_kmu.h`.

Added new version of libraries nrf_cc310_mbedcrypto/nrf_cc312_mbedcrypto built with mbed TLS version 2.23.0.

This version also adds experimental support for interrupts in selected versions of the library  (the libraries that do not support interrupts can be found in the ``no-interrupts`` folders).

This version is dependent on the nrf_cc310_platform or nrf_cc312_platform library for low-level initialization of the system and proper RTOS integration.


Added
=====

Added a new build of nrf_cc3xx_mbedcrypto libraries for nRF9160, nRF52840, and nRF5340.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc312_mbedcrypto, nRF5340 variants

  * ``cortex-m33/hard-float/libnrf_cc312_mbedcrypto_0.9.3.a``
  * ``cortex-m33/soft-float/libnrf_cc312_mbedcrypto_0.9.3.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc312_mbedcrypto_0.9.3.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc312_mbedcrypto_0.9.3.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc312_mbedcrypto_0.9.3.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc312_mbedcrypto_0.9.3.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc312_mbedcrypto_0.9.3.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc312_mbedcrypto_0.9.3.a``


* nrf_cc310_mbedcrypto, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc312_mbedcrypto_0.9.3.a``
  * ``cortex-m33/soft-float/libnrf_cc310_mbedcrypto_0.9.3.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.3.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.3.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.3.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.3.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.3.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.3.a``


* nrf_cc310_mbedcrypto, nRF52840 variants

  * ``cortex-m4/soft-float/libnrf_cc310_mbedcrypto_0.9.3.a``
  * ``cortex-m4/hard-float/libnrf_cc310_mbedcrypto_0.9.3.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.3.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.3.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.3.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.3.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.3.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.3.a``


nrf_cc310_mbedcrypto - 0.9.2
****************************

New experimental version of nrf_cc310_mbedcrypto with fixes for power management issues with pending interrupts.

This version also adds experimental support for interrupts in selected versions of the library  (the libraries that do not support interrupts can be found in the ``no-interrupts`` folders).

This version is dependent on the nrf_cc310_platform library for low-level initialization of the system and proper RTOS integration.

Added
=====

Added a new build of nrf_cc310_mbedcrypto library for nRF9160 and nRF52 architectures.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc310_mbedcrypto, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_mbedcrypto_0.9.2.a``
  * ``cortex-m33/soft-float/libnrf_cc310_mbedcrypto_0.9.2.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.2.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.2.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.2.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.2.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.2.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.2.a``

* nrf_cc310_mbedcrypto, nRF52 variants

  * ``cortex-m4/soft-float/libnrf_cc310_mbedcrypto_0.9.2.a``
  * ``cortex-m4/hard-float/libnrf_cc310_mbedcrypto_0.9.2.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.2.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.2.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.2.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.2.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.2.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.2.a``


nrf_cc310_mbedcrypto - 0.9.1
****************************

New experimental version of nrf_cc310_mbedcrypto with general bugfixes.

This version is dependent on the nrf_cc310_platform library for low-level initialization of the system and proper RTOS integration.

Added
=====

Added a new build of nrf_cc310_mbedcrypto library for nRF9160 and nRF52 architectures.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc310_mbedcrypto, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_mbedcrypto_0.9.1.a``
  * ``cortex-m33/soft-float/libnrf_cc310_mbedcrypto_0.9.1.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.1.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.1.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.1.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.1.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.1.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.1.a``

* nrf_cc310_mbedcrypto, nRF52 variants

  * ``cortex-m4/soft-float/libnrf_cc310_mbedcrypto_0.9.1.a``
  * ``cortex-m4/hard-float/libnrf_cc310_mbedcrypto_0.9.1.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.1.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.1.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.1.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.1.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.1.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.1.a``


nrf_cc310_mbedcrypto - 0.9.0
****************************

New experimental version of nrf_cc310_mbedcrypto with general bugfixes.

This version is dependent on the newly added nrf_cc310_platform library for low-level  initialization of the system and proper RTOS integration.

Added
=====

Added a new build of nrf_cc310_mbedcrypto library for nRF9160 and nRF52 architectures.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc310_mbedcrypto, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_mbedcrypto_0.9.0.a``
  * ``cortex-m33/soft-float/libnrf_cc310_mbedcrypto_0.9.0.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.0.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.0.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.0.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.0.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.0.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.0.a``

* nrf_cc310_mbedcrypto, nRF52 variants

  * ``cortex-m4/soft-float/libnrf_cc310_mbedcrypto_0.9.0.a``
  * ``cortex-m4/hard-float/libnrf_cc310_mbedcrypto_0.9.0.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.0.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.9.0.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.0.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.9.0.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.0.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.9.0.a``


nrf_cc310_mbedcrypto - 0.8.1
****************************

New experimental version of nrf_cc310_mbedcrypto with general bugfixes.

.. note::
  This version should be used for nRF9160 devices. Use of earlier versions may lead to
  undefined behavior on some nRF9160 devices.

Added
=====

Added a new build of nrf_cc310_mbedcrypto library for nRF9160 and nRF52 architectures.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc310_mbedcrypto, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_mbedcrypto_0.8.1.a``
  * ``cortex-m33/soft-float/libnrf_cc310_mbedcrypto_0.8.1.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.8.1.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.8.1.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.8.1.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.8.1.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.8.1.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.8.1.a``

* nrf_cc310_mbedcrypto, nRF52 variants

  * ``cortex-m4/soft-float/libnrf_cc310_mbedcrypto_0.8.1.a``
  * ``cortex-m4/hard-float/libnrf_cc310_mbedcrypto_0.8.1.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.8.1.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.8.1.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.8.1.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.8.1.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.8.1.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.8.1.a``


nrf_cc310_mbedcrypto - 0.8.0
****************************

New experimental version of nrf_cc310_mbedcrypto with changes to platform initialization and general bugfixes.

..warning::
   This version may lead to undefined behavior on some nRF9160 devices.
   Please use a newer version.

Added
=====

Added a new build of nrf_cc310_mbedcrypto library for nRF9160 and nRF52 architectures.

.. note::

   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc310_mbedcrypto, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_mbedcrypto_0.8.0.a``
  * ``cortex-m33/soft-float/libnrf_cc310_mbedcrypto_0.8.0.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.8.0.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.8.0.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.8.0.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.8.0.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.8.0.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.8.0.a``

* nrf_cc310_mbedcrypto, nRF52 variants

  * ``cortex-m4/soft-float/libnrf_cc310_mbedcrypto_0.8.0.a``
  * ``cortex-m4/hard-float/libnrf_cc310_mbedcrypto_0.8.0.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.8.0.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.8.0.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.8.0.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.8.0.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.8.0.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.8.0.a``


nrf_cc310_mbedcrypto - 0.7.0
****************************

Initial release.

Added
=====

Added the following nrf_cc310_mbedcrypto libraries for nRF9160 and nRF52 architectures.

.. note::
   The *short-wchar* libraries are compiled with a wchar_t size of 16 bits.


* nrf_cc310_mbedcrypto, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_mbedcrypto_0.7.0.a``
  * ``cortex-m33/soft-float/libnrf_cc310_mbedcrypto_0.7.0.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.7.0.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.7.0.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.7.0.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.7.0.a``

  * short-wchar, no interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.7.0.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.7.0.a``

* nrf_cc310_mbedcrypto, nRF52 variants

  * ``cortex-m4/soft-float/libnrf_cc310_mbedcrypto_0.7.0.a``
  * ``cortex-m4/hard-float/libnrf_cc310_mbedcrypto_0.7.0.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.7.0.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.7.0.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.7.0.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.7.0.a``

  * short-wchar, no interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.7.0.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.7.0.a``
