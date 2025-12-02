.. _sqspi_nrf54H_series_porting_guide:

sQSPI nRF54H Series porting guide
#################################

.. contents::
   :local:
   :depth: 2

This page provides a comprehensive overview of the code structure, file hierarchy, and essential configurations and requirements needed to successfully port and implement an sQSPI application on an nRF54H Series device.

.. _nrf54h_series_porting_guide_code:

sQSPI application code
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
      └── sQSPI
          ├── include
          │   ├── hal
          │   │   └── nrf_qspi2.h
          │   ├── nrf54h
          │   │   ├── sqspi_firmware.h
          │   │   ├── sqspi_firmware_v1.2.1.h
          │   │   └── ...
          │   ├── nrf_config_sqspi.h
          │   ├── nrf_sp_qspi.h
          │   ├── nrf_sqspi.h
          └── src
              └── nrf_sqspi.c

.. note::
   The main interface for sQSPI is the :file:`nrf_sqspi.h` file.

Header files
============

For an sQSPI application to function correctly, it requires specific header files used by the driver code.
You must include paths to these files in the build environment's list of include paths.
The following list is a detailed breakdown of the necessary paths:

* Repository internal paths:

  * :file:`softperipheral/include` - Soft peripherals register interface and metadata
  * :file:`softperipheral/sQSPI/include` - sQSPI register interface, driver header and the configuration header :file:`nrf_config_sqspi.h`
  * :file:`softperipheral/sQSPI/include/nrf54h` - The sQSPI firmware for the Fast Lightweight Perpipheral Processor (FLPR)

To override the configuration enums in :file:`nrf_config_sqspi.h`, you can use the ``zephyr_compile_definitions`` macro in your application:

.. code-block:: c

   // Example call for enabling the sQSPI
   zephyr_compile_definitions("NRF_SQSPI_ENABLED=1")


Compiling source files
======================

For an sQSPI application to function properly, you must compile the driver implementation from the source file :file:`nrf_sqspi.c`.

Application core and FLPR configuration
***************************************

You must adjust the settings for the nRF54H Series SoC to run at highest base clock frequency.

To work with any of the following settings, ensure you have allocated memory for the data pointers used by the sQSPI driver.
The memory is independent from the one outlined in the :ref:`nrf54H_series_porting_guide_ram_configuration` subsection.
It is designated to function as shared memory for communication purposes, rather than containing the sQSPI executable code.

Security configuration
======================

Configure the security settings for the nRF54H Series device based on the operational requirements of your application.

Secure environment
------------------

In a secure configuration, both the application core and the FLPR core of the nRF54H Series device must operate within a secure environment enabled by TrustZone Secure.

Non-secure environment
----------------------

In a non-secure configuration, both the application core and the FLPR core of the nRF54H Series device must operate outside the secure environment (without TrustZone Secure).

.. _sqspi_nrf54h_series_porting_guide_gpio_config:

GPIO configuration
******************

The provided table details the configuration for each GPIO pin used in the sQSPI application.
The following options are available, assuming that the FLPR core has access to these ports and pins:

.. tabs::

   .. tab:: **P7** GPIO pins

    .. list-table::
      :widths: 10 10 20 20 20
      :header-rows: 1

      * - GPIO pin
        - sQSPI role
        - Direction config
        - Input config
        - Pin pull config
      * - **P7.0**
        - SCK
        - Output
        - Disconnected
        - No pull
      * - **P7.1**
        - IO0
        - Output
        - Connected
        - Pull-up
      * - **P7.2**
        - IO1
        - Output
        - Connected
        - Pull-up
      * - **P7.3**
        - IO2
        - Output
        - Connected
        - Pull-up
      * - **P7.4**
        - IO3
        - Output
        - Connected
        - Pull-up
      * - **P7.5**
        - CSN
        - Output
        - Disconnected
        - No pull

   .. tab:: **P9** GPIO pins

    .. list-table::
      :widths: 10 10 20 20 20
      :header-rows: 1

      * - GPIO pin
        - sQSPI role
        - Direction config
        - Input config
        - Pin pull config
      * - **P9.0**
        - SCK
        - Output
        - Disconnected
        - No pull
      * - **P9.1**
        - IO0
        - Output
        - Connected
        - Pull-up
      * - **P9.2**
        - IO1
        - Output
        - Connected
        - Pull-up
      * - **P9.3**
        - IO2
        - Output
        - Connected
        - Pull-up
      * - **P9.4**
        - IO3
        - Output
        - Connected
        - Pull-up
      * - **P9.5**
        - CSN
        - Output
        - Disconnected
        - No pull

.. note::
   sQSPI driver provides a default GPIO configuration and multiplexing.
   You can apply this setup by setting :c:var:`nrf_sqspi_cfg_t.skip_gpio_cfg` and :c:var:`nrf_sqspi_cfg_t.skip_pmux_cfg` to ``false``.

Configuring pins
================

In some cases, you might have to modify the sQSPI driver configuration.
For example, you might need to change the pin drive strength to guarantee signal integrity for a new PCB design.

.. note::
   When using a custom PCB with the nRF54H20 device you might need to match the output impedance on the port you are using.
   Refer to the :ref:`nrf:ug_nrf54h20_custom_pcb` page for details.

You must address these cases on the sQSPI application code:

