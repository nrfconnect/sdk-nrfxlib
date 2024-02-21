.. _socket_options_func:

Supported functions and socket options
######################################

.. contents::
   :local:
   :depth: 1

The networking sockets support a list of functions and socket options.
The implementation of networking sockets is namespaced (``nrf_``) to avoid conflicts with functions that might be part of libc or other POSIX compatibility libraries.

Supported functions
*******************

Following is a list of supported functions:

* :c:func:`nrf_socket`
* :c:func:`nrf_close`
* :c:func:`nrf_connect`
* :c:func:`nrf_bind`
* :c:func:`nrf_listen`
* :c:func:`nrf_accept`
* :c:func:`nrf_send`
* :c:func:`nrf_sendto`
* :c:func:`nrf_recv`
* :c:func:`nrf_recvfrom`
* :c:func:`nrf_setsockopt`
* :c:func:`nrf_getsockopt`
* :c:func:`nrf_poll`
* :c:func:`nrf_getaddrinfo`
* :c:func:`nrf_freeaddrinfo`
* :c:func:`nrf_fcntl`
* :c:func:`nrf_getifaddrs`
* :c:func:`nrf_freeifaddrs`
* :c:func:`nrf_inet_pton`
* :c:func:`nrf_inet_ntop`
* :c:func:`nrf_setdnsaddr`

The library supports up to eight networking sockets.

Socket options
**************

The following table shows all socket options supported by the Modem library.

