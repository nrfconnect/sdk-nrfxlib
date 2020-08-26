.. _at_commands:

AT commands
###########

You can use the BSD socket interface to send AT commands to the LTE modem and to receive the responses.

To do so, create a socket with the proprietary address family :c:type:`NRF_AF_LTE` and the protocol family :c:type:`NRF_PROTO_AT`.
You can then write AT command strings to that socket using :c:func:`nrf_send` and :c:func:`nrf_write`, and they are sent to the modem.
The responses are available when you read from the socket using :c:func:`nrf_read` and :c:func:`nrf_recv`.
Received AT messages are not truncated; to read the full response, provide a sufficiently large buffer or fetch the full response in several read operations.

See the `AT Commands Reference Guide`_ for detailed information on the available AT commands.

The following BSD socket functions are available for the :c:type:`NRF_PROTO_AT` protocol family:

* :c:func:`nrf_socket` for creating a socket
* :c:func:`nrf_close` for closing a socket
* :c:func:`nrf_fcntl` for managing socket options
* :c:func:`nrf_read` for reading data from a socket
* :c:func:`nrf_recv` for reading data from a socket and concatenating several received messages into one receive buffer
* :c:func:`nrf_write` for writing data to a socket
* :c:func:`nrf_send` for writing data to a socket using specific flags

By default, read and write functions are blocking.
To make them non-blocking, use :c:func:`nrf_fcntl` to set the NRF_O_NONBLOCK flag, or pass NRF_MSG_DONTWAIT as flag to the function call.

The following code example shows how to send an AT command and receive the response:

.. code-block:: c

   #include "nrf_socket.h" // socket(), NRF_AF_LTE family type, NRF_PROTO_AT protocol.
   #include <string.h>     // strncmp()

   int func(void)
   {
       // Create a socket for AT commands.
       int fd = nrf_socket(NRF_AF_LTE, 0, NRF_PROTO_AT);

       // Write the AT command.
       nrf_write(fd, "AT+CEREG=2", 10);

       // Allocate a response buffer.
       char ok_buffer[10];

       // Read an AT message (read 10 bytes to ensure that the
       // entire message is consumed).
       int num_of_bytes_recvd = nrf_read(fd, ok_buffer, 10);

       // Compare buffer content against expected return value.
       if (strncmp("OK", ok_buffer, 2) != 0)
       {
           // Return in case of failure.
           return -1;
       }

       // Return on success.
       return 0;
   }
