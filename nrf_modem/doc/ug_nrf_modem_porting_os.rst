.. _nrf_modem_ug_porting:

Porting the Modem library to your OS
####################################

.. contents::
   :local:
   :depth: 2

The :ref:`nrf_modem` from Nordic Semiconductor is an OS-agnostic C library where OS-specific parts are moved out of the library.
To use the library with your OS or scheduler, you must first port it by creating a custom :file:`nrf_modem_os.c` file, which serves as an OS abstraction layer.
The library exposes a :file:`nrf_modem_os.h` header file that defines the functions that must be implemented in :file:`nrf_modem_os.c`.
The :file:`nrf_modem.h` header file exposes functions provided by the library that the OS integration module can interface with.

The following diagram presents the Modem library OS abstraction layer.
Arrows indicate that the elements can communicate with each other directly.

.. figure:: images/nrf_modem_layers.svg
   :alt: Modem library OS abstraction overview

   Modem library OS abstraction overview

Creating the OS abstraction layer
*********************************

To create an OS abstraction layer for the Modem library, you must implement the functions in the :file:`nrf_modem_os.h` file.
An OS abstraction layer implementation for the Zephyr RTOS is also available in the |NCS|.
See :file:`nrf_modem_os.c` for more details.
The implementation details for each function are shown in the section following and in the header file:

nrf_modem_os_init()
===================

This function is called by the Modem library when the application has issued :c:func:`nrf_modem_init`.
It is responsible for initializing OS-specific functionality related to the Modem library OS abstraction.

If Nordic Proprietary trace is enabled, the library generates trace data that can be retrieved using the :c:func:`nrf_modem_trace_get` function.
See :ref:`trace_handling` for more information.

*Required actions*:

* Initialize timers/threads.
* Configure low priority Modem library scheduling IRQ (SoftIRQ).
* Initialize heap memory.
* If Nordic Proprietary trace is enabled, initialize a trace thread and the trace backend (for example, UART or SPI).

nrf_modem_os_busywait()
=======================

This function is called by the Modem library when a blocking timed context is required.

.. _nrf_modem_os_timedwait:

nrf_modem_os_timedwait()
========================

This function is called by the Modem library when a timed context or sleep is required.
A blind return value of 0 will make all the Modem library operations always blocking.

*Required actions* to be taken that do not block the operations:

* Start counting the time (this can be based on a Timer or Thread for instance).
* In the case of a call to :c:func:`nrf_modem_os_event_notify` function, all waiting threads must wake up, reporting back the remaining time of the timer through the ``timeout`` variable.

The following points decide the *Function return value*:

* If the modem is not initialized, that is, if :c:func:`nrf_modem_is_initialized` returns false, function return value will be ``-NRF_ESHUTDOWN``.
* If there is a time out, function return value will be ``-NRF_EAGAIN``.
* In all other cases, function return value will be ``0``.

nrf_modem_os_event_notify()
===========================

This function is called by the Modem library when an event occurs and all threads waiting in :c:func:`nrf_modem_os_timedwait` function must wake up.

*Required action*:

Wake up all threads sleeping in the :c:func:`nrf_modem_os_timedwait` function.
See :ref:`nrf_modem_os_timedwait` for more details.

nrf_modem_os_alloc()
====================

This function is called by the library to allocate memory dynamically, and it is like a *malloc* call.
There are no specific requirements related to the location where this memory must be allocated in RAM.

nrf_modem_os_free()
===================

This function must free the memory allocated by :c:func:`nrf_modem_os_alloc`.

nrf_modem_os_shm_tx_alloc()
===========================

This function is called by the library to dynamically allocate the memory that must be *shared with the modem core*.
This function allocates memory on the TX memory region that is passed to the :c:func:`nrf_modem_init` function during the initialization.

nrf_modem_os_shm_tx_free()
==========================

This function releases the memory allocated by :c:func:`nrf_modem_os_shm_tx_alloc`.

nrf_modem_os_errno_set()
========================

This function translates errnos from the Modem library to the OS-defined ones.

*Required action*:

* Implement a translation for each errno set by the Modem library.
  If it overlaps with errnos of your OS, the translation is not needed.

nrf_modem_os_application_irq_set()
==================================

