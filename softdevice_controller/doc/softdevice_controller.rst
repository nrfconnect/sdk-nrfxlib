.. _softdevice_controller_readme:

Integration with applications
#############################

.. contents::
   :local:
   :depth: 2

The |controller| is an RTOS-agnostic library built for the Nordic Semiconductor devices that support Bluetooth.

For the nRF53 Series, the requirements described in this document are only relevant for applications running alongside the |controller| on the network processor.
For the nRF54H Series, some peripherals in the global domain are reserved so the requirements described here are relevant for all processors.

Applications utilizing the |controller| library are responsible for the following:

* Ensuring a source of random numbers is available to the |controller| (see the message sequence charts below).
* The |controller| relies on the functionality provided by Nordic Semiconductor's :ref:`mpsl`, which has its own set of requirements that the application must follow.
* Ensuring thread-safe operation.
  This can be achieved by either:

   * Calling all |controller| and MPSL APIs from the same execution priority.
   * Ensuring that no API call is interrupted by other API calls, for example by using critical sections.
* Configuring the number of available links:

   * Link configuration must be done before enabling the controller.
   * The application is required to provide a memory buffer the size of which is dependent on the link configuration.


The following peripherals are owned by the |controller| and must not be accessed directly by the application:

For the nRF52 Series:

* ``CCM``
* ``AAR``
* ``NVMC``
* ``PPI`` channels from ``17`` to ``31``

For the nRF53 Series:

* ``CCM``
* ``AAR``
* ``NVMC``
* ``DPPI`` channels from ``3`` to ``12``

For the nRF54H Series:

* ``CCM031``
* ``AAR031``
* ``DPPIC020`` channels from ``1`` to ``11``
* ``DPPIC030`` channels ``1`` and ``3``
* ``PPIB030`` channels from ``0`` to ``3``

For the nRF54L Series:

* ``CCM00``
* ``AAR00``
* ``RRAMC``
* ``DPPIC10`` channels from ``1`` to ``11``
* ``DPPIC00`` channels ``1`` and ``3``
* ``PPIB00`` channels from ``0`` to ``3``
* ``PPIB10`` channels from ``0`` to ``3``


All of the listed resources can be accessed directly using the :ref:`mpsl_timeslot` feature.

Message sequence charts
***********************

The following message sequence chart shows an example of how to initialize the library.
The :ref:`mpsl` must be initialized before the |controller| is initialized.
The application can choose the features that are enabled and the number of links that are supported.

.. msc::
    hscale = "1.5";
    Host,Controller;
    |||;
    Host rbox Controller [label = "Library initialization"];
    Host->Controller      [label="sdc_init()"];
    Host rbox Controller [label = "Library configuration"];
    Host->Controller      [label="sdc_rand_source_register(...)"];
    Host->Controller      [label="sdc_cfg_set(SDC_CFG_TYPE_ADV_COUNT)"];
    Host->Controller      [label="sdc_cfg_set(SDC_CFG_TYPE_CENTRAL_COUNT)"];
    Host->Controller      [label="sdc_cfg_set(SDC_CFG_TYPE_PERIPHERAL_COUNT)"];
    Host rbox Controller[label = "Feature inclusion"];
    Host->Controller      [label="sdc_support_adv()"];
    Host->Controller      [label="sdc_support_peripheral()"];
    Host->Controller      [label="sdc_support_2m_phy()"];
    Host rbox Controller [label = "Enable the SoftDevice Controller"];
    Host->Controller      [label="sdc_enable()"];

The following message sequence chart shows a scenario where the application sends a command to the controller.
Later, it retrieves an event and a data packet.
When the host signal is triggered, this indicates that one or more HCI data or event packets are available.
Therefore, the host should try to pull data and events until it fails.

.. msc::
    hscale = "1.5";
    Host,Controller;
    |||;
    Host rbox Controller [label = "Send a command to the controller"];
    Host->Controller      [label="sdc_hci_cmd_cb_set_event_mask()"];
    Host<<Controller      [label="Command Complete Status"];
    Host rbox Controller [label = "Send data to the controller"];
    Host->Controller      [label="sdc_hci_data_put()"];
    Host<-Controller      [label="Host signal is triggered"];
    Host->Controller      [label="sdc_evt_get()"];
    Host<<Controller      [label="HCI Number Of Completed packets"];
    Host rbox Controller [label = "The controller receives some data and raises an event"];
    Host<-Controller      [label="Host signal is triggered"];
    Host->Controller      [label="sdc_evt_get()"];
    Host<<Controller      [label="Retrieved event"];
    Host->Controller      [label="sdc_data_get()"];
    Host<<Controller      [label="Retrieved data"];


Architecture diagrams
*********************

The following image shows how the |controller| and MPSL integrates into an RTOS-free environment:

.. figure:: pic/Architecture_Without_RTOS.svg
   :alt: Architecture without RTOS

   Architecture without RTOS

The following image shows how the |controller| and MPSL integrates into an RTOS:

.. figure:: pic/Architecture_With_RTOS.svg
   :alt: Architecture with RTOS

   Architecture with RTOS
