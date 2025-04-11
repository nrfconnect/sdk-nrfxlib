.. _sqspi_features:

Features
########

This page provides an overview of the functionalities and configurations available for soft peripherals for the current state of development.
See the following sections for more details.

.. _sqspi_features_spi_modes:

SPI transfer modes
******************

Soft peripherals support the following transfer modes:

* TX (transfer only)

   * Single
   * Dual
   * Quad

* RX (receive only)

   * Single
   * Dual
   * Quad

* TX/RX

   * Single

    * Full duplex - SPI reads by switching to receiving data (MISO) after transmitting data (MOSI).
    * Half duplex - Three-wire setup (MOSI/MISO, SCLK, CSN).
      The pad switches to receiving after transmitting.

.. _sqspi_features_clock_phase:

Clock phase and polarity
************************

The following table outlines the various Serial Peripheral Interface (SPI) modes supported by the system, detailing the clock polarity (CPOL), clock phase (CPHA), idle state of the serial clock (SCLK), and the timing of data output and input relative to the SCLK.

.. list-table:: SPI modes
   :widths: auto
   :header-rows: 1

   * - SPI mode
     - CPOL
     - CPHA
     - SCLK idle
     - Data out
     - Data in
   * - 0
     - 0
     - 0
     - 0
     - Falling SCLK
     - Rising SCLK
   * - 1 `*`
     - 0
     - 1
     - 1
     - Rising SCLK
     - Falling SCLK
   * - 2 `*`
     - 1
     - 0
     - 0
     - Rising SCLK
     - Falling SCLK
   * - 3 `*`
     - 1
     - 1
     - 1
     - Falling SCLK
     - Rising SCLK

`*` These SPI modes have minor :ref:`sqspi_limitations`.

.. _sqspi_features_read_sampling:

Delayed read sampling
*********************

Delayed read sampling introduces a controlled delay between the transmission of a bit and the sampling of input data.
This feature enhances synchronization and accuracy in data communications.

.. _sqspi_features_packet_formats:

Packet formats
**************

This section provides an overview of the packet formats available for data transmission using soft peripherals.
The following features are supported:

* Configurable length in:

   * Number of bits for command and address
   * Number of SCLK cycles for dummy data
   * Number of bytes for data

  For details on how to adjust these values, see the :c:struct:`nrfx_qspi2_xfer_t` struct.

* Dual lane distribution

  .. list-table:: Supported dual-lane distribution
     :widths: auto
     :header-rows: 1

     * - Command
       - Address
       - Data
     * - 1
       - 1
       - 2
     * - 1
       - 2
       - 2

* Quad lane distribution

  .. list-table:: Supported quad-lane distribution
     :widths: auto
     :header-rows: 1

     * - Command
       - Address
       - Data
     * - 1
       - 1
       - 4
     * - 1
       - 4
       - 4
