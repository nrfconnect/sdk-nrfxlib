.. _nrf_dm_changelog:

Changelog
#########

.. contents::
   :local:
   :depth: 2

All the notable changes to this project are documented in this file.

main
****

All the notable changes added to the |NCS| main are documented in this section.

Added
=====

* Experimental support for the nRF5340 network core when using ``lib/nRF5340_CPUNET/soft_float/libnrf_dm.a``
* Experimental support for a reduced library for the nRF5340 application core when using ``lib/nRF5340_CPUAPP/hard_float/libnrf_dm_calc.a``
* Experimental support for nRF52832 when using ``lib/nRF52832/hard_float/libnrf_dm.a``

Changes
=======

* Added a quality indicator per tone.
* Reduced the memory usage in libnrf_dm.
* Changed internal timings which makes this release incompatible with previous releases.
* Changed the handling of the reports.

  :c:func:`nrf_dm_calc` and :c:func:`nrf_dm_high_precision_calc` now takes the report as an input.
  The report needs to be populated using :c:func:`nrf_dm_populate_report` before it can be used for :c:func:`nrf_dm_calc` and :c:func:`nrf_dm_high_precision_calc`.
* Removed distance offset compensation of fixed user-configurable offsets.
  This now needs to be done in the application.


Migration information
=====================
* This release is incompatible with previous versions of nrf_dm.
* The application now needs to provide memory for the report.
* Use :c:func:`nrf_dm_populate_report` to load the raw data.
  This needs to be done before passing the report to :c:func:`nrf_dm_calc` or :c:func:`nrf_dm_high_precision_calc`
* :c:func:`nrf_dm_calc` and :c:func:`nrf_dm_high_precision_calc` now takes the report as an input.
* Offset compensation now needs to be performed in the application.
  The relevant options in :c:type:`nrf_dm_config_t` were removed.



nRF Connect SDK v1.9.0
**********************

All the notable changes added to the |NCS| v1.9.0 release are documented in this section.

Added
=====

Initial release

* Added experimental support for nRF52840:

  * ``lib/nRF52840/hard_float/libnrf_dm.a``

* Added experimental support for nRF52833:

  * ``lib/nRF52833/hard_float/libnrf_dm.a``
