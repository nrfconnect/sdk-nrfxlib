.. _scan_timing:

sCAN baud rate settings
#######################

The sCAN API exposes baud rate settings through :c:struct:`nrf_scan_timing_t`, which is passed to :c:func:`nrf_scan_timing`.
These parameters are translated into a value for FLPR's internal counters.

Parameter calculation follows the CAN Specification 2.0 A/B and paper CiA99.

Assuming the SoC is running at highest base clock frequency, FLPR's frequency is:

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


Configuration parameters make up a transmitted bit.
The number of time quanta per each transmitted bit is calculated as follows:

.. math::

   \text{num_tq_per_bit} = 1 + \text{prop_seg} + \text{phase_seg1} + \text{phase_seg2}

The timing parameters must meet the following constraints:

* ``prescaler`` must be at least ``8``.
* ``prop_seg`` and ``phase_seg1`` must each be between ``1`` and ``8``.
* ``phase_seg2`` must be between ``1`` and ``phase_seg1``.
* The sum of ``prop_seg``, ``phase_seg1``, and ``phase_seg2`` must be at least ``7``.
* ``sjw`` must be between ``1`` and the lower of ``phase_seg1`` and ``4``.

The following is an example configuration for a baud rate of 1 Mbps:

.. code-block:: c

   nrf_scan_timing_t m_scan_timing = {
       .prescaler = 8, .prop_seg = 1, .phase_seg1 = 7, .phase_seg2 = 7, .sjw = 1};
