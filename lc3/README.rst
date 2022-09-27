.. _lc3:

Low Complexity Communication Codec (LC3)
########################################

.. contents::
   :local:
   :depth: 2

Low Complexity Communication Codec (LC3) is the default software codec for the :ref:`nrf53_audio_app` application, conformant to the `Bluetooth® LE Audio specifications`_ (Bluetooth 5.2, QDID #156294).
For more information about the codec usage in the application, see the application documentation page in the |NCS| documentation.

The nrfxlib module includes the :file:`lc3/include/sw_codec_lc3.h` and :file:`lc3/src/sw_codec_lc3.c` files that form a translation layer for using a standardized API in the nRF5340 Audio application.

.. note::
   For the terms and conditions for using the LC3 codec, see :file:`license.txt` and :file:`attribution.txt` at :file:`lc3/`.

.. toctree::
   :maxdepth: 1
   :caption: Subpages:

   CHANGELOG
   doc/api
