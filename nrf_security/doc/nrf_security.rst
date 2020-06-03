.. _nrf_security_readme:

Nordic security module
######################

The nrf_security module provides an integration between mbed TLS and software libraries that provide hardware-accelerated cryptographic functionality on selected Nordic Semiconductor SoCs.
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

Setting this variable allows for additional Kconfig variables, depending to the number of features requested.
These configurations are then used to generate an mbed TLS configuration file used during compilation.

It is possible to provide your own custom mbed TLS configuration file by deselecting the :option:`CONFIG_GENERATE_MBEDTLS_CFG_FILE` Kconfig variable.

.. note::
   Deselecting the :option:`CONFIG_GENERATE_MBEDTLS_CFG_FILE` Kconfig variable
   is not recommended.
   But if doing so, please read `user-provided mbed TLS config header`_.


nrf_security backends
=====================

The nrf_security module supports multiple enabled backends at the same time.
This mechanism is intended to extend the available feature set of hardware accelerated cryptography.
Enabling one or more backends adds more configuration options grouped into classes of cryptographic algorithms.

Note that some cryptographic features are provided as an option regardless of the backend enabled.
In such cases, the feature is compiled using standard mbed TLS.
An example of this is SHA-512, which is only accessible using standard mbed TLS code.

The configuration options added after enabling one or more backends will change based on the number of enabled backends.
Some configuration options allow for adding support from multiple backends by utilizing the mbed TLS glue layer, while other provide a selection between the enabled backends (as radio buttons).

The nrf_security module supports the following backends:

* Arm CryptoCell CC310 (in nRF52840 and nRF9160)
* nrf_oberon binary library
* Standard mbed TLS


Arm CryptoCell CC310 backend
----------------------------

The Arm CryptoCell CC310 backend is a library which provides hardware accelerated cryptography using the CC310 hardware.

The Arm CryptoCell CC310 backend is only available on the following devices:

* nRF52840
* nRF9160


Enabling the Arm CryptoCell CC310 backend
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To enable the Arm CryptoCell CC310 backend, set the :option:`CONFIG_CC310_BACKEND` Kconfig variable to true.

.. note:: This backend is only available in nRF52840 and nRF9160.


nrf_oberon backend
------------------

The :ref:`nrf_oberon_readme` library is a binary library that provides select cryptographic algorithms optimized for use in nRF devices.
This provides faster execution than standard mbed TLS implementations.

The nrf_oberon backend provides support for AES ciphers, SHA-1, SHA-256, and ECC (ECDH, ECDSA, and ECJPAKE) using secp256r1.

Enabling the nrf_oberon backend
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To use the :ref:`nrf_oberon_readme` as a backend, set the :option:`CONFIG_OBERON_BACKEND` Kconfig variable to true.


Standard mbed TLS backend
-------------------------

The standard `mbed TLS backend`_ is a software-only library provided by Arm.

The standard mbed TLS backend can be used on nRF devices that do not feature the CC310 hardware.
Alternatively, it can be used on CC310 enabled devices to add support for features not available in the `Arm CryptoCell CC310 backend`_, for example AES-256 or ECC Brainpool curves.


Enabling the standard mbed TLS backend
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To enable the standard mbed TLS backend, set the :option:`CONFIG_MBEDTLS_VANILLA_BACKEND` Kconfig variable to true.


Using the nrf_cc310_mbedcrypto as backend
-----------------------------------------

To use the :ref:`nrf_cc310_mbedcrypto_readme` as a backend, the CC310 hardware must first be initialized.

The CC310 is initialized in :file:`<NCS>/nrf/drivers/hw_cc310/hw_cc310.c` and is controlled with the `CONFIG_HW_CC310` Kconfig variable.
This variable default value is `y` when the CC310 is available.


mbed TLS glue layer
===================
The mbed TLS glue layer allows for using both the hardware-accelerated and software implementation of cryptography at the same time.
A typical use case is if the cryptographic algorithm is limited in hardware support (for example, the AES key size or supported ciphers).
In this case, the mbed TLS glue layer enables a dynamic check to verify whether the cryptographic algorithm is supported in hardware.
If it is not supported, the glue layer calls into the software implementation as a fallback.

