.. _nrf_modem_changelog:

Changelog
#########

.. contents::
   :local:
   :depth: 2

All notable changes to this project are documented in this file.

nrf_modem 2.1.0
***************

* Major improvements to modem tracing.
  The application can now obtain trace data using the newly introduced :c:func:`nrf_modem_trace_get` function.
  Traces can be processed as necessary, and freed using the :c:func:`nrf_modem_trace_processed` function.
  The following functions have been removed from the OS glue:

    * :c:func:`nrf_modem_os_trace_put`
    * :c:func:`nrf_modem_os_trace_alloc`
    * :c:func:`nrf_modem_os_trace_free`
    * :c:func:`nrf_modem_os_trace_irq_set`
    * :c:func:`nrf_modem_os_trace_irq_clear`
    * :c:func:`nrf_modem_os_trace_irq_enable`
    * :c:func:`nrf_modem_os_trace_irq_disable`

  The following functions have been removed from the :file:`nrf_modem.h` file:

    * :c:func:`nrf_modem_trace_irq_handler`
    * :c:func:`nrf_modem_trace_processed_callback`

* Improvements to AT filters.
  AT filters now apply to the formatted AT command.
  The :c:member:`paused` is added to the :c:type:`nrf_modem_at_cmd_filter` structure to pause filters whenever required.
* Added support for modem's POFWARN related errors.
* Fixed a bug where closing a (D)TLS socket during the TLS handshake could make further calls to :c:func:`nrf_connect` fail.
* Fixed a bug where the :c:func:`nrf_send` function could return an error without setting an errno.
* When called with ``NRF_MSG_WAITALL``, the :c:func:`nrf_recv` function now returns the number of bytes received so far in case the socket is closed, or when the TCP connection is terminated by the remote peer.
* Fixed a bug where, in rare cases, the :c:func:`nrf_recv` function on a ``NRF_SOCK_STREAM`` socket incorrectly returned ``0`` even though more bytes were available to read.
* Fixed a bug where, in rare cases, the :c:func:`nrf_recv` function would crash.
* Fixed a few instances of incorrect return values from the :c:func:`nrf_getaddrinfo` function.
* Removed the :c:type:`nrf_socket_family_t` type.
* Removed the unimplemented ``NRF_SO_SEC_CIPHER_IN_USE`` socket option.
* Removed several type definitions.

nrf_modem 2.0.1+b1
******************

* Corrected the ABI for the hard-float binary.

nrf_modem 2.0.1
***************

* Minor improvements to :c:func:`nrf_modem_shutdown`.
* Fixed a bug where :c:func:`nrf_modem_build_version` did not give the correct version number.

nrf_modem 2.0.0
***************

* Numerous fixes and improvements to networking sockets.
* Increased logging output (in log version of the library).
* Improved modem fault handling. A new field has been added to :c:type:`nrf_modem_init_params_t` to receive a callback upon modem faults.
* Added modem fault reasons to the :file:`nrf_modem.h` file.
* Added :c:func:`nrf_modem_is_initialized` function to query the modem initialization status.
* Added :c:func:`nrf_modem_os_event_notify` function to wake up threads sleeping in the :c:func:`nrf_modem_os_timedwait` function.
* Added :c:func:`nrf_modem_os_sem_count_get` function to retrieve a semaphore's count.
* Added :c:func:`nrf_modem_os_trace_alloc` and :c:func:`nrf_modem_os_trace_free` functions to allocate trace metadata on a dedicated memory heap.
* Updated :c:func:`nrf_modem_shutdown` function to shutdown quicker when a debugger is attached or the modem has faulted.
* Updated :c:func:`nrf_modem_os_timedwait` function to return negative values, aligning with other APIs.
* Updated :c:func:`nrf_modem_os_sem_take` function to return ``-NRF_EAGAIN`` on error.
* Renamed the option ``NRF_SO_HOSTNAME`` to ``NRF_SO_SEC_HOSTNAME``.
* Renamed the option ``NRF_SO_CIPHERSUITE_LIST`` to ``NRF_SO_SEC_CIPHERSUITE_LIST``.
* Renamed the option ``NRF_SO_CIPHER_IN_USE`` to ``NRF_SO_SEC_CIPHER_IN_USE``.
* Fixed a bug which could lead to ``NRF_MODEM_GNSS_EVT_FIX`` event being sent before ``NRF_MODEM_GNSS_EVT_UNBLOCKED`` event.
* Removed the :c:func:`nrf_modem_recoverable_error_handler` function.
* Removed the :c:func:`nrf_modem_os_log_strdup` function.
* Removed ``NRF_MODEM_AT_MAX_CMD_SIZE`` and ``NRF_MODEM_IP_MAX_MESSAGE_SIZE`` macros from :file:`nrf_modem_limits.h`.
* Removed unused ``NRF_SPROTO_TLS1v3`` macro.
* Removed unused ``NRF_MSG_DONTROUTE``, ``NRF_MSG_OOB``, ``NRF_MSG_TRUNC`` macros.
* Removed unimplemented ``nrf_select`` function and relative ``NRF_FD_*`` macros.
* Removed unused ``nrf_sec_config_t`` type.

