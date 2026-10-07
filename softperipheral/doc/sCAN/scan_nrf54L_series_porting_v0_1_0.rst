.. _scan_nrf54L_series_porting_guide:

sCAN nRF54L Series porting guide
################################

.. contents::
   :local:
   :depth: 2

This page describes the code structure, file hierarchy, configuration, and requirements for implementing an sCAN application on an nRF54L Series device.
This guide targets applications built with the nRF Connect SDK and Zephyr.

.. _scan_nrf54l_series_porting_guide_code:

sCAN application code
*********************

This structure shows the relevant files and directories in the `sdk-nrfxlib`_ repository:

.. code-block::

   nrfxlib/
   ├──...
   └── softperipheral/
      ├── include
      │   ├── softperipheral_meta.h
      │   └── softperipheral_regif.h
      ├── ...
      └── sCAN
          ├── include
          │   ├── hal
          │   │   └── nrf_can.h
          │   ├── nrf54l
          │   │   ├── scan_firmware.h
          │   │   ├── scan_firmware_v0.1.0.h
          │   │   └── ...
          │   ├── nrf_config_scan.h
          │   ├── nrf_sp_can.h
          │   ├── nrf_scan.h
          └── src
              └── nrf_scan.c

.. note::
   The :file:`nrf_scan.h` file provides the main sCAN interface.

Header files
============

An sCAN application requires the header files used by the driver.
You must include paths to these files in the build environment's list of include paths.
The following paths are required:

* Repository internal paths:

  * :file:`softperipheral/include` - Soft peripheral register interface and metadata.
  * :file:`softperipheral/sCAN/include` - sCAN register interface and driver header file.
  * :file:`softperipheral/sCAN/include/nrf54l` - sCAN firmware for the Fast Lightweight Peripheral Processor (FLPR).

To override the configuration options in :file:`nrf_config_scan.h`, use the ``zephyr_compile_definitions`` macro in your application:

.. code-block:: c

   // Enable sCAN.
   zephyr_compile_definitions("NRF_SCAN_ENABLED=1")

Compiling source files
======================

For an sCAN application to function properly, compile the driver implementation in :file:`nrf_scan.c`.

Application core and FLPR configuration
***************************************

Configure the nRF54L Series SoC to run at its highest base clock frequency.

Before applying the following settings, allocate memory for the data pointers used by the sCAN driver.
This memory is separate from the memory described in the :ref:`scan_nrf54L_series_porting_guide_ram_configuration` section.
It provides shared memory for communication and does not contain the sCAN executable code.

Security configuration
======================

Configure the security settings of the nRF54L Series device to meet the requirements of your application.

Secure environment
------------------

In a secure configuration, both the application core and the FLPR core must operate in a TrustZone secure environment.

The following example configures permissions for the FLPR instance:

.. code-block:: c

  // Set FLPR to secure.
  NRF_SPU00_S->PERIPH[0xC].PERM = (SPU_PERIPH_PERM_SECATTR_Secure << SPU_PERIPH_PERM_SECATTR_Pos);

Non-secure environment
----------------------

In a non-secure configuration, both the application core and the FLPR core must operate outside the TrustZone secure environment.

.. _scan_nrf54l_series_porting_guide_gpio_config:

GPIO configuration
******************

The following table describes the configuration of each GPIO pin used by the sCAN application.
This configuration assumes that the FLPR core has access to the listed port and pins.

 .. list-table::
   :widths: 10 10 20 20 20
   :header-rows: 1

   * - GPIO pin
     - sCAN role
     - Direction config
     - Input config
     - Pin pull config
   * - **P2.2**
     - TX
     - Output
     - Disconnected
     - Pull-up
   * - **P2.4**
     - RX
     - Output
     - Disconnected
     - No-pull

Configuring pins
================

In the devicetree overlay, disable peripherals that conflict with sCAN on the same port and any conflicting memory partitions.
The following example grants FLPR access to the required pins.

The ``pinctrl`` block configures the GPIOs as described in the :ref:`scan_nrf54l_series_porting_guide_gpio_config` section.

The ``cpuflpr_vpr`` block and the subsequent block allocate the VPR memory region and the RAM used by sCAN.

