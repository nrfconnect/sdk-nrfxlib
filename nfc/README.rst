.. _nfc:

Near Field Communication (NFC)
##############################

Near Field Communication (NFC) is a technology for wireless transfer of small amounts of data between two devices.
The provided NFC libraries are RTOS-agnostic and built for Nordic Semiconductor SoCs.

See the :ref:`nrf:ug_nfc` user guide for information about how to use the libraries in the |NCS|.

The following libraries are available:

:ref:`Type 2 Tag <nrf:lib_nfc_t2t>`
  Supports NFC-A Type 2 Tag in read-only mode.
  See :ref:`type_2_tag` for more information.

:ref:`Type 4 Tag <nrf:lib_nfc_t4t>`
  Supports NFC-A Type 4 Tag.
  See :ref:`type_4_tag` for more information.

Each library is distributed in both soft-float and hard-float builds.

.. important::
   The libraries are for evaluation purposes only.


.. toctree::
   :maxdepth: 2
   :caption: Subpages:

   doc/integration_notes
   doc/type_2_tag
   doc/type_4_tag
   CHANGELOG
