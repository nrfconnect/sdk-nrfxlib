.. _scan_limitations:

sCAN limitations
################

When working with sCAN, you should be aware of the following limitations.

.. contents::
   :local:
   :depth: 2

v0.1.0
******

Refer to the following detailed descriptions of current limitations:

* The sCAN time segment parameters in :c:struct:`nrf_scan_timing_t` (``prop_seg``, ``phase_seg1``, and ``phase_seg2``) must comply with CAN Specification 2.0 A/B.
* The ``prescaler`` parameter in :c:struct:`nrf_scan_timing_t` has a minimum value of ``8``.
* sCAN fault confinement has not been fully verified.
* At the protocol level, sCAN can receive overload frames but cannot transmit them.
