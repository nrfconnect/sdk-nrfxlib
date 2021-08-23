.. _mpsl:

Multiprotocol Service Layer
===========================

The Multiprotocol Service Layer (MPSL) is a library of common services for single and multiprotocol implementations.

The library is available as soft-float, softfp-float, and hard-float builds for the nRF52 Series and as soft-float build for the nRF53 Series.

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
  It supports only the nRF52 Series.
* SoC Temperature.
  It provides an API for obtaining the temperature measured on the SoC.
* Radio coexistence.
  It allows the application to interface with several types of packet traffic arbiters (PTAs).


.. toctree::
   :maxdepth: 2
   :caption: Subpages:

   doc/mpsl
   doc/clock
   doc/timeslot
   doc/radio_notification
   doc/tx_power_control
   doc/fem
   doc/cx
   CHANGELOG
   doc/api
