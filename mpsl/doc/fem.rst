.. _mpsl_fem:

Front-end module feature
########################

.. contents::
   :local:
   :depth: 2

The front-end module feature allows the application to interface with several types of front-end modules (FEMs).
This allows an increase of the transmitted power through a Power Amplifier (PA) or an increase of the sensitivity through a Low-Noise Amplifier (LNA).
Any increase in power and sensitivity results in an increased communication range.
The exact PA and LNA gains are dependent on the specific FEM used.

Implementation
**************

Two FEM implementations are provided:

* nRF21540 GPIO.
   It is compatible with the nRF21540 FEM and implements a 3-pin interface.
   Optionally, support for the MODE pin can be enabled.

* Simple GPIO.
   It is a simplified version, made to be compatible with other front-end modules.
   It implements a 2-pin interface.

Both implementations use PA and LNA pins for controlling the FEM.
Additionally, the nRF21540 GPIO implementation uses the PDN pin for powering down the FEM internal circuits, to reduce energy consumption.
It can also optionally use the MODE pin for switching PA gain between two preconfigured values.

Tx power split
**************

When an application requests a given transmission power, it wants to achieve that power at the antenna.
Usually the application does not know all hardware components, such as RF front-end modules, on the radio signal path.
To achieve a specific value of transmission power at the antenna, every FEM implementation provides a dedicated API that calculates the PA gain and SoC output power combination that results in the requested power at the antenna.

FEM implementations with multiple PA gains available (e.g. nRF21540 with MODE pin support) choose which gain to use based on hardware limitations and the requested power value.

Configurable timings
********************

In both implementations, two timings can be configured:

* ``LNA time gap``, the time between the LNA activation and the start of radio reception.
* ``PA time gap``, the time between the PA activation and the start of radio transmission.

For the nRF21540, two additional timings can also be configured:

* ``TRX hold time``, the time interval for which the FEM is kept powered up after the PDN deactivation event occurs.
* ``PDN settle time``, the time interval reserved for the FEM settle, before the PA or LNA activation.

General usage
*************

The Power Amplifier and the Low-Noise Amplifier are responsible for, respectively, transmission and reception and are configured and activated independently.
The two functionalities can't be configured and set to operate simultaneously, because they share some resources.
As such, after operating with a Power Amplifier, the PA configuration must be cleared to be able to configure and use a Low-Noise Amplifier afterward, and vice versa.

Both amplifiers are controlled through activation and deactivation events.
Two types of events are supported:

* A *timer event*.
  It is the ``COMPARE`` event of a hardware timer and it can be used for both PA/LNA activation and deactivation.
* A *generic event*.
  It is any other event type and it can only be used for PA/LNA deactivation.

To prepare a generic event, it is required that the application provides the event register.
To prepare a timer event, it is required that the application provides the following:

* The instance of the timer, which the protocol has to start by itself.
* The *Compare Channels* mask, which tells the front-end module which Compare Channels of the provided Timer are free to use.
* The Start time, at which the front-end module can start preparing the PA or LNA.
* The End time, at which the front-end module must be ready for the RF procedure.

Then, the module can configure the timer to activate or deactivate the FEM accordingly, taking also into account the FEM settle time.

See below for an example of activating the LNA for Rx operation, using the following parameters:

* RX ramp-up time - 40 us
* LNA settle time - 13 us
* LNA deactivation event - ``rx_end``
* LNA activation timer - ``TIMER0``

The *RX ramp-up time* is the total time scheduled by the application.

See below for the steps needed to properly configure LNA in this example:

1 The application configures the LNA to be activated by the timer event, with the start time set to 0 us and the end time set to 40 us.
# The application provides the ``rx_end`` event as the LNA deactivation event.
# The FEM module reads the scheduled time and sets the ``TIMER0`` compare channel to 27 us, as a result of the RX ramp-up time (40 us) minus the LNA settle time (13 us).
# The application starts the RX operation.
# The application starts ``TIMER0``.

The following picture illustrates the timings in this scenario:

.. figure:: pic/FEM_timing_simple.svg
   :alt: Timing of LNA pin for reception

   Timing of LNA pin for reception

The following picture illustrates the calls between the application, the FEM module, and the hardware in this scenario:

.. figure:: pic/FEM_sequence_simple.svg
   :alt: Sequence diagram of LNA control for reception

   Sequence diagram of LNA control for reception

nRF21540 usage
**************

In the nRF21540 implementation, the PDN pin is used to power down the FEM internal circuits.
The FEM can be powered down on an explicit application request.
The FEM is powered back up automatically before PA or LNA are activated.

See below for an example of controlling LNA and PDN during Rx operation, using the following parameters:

* RX ramp-up time - 40 us
* LNA settle time - 13 us
* PDN settle time - 18 us
* LNA deactivation event - ``rx_end``
* PDN deactivation event - ``software``
* LNA activation timer - ``TIMER1``

The *RX ramp-up time* is the total time scheduled by the application.

See below for the steps needed to properly configure LNA and PDN in this example:

* The application configures the power-down passing ``rx_end`` as the activation event.
* The application configures LNA to be activated by the timer event, with the start time set to 0 us and the end time set to 40 us.
* The application provides the ``rx_end`` event as the LNA deactivation event.
* The FEM module reads the scheduled time and sets ``TIMER1`` compare channels to 27 us (40-13) and 9 us (27-18).
* The application starts Rx operation.
* The application starts ``TIMER1``.

The following picture illustrates the timing in this scenario:

.. figure:: pic/FEM_timing_nRF21540.svg
   :alt: Timing of LNA and PDN pins for reception

   Timing of LNA and PDN pins for reception

The following picture presents the calls between the application, the FEM module, and the hardware in this scenario:

.. figure:: pic/FEM_sequence_nRF21540.svg
   :alt: Sequence diagram of LNA and PDN control for reception

   Sequence diagram of LNA and PDN control for reception

PA gain control
===============

Optionally, an nRF21540 implementation can control the MODE pin to select one of two available PA gains.

To enable this feature, the MODE pin must be enabled in the interface configuration that the application passes on initialization.
