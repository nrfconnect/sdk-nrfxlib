.. _nrf_security_driver_config:

Driver configurations and supported features
############################################

.. contents::
   :local:
   :depth: 2

This section covers the configurations available when using PSA drivers.

.. _nrf_security_drivers_config_multiple:

Configuring multiple drivers
****************************

Multiple PSA drivers can be enabled at the same time, with added support for fine-grained control of which drivers implement support for cryptographic features.

To enable a PSA driver, set the configurations according to the following table:

+---------------+--------------------------------------------------+------------------------------------------------+
| PSA driver    | Configuration option                             | Notes                                          |
+===============+==================================================+================================================+
| nrf_cc3xx     | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_CC3XX` | Only on nRF52840, nRF9160, and nRF5340 devices |
+---------------+--------------------------------------------------+------------------------------------------------+
| nrf_oberon    | :kconfig:option:`CONFIG_PSA_RYPTO_DRIVER_OBERON` |                                                |
+---------------+--------------------------------------------------+------------------------------------------------+

If multiple drivers are enabled, the first ordered item in this table takes precedence for an enabled cryptographic feature, unless the driver does not enable or support it.

Enabling or disabling PSA driver specific configurations controls the support for a given algorithm, per driver.

If a specific cryptographic feature is not supported by a PSA driver but the algorithm is configured to be used, then :ref:`nrf_security_drivers_builtin` will be enabled to ensure the feature is available.

AES cipher configuration
************************

AES cipher modes can be enabled by setting one or more of the following Kconfig options:

+----------------+------------------------------------------------------+
| Cipher mode    | Configuration option                                 |
+================+======================================================+
| ECB_NO_PADDING | :kconfig:option:`CONFIG_PSA_WANT_ALG_ECB_NO_PADDING` |
+----------------+------------------------------------------------------+
| CBC_NO_PADDING | :kconfig:option:`CONFIG_PSA_WANT_ALG_CBC_NO_PADDING` |
+----------------+------------------------------------------------------+
| CBC_PKCS7      | :kconfig:option:`CONFIG_PSA_WANT_ALG_CBC_PKCS7`      |
+----------------+------------------------------------------------------+
| CFB            | :kconfig:option:`CONFIG_PSA_WANT_ALG_CFB`            |
+----------------+------------------------------------------------------+
| CTR            | :kconfig:option:`CONFIG_PSA_WANT_ALG_CTR`            |
+----------------+------------------------------------------------------+
| OFB            | :kconfig:option:`CONFIG_PSA_WANT_ALG_OFB`            |
+----------------+------------------------------------------------------+
| XTS            | :kconfig:option:`CONFIG_PSA_WANT_ALG_XTS`            |
+----------------+------------------------------------------------------+


AES cipher driver configuration
===============================

You can use the Kconfig options from the following table for fine-grained control over which drivers provide AES cipher support:

+----------------+---------------------------------------------------------------------+----------------------------------------------------------------------+
| Cipher mode    | nrf_cc3xx driver support                                            | nrf_oberon driver support                                            |
+================+=====================================================================+======================================================================+
| ECB_NO_PADDING | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_ECB_NO_PADDING_CC3XX` | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_ECB_NO_PADDING_OBERON` |
+----------------+---------------------------------------------------------------------+----------------------------------------------------------------------+
| CBC_NO_PADDING | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_CBC_NO_PADDING_CC3XX` | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_CBC_NO_PADDING_OBERON` |
+----------------+---------------------------------------------------------------------+----------------------------------------------------------------------+
| CBC_PKCS7      | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_CBC_PKCS7_CC3XX`      | Not supported                                                        |
+----------------+---------------------------------------------------------------------+----------------------------------------------------------------------+
| CFB            | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_CFB_CC3XX`            | Not supported                                                        |
+----------------+---------------------------------------------------------------------+----------------------------------------------------------------------+
| CTR            | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_CTR_CC3XX`            | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_CTR_OBERON`            |
+----------------+---------------------------------------------------------------------+----------------------------------------------------------------------+
| OFB            | Not supported                                                       | Not supported                                                        |
+----------------+---------------------------------------------------------------------+----------------------------------------------------------------------+
| XTS            | Not supported                                                       | Not supported                                                        |
+----------------+---------------------------------------------------------------------+----------------------------------------------------------------------+

.. note::
   * If an AES cipher mode is enabled and no PSA driver enables or supports it, then :ref:`nrf_security_drivers_builtin` support is enabled and used.
   * The :ref:`nrf_security_drivers_cc3xx` is limited to AES key sizes of 128 bits on devices with Arm CryptoCell cc310.


MAC configuration
*****************

You can enable MAC support by setting one or more Kconfig options in the following table:

+----------------+--------------------------------------------+
| MAC cipher     | Configuration option                       |
+================+============================================+
| ECB_NO_PADDING | :kconfig:option:`CONFIG_PSA_WANT_ALG_CMAC` |
+----------------+--------------------------------------------+
| CBC_NO_PADDING | :kconfig:option:`CONFIG_PSA_WANT_ALG_HMAC` |
+----------------+--------------------------------------------+

MAC driver configurations
=========================

You can use the Kconfig options in the following table for fine-grained control over which drivers provide AEAD support:

+----------------+-----------------------------------------------------------+----------------------------+
| MAC cipher     | nrf_cc3xx driver support                                  | nrf_oberon driver support  |
+================+===========================================================+============================+
| ECB_NO_PADDING | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_CMAC_CC3XX` | Not supported              |
+----------------+-----------------------------------------------------------+----------------------------+
| CBC_NO_PADDING | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_HMAC_CC3XX` | Not supported              |
+----------------+-----------------------------------------------------------+----------------------------+

.. note::
   * If a MAC algorithm is enabled and no PSA driver enables or supports it, then :ref:`nrf_security_drivers_builtin` support is enabled and used.
   * The :ref:`nrf_security_drivers_cc3xx` is limited to AES CMAC key sizes of 128 bits on devices with Arm CryptoCell cc310.
   * The :ref:`nrf_security_drivers_cc3xx` is limited to HMAC using SHA-1, SHA-224, and SHA-256 on devices with Arm CryptoCell cc310.


AEAD configurations
*******************

You can enable Authenticated Encryption with Associated Data (AEAD) by setting one or more Kconfig options in the following table:

+----------------+---------------------------------------------------------+
| AEAD cipher    | Configuration option                                    |
+================+=========================================================+
| AES CCM        | :kconfig:option:`CONFIG_PSA_WANT_ALG_CCM`               |
+----------------+---------------------------------------------------------+
| AES GCM        | :kconfig:option:`CONFIG_PSA_WANT_ALG_GCM`               |
+----------------+---------------------------------------------------------+
| ChaCha/Poly    | :kconfig:option:`CONFIG_PSA_WANT_ALG_CHACHA20_POLY1305` |
+----------------+---------------------------------------------------------+


AEAD driver configurations
==========================

You can use the Kconfig options in the following table for fine-grained control over which drivers provide AEAD support:

+----------------+------------------------------------------------------------------------+-------------------------------------------------------------------------+
| AEAD cipher    | nrf_cc3xx driver support                                               | nrf_oberon driver support                                               |
+================+========================================================================+=========================================================================+
| AES CCM        | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_CCM_CC3XX`               | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_CCM_OBERON`               |
+----------------+------------------------------------------------------------------------+-------------------------------------------------------------------------+
| AES GCM        | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_GCM_CC3XX`               | Not Supported                                                           |
+----------------+------------------------------------------------------------------------+-------------------------------------------------------------------------+
| ChaCha/Poly    | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_CHACHA20_POLY1305_CC3XX` | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_CHACHA20_POLY1305_OBERON` |
+----------------+------------------------------------------------------------------------+-------------------------------------------------------------------------+

