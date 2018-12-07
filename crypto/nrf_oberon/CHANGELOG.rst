.. _crypto_changelog_oberon:

Changelog - nRF Oberon
######################

All notable changes to this project are documented in this file.

nrf_oberon - 2.0.7
******************

Initial release.

Added
=====

Added the following Oberon crypto libraries for nRF9160, nRF52, and nRF51 architectures.

.. note::
  * short-wchar: Those libraries are compiled with a wchar_t size of 16 bits.

* nrf_oberon, nrf9160 variants

  * ``cortex-m33/hard-float/liboberon_2.0.7.a``
  * ``cortex-m33/soft-float/liboberon_2.0.7.a``

  * short-wchar

    * ``cortex-m33/hard-float/short-wchar/liboberon_2.0.7.a``
    * ``cortex-m33/soft-float/short-wchar/liboberon_2.0.7.a``

  * Keil

    * ``cortex-m33/hard-float/short-wchar/oberon_2.0.7.lib``
    * ``cortex-m33/soft-float/short-wchar/oberon_2.0.7.lib``

* nrf_oberon, nrf52 variants

  * ``cortex-m4/hard-float/liboberon_2.0.7.a``
  * ``cortex-m4/soft-float/liboberon_2.0.7.a``

  * short-wchar

    * ``cortex-m4/hard-float/short-wchar/liboberon_2.0.7.a``
    * ``cortex-m4/soft-float/short-wchar/liboberon_2.0.7.a``

  * Keil

    * ``cortex-m4/soft-float/short-wchar/oberon_2.0.7.lib``
    * ``cortex-m4/hard-float/short-wchar/oberon_2.0.7.lib``

* nrf_oberon, nrf51 variants

  * ``cortex-m0/soft-float/liboberon_2.0.7.a``

  * short-wchar

    * ``cortex-m0/soft-float/short-wchar/liboberon_2.0.7.a``

  * Keil

    * ``cortex-m0/soft-float/short-wchar/oberon_2.0.7.lib``
