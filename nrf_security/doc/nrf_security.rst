.. _nrf_security_readme:

Nordic security module
######################

The nrf_security module provides an integration between mbed TLS and software libraries that provide hardware-accelerated cryptographic functionality on selected Nordic Semiconductor SoCs as well as alternate software-based implementations of the mbed TLS APIs.
This module includes an mbed TLS glue layer to enable both hardware-accelerated and software-based mbed TLS implementation at the same time.

.. note::
   The nrf_security module interfaces with the :ref:`nrf_cc310_mbedcrypto_readme`.
   This library conforms to a specific version of mbed TLS.


Prerequisites
=============
The nrf_security module requires mbed TLS. It checks for the presence of mbed TLS in the following ways:

* CMake uses west as part of the build process to locate mbed TLS.
* ``ARM_MBEDTLS_PATH`` can be set to enforce a specific path to mbed TLS or in case west is not available

If west is not installed and mbed TLS is not located, CMake reports the following error:

.. code-block:: console

    west not installed, please provide ARM_MBEDTLS_PATH to CMake to support security libraries

If west is installed and mbed TLS is not located, CMake reports the following error:

.. code-block:: console

    Failed to find mbed TLS, cannot build security libraries

Configuration
=============
Use Kconfig to configure the nrf_security module.
To enable the module, set the :option:`CONFIG_NORDIC_SECURITY_BACKEND` Kconfig variable in the `Nordic Security` menu.

Setting this variable allows for additional Kconfig variables, depending on the number of features requested.
These configurations are then used to generate an mbed TLS configuration file used during compilation.

It is possible to provide your own custom mbed TLS configuration file by deselecting the :option:`CONFIG_GENERATE_MBEDTLS_CFG_FILE` Kconfig variable.

.. note::
   Deselecting the :option:`CONFIG_GENERATE_MBEDTLS_CFG_FILE` Kconfig variable is not recommended.
   If you decide to do so, see `user-provided mbed TLS config header`_.


nrf_security backends
=====================

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


Arm CryptoCell cc310 backend
----------------------------

The Arm CryptoCell cc310 backend is a runtime library that provides hardware-accelerated cryptography using the Arm CryptoCell cc310 hardware.

The Arm CryptoCell cc310 backend is only available on the following devices:

* nRF52840
* nRF9160


Enabling the Arm CryptoCell cc310 backend
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The Arm CryptoCell cc310 backend can be enabled by setting the :option:`CONFIG_CC310_BACKEND` Kconfig variable.

.. note:: This backend is only available in nRF52840 and nRF9160.


nrf_oberon backend
------------------

The :ref:`nrf_oberon_readme` library is a binary library that provides select cryptographic algorithms optimized for use in nRF devices.
This provides faster execution than the original mbed TLS implementation.

The nrf_oberon backend provides support for AES ciphers, SHA-1, SHA-256, and ECC (ECDH, ECDSA, and ECJPAKE) using NIST curve secp256r1.

Enabling the nrf_oberon backend
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To use the :ref:`nrf_oberon_readme` as a backend, set the :option:`CONFIG_OBERON_BACKEND` Kconfig variable to true.


Original mbed TLS backend
-------------------------

The original mbed TLS backend uses the open-source software from the `Arm mbed TLS project`_.

The software is unaltered from the Arm mbed TLS project source distribution, meaning it is in its `vanilla` form.

You can use the original mbed TLS backend to add support for features not available in the `Arm CryptoCell cc310 backend`_, for example AES-256 or ECC Brainpool curve types.

Similarly, you can use the original mbed TLS backend to add support for features not available in the `nrf_oberon backend`_, for example to support more ECC curve types.


Enabling the original mbed TLS backend
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To enable the original mbed TLS backend, set the :option:`CONFIG_MBEDTLS_VANILLA_BACKEND` Kconfig variable to true.


Using the nrf_cc310_mbedcrypto as backend
-----------------------------------------

To use the :ref:`nrf_cc310_mbedcrypto_readme` as a backend, the Arm CryptoCell cc310 hardware must be first initialized.

The Arm CryptoCell cc310 hardware is initialized in :file:`<NCS>/nrf/drivers/hw_cc310/hw_cc310.c` and is controlled with the :option:`CONFIG_HW_CC310` Kconfig variable.
The Kconfig variable has a default value of 'y' when cc310 is available in the SoC.


mbed TLS glue layer
===================
The mbed TLS glue layer allows for using both the hardware-accelerated and software implementation of cryptography at the same time.
A typical use case is if the cryptographic algorithm is limited in hardware support (for example, the AES key size or supported ciphers).
In this case, the mbed TLS glue layer enables a dynamic check to verify whether the cryptographic algorithm is supported in hardware.
If it is not supported, the glue layer calls into a different enabled backend as a fallback.

The mbed TLS glue layer is written by Nordic Semiconductor ASA and is a part of the nrf_security module.
The mbed TLS glue layer is dependent on Kconfig variables in the |NCS| build system.

mbed TLS glue layer - support and priority
------------------------------------------

To select the backend, the mbed TLS glue layer implements a functionality check with priority reporting.
The check is performed using a `check function`.
This function is called from the API which provides configuration changes that determine the selection between the enabled backends.

.. code-block:: c
    :caption: Example: cc310 backend AES CCM support and priority check

    static int mbedtls_ccm_check(mbedtls_cipher_id_t cipher, unsigned int keybits) {
            return (keybits == 128) ? 3 : 0;
    }