+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| Option level    | Option                          | Type                   | Operations | Description                                                                                |
+=================+=================================+========================+============+============================================================================================+
| NRF_SOL_SOCKET  | NRF_SO_ERROR                    | ``int``                | get        | Requests and clears pending error information on the socket.                               |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET  | NRF_SO_REUSEADDR                | ``int``                | set        | Non-zero requests reuse of local addresses in :c:func:`nrf_bind` (protocol-specific).      |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET  | NRF_SO_RCVTIMEO                 | ``struct nrf_timeval`` | get/set    | Timeout value for a socket receive and accept operations.                                  |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET  | NRF_SO_SNDTIMEO                 | ``struct nrf_timeval`` | get/set    | Timeout value for a socket send operation.                                                 |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET  | NRF_SO_BINDTOPDN                | ``int``                | set        | Bind this socket to a specific PDN ID.                                                     |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET  | NRF_SO_POLLCB                   | ``struct nrf_pollcb``  | set        | Set callbacks for :c:func:`nrf_poll` events on sockets.                                    |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET  | NRF_SO_EXCEPTIONAL_DATA         | ``int``                | get/set    | Send data on socket as part of exceptional event.                                          |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET  | NRF_SO_KEEPOPEN                 | ``int``                | get/set    | Keep the socket open when its PDN connection is lost, or the device is set to flight mode. |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET  | NRF_SO_RAI_LAST                 | ``int``                | set        | Deprecated. Use :c:macro:`NRF_SO_RAI` with value ``NRF_RAI_LAST`` instead.                 |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET  | NRF_SO_RAI_ONE_RESP             | ``int``                | set        | Deprecated. Use :c:macro:`NRF_SO_RAI` with value ``NRF_RAI_ONE_RESP`` instead.             |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET  | NRF_SO_RAI_ONGOING              | ``int``                | set        | Deprecated. Use :c:macro:`NRF_SO_RAI` with value ``NRF_RAI_ONGOING`` instead.              |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET  | NRF_SO_RAI_WAIT_MORE            | ``int``                | set        | Deprecated. Use :c:macro:`NRF_SO_RAI` with value ``NRF_RAI_WAIT_MORE`` instead.            |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET  | NRF_SO_RAI_NO_DATA              | ``int``                | set        | Deprecated. Use :c:macro:`NRF_SO_RAI` with value ``NRF_RAI_NO_DATA`` instead.              |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET  | NRF_SO_RAI                      | ``int``                | set        | Release Assistance Indication (RAI).                                                       |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_IPPROTO_ALL | NRF_SO_SILENCE_ALL              | ``int``                | get/set    | Non-zero disables ICMP echo replies on both IPv4 and IPv6.                                 |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_IPPROTO_IP  | NRF_SO_IP_ECHO_REPLY            | ``int``                | get/set    | Non-zero enables ICMP echo replies on IPv4.                                                |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_IPPROTO_IPV6| NRF_SO_IPV6_ECHO_REPLY          | ``int``                | get/set    | Non-zero enables ICMP echo replies on IPv6.                                                |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_IPPROTO_TCP | NRF_SO_TCP_SRV_SESSTIMEO        | ``int``                | get/set    | Non-zero enables TCP server session timeout after a configurable period of inactivity.     |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE  | NRF_SO_SEC_TAG_LIST             | ``nrf_sec_tag_t *``    | get/set    | Set/get the security tag associated with a socket.                                         |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE  | NRF_SO_SEC_HOSTNAME             | ``char *``             | get/set    | Set/get the hostname to check against during TLS handshakes.                               |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE  | NRF_SO_SEC_CIPHERSUITE_LIST     | ``int *``              | get/set    | Set/get allowed cipher suite list.                                                         |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE  | NRF_SO_SEC_PEER_VERIFY          | ``int``                | get/set    | Set/get Peer verification level.                                                           |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE  | NRF_SO_SEC_ROLE                 | ``int``                | get/set    | Set/get TLS role.                                                                          |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE  | NRF_SO_SEC_SESSION_CACHE        | ``int``                | get/set    | Non-zero enables TLS session cache.                                                        |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE  | NRF_SO_SEC_SESSION_CACHE_PURGE  | ``int``                | set        | Delete TLS session cache.                                                                  |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE  | NRF_SO_SEC_DTLS_HANDSHAKE_TIMEO | ``int``                | get/set    | Set/get DTLS handshake timeout.                                                            |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE  | NRF_SO_SEC_HANDSHAKE_STATUS     | ``int``                | get        | Get latest TLS/DTLS completed handshake procedure end status.                              |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE  | NRF_SO_SEC_DTLS_CID             | ``int``                | get/set    | Set DTLS Connection ID setting.                                                            |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE  | NRF_SO_SEC_DTLS_CID_STATUS      | ``int``                | get        | Get status of DTLS Connection ID.                                                          |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE  | NRF_SO_SEC_DTLS_CONN_SAVE       | ``int``                | set        | Save DTLS connection.                                                                      |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE  | NRF_SO_SEC_DTLS_CONN_LOAD       | ``int``                | set        | Load DTLS connection.                                                                      |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE  | NRF_SO_SEC_CIPHERSUITE_USED     | ``int``                | get        | Get TLS cipher suite used for the handshake.                                               |
+-----------------+---------------------------------+------------------------+------------+--------------------------------------------------------------------------------------------+

The details for each socket option are described as follows:

NRF_SO_ERROR
   When this option is specified, :c:func:`nrf_getsockopt` returns any pending errors on the socket and clears the error status.
   It returns a ``0`` value if there is no pending error.
   :c:macro:`NRF_SO_ERROR` might be used to check for asynchronous errors on connected connectionless-mode sockets or for other types of asynchronous errors.
   :c:macro:`NRF_SO_ERROR` has no default value.

NRF_SO_REUSEADDR
  Allow for the reuse of local addresses by using the :c:func:`nrf_bind` function.
  The default value for :c:macro:`NRF_SO_REUSEADDR` is ``off``, that is, reuse of local addresses is not permitted.

NRF_SO_RCVTIMEO
   Set a timeout value for the :c:func:`nrf_recv`, :c:func:`nrf_recvfrom` and :c:func:`nrf_accept` operations.
   This option accepts an :c:struct:`nrf_timeval` structure with a number of seconds and microseconds specifying the limit on how long to wait for an input operation to complete.
   If a receive operation has blocked for this much time without receiving additional data, it returns with a partial count, or ``errno`` is set to ``NRF_EAGAIN`` or ``NRF_EWOULDBLOCK`` if no data were received.
   If an accept operation has blocked for this much time without receiving an incoming connection, it returns ``-1`` and ``errno`` is set to ``NRF_EAGAIN``.
   The default for this option is the value ``0``, which indicates that a receive or accept operation will not time out.

