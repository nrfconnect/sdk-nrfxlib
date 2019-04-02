.. _bsdlib_changelog:

Changelog
#########

All notable changes to this project are documented in this file.

bsdlib 0.2.3
************

Compatibility warning:
* Updated API towards bsd_os_timedwait function. The timeout parameter is now a
in and out parameter. The bsd_os implementation is now expected to set the
remaining time left of the timeout value in return.


bsdlib 0.2.2
************

Updated library with API for setting APN name when doing getaddrinfo request.

* Providing API through nrf_getaddrinfo, ai_next to set a second hint that defines the APN name to use for getaddrinfo query.
  The hint must be using NRF_AF_LTE, NRF_SOCK_MGMT, and NRF_PROTO_PDN as family, type, and protocol.
  The APN is set through the ai_canonname field.

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
