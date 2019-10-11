.. _ble_controller_readme:

Integration with applications
=============================

The nRF BLE Controller is an RTOS-agnostic library built for the 
Nordic Semiconductor nRF52 Series that supports Bluetooth 5.

Applications utilizing the nRF BLE Controller library are responsible for
the following:

-  Forwarding RTC0, TIMER0, RADIO, POWER\_CLOCK, and RNG interrupts to the
   nRF BLE Controller library.
   The application should not add additional processing in the
   corresponding interrupt handlers.
   Otherwise, the behavior is undefined.
-  Calling the :cpp:func:`ble_controller_low_prio_tasks_process()` when the low
   priority tasks processing interrupt occurs.
-  Ensuring thread-safe operation. This can be achieved by either:

   -  Calling all APIs from the same execution priority,
   -  ensuring that no API call is interrupted by other API calls, for example
      by using critical sections.

-  Configuring the number of available links:

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
interrupt for low priority task processing.
This interrupt is provided by the application in :cpp:func:`ble_controller_init()`.

The APIs provided in :file:`ble_controller_soc.h` and :file:`timeslot.h` give the
application limited access to these peripherals.

Architecture diagrams
---------------------

The image below shows how the nRF BLE Controller library integrates in an
RTOS-free environment.

.. figure:: pic/Architecture_Without_RTOS.svg

The image below shows how the nRF BLE Controller library integrates with an RTOS.

.. figure:: pic/Architecture_With_RTOS.svg
