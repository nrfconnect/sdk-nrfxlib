.. _bluetooth_coex:

Bluetooth External Radio Coexistence
####################################

.. contents::
   :local:
   :depth: 2

The Bluetooth® coexistence feature can use the 3-wire Packet Traffic Arbitration protocol or the 1-wire LTE coexistence protocol.

**Application Programming Interface:**

Details of the API for 3-wire and 1-wire can be found in the file :file:`nrfxlib/mpsl/include/mpsl_coex.h`.


.. NOTE::
   This implementation is only supported on nRF52 devices.


3-Wire coexistence protocol
---------------------------


IEEE 802.15.2-2003 provides recommendations to facilitate coexistence.
This implementation is based on the Packet Traffic Arbitration (PTA) solution in section 6.
Since IEEE 802.15.2-2003 is not a standard but a recommendation, implementations may vary.

Interface
**********

PTA is a system implemented outside of the Bluetooth SoC, and its role can be fulfilled by another wireless device, such as by a WiFi SoC.
The Bluetooth stack communicates with PTA using a 3-wire hardware interface, with Bluetooth acting as a slave.

When the interface is enabled, the :ref:`softdevice_controller` submits a request to PTA before any radio activity.
PTA can then grant or deny the requested radio activity.
PTA can withdraw its approval at any time during a granted radio activity request.

Hardware resources
==================

.. table:: 3-wire protocol HW resources

   =================  =====  ========================================================================================
   Peripheral         Count  Description
   =================  =====  ========================================================================================
   HF Timer instance  1      Timer instance for adhering to the timing requirements, referred to as the *coex timer*.
   GPIO pins          3      Pin selection for the 3-wire interface.
   GPIOTE channels    3      Each GPIOTE channel corresponds to a GPIO pin.
   PPI channels       4      Three channels for the three GPIOTEs, and one for handling radio events.
   =================  =====  ========================================================================================

Interface signals
=================

The interface consists of three signals mapped to the GPIO pins by the application's interface configuration.
A signal's active level (high or low) is programmable.
All timing diagrams here assume that all three signals are active high

.. list-table:: Coexistence interface signal description
   :widths: 15 15 30
   :header-rows: 1

   * - Signal name
     - Driven by
     - Description
   * - :c:enumerator:`BLE_REQUEST`
     - :ref:`softdevice_controller`
     - Activated to submit a request for radio activity to PTA.
       The signal remains active for the duration of the requested radio activity.
   * - :c:enumerator:`BLE_PRIORITY`
     - :ref:`softdevice_controller`
     - Indicates the priority and the direction of the requested radio activity.
   * - :c:enumerator:`BLE_GRANT`
     - PTA
     - Deactivated to indicate that a request is denied.

The coexistence interface can be enabled or disabled when the :ref:`softdevice_controller` is active.
This change is not immediate and depends on the current :ref:`softdevice_controller` activity.
When the coexistence interface is disabled, the associated GPIO pins are set to a high impedance state.

Timing parameters
=================

.. include:: bluetooth_coex_3w_interface_shared_section.rst

See :ref:`bluetooth_coex_example_diagrams` for example timing diagrams.



1-Wire coexistence protocol
---------------------------


Interface
**********

The 1-wire protocol lets Bluetooth LE nRF chips coexist alongside an LTE device on a separate chip.
It was specifically designed for the `coex interface of nRF9160 <https://infocenter.nordicsemi.com/index.jsp?topic=%2Fps_nrf9160%2Fip%2Fradio_lte%2Fdoc%2Fmagpio_if.html>`_.


Hardware resources
==================

The 1-wire Coexistence protocol requires the following peripherals:

.. table:: 1-wire coexistence protocol HW resources

   ===============  =====  ========================================================
   Peripheral       Count  Description
   ===============  =====  ========================================================
   GPIO pins        1      Pin selected for the 1-wire interface BLE_GRANT
   GPIOTE channels  1      One GPIOTE channel for registering BLE_GRANT pin changes
   PPI channels     1      Connecting the BLE_GRANT to Radio DISABLE task
   ===============  =====  ========================================================

Interface signals
=================

The 1-wire interface is a single uni-directional input controlled by the LTE device:

.. table:: 1-wire LTE coexistence protocol pin

   =========  =========  =====================================
   Pin        Direction  Description
   =========  =========  =====================================
   BLE_GRANT  In         Used by the LTE to grant radio access
   =========  =========  =====================================

The interface consists of a BLE_GRANT signal mapped to the GPIO pin by the application's interface configuration.
The signal's active level (high or low) is programmable.
Whenever the nRF SoC requires the use of the radio for any RF activity, it needs to test that the BLE_GRANT pin level is valid for Bluetooth LE radio use (such as 0 for the nRF9160).

The figure below illustrates these timings with regard to the LTE and BLE_GRANT signal.

.. figure:: pic/mpsl_coex_1wire_timing_grant.svg

   1-wire LTE Coexistence protocol timing

When the BLE_GRANT is revoked (for example the pin de-asserts for the nRF9160), the Bluetooth LE radio immediately gets disabled with a maximum delay of 7us.

When the coexistence interface is disabled, the associated GPIO pins are set to a high impedance state.
