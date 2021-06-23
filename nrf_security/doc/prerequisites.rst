.. _nrf_security_prereq:

Prerequisites
#############

The nrf_security module requires Mbed TLS. It checks for the presence of Mbed TLS in the following ways:

* CMake uses west as part of the build process to locate Mbed TLS.
* ``ARM_MBEDTLS_PATH`` can be set to enforce a specific path to Mbed TLS or in case west is not available

If west is not installed and Mbed TLS is not located, CMake reports the following error:

.. code-block:: console

    west not installed, please provide ARM_MBEDTLS_PATH to CMake to support security libraries

If west is installed and Mbed TLS is not located, CMake reports the following error:

.. code-block:: console

    Failed to find Mbed TLS, cannot build security libraries
