.. _nrf_security_config:

Configuration
#############

The Nordic Security Module is enabled through Kconfig with the option :kconfig:`CONFIG_NORDIC_SECURITY_BACKEND` and additional configuration is usually done with Kconfig.

These configurations are then used to generate an Mbed TLS configuration file used during compilation.

Although not recommended, it is possible to provide a custom Mbed TLS configuration file by disabling :kconfig:`CONFIG_GENERATE_MBEDTLS_CFG_FILE`.
See :ref:`nrf_security_tls_header`.


Building with TF-M
******************

If :kconfig:`CONFIG_BUILD_WITH_TFM` is enabled together with :kconfig:`CONFIG_NORDIC_SECURITY_BACKEND`, the TF-M secure image will enable the use of the hardware acceleration of Arm CryptoCell.
In this case, the Kconfig configurations in the Nordic Security Backend control the features enabled in TF-M.
