.. nrf_modem_api:

API documentation
#################

.. contents::
   :local:
   :depth: 2

Library Management
******************

.. doxygengroup:: nrf_modem
   :project: nrfxlib
   :members:

nrf_modem_init() return values for modem firmware updates
=========================================================

.. doxygengroup:: nrf_modem_dfu
   :project: nrfxlib
   :members:

Limits of the Modem library
***************************

.. doxygengroup:: nrf_modem_limits
   :project: nrfxlib
   :members:

Resources for the Modem library
*******************************

.. doxygengroup:: nrf_modem_platform_ipc
   :project: nrfxlib
   :members:

.. doxygengroup:: nrf_modem_reserved_memory
   :project: nrfxlib
   :members:

.. doxygengroup:: nrf_modem_reserved_interrupts
   :project: nrfxlib
   :members:

Socket parameter enumerators
****************************

Socket families
===============

.. doxygengroup:: nrf_socket_families
   :project: nrfxlib
   :members:

Socket types
============

.. doxygengroup:: nrf_socket_types
   :project: nrfxlib
   :members:

Socket protocols
================

.. doxygengroup:: nrf_socket_protocols
   :project: nrfxlib
   :members:

Socket API
**********

.. doxygengroup:: nrf_socket_api
   :project: nrfxlib
   :members:

TLS socket
**********

.. doxygengroup:: nrf_socket_tls
   :project: nrfxlib
   :members:

GNSS socket
***********

GNSS socket options
===================

.. doxygengroup:: nrf_socket_gnss_options
   :project: nrfxlib
   :members:

GNSS socket option data types
=============================

.. doxygengroup:: nrf_socketopt_gnss_types
   :project: nrfxlib
   :members:

NMEA mask enumerator
====================

.. doxygengroup:: nrf_socket_gnss_nmea_str_mask
   :project: nrfxlib
   :members:

Power save mode enumerator
==========================

.. doxygengroup:: nrf_socket_gnss_psm_modes
   :project: nrfxlib
   :members:

Use case enumerator
===================

.. doxygengroup:: nrf_socket_gnss_use_case_modes
   :project: nrfxlib
   :members:

GNSS socket data frames
=======================

.. doxygengroup:: nrf_socket_gnss_data_frame
   :project: nrfxlib
   :members:

.. _gnss_socket_agps_df:

GNSS socket AGPS data frames
============================

.. doxygengroup:: nrf_socket_gnss_data_agps
   :project: nrfxlib
   :members:

DFU socket
**********

.. doxygengroup:: nrf_socket_dfu
   :project: nrfxlib
   :members:

PDN socket
**********

.. doxygengroup:: nrf_socket_pdn
   :project: nrfxlib
   :members:

Socket address resolution API
*****************************

.. doxygengroup:: nrf_socket_api_utils
   :project: nrfxlib
   :members:

File descriptor sets API
************************

File descriptor sets are used as input to the nrf_select() function for doing I/O
multiplexing. The maximum number of descriptors contained in a set is defined by
NRF_FD_SETSIZE.

.. doxygengroup:: nrf_fd_set_api
   :project: nrfxlib
   :members:

Socket polling API
******************

Necessary data types and defines to poll for
events on one or more sockets using nrf_poll().

.. doxygengroup:: nrf_socket_api_poll
   :project: nrfxlib
   :members:

OS specific definitions
***********************

.. doxygengroup:: nrf_modem_os
   :project: nrfxlib
   :members:
