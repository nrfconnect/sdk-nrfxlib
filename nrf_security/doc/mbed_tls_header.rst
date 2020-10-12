.. _nrf_security_tls_header:

User-provided mbed TLS config header
####################################

The Nordic Security Module provides a Kconfig interface to control compilation and linking of mbed TLS and the :ref:`nrf_cc3xx_mbedcrypto_readme` library.

The Kconfig interface and build system ensures that the configuration of nrf_security is valid and working, and ensures that dependencies between different cryptographic APIs are met.

It is therefore highly recommended to let the build system generate the mbed TLS configuration header.

However, for special use cases that cannot be achieved using the Kconfig configuration tool, it is possible to provide a custom mbed TLS configuration header.

When doing so, you must make sure that the system is working. First, use Kconfig and the build system to create an mbed TLS configuration header as a starting point, and then tweak this file to include settings that are not available in Kconfig.

.. note::
   When providing a custom mbed TLS configuration header, it is important that the following criteria are still met:

   * Entropy length of 144, i.e. ``#define MBEDTLS_ENTROPY_MAX_GATHER 144``
   * Force SHA256
