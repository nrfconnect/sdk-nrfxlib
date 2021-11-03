.. _pdn:

Packet Data Networks
####################

.. contents::
   :local:
   :depth: 2

The Modem library supports selecting which Packet Data Network (PDN) to use on a network socket and for DNS queries.
The configuration of Packet Data Protocol (PDP) contexts, and the activation of PDN connections are not handled by the Modem library.
To configure PDP contexts and activate PDN connections, the application must use the nRF9160 modem Packet Domain AT commands.

When performing network operations on any PDN, the application has the responsibility to ensure that the PDN connection is available.
For more information about how to configure PDP contexts, activate PDN connections, and determine their state, see the `nRF9160 modem Packet Domain AT commands`_ documentation.


Configuring a network socket to use a PDN
*****************************************

The application can select which PDN to use on a specific socket by using the :c:func:`nrf_setsockopt`, with the ``NRF_SO_BINDTODEVICE`` option and specifying the PDN ID as a string, prefixed by ``pdn``.
For example, to select the PDN with ID 1, the application must pass ``pdn1`` as the option value.

The following code shows how to create an IPv4 TCP stream socket and configure it to use the PDN with ID 1:

.. code-block:: c

   fd = nrf_socket(NRF_AF_INET, NRF_SOCK_STREAM, NRF_IPPROTO_TCP);
   nrf_setsockopt(fd, NRF_SOL_SOCKET, NRF_SO_BINDTODEVICE, "pdn1", strlen("pdn1"));


Routing a DNS query on a PDN
****************************

The application can route a DNS query made by :c:func:`nrf_getaddrinfo` to a specific PDN by setting the ``NRF_AI_PDNSERV`` flag in the ``ai_flags`` field of the ``nrf_addrinfo`` input structure, and specify the PDN ID as a string prefixed by ``pdn``, in the ``service`` argument to the :c:func:`nrf_getaddrinfo` call.

The following code shows how to route a DNS query to the PDN with ID 1:

.. code-block:: c

   struct nrf_addrinfo hints = {
     .ai_flags = NRF_AI_PDNSERV, /* flag to specify PDN ID */
   }

   nrf_getaddrinfo("example.com", "pdn1", &hints, &result);