nrf_modem 1.5.2
***************

* Added :c:func:`nrf_modem_os_trace_irq_enable` and :c:func:`nrf_modem_os_trace_irq_disable` functions.
* Added support for calling :c:func:`nrf_modem_trace_processed_callback` from a thread.

nrf_modem 1.5.1
***************

* Fixed a bug where :c:func:`nrf_modem_trace_processed_callback` could crash in some cases.

nrf_modem 1.5.0
***************

* Added support for deferred processing of modem traces.
  Introduced the :c:func:`nrf_modem_trace_processed_callback` function that the application must call after it has processed a trace received in :c:func:`nrf_modem_os_trace_put`.
* It is now possible to unset the AT notification handler by passing NULL to :c:func:`nrf_modem_at_notif_handler_set`.
* The number of required semaphores is now exported in :file:`nrf_modem_os.h`.
* Removed the AT socket.
* Removed the DFU socket.
* Fixed a bug where :c:func:`nrf_getsockopt` do not truncate the socket option as intended when the buffer provided was too small.
* Fixed a bug where closing a socket while another thread was in a :c:func:`recv` operation on the same socket would result in a crash.
* Fixed a bug in the delta DFU interface where the :c:func:`nrf_modem_delta_dfu_offset` call returns an unexpected error code in some cases.

nrf_modem 1.4.1
***************

* Fixed a bug in :c:func:`nrf_send` which could result in the function incorrectly returning -1 and setting the errno to ``NRF_EINPROGRESS``.

nrf_modem 1.4.0
***************

* The PDN socket has been removed.
* The GNSS socket has been removed.
* nrf_errno errno values have been aligned with those of newlibc.
* The :ref:`Modem API <nrf_modem_api>` (:file:`nrf_modem.h`) has been updated to return negative errno values on error.
* The :ref:`Full Modem DFU API <nrf_modem_full_dfu_api>` (:file:`nrf_modem_full_dfu.h`) has been updated to return negative errno values on error.
* The :ref:`GNSS API <nrf_modem_gnss_api>` (:file:`nrf_modem_gnss.h`) has been updated to return negative errno values on error.
* The :c:func:`nrf_modem_gnss_init` and :c:func:`nrf_modem_gnss_deinit` functions have been removed.
* Added the GNSS velocity estimate validity bit ``NRF_MODEM_GNSS_PVT_FLAG_VELOCITY_VALID``.
* Added the GNSS delete bitmask ``NRF_MODEM_GNSS_DELETE_GPS_TOW_PRECISION`` for time-of-week precision estimate.
* Added support for several new fields in the GNSS PVT notification.
* Added support for retrieving GNSS A-GPS data expiry.
* Added the :c:func:`nrf_modem_at_cmd_filter_set` function to set a callback for custom AT commands.
* Fixed a bug in :c:func:`nrf_modem_at_cmd_async` which could result in the wrong response being returned, or a bad memory access.
* The application can no longer specify the APN to be used with a socket using the ``NRF_SO_BINDTODEVICE`` socket option.
* The application can no longer specify the APN to be used for DNS queries using the ``ai_canonname`` field of the input hints structure in :c:func:`nrf_getaddrinfo`.
* Fixed a potential concurrency issue in :c:func:`nrf_getaddrinfo` that would cause the output ``hints`` structure to contain no address upon successful completion.
* Fixed a bug in :c:func:`nrf_getsockopt` that would let the function return an incorrect value in case of error when called on TLS and DTLS sockets.
* Added a parameter to :c:func:`nrf_setdnsaddr` to specify the size of the supplied address.
* Updated :c:func:`nrf_setdnsaddr` to return -1 and set errno on error.
* The :c:func:`nrf_modem_os_application_irq_handler` and :c:func:`nrf_modem_os_trace_irq_handler` functions have been renamed to :c:func:`nrf_modem_application_irq_handler` and :c:func:`nrf_modem_trace_irq_handler` respectively, and their definition has been moved to :file:`nrf_modem.h`.
* Added support for APN rate control feature of modem firmware v1.3.1.
* The glue layer now defines a few new functions used for logging.
* An additional version of the library is released, which is capable of outputting logs. A minimal set of logs has been added for this release.
* All library versions are now released with debugging symbols.

