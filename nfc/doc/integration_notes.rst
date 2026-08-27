.. _nfc_integration_notes:

Integration notes
#################

.. contents::
   :local:
   :depth: 2

When integrating the NFC libraries in your application, consider the following notes.

Supported device families
*************************

The NFC libraries support the following device families on SoCs where the NFCT peripheral is available:

* nRF52 Series
* nRF53 Series
* nRF54L Series
* nRF54H Series
* nRF71 Series

See :ref:`type_2_tag` and :ref:`type_4_tag` for tag-type-specific details.

Integration requirements
************************

* The libraries require the NFCT driver from the `nrfx`_ repository.
* To use the libraries in your runtime environment, you must implement an NFC Platform software module.
  See :ref:`nfc_platform` for the required API and :ref:`nfc_platform_api` for the full reference.
  The |NCS| provides a reference implementation of this module (see :ref:`nfc_platform_ncs`).
* Each library must be the only user of the NFCT peripheral.
* On SoCs that require NFCT errata workarounds, each library must also be the only user of the timer instance selected for the NFCT driver.
  See :ref:`nfc_nfct_timer` for details.

.. _nfc_nfct_timer:

NFCT driver Timer instance
==========================

The NFCT driver uses one timer peripheral to implement errata workarounds on affected SoCs.
The timer is required on devices with the following anomalies:

* nRF52 Series `anomaly 79`_ and `anomaly 190`_
* nRF53 Series `anomaly 70`_
* nRF54L Series anomaly 60
* nRF71 Series anomaly 60

The timer is not used on SoCs that do not require these workarounds, such as the nRF54H Series SoC.

When the timer is used, you cannot share this timer instance with other parts of your application.
You must also connect the timer interrupt to the :c:func:`nrfx_nfct_workaround_timer_handler` function in your NFC platform implementation.

The default timer instance is defined by the ``NRFX_NFCT_CONFIG_TIMER_INSTANCE_ID`` macro in the :file:`nrfx_config.h` header.
The following table lists the default values on affected SoCs:

.. list-table:: Default NFCT Timer instances
   :header-rows: 1
   :widths: auto

   * - Device family
     - Default Timer instance
   * - nRF52 Series
     - TIMER4
   * - nRF53 Series
     - TIMER2
   * - nRF54L Series
     - TIMER24
   * - nRF71 Series
     - TIMER24

You can override the default configuration by defining ``NRFX_NFCT_CONFIG_TIMER_INSTANCE_ID`` before including the nrfx template configuration, for example:

.. code-block:: c

   #ifndef NRFX_CONFIG_H__
   #define NRFX_CONFIG_H__

   /* Set the Timer TIMER22 instance for the NFCT driver. */
   #define NRFX_NFCT_CONFIG_TIMER_INSTANCE_ID 22

   /* Use defaults for undefined symbols. */
   #include "nrfx_templates_config.h"
   #endif /* NRFX_CONFIG_H__ */

.. note::
   On the nRF54L Series devices, it is not recommended to change the default timer instance.

.. _nfc_platform:

NFC Platform module
*******************

The NFC Platform module allows the NFC libraries to operate in different runtime environments.
It is declared in the :file:`nfc_platform.h` header file.

This module is responsible for activating the NFCT driver when the following conditions are fulfilled:

* NFC field is present.
* HFXO is running.

Overview
========

During initialization, the NFC libraries call the NFC platform module to set up the runtime environment and allocate resources that must be provided by the application.
The module must implement the functions listed in the following sections.

For the complete API definition, see :ref:`nfc_platform_api`.

Required functions
==================

:c:func:`nfc_platform_setup`
   Called by the NFC libraries at initialization.
   This function sets up the clock interface, connects the NFCT and timer (when required) interrupts with their respective IRQ handler functions from nrfx, and stores the :c:type:`nfc_lib_cb_resolve_t` callback resolution function pointer supplied by the library.

   Store this callback resolution function pointer for later use in the :c:func:`nfc_platform_cb_request` function.
   Also, set the interrupt priority value to ``*p_irq_priority``.
   In the |NCS| implementation, this value is taken from the NFCT devicetree node.

:c:func:`nfc_platform_nfcid1_default_bytes_get`
   Used to fetch default bytes for NFCID1 stored in FICR registers.
   Access to FICR registers differs between secure and non-secure processing environments.

:c:func:`nfc_platform_event_handler`
   Called by the NFC libraries to forward NFC events received from the NFCT driver.
   This handler tracks the event flow to determine when HFXO must be running and when the NFCT peripheral can be activated.
   The NFCT peripheral can be activated only when HFXO is running.

   It is recommended to request HFXO asynchronously with a notification when the clock has been started, and activate the NFCT peripheral after receiving this notification.

:c:func:`nfc_platform_cb_request`
   Called by the NFC libraries from the NFCT interrupt context to deliver events to the application callback.
   This function decouples the user callback from the NFCT IRQ handler.

   Use the :c:type:`nfc_lib_cb_resolve_t` function pointer stored during :c:func:`nfc_platform_setup` to invoke the application callback.
   The implementation can call this function directly or schedule the callback in another execution context, for example a work queue or a dedicated thread, to keep the interrupt handler short.

   A direct call is sufficient for minimal bare-metal implementations where the application callback does not use RTOS services.
   Deferred execution is recommended when the callback accesses RTOS primitives or must not run in interrupt context.

   When the ``copy_data`` argument is ``true``, the data referenced by ``p_data`` must be copied before the interrupt handler returns, because the source buffer may become invalid afterward.
   When ``copy_data`` is ``false``, the data pointer must remain valid until the callback is executed.

   For deferred execution, use the ``ctx_len`` and ``data_len`` arguments to copy the callback context and data into your scheduling mechanism.
   These length arguments can be ignored when calling the callback resolution function directly.

