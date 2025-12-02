.. _sqspi_nrf54L_series_porting_guide:

sQSPI nRF54L Series porting guide
#################################

.. contents::
   :local:
   :depth: 2

This page provides a comprehensive overview of the code structure, file hierarchy, and essential configurations and requirements needed to successfully port and implement an sQSPI application on nRF54L Series devices.

.. _nrf54l_series_porting_guide_code:

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
          │   ├── nrf54l
          │   │   ├── sqspi_firmware.h
          │   │   └── sqspi_firmware_v1.2.1.h
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

sQSPI application requires specific header files used by the driver code (internal paths) and specific header files for nrfx (external dependencies).
You must include paths to these files in the build environment's list of include paths.
The following list is a detailed breakdown of the necessary paths:

* Repository internal paths:

  * :file:`softperipheral/include` - Soft peripherals register interface and metadata
  * :file:`softperipheral/sQSPI/include` - sQSPI register interface and driver header
  * :file:`softperipheral/sQSPI/include/nrf54l` - The sQSPI firmware for the Fast Lightweight Perpipheral Processor (FLPR)

* External dependencies:

   * :file:`nrfx` - Includes the main nrfx header file (:file:`nrfx.h`) and hal, which is sometimes necessary for additional GPIO configuration.
   * :file:`nrfx/drivers/include` - Includes the nrfx driver header files.
     For example, the :file:`nrfx_dppi.h` file.
   * Path to the application-specific headers and macros.
     For example, the :file:`nrfx_glue.h`, :file:`nrfx_log.h`, or a configuration header file.
     See an example configuration for the :file:`nrfx_config.h` file:

.. code-block:: c

  #ifndef NRFX_CONFIG_H__
  #define NRFX_CONFIG_H__

  #include "softperipheral_regif.h" // To Resolve correct VPR IRQn for the SoC.
  #define nrf_sqspi_irq_handler        SP_VPR_IRQHandler

  #define NRF_SQSPI_ENABLED            (1)
  #define NRF_SQSPI_MAX_NUM_DATA_LINES (4)


  #endif // NRFX_CONFIG_H__


Compiling source files
======================

For an sQSPI application to function properly, you must compile the driver implementation from the source file :file:`nrf_sqspi.c`.

Application core and FLPR configuration
***************************************

You must adjust the settings for the nRF54L Series SoC to run at highest base clock frequency.
To work with any of the following settings, ensure you have completed the following:

* You have allocated memory for the data pointers used by the sQSPI driver.
  The memory is independent from the one outlined in the :ref:`nrf54L_series_porting_guide_ram_configuration` subsection.
  It is designated to function as shared memory for communication purposes, rather than containing the sQSPI executable code.

* You have enabled the FLPR access to the ``MEMCONF`` peripheral.
  This access is necessary to properly deactivate sQSPI.

Security configuration
======================

Configure the security settings for the nRF54L Series device based on the operational requirements of your application.

Secure environment
------------------

In a secure configuration, both the application core and the FLPR core of the nRF54L Series device must operate within a secure environment enabled by TrustZone Secure.

The following code snippet shows how the application code can configure permissions for the FLPR instance:

.. code-block:: c

  // NOTE: Setting FLPR to secure
  NRF_SPU00_S->PERIPH[0xC].PERM = (SPU_PERIPH_PERM_SECATTR_Secure << SPU_PERIPH_PERM_SECATTR_Pos);

Non-secure environment
----------------------

In a non-secure configuration, both the application core and the FLPR core of the nRF54L Series device must operate outside the secure environment (without TrustZone Secure).

GPIO configuration
******************

The provided table details the configuration for each GPIO pin used in the sQSPI application.
The following options are available, assuming that the FLPR core has access to these ports and pins:

.. list-table::
   :widths: 10 10 20 20 20
   :header-rows: 1

   * - GPIO pin
     - sQSPI role
     - Direction config
     - Input config
     - Pin pull config
   * - **P2.0**
     - IO3
     - Output
     - Connected
     - Pull-up
   * - **P2.1**
     - SCK
     - Output
     - Disconnected
     - No pull
   * - **P2.2**
     - IO0
     - Output
     - Connected
     - Pull-up
   * - **P2.3**
     - IO2
     - Output
     - Connected
     - Pull-up
   * - **P2.4**
     - IO1
     - Output
     - Connected
     - Pull-up
   * - **P2.5**
     - CSN
     - Output
     - Disconnected
     - No pull

