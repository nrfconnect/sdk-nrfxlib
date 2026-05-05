.. _dppi_integration_for_soft_peripherals:

DPPI Integration for Soft Peripherals
#####################################

.. contents::
   :local:
   :depth: 2

Soft peripherals can be integrated with Nordic Semiconductor's DPPI (Distributed programmable peripheral interconnect).
This offers the possibility of offloading triggering an action on the Soft Peripheral from the application to another IP.
For example, starting a pre-configured periodical read/write operation when a external timer expires, which also offers
lower power consumption benefits if the application core can sleep and said timer belongs to a separate always on power domain.

How Application communicates with the Soft Peripherals
******************************************************

Soft Peripherals operate using an internal state machine (unless explicitly stated otherwise) that changes states and executes
transfers depending on their corresponding application side API function calls. This is an overview of the sequence:

1. These API function calls use what we define as sync barriers to communicate with the Soft Peripheral firmware running on FLPR.
2. The sync barriers operate by triggering IRQ towards FLPR and waiting for a handshake to return.

Sync barriers trigger an IRQ on FLPR by writing to its ``TASKS``, Nordic SoCs can have different settings for which FLPR ``TASKS`` indexes
are hardwired to IRQ lines.

Adding DPPI
===========

Nordic SoCs can have different settings for which FLPR ``TASKS`` indexes are DPPI capable, since these indexes will usually have a partial
overlap with those hardwired to IRQ lines, it is possible to trigger the same IRQ without having to use a sync barrier,
therefore avoiding the use of application side API function calls.

It is possible to enable this behavior by setting FLPR ``SUBSCRIBE`` for the corresponding ``TASKS`` index
(provided said index meets the overlap criteria)

For more details on ``TASKS`` and ``SUBSCRIBE`` see VPR - Real Time Peripherals - VEVIF in the Nordic SoC's documentation.

Slots and Role Map
******************

Soft Peripherals APIs use a slot + role map configuration structure.
A slot will correspond to a FLPR ``TASKS`` index and its IRQ handler, a role corresponds to the behavior/sync barrier associated to the slot.
Behavior dynamically changes by changing the callback function that the IRQ handler uses.

All Soft Peripheral APIs have convenience functions that change the role map (behavior per slot) and
enable ``SUBSCRIBE`` (for an individual slot) 

The convention is one slot per each ``TASKS`` index that is hardwired to and IRQ line. Slots 0 to 3 for those that are DPPI capable
and slots 4 and 5 for those that are not.

Soft Peripherals APIs have mechanisms to keep track of the current role map and be consistent with the Soft Peripheral FW
running on FLPR. 

All Soft Peripherals have a default role map with no ``SUBSCRIBE`` enabled, the user can choose to either enable ``SUBSCRIBE`` for the default role map
(most common use case, as described above) or change the role map and enable the corresponding ``SUBSCRIBE``

For more details on the ``TASKS`` indexes that are IRQ capable and DPPI capable, refer to the Nordic SoC's VPR configuration documentation.

Default role map
****************

These role maps are designed to avoid possible conflicts with other system components that may use DPPI channels
hardwired to FLPR, see :ref:`softdevice_controller_readme`. The intention is for Soft Peripherals operation to be transparent, even if the
user does not call the related API functions.

.. tabs::

   .. tab:: **nRF54L15/nRF54LM20**

     .. list-table:: Soft Peripherals Default Role Map
        :widths: auto
        :header-rows: 1
     
        * - Slot
          - ``TASKS`` index
          - DPPIC00 channel
          - Role
          - Sync Barrier
        * - Slot 0
          - 16
          - 0
          - DPPI_0 (START)
          - No sync barrier, starts a transfer (assuming it is prepared beforehand)
        * - Slot 1
          - 17
          - 1
          - NONE
          - Not in use
        * - Slot 2
          - 18
          - 2
          - STOP
          - ``__SSB()`` Stops an ongoing transfer
        * - Slot 3
          - 19
          - 3
          - NONE
          - Not in use
        * - Slot 4
          - 20
          - Not DPPI capable
          - CONFIG
          - ``__CSB()`` Parsing a configuration
        * - Slot 5
          - 21
          - Not DPPI capable
          - ACTION
          - ``__ASB()`` Enabling or disabling the Soft Peripheral, preparing a transfer

   .. tab:: **nRF54H20**

     .. list-table:: Soft Peripherals Default Role Map
        :widths: auto
        :header-rows: 1
     
        * - Slot
          - ``TASKS`` index
          - DPPIC120 channel
          - Role
          - Sync Barrier
        * - Slot 0
          - 24
          - 4
          - DPPI_0 (START)
          - No sync barrier, starts a transfer (assuming it is prepared beforehand)
        * - Slot 1
          - 25
          - 5
          - NONE
          - Not in use
        * - Slot 2
          - 26
          - 6
          - STOP
          - ``__SSB()`` Stops an ongoing transfer
        * - Slot 3
          - 27
          - 7
          - NONE
          - Not in use
        * - Slot 4
          - 16
          - Not DPPI capable
          - CONFIG
          - ``__CSB()`` Parsing a configuration
        * - Slot 5
          - 17
          - Not DPPI capable
          - ACTION
          - ``__ASB()`` Enabling or disabling the Soft Peripheral, preparing a transfer

