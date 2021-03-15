.. _nrf_802154:

nRF 802.15.4 Radio Driver
=========================

The nRF 802.15.4 Radio Driver is a set of libraries that implement the IEEE 802.15.4 functionalities for the SoCs from the nRF52 and nRF53 families.
The following functionalities are implemented:

* Frame transmission.
* Frame reception and filtering.
* Acknowledgments.
  It allows sending and receiving acknowledgments according to the protocol specification and the timing requirements.
  Both ``Imm-Acks`` and ``Enh-Acks`` are supported.
* Promiscuous mode.
* Low power (sleep) mode.
* Carrier transmission.
  It allows sending a carrier wave continuously, either modulated or unmodulated.
  This feature can be used for radio tests.
* Multiprotocol support.
  It allows the concurrency of Bluetooth LE and 802.15.4 operations.
* CSMA/CA support.
  It provides a built-in CSMA/CA mechanism, optimized for performance.
* Delayed transmission and reception.
  It allows the scheduling of transmissions and receptions, to execute them at a predefined moment.
  It can be used for features like CSL, TSCH, and Zigbee GPP.
* Timestamping.
  It provides precise frame timestamps.
* PA/LNA.
  It allows the Radio Driver to control an external PA/LNA module to amplify the RF signal.

The 802.15.4 Radio Driver consists of the following libraries:

* 802.15.4 Radio Core
* 802.15.4 Service Layer
* 802.15.4 Serialization

The libraries are compatible with the following SoCs:

* nRF52811
* nRF52833
* nRF52840
* nRF5340

The nRF 802.15.4 Radio Driver supports the CMake build system and can be directly included in a project using the CMake build system with the ``add_subdirectory(nrfxlib/nrf_802154)`` CMake command.

To add the 802.15.4 radio driver to a project using other build systems, create build system scripts listing all the necessary 802.15.4 radio driver files to build.
The lists of the necessary files can be found in the following locations:

* :file:`nrfxlib/nrf_802154/driver/CMakeLists.txt`
* :file:`nrfxlib/nrf_802154/serialization/CMakeLists.txt`
* :file:`nrfxlib/nrf_802154/sl/sl/CMakeLists.txt`

.. toctree::
   :maxdepth: 2
   :caption: Subpages:

   doc/architecture
   doc/feature_description
   doc/front_end_module
   doc/hardware_resources
   doc/multiprotocol_support
   doc/rd_service_layer_lib
   doc/CHANGELOG
