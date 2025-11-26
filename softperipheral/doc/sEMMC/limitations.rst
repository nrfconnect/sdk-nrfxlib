.. _semmc_limitations:

sEMMC limitations
#################

When working with sEMMC, you should be aware of the following limitations.

.. contents::
   :local:
   :depth: 2

v0.1.0
******

Refer to the following detailed descriptions of current limitations:

* sEMMC does not support octal-lane data transmissions.
* sEMMC does not support dual data rate, and as such no support for HS400.
* :c:var:`nrf_semmc_config_t.read_delay` can not be equal to or higher than the :c:var:`clkdiv` parameter in :ref:`semmc_timing`.
* :c:var:`clkdiv` parameter in :ref:`semmc_timing` must be even.
* Maximum frequency is limited to 80MHz on nRF54H Series (assuming a maximum device frequency of 320MHz, in HS200 mode) and 32MHz on nRF54L Series devices.
* A tuning cycle using CMD21 might be needed to set the proper value of :c:var:`nrf_semmc_config_t.read_delay`. 
* Data buffers and response buffers needs to be pointers with a 32-bit aligned address.
* :c:var:`nrf_emmc_data_t.block_size` needs to be minimum 4 bytes for quad-lane data transmissions.
* Setting :c:var:`nrf_semmc_config_t.process_response` to ``NRF_EMMC_RESPONSE_PROC_IGNORE`` will only apply to any transaction in the read direction.
* sEMMC is not able to process response and data at the same time. See for example :ref:`semmc_nrf54H_series_porting_guide_read_response` on how to mitigate this.
* Any prepared transfer with :c:func:`nrf_semmc_cmd_prepare` will be discarded when using :c:func:`nrf_semmc_disable`.
