.. _supported_features:

Supported features
##################

The current implementation of the Modem library supports the following features and protocols:

* TCP (client/server)
* UDP
* RAW sockets
* DTLS 1.2 client
* TLS 1.2 client
* AT commands
* GNSS
* A-GPS
* PDN selection for network sockets and DNS queries
* Modem DFU

The library implements the following socket API functions:

* socket
* close
* bind
* listen
* accept
* connect
* recvfrom
* recv
* sendto
* send
* getsockopt
* setsockopt
* fcntl
* poll
* inet_pton
* inet_ntop
* getaddrinfo
* freeaddrinfo
* setdnsaddr
