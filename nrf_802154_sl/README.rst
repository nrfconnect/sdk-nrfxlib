.. _nrf_802154_sl:

nRF 802.15.4 Service Layer
==========================

The nRF 802.15.4 Service Layer (SL) is a library that allows you to enable advanced features of the nRF 802.15.4 Radio Driver.

Currently, the library implements the following features:

* Multiprotocol support.
  It allows the concurrency of Bluetooth LE and 802.15.4 operations.
* CSMA/CA support.
  It provides a built-in CSMA/CA mechanism, optimized for performance.
* Front-end Module (FEM) support.
  It provides an API for controlling external signal amplifiers.
* Delayed transmission and reception.
  It allows the scheduling of transmissions and receptions, to execute them at a predefined moment.
* Timestamping.
  It provides precise frame timestamps.

The library also defines APIs for features that are not yet supported:

* WiFi coexistence support.
  It provides an API for facilitating the integration with an external PTA.
* Antenna diversity support.
  It provides an API for runtime antenna selection.

The library is available as binaries for the following SoCs:

* nRF52840
* nRF52833
* nRF52820
* nRF52811

For the SOCs equipped with a floating-point unit (nRF52840 and nRF52833), the library is available in the soft-float, softfp-float, and hard-float build versions.
For the other SOCs, it is available only in the soft-float build version.

To use the library:

* Add the header files to the include directories of the project
* Add the binaries to the linking stage of the build.
  In the |NCS|, this is done through Kconfig.

.. toctree::
   :maxdepth: 2
   :caption: Subpages:

   CHANGELOG
