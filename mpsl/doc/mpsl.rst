.. _mpsl_lib:

Integration notes
#################

This page describes how to integrate the Multiprotocol Service Layer (MPSL) into an application.
The descriptions are valid for both RTOS and RTOS-free environments.

For the nRF53 Series, the requirements described are only relevant for applications running alongside the MPSL on the network processor.

The following peripherals are owned by MPSL and must not be accessed directly by the application:

 * RTC0
 * TIMER0
 * RADIO
 * CLOCK
 * TEMP
 * PPI channel 19, 30, 31, for the nRF52 Series
 * DPPI channels 0 - 2, for the nRF53 Series

Limited access to these peripherals is provided through the MPSL Timeslot module and through other MPSL APIs.

Thread and Interrupt Safety
***************************
The MPSL library is not reentrant, so for thread-safe operation, some considerations must be made.

Interrupt configuration
=======================
MPSL enables interrupts for RTC0, TIMER0, POWER_CLOCK, and ``low_prio_irq``.
All other interrupts must be enabled and configured by the application.
If the Timeslot API is used for RADIO access, the application is responsible for enabling and disabling the interrupt for RADIO.

Interrupts for RTC0, TIMER0, and RADIO must be configured for priority level 0 (:c:macro:`MPSL_HIGH_IRQ_PRIORITY`) by the application.

The following interrupts do not have real time requirements:

 * POWER_CLOCK interrupt

   It is up to the application to forward any clock related events to :cpp:func:`MPSL_IRQ_CLOCK_Handler` in lower priority.
   Irrelevant events are ignored, so the application is free to forward all events for the POWER_CLOCK interrupt.


 * ``low_prio_irq`` interrupt

   Low priority work is signaled by MPSL by pending the IRQ specified in the ``low_prio_irq`` argument to :cpp:func:`mpsl_init`.
   When this interrupt is triggered, :cpp:func:`mpsl_low_priority_process` should be called as soon as possible, at least within a couple of ms.
   The application should configure this interrupt priority lower than c:macro:MPSL_HIGH_IRQ_PRIORITY level (that is, a higher numerical value).
   The interrupt is enabled with :cpp:func:`mpsl_init` and disabled with :cpp:func:`mpsl_uninit` by MPSL.


Scheduling
==========
Interaction of the MPSL library with protocol stacks is designed to run at two interrupt priority levels: one for the high priority handlers, and one for the low priority handler.
Interaction of the MPSL library with the application happens in thread context and in the low priority handler.

High priority
-------------
The high priority handlers are mostly used for timing-critical operations related to radio or scheduling.
Interrupting or delaying these handlers leads to undefined behavior.

Low priority
------------
Low priority is used for background tasks that are not directly tied to the radio or scheduling.
These tasks are designed in such a way that they can be interrupted by high priority code.
The tasks are however not designed to be interrupted by other low priority tasks.
Therefore, make sure that only one MPSL API function is called from the application at any time.

 * All protocol stacks using MPSL must be synchronized (i.e. not called concurrently) to avoid concurrent calls to MPSL functions.
 * Application must only call MPSL APIs from non-preemptible threads, or with interrupts disabled (e.g. during initialization).
 * The :cpp:func:`mpsl_low_priority_process` function should only be called from thread context, i.e. not directly from the software interrupt handler.
 * Alternatively, you can use synchronization primitives to ensure that no MPSL functions are called at the same time.

Other priorities
----------------
MPSL inititialization functions, like :cpp:func:`mpsl_init` and :cpp:func:`mpsl_uninit`, are not thread-safe.
Do not call them while, for example, a protocol timeslot is in progress.
This must be enforced by application and protocol stacks. 

MPSL should be initialized before any protocol stack is enabled, and uninitialized after all protocol stacks have been disabled.

Architecture diagrams
---------------------

The following image shows how the MPSL integrates in an RTOS-free environment.

.. figure:: pic/Architecture_Without_RTOS.svg
   :alt: MPSL integration in an RTOS-free environment

   MPSL integration in an RTOS-free environment

The following image shows how the the MPSL integrates with an RTOS.

.. figure:: pic/Architecture_With_RTOS.svg
   :alt: MPSL integration with an RTOS

   MPSL integration with an RTOS