In this example, the AES CCM support in the backend will report priority level 3 if the key size is 128, or 0 if the key size is different.
The `Arm CryptoCell cc310 backend`_ does not support a larger key size.
If the key size is larger than 128 bits, then another enabled backend is used.

.. note::
   The check function can be called from multiple APIs in the mbed TLS glue layer.
   An example of a location that determines backend selection is an API to set an encryption key, in which case the key size may change, demanding a new selection of available and prioritized backends.

.. note::
   Hardware-accelerated cryptography through the `Arm CryptoCell cc310 backend`_ is prioritized if it is supported.


Enabling the mbed TLS glue layer
--------------------------------

The mbed TLS glue layer is automatically enabled when two backends are enabled for one or more of the following groups of algorithms:

* AES (ECB, CBC, CTR, and CCM/CCM*)
* CMAC
* DHM


mbed TLS glue layer mechanisms
------------------------------
The mbed TLS glue layer relies on symbol renaming of known APIs in mbed TLS to prevent collisions of identically named functions in multiple backends.
The backend implementation is reached using a table of function pointers corresponding to the renamed symbols.

.. code-block:: c
    :caption: Example: cc310 backend ECDH function table

    const mbedtls_ecdh_funcs mbedtls_ecdh_cc310_backend_funcs = {
            .check = mbedtls_ecdh_check,
            .gen_public = mbedtls_ecdh_gen_public,
            .compute_shared = mbedtls_ecdh_compute_shared,
    };

:cpp:func:`mbedtls_ecdh_cc310_backend_funcs` points to mbed TLS APIs in :ref:`nrf_cc310_mbedcrypto_readme` which is renamed if mbed TLS glue layer is enabled.
The function pointers `gen_public` and `compute_shared` have signatures equal to the corresponding mbed TLS APIs.


The complete list of APIs that can be renamed in the mbed TLS glue layer can be found in :file:`nrfxlib/nrf_security/src/mbedcrypto_glue/symbol_rename.template.txt`


nrf_security backend configurations
===================================

This section covers the configurations available when one or more nrf_security backends are enabled.
This includes linking directly to the backend library or utilizing the mbed TLS glue layer.

Configuring multiple backends
-----------------------------
Different backends can support different cryptographic algorithms.
The configuration options listed in subsequent sections are either `Glue`_, `Shared`_, or `Choice`_.
Additionaly, if only a subset of the backends supports a given feature, this information is provided in the tables.

Glue
~~~~
The configuration options will list `Glue` for the cryptographic algorithms that can be enabled in multiple backends at the same time.

Shared
~~~~~~
For some cryptographic features, the implementation is only available in the `Original mbed TLS backend`_.
For convenience, this is made available in any backend, although the implementation will always use open-source code from the Arm mbed TLS project.

Choice
~~~~~~
The configuration options will list `Choice` for the cryptographic algorithms that are supported by multiple backends, but only one of them can be enabled at the same time.
To enable the cryptographic algorithm, a `base` configuration must be enabled, and then the backend can be selected using a setting prefixed with `CONFIG_CHOICE_`.

.. note::
   The first ordered item in the list of available choices is selected by default.


AES configuration
------------------

AES core support can be enabled by setting the :option:`CONFIG_MBEDTLS_AES_C` Kconfig variable.
Enabling AES core support enables AES ECB cipher mode and allows for the following ciphers to be configured: CTR, OFB, CFB, CBC, XTS, CMAC, CCM/CCM*, and GCM.

Single backend
~~~~~~~~~~~~~~
AES core support can be enabled by setting the :option:`CONFIG_MBEDTLS_AES_C` Kconfig variable.

+--------------+-----------------------------------+
| Cipher mode  | Configurations                    |
+==============+===================================+
| ECB          | :option:`CONFIG_MBEDTLS_AES_C`    |
+--------------+-----------------------------------+

.. note::
   * The `Arm CryptoCell cc310 backend`_ is limited to key sizes of 128 bits.

Multiple backends
~~~~~~~~~~~~~~~~~
AES core support can be enabled by setting setting the :option:`CONFIG_MBEDTLS_AES_C` Kconfig variable, and one or more of the following Kconfig variables:

+--------------+----------------+------------------------------------------------------------+
| Cipher mode  | Support        | Configurations                                             |
+==============+================+============================================================+
| ECB          | Glue           | cc310: :option:`CONFIG_CC310_MBEDTLS_AES_C`                |
|              |                |                                                            |
|              |                | nrf_oberon: :option:`CONFIG_OBERON_MBEDTLS_AES_C`          |
|              |                |                                                            |
|              |                | Original mbed TLS: :option:`CONFIG_VANILLA_MBEDTLS_AES_C`  |
+--------------+----------------+------------------------------------------------------------+

.. note::
   * The `Arm CryptoCell cc310 backend`_ is limited to key sizes of 128 bits.
   * Enabling the `nrf_oberon backend`_ replaces select internal APIs for AES block encrypt/decrypt and set key operations for encrypt/decrypt.
   * If both nrf_oberon backend and `Original mbed TLS backend`_ are enabled, the implementation from
     nrf_oberon backend will provide support for AES ECB.


AES cipher configuration
------------------------
AES cipher mode configuration is dependent on AES core support according to `AES configuration`_.
Configure the AES cipher mode support according to single or multiple enabled backends.

Single or multiple backends
~~~~~~~~~~~~~~~~~~~~~~~~~~~
AES cipher modes can be enabled by setting one or more of the following Kconfig variables:

