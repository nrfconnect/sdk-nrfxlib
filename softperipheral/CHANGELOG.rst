.. _softperipherals_changelog:

Changelog
#########

.. contents::
   :local:
   :depth: 2

All the notable changes to this project are documented on this page.

nRF Connect SDK v3.2.0
**********************

This version contains an initial release of the Soft Peripheral sEMMC.
See the following list of changes:

* Added:

  * The first implementation of Soft Peripheral sEMMC for nRF54H Series and nRF54L Series SoCs.
    For details, see the :ref:`semmc_changelog` page.

  * New virtual register for sQSPI.
    It updates sQSPI to version 1.2.1 and also supports the nRF54LM20 device.
    For details, see the :ref:`sqspi_changelog` page.

nRF Connect SDK v3.1.0
**********************

This is a release that focuses on improving existing soft peripherals.
See the following list of changes:


* Added:

  * Position Independent Code (PIC) for all Soft Peripheral binaries.

* Bug fixes:

  * Fixed higher frequency transfers for Soft Peripheral sQSPI for the nRF54L15 SoC.
    For details, see the :ref:`sqspi_changelog` page.

nRF Connect SDK v3.0.0
**********************

This is an initial release for soft peripherals.
See the following list of changes:


* Added:

  * The first implementation of Soft Peripheral sQSPI for the nRF54L15 and nRF54H20 SoCs.
    For details, see the :ref:`sqspi_changelog` page.
