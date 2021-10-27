.. _bluetooth_coex:

Bluetooth External Radio Coexistence
####################################

.. contents::
   :local:
   :depth: 2

IEEE 802.15.2-2003 provides recommendations to facilitate coexistence.
This implementation is based on the Packet Traffic Arbitration (PTA) solution.
Since IEEE 802.15.2-2003 is not a standard but a recommendation, implementations may vary.

The Bluetooth® stack communicates with PTA using a 3-wire hardware interface, with Bluetooth acting as a slave.

.. NOTE::
   This implementation is only supported on nRF52 devices.

Protocol
********

The 3-wire interface has two lines controlled by Bluetooth, and one controlled by PTA:

.. table:: 3-wire protocol pins

   ==============  ==========================================================
   Pin             Description
   ==============  ==========================================================
   BLE_REQUEST     Bluetooth requests radio operation.
   BLE_PRI_STATUS  Bluetooth indicates priority and type of activity (TX/RX).
   BLE_GRANT       PTA grants/denies request.
   ==============  ==========================================================

The 3-wire protocol has three timing parameters:

.. table:: 3-wire protocol timing

   =========  =================  ====================================================================================================
   Parameter  Value              Description
   =========  =================  ====================================================================================================
   T1         User configurable  Time from when BLE_REQUEST is asserted until the type of transaction is indicated.
   T2         User configurable  Time from when BLE_REQUEST is asserted until the radio can assume that the request has been granted.
   T3         10µs               Time from when BLE_GRANT is de-asserted until the radio must shut down.
   =========  =================  ====================================================================================================

T1 and T2 are configurable through type_delay_us and radio_delay_us.

.. NOTE::
   The sum of type_delay_us and radio_delay_us cannot be more than 40us due to radio ramp-up time.

.. figure:: pic/mpsl_coex_3wire_timings_delays.png

   3-wire protocol timings

Whenever Bluetooth requires the use of the radio for any RF activity, it will assert BLE_REQUEST.
The BLE_PRI_STATUS pin will first indicate the priority of the request.
After T1, the BLE_PRI_STATUS pin will indicate the type of transaction (RX/TX).
Then, after T2 has passed, the radio will start on-air activity.

When PTA detects a request, it will sample priority and type, and will grant or deny the request before T2.
PTA may revoke a granted session at any time.
When the grant pin de-asserts, the radio will be disabled and the request line will be de-asserted within T3.
The grant line will stay de-asserted until the request is released.

Hardware resources
******************

.. table:: 3-wire protocol HW resources

   =================  =====  ========================================================================================
   Peripheral         Count  Description
   =================  =====  ========================================================================================
   HF Timer instance  1      Timer instance for adhering to the timing requirements, referred to as the *coex timer*.
   GPIO pins          3      Pin selection for the 3-wire interface.
   GPIOTE channels    3      Each GPIOTE channel corresponds to a GPIO pin.
   PPI channels       4      Three channels for the three GPIOTEs, and one for handling radio events.
   =================  =====  ========================================================================================

Application Programming Interface
*********************************

Details of the API can be found in file :file:`nrfxlib/mpsl/include/mpsl_coex.h`.
