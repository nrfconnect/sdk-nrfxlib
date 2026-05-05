.. _semmc_features:

sEMMC features
##############

.. contents::
   :local:
   :depth: 2

This page provides an overview of the functionalities and configurations available for the Soft Peripheral sEMMC for the current state of development.
See the following sections for more details.

.. _semmc_features_command_support:

Command support for EMMC
************************

sEMMC supports the following JEDEC Standard No. 84-B51A command groups:

* Class 0 - Basic
* Class 2 - Block read
* Class 4 - Block write
* Class 5 - Erase
* Class 6 - Write protection
* Class 7 - Lock device
* Class 11 - Command queuing

.. _semmc_features_hs200_mode:

HS200 mode support for EMMC
***************************

sEMMC supports the HS200 mode for high-speed transfers.
This allows sEMMC to operate at frequencies up to 80MHz on the nRF54H20 device.

.. note::
   You need to perform a tuning cycle using CMD21 to find the optimal sampling point for reads.
   After completing the tuning cycle, set the sampling value in the :c:var:`nrf_semmc_config_t.read_delay` variable.

Command support for SDCARD
**************************

sEMMC supports the following SD Specifications Part 1 Physical Layer Specification Version 9.10 command groups:

* Class 0 - Basic
* Class 2 - Block read
* Class 4 - Block write
* Class 5 - Erase
* Class 6 - Write protection
* Class 10 - Switch (Only CMD6)

SDR speed modes support for SDCARD
**********************************

Requires voltage switch from 3.3V to 1.8V, which requires external hardware, theoreticaly it is possible to reach up to SDR50: UHS-I.
See :ref:`semmc_limitations` 
