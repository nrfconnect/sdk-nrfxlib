.. _ot_libs:

OpenThread pre-built libraries
##############################

The |NCS| allows for using certified OpenThread libraries.
This simplifies the OpenThread certification process of the final product.

.. note:: The currently provided libraries are not yet certified.


Features of the pre-built libraries
***********************************

The OpenThread pre-built libraries were created based on commit `656802a76acd2920b74ee4d49363f70aeb3b9621` of OpenThread,
using the GNU Arm Embedded Toolchain 9-2019-q4-major.

The following additional Kconfig options were set for the build:

* :option:`CONFIG_OPENTHREAD_BORDER_AGENT` set to ``y``
* :option:`CONFIG_OPENTHREAD_BORDER_ROUTER` set to ``y``
* :option:`CONFIG_OPENTHREAD_CHANNEL` set to ``11``
* :option:`CONFIG_OPENTHREAD_CHILD_SUPERVISION` set to ``y``
* :option:`CONFIG_OPENTHREAD_COAP` set to ``y``
* :option:`CONFIG_OPENTHREAD_COAPS` set to ``y``
* :option:`CONFIG_OPENTHREAD_COMMISSIONER` set to ``y``
* :option:`CONFIG_OPENTHREAD_DEBUG` set to ``y``
* :option:`CONFIG_OPENTHREAD_DHCP6_CLIENT` set to ``y``
* :option:`CONFIG_OPENTHREAD_DHCP6_SERVER` set to ``y``
* :option:`CONFIG_OPENTHREAD_DIAG` set to ``y``
* :option:`CONFIG_OPENTHREAD_DNS_CLIENT` set to ``y``
* :option:`CONFIG_OPENTHREAD_ECDSA` set to ``y``
* :option:`CONFIG_OPENTHREAD_ENABLE_SERVICE` set to ``y``
* :option:`CONFIG_OPENTHREAD_FTD` set to ``y``
* :option:`CONFIG_OPENTHREAD_IP6_FRAGM` set to ``y``
* :option:`CONFIG_OPENTHREAD_JAM_DETECTION` set to ``y``
* :option:`CONFIG_OPENTHREAD_JOINER` set to ``y``
* :option:`CONFIG_OPENTHREAD_L2_DEBUG` set to ``y``
* :option:`CONFIG_OPENTHREAD_L2_LOG_LEVEL` set to ``3``
* :option:`CONFIG_OPENTHREAD_L2_LOG_LEVEL_INF` set to ``y``
* :option:`CONFIG_OPENTHREAD_LOG_LEVEL` set to ``1``
* :option:`CONFIG_OPENTHREAD_LOG_LEVEL_CRIT` set to ``y``
* :option:`CONFIG_OPENTHREAD_MAC_FILTER` set to ``y``
* :option:`CONFIG_OPENTHREAD_MBEDTLS` set to ``y``
* :option:`CONFIG_OPENTHREAD_MBEDTLS_LIB_NAME` set to ``mbedTLS``
* :option:`CONFIG_OPENTHREAD_MTD_NETDIAG` set to ``y``
* :option:`CONFIG_OPENTHREAD_NETWORK_NAME` set to ``ot_zephyr``
* :option:`CONFIG_OPENTHREAD_NORDIC_LIBRARY` set to ``y``
* :option:`CONFIG_OPENTHREAD_NORDIC_LIBRARY_MASTER` set to ``y``
* :option:`CONFIG_OPENTHREAD_NRF_SECURITY` set to ``y``
* :option:`CONFIG_OPENTHREAD_PANID` set to ``43981``
* :option:`CONFIG_OPENTHREAD_PKT_LIST_SIZE` set to ``10``
* :option:`CONFIG_OPENTHREAD_PLATFORM_INFO` set to ``ZEPHYR``
* :option:`CONFIG_OPENTHREAD_RAW` set to ``y``
* :option:`CONFIG_OPENTHREAD_SHELL` set to ``y``
* :option:`CONFIG_OPENTHREAD_SLAAC` set to ``y``
* :option:`CONFIG_OPENTHREAD_SNTP_CLIENT` set to ``y``
* :option:`CONFIG_OPENTHREAD_SOURCES` set to ``y``
* :option:`CONFIG_OPENTHREAD_THREAD_PRIORITY` set to ``8``
* :option:`CONFIG_OPENTHREAD_THREAD_STACK_SIZE` set to ``6144``
* :option:`CONFIG_OPENTHREAD_THREAD_VERSION` set to ``1.1``
* :option:`CONFIG_OPENTHREAD_THREAD_VERSION_1_1` set to ``y``
* :option:`CONFIG_OPENTHREAD_UDP_FORWARD` set to ``y``
* :option:`CONFIG_OPENTHREAD_XPANID` set to ``de:ad:00:be:ef:00:ca:fe``


Using the pre-built libraries
*****************************

Set the following Kconfig option to enable OpenThread pre-built libraries: :option:`CONFIG_OPENTHREAD_LIBRARY_1_1`.
This option disables building OpenThread from source files and links pre-built libraries instead.
