.. _nrf_modem_api:

API documentation
#################

.. contents::
   :local:
   :depth: 2

Library Management
******************

.. doxygengroup:: nrf_modem
   :project: nrfxlib

nrf_modem_init() return values for modem firmware updates
=========================================================

.. doxygengroup:: nrf_modem_dfu
   :project: nrfxlib

.. _nrf_modem_trace:

Modem traces
============

.. doxygengroup:: nrf_modem_trace
   :project: nrfxlib

.. _nrf_modem_fault:

Modem fault reasons
===================

.. doxygengroup:: nrf_modem_fault_handling
   :project: nrfxlib

Limits of the Modem library
***************************

.. doxygengroup:: nrf_modem_limits
   :project: nrfxlib

Socket parameter enumerators
****************************

Socket families
===============

.. doxygengroup:: nrf_socket_families
   :project: nrfxlib

Socket types
============

.. doxygengroup:: nrf_socket_types
   :project: nrfxlib

Socket protocols
================

.. doxygengroup:: nrf_socket_protocols
   :project: nrfxlib

Socket option levels
====================

.. doxygengroup:: nrf_socket_options_levels
   :project: nrfxlib

Generic socket options
======================

.. doxygengroup:: nrf_socket_options_sockets
   :project: nrfxlib

Socket send and recv flags
==========================

.. doxygengroup:: nrf_socket_send_recv_flags
   :project: nrfxlib

fcntl parameters
================

.. doxygengroup:: nrf_fcnt_commands
   :project: nrfxlib

.. doxygengroup:: nrf_fcnt_flags
   :project: nrfxlib

Socket API
**********

.. doxygengroup:: nrf_socket_api
   :project: nrfxlib

TLS socket
**********

.. doxygengroup:: nrf_socket_tls
   :project: nrfxlib

DTLS handshake timeout values
=============================

.. doxygengroup:: nrf_socket_so_sec_handshake_timeouts
   :project: nrfxlib

TLS/DTLS peer verification options
==================================

.. doxygengroup:: nrf_socket_sec_peer_verify_options
   :project: nrfxlib

TLS/DTLS session cache options
==============================

.. doxygengroup:: nrf_socket_session_cache_options
   :project: nrfxlib

TLS/DTLS socket connection roles
================================

.. doxygengroup:: nrf_socket_sec_roles
   :project: nrfxlib

.. _nrf_supported_tls_cipher_suites:

Supported cipher suites
=======================

.. doxygengroup:: nrf_socket_tls_cipher_suites
   :project: nrfxlib

DTLS connection ID settings
===========================

.. doxygengroup:: nrf_so_sec_dtls_cid_settings
   :project: nrfxlib

DTLS connection ID statuses
===========================

.. doxygengroup:: nrf_so_sec_dtls_cid_statuses
   :project: nrfxlib

TLS/DTLS handshake statuses
===========================

.. doxygengroup:: nrf_so_sec_handshake_statuses
   :project: nrfxlib


Socket address resolution API
*****************************

.. doxygengroup:: nrf_socket_address_resolution
   :project: nrfxlib

Socket polling API
******************

Necessary data types and defines to poll for
events on one or more sockets using nrf_poll().

.. doxygengroup:: nrf_socket_api_poll
   :project: nrfxlib

.. _nrf_modem_at_api:

AT API
******

.. doxygengroup:: nrf_modem_at
   :project: nrfxlib

.. _nrf_modem_bootloader_api:
.. _nrf_modem_full_dfu_api:

Bootloader API
**************

.. doxygengroup:: nrf_modem_bootloader
   :project: nrfxlib

.. _nrf_modem_delta_dfu_api:

Delta DFU API
*************

.. doxygengroup:: nrf_modem_delta_dfu
   :project: nrfxlib

.. doxygengroup:: nrf_modem_delta_dfu_errors
   :project: nrfxlib

.. _nrf_modem_gnss_api:

GNSS API
********

.. doxygengroup:: nrf_modem_gnss
   :project: nrfxlib

Event types
===========

.. doxygengroup:: nrf_modem_gnss_event_type
   :project: nrfxlib

Data types
==========

.. doxygengroup:: nrf_modem_gnss_data_type
   :project: nrfxlib

PVT notification flags bitmask values
=====================================

.. doxygengroup:: nrf_modem_gnss_pvt_flag_bitmask
   :project: nrfxlib

Satellite flags bitmask values
==============================

.. doxygengroup:: nrf_modem_gnss_sv_flag_bitmask
   :project: nrfxlib

A-GPS data request bitmask values
=================================

.. doxygengroup:: nrf_modem_gnss_agps_data_bitmask
   :project: nrfxlib

A-GPS data types
================

.. doxygengroup:: nrf_modem_gnss_agps_data_type
   :project: nrfxlib

Delete bitmask values
=====================

.. doxygengroup:: nrf_modem_gnss_delete_bitmask
   :project: nrfxlib

GNSS system bitmask values
==========================

.. doxygengroup:: nrf_modem_gnss_system_bitmask
   :project: nrfxlib

NMEA string bitmask values
==========================

.. doxygengroup:: nrf_modem_gnss_nmea_string_bitmask
   :project: nrfxlib

Power save modes
================

.. doxygengroup:: nrf_modem_gnss_power_save_modes
   :project: nrfxlib

Use case bitmask values
=======================

.. doxygengroup:: nrf_modem_gnss_use_case_bitmask
   :project: nrfxlib

Sleep timing sources
====================

.. doxygengroup:: nrf_modem_gnss_timing_source
   :project: nrfxlib

QZSS NMEA modes
===============

.. doxygengroup:: nrf_modem_gnss_qzss_nmea_mode
   :project: nrfxlib

Dynamics modes
==============

.. doxygengroup:: nrf_modem_gnss_dynamics_mode
   :project: nrfxlib

OS specific definitions
***********************

.. doxygengroup:: nrf_modem_os
   :project: nrfxlib
