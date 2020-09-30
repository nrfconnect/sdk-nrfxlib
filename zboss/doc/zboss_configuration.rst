.. _zboss_configuration:

ZBOSS configuration
###################

The ZBOSS stack is distributed as a set of precompiled linkable libraries.
There are two variants of the libraries that support different Zigbee device roles, each variant having its own configuration.
For a complete list of the ZBOSS configuration options, see the following files:

* :file:`zboss/include/osif/libzboss_config.h` - library for Coordinators and Routers
* :file:`zboss/include/osif/libzboss_config.ed.h` - library for End Devices

These libraries are used in the Zigbee protocol configuration in |NCS| when defining the Zigbee device role, as described in :ref:`nrf:zigbee_ug_configuration` in the |NCS| documentation.