.. tabs::

   .. tab:: **nRF54L15**

      .. code-block:: dts

         &pinctrl {
           scan_default: scan_default {
             group1 {
               psels = <NRF_PSEL(SDP_MSPI_DQ1, 2, 2)>; // TX
               nordic,drive-mode = <NRF_DRIVE_S0S1>;
               bias-pull-up;
             };
             group2 {
               psels = <NRF_PSEL(SDP_MSPI_DQ3, 2, 4)>; // RX
               nordic,drive-mode = <NRF_DRIVE_S0S1>;
               bias-disable;
             };
           };

           scan_sleep: scan_sleep {
             group1 {
               low-power-enable;
               psels = <NRF_PSEL(SDP_MSPI_DQ1, 2, 2)>,
                       <NRF_PSEL(SDP_MSPI_DQ3, 2, 4)>;
             };
           };
         };

         &cpuflpr_vpr {
           pinctrl-0 = <&scan_default>;
           pinctrl-1 = <&scan_sleep>;
           pinctrl-names = "default", "sleep";
           interrupts = <76 NRF_DEFAULT_IRQ_PRIORITY>;
           status = "okay";
           /delete-property/ execution-memory;
         };

         / {
           reserved-memory {
             #address-cells = <1>;
             #size-cells = <1>;
             ranges;

             softperiph_ram: memory@2003B400 {
               reg = <0x2003B400 0x4400>;
               ranges = <0 0x2003B400 0x4400>;
               #address-cells = <1>;
               #size-cells = <1>;

               dut: scan: scan@4200 {
                 #address-cells = <1>;
                 #size-cells = <0>;
                 reg = <0x4200 0x200>;
                 zephyr,pm-device-runtime-auto;
               };
             };
           };
         };

   .. tab:: **nRF54LM20**

      .. code-block:: dts

         &pinctrl {
           scan_default: scan_default {
             group1 {
               psels = <NRF_PSEL(SDP_MSPI_DQ1, 2, 2)>; // TX
               nordic,drive-mode = <NRF_DRIVE_S0S1>;
               bias-pull-up;
             };
             group2 {
               psels = <NRF_PSEL(SDP_MSPI_DQ3, 2, 4)>; // RX
               nordic,drive-mode = <NRF_DRIVE_S0S1>;
               bias-disable;
             };
           };

           scan_sleep: scan_sleep {
             group1 {
               low-power-enable;
               psels = <NRF_PSEL(SDP_MSPI_DQ1, 2, 2)>,
                       <NRF_PSEL(SDP_MSPI_DQ3, 2, 4)>;
             };
           };
         };

         &cpuflpr_vpr {
           pinctrl-0 = <&scan_default>;
           pinctrl-1 = <&scan_sleep>;
           pinctrl-names = "default", "sleep";
           interrupts = <76 NRF_DEFAULT_IRQ_PRIORITY>;
           status = "okay";
           /delete-property/ execution-memory;
         };

         / {
           reserved-memory {
             #address-cells = <1>;
             #size-cells = <1>;
             ranges;

             softperiph_ram: memory@2007B400 {
               reg = <0x2007B400 0x4400>;
               ranges = <0 0x2007B400 0x4400>;
               #address-cells = <1>;
               #size-cells = <1>;

               dut: scan: scan@4200 {
                 #address-cells = <1>;
                 #size-cells = <0>;
                 reg = <0x4200 0x200>;
                 zephyr,pm-device-runtime-auto;
               };
             };
           };
         };

Add the following code to your application to apply the pin configuration from the sCAN board overlay:

.. code-block:: c

   // Apply the FLPR pin configuration.

   uint32_t io[2] = {
       NRF_PIN_PORT_TO_PIN_NUMBER(2, 2), // TX
       NRF_PIN_PORT_TO_PIN_NUMBER(4, 2), // RX
   };
   for (int i = 0; i < 2; i++) {
     nrf_gpio_pin_control_select(io[i], NRF_GPIO_PIN_SEL_VPR);
   }
   nrf_gpio_cfg(io[0], NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT,
                NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);
   nrf_gpio_cfg(io[1], NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT,
                NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);


Memory retention configuration
******************************

The sCAN soft peripheral requires RAM retention to enter its lowest-power mode through the :c:func:`nrf_scan_disable` function.

