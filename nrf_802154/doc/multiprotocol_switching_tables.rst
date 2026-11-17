:orphan:

.. note::
   The following data and tables are provided for informational purposes only, do not constitute requirements,
   and were obtained from measurements on Nordic development kits.


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

+--------------------+---------------------+-----------------+------------------+
| Bluetooth activity |      Direction      | nRF53 mean [µs] | nRF54L mean [µs] |
+====================+=====================+=================+==================+
|   advertisement    | 802.15.4→Bluetooth  |       198       |       214        |
+--------------------+---------------------+-----------------+------------------+
|   advertisement    | Bluetooth→802.15.4  |       260       |       147        |
+--------------------+---------------------+-----------------+------------------+
|      scanning      | 802.15.4→Bluetooth  |       180       |       194        |
+--------------------+---------------------+-----------------+------------------+
|      scanning      | Bluetooth→802.15.4  |       228       |       129        |
+--------------------+---------------------+-----------------+------------------+
|     connection     | 802.15.4→Bluetooth  |       314       |       293        |
+--------------------+---------------------+-----------------+------------------+
|     connection     | Bluetooth→802.15.4  |       294       |       168        |
+--------------------+---------------------+-----------------+------------------+

Multiprotocol radio switching time measurements by device family
----------------------------------------------------------------


nRF53 family
^^^^^^^^^^^^

+--------------------+---------------------+---------------------------------+
| Bluetooth activity |      Direction      | Timings (min | mean | max) [µs] |
+====================+=====================+=================================+
|   advertisement    | 802.15.4→Bluetooth  |         197 | 198 | 198         |
+--------------------+---------------------+---------------------------------+
|   advertisement    | Bluetooth→802.15.4  |         258 | 260 | 260         |
+--------------------+---------------------+---------------------------------+
|      scanning      | 802.15.4→Bluetooth  |         179 | 180 | 181         |
+--------------------+---------------------+---------------------------------+
|      scanning      | Bluetooth→802.15.4  |         227 | 228 | 229         |
+--------------------+---------------------+---------------------------------+
|     connection     | 802.15.4→Bluetooth  |         301 | 314 | 327         |
+--------------------+---------------------+---------------------------------+
|     connection     | Bluetooth→802.15.4  |         281 | 294 | 312         |
+--------------------+---------------------+---------------------------------+


nRF54L family
^^^^^^^^^^^^^

+--------------------+---------------------+---------------------------------+
| Bluetooth activity |      Direction      | Timings (min | mean | max) [µs] |
+====================+=====================+=================================+
|   advertisement    | 802.15.4→Bluetooth  |         213 | 214 | 214         |
+--------------------+---------------------+---------------------------------+
|   advertisement    | Bluetooth→802.15.4  |         146 | 147 | 148         |
+--------------------+---------------------+---------------------------------+
|      scanning      | 802.15.4→Bluetooth  |         193 | 194 | 195         |
+--------------------+---------------------+---------------------------------+
|      scanning      | Bluetooth→802.15.4  |         128 | 129 | 131         |
+--------------------+---------------------+---------------------------------+
|     connection     | 802.15.4→Bluetooth  |         290 | 293 | 294         |
+--------------------+---------------------+---------------------------------+
|     connection     | Bluetooth→802.15.4  |         158 | 168 | 181         |
+--------------------+---------------------+---------------------------------+