The mbed TLS glue layer is written by Nordic Semiconductor ASA and is a part of the nrf_security module.
The mbed TLS glue layer is dependent on Kconfig variables in the |NCS| build system.

mbed TLS glue layer - support and priority
------------------------------------------

To select the backend, the mbed TLS glue layer implements a functionality check with priority reporting.
The check is performed using a `check function`.
This function is called from the API which provides configuration changes that determine the selection between the enabled backends.

.. code-block:: c
    :caption: Example: CC310 backend AES CCM support and priority check

    static int mbedtls_ccm_check(mbedtls_cipher_id_t cipher, unsigned int keybits) {
            return (keybits == 128) ? 2 : 0;
    }

In this example, the AES CCM support in the backend will report priority level 3 if the key size is 128, or 0 if the key size is different.
The CC310 backend does not support a larger key size.
If it is larger, the standard mbed TLS will be used (if the backend is enabled).

.. note::
   The check function can be called from multiple APIs in the mbed TLS glue layer.
   An example of a location that determines backend selection is an API to set an encryption key, in which case the key size may change, demanding a new selection of available and prioritized backends.

.. note::
   Hardware-accelerated cryptography through the CC310 backend will be prioritized if it is supported.
   Standard mbed TLS is only used as a fallback.


Enabling the mbed TLS glue layer
--------------------------------

The mbed TLS glue layer is automatically enabled when two backends are enabled for one or more of the following groups of algorithms:

* AES (common and CCM)
* CMAC
* DHM

.. note::
   Note that the mbed TLS glue layer will be extended in the upcoming versions.


mbed TLS glue layer mechanisms
------------------------------
The mbed TLS glue layer relies on symbol renaming of known APIs in mbed TLS to prevent collisions of identically named functions in multiple backends.
The backend implementation is reached using a table of function pointers corresponding to the renamed symbols.

.. code-block:: c
    :caption: Example: CC310 backend ECDH function table

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
There is a difference in support according to the group of cryptographic algorithms.
The configuration options listed in subsequent chapters are `Glue`, `Standard only`, or `Choice`.

Glue
~~~~
The configuration options will list `Glue` for the cryptographic algorithms that can be enabled in multiple backends at the same time.

Standard only
~~~~~~~~~~~~~
The configuration options will list `Standard only` for the cryptographic algorithms that are not currently available in hardware.
In this case, the only option to support the feature is through standard mbed TLS software implementations or through nrf_oberon.
If nrf_oberon support is present, it is shown in the next column in the table or in the notes.

Choice
~~~~~~
The configuration options will list `Choice` for the cryptographic algorithms that are supported by multiple backends, but only one of them can be enabled at the same time.
To enable the cryptographic algorithm, one of the backend defines must be enabled.

AES configuration
------------------

AES core configuration can be enabled by setting the :option:`CONFIG_MBEDTLS_AES_C` Kconfig variable.
Enabling AES core support enables AES ECB cipher mode and allows for the following ciphers to be configured: CTR, OFB, CFB, CBC, XTS, CMAC, CCM, CCM*, and GCM.

Single backend
~~~~~~~~~~~~~~
AES core support can be configured by setting the :option:`CONFIG_MBEDTLS_AES_C` Kconfig variable.

Multiple backends
~~~~~~~~~~~~~~~~~
AES core support can be configured by setting setting Kconfig variables according to the following table:

+--------------+----------------+----------------------------------------------------+
| Cipher mode  | Support        | Configurations                                     |
+==============+================+====================================================+
| ECB          | Glue           | CC310: :option:`CONFIG_CC310_MBEDTLS_AES_C`        |
|              |                |                                                    |
|              |                | nrf_oberon: :option:`CONFIG_OBERON_MBEDTLS_AES_C`  |
|              |                |                                                    |
|              |                | Standard: :option:`CONFIG_VANILLA_MBEDTLS_AES_C`   |
+--------------+----------------+----------------------------------------------------+

