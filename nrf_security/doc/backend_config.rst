.. _nrf_security_backend_config:

Backend configurations and supported features
#############################################

.. contents::
   :local:
   :depth: 2

This section covers the configurations available when either linking directly to a backend library or when using the Mbed TLS glue layer.

.. _nrf_security_backend_config_multiple:

Configuring multiple backends
*****************************

Different backends support different cryptographic algorithms.
The algorithms listed in subsequent sections are implemented either as *Glue* or *Choice*.

Glue
   Algorithms that can be enabled in multiple backends simultaneously.
Choice
   Algorithms that can not be enabled in multiple backends simultaneously.
   To enable the algorithm, a `base` configuration must first be enabled, and then the backend is chosen using a Kconfig option prefixed with ``CONFIG_CHOICE_``.
   If no backend is explicitly chosen it will be selected automatically with :ref:`nrf_security_backends_cc3xx` having precedence over :ref:`nrf_security_backends_oberon` and :ref:`nrf_security_backends_oberon` having precedence over :ref:`nrf_security_backends_orig_mbedtls`.

It will be noted in the below tables when only some backends support a feature.

AES configuration
*****************

The AES core is enabled with the Kconfig option :kconfig:`CONFIG_MBEDTLS_AES_C`.
This enables AES ECB cipher mode and allows the ciphers and modes CTR, OFB, CFB, CBC, XTS, CMAC, CCM/CCM*, and GCM to be configured.

Single backend
==============

+--------------+------------------------------------+
| Cipher mode  | Configurations                     |
+==============+====================================+
| ECB          | :kconfig:`CONFIG_MBEDTLS_AES_C`    |
+--------------+------------------------------------+

.. note::
   The :ref:`nrf_security_backends_cc3xx` is limited to key sizes of 128 bits on devices with Arm CryptoCell cc310.

Multiple backends
=================

+--------------+----------------+------------------------------------------------------------+
| Cipher mode  | Support        | Configurations                                             |
+==============+================+============================================================+
| ECB          | Glue           | cc3xx: :kconfig:`CONFIG_CC3XX_MBEDTLS_AES_C`               |
|              |                |                                                            |
|              |                | nrf_oberon: :kconfig:`CONFIG_OBERON_MBEDTLS_AES_C`         |
|              |                |                                                            |
|              |                | Original Mbed TLS: :kconfig:`CONFIG_VANILLA_MBEDTLS_AES_C` |
+--------------+----------------+------------------------------------------------------------+

.. note::
   * The :ref:`nrf_security_backends_oberon` uses some functionality from Original Mbed TLS for AES operations.
   * The :ref:`nrf_security_backends_oberon` has priority over the :ref:`nrf_security_backends_orig_mbedtls`.

Feature support
===============

+-------------+-------------------+-------------+
| Cipher mode | Backend           | Key size    |
+=============+===================+=============+
| ECB         | cc3xx             | 128-bit key |
|             +-------------------+-------------+
|             | nrf_oberon        | 128-bit key |
|             |                   +-------------+
|             |                   | 192-bit key |
|             |                   +-------------+
|             |                   | 256-bit key |
|             +-------------------+-------------+
|             | Original Mbed TLS | 128-bit key |
|             |                   +-------------+
|             |                   | 192-bit key |
|             |                   +-------------+
|             |                   | 256-bit key |
+-------------+-------------------+-------------+

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
| CTR          | Glue            | :kconfig:`CONFIG_MBEDTLS_CIPHER_MODE_CTR`   |                                        |
+--------------+-----------------+---------------------------------------------+----------------------------------------+
| CBC          | Glue            | :kconfig:`CONFIG_MBEDTLS_CIPHER_MODE_CBC`   |                                        |
+--------------+-----------------+---------------------------------------------+----------------------------------------+
| CFB          |                 | :kconfig:`CONFIG_MBEDTLS_CIPHER_MODE_CFB`   | Original Mbed TLS and nrf_oberon only  |
+--------------+-----------------+---------------------------------------------+----------------------------------------+
| OFB          |                 | :kconfig:`CONFIG_MBEDTLS_CIPHER_MODE_OFB`   | Original Mbed TLS and nrf_oberon only  |
+--------------+-----------------+---------------------------------------------+----------------------------------------+
| XTS          |                 | :kconfig:`CONFIG_MBEDTLS_CIPHER_MODE_XTS`   | Original Mbed TLS and nrf_oberon only  |
+--------------+-----------------+---------------------------------------------+----------------------------------------+

.. note::
   * AES cipher modes are dependent on enabling AES core support according to `AES configuration`_.
   * XTS will not be available if multiple backends are enabled for AES.
   * If both :ref:`nrf_security_backends_oberon` and :ref:`nrf_security_backends_orig_mbedtls` is enabled, the implementation from
     nrf_oberon backend will provide support for AES cipher modes.

Feature support
===============

