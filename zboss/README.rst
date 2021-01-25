.. _zboss:

ZBOSS Zigbee stack
##################

The |NCS|'s Zigbee stack uses ZBOSS – a portable, high-performance Zigbee software protocol stack that allows for interoperability, customizing, testing, and optimizing of your Zigbee solution.

The nrfxlib repository contains the following versions of the ZBOSS libraries:

* *Production* version that contains the latest stable ZBOSS libraries.
  This version is enabled with the :option:`CONFIG_ZIGBEE_LIBRARY_PRODUCTION` KConfig option and its files are located in the :file:`zboss/production/` directory.
  This version is enabled by default.
* *Development* version that contains the latest version of ZBOSS libraries, with experimental features included.
  This version is enabled with the :option:`CONFIG_ZIGBEE_LIBRARY_DEVELOPMENT` KConfig option and its files are located in the :file:`zboss/development/` directory.
  This version might not conform to the latest Zigbee Pro R22 test specification.

For information about additional configuration of these libraries and their certification status, see :ref:`zboss_configuration`.
For detailed documentation of the ZBOSS API for these versions and instructions on how to use it, see `ZBOSS API documentation`_.

.. toctree::
   :maxdepth: 1
   :caption: Subpages:

   CHANGELOG
   doc/zboss_configuration
