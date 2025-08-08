.. _sqspi_changelog:

sQSPI changelog
###############

.. contents::
   :local:
   :depth: 2

All the notable changes to sQSPI are documented on this page.

v1.1.0
******

See the list of changes for the current release.

* Added:

  * The following for nRF54L15 devices:

    * Fixes on API communication layer

  * The following for nRF54H20 devices:

    * Fixes for booting bug
    * Fixes on API communication layer

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
