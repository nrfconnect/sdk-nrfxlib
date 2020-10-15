.. _nrf_security_backend_config:

Backend configurations
######################

.. contents::
   :local:
   :depth: 2

This section covers the configurations available when one or more nrf_security backends are enabled.
This includes linking directly to the backend library or utilizing the mbed TLS glue layer.

.. _nrf_security_backend_config_multiple:

Configuring multiple backends
*****************************

Different backends can support different cryptographic algorithms.
The configuration options listed in subsequent sections are either *Glue*, *Shared*, or *Choice*.

Glue
   The configuration options will list `Glue` for the cryptographic algorithms that can be enabled in multiple backends at the same time.
Shared
   For some cryptographic features, the implementation is only available in the :ref:`nrf_security_backends_orig_mbedtls`.
   For convenience, this is made available in any backend, although the implementation will always use open-source code from the Arm mbed TLS project.
Choice
   The configuration options will list `Choice` for the cryptographic algorithms that are supported by multiple backends, but only one of them can be enabled at the same time.
   To enable the cryptographic algorithm, a `base` configuration must be enabled, and then the backend can be selected using a setting prefixed with ``CONFIG_CHOICE_``.

If only a subset of the backends supports a given feature, this information is provided in the tables.

.. note::
   The first ordered item in the list of available choices is selected by default.


AES configuration
*****************

AES core support can be enabled by setting the :option:`CONFIG_MBEDTLS_AES_C` Kconfig variable.
Enabling AES core support enables AES ECB cipher mode and allows for the following ciphers to be configured: CTR, OFB, CFB, CBC, XTS, CMAC, CCM/CCM*, and GCM.

Single backend
==============

AES core support can be enabled by setting the :option:`CONFIG_MBEDTLS_AES_C` Kconfig variable.

+--------------+-----------------------------------+
| Cipher mode  | Configurations                    |
+==============+===================================+
| ECB          | :option:`CONFIG_MBEDTLS_AES_C`    |
+--------------+-----------------------------------+

.. note::
   * The :ref:`nrf_security_backends_cc3xx` is limited to key sizes of 128 bits on devices with Arm CryptoCell cc310.

Multiple backends
=================

AES core support can be enabled by setting setting the :option:`CONFIG_MBEDTLS_AES_C` Kconfig variable, and one or more of the following Kconfig variables:

+--------------+----------------+------------------------------------------------------------+
| Cipher mode  | Support        | Configurations                                             |
+==============+================+============================================================+
| ECB          | Glue           | cc3xx: :option:`CONFIG_CC3XX_MBEDTLS_AES_C`                |
|              |                |                                                            |
|              |                | nrf_oberon: :option:`CONFIG_OBERON_MBEDTLS_AES_C`          |
|              |                |                                                            |
|              |                | Original mbed TLS: :option:`CONFIG_VANILLA_MBEDTLS_AES_C`  |
+--------------+----------------+------------------------------------------------------------+

.. note::
   * The :ref:`nrf_security_backends_cc3xx` is limited to key sizes of 128 bits on devices with Arm CryptoCell cc310.
   * Enabling the :ref:`nrf_security_backends_oberon` replaces select internal APIs for AES block encrypt/decrypt and set key operations for encrypt/decrypt.
   * If both nrf_oberon backend and :ref:`nrf_security_backends_orig_mbedtls` are enabled, the implementation from
     nrf_oberon backend will provide support for AES ECB.


AES cipher configuration
************************

AES cipher mode configuration is dependent on AES core support according to `AES configuration`_.
Configure the AES cipher mode support according to single or multiple enabled backends.

Single or multiple backends
===========================

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
   * The :ref:`nrf_security_backends_cc3xx` is limited to key sizes of 128 bits on devices with Arm CryptoCell cc310.
   * Currently, AES cipher modes CFB, OFB, and XTS are not supported by the Arm CryptoCell cc3xx backend.
   * XTS will not be available if multiple backends are enabled for AES.
   * If both :ref:`nrf_security_backends_oberon` and :ref:`nrf_security_backends_orig_mbedtls` is enabled, the implementation from
     nrf_oberon backend will provide support for AES cipher modes.


