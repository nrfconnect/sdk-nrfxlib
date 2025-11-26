.. _nrf_modem_variants:

Variants and features
#####################

The Modem library comes in two *variants*, each with a specific feature set and support for the equivalent *modem firmware product*.

The cellular variant of the Modem library supports the following modem firmware products:

  * mfw_nrf9160
  * mfw_nrf91x1
  * mfw_nrf9151-ntn

The DECT NR+ variant of the Modem library supports the following modem firmware products:

  * mfw-nr+_nrf91x1

Features that are common across *modem firmware products*, are also common across *Modem library variants*.
The table below shows which feature and API is supported in which Modem library variant:

.. table:: Supported features by Modem library variant and modem firmware product.

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
  | :ref:`nrf_modem_at` [#at-note]_ | :file:`nrf_modem_at.h`         | Yes                                         | Yes             |
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

.. [#at-note] Although the Modem library AT interface is fully supported by both Modem library variants, the AT command set supported by the cellular and DECT NR+ modem firmware products is different.

.. note::
  The different APIs may have certain subsets of functionality that are only supported by certain modem firmware products and versions.
  For example, :ref:`nrf_sockets` is supported for all cellular firmware products (and Modem library variants), but the socket option ``NRF_SO_SEC_CIPHERSUITE_USED`` is not suported by mfw_nrf9160.
  Exceptions like this are documented with notes like the one below:

    .. code-block:: c

     /**
      * @note This is only supported by the following modem firmware products:
      *       - mfw_nrf91x1
      *       - mfw_nrf9151-ntn
      */