nrf_modem 1.3.0
***************

* Added new AT interface for AT commands.
* Added new Delta DFU interface for modem firmware delta updates.
* The AT socket has been deprecated.
* The DFU socket has been deprecated.
* Fixed a bug in :c:func:`nrf_send` for blocking sockets where calling the function very quickly would cause the application to hang up.

nrf_modem 1.2.2
***************

* Fixed a memory leak in :c:func:`nrf_recv` when reading many packets quickly.
* Fixed a bug in :c:func:`nrf_getaddrinfo` where the function was not returning the proper protocol suggested by the hints.
* Fixed a bug in :c:func:`nrf_getaddrinfo` where specifying ``NRF_AF_UNSPEC`` would incorrectly return an error.
* Fixed a bug in :c:func:`nrf_setsockopt` where the option ``NRF_SO_HOSTNAME`` would incorrectly return an error when the hostname was NULL and optlen was 0.
* Fixed a bug in :c:func:`nrf_modem_gnss_init` where calling the function would lead to field accuracy speed to always be 0 and to the new GNSS events not working.
  This issue would occur when GNSS is not enabled in %XSYSTEMMODE and modem functional mode is not online.

nrf_modem 1.2.1
***************

* Fixed an issue where :c:func:`nrf_getaddrinfo` would set a wrong errno when returning ``NRF_EAI_SYSTEM``.
* Fixed an issue where the ``NRF_SO_TCP_SRV_SESSTIMEO``, ``NRF_SO_SILENCE_IP_ECHO_REPLY`` and ``NRF_SO_SILENCE_IPV6_ECHO_REPLY`` socket options returned an error when set using :c:func:`nrf_setsockopt`.
* Renamed the socket option ``NRF_SO_SILENCE_IP_ECHO_REPLY`` to ``NRF_SO_IP_ECHO_REPLY``.
* Renamed the socket option ``NRF_SO_SILENCE_IPV6_ECHO_REPLY`` to ``NRF_SO_IPV6_ECHO_REPLY``.

nrf_modem 1.2.0
***************

* Added the new GNSS API.
* The GNSS socket has been deprecated.
* Added the ``NRF_SO_TCP_SRV_SESSTIMEO`` socket option to control TCP server timeout.
* Added the ``NRF_AF_UNSPEC`` address family for :c:func:`nrf_getaddrinfo`.
* The ``NRF_POLLIN`` flag is now set with ``NRF_POLLHUP`` for stream sockets.

nrf_modem 1.1.0
***************

* The PDN socket has been deprecated.
* Added the possibility to specify the PDN ID to bind a socket by using the ``NRF_SO_BINDTODEVICE`` socket option.
* Added the ``NRF_AI_PDNSERV`` flag for :c:func:`nrf_getaddrinfo` to specify the PDN ID to route a DNS query.
* Added the ``NRF_SO_SEC_DTLS_HANDSHAKE_TIMEO`` socket option to set the DTLS handshake timeout.
* Added the ``NRF_SO_SEC_SESSION_CACHE_PURGE`` socket option to purge TLS/DTLS session cache.
* Updated :c:func:`nrf_connect` to set ``errno`` to ``NRF_ECONNREFUSED`` when failing due to a missing certificate, wrong certificate, or a wrong private key.
* Updated :c:func:`nrf_getaddrinfo` to return POSIX-compatible error codes from :file:`nrf_gai_error.h`.
* Fixed a potential concurrency issue in :c:func:`nrf_getaddrinfo`.
* Fixed the :c:func:`nrf_poll` behavior when ``fd`` is less than zero.
* Fixed the :c:func:`nrf_poll` behavior when ``nfds`` is zero.

nrf_modem 1.0.3
***************

* Fixed an issue (introduced in version 1.0.2) where :c:func:`nrf_recv` did not return as soon as the data became available on the socket.
* Fixed an issue (introduced in version 1.0.2) where :c:func:`nrf_send` did not correctly report the amount of data sent for TLS and DTLS sockets.

nrf_modem 1.0.2
***************