CMAC configuration
******************

Cipher-based Message Authentication Code (CMAC) support can be enabled by setting Kconfig variables according to single or multiple enabled backends.

Single backend
==============

CMAC can be enabled by setting the :option:`CONFIG_MBEDTLS_CMAC_C` Kconfig variable.

+--------------+-----------------------------------+
| Algorithm    | Configurations                    |
+==============+===================================+
| CMAC         | :option:`CONFIG_MBEDTLS_CMAC_C`   |
+--------------+-----------------------------------+

Multiple backends
=================

CMAC can be enabled by setting the :option:`CONFIG_MBEDTLS_CMAC_C` Kconfig variable, and one or more of the following Kconfig variables:

+--------------+-----------------------------+--------------------------------------------------------------------+
| Algorithm    | Support                     | Configurations                                                     |
+==============+=============================+====================================================================+
| CMAC         | Choice                      | cc3xx: :option:`CONFIG_CHOICE_CC3XX_MBEDTLS_CMAC_C`                |
|              |                             |                                                                    |
|              |                             | nrf_oberon: :option:`CONFIG_CHOICE_OBERON_MBEDTLS_CMAC_C`          |
|              |                             |                                                                    |
|              |                             | Original mbed TLS: :option:`CONFIG_CHOICE_VANILLA_MBEDTLS_CMAC_C`  |
+--------------+-----------------------------+--------------------------------------------------------------------+

.. note::
   * For features provided with :ref:`Choice<nrf_security_backend_config_multiple>` support, the enabled backend that is first in order is selected by default.
   * The :ref:`nrf_security_backends_cc3xx` is limited to key sizes of 128 bits on devices with Arm CryptoCell cc310.

AEAD configurations
*******************

Authenticated Encryption with Associated Data (AEAD) can be enabled according to the cipher in question.
AEAD cipher mode support support can be enabled by setting Kconfig variables according to single or multiple enabled backends.

Single backend
==============

AEAD cipher mode support can be enabled by setting one or more of the following Kconfig variables:

+--------------+-----------------------------------------+-----------------------------------------+
| AEAD cipher  | Configurations                          | Note                                    |
+==============+=========================================+=========================================+
| AES CCM/CCM* | :option:`CONFIG_MBEDTLS_CCM_C`          |                                         |
+--------------+-----------------------------------------+-----------------------------------------+
| AES GCM      | :option:`CONFIG_MBEDTLS_GCM_C`          | Original mbed TLS, or nrf_oberon,       |
|              |                                         | or cc312                                |
+--------------+-----------------------------------------+-----------------------------------------+
| ChaCha20     | :option:`CONFIG_MBEDTLS_CHACHA20_C`     |                                         |
+--------------+-----------------------------------------+-----------------------------------------+
| Poly1305     | :option:`CONFIG_MBEDTLS_POLY1305_C`     |                                         |
+--------------+-----------------------------------------+-----------------------------------------+
| ChaCha-Poly  | :option:`CONFIG_MBEDTLS_CHACHAPOLY_C`   | Requires `Poly1305` and `ChaCha20`      |
+--------------+-----------------------------------------+-----------------------------------------+

.. note::
   * AEAD AES cipher modes are dependent on enabling AES core support according to `AES configuration`_.
   * The :ref:`nrf_security_backends_cc3xx` is limited to key sizes of 128 bits on devices with Arm CryptoCell cc310.
   * The ChaCha-Poly implemented by the Arm CryptoCell cc3xx backend does not support incremental operations.


Multiple backends
=================

Multiple backend configurations for various AEAD cyphers are presented in the following tables.

AES CCM/CCM*
------------

AES CCM/CCM* can be enabled by setting the :option:`CONFIG_MBEDTLS_CCM_C` Kconfig variable, and one or more of the following Kconfig variables:

