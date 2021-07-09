.. _mpsl_clock:

Clock
*****

The MPSL clock module provides access to LFCLK configuration and allows the application to request the HFCLK crystal oscillator source.

Design description
==================

LFCLK
-----
MPSL can use one of two available LFCLK sources:

* 32.768 kHz internal RC oscillator (LFRC)
* 32.768 kHz crystal oscillator (LFXO)

When MPSL is enabled, the application must provide the selected LFCLK source, calibration configurations, and the clock accuracy.
The selected accuracy is only for forwarding to protocol stacks.

The MPSL clock module is responsible for configuring the LFCLK source, and for keeping it calibrated when the RC oscillator is the selected clock source.
The RC oscillator frequency will be affected by variation in temperature.
To ensure the required accuracy, the RC oscillator must therefore be calibrated using the HFCLK crystal oscillator as a reference.
The calibration must be performed periodically, and additional calibration is required for temperature changes of more than 0.5°C.
See the relevant SoC product specification for more information.
The MPSL clock module runs the calibration automatically, but the application must specify how often the clock module takes measurements to detect temperature change.

Consider how frequently significant temperature changes are expected to occur in the intended environment of the end product.
It is recommended to use a temperature polling interval of 4 seconds, and to force clock calibration every second interval (``.rc_ctiv=16``, ``.rc_temp_ctiv=2``).

HFCLK
-----
The HFCLK controller supports the following high frequency clock (HFCLK) sources:

* 64 MHz internal oscillator (HFINT)
* 64 MHz crystal oscillator (HFXO)

The HFXO must be running to use the RADIO.
MPSL makes sure the HFXO is started in time for a scheduled RADIO activity.

.. note::
    For correct operation of the RADIO peripheral, the HFXO startup time must be set to less than 1.5 ms.
    The external crystal oscillator and other related components must be chosen accordingly.
    See the relevant SoC product specification for more information.

The MPSL clock module API enables the application to safely request and release the HFXO while protocol stacks are enabled.
MPSL may continue to use the HFXO after it is released if it is requested by protocol stacks.
HFXO will be stopped when it is no longer needed.
