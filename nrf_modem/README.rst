.. _nrf_modem:

Modem library
#############
The Modem library is the primary interface for operating the nRF9160 modem, establishing the LTE-M and NB-IoT connections, and receiving the position data (GPS).
The library abstracts away the details of memory management and synchronization, allowing you to focus on writing the application using standardized API calls.

The library aims to be compatible with the socket standard to simplify writing and porting of existing applications.
The library supports up to eight networking sockets.

To get started with socket programming, you should check out some introductions and tutorials, such as `Beej's Guide to Network Programming`_.

The Modem library has been designed in such way that you can port it to any RTOS.
Therefore, procedures that would normally require some kind of OS interaction, like going into sleep mode and IRQ reprioritization, have been moved out of the library to a :file:`.c` file in which OS-specific integration can be implemented.
For more information, see :ref:`nrf_modem_ug_porting`.

.. toctree::
   :maxdepth: 2
   :caption: Subpages:

   doc/supported_features
   doc/architecture
   doc/at_interface
   doc/gnss_interface
   doc/full_dfu
   doc/delta_dfu
   doc/pdn
   doc/tls_dtls_configuration
   doc/modem_trace
   doc/fault_handling
   doc/limitations
   doc/ug_nrf_modem_porting_os
   doc/CHANGELOG
   doc/api
