.. _nrf_security_readme:

Nordic security module
######################

The nrf_security module provides an integration between mbed TLS and software libraries that provide hardware-accelerated cryptographic functionality on selected Nordic Semiconductor SoCs.
This module includes an mbed TLS glue layer to enable both hardware-accelerated and standard mbed TLS software implementation at the same time.

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


Enabling backends
=================

The nrf_security module supports multiple enabled backends at the same time.
This is done to extend the feature set of hardware accelerated cryptography in case it is limited.
Enabling one or more backends adds more configuration options grouped into classes of cryptographic algorithms.

Note that configuration options added after enabling one or more backends will change based on the number of enabled backends.
Some configuration options allow for adding support from multiple backends by utilizing the mbed TLS glue layer, while other provide a selection between the enabled backends (as radio buttons).

The nrf_security module supports two backends:
* Arm CryptoCell CC310 (in nRF52840 and nRF9160)
* Standard mbed TLS

Enabling the Arm CryptoCell CC310 backend
-----------------------------------------

To enable the Arm CryptoCell CC310 backend, set the :option:`CONFIG_CC310_BACKEND` Kconfig variable to true.

.. note:: This backend is only available in nRF52840 and nRF9160.

Enabling the standard mbed TLS backend
--------------------------------------

To enable the standard mbed TLS backend, set the :option:`CONFIG_MBEDTLS_VANILLA_BACKEND` Kconfig variable to true.


Using the nrf_cc310_mbedcrypto as backend
-----------------------------------------

To use the :ref:`nrf_cc310_mbedcrypto_readme` as a backend, it must be initialized using the standard mbed TLS APIs for platform abstraction:

.. code-block:: c
    :caption: Initializing the nrf_cc310_mbedcrypto backend
	
    static mbedtls_platform_context platform_context = {0};
    int ret;
    ret = mbedtls_platform_set_calloc_free(alloc_fn, free_fn);
    if (ret != 0) {
            /* Failed to set the alternative calloc/free */
            return ret;
    }
    
    ret = mbedtls_platform_setup(&platform_context);
    if (ret != 0) {
            /* Failed to initialize nrf_cc310_mbedcrypto platform */
            return ret,
    }


mbed TLS glue layer
===================
The mbed TLS glue layer allows for using both the hardware-accelerated and software implementation of cryptography at the same time.
A typical use case is if the cryptographic algorithm is limited in hardware support (for example, the AES key size or supported ciphers).
In this case, the mbed TLS glue layer enables a dynamic check to verify whether the cryptographic algorithm is supported in hardware.
If it is not supported, the glue layer calls into the software implementation as a fallback.

The mbed TLS glue layer is written by Nordic Semiconductor ASA and is a part of the nrf_security module.
The mbed TLS glue layer is dependent on Kconfig variables in the nRF Connect SDK build system.

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

In this example, the AES CCM support in the backend will report priority level 2 if the key size is 128, or 0 if the key size is different.
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
* ECDH/ECDSA

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
In this case, the only option to support the feature is through standard mbed TLS software implementations.

Choice
~~~~~~
The configuration options will list `Choice` for the cryptographic algorithms that are supported by multiple backends, but only one of them can be enabled at the same time.
To enable the cryptographic algorithm, one of the backend defines must be enabled.

AES configuration
------------------
AES configurations must be made depending if a single or multiple backends are used.
Enabling AES core support enables AES ECB cipher mode and allows for the following ciphers to be configured: CTR, OFB, CFB, CBC, XTS, CMAC, CCM, CCM*, and GCM.

Single backend
~~~~~~~~~~~~~~
AES core support can be configured by setting the :option:`CONFIG_MBEDTLS_AES_C` Kconfig variable.

Multiple backends
~~~~~~~~~~~~~~~~~
AES core support can be configured by setting setting Kconfig variables according to the following table:

+--------------+----------------+-------------------------------------------------+
| Cipher mode  | Support        | Configurations                                  |
+==============+================+=================================================+
| ECB          | Glue           | CC310: :option:`CONFIG_CC310_MBEDTLS_AES_C`     |
|              |                |                                                 |
|              |                | Standard: :option:`CONFIG_VANILLA_MBEDTLS_AES_C`|
+--------------+----------------+-------------------------------------------------+

