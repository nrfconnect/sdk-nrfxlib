:orphan:

Disclaimer
----------
.. note::
   The following data and tables are provided for informational purposes only, do not constitute requirements,
   and were obtained from measurements on Nordic DK development kits.


Radio Transition Measurements
------------------------------------------

The following measurement utilizes the nRF21540 Front-End Module (FEM) connected to a Nordic SoC, which provides
essential timing data via GPIO pins. The RX_EN pin manages the Low-Noise Amplifier (LNA) function of the
FEM. When RX_EN is low (deactivated), the LNA is disabled, signaling that receive operations are ending or the
radio is being deactivated at the conclusion of a time slot.

During protocol transitions, radio switching timing metric is measured in both directions:

Radio Switching Time:
    802.15.4 to BLE Transition:
        - Start: FEM RX_EN pin event (802.15.4 radio turning off on the end of 802.15.4 activity)
        - End: RADIO READY event for BLE operation (ble radio turned on and ready)

    BLE to 802.15.4 Transition:
        - Start: RADIO DISABLED event (BLE radio turning off)
        - End: RADIO READY event for 802.15.4 operation (802.15.4 radio turned on and ready)

The measurements are collected during BLE advertisement, scanning, and connection activities during 802.15.4
radio state set to receive mode.


Multiprotocol radio switching time comparison by device family
--------------------------------------------------------------

+-------------------+---------------+-----------------+-----------------+------------------+
| BLE activity name |   Direction   | nRF52 mean [us] | nRF53 mean [us] | nRF54L mean [us] |
+===================+===============+=================+=================+==================+
|   advertisement   | 802.15.4→BLE  |       324       |       346       |       268        |
+-------------------+---------------+-----------------+-----------------+------------------+
|   advertisement   | BLE→802.15.4  |       289       |       335       |       172        |
+-------------------+---------------+-----------------+-----------------+------------------+
|     scanning      | 802.15.4→BLE  |       303       |       315       |       256        |
+-------------------+---------------+-----------------+-----------------+------------------+
|     scanning      | BLE→802.15.4  |       234       |       292       |       150        |
+-------------------+---------------+-----------------+-----------------+------------------+
|    connection     | 802.15.4→BLE  |       386       |       388       |       348        |
+-------------------+---------------+-----------------+-----------------+------------------+
|    connection     | BLE→802.15.4  |       333       |       398       |       211        |
+-------------------+---------------+-----------------+-----------------+------------------+

Multiprotocol radio switching time measurements by device family
----------------------------------------------------------------


nRF52 family
^^^^^^^^^^^^
+-------------------+---------------+---------------------------------+
| BLE activity name |   Direction   | Timings (min | mean | max) [us] |
+===================+===============+=================================+
|   advertisement   | 802.15.4→BLE  |         305 | 324 | 427         |
+-------------------+---------------+---------------------------------+
|   advertisement   | BLE→802.15.4  |         273 | 289 | 299         |
+-------------------+---------------+---------------------------------+
|     scanning      | 802.15.4→BLE  |         287 | 303 | 317         |
+-------------------+---------------+---------------------------------+
|     scanning      | BLE→802.15.4  |         232 | 234 | 239         |
+-------------------+---------------+---------------------------------+
|    connection     | 802.15.4→BLE  |         372 | 386 | 398         |
+-------------------+---------------+---------------------------------+
|    connection     | BLE→802.15.4  |         312 | 333 | 354         |
+-------------------+---------------+---------------------------------+


nRF53 family
^^^^^^^^^^^^
+-------------------+---------------+---------------------------------+
| BLE activity name |   Direction   | Timings (min | mean | max) [us] |
+===================+===============+=================================+
|   advertisement   | 802.15.4→BLE  |         320 | 346 | 505         |
+-------------------+---------------+---------------------------------+
|   advertisement   | BLE→802.15.4  |         324 | 335 | 352         |
+-------------------+---------------+---------------------------------+
|     scanning      | 802.15.4→BLE  |         298 | 315 | 328         |
+-------------------+---------------+---------------------------------+
|     scanning      | BLE→802.15.4  |         290 | 292 | 305         |
+-------------------+---------------+---------------------------------+
|    connection     | 802.15.4→BLE  |         375 | 388 | 401         |
+-------------------+---------------+---------------------------------+
|    connection     | BLE→802.15.4  |         379 | 398 | 418         |
+-------------------+---------------+---------------------------------+


nRF54L family
^^^^^^^^^^^^^
+-------------------+---------------+---------------------------------+
| BLE activity name |   Direction   | Timings (min | mean | max) [us] |
+===================+===============+=================================+
|   advertisement   | 802.15.4→BLE  |         184 | 268 | 273         |
+-------------------+---------------+---------------------------------+
|   advertisement   | BLE→802.15.4  |         171 | 172 | 173         |
+-------------------+---------------+---------------------------------+
|     scanning      | 802.15.4→BLE  |         255 | 256 | 257         |
+-------------------+---------------+---------------------------------+
|     scanning      | BLE→802.15.4  |         149 | 150 | 150         |
+-------------------+---------------+---------------------------------+
|    connection     | 802.15.4→BLE  |         345 | 348 | 348         |
+-------------------+---------------+---------------------------------+
|    connection     | BLE→802.15.4  |         195 | 211 | 225         |
+-------------------+---------------+---------------------------------+