+--------------+-----------------+---------------------------------------------+----------------------------------------+
| Cipher mode  | Support         | Configurations                              | Note                                   |
+==============+=================+=============================================+========================================+
| CTR          | Glue            | :option:`CONFIG_MBEDTLS_CIPHER_MODE_CTR`    |                                        |
+--------------+-----------------+---------------------------------------------+----------------------------------------+
| CBC          | Glue            | :option:`CONFIG_MBEDTLS_CIPHER_MODE_CBC`    |                                        |
+--------------+-----------------+---------------------------------------------+----------------------------------------+
| CFB          |                 | :option:`CONFIG_MBEDTLS_CIPHER_MODE_CFB`    | Original mbed TLS and nrf_oberon only  |
+--------------+-----------------+---------------------------------------------+----------------------------------------+
| OFB          |                 | :option:`CONFIG_MBEDTLS_CIPHER_MODE_OFB`    | Original mbed TLS and nrf_oberon only  |
+--------------+-----------------+---------------------------------------------+----------------------------------------+
| XTS          |                 | :option:`CONFIG_MBEDTLS_CIPHER_MODE_XTS`    | Original mbed TLS and nrf_oberon only  |
+--------------+-----------------+---------------------------------------------+----------------------------------------+

.. note::
   * AES cipher modes are dependent on enabling AES core support according to `AES configuration`_.
   * The `Arm CryptoCell cc310 backend`_ is limited to key sizes of 128 bits.
   * Currently, AES cipher modes CFB, OFB, and XTS are not supported by the Arm CryptoCell cc310 backend.
   * XTS will not be available if multiple backends are enabled for AES.
   * If both `nrf_oberon backend`_ and `Original mbed TLS backend`_ is enabled, the implementation from
     nrf_oberon backend will provide support for AES cipher modes.


CMAC configuration
------------------
Cipher-based Message Authentication Code (CMAC) support can be enabled by setting Kconfig variables according to single or multiple enabled backends.

Single backend
~~~~~~~~~~~~~~
CMAC can be enabled by setting the :option:`CONFIG_MBEDTLS_CMAC_C` Kconfig variable.

+--------------+-----------------------------------+
| Algorithm    | Configurations                    |
+==============+===================================+
| CMAC         | :option:`CONFIG_MBEDTLS_CMAC_C`   |
+--------------+-----------------------------------+

Multiple backends
~~~~~~~~~~~~~~~~~
CMAC can be enabled by setting the :option:`CONFIG_MBEDTLS_CMAC_C` Kconfig variable, and one or more of the following Kconfig variables:

+--------------+-----------------------------+-----------------------------------------------------------------+
| Algorithm    | Support                     | Configurations                                                  |
+==============+=============================+=================================================================+
| CMAC         | Glue                        | cc310: :option:`CONFIG_CC310_MBEDTLS_CMAC_C`                    |
|              |                             |                                                                 |
|              |                             | nrf_oberon: :option:`CONFIG_OBERON_MBEDTLS_CMAC_C`              |
|              |                             |                                                                 |
|              |                             | Original mbed TLS: :option:`CONFIG_VANILLA_MBEDTLS_CMAC_C`      |
+--------------+-----------------------------+-----------------------------------------------------------------+

.. note::
   * The `Arm CryptoCell cc310 backend`_ is limited to key sizes of 128 bits.
   * If both `nrf_oberon backend`_ and `Original mbed TLS backend`_ is enabled, the implementation from
     nrf_oberon backend will provide support for CMAC.

AEAD configurations
-------------------
Authenticated Encryption with Associated Data (AEAD) can be enabled according to the cipher in question.
AEAD cipher mode support support can be enabled by setting Kconfig variables according to single or multiple enabled backends.

Single backend
~~~~~~~~~~~~~~
AEAD cipher mode support can be enabled by setting one or more of the following Kconfig variables:

+--------------+-----------------------------------------+-----------------------------------------+
| AEAD cipher  | Configurations                          | Note                                    |
+==============+=========================================+=========================================+
| AES CCM/CCM* | :option:`CONFIG_MBEDTLS_CCM_C`          |                                         |
+--------------+-----------------------------------------+-----------------------------------------+
| AES GCM      | :option:`CONFIG_MBEDTLS_GCM_C`          | Original mbed TLS or nrf_oberon only    |
+--------------++----------------------------------------+-----------------------------------------+
| ChaCha20     | :option:`CONFIG_MBEDTLS_CHACHA20_C`     |                                         |
+--------------+-----------------------------------------+-----------------------------------------+
| Poly1305     | :option:`CONFIG_MBEDTLS_POLY1305_C`     |                                         |
+--------------+-----------------------------------------+-----------------------------------------+
| ChaCha-Poly  | :option:`CONFIG_MBEDTLS_CHACHAPOLY_C`   | Requires `Poly1305` and `ChaCha20`      |
+--------------+-----------------------------------------+-----------------------------------------+

.. note::
   * AEAD AES cipher modes are dependent on enabling AES core support according to `AES configuration`_.
   * The `Arm CryptoCell cc310 backend`_ is limited to key sizes of 128 bits.
   * The ChaCha-Poly implemented by the Arm CryptoCell cc310 backend does not support incremental operations.


Multiple backends
~~~~~~~~~~~~~~~~~

AES CCM/CCM*
::::::::::::

