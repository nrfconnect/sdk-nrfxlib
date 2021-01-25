.. _zboss_configuration:

ZBOSS library configuration
###########################

The ZBOSS stack is distributed as a set of precompiled linkable libraries.
These libraries are available in the following versions:

* `ZBOSS production libraries`_
* `ZBOSS development libraries`_

Each version contains variants of the libraries that support different Zigbee device roles, each variant having its own configuration.
This allows you to scale the application and select the most suitable set of features.

ZBOSS production libraries
**************************

|list_intro|

* :file:`zboss/production/include/osif/libzboss_config.h` - library for Coordinators and Routers
* :file:`zboss/production/include/osif/libzboss_config.ed.h` - library for End Devices

|list_outro|

The ZBOSS production library version is enabled by default with the :option:`CONFIG_ZIGBEE_LIBRARY_PRODUCTION` KConfig option.

ZBOSS development libraries
***************************

The ZBOSS libraries in the development state include all the production code, but also features that are still in the experimental state.
|list_intro|

* :file:`zboss/development/include/osif/libzboss_config.h` - library for Coordinators and Routers
* :file:`zboss/development/include/osif/libzboss_config.ed.h` - library for End Devices

|list_outro|

You can select the ZBOSS development library version with the :option:`CONFIG_ZIGBEE_LIBRARY_DEVELOPMENT` KConfig option.
You can enable additional features in the development version of ZBOSS libraries using the following Kconfig options:

* :option:`CONFIG_ZIGBEE_LIBRARY_NCP_DEV` - With this option enabled, the application may link NCP commands implementation.
  This option is enabled by default in the ``NCP sample``.
* :option:`CONFIG_ZIGBEE_GP_CB` - With this option enabled, the application can support the Green Power Combo feature, which implements the basic set of Green Power Proxy and Green Power Sink functionalities within a single device.
  This option is added only for evaluation purposes and does not have a dedicated sample.

Both of these features are experimental.

ZBOSS library certification
***************************

Zigbee ZBOSS libraries in |NCS| in the production state are always tested in-house by Nordic Semiconductor to conform to Zigbee Pro R22 test specification.
However, these libraries are only periodically verified by external test service provider and granted Zigbee Compliant Platform certification by Zigbee Alliance.

.. |list_intro| replace:: For a complete list of the ZBOSS configuration options, see the following files:
.. |list_outro| replace:: These libraries are used in the Zigbee protocol configuration in |NCS| when defining the Zigbee device role, as described in :ref:`nrf:zigbee_ug_configuration` in the |NCS| documentation.
