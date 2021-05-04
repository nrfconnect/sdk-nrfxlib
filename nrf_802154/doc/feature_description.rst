.. _rd_feature_description:

Driver features
###############

.. contents::
   :local:
   :depth: 2

This page describes the nRF IEEE 802.15.4 radio driver features.

.. _features_description_frames_filtering:

Receiving and filtering frames
******************************

The radio driver can receive unicast 802.15.4 frames and broadcast 802.15.4 frames on channels 11-26 from channel page 0.
The driver performs most of the received frame filtering procedure (IEEE 802.15.4-2006: 7.5.6.2).
The filtering procedure is performed during the frame reception, and it is divided into the following stages:

1. When the FCF is received (first BCMATCH event), the driver checks if the length of the frame is valid and verifies the frame type and version.
#. When destination address fields (PAN ID and address) are present and received (second BCMATCH event), the driver checks if the frame is destined to this node (broadcast or unicast).
#. When the entire frame is received (END event), the driver verifies if the FCS field contains a valid value.

If all checks are passed, the driver passes the received frame to the MAC layer.

.. note::
   Steps 1 and 2 can be bypassed in :ref:`features_description_promiscuous_mode`.

A received frame includes a timestamp captured when the last symbol of the frame is received.
The timestamp can be used to support synchronous communication like CSL or TSCH.

.. _features_description_automatic_sending_ack:

Sending automatically ACK frames
********************************

The MAC layer can configure the driver to automatically send ACK frames.
This feature is enabled by default.

This automatically created ACK frame is compliant with IEEE 802.15.4 from 2006 (7.2.2.3) or IEEE 802.15.4 from 2015 (6.7.2 and 6.7.4.2).
This frame is sent exactly 192 us (aTurnaroundTime) after a data frame is received.
The ACK frame is sent only if the received frame passes all the steps of :ref:`the filter <features_description_frames_filtering>`, even in promiscuous mode, and if the ACK request bit is present in the FCF of the received frame.

The automatic ACK procedure uses a dedicated TIMER peripheral to keep valid inter-frame spacing, regardless of the ISR latency.
The driver sets the PPI and TIMER registers to end the transmitter ramp up exactly 192 us (aTurnaroundTime) after the frame is received.
If any of the filter steps fails, the driver does not set PPI to start the transmitter ramp up when TIMER fires.
If all filter steps pass, the driver checks the version of the received frame and prepares either an immediate acknowledgment (Imm-Ack) frame or an enhanced acknowledgment (Enh-Ack) frame:

* In the Imm-Ack, the driver sets the correct sequence number in the ACK frame, prepares PPIs to ramp up the transmitter, and optionally performs the :ref:`automatic pending bit procedure <features_description_setting_pending_bit>`.
* In the Enh-Ack, the driver fills all required fields (including the :ref:`automatic pending bit procedure <features_description_setting_pending_bit>`) and optionally adds the last configured information elements to the acknowledgment frame.
  The ACK frame is sent automatically by TIMER, RADIO shorts, and PPIs.

.. _features_description_setting_pending_bit:

Setting pending bit in ACK frames
*********************************

The MAC layer can configure the driver to automatically set a pending bit in the automatically generated ACK frames.
This feature is enabled by default.

The pending bit is handled according to the protocol and the respective setting mode:

* Blacklist mode (Zigbee):

  * If the frame being acknowledged is not a data poll command frame, the pending bit is cleared (0).
  * If the driver matches the source address with an entry in the array, the pending bit is cleared (0).
  * If the array does not contain an address matching the source address and the frame being acknowledged is a data poll command frame, the pending bit is set (1).
* Thread mode:

  * If the driver matches the source address with an entry in the array, the pending bit is set (1).
  * If the array does not contain an address matching the source address, the pending bit is cleared (0).

If the ACK frame cannot be prepared before the transmission is ready to begin, the ACK frame is not transmitted.

