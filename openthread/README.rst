.. _ot_libs:

OpenThread pre-built libraries
##############################

The |NCS| allows for using certified OpenThread libraries.
This simplifies the OpenThread certification process of the final product.

.. note:: The currently provided libraries are not yet certified.


Features of the pre-built libraries
***********************************

The OpenThread pre-built libraries were created based on commit `168248677cacbf9cc5e0c94b8f0682259cfd5f40` of OpenThread.

The following additional Kconfig options were enabled for the build:

* :option:`CONFIG_OPENTHREAD_COMMISSIONER`
* :option:`CONFIG_OPENTHREAD_JOINER`
* :option:`CONFIG_OPENTHREAD_DHCP6_CLIENT`
* :option:`CONFIG_OPENTHREAD_SLAAC`
* :option:`CONFIG_OPENTHREAD_BORDER_ROUTER`


Using the pre-built libraries
*****************************

Set the following Kconfig option to enable OpenThread pre-built libraries: :option:`CONFIG_OPENTHREAD_LIBRARY_1_1`.
This option disables building OpenThread from source files and links pre-built libraries instead.