.. note::
   * If an AEAD algorithm is enabled and no PSA driver enables or supports it, then :ref:`nrf_security_drivers_builtin` support is enabled and used.
   * The :ref:`nrf_security_drivers_cc3xx` is limited to AES key sizes of 128 bits on devices with Arm CryptoCell cc310.
   * The :ref:`nrf_security_drivers_cc3xx` does not provide hardware support for AES GCM on devices with Arm CryptoCell cc310.


ECC configurations
******************

You can enable Elliptic Curve Cryptography (ECC) by setting one or more Kconfig options in the following table:

+-----------------------+-----------------------------------------------------------+
| ECC algorithm         | Configuration option                                      |
+=======================+===========================================================+
| ECDH                  | :kconfig:option:`CONFIG_PSA_WANT_ALG_ECDH`                |
+-----------------------+-----------------------------------------------------------+
| ECDSA                 | :kconfig:option:`CONFIG_PSA_WANT_ALG_ECDSA`               |
+-----------------------+-----------------------------------------------------------+
| ECDSA (deterministic) | :kconfig:option:`CONFIG_PSA_WANT_ALG_DETERMINISTIC_ECDSA` |
+-----------------------+-----------------------------------------------------------+

The ECC algorithm support is dependent on one or more Kconfig options enabling curve support according to `ECC curve configurations`_.


ECC driver configurations
=========================

You can use the Kconfig options in the following table for fine-grained control over which drivers provide ECC support:

+-----------------------+--------------------------------------------------------------------------+---------------------------------------------------------------------------+
| ECC algorithm         | nrf_cc3xx driver support                                                 | nrf_oberon driver support                                                 |
+=======================+==========================================================================+===========================================================================+
| ECDH                  | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_ECDH_CC3XX`                | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_ECDSA_OBERON`               |
+-----------------------+--------------------------------------------------------------------------+---------------------------------------------------------------------------+
| ECDSA                 | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_ECDSA_CC3XX`               | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_ECDSA_OBERON`               |
+-----------------------+--------------------------------------------------------------------------+---------------------------------------------------------------------------+
| ECDSA (deterministic) | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_DETERMINISTIC_ECDSA_CC3XX` | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_DETERMINISTIC_ECDSA_OBERON` |
+-----------------------+--------------------------------------------------------------------------+---------------------------------------------------------------------------+

.. note::
   * If an ECC algorithm is enabled and no PSA driver enables or supports it, then :ref:`nrf_security_drivers_builtin` support is enabled and used.
   * The :ref:`nrf_security_drivers_oberon` is currently limited to curve types secp224r1 and secp256r1 for ECDH and ECDSA.


ECC curve configurations
************************

You can configure elliptic curve support by setting one or more Kconfig options in the following table:

+-----------------------+-----------------------------------------------------------+
| ECC curve type        | Configuration option                                      |
+=======================+===========================================================+
| Brainpool256r1        | :kconfig:option:`CONFIG_PSA_WANT_ECC_BRAINPOOL_P_R1_256`  |
+-----------------------+-----------------------------------------------------------+
| Brainpool384r1        | :kconfig:option:`CONFIG_PSA_WANT_ECC_BRAINPOOL_P_R1_384`  |
+-----------------------+-----------------------------------------------------------+
| Brainpool512r1        | :kconfig:option:`CONFIG_PSA_WANT_ECC_BRAINPOOL_P_R1_512`  |
+-----------------------+-----------------------------------------------------------+
| Curve25519            | :kconfig:option:`CONFIG_PSA_WANT_ECC_MONTGOMERY_255`      |
+-----------------------+-----------------------------------------------------------+
| Curve448              | :kconfig:option:`CONFIG_PSA_WANT_ECC_MONTGOMERY_448`      |
+-----------------------+-----------------------------------------------------------+
| secp192k1             | :kconfig:option:`CONFIG_PSA_WANT_ECC_SECP_K1_192`         |
+-----------------------+-----------------------------------------------------------+
| secp256k1             | :kconfig:option:`CONFIG_PSA_WANT_ECC_SECP_K1_256`         |
+-----------------------+-----------------------------------------------------------+
| secp192r1             | :kconfig:option:`CONFIG_PSA_WANT_ECC_SECP_R1_192`         |
+-----------------------+-----------------------------------------------------------+
| secp224r1             | :kconfig:option:`CONFIG_PSA_WANT_ECC_SECP_R1_224`         |
+-----------------------+-----------------------------------------------------------+
| secp256r1             | :kconfig:option:`CONFIG_PSA_WANT_ECC_SECP_R1_256`         |
+-----------------------+-----------------------------------------------------------+
| secp384r1             | :kconfig:option:`CONFIG_PSA_WANT_ECC_SECP_R1_384`         |
+-----------------------+-----------------------------------------------------------+
| secp521r1             | :kconfig:option:`CONFIG_PSA_WANT_ECC_SECP_R1_521`         |
+-----------------------+-----------------------------------------------------------+


