.. _scan_limitations:

sCAN limitations
################

When working with sCAN, you should be aware of the following limitations.

.. contents::
   :local:
   :depth: 2

v0.2.0
******

Refer to the following detailed descriptions of current limitations:

* The sCAN time segment parameters in :c:struct:`nrf_scan_timing_t` (``prop_seg``, ``phase_seg1``, and ``phase_seg2``) must comply with CAN Specification 2.0 A/B.
* The ``prescaler`` parameter in :c:struct:`nrf_scan_timing_t` has a minimum value of ``8``.
* At the protocol level, sCAN can receive overload frames but cannot transmit them.
* In ``ONESHOT`` mode, sCAN always raises ``NRF_SCAN_EVT_TX_COMPLETE`` after the single transmission attempt, whether it succeeded or failed.
  Before the event is raised, the status can report ``ARBITRATIONLOST`` or ``ARBITRATIONLOSTRXOK`` (both reported as ``NRF_SCAN_ERROR_ARBITRATION_LOST`` by :c:func:`nrf_scan_get_status`).
  A frame that won arbitration and matches an RX filter is reported with ``NRF_SCAN_EVT_RX_COMPLETE``, usually in the same interrupt, after ``NRF_SCAN_EVT_TX_COMPLETE``.
  Always read the status on ``NRF_SCAN_EVT_TX_COMPLETE`` to determine the outcome, and resend the frame if needed.
* In the ``BUS_OFF`` state, sCAN does not drive the bus, does not receive frames, and does not report errors.
  It only waits for 128 occurrences of 11 consecutive recessive bits, and then returns to ``ERROR_ACTIVE``.
  :c:func:`nrf_scan_send` is refused while bus-off, and a transmission started through DPPI is dropped without any event.
  ``NRF_SCAN_EVT_TX_COMPLETE`` is raised only for the transmission that was pending when bus-off was entered.
* When several error conditions are flagged at the same time, :c:func:`nrf_scan_get_status` reports the generic ``NRF_SCAN_ERROR_PROTOCOL`` error instead of a specific one.
* Only one transmission can be pending at a time.
  :c:func:`nrf_scan_send` returns ``NRF_SCAN_ERROR_BUSY`` until the previous transmission completes, and there is no transmit queue.
* sCAN supports a maximum of 4 RX mailboxes and filters.
  A mailbox stays locked after a filter match until the application calls :c:func:`nrf_scan_unlock_rx_mailbox`.
  Frames matching a locked mailbox are not stored in it.
* :c:func:`nrf_scan_abort` and :c:func:`nrf_scan_disable` set sCAN to the ``STOPPED`` state, reset the transmit and receive error counters, and clear the timing configuration.
  Call :c:func:`nrf_scan_timing` again before the next :c:func:`nrf_scan_send`.
* sCAN supports classic CAN only, with at most 8 data bytes per frame.
  CAN FD is not supported.
* The baud rate calculation assumes that the SoC runs at its highest base clock frequency (see :ref:`scan_timing`).
* Only one sCAN instance is supported, and the API functions are not reentrant for the same instance (calls from the event callback are an exception).
* DPPI subscriptions are not permitted by default.
  The DPPI role map and subscription permissions set with :c:func:`nrf_scan_dppi_role_map_set` and :c:func:`nrf_scan_dppi_subscribe_enable` do not survive an uninitialization and initialization cycle, and cannot be changed while a request is in flight.

v0.1.0
******

Refer to the following detailed descriptions of current limitations:

* The sCAN time segment parameters in :c:struct:`nrf_scan_timing_t` (``prop_seg``, ``phase_seg1``, and ``phase_seg2``) must comply with CAN Specification 2.0 A/B.
* The ``prescaler`` parameter in :c:struct:`nrf_scan_timing_t` has a minimum value of ``8``.
* sCAN fault confinement has not been fully verified.
* At the protocol level, sCAN can receive overload frames but cannot transmit them.
