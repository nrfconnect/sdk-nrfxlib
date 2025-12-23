.. _nrf_modem_dect_clock_sync:

DECT NR+ clock synchronization
###############################

.. contents::
   :local:
   :depth: 2

The DECT clock synchronization interface in the Modem library is used to synchronize the modem's internal clock to an external 1 PPS (pulse-per-second) reference signal.

This interface is designed for use with the DECT NR+ PHY firmware variant of the nRF91x1 firmware.
Clock synchronization enables precise time alignment between multiple devices or with external timing sources, which is critical for coordinated radio operations and time-sensitive applications.

For details about the key hardware capabilities of the nRF9151 SiP and its DECT NR+ PHY firmware implementation, refer to the `nRF9151 DECT NR+ product specification`_.

.. note::
   To obtain the DECT NR+ PHY firmware, you must contact the Nordic Semiconductor sales department.

For more details about the Modem library features that are available for each firmware variant, see :ref:`nrf_modem_variants`.

General operation
*****************

The DECT clock synchronization interface provides the following key features:

* Synchronization to an external 1 PPS signal
* Query of synchronization pulse timing information
* Query of synchronization state and accuracy
* Reference output signal generation

All operations in the clock synchronization interface are asynchronous and their completion is signaled to the application using events.

Clock synchronization states
=============================

The clock synchronization functionality has the following states:

* Inactive - Clock synchronization is not enabled
* Active - Clock synchronization is enabled and the modem is aligning its clock
* Synchronized - Clock synchronization is enabled and the modem clock is synchronized to the external signal

Event handling
==============

Before using any clock synchronization operations, the application must set the event handler for clock sync events by calling the :c:func:`nrf_modem_dect_clock_sync_event_handler_set` function.

All clock synchronization operations are asynchronous and their results are delivered through events to this handler.

Initialization prerequisites
=============================

Before using the clock synchronization interface, the application must:

#. Initialize the Modem library by calling the :c:func:`nrf_modem_init` function.
#. Register the event handler by calling the :c:func:`nrf_modem_dect_clock_sync_event_handler_set` function.

External clock signal
*********************

The external clock signal must be a 1 PPS (pulse-per-second) signal connected to the modem's clock synchronization input.
When synchronization is enabled, the modem adjusts its internal clock rate to ensure that exactly ``NRF_MODEM_DECT_MODEM_TIME_TICK_RATE_KHZ`` clock cycles occur between consecutive external clock pulses.

Hardware connection
===================

The 1 PPS signal must be connected to the SIM_DET pin of the modem.

.. important::
   The SIM_DET pin is instructed to be left floating in the `nRF9151 DK hardware documentation`_.
   The nRF9151 DK does not route the SIM_DET pin to any connector, so custom hardware designed around the nRF9151 SiP is required for this feature.

Signal characteristics
======================

The 1 PPS signal should have the following characteristics:

* Frequency: Exactly 1 Hz (one pulse per second)
* Pulse width: 100 ms (recommended for optimal performance)
* Signal type: Digital pulse
* Accuracy: As required by the application (typically better than 1 ppm)

.. note::
   The quality and stability of the external clock signal directly affects the synchronization accuracy achieved by the modem.

Synchronization accuracy
========================

The modem can achieve a synchronization accuracy (jitter) of up to 0.001 ppm (parts per million) when synchronized to a stable external clock source.

The actual synchronization accuracy can be monitored by querying the synchronization state, which provides the standard deviation of the clock error in parts per million.

Clock synchronization operations
*********************************

The clock synchronization interface provides several operations to control and monitor the synchronization state.

Enabling clock synchronization
===============================

The application can enable external clock synchronization by calling the :c:func:`nrf_modem_dect_clock_sync_enable` function.

This operation is asynchronous.
When the operation has completed, its result is signaled to the application in the :c:enumerator:`NRF_MODEM_DECT_CLOCK_SYNC_EVT_ENABLE` event.

The :c:member:`nrf_modem_dect_clock_sync_enable_event.err` field in the event indicates the result of the operation:

* :c:enumerator:`NRF_MODEM_DECT_CLOCK_SYNC_NO_ERROR` - Operation completed successfully
* :c:enumerator:`NRF_MODEM_DECT_CLOCK_SYNC_ERROR` - An error occurred

Once enabled, the modem begins the process of synchronizing its internal clock to the external 1 PPS signal.
The time to achieve full synchronization depends on the stability of the external clock source and environmental conditions.

Disabling clock synchronization
================================

The application can disable external clock synchronization by calling the :c:func:`nrf_modem_dect_clock_sync_disable` function.

