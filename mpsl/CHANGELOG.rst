.. _mpsl_changelog:

Changelog
#########

.. contents::
   :local:
   :depth: 2

All the notable changes to this project are documented in this file.

Master branch
*************

All the notable changes added to the master branch are documented in this section.

Added
=====

* Added support for nRF21540 GPIO Front-End Module for nRF53 Series.
* Added a public interface for the radio coexistence protocol API (KRKNWK-7490).
  This public interface can be used to replace the example implementation of the radio coexistence with other implementations compliant with other Packet Traffic Arbitration (PTA) interfaces.

Changes
=======

* The :file:`mpsl_fem_protocol_api.h` was moved from the :file:`include` directory to the :file:`include/protocol` directory (KRKNWK-10507).
* The start signal jitter for the timeslot API (``MPSL_TIMESLOT_START_JITTER_US``) has been reduced to 1us. (DRGN-15842).

nRF Connect SDK v1.6.0
**********************

All the notable changes included in the nRF Connect SDK v1.6.0 release are documented in this section.

Added
=====

* Added an API to set and get a flag that determines whether to apply the DEVICE-CONFIG-254.
  The API user shall set the flag to desired value with :c:func:`mpsl_fem_device_config_254_shall_apply_set`.
  The protocol stacks' responsibility is to check the flag value using :c:func:`mpsl_fem_device_config_254_shall_apply_get` and apply the configuration when the flag value is true.
* Added support for the nRF5340 device, which was previously only supported for evaluation purposes (DRGN-8639).
* Added a function to the FEM protocol API to disable FEM when no radio activity is expected.
  This function is intended to reduce power consumption (KRKNWK-8842).

Changes
=======

* Reserved TIMER1 for use by MPSL on nRF5340 (DRGN-15721).

Bug fixes
=========

* Fixed an issue where the clock configuration option :c:member:`mpsl_clock_lfclk_cfg_t.skip_wait_lfclk_started` did not work as expected with nRF5340 devices (DRGN-15223).

nRF Connect SDK v1.5.0
**********************

All the notable changes included in the nRF Connect SDK v1.5.0 release are documented in this section.

Added
=====

* Added a new signal to the MPSL timeslot: ``MPSL_TIMESLOT_SIGNAL_OVERSTAYED``.
  This signal is given to the application when a timeslot session is closed too late (DRGN-14677).

* Added a new clock configuration option :c:member:`skip_wait_lfclk_started` in :c:struct:`mpsl_clock_lfclk_cfg_t`, which does not wait for the start of the low-frequency clock (DRGN-14204).

* Added macro MPSL_RESERVED_PPI_CHANNELS for a bit mask of (D)PPI channels reserved by MPSL (DRGN-13356).

Bug fixes
=========

* Fixed an issue where the low-frequency clock was configured incorrectly when the source configuration signal was set to either External Full swing or External Low swing (DRGN-15064).

* Fixed an issue where MPSL waited for the low-frequency clock to start even though it was configured not to wait for it (DRGN-15176).

nRF Connect SDK v1.4.0
**********************

All the notable changes included in the nRF Connect SDK v1.4.0 release are documented in this section.

Changes
=======

* Changed the timeslot implementation to support up to 8 concurrent sessions (DRGN-13952).
  It is now necessary to supply a timeslot context configuration using :c:func:`mpsl_timeslot_session_count_set`.
  All timeslot APIs now take a ``session_id`` as input.
  The session id is retrieved from :c:func:`mpsl_timeslot_session_open`.

* Added an API to use Front-End Modules, like the nRF21540 GPIO or a simple GPIO, with the protocols and an API to configure them using the application.
  Only the nRF52 Series is supported.

Bug fixes
=========

* Fixed an issue where both the high-frequency clock and ``TIMER0`` were not turned off during idle periods shorter than 9 ms (DRGN-14152).
  This increased the average power consumption.
  Such a case could occur when running a |BLE| connection with a connection interval of 7.5 ms.

Known Issues and limitations
============================

See the :ref:`nrf:known_issues` page in |NCS| for the list of known issues and limitations for this release.

nRF Connect SDK v1.3.0
**********************

All the notable changes included in the nRF Connect SDK v1.3.0 release are documented in this section.

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

Bug fixes
=========

* Fixed an issue on nRF53 where an assert could occur when using a timeslot.

nRF Connect SDK v1.2.0
**********************

All the notable changes included in the nRF Connect SDK v1.2.0 release are documented in this section.

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
* It is no longer allowed to call :c:func:`mpsl_init` if MPSL is already initialized.
* Clock configuration parameters for any stack that uses MPSL must be provided in :c:type:`mpsl_clock_lfclk_cfg_t` to :c:func:`mpsl_init`.
  This now also involves clock accuracy.
* Clock accuracy must be specified in parts per million (ppm).
* Renamed the MPSL clock API for the high-frequency and low-frequency clocks.

MPSL 0.5.0-1.prealpha
*********************

Initial release.

Added
=====

* Added the following MPSL library build variants:

  * ``hard-float/libmpsl_nrf52.a``
  * ``soft-float/libmpsl_nrf52.a``
  * ``softfp-float/libmpsl_nrf52.a``