.. note::
   * Enabling only the CC310 backend is limited to 128 bit keys. To add support for larger key sizes as well, enable standard support.
   * Enabling the nrf_oberon backend replaces select internal APIs for block encrypt/decrypt and set key for encrypt/decrypt.


AES cipher configuration
------------------------
AES cipher mode configuration is dependent on AES core support according to `AES configuration`_.
Configure the AES cipher mode support according to single or multiple enabled backends.

Single backend
~~~~~~~~~~~~~~
AES cipher mode support can be configured according to the following table:

+--------------+----------------+---------------------------------------------+
| Cipher mode  | Note           | Configurations                              |
+==============+================+=============================================+
| CTR          |                | :option:`CONFIG_MBEDTLS_CIPHER_MODE_CTR`    |
+--------------+----------------+---------------------------------------------+
| CBC          |                | :option:`CONFIG_MBEDTLS_CIPHER_MODE_CBC`    |
+--------------+----------------+---------------------------------------------+
| CFB          |                | :option:`CONFIG_MBEDTLS_CIPHER_MODE_CFB`    |
+--------------+----------------+---------------------------------------------+
| OFB          |                | :option:`CONFIG_MBEDTLS_CIPHER_MODE_OFB`    |
+--------------+----------------+---------------------------------------------+
| XTS          | Standard only  | :option:`CONFIG_MBEDTLS_CIPHER_MODE_XTS`    |
+--------------+----------------+---------------------------------------------+
| CMAC         |                | :option:`CONFIG_MBEDTLS_CMAC_C`             |
+--------------+----------------+---------------------------------------------+

.. note::
   * Currently, AES cipher mode XTS is not supported by the CC310 backend.
   * The CC310 backend is limited to key sizes of 128 bits.


Multiple backends
~~~~~~~~~~~~~~~~~
AES cipher mode support can be configured according to the following table:

+--------------+----------------+-----------------------------------------------------------+
| Cipher mode  | Support        | Configurations                                            |
+==============+================+===========================================================+
| CTR          | Glue           | :option:`CONFIG_MBEDTLS_CIPHER_MODE_CTR`                  |
+--------------+----------------+-----------------------------------------------------------+
| CBC          | Glue           | :option:`CONFIG_MBEDTLS_CIPHER_MODE_CBC`                  |
+--------------+----------------+-----------------------------------------------------------+
| CFB          | Glue           | :option:`CONFIG_MBEDTLS_CIPHER_MODE_CFB`                  |
+--------------+----------------+-----------------------------------------------------------+
| OFB          | Glue           | :option:`CONFIG_MBEDTLS_CIPHER_MODE_OFB`                  |
+--------------+----------------+-----------------------------------------------------------+
| XTS          | Standard only  | :option:`CONFIG_MBEDTLS_CIPHER_MODE_XTS`                  |
+--------------+----------------+-----------------------------------------------------------+
| CMAC         | Choice         | CC310: :option:`CONFIG_CC310_MBEDTLS_CMAC_C` or           |
|              |                |                                                           |
|              |                | Standard: :option:`CONFIG_VANILLA_MBEDTLS_CMAC_C`         |
+--------------+----------------+-----------------------------------------------------------+

.. note::
   * The CC310 backend is limited to key sizes of 128 bits.
   * XTS will not be available if multiple backends are enabled for AES.


AEAD configurations
-------------------
Authenticated Encryption with Associated Data (AEAD) can be enabled according to the cipher in question.
Configure the AEAD cipher mode support according to single or multiple enabled backends.

Single backend
~~~~~~~~~~~~~~

AEAD cipher mode support can be configured according to the following table:

