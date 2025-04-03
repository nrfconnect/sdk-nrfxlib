.. _nrf54L15_porting_guide:

nRF54L15 porting guide
######################

.. contents::
   :local:
   :depth: 2

This page provides a comprehensive overview of the code structure, file hierarchy, and essential configurations and requirements needed to successfully port and implement an sQSPI aplication on the nRF54L15 device.

.. _nrf54l15_porting_guide_code:

sQSPI Application code
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
          │   ├── nrf54l15
          │   │   ├── sqspi_firmware.h
          │   │   ├── sqspi_firmware_v0.1.0.h
          │   │   └── ...
          │   ├── nrf_sp_qspi.h
          │   ├── nrfx_config_qspi2.h
          │   ├── nrfx_qspi2.h
          └── src
              └── nrfx_qspi2.c

.. note::
   The main interface for sQSPI is the :file:`nrfx_qspi2.h` file.

Header files
============

For an sQSPI application to function correctly, it requires specific header files used by the driver code (internal paths) and specific header files for nrfx (external dependencies).
You must include paths to these files in the build environment's list of include paths.
The following list is a detailed breakdown of the necessary paths:

* Repository internal paths:

  * :file:`softperipheral/include` - Soft peripherals register interface and metadata
  * :file:`softperipheral/sQSPI/include` - sQSPI register interface and driver header
  * :file:`softperipheral/sQSPI/include/nrf54l15` - The sQSPI firmware for the Fast Lightweight Perpipheral Processor (FLPR)

* External dependencies:

   * :file:`nrfx` - Includes the main nrfx header file (:file:`nrfx.h`).
   * :file:`nrfx/drivers/include` - Includes the nrfx driver header files.
     For example, the :file:`nrfx_dppi.h` file.
   * Path to the application-specific headers and macros.
     For example, the :file:`nrfx_glue.h`, :file:`nrfx_log.h`, or a configuration header file.
     See an example configuration for the :file:`nrfx_config.h` file:

      .. code-block:: c

         #ifndef NRFX_CONFIG_H__
         #define NRFX_CONFIG_H__

         #include "softperipheral_regif.h" // To Resolve correct VPR IRQn for the SoC.
         #define nrfx_qspi2_irq_handler        SP_VPR_IRQHandler

         #define NRFX_QSPI2_ENABLED            (1)
         #define NRFX_QSPI2_MAX_NUM_DATA_LINES (4)

         #endif // NRFX_CONFIG_H__

Compiling source files
======================

For an sQSPI application to function properly, you must compile the driver implementation from the source file :file:`nrfx_qspi2.c`.

Application core and FLPR configuration
***************************************

You must adjust the settings for the nRF54L15 SoC to run at highest base clock frequency.

To work with any of the following settings, ensure you have completed the following:

* You have allocated memory for the data pointers used by the sQSPI driver.
  The memory is independent from the one outlined in the :ref:`nrf54L15_porting_guide_ram_configuration` subsection.
  It is designated to function as shared memory for communication purposes, rather than containing the sQSPI executable code.

* You have enabled the FLPR access to the ``MEMCONF`` peripheral.
  This access is necessary to properly deactivate sQSPI.

Security configuration
======================

Configure the security settings for the nRF54L15 device based on the operational requirements of your application.

Secure environment
------------------

In a secure configuration, both the application core and the FLPR core of the nRF54L15 device must operate within a secure environment enabled by TrustZone Secure.

The following code snippet shows how the application code can configure permissions for the FLPR instance:

.. code-block:: c

  // NOTE: Setting FLPR to secure
  NRF_SPU00_S->PERIPH[0xC].PERM = (SPU_PERIPH_PERM_SECATTR_Secure << SPU_PERIPH_PERM_SECATTR_Pos);

Non-secure environment
----------------------

In a non-secure configuration, both the application core and the FLPR core of the nRF54L15 device must operate outside the secure environment (without TrustZone Secure).

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
   sQSPI driver provides a default GPIO configuration and multiplexing.
   You can apply this setup by setting :c:var:`nrfx_qspi2_cfg_t.skip_gpio_cfg` and :c:var:`nrfx_qspi2_cfg_t.skip_pmux_cfg` to ``false``.

Configuring pins
================

In some cases you might have to modify the sQSPI driver configuration.
For example, when changing pin drive strength to guarantee signal integrity for a new PCB design.
You must address these cases on the sQSPI application code:

* If you set the :c:var:`nrfx_qspi2_cfg_t.skip_gpio_cfg` variable to ``true``, the GPIO configuration is not managed by the sQSPI driver and it must be manually handled by the application. 
* If you set the :c:var:`nrfx_qspi2_cfg_t.skip_pmux_cfg` variable to ``true``, the GPIO multiplexing is not managed by the sQSPI driver and it must be manually handled by the application. 

The following code snippet shows how the application code can allocate the required pins and override the sQSPI driver's default configuration:

.. code-block:: c

   //qspi pin config
   static nrfx_qspi2_cfg_t qspi_config = {
       .pins =
           {
               .sck = NRF_PIN_PORT_TO_PIN_NUMBER(1, 2),
               .strobe = NRFX_QSPI2_PINS_UNUSED,
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

    nrfx_qspi2_dev_cfg_t qspi_dev_config = {.csn_pin = NRF_PIN_PORT_TO_PIN_NUMBER(5, 2),
                                            .sck_freq_khz = ...,
                                            ...
                                            };

   nrfx_qspi2_init(&qspi, &qspi_config);

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
       for (int i = 0; i < NRFX_QSPI2_MAX_NUM_DATA_LINES; i++) {
           nrf_gpio_pin_control_select(p_config->pins.io[i], NRF_GPIO_PIN_SEL_VPR);
       }
   }

Additionally, the sQSPI application requires FastPad bias configuration to run at high frequency on SCLK.

The following code snippet shows how the application code can set the FastPad bias on port **P2** by running the following code:

.. code-block:: c

   // If drive strength (E0E1), set bias on the gpio port to 3 (FastPad configuration)
   volatile unsigned* reg = (unsigned*)(0x50050430);
   *reg = 0x3;

Memory retention configuration
******************************

The sQSPI soft peripheral requires RAM retention in order to go into the lowest power consumption mode, which can be called through the :c:func:`nrfx_qspi2_deactivate` API.

Assuming there is an access to the peripheral `MEMCONF`, the following code snippet illustrates how to enable FLPR RAM retention, followed by deactivation and reactivation, and finally how to disable RAM retention:

 .. code-block:: c

    //Deactivate sequence
    nrf_memconf_ramblock_ret_enable_set(NRF_MEMCONF, 1, MEMCONF_POWER_RET_MEM0_Pos, true);
    nrfx_qspi2_deactivate(&m_qspi);
    //activate sequence
    nrfx_qspi2_activate(&m_qspi);
    nrf_memconf_ramblock_ret_enable_set(NRF_MEMCONF, 1, MEMCONF_POWER_RET_MEM0_Pos, false);

.. _nrf54L15_porting_guide_ram_configuration:

RAM configuration
*****************

The sQSPI Soft Peripheral operates from RAM.
Your build environment must reserve the required RAM and ensure that it is readable and writable by both the application core and the FLPR core.
This table details the memory region:

.. list-table:: RAM Configuration Table
   :widths: auto
   :header-rows: 1

   * - Start Address
     - Size
   * - 0x2003c000
     - 0x4000

The build environment described in the :ref:`nrf54l15_porting_guide_code` section must comply with these requirements.
This includes proper settings in linker scripts, device tree specifications (DTS), and resource allocation.
