.. _nrf_modem_variants:

Variants and features
#####################

The Modem library comes in two variants, each with a specific feature set and support for a specific variant of the nRF91 Series firmware.

The cellular variant of the Modem library supports the nRF91 Series cellular firmware.
The DECT PHY variant of the Modem library supports the nRF91 Series DECT NR+ PHY firmware.

The features of the nRF91 Series firmware that are common across firmware variants, are also common across Modem library variants, for example, SoC management, bootloader, AT commands, and others.

The table below shows which feature and API is supported in which Modem library variant:

+------------------------------+--------------------------------+----------+--------------+
| Feature                      | API header                     | Cellular | DECT NR+ PHY |
+==============================+================================+==========+==============+
| Modem SoC management         | :file:`nrf_modem.h`            | Yes      | Yes          |
+------------------------------+--------------------------------+----------+--------------+
| :ref:`nrf_modem_fault`       | :file:`nrf_modem.h`            | Yes      | Yes          |
+------------------------------+--------------------------------+----------+--------------+
| :ref:`nrf_modem_trace`       | :file:`nrf_modem_trace.h`      | Yes      | Yes          |
+------------------------------+--------------------------------+----------+--------------+
| :ref:`nrf_modem_bootloader`  | :file:`nrf_modem_bootloader.h` | Yes      | Yes          |
+------------------------------+--------------------------------+----------+--------------+
| :ref:`nrf_modem_delta_dfu`   | :file:`nrf_modem_delta_dfu.h`  | Yes      | Yes          |
+------------------------------+--------------------------------+----------+--------------+
| :ref:`nrf_modem_log`         | :file:`nrf_modem_os.h`         | Yes      | Yes          |
+------------------------------+--------------------------------+----------+--------------+
| :ref:`nrf_modem_at`          | :file:`nrf_modem_at.h`         | Yes      | Yes          |
+------------------------------+--------------------------------+----------+--------------+
| :ref:`nrf_modem_dect_phy`    | :file:`nrf_modem_dect_phy.h`   | No       | Yes          |
+------------------------------+--------------------------------+----------+--------------+
| :ref:`nrf_sockets`           | :file:`nrf_socket.h`           | Yes      | No           |
+------------------------------+--------------------------------+----------+--------------+
| :ref:`nrf_modem_gnss`        | :file:`nrf_modem_gnss.h`       | Yes      | No           |
+------------------------------+--------------------------------+----------+--------------+
| :ref:`nrf_modem_softsim`     | :file:`nrf_modem_softsim.h`    | Yes      | No           |
+------------------------------+--------------------------------+----------+--------------+

.. note::
   Although the Modem library AT interface is fully supported by both Modem library variants, the AT command set supported by the cellular and DECT NR+ PHY firmware is different.