Example usage
*************

The Soft Peripheral APIs will essentially have 2 functions: one to change the role map and another to enable ``SUBSCRIBE`` per slot.
The most common use case would be to enable ``SUBSCRIBE`` for the DPPI capable slots in the default role map.

Example initialization sequence (assuming sQSPI running on L series device):

.. code-block:: c

   //SP_REGIF_BASE depends on the register interface address from a DTS overlay

   static nrf_sqspi_t m_qspi = {.p_reg = (void *)SP_REGIF_BASE, .drv_inst_idx = 0};
   
   static nrf_sqspi_cfg_t m_qspi_config = {
       .pins =
           {
               .sck = NRF_PIN_PORT_TO_PIN_NUMBER (1, 2),
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
   
   // format data struct and IRQ handler callback function skipped

   //Applying pinctrl config from DTS overlay and linking IRQ handler goes here

   if (nrf_sqspi_init(&m_qspi, &m_qspi_config) != NRF_SQSPI_SUCCESS){
     return -1;
   }
   if (!nrf_sqspi_init_check(&m_qspi)){
     return -1;
   }
   if (nrf_sqspi_activate(&m_qspi) != NRF_SQSPI_SUCCESS){
     return -1;
   }

Enable ``SUBSCRIBE`` for slot 0 (START) and 2 (STOP) after Soft Peripheral's activate/enable:

.. code-block:: c

   if (nrf_sqspi_dppi_subscribe_enable(&m_qspi, 0, true) != NRF_SQSPI_SUCCESS){
     return -1;
   }
   if (nrf_sqspi_dppi_subscribe_enable(&m_qspi, 2, true) != NRF_SQSPI_SUCCESS){
     return -1;
   }

Customizing the role map
========================

It is possible to modify the role map independent from enabling ``SUBSCRIBE``. This example follows the same pattern
as before but changing the role map first:

.. code-block:: c

   //NOTE: Custom config on slots 0-3 with SUBSCRIBE enabled (assuming we want all DPPI capable TASKS indexes)
   	static const uint8_t custom_roles[SP_TASK_ENTRY_COUNT] = {
   	    SP_DPPI_ROLE_DPPI_0, /* slot 0 - ch0 */
   	    SP_DPPI_ROLE_CONFIG, /* slot 1 - ch1 */
   	    SP_DPPI_ROLE_ACTION, /* slot 2 - ch2 */
   	    SP_DPPI_ROLE_STOP,   /* slot 3 - ch3 */
   	    SP_DPPI_ROLE_NONE,   /* slot 4 - task with no DPPI path */
   	    SP_DPPI_ROLE_NONE,   /* slot 5 - task with no DPPI path */
   	};
   	if(nrf_sqspi_dppi_role_map_set(&m_sqspi, custom_roles) != NRF_SQSPI_SUCCESS){
       return -1;
    }
   	for (uint8_t slot = 0; slot < SP_DPPI_SLOT_COUNT; slot++) {
   	  if(nrf_sqspi_dppi_subscribe_enable(&m_sqspi, slot, true) != NRF_SEMMC_SUCCESS){
         return -1;
      }
   	}

Special considerations
**********************

- In Nordic SoCs that support Soft Peripherals, FLPR's DPPI capable ``TASKS`` indexes are tied to specific DPPI channels at all times,
  this is not configurable. The user can only control the corresponding ``SUBSCRIBE``.
- API convenience functions to either change role map or enable ``SUBSCRIBE``, are meant to be called after ``nrf_<sp>_enable``/``nrf_<sp>_activate``
- If the Soft Peripheral is uninitialized and initialized again (``nrf_<sp>_uninit`` + ``nrf_<sp>_init`` sequence),
  re configuring the role map and re-enabling ``SUBSCRIBE`` is required (if applicable) after ``nrf_<sp>_enable``/``nrf_<sp>_activate``.
  Since initialization effectively resets FLPR, the Soft Peripheral will revert back to the default role map with no ``SUBSCRIBE`` enabled,
  unintialization reflects this on the application API side to keep a consistent state on both sides.
