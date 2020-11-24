.. _pdn_extension:

PDN management
##############

.. contents::
   :local:
   :depth: 2

The Modem library supports configuring up to 10 packet data network (PDN) sockets.
PDN sockets are an extension to the standard socket API.
They allow for configuring PDN connections as identified by the user's Access Point Name (APN).

After a PDN socket is configured, it can be used by a data socket (IP or RAW socket).
The purpose of PDN sockets is to dictate where to send the data, or to do DNS queries on the PDN that is associated with the given APN.

The following sections demonstrate pseudo code for typical use cases of PDN sockets.

Creating a PDN socket
*********************

.. code-block:: c

   pdn_fd = nrf_socket(NRF_AF_LTE, NRF_SOCK_MGMT, NRF_NPROTO_PDN);
   nrf_connect(pdn_fd, (struct nrf_sockaddr *)"custom.apn", strlen("custom.apn"));

When creating a PDN socket, the library requests a context ID (CID) to be created.
It then configures the CID, which creates a PDN.
This PDN is used to handle data that is sent over the connection.

The example code shows how to create a PDN socket with the protocol family :c:type:`NRF_NPROTO_PDN` and the proprietary address family :c:type:`NRF_AF_LTE`.
When the :c:func:`nrf_socket` function returns, a CID has been allocated.
Next, the :c:func:`nrf_connect` function is used to configure the created CID to use the application's APN.
If this operation is successful, the library attempts to activate the associated PDN handle.
If this activation succeeds, :c:func:`nrf_connect` returns with success.

The file descriptor that is returned can only be used to operate on the PDN socket itself.
For example, you can use it to set and retrieve socket options, to connect, or to close the PDN socket.
To use the configured PDN, you must use the associated APN name to identify which PDN to use to transfer data (IP/RAW/DNS data).
See also `Binding a data socket to an APN`_.

Closing a PDN socket
********************

.. code-block:: c

   nrf_close(pdn_fd);

The close function for PDN sockets de-registers the associated CID/PDN by deactivating the PDN.
It also removes the configuration of the CID.

Any socket that has been bound to the APN/PDN must be closed before closing the PDN socket.
Otherwise, it will not have any transport and will start reporting errors.

Binding a data socket to an APN
*******************************

.. code-block:: c

   tcp_fd = nrf_socket(NRF_AF_INET, NRF_SOCK_STREAM, NRF_IPPROTO_TCP);
   nrf_setsockopt(tcp_fd, NRF_SOL_SOCKET, NRF_SO_BINDTODEVICE, "custom.apn", strlen("custom.apn"));
   # Data will now flow over "custom.apn" instead of Default context.
   nrf_connect(...);
   nrf_send(tcp_fd, "data", 4, 0);

The example code shows how to create an IPv4 TCP stream socket and bind it to an APN using the NRF_SO_BINDTODEVICE socket option.
The APN name must be set up and activated prior to binding it.
See `Creating a PDN socket`_.

.. note::
   If the PDN that the socket is being bound to has been deactivated by the network, setting the NRF_SO_BINDTODEVICE option returns an error stating that the APN parameter is not valid.
   To fix this error, you must create another PDN socket that is associated with the APN, and bind the data socket to it.

Configuring the type of transport
*********************************

.. code-block:: c

   nrf_setsockopt(pdn_fd, NRF_SOL_PDN, NRF_SO_PDN_AF, [NRF_AF_INET, NRF_AF_INET6],
                  sizeof(nrf_sa_family_t) * 2);

You can configure a PDN socket to use a specific Internet family type of transport, for example, IPv4 only, IPv6 only, or dual stack.
To do so, set the socket option :c:type:`NRF_SO_PDN_AF` to request a specific type of connection.
You must do this after creating the socket and before activating it with :c:func:`nrf_connect`.

To retrieve a list of what address families were activated by the network, call :c:func:`nrf_getsockopt` again after activation, using the same socket option.