ECC curve driver configurations
===============================

You can sue the Kconfig options in the following table for fine-grained control over which drivers provide elliptic curve support:

+-----------------------+--------------------------------------------------------------------------+---------------------------------------------------------------------------+
| ECC curve type        | nrf_cc3xx driver support                                                 | nrf_oberon driver support                                                 |
+=======================+==========================================================================+===========================================================================+
| Brainpool256r1        | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ECC_BRAINPOOL_P_R1_256_CC3XX`  | Not supported                                                             |
+-----------------------+--------------------------------------------------------------------------+---------------------------------------------------------------------------+
| Brainpool384r1        | Not supported                                                            | Not supported                                                             |
+-----------------------+--------------------------------------------------------------------------+---------------------------------------------------------------------------+
| Brainpool512r1        | Not supported                                                            | Not supported                                                             |
+-----------------------+--------------------------------------------------------------------------+---------------------------------------------------------------------------+
| Curve25519            | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ECC_MONTGOMERY_255_CC3XX`      | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ECC_MONTGOMERY_255_OBERON`      |
+-----------------------+--------------------------------------------------------------------------+---------------------------------------------------------------------------+
| Curve448              | Not supported                                                            | Not supported                                                             |
+-----------------------+--------------------------------------------------------------------------+---------------------------------------------------------------------------+
| secp192k1             | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ECC_SECP_K1_192_CC3XX`         | Not supported                                                             |
+-----------------------+--------------------------------------------------------------------------+---------------------------------------------------------------------------+
| secp256k1             | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ECC_SECP_K1_256_CC3XX`         | Not supported                                                             |
+-----------------------+--------------------------------------------------------------------------+---------------------------------------------------------------------------+
| secp192r1             | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ECC_SECP_R1_192_CC3XX`         | Not supported                                                             |
+-----------------------+--------------------------------------------------------------------------+---------------------------------------------------------------------------+
| secp224r1             | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ECC_SECP_R1_224_CC3XX`         | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ECC_SECP_R1_224_OBERON`         |
+-----------------------+--------------------------------------------------------------------------+---------------------------------------------------------------------------+
| secp256r1             | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ECC_SECP_R1_256_CC3XX`         | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ECC_SECP_R1_256_OBERON`         |
+-----------------------+--------------------------------------------------------------------------+---------------------------------------------------------------------------+
| secp384r1             | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ECC_SECP_R1_384_CC3XX`         | Not supported                                                             |
+-----------------------+--------------------------------------------------------------------------+---------------------------------------------------------------------------+
| secp521r1             | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ECC_SECP_R1_521_CC3XX`         | Not supported                                                             |
+-----------------------+--------------------------------------------------------------------------+---------------------------------------------------------------------------+


RSA configurations
******************

You can enable Rivest-Shamir-Adleman (RSA) support by setting one or more Kconfig options in the following table:

+-----------------------+----------------------------------------------------------+
| RSA algorithms        | Configuration option                                     |
+=======================+==========================================================+
| RSA OAEP              | :kconfig:option:`CONFIG_PSA_WANT_ALG_RSA_OAEP`           |
+-----------------------+----------------------------------------------------------+
| RSA PKCS#1 v1.5 crypt | :kconfig:option:`CONFIG_PSA_WANT_ALG_RSA_PKCS1V15_CRYPT` |
+-----------------------+----------------------------------------------------------+
| RSA PKCS#1 v1.5 sign  | :kconfig:option:`CONFIG_PSA_WANT_ALG_RSA_PKCS1V15_SIGN`  |
+-----------------------+----------------------------------------------------------+
| RSA PSS               | :kconfig:option:`CONFIG_PSA_WANT_ALG_RSA_PSS`            |
+-----------------------+----------------------------------------------------------+


