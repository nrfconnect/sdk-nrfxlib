.. _softdevice_controller_cs:

LE Channel Sounding
###################

.. contents::
   :local:
   :depth: 2

LE Channel Sounding (CS) is a feature defined in the `Bluetooth Core Specification`_.
CS allows two devices in a connection to perform measurements of their communication channel, both the round trip time (RTT), and the phase and magnitude (as in-phase and quadrature (IQ) values) can be measured.
The RTT and IQ values can then be sent to an application to calculate the distance between devices.

Tested configurations
*********************

The CS feature is highly configurable and it is not feasible to test all possible configurations and role combinations.
Since there is currently no standardized profile for CS, testing has been focused on covering the strictest possible parameters.
The :ref:`cs_supported_capabilities` sections document the CS timings supported by the |controller|.
The shortest supported timings and the mandatory timings, as specified in the `Bluetooth Core Specification`_, have been tested extensively.

In CS, the |BLE| parameters selected have a considerable impact on the performance.
The connection interval is relevant, as it affects CS scheduling and the maximum duration of CS subevents that do not interfere with the ACL.
In the |controller|, CS has been tested with the following connection intervals:

 * 7.5 ms
 * 10 ms
 * 20 ms
 * 50 ms

In all tests, the CS subevent length has been set to less than the ACL connection interval.


Support status
**************

Channel Sounding is currently supported only on the nRF54L15 SoC.
CS can be enabled by building with the :kconfig:option:`CONFIG_BT_CTLR_CHANNEL_SOUNDING` Kconfig option enabled.
When enabling CS in the |controller|, all CS-related commands are supported.

.. _cs_supported_capabilities:

Supported capabilities
----------------------

The |controller| currently has the following supported capabilities:

.. list-table:: CS feature support for the |controller|
   :widths: 20 20 20
   :header-rows: 1

   * - Feature
     - Supported
     - Not Supported
   * - CS Initiator
     - X
     -
   * - CS Reflector
     - X
     -
   * - Step Mode 0
     - X
     -
   * - Step Mode 1
     - X
     -
   * - Step Mode 2
     - X
     -
   * - Step Mode 3
     - X
     -
   * - Multiple Antenna paths
     - Yes, up to 4 antenna paths
     -
   * - Multiple Antenna elements
     - Yes, up to 4 antenna elements
     -
   * - Indefinite Procedure Repeats
     - X
     -
   * - RTT with AA-only
     - X
     -
   * - RTT with Sounding Sequence
     -
     - X
   * - RTT with Random Payload
     - Yes, 32, 64, 96, and 128 bits payloads
     -
   * - 150 ns time-of-flight precision
     - X
     -
   * - 10 ns time-of-flight precision
     -
     - X
   * - Channel Sounding Tone Quality Indicator
     - X
     -
   * - Channel Selection Algorithm #3C
     -
     - X
   * - Normalized Attack Detection Metric
     -
     - X
   * - 1M CS Sync PHY
     - X
     -
   * - 2M CS Sync PHY
     - X
     -
   * - 2M 2BT CS Sync PHY
     -
     - X
   * - Zero Transmitter Frequency Actuation Error
     - X
     -
   * - Multiple Configurations per ACL
     -
     - X

In addition to the supported capabilities listed above, the |controller| supports the following step timings:

.. list-table:: Supported step timing values for the |controller|
   :widths: 30 30
   :header-rows: 1

   * - Timing Parameter
     - Supported Times
   * - T_IP1
     - 60 µs or greater.
   * - T_IP2
     - 20 µs or greater.
   * - T_FCS
     - 60 µs or greater.
   * - T_PM
     - 10 µs or greater.
   * - T_SW
     - 10 µs.

.. note::
   The T_SW time reported is based on the implementation of antenna switching in the |NCS|, as described in :ref:`cs_multiple_antenna_support`.
   If users implement their own switching functions, they must ensure that the new implementation meets the timing requirements reported in :ref:`cs_supported_capabilities`.

.. _cs_multiple_antenna_support:

Multiple antennas support
-------------------------

