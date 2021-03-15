.. _rd_feature_description:

Feature description
###################

.. contents::
   :local:
   :depth: 2

This page describes the nRF IEEE 802.15.4 radio driver features.

.. _features_description_frames_filtering:

Receiving and filtering frames
******************************

The radio driver can receive unicast 802.15.4 frames and broadcast 802.15.4 frames on channels 11-26 from channel page 0.
The driver performs most of the received frame filtering procedure (IEEE 802.15.4-2006: 7.5.6.2).
The filtering procedure is performed during the frame reception.

The filtering during frame reception is made of the following stages:

1. When FCF is received (first BCMATCH event), the driver checks if the length of the frame is valid and verifies the frame type and version.
#. When destination address fields (PAN ID and address) are present and received (second BCMATCH event), the driver checks if the frame is destined to this node (broadcast or unicast).
#. When the entire frame is received (END event), the driver verifies if the FCS field contains a valid value.

If all checks pass, the driver passes the received frame to the MAC layer.

.. note::
   Steps 1 and 2 can be bypassed in :ref:`features_description_promiscuous_mode`.

A received frame includes a timestamp captured when the last symbol of the frame is received.
The timestamp can be used to support synchronous communication like CSL or TSCH.

.. _features_description_automatic_sending_ack:

Automatic sending of ACK frames
*******************************

The MAC layer can configure the driver to automatically send ACK frames.
This feature is enabled by default.

The automatically created ACK frame is compliant with IEEE 802.15.4 from 2006 (7.2.2.3) or IEEE 802.15.4 from 2015 (6.7.2 and 6.7.4.2).
This frame is sent exactly 192 us (aTurnaroundTime) after a data frame is received.
The ACK frame is sent only if the received frame passes all steps of :ref:`the filter <features_description_frames_filtering>` (even in promiscuous mode) and if the ACK request bit is present in FCF of the received frame.

The automatic ACK procedure uses a dedicated TIMER peripheral to keep valid inter-frame spacing regardless of the ISR latency.
The driver sets PPI and TIMER registers to end transmitter ramping up exactly 192 us (aTurnaroundTime) after the frame is received.
If any of the filter steps fails, the driver does not set PPI to start the transmitter ramp up when TIMER fires.
If all filter steps pass, the driver checks the version of the received frame and prepares an immediate acknowledgment (Imm-Ack) frame or an enhanced acknowledgment (Enh-Ack) frame accordingly:

* In the Imm-Ack, the driver sets the correct sequence number in the ACK frame, prepares PPIs to ramp up the transmitter, and optionally performs :ref:`automatic pending bit procedure <features_description_setting_pending_bit>`.
* In the Enh-Ack, the driver fills all required fields (including :ref:`automatic pending bit procedure <features_description_setting_pending_bit>`) and optionally adds the last configured Information Elements to the acknowledgment frame.
  The ACK frame is sent automatically by TIMER, RADIO shorts, and PPIs.

.. _features_description_setting_pending_bit:

Setting pending bit in ACK frames
*********************************

The MAC layer can configure the driver to automatically set a pending bit in the automatically generated ACK frames.
Higher layer protocols that support this feature each handle the pending bit differently.

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

Transmission of unicast and broadcast frames
********************************************

The radio driver allows the MAC layer to transmit a frame containing any PSDU.
The RADIO peripheral updates the FCS field of every frame automatically.

A transmission procedure can be preceded by a CCA procedure.
The driver automatically receives an ACK frame if requested.

.. _features_description_cca:

Automatic CCA procedure before transmission
*******************************************

The MAC layer can request the driver to perform a CCA procedure before transmission.
If a CCA procedure is requested, the driver performs the CCA procedure.

Depending on the channel activity:

* If the channel is busy, the driver notifies the MAC layer and ends the transmission procedure.
* If the channel is idle, the driver starts the transmission immediately after the CCA procedure ends.

.. _features_description_receiving_ack:

Automatic receiving of ACK frames
*********************************

If the FCF of the frame requested for transmission has the ACK request bit cleared, the driver ends the transmission procedure and notifies the MAC layer right after the RADIO peripheral ends transmission of the frame.

If the FCF of the frame has the ACK request bit set, the driver waits for the ACK frame.
The wait can be interrupted by the following events:

* The driver receives the expected ACK frame.

  * In this case, the driver resets the receiver, enters the receive state, and notifies the MAC layer that the transmission succeeded.
* The driver receives a frame different from the expected ACK.

  * In this case, the driver resets the receiver, enters the receive state, and notifies the MAC layer that the transmission failed.
* If the ACK timer expires, the driver resets the receiver, enters the receive state, and notifies the MAC layer that the transmission failed.
* The wait for ACK is terminated by another radio operation requested by the driver.
  Such operation can be requested by a higher layer using a public API call, or internally by a scheduled operation like :ref:`delayed TX or delayed RX <features_description_delayed_ops>`.
  If the wait for ACK is terminated, the driver notifies the MAC layer that the transmission was terminated.

