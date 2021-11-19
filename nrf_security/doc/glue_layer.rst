.. _nrf_security_glue_layer:

Mbed TLS glue layer
###################

.. contents::
   :local:
   :depth: 2

The Mbed TLS glue layer allows for using both the hardware-accelerated and software implementation of cryptography at the same time.
A typical use case is if the cryptographic algorithm is limited in hardware support (for example, the AES key size or supported ciphers).
In this case, the Mbed TLS glue layer enables a dynamic check to verify whether the cryptographic algorithm is supported in hardware.
If it is not supported, the glue layer calls into a different enabled backend as a fallback.

The Mbed TLS glue layer is written by Nordic Semiconductor ASA and is a part of the nrf_security module.
The Mbed TLS glue layer is dependent on Kconfig variables in the |NCS| build system.

Support and priority
********************

To select the backend, the Mbed TLS glue layer implements a functionality check with priority reporting.
The check is performed using a `check function`.
This function is called from the API which provides configuration changes that determine the selection between the enabled backends.

.. code-block:: c
    :caption: Example: cc3xx backend AES CCM support and priority check

    static int mbedtls_ccm_check(mbedtls_cipher_id_t cipher, unsigned int keybits) {
            return (keybits == 128) ? 3 : 0;
    }

In this example, the AES CCM support in the backend will report priority level 3 if the key size is 128, or 0 if the key size is unsupported.
Higher priority values are selected first.
It reports this because :ref:`nrf_security_backends_cc3xx` only supports key size 128.

.. note::
   The check function can be called from multiple APIs in the Mbed TLS glue layer.
   An example of a location that determines backend selection is an API to set an encryption key, in which case the key size may change, demanding a new selection of available and prioritized backends.

.. note::
   Hardware-accelerated cryptography through the :ref:`nrf_security_backends_cc3xx` is prioritized if it is supported.


Enabling the Mbed TLS glue layer
********************************

The Mbed TLS glue layer is automatically enabled when two backends are enabled for one or more of the following groups of algorithms:

* AES (ECB, CBC, CTR, and CCM/CCM*)
* CMAC
* DHM


Mbed TLS glue layer mechanisms
******************************
The Mbed TLS glue layer will rename symbols to resolve the aliasing issues that occur from multiple enabled backends.
The backend implementation is reached using a table of function pointers to the renamed symbols.

.. code-block:: c
    :caption: Example: cc3xx backend ECDH function table

    const mbedtls_ecdh_funcs mbedtls_ecdh_cc3xx_backend_funcs = {
            .check = mbedtls_ecdh_check,
            .gen_public = mbedtls_ecdh_gen_public,
            .compute_shared = mbedtls_ecdh_compute_shared,
    };

In this example :cpp:var:`mbedtls_ecdh_cc3xx_backend_funcs` points to APIs in :ref:`nrf_cc3xx_mbedcrypto_readme` which have been renamed.
The function pointers `gen_public` and `compute_shared` have signatures equal to the corresponding Mbed TLS APIs.


The APIs that can be renamed are listed in :file:`nrfxlib/nrf_security/src/mbedcrypto_glue/symbol_rename.template.txt`