.. note::
   sQSPI driver provides a default GPIO configuration and multiplexing. You can apply this setup by setting :c:var:`nrf_sqspi_cfg_t.skip_gpio_cfg` and :c:var:`nrf_sqspi_cfg_t.skip_pmux_cfg` to ``false``.

Configuring pins
================

In some cases you might have to modify the sQSPI driver configuration.
For example, when changing pin drive strength to guarantee signal integrity for a new PCB design.
You must address these cases on the sQSPI application code:

* If you set the :c:var:`nrf_sqspi_cfg_t.skip_gpio_cfg` variable to ``true``, the GPIO configuration is not managed by the sQSPI driver and it must be manually handled by the application.
* If you set the :c:var:`nrf_sqspi_cfg_t.skip_pmux_cfg` variable to ``true``, the GPIO multiplexing is not managed by the sQSPI driver and it must be manually handled by the application.

The following code snippet shows how the application code can allocate the required pins and override the sQSPI driver's default configuration:

.. code-block:: c

   //qspi pin config
   static nrf_sqspi_cfg_t qspi_config = {
       .pins =
           {
               .sck = NRF_PIN_PORT_TO_PIN_NUMBER(1, 2),
               .strobe = NRF_SQSPI_PINS_UNUSED,
               .io =
                   {
                       NRF_PIN_PORT_TO_PIN_NUMBER(2, 2),
                       NRF_PIN_PORT_TO_PIN_NUMBER(4, 2),
                       NRF_PIN_PORT_TO_PIN_NUMBER(3, 2),
                       NRF_PIN_PORT_TO_PIN_NUMBER(0, 2),
                   },
           },
       .skip_gpio_cfg = false,
       .skip_pmux_cfg = false,
   };

    nrf_sqspi_dev_cfg_t qspi_dev_config = {.csn_pin = NRF_PIN_PORT_TO_PIN_NUMBER(5, 2),
                                            .sck_freq_khz = ...,
                                            ...
                                            };

   nrf_sqspi_init(&qspi, &qspi_config);

   if (qspi_config.skip_gpio_cfg == false) { //This overrides the driver's default configuration
       // Set drive strength E0E1, as it is non-standard.
       nrf_gpio_cfg(qspi_dev_config.csn_pin, NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_NOPULL,
                    NRF_GPIO_PIN_E0E1, NRF_GPIO_PIN_NOSENSE);
       nrf_gpio_cfg(qspi_config.pins.sck, NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_NOPULL,
                    NRF_GPIO_PIN_E0E1, NRF_GPIO_PIN_NOSENSE);
       for (int i = 0; i < 4; i++) {
           nrf_gpio_cfg(qspi_config.pins.io[i], NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_CONNECT,
                        NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_E0E1, NRF_GPIO_PIN_NOSENSE);
       }
   }
   if (qspi_config.skip_pmux_cfg == false) { //This overrides the driver's default configuration
       // Set pin source as FLPR.
       nrf_gpio_pin_control_select(p_config->pins.sck, NRF_GPIO_PIN_SEL_VPR);
       nrf_gpio_pin_control_select(p_config->csn_pin, NRF_GPIO_PIN_SEL_VPR);
       for (int i = 0; i < NRF_SQSPI_MAX_NUM_DATA_LINES; i++) {
           nrf_gpio_pin_control_select(p_config->pins.io[i], NRF_GPIO_PIN_SEL_VPR);
       }
   }

.. _nrf54L_series_porting_guide_high_speed_transfers:

High speed transfers
====================

.. note::
   High speed transfers (above 32 MHz) on the nRF54L15 DK are only supported starting from sQSPI v1.0.0.

High speed read transfers use hardware-enabled delay sampling and require resetting the pad configuration between reads.
This applies to the entire port **P2**, which is important if you use other pins on this port in addition to those allocated for sQSPI.
In this context, *reset* means first setting the drive strength to standard, then setting it back to extra high.
When using sQSPI on the nRF54L Series devices, the Soft Peripheral must have exclusive access to extra-high-drive strength.

When sQSPI performs read transfers at frequencies greater than or equal to 32 MHz, no other peripheral or ETM trace on the **P2** port can use the extra-high-drive GPIO configuration.
They are limited to using high-drive strength.
You can enable high-speed transfers by setting the ``nrf_sqspi_dev_cfg_t.sample_delay_cyc`` option and not setting ``nrf_sqspi_dev_cfg_t.sck_freq_khz``.
This means that if the frequency is greater than or equal to 32 MHz, then ``nrf_sqspi_dev_cfg_t.sample_delay_cyc`` must be set to 0.