.. note::
   Enabling only the CC310 backend is limited to 128 bit keys. To add support for larger key sizes as well, enable standard support.

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
| CFB          | Standard only  | :option:`CONFIG_MBEDTLS_CIPHER_MODE_CFB`    |
+--------------+----------------+---------------------------------------------+
| OFB          | Standard only  | :option:`CONFIG_MBEDTLS_CIPHER_MODE_OFB`    |
+--------------+----------------+---------------------------------------------+
| XTS          | Standard only  | :option:`CONFIG_MBEDTLS_CIPHER_MODE_XTS`    |
+--------------+----------------+---------------------------------------------+
|CMAC          |                | :option:`CONFIG_MBEDTLS_CMAC_C`             |
+--------------+----------------+---------------------------------------------+

.. note::
   Currently, AES cipher modes OFB, CFB, and XTS are only supported by standard mbed TLS.

.. note::
   The CC310 backend is limited to key sizes of 128 bits.

Multiple backends
~~~~~~~~~~~~~~~~~
AES cipher mode support can be configured according to the following table:

+--------------+----------------+-----------------------------------------------------------+
| Cipher mode  | Support        | Configurations                                            |
+==============+================+===========================================================+
| CTR          | Glue           | CC310: :option:`CONFIG_CC310_MBEDTLS_CIPHER_MODE_CTR`     |
|              |                |                                                           |
|              |                | Standard: :option:`CONFIG_VANILLA_MBEDTLS_CIPHER_MODE_CTR`|
+--------------+----------------+-----------------------------------------------------------+
| CBC          | Glue           | CC310: :option:`CONFIG_CC310_MBEDTLS_CIPHER_MODE_CBC`     |
|              |                |                                                           |
|              |                | Standard: :option:`CONFIG_VANILLA_MBEDTLS_CIPHER_MODE_CBC`|
+--------------+----------------+-----------------------------------------------------------+
| CFB          | Standard only  | :option:`CONFIG_MBEDTLS_CIPHER_MODE_CFB`                  |
+--------------+----------------+-----------------------------------------------------------+
| OFB          | Standard only  | :option:`CONFIG_MBEDTLS_CIPHER_MODE_OFB`                  |
+--------------+----------------+-----------------------------------------------------------+
| XTS          | Standard only  | :option:`CONFIG_MBEDTLS_CIPHER_MODE_XTS`                  |
+--------------+----------------+-----------------------------------------------------------+
| CMAC         | Choice         | CC310: :option:`CONFIG_CC310_MBEDTLS_CMAC_C` or           |
|              |                |                                                           |
|              |                | Standard: :option:`CONFIG_VANILLA_MBEDTLS_CMAC_C`         |
+--------------+----------------+-----------------------------------------------------------+

.. note::
   The CC310 backend is limited to key sizes of 128 bits.

AEAD configurations
-------------------
Authenticated Encryption with Associated Data (AEAD) can be enabled according to the cipher in question.
Configure the AEAD cipher mode support according to single or multiple enabled backends.

Single backend
~~~~~~~~~~~~~~

AEAD cipher mode support can be configured according to the following table:

+--------------+----------------+---------------------------------------+
| AEAD cipher  | Note           | Configurations                        |
+==============+================+=======================================+
| AES CCM/CCM* |                | :option:`CONFIG_MBEDTLS_CCM_C`        |
+--------------+----------------+---------------------------------------+
| AES GCM      | Standard only  | :option:`CONFIG_MBEDTLS_GCM_C`        |
+--------------+----------------+---------------------------------------+
| ChaCha-Poly  | Standard only  | :option:`CONFIG_MBEDTLS_CHACHA20_C`   |
+--------------+----------------+---------------------------------------+

.. note::
   AEAD AES cipher modes are dependent on enabling AES core support according to `AES configuration`_.

.. note::
   The CC310 backend is limited to key sizes of 128 bits.

Multiple backends
~~~~~~~~~~~~~~~~~

AEAD cipher mode support can be configured according to the following table:

+--------------+----------------+-------------------------------------------------------+
| AEAD cipher  | Support        | Configurations                                        |
+==============+================+=======================================================+
| AES CCM      | Glue           | CC310: :option:`CONFIG_CC310_MBEDTLS_CCM_C`           |
|              |                |                                                       |
| AES CCM*     |                | Standard: :option:`CONFIG_VANILLA_MBEDTLS_CCM_C`      |
+--------------+----------------+-------------------------------------------------------+
| AES GCM      | Standard only  | :option:`CONFIG_MBEDTLS_GCM_C`                        |
+--------------+----------------+-------------------------------------------------------+
| ChaCha-Poly  | Standard only  | :option:`CONFIG_MBEDTLS_CHACHA20_C`                   |
+--------------+----------------+-------------------------------------------------------+

