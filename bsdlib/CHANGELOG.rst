.. _bsdlib_changelog:

Changelog
#########

All notable changes to this project are documented in this file.

bsdlib 0.2.1
************

Updated library with bugfixes:

* Updated ``nrf_inbuilt_key.h`` with smaller documentation fixes.
* Bugfix in the ``nrf_inbuilt_key`` API to allow PSK and Identity to be provisioned successfully.
* Bugfix in the ``nrf_inbuilt_key`` API to allow security tags in the range of 65535 to 2147483647 to be deleted, read, and listed.
* Bugfix in proprietary trace log.

bsdlib 0.2.0
************

Updated library and header files:

* Enabled Nordic Semiconductor proprietary trace log. Increased consumption of the dedicated library RAM, indicated in bsd_platform.h.
* Resolved include of ``stdint.h`` in ``bsd.h``.

bsdlib 0.1.0
************

Initial release.

Added
=====

* Added the following BSD Socket library variants for nrf9160, for soft-float and hard-float builds:

  * ``libbsd_nrf9160_xxaa.a``
  * ``liboberon_2.0.5.a`` (dependency of libbsd)
