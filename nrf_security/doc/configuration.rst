.. _nrf_security_config:

Configuration
#############

Use Kconfig to configure the nrf_security module.
To enable the module, set the :option:`CONFIG_NORDIC_SECURITY_BACKEND` Kconfig variable in the `Nordic Security` menu.

Setting this variable allows for additional Kconfig variables, depending on the number of features requested.
These configurations are then used to generate an mbed TLS configuration file used during compilation.

It is possible to provide your own custom mbed TLS configuration file by deselecting the :option:`CONFIG_GENERATE_MBEDTLS_CFG_FILE` Kconfig variable.

.. note::
   Deselecting the :option:`CONFIG_GENERATE_MBEDTLS_CFG_FILE` Kconfig variable is not recommended.
   If you decide to do so, see :ref:`nrf_security_tls_header`.
