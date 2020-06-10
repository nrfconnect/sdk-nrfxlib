.. _mpsl_changelog:

Changelog
#########

All notable changes to this project are documented in this file.

Master branch
*************

Changes
=======
* Change the timeslot implementation to support up to 8 concurrent sessions.
  As part of this change, it is now necessary to supply a timeslot context configuration
  using :cpp:func:`mpsl_timeslot_session_count_set`. All APIs timeslot APIs now take
  a session_id as input. The session id is retrieved from :cpp:func:`mpsl_timeslot_session_open()`.

Bugfixes
========

* Fixed an issue where the high frequency clock and TIMER0 was not turned off during idle periods shorter than 9 ms.
  This increased the average power consumption.
  Such a case could occur when running a Bluetooth LE connection with a connection interval of 7.5 ms. 

nRF Connect SDK v1.3.0
**********************

Added
=====

* Added API for fetching build revision information.
* Added API to set a TX Power envelope.
  Protocols using MPSL will limit their TX power to a value equal to, or lower than, the provided value.
* Added support for using a low-swing and full-swing LF clock.
* The support for integrating an 802.15.4 driver is now improved.

Changes
=======
* Removed ``MPSL_RADIO_NOTIFICATION_DISTANCE_425US`` and replaced it by ``MPSL_RADIO_NOTIFICATION_DISTANCE_420US``.
* On nRF53, the fix for Errata 16 is now applied. 
* The scheduling overhead of a timeslot event is reduced. 

Bugfixes
========

* Fixed an issue on nRF53 where an assert could occur when using a timeslot.

nRF Connect SDK v1.2.0
**********************

Added
=====

* Added a library version with preliminary support for the nRF5340 device.
  The feature set is the same as in the MPSL library for nRF52.
  The following library has been added:

  * :file:`soft-float/libmpsl.a`

Changes
=======

* Removed ``_nrf52`` from the MPSL library file names.
* Version numbers have been removed from the libraries.
* It is no longer allowed to call :cpp:func:`mpsl_init()` if MPSL is already initialized.
* Clock configuration parameters for any stack that uses MPSL must be provided in :c:type:`mpsl_clock_lfclk_cfg_t` to :cpp:func:`mpsl_init()`.
  This now also involves clock accuracy.
* Clock accuracy must be specified in parts per million (ppm).
* Renamed MPSL clock API for high and low frequency clock.

MPSL 0.5.0-1.prealpha
*********************
Initial release.

Added
=====

* Added the following MPSL library build variants:

  * ``hard-float/libmpsl_nrf52.a``
  * ``soft-float/libmpsl_nrf52.a``
  * ``softfp-float/libmpsl_nrf52.a``
