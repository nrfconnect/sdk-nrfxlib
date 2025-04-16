.. _mpsl_clock:

MPSL Clock
##########

.. contents::
    :local:
    :depth: 2

The page describes the MPSL clock module design.

Overview
********

The MPSL requires access to both the low-frequency clock (LFCLK) and the high-frequency clock (HFCLK) for its operation.

On the nRF52 Series, nRF53 Series, and nRF54L Series SoCs, the module provides access to the LFCLK configuration and allows the application to request the crystal oscillator source of the HFCLK.
On the nRF54H Series SoCs, the module provides APIs to integrate the MPSL with an external clock driver.

See the following :ref:`mpsl_internal_clock_control` and :ref:`mpsl_external_clock_control` sections for  descriptions of the design.

.. _mpsl_internal_clock_control:

MPSL internal clock control
***************************

The internal clock control feature is available only for the nRF52 Series, nRF53 Series, and nRF54L Series SoCs.

Low-frequency clock (LFCLK)
---------------------------

MPSL can use one of the following available low-frequency clock (LFCLK) sources:

* 32.768 kHz internal RC oscillator (LFRC)
* 32.768 kHz crystal oscillator (LFXO)

When MPSL is enabled, the application must provide the selected LFCLK source, calibration configurations, and clock accuracy.
The selected accuracy is meant to be forwarded only to the protocol stacks.

The MPSL clock module configures the LFCLK source and keeps it calibrated when the RC oscillator is the selected clock source.

Variations in temperature affect the RC oscillator frequency.
As such, it requires additional calibration for temperature changes of more than 0.5°C.

To ensure the required accuracy, the MPSL clock module must calibrate periodically the RC oscillator using the HFCLK crystal oscillator as a reference.
See the relevant SoC product specification for more information.
As the MPSL clock module runs the calibration automatically, the application must specify how often the clock module takes measurements to detect temperature changes.

.. note::
   You should consider how frequently significant temperature changes are expected to occur in the intended environment of the end product.
   It is recommended to use a temperature polling interval of 4 seconds, and to force the clock calibration every second interval (``.rc_ctiv=16``, ``.rc_temp_ctiv=2``).

High-frequency clock (HFCLK)
----------------------------

The high-frequency clock (HFCLK) controller supports the following HFCLK sources:

* 64 MHz internal oscillator (HFINT)
* 64 MHz crystal oscillator (HFXO)

The HFXO must be running to use the ``RADIO`` peripheral.
MPSL makes sure the HFXO is started in time for every scheduled ``RADIO`` activity.

.. note::
    For the correct operation of the ``RADIO`` peripheral, you must set the HFXO startup time to less than 1.5 ms.
    As such, you must choose the external crystal oscillator and the other related components accordingly.
    See the relevant SoC product specification for more information.

The MPSL clock module API enables the application to safely request and release the HFXO while the protocol stacks are enabled.
MPSL can continue to use the HFXO after it is released if it is requested by protocol stacks.
HFXO is stopped when it is no longer needed.

.. _mpsl_external_clock_control:

MPSL external clock control integration
***************************************

The MPSL clock module provides APIs to register the :c:func:`mpsl_clock_ctrl_source_register` and unregister the :c:func:`mpsl_clock_ctrl_source_unregister` external clock control driver.
The call to :c:func:`mpsl_clock_ctrl_source_register` must be done before the :c:func:`mpsl_init` call, to have an effect.
This API also enables the use of the external clock control by the MPSL.
The call to :c:func:`mpsl_clock_ctrl_source_unregister` must be done after the :c:func:`mpsl_uninit` call.

.. note::
    When you use the MPSL with external clock control feature, the MPSL internal clock control APIs are not allowed to be called.

APIs and data required for integration of MPSL with external clock control are provided by :c:func:`mpsl_clock_lfclk_ctrl_source_t` for LFCLK and :c:func:`mpsl_clock_hfclk_ctrl_source_t` for HFCLK.

Low-frequency clock (LFCLK) integration
---------------------------------------

It is the external clock control driver's responsibility to configure the LFCLK source and keep it calibrated.
On the nRF52 Series and nRF53 Series, the MPSL may initiate additional calibration events by using the :c:func:`lfclk_calibration_start` API.

The MPSL will defer waiting for LFCLK until it is used for the first time if :c:func:`skip_wait_lfclk_started` is set to ``true``.

The :c:member:`lfclk_request`, :c:member:`lfclk_release`, and :c:member:`lfclk_wait` APIs are mandatory.
The :c:member:`accuracy_ppm` value is meant to be forwarded only to the protocol stacks.

High-frequency clock (HFCLK) integration
----------------------------------------

The HFXO must be running to use the ``RADIO`` peripheral.

.. note::
    On the nRF54H Series SoC, the HFXO is required for the use of any peripheral located in the radio power domain in the radio core.

MPSL makes sure the HFXO is started in time for every scheduled ``RADIO`` activity.
HFXO is stopped when it is no longer needed.

The t:c:member:`hfclk_request`, :c:member:`hfclk_release`, and :c:member:`hfclk_is_running` APIs are mandatory.
These APIs will be called by MPSL from high priority, so they may not use blocking operations.

You must correctly configure the  :c:member:`startup_time_us` for the ``RADIO`` operation.
You must set the HFXO startup time to less than 1.5 ms.
As such, you must choose the external crystal oscillator and the other related components accordingly.
See the relevant SoC product specification for more information.
