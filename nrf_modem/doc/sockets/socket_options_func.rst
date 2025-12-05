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
* :c:func:`nrf_socket_data_enabled_set`
* :c:func:`nrf_socket_data_enabled_get`

The library supports up to eight networking sockets.

Socket options
**************

The following table shows all socket options supported by the Modem library.

+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| Option level     | Option                                      | Type                        | Operations | Description                                                                                          |
+==================+=============================================+=============================+============+======================================================================================================+
| NRF_SOL_SOCKET   | :c:macro:`NRF_SO_ERROR`                     | ``int``                     | get        | Requests and clears pending error information on the socket.                                         |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET   | :c:macro:`NRF_SO_REUSEADDR`                 | ``int``                     | set        | Allow for the reuse of local addresses by using the :c:func:`nrf_bind` function (protocol-specific). |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET   | :c:macro:`NRF_SO_RCVTIMEO`                  | ``struct nrf_timeval``      | get/set    | Timeout value for a socket receive and accept operations.                                            |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET   | :c:macro:`NRF_SO_SNDTIMEO`                  | ``struct nrf_timeval``      | get/set    | Timeout value for a socket send operation.                                                           |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET   | :c:macro:`NRF_SO_BINDTOPDN`                 | ``int``                     | set        | Bind a socket to a Packet Data Network (PDN) ID.                                                     |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET   | :c:macro:`NRF_SO_POLLCB`                    | ``struct nrf_pollcb``       | set        | Set a callback for :c:func:`nrf_poll` events on sockets.                                             |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET   | :c:macro:`NRF_SO_EXCEPTIONAL_DATA`          | ``int``                     | get/set    | Send data on a socket as part of an exceptional event.                                               |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET   | :c:macro:`NRF_SO_KEEPOPEN`                  | ``int``                     | get/set    | Keep the socket open when its PDN connection is lost, or the device is set to flight mode.           |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET   | :c:macro:`NRF_SO_RAI`                       | ``int``                     | set        | Release Assistance Indication (RAI).                                                                 |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET   | :c:macro:`NRF_SO_SENDCB`                    | ``struct nrf_modem_sendcb`` | set        | Callback to be called when a send request is acknowledged.                                           |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_IPPROTO_ALL  | :c:macro:`NRF_SO_SILENCE_ALL`               | ``int``                     | get/set    | Disable ICMP echo replies on both IPv4 and IPv6.                                                     |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_IPPROTO_IP   | :c:macro:`NRF_SO_IP_ECHO_REPLY`             | ``int``                     | get/set    | Enable ICMP echo replies on IPv4.                                                                    |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_IPPROTO_IPV6 | :c:macro:`NRF_SO_IPV6_ECHO_REPLY`           | ``int``                     | get/set    | Enable ICMP echo replies on IPv6.                                                                    |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_IPPROTO_IPV6 | :c:macro:`NRF_SO_IPV6_DELAYED_ADDR_REFRESH` | ``int``                     | get/set    | Delay IPv6 address refresh during power saving mode.                                                 |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_IPPROTO_TCP  | :c:macro:`NRF_SO_TCP_SRV_SESSTIMEO`         | ``int``                     | get/set    | Configure the TCP server session inactivity timeout for a socket.                                    |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_TAG_LIST`              | ``nrf_sec_tag_t *``         | get/set    | Set/get the security tag associated with a socket.                                                   |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_HOSTNAME`              | ``char *``                  | get/set    | Set/get the hostname to check against during TLS handshakes.                                         |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_CIPHERSUITE_LIST`      | ``int *``                   | get/set    | Select which cipher suites are allowed to be used during the TLS handshake.                          |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_PEER_VERIFY`           | ``int``                     | get/set    | Socket option to configure peer verification level.                                                  |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_ROLE`                  | ``int``                     | get/set    | Configure the role for the connection.                                                               |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_SESSION_CACHE`         | ``int``                     | get/set    | Enable TLS session caching.                                                                          |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_SESSION_CACHE_PURGE`   | ``int``                     | set        | Purge TLS session cache.                                                                             |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_DTLS_HANDSHAKE_TIMEO`  | ``int``                     | get/set    | Configure the DTLS handshake timeout value.                                                          |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_HANDSHAKE_STATUS`      | ``int``                     | get        | Get latest TLS/DTLS completed handshake procedure end status.                                        |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_DTLS_CID`              | ``int``                     | get/set    | Enable/disable the DTLS connection ID.                                                               |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_DTLS_CID_STATUS`       | ``int``                     | get        | Get status of DTLS Connection ID.                                                                    |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_DTLS_CONN_SAVE`        | ``int``                     | set        | Save DTLS connection.                                                                                |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_DTLS_CONN_LOAD`        | ``int``                     | set        | Load DTLS connection.                                                                                |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_CIPHERSUITE_USED`      | ``int``                     | get        | Retrieve the cipher suite used during the TLS/DTLS handshake.                                        |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_DTLS_FRAG_EXT`         | ``int``                     | get/set    | Enable/disable the DTLS fragmentation extension.                                                     |
+------------------+---------------------------------------------+-----------------------------+------------+------------------------------------------------------------------------------------------------------+

The details for each socket option are described as follows:

Supported flags for socket functions
************************************

The following flags are supported for socket functions such as :c:func:`nrf_send`, :c:func:`nrf_recv`, :c:func:`nrf_sendto`, and :c:func:`nrf_recvfrom`:

.. _nrf_socket_send_recv_flags_matrix:

The following table summarizes which send/recv flags are supported by each socket API function.

  +-------------------------------+-------------------------------+-------------------------------+-------------------------------+-------------------------------+
  | Flag                          | :ref:`nrf_send`               | :ref:`nrf_sendto`             | :ref:`nrf_recv`               | :ref:`nrf_recvfrom`           |
  |                               |                               |                               |                               |                               |
  +===============================+===============================+===============================+===============================+===============================+
  | :c:macro:`NRF_MSG_PEEK`       | Not supported                 | Not supported                 | Supported [#peek-note]_       | Supported [#peek-note]_       |
  +-------------------------------+-------------------------------+-------------------------------+-------------------------------+-------------------------------+
  | :c:macro:`NRF_MSG_DONTWAIT`   | Supported                     | Supported                     | Supported                     | Supported                     |
  +-------------------------------+-------------------------------+-------------------------------+-------------------------------+-------------------------------+
  | :c:macro:`NRF_MSG_WAITALL`    | Not supported                 | Not supported                 | Supported [#waitall-note]_    | Supported [#waitall-note]_    |
  +-------------------------------+-------------------------------+-------------------------------+-------------------------------+-------------------------------+
  | :c:macro:`NRF_MSG_WAITACK`    | Supported [#waitack-note]_    | Supported [#waitack-note]_    | Not supported                 | Not supported                 |
  +-------------------------------+-------------------------------+-------------------------------+-------------------------------+-------------------------------+

.. [#peek-note]
   Only supported for :ref:`nrf_recv` and :ref:`nrf_recvfrom`. When used, data is copied into the
   user buffer but remains in the socket's input queue until it is read without
   :c:macro:`NRF_MSG_PEEK`.

.. [#waitall-note]
   Only supported for :ref:`nrf_recv` and :ref:`nrf_recvfrom`. Not valid for datagram sockets.
   The call blocks until the requested number of bytes has been received, an error occurs,
   or the connection is closed.

.. [#waitack-note]
   Only supported for :ref:`nrf_send` and :ref:`nrf_sendto`. The operation blocks until data is
   acknowledged by the peer (:c:macro:`NRF_SOCK_STREAM`) or sent on-air by the modem
   (:c:macro:`NRF_SOCK_DGRAM`). The behavior is affected by the :c:macro:`NRF_SO_SNDTIMEO`
   socket option. This flag cannot be used together with the :c:macro:`NRF_SO_SENDCB`
   socket option.

*  :c:macro:`NRF_MSG_PEEK`
      Returns data from the beginning of receive queue without removing it from the input queue.
      This flag is only supported for the :c:func:`nrf_recv` and :c:func:`nrf_recvfrom` functions.
      When this flag is used, the data is copied into the provided buffer but remains in the socket's input queue for future reads.
      Subsequent calls to :c:func:`nrf_recv` or :c:func:`nrf_recvfrom` returns the same data until it is read without the :c:macro:`NRF_MSG_PEEK` flag.
      This flag is useful when an application needs to inspect incoming data without consuming it, allowing for multiple reads of the same data.
      The default behavior is to remove the data from the input queue after it has been read.

* :c:macro:`NRF_MSG_DONTWAIT`
     Enables non-blocking operation for this specific function call.
     If the operation blocks, the function returns immediately with ``-1`` and sets ``errno`` to ``NRF_EAGAIN`` or ``NRF_EWOULDBLOCK``.
     This flag has no effect if the socket is already set to non-blocking mode using :c:func:`nrf_fcntl`.
     This flag is useful when an application wants to perform a single non-blocking operation on a socket that is otherwise in blocking mode.
     The default behavior is to follow the socket's blocking mode.

* :c:macro:`NRF_MSG_WAITALL`
     Requests that the function blocks read operation until the full amount of data requested has been received.
     This flag is only supported for the :c:func:`nrf_recv` and :c:func:`nrf_recvfrom` functions.
     This flag is not valid for datagram sockets
     If this flag is used, the function continues to block and wait for more data until either the requested number of bytes has been received or an error occurs.
     If the connection is closed before all requested data is received, the function returns the number of bytes that were actually received.
     This flag is useful when an application needs to ensure that it receives a complete message or data block in a single operation.
     The default behavior is to return as soon as any data is available, even if it is less than the requested amount.

* :c:macro:`NRF_MSG_WAITACK`
     Requests a blocking send operation until the request is acknowledged.
     This flag is only supported for the :c:func:`nrf_send` and :c:func:`nrf_sendto` functions.
     When this flag is used, write operations on datagram sockets are blocked until the data has been sent on air, and write operations on stream sockets are blocked until data reception is acknowledged by the peer.
     The operation timeout can be configured using the :c:macro:`NRF_SO_SNDTIMEO` socket option.
     The valid timeout values are 1 to 600 seconds.
     For the :c:macro:`NRF_SOCK_STREAM` socket type , the operation is blocked until the data is acknowledged by the peer, and for the :c:macro:`NRF_SOCK_DGRAM` socket type, until the data is sent on-air by the modem.
     This flag is useful in scenarios where confirmation of receipt is critical.
     The default behavior is to return immediately after queuing the data for sending, without waiting for acknowledgment.
     This send flag cannot be used along with the :c:macro:`NRF_SO_SENDCB` socket option.
