.. _semmc_nrf54H_series_porting_guide:

sEMMC nRF54H Series porting guide
#################################

.. contents::
   :local:
   :depth: 2

This page provides a comprehensive overview of the code structure, file hierarchy, and essential configurations and requirements needed to successfully port and implement an sEMMC application on an nRF54H Series device.

.. _semmc_nrf54h_series_porting_guide_code:

sEMMC application code
**********************

This structure shows the relevant files and directories in the `sdk-nrfxlib`_ repository:

.. code-block::

   nrfxlib/
   ├──...
   └── softperipheral/
      ├── include
      │   ├── softperipheral_meta.h
      │   └── softperipheral_regif.h
      ├── ...
      └── sEMMC
          ├── include
          │   ├── hal
          │   │   └── nrf_emmc.h
          │   ├── nrf54h
          │   │   ├── semmc_firmware.h
          │   │   └── semmc_firmware_v0.1.0.h
          │   │   └── ...
          │   ├── nrf_config_semmc.h
          │   ├── nrf_sp_emmc.h
          │   ├── nrf_semmc.h
          └── src
              └── nrf_semmc.c

.. note::
   The main interface for sEMMC is in the :file:`nrf_semmc.h` file.
   It contains a list of supported and tested EMMC commands.

Header files
============

sEMMC application requires specific header files used by the driver code.
You must include paths to these files in the build environment's list of include paths.
The following list is a detailed breakdown of the necessary paths:

* Repository internal paths:

  * :file:`softperipheral/include` - Soft peripherals register interface and metadata
  * :file:`softperipheral/sEMMC/include` - sEMMC register interface and driver header
  * :file:`softperipheral/sEMMC/include/nrf54h` - The sEMMC firmware for the Fast Lightweight Perpipheral Processor (FLPR)

To override the configuration enums in :file:`nrf_config_semmc.h`, use the ``zephyr_compile_definitions`` macro in your application:

.. code-block:: c

   // Example call for enabling the sEMMC
   zephyr_compile_definitions("NRF_SEMMC_ENABLED=1")

Compiling source files
======================

For a sEMMC application to function properly, you must compile the driver implementation from the source file :file:`nrf_semmc.c`.

Application core and FLPR configuration
***************************************

You must adjust the settings for the nRF54H Series SoC to run at highest base clock frequency.

To work with any of the following settings, ensure you have allocated memory for the data pointers used by the sEMMC driver.
The memory is independent from the one outlined in the :ref:`semmc_nrf54H_series_porting_guide_ram_configuration` subsection.
It is designated to function as shared memory for communication purposes, rather than containing the sEMMC executable code.

Security configuration
======================

Configure the security settings for the nRF54H Series device based on the operational requirements of your application.

Secure environment
------------------

In a secure configuration, both the application core and the FLPR core of the nRF54H Series device must operate within a secure environment enabled by TrustZone Secure.

Non-secure environment
----------------------

In a non-secure configuration, both the application core and the FLPR core of the nRF54H Series device must operate outside the secure environment (without TrustZone Secure).

.. _semmc_nrf54h_series_porting_guide_gpio_config:

GPIO configuration
******************

The provided table details the configuration for each GPIO pin used in the sEMMC application.
The following options are available, assuming that the FLPR core has access to these ports and pins:

 .. list-table::
   :widths: 10 10 20 20 20
   :header-rows: 1

   * - GPIO pin
     - sEMMC role
     - Direction config
     - Input config
     - Pin pull config
   * - **P7.0**
     - D3
     - Output
     - Disconnected
     - Pull-up
   * - **P7.1**
     - CLK
     - Output
     - Disconnected
     - Pull-down
   * - **P7.2**
     - D0
     - Output
     - Disconnected
     - Pull-up
   * - **P7.3**
     - D2
     - Output
     - Disconnected
     - Pull-up
   * - **P7.4**
     - D1
     - Output
     - Disconnected
     - Pull-up
   * - **P7.5**
     - CMMD
     - Output
     - Disconnected
     - Pull-up

Configuring pins
================

