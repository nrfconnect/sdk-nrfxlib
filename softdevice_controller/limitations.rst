.. softdevice_controller_limitations:

Limitations
###########

When working with the SoftDevice Controller, you should be aware of the following limitations.
In addition, see :ref:`nrf:known_issues` for temporary issues that will be fixed in future releases.

YOPAN-87: The RSSI value reported by the Softdevice Controller requires additional temperature compensation.
  The RSSI value reported by the Softdevice Controller is the raw value from the radio peripheral.
  Some SoCs require compensation of the RSSI value based on the chip temperature.
  See the Errata document for the respective SoC for detailed information.

  **Workaround:**
  For the nRF53 Series, you can retrieve the chip temperature by reading the value of the temperature peripheral on the network core.
  To do this with the SoftDevice Controller, use the Zephyr HCI VS Read Chip Temperature command (``BT_HCI_OP_VS_READ_CHIP_TEMP``).

  For the nRF52 Series, you can use the Zephyr sensor API instead of the HCI command to retrieve the chip temperature.

  You can then use the retrieved temperature value to compensate the raw RSSI value, following the workaround in the Errata document.

DRGN-8628: Setting Filter_Duplicates in LE Set Scan Enable and LE Set Extended Scan Enable commands does not take effect
  The host receives all advertisements from advertisers in advertising report events even if Filter_Duplicates is set in LE Set Scan Enable and LE Set Extended Scan Enable.
  Filter_Duplicates cannot guarantee that all duplicate advertising reports are filtered out, because the controller has a limited "cache" of known advertising reports.

  **Workaround:** Filter out the duplicate advertising reports in the application.
