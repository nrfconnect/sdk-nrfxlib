.. _scan_features:

sCAN features
##############

.. contents::
   :local:
   :depth: 2

This page provides an overview of the functionalities and configurations available for the Soft Peripheral sCAN for the current state of development.
See the following sections for more details.

.. _scan_features_modes_of_operation:

Modes of operation
******************

sCAN supports the following modes of operation:

* NORMAL - RX uses ID filters, TX will be attempted until success
* LOOPBACK - API test mode moves TX data to RX data automatically
* LISTENONLY - Do not send dominant bits
* ONESHOT - TX attempted only once

.. _semmc_features_mailboxes_and_filters:

Mailboxes and filters
*********************

sCAN uses a mailbox system, each with an associated data frame and filter. Mailboxes will lock themselves after a filter match to prevent loosing frames.
sCAN API provides convenience functions for unlocking a mailbox after the frame has been processed by the user application.

The filtering system is based on an ID/mask pair, the bit width of the filter can be configured to be standard (11bit) or extended (29bit).
sCAN API provides functions to set, enable and disable a filter for a given mailbox.

.. note::
   Refer to the :file:`nrf_scan.h` header for more details.
