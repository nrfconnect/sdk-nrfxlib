.. _nrf_cc310_mbedcrypto_readme:

nrf_cc310_mbedcrypto library
############################

The nrf_cc310_mbedcrypto library is software library to interface with the Arm CryptoCell CC310 hardware accelerator that is available on the nRF52840 SoC and the nRF9160 SiP.
The library adds hardware support for selected cryptographic algorithms.

Integration with mbed TLS
=========================
The nrf_cc310_mbedcrypto library provides low-level integration towards selected versions of mbed TLS deliverables.
The APIs expressed in this library use the alternative implementation abstraction layer inside mbed TLS for selected modules.

.. note::
   It is not recommended to link to this library directly. Use the :ref:`nrf_security_readme`.


Supported cryptographic algorithms
==================================

The following tables show the supported cryptographic algorithms in the Arm CryptoCell CC310 hardware accelerator in nRF52840 and nRF9160, as well as the current state of support in the nrf_cc310_mbedcrypto library.

.. note::
   If `no mbed TLS support` is listed in limitations, it indicates that the hardware supports it, but it is not exposed in an API that works with mbed TLS.
   
   If `mbedtls_extra only` is listed in limitations, it indicates that an API similar to recent versions of mbed TLS is made available with the library.


AES - Advanced Encryption Standard
----------------------------------
+-----------------------+-------------------------------+
| Cipher                | Limitations                   |
+=======================+===============================+
| CTR                   | 128-bit                       |
+-----------------------+-------------------------------+
| CBC                   | 128-bit                       |
+-----------------------+-------------------------------+
| OFB                   | 128-bit, no mbed TLS support  |
+-----------------------+-------------------------------+
| CFB                   | 128-bit, no mbed TLS support  |
+-----------------------+-------------------------------+
| CMAC                  | 128-bit                       |
+-----------------------+-------------------------------+


AEAD - Authenticated Encryption with Associated Data
----------------------------------------------------
+-----------------------+-------------------------------+
| Cipher                | Limitations                   |
+=======================+===============================+
| CCM/CCM*              | 128-bit                       |
+-----------------------+-------------------------------+
| ChaCha-Poly           | 128-bit, mbedtls_extra only   |
+-----------------------+-------------------------------+

.. note::
   APIs currently in mbedtls_extra, as well as APIs with no current HW TLS support, will be supported in upcoming releases of the nrf_cc310_mbedcrypto library.

Diffie-Hellman-Merkel
---------------------
Supported for key sizes <= 2048 bits.

RSA
---
PKCS#1 v1.5 and v2.1 is supported for signing and encryption including:

* RSASSA-PSS
* RSAES-OEAP

Supported for key sizes <= 2048 bits.

Secure Hash
-----------
SHA-1 and SHA-256 is supported.

ECDSA and ECDH
--------------
ECDSA and ECDH is supported for the following elliptic curves:

SEC 2/NIST 186-4:

* secp160r1
* secp192r1
* secp224r1
* secp256r1
* secp384r1
* secp521r1

Koblitz:

* secp160k1
* secp192k1
* secp224k1
* secp256k1

Edwards/Montgommery:

* Ed25519
* Curve25519

Additional items in mbedtls_extra
---------------------------------
The following is a list of features available in mbedtls_extra as non-standard mbed TLS APIs:

* AES key wrap functions
* ChaCha20 and Poly1305
* ECIES
* HKDF
* SRP, up to 3072 bits

Using the library
=================

Providing platform specific calloc/free
---------------------------------------
This library facilitates the same type of memory management as regular mbed TLS deliverables.
This includes using internal calls to calloc/free from the library code when memory is needed.

The following API must be used to change the default `calloc`/`free` function:

.. code-block:: c
    :caption: Setting custom calloc/free
	
    int ret;
    
    ret = mbedtls_platform_set_calloc_free(alloc_fn, free_fn);
    if (ret != 0) {
            /* Failed to set the alternative calloc/free */
            return ret;
    }

.. note::
   This API must be called prior to calling :cpp:func:`mbedtls_platform_setup`.

.. note::
   The library will default to use clib calloc/free functions if the :cpp:func:`mbedtls_platform_set_calloc_free` is not used.


Initializing the library
------------------------
The library requires initialization before use.
You can initialize it by calling the :cpp:func:`mbedtls_platform_setup`/:cpp:func:`mbedtls_platform_teardown` functions.

.. code-block:: c
    :caption: Initializing the library
	
    int ret;
    static mbedtls_platform_context platform_context = {0};
    
    ret = mbedtls_platform_setup(&platform_context);
    if (ret != 0) {
            /* Failed to initialize nrf_cc310_mbedcrypto platform */
            return ret,
    }

.. note::
   There is no need to enable/disable the CC310 hardware by writing to the ``NRF_CRYPTOCELL->ENABLE`` and ``NRF_CRYPTOCELL_S->ENABLE`` registers.
   This happens automatically when calling APIs in this library.

RNG initialization memory management
------------------------------------

The nrf_cc310_mbedcrypto library allocates a work buffer during RNG initialization using calloc/free.
The size of this work buffer is 6112 bytes.
An alternative to allocating this on the heap is to provide a reference to a static variable inside the :cpp:type:`mbedtls_platform_context` structure type.

.. code-block:: c
    :caption: Preventing heap-allocation for RNG initialization
	
    int ret;
    static mbedtls_rng_workbuf_internal rng_workbuf;
    static mbedtls_platform_context platform_context = {0};
    platform_context.p_rnd_workbuf = &rng_workbuf;
    
    ret = mbedtls_platform_setup(&platform_context);
    if (ret != 0) {
            /* Failed to initialize nrf_cc310_mbedcrypto platform */
            return ret,
    }

Usage restrictions
------------------

On the nRF9160 SiP, the nrf_cc310_mbedcrypto library is restricted to only work in secure processing environment.
The library uses mutexes to ensure single usage of hardware modules.