+--------------+-----------------+-----------------------------------------------------------------+
| AEAD cipher  | Support         | Configurations                                                  |
+==============+=================+=================================================================+
| AES CCM/CCM* | Glue            | cc3xx: :option:`CONFIG_CC3XX_MBEDTLS_CCM_C`                     |
|              |                 |                                                                 |
|              |                 | nrf_oberon: :option:`CONFIG_OBERON_MBEDTLS_CCM_C`               |
|              |                 |                                                                 |
|              |                 | Original mbed TLS: :option:`CONFIG_VANILLA_MBEDTLS_CCM_C`       |
+--------------+-----------------+-----------------------------------------------------------------+

.. note::
   * AEAD AES cipher modes are dependent on AES core support according to `AES configuration`_.
   * The :ref:`nrf_security_backends_cc3xx` is limited to key sizes of 128 bits on devices with Arm CryptoCell cc310.
   * If both :ref:`nrf_security_backends_oberon` and :ref:`nrf_security_backends_orig_mbedtls` is enabled, the nrf_oberon backend will
     provide support for AES CCM/CCM*.

AES GCM
-------

AES GCM can be enabled by setting the :option:`CONFIG_MBEDTLS_GCM_C` Kconfig variable.

+--------------+-----------------------------------+--------------------------------------+
| AEAD cipher  | Configurations                    | Note                                 |
+==============+===================================+======================================+
| AES GCM      | :option:`CONFIG_MBEDTLS_GCM_C`    | Original mbed TLS or nrf_oberon only |
+--------------+-----------------------------------+--------------------------------------+

.. note::
   * AES GCM is not supported in Arm CryptoCell cc3xx hardware.

ChaCha20
--------

ChaCha20 support can be enabled by setting the :option:`CONFIG_MBEDTLS_CHACHA20_C` Kconfig variable, and one of the following Kconfig variables:

+--------------+-----------------+--------------------------------------------------------------------------+
| AEAD cipher  | Support         | Configurations                                                           |
+==============+=================+==========================================================================+
| ChaCha20     | Choice          | cc3xx: :option:`CONFIG_CHOICE_CC3XX_MBEDTLS_CHACHA20_C`                  |
|              |                 |                                                                          |
|              |                 | Original mbed TLS: :option:`CONFIG_CHOICE_VANILLA_MBEDTLS_CHACHA20_C`    |
+--------------+-----------------+--------------------------------------------------------------------------+

.. note::
   * For features provided with :ref:`Choice<nrf_security_backend_config_multiple>` support, the enabled backend that is first in order is selected by default.
   * The ChaCha-Poly implemented by the :ref:`nrf_security_backends_cc3xx` does not support incremental operations.

Poly1305
--------

Poly1305 can be enabled by setting the :option:`CONFIG_MBEDTLS_POLY1305_C` Kconfig variable and one of the following Kconfig variables:

+--------------+-----------------+-----------------------------------------------------------------------+
| AEAD cipher  | Support         | Configurations                                                        |
+==============+=================+=======================================================================+
| Poly1305     | Choice          | cc3xx: :option:`CONFIG_CHOICE_CC3XX_MBEDTLS_POLY1305_C`               |
|              |                 |                                                                       |
|              |                 | Original mbed TLS: :option:`CONFIG_CHOICE_VANILLA_MBEDTLS_POLY1305_C` |
+--------------+-----------------+-----------------------------------------------------------------------+

.. note::
   * For features provided with :ref:`Choice<nrf_security_backend_config_multiple>` support, the enabled backend that is first in order is selected by default.
   * The ChaCha-Poly implemented by the :ref:`nrf_security_backends_cc3xx` does not support incremental operations.


ChaCha-Poly
-----------

ChaCha-Poly can be enabled by setting the :option:`CONFIG_MBEDTLS_CHACHAPOLY_C` Kconfig variable, and one of the following Kconfig variables:

