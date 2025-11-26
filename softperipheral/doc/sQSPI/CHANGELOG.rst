.. _sqspi_changelog:

sQSPI changelog
###############

.. contents::
   :local:
   :depth: 2

All the notable changes to sQSPI are documented on this page.
Refer to :ref:`intro_soft_peripherals_platform_support` for supported devices for each version.

v1.2.0
******

See the list of changes for the current release.

* Added:

  * Support for the nRF54LM20 device.
  * A new virtual register on API communication layer for both nRF54H Series and nRF54L Series devices.

* Updated:

  * Documentation regarding prepared transfers - they are now discarded upon deactivation.
  * Porting guides for the nRF54H Series and nRF54L Series devices.
  * Naming convention for the Soft Peripheral, which now uses the device series instead of specific product names.

v1.1.0
******

See the list of changes for the current release.

* Added:

  * Fixes on API communication layer for nRF54L15 devices.
  * Fixes for a booting bug and on API communication layer for nRF54H20 devices.

v1.0.0
******

See the list of changes for the current release.

.. note::
   This release introduces a change in the API, updating the naming convention from ``nrfx_qspi2`` to ``nrf_sqspi`` prefix, which breaks backward compatibility.

* Added:

  * The following for nRF54L15 devices:

    * Support for preparing (holding) transfers.
    * Higher speed transfers are now supported (>=33 MHz) by using GPIOHSPADCTL.
    * Position Independent Code for more flexibility when placing the firmware.
    * Initial support to use with displays.

v0.1.0
******

This is an initial release.

* Added the following for the nRF54L15 and nRF54H20 devices:

  * TX, RX, and TX/RX :ref:`SPI transfer modes<sqspi_features_spi_modes>` support.
  * Configurable :ref:`clock phase and polarity<sqspi_features_clock_phase>`.
  * :ref:`Delayed read sampling <sqspi_features_read_sampling>`.
  * Support for :ref:`various packet formats <sqspi_features_packet_formats>`.
