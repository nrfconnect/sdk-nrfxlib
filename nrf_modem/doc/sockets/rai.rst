.. _release_assistance_indication:

Release Assistance Indication (RAI)
###################################

.. contents::
   :local:
   :depth: 1

Release Assistance Indication (RAI) is a feature introduced in 3GPP Release 13 and further enhanced in Release 14.
It is designed to optimize the power consumption of IoT devices by allowing them to inform the network that they do not expect any more data to be sent or received after their current transmission.
This enables the device to transition to a low-power state more quickly.
RAI is particularly useful for devices operating on LTE-M and NB-IoT networks, where power efficiency is crucial.
By using RAI, devices can reduce the time they spend in the higher power-consuming ``RRC_CONNECTED`` state, thus saving battery life.

.. note::
   The decision to release the connection is still at the discretion of the network provider.

There are two types of RAI:

* Control Plane RAI (CP-RAI) - Used for control plane data in NB-IoT.
* Access Stratum RAI (AS-RAI) - Used for both control plane and user plane data in LTE-M and NB-IoT.

The following are the advantages of using RAI:

* Reduced Power Consumption - By avoiding unnecessary RRC connection states, devices conserve power.
* Improved Battery Life - Extends the operational lifespan of battery-powered IoT devices.
* Enhanced Network Efficiency - Helps in reducing network congestion by minimizing the time devices spend connected to the network.

Enabling RAI
************

RAI can be enabled or disabled using the following AT commands:

* To enable: ``AT%RAI=1``
* To disable: ``AT%RAI=0``

Using RAI
*********

After enabling RAI, you control it using the :c:func:`nrf_setsockopt` function with the :c:macro:`NRF_SO_RAI` socket option and with the following values:

* ``NRF_RAI_NO_DATA`` - No more outgoing data and no incoming data are expected.
* ``NRF_RAI_LAST`` - No more outgoing data after the next send operation.
* ``NRF_RAI_ONE_RESP`` - Expecting a single response after the next send operation.
* ``NRF_RAI_ONGOING`` - The socket is actively used, and the connection should be maintained.
* ``NRF_RAI_WAIT_MORE`` - The socket is actively used by a server application, and the connection should be maintained.

Code examples
*************

The following code snippets illustrate some cases on how to apply RAI using the :c:func:`nrf_setsockopt` function.

An example that shows how to use ``NRF_RAI_LAST`` when sending a UDP packet:

.. code-block:: c

   #include <nrf_socket.h>

   int send_udp_packet_with_rai_last(int fd)
   {
       int err;
       const char message[] = "Hello, World!";
       struct nrf_sockaddr_in dest_addr = {
           .sin_family = NRF_AF_INET,
           .sin_port = nrf_htons(1234),
           .sin_addr = {
               .s_addr = nrf_htonl(NRF_INADDR_ANY)
           }
       };

       /* Set the RAI_LAST option to indicate that no further data will be sent after this packet */
       int option = NRF_RAI_LAST;
       err = nrf_setsockopt(fd, NRF_SOL_SOCKET, NRF_SO_RAI, &option, sizeof(option));
       if (err) {
           printf("Failed to set NRF_SO_RAI option, error: %d\n", err);
           return err;
       }

       /* Transmit the message */
       err = nrf_sendto(fd, message, sizeof(message), 0,
                        (struct nrf_sockaddr *)&dest_addr, sizeof(dest_addr));
       if (err < 0) {
           printf("Failed to send UDP packet with RAI_LAST, error: %d\n", err);
           return err;
       }

       return 0;
   }

An example that shows how to use ``NRF_RAI_NO_DATA`` when receiving a UDP packet:

.. code-block:: c

   #include <nrf_socket.h>

   int receive_udp_packet_and_set_rai_no_data(int fd)
   {
       int err;
       char buffer[128];
       struct nrf_sockaddr_in src_addr;
       nrf_socklen_t addrlen = sizeof(src_addr);

       /* Receive an incoming message */
       err = nrf_recvfrom(fd, buffer, sizeof(buffer), 0,
                          (struct nrf_sockaddr *)&src_addr, &addrlen);
       if (err < 0) {
           printf("Failed to receive UDP packet, error: %d\n", err);
           return err;
       }

       /* Set the RAI_NO_DATA option to signal that the device does not expect to send or receive further data */
       int option = NRF_RAI_NO_DATA;
       err = nrf_setsockopt(fd, NRF_SOL_SOCKET, NRF_SO_RAI, &option, sizeof(option));
       if (err) {
           printf("Failed to set NRF_SO_RAI option, error: %d\n", err);
           return err;
       }

       return 0;
   }

.. important::
   These examples assume network support for both CP-RAI and AS-RAI to observe the expected results.

.. note::
   RAI flags can be used for secure sockets similarly to other sockets.

.. note::
   Implementing RAI should be a strategic decision based on the application's knowledge of its data transmission patterns.
   If there is uncertainty regarding future data transfers, it is advisable to refrain from using RAI.
   This caution helps avoid the additional energy expenditure associated with re-establishing the radio connection.

Checking RAI status
*******************

To verify if the RAI information was reported to the network:

* For CP-RAI, check the ``ESM DATA TRANSPORT PDU`` sent by the modem.
* For AS-RAI, currently, it is not visible in customer builds without specific modem traces.

RAI best practices
******************

The following are the best practices to be considered while using RAI:

* Use RAI when no additional uplink or downlink traffic is expected in the near term.
* Avoid using RAI if subsequent data transmission is anticipated, as it may lead to unnecessary RRC bearer reestablishments.
* Consider each socket separately for RAI usage, and the modem will aggregate the RAI status across all sockets.

References
**********

* `3GPP TS 24.301 Technical Specification`_

Conclusion
**********

RAI is a useful feature for optimizing network resource usage and device power consumption.
By indicating the end of data transmission, devices can potentially reduce the time they spend connected to the network, saving battery life and freeing up network resources.
The RAI functionality must be tested with your network provider to ensure compatibility and to understand the network's behavior in response to RAI signals.