AES CCM/CCM* can be enabled by setting the :option:`CONFIG_MBEDTLS_CCM_C` Kconfig variable, and one or more of the following Kconfig variables:

+--------------+-----------------+-----------------------------------------------------------------+
| AEAD cipher  | Support         | Configurations                                                  |
+==============+=================+=================================================================+
| AES CCM/CCM* | Glue            | cc310: :option:`CONFIG_CC310_MBEDTLS_CCM_C`                     |
|              |                 |                                                                 |
|              |                 | nrf_oberon: :option:`CONFIG_OBERON_MBEDTLS_CCM_C`               |
|              |                 |                                                                 |
|              |                 | Original mbed TLS: :option:`CONFIG_VANILLA_MBEDTLS_CCM_C`       |
+--------------+-----------------+-----------------------------------------------------------------+

.. note::
   * AEAD AES cipher modes are dependent on AES core support according to `AES configuration`_.
   * The `Arm CryptoCell cc310 backend`_ is limited to key sizes of 128 bits.
   * If both `nrf_oberon backend`_ and `Original mbed TLS backend`_ is enabled, the nrf_oberon backend will
     provide support for AES CCM/CCM*.

AES GCM
:::::::
AES GCM can be enabled by setting the :option:`CONFIG_MBEDTLS_GCM_C` Kconfig variable.

+--------------+-----------------------------------+--------------------------------------+
| AEAD cipher  | Configurations                    | Note                                 |
+==============+===================================+======================================+
| AES GCM      | :option:`CONFIG_MBEDTLS_GCM_C`    | Original mbed TLS or nrf_oberon only |
+--------------+-----------------------------------+--------------------------------------+

.. note::
   * AES GCM is not supported in Arm CryptoCell cc310 hardware.

ChaCha20
::::::::
ChaCha20 support can be enabled by setting the :option:`CONFIG_MBEDTLS_CHACHA20_C` Kconfig variable, and one of the following Kconfig variables:

+--------------+-----------------+--------------------------------------------------------------------------+
| AEAD cipher  | Support         | Configurations                                                           |
+==============+=================+==========================================================================+
| ChaCha20     | Choice          | cc310: :option:`CONFIG_CHOICE_CC310_MBEDTLS_CHACHA20_C`                  |
|              |                 |                                                                          |
|              |                 | Original mbed TLS: :option:`CONFIG_CHOICE_VANILLA_MBEDTLS_CHACHA20_C`    |
+--------------+-----------------+--------------------------------------------------------------------------+

.. note::
   * For features provided with `Choice`_ support, the enabled backend that is first in order is selected by default.
   * The ChaCha-Poly implemented by the `Arm CryptoCell cc310 backend`_ does not support incremental operations.

Poly1305
::::::::
Poly1305 can be enabled by setting the :option:`CONFIG_MBEDTLS_POLY1305_C` Kconfig variable and one of the following Kconfig variables:

+--------------+-----------------+-----------------------------------------------------------------------+
| AEAD cipher  | Support         | Configurations                                                        |
+==============+=================+=======================================================================+
| Poly1305     | Choice          | cc310: :option:`CONFIG_CHOICE_CC310_MBEDTLS_POLY1305_C`               |
|              |                 |                                                                       |
|              |                 | Original mbed TLS: :option:`CONFIG_CHOICE_VANILLA_MBEDTLS_POLY1305_C` |
+--------------+-----------------+-----------------------------------------------------------------------+

.. note::
   * For features provided with `Choice`_ support, the enabled backend that is first in order is selected by default.
   * The ChaCha-Poly implemented by the `Arm CryptoCell cc310 backend`_ does not support incremental operations.


ChaCha-Poly
:::::::::::

ChaCha-Poly can be enabled by setting the :option:`CONFIG_MBEDTLS_CHACHAPOLY_C` Kconfig variable, and one of the following Kconfig variables:

+--------------+-----------------+--------------------------------------------------------------------------+
| AEAD cipher  | Support         | Configurations                                                           |
+==============+=================+==========================================================================+
| ChaCha-Poly  | Choice          | cc310: :option:`CONFIG_CHOICE_VANILLA_MBEDTLS_CHACHAPOLY_C`              |
|              |                 |                                                                          |
|              |                 | Original mbed TLS: :option:`CONFIG_CHOICE_VANILLA_MBEDTLS_CHACHAPOLY_C`  |
+--------------+-----------------+--------------------------------------------------------------------------+

.. note::
   * ChaCha-Poly support requires enabling both `ChaCha20`_ and `Poly1305`_.
   * For features provided with `Choice`_ support, the enabled backend that is first in order is selected by default.
   * The ChaCha-Poly implementation in `Arm CryptoCell cc310 backend`_ does not support incremental operations.

DHM configurations
------------------
Diffie-Hellman-Merkel (DHM) support can be enabled by setting Kconfig variables according to single or multiple enabled backends.

Single backend
~~~~~~~~~~~~~~
DHM can be enabled by setting the :option:`CONFIG_MBEDTLS_DHM_C` Kconfig variable.

+--------------+--------------------------------------+
| Algorithm    | Configurations                       |
+==============+======================================+
| DHM          | :option:`CONFIG_MBEDTLS_DHM_C`       |
+--------------+--------------------------------------+

.. note::
   `Arm CryptoCell cc310 backend`_  is limited to key size of <= 2048 bits.

Multiple backends
~~~~~~~~~~~~~~~~~
DHM can be enabled by setting the :option:`CONFIG_MBEDTLS_DHM_C` Kconfig variable, and one or more of the following Kconfig variables:

