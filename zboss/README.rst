.. _zboss:

ZBOSS Zigbee stack
==================

The |NCS|'s Zigbee stack uses ZBOSS – a portable, high-performance Zigbee software protocol stack that allows for interoperability, customizing, testing, and optimizing of your Zigbee solution.

For detailed documentation of the ZBOSS API and instructions on how to use it, see `ZBOSS API documentation`_.

ZBOSS configuration
===================

The ZBOSS stack is distributed as a set of precompiled linkable libraries.
There are two variants of the libraries that support different Zigbee device roles, each variant having its own configuration.
For an overview of the ZBOSS library configuration, see the following files:

* :file:`zboss/include/osif/libzboss_config.h` - library for Coordinators and Routers
* :file:`zboss/include/osif/libzboss_config.ed.h` - library for End Devices

These libraries are used in the Zigbee protocol configuration in |NCS| when defining the Zigbee device role, as described in :ref:`nrf:zigbee_ug_configuration` in the |NCS| documentation.

.. toctree::
   :maxdepth: 1
   :caption: Contents:

   CHANGELOG
