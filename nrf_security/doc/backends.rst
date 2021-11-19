.. _nrf_security_backends:

nrf_security backends
#####################

.. contents::
   :local:
   :depth: 2

The nrf_security module supports multiple enabled backends at the same time.
This mechanism is intended to extend the available feature set of hardware-accelerated cryptography or to provide alternative implementations of the Mbed TLS APIs.
Enabling one or more backends adds more configuration options grouped into classes of cryptographic algorithms.

The configuration options added after enabling one or more backends will change based on the number of enabled backends.
Some configuration options allow for adding support from multiple backends by utilizing the Mbed TLS glue layer, while other provide a selection between the enabled backends (as radio buttons).

The nrf_security module supports the following backends:

* Arm CryptoCell cc3xx
* nrf_oberon binary library
* Original Mbed TLS

.. note::
   Note that whenever this documentation mentions 'original' Mbed TLS, it refers to the open-source `Arm Mbed TLS project`_, not the customized version available in Zephyr.
   Kconfig options used to configure this backend include the term ``VANILLA``.

.. _nrf_security_backends_cc3xx:

Arm CryptoCell cc3xx backend
****************************

The Arm CryptoCell cc3xx backend is a runtime library that provides hardware-accelerated cryptography using the Arm CryptoCell cc310/cc312 hardware.

The Arm CryptoCell cc3xx backend is only available on the following devices:

* nRF52840
* nRF9160
* nRF5340


Enabling the Arm CryptoCell cc3xx backend
=========================================

The Arm CryptoCell cc3xx backend can be enabled by setting the :kconfig:`CONFIG_CC3XX_BACKEND` Kconfig variable.


Using the Arm CryptoCell cc3xx backend
======================================

To use the :ref:`nrf_cc3xx_mbedcrypto_readme` as a backend, the Arm CryptoCell cc310/cc312 hardware must be first initialized.

The Arm CryptoCell cc3xx hardware is initialized in :file:`<NCS>/nrf/drivers/hw_cc310/hw_cc310.c` and is controlled with the :kconfig:`CONFIG_HW_CC3XX` Kconfig variable.
The Kconfig variable has a default value of 'y' when cc3xx is available in the SoC.

.. _nrf_security_backends_oberon:

nrf_oberon backend
******************

The :ref:`nrf_oberon_readme` is distributed as a closed-source binary that provides select cryptographic algorithms optimized for use in nRF devices.
This provides faster execution than the original Mbed TLS implementation.

The nrf_oberon backend provides support for AES ciphers, SHA-1, SHA-256, and ECC (ECDH, ECDSA, and ECJPAKE) using NIST curve secp256r1.

Enabling the nrf_oberon backend
===============================

To use the :ref:`nrf_oberon_readme` as a backend, set the :kconfig:`CONFIG_OBERON_BACKEND` Kconfig variable to true.

.. _nrf_security_backends_orig_mbedtls:

Original Mbed TLS backend
*************************

The original Mbed TLS backend uses the open-source software from the `Arm Mbed TLS project`_.

The software is unaltered from the Arm Mbed TLS project source distribution, meaning it is in its `vanilla` form.

You can use the original Mbed TLS backend to add support for features not available in the `Arm CryptoCell cc3xx backend`_, for example AES-256 or ECC Brainpool curve types.

Similarly, you can use the original Mbed TLS backend to add support for features not available in the `nrf_oberon backend`_, for example to support more ECC curve types.


Enabling the original Mbed TLS backend
======================================

To enable the original Mbed TLS backend, set the :kconfig:`CONFIG_MBEDTLS_VANILLA_BACKEND` Kconfig variable to true.
