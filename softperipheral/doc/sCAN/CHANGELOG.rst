.. _scan_changelog:

sCAN changelog
###############

.. contents::
   :local:
   :depth: 2

All the notable changes to sCAN are documented on this page.
Refer to :ref:`intro_soft_peripherals_platform_support` for supported Nordic devices for each version.

v0.1.0
******

This is an initial release.

* Added the following support for nRF54L Series devices:

  * Standard and extended message formats
  * API used to send/receive Data and Remote frames
  * Protocol-handled Error (sending/receiving) and Overload (only receiving) frames.
  * Error detection and signaling
  * Fault confinement
  * Mailbox system
  * Message filtering via ID and mask with configurable bit width
