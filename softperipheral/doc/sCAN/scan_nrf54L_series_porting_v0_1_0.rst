.. _scan_nrf54L_series_porting_guide:

sCAN nRF54L Series porting guide
################################

.. contents::
   :local:
   :depth: 2

This page provides a comprehensive overview of the code structure, file hierarchy, and essential configurations and requirements needed to successfully port and implement an sCAN application on an nRF54L Series device.

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
          │   │   └── scan_firmware_v0.1.0.h
          │   │   └── ...
          │   ├── nrf_config_scan.h
          │   ├── nrf_sp_can.h
          │   ├── nrf_scan.h
          └── src
              └── nrf_scan.c

.. note::
   The main interface for sCAN is in the :file:`nrf_scan.h` file.

Header files
============

sCAN application requires specific header files used by the driver code.
You must include paths to these files in the build environment's list of include paths.
The following list is a detailed breakdown of the necessary paths:

* Repository internal paths:

  * :file:`softperipheral/include` - Soft peripherals register interface and metadata
  * :file:`softperipheral/sCAN/include` - sCAN register interface and driver header
  * :file:`softperipheral/sCAN/include/nrf54l` - The sCAN firmware for the Fast Lightweight Perpipheral Processor (FLPR)

To override the configuration enums in :file:`nrf_config_scan.h`, use the ``zephyr_compile_definitions`` macro in your application:

.. code-block:: c

   // Example call for enabling the sCAN
   zephyr_compile_definitions("NRF_SCAN_ENABLED=1")

Compiling source files
======================

For a sCAN application to function properly, you must compile the driver implementation from the source file :file:`nrf_scan.c`.

Application core and FLPR configuration
***************************************

You must adjust the settings for the nRF54L Series SoC to run at highest base clock frequency.

To work with any of the following settings, ensure you have allocated memory for the data pointers used by the sCAN driver.
The memory is independent from the one outlined in the :ref:`scan_nrf54L_series_porting_guide_ram_configuration` subsection.
It is designated to function as shared memory for communication purposes, rather than containing the sCAN executable code.

Security configuration
======================

Configure the security settings for the nRF54L Series device based on the operational requirements of your application.

Secure environment
------------------

In a secure configuration, both the application core and the FLPR core of the nRF54L Series device must operate within a secure environment enabled by TrustZone Secure.

The following example code snippet shows how the application code can configure permissions for the FLPR instance:

.. code-block:: c

  // NOTE: Setting FLPR to secure
  NRF_SPU00_S->PERIPH[0xC].PERM = (SPU_PERIPH_PERM_SECATTR_Secure << SPU_PERIPH_PERM_SECATTR_Pos);

Non-secure environment
----------------------

In a non-secure configuration, both the application core and the FLPR core of the nRF54L Series device must operate outside the secure environment (without TrustZone Secure).

.. _scan_nrf54l_series_porting_guide_gpio_config:

GPIO configuration
******************

The provided table details the configuration for each GPIO pin used in the sCAN application.
The following options are available, assuming that the FLPR core has access to these ports and pins:

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

Any conflicting peripherals and existing memory partitions on the same port as sCAN must be disabled in the devicetree overlay, depending on your requirements.
See an example code snippet of a devicetree overlay that grants FLPR access to the necessary pins.

The first part of the snippet, the ``pinctrl`` block, configures the relevant GPIOs as described in the :ref:`scan_nrf54l_series_porting_guide_gpio_config` section.

Next, the ``cpuflpr_vpr`` block and the subsequent block handle the allocation of the VPR memory region and the RAM used by sCAN.

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

             softperiph_ram: memory@2003B400 {
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

You will need the following lines in your application to apply the pin configuration from your sCAN board overlay, as well as to initialize the Soft Peripheral:

.. code-block:: c

   //The following lines in your setup functions apply the FLPR pins to PINCTRL:

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

The sCAN soft peripheral requires RAM retention in order to go into the lowest power consumption mode, which can be called through the :c:func:`nrf_scan_disable` function.

Assuming there is an access to the peripheral ``MEMCONF``, the following code snippet illustrates how to enable FLPR RAM retention, followed by disabling and re-enabling, and finally how to disable RAM retention:

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

The sCAN Soft Peripheral operates from RAM.

.. note::
   sCAN supports Position Independent Code (PIC), which allows an application to determine where to load the Soft Peripheral firmware.

Your build environment must reserve the required RAM and ensure that it is readable and writable by both the application core and the FLPR core.
The following table details the memory regions required for your nRF54L Series device:

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
This includes proper settings in linker scripts, device tree specifications (DTS), and resource allocation.

To initialize the sCAN struct on an nRF54L Series device, use the following lines:

 .. code-block:: c

    #define SP_REGIF_BASE (SP_FIRMWARE_ADDR + 0x4200)
    //or, from DTS: 
    #define SP_FIRMWARE_ADDR DT_REG_ADDR(DT_NODELABEL(softperiph_ram))
    #define SP_REGIF_BASE DT_REG_ADDR(DT_NODELABEL(scan))

    // Initialize sCAN with the virtual register interface placed at SP_REGIF_BASE
    static nrf_scan_t m_scan = {.p_reg = (void *)SP_REGIF_BASE, .drv_inst_idx = 0};

Ensure that ``SP_FIRMWARE_ADDR`` is set so that it does not overlap with the context saving address.
For nRF54L Series devices, the firmware is placed before the context saving address.
This means, that ``SP_FIRMWARE_ADDR`` plus the component sizes listed in the table must be of a lower value than the context saving address.

The values in the table below have been tested and are considered production-ready for ``SP_FIRMWARE_ADDR``:

.. list-table:: sCAN firmware address values
   :widths: 15 20
   :header-rows: 1

   * - Device
     - `SP_FIRMWARE_ADDR`
   * - nRF54L15
     - 0x2003B400
   * - nRF54LM20
     - 0x2007B400


IRQ connection
**************

For sCAN to communicate with the application core, the sCAN IRQ handler must be registered.
The following code line registers the IRQ handler to FLPR:

.. code-block:: c

   IRQ_CONNECT(DT_IRQN(VPR_NODE), DT_IRQ(VPR_NODE, priority), nrfx_isr, nrf_scan_irq_handler, 0);
