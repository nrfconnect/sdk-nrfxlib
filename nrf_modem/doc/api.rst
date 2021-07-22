.. _nrf_modem_api:

API documentation
#################

.. contents::
   :local:
   :depth: 2

.. note::

   Starting from Modem library v1.2.0, the GNSS socket is deprecated and replaced with the `GNSS API <https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrfxlib/nrf_modem/doc/api.html#gnss-api>`_.

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

Socket option levels
====================

.. doxygengroup:: nrf_socket_options_levels
   :project: nrfxlib
   :members:

Generic socket options
======================

.. doxygengroup:: nrf_socket_options_sockets
   :project: nrfxlib
   :members:

Socket send and recv flags
==========================

.. doxygengroup:: nrf_socket_send_recv_flags
   :project: nrfxlib
   :members:

fcntl parameters
================

.. doxygengroup:: nrf_fcnt_commands
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

DTLS handshake timeout values
=============================

.. doxygengroup:: nrf_socket_tls_dtls_handshake_timeouts
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

DFU errors
==========

.. doxygengroup:: nrf_dfu_errors
   :project: nrfxlib
   :members:

PDN socket
**********

.. doxygengroup:: nrf_socket_pdn
   :project: nrfxlib
   :members:

Socket address resolution API
*****************************

.. doxygengroup:: nrf_socket_address_resolution
   :project: nrfxlib
   :members:

File descriptor sets API
************************

File descriptor sets are used as input to the nrf_select() function for doing I/O
multiplexing. The maximum number of descriptors contained in a set is defined by
NRF_FD_SETSIZE.

.. note::
   :c:func:`nrf_select` is currently not supported.

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

.. _nrf_modem_at_api:

AT API
******

.. doxygengroup:: nrf_modem_at
   :project: nrfxlib
   :members:

.. _nrf_modem_full_dfu_api:

Full DFU API
************

.. doxygengroup:: nrf_modem_full_dfu
   :project: nrfxlib
   :members:

.. _nrf_modem_delta_dfu_api:

Delta DFU API
*************

.. doxygengroup:: nrf_modem_delta_dfu
   :project: nrfxlib
   :members:

GNSS API
********

.. doxygengroup:: nrf_modem_gnss
   :project: nrfxlib
   :members:

Event types
===========

.. doxygengroup:: nrf_modem_gnss_event_type
   :project: nrfxlib
   :members:

Data types
==========

.. doxygengroup:: nrf_modem_gnss_data_type
   :project: nrfxlib
   :members:

PVT notification flags bitmask values
=====================================

.. doxygengroup:: nrf_modem_gnss_pvt_flag_bitmask
   :project: nrfxlib
   :members:

Satellite flags bitmask values
==============================

.. doxygengroup:: nrf_modem_gnss_sv_flag_bitmask
   :project: nrfxlib
   :members:

A-GPS data request bitmask values
=================================

.. doxygengroup:: nrf_modem_gnss_agps_data_bitmask
   :project: nrfxlib
   :members:

A-GPS data types
================

.. doxygengroup:: nrf_modem_gnss_agps_data_type
   :project: nrfxlib
   :members:

Delete bitmask values
=====================

.. doxygengroup:: nrf_modem_gnss_delete_bitmask
   :project: nrfxlib
   :members:

GNSS system bitmask values
==========================

.. doxygengroup:: nrf_modem_gnss_system_bitmask
   :project: nrfxlib
   :members:

NMEA string bitmask values
==========================

.. doxygengroup:: nrf_modem_gnss_nmea_string_bitmask
   :project: nrfxlib
   :members:

Power save modes
================

.. doxygengroup:: nrf_modem_gnss_power_save_modes
   :project: nrfxlib
   :members:

Use case bitmask values
=======================

.. doxygengroup:: nrf_modem_gnss_use_case_bitmask
   :project: nrfxlib
   :members:

Sleep timing sources
====================

.. doxygengroup:: nrf_modem_gnss_timing_source
   :project: nrfxlib
   :members:

QZSS NMEA modes
===============

.. doxygengroup:: nrf_modem_gnss_qzss_nmea_mode
   :project: nrfxlib
   :members:

Dynamics modes
==============

.. doxygengroup:: nrf_modem_gnss_dynamics_mode
   :project: nrfxlib
   :members:

OS specific definitions
***********************

.. doxygengroup:: nrf_modem_os
   :project: nrfxlib
   :members:
