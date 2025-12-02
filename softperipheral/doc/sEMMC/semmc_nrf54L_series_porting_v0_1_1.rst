.. _semmc_nrf54l_series_porting_guide:

sEMMC nRF54L Series porting guide
#################################

.. contents::
   :local:
   :depth: 2

This page provides a comprehensive overview of the code structure, file hierarchy, and essential configurations and requirements needed to successfully port and implement an sEMMC application on the nRF54L Series device.

.. _semmc_nrf54l_series_porting_guide_code:

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
          │   ├── nrf54l
          │   │   ├── semmc_firmware.h
          │   │   └── semmc_firmware_v0.1.1.h
          │   │   └── ...
          │   ├── nrf_config_semmc.h
          │   ├── nrf_sp_emmc.h
          │   ├── nrf_semmc.h
          └── src
              └── nrf_semmc.c

.. note::
   The main interface for sEMMC is the :file:`nrf_semmc.h` file.
   It contains a list of supported and tested EMMC commands.

Header files
============

sEMMC application requires specific header files used by the driver code (internal paths) and specific header files for nrfx (external dependencies).
You must include paths to these files in the build environment's list of include paths.
The following list is a detailed breakdown of the necessary paths:

* Repository internal paths:

  * :file:`softperipheral/include` - Soft peripherals register interface and metadata
  * :file:`softperipheral/sEMMC/include` - sEMMC register interface and driver header
  * :file:`softperipheral/sEMMC/include/nrf54l` - The sEMMC firmware for the Fast Lightweight Perpipheral Processor (FLPR)

* External dependencies:

   * :file:`nrfx` - Includes the main nrfx header file (:file:`nrfx.h`) and hal, which is sometimes necessary for additional GPIO configuration.
   * :file:`nrfx/drivers/include` - Includes the nrfx driver header files.
     For example, the :file:`nrfx_dppi.h` file.
   * Path to the application-specific headers and macros.
     For example, the :file:`nrfx_glue.h`, :file:`nrfx_log.h`, or a configuration header file.
     See an example configuration for the :file:`nrfx_config.h` file (valid for all nRF54L Series devices):

.. code-block:: c

   #ifndef NRFX_CONFIG_H__
   #define NRFX_CONFIG_H__

   #include "softperipheral_regif.h" // To Resolve correct VPR IRQn for the SoC.
   #define nrf_semmc_irq_handler        SP_VPR_IRQHandler

   #define NRF_SEMMC_ENABLED          (1)
   #define NRF_SEMMC_RESPONSE_SIZE    (4)


   #endif // NRFX_CONFIG_H__

Compiling source files
======================

For an sEMMC application to function properly, you must compile the driver implementation from the source file by including :file:`nrf_semmc.c` at the top of your application.

Application core and FLPR configuration
***************************************

You must adjust the settings for the nRF54L Series SoC to run at highest base clock frequency.
To work with any of the following settings, ensure you have completed the following:

* You have allocated memory for the data pointers used by the sEMMC driver.
  The memory is independent from the one outlined in the :ref:`semmc_nrf54l_series_porting_guide_ram_configuration` subsection.
  It is designated to function as shared memory for communication purposes, rather than containing the sEMMC executable code.

* You have enabled the FLPR access to the ``MEMCONF`` peripheral.
  This access is necessary to properly disable sEMMC.

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

GPIO configuration
******************

The provided table details the configuration for each GPIO pin used in the sEMMC application.
You will also need to set the drive strength of the pins to extra high drive.
The following option is possible on nRF54L Series devices, assuming that the FLPR core has access to these ports and pins:

.. list-table::
   :widths: 10 10 20 20 20
   :header-rows: 1

   * - GPIO pin
     - sEMMC role
     - Direction config
     - Input config
     - Pin pull config
   * - **P2.0**
     - D3
     - Output
     - Disconnected
     - Pull-up
   * - **P2.1**
     - CLK
     - Output
     - Disconnected
     - Pull-down
   * - **P2.2**
     - D0
     - Output
     - Disconnected
     - Pull-up
   * - **P2.3**
     - D2
     - Output
     - Disconnected
     - Pull-up
   * - **P2.4**
     - D1
     - Output
     - Disconnected
     - Pull-up
   * - **P2.5**
     - CMMD
     - Output
     - Disconnected
     - Pull-up

This code snippet shows how you can set up these pins in your application:

.. code-block:: c

   uint32_t io[6] = {
     NRF_PIN_PORT_TO_PIN_NUMBER(1, 2), //CLK
     NRF_PIN_PORT_TO_PIN_NUMBER(2, 2), //D0
     NRF_PIN_PORT_TO_PIN_NUMBER(4, 2), //D1
     NRF_PIN_PORT_TO_PIN_NUMBER(3, 2), //D2
     NRF_PIN_PORT_TO_PIN_NUMBER(0, 2), //D3
     NRF_PIN_PORT_TO_PIN_NUMBER(5, 2)  //CMMD
   };
   nrf_gpio_cfg(io[0], NRF_GPIO_PIN_DIR_OUTPUT,
                NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_PULLDOWN,
                NRF_GPIO_PIN_E0E1, NRF_GPIO_PIN_NOSENSE);
   for (int i = 1; i < 6; i++) {
     nrf_gpio_cfg(io[i], NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT,
                  NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_E0E1, NRF_GPIO_PIN_NOSENSE);
   }
   for (int i = 0; i < 6; i++) {
     nrf_gpio_pin_control_select(io[i], NRF_GPIO_PIN_SEL_VPR);
   }

