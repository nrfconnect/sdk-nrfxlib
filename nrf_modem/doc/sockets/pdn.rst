.. _packet_data_networks:

Packet Data Networks (PDN)
##########################

.. contents::
   :local:
   :depth: 1

The Modem library supports selecting which Packet Data Network (PDN) to use on a network socket and for DNS queries.
The configuration of Packet Data Protocol (PDP) contexts, and the activation of PDN connections are not handled by the Modem library.
To configure PDP contexts and activate PDN connections, the application must use the packet domain AT commands.

When performing network operations on any PDN, the application ensures that the PDN connection is available.
For more information about how to configure PDP contexts, activate PDN connections, and determine their state, see the `Packet domain commands`_ section in the nRF9160 AT Commands Reference Guide or the same section in the `nRF91x1 AT Commands Reference Guide`_ depending on the SiP you are using.

Alternatively, the :ref:`pdn_readme` library in |NCS| can be used to receive events on PDN connectivity and manage packet data networks.


Configuring a socket to use a PDN
*********************************

The application can select which PDN to use on a specific socket by using the :c:func:`nrf_setsockopt` function with the :c:macro:`NRF_SO_BINDTOPDN` socket option and specifying the PDN ID as an integer.

The following code shows how to create an IPv4 TCP stream socket and configure it to use the PDN with ID 1:

.. code-block:: c

   int pdn_id = 1;

   fd = nrf_socket(NRF_AF_INET, NRF_SOCK_STREAM, NRF_IPPROTO_TCP);
   nrf_setsockopt(fd, NRF_SOL_SOCKET, NRF_SO_BINDTOPDN, &pdn_id, sizeof(pdn_id));


Routing a DNS query on a PDN
****************************

The application can route a DNS query using the :c:func:`nrf_getaddrinfo` function to a specific PDN.
This can be done by setting the ``NRF_AI_PDNSERV`` flag in the ``ai_flags`` field of the ``nrf_addrinfo`` input structure, and specifying the PDN ID as a string prefixed by ``pdn``, in the ``service`` argument to the :c:func:`nrf_getaddrinfo` function call.

The following code shows how to route a DNS query to the PDN with ID 1:

.. code-block:: c

   struct nrf_addrinfo hints = {
     .ai_flags = NRF_AI_PDNSERV, /* flag to specify PDN ID */
   }

   nrf_getaddrinfo("example.com", "pdn1", &hints, &result);

.. _handling_pdn_errors_on_sockets:

Handling PDN errors on sockets
******************************

During operation, an active PDN connection may be deactivated due to loss of connectivity or other reasons.

The following applies to sockets that have set the :c:macro:`NRF_SO_KEEPOPEN` socket option:

The loss of an active PDN connection will not result in errors.
Socket operations can return errors due to lack of PDN connectivity.
Upon PDN reactivation, the socket is functional and can resume its operations.
The application is responsible for detecting and re-attempting operations that fail while the socket does not have an active PDN.

The following applies to sockets that have not set the :c:macro:`NRF_SO_KEEPOPEN` socket option:

When a socket operation is attempted on a socket that no longer has an active PDN connection, the operation will return ``-1`` and set ``errno`` to ``NRF_ENETDOWN``.
If the socket is being polled, the :c:func:`nrf_poll` function will set the ``POLLERR`` flag and set the socket error to ``NRF_ENETDOWN``.
The socket error can be retrieved using the :c:macro:`NRF_SO_ERROR` socket option.
When the ``NRF_ENETDOWN`` error is detected, the socket is no longer usable and must be closed by the application.
The application is responsible for detecting when the PDN connection is activated again, before re-creating the socket and attempting the failed operation again.
