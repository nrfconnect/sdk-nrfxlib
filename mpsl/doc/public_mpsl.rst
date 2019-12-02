.. _external_mpsl:

Integration with applications
#############################


.. toctree::
   :maxdepth: 2
   :caption: Contents:

Peripherals
===========
The following peripherals are owned by MPSL and must not be accessed directly by the application:

-  RTC0
-  TIMER0
-  RADIO
-  CLOCK
-  TEMP

Limited access to these peripherals is provided through the MPSL Timeslot module and through other MPSL APIs.

Thread and Interrupt Safety
===========================
The MPSL library is not reentrant, so for thread-safe operation, some considerations must be made.

Interrupt configuration
-----------------------
* MPSL will enable the interrupts for RTC0, TIMER0, POWER_CLOCK, and low_prio_irq.
* All other interrupts must be enabled and configured by the application.

  * If the Timeslot API is used for RADIO access, the application is responsible for enabling and disabling interrupt for RADIO.

* Interrupts for RTC0, TIMER0 and RADIO should be configured for priority level 0 (:c:macro:`MPSL_HIGH_IRQ_PRIORITY`) by the application.
* The following interrupts does not have real time requirements:

  * POWER_CLOCK interrupt

    * It is up to the application to forward any clock related events to :cpp:func:`MPSL_IRQ_CLOCK_Handler` in lower priority.
      Irrelevant events are ignored, so the application is free to forward all events for the POWER_CLOCK interrupt.

  * `low_prio_irq` interrupt

    * Low priority work will be signaled by MPSL by pending the IRQ specified in the `low_prio_irq` argument to :cpp:func:`mpsl_init`. When this interrupt is triggered :cpp:func:`mpsl_low_priority_process` should be called within reasonable time. This will issue timeslot signals like blocked or canceled, among other things.
    * The application should configure the interrupt priority lower than c:macro:MPSL_HIGH_IRQ_PRIORITY level, that is higher numerical value. The interrupt will be enabled in :cpp:func:`mpsl_init` and disabled in :cpp:func:`mpsl_uninit` by MPSL.

Scheduling
----------
MPSL's interaction with protocol stacks is designed to run at two interrupt priority levels; one for the high priority handlers, and one for the low priority handler.
MPSL's interaction with the application happens in thread context and in the low priority handler.

High Priority
"""""""""""""
The high priority handlers are mostly used for timing-critical operations related to radio or scheduling. Interrupting or delaying these handlers will lead to undefined behaviour.

Low Priority
""""""""""""
Low priority is used for background tasks that are not directly tied to the radio or scheduling. These tasks are designed in such a way that they can be interrupted by high priority code. The tasks are however not designed to be interrupted by other low priority tasks. Therefore it must be ensured that only `one` MPSL API function is called from the application at any time.

 * All protocol stacks using MPSL must be synchronized (i.e. not called concurrently) to avoid concurrent calls to MPSL functions.
 * Application must only call MPSL APIs from non-preemptible threads, or with interrupts disabled (e.g. during initialization)
 * The :cpp:func:`mpsl_low_priority_process` function should only be called from thread context, i.e. not directly from the software interrupt handler.
 * Alternatively, one can use synchronization primitives to ensure that no MPSL functions are called at the same time.

Other Priorities
""""""""""""""""
MPSL inititialization functions like :cpp:func:`mpsl_init` and :cpp:func:`mpsl_uninit` are not thread safe, and must not be called while for example a protocol timeslot is in progress. This must be enforced by application and protocol stacks. 

* MPSL should be initialized before any protocol stack is enabled, and uninitialized after all protocol stacks have been disabled.