In some cases, you might have to modify the sEMMC driver configuration.
For example, you might need to change the pin drive strength to guarantee signal integrity for a new PCB design.
You must address these cases on the sEMMC application code.

.. note::
   When using a custom PCB with the nRF54H20 device you might need to match the output impedance on the port you are using.
   Refer to the :ref:`nrf:ug_nrf54h20_custom_pcb` page for details.

GPIO multiplexing must be handled by setting the correct ``CTRLSEL`` value in UICR.

Any conflicting peripherals and existing memory partitions on the same port as sEMMC must be disabled in the devicetree overlay, depending on your requirements.
See an example code snippet of a devicetree overlay that grants FLPR access to the necessary pins.

The first part of the snippet, the ``pinctrl`` block, configures the relevant GPIOs as described in the :ref:`semmc_nrf54h_series_porting_guide_gpio_config` section.

Next, the ``cpuflpr_vpr`` block and the subsequent block handle the allocation of the VPR memory region and the RAM used by sEMMC.

.. code-block:: dts

   &pinctrl {
     semmc_default: semmc_default {
       group1 {
         psels = <NRF_PSEL(SDP_MSPI_SCK, 7, 0)>;
         nordic,drive-mode = <NRF_DRIVE_E0E1>;
         bias-pull-down;
       };
       group2 {
         psels = <NRF_PSEL(SDP_MSPI_DQ0, 7, 1)>,
           <NRF_PSEL(SDP_MSPI_DQ1, 7, 2)>,
           <NRF_PSEL(SDP_MSPI_DQ2, 7, 3)>,
           <NRF_PSEL(SDP_MSPI_DQ3, 7, 4)>,
           <NRF_PSEL(SDP_MSPI_CS0, 7, 5)>,
           <NRF_PSEL(SDP_MSPI_CS1, 7, 6)>;
         nordic,drive-mode = <NRF_DRIVE_E0E1>;
         bias-pull-up;
       };
     };
     semmc_sleep: semmc_sleep {
       group1 {
         low-power-enable;
         psels = <NRF_PSEL(SDP_MSPI_SCK, 7, 0)>,
                 <NRF_PSEL(SDP_MSPI_DQ0, 7, 1)>,
                 <NRF_PSEL(SDP_MSPI_DQ1, 7, 2)>,
                 <NRF_PSEL(SDP_MSPI_DQ2, 7, 3)>,
                 <NRF_PSEL(SDP_MSPI_DQ3, 7, 4)>,
                 <NRF_PSEL(SDP_MSPI_CS0, 7, 5)>,
                 <NRF_PSEL(SDP_MSPI_CS1, 7, 6)>;
       };
     };
   };

   &cpuflpr_vpr {
     pinctrl-0 = <&semmc_default>;
     pinctrl-1 = <&semmc_sleep>;
     pinctrl-names = "default";
     interrupts = <212 NRF_DEFAULT_IRQ_PRIORITY>;
     status = "okay";
     /delete-property/ execution-memory;
   };

   /delete-node/ &cpuflpr_code_data;
   /delete-node/ &cpuapp_cpuflpr_ipc_shm;
   /delete-node/ &cpuflpr_cpuapp_ipc_shm;
   /delete-node/ &cpuapp_cpuflpr_ipc;
   / {
     reserved-memory {
       softperiph_ram: memory@2f890000 {
         reg = <0x2f890000 0x4140>;
         ranges = <0 0x2f890000 0x4140>;
         #address-cells = <1>;
         #size-cells = <1>;

         dut: semmc: semmc@3f40 {
           #address-cells = <1>;
           #size-cells = <0>;
           reg = <0x3f40 0x200>;
           zephyr,pm-device-runtime-auto;
           memory-regions = <&semmc_buffers>;
         };
       };

       semmc_buffers: memory@2f894140 {
         compatible = "zephyr,memory-region";
         reg = <0x2f894140 0x4000>;
         #memory-region-cells = <0>;
         zephyr,memory-region = "SEMMC_BUFFERS";
         zephyr,memory-attr = <DT_MEM_ARM(ATTR_MPU_RAM_NOCACHE)>;
       };
     };
   };

