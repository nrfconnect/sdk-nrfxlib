.. _nrf_security_config:

Configuration
#############

.. contents::
   :local:
   :depth: 2

You can enable the Nordic Security Module using `PSA driver support`_ or with `Legacy crypto support`_.

PSA driver support
******************

To enable Nordic Security Module with the PSA driver support, set the :kconfig:`CONFIG_NRF_SECURITY` Kconfig option along with additional configuration options, as described in :ref:`nrf_security_driver_config`.

The PSA driver support requires using PSA Crypto APIs.

.. _legacy_crypto_support:

Legacy crypto support
*********************

To enable the legacy crypto support mode of Nordic Security Module, set the :kconfig:CONFIG_NORDIC_SECURITY_BACKEND` Kconfig option along with additional configuration options, as described in :ref:`nrf_security_legacy_config`.

The legacy crypto support allows backwards compatibility for software that requires usage of Mbed TLS crypto toolbox functions prefixed with `mbedtls_`.

Custom Mbed TLS configuration files
***********************************

The Nordic Security Module (nrf_security) Kconfig options are used to generate an Mbed TLS configuration file.

Although not recommended, it is possible to provide a custom Mbed TLS configuration file by disabling :kconfig:`CONFIG_GENERATE_MBEDTLS_CFG_FILE`.
See :ref:`nrf_security_tls_header`.

Building with TF-M
******************

If :kconfig:`CONFIG_BUILD_WITH_TFM` is enabled together with :kconfig:`CONFIG_NRF_SECURITY`, the TF-M secure image will enable the use of the hardware acceleration of Arm CryptoCell.
In this case, the Kconfig configurations in the Nordic Security Module control the features enabled in TF-M.