+-------------+-------------------+-------------+-----------------------+
| Cipher mode | Backend           | Key size    | Note                  |
+=============+===================+=============+=======================+
| CTR         | cc3xx             | 128-bit key |                       |
|             +-------------------+-------------+-----------------------+
|             | nrf_oberon        | 128-bit key |                       |
|             |                   +-------------+-----------------------+
|             |                   | 192-bit key |                       |
|             |                   +-------------+-----------------------+
|             |                   | 256-bit key |                       |
|             +-------------------+-------------+-----------------------+
|             | Original Mbed TLS | 128-bit key |                       |
|             |                   +-------------+-----------------------+
|             |                   | 192-bit key |                       |
|             |                   +-------------+-----------------------+
|             |                   | 256-bit key |                       |
+-------------+-------------------+-------------+-----------------------+
| CBC         | cc3xx             | 128-bit key |                       |
|             +-------------------+-------------+-----------------------+
|             | nrf_oberon        | 128-bit key |                       |
|             |                   +-------------+-----------------------+
|             |                   | 192-bit key |                       |
|             |                   +-------------+-----------------------+
|             |                   | 256-bit key |                       |
|             +-------------------+-------------+-----------------------+
|             | Original Mbed TLS | 128-bit key |                       |
|             |                   +-------------+-----------------------+
|             |                   | 192-bit key |                       |
|             |                   +-------------+-----------------------+
|             |                   | 256-bit key |                       |
+-------------+-------------------+-------------+-----------------------+
| CFB         | cc3xx             | N/A         | Backend not supported |
|             +-------------------+-------------+-----------------------+
|             | nrf_oberon        | 128-bit key |                       |
|             |                   +-------------+-----------------------+
|             |                   | 192-bit key |                       |
|             |                   +-------------+-----------------------+
|             |                   | 256-bit key |                       |
|             +-------------------+-------------+-----------------------+
|             | Original Mbed TLS | 128-bit key |                       |
|             |                   +-------------+-----------------------+
|             |                   | 192-bit key |                       |
|             |                   +-------------+-----------------------+
|             |                   | 256-bit key |                       |
+-------------+-------------------+-------------+-----------------------+
| OFB         | cc3xx             | N/A         | Backend not supported |
|             +-------------------+-------------+-----------------------+
|             | nrf_oberon        | 128-bit key |                       |
|             |                   +-------------+-----------------------+
|             |                   | 192-bit key |                       |
|             |                   +-------------+-----------------------+
|             |                   | 256-bit key |                       |
|             +-------------------+-------------+-----------------------+
|             | Original Mbed TLS | 128-bit key |                       |
|             |                   +-------------+-----------------------+
|             |                   | 192-bit key |                       |
|             |                   +-------------+-----------------------+
|             |                   | 256-bit key |                       |
+-------------+-------------------+-------------+-----------------------+
| XTS         | cc3xx             | N/A         | Backend not supported |
|             +-------------------+-------------+-----------------------+
|             | nrf_oberon        | 128-bit key |                       |
|             |                   +-------------+-----------------------+
|             |                   | 192-bit key |                       |
|             |                   +-------------+-----------------------+
|             |                   | 256-bit key |                       |
|             +-------------------+-------------+-----------------------+
|             | Original Mbed TLS | 128-bit key |                       |
|             |                   +-------------+-----------------------+
|             |                   | 192-bit key |                       |
|             |                   +-------------+-----------------------+
|             |                   | 256-bit key |                       |
+-------------+-------------------+-------------+-----------------------+


CMAC configuration
******************

Cipher-based Message Authentication Code (CMAC) support can be enabled by setting Kconfig variables according to single or multiple enabled backends.

Single backend
==============

CMAC can be enabled by setting the :kconfig:`CONFIG_MBEDTLS_CMAC_C` Kconfig variable.

+--------------+-----------------------------------+
| Algorithm    | Configurations                    |
+==============+===================================+
| CMAC         | :kconfig:`CONFIG_MBEDTLS_CMAC_C`  |
+--------------+-----------------------------------+

Multiple backends
=================

CMAC can be enabled by setting the :kconfig:`CONFIG_MBEDTLS_CMAC_C` Kconfig variable, and one or more of the following Kconfig variables:

+--------------+-----------------------------+--------------------------------------------------------------------+
| Algorithm    | Support                     | Configurations                                                     |
+==============+=============================+====================================================================+
| CMAC         | Choice                      | cc3xx: :kconfig:`CONFIG_CHOICE_CC3XX_MBEDTLS_CMAC_C`               |
|              |                             |                                                                    |
|              |                             | nrf_oberon: :kconfig:`CONFIG_CHOICE_OBERON_MBEDTLS_CMAC_C`         |
|              |                             |                                                                    |
|              |                             | Original Mbed TLS: :kconfig:`CONFIG_CHOICE_VANILLA_MBEDTLS_CMAC_C` |
+--------------+-----------------------------+--------------------------------------------------------------------+

.. note::
   * For features provided with :ref:`Choice<nrf_security_backend_config_multiple>` support, the enabled backend that is first in order is selected by default.
   * The :ref:`nrf_security_backends_cc3xx` is limited to key sizes of 128 bits on devices with Arm CryptoCell cc310.

Feature support
===============