This operation is asynchronous.
When the operation has completed, its result is signaled to the application in the :c:enumerator:`NRF_MODEM_DECT_CLOCK_SYNC_EVT_DISABLE` event.

The :c:member:`nrf_modem_dect_clock_sync_disable_event.err` field in the event indicates the result of the operation:

* :c:enumerator:`NRF_MODEM_DECT_CLOCK_SYNC_NO_ERROR` - Operation completed successfully
* :c:enumerator:`NRF_MODEM_DECT_CLOCK_SYNC_ERROR` - An error occurred

Once disabled, the modem's clock will no longer be synchronized to the external signal and will run at its nominal rate.

Querying pulse timing information
==================================

The application can query the modem for the timing of the next synchronization pulse by calling the :c:func:`nrf_modem_dect_clock_sync_get_pulse_info` function.

This function takes a parameter specifying the pulse direction to query:

* :c:enumerator:`NRF_MODEM_DECT_CLOCK_SYNC_PULSE_INCOMING` - Query the received/incoming sync pulse time
* :c:enumerator:`NRF_MODEM_DECT_CLOCK_SYNC_PULSE_OUTGOING` - Query the transmitted/outgoing sync pulse time

This operation is asynchronous.
When the operation has completed, its result is signaled to the application in the :c:enumerator:`NRF_MODEM_DECT_CLOCK_SYNC_EVT_PULSE_INFO` event.

The :c:member:`nrf_modem_dect_clock_sync_pulse_info_event.err` field in the event indicates the result of the operation:

* :c:enumerator:`NRF_MODEM_DECT_CLOCK_SYNC_NO_ERROR` - Operation completed successfully
* :c:enumerator:`NRF_MODEM_DECT_CLOCK_SYNC_NOT_ACTIVE` - Clock synchronization is not active
* :c:enumerator:`NRF_MODEM_DECT_CLOCK_SYNC_ERROR` - An error occurred

If successful, the event contains:

* :c:member:`nrf_modem_dect_clock_sync_pulse_info_event.pulse_direction` - The queried pulse direction (copied from the request)
* :c:member:`nrf_modem_dect_clock_sync_pulse_info_event.pulse_time` - The pulse occurrence time in modem time units

The pulse time value can be used to schedule radio operations or other time-critical activities in relation to the synchronization pulses.

Querying synchronization state
===============================

The application can query the current synchronization state by calling the :c:func:`nrf_modem_dect_clock_sync_get_state` function.

This operation is asynchronous.
When the operation has completed, its result is signaled to the application in the :c:enumerator:`NRF_MODEM_DECT_CLOCK_SYNC_EVT_STATE` event.

The :c:member:`nrf_modem_dect_clock_sync_state_event.err` field in the event indicates the result of the operation:

* :c:enumerator:`NRF_MODEM_DECT_CLOCK_SYNC_NO_ERROR` - Operation completed successfully
* :c:enumerator:`NRF_MODEM_DECT_CLOCK_SYNC_ERROR` - An error occurred

If successful, the event contains:

* :c:member:`nrf_modem_dect_clock_sync_state_event.synchronized` - Indicates whether the modem clock is synchronized to the external clock (``true`` = synchronized, ``false`` = not synchronized)
* :c:member:`nrf_modem_dect_clock_sync_state_event.clock_std_ppm` - Standard deviation of the clock error in parts per million (only valid when synchronized)

Clock error standard deviation
------------------------------

The :c:member:`nrf_modem_dect_clock_sync_state_event.clock_std_ppm` field provides information about the quality of the synchronization.

This value is encoded in Q3.12 fixed-point format:

* Bit 15: Sign bit (0 = positive, 1 = negative)
* Bits 14-12: Integer part (3 bits)
* Bits 11-0: Fractional part (12 bits)

To convert the value to floating-point parts per million (ppm)::

    float ppm = clock_std_ppm / 4096.0f;

For example:

* A value of 0x1000 (4096 decimal) represents 1.0 ppm standard deviation
* A value of 0x0800 (2048 decimal) represents 0.5 ppm standard deviation
* A value of 0x0200 (512 decimal) represents 0.125 ppm standard deviation

Lower values indicate better synchronization accuracy.

Reference output signal
***********************

The modem can generate a reference output signal that reflects its internal time base.
This signal is output on the COEX1 pin and can be used to synchronize external devices to the modem's clock.

Reference output characteristics
=================================

When enabled, the reference output signal has the following characteristics:

* Frequency: 1 Hz (one pulse per second)
* Pulse width: 100 ms
* Pin: COEX1

