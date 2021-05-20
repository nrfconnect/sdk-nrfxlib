.. _softdevice_controller_changelog:

Changelog
#########

.. contents::
   :local:
   :depth: 2

All notable changes to this project are documented in this file.

Master branch
*************

Added
=====

* Added radio front-end module (FEM) support, based on the :ref:`mpsl_fem` (DRGN-11059).
* Added support for the vendor specific HCI command: Read Supported Vendor Specific Commands (DRGN-13763).
* Added support for the vendor specific HCI command: Zephyr Read Key Hierarchy Roots (DRGN-13237).
* On nRF53 Series devices, the maximum supported radio output power is increased from 0 dBm to 3 dBm.
  In this case, NRF_VREQCTRL->VREGRADIO.VREQH will be set (DRGN-15476).

Changes
=======

* Moved permanent limitations from the :ref:`nrf:known_issues` page to :ref:`softdevice_controller_limitations`.

Bug fixes
=========

* Fixed an issue where the channel map provided by the LE Host Set Channel Classification HCI command was not applied on the secondary advertising channels (DRGN-13594).
* The SoftDevice Controller can now be qualified on nRF52832 (DRGN-15382).
* Fixed an issue where setting a legacy advertiser's scan response data using extended advertising HCI commands corrupted the advertising data (DRGN-15465).
* Fixed an issue where, in rare cases, an assert could occur when receiving a packet as a slave.
  This could only occur after performing a data length procedure on Coded PHY (DRGN-15251).
* Fixed an issue where "HCI Read RSSI" would always return a Command Disallowed (0x0C) error code (DRGN-15310).
* Fixed an issue where setting radio output power using the Vendor specific HCI command: Zephyr Write TX Power Level returned "Unsupported Feature or Parameter value (0x11).
  Now the controller will select an output power level that is lower or equal to the one requested. It will return success and the selected power level (DRGN-15369).

nRF Connect SDK v1.5.0
**********************

Added
=====

* Added :c:func:`sdc_support_ext_scan` which makes support for extended scanning configurable (DRGN-14902).
* Added :c:func:`sdc_support_ext_adv` which makes support for extended advertising configurable (DRGN-14914).
* Added support for the vendor specific HCI command: Zephyr Read Chip Temperature (DRGN-13769).
* Added support for the vendor specific HCI command: Zephyr Read Tx Power (DRGN-15250).

Changes
=======

* Decoupled the controller from the random number generator (DRGN-12507).
  This functionality must now be provided by the user.
  The user must provide ``rand_prio_low_get()``, ``rand_prio_high_get()`` and ``rand_poll()`` through the newly introduced :c:func:`sdc_rand_source_register()`.
  These function can be mapped to the Zephyr Entropy API:

    * ``rand_prio_low_get()`` <-> ``entropy_get_entropy_isr()`` (``*_isr()`` for nonblocking behavior)
    * ``rand_prio_high_get()`` <-> ``entropy_get_entropy_isr()``
    * ``rand_poll()`` <-> ``entropy_get_entropy()``

* Renamed and reconfigured the libraries (DRGN-15118).
  Refer to the README for their corresponding supported feature sets.
  The new names are now:

    * ``libsoftdevice_controller_peripheral.a``
    * ``libsoftdevice_controller_central.a``
    * ``libsoftdevice_controller_multirole.a``

* All libraries are now compatible with all platforms within a given family (DRGN-15118).

Bug fixes
=========

* Fixed an issue where the application could not immediately restart a connectable advertiser after a high duty cycle advertiser timed out (DRGN-13029).
* Fixed an issue where a directed advertiser used a resolvable address as the TargetA when the local device address was set to public or random device address (DRGN-13921).
* Fixed an issue where "HCI LE Set Extended Advertising Parameters" should have returned "Packet Too Long (0x45)" when the advertising set was already configured with data which was longer than it could fit within the advertising interval.
  Previously, the advertising data was cleared every time the advertising set was configured (DRGN-14008).
* Fixed an issue where the link would disconnect with reason "LMP Response Timeout (0x22)".
  This would occur if "HCI LE Long Term Key Request event" was disabled and the slave received an encryption request (DRGN-15226).
* Fixed an issue where the LL control procedures LE start encryption and LE connection parameter update could not be initiated at the same time (DRGN-11963).
* Fixed an issue where the generation of QoS Connection event was not disabled after an HCI reset (DRGN-15291).

Known issues
============

See the :ref:`nrf:known_issues` page in |NCS| for the list of known issues and limitations for this release.

