.. _scan_features:

sCAN features
##############

.. contents::
   :local:
   :depth: 2

This page provides an overview of the features and configuration options currently supported by the sCAN soft peripheral.
See the following sections for more details.

.. _scan_features_modes_of_operation:

Modes of operation
******************

sCAN supports the following modes of operation:

* ``NORMAL`` - Receives frames using ID filters and attempts transmission until it succeeds.
* ``LOOPBACK`` - Routes transmitted data to received data in software for API testing.
* ``LISTENONLY`` - Does not transmit dominant bits.
* ``ONESHOT`` - Attempts transmission only once.

.. _scan_features_mailboxes_and_filters:

Mailboxes and filters
*********************

sCAN uses a mailbox system in which each mailbox has an associated data frame and filter.
After a filter match, the mailbox locks to avoid overwriting an unread frame.
The sCAN API provides convenience functions for unlocking a mailbox after the application processes the frame.

The filtering system uses an ID and mask pair.
You can configure the filter width as standard (11-bit) or extended (29-bit).
The sCAN API provides functions for setting, enabling, and disabling the filter for a given mailbox.

For API details, see the :file:`nrf_scan.h` header file.