* Implemented RAI (Release Assistance Indication) support in Modem library.
* Fixed an issue that leads to the reporting of both ``NRF_POLLIN`` and ``NRF_POLLHUP`` by :c:func:`nrf_poll` when a connection is closed by the peer.
* Fixed an issue where a :c:func:`nrf_recv` call on a non-blocking socket would not always behave correctly when the ``NRF_MSG_WAITALL`` flag or the ``NRF_MSG_DONTWAIT`` flag was used.
* Fixed an issue where a blocking :c:func:`nrf_send` could return before sending all the data in some cases.
* Reduced the Heap memory usage in :c:func:`nrf_recv` by 20 percent when using IPv4.
* :c:func:`nrf_listen` on a connected socket will now correctly set errno to ``NRF_EINVAL``, instead of ``NRF_EBADF``.
* :c:func:`nrf_accept` on a non-listening socket will now correctly set errno to ``NRF_EINVAL``, instead of ``NRF_EBADF``.
* Added support for binding RAW sockets to PDNs.

nrf_modem 1.0.1
***************

* Reverted the :c:func:`nrf_getaddrinfo` function behavior to be the same as in v0.8.99, since the LwM2M carrier library is not compatible with the newly introduced POSIX errors codes yet.
* Removed the :file:`nrf_gai_error.h` header.

nrf_modem 1.0.0
***************

* Added support for full modem firmware updates.
* Added support for configuring the size and location of the shared memory area.
* Switched to an external memory allocator that is provided by the glue.
* Added a macro to retrieve the library version.
* Added a function to retrieve the library build version.
* Updated to return POSIX error codes in :c:func:`nrf_getaddrinfo`.
* Fixed an issue where :c:func:`nrf_poll` would incorrectly report ``NRF_POLLERR``.
* Fixed an issue where :c:func:`nrf_getsockopt` called with ``NRF_SO_PDN_STATE`` would incorrectly set errno.
* Fixed an issue where disabling the trace output causes the modem to crash in some situations.

nrf_modem 0.8.99
****************

* Renamed from bsdlib to Modem library (nrf_modem).
* Enabled size optimizations and reduced FLASH footprint.

bsdlib 0.8.1
************

* Fixed compatibility issue with SES.
* Fixed an issue with a strcmp in the PDN socket that might compare to long strings in some cases.

bsdlib 0.8.0
************

* Fixed the issue with stalled TLS handshake.
* Fixed the issue with TLS connection where :c:func:`nrf_connect` hangs.
* Fixed the issue of :c:func:`nrf_sendto` timeout not working in some cases.
* Updated the documentation to reflect that NRF_SO_CHIPER_IN_USE is not currently supported.
* Fixed the issue of missing AT socket and POLLIN events.
* Added support for PDN authentication parameters.
* Added flushing of the GNSS socket queue if the stop command is issued.
* Added support for GPS low accuracy use case.

bsdlib 0.7.9
************

* Fixed an issue introduced with the TLS server support that made :c:func:`nrf_connect` hang forever.

bsdlib 0.7.8
************

* Fixed the issue where the modem communication would not work after a shutdown-init sequence.
* Added TLS server support


bsdlib 0.7.7
************

* Fixed a bug in bsd_init() (introduced in the version 0.7.5) that caused the library to be in an inconsistent state when updating the modem firmware.

bsdlib 0.7.6
************

* Added bsdlib support for ``TLS_CIPHERSUITE_LIST``.
  getsockopt() lists the supported cipher suites and setsockopt() selects a supported cipher suite.
* Support for sending packets sized more than 2048 bytes in TLS socket.

bsdlib 0.7.5
************

* Updated bsd_shutdown() to perform a proper shutdown of the modem and the library.
* Updated bsd_init() to properly support multiple initializations of the modem and the library.

bsdlib 0.7.4
************

* New socket options added:``SILENCE_ALL``, ``SILENCE_IP_ECHO_REPLY``, ``SILENCE_IPV6_ECHO_REPLY`` and ``REUSEADDR``
* Fix to fidoless trace disable

bsdlib 0.7.3
************

* Aligned the naming of ``nrf_pollfd`` structure elements with ``pollfd``.
* Fixed IP socket state after accept() function call.

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
* Fixed a bug where bsdlib_shutdown() did not work correctly.
* PDN is now disconnected properly if :c:func:`nrf_connect` fails.
* Fixed a bug in the GPS socket driver where it would try to free the same memory twice.
* Fixed a bug where TCP/IP session would hang when the transfer is completed.
* Fixed various GNSS documentation issues.

bsdlib 0.6.2
************

