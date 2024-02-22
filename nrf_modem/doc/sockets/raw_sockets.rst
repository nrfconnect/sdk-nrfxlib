.. _raw_sockets:

Raw sockets
###########

The nRF91 Series SiPs support the use of raw sockets, which allow applications to send and receive data at the network layer (Layer 3 in the OSI model) without any transport layer protocol.
To create a raw socket, the application must use the :c:func:`nrf_socket` function with :c:macro:`NRF_AF_PACKET` as the address family and :c:macro:`NRF_SOCK_RAW` as the socket type.

.. important::
   A raw socket cannot keep its data separate from the data of another IP socket when both are operating on the same PDN.
   When a raw socket is active, it intercepts all downlink data intended for other sockets on the same PDN, which can disrupt normal socket operations.

If an application requires the use of raw sockets, it is recommended to use them on a dedicated PDN with a distinct APN to ensure that a separate PDN connection is established.
However, in some cases, establishing a truly dedicated PDN may not be feasible, and the PDN may still be shared with the default PDN.
In those cases, it is advised to close the raw socket before opening any IP sockets.
This ensures that downlink data is not inadvertently routed to the raw socket, allowing IP sockets to function correctly.

For applications that need to use raw sockets, the following code snippet demonstrates how to create a raw socket:

.. code-block:: c

   int fd;
   /* Create a raw socket. */
   fd = nrf_socket(NRF_AF_PACKET, NRF_SOCK_RAW, 0);

It is essential for developers to manage the lifecycle of raw sockets carefully and to be aware of the potential for data routing conflicts when multiple sockets are in use on the same PDN.
