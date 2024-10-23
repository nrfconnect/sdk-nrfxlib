.. _softdevice_controller_cs:

LE Channel Sounding
###################

.. contents::
   :local:
   :depth: 2

LE Channel Sounding (CS) is a feature defined in the `Bluetooth Core Specification`_.
CS allows two devices in a connection to perform measurements of their communication channel, both the round trip time (RTT), and the phase and magnitude (as in-phase and quadrature (IQ) values) can be measured.
The RTT and IQ values can then be sent to an application to calculate the distance between devices.

Channel Sounding currently has :ref:`Experimental <nrf:software_maturity>` support in the |controller| for the nRF54L15 SoC.
CS can be enabled by building with the :kconfig:option:`CONFIG_BT_CTLR_CHANNEL_SOUNDING` Kconfig option enabled.

Developing with CS
******************

Currently, the :zephyr:code-sample:`bluetooth_hci_uart` sample is recommended to get started with Channel Sounding.
Build the sample with the following command to enable the CS feature:

.. code-block:: console

   west build -b nrf54l15dk/nrf54l15/cpuapp --pristine -- -DCONFIG_BT_CTLR_CHANNEL_SOUNDING=y

You can use the sample to run the following supported HCI commands over UART:

 * LE CS Read Local Supported Capabilities
 * LE CS Read Remote Supported Capabilities
 * LE CS Security Enable
 * LE CS Set Default Settings
 * LE CS Read Remote FAE Table
 * LE CS Create Config
 * LE CS Set Channel Classification
 * LE CS Set Procedure Parameters
 * LE CS Procedure Enable
 * LE CS Test
 * LE CS Test End

Experimental Limitations
************************

Currently, the following limitations apply to the :ref:`Experimental <nrf:software_maturity>` support of Channel Sounding:

 * :ref:`radio coexistence <nrf:ug_radio_coex>` and :ref:`front-end modules <mpsl_fem>` are not supported.
 * Only one subevent per event is supported in CS.
