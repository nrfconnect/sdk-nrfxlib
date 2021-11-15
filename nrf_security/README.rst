.. _nrf_security_readme:
.. _nrf_security:

Nordic Security Module
######################

The Nordic Security Module (nrf_security) provides an integration between Mbed TLS and software libraries that provide hardware-accelerated cryptographic functionality on selected Nordic Semiconductor SoCs as well as alternate software-based implementations of the Mbed TLS APIs.
This module includes an Mbed TLS glue layer to enable both hardware-accelerated and software-based Mbed TLS implementation at the same time.

.. note::
   The nrf_security module interfaces with the :ref:`nrf_cc3xx_mbedcrypto_readme`.
   This library conforms to a specific version of Mbed TLS.

Prerequisites
*************

Mbed TLS is required and usually provided by west, but it can also be
manually configured with the CMake variable ``ARM_MBEDTLS_PATH``.

.. toctree::
   :maxdepth: 2
   :caption: Subpages:

   doc/configuration
   doc/backends
   doc/glue_layer
   doc/backend_config
   doc/adv_backend_config
   doc/mbed_tls_header
   doc/api
