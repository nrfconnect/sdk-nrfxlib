.. _ot_libs:

OpenThread pre-built libraries
##############################

.. contents::
   :local:
   :depth: 2

The |NCS| allows for using certified OpenThread libraries for Nordic devices.
This simplifies the OpenThread certification process of the final product.

The libraries vary depending on stack configuration and hardware cryptography capabilities.
You can choose the stack configuration from one of the predefined set of values, also called feature sets.
For more information about OpenThread feature sets and how to select them in |NCS|, see :ref:`nrf:thread_ug_feature_sets`.

The cryptography variant is selected automatically based on the microcontroller capabilities.

Thread certified library variants
*********************************

The following table describes the available Thread certified libraries for all |NCS| releases.

.. list-table::
   :header-rows: 1

   * - SoC
     - |NCS| tag
     - Thread version
     - Crypto backend
     - OpenThread commit
     - GNU Arm Embedded Toolchain
     - Feature set
   * - nRF52840
     - v1.4.0
     - :option:`CONFIG_OPENTHREAD_THREAD_VERSION_1_1`
     - CC310
     - ``07f430dac6c400abdd358ba1173c0f94b94a5a80``
     - ``9-2019-q4-major``
     - :option:`CONFIG_OPENTHREAD_NORDIC_LIBRARY_MASTER`
   * - nRF52833
     - v1.4.0
     - :option:`CONFIG_OPENTHREAD_THREAD_VERSION_1_1`
     - nrf_oberon
     - ``07f430dac6c400abdd358ba1173c0f94b94a5a80``
     - ``9-2019-q4-major``
     - :option:`CONFIG_OPENTHREAD_NORDIC_LIBRARY_MASTER`

For a comprehensive list of the used Kconfig options, check the :file:`openthread_lib_configuration.txt` file within each library folder.

.. note:: Only library variants shown in the table have been certified.
