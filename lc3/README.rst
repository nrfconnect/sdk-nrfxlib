.. _lc3:

Low Complexity Communication Codec (LC3)
########################################

.. contents::
   :local:
   :depth: 2

Low Complexity Communication Codec (LC3) is the default software codec for the :ref:`nrf53_audio_app` application, conformant to the `Bluetooth® LE Audio specifications`_ (Bluetooth 5.2, QDID #156294).
For more information about the codec usage in the application, see the application documentation page in the |NCS| documentation.

The nrfxlib module includes the :file:`lc3/include/sw_codec_lc3.h` and :file:`lc3/src/sw_codec_lc3.c` files that form a translation layer for using a standardized API in the nRF5340 Audio application.

.. _lc3_legal:

License and attribution
***********************

For the terms and conditions for using the LC3 codec, see :file:`license.txt` and :file:`attribution.txt` at :file:`lc3/`.

.. _lc3_api:

API documentation
*****************

For API documenation, check the `LC3 API documentation file`_.

.. _lc3_changelog:

Changelog
*********

All notable changes to this project are documented in this section.

main branch
===========

Initial release.
This release includes the following changes to the codec.

LC3 v1.0.4
----------

Build information:

* Platform: ARM Cortex-M33, Cortex-M44
* Compiler: ARM v6, -Ofast
* Hard floating point, fixed point

Changes:

* Replaced floating point stdlib dependencies in data path with T2 functions.
* Optimized the LTPF decoder, fixed point.

LC3 v1.0.3
----------

Changes:

* MDCT optimizations.
* LTPF optimizations.
* Arithmetic encoding optimizations.

LC3 v1.0.2
----------

Changes:

* Platform-specific optimizations.

LC3 v1.0.1
----------

Changes:

* Fixed point optimizations.

LC3 v1.0.0
----------

Changes:

* ARC LTPF SIMD optimizations.

LC3 v0.9.14
-----------

Changes:

* ARM optimizations.

LC3 v0.9.13
-----------

Changes:

* Added compile option to separate encode/decode memory.

LC3 v0.9.12
-----------

Changes:

* ARM optimizations.

LC3 v0.9.11
-----------

Changes:

* ARM optimizations.

LC3 v0.9.10
-----------

Changes:

* Incorporated all technical errata up to 15138.

LC3 v0.9.9
----------

Changes:

* Non-functional changes.

LC3 v0.9.8
----------

Changes:

* Fixed point memory optimizations.
* Fixed point 7.5 ms frame size conformance improvements, meets stricter 0.06 ODG threshold in conformance tests.
* Fixed scaling for > 16 bit PCM output in fixed point.

LC3 v0.9.7
----------

Changes:

* Fixed Glockenspiel 24 kHz / 48 kbps / 7.5 ms exceeding 0.06 ODG in floating point encode/decode test.
* Performance optimizations.

LC3 v0.9.6
----------

Changes:

* Added 7.5 ms support for floating point.

LC3 v0.9.5
----------

Changes:

* Fixed point memory optimizations.

LC3 v0.9.4
----------

Changes:

* Added 24 and 32 bit PCM input and output to T2_LC3.
* Optimizations in pitch lag detection.
* Added API for amount of packet loss correction (PLC) applied.
* Added API for providing static buffers.
* Added error code offset to avoid value conflict with other modules.
* Added API for the number of bytes read from encoder input.

LC3 v0.9.3
----------

Changes:

* Fixed an issue in T2_LC3 where decoding a single frame resulted in 1/4 frame of additional PCM output.

LC3 v0.9.2
----------

Changes:

* Added x64 Linux build T2_LC3.
* Added support for multi-channel WAV files to T2_LC3 application.
* Removed a DLL dependency from T2_LC3 Windows build.