+-----------+-------------------+-------------+
| Algorithm | Backend           | Key size    |
+===========+===================+=============+
| CMAC      | cc3xx             | 128-bit key |
|           +-------------------+-------------+
|           | nrf_oberon        | 128-bit key |
|           |                   +-------------+
|           |                   | 192-bit key |
|           |                   +-------------+
|           |                   | 256-bit key |
|           +-------------------+-------------+
|           | Original Mbed TLS | 128-bit key |
|           |                   +-------------+
|           |                   | 192-bit key |
|           |                   +-------------+
|           |                   | 256-bit key |
+-----------+-------------------+-------------+


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
| AES CCM/CCM* | :kconfig:`CONFIG_MBEDTLS_CCM_C`         |                                         |
+--------------+-----------------------------------------+-----------------------------------------+
| AES GCM      | :kconfig:`CONFIG_MBEDTLS_GCM_C`         | Original Mbed TLS, or nrf_oberon,       |
|              |                                         | or cc312                                |
+--------------+-----------------------------------------+-----------------------------------------+
| ChaCha20     | :kconfig:`CONFIG_MBEDTLS_CHACHA20_C`    |                                         |
+--------------+-----------------------------------------+-----------------------------------------+
| Poly1305     | :kconfig:`CONFIG_MBEDTLS_POLY1305_C`    |                                         |
+--------------+-----------------------------------------+-----------------------------------------+
| ChaCha-Poly  | :kconfig:`CONFIG_MBEDTLS_CHACHAPOLY_C`  | Requires `Poly1305` and `ChaCha20`      |
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

AES CCM/CCM* can be enabled by setting the :kconfig:`CONFIG_MBEDTLS_CCM_C` Kconfig variable, and one or more of the following Kconfig variables:

+--------------+-----------------+-----------------------------------------------------------------+
| AEAD cipher  | Support         | Configurations                                                  |
+==============+=================+=================================================================+
| AES CCM/CCM* | Glue            | cc3xx: :kconfig:`CONFIG_CC3XX_MBEDTLS_CCM_C`                    |
|              |                 |                                                                 |
|              |                 | nrf_oberon: :kconfig:`CONFIG_OBERON_MBEDTLS_CCM_C`              |
|              |                 |                                                                 |
|              |                 | Original Mbed TLS: :kconfig:`CONFIG_VANILLA_MBEDTLS_CCM_C`      |
+--------------+-----------------+-----------------------------------------------------------------+

.. note::
   * AEAD AES cipher modes are dependent on AES core support according to `AES configuration`_.
   * The :ref:`nrf_security_backends_cc3xx` is limited to key sizes of 128 bits on devices with Arm CryptoCell cc310.
   * If both :ref:`nrf_security_backends_oberon` and :ref:`nrf_security_backends_orig_mbedtls` is enabled, the nrf_oberon backend will
     provide support for AES CCM/CCM*.

AES GCM
-------

AES GCM can be enabled by setting the :kconfig:`CONFIG_MBEDTLS_GCM_C` Kconfig variable.

+--------------+-----------------------------------+--------------------------------------+
| AEAD cipher  | Configurations                    | Note                                 |
+==============+===================================+======================================+
| AES GCM      | :kconfig:`CONFIG_MBEDTLS_GCM_C`   | Original Mbed TLS or nrf_oberon only |
+--------------+-----------------------------------+--------------------------------------+

.. note::
   * AES GCM is not supported in Arm CryptoCell cc3xx hardware.

ChaCha20
--------

ChaCha20 support can be enabled by setting the :kconfig:`CONFIG_MBEDTLS_CHACHA20_C` Kconfig variable, and one of the following Kconfig variables:

+--------------+-----------------+--------------------------------------------------------------------------+
| AEAD cipher  | Support         | Configurations                                                           |
+==============+=================+==========================================================================+
| ChaCha20     | Choice          | cc3xx: :kconfig:`CONFIG_CHOICE_CC3XX_MBEDTLS_CHACHA20_C`                 |
|              |                 |                                                                          |
|              |                 | nrf_oberon: :kconfig:`CONFIG_CHOICE_OBERON_MBEDTLS_CHACHA20_C`           |
|              |                 |                                                                          |
|              |                 | Original Mbed TLS: :kconfig:`CONFIG_CHOICE_VANILLA_MBEDTLS_CHACHA20_C`   |
+--------------+-----------------+--------------------------------------------------------------------------+

.. note::
   * For features provided with :ref:`Choice<nrf_security_backend_config_multiple>` support, the enabled backend that is first in order is selected by default.
   * The ChaCha-Poly implemented by the :ref:`nrf_security_backends_cc3xx` does not support incremental operations.

Poly1305
--------

Poly1305 can be enabled by setting the :kconfig:`CONFIG_MBEDTLS_POLY1305_C` Kconfig variable and one of the following Kconfig variables:

+--------------+-----------------+-----------------------------------------------------------------------+
| AEAD cipher  | Support         | Configurations                                                        |
+==============+=================+=======================================================================+
| Poly1305     | Choice          | cc3xx: :kconfig:`CONFIG_CHOICE_CC3XX_MBEDTLS_POLY1305_C`              |
|              |                 |                                                                       |
|              |                 | nrf_oberon: :kconfig:`CONFIG_CHOICE_OBERON_MBEDTLS_POLY1305_C`        |
|              |                 |                                                                       |
|              |                 | Original Mbed TLS: :kconfig:`CONFIG_CHOICE_VANILLA_MBEDTLS_POLY1305_C`|
+--------------+-----------------+-----------------------------------------------------------------------+

.. note::
   * For features provided with :ref:`Choice<nrf_security_backend_config_multiple>` support, the enabled backend that is first in order is selected by default.
   * The ChaCha-Poly implemented by the :ref:`nrf_security_backends_cc3xx` does not support incremental operations.


ChaCha-Poly
-----------

ChaCha-Poly can be enabled by setting the :kconfig:`CONFIG_MBEDTLS_CHACHAPOLY_C` Kconfig variable, and one of the following Kconfig variables:

+--------------+-----------------+--------------------------------------------------------------------------+
| AEAD cipher  | Support         | Configurations                                                           |
+==============+=================+==========================================================================+
| ChaCha-Poly  | Choice          | cc3xx: :kconfig:`CONFIG_CHOICE_VANILLA_MBEDTLS_CHACHAPOLY_C`             |
|              |                 |                                                                          |
|              |                 | nrf_oberon: :kconfig:`CONFIG_CHOICE_VANILLA_MBEDTLS_CHACHAPOLY_C`        |
|              |                 |                                                                          |
|              |                 | Original Mbed TLS: :kconfig:`CONFIG_CHOICE_VANILLA_MBEDTLS_CHACHAPOLY_C` |	
+--------------+-----------------+--------------------------------------------------------------------------+

.. note::
   * ChaCha-Poly support requires enabling both `ChaCha20`_ and `Poly1305`_.
   * For features provided with :ref:`Choice<nrf_security_backend_config_multiple>` support, the enabled backend that is first in order is selected by default.

Feature support
===============

+--------------+-------------------+-------------+----------------------------------------------------------------------+
| AEAD cipher  | Backend           | Key size    | Note                                                                 |
+==============+===================+=============+======================================================================+
| AES CCM/CCM* | cc3xx             | 128-bit key |                                                                      |
|              +-------------------+-------------+----------------------------------------------------------------------+
|              | nrf_oberon        | 128-bit key |                                                                      |
|              |                   +-------------+----------------------------------------------------------------------+
|              |                   | 192-bit key |                                                                      |
|              |                   +-------------+----------------------------------------------------------------------+
|              |                   | 256-bit key |                                                                      |
|              +-------------------+-------------+----------------------------------------------------------------------+
|              | Original Mbed TLS | 128-bit key |                                                                      |
|              |                   +-------------+----------------------------------------------------------------------+
|              |                   | 192-bit key |                                                                      |
|              |                   +-------------+----------------------------------------------------------------------+
|              |                   | 256-bit key |                                                                      |
+--------------+-------------------+-------------+----------------------------------------------------------------------+
| AES GCM      | cc312             | 128-bit key | cc310 not supported                                                  |
|              +-------------------+-------------+----------------------------------------------------------------------+
|              | nrf_oberon        | 128-bit key |                                                                      |
|              |                   +-------------+----------------------------------------------------------------------+
|              |                   | 192-bit key |                                                                      |
|              |                   +-------------+----------------------------------------------------------------------+
|              |                   | 256-bit key |                                                                      |
|              +-------------------+-------------+----------------------------------------------------------------------+
|              | Original Mbed TLS | 128-bit key |                                                                      |
|              |                   +-------------+----------------------------------------------------------------------+
|              |                   | 192-bit key |                                                                      |
|              |                   +-------------+----------------------------------------------------------------------+
|              |                   | 256-bit key |                                                                      |
+--------------+-------------------+-------------+----------------------------------------------------------------------+
| ChaCha20     | cc3xx             | 256-bit key |                                                                      |
|              +-------------------+-------------+----------------------------------------------------------------------+
|              | nrf_oberon        | 256-bit key |                                                                      |
|              +-------------------+-------------+----------------------------------------------------------------------+
|              | Original Mbed TLS | 256-bit key |                                                                      |
+--------------+-------------------+-------------+----------------------------------------------------------------------+
| Poly1305     | cc3xx             | 256-bit key |                                                                      |
|              +-------------------+-------------+----------------------------------------------------------------------+
|              | nrf_oberon        | 256-bit key |                                                                      |
|              +-------------------+-------------+----------------------------------------------------------------------+
|              | Original Mbed TLS | 256-bit key |                                                                      |
+--------------+-------------------+-------------+----------------------------------------------------------------------+
| ChaCha-Poly  | cc3xx             | 256-bit key | The ChaCha-Poly implementation in :ref:`nrf_security_backends_cc3xx` |
|              |                   |             | does not support incremental operations.                             |
|              +-------------------+-------------+----------------------------------------------------------------------+
|              | nrf_oberon        | 256-bit key |                                                                      |
|              +-------------------+-------------+----------------------------------------------------------------------+
|              | Original Mbed TLS | 256-bit key |                                                                      |
+--------------+-------------------+-------------+----------------------------------------------------------------------+

DHM configurations
******************

Diffie-Hellman-Merkle (DHM) support can be enabled by setting Kconfig variables according to single or multiple enabled backends.

Single backend
==============

DHM can be enabled by setting the :kconfig:`CONFIG_MBEDTLS_DHM_C` Kconfig variable.

+--------------+--------------------------------------+
| Algorithm    | Configurations                       |
+==============+======================================+
| DHM          | :kconfig:`CONFIG_MBEDTLS_DHM_C`      |
+--------------+--------------------------------------+


Multiple backends
=================

DHM can be enabled by setting the :kconfig:`CONFIG_MBEDTLS_DHM_C` Kconfig variable, and one or more of the following Kconfig variables:

+--------------+--------------+--------------------------------------------------------------+
| Algorithm    | Support      | Configurations                                               |
+==============+==============+==============================================================+
| DHM          | Glue         | cc3xx: :kconfig:`CONFIG_CC3XX_MBEDTLS_DHM_C`                 |
|              |              |                                                              |
|              |              | Original Mbed TLS: :kconfig:`CONFIG_VANILLA_MBEDTLS_DHM_C`   |
+--------------+--------------+--------------------------------------------------------------+

Feature support
===============