:c:func:`nfc_platform_buffer_alloc`
   Allocates the buffer used directly by the NFCT peripheral for data exchange.
   The library calls this function during initialization and uses the returned memory for all subsequent NFC communication.

:c:func:`nfc_platform_buffer_free`
   Releases the buffer previously allocated by :c:func:`nfc_platform_buffer_alloc`.
   The library calls this function when the NFC operation ends.
   If the buffer pointer is ``NULL``, no operation is performed.

.. _nfc_platform_buffer:

Data exchange buffer
====================

The NFC libraries no longer allocate the NFCT data buffer internally.
Your NFC Platform implementation must provide this buffer through the :c:func:`nfc_platform_buffer_alloc` function.

The buffer is accessed directly by the NFCT EasyDMA interface.
On some SoCs, such as the nRF54H20, the buffer must be placed in a memory region accessible by the NFCT peripheral.
Refer to the product specification for memory access requirements.

The required buffer size depends on the NFC tag type:

.. list-table:: NFC Platform buffer sizes
   :header-rows: 1
   :widths: auto

   * - Tag type
     - Allocation size
     - Memory layout
   * - Type 2 Tag
     - 16 bytes
     - Single buffer of :c:macro:`NFC_PLATFORM_T2T_BUFFER_SIZE` bytes
   * - Type 4 Tag
     - 515 bytes
     - 256-byte RX area followed by a 259-byte TX area (:c:macro:`NFC_PLATFORM_T4T_BUFFER_SIZE`)

For Type 4 Tag, the library uses the first 256 bytes for RX (receive) data and the following :c:macro:`NFC_PLATFORM_T4T_BUFFER_SIZE` bytes for TX (transmit) data.
For Type 2 Tag, allocate a single buffer of :c:macro:`NFC_PLATFORM_T2T_BUFFER_SIZE` bytes.

The following example shows a minimal static buffer allocation for Type 4 Tag:

.. code-block:: c

   #include <nfc_platform.h>

   #define NFC_T4T_RX_BUFFER_SIZE 256U
   #define NFC_T4T_TOTAL_BUFFER_SIZE (NFC_T4T_RX_BUFFER_SIZE + NFC_PLATFORM_T4T_BUFFER_SIZE)

   static uint8_t nfc_platform_buffer[NFC_T4T_TOTAL_BUFFER_SIZE];

   uint8_t *nfc_platform_buffer_alloc(size_t size)
   {
       if (size > sizeof(nfc_platform_buffer)) {
           return NULL;
       }

       return nfc_platform_buffer;
   }

   void nfc_platform_buffer_free(uint8_t *p_buffer)
   {
       ARG_UNUSED(p_buffer);
   }

On the nRF54H20, assign a DMA-accessible memory region to the NFCT node in devicetree and place the platform buffer in the corresponding linker section.
The following devicetree snippet shows the NFCT configuration used on the nRF54H20 DK:

.. code-block:: devicetree

   &nfct {
           status = "okay";
           memory-regions = <&cpuapp_dma_region>;
   };

In the |NCS| implementation, the buffer linker section is derived automatically from this property.
See :ref:`nfc_platform_ncs` for details.

.. _nfc_platform_ncs:

|NCS| implementation
====================

In the |NCS|, an implementation of the NFC Platform module for the Zephyr environment is provided in the :file:`subsys/nfc/lib/platform.c` file.
It is enabled automatically when using :kconfig:option:`CONFIG_NFC_T2T_NRFXLIB` or :kconfig:option:`CONFIG_NFC_T4T_NRFXLIB` through the :kconfig:option:`CONFIG_NFC_PLATFORM` Kconfig option.

The implementation performs the following tasks:

* Starts the HFXO when an NFC field is detected and stops it when the field is lost.
  HFXO runs only while an NFC field is present.
* Allocates the NFCT data buffer in a linker section derived from the NFCT devicetree ``memory-regions`` property when it is defined.
  For Type 4 Tag, the |NCS| allocates 518 bytes (``2 * NFC_PLATFORM_T4T_BUFFER_SIZE``), which satisfies the 515-byte requirement of the library.
* Connects the NFCT interrupt and, on affected SoCs, the timer workaround interrupt.
* Decouples the application callback from the NFCT interrupt context.

Callback execution context
--------------------------

The :kconfig:option:`CONFIG_NFC_THREAD_CALLBACK` Kconfig option controls how the :c:func:`nfc_platform_cb_request` function delivers events to the application.
When enabled, callbacks are executed outside the interrupt context using one of the following mechanisms:

* System workqueue (default)
* Dedicated NFC thread (:kconfig:option:`CONFIG_NFC_OWN_THREAD`)

When this option is disabled, the :c:func:`nfc_platform_cb_request` function invokes the callback resolution function directly from the NFCT interrupt context.

When :kconfig:option:`CONFIG_NFC_LOW_LATENCY_IRQ` is enabled, a software interrupt is used to reduce interrupt latency.
This option is not available on the nRF54H Series devices.
When deferred callback execution is enabled, the ``ctx_len`` and ``data_len`` values are stored in the internal ring buffer.

Related Kconfig options include :kconfig:option:`CONFIG_NFC_RING_SIZE`, :kconfig:option:`CONFIG_NFC_LIB_CTX_MAX_SIZE`, and :kconfig:option:`CONFIG_NFCT_IRQ_PRIORITY`.

See the :ref:`nrf:ug_nfc` user guide for general information about using NFC in the |NCS|.

.. _nfc_platform_api:

API documentation
*****************

.. doxygengroup:: nfc_platform