The output signal is generated based on the modem's internal time, occurring once per second as measured by the modem.

Enabling reference output
=========================

The application can enable the reference output signal by calling the :c:func:`nrf_modem_dect_clock_sync_enable_ref_output` function.

This operation is asynchronous.
When the operation has completed, its result is signaled to the application in the :c:enumerator:`NRF_MODEM_DECT_CLOCK_SYNC_EVT_ENABLE_REF_OUTPUT` event.

The :c:member:`nrf_modem_dect_clock_sync_enable_ref_output_event.err` field in the event indicates the result of the operation:

* :c:enumerator:`NRF_MODEM_DECT_CLOCK_SYNC_NO_ERROR` - Operation completed successfully
* :c:enumerator:`NRF_MODEM_DECT_CLOCK_SYNC_ERROR` - An error occurred

Disabling reference output
==========================

The application can disable the reference output signal by calling the :c:func:`nrf_modem_dect_clock_sync_disable_ref_output` function.

This operation is asynchronous.
When the operation has completed, its result is signaled to the application in the :c:enumerator:`NRF_MODEM_DECT_CLOCK_SYNC_EVT_DISABLE_REF_OUTPUT` event.

The :c:member:`nrf_modem_dect_clock_sync_disable_ref_output_event.err` field in the event indicates the result of the operation:

* :c:enumerator:`NRF_MODEM_DECT_CLOCK_SYNC_NO_ERROR` - Operation completed successfully
* :c:enumerator:`NRF_MODEM_DECT_CLOCK_SYNC_ERROR` - An error occurred

Once disabled, the COEX1 pin will no longer output the reference signal.

Use cases and application examples
***********************************

The clock synchronization interface enables several use cases that require precise timing coordination.

Synchronized network operation
===============================

Multiple devices can synchronize to a common external clock source (such as a GPS-disciplined oscillator) to ensure their internal clocks remain aligned.
This enables:

* Time-division multiple access (TDMA) schemes with precise slot timing
* Coordinated transmission and reception schedules
* Network-wide time synchronization for distributed systems

Example workflow::

    1. All devices connect to the same 1 PPS reference source
    2. Each device enables clock synchronization
    3. Devices monitor the synchronization state to confirm they are synchronized
    4. Once synchronized, devices can schedule radio operations with precise relative timing
    5. Devices periodically query the synchronization state to monitor clock accuracy

Master-slave clock distribution
================================

A master device can synchronize to an external reference and then redistribute timing to slave devices using the reference output.

Master device::

    1. Enable clock synchronization to external 1 PPS reference
    2. Wait for synchronized state
    3. Enable reference output on COEX1 pin
    4. Slave devices connect to the master's COEX1 output

Slave devices::

    1. Connect clock sync input to master's COEX1 output
    2. Enable clock synchronization
    3. Wait for synchronized state
    4. Begin time-coordinated operations

Pulse timing for scheduled operations
======================================

Applications can query pulse timing information to schedule radio operations precisely aligned with synchronization events::

    1. Enable clock synchronization
    2. Query incoming pulse information
    3. Calculate operation start time relative to the next pulse
    4. Schedule DECT PHY operations at calculated time
    5. Repeat for subsequent operations

This enables precise coordination between the external timing reference and radio activities.

Error handling
**************

All clock synchronization operations can return errors that must be handled by the application.

Common error conditions
=======================

The following error conditions may be encountered:

Clock synchronization not active
---------------------------------

The :c:enumerator:`NRF_MODEM_DECT_CLOCK_SYNC_NOT_ACTIVE` error is returned when querying pulse information while clock synchronization is disabled.

To resolve this error:

#. Enable clock synchronization using :c:func:`nrf_modem_dect_clock_sync_enable`
#. Wait for the :c:enumerator:`NRF_MODEM_DECT_CLOCK_SYNC_EVT_ENABLE` event
#. Retry the pulse information query

General errors
--------------

The :c:enumerator:`NRF_MODEM_DECT_CLOCK_SYNC_ERROR` error indicates that an error occurred during the operation.

Possible causes include:

* Invalid operation parameters
* Modem communication failure
* Hardware fault

If this error occurs:

#. Check that all operation parameters are valid
#. Verify that the Modem library is initialized
#. Ensure the event handler is set
#. Check system logs for additional error information

Function return codes
=====================

All clock synchronization functions return standard error codes:

* ``0`` - Request was successfully sent to the modem
* ``-NRF_EPERM`` - The Modem library is not initialized
* ``-NRF_EFAULT`` - No event handler is set
* ``-NRF_ENOMEM`` - Not enough shared memory for the request