You will need the following lines in your application to apply the pin configuration from your sEMMC board DTS overlay, as well as to initialize the Soft Peripheral:

.. code-block:: c

   #define VPR_NODE DT_NODELABEL(cpuflpr_vpr)
   #define SP_REGIF_BASE DT_REG_ADDR(DT_NODELABEL(semmc))

   // The following lines in your setup function will apply the FLPR pins to PINCTRL
   PINCTRL_DT_DEFINE(VPR_NODE);
   int rc = pinctrl_apply_state(PINCTRL_DT_DEV_CONFIG_GET(VPR_NODE), PINCTRL_STATE_DEFAULT);
   zassert_true(rc >= 0, "Pins not configured properly! Returned with code: 0x&X\t\n", rc);

   // The following line initializes sEMMC
   static nrf_semmc_t m_semmc = {.p_reg = (void *)SP_REGIF_BASE, .drv_inst_idx = 0};


.. _semmc_nrf54H_series_porting_guide_ram_configuration:

RAM configuration
*****************

The sEMMC Soft Peripheral operates from RAM.

.. note::
   sEMMC supports Position Independent Code (PIC), which allows an application to determine where to load the Soft Peripheral firmware.

Your build environment must reserve the required RAM and ensure that it is readable and writable by both the application core and the FLPR core.
The following table details the memory region, which should be non-cacheable:

.. list-table:: RAM Configuration Table
   :widths: auto
   :header-rows: 1

   * - Component
     - Address offset
     - Size
   * - sEMMC firmware
     - ``SP_FIRMWARE_ADDR``
     - 0x3740
   * - sEMMC execution RAM
     - ``SP_FIRMWARE_ADDR`` + 0x3740
     - 0x600
   * - sEMMC virtual register interface
     - ``SP_FIRMWARE_ADDR`` + 0x3D40
     - 0x200
   * - Context saving
     - 0x2f890000
     - 0x200 (but the entire block should be retained)

The build environment described in the :ref:`semmc_nrf54h_series_porting_guide_code` section, you must comply with these requirements.
This includes proper settings in linker scripts, device tree specifications (DTS), and resource allocation.

Ensure that ``SP_FIRMWARE_ADDR`` is set so that it does not overlap with the context saving address.
For nRF54H Series devices, the firmware has been tested using values that follow the context saving address.
This means, that ``SP_FIRMWARE_ADDR`` should be set higher than the context saving address plus the context saving offset.
The value ``0x2f890200`` for ``SP_FIRMWARE_ADDR`` has been tested and is considered production-ready.


IRQ connection
**************

For sEMMC to communicate with the application core, the sEMMC IRQ handler must be registered.
The following code line registers the IRQ handler to FLPR:

.. code-block:: c

   IRQ_CONNECT(DT_IRQN(VPR_NODE), DT_IRQ(VPR_NODE, priority), nrfx_isr, nrf_semmc_irq_handler, 0);


.. _semmc_nrf54H_series_porting_guide_read_response:

Read responses
**************

sEMMC cannot process read data and response at the same time (see the :ref:`semmc_limitations`).
However, you can mitigate it in the following ways:

* The recommended way is to perform the read operation with the :c:var:`nrf_semmc_config_t.process_response` variable set to ``NRF_EMMC_RESPONSE_PROC_PROCESS``.
  The response is available in the :c:var:`nrf_semmc_cmd_desc_t.err` variable.
  If the response is ``NRF_SEMMC_SUCCESS``, the read was successful.
  If you receive a different response, retry the read operation, this time with the :c:var:`nrf_semmc_config_t.process_response` variable set to ``NRF_EMMC_RESPONSE_PROC_IGNORE``.

* Alternatively, you can perform the read operation with the :c:var:`nrf_semmc_config_t.process_response` variable set to ``NRF_EMMC_RESPONSE_PROC_IGNORE``, and then follow it with a CMD13 command to verify if the status is correct.