+--------------+-----------------+--------------------------------------------------------------------------+
| AEAD cipher  | Support         | Configurations                                                           |
+==============+=================+==========================================================================+
| ChaCha-Poly  | Choice          | cc3xx: :option:`CONFIG_CHOICE_VANILLA_MBEDTLS_CHACHAPOLY_C`              |
|              |                 |                                                                          |
|              |                 | Original mbed TLS: :option:`CONFIG_CHOICE_VANILLA_MBEDTLS_CHACHAPOLY_C`  |
+--------------+-----------------+--------------------------------------------------------------------------+

.. note::
   * ChaCha-Poly support requires enabling both `ChaCha20`_ and `Poly1305`_.
   * For features provided with :ref:`Choice<nrf_security_backend_config_multiple>` support, the enabled backend that is first in order is selected by default.
   * The ChaCha-Poly implementation in :ref:`nrf_security_backends_cc3xx` does not support incremental operations.

DHM configurations
******************

Diffie-Hellman-Merkel (DHM) support can be enabled by setting Kconfig variables according to single or multiple enabled backends.

Single backend
==============

DHM can be enabled by setting the :option:`CONFIG_MBEDTLS_DHM_C` Kconfig variable.

+--------------+--------------------------------------+
| Algorithm    | Configurations                       |
+==============+======================================+
| DHM          | :option:`CONFIG_MBEDTLS_DHM_C`       |
+--------------+--------------------------------------+

.. note::
   :ref:`nrf_security_backends_cc3xx`  is limited to key size of <= 2048 bits.

Multiple backends
=================

DHM can be enabled by setting the :option:`CONFIG_MBEDTLS_DHM_C` Kconfig variable, and one or more of the following Kconfig variables:

+--------------+--------------+--------------------------------------------------------------+
| Algorithm    | Support      | Configurations                                               |
+==============+==============+==============================================================+
| DHM          | Glue         | cc3xx: :option:`CONFIG_CC3XX_MBEDTLS_DHM_C`                  |
|              |              |                                                              |
|              |              | Original mbed TLS: :option:`CONFIG_VANILLA_MBEDTLS_DHM_C`    |
+--------------+--------------+--------------------------------------------------------------+

.. note::
   * :ref:`nrf_security_backends_cc3xx`  is limited to key size of <= 2048 bits.


ECC configurations
******************

Elliptic Curve Cryptography (ECC) configuration provides support for Elliptic Curve over GF(p).
ECC core support can be enabled by setting the :option:`CONFIG_MBEDTLS_ECP_C` Kconfig variable.
Enabling :option:`CONFIG_MBEDTLS_ECP_C` will activate configuration options that depend upon ECC, such as ECDH, ECDSA, ECJPAKE, and a selection of ECC curves to support in the system.
If multiple backends are available, you can select which backend to use for :option:`CONFIG_MBEDTLS_ECP_C`.
This backend will be used to provide support for ECDH, ECDSA, and/or ECJPAKE (if enabled).

Single backend
==============

ECC core support can be enabled by setting the :option:`CONFIG_MBEDTLS_ECP_C` Kconfig variable.

Multiple backends
=================

ECC core support can be enabled by setting the :option:`CONFIG_MBEDTLS_ECP_C` Kconfig variable, and one of the following Kconfig variables:

+--------------+--------------+--------------------------------------------------------------------+
| Algorithm    | Support      | Configurations                                                     |
+==============+==============+====================================================================+
| ECP          | Choice       | cc3xx: :option:`CONFIG_CHOICE_CC3XX_MBEDTLS_ECP_C`                 |
|              |              |                                                                    |
|              |              | nrf_oberon: :option:`CONFIG_CHOICE_OBERON_MBEDTLS_ECP_C`           |
|              |              |                                                                    |
|              |              | Original mbed TLS: :option:`CONFIG_CHOICE_VANILLA_MBEDTLS_ECP_C`   |
+--------------+--------------+--------------------------------------------------------------------+

