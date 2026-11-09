.. _nrf_cc310_platform_readme:
.. _nrf_cc3xx_platform_readme:

nrf_cc3xx_platform library
##########################

.. contents::
   :local:
   :depth: 2

The nrf_cc3xx_platform library is a software library that interfaces with the Arm CryptoCell CC310/CC312 hardware accelerator that is available on the nRF52840 SoC, the nRF53 Series, and the nRF91 Series.

The platform library provides low-level functionality needed by the CC310/CC312 mbedcrypto library.
This includes memory, mutex, and entropy APIs needed for the CC310/CC312 mbedcrypto library.

The library adds hardware support for the True Random Number Generator, TRNG, available in the CC310/CC312 hardware.

In the |NCS|, the library is used by the :ref:`nrf:crypto_drivers_cc3xx` and the :ref:`nrf_cc3xx_mbedcrypto_readme`.
You can also use the library on its own.

.. _nrf_cc3xx_platform_initialization:

Initializing the library
========================

When you use the library through the :ref:`nrf:crypto_drivers_cc3xx` in the |NCS|, the :ref:`nrf:lib_hw_cc3xx` driver initializes the nrf_cc3xx_platform library.
You do not need to call the initialization APIs in application code.

When you use the nrf_cc3xx_platform library on its own, for example to access KMU, entropy, or CTR_DRBG APIs without using the :ref:`nrf:crypto_drivers_cc3xx`, you must initialize the library before you call other nrf_cc3xx_platform APIs.

Use one of the following functions, depending on whether you need random number generation:

* :c:func:`nrf_cc3xx_platform_init` - Initializes the platform using CTR_DRBG for RNG support.
* :c:func:`nrf_cc3xx_platform_init_hmac_drbg` - Initializes the platform using HMAC_DRBG for RNG support.
* :c:func:`nrf_cc3xx_platform_init_no_rng` - Initializes the platform without RNG support.

See :ref:`crypto_api_nrf_cc3xx_platform_init` for the API reference.

.. code-block:: c
   :caption: Initializing the nrf_cc3xx_platform library

   if (nrf_cc3xx_platform_init() != 0) {
           /* nrf_cc3xx_platform failed to initialize. */
           return -1;
   }

.. note::
   There is no need to enable or disable the CC310 hardware by writing to the ``NRF_CRYPTOCELL->ENABLE`` and ``NRF_CRYPTOCELL_S->ENABLE`` registers.
   This happens automatically when you call APIs in this library.

In the Bare Metal option, configure the mutex and abort handlers before initialization.
See the mutex and abort sections in :ref:`crypto_api_nrf_cc3xx_platform`.

API documentation
=================

:ref:`crypto_api_nrf_cc3xx_platform`
