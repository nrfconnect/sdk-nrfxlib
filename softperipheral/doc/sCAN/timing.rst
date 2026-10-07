.. _scan_timing:

sCAN baud rate settings
#######################

.. contents::
   :local:
   :depth: 2

The sCAN API exposes baud rate settings through :c:struct:`nrf_scan_timing_t`, which is passed to :c:func:`nrf_scan_timing`.
These parameters are translated into a value for FLPR's internal counters.

Timing calculation
******************

The parameter calculation follows CAN Specification 2.0 A/B and the CiA99 paper.

When the SoC runs at its highest base clock frequency, the FLPR frequency is as follows:

.. tabs::

   .. tab:: **nRF54L15**
      .. math::
         \text{max_device_freq_hz}=128000000

   .. tab:: **nRF54LM20**
      .. math::
         \text{max_device_freq_hz}=128000000

A time quantum is the base unit for the configuration parameters.

.. math::

   \text{tq} = \left\lfloor \frac{\text{max_device_freq_hz}}
        {\text{nrf_scan_timing_t.prescaler} \cdot \text{desired_baud_rate}} \right\rceil


The timing segments make up each transmitted bit.
The total number of time quanta in each transmitted bit is calculated as follows:

.. math::

   \text{num_tq_per_bit} = 1 + \text{prop_seg} + \text{phase_seg1} + \text{phase_seg2}

Timing constraints
******************

The timing parameters must meet the following constraints:

* ``prescaler`` must be at least ``8``.
* ``prop_seg`` and ``phase_seg1`` must each be between ``1`` and ``8``.
* ``phase_seg2`` must be between ``1`` and ``phase_seg1``.
* The sum of ``prop_seg``, ``phase_seg1``, and ``phase_seg2`` must be at least ``7``.
* ``sjw`` must be between ``1`` and the lower of ``phase_seg1`` and ``4``.

Example configuration
*********************

The following example configures a baud rate of 1 Mbps:

.. code-block:: c

   nrf_scan_timing_t m_scan_timing = {
       .prescaler = 8,
       .prop_seg = 1,
       .phase_seg1 = 7,
       .phase_seg2 = 7,
       .sjw = 1
   };
