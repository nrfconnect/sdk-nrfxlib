.. _crypto_changelog_nrf_cc310_platform:

Changelog - nrf_cc310_platform
##############################

All notable changes to this project are documented in this file.

nrf_cc310_platform - 0.9.2
**************************

New version of nrf_cc310_platform library fixing power management issues with pending interrupts.

This version also adds experimental support for interrupts in selected versions of the library (the libraries that do not support interrupts can be found in the ``no-interrupts`` folders).

This version must match the version of nrf_cc310_mbedcrypto if it is also used.

Added
=====

Added new build of nrf_cc310_platform library for nRF9160 and nRF52 architectures.

.. note::

   short-wchar: Those libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc310_platform, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_platform_0.9.2.a``
  * ``cortex-m33/soft-float/libnrf_cc310_platform_0.9.2.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_platform_0.9.2.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_platform_0.9.2.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_platform_0.9.2.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_platform_0.9.2.a``

  * short-wchar, No interrupts

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

  * short-wchar, No interrupts

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

Added new build of nrf_cc310_platform library for nRF9160 and nRF52 architectures.

.. note::

   short-wchar: Those libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc310_platform, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_platform_0.9.1.a``
  * ``cortex-m33/soft-float/libnrf_cc310_platform_0.9.1.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_platform_0.9.1.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_platform_0.9.1.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_platform_0.9.1.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_platform_0.9.1.a``

  * short-wchar, No interrupts

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

  * short-wchar, No interrupts

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

Added new build of nrf_cc310_platform library for nRF9160 and nRF52 architectures.

.. note::

   short-wchar: Those libraries are compiled with a wchar_t size of 16 bits.

* nrf_cc310_platform, nRF9160 variants

  * ``cortex-m33/hard-float/libnrf_cc310_platform_0.9.0.a``
  * ``cortex-m33/soft-float/libnrf_cc310_platform_0.9.0.a``

  * No interrupts

    * ``cortex-m33/soft-float/no-interrupts/libnrf_cc310_platform_0.9.0.a``
    * ``cortex-m33/hard-float/no-interrupts/libnrf_cc310_platform_0.9.0.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/libnrf_cc310_platform_0.9.0.a``
    * ``cortex-m33/soft-float/short-wchar/libnrf_cc310_platform_0.9.0.a``

  * short-wchar, No interrupts

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

  * short-wchar, No interrupts

    * ``cortex-m4/soft-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.0.a``
    * ``cortex-m4/hard-float/short-wchar/no-interrupts/libnrf_cc310_platform_0.9.0.a``