This function is called by the Modem library when the library wants to set a pending IRQ on the low priority scheduling IRQ of the Modem library.

*Required action*:

* Set a pending IRQ on the low priority scheduling IRQ of the Modem library using OS primitives or NVIC functions.

nrf_modem_os_application_irq_clear()
====================================

This function is called by the Modem library when the library wants to clear IRQ on the low priority scheduling IRQ of the Modem library.

*Required action*:

* Clear the low priority scheduling IRQ of the Modem library using OS primitives or NVIC functions.

nrf_modem_application_irq_handler()
===================================

This function is implemented in the Modem library and must be called upon the low priority IRQ handler of the Modem library, which is triggered by the :c:func:`nrf_modem_os_application_irq_set` function.

nrf_modem_os_event_notify()
===========================

This function is called by the Modem library when an event occurs and all threads waiting in the :c:func:`nrf_modem_os_timedwait` function must wake up.

*Required action*:

* Wake all threads that are sleeping in :c:func:`nrf_modem_os_timedwait`. For details, see :c:func:`nrf_modem_os_timedwait`.

nrf_modem_os_is_in_isr()
========================

This function is called by the library to check whether or not it is executing in a interrupt context.

nrf_modem_os_sem_init()
=======================

This function is called by the library to allocate and initialize a semaphore.

*Required action*:

* Allocate and initialize a semaphore.
* If an address of an already allocated semaphore is provided as an input, the allocation part is skipped and the semaphore is only reinitialized.

nrf_modem_os_sem_give()
=======================

This function is called by the library to give a semaphore.

nrf_modem_os_sem_take()
=======================

This function is called by the library to take a semaphore.

nrf_modem_os_log()
==================

This function is called by the library to output logs.
This function can be called in an interrupt context.

nrf_modem_os_logdump()
======================

This function is called by the library to dump binary data.
This function can be called in an interrupt context.

Other scenarios to handle the Modem library
===========================================

#. The Modem library uses the nrfxlib IPC driver.
   The application must either include the nrfxlib IPC driver or implement its own IPC driver using the same signature as the nrfxlib IPC driver.

#. If the OS has its own IRQ handler scheme that does not directly forward the IPC_IRQHandler, the OS must route the IPC_IRQHandler to the nrfxlib IPC IRQ handler.

Memory
******

The Modem library needs a region of RAM within the first lower 128KB to share with the modem.
To be accessible by both the modem and the application, this region of RAM must be configured as non-secure by the secure application.

The following RAM overview diagram shows the placement of Modem library in the sequential RAM, and it also indicates the configurable memory position values.


.. figure:: images/nrf_modem_memory.svg
   :alt: Modem library memory overview

   Modem library memory overview


Peripheral requirements
***********************

As the Modem library has been compiled to operate on peripherals in the non-secure domain, the following two peripherals must be configured to be non-secure:

* NRF_IPC
* NRF_POWER

If you are using the hard-float variant of the Modem library, the FPU must be activated in both the secure domain and the non-secure domain, and must be configured to allow the non-secure application to run FPU instructions.

The :file:`nrfx/mdk/system_nrf9160.c` file provides a template on how to configure the FPU in both cases.
The system file also provides several Errata workarounds specific to the chip variant used, which are needed for any secure domain application.

Handling of a modem fault
*************************