+--------------+------------------------------------+---------------------------------------+
| AEAD cipher  | Note                               | Configurations                        |
+==============+====================================+=======================================+
| AES CCM/CCM* |                                    | :option:`CONFIG_MBEDTLS_CCM_C`        |
+--------------+------------------------------------+---------------------------------------+
| AES GCM      | Standard only                      | :option:`CONFIG_MBEDTLS_GCM_C`        |
+--------------+------------------------------------+---------------------------------------+
| ChaCha20     |                                    | :option:`CONFIG_MBEDTLS_CHACHA20_C`   |
+--------------+------------------------------------+---------------------------------------+
| Poly1305     |                                    | :option:`CONFIG_MBEDTLS_POLY1305_C`   |
+--------------+------------------------------------+---------------------------------------+
| ChaCha-Poly  | Requires `Poly1305` and `ChaCha20` | :option:`CONFIG_MBEDTLS_CHACHAPOLY_C` |
+--------------+------------------------------------+---------------------------------------+

.. note::
   * AES GCM is supported by the nrf_oberon backend.
   * AEAD AES cipher modes are dependent on enabling AES core support according to `AES configuration`_.
   * The CC310 backend is limited to key sizes of 128 bits.


Multiple backends
~~~~~~~~~~~~~~~~~

AEAD cipher mode support can be configured according to the following table:

+--------------+----------------+---------------------------------------------------------+
| AEAD cipher  | Support        | Configurations                                          |
+==============+================+=========================================================+
| AES CCM      | Glue           | CC310: :option:`CONFIG_CC310_MBEDTLS_CCM_C`             |
|              |                |                                                         |
|              |                | nrf_oberon :option:`CONFIG_OBERON_MBEDTLS_CCM_C`        |
|              |                |                                                         |
| AES CCM*     |                | Standard: :option:`CONFIG_VANILLA_MBEDTLS_CCM_C`        |
+--------------+----------------+---------------------------------------------------------+
| AES GCM      | Standard only  | :option:`CONFIG_MBEDTLS_GCM_C`                          |
+--------------+----------------+---------------------------------------------------------+
| ChaCha20     | Choice         | CC310: :option:`CONFIG_CC310_MBEDTLS_CHACHA20_C` or     |
|              |                |                                                         |
|              |                | Standard: :option:`CONFIG_VANILLA_MBEDTLS_CHACHA20_C`   |
+--------------+----------------+---------------------------------------------------------+
| Poly1305     | Choice         | CC310: :option:`CONFIG_CC310_MBEDTLS_POLY1305_C`        |
|              |                |                                                         |
|              |                | Standard: :option:`CONFIG_VANILLA_MBEDTLS_POLY1305_C`   |
+--------------+----------------+---------------------------------------------------------+
| ChaCha-Poly  | Choice         | CC310: :option:`CONFIG_VANILLA_MBEDTLS_CHACHAPOLY_C`    |
|              |                |                                                         |
|              |                | Standard: :option:`CONFIG_VANILLA_MBEDTLS_CHACHAPOLY_C` |
+--------------+----------------+---------------------------------------------------------+

.. note::
   * AEAD AES cipher modes are dependent on AES core support according to `AES configuration`_.
   * The CC310 backend is limited to key sizes of 128 bits.


DHM configurations
------------------
The Diffie-Hellman-Merkel (DHM) module can be configured by setting Kconfig variables according to single or multiple enabled backends.

Single backend
~~~~~~~~~~~~~~

DHM support can be configured according to the following table:

+--------------+----------------+---------------------------------------+
| Module       | Note           | Configurations                        |
+==============+================+=======================================+
| DHM          |                | :option:`CONFIG_MBEDTLS_DHM_C`        |
+--------------+----------------+---------------------------------------+

.. note::
   The CC310 backend is limited to key size of <= 2048 bits.

Multiple backends
~~~~~~~~~~~~~~~~~

DHM support can be configured according to the following table:

+--------------+----------------+-------------------------------------------------------+
| Module       | Support        | Configurations                                        |
+==============+================+=======================================================+
| DHM          | Glue           | CC310: :option:`CONFIG_CC310_MBEDTLS_DHM_C`           |
|              |                |                                                       |
|              |                | Standard: :option:`CONFIG_VANILLA_MBEDTLS_DHM_C`      |
+--------------+----------------+-------------------------------------------------------+

