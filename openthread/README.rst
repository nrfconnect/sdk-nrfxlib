.. _ot_libs:

OpenThread pre-built libraries
##############################

The |NCS| allows for using certified OpenThread libraries for Nordic devices.
This simplifies the OpenThread certification process of the final product.

The libraries vary depending on stack configuration and hardware cryptography capabilities.
You can choose the stack configuration from one of the predefined set of values, also called feature sets.
For more information about OpenThread feature sets and how to select them in |NCS|, see :ref:`nrf:thread_ug_feature_sets`.

The cryptography variant is selected automatically based on the microcontroller capabilities.

.. note:: The currently provided libraries are not yet certified.

Pre-built library variants
**************************

The OpenThread pre-built libraries were created based on commit ``07f430dac6c400abdd358ba1173c0f94b94a5a80`` of OpenThread,
using the GNU Arm Embedded Toolchain version ``9-2019-q4-major``.

The following table describes the Kconfig options that were set for the build for each variant.
For a more concise list of features enabled for each feature set, see :ref:`nrf:thread_ug_feature_sets`.

.. list-table::
   :widths: 40 15 15 15
   :header-rows: 1

   * - OpenThread Kconfig option
     - Master Image
     - FTD Image
     - MTD Image
   * - :option:`CONFIG_OPENTHREAD_BORDER_AGENT`
     - ``y``
     - ``n``
     - ``n``
   * - :option:`CONFIG_OPENTHREAD_BORDER_ROUTER`
     - ``y``
     - ``n``
     - ``n``
   * - :option:`CONFIG_OPENTHREAD_CHILD_SUPERVISION`
     - ``y``
     - ``y``
     - ``y``
   * - :option:`CONFIG_OPENTHREAD_COAP`
     - ``y``
     - ``y``
     - ``y``
   * - :option:`CONFIG_OPENTHREAD_COAPS`
     - ``y``
     - ``y``
     - ``y``
   * - :option:`CONFIG_OPENTHREAD_COMMISSIONER`
     - ``y``
     - ``n``
     - ``n``
   * - :option:`CONFIG_OPENTHREAD_DEBUG`
     - ``y``
     - ``y``
     - ``y``
   * - :option:`CONFIG_OPENTHREAD_DHCP6_CLIENT`
     - ``y``
     - ``y``
     - ``y``
   * - :option:`CONFIG_OPENTHREAD_DHCP6_SERVER`
     - ``y``
     - ``n``
     - ``n``
   * - :option:`CONFIG_OPENTHREAD_DIAG`
     - ``y``
     - ``n``
     - ``n``
   * - :option:`CONFIG_OPENTHREAD_DNS_CLIENT`
     - ``y``
     - ``y``
     - ``y``
   * - :option:`CONFIG_OPENTHREAD_ECDSA`
     - ``y``
     - ``y``
     - ``y``
   * - :option:`CONFIG_OPENTHREAD_ENABLE_SERVICE`
     - ``y``
     - ``y``
     - ``n``
   * - :option:`CONFIG_OPENTHREAD_FTD`
     - ``y``
     - ``y``
     - ``n``
   * - :option:`CONFIG_OPENTHREAD_MTD`
     - ``n``
     - ``n``
     - ``y``
   * - :option:`CONFIG_OPENTHREAD_IP6_FRAGM`
     - ``y``
     - ``y``
     - ``y``
   * - :option:`CONFIG_OPENTHREAD_JAM_DETECTION`
     - ``y``
     - ``y``
     - ``y``
   * - :option:`CONFIG_OPENTHREAD_JOINER`
     - ``y``
     - ``y``
     - ``y``
   * - :option:`CONFIG_OPENTHREAD_L2_DEBUG`
     - ``y``
     - ``y``
     - ``y``
   * - :option:`CONFIG_OPENTHREAD_L2_LOG_LEVEL`
     - ``3``
     - ``3``
     - ``3``
   * - :option:`CONFIG_OPENTHREAD_L2_LOG_LEVEL_INF`
     - ``y``
     - ``y``
     - ``y``
   * - :option:`CONFIG_OPENTHREAD_LOG_LEVEL`
     - ``1``
     - ``1``
     - ``1``
   * - :option:`CONFIG_OPENTHREAD_LOG_LEVEL_CRIT`
     - ``y``
     - ``y``
     - ``y``
   * - :option:`CONFIG_OPENTHREAD_MAC_FILTER`
     - ``y``
     - ``y``
     - ``y``
   * - :option:`CONFIG_OPENTHREAD_MBEDTLS`
     - ``n``
     - ``n``
     - ``n``
   * - :option:`CONFIG_OPENTHREAD_MTD_NETDIAG`
     - ``y``
     - ``n``
     - ``n``
   * - :option:`CONFIG_OPENTHREAD_NORDIC_LIBRARY`
     - ``y``
     - ``y``
     - ``y``
   * - :option:`CONFIG_OPENTHREAD_NORDIC_LIBRARY_MASTER`
     - ``y``
     - ``n``
     - ``n``
   * - :option:`CONFIG_OPENTHREAD_NORDIC_LIBRARY_FTD`
     - ``n``
     - ``y``
     - ``n``
   * - :option:`CONFIG_OPENTHREAD_NORDIC_LIBRARY_MTD`
     - ``n``
     - ``n``
     - ``y``
   * - :option:`CONFIG_OPENTHREAD_NRF_SECURITY`
     - ``y``
     - ``y``
     - ``y``
   * - :option:`CONFIG_OPENTHREAD_PKT_LIST_SIZE`
     - ``10``
     - ``10``
     - ``10``
   * - :option:`CONFIG_OPENTHREAD_RAW`
     - ``y``
     - ``n``
     - ``n``
   * - :option:`CONFIG_OPENTHREAD_SHELL`
     - ``y``
     - ``y``
     - ``y``
   * - :option:`CONFIG_OPENTHREAD_SLAAC`
     - ``y``
     - ``y``
     - ``y``
   * - :option:`CONFIG_OPENTHREAD_SNTP_CLIENT`
     - ``y``
     - ``y``
     - ``y``
   * - :option:`CONFIG_OPENTHREAD_THREAD_VERSION`
     - ``1.1``
     - ``1.1``
     - ``1.1``
   * - :option:`CONFIG_OPENTHREAD_UDP_FORWARD`
     - ``y``
     - ``y``
     - ``n``
