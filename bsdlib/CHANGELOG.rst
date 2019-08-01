.. _bsdlib_changelog:

Changelog
#########

All notable changes to this project are documented in this file.

bsdlib 0.3.4
************

Updated library with various changes:

* Improved error handling when running out of memory.
* Modified :cpp:func:`nrf_inbuilt_key_exists()` so that it does not return an error if a key does not exist. `p_exists` will be updated correctly in this case.
* Fixed a memory leak in :cpp:func:`nrf_inbuilt_key_exists()` on error.

bsdlib 0.3.3
************

Updated library with various changes:

* Bugfix internal to the library solving issue with unresponsive sockets.

bsdlib 0.3.2
************

Updated library with various changes:

* Changed socket option ``NRF_SO_RCVTIMEO`` to use nrf_timeval struct instead of uint32_t.
* Improved the PDN socket close (``NRF_PROTO_PDN``) function.
* Added new errno values ``NRF_ENOEXEC``, ``NRF_ENOSPC``, and ``NRF_ENETRESET``.
* Added a return value on :cpp:func:`bsd_init()` to indicate MODEM_DFU result codes or initialization result.
* Corrected GNSS struct :c:type:`nrf_gnss_datetime_t` to use correct size on the ms member.
* Updated modem DFU interface.
* Improved error reporting on network or connection loss.
* Corrected the value of ``NRF_POLLNVAL``.
* Improved TCP peer stream closed notification and empty packet indication.

bsdlib 0.3.1
************

Updated library with various changes:

* Corrected GNSS API to not fault if not read fast enough.
* Improved length reporting on GNSS NMEA strings to report length until zero-termination.
* Improved closing of GNSS socket. If closed, it will now also stop the GNSS from running.
* Corrected bitmask value of NRF_GNSS_SV_FLAG_UNHEALTHY.
* Added side API for APN Class management.
* Removed NRF_SO_PDN_CLASS from nrf_socket.h as it is replaced by side API for APN class management.
* Improved nrf_poll() error return on non-timeout errors to be NRF_EAGAIN, to align with standard return codes from poll().
* Added implementation of inet_pton() and inet_ntop().
* Added empty packet to indicate EOF when TCP peer has closed the connection.
* Added NRF_POLLHUP to poll() bitmask to indicate sockets that peer has closed the connection (EOF).

bsdlib 0.3.0
************

Updated library with experimental GNSS support.

bsdlib 0.2.4
************

Updated library with bug fixes:

* Fix issue of reporting NRF_POLLIN on a socket handle using nrf_poll, even if no new data has arrived.
* Fix issue of sockets not blocking on recv/recvfrom when no data is available.

bsdlib 0.2.3
************

Updated library with various changes:

* Updated library to use nrf_oberon v3.0.0.
* Updated the library to be deployed without inbuilt libc or libgcc symbols
  (-nostdlib -nodefaultlibs -nostartfiles -lnosys).
* Fixed issues with some unresolved symbols internal to the library.
* Updated API towards bsd_os_timedwait function.
  The time-out parameter is now an in and out parameter.
  The bsd_os implementation is now expected to set the remaining time left of the time-out value in return.

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
