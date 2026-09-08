.. _scan_timing:

sCAN baudrate settings
######################

The sCAN API exposes baudrate settings through :c:struct:`nrf_scan_timing_t`, which in turn is used by :c:func:`nrf_scan_timing`.
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

A time quanta is the base unit for the configuration parameters

.. math::

   tq = \left\lfloor \frac{\text{max_device_freq_hz}}
        {\text{nrf_scan_timing_t.prescaler} \cdot \text{desired_baud_rate}} \right\rceil


This is an example configuration for baudrate of 1Mbps:

.. code-block:: c

   nrf_scan_timing_t m_scan_timing = {
       .prescaler = 8, .prop_seg = 1, .phase_seg1 = 7, .phase_seg2 = 7, .sjw = 1};