.. note::
   AEAD AES cipher modes are dependent on AES core support according to `AES configuration`_.

.. note::
   The CC310 backend is limited to key sizes of 128 bits.


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

ECDH configurations
-------------------
Elliptic Curve Diffie-Hellman (ECDH) support can be configured by setting Kconfig variables according to single or multiple enabled backends.

Single backend
~~~~~~~~~~~~~~
ECDH support can be configured by setting the :option:`CONFIG_MBEDTLS_ECDH_C` Kconfig variable.

+--------------+----------------+-----------------------------------+
| Module       | Note           | Configurations                    |
+==============+================+===================================+
| ECDH         |                | :option:`CONFIG_MBEDTLS_ECDH_C`   |
+--------------+----------------+-----------------------------------+

.. note::
   The :ref:`nrf_cc310_mbedcrypto_readme` does not integrate on ECP layer.
   Only the top-level APIs for ECDH are replaced.

Multiple backends
~~~~~~~~~~~~~~~~~
ECDH support can be configured by setting Kconfig according to the following table:

+--------------+----------------+-------------------------------------------------------+
| Module       | Support        | Configurations                                        |
+==============+================+=======================================================+
| ECDH         | Glue           | CC310: :option:`CONFIG_CC310_MBEDTLS_ECDH_C`          |
|              |                |                                                       |
|              |                | Standard: :option:`CONFIG_VANILLA_MBEDTLS_ECDH_C`     |
+--------------+----------------+-------------------------------------------------------+

.. note::
   The :ref:`nrf_cc310_mbedcrypto_readme` does not integrate on ECP layer.
   Only the top-level APIs for ECDH are replaced.


ECDSA configurations
--------------------

The Elliptic Curve Digital Signature Algorithm (ECDSA) support can be configured by setting Kconfig variables according to single or multiple enabled backends.

Single backend
~~~~~~~~~~~~~~
ECDSA support can be configured by setting Kconfig  variables according to the following table:

+--------------+----------------+---------------------------------------+
| Module       | Note           | Configurations                        |
+==============+================+=======================================+
| ECDSA        |                | :option:`CONFIG_MBEDTLS_ECDSA_C`      |
+--------------+----------------+---------------------------------------+

.. note::
   The :ref:`nrf_cc310_mbedcrypto_readme` does not integrate on ECP layer.
   Only the top-level APIs for ECDSA are replaced.

Multiple backends
~~~~~~~~~~~~~~~~~
ECDSA support can be configured by setting Kconfig variables according to the following table:

+--------------+----------------+-------------------------------------------------------+
| Module       | Support        | Configurations                                        |
+==============+================+=======================================================+
| ECDSA        | Glue           | CC310: :option:`CONFIG_CC310_MBEDTLS_ECDSA_C`         |
|              |                |                                                       |
|              |                | Standard: :option:`CONFIG_VANILLA_MBEDTLS_ECDSA_C`    |
+--------------+----------------+-------------------------------------------------------+

.. note::
   The :ref:`nrf_cc310_mbedcrypto_readme` does not integrate on ECP layer.
   Only the top-level APIs for ECDSA are replaced.


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

+--------------+-------------------+----------------------------------------------------+
| Algorith     | Configuration     | Backend selection                                  |
+==============+===================+====================================================+
| SHA-1        | Choice            | CC310: :option:`CONFIG_CC310_MBEDTLS_SHA1_C` or    |
|              |                   |                                                    |
|              |                   | Standard: :option:`CONFIG_VANILLA_MBEDTLS_SHA1_C`  |
+--------------+-------------------+----------------------------------------------------+
| SHA-256      | Choice            | CC310: :option:`CONFIG_CC310_MBEDTLS_SHA256_C` or  |
|              |                   |                                                    |
|              |                   | Standard: :option:`CONFIG_VANILLA_MBEDTLS_SHA256_C`|
+--------------+-------------------+----------------------------------------------------+
| SHA-512      | Standard only     | :option:`CONFIG_MBEDTLS_SHA512_C`                  |
+--------------+-------------------+----------------------------------------------------+


Platform specific configurations
--------------------------------

The Kconfig integration of nrf_security module coupled with CMake sets some default configurations to ensure the backend works as expected.
This includes integration with entropy data sampled from the Arm CC310 hardware, as well as a way to abstract the memory management in an RTOS context.
See the :ref:`nrf_cc310_mbedcrypto_readme` documentation for details.

API documentation
=================

:ref:`nrf_security_api_mbedcrypto_glue`
