.. _sqspi_timing:

sQSPI and FLPR counters
#######################

The sQSPI API translates the parameter :c:var:`nrf_sqspi_dev_cfg_t.sck_freq_khz` into a value that can be used by FLPR's internal counters.
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
  clkdiv=\frac{max\_device\_freq\_hz}{1000 * nrf\_sqspi\_dev\_cfg\_t.sck\_freq\_khz}

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

   .. tab:: **nRF54L15, without GPIOHSPADCTRL**

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

   .. tab:: **nRF54L15, with GPIOHSPADCTRL**

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
        - 4.865
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

.. note::
   Achieving 64MHz on nRF54L15 is possible when the slave access time plus the flight time of signal propagation
   is less than :math:`\frac{1}{64MHz} - t_{SDI}`. For example, to achieve 64MHz on nRF54L15 using GPIOHSPADCTRL gives a
   maximum slave access time of :math:`15.625ns - 4.865ns = 10.76ns`.

The following timing diagrams provide a visual reference to show how the parameters t\ :sub:`DOV` and t\ :sub:`SDI` relate to SCLK.
SDIO represents the data lines.

.. image:: images/sqspi_timing.png
  :alt: sQSPI timing diagram 1
  :scale: 70%

.. image:: images/sqspi_timing_1.png
  :alt: sQSPI timing diagram 2
  :scale: 80%
