.. _zboss_configuration:

ZBOSS library configuration
###########################

.. contents::
   :local:
   :depth: 2

The ZBOSS stack is distributed as a set of precompiled linkable libraries.

The libraries support different Zigbee device roles, with each variant having its own configuration.
This allows you to scale the application and select the most suitable set of features.

|list_intro|

* :file:`zboss/include/osif/libzboss_config.h` - library for Coordinators and Routers
* :file:`zboss/include/osif/libzboss_config.ed.h` - library for End Devices

|list_outro|

You can enable additional features in the ZBOSS libraries using the following Kconfig options:

* :option:`CONFIG_ZIGBEE_LIBRARY_NCP_DEV` - With this option enabled, the application links with an additional library, which implements NCP commands.
  This option is enabled by default in the ``NCP sample``.
* :option:`CONFIG_ZIGBEE_GP_CB` - With this option enabled, the application can support the Green Power Combo feature, which implements the basic set of Green Power Proxy and Green Power Sink functionalities within a single device.
  This option is added only for evaluation purposes and does not have a dedicated sample.

Both of these features are experimental.

ZBOSS library certification
***************************

Zigbee ZBOSS libraries in the |NCS| are always tested in-house by Nordic Semiconductor to conform to Zigbee Pro R22 test specification.
However, these libraries are only periodically verified by external test service provider and granted Zigbee Compliant Platform certification by Zigbee Alliance.

.. |list_intro| replace:: For a complete list of the ZBOSS configuration options, see the following files:
.. |list_outro| replace:: These libraries are used in the Zigbee protocol configuration in the |NCS| when defining the Zigbee device role, as described in :ref:`nrf:zigbee_ug_configuration` in the |NCS| documentation.