On initialization of the Modem library, the application registers a modem fault handler through the initialization parameters.
If a fault occurs in the modem, the application is notified through the fault handler, which enables the application to read the fault reason (and in some cases the Modem's Program Counter) and take appropriate action.

The following are the three alternatives to how the application can handle the modem fault:

* The application logs the fault with no other action.
* Restart the application.
* Shutdown and reinitialize the modem and the Modem library.

.. _trace_handling:

Handling of modem traces
************************

Modem traces are retrieved by the application and output to a trace medium, typically UART.
Trace data is retrieved by calling :c:func:`nrf_modem_trace_get`.
For more information, see :ref:`modem_trace`.

Message sequence diagrams
*************************

The following message sequence diagrams show the interactions between the application, Modem library, and the OS.

#. Sequence of the initialization of the Modem library.
   Configuration of the high and low priority IRQs:

    .. figure:: images/nrf_modem_initialization_sequence.svg
        :alt: Initialization (main thread)

        Initialization (main thread)

#. Handling an event sent from the Modem library to a lower priority to be able to receive new events:

    .. figure:: images/nrf_modem_event_sequence.svg
        :alt: Event handling, lowering priority

        Event handling, lowering priority

#. Handling a timeout or sleep:

    .. figure:: images/nrf_modem_timers_sequence.svg
        :alt: Timedwait

        Timedwait


Reference template for the nrf_modem_os.c file
**********************************************

The following code snippet shows a simple implementation of the Modem library OS abstraction layer.
You can use it as a template and customize it for your OS or scheduler.

.. code-block:: c

   #include <nrf_modem.h>
   #include <nrf_modem_os.h>
   #include <nrf_errno.h>
   #include <nrf_modem_platform.h>
   #include <nrf_modem_limits.h>

   #include <nrf.h>
   #include "errno.h"

    void read_task_create(void)
	{
		// The read task is achieved using SW interrupt.
		NVIC_SetPriority(NRF_MODEM_APPLICATION_IRQ, NRF_MODEM_APPLICATION_IRQ_PRIORITY);
		NVIC_ClearPendingIRQ(NRF_MODEM_APPLICATION_IRQ);
		NVIC_EnableIRQ(NRF_MODEM_APPLICATION_IRQ);
	}

    void nrf_modem_os_init(void)
    {
        read_task_create();
        // Initialize timers / sleeping threads used in the nrf_modem_os_timedwait function.
        // If enabled, initialize the trace task and mediums.
    }

    void *nrf_modem_os_shm_tx_alloc(size_t bytes)
    {
        // Allocate a buffer on the TX area of shared memory.
    }

    void nrf_modem_os_shm_tx_free(void *mem)
    {
        // Free a shared memory buffer in the TX area.
    }

    void *nrf_modem_os_alloc(size_t bytes)
    {
        // Allocate dynamic memory
    }

    void nrf_modem_os_free(void *mem)
    {
        // Free dynamic memory
    }

    void nrf_modem_os_busywait(int32_t usec)
    {
        // Busy wait for a given number of microseconds.
    }

    int32_t nrf_modem_os_timedwait(uint32_t context, int32_t * timeout)
    {
        // Return remaining time by reference in timeout parameter,
        // if not yet timed out and modem is initialized.
        // Return -NRF_ESHUTDOWN if the modem is not initialized.
        // Else return -NRF_ETIMEDOUT if timeout has triggered.
        // A blind return value of 0 will make all Modem library operations
        // always block.
        return 0;
    }

    void nrf_modem_os_application_irq_set(void) {
		NVIC_SetPendingIRQ(NRF_MODEM_APPLICATION_IRQ);
	}

	void nrf_modem_os_application_irq_clear(void) {
		NVIC_ClearPendingIRQ(NRF_MODEM_APPLICATION_IRQ);
	}

	void NRF_MODEM_APPLICATION_IRQ_HANDLER(void) {
		nrf_modem_application_irq_handler();
	}

    void nrf_modem_os_errno_set(int errno_val)
    {
        // Translate nrf_errno.h errno to the OS specific value.
    }

    bool nrf_modem_os_is_in_isr(void)
    {
        // Return true if called in an interrupt context, false otherwise.
    }

    int nrf_modem_os_sem_init(void **sem,
                              unsigned int initial_count,
                              unsigned int limit)
    {
        // Allocate and initialize a semaphore.
        // If an address of an already allocated semaphore is provided as an input,
        // the allocation part is skipped and the semaphore is only reinitialized.
    }

    void nrf_modem_os_sem_give(void *sem)
    {
        // Let the Modem library give back the semaphore.
    }

    int nrf_modem_os_sem_take(void *sem, int timeout)
    {
        // Let the Modem library take the semaphore within a given time.
        // If taking the semaphore times out, return NRF_ETIMEDOUT
    }

    void nrf_modem_os_event_notify()
    {
      // Wake threads in nrf_modem_os_timedwait()
    }

    void nrf_modem_os_log(int level, const char *fmt, ...)
    {
        // Send modem log to preferred logging interface.
    }

    void nrf_modem_os_logdump(int level, const char *str, const void *data, size_t len)
    {
        //Send modem logdump (hexdump) to preferred logging interface.
    }
