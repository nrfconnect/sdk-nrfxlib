.. _scan_changelog:

sCAN changelog
###############

.. contents::
   :local:
   :depth: 2

All the notable changes to sCAN are documented on this page.
Refer to :ref:`intro_soft_peripherals_platform_support` for supported Nordic devices for each version.

v0.2.0
******

See the list of changes for the current release.

* Added:

  * The ``NRF_SCAN_ERROR_ARBITRATION_LOST`` error type, reported by :c:func:`nrf_scan_get_status` when another node wins arbitration.

* Updated:

  * Fault confinement, which is now fully verified.
    This includes the ``ERROR_WARNING``, ``ERROR_PASSIVE``, ``BUS_OFF`` state transitions and the ``BUS_OFF`` recovery.
  * Documentation of the sCAN state definitions and of the ``ONESHOT`` and ``BUS_OFF`` behavior.
    For details, see the :ref:`scan_limitations` page.

* Bug fixes:

  * Fixed the reporting of error status bits, so that arbitration loss is no longer reported as a generic protocol error.
  * Fixed missed sCAN events when the VPR event flag was cleared before the interrupt handler read it.

v0.1.0
******

This is an initial release.

* Added support for the following features on nRF54L Series devices:

  * Standard and extended message formats.
  * API-based transmission and reception of data and remote frames.
  * Protocol-level transmission and reception of error frames, and reception of overload frames.
  * Error detection and signaling.
  * Fault confinement.
  * Mailbox system.
  * Message filtering using an ID and mask with a configurable bit width.
