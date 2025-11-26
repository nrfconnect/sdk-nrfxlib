.. _semmc_timing:

sEMMC and FLPR counters
#######################

The sEMMC API translates the parameter :c:var:`nrf_semmc_config_t.clk_freq_hz` into a value that can be used by FLPR's internal counters.
The translation has the following constraints (assuming the SoC is running at highest base clock frequency):

.. tabs::

   .. tab:: **nRF54L15**
      .. math::
         max\_device\_freq\_hz=128000000

   .. tab:: **nRF54LM20**
      .. math::
         max\_device\_freq\_hz=128000000

   .. tab:: **nRF54H20**
      .. math::
         max\_device\_freq\_hz=320000000

The maximum core frequency of the device translates to a clock divider used in FLPR's internal counters:

.. math::
  clkdiv=\frac{max\_device\_freq\_hz}{nrf\_semmc\_config\_t.clk\_freq\_hz}

.. math::
  FLPR\_counter=
  \begin{cases}
      \frac{clkdiv}{2}-1,& \text{if } clkdiv > 2\\
      0,              & \text{otherwise}
  \end{cases}