+-----------+-------------------+----------------------+-----------------------+
| Algorithm | Backend           | Key size             | Note                  |
+===========+===================+======================+=======================+
| DHM       | cc3xx             | Limited to 2048 bits |                       |
|           +-------------------+----------------------+-----------------------+
|           | nrf_oberon        | N/A                  | Backend not supported |
|           +-------------------+----------------------+-----------------------+
|           | Original Mbed TLS | No limitation        |                       |
+-----------+-------------------+----------------------+-----------------------+

ECC configurations
******************

Elliptic Curve Cryptography (ECC) configuration provides support for Elliptic Curve over GF(p).
ECC core support can be enabled by setting the :kconfig:`CONFIG_MBEDTLS_ECP_C` Kconfig variable.
Enabling :kconfig:`CONFIG_MBEDTLS_ECP_C` will activate configuration options that depend upon ECC, such as ECDH, ECDSA, ECJPAKE, and a selection of ECC curves to support in the system.
If multiple backends are available, you can select which backend to use for :kconfig:`CONFIG_MBEDTLS_ECP_C`.
This backend will be used to provide support for ECDH, ECDSA, and/or ECJPAKE (if enabled).

Single backend
==============

ECC core support can be enabled by setting the :kconfig:`CONFIG_MBEDTLS_ECP_C` Kconfig variable.

Multiple backends
=================

ECC core support can be enabled by setting the :kconfig:`CONFIG_MBEDTLS_ECP_C` Kconfig variable, and one of the following Kconfig variables:

+--------------+--------------+--------------------------------------------------------------------+
| Algorithm    | Support      | Configurations                                                     |
+==============+==============+====================================================================+
| ECP          | Choice       | cc3xx: :kconfig:`CONFIG_CHOICE_CC3XX_MBEDTLS_ECP_C`                |
|              |              |                                                                    |
|              |              | nrf_oberon: :kconfig:`CONFIG_CHOICE_OBERON_MBEDTLS_ECP_C`          |
|              |              |                                                                    |
|              |              | Original Mbed TLS: :kconfig:`CONFIG_CHOICE_VANILLA_MBEDTLS_ECP_C`  |
+--------------+--------------+--------------------------------------------------------------------+

.. note::
   * For features provided with :ref:`Choice<nrf_security_backend_config_multiple>` support, the enabled backend that is first in order is selected by default.

Feature support
===============

+-----------+-------------------+-------------+------------+
| Algorithm | Backend           | Curve group | Curve type |
+===========+===================+=============+============+
| ECP       | cc3xx             | NIST        | secp192r1  |
|           |                   |             +------------+
|           |                   |             | secp224r1  |
|           |                   |             +------------+
|           |                   |             | secp256r1  |
|           |                   |             +------------+
|           |                   |             | secp384r1  |
|           |                   |             +------------+
|           |                   |             | secp521r1  |
|           |                   +-------------+------------+
|           |                   | Koblitz     | secp192k1  |
|           |                   |             +------------+
|           |                   |             | secp224k1  |
|           |                   |             +------------+
|           |                   |             | secp256k1  |
|           |                   +-------------+------------+
|           |                   | Curve25519  | Curve25519 |
|           +-------------------+-------------+------------+
|           | nrf_oberon        | NIST        | secp256r1  |
|           |                   |             +------------+
|           |                   |             | secp224r1  |
|           |                   +-------------+------------+
|           |                   | Curve25519  | Curve25519 |
|           +-------------------+-------------+------------+
|           | Original Mbed TLS | NIST        | secp192r1  |
|           |                   |             +------------+
|           |                   |             | secp224r1  |
|           |                   |             +------------+
|           |                   |             | secp256r1  |
|           |                   |             +------------+
|           |                   |             | secp384r1  |
|           |                   |             +------------+
|           |                   |             | secp521r1  |
|           |                   +-------------+------------+
|           |                   | Koblitz     | secp192k1  |
|           |                   |             +------------+
|           |                   |             | secp224k1  |
|           |                   |             +------------+
|           |                   |             | secp256k1  |
|           |                   +-------------+------------+
|           |                   | Brainpool   | bp256r1    |
|           |                   |             +------------+
|           |                   |             | bp384r1    |
|           |                   |             +------------+
|           |                   |             | bp512r1    |
|           |                   +-------------+------------+
|           |                   | Curve25519  | Curve25519 |
+-----------+-------------------+-------------+------------+

ECDH configurations
*******************

Elliptic Curve Diffie-Hellman (ECDH) support can be enabled by setting the :kconfig:`CONFIG_MBEDTLS_ECDH_C` Kconfig variable.

+--------------+--------------------------------------+
| Algorithm    | Configurations                       |
+==============+======================================+
| ECDH         | :kconfig:`CONFIG_MBEDTLS_ECDH_C`     |
+--------------+--------------------------------------+

.. note::
   * ECDH support depends upon `ECC Configurations`_ being enabled.
   * The :ref:`nrf_cc3xx_mbedcrypto_readme` does not integrate on ECP layer.
     Only the top-level APIs for ECDH are replaced.

Feature support
===============

