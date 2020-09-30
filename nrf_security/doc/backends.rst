.. _nrf_security_backends:

nrf_security backends
#####################

The nrf_security module supports multiple enabled backends at the same time.
This mechanism is intended to extend the available feature set of hardware-accelerated cryptography or to provide alternative implementations of the mbed TLS APIs.
Enabling one or more backends adds more configuration options grouped into classes of cryptographic algorithms.

Note that some cryptographic features are provided as an option regardless of the backend enabled.
In such cases, the feature is compiled using open-source software from the original Arm mbed TLS project.
An example of this is `SHA-512`, which is only accessible using open-source software from the Arm mbed TLS project.

The configuration options added after enabling one or more backends will change based on the number of enabled backends.
Some configuration options allow for adding support from multiple backends by utilizing the mbed TLS glue layer, while other provide a selection between the enabled backends (as radio buttons).

The nrf_security module supports the following backends:

* Arm CryptoCell cc310 (in nRF52840 and nRF9160)
* nrf_oberon binary library
* Original mbed TLS

.. note::
   Note that whenever this documentation mentions 'original' mbed TLS, it refers to the open-source `Arm mbed TLS project`_, not the customized version available in Zephyr.
   KConfig options used to configure this backend include the term ``VANILLA``.

.. _nrf_security_backends_cc310:

Arm CryptoCell cc310 backend
****************************

The Arm CryptoCell cc310 backend is a runtime library that provides hardware-accelerated cryptography using the Arm CryptoCell cc310 hardware.

The Arm CryptoCell cc310 backend is only available on the following devices:

* nRF52840
* nRF9160


Enabling the Arm CryptoCell cc310 backend
=========================================

The Arm CryptoCell cc310 backend can be enabled by setting the :option:`CONFIG_CC310_BACKEND` Kconfig variable.

.. note:: This backend is only available in nRF52840 and nRF9160.


.. _nrf_security_backends_oberon:

nrf_oberon backend
******************

The :ref:`nrf_oberon_readme` library is a binary library that provides select cryptographic algorithms optimized for use in nRF devices.
This provides faster execution than the original mbed TLS implementation.

The nrf_oberon backend provides support for AES ciphers, SHA-1, SHA-256, and ECC (ECDH, ECDSA, and ECJPAKE) using NIST curve secp256r1.

Enabling the nrf_oberon backend
===============================

To use the :ref:`nrf_oberon_readme` as a backend, set the :option:`CONFIG_OBERON_BACKEND` Kconfig variable to true.

.. _nrf_security_backends_orig_mbedtls:

Original mbed TLS backend
*************************

The original mbed TLS backend uses the open-source software from the `Arm mbed TLS project`_.

The software is unaltered from the Arm mbed TLS project source distribution, meaning it is in its `vanilla` form.

You can use the original mbed TLS backend to add support for features not available in the `Arm CryptoCell cc310 backend`_, for example AES-256 or ECC Brainpool curve types.

Similarly, you can use the original mbed TLS backend to add support for features not available in the `nrf_oberon backend`_, for example to support more ECC curve types.


Enabling the original mbed TLS backend
======================================

To enable the original mbed TLS backend, set the :option:`CONFIG_MBEDTLS_VANILLA_BACKEND` Kconfig variable to true.


Using the nrf_cc310_mbedcrypto as backend
*****************************************

To use the :ref:`nrf_cc310_mbedcrypto_readme` as a backend, the Arm CryptoCell cc310 hardware must be first initialized.

The Arm CryptoCell cc310 hardware is initialized in :file:`<NCS>/nrf/drivers/hw_cc310/hw_cc310.c` and is controlled with the :option:`CONFIG_HW_CC310` Kconfig variable.
The Kconfig variable has a default value of 'y' when cc310 is available in the SoC.