.. admonition:: Limitation

   The current implementation is hardcoded to always request a dual IP stack.


Checking if a PDN is active
***************************

.. code-block:: c

   nrf_pdn_state_t state;
   nrf_getsockopt(pdn_fd, NRF_SOL_PDN, NRF_SO_PDN_STATE, &state, sizeof(nrf_pdn_state_t));

The example code shows how to check whether a PDN socket is still active.

However, if a long time has elapsed after the last transmission on any socket bound to a PDN, the PDN socket state might be reported as active, even though the network has deactivated the PDN.
In this case, sending data on any socket bound to that PDN will fail with error ``NRF_ENETDOWN`` and you must create another PDN socket.
See `Creating a PDN socket`_.

.. note::
   Setting the socket option :c:type:`NRF_SO_PDN_STATE` with :c:func:`nrf_setsockopt` to manually control activation/deactivation is not supported.


Retrieving the context ID (CID)
*******************************

.. code-block:: c

   nrf_pdn_context_id_t cid;
   nrf_getsockopt(pdn_fd, NRF_SOL_PDN, NRF_SO_PDN_CONTEXT_ID, &cid, sizeof(nrf_pdn_context_id_t));

In some use cases, it is useful to know the CID that is assigned to a PDN socket.
To retrieve the CID, get the socket option :c:type:`NRF_SO_PDN_CONTEXT_ID` with :c:func:`nrf_getsockopt`.

:c:type:`NRF_SO_PDN_CONTEXT_ID` is a read-only socket option.
The CID is an internal allocation and cannot be configured by the user.


Sending a DNS query
*******************

.. code-block:: c

   struct nrf_addrinfo hints = {
     .ai_family = NRF_AF_INET,
     .ai_socktype = NRF_SOCK_STREAM
   }

   struct nrf_addrinfo apn_hints;

   apn_hints.ai_family = NRF_AF_LTE;
   apn_hints.ai_socktype = NRF_SOCK_MGMT;
   apn_hints.ai_protocol = NRF_PROTO_PDN;
   apn_hints.ai_canonname = "custom.apn";

   hints.ai_next = &apn_hints;

   nrf_getaddrinfo("example.com", NULL, &hints, &result);

The example code shows how to specify an APN to use for a DNS query.
In this example, the DNS hints are extended with an extra linked-list hint that specifies the APN to use for the query.

Like for data sockets, the APN name must be set up and activated before the DNS query.
Otherwise, the query will fail.

Waiting for an IPv6 connection
******************************

.. code-block:: c

   at_fd = nrf_socket(NRF_AF_LTE, 0, NRF_NPROTO_AT);
   nrf_send(at_fd, "AT+CGEREP=1", strlen("AT+CGEREP=1"));
   # -> AT reponse "OK"

   # Set up PDN socket using the APN (PDN initialize and connect)

   nrf_getsockopt(pdn_fd, NRF_SOL_PDN, NRF_SO_PDN_CONTEXT_ID, &context_id, sizeof(uint8_t));
   while not (context_id in ":CGEV IPV6 <cid>");
   nrf_close(at_fd);

When requesting an IPv6 connection (which requires PDN type IPV6 or IPV4V6), the IPv6 address might not be correct even if the request is successful.
The reason for this is that many networks use SLAAC, where the prefix of the IPv6 address is set after the PDN connection is created.
Therefore, you might need to wait with using an IPv6 socket until the prefix is properly set.
If the address cannot be obtained, a ``+CGEV: IPV6 FAIL <cid>`` error is returned.

The example code shows how to set up an AT socket that logs "CGEV" entries.
It signals when IPv6 is available for use by an IPv6 socket, either while connecting to a PDN or afterwards.

Note that the example code does not cover the default PDN (CID 0), because this PDN is always available.
However, the algorithm for checking that IPv6 is available on the default PDN is the same as if it was manually set up with an APN.
