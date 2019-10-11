.. _nrf_cc310_platform_readme:

nrf_cc310_platform library
############################

The nrf_cc310_platform library is software library to interface with the Arm CryptoCell CC310 hardware accelerator that is available on the nRF52840 SoC and the nRF9160 SiP.

The platform library provides low-level functionality needed by the CC310
mbedcrypto library.

This includes memory, mutex, and entropy APIs needed for the CC310 mbedcrypto
library.

The library adds hardware support for the True Random Number Generator, TRNG,
available in the CC310 hardware.

API documentation
=================

:ref:`crypto_api_nrf_cc310_platform`
