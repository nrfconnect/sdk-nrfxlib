.. _flavour_features:

Flavours and features
#####################

The Modem library comes in two variants, or flavours, each with a specific feature set and support for a specific variant of modem firmware.

The cellular (or LTE) variant of the Modem library supports the nRF91 series cellular modem firmware family.
The DECT PHY variant of the Modem library supports the nRF91 series DECT NR+ PHY modem firmware family.

The features of the modem firmware which are common across modem firmware families, are also common across Modem library flavours, e.g. SoC management, bootloader, AT commands.

The table below shows which feature and API is supported in which Modem library flavour.

+------------------------------+--------------------------------+----------+----------+
| Feature                      | API header                     | Cellular | DECT PHY |
+==============================+================================+==========+==========+
| Modem SoC management         | :file:`nrf_modem.h`            | Yes      | Yes      |
+------------------------------+--------------------------------+----------+----------+
| :ref:`nrf_modem_fault`       | :file:`nrf_modem.h`            | Yes      | Yes      |
+------------------------------+--------------------------------+----------+----------+
| :ref:`nrf_modem_trace`       | :file:`nrf_modem_trace.h`      | Yes      | Yes      |
+------------------------------+--------------------------------+----------+----------+
| :ref:`nrf_modem_bootloader`  | :file:`nrf_modem_bootloader.h` | Yes      | Yes      |
+------------------------------+--------------------------------+----------+----------+
| :ref:`nrf_modem_delta_dfu`   | :file:`nrf_modem_delta_dfu.h`  | Yes      | Yes      |
+------------------------------+--------------------------------+----------+----------+
| :ref:`nrf_modem_log`         | :file:`nrf_modem_os.h`         | Yes      | Yes      |
+------------------------------+--------------------------------+----------+----------+
| :ref:`nrf_modem_at`          | :file:`nrf_modem_at.h`         | Yes      | Yes      |
+------------------------------+--------------------------------+----------+----------+
| :ref:`nrf_modem_gnss`        | :file:`nrf_modem_gnss.h`       | Yes      | No       |
+------------------------------+--------------------------------+----------+----------+
| :ref:`nrf_sockets`           | :file:`nrf_socket.h`           | Yes      | No       |
+------------------------------+--------------------------------+----------+----------+
| :ref:`nrf_modem_softsim`     | :file:`nrf_modem_softsim.h`    | Yes      | No       |
+------------------------------+--------------------------------+----------+----------+
| :ref:`nrf_modem_dect_phy`    | :file:`nrf_modem_dect_phy.h`   | No       | Yes      |
+------------------------------+--------------------------------+----------+----------+

.. note::
   Although the Modem library AT interface is fully supported by both Modem library flavours, the AT command set supported by the cellular and DECT NR+ PHY modem firmware is different.