nRF Connect SDK v1.4.0
**********************

Added
=====

* Added Read Transmit Power Level command (DRGN-12236).
* Added LE Read Transmit Power command (DRGN-12236).
* Added LE Read Advertising Physical Channel Tx Power command (DRGN-12238).
* Added support for setting the event length of a connection. See :c:func:`hci_vs_cmd_event_length_set` (DRGN-12696).
* Added Set Controller to Host Flow Control command (DRGN-13331).
* Added Host Buffer Size command (DRGN-13331).
* Added Host Number of Complete Packets command (DRGN-13331).
* Added support for the Vendor specific HCI command: Zephyr Write BD Addr (DRGN-14511).
* Added LE Read PHY command (DRGN-14664).
* Added APIs for every supported HCI command (DRGN-13723).
* Added :c:func:`sdc_support_adv` which makes the advertising state configurable (DRGN-14759).
* Added :c:func:`sdc_support_slave` which makes the slave role configurable (DRGN-14759).
* Added :c:func:`sdc_support_scan` which makes the scanning state configurable (DRGN-14759).
* Added :c:func:`sdc_support_master` which makes the master role configurable (DRGN-14759).

Changes
=======

* When linking the final binary, the image size is reduced. Only the requested features are included. See :c:func:`sdc_support_adv` and similar APIs for more details.
* When LLPM mode is enabled, the connection event length is now no longer implicitly set to 1 ms (DRGN-12696).
* When the connection interval is an LLPM connection interval, that is, below 7.5 ms, link layer procedures with an instant will use an instant larger than 6 connection events (DRGN-14379).
* The nRF Bluetooth LE Controller was renamed to SoftDevice Controller (DRGN-14283).
  APIs are updated accordingly:

  * BLE_CONTROLLER -> SDC
  * ble_controller -> sdc
  * HCI APIs are now prefixed with sdc

* The name of the library file was changed to :file:`libsoftdevice_controller.a` (DRGN-14283).
* SoC APIs have been renamed (DRGN-14283):

  * ble_controller_flash_write -> sdc_soc_flash_write_async
  * ble_controller_flash_page_erase -> sdc_soc_flash_page_erase_async
  * ble_controller_rand_vector_get -> sdc_soc_rand_vector_poll
  * ble_controller_rand_vector_get_blocking -> sdc_soc_rand_vector_get
  * ble_controller_ecb_block_encrypt -> sdc_soc_ecb_block_encrypt

* Vendor specific HCI APIs have been renamed (DRGN-14701):

  * HCI_VS_OPCODE   -> HCI_OPCODE_VS
  * HCI_VS_SUBEVENT -> HCI_SUBEVENT_VS
  * hci_vs_cmd      -> hci_cmd_vs

Bug fixes
=========

* Fixed an issue in master role which could cause disconnects if there were scheduling conflicts while doing a control procedures with an instant (DRGN-11222).

Known issues and limitations
============================

See the :ref:`nrf:known_issues` page in |NCS| for the list of known issues and limitations for this release.

nRF Connect SDK v1.3.0
**********************

Added
=====

* Added Read Authenticated Payload Timeout command.
* Added Write Authenticated Payload Timeout command.
* Added Set Event Mask command.
* Added Set Event Mask Page 2 command.
* Added :c:func:`ble_controller_support_le_2m_phy` which makes LE 2M PHY support configurable.
* Added :c:func:`ble_controller_support_le_coded_phy` which makes LE Coded PHY support configurable.
* Added LE Read Supported States command.
* Added LE Set Advertising Set Random Address command.
* Added LE Remove Advertising Set command.
* Added LE Clear Advertising Sets command.
* Added support for the Vendor specific HCI command: Zephyr Read Version Information.
* Added support for the Vendor specific HCI command: Zephyr Read Supported Commands.
* Added support for the Vendor specific HCI command: Zephyr Read Static Addresses.
* Added support for the Vendor specific HCI command: Zephyr Write TX Power Level (per Role/Connection).

Changes
=======

* ``HCI_VS_SUBEVENT_CODE_QOS_CONN_EVENT_REPORT`` was renamed to ``HCI_VS_SUBEVENT_QOS_CONN_EVENT_REPORT``.
* ``hci_vs_evt_qos_conn_event_report_t`` was renamed to ``hci_vs_subevent_qos_conn_event_report_t``.
* ``hci_vs_cmd_zephyr_read_supported_commands_return_t`` was converted from a struct to a union.
  The content remains the same.
