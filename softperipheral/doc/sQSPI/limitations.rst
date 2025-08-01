.. _sqspi_limitations:

Limitations
###########

When working with sQSPI, you should be aware of the following limitations.

.. contents::
   :local:
   :depth: 2

v1.0.0
******

Refer to the following detailed descriptions of current limitations:

* sQSPI does not support slave mode operations; it can only operate as a controller (master).
* The sQSPI support for SPI half-duplex setup is implemented but not tested.
* sQSPI SPI modes 1,2, and 3 (see the :c:var:`nrf_sqspi_dev_cfg_t.spi_cpolpha` configuration structure) may present artifacts (delays and clock stretching) on the last SCLK cycle during a transfer.
* The nrfx API for sQSPI does not support configuring the use of the positive or negative edge of SCLK delayed read sampling.
* sQSPI employs the :c:var:`nrf_sqspi_dev_cfg_t.sample_delay_cyc` parameter as an offset to FLPR's base clock counter , not SLCK clock cycles (see the :c:struct:`nrf_sqspi_dev_cfg_t` struct).
* sQSPI :c:var:`nrf_sqspi_dev_cfg_t.sample_delay_cyc` has a limit:

  .. math::
     nrf\_sqspi\_dev\_cfg\_t.sample\_delay\_cyc <= \frac{FLPR_{base\_freq}}{2*nrf\_sqspi\_dev\_cfg\_t.sck\_freq\_khz} - 1

* Implementation of sQSPI quad or dual lane for command transmission (for example, ``2_2_2`` and ``4_4_4`` modes) is implemented but has not been tested.
* sQSPI does not support a configurable pin for the CSN line; only **PIN 5** is supported (see :c:var:`nrf_sqspi_dev_cfg_t.csn_pin`).