.. note::
   The CC310 backend is limited to key size of <= 2048 bits.


ECC configurations
------------------
Elliptic Curve Cryptography (ECC) configuration provides support for Elliptic Curve over GF(p) library from CC310, nrf_oberon, or  mbed TLS standard library.
ECC can be enabled by setting the :option:`CONFIG_MBEDTLS_ECP_C` Kconfig variable.
Enabling :option:`CONFIG_MBEDTLS_ECP_C` will activate configuration options that depend upon ECC, such as ECDH, ECDSA, ECJPAKE, and a selection of ECC curves to support in the system.
If multiple backends are available, you can select which backend to use for :option:`CONFIG_MBEDTLS_ECP_C`.
This backend will be used to provide support for ECDH, ECDSA, and/or ECJPAKE (if enabled).

Single backend
~~~~~~~~~~~~~~
Elliptic Curve over GF(p) provides core support for Elliptic Curve Cryptography and can be configured by setting the :option:`CONFIG_MBEDTLS_ECP_C` Kconfig variable.

Multiple backends
~~~~~~~~~~~~~~~~~
When multiple backends are available, you can configure which backend to use for Elliptic Curve over GF(p) according to the following table.

+--------------+----------------+--------------------------------------------------+
| Module       | Support        | Configurations                                   |
+==============+================+==================================================+
| ECP          | Choice         | CC310: :option:`CONFIG_CC310_MBEDTLS_ECP_C`      |
|              |                |                                                  |
|              |                | nrf_oberon: :option:`CONFIG_OBERON_MBEDTLS_ECP_C`|
|              |                |                                                  |
|              |                | Standard: :option:`CONFIG_VANILLA_MBEDTLS_ECP_C` |
+--------------+----------------+--------------------------------------------------+

.. note::
   The :ref:`nrf_oberon_readme` only supports ECC curve secp256r1.

ECDH configurations
-------------------
Elliptic Curve Diffie-Hellman (ECDH) support can be configured by setting a Kconfig variable.
ECDH support depends upon `ECC Configurations`_ being enabled.

Enabling ECDH support
~~~~~~~~~~~~~~~~~~~~~
ECDH support can be configured by setting the :option:`CONFIG_MBEDTLS_ECDH_C` Kconfig variable.

+--------------+----------------+-----------------------------------+
| Module       | Note           | Configurations                    |
+==============+================+===================================+
| ECDH         |                | :option:`CONFIG_MBEDTLS_ECDH_C`   |
+--------------+----------------+-----------------------------------+

.. note::
   * The :ref:`nrf_cc310_mbedcrypto_readme` does not integrate on ECP layer.
     Only the top-level APIs for ECDH are replaced.
   * The :ref:`nrf_oberon_readme` only supports ECC curve secp256r1.


ECDSA configurations
--------------------

The Elliptic Curve Digital Signature Algorithm (ECDSA) support can be configured by setting a Kconfig variable.
ECDSA support depends upon `ECC Configurations`_ being enabled.

Enabling ECDSA support
~~~~~~~~~~~~~~~~~~~~~~
ECDSA support can be configured by setting Kconfig variables according to the following table:

+--------------+----------------+---------------------------------------+
| Module       | Note           | Configurations                        |
+==============+================+=======================================+
| ECDSA        |                | :option:`CONFIG_MBEDTLS_ECDSA_C`      |
+--------------+----------------+---------------------------------------+

.. note::
   * The :ref:`nrf_cc310_mbedcrypto_readme` does not integrate on ECP layer.
     Only the top-level APIs for ECDSA are replaced.
   * The :ref:`nrf_oberon_readme` only supports ECC curve secp256r1.


ECJPAKE configurations
----------------------

The Elliptic Curve, Password Authenticated Key Exchange by Juggling (ECJPAKE)
support can be configured by setting Kconfig a variable.
ECJPAKE support depends upon `ECC Configurations`_ being enabled.

