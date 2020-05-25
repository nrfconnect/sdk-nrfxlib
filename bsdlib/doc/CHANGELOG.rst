.. _bsdlib_changelog:

Changelog
#########

All notable changes to this project are documented in this file.

bsdlib 0.7.3
************

* Aligned the naming of ``nrf_pollfd`` structure elements with ``pollfd``.
* Fixed IP socket state after :cpp:func:`accept()` call.

bsdlib 0.7.2
************

* Added support in bsd_init() to disable fidoless traces and define the memory location and amount reserved for bsdlib. 

bsdlib 0.7.1
************

* Updated GNSS documentation.
* Changing socket mode from non-blocking to blocking when there is a pending connection will now give an error.
* Fixed an issue where FOTA would hang after reboot.

bsdlib 0.7.0
************

* Major rewrite of the lower transport layer to fix an issue where packages were lost in a high bandwidth application.
* Added support for GPS priority setting to give the GPS module priority over LTE to generate a fix.
* Added parameter checking and only return -1 on error for the PDN set socket option function.
* Added support for send timeout on TCP, UDP (including secure sockets), and AT sockets.
* Added support for MSG_TRUNC on AT, GNSS, TCP, and UDP sockets.
* Allocating more sockets than available will now return ENOBUFS instead of ENOMEM.
* Delete mask can now be applied in stopped mode, without the need to transition to started mode first.
* ``ai_canonname`` in the ``addrinfo`` structure is now properly allocated and null-terminated.
* Fixed a bug where :cpp:func:`bsdlib_shutdown()` did not work correctly.
* PDN is now disconnected properly if :cpp:func:`nrf_connect()` fails.
* Fixed a bug in the GPS socket driver where it would try to free the same memory twice.
* Fixed a bug where TCP/IP session would hang when the transfer is completed.
* Fixed various GNSS documentation issues.

bsdlib 0.6.2
************

* TLS session cache is now disabled by default due to missing support in modem firmware version 1.1.1 and older.
* When passing an address, the function :cpp:func:`sendto()` now sets the errno to ``NRF_EISCONN`` instead of``NRF_EINVAL`` if the socket type is ``NRF_SOCK_STREAM``.
* Calling :cpp:func:`connect()` on an already connected socket now properly returns ``NRF_EISCONN`` instead of ``NRF_EBADF``.
* Sockets with family ``NRF_AF_LTE`` must now be created with type ``NRF_SOCK_DGRAM``.
* Setting the timeout in :cpp:func:`recv()` to a larger than the maximum supported value now properly returns ``NRF_EDOM`` instead of ``NRF_EINVAL``.
* Fixed an overflow in timeout computation.
* Operations on sockets that do not match the socket family now return ``NRF_EAFNOSUPPORT`` instead of ``NRF_EINVAL``.
* Creating a socket when no sockets are available now returns ``NRF_ENOBUFS`` instead of ``NRF_ENOMEM``.
* Improved validation of family, type, and protocol arguments in :cpp:func:`socket()`.
* Improved validation of supported flags on :cpp:func:`send()` and :cpp:func:`recv()` for protocols.

bsdlib 0.6.1
************

* Implemented TLS host name verification.
* Implemented TLS session caching, enabled by default.
* Added the :cpp:func:`nrf_setdnsaddr()` function to set the secondary DNS address.
* Removed unused ``BSD_MAX_IP_SOCKET_COUNT`` and ``BSD_MAX_AT_SOCKET_COUNT`` macros.
* Fixed a bug that prevented the application from detecting AGPS notifications.
* Fixed a bug where the application could not allocate the 8th socket.

bsdlib 0.6.0
************

* Removed the ``nrf_inbuilt_key`` API.
  From now on, the application is responsible for provisioning keys using the AT command **%CMNG**.
* Removed the ``nrf_apn_class`` API.
  From now on, the application is responsible for handling the Access Point Name (APN) class.
* Removed the crypto dependency towards ``nrf_oberon`` from the library.
  The library does not need any special cryptography functions anymore, because the application is now responsible for signing AT commands.

bsdlib 0.5.1
************

* Fixed internal memory issue in GNSS which lead to crash when running for hours.

bsdlib 0.5.0
************

* :cpp:func:`bsd_irrecoverable_handler()` has been removed.
  The application no longer needs to implement it to receive errors during initialization, which are instead reported via :cpp:func:`bsd_init()`.
* :cpp:func:`bsd_shutdown()` now returns an integer.
* Added RAW socket support.
* Added missing AGPS data models.
* Added APGS notification support.
* Fixed an issue where AGPS data could not be written when the GPS socket was in stopped state.
* Fixed a memory leak in GPS socket.


bsdlib 0.4.3
************

Updated the library with the following changes:

* Added support for signaling if a peer sends larger TLS fragments than receive buffers can handle.
  If this scenario is triggered, ``NRF_ENOBUFS`` is reported in :cpp:func:`recv()`.
  The link is also disconnected on TLS level by issuing an ``Encryption Alert``, and TCP is reset from the device side.
  Subsequent calls to :cpp:func:`send()` or :cpp:func:`recv()` report ``NRF_ENOTCONN``.
  The feature will be supported in an upcoming modem firmware version.
* Resolved an issue where sending large TLS messages very close to each other in time would result in a blocking :cpp:func:`send()` that did not return.

bsdlib 0.4.2
************

* Reduced ROM footprint.
* Miscellaneous improvements to PDN sockets.
* Fixed an issue when linking with mbedTLS.


bsdlib 0.4.1
************

Updated the library with the following changes:

* Added socket option ``NRF_SO_PDN_CONTEXT_ID`` for PDN protocol sockets to retrieve the Context ID of the created PDN.
* Added socket option ``NRF_SO_PDN_STATE`` for PDN protocol socket to check the active state of the PDN.
* Fixed a TCP stream empty packet indication when a blocking receive got the peer closed notification while waiting for data to arrive.
* Fixed an issue where IP sockets did not propagate a fine-grained error reason, and all disconnect events resulted in ``NRF_ENOTCONN``.
  Now the error reasons could be one of the following: ``NRF_ENOTCONN``, ``NRF_ECONNRESET``, ``NRF_ENETDOWN``, ``NRF_ENETUNREACH``.
* Fixed an issue with a blocking :cpp:func:`send()` operation on IP sockets that was not really blocking but returning immediately in case of insufficient memory to perform the operation.
  The new behavior is that blocking sockets will block until the message is sent.
  Also, because of internal limitations, a non-blocking socket might block for a short while until shortage of memory has been detected internally, and then return with errno set to ``NRF_EAGAIN``.
* Corrected errno that is set by :cpp:func:`send()` from ``NRF_ENOMEM`` to ``NRF_EMSGSIZE`` in case of attempts on sending larger messages than supported by the library.
* Added a define ``BSD_IP_MAX_MESSAGE_SIZE`` in :file:`bsd_limits.h` to hint what size is used to report ``NRF_EMSGSIZE`` in the updated :cpp:func:`send()` function.
* Fixed an issue with :cpp:func:`nrf_inbuilt_key_read()` not respecting the ``p_buffer_len`` input parameter, making it possible for the library to write out-of-bounds on the buffer provided.


bsdlib 0.4.0
************

* Added AGPS support to GNSS socket driver.
* Added support for GNSS power save modes.
* Added support for deleting stored GPS data.
* Changed NRF_CONFIG_NMEA* define names to NRF_GNSS_NMEA* for alignment.


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