.. note::
   * For features provided with :ref:`Choice<nrf_security_backend_config_multiple>` support, the enabled backend that is first in order is selected by default.
   * The :ref:`nrf_oberon_readme` only supports ECC curve secp256r1.


ECDH configurations
*******************

Elliptic Curve Diffie-Hellman (ECDH) support can be enabled by setting the :option:`CONFIG_MBEDTLS_ECDH_C` Kconfig variable.

+--------------+--------------------------------------+
| Algorithm    | Configurations                       |
+==============+======================================+
| ECDH         | :option:`CONFIG_MBEDTLS_ECDH_C`      |
+--------------+--------------------------------------+

.. note::
   * ECDH support depends upon `ECC Configurations`_ being enabled.
   * The :ref:`nrf_cc3xx_mbedcrypto_readme` does not integrate on ECP layer.
     Only the top-level APIs for ECDH are replaced.
   * The :ref:`nrf_oberon_readme` only supports ECC curve secp256r1.


ECDSA configurations
********************

Elliptic Curve Digital Signature Algorithm (ECDSA) support can be enabled be configured by setting the :option:`CONFIG_MBEDTLS_ECDSA_C` Kconfig variable.

+--------------+---------------------------------------+
| Algorithm    | Configurations                        |
+==============+=======================================+
| ECDSA        | :option:`CONFIG_MBEDTLS_ECDSA_C`      |
+--------------+---------------------------------------+

.. note::
   * ECDSA support depends upon `ECC Configurations`_ being enabled.
   * The :ref:`nrf_cc3xx_mbedcrypto_readme` does not integrate on ECP layer.
     Only the top-level APIs for ECDSA are replaced.
   * The :ref:`nrf_oberon_readme` only supports ECC curve secp256r1.


ECJPAKE configurations
**********************

Elliptic Curve, Password Authenticated Key Exchange by Juggling (ECJPAKE) support can be enabled by setting the :option:`CONFIG_MBEDTLS_ECJPAKE_C` Kconfig variable.

+--------------+---------------------------------------+
| Algorithm    | Configurations                        |
+==============+=======================================+
| ECJPAKE      | :option:`CONFIG_MBEDTLS_ECJPAKE_C`    |
+--------------+---------------------------------------+

.. note::
   * ECJPAKE support depends upon `ECC Configurations`_ being enabled.
   * The :ref:`nrf_oberon_readme` only supports ECC curve secp256r1.

.. _nrf_security_backend_config_ecc_curves:

ECC curves configurations
*************************

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
******************

Rivest-Shamir-Adleman (RSA) support can be enabled by setting Kconfig variables according to single or multiple enabled backends.

Single backend
==============

RSA support can be enabled by setting the :option:`CONFIG_MBEDTLS_RSA_C` Kconfig variable.

+--------------+---------------------------------------+
| Algorithm    | Configurations                        |
+==============+=======================================+
| RSA          | :option:`CONFIG_MBEDTLS_RSA_C`        |
+--------------+---------------------------------------+

.. note::
   :ref:`nrf_security_backends_cc3xx`  is limited to key sizes of <= 2048 bits.

Multiple backends
=================

RSA support can be enabled by setting the :option:`CONFIG_MBEDTLS_RSA_C` Kconfig variable, and one of the following Kconfig variables:

+--------------+-----------------+--------------------------------------------------------------------+
| Algorithm    | Support         | Configurations                                                     |
+==============+=================+====================================================================+
| RSA          | Choice          | cc3xx: :option:`CONFIG_CHOICE_CC3XX_MBEDTLS_RSA_C`                 |
|              |                 |                                                                    |
|              |                 | Original mbed TLS: :option:`CONFIG_CHOICE_VANILLA_MBEDTLS_RSA_C`   |
+--------------+-----------------+--------------------------------------------------------------------+

.. note::
   * For configurations providing :ref:`Choice<nrf_security_backend_config_multiple>` support, the enabled backend that is first in order is selected by default.
   * :ref:`nrf_security_backends_cc3xx`  is limited to key sizes of <= 2048 bits.