Enabling ECJPAKE support
~~~~~~~~~~~~~~~~~~~~~~~~
ECJPAKE support can be configured by setting the :option:`CONFIG_MBEDTLS_ECJPAKE_C` Kconfig variable.

+--------------+----------------+---------------------------------------+
| Module       | Note           | Configurations                        |
+==============+================+=======================================+
| ECJPAKE      |                | :option:`CONFIG_MBEDTLS_ECJPAKE_C`    |
+--------------+----------------+---------------------------------------+

.. note::
   The :ref:`nrf_oberon_readme` only supports ECC curve secp256r1.

ECC curves configurations
-------------------------
It is possible to configure the curves that should be supported in the system
depending on the backend selected.

The following table shows the curves that can be configured.

+----------------------------+----------------+------------------------------------------------------+
| Curve                      | Note           | Configurations                                       |
+============================+================+======================================================+
| NIST secp192r1             |                | :option:`CONFIG_MBEDTLS_ECP_DP_SECP192R1_ENABLED`    |
+----------------------------+----------------+------------------------------------------------------+
| NIST secp224r1             |                | :option:`CONFIG_MBEDTLS_ECP_DP_SECP224R1_ENABLED`    |
+----------------------------+----------------+------------------------------------------------------+
| NIST secp256r1             |                | :option:`CONFIG_MBEDTLS_ECP_DP_SECP256R1_ENABLED`    |
+----------------------------+----------------+------------------------------------------------------+
| NIST secp384r1             |                | :option:`CONFIG_MBEDTLS_ECP_DP_SECP384R1_ENABLED`    |
+----------------------------+----------------+------------------------------------------------------+
| NIST secp521r1             |                | :option:`CONFIG_MBEDTLS_ECP_DP_SECP521R1_ENABLED`    |
+----------------------------+----------------+------------------------------------------------------+
| Koblitz secp192k1          |                | :option:`CONFIG_MBEDTLS_ECP_DP_SECP192K1_ENABLED`    |
+----------------------------+----------------+------------------------------------------------------+
| Koblitz secp224k1          |                | :option:`CONFIG_MBEDTLS_ECP_DP_SECP224K1_ENABLED`    |
+----------------------------+----------------+------------------------------------------------------+
| Koblitz secp256k1          |                | :option:`CONFIG_MBEDTLS_ECP_DP_SECP256K1_ENABLED`    |
+----------------------------+----------------+------------------------------------------------------+
| Brainpool bp256r1          | Standard Only  | :option:`CONFIG_MBEDTLS_ECP_DP_BP256R1_ENABLED`      |
+----------------------------+----------------+------------------------------------------------------+
| Brainpool bp384r1          | Standard Only  | :option:`CONFIG_MBEDTLS_ECP_DP_BP384R1_ENABLED`      |
+----------------------------+----------------+------------------------------------------------------+
| Brainpool bp512r1          | Standard Only  | :option:`CONFIG_MBEDTLS_ECP_DP_BP512R1_ENABLED`      |
+----------------------------+----------------+------------------------------------------------------+
| Curve25519 / Edwards25519  |                | :option:`CONFIG_MBEDTLS_ECP_DP_CURVE25519_ENABLED`   |
+----------------------------+----------------+------------------------------------------------------+
| Curve448 / Edwards448      | Standard Only  | :option:`CONFIG_MBEDTLS_ECP_DP_CURVE448_ENABLED`     |
+----------------------------+----------------+------------------------------------------------------+

.. note::
   * For the curves available in `Standard Only` mbed TLS to ba available then all enabled ECC based features must use mbed TLS as backend. That is: ECDH, ECDSA, ECJPAKE.
   * The :ref:`nrf_oberon_readme` only supports ECC curve secp256r1.
     Choosing the nrf_oberon backend does not allow enabling the rest of the ECC curve types.



RSA configurations
------------------

The Rivest-Shamir-Adleman (RSA) cryptosystem can be configured by setting Kconfig variables according to single or multiple enabled backends.

Single backend
~~~~~~~~~~~~~~

