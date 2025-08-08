.. _sQSPI:

sQSPI
#####

.. contents::
   :local:
   :depth: 2

This documentation outlines the soft peripheral sQSPI.
It covers setup, integration, and operational guidelines for using sQSPI through the nrfx driver API, alongside detailed descriptions of limitations and performance considerations.

To better understand the capabilities and limitations of sQSPI, see its comparison with the standard QSPI:

.. list-table::
   :header-rows: 1

   * - Feature
     - QSPI support
     - sQSPI support
   * - DDR Clocking
     - Yes
     - No
   * - Execute in Place (XIP)
     - Yes
     - No
   * - Synchronous Serial Protocol (SSP)
     - Yes
     - No
   * - MICROWIRE
     - Yes
     - No
   * - JEDEC xSPI
     - Yes
     - No
   * - HyperBus
     - Yes
     - No
   * - Data masking
     - Yes
     - No
   * - SPI dynamic wait state
     - Yes
     - No
   * - Multi-master bus topologies (arbitration)
     - Yes
     - No
   * - Clock stretching
     - Yes
     - No
   * - Toggling CS high between data frames - Target Select Toggle Enable (SSTE)
     - Yes
     - No
   * - QSPI address or command lengths using 4-bit increments
     - Yes
     - No

.. toctree::
   :maxdepth: 1
   :glob:
   :caption: Subpages:

   features.rst
   nrf54L15_porting_v1_1_0.rst
   nrf54H20_porting_v1_1_0.rst
   display.rst
   timing.rst
   CHANGELOG.rst
   limitations.rst
   api_reference.rst
