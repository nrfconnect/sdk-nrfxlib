.. _sqspi_limitations:

Limitations
###########

When working with sQSPI, you should be aware of the following limitations.

.. contents::
   :local:
   :depth: 2

v1.1.0
******

Refer to the following detailed descriptions of current limitations:

* sQSPI does not support slave mode operations; it can only operate as a controller (master).
* The sQSPI support for SPI half-duplex setup is implemented but not tested.
* sQSPI SPI modes 1,2, and 3 (see the :c:var:`nrf_sqspi_dev_cfg_t.spi_cpolpha` configuration structure) may present artifacts (clock stretching or extra edges) on the last SCLK cycle during a read, this has no effect on data integrity.
* The nrfx API for sQSPI does not support configuring the use of the positive or negative edge of SCLK delayed read sampling.
* sQSPI employs the :c:var:`nrf_sqspi_dev_cfg_t.sample_delay_cyc` parameter as an offset to FLPR's base clock counter , not SLCK clock cycles (see the :c:struct:`nrf_sqspi_dev_cfg_t` struct).
* sQSPI :c:var:`nrf_sqspi_dev_cfg_t.sample_delay_cyc` is constrained to either a value of 0 if using ``GPIOHSPADCTRL`` for high speed transfers (see :ref:`nrf54L15_porting_guide_high_speed_transfers`) or a value greater than 0 but lower than that of ``FLPR_counter`` if a high speed transfer is not needed (see :ref:`sqspi_timing`).
* Implementation of sQSPI quad or dual lane for command transmission (for example, ``2_2_2`` and ``4_4_4`` modes) is implemented but has not been tested.
* sQSPI does not support a configurable pin for the CSN line; only **PIN 5** is supported (see :c:var:`nrf_sqspi_dev_cfg_t.csn_pin`).
* :c:var:`nrf_sqspi_xfer_t.p_data` needs to be a pointer with a 32 bit aligned address.
* sQSPI high speed reading cannot happen directly when changing from SPI mode 0 to 3. Either do a high speed write or a slow read (5.8 MHz) in between.
* :c:var:`nrf_sqspi_xfer_t.addr_length` must be greater than :c:var:`nrf_sqspi_dev_cfg_t.mspi_lines`.

* sQSPI will present undefined behavior in the following cases:
   * 0 byte reads: :c:var:`nrf_sqspi_xfer_t.dir` set to ``NRF_SQSPI_XFER_DIR_RX`` and :c:var:`nrf_sqspi_xfer_t.data_length` set to 0.
   * 0 command bits: :c:var:`nrf_sqspi_xfer_t.cmd_length` set to 0.
   * 0 address bits: :c:var:`nrf_sqspi_xfer_t.addr_length` set to 0.

Data format (display use) limitations:

* :c:var:`nrf_sqspi_data_fmt_t.data_bit_reorder_unit` must be set to the same value as :c:var:`nrf_sqspi_data_fmt_t.data_swap_unit`.
* If :c:var:`nrf_sqspi_data_fmt_t.data_padding` is not set to 0: :c:var:`nrf_sqspi_data_fmt_t.data_container` + :c:var:`nrf_sqspi_data_fmt_t.data_padding` must equal 32.
* Combination of :c:var:`nrf_sqspi_data_fmt_t.data_padding` = 24 and :c:var:`nrf_sqspi_data_fmt_t.data_swap_unit` = 8 when ``clkdiv`` equal or less than 6 (see :ref:`sqspi_timing`) and :c:var:`nrf_sqspi_dev_cfg_t.mspi_lines` is ``NRF_SQSPI_SPI_LINES_DUAL_X_Y_2`` or ``NRF_SQSPI_SPI_LINES_QUAD_X_Y_4`` leads to wrong frequency on SCLK.
* The value calculated as 32 - :c:var:`nrf_sqspi_data_fmt_t.data_padding` must be a multiple of :c:var:`nrf_sqspi_data_fmt_t.data_swap_unit`.
* :c:var:`nrf_sqspi_data_fmt_t.data_swap_unit` values 0 and 4, not supported.
* :c:var:`nrf_sqspi_data_fmt_t.data_bit_order` and :c:var:`nrf_sqspi_data_fmt_t.addr_bit_order` do not support value ``NRF_SQSPI_DATA_FMT_BIT_ORDER_LSB_FIRST``.