RSA driver configurations
=========================

You can use the Kconfig options in the following table for fine-grained control over which drivers provide RSA support:

+-----------------------+--------------------------------------------------------------------------+----------------------------+
| RSA algorithms        | nrf_cc3xx driver support                                                 | nrf_oberon driver support  |
+=======================+==========================================================================+============================+
| RSA OAEP              | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_RSA_OAEP_CC3XX`            | Not supported              |
+-----------------------+--------------------------------------------------------------------------+----------------------------+
| RSA PKCS#1 v1.5 crypt | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_RSA_PKCS1V15_CRYPT_CC3XX`  | Not supported              |
+-----------------------+--------------------------------------------------------------------------+----------------------------+
| RSA PKCS#1 v1.5 sign  | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_RSA_PKCS1V15_SIGN_CC3XX`   | Not supported              |
+-----------------------+--------------------------------------------------------------------------+----------------------------+
| RSA PSS               | Not supported                                                            | Not supported              |
+-----------------------+--------------------------------------------------------------------------+----------------------------+

.. note::
   * If an RSA algorithm is enabled and no PSA driver enables or supports it, then :ref:`nrf_security_drivers_builtin` support is enabled and used.
   * :ref:`nrf_security_drivers_cc3xx`  is limited to key sizes less than or equal to 2048 bits.


Secure Hash configurations
**************************

You can configure the Secure Hash algorithms by setting one or more Kconfig options according to the following table:

+-----------------------+-----------------------------------------------+
| Hash algorithm        | Configuration option                          |
+=======================+===============================================+
| SHA-1                 | :kconfig:option:`CONFIG_PSA_WANT_ALG_SHA_1`   |
+-----------------------+-----------------------------------------------+
| SHA-224               | :kconfig:option:`CONFIG_PSA_WANT_ALG_SHA_224` |
+-----------------------+-----------------------------------------------+
| SHA-256               | :kconfig:option:`CONFIG_PSA_WANT_ALG_SHA_256` |
+-----------------------+-----------------------------------------------+
| SHA-384               | :kconfig:option:`CONFIG_PSA_WANT_ALG_SHA_384` |
+-----------------------+-----------------------------------------------+
| SHA-512               | :kconfig:option:`CONFIG_PSA_WANT_ALG_SHA_512` |
+-----------------------+-----------------------------------------------+


Secure Hash driver configurations
=================================

You can use the PSA driver-specific configurations provided in this table for fine-grained control over which drivers provide the Secure Hash algorithm.

+-----------------------+---------------------------------------------------------------+---------------------------------------------------------------+
| Hash algorithm        |  nrf_cc3xx driver support                                     | nrf_oberon driver support                                     |
+=======================+===============================================================+===============================================================+
| SHA-1                 |  :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_SHA_1_CC3XX`   | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_SHA_1_OBERON`   |
+-----------------------+---------------------------------------------------------------+---------------------------------------------------------------+
| SHA-224               |  :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_SHA_224_CC3XX` | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_SHA_224_OBERON` |
+-----------------------+---------------------------------------------------------------+---------------------------------------------------------------+
| SHA-256               |  :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_SHA_256_CC3XX` | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_SHA_256_OBERON` |
+-----------------------+---------------------------------------------------------------+---------------------------------------------------------------+
| SHA-384               |  Not supported                                                | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_SHA_384_OBERON` |
+-----------------------+---------------------------------------------------------------+---------------------------------------------------------------+
| SHA-512               |  Not supported                                                | :kconfig:option:`CONFIG_PSA_CRYPTO_DRIVER_ALG_SHA_512_OBERON` |
+-----------------------+---------------------------------------------------------------+---------------------------------------------------------------+

.. note::
   * If Secure Hash algorithm is enabled and no PSA driver enables or supports it, then :ref:`nrf_security_drivers_builtin` support is enabled and used.