.. _semmc_nrf54L_series_porting_guide_high_speed_transfers:

High speed transfers
====================

To enable communcation at 32MHz on an nRF54L Series device, you need to set the bias of the GPIOHSPADCTRL module.
The following code snippet shows how you can set the value in your application:

.. code-block:: c

   gpiohs_bias_val = 0x2;
   NRF_GPIOHSPADCTRL->BIAS = gpiohs_bias_val;

Memory retention configuration
******************************

The sEMMC soft peripheral requires RAM retention in order to go into the lowest power consumption mode, which can be called through the :c:func:`nrf_semmc_disable` function.

Assuming there is an access to the peripheral ``MEMCONF``, the following code snippet illustrates how to enable FLPR RAM retention, followed by disabling and re-enabling, and finally how to disable RAM retention:

.. code-block:: c

  // Disable sequence
  nrf_memconf_ramblock_ret_enable_set(NRF_MEMCONF, 1, MEMCONF_POWER_RET_MEM0_Pos, true);
  nrf_semmc_disable(&m_semmc);
  // Enable sequence
  nrf_semmc_enable(&m_semmc);
  nrf_memconf_ramblock_ret_enable_set(NRF_MEMCONF, 1, MEMCONF_POWER_RET_MEM0_Pos, false);

.. _semmc_nrf54L_series_porting_guide_ram_configuration:

RAM configuration
*****************

The sEMMC Soft Peripheral operates from RAM.

.. note::
   sEMMC supports Position Independent Code (PIC).
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
        * - sEMMC firmware
          - ``SP_FIRMWARE_ADDR``
          - 0x3C00
        * - sEMMC execution RAM
          - ``SP_FIRMWARE_ADDR`` + 0x3C00
          - 0x600
        * - sEMMC virtual register interface
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
        * - sEMMC firmware
          - ``SP_FIRMWARE_ADDR``
          - 0x3C00
        * - sEMMC execution RAM
          - ``SP_FIRMWARE_ADDR`` + 0x3C00
          - 0x600
        * - sEMMC virtual register interface
          - ``SP_FIRMWARE_ADDR`` + 0x4200
          - 0x200
        * - Context saving
          - 0x2007FD40
          - 0x200 (but the entire block should be retained)

The build environment described in the :ref:`semmc_nrf54l_series_porting_guide_code` section must comply with these requirements.
This includes proper settings in linker scripts, device tree specifications (DTS), and resource allocation.

To initialize the sEMMC struct on an nRF54L Series device, use the following lines:

 .. code-block:: c

    #define SP_REGIF_BASE (SP_FIRMWARE_ADDR + 0x4200)

    // Initialize sEMMC with the virtual register interface placed at SP_REGIF_BASE
    static nrf_semmc_t m_semmc = {.p_reg = (void *)SP_REGIF_BASE, .drv_inst_idx = 0};

Ensure that ``SP_FIRMWARE_ADDR`` is set so that it does not overlap with the context saving address.
For nRF54L Series devices, the firmware is placed before the context saving address.
This means, that ``SP_FIRMWARE_ADDR`` plus the component sizes listed in the table must be of a lower value than the context saving address.

The values in the table below have been tested and are considered production-ready for ``SP_FIRMWARE_ADDR``:

.. list-table:: sEMMC firmware address values
   :widths: 15 20
   :header-rows: 1

   * - Device
     - `SP_FIRMWARE_ADDR`
   * - nRF54L15
     - 0x2003B400
   * - nRF54LM20
     - 0x2007B400


.. _semmc_nrf54L_series_porting_guide_read_response:

Read responses
**************

sEMMC cannot process read data and response at the same time (see the :ref:`semmc_limitations`).
However, you can mitigate it in the following ways:

* The recommended way is to perform the read operation with the :c:var:`nrf_semmc_config_t.process_response` variable set to ``NRF_EMMC_RESPONSE_PROC_PROCESS``.
  The response is available in the :c:var:`nrf_semmc_cmd_desc_t.err` variable.
  If the response is ``NRF_SEMMC_SUCCESS``, the read was successful.
  If you receive a different response, retry the read operation, this time with the :c:var:`nrf_semmc_config_t.process_response` variable set to ``NRF_EMMC_RESPONSE_PROC_IGNORE``.

* Alternatively, you can perform the read operation with the :c:var:`nrf_semmc_config_t.process_response` variable set to ``NRF_EMMC_RESPONSE_PROC_IGNORE``, and then follow it with a CMD13 command to verify if the status is correct.
