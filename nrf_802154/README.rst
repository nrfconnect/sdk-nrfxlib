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

* nRF52833
* nRF52840
* nRF5340

Including the nRF 802.15.4 Radio Driver in a project
----------------------------------------------------

.. _nrf_802154_build_cmake:

Using CMake
^^^^^^^^^^^

The nRF 802.15.4 Radio Driver supports the CMake build system and can be directly included in a project using the CMake build system with the ``add_subdirectory(nrfxlib/nrf_802154)`` CMake command.

To do so, you must first set the ``NRF52_SERIES`` or ``NRF53_SERIES`` CMake variable to ``1``, depending on the used SoC family.

If building on a single core SoC, you should then do the following:

1. Link the following libraries (CMake targets) to the application library:

    * ``nrf-802154-driver``
    * ``nrf-802154-sl``
    * ``nrf-802154-driver-interface``

#. Create another library named ``nrf-802154-platform``.
   This library should contain implementations of all the functions declared in the include files in the directories:

    * :file:`nrfxlib/nrf_802154/driver/include/platform`
    * :file:`nrfxlib/nrf_802154/sl/include/platform`

#. To resolve circular dependencies, link the ``nrf-802154-sl`` library to the ``nrf-802154-platform`` library using the ``PRIVATE`` scope.
#. Either set the appropriate ``NRF_802154_..._RADIO_IRQ_HANDLING`` compilation defines to ``1`` or the appropriate ISRs must call the handlers declared in ``nrf_802154_irq_handlers.h``.

If building on a multicore SoC, you can follow two approaches:

* You can build the entire application on the core controlling the RADIO peripheral.
  In this case, follow the same building process as for a single core SoC.
* The application core is running the application, while the radio core is running the 802.15.4 driver (this must be the core controlling the RADIO peripheral).
  The application core calls functions of the radio core via a serialization layer.

In the second scenario, you must do the following:

1. Follow the steps described for the single core application only for the radio core.
#. Link for both cores the following libraries:

* ``nrf-802154-serialization``
* ``nrf-802154-serialization-interface``

#. Add to the ``nrf-802154-platform`` library for both cores the implementations of all the functions declared in the include files in the :file:`nrfxlib/nrf_802154/serialization/include/platform` directory.

Using other build systems
^^^^^^^^^^^^^^^^^^^^^^^^^

To add the 802.15.4 radio driver to a project using other build systems, create build system scripts listing all the necessary 802.15.4 radio driver files to build.
The lists of the necessary files can be found in the following locations:

* :file:`nrfxlib/nrf_802154/common/CMakeLists.txt`
* :file:`nrfxlib/nrf_802154/driver/CMakeLists.txt`
* :file:`nrfxlib/nrf_802154/serialization/CMakeLists.txt`
* :file:`nrfxlib/nrf_802154/sl/sl/CMakeLists.txt`

Additionally, you must add all the function implementations which are added to the ``nrf-802154-platform`` library when building with CMake.

.. toctree::
   :maxdepth: 2
   :caption: Subpages:

   doc/architecture
   doc/feature_description
   doc/wifi_coex_module
   doc/hardware_resources
   doc/multiprotocol_support
   doc/antenna_diversity
   doc/rd_service_layer_lib
   doc/rd_limitations
   doc/CHANGELOG
   doc/api