A high-speed sQSPI transfer requires access to the peripheral ``GPIOHSPADCTRL`` (GPIO High Speed Pad Control).
You must set the ``GPIOHSPADCTRL.BIAS`` register for higher slew setting and replica bias. 

The following table shows the limitations of each operation mode:

.. list-table:: Drive strengths and high speed transfers
   :widths: auto
   :header-rows: 1

   * - sQSPI drive strength
     - BIAS
     - Max. sQSPI frequency
     - ``sample_delay_cyc``
     - Max. peripheral drive strength
     - Max. peripheral frequency
   * - Extra-high-drive
     - ``0x6``
     - ``64 MHz``
     - ``0``
     - High-drive
     - ``16 MHz``
   * - Extra-high-drive
     - ``0x1``
     - ``32 MHz``
     - ``1``
     - Extra high-drive
     - ``32 MHz``
   * - Extra-high-drive
     - ``0x0``
     - ``16 MHz``
     - ``>= 1``
     - Extra-high-drive
     - ``32 MHz``
   * - High-drive
     - ``0x0``
     - ``16 MHz``
     - ``>= 1``
     - Extra-high-drive
     - ``32 MHz``

.. note::
   At 32 MHz, extra-high-drive strength may allow sQSPI to work without hardware-enabled delayed sampling.
   Depending on the external circuitry you might need to set ``sample_delay_cyc`` to 0 to enable the hardware-enabled delay sampling. 


Memory retention configuration
******************************

The sQSPI soft peripheral requires RAM retention in order to go into the lowest power consumption mode, which can be called through the :c:func:`nrf_sqspi_deactivate` function.

Assuming there is an access to the peripheral ``MEMCONF``, the following code snippet illustrates how to enable FLPR RAM retention, followed by deactivation and reactivation, and finally how to disable RAM retention:

.. code-block:: c

  // Deactivate sequence
  nrf_memconf_ramblock_ret_enable_set(NRF_MEMCONF, 1, MEMCONF_POWER_RET_MEM0_Pos, true);
  nrf_sqspi_deactivate(&m_qspi);
  // Activate sequence
  nrf_qspi_activate(&m_qspi);
  nrf_memconf_ramblock_ret_enable_set(NRF_MEMCONF, 1, MEMCONF_POWER_RET_MEM0_Pos, false);

.. _nrf54l15_porting_guide_ram_configuration:
.. _nrf54L_series_porting_guide_ram_configuration:

RAM configuration
*****************

The sQSPI Soft Peripheral operates from RAM.

.. note::
   Starting from sQSPI 1.0.0, Position Independent Code (PIC) is supported.
   This allows an application to determine where to load the Soft Peripheral firmware.

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
          - 0x2003FE00
          - 0x200 (but the entire block should be retained)

  .. tab:: **nRF54LM20**

      .. list-table:: nRF54LM20 RAM Configuration Table
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
          - 0x2007FD40
          - 0x200 (but the entire block should be retained)

The build environment described in the :ref:`nrf54l_series_porting_guide_code` section must comply with these requirements.
This includes proper settings in linker scripts, device tree specifications (DTS), and resource allocation.

To initialize the sQSPI struct on an nRF54L Series device, use the following lines:

 .. code-block:: c

    #define SP_REGIF_BASE (SP_FIRMWARE_ADDR + 0x3B40)

    // Initialize sQSPI with the virtual register interface placed at SP_REGIF_BASE
    static nrf_sqspi_t m_sqspi = {.p_reg = (void *)SP_REGIF_BASE, .drv_inst_idx = 0};

Ensure that ``SP_FIRMWARE_ADDR`` is set so that it does not overlap with the context saving address.
For nRF54L Series devices, the firmware is placed before the context saving address.
This means, that ``SP_FIRMWARE_ADDR`` plus the component sizes listed in the table must be of a lower value than the context saving address.

The values in the table below have been tested and are considered production-ready for ``SP_FIRMWARE_ADDR``:

.. list-table:: sQSPI firmware address values
   :widths: 20 20
   :header-rows: 1

   * - Device
     - `SP_FIRMWARE_ADDR`
   * - nRF54L15
     - 0x2003C000
   * - nRF54LM20
     - 0x2007C000