* If you set the :c:var:`nrf_sqspi_cfg_t.skip_gpio_cfg` variable to ``true``, the GPIO configuration is not managed by the sQSPI driver and it must be manually handled by the application.
  This is a requirement for the nRF54H20 device.
* If you set the :c:var:`nrf_sqspi_cfg_t.skip_pmux_cfg` variable to ``true``, the GPIO multiplexing is not managed by the sQSPI driver and it must be manually handled by the application.
  This is a requirement for the nRF54H20 device.

GPIO multiplexing must be handled by setting the correct ``CTRLSEL`` value in UICR.

Additionally, the sQSPI application requires FastPad bias configuration to run at high frequency on SCLK.

Any conflicting peripherals and existing memory partitions on the same port as sQSPI must be disabled in the devicetree overlay, depending on your requirements.
See an example code snippet of a devicetree overlay that grants FLPR access to the **P7** port.

The first part of the snippet, the ``pinctrl`` block, configures the relevant GPIOs as described in the :ref:`sqspi_nrf54h_series_porting_guide_gpio_config` section.

Next, the ``cpuflpr_vpr`` block and the subsequent block handle the allocation of the VPR memory region and the RAM used by sQSPI.

.. code-block:: dts

   &pinctrl {
     sqspi_default: sqspi_default {
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

     sqspi_sleep: sqspi_sleep {
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
     pinctrl-0 = <&sqspi_default>;
     pinctrl-1 = <&sqspi_sleep>;
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
         reg = <0x2f890000 0x4000>;
         ranges = <0 0x2f890000 0x4000>;
         #address-cells = <1>;
         #size-cells = <1>;

         dut: sqspi: sqspi@3d40 {
           compatible = "nordic,nrf-sqspi";
           #address-cells = <1>;
           #size-cells = <0>;
           reg = <0x3d40 0x200>;
           zephyr,pm-device-runtime-auto;
           memory-regions = <&sqspi_buffers>;
         };
       };

       sqspi_buffers: memory@2f894000 {
         compatible = "zephyr,memory-region";
         reg = <0x2f894000 0x4000>;
         #memory-region-cells = <0>;
         zephyr,memory-region = "SQSPI_BUFFERS";
         zephyr,memory-attr = <DT_MEM_CACHEABLE>;
       };
     };
   };


You will need the following lines in your application to apply the pin configuration from your sQSPI board DTS overlay, as well as to initialize the Soft Peripheral:

.. code-block:: c

   #define VPR_NODE DT_NODELABEL(cpuflpr_vpr)
   #define SP_REGIF_BASE DT_REG_ADDR(DT_NODELABEL(sqspi))

   // The following lines in your setup function will apply the FLPR pins to PINCTRL
   PINCTRL_DT_DEFINE(VPR_NODE);
   int rc = pinctrl_apply_state(PINCTRL_DT_DEV_CONFIG_GET(VPR_NODE), PINCTRL_STATE_DEFAULT);
   zassert_true(rc >= 0, "Pins not configured properly! Returned with code: 0x&X\t\n", rc);

   // The following line initializes sQSPI
   static nrf_sqpsi_t m_sqspi = {.p_reg = (void *)SP_REGIF_BASE, .drv_inst_idx = 0};

.. _nrf54h20_porting_guide_ram_configuration:
.. _nrf54H_series_porting_guide_ram_configuration:

RAM configuration
*****************

The sQSPI Soft Peripheral operates from RAM.

.. note::
   Starting from sQSPI 1.0.0, sQSPI supports Position Independent Code (PIC), which allows an application to determine where to load the Soft Peripheral firmware.

Your build environment must reserve the required RAM and ensure that it is readable and writable by both the application core and the FLPR core.
The following table details the memory region, which should be non-cacheable:

.. list-table:: RAM Configuration Table
   :widths: auto
   :header-rows: 1

   * - Component
     - Address offset
     - Size
   * - sQSPI firmware
     - ``SP_FIRMWARE_ADDR``
     - 0x3740
   * - sQSPI execution RAM
     - ``SP_FIRMWARE_ADDR`` + 0x3740
     - 0x400
   * - sQSPI virtual register interface
     - ``SP_FIRMWARE_ADDR`` + 0x3B40
     - 0x200
   * - Context saving
     - 0x2f890000
     - 0x200 (but the entire block should be retained)

The build environment described in the :ref:`nrf54h_series_porting_guide_code` section, you must comply with these requirements.
This includes proper settings in linker scripts, device tree specifications (DTS), and resource allocation.

Ensure that ``SP_FIRMWARE_ADDR`` is set so that it does not overlap with the context saving address.
For nRF54H Series devices, the firmware has been tested using values that follow the context saving address.
This means, that ``SP_FIRMWARE_ADDR`` should be set higher than the context saving address plus the context saving offset.
The value ``0x2f890200`` for ``SP_FIRMWARE_ADDR`` has been tested and is considered production-ready.

IRQ connection
**************

For sQSPI to communicate with the application core, the sQSPI IRQ handler must be registered.
The following code line registers the IRQ handler to FLPR:

.. code-block:: c

   IRQ_CONNECT(DT_IRQN(VPR_NODE), DT_IRQ(VPR_NODE, priority), nrfx_isr, nrf_sqspi_irq_handler, 0);
