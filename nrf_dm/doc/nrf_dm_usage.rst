.. _nrf_dm_usage:

Usage
#####

.. contents::
   :local:
   :depth: 2

You can use the Distance Measurement library as follows:

1. Call the :c:func:`nrf_dm_init` function.
   The library expects as arguments the PPI channel numbers, the antenna configuration, and a timer instance.
   The  :c:macro:`NRF_DM_DEFAULT_SINGLE_ANTENNA_CONFIG` macro provides a default antenna configuration.

   .. note::
      Only a single antenna is currently supported.

#. Configure the execution of the measurements using the :c:func:`nrf_dm_configure` function.
   Use the structure in :c:struct:`nrf_dm_config_t` to configure the library.

#. To perform a measurement, call the :c:func:`nrf_dm_proc_execute` function passing the corresponding timeout as an argument.
   This call should be executed at the same time on both the reflector and the initiator, but the reflector must start first.

   You can meet this synchronization requirement by starting the reflector earlier and increasing the timeout value, at the cost of higher power consumption.

#. Call the :c:func:`nrf_dm_calc` function to perform the calculation.
   The calculation must be performed between the measurements to obtain a valid result.

#. Call the :c:func:`nrf_dm_report_get` function to get the results.
   It returns a pointer to the :c:struct:`nrf_dm_report_t` structure which contains the :c:struct:`nrf_dm_dist_estimates_t` structure with an estimation of the distance.