+--------------+--------------+--------------------------------------------------------------+
| Algorithm    | Support      | Configurations                                               |
+==============+==============+==============================================================+
| DHM          | Glue         | cc310: :option:`CONFIG_CC310_MBEDTLS_DHM_C`                  |
|              |              |                                                              |
|              |              | Original mbed TLS: :option:`CONFIG_VANILLA_MBEDTLS_DHM_C`    |
+--------------+--------------+--------------------------------------------------------------+

.. note::
   * `Arm CryptoCell cc310 backend`_  is limited to key size of <= 2048 bits.


ECC configurations
------------------
Elliptic Curve Cryptography (ECC) configuration provides support for Elliptic Curve over GF(p).
ECC core support can be enabled by setting the :option:`CONFIG_MBEDTLS_ECP_C` Kconfig variable.
Enabling :option:`CONFIG_MBEDTLS_ECP_C` will activate configuration options that depend upon ECC, such as ECDH, ECDSA, ECJPAKE, and a selection of ECC curves to support in the system.
If multiple backends are available, you can select which backend to use for :option:`CONFIG_MBEDTLS_ECP_C`.
This backend will be used to provide support for ECDH, ECDSA, and/or ECJPAKE (if enabled).

Single backend
~~~~~~~~~~~~~~
ECC core support can be enabled by setting the :option:`CONFIG_MBEDTLS_ECP_C` Kconfig variable.

Multiple backends
~~~~~~~~~~~~~~~~~
ECC core support can be enabled by setting the :option:`CONFIG_MBEDTLS_ECP_C` Kconfig variable, and one of the following Kconfig variables:

+--------------+--------------+--------------------------------------------------------------------+
| Algorithm    | Support      | Configurations                                                     |
+==============+==============+====================================================================+
| ECP          | Choice       | cc310: :option:`CONFIG_CHOICE_CC310_MBEDTLS_ECP_C`                 |
|              |              |                                                                    |
|              |              | nrf_oberon: :option:`CONFIG_CHOICE_OBERON_MBEDTLS_ECP_C`           |
|              |              |                                                                    |
|              |              | Original mbed TLS: :option:`CONFIG_CHOICE_VANILLA_MBEDTLS_ECP_C`   |
+--------------+--------------+--------------------------------------------------------------------+

.. note::
   * For features provided with `Choice`_ support, the enabled backend that is first in order is selected by default.
   * The :ref:`nrf_oberon_readme` only supports ECC curve secp256r1.


ECDH configurations
-------------------
Elliptic Curve Diffie-Hellman (ECDH) support can be enabled by setting the :option:`CONFIG_MBEDTLS_ECDH_C` Kconfig variable.

+--------------+--------------------------------------+
| Algorithm    | Configurations                       |
+==============+======================================+
| ECDH         | :option:`CONFIG_MBEDTLS_ECDH_C`      |
+--------------+--------------------------------------+

.. note::
   * ECDH support depends upon `ECC Configurations`_ being enabled.
   * The :ref:`nrf_cc310_mbedcrypto_readme` does not integrate on ECP layer.
     Only the top-level APIs for ECDH are replaced.
   * The :ref:`nrf_oberon_readme` only supports ECC curve secp256r1.


ECDSA configurations
--------------------
Elliptic Curve Digital Signature Algorithm (ECDSA) support can be enabled be configured by setting the :option:`CONFIG_MBEDTLS_ECDSA_C` Kconfig variable.

+--------------+---------------------------------------+
| Algoritm     | Configurations                        |
+==============+=======================================+
| ECDSA        | :option:`CONFIG_MBEDTLS_ECDSA_C`      |
+--------------+---------------------------------------+

.. note::
   * ECDSA support depends upon `ECC Configurations`_ being enabled.
   * The :ref:`nrf_cc310_mbedcrypto_readme` does not integrate on ECP layer.
     Only the top-level APIs for ECDSA are replaced.
   * The :ref:`nrf_oberon_readme` only supports ECC curve secp256r1.


ECJPAKE configurations
----------------------
Elliptic Curve, Password Authenticated Key Exchange by Juggling (ECJPAKE) support can be enabled by setting the :option:`CONFIG_MBEDTLS_ECJPAKE_C` Kconfig variable.

+--------------+---------------------------------------+
| Algorithm    | Configurations                        |
+==============+=======================================+
| ECJPAKE      | :option:`CONFIG_MBEDTLS_ECJPAKE_C`    |
+--------------+---------------------------------------+

.. note::
   * ECJPAKE support depends upon `ECC Configurations`_ being enabled.
   * The :ref:`nrf_oberon_readme` only supports ECC curve secp256r1.

ECC curves configurations
-------------------------
It is possible to configure the curves that should be supported in the system
depending on the backend selected.

The following table shows the curves that can be enabled.

