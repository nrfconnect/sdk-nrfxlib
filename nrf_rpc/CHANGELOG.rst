.. _nrf_rpc_changelog:

Changelog
#########

.. contents::
   :local:
   :depth: 2

All the notable changes to this project are documented on this page.

nRF Connect SDK v3.1.99
***********************

Added
=====

* Added the possibility to measure command execution time.
  You can enable this feature with the :kconfig:option:`CONFIG_NRF_RPC_COMMAND_TIME_MEASURE` Kconfig option.
  You must also implement the :c:func:`nrf_rpc_os_timestamp_get_now` function, which returns the current timestamp expressed in milliseconds.

nRF Connect SDK v3.1.0
**********************

Added
=====

* Added a timeout feature for awaiting command responses.
  This change required making the following changes to the nRF RPC OS abstraction layer:

  * Introduced the :c:struct:`nrf_rpc_os_mutex` structure to protect the nRF RPC thread-local context.
  * Updated the :c:func:`nrf_rpc_os_msg_get` function to unlock the thread-local context mutex and to allow returning no data from the function if a timeout occurs.

nRF Connect SDK v2.8.0
**********************

Changes
=======

* Enabled zcbor's :c:member:`stop_on_error` flag before decoding the CBOR payload of an nRF RPC packet.
  When this flag is set, zcbor stops decoding subsequent data items in the case of decoding failure unless the error is explicitly cleared with the :c:func:`zcbor_pop_error` function.

nRF Connect SDK v2.5.0
**********************

Bug fixes
=========

* Fixed an issue where the :c:func:`nrf_rpc_cbor_cmd_rsp` function decodes only the first response element.

Changes
=======

* Enabled the nRF RPC library to be initialized more than once.

nRF Connect SDK v2.2.0
**********************

All the notable changes added to the |NCS| v2.2.0 release are documented in this section.

Added
=====

* Added the :kconfig:option:`CONFIG_NRF_RPC_ZCBOR_BACKUPS` Kconfig option for setting up the number of zcbor backups.

Changes
=======

* Enabled independent initialization of transport in each group.
  This makes RPC available for the groups that are ready, even though initialization of others may have failed.

nRF Connect SDK v2.1.0
**********************

All the notable changes added to the |NCS| v2.1.0 release are documented in this section.

Added
=====

* Added support for multi-instance transport.
  Each group can use a different transport instance, or groups can share a single transport instance.
* Added support for the nRF RPC protocol versioning.

Changes
=======

* Updated documentation.

nRF Connect SDK v2.0.0
**********************

All the notable changes added to the |NCS| v2.0.0 release are documented in this section.

Changes
=======

* Updated figures to follow style guidelines.
* Moved from TinyCBOR to zcbor.

nRF Connect SDK v1.9.0
**********************

No changes in this release.

nRF Connect SDK v1.8.0
**********************

All the notable changes added to the |NCS| v1.8.0 release are documented in this section.

Changes
=======

* Removed Latin abbreviations from the documentation.

nRF Connect SDK v1.7.0
**********************

All the notable changes added to the |NCS| v1.7.0 release are documented in this section.

Changes
=======

* Fixed formatting of Kconfig options.

nRF Connect SDK v1.6.0
**********************

All the notable changes added to the |NCS| v1.6.0 release are documented in this section.

Changes
=======

* Renamed :file:`nrf_ernno.h` to :file:`nrf_rpc_ernno.h`.

nRF Connect SDK v1.5.0
**********************

All the notable changes added to the |NCS| v1.5.0 release are documented in this section.

Changes
=======

* Removed "BSD" from LicenseRef text.
* Cleaned up the documentation.

nRF Connect SDK v1.4.0
**********************

All the notable changes added to the |NCS| v1.4.0 release are documented in this section.

Added
=====

Initial release.

* Added Remote Procedure Calls for nRF SoCs.
