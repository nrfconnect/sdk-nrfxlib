.. _zboss_configuration:

ZBOSS library configuration
###########################

.. contents::
   :local:
   :depth: 2

The ZBOSS stack is distributed as a set of precompiled linkable libraries.

The libraries support different Zigbee device roles, with each variant having its own configuration.
This allows you to scale the application and select the most suitable set of features.

For a complete list of the ZBOSS configuration options, see the following files:

* :file:`zboss/include/osif/libzboss_config.h` - library for Coordinators and Routers
* :file:`zboss/include/osif/libzboss_config.ed.h` - library for End Devices

These libraries are used in the Zigbee protocol configuration in the |NCS| when defining the Zigbee device role, as described in :ref:`nrf:zigbee_ug_configuration` in the |NCS| documentation.
They are included in the OSIF subsystem, which acts as the linking layer between the ZBOSS stack and |NCS|.
OSIF implements a series of functions used by ZBOSS and is included in the |NCS|'s Zigbee subsystem.

Configuration options
*********************

In the |NCS|, you can enable the ZBOSS library using the :kconfig:`CONFIG_ZIGBEE` Kconfig option.
Enabling this library is required when configuring the Zigbee protocol in the |NCS|, for example when testing the available :ref:`nrf:zigbee_samples`.

To enable additional features in the ZBOSS libraries, you can use the following Kconfig options:

* :kconfig:`CONFIG_ZIGBEE_LIBRARY_NCP_DEV` - With this option enabled, the application links with an additional library, which implements NCP commands.
  This option is enabled by default in the :ref:`Zigbee NCP sample <nrf:zigbee_ncp_sample>`.
  This option uses a production version of ZBOSS that has not been certified.
* :kconfig:`CONFIG_ZIGBEE_GP_CB` - With this option enabled, the application can support the Green Power Combo feature, which implements the basic set of Green Power Proxy and Green Power Sink functionalities within a single device.
  This option requires access to ZBOSS stack sources to be enabled.
  This option is added only for evaluation purposes and does not have a dedicated sample.