+-----------+-------------------+-------------+------------+
| Algorithm | Backend           | Curve group | Curve type |
+===========+===================+=============+============+
| ECDH      | cc3xx             | NIST        | secp192r1  |
|           |                   |             +------------+
|           |                   |             | secp224r1  |
|           |                   |             +------------+
|           |                   |             | secp256r1  |
|           |                   |             +------------+
|           |                   |             | secp384r1  |
|           |                   |             +------------+
|           |                   |             | secp521r1  |
|           |                   +-------------+------------+
|           |                   | Koblitz     | secp192k1  |
|           |                   |             +------------+
|           |                   |             | secp224k1  |
|           |                   |             +------------+
|           |                   |             | secp256k1  |
|           |                   +-------------+------------+
|           |                   | Curve25519  | Curve25519 |
|           +-------------------+-------------+------------+
|           | nrf_oberon        | NIST        | secp256r1  |
|           |                   |             +------------+
|           |                   |             | secp224r1  |
|           |                   +-------------+------------+
|           |                   | Curve25519  | Curve25519 |
|           +-------------------+-------------+------------+
|           | Original Mbed TLS | NIST        | secp192r1  |
|           |                   |             +------------+
|           |                   |             | secp224r1  |
|           |                   |             +------------+
|           |                   |             | secp256r1  |
|           |                   |             +------------+
|           |                   |             | secp384r1  |
|           |                   |             +------------+
|           |                   |             | secp521r1  |
|           |                   +-------------+------------+
|           |                   |  Koblitz    | secp192k1  |
|           |                   |             +------------+
|           |                   |             | secp224k1  |
|           |                   |             +------------+
|           |                   |             | secp256k1  |
|           |                   +-------------+------------+
|           |                   | Brainpool   | bp256r1    |
|           |                   |             +------------+
|           |                   |             | bp384r1    |
|           |                   |             +------------+
|           |                   |             | bp512r1    |
|           |                   +-------------+------------+
|           |                   | Curve25519  | Curve25519 |
+-----------+-------------------+-------------+------------+

ECDSA configurations
********************

Elliptic Curve Digital Signature Algorithm (ECDSA) support can be enabled be configured by setting the :kconfig:`CONFIG_MBEDTLS_ECDSA_C` Kconfig variable.

+--------------+---------------------------------------+
| Algorithm    | Configurations                        |
+==============+=======================================+
| ECDSA        | :kconfig:`CONFIG_MBEDTLS_ECDSA_C`     |
+--------------+---------------------------------------+

.. note::
   * ECDSA support depends upon `ECC Configurations`_ being enabled.
   * The :ref:`nrf_cc3xx_mbedcrypto_readme` does not integrate on ECP layer.
     Only the top-level APIs for ECDSA are replaced.

Feature support
===============

+-----------+-------------------+-------------+------------+
| Algorithm | Backend           | Curve group | Curve type |
+===========+===================+=============+============+
| ECDSA     | cc3xx             | NIST        | secp192r1  |
|           |                   |             +------------+
|           |                   |             | secp224r1  |
|           |                   |             +------------+
|           |                   |             | secp256r1  |
|           |                   |             +------------+
|           |                   |             | secp384r1  |
|           |                   |             +------------+
|           |                   |             | secp521r1  |
|           |                   +-------------+------------+
|           |                   | Koblitz     | secp192k1  |
|           |                   |             +------------+
|           |                   |             | secp224k1  |
|           |                   |             +------------+
|           |                   |             | secp256k1  |
|           |                   +-------------+------------+
|           |                   | Curve25519  | Curve25519 |
|           +-------------------+-------------+------------+
|           | nrf_oberon        | NIST        | secp256r1  |
|           |                   |             +------------+
|           |                   |             | secp224r1  |
|           |                   +-------------+------------+
|           |                   | Curve25519  | Curve25519 |
|           +-------------------+-------------+------------+
|           | Original Mbed TLS | NIST        | secp192r1  |
|           |                   |             +------------+
|           |                   |             | secp224r1  |
|           |                   |             +------------+
|           |                   |             | secp256r1  |
|           |                   |             +------------+
|           |                   |             | secp384r1  |
|           |                   |             +------------+
|           |                   |             | secp521r1  |
|           |                   +-------------+------------+
|           |                   |  Koblitz    | secp192k1  |
|           |                   |             +------------+
|           |                   |             | secp224k1  |
|           |                   |             +------------+
|           |                   |             | secp256k1  |
|           |                   +-------------+------------+
|           |                   | Brainpool   | bp256r1    |
|           |                   |             +------------+
|           |                   |             | bp384r1    |
|           |                   |             +------------+
|           |                   |             | bp512r1    |
|           |                   +-------------+------------+
|           |                   | Curve25519  | Curve25519 |
+-----------+-------------------+-------------+------------+

ECJPAKE configurations
**********************

Elliptic Curve, Password Authenticated Key Exchange by Juggling (ECJPAKE) support can be enabled by setting the :kconfig:`CONFIG_MBEDTLS_ECJPAKE_C` Kconfig variable.

+--------------+---------------------------------------+
| Algorithm    | Configurations                        |
+==============+=======================================+
| ECJPAKE      | :kconfig:`CONFIG_MBEDTLS_ECJPAKE_C`   |
+--------------+---------------------------------------+

.. note::
   ECJPAKE support depends upon `ECC Configurations`_ being enabled.

Feature support
===============

+-----------+-------------------+-------------+------------+
| Algorithm | Backend           | Curve group | Curve type |
+===========+===================+=============+============+
| ECJPAKE   | cc3xx             | NIST        | secp256r1  |
|           +-------------------+-------------+------------+
|           | nrf_oberon        | NIST        | secp256r1  |
|           +-------------------+-------------+------------+
|           | Original Mbed TLS | NIST        | secp256r1  |
+-----------+-------------------+-------------+------------+



