.. _mpsl_changelog:

Changelog
#########

All notable changes to this project are documented in this file.

MPSL 0.5.0-2.prealpha
*********************

Added
=====

* Added a library version with preliminary support for the nRF5340 device.
  The feature set is the same as in the MPSL library for nRF52.
  The following library has been added:

  * :file:`soft-float/libmpsl.a`

Changes
=======

* Removed ``_nrf52`` from the MPSL library file names.
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