These return codes indicate whether the request was successfully queued, not whether the operation itself succeeded.
The operation result is delivered asynchronously in the corresponding event.

Best practices
**************

Follow these best practices when using the clock synchronization interface:

Event handler setup
===================

Always set the event handler before calling any other clock synchronization functions::

    // Set event handler first
    nrf_modem_dect_clock_sync_event_handler_set(clock_sync_event_handler);

    // Then perform operations
    nrf_modem_dect_clock_sync_enable();

Monitoring synchronization state
=================================

Periodically query the synchronization state to:

* Verify that synchronization is maintained
* Monitor clock accuracy through the standard deviation metric
* Detect if synchronization is lost due to signal issues

Example monitoring interval: Every 10-60 seconds, depending on application requirements.

Handling state transitions
==========================

When enabling clock synchronization, the modem may take time to achieve full synchronization.
Applications should:

#. Enable clock synchronization
#. Wait for the enable event to confirm the operation started
#. Periodically query the synchronization state
#. Wait for ``synchronized`` to become ``true``
#. Monitor ``clock_std_ppm`` to verify acceptable accuracy
#. Begin time-critical operations only after confirming synchronization

Example::

    void clock_sync_event_handler(const struct nrf_modem_dect_clock_sync_event *event)
    {
        switch (event->id) {
        case NRF_MODEM_DECT_CLOCK_SYNC_EVT_ENABLE:
            if (event->clock_sync_enable.err == NRF_MODEM_DECT_CLOCK_SYNC_NO_ERROR) {
                // Successfully enabled, now query state periodically
                start_state_polling();
            }
            break;

        case NRF_MODEM_DECT_CLOCK_SYNC_EVT_STATE:
            if (event->clock_sync_state.synchronized) {
                float ppm = event->clock_sync_state.clock_std_ppm / 4096.0f;
                if (ppm < 1.0f) {  // Good synchronization
                    // Begin time-critical operations
                    begin_coordinated_operations();
                }
            } else {
                // Not synchronized yet, continue polling
                schedule_next_state_query();
            }
            break;
        }
    }

External signal quality
=======================

The quality of clock synchronization depends on the external 1 PPS signal:

* Use a stable, low-jitter clock source (for example, GPS-disciplined oscillator, atomic clock reference)
* Ensure clean signal routing with appropriate termination
* Protect against electrical noise and interference
* Verify signal integrity at the modem input

Reference output considerations
================================

When using the reference output feature:

* The COEX1 pin must not be used for other coexistence functions when reference output is enabled
* Account for the 100 ms pulse width when designing downstream circuitry
* The reference output reflects the modem's internal time, which may be adjusted during synchronization
* Disable reference output when not needed to free the COEX1 pin for other uses

API reference
*************

For detailed information about the clock synchronization API, including function prototypes, parameter descriptions, and data structures, refer to the header file:

* :file:`nrf_modem_dect_clock_sync.h`

Key API functions
=================

* :c:func:`nrf_modem_dect_clock_sync_event_handler_set` - Set the event handler
* :c:func:`nrf_modem_dect_clock_sync_enable` - Enable clock synchronization
* :c:func:`nrf_modem_dect_clock_sync_disable` - Disable clock synchronization
* :c:func:`nrf_modem_dect_clock_sync_get_pulse_info` - Query pulse timing information
* :c:func:`nrf_modem_dect_clock_sync_get_state` - Query synchronization state
* :c:func:`nrf_modem_dect_clock_sync_enable_ref_output` - Enable reference output
* :c:func:`nrf_modem_dect_clock_sync_disable_ref_output` - Disable reference output

Key data structures
===================

* :c:struct:`nrf_modem_dect_clock_sync_event` - Clock synchronization event
* :c:struct:`nrf_modem_dect_clock_sync_enable_event` - Enable event data
* :c:struct:`nrf_modem_dect_clock_sync_disable_event` - Disable event data
* :c:struct:`nrf_modem_dect_clock_sync_pulse_info_event` - Pulse info event data
* :c:struct:`nrf_modem_dect_clock_sync_state_event` - State event data
* :c:struct:`nrf_modem_dect_clock_sync_enable_ref_output_event` - Enable ref output event data
* :c:struct:`nrf_modem_dect_clock_sync_disable_ref_output_event` - Disable ref output event data

Enumerations
============

* :c:enum:`nrf_modem_dect_clock_sync_err` - Clock synchronization error codes
* :c:enum:`nrf_modem_dect_clock_sync_event_id` - Event identifiers
* :c:enum:`nrf_modem_dect_clock_sync_pulse_direction` - Pulse direction values
