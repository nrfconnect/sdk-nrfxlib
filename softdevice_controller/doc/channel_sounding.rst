.. _softdevice_controller_cs:

LE Channel Sounding
###################

.. contents::
   :local:
   :depth: 2

LE Channel Sounding (CS) is a feature defined in the `Bluetooth Core Specification`_.
CS allows two devices in a connection to perform measurements of their communication channel, both the round trip time (RTT), and the phase and magnitude (as in-phase and quadrature (IQ) values) can be measured.
The RTT and IQ values can then be sent to an application to calculate the distance between devices.

Support status
**************

Channel Sounding currently has :ref:`Experimental <nrf:software_maturity>` support in the |controller| for the nRF54L15 SoC.
CS can be enabled by building with the :kconfig:option:`CONFIG_BT_CTLR_CHANNEL_SOUNDING` Kconfig option enabled.

Command support
---------------

The following LE CS commands are supported:

 * LE CS Read Local Supported Capabilities
 * LE CS Read Remote Supported Capabilities
 * LE CS Write Cached Remote Supported Capabilities
 * LE CS Security Enable
 * LE CS Set Default Settings
 * LE CS Read Remote FAE Table
 * LE CS Write Cached Remote FAE Table
 * LE CS Create Config
 * LE CS Remove Config
 * LE CS Set Channel Classification
 * LE CS Set Procedure Parameters
 * LE CS Procedure Enable
 * LE CS Test
 * LE CS Test End

Supported capabilities
----------------------

The |controller| currently has the following supported capabilities:

 * One CS Configuration per ACL connection.
 * Indefinite Procedure Repeats.
 * Four Antennas and Four Antenna Paths.
 * CS Reflector and CS Initiator Roles.
 * Step modes 1, 2, and 3.
 * 150 ns time-of-flight precision
 * RTT with AA-only and 32, 64, 96, and 128 bits random payloads
 * 1M and 2M CS Sync Phy
 * No Transmitter Frequency Actuation Error

The supported timing values for the |controller| are as follows:

 * A T_IP1 of 60 µs or greater.
 * A T_IP2 of 30 µs or greater.
 * A T_FCS of 60 µs or greater.
 * A T_PM of 10 µs or greater.
 * A T_SW of 10 µs.

Multiple antennas support
-------------------------

Currently, the |controller| supports multiple antennas with a fixed pinout for the multi-antenna switch.
In order to use the |controller| with multiple antennas, an external GPIO controlled multi-antenna switch needs to be set up with the following truth table:

.. list-table:: Antenna control for multi-antenna switching.
   :widths: 30 5 5 5 5
   :header-rows: 1

   * - Active Antenna:
     - **P1.11**
     - **P1.12**
     - **P1.13**
     - **P1.14**
   * - Antenna 1
     - 1
     - 0
     - 0
     - 0
   * - Antenna 2
     - 0
     - 1
     - 0
     - 0
   * - Antenna 3
     - 0
     - 0
     - 1
     - 0
   * - Antenna 4
     - 0
     - 0
     - 0
     - 1

.. note::
   Currently, the |controller| needs a maximum of 4 µs to trigger an antenna switch within the T_SW period.
   This means that any multi-antenna switch with the correct pinout must switch within 6 µs to maintain 10 µs T_SW.

Experimental limitations
------------------------

Currently, the following limitations apply to the :ref:`Experimental <nrf:software_maturity>` support of Channel Sounding:

 * :ref:`radio coexistence <nrf:ug_radio_coex>` and :ref:`front-end modules <mpsl_fem>` are not supported.
 * Only one subevent per event is supported in CS.

Developing with CS
******************

The following samples are recommended for getting started with Channel Sounding:

1. Using the Ranging Service samples:

   * :ref:`Bluetooth: Channel Sounding Initiator with Ranging Requestor <nrf:channel_sounding_ras_initiator>`.
   * :ref:`Bluetooth: Channel Sounding Reflector with Ranging Responder <nrf:channel_sounding_ras_reflector>`.

2. Using the :zephyr:code-sample:`bluetooth_hci_uart` sample and running HCI commands over UART.

.. note::
   To build the :zephyr:code-sample:`bluetooth_hci_uart` sample with Channel Sounding enabled, set :kconfig:option:`CONFIG_BT_CTLR_CHANNEL_SOUNDING` to ``y``.

   .. code-block:: console

      west build -b nrf54l15dk/nrf54l15/cpuapp --pristine -- -DCONFIG_BT_CTLR_CHANNEL_SOUNDING=y

Optional CS Kconfigs
--------------------

Use the following Kconfig options to enable the desired optional CS features:

 * Set :kconfig:option:`BT_CTLR_SDC_CS_STEP_MODE3` to ``y`` to enable Channel Sounding step mode-3 support.

 * Set :kconfig:option:`BT_CTLR_SDC_CS_MAX_ANTENNA_PATHS` to a value between ``2`` and ``4`` to enable Channel Sounding multiple antenna paths support.
 * Set :kconfig:option:`BT_CTLR_SDC_CS_NUM_ANTENNAS` to a value between ``2`` and :kconfig:option:`BT_CTLR_SDC_CS_MAX_ANTENNA_PATHS` to enable Channel Sounding multiple antennas support.
