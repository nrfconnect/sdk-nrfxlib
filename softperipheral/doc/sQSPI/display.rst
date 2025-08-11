.. _sqspi_display:

sQSPI and display usage
#######################

The sQSPI API provides the data format interface for use with external displays.
See :c:struct:`nrf_sqspi_data_fmt_t`.

The following parameter combinations have been tested and are considered production ready:

.. list-table:: sQSPI data format
   :widths: auto
   :header-rows: 1

   * - :c:var:`nrf_sqspi_data_fmt_t.data_padding`
     - :c:var:`nrf_sqspi_data_fmt_t.data_container`
     - :c:var:`nrf_sqspi_data_fmt_t.data_swap_unit`
   * - 0
     - 8
     - 8
   * - 0
     - 8
     - 0
   * - 16
     - 16
     - 8
   * - 0
     - 32
     - 16

Using external sync bit (tearing effect)
****************************************

sQSPI v1.1.0 can be used alongside some application level processing to send a transfer when a pulse (tearing effect) is detected by either:

* Polling a GPIO pin.
* Setting an interrupt handler to an event linked to a specific GPIO pin (access to peripheral ``GPIOTE`` is required for this).
* Forwarding a GPIO event through DPPI towards a FLPR task linked to starting a transfer (access to peripheral ``DPPIC`` is required for this, also see :file:`softperipheral/include/softperipheral_regif.h` for the specific task).

You can use the first two options by holding a transfer and starting it when the pulse is detected (using flag ``NRF_SQSPI_FLAG_HOLD_XFER`` in :c:func:`nrf_sqspi_xfer`).
