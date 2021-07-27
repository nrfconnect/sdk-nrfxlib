.. _nrf_802154_changelog:

Changelog
#########

.. contents::
   :local:
   :depth: 2

All notable changes to this project are documented in this file.

nRF Connect SDK v1.6.1
**********************

No changes in this release.

nRF Connect SDK 1.6.0 - nRF 802.15.4 Radio Driver
*************************************************

Initial common release.

Added
=====

* Added the source code of the 802.15.4 Radio Driver.
* Added the 802.15.4 Service Layer library.
* Added the source code of the 802.15.4 Radio Driver API serialization library.
* Added the possibility to schedule two delayed reception windows.
* Added CSL phase injection.
* Added outgoing frame encryption and frame counter injection.
* Added Thread Link Metrics IEs injection.

Notable Changes
===============

* The release notes of the legacy versions of the Radio Driver are available in the `Radio Driver section`_ of the Infocenter.
* The changelog of the previous versions of the 802.15.4 SL library is now located at the bottom of this page.
* The Radio Driver documentation will now also include the Service Layer documentation.
* Future versions of the Radio Driver and the Service Layer will follow NCS version tags.
* The 802.15.4 Radio Driver API has been modified to support more than a single delayed reception window simultaneously.
  The :c:func:`nrf_802154_receive_at`, :c:func:`nrf_802154_receive_at_cancel`, and :c:func:`nrf_802154_receive_failed` functions take an additional parameter that identifies a given reception window unambiguously.

.. _Radio Driver section: https://infocenter.nordicsemi.com/topic/15.4_radio_driver_v1.10.0/rd_release_notes.html

nRF 802.15.4 Service Layer 0.5.0
********************************

* Added the possibility to check the 802.15.4 capabilities.

Added
=====

* Added the possibility to check the 802.15.4 capabilities.
  Built from commit *2966ae8b4b3fcf2b64d8b987703cbf4ecc0dd60b*.

nRF 802.15.4 Service Layer 0.4.0
********************************

* Added multiprotocol support for the nRF53 family.

Added
=====

* Added multiprotocol support for the nRF53 family.
  Built from commit *5d2497b78683687bdd57fcd6854b1bc3c26871be*.

nRF 802.15.4 Service Layer 0.3.0
********************************

* PA/LNA implementation has been moved to MPSL.
  Obsolete implementation and API have been removed.

Removed
=======

* Removed PA/LNA implementation and API.
  Built from commit *e268db75108016ee02965556aa52cf8437f5e071*.

nRF 802.15.4 Service Layer 0.2.0
********************************

Initial release.

Added
=====

* Added the :file:`libnrf_802154_sl.a` library.
  Built from commit *4c5ff68c4eb4ba817774bbd6c711a67dfde7d905*.
