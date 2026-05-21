.. _nrf_fuel_gauge_changelog:

Changelog
#########

.. contents::
   :local:
   :depth: 2

All notable changes to this project are documented on this page.

Main branch
***********

Changes
=======

* All functions now return an error code.
  Functions that previously returned a data value now return it through an output parameter instead.
* The ``nrf_fuel_gauge_runtime_parameters`` structure and the ``nrf_fuel_gauge_config_parameters`` parameters are now combined into a single structure, :c:struct:`nrf_fuel_gauge_config_parameters`.
  These are optional parameters that can be set at initialization or adjusted later using the :c:func:`nrf_fuel_gauge_config_params_adjust` function.
* The ``nrf_fuel_gauge_opt_params_default_get`` function is replaced with :c:func:`nrf_fuel_gauge_config_params_default_get` and :c:func:`nrf_fuel_gauge_config_params_current_get`.
* Renamed ``NRF_FUEL_GAUGE_CHARGE_STATE_CC_VBUS_LIMITED`` to ``NRF_FUEL_GAUGE_CHARGE_STATE_CC_LIMITED``.

Added
=====

* Experimental support for state-of-health estimation for secondary cell (rechargeable) batteries.
  This is a percentage value that indicates the current maximum capacity of the battery compared to its nominal capacity, estimated based on battery charge cycles.
* Optional lock function in the :c:struct:`nrf_fuel_gauge_init_parameters` structure to allow the library to be used in a multi-threaded environment.
* The following functions:

  * :c:func:`nrf_fuel_gauge_config_params_adjust` to adjust parameters at runtime.
  * :c:func:`nrf_fuel_gauge_config_params_default_get` to initialize the :c:struct:`nrf_fuel_gauge_config_parameters` structure with default values.
  * :c:func:`nrf_fuel_gauge_config_params_current_get` to retrieve the current configuration parameters.
  * :c:func:`nrf_fuel_gauge_soc_get` to retrieve the current state-of-charge value.
  * :c:func:`nrf_fuel_gauge_soh_get` to retrieve the current state-of-health value.
  * :c:func:`nrf_fuel_gauge_state_compatible_check` to check if a given state blob is compatible with the current library version.

* Convenience macros :c:macro:`NRF_FUEL_GAUGE_DEFAULT_INIT_PARAMETERS_PRIMARY` and :c:macro:`NRF_FUEL_GAUGE_DEFAULT_INIT_PARAMETERS_SECONDARY` used for simple initialization of the :c:struct:`nrf_fuel_gauge_init_parameters` structure.
* Library builds for Cortex-M0, Cortex-M0+, Cortex-M33 without DSP Extension, Cortex-M55, and Xtensa ESP32 architectures.

nRF Connect SDK v3.3.0
**********************

Changes
=======

* Updated to library version 1.1.1.
* When initializing the library from a stored state and a battery model pointer is provided, this pointer has precedence over the one stored in the state blob.

nRF Connect SDK v3.2.0
**********************

Changes
=======

* Updated to library version 1.1.0.
* Improved fuel gauge accuracy for small-capacity batteries using nPM1304.
* The ``discard_positive_deltaz`` member of the :c:struct:`nrf_fuel_gauge_runtime_parameters` structure now affects the secondary cell variant too.
  Setting it to true (default) will disallow state-of-charge to increase unless the battery is charging.

nRF Connect SDK v3.0.0
**********************

Changes
=======

* Updated to library version 1.0.1.
* Simplified the :c:func:`nrf_fuel_gauge_process` function, as vbus state information is now gathered through the :c:func:`nrf_fuel_gauge_ext_state_update` function.
* Simplified the :c:func:`nrf_fuel_gauge_ttf_get` function, as charger state information is now gathered through the :c:func:`nrf_fuel_gauge_ext_state_update` function.
* Simplified the use of the :c:func:`nrf_fuel_gauge_param_adjust` function by allowing parameters to be set to "nan" to indicate that they should not be adjusted.

Added
=====

* Support for primary cell (non-rechargeable) batteries.
* Battery models for Alkaline AA (1S and 2S configuration), Alkaline AAA (1S and 2S configuration), Alkaline coin cell LR44, and Lithium-manganese dioxide coin cell CR2032 batteries.
* A new :c:func:`nrf_fuel_gauge_ext_state_update` function to inform the library about external state changes.
  Such changes includes charger state, vbus connection status, and current measurement correction.
* A functionality to retrieve and restore the library state, using the :c:func:`nrf_fuel_gauge_state_get` function and state member of the :c:struct:`nrf_fuel_gauge_init_parameters` structure.
* A new member in the :c:struct:`nrf_fuel_gauge_state_info` structure to determine the current measurement value used after corrections.
* A new member in the :c:struct:`nrf_fuel_gauge_state_info` structure to determine the state-of-charge value before it is rounded off.
  The rounding off only applies to primary cell batteries.
* New undocumented members in the :c:struct:`nrf_fuel_gauge_config_parameters` structure, intended for future adjustability.

nRF Connect SDK v2.8.0
**********************

Changes
=======

* Updated to library version 0.11.1.
* The :c:func:`nrf_fuel_gauge_init` function no longer copies the battery model pointed to in the :c:struct:`nrf_fuel_gauge_init_parameters` structure.
  This means that you must ensure that the battery model is kept valid while using the library, either in ROM or RAM.
* The :c:func:`nrf_fuel_gauge_process` function has a new argument to specify when bus voltage is present.
  That is, when system is powered by vbus instead of or in addition to the battery.

Added
=====

* The :c:func:`nrf_fuel_gauge_opt_params_default_get` convenience function to initialize the :c:struct:`nrf_fuel_gauge_config_parameters` structure.
* The :c:func:`nrf_fuel_gauge_opt_params_adjust` function to adjust certain parameters without reinitializing the library.
* New configuration parameters in the :c:struct:`nrf_fuel_gauge_config_parameters` structure.
* New configuration parameter in the :c:struct:`nrf_fuel_gauge_runtime_parameters` structure.

Bug fixes
=========

* Fixed an issue where battery models with less than three temperature points could generate wrong state-of-charge after using the :c:func:`nrf_fuel_gauge_idle_set` function.

nRF Connect SDK v2.6.0
**********************

Changes
=======

* Update to library version 0.9.2.
* The :c:func:`nrf_fuel_gauge_ttf_get` function now uses charger state as argument instead of maximum charge current.
* The :c:func:`nrf_fuel_gauge_param_adjust` function now wraps the parameters in a struct.

Added
=====

* Optional parameters struct to the :c:struct:`nrf_fuel_gauge_init_parameters` structure.

nRF Connect SDK v2.4.0
**********************

Initial release.
