.. _softdevice_controller:

SoftDevice Controller
#####################

The |controller| is an RTOS-agnostic library built for the Nordic Semiconductor devices that support Bluetooth.
The library utilizes services provided by the :ref:`mpsl`.

The |controller| is distributed in different variants containing different features.
The libraries are available as soft-float, softfp-float, and hard-float builds for the nRF52, nRF54H and nRF54L Series, and as a soft-float build for the nRF53 Series.

|BLE| feature support:

+--------------------------------+-----------------+--------------+-----------+
|                                | Peripheral-only | Central-only | Multirole |
+================================+=================+==============+===========+
| 2 Mbps PHY                     | X               | X            | X         |
+--------------------------------+-----------------+--------------+-----------+
| Advertiser                     | X               |              | X         |
+--------------------------------+-----------------+--------------+-----------+
| Slave                          | X               |              | X         |
+--------------------------------+-----------------+--------------+-----------+
| Scanner                        |                 | X            | X         |
+--------------------------------+-----------------+--------------+-----------+
| Master                         |                 | X            | X         |
+--------------------------------+-----------------+--------------+-----------+
| Data Length Extensions         | X               | X            | X         |
+--------------------------------+-----------------+--------------+-----------+
| Advertising Extensions         |                 |              | X         |
+--------------------------------+-----------------+--------------+-----------+
| Periodic Advertising           |                 |              | X         |
+--------------------------------+-----------------+--------------+-----------+
| Connectionless CTE Advertising |                 |              | X         |
+--------------------------------+-----------------+--------------+-----------+
| Connection CTE Response        |                 |              | X         |
+--------------------------------+-----------------+--------------+-----------+
| Coded PHY (Long Range)         |                 |              | X         |
+--------------------------------+-----------------+--------------+-----------+
| LE Power Control Request       | X               | X            | X         |
+--------------------------------+-----------------+--------------+-----------+
| Periodic Advertising Sync      |                 |              | X         |
| Transfer - Sender              |                 |              |           |
+--------------------------------+-----------------+--------------+-----------+
| Periodic Advertising Sync      |                 |              | X         |
| Transfer - Receiver            |                 |              |           |
+--------------------------------+-----------------+--------------+-----------+
| Sleep Clock Accuracy Updates   |                 |              | X         |
+--------------------------------+-----------------+--------------+-----------+
| Periodic Advertising with      |                 |              | X         |
| Responses - Advertiser         |                 |              |           |
+--------------------------------+-----------------+--------------+-----------+
| Periodic Advertising with      |                 |              | X         |
| Responses - Scanner            |                 |              |           |
+--------------------------------+-----------------+--------------+-----------+
| Connected Isochronous Stream   |                 |              | X         |
| - Central                      |                 |              |           |
+--------------------------------+-----------------+--------------+-----------+
| Connected Isochronous Stream   |                 |              | X         |
| - Peripheral                   |                 |              |           |
+--------------------------------+-----------------+--------------+-----------+
| Isochronous Broadcaster        |                 |              | X         |
+--------------------------------+-----------------+--------------+-----------+
| Synchronized Receiver          |                 |              | X         |
+--------------------------------+-----------------+--------------+-----------+

.. note::
   The following limitations apply to the listed features:

   * For Connectionless CTE Advertising, angle of arrival (AoA) is supported, but angle of departure (AoD) is not.
   * For Connection CTE Response, angle of arrival (AoA) is supported, but angle of departure (AoD) is not.
   * For Periodic Advertising Sync Transfer - Receiver, only one sync transfer reception may be in progress at any one time per link.
   * For the Isochronous Channels features, nRF52820 and nRF52833 are the nRF52 Series devices that support encrypting and decrypting the Isochronous Channels packets.
   * For the nRF54H and nRF54L series, all Bluetooth features are experimental.

.. _sdc_proprietary_feature_support:

Proprietary feature support:

+--------------------------+-----------------+--------------+-----------+-----------------------------------------------------------------------------+
|                          | Peripheral-only | Central-only | Multirole | Description                                                                 |
+==========================+=================+==============+===========+=============================================================================+
| Low Latency Packet mode  |                 |              | X         | Enables using connection intervals below 7.5 ms                             |
+--------------------------+-----------------+--------------+-----------+-----------------------------------------------------------------------------+
| QoS Conn Event Reports   | X               | X            | X         | Reports QoS for every connection event.                                     |
|                          |                 |              |           | The application can then set an adapted channel map to avoid busy channels. |
+--------------------------+-----------------+--------------+-----------+-----------------------------------------------------------------------------+
| QoS Channel Survey       |                 | X            | X         | Provides measurements of the energy levels on the Bluetooth LE channels.    |
| (experimental support)   |                 |              |           | The application can then set an adapted channel map to avoid busy channels. |
+--------------------------+-----------------+--------------+-----------+-----------------------------------------------------------------------------+


.. note::

   Low Latency Packet mode is not supported on the nRF53 Series.

.. toctree::
   :maxdepth: 2
   :caption: Subpages:

   doc/softdevice_controller
   doc/scheduling
   doc/isochronous_channels
   limitations
   CHANGELOG
   doc/api