If the application has access to the ``MEMCONF`` peripheral, use the following sequence to enable FLPR RAM retention, disable and re-enable sCAN, and then disable RAM retention:

.. code-block:: c

  // Disable sequence
  nrf_memconf_ramblock_ret_enable_set(NRF_MEMCONF, 1, MEMCONF_POWER_RET_MEM0_Pos, true);
  nrf_scan_disable(&m_scan);
  // Enable sequence
  nrf_scan_enable(&m_scan);
  nrf_memconf_ramblock_ret_enable_set(NRF_MEMCONF, 1, MEMCONF_POWER_RET_MEM0_Pos, false);

.. _scan_nrf54L_series_porting_guide_ram_configuration:

RAM configuration
*****************

The sCAN soft peripheral operates from RAM.

.. note::
   sCAN supports position-independent code (PIC), allowing the application to select where to load the soft peripheral firmware.

Your build environment must reserve the required RAM and ensure that it is readable and writable by both the application core and the FLPR core.
The following tables describe the memory regions required for each supported nRF54L Series device:

.. tabs::

  .. tab:: **nRF54L15**

      .. list-table:: nRF54L15 RAM Configuration Table
        :widths: auto
        :header-rows: 1

        * - Component
          - Address offset
          - Size
        * - sCAN firmware
          - ``SP_FIRMWARE_ADDR``
          - 0x3C00
        * - sCAN execution RAM
          - ``SP_FIRMWARE_ADDR`` + 0x3C00
          - 0x600
        * - sCAN virtual register interface
          - ``SP_FIRMWARE_ADDR`` + 0x4200
          - 0x200
        * - Context saving
          - 0x2003FE00
          - 0x200 (but the entire block should be retained)

  .. tab:: **nRF54LM20**

      .. list-table:: nRF54LM20 RAM Configuration Table
        :widths: auto
        :header-rows: 1

        * - Component
          - Address offset
          - Size
        * - sCAN firmware
          - ``SP_FIRMWARE_ADDR``
          - 0x3C00
        * - sCAN execution RAM
          - ``SP_FIRMWARE_ADDR`` + 0x3C00
          - 0x600
        * - sCAN virtual register interface
          - ``SP_FIRMWARE_ADDR`` + 0x4200
          - 0x200
        * - Context saving
          - 0x2007FD40
          - 0x200 (but the entire block should be retained)

The build environment described in the :ref:`scan_nrf54l_series_porting_guide_code` section must comply with these requirements.
This includes appropriate linker script settings, devicetree specifications (DTS), and resource allocation.

Define the virtual register interface address manually as follows:

.. code-block:: c

   #define SP_REGIF_BASE (SP_FIRMWARE_ADDR + 0x4200)

Alternatively, obtain the firmware and virtual register interface addresses from the devicetree:

.. code-block:: c

   #define SP_FIRMWARE_ADDR DT_REG_ADDR(DT_NODELABEL(softperiph_ram))
   #define SP_REGIF_BASE DT_REG_ADDR(DT_NODELABEL(scan))

Initialize the sCAN instance with the virtual register interface at ``SP_REGIF_BASE``:

.. code-block:: c

   static nrf_scan_t m_scan = {.p_reg = (void *)SP_REGIF_BASE, .drv_inst_idx = 0};

Set ``SP_FIRMWARE_ADDR`` so that the allocated memory does not overlap the context-saving address.
On nRF54L Series devices, the firmware is placed before this address.
Therefore, the end of the memory regions listed in the tables must be lower than the context-saving address.

The following ``SP_FIRMWARE_ADDR`` values have been tested and are considered production-ready:

.. list-table:: sCAN firmware address values
   :widths: 15 20
   :header-rows: 1

   * - Device
     - ``SP_FIRMWARE_ADDR``
   * - nRF54L15
     - 0x2003B400
   * - nRF54LM20
     - 0x2007B400


IRQ connection
**************

Register the sCAN IRQ handler so that sCAN can communicate with the application core.
The following code connects the FLPR interrupt to the handler:

.. code-block:: c

   #define VPR_NODE DT_NODELABEL(cpuflpr_vpr)

   IRQ_CONNECT(DT_IRQN(VPR_NODE), DT_IRQ(VPR_NODE, priority), nrfx_isr, nrf_scan_irq_handler, 0);
