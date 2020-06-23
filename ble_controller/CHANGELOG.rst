.. _ble_controller_changelog:

Changelog
#########

All notable changes to this project are documented in this file.

Master branch
*************

Added
=====

* Added support for setting the event length of a connection. See :cpp:func:`hci_vs_cmd_event_length_set()`.
* Added Set Controller to Host Flow Control command.
* Added Host Buffer Size command.
* Added Host Number of Complete Packets command.

Changes
=======

* When LLPM mode is enabled, the connection event length is now no longer implicitly set to 1 ms.

nRF Connect SDK v1.3.0
**********************

Added
=====

* Added Read Authenticated Payload Timeout command.
* Added Write Authenticated Payload Timeout command.
* Added Set Event Mask command.
* Added Set Event Mask Page 2 command.
* Added :cpp:func:`ble_controller_support_le_2m_phy()` which makes LE 2M PHY support configurable.
* Added :cpp:func:`ble_controller_support_le_coded_phy()` which makes LE Coded PHY support configurable.
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

* Added :cpp:func:`ble_controller_support_dle()` which makes LE Data Length Extension support configurable.
* Added preliminary support for the S140 variant with the nRF5340 device.
  The Bluetooth LE Controller for nRF5340 supports the same feature set as its nRF52 Series counterpart.
  The following library has been added:

  * :file:`ble_controller/lib/cortex-m33+nodsp/soft-float/libble_controller_s140.a`

Changes
=======

* :cpp:func:`mpsl_init()` is no longer called by :cpp:func:`ble_controller_init()`.
  Application is therefore responsible for calling :cpp:func:`mpsl_init()`, and it must be done before :cpp:func:`ble_controller_init()` is called.
* Clock configuration parameters are now contained in :c:type:`mpsl_clock_lfclk_cfg_t` instead of :c:type:`nrf_lf_clock_cfg_t`, and must be provided to :cpp:func:`mpsl_init()` instead of :cpp:func:`ble_controller_init()`.
* Clock accuracy must now be specified in parts per million (ppm) instead of the previous enum value.
* The IRQ line to pend for low priority signal processing must be provided to :cpp:func:`mpsl_init()` instead of :cpp:func:`ble_controller_init()`.
* The application must call :cpp:func:`mpsl_low_priority_process()` instead of :cpp:func:`ble_controller_low_prio_tasks_process()` to process low priority signals.
* :cpp:func:`mpsl_uninit()` is no longer called by :cpp:func:`ble_controller_disable()`.
  Application must therefore call :cpp:func:`mpsl_uninit()` after :cpp:func:`ble_controller_disable()` to uninitialize MPSL.
* Interrupt handler APIs for the following peripherals are moved to MPSL: RADIO, RTC0, TIMER0, and POWER_CLOCK.
* High frequency clock API (``ble_controller_hf_clock_...``) is removed.
  Use corresponding API in MPSL instead.
* Temperature API (:cpp:func:`ble_controller_temp_get()`) is removed.
  Use corresponding API in MPSL instead.
* Timeslot API is removed.
  Use corresponding API in MPSL instead.
* Version numbers have been removed from the libraries.

Bugfixes
========

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

Bugfixes
========

* Fixed an issue where :cpp:func:`hci_data_get()` could return "No data available" when there was data available.
  This issue would only occur when connected to multiple devices at the same time.

nRF Bluetooth LE Controller 0.3.0-2.prealpha
********************************************

Bugfixes
========

* Fixed an issue where an assert occured when the host issued LE Write Suggested Default Data Length.

nRF Bluetooth LE Controller 0.3.0-1.prealpha
********************************************

Added
=====

* Increased the number of supported SoC and library combinations.
* Added API for estimating the dynamic memory usage returned by :cpp:func:`ble_controller_cfg_set()`.
* Added a new header :file:`ble_controller_hci_vs.h` that exposes definitions of
  Vendor Specific HCI commands and events.
* Added support for connection intervals less than the standard minimum of 7.5 ms.
  Note that this a proprietary feature that is not BLE compliant.
  This proprietary feature is named 'Low Latency Packet Mode (LLPM)'.
* Added support for enabling or disabling connection event length extension.
  When disabled, the maximum connection event length is set by ``ble_controller_cfg_event_length_t::event_length_us``.
  When enabled, the maximum connection event length is determined by the connection interval.
* Added support for generating QoS Connection event reports.
  When enabled, one report is generated with every connection event.
  The report contains information that can be used to change the BLE channel map.

Bugfixes
========

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

* Fixed a bug causing an assert in ``ble_controller_soc.c:29``.

nRF Bluetooth LE Controller 0.2.0-2.prealpha
********************************************

Minor documentation fixes.

Changes
=======

* Added names to nested structs and unions in :file:`timeslot.h` as a workaround for a Sphinx documentation build issue.
* Fixed internal links to functions and files.

nRF Bluetooth LE Controller 0.2.0-1.prealpha
********************************************

Updated Bluetooth LE Controller with bugfixes and updated APIs.

Added
=====

* Added API for fetching build revision information.
* Added :cpp:func:`ble_controller_rand_vector_get_blocking()` as a blocking call to get a vector of random bytes.
* Added API to get Bluetooth LE Controller build revision: :cpp:func:`ble_controller_build_revision_get()`.
* Added separate :cpp:func:`ble_controller_init()` API.

Bugfixes
========

* Fixed an issue in HCI control flow that severely limited Bluetooth LE throughput.

Changes
=======
* Moved ``fault_handler`` and ``p_clk_cfg`` from :cpp:func:`ble_controller_enable()` to :cpp:func:`ble_controller_init()`.
* Changed :cpp:func:`ble_controller_process_SWI5_IRQ()` to be IRQ independent.
  The generic :cpp:func:`ble_controller_low_prio_tasks_process()` is used instead and SWI5 is no longer reserved.
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
