.. _nrf_fuel_gauge:

nRF Fuel Gauge library
######################

.. contents::
   :local:
   :depth: 2

nRF Fuel Gauge processes battery measurements made by Power Management ICs (PMICs) and provides a state-of-charge (SOC) prediction, along with other metrics.
The library is meant for use with devices from the Nordic nPM Family of PMICs.
For the most accurate results, use a battery model that matches the battery used in the product.

The library supports primary cell (non-rechargeable) and secondary cell (rechargeable) batteries.
Several primary cell battery models are included in the library, such as Alkaline AA, Alkaline AAA, Alkaline coin cell LR44, and Lithium-manganese dioxide coin cell CR2032 batteries.
Secondary cell battery models can be derived using the nPM Powerup App in `nRF Connect for Desktop`_ and `nPM1300 Evaluation Kit`_.

See :ref:`fuel_gauge_api` for detailed usage.

.. toctree::
   :maxdepth: 1
   :caption: Subpages:

   CHANGELOG
   doc/api