.. note::
   The minimum supported resolution is 1 millisecond.

NRF_SO_SNDTIMEO
   Set a timeout value for the :c:func:`nrf_connect`, :c:func:`nrf_send`, and :c:func:`nrf_sendto` operations.
   The option accepts an :c:struct:`nrf_timeval` structure with a number of seconds and microseconds specifying the limit on how long to wait for an output operation to complete.
   The default for this option is the value ``0``, which indicates that these operations will not time out, or use the maximum timeout available.

.. note::
   The minimum supported resolution is 1 millisecond.

NRF_SO_BINDTOPDN
   Bind this socket to a particular packet data network ID.
   The passed option is an integer specifying the PDN ID.
   If a socket is bound to an interface, only packets received from that particular interface are processed by the socket.

NRF_SO_POLLCB
   Set a callback for events occurring on this socket such as :c:macro:`NRF_POLLIN` and :c:macro:`NRF_POLLOUT`.
   The :c:member:`nrf_modem_pollcb.callback` function is invoked every time any of the events specified by the :c:member:`nrf_modem_pollcb.events` bitmask field occurs.
   In addition, the :c:macro:`NRF_POLLHUP` and :c:macro:`NRF_POLLERR` events will also trigger the callback, regardless of whether they are set in the :c:member:`nrf_modem_pollcb.events` bitmask field.
   The callback receives a :c:struct:`nrf_pollfd` structure, populated in the same way as it would be populated by the :c:func:`nrf_poll` function.
   If the :c:member:`nrf_modem_pollcb.oneshot` field is set to ``true``, the callback will be invoked only once, and it is automatically unset afterwards.

.. important::
   The callback is invoked in an interrupt service routine.

NRF_SO_EXCEPTIONAL_DATA
   Send data on the socket as part of an exceptional event.
   Exceptional events are described in the 3GPP Release 14 specification.
   The feature requires network support.

   Before using this socket option, the PDN associated with the socket must be configured to allow exceptional events by using the ``AT%EXCEPTIONALDATA`` AT command.
   For more information about the ``AT%EXCEPTIONALDATA`` AT command, see the `nRF91x1 AT Commands Reference Guide`_.

   The socket option is supported from modem firmware v2.0.0.

NRF_SO_KEEPOPEN
   Keep the socket from being closed upon PDN disconnection or reactivation events, or when the device is set to flight mode (``+CFUN=4``).
   Until the PDN connection is reestablished, the socket is not functional.
   Output operations, such as the functions :c:func:`nrf_send` and :c:func:`nrf_connect` return an error and set ``errno`` to ``NRF_ENETUNREACH``.
   Input operations, such as the functions :c:func:`nrf_recv` and :c:func:`nrf_accept` block, since no data can be received, or return an error if the socket or the operation are non-blocking.
   Upon PDN connection reestablishment, the socket behavior depends on the socket type and protocol and on the IP address of the socket's newly established PDN connection, as shown in the following table:

   +-----------------+--------------------+--------------------------------------------------------------+
   | Socket type     | Socket protocol    | Socket is functional (no errors)                             |
   +=================+====================+==============================================================+
   | NRF_SOCK_DGRAM  | NRF_IPPROTO_UDP    | Always                                                       |
   +-----------------+--------------------+--------------------------------------------------------------+
   | NRF_SOCK_DGRAM  | NRF_SPROTO_DTLS1v2 | If using DTLS connection ID                                  |
   +-----------------+--------------------+--------------------------------------------------------------+
   | NRF_SOCK_STREAM | NRF_IPPROTO_TCP    | If the IP address of socket's PDN connection has not changed |
   +-----------------+--------------------+--------------------------------------------------------------+
   | NRF_SOCK_STREAM | NRF_SPROTO_TLS1v2  | If the IP address of socket's PDN connection has not changed |
   +-----------------+--------------------+--------------------------------------------------------------+
   | NRF_SOCK_RAW    | Any                | Always                                                       |
   +-----------------+--------------------+--------------------------------------------------------------+

   If the conditions to keep the socket open after PDN connection reestablishment are not met, the socket will report an error (and set ``errno`` to ``NRF_ENETDOWN``), and must be closed by the application.
   Otherwise, the socket is functional, and the application can use it.
   For further information on how to detect and handle these errors, see :ref:`handling_pdn_errors_on_sockets`.

