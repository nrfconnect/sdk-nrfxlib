.. _zboss_configuration:

ZBOSS library configuration
###########################

.. contents::
   :local:
   :depth: 2

The ZBOSS stack is distributed as a set of precompiled linkable libraries.
The available libraries are the `ZBOSS production libraries`_.

The libraries supports different Zigbee device roles, with each variant having its own configuration.
This allows you to scale the application and select the most suitable set of features.

These libraries are used in the Zigbee protocol configuration in the |NCS| when defining the Zigbee device role, as described in :ref:`nrf:zigbee_ug_configuration` in the |NCS| documentation.
They are included by the OSIF subsystem, which acts as the linking layer between the ZBOSS stack and |NCS|.
OSIF implements a series of functions used by ZBOSS and is included in the |NCS|'s Zigbee subsystem.

ZBOSS production libraries
**************************

For a complete list of the ZBOSS configuration options, see the following files:

* :file:`zboss/production/include/osif/libzboss_config.h` - Library for Coordinators and Routers
* :file:`zboss/production/include/osif/libzboss_config.ed.h` - Library for End Devices

The ZBOSS production library version is enabled by default with the :kconfig:option:`CONFIG_ZIGBEE_LIBRARY_PRODUCTION` Kconfig option.

Configuration options
*********************

In the |NCS|, you can enable the ZBOSS library using the :kconfig:option:`CONFIG_ZIGBEE` Kconfig option.
Enabling this library is required when configuring the Zigbee protocol in the |NCS|, for example when testing the available :ref:`nrf:zigbee_samples`.

To enable additional features in the ZBOSS libraries, you can use the following Kconfig options:

* :kconfig:option:`CONFIG_ZIGBEE_LIBRARY_NCP_DEV` - With this option enabled, the application links with an additional library, which implements NCP commands.
  This option is enabled by default in the :ref:`Zigbee NCP sample <nrf:zigbee_ncp_sample>`.
  This option uses a production version of ZBOSS that has not been certified.
* :kconfig:option:`CONFIG_ZIGBEE_GP_CB` - With this option enabled, the application can support the Green Power Combo feature, which implements the basic set of Green Power Proxy and Green Power Sink functionalities within a single device.
  This option can only be enabled for an application that is built from ZBOSS stack sources.
  It has been added only for evaluation purposes and does not have a dedicated sample.
