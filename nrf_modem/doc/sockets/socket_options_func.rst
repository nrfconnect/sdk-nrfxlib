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

+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| Option level     | Option                                      | Type                        | Operations | Description                                                                                |
+==================+=============================================+=============================+============+============================================================================================+
| NRF_SOL_SOCKET   | :c:macro:`NRF_SO_ERROR`                     | ``int``                     | get        | Request and clear pending error information on the socket.                                 |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET   | :c:macro:`NRF_SO_REUSEADDR`                 | ``int``                     | set        | Enable reuse of server addresses.                                                          |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET   | :c:macro:`NRF_SO_RCVTIMEO`                  | ``struct nrf_timeval``      | get/set    | Timeout value for a socket receive and accept operations.                                  |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET   | :c:macro:`NRF_SO_SNDTIMEO`                  | ``struct nrf_timeval``      | get/set    | Timeout value for a socket send operation.                                                 |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET   | :c:macro:`NRF_SO_BINDTOPDN`                 | ``int``                     | set        | Bind a socket to a Packet Data Network (PDN) ID.                                           |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET   | :c:macro:`NRF_SO_POLLCB`                    | ``struct nrf_pollcb``       | set        | Set a callback for :c:func:`nrf_poll` events on sockets.                                   |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET   | :c:macro:`NRF_SO_EXCEPTIONAL_DATA`          | ``int``                     | get/set    | Send data on a socket as part of an exceptional event.                                     |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET   | :c:macro:`NRF_SO_KEEPOPEN`                  | ``int``                     | get/set    | Keep the socket open when its PDN connection is lost, or the device is set to flight mode. |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET   | :c:macro:`NRF_SO_RAI`                       | ``int``                     | set        | Release Assistance Indication (RAI).                                                       |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SOCKET   | :c:macro:`NRF_SO_SENDCB`                    | ``struct nrf_modem_sendcb`` | set        | Callback to be called when a send request is acknowledged.                                 |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_IPPROTO_ALL  | :c:macro:`NRF_SO_SILENCE_ALL`               | ``int``                     | get/set    | Disable ICMP echo replies on both IPv4 and IPv6.                                           |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_IPPROTO_IP   | :c:macro:`NRF_SO_IP_ECHO_REPLY`             | ``int``                     | get/set    | Enable ICMP echo replies on IPv4.                                                          |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_IPPROTO_IPV6 | :c:macro:`NRF_SO_IPV6_ECHO_REPLY`           | ``int``                     | get/set    | Enable ICMP echo replies on IPv6.                                                          |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_IPPROTO_IPV6 | :c:macro:`NRF_SO_IPV6_DELAYED_ADDR_REFRESH` | ``int``                     | get/set    | Delay IPv6 address refresh during power saving mode.                                       |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_IPPROTO_TCP  | :c:macro:`NRF_SO_TCP_SRV_SESSTIMEO`         | ``int``                     | get/set    | Configure the TCP server session inactivity timeout for a socket.                          |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_TAG_LIST`              | ``nrf_sec_tag_t *``         | get/set    | Select an array of security tags to use for credentials when connecting.                   |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_HOSTNAME`              | ``char *``                  | get/set    | Configure the hostname used for peer verification.                                         |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_CIPHERSUITE_LIST`      | ``int *``                   | get/set    | Select which cipher suites are allowed to be used during the TLS handshake.                |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_PEER_VERIFY`           | ``int``                     | get/set    | Configure peer verification level.                                                         |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_ROLE`                  | ``int``                     | get/set    | Configure the role for the connection.                                                     |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_SESSION_CACHE`         | ``int``                     | get/set    | Enable/disable TLS session caching.                                                        |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_SESSION_CACHE_PURGE`   | ``int``                     | set        | Purge TLS session cache.                                                                   |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_DTLS_HANDSHAKE_TIMEO`  | ``int``                     | get/set    | Configure the DTLS handshake timeout value.                                                |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_HANDSHAKE_STATUS`      | ``int``                     | get        | Get the end status of the last completed TLS/DTLS handshake procedure.                     |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_DTLS_CID`              | ``int``                     | get/set    | Enable/disable the DTLS connection ID.                                                     |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_DTLS_CID_STATUS`       | ``int``                     | get        | Get status of DTLS Connection ID.                                                          |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_DTLS_CONN_SAVE`        | ``int``                     | set        | Save DTLS connection.                                                                      |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_DTLS_CONN_LOAD`        | ``int``                     | set        | Load DTLS connection.                                                                      |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_CIPHERSUITE_USED`      | ``int``                     | get        | Retrieve the cipher suite used during the TLS/DTLS handshake.                              |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+
| NRF_SOL_SECURE   | :c:macro:`NRF_SO_SEC_DTLS_FRAG_EXT`         | ``int``                     | get/set    | Enable/disable the DTLS fragmentation extension.                                           |
+------------------+---------------------------------------------+-----------------------------+------------+--------------------------------------------------------------------------------------------+

The details for each socket option are described in the API documentation, notably in the `Modem library socket options API`_ and `Modem library socket TLS API`_ sections.

Supported flags for socket functions
************************************

The following flags are supported for socket functions such as :c:func:`nrf_send`, :c:func:`nrf_recv`, :c:func:`nrf_sendto`, and :c:func:`nrf_recvfrom`:

.. _nrf_socket_send_recv_flags_matrix:

The following table summarizes which send/recv flags are supported by each socket API function.

  +-----------------------------+--------------------+----------------------+--------------------+------------------------+
  | Flag                        | :c:func:`nrf_send` | :c:func:`nrf_sendto` | :c:func:`nrf_recv` | :c:func:`nrf_recvfrom` |
  |                             |                    |                      |                    |                        |
  +=============================+====================+======================+====================+========================+
  | :c:macro:`NRF_MSG_PEEK`     | Not supported      | Not supported        | Supported          | Supported              |
  +-----------------------------+--------------------+----------------------+--------------------+------------------------+
  | :c:macro:`NRF_MSG_DONTWAIT` | Supported          | Supported            | Supported          | Supported              |
  +-----------------------------+--------------------+----------------------+--------------------+------------------------+
  | :c:macro:`NRF_MSG_WAITALL`  | Not supported      | Not supported        | Supported [1]_     | Supported [1]_         |
  +-----------------------------+--------------------+----------------------+--------------------+------------------------+
  | :c:macro:`NRF_MSG_WAITACK`  | Supported [2]_     | Supported [2]_       | Not supported      | Not supported          |
  +-----------------------------+--------------------+----------------------+--------------------+------------------------+

.. [1]
   Not valid for datagram sockets.
   The call blocks until the requested number of bytes has been received, an error occurs, or the connection is closed.

.. [2]
   The operation blocks until data is acknowledged by the peer (:c:macro:`NRF_SOCK_STREAM`) or sent on-air by the modem (:c:macro:`NRF_SOCK_DGRAM`).
   The behavior is affected by the :c:macro:`NRF_SO_SNDTIMEO` socket option.
   This flag cannot be used together with the :c:macro:`NRF_SO_SENDCB` socket option.

The flags are further described in the API documentation, notably in the `Modem library socket flags API`_ section.
