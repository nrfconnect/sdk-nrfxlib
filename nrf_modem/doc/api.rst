.. _nrf_modem_api:

API documentation
#################

.. contents::
   :local:
   :depth: 2

Library Management
******************

.. doxygengroup:: nrf_modem

nrf_modem_init() return values for modem firmware updates
=========================================================

.. doxygengroup:: nrf_modem_dfu

.. _nrf_modem_trace_api:

Modem traces
============

.. doxygengroup:: nrf_modem_trace

.. _nrf_modem_fault_api_reasons:

Modem fault reasons
===================

.. doxygengroup:: nrf_modem_fault_handling

Limits of the Modem library
***************************

.. doxygengroup:: nrf_modem_limits

Socket parameter enumerators
****************************

Socket families
===============

.. doxygengroup:: nrf_socket_families

Socket types
============

.. doxygengroup:: nrf_socket_types

Socket protocols
================

.. doxygengroup:: nrf_socket_protocols

Socket option levels
====================

.. doxygengroup:: nrf_socket_options_levels

Generic socket options
======================

.. doxygengroup:: nrf_socket_options_sockets

Socket send and recv flags
==========================

.. doxygengroup:: nrf_socket_send_recv_flags

fcntl parameters
================

.. doxygengroup:: nrf_fcnt_commands

.. doxygengroup:: nrf_fcnt_flags

Socket API
**********

.. doxygengroup:: nrf_socket_api

TLS socket
**********

.. doxygengroup:: nrf_socket_tls

DTLS handshake timeout values
=============================

.. doxygengroup:: nrf_socket_so_sec_handshake_timeouts

TLS/DTLS peer verification options
==================================

.. doxygengroup:: nrf_socket_sec_peer_verify_options

TLS/DTLS session cache options
==============================

.. doxygengroup:: nrf_socket_session_cache_options

TLS/DTLS socket connection roles
================================

.. doxygengroup:: nrf_socket_sec_roles

.. _nrf_supported_tls_cipher_suites:

Supported cipher suites
=======================

.. doxygengroup:: nrf_socket_tls_cipher_suites

DTLS connection ID settings
===========================

.. doxygengroup:: nrf_so_sec_dtls_cid_settings

DTLS connection ID statuses
===========================

.. doxygengroup:: nrf_so_sec_dtls_cid_statuses

TLS/DTLS handshake statuses
===========================

.. doxygengroup:: nrf_so_sec_handshake_statuses


Socket address resolution API
*****************************

.. doxygengroup:: nrf_socket_address_resolution

Socket polling API
******************

Necessary data types and defines to poll for
events on one or more sockets using :c:func:`nrf_poll`.

.. doxygengroup:: nrf_socket_api_poll

.. _nrf_modem_at_api:

AT API
******

.. doxygengroup:: nrf_modem_at

.. _nrf_modem_bootloader_api:

Bootloader API
**************

.. doxygengroup:: nrf_modem_bootloader

.. _nrf_modem_delta_dfu_api:

Delta DFU API
*************

.. doxygengroup:: nrf_modem_delta_dfu

.. doxygengroup:: nrf_modem_delta_dfu_errors

.. _nrf_modem_dect_phy_api:

DECT NR+ PHY API
****************

.. doxygengroup:: nrf_modem_dect_phy

.. _nrf_modem_gnss_api:

GNSS API
********

.. doxygengroup:: nrf_modem_gnss

Event types
===========

.. doxygengroup:: nrf_modem_gnss_event_type

Data types
==========

.. doxygengroup:: nrf_modem_gnss_data_type

PVT notification flags bitmask values
=====================================

.. doxygengroup:: nrf_modem_gnss_pvt_flag_bitmask

Satellite flags bitmask values
==============================

.. doxygengroup:: nrf_modem_gnss_sv_flag_bitmask

A-GNSS data request bitmask values
==================================

.. doxygengroup:: nrf_modem_gnss_agnss_data_bitmask

.. _agnss_data_type_enum_api:

A-GNSS data type enumerator
===========================

.. doxygengroup:: nrf_modem_gnss_agnss_data_type

Delete bitmask values
=====================

.. doxygengroup:: nrf_modem_gnss_delete_bitmask

GNSS signal bitmask values
==========================

.. doxygengroup:: nrf_modem_gnss_signal_bitmask

NMEA string bitmask values
==========================

.. doxygengroup:: nrf_modem_gnss_nmea_string_bitmask

Power save modes
================

.. doxygengroup:: nrf_modem_gnss_power_save_modes

Use case bitmask values
=======================

.. doxygengroup:: nrf_modem_gnss_use_case_bitmask

Sleep timing sources
====================

.. doxygengroup:: nrf_modem_gnss_timing_source

QZSS NMEA modes
===============

.. doxygengroup:: nrf_modem_gnss_qzss_nmea_mode

Dynamics modes
==============

.. doxygengroup:: nrf_modem_gnss_dynamics_mode

.. _nrf_modem_softsim_api:

SoftSIM API
***********

.. doxygengroup:: nrf_modem_softsim

OS specific definitions
***********************

.. doxygengroup:: nrf_modem_os

OS RPC transport definitions for the nRF9230 SoC
************************************************

.. doxygengroup:: nrf_modem_os_rpc