Users may define their own antenna switching behavior in the |NCS|.
By using the :c:func:`cs_antenna_switch_func` function and devicetree, users can register GPIO-controlled antenna switches with the |controller|.
The Channel Sounding implementation will then automatically use the registered pins to control an external antenna switch for multi-antenna designs.
The pins assigned to a multi-antenna design will be reserved by the |controller| and cannot be used by other activity.
If users do not want to use the implementation provided in the |NCS|, they must register their own callback function for antenna switching in :c:func:`sdc_support_channel_sounding`.

.. note::
   The |controller| requires an :c:type:`sdc_cs_antenna_switch_callback_t` antenna switching callback to :c:func:`sdc_support_channel_sounding` whenever it is configured with support for multiple antennas.
   This is done automatically in the |NCS|.

When using :c:func:`cs_antenna_switch_func`, users can register which pins control the antenna switch, as well as how the pins switch, using devicetree.
An example declaration of an antenna switch in devicetree is as follows:

.. code-block:: devicetree

   cs_antenna_switch: cs-antenna-config {
     status = "okay";
     compatible = "nordic,bt-cs-antenna-switch";
     ant-gpios = <&gpio1 11 (GPIO_ACTIVE_HIGH)>,
                 <&gpio1 12 (GPIO_ACTIVE_HIGH)>,
                 <&gpio1 13 (GPIO_ACTIVE_HIGH)>,
                 <&gpio1 14 (GPIO_ACTIVE_HIGH)>;
     multiplexing-mode = <0>;
   };

This setup corresponds to the following truth table:

.. list-table:: Antenna control for multi-antenna switching without multiplexing mode.
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

An alternative to the provided setup is to use multiplexing mode.
This method allows fewer pins to control multiple antennas simultaneously:

.. list-table:: Antenna control for multi-antenna switching using multiplexing mode.
   :widths: 30 5 5
   :header-rows: 1

   * - Active Antenna:
     - **P1.11**
     - **P1.12**
   * - Antenna 1
     - 0
     - 0
   * - Antenna 2
     - 0
     - 1
   * - Antenna 3
     - 1
     - 0
   * - Antenna 4
     - 1
     - 1

Multiplexing mode can be configured with devicetree as follows:

.. code-block:: devicetree

  cs_antenna_switch: cs-antenna-config {
    status = "okay";
    compatible = "nordic,bt-cs-antenna-switch";
    ant-gpios = <&gpio1 11 (GPIO_ACTIVE_HIGH)>,
                <&gpio1 12 (GPIO_ACTIVE_HIGH)>;
    multiplexing-mode = <1>;
  };

If a design does not use four antennas, users can register fewer pins to achieve the desired behavior:

.. code-block:: devicetree

  cs_antenna_switch: cs-antenna-config {
    status = "okay";
    compatible = "nordic,bt-cs-antenna-switch";
    ant-gpios = <&gpio1 11 (GPIO_ACTIVE_LOW)>,
                <&gpio1 12 (GPIO_ACTIVE_LOW)>,
                <&gpio1 13 (GPIO_ACTIVE_LOW)>;
    multiplexing-mode = <0>;
  };

.. list-table:: Antenna control for multi-antenna switching with only 3 antennas.
   :widths: 30 5 5 5
   :header-rows: 1

   * - Active Antenna:
     - **P1.11**
     - **P1.12**
     - **P1.13**
   * - Antenna 1
     - 0
     - 1
     - 1
   * - Antenna 2
     - 1
     - 0
     - 1
   * - Antenna 3
     - 1
     - 1
     - 0


Adding a custom antenna switch IC
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When using the provided :c:func:`cs_antenna_switch_func` implementation in the |NCS|, the |controller| uses a maximum of 4 µs to set a GPIO pin high.
Therefore, a multi-antenna switch using this solution must switch within 6 µs to maintain the 10 µs T_SW reported in :ref:`cs_supported_capabilities`.

In addition to switching fast enough to meet switch timings, any RF switch used for antenna switching in Channel Sounding must handle switching while the radio is active.
This is because the switch time is too short to power-cycle the radio between tones.

.. note::
 When registering multiple antennas with the |controller|, the first antenna in the antenna array will be used by default for |BLE| transmission.

Experimental limitations
------------------------

Currently, the following limitations apply to the :ref:`Experimental <nrf:software_maturity>` support of Channel Sounding:

 * :ref:`radio coexistence <nrf:ug_radio_coex>` and :ref:`front-end modules <mpsl_fem>` are not supported.

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