* The VersNr field in the LL_VERSION_IND packet now contains the value 0x0B to indicate Bluetooth Core Specification v5.2 compliance.
* The previously implemented Vendor Specific HCI command opcodes are now offset with 0x100.
* The previously implemented Vendor Specific HCI event codes are now offset with 0x80.
* When the controller receives an unknown command, it will raise "Command Status event" instead of "Command Complete event".
* When in slave latency, the controller now picks up data from the host for transmission earlier than it used to.
* In the LE Extended Advertising Report, the Direct Address Type values 0x02, 0x03, and 0xFE will only be used when the Scanning Filter Policy is equal to 0x02 or 0x03 and TargetA is a resolvable private address.
  If the address is resolved, then the Direct Address Type will contain the same value as the Own Address Type parameter of the command LE Set Extended Scan Parameters.
  This follows the Bluetooth Core Specification v5.2.
* On nRF53, the fix for Errata 16 is now applied.

nRF Connect SDK v1.2.0
**********************

Added
=====

* Added :c:func:`ble_controller_support_dle` which makes LE Data Length Extension support configurable.
* Added preliminary support for the S140 variant with the nRF5340 device.
  The Bluetooth LE Controller for nRF5340 supports the same feature set as its nRF52 Series counterpart.
  The following library has been added:

  * :file:`ble_controller/lib/cortex-m33+nodsp/soft-float/libble_controller_s140.a`

Changes
=======

* :c:func:`mpsl_init` is no longer called by :c:func:`ble_controller_init`.
  Application is therefore responsible for calling :c:func:`mpsl_init`, and it must be done before :c:func:`ble_controller_init` is called.
* Clock configuration parameters are now contained in :c:type:`mpsl_clock_lfclk_cfg_t` instead of :c:type:`nrf_lf_clock_cfg_t`, and must be provided to :c:func:`mpsl_init` instead of :c:func:`ble_controller_init`.
* Clock accuracy must now be specified in parts per million (ppm) instead of the previous enum value.
* The IRQ line to pend for low priority signal processing must be provided to :c:func:`mpsl_init` instead of :c:func:`ble_controller_init`.
* The application must call :c:func:`mpsl_low_priority_process` instead of :c:func:`ble_controller_low_prio_tasks_process` to process low priority signals.
* :c:func:`mpsl_uninit` is no longer called by :c:func:`ble_controller_disable`.
  Application must therefore call :c:func:`mpsl_uninit` after :c:func:`ble_controller_disable` to uninitialize MPSL.
* Interrupt handler APIs for the following peripherals are moved to MPSL: RADIO, RTC0, TIMER0, and POWER_CLOCK.
* High frequency clock API (``ble_controller_hf_clock_...``) is removed.
  Use corresponding API in MPSL instead.
* Temperature API (:c:func:`ble_controller_temp_get`) is removed.
  Use corresponding API in MPSL instead.
* Timeslot API is removed.
  Use corresponding API in MPSL instead.
* Version numbers have been removed from the libraries.

Bug fixes
=========

* Fixed an issue where the application could not immediately restart a connectable advertiser after a high duty cycle advertiser timed out.
* Fixed an issue where a control packet could be sent twice even after the packet was ACKed.
  This would only occur if the radio was forced off due to an unforeseen condition.
* Fixed an issue in HCI LE Set Extended Scan Enable where ``UNSUPPORTED_FEATURE`` was returned when duplicate filtering was enabled.
* Fixed an issue in HCI LE Set Advertising Parameters where ``UNSUPPORTED_FEATURE`` was returned when ``secondary_max_skip`` was set to a non-zero value.
  This issue occured when sending a packet on either LE 1M or LE 2M PHY after receiving or transmitting a packet on
  LE Coded PHY.
  If this occured while performing a Link Layer Control Procedure, the controller could end up retransmitting
  an ACKed packet, resulting in a disconnect.
* Fixed an issue where an assert could occur when receiving a packet with a CRC error after performing a data length procedure on Coded PHY.
* Fixed an issue where an assert occurred when setting a secondary PHY to 0 when using HCI LE Set Extended Advertising Parameters.
  This issue occurred when the advertising type was set to legacy advertising.

nRF Bluetooth LE Controller 0.3.0-3.prealpha
********************************************

Added
=====

* Added support for nRF52833.

Bug fixes
=========

* Fixed an issue where :c:func:`hci_data_get` could return "No data available" when there was data available.
  This issue would only occur when connected to multiple devices at the same time.

