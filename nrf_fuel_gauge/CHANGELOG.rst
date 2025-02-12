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

* Updated to library version 1.0.0.
* Simplified the :c:func:`nrf_fuel_gauge_process` function, as vbus state information is now gathered through the :c:func:`nrf_fuel_gauge_ext_state_update` function.
* Simplified the :c:func:`nrf_fuel_gauge_ttf_get` function, as charger state information is now gathered through the :c:func:`nrf_fuel_gauge_ext_state_update` function.
* Simplified the use of the :c:func:`nrf_fuel_gauge_param_adjust` function by allowing parameters to be set to "nan" to indicate that they should not be adjusted.

Added
=====

* Support for primary cell (non-recheargable) batteries.
* Battery models for Alkaline AA, Alkaline AAA, Alkaline coin cell LR44, and Lithium-manganese dioxide coin cell CR2032 batteries.
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
