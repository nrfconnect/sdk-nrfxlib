.. _ble_controller_changelog:

Changelog
#########

All notable changes to this project are documented in this file.

ble_controller 0.2.0-1.prealpha
*******************************

Updated BLE controller with bugfixes and updated APIs.

Added
=====

* Added API for fetching build revision information
* Added ``ble_controller_rand_vector_get_blocking()``
  as a blocking call to get a vector of random bytes.
* Added API to get BLE controller build revision: ``ble_controller_build_revision_get()``
* Added separate ``ble_controller_init()`` API.

Bugfixes
========

* Fixed issue in HCI control flow issue that severely limited BLE throughput.

Changes
=======
* Moved ``fault_handler`` and ``p_clk_cfg`` from ``ble_controller_enable()`` to
  ``ble_controller_init()``.
* Changed ``ble_controller_process_SWI5_IRQ()`` to be IRQ independent. The
  generic ``ble_controller_low_prio_tasks_process()`` is used instead and SWI5
  is no longer reserved.
* Aligned naming for ble_controller configuration names.
* Made minor changes to existing API
* Improved API documentation

ble_controller 0.1.0-2.prealpha
*******************************

No change to library files.

Added
=====

* Headers necessary to utilize the timeslot API.


ble_controller 0.1.0-1.prealpha
*******************************

Initial release.

Added
=====

* Added the following ble_controller_nrf52_0.1.0-1.prealpha library variants,
  each in soft-float, softfp-float, and hard-float builds:

  * ``libble_controller_s112_nrf52_0.1.0-1.prealpha.a``
  * ``libble_controller_s132_nrf52_0.1.0-1.prealpha.a``
  * ``libble_controller_s140_nrf52_0.1.0-1.prealpha.a``