.. note::
   Putting the device into functional mode ``0`` (``+CFUN=0``) always forces all sockets to be closed, regardless of the :c:macro:`NRF_SO_KEEPOPEN` socket option.

   This socket option is supported from modem firmware v2.0.1.

NRF_SO_RAI
   This socket option is used for Release Assistance Indication (RAI).
   The following values are accepted:

    * ``NRF_RAI_NO_DATA`` - Immediately enter RRC idle mode for this socket. Does not require a following output operation.
    * ``NRF_RAI_LAST`` - Enter RRC idle mode after the next output operation on this socket is complete.
    * ``NRF_RAI_ONE_RESP`` - After the next output operation is complete, wait for one more packet to be received from the network on this socket before entering RRC idle mode.
    * ``NRF_RAI_ONGOING`` - Keep RRC in connected mode after the next output operation on this socket (client side).
    * ``NRF_RAI_WAIT_MORE`` - Keep RRC in connected mode after the next output operation on this socket (server side).

NRF_SO_RAI_LAST
   Deprecated since v2.6.0. Use the :c:macro:`NRF_SO_RAI` socket option with value ``NRF_RAI_LAST`` instead.
   This is a Release assistance indication (RAI) socket option.
   Enter RRC idle mode after the next output operation on this socket is complete.

NRF_SO_RAI_ONE_RESP
   Deprecated since v2.6.0. Use the :c:macro:`NRF_SO_RAI` socket option with value ``NRF_RAI_ONE_RESP`` instead.
   This is a Release assistance indication (RAI) socket option.
   After the next output operation is complete, wait for one more packet to be received from the network on this socket before entering RRC idle mode.

NRF_SO_RAI_ONGOING
   Deprecated since v2.6.0. Use the :c:macro:`NRF_SO_RAI` socket option with value ``NRF_RAI_ONGOING`` instead.
   This is a Release assistance indication (RAI) socket option.
   Keep RRC in connected mode after the next output operation on this socket (client side).

NRF_SO_RAI_WAIT_MORE
   Deprecated since v2.6.0. Use the :c:macro:`NRF_SO_RAI` socket option with value ``NRF_RAI_WAIT_RESP`` instead.
   This is a Release assistance indication (RAI) socket option.
   Keep RRC in connected mode after the next output operation on this socket (server side).

NRF_SO_RAI_NO_DATA
   Deprecated since v2.6.0. Use the :c:macro:`NRF_SO_RAI` socket option with value ``NRF_RAI_NO_DATA`` instead.
   This is a Release assistance indication (RAI) socket option.
   Immediately enter RRC idle mode for this socket.
   Does not require a following output operation.

NRF_SO_SILENCE_ALL
   Disable ICMP echo replies on both IPv4 and IPv6.
   The option value is an integer, a ``1`` value disables echo replies.
   Default value is ``0`` (OFF).

NRF_SO_IP_ECHO_REPLY
   Enable ICMP echo replies on IPv4.
   The option value is an integer, a ``0`` value disables echo replies on IPv4.
   Default value is ``1`` (ON).

NRF_SO_IPV6_ECHO_REPLY
   Enable ICMP echo replies on IPv6.
   The option value is an integer, a ``0`` value disables echo replies on IPv6.
   Default value is ``1`` (ON).

NRF_SO_TCP_SRV_SESSTIMEO
   Configure the TCP server session inactivity timeout for a socket.
   The timeout value is specified in seconds.
   Allowed values for this option range from ``0`` to ``135``, inclusive.
   The default value is ``0`` (no timeout).

.. note::
   This option must be set on the listening socket, but it can be overridden on the accepting socket afterwards.

NRF_SO_SEC_TAG_LIST
   Set an array of security tags to use for credentials when connecting.
   The option length is the size in bytes of the array of security tags.
   Passing ``NULL`` as an option value and ``0`` as an option length removes all security tags associated with a socket.
   By default, no security tags are associated with a socket.
   The maximum number of security tags are given by the :c:macro:`NRF_SOCKET_TLS_MAX_SEC_TAG_LIST_SIZE` macro in :file:`nrf_socket.h`.

NRF_SO_SEC_HOSTNAME
   Set the hostname used for peer verification.
   The option value is a null-terminated string containing the host name to verify against.
   The option length is the size in bytes of the hostname.
   Passing ``NULL`` as an option value and ``0`` as an option length disables peer hostname verification.
   By default, peer hostname verification is disabled.

NRF_SO_SEC_CIPHERSUITE_LIST
   Select which cipher suites are allowed to be used during the TLS handshake.
   The cipher suites are identified by their IANA assigned values.
   By default, all supported cipher suites are allowed.
   For a complete list, see the :ref:`supported cipher_suites <nrf_supported_tls_cipher_suites>` API documentation or refer to the release notes of the modem firmware.
   For more information, see the release notes in the `nRF9160 modem firmware zip file`_ or `nRF91x1 LTE firmware zip file`_ depending on the SiP you are using.

NRF_SO_SEC_PEER_VERIFY
  Set the peer verification level.
  The following values are accepted:

   * :c:macro:`NRF_SO_SEC_PEER_VERIFY_NONE` - No peer verification
   * :c:macro:`NRF_SO_SEC_PEER_VERIFY_OPTIONAL` - Optional peer verification
   * :c:macro:`NRF_SO_SEC_PEER_VERIFY_REQUIRED` - Peer verification is required

   By default, peer verification is required.

NRF_SO_SEC_ROLE
   Set the role for the connection.
   The following values are accepted:

   * :c:macro:`NRF_SO_SEC_ROLE_CLIENT` - Client role
   * :c:macro:`NRF_SO_SEC_ROLE_SERVER` - Server role

   The default role is client.
   For TLS, the choice is implicit from the usage of :c:func:`nrf_listen`, :c:func:`nrf_accept` and :c:func:`nrf_connect`.

NRF_SO_SEC_SESSION_CACHE
   This option controls TLS session caching.
   The following values are accepted:

   * :c:macro:`NRF_SO_SEC_SESSION_CACHE_DISABLED` - Disable TLS session caching
   * :c:macro:`NRF_SO_SEC_SESSION_CACHE_ENABLED` - Enable TLS session caching

   By default, TLS session caching is disabled.

NRF_SO_SEC_SESSION_CACHE_PURGE
   Delete TLS session cache.
   This option is write-only.

NRF_SO_SEC_DTLS_HANDSHAKE_TIMEO
   Set the DTLS handshake timeout.
   The socket option is supported from modem firmware version 1.3.x.
   The following values are accepted:

   * 0 -  No timeout
   * :c:macro:`NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_1S` - 1 second
   * :c:macro:`NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_3S` - 3 seconds
   * :c:macro:`NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_7S` - 7 seconds
   * :c:macro:`NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_15S` - 15 seconds
   * :c:macro:`NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_31S` - 31 seconds
   * :c:macro:`NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_63S` - 63 seconds
   * :c:macro:`NRF_SO_SEC_DTLS_HANDSHAKE_TIMEOUT_123S` - 123 seconds

   The default is no timeout.

NRF_SO_SEC_HANDSHAKE_STATUS
   Get the latest TLS/DTLS completed handshake procedure end status.
   This option is read-only.
   The socket option is supported from modem firmware v2.x.x.
   The following values are expected:

   * :c:macro:`NRF_SO_SEC_HANDSHAKE_STATUS_FULL` - TLS/DTLS attach/negotiation procedure was made with a full handshake, and session cache data was not used or it was not accepted by the server.
   * :c:macro:`NRF_SO_SEC_HANDSHAKE_STATUS_CACHED` - The latest TLS/DTLS attach negotiation was completed successfully with cached session data.

   The default is a full handshake.

.. _dtls_cid_setting:

NRF_SO_SEC_DTLS_CID
   Set DTLS Connection ID setting.
   This socket option decides whether the modem will request or accept a DTLS connection ID when performing the server handshake.
   The socket option is supported from modem firmware v1.3.x, where x is greater than or equal to 5, and v2.x.x.
   The following values are accepted:

   * :c:macro:`NRF_SO_SEC_DTLS_CID_DISABLED` - The connection ID extension is not included in the client hello, so the DTLS connection ID is not used.
   * :c:macro:`NRF_SO_SEC_DTLS_CID_SUPPORTED` - The connection ID extension with a zero-length CID is included in the client hello, so the modem will accept a DTLS connection ID from the server.
   * :c:macro:`NRF_SO_SEC_DTLS_CID_ENABLED` - The connection ID extension with a valid CID is included in the client hello, so the modem will request DTLS connection ID support.

   The default is disabled.

.. _dtls_cid_status:

NRF_SO_SEC_DTLS_CID_STATUS
   Get the status of DTLS connection ID.
   The status tells whether the connection ID is used in the current connection and in which direction it is used.
   This option is read-only.
   The socket option is supported from modem firmware v1.3.x, where x is greater than or equal to 5, and v2.x.x.
   The following values are expected:

   * :c:macro:`NRF_SO_SEC_DTLS_CID_STATUS_DISABLED` - The DTLS connection ID is not included in DTLS records sent to and from the modem.
     This status is returned before the DTLS handshake is complete.
   * :c:macro:`NRF_SO_SEC_DTLS_CID_STATUS_DOWNLINK` - The DTLS connection ID is included only in DTLS records sent to the modem.
   * :c:macro:`NRF_SO_SEC_DTLS_CID_STATUS_UPLINK` - The DTLS connection ID is included only in DTLS records sent from the modem.
   * :c:macro:`NRF_SO_SEC_DTLS_CID_STATUS_BIDIRECTIONAL` - The DTLS connection ID is included in DTLS records sent to and from the modem.

NRF_SO_SEC_DTLS_CONN_SAVE
   Save DTLS connection.
   This socket option can be used to pause a session that is not frequently used by the application.
   Saving the session will free memory in the modem, so the memory can be used for other connections.
   If the socket is closed, the saved DTLS data is cleaned and the connection with the server is lost.

   This option is write-only.
   This option require a DTLS v1.2 connection with renegotiation disabled.
   The socket option is supported from modem firmware v1.3.x, where x is greater than or equal to 5, and v2.x.x.

   Once the DTLS context is saved, the socket can't be used before the DTLS context is loaded with :c:macro:`NRF_SO_SEC_DTLS_CONN_LOAD`.

   This option fails with ``errno`` set to ``NRF_EAGAIN`` if an error happened during serialization of the SSL context.
   This can occur, for instance, when the modem cannot allocate enough memory or if the socket is busy sending or receiving data.
   In this case, the SSL context is still present in the socket, so data sending is still possible.
   The option fails with ``errno`` set to ``NRF_EINVAL`` if the socket option is not supported with the current configuration, for instance because the DTLS handshake is not completed,
   the connection is not an DTLS v1.2 connection with renegotiation disabled, or the connection does not use an AEAD cipher suite (AES-CCM or AES-GCM).
   The option fails with ``errno`` set to ``NRF_ENOMEM`` if the number of saved connections exceeds four.

NRF_SO_SEC_DTLS_CONN_LOAD
   Load DTLS connection.
   This option is write-only.
   The socket option is supported from modem firmware v1.3.x, where x is greater than or equal to 5, and v2.x.x.

   This option fails with ``errno`` set to ``NRF_EAGAIN`` if an error happened during deserialization of the SSL context.
   This can occur, for instance, when the modem cannot allocate enough memory or the connection is not saved.
   The option fails with ``errno`` set to ``NRF_EINVAL`` if the socket option is not supported with the current configuration.

NRF_SO_SEC_CIPHERSUITE_USED
   Get chosen TLS cipher suite.
   This option is read-only.
   The socket option is supported from modem firmware v2.x.x.
