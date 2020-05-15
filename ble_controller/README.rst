.. _ble_controller:

nRF Bluetooth LE Controller
###########################

The |controller| is an RTOS-agnostic library built for the Nordic Semiconductor nRF52 and nRF53 Series that supports Bluetooth 5.
In addition to providing a Bluetooth HCI interface, it also provides:

* Multiprotocol support through APIs for requesting RADIO timeslots
* APIs for accessing flash memory concurrently with radio activity
* APIs for controlling the high frequency clock

The |controller| is distributed in different variants containing different features.
All variants are available as soft-float, softfp-float, and hard-float builds.

SoC support:

+------------+--------+--------+--------+
|            | S112   | S132   | S140   |
+============+========+========+========+
| nRF52805   | X      |        |        |
+------------+--------+--------+--------+
| nRF52810   | X      | X      |        |
+------------+--------+--------+--------+
| nRF52811   | X      |        | X      |
+------------+--------+--------+--------+
| nRF52832   | X      | X      |        |
+------------+--------+--------+--------+
| nRF52833   |        |        | X      |
+------------+--------+--------+--------+
| nRF52840   |        |        | X      |
+------------+--------+--------+--------+
| nRF5340    |        |        | X      |
+------------+--------+--------+--------+


Feature support:

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

.. important::
   For nRF5340, the libraries are:

   -  For evaluation purposes only.
   -  Not fully functional and not properly tested.
   -  Not optimized for performance.


.. toctree::
   :maxdepth: 2
   :caption: Contents:

   CHANGELOG
   doc/ble_controller
   doc/api
