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

+--------------------+---------------------+-----------------+-----------------+------------------+
| Bluetooth activity |      Direction      | nRF52 mean [µs] | nRF53 mean [µs] | nRF54L mean [µs] |
+====================+=====================+=================+=================+==================+
|   advertisement    | 802.15.4→Bluetooth  |       285       |       303       |       277        |
+--------------------+---------------------+-----------------+-----------------+------------------+
|   advertisement    | Bluetooth→802.15.4  |       228       |       276       |       164        |
+--------------------+---------------------+-----------------+-----------------+------------------+
|      scanning      | 802.15.4→Bluetooth  |       269       |       275       |       259        |
+--------------------+---------------------+-----------------+-----------------+------------------+
|      scanning      | Bluetooth→802.15.4  |       196       |       232       |       145        |
+--------------------+---------------------+-----------------+-----------------+------------------+
|     connection     | 802.15.4→Bluetooth  |       367       |       401       |       333        |
+--------------------+---------------------+-----------------+-----------------+------------------+
|     connection     | Bluetooth→802.15.4  |       237       |       305       |       184        |
+--------------------+---------------------+-----------------+-----------------+------------------+

Multiprotocol radio switching time measurements by device family
----------------------------------------------------------------


nRF52 family
^^^^^^^^^^^^

+--------------------+---------------------+---------------------------------+
| Bluetooth activity |      Direction      | Timings (min | mean | max) [µs] |
+====================+=====================+=================================+
|   advertisement    | 802.15.4→Bluetooth  |         259 | 285 | 291         |
+--------------------+---------------------+---------------------------------+
|   advertisement    | Bluetooth→802.15.4  |         227 | 228 | 230         |
+--------------------+---------------------+---------------------------------+
|      scanning      | 802.15.4→Bluetooth  |         242 | 269 | 274         |
+--------------------+---------------------+---------------------------------+
|      scanning      | Bluetooth→802.15.4  |         185 | 196 | 196         |
+--------------------+---------------------+---------------------------------+
|     connection     | 802.15.4→Bluetooth  |         353 | 367 | 380         |
+--------------------+---------------------+---------------------------------+
|     connection     | Bluetooth→802.15.4  |         219 | 237 | 261         |
+--------------------+---------------------+---------------------------------+


nRF53 family
^^^^^^^^^^^^

+--------------------+---------------------+---------------------------------+
| Bluetooth activity |      Direction      | Timings (min | mean | max) [µs] |
+====================+=====================+=================================+
|   advertisement    | 802.15.4→Bluetooth  |         202 | 303 | 315         |
+--------------------+---------------------+---------------------------------+
|   advertisement    | Bluetooth→802.15.4  |         267 | 276 | 277         |
+--------------------+---------------------+---------------------------------+
|      scanning      | 802.15.4→Bluetooth  |         182 | 275 | 293         |
+--------------------+---------------------+---------------------------------+
|      scanning      | Bluetooth→802.15.4  |         230 | 232 | 258         |
+--------------------+---------------------+---------------------------------+
|     connection     | 802.15.4→Bluetooth  |         388 | 401 | 414         |
+--------------------+---------------------+---------------------------------+
|     connection     | Bluetooth→802.15.4  |         291 | 305 | 322         |
+--------------------+---------------------+---------------------------------+


nRF54L family
^^^^^^^^^^^^^

+--------------------+---------------------+---------------------------------+
| Bluetooth activity |      Direction      | Timings (min | mean | max) [µs] |
+====================+=====================+=================================+
|   advertisement    | 802.15.4→Bluetooth  |         277 | 277 | 277         |
+--------------------+---------------------+---------------------------------+
|   advertisement    | Bluetooth→802.15.4  |         161 | 164 | 165         |
+--------------------+---------------------+---------------------------------+
|      scanning      | 802.15.4→Bluetooth  |         258 | 259 | 259         |
+--------------------+---------------------+---------------------------------+
|      scanning      | Bluetooth→802.15.4  |         144 | 145 | 148         |
+--------------------+---------------------+---------------------------------+
|     connection     | 802.15.4→Bluetooth  |         332 | 333 | 333         |
+--------------------+---------------------+---------------------------------+
|     connection     | Bluetooth→802.15.4  |         174 | 184 | 197         |
+--------------------+---------------------+---------------------------------+
