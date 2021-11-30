.. _nrf_dm:

Nordic Distance Measurement library
###################################

The Distance Measurement library measures the distance between nRF52 Series devices.
It does the following:

* It provides an easy-to-use interface for measuring distances.
* It computes the distance using all the information available to the device.
* It computes the distance based on the measured differential RF physical channel frequency response (MCPD mode) or the real time flight of packets (RTT mode).
* It performs all the mathematics necessary to compute the distance inside the library.

The library is available as hard-float builds for the nRF52 Series, and it is included as a pre-built static library.

The library has the following dependencies:

* The DSP math library from CMSIS/DSP.
* The functions from the C Library :file:`math.h`.

.. toctree::
   :maxdepth: 2
   :caption: Subpages:

   doc/nrf_dm_overview
   doc/nrf_dm_usage
   doc/CHANGELOG
   doc/api