.. _features_description_transmission:

Transmitting unicast and broadcast frames
*****************************************

The radio driver allows the MAC layer to transmit frames containing any PSDU.
The RADIO peripheral updates the FCS field of every frame automatically.

A transmission procedure can be preceded by a CCA procedure.
The driver automatically receives an ACK frame if requested.

.. _features_description_cca:

Performing an automatic CCA procedure before transmission
*********************************************************

The MAC layer can request the driver to perform a CCA procedure before transmission.
If a CCA procedure is requested, the driver performs it.

Depending on the channel activity:

* If the channel is busy, the driver notifies the MAC layer and ends the transmission procedure.
* If the channel is idle, the driver starts the transmission immediately after the CCA procedure ends.

.. _features_description_receiving_ack:

Receiving automatically ACK frames
**********************************

If the FCF of the frame requested for transmission has the ACK request bit cleared, the driver ends the transmission procedure and notifies the MAC layer right after the RADIO peripheral ends the transmission of the frame.

If the FCF of the frame has the ACK request bit set, the driver waits for the ACK frame.
The wait can be interrupted by the following events:

* The driver receives the expected ACK frame.
  In this case, the driver resets the receiver, enters the receive state, and notifies the MAC layer that the transmission succeeded.
* The driver receives a frame different from the expected ACK.
  In this case, the driver resets the receiver, enters the receive state, and notifies the MAC layer that the transmission failed.
* The ACK timer expires.
  In this case, the driver resets the receiver, enters the receive state, and notifies the MAC layer that the transmission failed.
* The wait for ACK is terminated by another radio operation requested by the driver.
  Such operation can be requested by a higher layer using a public API call, or internally by a scheduled operation like :ref:`delayed TX or delayed RX <features_description_delayed_ops>`.
  If the wait for ACK is terminated, the driver notifies the MAC layer that the transmission was terminated.

.. _features_description_standalone_cca:

Performing a stand-alone CCA procedure
**************************************

The driver can perform a stand-alone CCA procedure.

The driver notifies the MAC layer about the result of the CCA procedure through the :c:func:`cca_done` call.
After the CCA procedure ends, the driver enters the receive state.

.. _features_description_low_power:

Entering low-power mode
***********************

The MAC layer can request the driver to enter the low-power mode (sleep).

In this mode, the RADIO peripheral cannot receive or transmit any frames, but power consumption is minimal.

.. _features_description_energy_detection:

Performing energy detection
***************************

The driver can perform an energy detection procedure for the time given by the MAC layer.
This procedure returns the maximal energy level detected during the procedure.
The time given by the MAC layer is rounded up to a multiple of 128 us.

.. note::
   The energy detection procedure in a multiprotocol configuration may take longer than the requested time.
   Energy detection is interrupted by any radio activity from other protocols, but the total time of energy-detection periods is greater or equal to the time requested by the MAC layer.

.. _features_description_promiscuous_mode:

Running in promiscuous mode
***************************

While in promiscuous mode, the driver reports to the MAC layer the received frames that meet one of the following requirements:

* Pass all the steps listed above in the :ref:`Receiving and filtering frames <features_description_frames_filtering>` section.
* Fail step 1 or 2 of the abovementioned steps.

If any step of the filter fails, the driver does not :ref:`automatically transmit an ACK frame <features_description_receiving_ack>` in response to the received frame.

.. _features_description_cc_transmission:

Transmitting a continuous carrier wave
**************************************

The driver can send a continuous unmodulated carrier wave on a selected channel.

The continuous carrier transmission forces CCA (ED mode) to report a busy channel on nearby devices.
To stop the continuous carrier transmission, the MAC layer must request to the driver to enter the receive or the sleep state.

The continuous carrier wave is transmitted when the RADIO peripheral is in the TXIDLE state.

