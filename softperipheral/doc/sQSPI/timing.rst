.. _sqspi_timing:

sQSPI and FLPR counters
#######################

sQSPI API translates parameter :c:var:`nrf_sqspi_dev_cfg_t.sck_freq_khz` into a value that can be used by FLPR's internal counters.
The translation has the following constraints (assuming the SoC is running at highest base clock frequency):

.. tabs::

   .. tab:: **nRF54L15**
      .. math::
         clkdiv=\frac{128000000}{1000 * nrf\_sqspi\_dev\_cfg\_t.sck\_freq\_khz}

   .. tab:: **nRF54H20**
      .. math::
         clkdiv=\frac{320000000}{1000 * nrf\_sqspi\_dev\_cfg\_t.sck\_freq\_khz}

.. math::
  FLPR\_counter=
  \begin{cases}
      \frac{clkdiv}{2}-1,& \text{if } clkdiv > 2\\
      0,              & \text{otherwise}
  \end{cases}

Timing parameters
#################

When working with sQSPI, you should be aware of the associated timing parameters. 

.. contents::
   :local:
   :depth: 2

.. tabs::

   .. tab:: **nRF54L15**

    .. list-table::
      :widths: 20 20 10 10 10 10
      :header-rows: 1

      * - Symbol
        - Description
        - Min
        - Typ
        - Max
        - Units
      * - F\ :sub:`sQSPI,SCLK`
        - SCLK frequency
        -
        -
        - 64
        - MHz
      * - DC\ :sub:`sQSPI,SCLK`
        - SCLK duty cycle
        - 
        - 50
        -
        - %
      * - t\ :sub:`DOV`
        - Data out valid
        -
        -
        - 2.2
        - ns
      * - t\ :sub:`SDI`
        - Data in setup time
        -
        -
        - 11.7
        - ns

   .. tab:: **nRF54H20**

    .. list-table::
      :widths: 20 20 10 10 10 10
      :header-rows: 1

      * - Symbol
        - Description
        - Min
        - Typ
        - Max
        - Units
      * - F\ :sub:`sQSPI,SCLK`
        - SCLK frequency
        -
        -
        - 80
        - MHz
      * - DC\ :sub:`sQSPI,SCLK`
        - SCLK duty cycle
        -
        - 50
        -
        - %
      * - t\ :sub:`DOV`
        - Data out valid
        -
        -
        -
        - ns
      * - t\ :sub:`SDI`
        - Data in setup time
        -
        -
        -
        - ns

The following timing diagrams provide a visual reference to show how the parameters t\ :sub:`DOV` and t\ :sub:`SDI` relate to SCLK.
SDIO represents the data lines.

.. image:: images/sqspi_timing.png
  :alt: sQSPI timing diagram 1
  :scale: 70%

.. image:: images/sqspi_timing_1.png
  :alt: sQSPI timing diagram 2
  :scale: 80%