+-----------------------------+-----------------------------------------------------+--------------------------+
| Curve                       | Configurations                                      | Note                     |
+=============================+=====================================================+==========================+
| NIST secp192r1              | :option:`CONFIG_MBEDTLS_ECP_DP_SECP192R1_ENABLED`   |                          |
+-----------------------------+-----------------------------------------------------+--------------------------+
| NIST secp224r1              | :option:`CONFIG_MBEDTLS_ECP_DP_SECP224R1_ENABLED`   |                          |
+-----------------------------+-----------------------------------------------------+--------------------------+
| NIST secp256r1              | :option:`CONFIG_MBEDTLS_ECP_DP_SECP256R1_ENABLED`   |                          |
+-----------------------------+-----------------------------------------------------+--------------------------+
| NIST secp384r1              | :option:`CONFIG_MBEDTLS_ECP_DP_SECP384R1_ENABLED`   |                          |
+-----------------------------+-----------------------------------------------------+--------------------------+
| NIST secp521r1              | :option:`CONFIG_MBEDTLS_ECP_DP_SECP521R1_ENABLED`   |                          |
+-----------------------------+-----------------------------------------------------+--------------------------+
| Koblitz secp192k1           | :option:`CONFIG_MBEDTLS_ECP_DP_SECP192K1_ENABLED`   |                          |
+-----------------------------+-----------------------------------------------------+--------------------------+
| Koblitz secp224k1           | :option:`CONFIG_MBEDTLS_ECP_DP_SECP224K1_ENABLED`   |                          |
+-----------------------------+-----------------------------------------------------+--------------------------+
| Koblitz secp256k1           | :option:`CONFIG_MBEDTLS_ECP_DP_SECP256K1_ENABLED`   |                          |
+-----------------------------+-----------------------------------------------------+--------------------------+
| Brainpool bp256r1           | :option:`CONFIG_MBEDTLS_ECP_DP_BP256R1_ENABLED`     | Original mbed TLS only   |
+-----------------------------+-----------------------------------------------------+--------------------------+
| Brainpool bp384r1           | :option:`CONFIG_MBEDTLS_ECP_DP_BP384R1_ENABLED`     | Original mbed TLS only   |
+-----------------------------+-----------------------------------------------------+--------------------------+
| Brainpool bp512r1           | :option:`CONFIG_MBEDTLS_ECP_DP_BP512R1_ENABLED`     | Original mbed TLS only   |
+-----------------------------+-----------------------------------------------------+--------------------------+
| Curve25519 / Edwards25519   | :option:`CONFIG_MBEDTLS_ECP_DP_CURVE25519_ENABLED`  |                          |
+-----------------------------+-----------------------------------------------------+--------------------------+
| Curve448 / Edwards448       | :option:`CONFIG_MBEDTLS_ECP_DP_CURVE448_ENABLED`    | Original mbed TLS only   |
+-----------------------------+-----------------------------------------------------+--------------------------+

.. note::
   * The :ref:`nrf_oberon_readme` only supports ECC curve secp256r1.
   * Choosing the nrf_oberon backend does not allow enabling the rest of the ECC curve types.


RSA configurations
------------------

Rivest-Shamir-Adleman (RSA) support can be enabled by setting Kconfig variables according to single or multiple enabled backends.

Single backend
~~~~~~~~~~~~~~
RSA support can be enabled by setting the :option:`CONFIG_MBEDTLS_RSA_C` Kconfig variable.

+--------------+---------------------------------------+
| Algorithm    | Configurations                        |
+==============+=======================================+
| RSA          | :option:`CONFIG_MBEDTLS_RSA_C`        |
+--------------+---------------------------------------+

.. note::
   `Arm CryptoCell cc310 backend`_  is limited to key sizes of <= 2048 bits.

Multiple backends
~~~~~~~~~~~~~~~~~
RSA support can be enabled by setting the :option:`CONFIG_MBEDTLS_RSA_C` Kconfig variable, and one of the following Kconfig variables:

+--------------+-----------------+--------------------------------------------------------------------+
| Algorithm    | Support         | Configurations                                                     |
+==============+=================+====================================================================+
| RSA          | Choice          | cc310: :option:`CONFIG_CHOICE_CC310_MBEDTLS_RSA_C`                 |
|              |                 |                                                                    |
|              |                 | Original mbed TLS: :option:`CONFIG_CHOICE_VANILLA_MBEDTLS_RSA_C`   |
+--------------+-----------------+--------------------------------------------------------------------+

.. note::
   * For configurations providing `Choice`_ support, the enabled backend that is first in order is selected by default.
   * `Arm CryptoCell cc310 backend`_  is limited to key sizes of <= 2048 bits.

Secure Hash configurations
--------------------------

The Secure Hash algorithms can be configured by setting Kconfig variables according to single or multiple enabled backends.

Single backend
~~~~~~~~~~~~~~

SHA support can be enabled by setting Kconfig according to the following table:

+--------------+--------------------+--------------------------------------+
| Algorith     | Support            | Backend selection                    |
+==============+====================+======================================+
| SHA-1        |                    | :option:`CONFIG_MBEDTLS_SHA1_C`      |
+--------------+--------------------+--------------------------------------+
| SHA-256      |                    | :option:`CONFIG_MBEDTLS_SHA256_C`    |
+--------------+--------------------+--------------------------------------+
| SHA-512      | Shared             | :option:`CONFIG_MBEDTLS_SHA512_C`    |
+--------------+--------------------+--------------------------------------+

Multiple backends
~~~~~~~~~~~~~~~~~

SHA-1
:::::

SHA-1 support can be enabled by setting the :option:`CONFIG_MBEDTLS_SHA1_C` Kconfig variable, and one of the following Kconfig variables:

