.. _mpsl:

Multiprotocol Service Layer
===========================

The Multiprotocol Service Layer (MPSL) is a library of common services for single and multiprotocol implementations.

The library is available as soft-float, softfp-float, and hard-float builds for the nRF52 and nRF53 series.

This library provides the following key features:

* Timeslot.
  It allows the application to schedule its own events concurrently with other protocols.
* Radio notifications.
  It provides a configurable interrupt, usable before and/or after radio activity.
* TX Power control.
  It provides an API for setting the maximum TX power per channel.
* Clock control.
  It provides an API for configuring and controlling the low and high-frequency clock.
* FEM.
  It provides an API for controlling external Front-End Modules.
  It supports only the nRF52 series.
* SoC Temperature.
  It provides an API for obtaining the temperature measured on the SoC.

.. note::
   Please note the following when using the library for the nRF5340:

   * The libraries are for evaluation purposes only.
   * They are neither fully functional, nor properly tested.
   * They are not optimized for performance.


.. toctree::
   :maxdepth: 2
   :caption: Contents:

   CHANGELOG
   doc/mpsl
   doc/timeslot
   doc/radio_notification
   doc/tx_power_control
   doc/fem
   doc/api