* TLS session cache is now disabled by default due to missing support in modem firmware version 1.1.1 and older.
* When passing an address, the function sendto() now sets the errno to ``NRF_EISCONN`` instead of ``NRF_EINVAL`` if the socket type is ``NRF_SOCK_STREAM``.
* Calling connect() on an already connected socket now properly returns ``NRF_EISCONN`` instead of ``NRF_EBADF``.
* Sockets with family ``NRF_AF_LTE`` must now be created with type ``NRF_SOCK_DGRAM``.
* Setting the timeout in recv() to a larger than the maximum supported value now properly returns ``NRF_EDOM`` instead of ``NRF_EINVAL``.
* Fixed an overflow in timeout computation.
* Operations on sockets that do not match the socket family now return ``NRF_EAFNOSUPPORT`` instead of ``NRF_EINVAL``.
* Creating a socket when no sockets are available now returns ``NRF_ENOBUFS`` instead of ``NRF_ENOMEM``.
* Improved validation of family, type, and protocol arguments in socket().
* Improved validation of supported flags on send() and recv() for protocols.

bsdlib 0.6.1
************

* Implemented TLS host name verification.
* Implemented TLS session caching, enabled by default.
* Added the :c:func:`nrf_setdnsaddr` function to set the secondary DNS address.
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

* Fixed internal memory issue in GNSS, which lead to crash when running for hours.

bsdlib 0.5.0
************

* bsd_irrecoverable_handler() has been removed.
  The application no longer needs to implement it to receive errors during initialization, which are instead reported through bsd_init().
* bsd_shutdown() now returns an integer.
* Added RAW socket support.
* Added missing AGPS data models.
* Added APGS notification support.
* Fixed an issue where AGPS data could not be written when the GPS socket was in stopped state.
* Fixed a memory leak in GPS socket.


bsdlib 0.4.3
************

Updated the library with the following changes:

* Added support for signaling if a peer sends larger TLS fragments than receive buffers can handle.
  If this scenario is triggered, ``NRF_ENOBUFS`` is reported in recv().
  The link is also disconnected on TLS level by issuing an ``Encryption Alert``, and TCP is reset from the device side.
  Subsequent calls to send() or recv() report ``NRF_ENOTCONN``.
  The feature will be supported in an upcoming modem firmware version.
* Resolved an issue where sending large TLS messages very close to each other in time would result in a blocking send() that did not return.

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
* Fixed an issue with a blocking send() operation on IP sockets that was not really blocking but returning immediately in case of insufficient memory to perform the operation.
  The new behavior is that blocking sockets will block until the message is sent.
  Also, because of internal limitations, a non-blocking socket might block for a short while until shortage of memory has been detected internally, and then return with errno set to ``NRF_EAGAIN``.
* Corrected errno that is set by send() from ``NRF_ENOMEM`` to ``NRF_EMSGSIZE`` in case of attempts on sending larger messages than supported by the library.
* Added a define ``BSD_IP_MAX_MESSAGE_SIZE`` in :file:`bsd_limits.h` to hint what size is used to report ``NRF_EMSGSIZE`` in the updated send() function.
* Fixed an issue with nrf_inbuilt_key_read() not respecting the ``p_buffer_len`` input parameter, making it possible for the library to write out-of-bounds on the buffer provided.


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
* Modified :c:func:`nrf_inbuilt_key_exists` so that it does not return an error if a key does not exist. `p_exists` will be updated correctly in this case.
* Fixed a memory leak in nrf_inbuilt_key_exists() on error.

bsdlib 0.3.3
************

Updated library with various changes:

* Bug fix internal to the library solving issue with unresponsive sockets.

bsdlib 0.3.2
************

Updated library with various changes:

* Changed socket option ``NRF_SO_RCVTIMEO`` to use nrf_timeval struct instead of uint32_t.
* Improved the PDN socket close (``NRF_PROTO_PDN``) function.
* Added new errno values ``NRF_ENOEXEC``, ``NRF_ENOSPC``, and ``NRF_ENETRESET``.
* Added a return value on bsd_init() to indicate MODEM_DFU result codes or initialization result.
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
  The timeout parameter is now an in and out parameter.
  The bsd_os implementation is now expected to set the remaining time left of the time-out value in return.

bsdlib 0.2.2
************

Updated library with API for setting APN name when doing getaddrinfo request.

* Providing API through nrf_getaddrinfo, ai_next to set a second hint that defines the APN name to use for getaddrinfo query.
  The hint must be using NRF_AF_LTE, NRF_SOCK_MGMT, and NRF_PROTO_PDN as family, type, and protocol.
  The APN is set through the ai_canonname field.

bsdlib 0.2.1
************

Updated library with bug fixes:

* Updated ``nrf_inbuilt_key.h`` with smaller documentation fixes.
* Bug fix in the ``nrf_inbuilt_key`` API to allow PSK and Identity to be provisioned successfully.
* Bug fix in the ``nrf_inbuilt_key`` API to allow security tags in the range of 65535 to 2147483647 to be deleted, read, and listed.
* Bug fix in proprietary trace log.

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
