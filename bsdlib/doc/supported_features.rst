.. _supported_features:

Supported features
##################

The current implementation of the BSD library supports the following features and protocols:

* TCP (client/server)
* UDP
* RAW sockets
* DTLS 1.2 client
* TLS 1.2 client
* AT commands
* GNSS
* A-GPS
* PDN management
* Modem DFU

.. note::
   The DNS address management protocol is not yet implemented.

The library implements the following standard BSD functions:

* socket
* close
* bind
* listen
* accept
* connect
* recvfrom
* recv
* read
* sendto
* send
* write
* getsockopt
* setsockopt
* fcntl
* poll
* inet_pton
* inet_ntop
* getaddrinfo
* freeaddrinfo

.. note::
   Each socket supports a subset of these functions.
