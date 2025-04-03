.. _nrf54H20_porting_guide:

nRF54H20 porting guide
######################

.. contents::
   :local:
   :depth: 2

This page provides a comprehensive overview of the code structure, file hierarchy, and essential configurations and requirements needed to successfully port and implement an sQSPI aplication on the nRF54H20 device.

.. _nrf54h20_porting_guide_code:

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
          │   ├── nrf54h20
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
  * :file:`softperipheral/sQSPI/include/nrf54h20` - The sQSPI firmware for the Fast Lightweight Perpipheral Processor (FLPR)

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

You must adjust the settings for the nRF54H20 SoC to run at highest base clock frequency.

To work with any of the following settings, ensure you have completed the following:

* You have allocated memory for the data pointers used by the sQSPI driver.
  The memory is independent from the one outlined in the :ref:`nrf54H20_porting_guide_ram_configuration` subsection.
  It is designated to function as shared memory for communication purposes, rather than containing the sQSPI executable code.

Security configuration
======================

Configure the security settings for the nRF54H20 device based on the operational requirements of your application.

Secure environment
------------------

In a secure configuration, both the application core and the FLPR core of the nRF54H20 device must operate within a secure environment enabled by TrustZone Secure.

Non-secure environment
----------------------

In a non-secure configuration, both the application core and the FLPR core of the nRF54H20 device must operate outside the secure environment (without TrustZone Secure).

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
   You can apply this setup by setting :c:var:`nrfx_qspi2_cfg_t.skip_gpio_cfg` and :c:var:`nrfx_qspi2_cfg_t.skip_pmux_cfg` to ``false``.

Configuring pins
================

In some cases you might have to modify the sQSPI driver configuration.
For example, when changing pin drive strength to guarantee signal integrity for a new PCB design.
You must address these cases on the sQSPI application code:

* If you set the :c:var:`nrfx_qspi2_cfg_t.skip_gpio_cfg` variable to ``true``, the GPIO configuration is not managed by the sQSPI driver and it must be manually handled by the application. 
  This is a requirement for the nRF54H20 device.
* If you set the :c:var:`nrfx_qspi2_cfg_t.skip_pmux_cfg` variable to ``true``, the GPIO multiplexing is not managed by the sQSPI driver and it must be manually handled by the application. 
  This is a requirement for the nRF54H20 device.

GPIO multiplexing must be handled by setting the correct ``CTRLSEL`` value in UICR.

Additionally, the sQSPI application requires FastPad bias configuration to run at high frequency on SCLK.

.. _nrf54H20_porting_guide_ram_configuration:

RAM configuration
*****************

The sQSPI Soft Peripheral operates from RAM.
Your build environment must reserve the required RAM and ensure that it is readable and writable by both the application core and the FLPR core.
This table details the memory region, it should be non-cacheable:

.. list-table:: RAM Configuration Table
   :widths: auto
   :header-rows: 1

   * - Start Address
     - Size
   * - 0x2f890000
     - 0x4000

The build environment described in the :ref:`nrf54h20_porting_guide_code` section must comply with these requirements.
This includes proper settings in linker scripts, device tree specifications (DTS), and resource allocation.
