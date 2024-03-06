.. _nrf_fuel_gauge_changelog:

Changelog
#########

.. contents::
   :local:
   :depth: 2

All notable changes to this project are documented on this page.

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
