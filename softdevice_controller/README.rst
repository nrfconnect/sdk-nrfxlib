.. _softdevice_controller:

SoftDevice Controller
###########################

The |controller| is an RTOS-agnostic library built for the Nordic Semiconductor nRF52 and nRF53 Series.
It supports Bluetooth 5. The library utilizes services provided by the Nordic :ref:`mpsl`.

The |controller| is distributed in different variants containing different features.
Variants for the Arm Cortex-M4 processor are available as soft-float, softfp-float, and hard-float.
Variants for the Arm Cortex-M33 processor are available as soft-float only.

SoC support:

+------------+--------+--------+--------+
|            | S112   | S132   | S140   |
+============+========+========+========+
| nRF52805   | Rev 1  |        |        |
+------------+--------+--------+--------+
| nRF52810   | Rev 1  | Rev 1  |        |
|            | Rev 2  | Rev 2  |        |
+------------+--------+--------+--------+
| nRF52811   | Rev 1  | Rev 1  | Rev 1  |
+------------+--------+--------+--------+
| nRF52820   | Rev 2  |        | Rev 2  |
+------------+--------+--------+--------+
| nRF52832   | Rev 2  | Rev 2  |        |
|            |        |        |        |
+------------+--------+--------+--------+
| nRF52833   |        |        | Rev 1  |
+------------+--------+--------+--------+
| nRF52840   |        |        | Rev 2  |
+------------+--------+--------+--------+
| nRF5340    |        |        | Eng A  |
+------------+--------+--------+--------+

|BLE| feature support:

+--------------------------+--------+--------+--------+
|                          | S112   | S132   | S140   |
+==========================+========+========+========+
| 2 Mbps PHY               | X      | X      | X      |
+--------------------------+--------+--------+--------+
| Advertiser               | X      | X      | X      |
+--------------------------+--------+--------+--------+
| Slave                    | X      | X      | X      |
+--------------------------+--------+--------+--------+
| Scanner                  |        | X      | X      |
+--------------------------+--------+--------+--------+
| Master                   |        | X      | X      |
+--------------------------+--------+--------+--------+
| Data Length Extensions   |        | X      | X      |
+--------------------------+--------+--------+--------+
| Advertising Extensions   |        | X      | X      |
+--------------------------+--------+--------+--------+
| Coded PHY (Long Range)   |        |        | X      |
+--------------------------+--------+--------+--------+

Proprietary feature support:

+--------------------------+--------+--------+--------+
|                          | S112   | S132   | S140   |
+==========================+========+========+========+
| Low Latency Packet mode  |        | X      | X      |
+--------------------------+--------+--------+--------+
| QoS Conn Event Reports   | X      | X      | X      |
+--------------------------+--------+--------+--------+

.. note::
   Please note the following when using the library for the nRF5340:

   * The libraries are for evaluation purposes only.
   * They are neither fully functional, nor properly tested.
   * They are not optimized for performance.


.. toctree::
   :maxdepth: 2
   :caption: Subpages:

   CHANGELOG
   doc/softdevice_controller
   doc/api
