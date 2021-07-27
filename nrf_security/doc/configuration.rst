.. _nrf_security_config:

Configuration
#############

Use Kconfig to configure the nrf_security module.
To enable the module, set the :kconfig:`CONFIG_NORDIC_SECURITY_BACKEND` Kconfig variable in the `Nordic Security` menu.

Setting this variable allows for additional Kconfig variables, depending on the number of features requested.
These configurations are then used to generate an Mbed TLS configuration file used during compilation.

It is possible to provide your own custom Mbed TLS configuration file by deselecting the :kconfig:`CONFIG_GENERATE_MBEDTLS_CFG_FILE` Kconfig variable.

.. note::
   Deselecting the :kconfig:`CONFIG_GENERATE_MBEDTLS_CFG_FILE` Kconfig variable is not recommended.
   If you decide to do so, see :ref:`nrf_security_tls_header`.

Building with TF-M
******************

If :kconfig:`CONFIG_BUILD_WITH_TFM` is enabled together with :kconfig:`CONFIG_NORDIC_SECURITY_BACKEND`, the TF-M secure image will enable the use of the hardware acceleration of Arm CryptoCell.
In such case, the Kconfig configurations in the Nordic Security Backend control the features enabled through TF-M.