.. _nrf_security_backend_config_ecc_curves:

ECC curves configurations
*************************

It is possible to configure the curves that should be supported in the system
depending on the backend selected.

The following table shows the curves that can be enabled.

+-----------------------------+-----------------------------------------------------+--------------------------+
| Curve                       | Configurations                                      | Note                     |
+=============================+=====================================================+==========================+
| NIST secp192r1              | :kconfig:`CONFIG_MBEDTLS_ECP_DP_SECP192R1_ENABLED`  |                          |
+-----------------------------+-----------------------------------------------------+--------------------------+
| NIST secp224r1              | :kconfig:`CONFIG_MBEDTLS_ECP_DP_SECP224R1_ENABLED`  |                          |
+-----------------------------+-----------------------------------------------------+--------------------------+
| NIST secp256r1              | :kconfig:`CONFIG_MBEDTLS_ECP_DP_SECP256R1_ENABLED`  |                          |
+-----------------------------+-----------------------------------------------------+--------------------------+
| NIST secp384r1              | :kconfig:`CONFIG_MBEDTLS_ECP_DP_SECP384R1_ENABLED`  |                          |
+-----------------------------+-----------------------------------------------------+--------------------------+
| NIST secp521r1              | :kconfig:`CONFIG_MBEDTLS_ECP_DP_SECP521R1_ENABLED`  |                          |
+-----------------------------+-----------------------------------------------------+--------------------------+
| Koblitz secp192k1           | :kconfig:`CONFIG_MBEDTLS_ECP_DP_SECP192K1_ENABLED`  |                          |
+-----------------------------+-----------------------------------------------------+--------------------------+
| Koblitz secp224k1           | :kconfig:`CONFIG_MBEDTLS_ECP_DP_SECP224K1_ENABLED`  |                          |
+-----------------------------+-----------------------------------------------------+--------------------------+
| Koblitz secp256k1           | :kconfig:`CONFIG_MBEDTLS_ECP_DP_SECP256K1_ENABLED`  |                          |
+-----------------------------+-----------------------------------------------------+--------------------------+
| Brainpool bp256r1           | :kconfig:`CONFIG_MBEDTLS_ECP_DP_BP256R1_ENABLED`    | Original Mbed TLS only   |
+-----------------------------+-----------------------------------------------------+--------------------------+
| Brainpool bp384r1           | :kconfig:`CONFIG_MBEDTLS_ECP_DP_BP384R1_ENABLED`    | Original Mbed TLS only   |
+-----------------------------+-----------------------------------------------------+--------------------------+
| Brainpool bp512r1           | :kconfig:`CONFIG_MBEDTLS_ECP_DP_BP512R1_ENABLED`    | Original Mbed TLS only   |
+-----------------------------+-----------------------------------------------------+--------------------------+
| Curve25519                  | :kconfig:`CONFIG_MBEDTLS_ECP_DP_CURVE25519_ENABLED` |                          |
+-----------------------------+-----------------------------------------------------+--------------------------+
| Curve448                    | :kconfig:`CONFIG_MBEDTLS_ECP_DP_CURVE448_ENABLED`   | Original Mbed TLS only   |
+-----------------------------+-----------------------------------------------------+--------------------------+

.. note::
   * The :ref:`nrf_oberon_readme` only supports ECC curve secp256r1.
   * Choosing the nrf_oberon backend does not allow enabling the rest of the ECC curve types.


RSA configurations
******************

Rivest-Shamir-Adleman (RSA) support can be enabled by setting Kconfig variables according to single or multiple enabled backends.

Single backend
==============

RSA support can be enabled by setting the :kconfig:`CONFIG_MBEDTLS_RSA_C` Kconfig variable.

+--------------+---------------------------------------+
| Algorithm    | Configurations                        |
+==============+=======================================+
| RSA          | :kconfig:`CONFIG_MBEDTLS_RSA_C`       |
+--------------+---------------------------------------+

.. note::
   :ref:`nrf_security_backends_cc3xx`  is limited to key sizes of <= 2048 bits.

Multiple backends
=================

RSA support can be enabled by setting the :kconfig:`CONFIG_MBEDTLS_RSA_C` Kconfig variable, and one of the following Kconfig variables:

+--------------+-----------------+--------------------------------------------------------------------+
| Algorithm    | Support         | Configurations                                                     |
+==============+=================+====================================================================+
| RSA          | Choice          | cc3xx: :kconfig:`CONFIG_CHOICE_CC3XX_MBEDTLS_RSA_C`                |
|              |                 |                                                                    |
|              |                 | Original Mbed TLS: :kconfig:`CONFIG_CHOICE_VANILLA_MBEDTLS_RSA_C`  |
+--------------+-----------------+--------------------------------------------------------------------+

.. note::
   For configurations providing :ref:`Choice<nrf_security_backend_config_multiple>` support, the enabled backend that is first in order is selected by default.

Feature support
===============

+-----------+-------------------+----------------------+-----------------------+
| Algorithm | Backend           | Key size             | Note                  |
+===========+===================+======================+=======================+
| RSA       | cc3xx             | Limited to 2048 bits |                       |
|           +-------------------+----------------------+-----------------------+
|           | nrf_oberon        | N/A                  | Backend not supported |
|           +-------------------+----------------------+-----------------------+
|           | Original Mbed TLS | No limitation        |                       |
+-----------+-------------------+----------------------+-----------------------+

Secure Hash configurations
**************************

