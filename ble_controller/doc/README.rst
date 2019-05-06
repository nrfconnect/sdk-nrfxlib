nRF BLE Controller
==================

| The nRF BLE Controller is an RTOS-agnostic library built for the Nordic
  Semiconductor nRF52 Series that supports Bluetooth 5.
| In addition to providing a BLE HCI interface, it also provides:

-  Multiprotocol support through APIs for requesting RADIO timeslots
-  APIs for accessing flash memory concurrently with radio activity
-  APIs for controlling the high frequency clock

| The nRF BLE Controller is distributed in different variants containing
  different features.
| All variants are available as soft-float, softfp-float, and hard-float
  builds.

SoC support:

+------------+--------+--------+--------+
|            | S112   | S132   | S140   |
+============+========+========+========+
| nRF52810   | X      |        |        |
+------------+--------+--------+--------+
| nRF52832   | X      | X      |        |
+------------+--------+--------+--------+
| nRF52840   |        |        | X      |
+------------+--------+--------+--------+

Feature support:

+--------------------------+--------+--------+--------+
|                          | S112   | S132   | S140   |
+==========================+========+========+========+
| 2 Mbps PHY               | X      | X      | X      |
+--------------------------+--------+--------+--------+
| Advertiser               | X      | X      | X      |
+--------------------------+--------+--------+--------+
| Slave                    | X      | X      | X      |
+--------------------------+--------+--------+--------+
| Scanner                  |        | X      | X      |
+--------------------------+--------+--------+--------+
| Master                   |        | X      | X      |
+--------------------------+--------+--------+--------+
| Data Length Extensions   |        | X      | X      |
+--------------------------+--------+--------+--------+
| Advertising Extensions   |        | X      | X      |
+--------------------------+--------+--------+--------+
| Coded PHY (Long Range)   |        |        | X      |
+--------------------------+--------+--------+--------+

*Disclaimer*

-  The libraries are for evaluation purposes only.
-  The libraries are not fully functional and not yet fully tested.
-  The libraries are not optimized for performance.

Integration guide
-----------------

Applications utilizing the nRF BLE Controller library are responsible for
the following:

-  Forwarding RTC0, TIMER0, RADIO, POWER\_CLOCK, and RNG interrupts to the
   nRF BLE Controller library.
   The application should not add additional processing in the
   corresponding interrupt handlers,
   otherwise the behavior is undefined.
-  Calling the :cpp:func:`ble_controller_low_prio_tasks_process()` when the low
   priority tasks processing interrupt occurs.
-  Ensuring thread safe operation. This is achieved by either:

   -  Call all APIs from the same execution priority.
   -  Ensure that no API call is interrupted by other API calls, i.e. by
      using critical sections.

-  Configure the number of available links:

   -  Link configuration must be done before enabling the controller.
   -  The application is required to provide a memory buffer the size of
      which is dependent on the link configuration.

The application is not allowed to access the following peripherals
directly:

-  RTC0
-  TIMER0
-  RADIO
-  CLOCK
-  RNG
-  TEMP
-  ECB
-  AAR
-  NVMC
-  PPI channels 17 - 31

In addition to these peripherals, the nRF BLE Controller library requires one
interrupt for low priority task processing. This interrupt is provided by the
application in :cpp:func:`ble_controller_init()`.

The APIs provided in :file:`ble_controller_soc.h` and :file:`timeslot.h` give the
application limited access to these peripherals.

Architecture diagrams
---------------------

The image below shows how the nRF BLE Controller library integrates in an
RTOS-free environment.

.. figure:: pic/Architecture_Without_RTOS.svg

The image below shows how the nRF BLE Controller library integrates with an RTOS.

.. figure:: pic/Architecture_With_RTOS.svg