.. _features_description_standalone_cca:

Stand-alone CCA procedure
*************************

The driver can perform a stand-alone CCA procedure.

The driver notifies the MAC layer about the result of the CCA procedure through the :c:func:`cca_done` call.
After the CCA procedure ends, the driver enters the receive state.

.. _features_description_low_power:

Low power mode
**************

The MAC layer can request the driver to enter the low power mode (sleep).

In this mode, the RADIO peripheral cannot receive or transmit any frames, but power consumption is minimal.

.. _features_description_energy_detection:

Energy detection
****************

The driver can perform an energy detection procedure for the time given by the MAC layer.
This procedure returns the maximal energy level detected during the procedure.
The time given by the MAC layer is rounded up to a multiplication of 128 us.

.. note::
   The energy detection procedure in a multiprotocol configuration may take longer than the requested time.
   Energy detection is interrupted by any radio activity from other protocols, but the total time of energy-detection periods is greater or equal to the time requested by the MAC layer.

.. _features_description_promiscuous_mode:

Promiscuous mode
****************

While in promiscuous mode, the driver reports to the MAC layer the received frames that meet one of the following requirements:

* Pass all :ref:`filter <features_description_frames_filtering>` steps
* Fail steps 1 or 2.

If any step of the filter fails, the driver does not :ref:`automatically transmit an ACK frame <features_description_receiving_ack>` in response to the received frame.

.. _features_description_cc_transmission:

Continuous carrier transmission
*******************************

The driver can send a continuous unmodulated carrier wave on a selected channel.

This mode is intended for device testing and must not be used in a product application.
The continuous carrier transmission forces CCA (ED mode) to report a busy channel on nearby devices.
The MAC layer should request entering the receive state or the sleep state by the driver to stop the continuous carrier transmission.

The continuous carrier is transmitted when the RADIO peripheral is in the TXIDLE state.

In the case of nRF53, the serialization of this function between the APP and NET cores is not supported, only applications ran entirely on the NET core will be able to call this function.

.. _features_description_mc_transmission:

Modulated carrier transmission
******************************

The driver can send a continuous modulated carrier wave on a selected channel.
The wave is modulated with the payload given by the MAC layer.
SHR, PHR, and FCS are applied to the payload.
FCS of the previous frame is transmitted back-to-back with the SHR of the next frame.

This mode is intended for device testing and must not be used in a product application.
The :ref:`continuous carrier transmission <features_description_cc_transmission>` forces CCA (ED mode) to report a busy channel on nearby devices.
The MAC layer should request entering the receive state by the driver to stop continuous carrier transmission.

The modulated carrier is transmitted when RADIO peripheral is in the TX state with PHYEND_START short enabled.

In the case of nRF53, the serialization of this function between the APP and NET cores is not supported, only applications ran entirely on the NET core will be able to call this function.

.. _features_description_csma:

CSMA-CA
*******

This feature of the driver performs the CSMA-CA procedure followed by frame transmission.

The MAC layer must call :c:func:`csma_ca` to initiate this procedure.
The end of the procedure is notified by the :c:func:`tx_started` or :c:func:`transmit_failed` function.
The driver :ref:`receives ACK frames <features_description_receiving_ack>` like after any other transmission procedure.

This feature requires using nordic proprietary 802.15.4 Service Layer, and as such is currently not supported by nRF53 chips.

.. _features_description_delayed_ops:

Delayed operations
******************

The driver can transmit or receive a frame at a requested time, which provides support for synchronous communication.
This feature can be used by a higher layer to support features like CSL, TSCH, or Zigbee GP Proxy.

Delayed transmission and reception operations are not exclusive.
This means that there can be one transmission and one reception scheduled at the same time.
On the other hand, the driver does not check if the operations are conflicting.
As a result, delayed operations may interrupt each other.

This feature requires the support for scheduling radio operations in the 802.15.4 Service Layer, and as such is currently not supported by nRF53 chips.

.. _features_description_delayed_rx:

Delayed reception
*****************

The delayed reception feature puts the driver in the RECEIVE state for a given time period.

If the start of a frame is detected at the end of the reception window, the window is automatically extended to be able to receive the whole frame and transmit acknowledgement.

The window is kept open after the frame is received.
The end of the window is notified to the MAC layer with the `rx_failed` (RX_TIMEOUT) notification.
The driver does not automatically transit to the sleep state at the end of the reception window.
It is the responsibility of the MAC layer to request the transition to the required state and request the next delayed reception operation.

This feature requires the support for scheduling radio operations in the 802.15.4 Service Layer, and as such is currently not supported by nRF53 chips.