+--------------+-----------------+--------------------------------------------------------------------+
| Algorith     | Support         | Backend selection                                                  |
+==============+=================+====================================================================+
| SHA-1        | Choice          | cc310: :option:`CONFIG_CHOICE_CC310_MBEDTLS_SHA1_C`                |
|              |                 |                                                                    |
|              |                 | nrf_oberon: :option:`CONFIG_CHOICE_OBERON_MBEDTLS_SHA1_C`          |
|              |                 |                                                                    |
|              |                 | Original mbed TLS: :option:`CONFIG_CHOICE_VANILLA_MBEDTLS_SHA1_C`  |
+--------------+-----------------+--------------------------------------------------------------------+

.. note::
   * For features provided with `Choice`_ support, the enabled backend that is first in order is selected by default.

SHA-256
:::::::

SHA-256 support can be enabled by setting the :option:`CONFIG_MBEDTLS_SHA256_C` Kconfig variable, and one of the following Kconfig variables:

+--------------+-----------------+-----------------------------------------------------------------------+
| Algorith     | Support         | Backend selection                                                     |
+==============+=================+=======================================================================+
| SHA-256      | Choice          | cc310: :option:`CONFIG_CHOICE_CC310_MBEDTLS_SHA256_C`                 |
|              |                 |                                                                       |
|              |                 | nrf_oberon: :option:`CONFIG_CHOICE_OBERON_MBEDTLS_SHA256_C`           |
|              |                 |                                                                       |
|              |                 | Original mbed TLS: :option:`CONFIG_CHOICE_VANILLA_MBEDTLS_SHA256_C`   |
+--------------+-----------------+-----------------------------------------------------------------------+

.. note::
   * For features provided with `Choice`_ support, the enabled backend that is first in order is selected by default.
   * The nrf_oberon backend does not support SHA-224.

SHA-512
:::::::

SHA-256 support can be configured by setting the :option:`CONFIG_MBEDTLS_SHA512_C` Kconfig variable.

+--------------+-----------------+-----------------------------------------------------------------+
| Algorith     | Support         | Backend selection                                               |
+==============+=================+=================================================================+
| SHA-512      | Shared          | :option:`CONFIG_MBEDTLS_SHA512_C`                               |
+--------------+-----------------+-----------------------------------------------------------------+

.. note::
   * For features provided with `Choice`_ support, the enabled backend that is first in order is selected by default.

Platform specific configurations
--------------------------------

The Kconfig integration of nrf_security module coupled with CMake sets some default configurations to ensure the backend works as expected.
This includes integration with entropy data sampled from the Arm CryptoCell cc310 hardware, as well as a way to abstract the memory management in an RTOS context.
See the :ref:`nrf_cc310_mbedcrypto_readme` documentation for details.


Advanced configuration section
------------------------------

The advanced configuration section in Kconfig can be used to fine-tune the build of specific backends.
For example, the options available in the advanced configuration section can help to reduce the memory usage and/or flash footprint of the `Original mbed TLS backend`_.

Actual size reductions depend on the option being adjusted.

Before modifying the default settings, see this `article on reducing mbed TLS memory and storage footprint`_.

.. note::
   The settings available in the advanced configuration section are not validated.
   Adjust these settings with caution.


AES tables in ROM
~~~~~~~~~~~~~~~~~

You can set the :option:`CONFIG_MBEDTLS_AES_ROM_TABLES` Kconfig variable to place the AES lookup tables in ROM instead of RAM.
This decreases the RAM usage by about 8 KB with an additional cost of about 8 KB of ROM.
Note that executing operations in ROM is slower.
If the configuration :option:`CONFIG_MBEDTLS_AES_FEWER_TABLES` is set, then the size moved from RAM to ROM is about 2 KB.

+------------------------------------------------+---------+-------+-----+
| Option                                         | Default | Min   | Max |
+================================================+=========+=======+=====+
| :option:`CONFIG_MBEDTLS_AES_ROM_TABLES`        | `n`     | `n`   | `y` |
+------------------------------------------------+---------+-------+-----+

.. note::
   This configuration is only available in `Original mbed TLS backend`_.

Fewer AES tables
~~~~~~~~~~~~~~~~

The :option:`CONFIG_MBEDTLS_AES_FEWER_TABLES` Kconfig variable controls the size of the AES lookup tables in use.
Enabling this omits about 75% of the AES tables in RAM or ROM.
If the option is enabled, AES must perform more calculations, which impacts the overall performance.

+------------------------------------------------+---------+-------+-----+
| Option                                         | Default | Min   | Max |
+================================================+=========+=======+=====+
| :option:`CONFIG_MBEDTLS_AES_FEWER_TABLES`      | `n`     | `n`   | `y` |
+------------------------------------------------+---------+-------+-----+

.. note::
   This configuration is only available in `Original mbed TLS backend`_.


Multiple Precision Integers (MPI) / Bignum calculation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The :option:`CONFIG_MBEDTLS_MPI_WINDOW_SIZE` Kconfig variable controls the window size used in mbed TLS for MPI calculations.
Reduce this value to reduce memory usage. Note that reducing this this value may have an impact on the performance.

The :option:`CONFIG_MBEDTLS_MPI_MAX_SIZE` Kconfig variable controls the maximum size of MPIs that can be used for calculation.
Reduce this value only if you are sure that the system will not need larger sizes.


+------------------------------------------------+---------+-------+------+
| Option                                         | Default | Min   | Max  |
+================================================+=========+=======+======+
| :option:`CONFIG_MBEDTLS_MPI_WINDOW_SIZE`       | 6       | 1     | 6    |
+------------------------------------------------+---------+-------+------+
| :option:`CONFIG_MBEDTLS_MPI_MAX_SIZE`          | 1024    | 0     | 1024 |
+------------------------------------------------+---------+-------+------+

