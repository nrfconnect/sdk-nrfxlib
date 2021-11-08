.. _nrf_security_tls_header:

User-provided Mbed TLS config header
####################################

The Nordic Security Module provides a Kconfig interface to control compilation and linking of Mbed TLS and the :ref:`nrf_cc3xx_mbedcrypto_readme` library.

The Kconfig interface and build system ensures that the configuration of nrf_security is valid and working, and ensures that dependencies between different cryptographic APIs are met.

It is therefore highly recommended to let the build system generate the Mbed TLS configuration header.

However, for special use cases that cannot be achieved using the Kconfig configuration tool, it is possible to provide a custom Mbed TLS configuration header.

When doing so, you must make sure that the system is working. First, use Kconfig and the build system to create an Mbed TLS configuration header as a starting point, and then tweak this file to include settings that are not available in Kconfig.

.. note::
   When providing a custom Mbed TLS configuration header, it is important that the following criteria are still met:

   * Entropy length of 144, that is, ``#define MBEDTLS_ENTROPY_MAX_GATHER 144``
   * Force SHA256
