.. _mpsl:

Multiprotocol Service Layer
===========================

The Multiprotocol Service Layer (MPSL) is a library of common services for single and multiprotocol implementations.

The library is available as soft-float, softfp-float, and hard-float builds for the nRF52 and nRF53 series.

Key features of this library include:

* Timeslot - allows the application to schedule its own events concurrently with other protocols.
* Radio notifications - a configurable interrupt before and/or after radio activity.
* TX Power control - the MPSL TX Power interface provides APIs to set the maximum TX power per channel.
* Clock control - APIs for configuring and controlling the low and high-frequency clock.
* API for obtaining the temperature measured on the SoC.

.. important::
   For nRF5340, the libraries are:

   * For evaluation purposes only.
   * Not fully functional and not properly tested.
   * Not optimized for performance.


.. toctree::
   :maxdepth: 2
   :caption: Contents:

   CHANGELOG
   doc/mpsl
   doc/timeslot
   doc/radio_notification
   doc/tx_power_control
   doc/api
