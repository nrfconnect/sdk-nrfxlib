:orphan:

.. note::
   The following data and tables are provided for informational purposes only, do not constitute requirements,
   and were obtained from measurements on Nordic DK development kits.


Radio Transition Measurements
-----------------------------

The following measurement uses the nRF21540 Front-End Module (FEM) connected to a Nordic SoC, which provides
essential timing data through GPIO pins.
The RX_EN pin manages the Low-Noise Amplifier (LNA) function of the FEM.
When RX_EN is low (deactivated), the LNA is disabled, indicating that receive operations are ending or the
radio is being deactivated at the end of a time slot.

During protocol transitions, radio switching timing metric is measured in both directions:

Radio switching time:
    802.15.4 to Bluetooth transition:
        * Start: FEM RX_EN pin event (802.15.4 radio turning off on the end of 802.15.4 activity)
        * End: RADIO READY event for Bluetooth operation (Bluetooth LE radio turned on and ready)

    Bluetooth to 802.15.4 transition:
        * Start: RADIO DISABLED event (Bluetooth radio turning off)
        * End: RADIO READY event for 802.15.4 operation (802.15.4 radio turned on and ready)

The measurements are collected during Bluetooth advertisement, scanning, and connection activities while the
nRF 802.15.4 radio driver is in the receive state.


Multiprotocol radio switching time comparison by device family
--------------------------------------------------------------

+--------------------+---------------------+-----------------+-----------------+------------------+
| Bluetooth activity |      Direction      | nRF52 mean [µs] | nRF53 mean [µs] | nRF54L mean [µs] |
+====================+=====================+=================+=================+==================+
|   advertisement    | 802.15.4→Bluetooth  |       324       |       346       |       268        |
+--------------------+---------------------+-----------------+-----------------+------------------+
|   advertisement    | Bluetooth→802.15.4  |       289       |       335       |       172        |
+--------------------+---------------------+-----------------+-----------------+------------------+
|      scanning      | 802.15.4→Bluetooth  |       303       |       315       |       256        |
+--------------------+---------------------+-----------------+-----------------+------------------+
|      scanning      | Bluetooth→802.15.4  |       234       |       292       |       150        |
+--------------------+---------------------+-----------------+-----------------+------------------+
|     connection     | 802.15.4→Bluetooth  |       386       |       388       |       348        |
+--------------------+---------------------+-----------------+-----------------+------------------+
|     connection     | Bluetooth→802.15.4  |       333       |       398       |       211        |
+--------------------+---------------------+-----------------+-----------------+------------------+

Multiprotocol radio switching time measurements by device family
----------------------------------------------------------------


nRF52 family
^^^^^^^^^^^^

+--------------------+---------------------+---------------------------------+
| Bluetooth activity |      Direction      | Timings (min | mean | max) [µs] |
+====================+=====================+=================================+
|   advertisement    | 802.15.4→Bluetooth  |         305 | 324 | 427         |
+--------------------+---------------------+---------------------------------+
|   advertisement    | Bluetooth→802.15.4  |         273 | 289 | 299         |
+--------------------+---------------------+---------------------------------+
|      scanning      | 802.15.4→Bluetooth  |         287 | 303 | 317         |
+--------------------+---------------------+---------------------------------+
|      scanning      | Bluetooth→802.15.4  |         232 | 234 | 239         |
+--------------------+---------------------+---------------------------------+
|     connection     | 802.15.4→Bluetooth  |         372 | 386 | 398         |
+--------------------+---------------------+---------------------------------+
|     connection     | Bluetooth→802.15.4  |         312 | 333 | 354         |
+--------------------+---------------------+---------------------------------+


nRF53 family
^^^^^^^^^^^^

+--------------------+---------------------+---------------------------------+
| Bluetooth activity |      Direction      | Timings (min | mean | max) [µs] |
+====================+=====================+=================================+
|   advertisement    | 802.15.4→Bluetooth  |         320 | 346 | 505         |
+--------------------+---------------------+---------------------------------+
|   advertisement    | Bluetooth→802.15.4  |         324 | 335 | 352         |
+--------------------+---------------------+---------------------------------+
|      scanning      | 802.15.4→Bluetooth  |         298 | 315 | 328         |
+--------------------+---------------------+---------------------------------+
|      scanning      | Bluetooth→802.15.4  |         290 | 292 | 305         |
+--------------------+---------------------+---------------------------------+
|     connection     | 802.15.4→Bluetooth  |         375 | 388 | 401         |
+--------------------+---------------------+---------------------------------+
|     connection     | Bluetooth→802.15.4  |         379 | 398 | 418         |
+--------------------+---------------------+---------------------------------+


nRF54L family
^^^^^^^^^^^^^

+--------------------+---------------------+---------------------------------+
| Bluetooth activity |      Direction      | Timings (min | mean | max) [µs] |
+====================+=====================+=================================+
|   advertisement    | 802.15.4→Bluetooth  |         184 | 268 | 273         |
+--------------------+---------------------+---------------------------------+
|   advertisement    | Bluetooth→802.15.4  |         171 | 172 | 173         |
+--------------------+---------------------+---------------------------------+
|      scanning      | 802.15.4→Bluetooth  |         255 | 256 | 257         |
+--------------------+---------------------+---------------------------------+
|      scanning      | Bluetooth→802.15.4  |         149 | 150 | 150         |
+--------------------+---------------------+---------------------------------+
|     connection     | 802.15.4→Bluetooth  |         345 | 348 | 348         |
+--------------------+---------------------+---------------------------------+
|     connection     | Bluetooth→802.15.4  |         195 | 211 | 225         |
+--------------------+---------------------+---------------------------------+