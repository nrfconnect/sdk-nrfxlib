.. _nrf_modem_variants:

Variants and features
#####################

The Modem library comes in two variants, each with a specific feature set and support for the equivalent modem firmware.

The cellular variant of the Modem library supports the following modem firmware:

  * mfw_nrf9160
  * mfw_nrf91x1
  * mfw_nrf9151-ntn

The DECT NR+ variant of the Modem library supports the following modem firmware:

  * mfw-nr+_nrf91x1

Features that are common across modem firmware, are also common across Modem library variants.
The table below shows which feature and API is supported in which Modem library variant:

.. table:: Supported features by Modem library variant and modem firmware.

  +---------------------------------+--------------------------------+---------------------------------------------+-----------------+
  | Feature                         | API header                     | Cellular                                    | DECT NR+        |
  |                                 |                                |                                             |                 |
  |                                 |                                |                                             |                 |
  |                                 |                                +-------------+-------------+-----------------+-----------------+
  |                                 |                                | mfw_nrf9160 | mfw_nrf91x1 | mfw_nrf9151-ntn | mfw-nr+_nrf91x1 |
  +=================================+================================+=============+=============+=================+=================+
  | Modem SoC management            | :file:`nrf_modem.h`            | Yes                                         | Yes             |
  +---------------------------------+--------------------------------+---------------------------------------------+-----------------+
  | :ref:`nrf_modem_fault`          | :file:`nrf_modem.h`            | Yes                                         | Yes             |
  +---------------------------------+--------------------------------+---------------------------------------------+-----------------+
  | :ref:`nrf_modem_trace`          | :file:`nrf_modem_trace.h`      | Yes                                         | Yes             |
  +---------------------------------+--------------------------------+---------------------------------------------+-----------------+
  | :ref:`nrf_modem_bootloader`     | :file:`nrf_modem_bootloader.h` | Yes                                         | Yes             |
  +---------------------------------+--------------------------------+---------------------------------------------+-----------------+
  | :ref:`nrf_modem_delta_dfu`      | :file:`nrf_modem_delta_dfu.h`  | Yes                                         | Yes             |
  +---------------------------------+--------------------------------+---------------------------------------------+-----------------+
  | :ref:`nrf_modem_log`            | :file:`nrf_modem_os.h`         | Yes                                         | Yes             |
  +---------------------------------+--------------------------------+---------------------------------------------+-----------------+
  | :ref:`nrf_modem_at` [1]_        | :file:`nrf_modem_at.h`         | Yes                                         | Yes             |
  +---------------------------------+--------------------------------+---------------------------------------------+-----------------+
  | :ref:`nrf_modem_dect_phy`       | :file:`nrf_modem_dect_phy.h`   | No                                          | Yes             |
  +---------------------------------+--------------------------------+---------------------------------------------+-----------------+
  | :ref:`nrf_sockets`              | :file:`nrf_socket.h`           | Yes                                         | No              |
  +---------------------------------+--------------------------------+---------------------------------------------+-----------------+
  | :ref:`nrf_modem_gnss`           | :file:`nrf_modem_gnss.h`       | Yes                                         | No              |
  +---------------------------------+--------------------------------+-------------+-------------+-----------------+-----------------+
  | :ref:`nrf_modem_softsim`        | :file:`nrf_modem_softsim.h`    | No          | Yes         | Yes             | No              |
  +---------------------------------+--------------------------------+-------------+-------------+-----------------+-----------------+
  | :ref:`nrf_modem_rs_capture_api` | :file:`nrf_modem_rs_capture.h` | No          | Yes         | Yes             | No              |
  +---------------------------------+--------------------------------+-------------+-------------+-----------------+-----------------+

.. [1] Although the Modem library AT interface is fully supported by both Modem library variants, the AT command set supported by the cellular modem firmware and the DECT NR+ modem firmware is different.

.. note::
  The different APIs might have certain subsets of functionality that are only supported by certain modem firmware and modem firmware versions.
  For example, :ref:`nrf_sockets` is supported for all cellular firmware (and Modem library variants), but the socket option ``NRF_SO_SEC_CIPHERSUITE_USED`` is not supported by mfw_nrf9160.
  Exceptions like this are documented with notes like the following in the header files:

    .. code-block:: c

     /**
      * @note This is only supported by the following modem firmware:
      *       - mfw_nrf91x1
      *       - mfw_nrf9151-ntn
      */
