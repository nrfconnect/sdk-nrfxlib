.. _crypto_changelog_nrf_cc310_mbedcrypto:

Changelog - nRF cc310 mbed crypto
#################################

All notable changes to this project are documented in this file.

nrf_cc310_mbedcrypto - 0.8.2
****************************

New experimental version of nrf_cc310_mbedcrypto with general bugfixes.

Added
=====

Added new build of nRF cc310 mbed crypto library for nRF9160 and nRF52 architectures.

..note:

   short-wchar: Those libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc310_mbedcrypto, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_mbedcrypto_0.8.2.a``
  * ``cortex-m33/soft-float/libnrf_cc310_mbedcrypto_0.8.2.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.8.2.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.8.2.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.8.2.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.8.2.a``

  * short-wchar, No interrupts

    * ``cortex-m33/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.8.2.a``
    * ``cortex-m33/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.8.2.a``

* nrf_cc310_mbedcrypto, nRF52 variants

  * ``cortex-m4/soft-float/libnrf_cc310_mbedcrypto_0.8.2.a``
  * ``cortex-m4/hard-float/libnrf_cc310_mbedcrypto_0.8.2.a``

  * No interrupts

    * ``cortex-m4/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.8.2.a``
    * ``cortex-m4/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.8.2.a``

  * short-wchar

    * ``cortex-m4/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.8.2.a``
    * ``cortex-m4/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.8.2.a``

  * short-wchar, No interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.8.2.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.8.2.a``


nrf_cc310_mbedcrypto - 0.8.1
****************************

New experimental version of nrf_cc310_mbedcrypto with general bugfixes. 

..note::
  This version should be used for nRF9160 devices. Use of earlier versions may lead to 
  undefined behavior on some nRF9160 devices.

Added
=====

Added new build of nRF cc310 mbed crypto library for nRF9160 and nRF52 architectures.

..note:

   short-wchar: Those libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc310_mbedcrypto, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_mbedcrypto_0.8.1.a``
  * ``cortex-m33/soft-float/libnrf_cc310_mbedcrypto_0.8.1.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.8.1.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.8.1.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.8.1.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.8.1.a``

  * short-wchar, No interrupts

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

  * short-wchar, No interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.8.1.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.8.1.a``


nrf_cc310_mbedcrypto - 0.8.0
****************************

New experimental version of nrf_cc310_mbedcrypto with changes to platform initialization and general bugfixes.

..warning::
   This version may lead to undefined behavior on some nRF9160 devices. Please use a newer version.

Added
=====

Added new build of nRF cc310 mbed crypto library for nRF9160 and nRF52 architectures.

..note:

   short-wchar: Those libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc310_mbedcrypto, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_mbedcrypto_0.8.0.a``
  * ``cortex-m33/soft-float/libnrf_cc310_mbedcrypto_0.8.0.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.8.0.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.8.0.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.8.0.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.8.0.a``

  * short-wchar, No interrupts

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

  * short-wchar, No interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.8.0.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.8.0.a``


nrf_cc310_mbedcrypto - 0.7.0
****************************

Initial release.

Added
=====

Added the following nRF cc310 mbed crypto libraries for nRF9160 and nRF52 architectures.

.. note::
   short-wchar: Those libraries are compiled with a wchar_t size of 16 bits.


* nrf_cc310_mbedcrypto, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_mbedcrypto_0.7.0.a``
  * ``cortex-m33/soft-float/libnrf_cc310_mbedcrypto_0.7.0.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_mbedcrypto_0.7.0.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_mbedcrypto_0.7.0.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_mbedcrypto_0.7.0.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_mbedcrypto_0.7.0.a``

  * short-wchar, No interrupts

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

  * short-wchar, No interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.7.0.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_mbedcrypto_0.7.0.a``