Secure Hash configurations
**************************

The Secure Hash algorithms can be configured by setting Kconfig variables according to single or multiple enabled backends.

Single backend
==============

SHA support can be enabled by setting Kconfig according to the following table:

+--------------+--------------------+--------------------------------------+
| Algorithm    | Support            | Backend selection                    |
+==============+====================+======================================+
| SHA-1        |                    | :option:`CONFIG_MBEDTLS_SHA1_C`      |
+--------------+--------------------+--------------------------------------+
| SHA-256      |                    | :option:`CONFIG_MBEDTLS_SHA256_C`    |
+--------------+--------------------+--------------------------------------+
| SHA-512      | Shared             | :option:`CONFIG_MBEDTLS_SHA512_C`    |
+--------------+--------------------+--------------------------------------+

Multiple backends
=================

Multiple backend configurations for Secure Hash algorithms are presented in the following tables.

SHA-1
-----

SHA-1 support can be enabled by setting the :option:`CONFIG_MBEDTLS_SHA1_C` Kconfig variable, and one of the following Kconfig variables:

+--------------+-----------------+--------------------------------------------------------------------+
| Algorithm    | Support         | Backend selection                                                  |
+==============+=================+====================================================================+
| SHA-1        | Choice          | cc3xx: :option:`CONFIG_CHOICE_CC3XX_MBEDTLS_SHA1_C`                |
|              |                 |                                                                    |
|              |                 | nrf_oberon: :option:`CONFIG_CHOICE_OBERON_MBEDTLS_SHA1_C`          |
|              |                 |                                                                    |
|              |                 | Original mbed TLS: :option:`CONFIG_CHOICE_VANILLA_MBEDTLS_SHA1_C`  |
+--------------+-----------------+--------------------------------------------------------------------+

.. note::
   * For features provided with :ref:`Choice<nrf_security_backend_config_multiple>` support, the enabled backend that is first in order is selected by default.

SHA-256
-------

SHA-256 support can be enabled by setting the :option:`CONFIG_MBEDTLS_SHA256_C` Kconfig variable, and one of the following Kconfig variables:

+--------------+-----------------+-----------------------------------------------------------------------+
| Algorithm    | Support         | Backend selection                                                     |
+==============+=================+=======================================================================+
| SHA-256      | Choice          | cc3xx: :option:`CONFIG_CHOICE_CC3XX_MBEDTLS_SHA256_C`                 |
|              |                 |                                                                       |
|              |                 | nrf_oberon: :option:`CONFIG_CHOICE_OBERON_MBEDTLS_SHA256_C`           |
|              |                 |                                                                       |
|              |                 | Original mbed TLS: :option:`CONFIG_CHOICE_VANILLA_MBEDTLS_SHA256_C`   |
+--------------+-----------------+-----------------------------------------------------------------------+

.. note::
   * For features provided with :ref:`Choice<nrf_security_backend_config_multiple>` support, the enabled backend that is first in order is selected by default.
   * The nrf_oberon backend does not support SHA-224.

SHA-512
-------

SHA-256 support can be configured by setting the :option:`CONFIG_MBEDTLS_SHA512_C` Kconfig variable.

+--------------+-----------------+-----------------------------------------------------------------+
| Algorithm    | Support         | Backend selection                                               |
+==============+=================+=================================================================+
| SHA-512      | Shared          | :option:`CONFIG_MBEDTLS_SHA512_C`                               |
+--------------+-----------------+-----------------------------------------------------------------+

.. note::
   * For features provided with :ref:`Choice<nrf_security_backend_config_multiple>` support, the enabled backend that is first in order is selected by default.

Backend platform-specific configurations
****************************************

The Kconfig integration of nrf_security module coupled with CMake sets some default configurations to ensure that the backend works as expected.

This includes integration with entropy data sampled from the Arm CryptoCell cc3xx hardware, as well as a way to abstract the memory management in an RTOS context.
See the :ref:`nrf_cc3xx_mbedcrypto_readme` documentation for details.
