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
|   advertisement    | 802.15.4→Bluetooth  |       280       |       273       |       279        |
+--------------------+---------------------+-----------------+-----------------+------------------+
|   advertisement    | Bluetooth→802.15.4  |       204       |       290       |       158        |
+--------------------+---------------------+-----------------+-----------------+------------------+
|      scanning      | 802.15.4→Bluetooth  |       269       |       242       |       261        |
+--------------------+---------------------+-----------------+-----------------+------------------+
|      scanning      | Bluetooth→802.15.4  |       178       |       237       |       137        |
+--------------------+---------------------+-----------------+-----------------+------------------+
|     connection     | 802.15.4→Bluetooth  |       356       |       391       |       335        |
+--------------------+---------------------+-----------------+-----------------+------------------+
|     connection     | Bluetooth→802.15.4  |       267       |       339       |       198        |
+--------------------+---------------------+-----------------+-----------------+------------------+

Multiprotocol radio switching time measurements by device family
----------------------------------------------------------------


nRF52 family
^^^^^^^^^^^^

+--------------------+---------------------+---------------------------------+
| Bluetooth activity |      Direction      | Timings (min | mean | max) [µs] |
+====================+=====================+=================================+
|   advertisement    | 802.15.4→Bluetooth  |         274 | 280 | 305         |
+--------------------+---------------------+---------------------------------+
|   advertisement    | Bluetooth→802.15.4  |         203 | 204 | 204         |
+--------------------+---------------------+---------------------------------+
|      scanning      | 802.15.4→Bluetooth  |         257 | 269 | 289         |
+--------------------+---------------------+---------------------------------+
|      scanning      | Bluetooth→802.15.4  |         174 | 178 | 179         |
+--------------------+---------------------+---------------------------------+
|     connection     | 802.15.4→Bluetooth  |         342 | 356 | 369         |
+--------------------+---------------------+---------------------------------+
|     connection     | Bluetooth→802.15.4  |         250 | 267 | 295         |
+--------------------+---------------------+---------------------------------+


nRF53 family
^^^^^^^^^^^^

+--------------------+---------------------+---------------------------------+
| Bluetooth activity |      Direction      | Timings (min | mean | max) [µs] |
+====================+=====================+=================================+
|   advertisement    | 802.15.4→Bluetooth  |         233 | 273 | 325         |
+--------------------+---------------------+---------------------------------+
|   advertisement    | Bluetooth→802.15.4  |         288 | 290 | 291         |
+--------------------+---------------------+---------------------------------+
|      scanning      | 802.15.4→Bluetooth  |         211 | 242 | 304         |
+--------------------+---------------------+---------------------------------+
|      scanning      | Bluetooth→802.15.4  |         235 | 237 | 240         |
+--------------------+---------------------+---------------------------------+
|     connection     | 802.15.4→Bluetooth  |         378 | 391 | 404         |
+--------------------+---------------------+---------------------------------+
|     connection     | Bluetooth→802.15.4  |         324 | 339 | 361         |
+--------------------+---------------------+---------------------------------+


nRF54L family
^^^^^^^^^^^^^

+--------------------+---------------------+---------------------------------+
| Bluetooth activity |      Direction      | Timings (min | mean | max) [µs] |
+====================+=====================+=================================+
|   advertisement    | 802.15.4→Bluetooth  |         276 | 279 | 280         |
+--------------------+---------------------+---------------------------------+
|   advertisement    | Bluetooth→802.15.4  |         155 | 158 | 159         |
+--------------------+---------------------+---------------------------------+
|      scanning      | 802.15.4→Bluetooth  |         259 | 261 | 262         |
+--------------------+---------------------+---------------------------------+
|      scanning      | Bluetooth→802.15.4  |         136 | 137 | 137         |
+--------------------+---------------------+---------------------------------+
|     connection     | 802.15.4→Bluetooth  |         333 | 335 | 335         |
+--------------------+---------------------+---------------------------------+
|     connection     | Bluetooth→802.15.4  |         193 | 198 | 212         |
+--------------------+---------------------+---------------------------------+