.. note::
   This configuration is only available in cc310 backend and `Original mbed TLS backend`_.


Elliptic Curves
~~~~~~~~~~~~~~~

The :option:`CONFIG_MBEDTLS_ECP_MAX_BITS` Kconfig variable controls the largest elliptic curve supported in the library.

If the curves that are used are smaller than 521 bits, then this option can be reduced in order to save memory.
See `ECC curves configurations`_ for information on how to select the curves to use.
For example, if `NIST secp384r1` is the only curve enabled, then :option:`CONFIG_MBEDTLS_ECP_MAX_BITS` can be reduced to 384 bits.

The :option:`CONFIG_MBEDTLS_ECP_WINDOW_SIZE` Kconfig variable controls the window size used for elliptic curve multiplication.
This value can be reduced down to 2 to reduce memory usage.
Keep in mind that reducing the value impacts the performance of the system.

The :option:`CONFIG_MBEDTLS_ECP_FIXED_POINT_OPTIM` Kconfig variable controls ECP fixed point optimizations.
If disabled, the system uses less memory, but performance of the system is reduced.


+------------------------------------------------+---------+-------+-----+
| Option                                         | Default | Min   | Max |
+================================================+=========+=======+=====+
| :option:`CONFIG_MBEDTLS_ECP_MAX_BITS`          | 521     | 0     | 521 |
+------------------------------------------------+---------+-------+-----+
| :option:`CONFIG_MBEDTLS_ECP_WINDOW_SIZE`       | 6       | 2     | 6   |
+------------------------------------------------+---------+-------+-----+
| :option:`CONFIG_MBEDTLS_ECP_FIXED_POINT_OPTIM` | `y`     | `n`   | `y` |
+------------------------------------------------+---------+-------+-----+

.. note::
   This configuration is only available in cc310 backend and `Original mbed TLS backend`_.


SHA-256
~~~~~~~

The :option:`CONFIG_MBEDTLS_SHA256_SMALLER` Kconfig variable can be used to select a SHA-256 implementation with smaller footprint.
Such configuration reduces SHA-256 calculation performance.

For example, on a Cortex-M4, the size of :cpp:func:`mbedtls_sha256_process()` is reduced from ~2 KB to ~0.5 KB, however it also performs around 30% slower.

+------------------------------------------------+---------+-------+-----+
| Option                                         | Default | Min   | Max |
+================================================+=========+=======+=====+
| :option:`CONFIG_MBEDTLS_SHA256_SMALLER`        | `n`     | `n`   | `y` |
+------------------------------------------------+---------+-------+-----+

.. note::
   This configuration is only available in `Original mbed TLS backend`_.

SSL Configurations
~~~~~~~~~~~~~~~~~~

The :option:`CONFIG_MBEDTLS_SSL_MAX_CONTENT_LEN` Kconfig variable can be used to specify the maximum size for incoming and outgoing mbed TLS I/O buffers.
The default value is 16384 as specified in RFC5246, however if both sides are under your control, this value can safely be reduced under the following conditions:

* Both sides support the max_fragment_length SSL extension, RFC8449.
  The max_fragment_length allows for buffer reduction to less than 1 KB.
* Knowledge of the maximum size that will ever be sent in a single SSL/TLS frame.

If one of those conditions is met, the buffer size can safely be reduced to a more appropriate value for memory constrained devices.

The :option:`CONFIG_MBEDTLS_SSL_CIPHERSUITES` Kconfig variable is a custom list of cipher suites to support in SSL/TLS.
The cipher suites are provided as a comma-separated string, in order of preference.
This list can only be used for restricting cipher suites available in the system.


+------------------------------------------------+---------+-----------+-------+-------+
| Option                                         | Type    | Default   | Min   | Max   |
+================================================+=========+===========+=======+=======+
| :option:`CONFIG_MBEDTLS_SSL_MAX_CONTENT_LEN`   | Integer | 16384     | 0     | 16384 |
+------------------------------------------------+---------+-----------+-------+-------+
| :option:`CONFIG_MBEDTLS_SSL_CIPHERSUITES`      | String  | `<empty>` |       |       |
+------------------------------------------------+---------+-----------+-------+-------+

.. note::
   The string in :option:`CONFIG_MBEDTLS_SSL_CIPHERSUITES` should not be quoted.


User-provided mbed TLS config header
------------------------------------

The :ref:`nrf_security_readme` provides a Kconfig interface to control compilation and linking of mbed TLS and the :ref:`nrf_cc310_mbedcrypto_readme` library.

The Kconfig interface and build system ensures that the configuration of :ref:`nrf_security_readme` is valid and working, and ensures that dependencies between different cryptographic APIs are met.

It is therefore highly recommended to let the build system generate the mbed TLS configuration header.

However, for special use cases that cannot be achieved using the Kconfig configuration tool, it is possible to provide a custom mbed TLS configuration header.
When doing so, make sure that the system is working.

It is therefore advised to use Kconfig and the build system to create an mbed TLS configuration header as a starting point, and then tweak this file to include settings that are not available in Kconfig.

.. note::
   When providing a custom mbed TLS configuration header, it is important that the following criteria are still met:

   * Entropy length of 144, i.e. ``#define MBEDTLS_ENTROPY_MAX_GATHER 144``
   * Force SHA256


API documentation
=================

:ref:`nrf_security_api_mbedcrypto_glue`