nRF Bluetooth LE Controller 0.3.0-2.prealpha
********************************************

Bug fixes
=========

* Fixed an issue where an assert occured when the host issued LE Write Suggested Default Data Length.

nRF Bluetooth LE Controller 0.3.0-1.prealpha
********************************************

Added
=====

* Increased the number of supported SoC and library combinations.
* Added API for estimating the dynamic memory usage returned by :c:func:`ble_controller_cfg_set`.
* Added a new header :file:`ble_controller_hci_vs.h` that exposes definitions of
  Vendor Specific HCI commands and events.
* Added support for connection intervals less than the standard minimum of 7.5 ms.
  Note that this a proprietary feature that is not Bluetooth compliant.
  This proprietary feature is named 'Low Latency Packet Mode (LLPM)'.
* Added support for enabling or disabling connection event length extension.
  When disabled, the maximum connection event length is set by ``ble_controller_cfg_event_length_t::event_length_us``.
  When enabled, the maximum connection event length is determined by the connection interval.
* Added support for generating QoS Connection event reports.
  When enabled, one report is generated with every connection event.
  The report contains information that can be used to change the Bluetooth LE channel map.

Bug fixes
=========

* Fixed an issue where HCI Read Local Supported Commands command did not indicate support for HCI LE Set Privacy Mode command.
* Fixed an issue where an ASSERT occured when setting advertising data after HCI Reset without setting advertising parameters.
* Fixed an issue where an ASSERT occured when writing to flash.
* Fixed an issue where a directed advertiser could time out without sending a packet on air.

Changes
=======
* The VersNr field in the LL_VERSION_IND packet now contains the value 0x0A to indicate Bluetooth Core Specification v5.1 compatibility.
* Bluetooth Core Specification Erratum #10750 is incorporated.
  The LE Data Length Change event will now be raised when switching to and from Coded PHY.
  On-air behavior has not changed.
* Bluetooth Core Specification Erratum #10818 is incorporated. The controller now allows HCI ACL data packets with 0-length payload, but does not transmit anything until receiving the next non-zero continuation fragment.
* Cleaned up invalid doxygen comments.


nRF Bluetooth LE Controller 0.2.0-4.prealpha
********************************************

* Added a workaround to block the host from sending HCI commands when a shared command is in progress.


nRF Bluetooth LE Controller 0.2.0-3.prealpha
********************************************

Fixed a bug causing an assert in ``ble_controller_soc.c:29``.

nRF Bluetooth LE Controller 0.2.0-2.prealpha
********************************************

Minor documentation fixes.

Changes
=======

* Added names to nested structs and unions in :file:`timeslot.h` as a workaround for a Sphinx documentation build issue.
* Fixed internal links to functions and files.

nRF Bluetooth LE Controller 0.2.0-1.prealpha
********************************************

Updated Bluetooth LE Controller with bug fixes and updated APIs.

Added
=====

* Added API for fetching build revision information.
* Added :c:func:`ble_controller_rand_vector_get_blocking` as a blocking call to get a vector of random bytes.
* Added API to get Bluetooth LE Controller build revision: :c:func:`ble_controller_build_revision_get`.
* Added separate :c:func:`ble_controller_init` API.

Bug fixes
=========

Fixed an issue in HCI control flow that severely limited Bluetooth LE throughput.

Changes
=======
* Moved ``fault_handler`` and ``p_clk_cfg`` from :c:func:`ble_controller_enable` to :c:func:`ble_controller_init`.
* Changed :c:func:`ble_controller_process_SWI5_IRQ` to be IRQ independent.
  The generic :c:func:`ble_controller_low_prio_tasks_process` is used instead and SWI5 is no longer reserved.
* Aligned naming for Bluetooth LE Controller configuration names.
* Made minor changes to existing API.
* Improved API documentation.

nRF Bluetooth LE Controller 0.1.0-2.prealpha
********************************************

No change to library files.

Added
=====

* Headers necessary to utilize the timeslot API.


nRF Bluetooth LE Controller 0.1.0-1.prealpha
********************************************

Initial release.

Added
=====

* Added the following ble_controller_nrf52_0.1.0-1.prealpha library variants, each in soft-float, softfp-float, and hard-float builds:

  * ``libble_controller_s112_nrf52_0.1.0-1.prealpha.a``
  * ``libble_controller_s132_nrf52_0.1.0-1.prealpha.a``
  * ``libble_controller_s140_nrf52_0.1.0-1.prealpha.a``
