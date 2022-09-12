.. _nrf_802154_changelog:

Changelog
#########

.. contents::
   :local:
   :depth: 2

All notable changes to this project are documented in this file.
See also :ref:`nrf_802154_limitations` for permanent limitations.

nRF Connect SDK v2.1.0 - nRF 802.15.4 Radio Driver
**************************************************

Bug fixes
=========

* Fixed an issue where the channel for the delayed transmission on the nRF5340 SoC when passing NULL metadata would be set to 11.
  This was inconsistent with the behavior on nRF52 Series' SoCs and the channel now defaults to the value in the Personal Area Network Information Base (PIB). (KRKNWK-13539)
* Fixed an issue causing the calculated CSL phase to be too small. (KRKNWK-13782)
* Fixed an issue causing the nRF5340 SoC to prematurely run out of buffers for received frames on the application core. (KRKNWK-12493)
* Fixed an issue causing the nRF5340 SoC to transmit with minimum power when the requested transmit power was greater than 0 dBm. (KRKNWK-14487)

nRF Connect SDK v2.0.0 - nRF 802.15.4 Radio Driver
**************************************************

Notable changes
===============

* Reworked the implementation of the internal timer to support 64-bit timestamps. (KRKNWK-8612)
* The transmit power is now expressed as antenna output power, including any front-end module used.

Added
=====

* The transmit power can be set for each transmission request through the transmit metadata. (KRKNWK-13484)
* The use of runtime gain control of the front-end module is now provided by the MPSL library. (KRKNWK-13713)

Bug fixes
=========

* Fixed a stability issue where switching the GRANT line of the coexistence interface could cause a crash. (KRKNWK-11900)
* Fixed an issue where the setting ``NRF_802154_DELAYED_TRX_ENABLED=0`` would make the build fail.
* Fixed an issue where the CSMA-CA procedure was not aborted by pending operations with higher priority.
* Fixed an issue where a notification about an HFCLK change could be delayed by a high priority ISR and could cause a crash. (KRKNWK-11466)
* Fixed an issue where canceling a delayed time slot (for CSMA-CA, delayed transmission, and delayed reception operations) after the preconditions were requested could cause a crash. (KRKNWK-13175)
* Fixed an issue where a coexistence request would not be released at the end of the time slot while operating in multiprotocol mode.
* Fixed an issue where the reported ED values with temperature correction were imprecise. (KRKNWK-13599)
* Disabled the build of CSMA-CA when using the open-source service layer.

Other changes
=============

* Removed the files :file:`nrf_802154_ack_timeout.c` and :file:`nrf_802154_priority_drop_swi.c`.

nRF Connect SDK v1.9.0 - nRF 802.15.4 Radio Driver
**************************************************

Added
=====

* Delayed transmission and reception feature support for nRF5340. (KRKNWK-12074)
* Backforwarding of transmitted frames to support retransmissions through serialization for nRF5340. (KRKNWK-10114)
* Serialization of API required by Thread 1.2 (KRKNWK-12077) and other API for nRF5340.

Bug fixes
=========

* Fixed an issue where interleaving transmissions of encrypted and unencrypted frames could cause memory corruption. (KRKNWK-12261)
* Fixed an issue where interruption of a reception of encrypted frame could cause memory corruption. (KRKNWK-12622)
* Fixed an issue where transmission of an encrypted frame could transmit a frame filled partially with zeros instead of proper ciphertext. (KRKNWK-12770)
* Fixed stability issues related to CSMA-CA occurring with enabled experimental coexistence feature from :ref:`mpsl`. (KRKNWK-12701)

nRF Connect SDK v1.8.0 - nRF 802.15.4 Radio Driver
**************************************************

Notable changes
===============

* Incoming frames with Header IEs present but with no payload IEs and with no payload do not need IE Termination Header provided anymore. (KRKNWK-11875)

Bug fixes
=========

* Fixed an issue where the notification queue would be overflowed under stress. (KRKNWK-11606)
* Fixed an issue where ``nrf_802154_transmit_failed`` callout would not always correctly propagate the frame properties. (KRKNWK-11605)

nRF Connect SDK v1.7.0 - nRF 802.15.4 Radio Driver
**************************************************

Added
=====

* Adopted usage of the Zephyr temperature platform for the RSSI correction.
* Support for the coexistence feature from :ref:`mpsl`.
* Support for nRF21540 FEM GPIO interface on nRF53 Series.

Notable changes
===============

* Modified the 802.15.4 Radio Driver Transmit API.
  It now allows specifying whether to encrypt or inject dynamic data into the outgoing frame, or do both.
  The :c:type:`nrf_802154_transmitted_frame_props_t` type is used for this purpose.

Bug fixes
=========

* Fixed an issue where it would not be possible to transmit frames with invalid Auxiliary Security Header if :kconfig:option:`CONFIG_NRF_802154_ENCRYPTION` was set to ``n``. (KRKNWK-11218)
* Fix an issue with the IE Vendor OUI endianness. (KRKNWK-10633)
* Fixed various bugs in the MAC Encryption layer. (KRKNWK-10646)

nRF Connect SDK v1.6.0 - nRF 802.15.4 Radio Driver
**************************************************

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