The Secure Hash algorithms can be configured by setting Kconfig variables according to single or multiple enabled backends.

Single backend
==============

SHA support can be enabled by setting Kconfig according to the following table:

+--------------+--------------------+--------------------------------------+
| Algorithm    | Support            | Backend selection                    |
+==============+====================+======================================+
| SHA-1        |                    | :kconfig:`CONFIG_MBEDTLS_SHA1_C`     |
+--------------+--------------------+--------------------------------------+
| SHA-256      |                    | :kconfig:`CONFIG_MBEDTLS_SHA256_C`   |
+--------------+--------------------+--------------------------------------+
| SHA-512      |                    | :kconfig:`CONFIG_MBEDTLS_SHA512_C`   |
+--------------+--------------------+--------------------------------------+

Multiple backends
=================

Multiple backend configurations for Secure Hash algorithms are presented in the following tables.

SHA-1
-----

SHA-1 support can be enabled by setting the :kconfig:`CONFIG_MBEDTLS_SHA1_C` Kconfig variable, and one of the following Kconfig variables:

+--------------+-----------------+--------------------------------------------------------------------+
| Algorithm    | Support         | Backend selection                                                  |
+==============+=================+====================================================================+
| SHA-1        | Choice          | cc3xx: :kconfig:`CONFIG_CHOICE_CC3XX_MBEDTLS_SHA1_C`               |
|              |                 |                                                                    |
|              |                 | nrf_oberon: :kconfig:`CONFIG_CHOICE_OBERON_MBEDTLS_SHA1_C`         |
|              |                 |                                                                    |
|              |                 | Original Mbed TLS: :kconfig:`CONFIG_CHOICE_VANILLA_MBEDTLS_SHA1_C` |
+--------------+-----------------+--------------------------------------------------------------------+

.. note::
   For features provided with :ref:`Choice<nrf_security_backend_config_multiple>` support, the enabled backend that is first in order is selected by default.

SHA-256
-------

SHA-256 support can be enabled by setting the :kconfig:`CONFIG_MBEDTLS_SHA256_C` Kconfig variable, and one of the following Kconfig variables:

+--------------+-----------------+-----------------------------------------------------------------------+
| Algorithm    | Support         | Backend selection                                                     |
+==============+=================+=======================================================================+
| SHA-256      | Choice          | cc3xx: :kconfig:`CONFIG_CHOICE_CC3XX_MBEDTLS_SHA256_C`                |
|              |                 |                                                                       |
|              |                 | nrf_oberon: :kconfig:`CONFIG_CHOICE_OBERON_MBEDTLS_SHA256_C`          |
|              |                 |                                                                       |
|              |                 | Original Mbed TLS: :kconfig:`CONFIG_CHOICE_VANILLA_MBEDTLS_SHA256_C`  |
+--------------+-----------------+-----------------------------------------------------------------------+

.. note::
   For features provided with :ref:`Choice<nrf_security_backend_config_multiple>` support, the enabled backend that is first in order is selected by default.

SHA-512
-------

SHA-256 support can be configured by setting the :kconfig:`CONFIG_MBEDTLS_SHA512_C` Kconfig variable.

+--------------+-----------------+-----------------------------------------------------------------+
| Algorithm    | Support         | Backend selection                                               |
+==============+=================+=================================================================+
| SHA-512      |                 | :kconfig:`CONFIG_MBEDTLS_SHA512_C`                              |
+--------------+-----------------+-----------------------------------------------------------------+

.. note::
   For features provided with :ref:`Choice<nrf_security_backend_config_multiple>` support, the enabled backend that is first in order is selected by default.

Feature support
===============

+-----------+--------------------+----------------------------------------+
| Algorithm | Supported backends | Note                                   |
+===========+====================+========================================+
| SHA-1     | cc3xx              |                                        |
|           +--------------------+----------------------------------------+
|           | nrf_oberon         |                                        |
|           +--------------------+----------------------------------------+
|           | Original Mbed TLS  |                                        |
+-----------+--------------------+----------------------------------------+
| SHA-224   | cc3xx              | SHA-224 is enabled by enabling SHA-256 |
|           +--------------------+                                        |
|           | nrf_oberon         |                                        |
|           +--------------------+                                        |
|           | Original Mbed TLS  |                                        |
+-----------+--------------------+----------------------------------------+
| SHA-256   | cc3xx              |                                        |
|           +--------------------+                                        |
|           | nrf_oberon         |                                        |
|           +--------------------+                                        |
|           | Original Mbed TLS  |                                        |
+-----------+--------------------+----------------------------------------+
| SHA-384   | cc3xx              | SHA-384 is enabled by enabling SHA-512 |
|           +--------------------+                                        |
|           | nrf_oberon         |                                        |
|           +--------------------+                                        |
|           | Original Mbed TLS  |                                        |
+-----------+--------------------+----------------------------------------+
| SHA-512   | cc3xx              |                                        |
|           +--------------------+                                        |
|           | nrf_oberon         |                                        |
|           +--------------------+                                        |
|           | Original Mbed TLS  |                                        |
+-----------+--------------------+----------------------------------------+

Backend platform-specific configurations
****************************************

The Kconfig integration of nrf_security module coupled with CMake sets some default configurations to ensure that the backend works as expected.

This includes integration with entropy data sampled from the Arm CryptoCell cc3xx hardware, as well as a way to abstract the memory management in an RTOS context.
See the :ref:`nrf_cc3xx_mbedcrypto_readme` documentation for details.