RSA support can be configured by setting Kconfig variables according to the following table:

+--------------+----------------+---------------------------------------+
| Module       | Note           | Configurations                        |
+==============+================+=======================================+
| RSA          |                | :option:`CONFIG_MBEDTLS_RSA_C`        |
+--------------+----------------+---------------------------------------+

.. note::
   The CC310 backend is limited to key sizes of <= 2048 bits.

Multiple backends
~~~~~~~~~~~~~~~~~

RSA support can be configured by setting Kconfig according to the following table:

+--------------+----------------+-------------------------------------------------------+
| Module       | Support        | Configurations                                        |
+==============+================+=======================================================+
| RSA          | Choice         | CC310: :option:`CONFIG_CC310_MBEDTLS_RSA_C` or        |
|              |                |                                                       |
|              |                | Standard: :option:`CONFIG_VANILLA_MBEDTLS_RSA_C`      |
+--------------+----------------+-------------------------------------------------------+

.. note::
   The CC310 backend is limited to key sizes of <= 2048 bits.

Secure Hash configurations
--------------------------

The Secure Hash algorithms can be configured by setting Kconfig variables according to single or multiple enabled backends.

Single backend
~~~~~~~~~~~~~~

SHA support can be configured by setting Kconfig according to the following table:

+--------------+-------------------+------------------------------------+
| Algorith     | Note              | Backend selection                  |
+==============+===================+====================================+
| SHA-1        |                   | :option:`CONFIG_MBEDTLS_SHA1_C`    |
+--------------+-------------------+------------------------------------+
| SHA-256      |                   | :option:`CONFIG_MBEDTLS_SHA256_C`  |
+--------------+-------------------+------------------------------------+
| SHA-512      | Standard only     | :option:`CONFIG_MBEDTLS_SHA512_C`  |
+--------------+-------------------+------------------------------------+

Multiple backends
~~~~~~~~~~~~~~~~~

SHA support can be configured by setting Kconfig according to the following table:

+--------------+-------------------+-----------------------------------------------------+
| Algorith     | Configuration     | Backend selection                                   |
+==============+===================+=====================================================+
| SHA-1        | Choice            | CC310: :option:`CONFIG_CC310_MBEDTLS_SHA1_C` or     |
|              |                   |                                                     |
|              |                   | nrf_oberon: :option:`CONFIG_OBERON_MBEDTLS_SHA1_C`  |
|              |                   |                                                     |
|              |                   | Standard: :option:`CONFIG_VANILLA_MBEDTLS_SHA1_C`   |
+--------------+-------------------+-----------------------------------------------------+
| SHA-256      | Choice            | CC310: :option:`CONFIG_CC310_MBEDTLS_SHA256_C` or   |
|              |                   |                                                     |
|              |                   | nrf_oberon: :option:`CONFIG_OBERON_MBEDTLS_SHA256_C`|
|              |                   |                                                     |
|              |                   | Standard: :option:`CONFIG_VANILLA_MBEDTLS_SHA256_C` |
+--------------+-------------------+-----------------------------------------------------+
| SHA-512      | Standard only     | :option:`CONFIG_MBEDTLS_SHA512_C`                   |
+--------------+-------------------+-----------------------------------------------------+


Platform specific configurations
--------------------------------

The Kconfig integration of nrf_security module coupled with CMake sets some default configurations to ensure the backend works as expected.
This includes integration with entropy data sampled from the Arm CC310 hardware, as well as a way to abstract the memory management in an RTOS context.
See the :ref:`nrf_cc310_mbedcrypto_readme` documentation for details.


Advanced configuration section
------------------------------

The advanced configuration section in Kconfig can be used to fine-tune the build of the `standard mbed TLS backend`_ library.
For example, the options available in the advanced configuration section can help to reduce the memory usage and flash footprint of the library.

Actual size reductions depend on the option being adjusted.
They also depend on whether `standard mbed TLS backend`_ is the only backend enabled, or whether the `mbed TLS glue layer`_ is used as well.

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