.. note::
   * This mode is intended for device testing and must not be used in an end-user application.
   * The nRF53 series does not support the serialization of this function between the APP and NET cores.
     Only applications ran entirely on the NET core will be able to call this function.


.. _features_description_mc_transmission:

Transmitting a modulated carrier wave
*************************************

The driver can send a continuous modulated carrier wave on a selected channel.
The wave is modulated with the payload given by the MAC layer.
SHR, PHR, and FCS are applied to the payload.
The FCS of the previous frame is transmitted back-to-back with the SHR of the next frame.

The :ref:`continuous carrier transmission <features_description_cc_transmission>` forces CCA (ED mode) to report a busy channel on nearby devices.
To stop continuous carrier transmission, the MAC layer must request to the driver to enter the receive state.

The modulated carrier is transmitted when the RADIO peripheral is in the TX state with the PHYEND_START short enabled.

.. note::
   * This mode is intended for device testing and must not be used in an end-user application.
   * The nRF53 series does not support the serialization of this function between the APP and NET cores.
     Only applications ran entirely on the NET core will be able to call this function.

.. _features_description_csma:

Performing CSMA-CA
******************

The driver can perform the CSMA-CA procedure followed by the frame transmission.

The MAC layer must call :c:func:`csma_ca` to initiate this procedure.
The end of the procedure is notified by either the :c:func:`tx_started` or the :c:func:`transmit_failed` functions.
The driver :ref:`receives ACK frames <features_description_receiving_ack>` like after any other transmission procedure.

.. note::
   This feature requires the usage of the proprietary 802.15.4 Service Layer, which is currently not supported by nRF53 chips.

.. _features_description_delayed_ops:

Performing delayed operations
*****************************

The driver can transmit or receive a frame at a specific requested time.
This provides support for synchronous communication and can be used by a higher layer to support features like CSL, TSCH, or Zigbee GP Proxy.

The radio driver can also schedule up to one delayed transmission or two delayed receptions for a given moment in time.
In this scenario, the driver does not verify if the scheduled delayed operations do overlap but, still, it can execute only a single operation at a time.
If a new delayed operation is scheduled to be executed while a previous one is still ongoing, the driver will prematurely abort the previous operation.

.. note::
   This feature requires the support for scheduling radio operations in the 802.15.4 Service Layer, which is currently not supported by nRF53 chips.

.. _features_description_delayed_rx:

Performing delayed reception
============================

The driver can perform a delayed reception entering the RECEIVE state for a given time period.

When the driver detects the start of a frame at the end of the reception window, it automatically extends the window to be able to receive the whole frame and transmit the acknowledgment.
It then notifies the end of the window to the MAC layer with the `rx_failed` (RX_TIMEOUT) notification.

At the end of the reception window, the driver does not automatically transit to the SLEEP state.
Instead, the MAC layer must request the transition to the required state and, optionally, request the next delayed-reception operation.

To distinguish between notifications issued by different delayed-reception windows, the higher layer must also provide a unique identifier when requesting a window.
Then driver passes later that identifier to the notifications as a parameter.

.. _features_description_ie_csl_phase_injection:

Injecting the CSL Phase Information Element
*******************************************

The driver can update the Coordinated Sampled Listening (CSL) phase in a transmitted frame at the moment of the frame transmission, by performing a CSL phase injection, for both data frames and enhanced ACK frames.

The driver calculates the injected CSL phase value from the moment it ended the transmission of the last SHR symbol to the middle of the first pending delayed-reception window.
If there are no pending delayed-reception windows or the frame does not contain a CSL Information Element (IE), the driver does not perform any action and it does not modify the frame.

As such, the higher layer must prepare a properly formatted frame, containing the following:

* The CSL Information Element with the ``Period`` field set correctly.
* The ``CSL Phase`` field containing a placeholder value.

.. note::
   This feature requires the support for scheduling radio operations in the 802.15.4 Service Layer, which is currently not supported by nRF53 chips.
