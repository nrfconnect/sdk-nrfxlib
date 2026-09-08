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

* sCAN time segment parameters for :c:struct:`nrf_scan_timing_t` (``prop_seg``, ``phase_seg1`` and ``phase_seg2``) have to be compliant with CAN Specification 2.0 A/B .
* sCAN ``prescaler`` parameter in :c:struct:`nrf_scan_timing_t` has a lower limit of 8.
* sCAN fault containment has not been fully verified.
