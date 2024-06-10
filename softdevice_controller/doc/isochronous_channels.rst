.. _softdevice_controller_iso:

LE Isochronous Channels
#######################

.. contents::
   :local:
   :depth: 2

LE Isochronous Channels (ISO) is a set of features defined in the `Bluetooth Core Specification`_.
It allows for unreliable transport of data in one-to-one, one-to-many and many-to-one topologies.

In ISO, data is transmitted in Service Data Units (SDUs).
The source of the data provides one SDU every SDU interval and these SDUs have a fixed timeout for their delivery.
SDUs are transmitted at a fixed schedule, which allows to time-synchronize multiple receiving devices with microsecond-scale accuracy.

The ISO feature set is split into four separate features:

* Connected Isochronous Stream - Central
* Connected Isochronous Stream - Peripheral
* Isochronous Broadcaster
* Synchronized Receiver

The first 2 features use Connected Isochronous Streams (CIS) grouped into Connected Isochronous Groups (CIG).
In this case, data is sent in a unidirectional or bidirectional connection.

The last 2 features use Broadcast Isochronous Streams (BIS) grouped into Broadcast Isochronous Groups (BIG).
In this case, data is sent in a unidirectional stream from a source device to an arbitrary number of sink devices.


Tested configurations
*********************

The ISO feature is highly configurable and it is not feasible to test all possible configurations.
Testing of the |controller| implementation of ISO focuses on the audio use-case configurations that are described in the Basic Audio Profile (BAP) specification.
The BAP specification is available for download from the `Bluetooth Specifications and Documents`_ page.

Configurations for CIS
----------------------

All CIS BAP configurations are tested with two CISes.
The chosen ACL interval in each test is 60 ms, 67.5 ms or 70 ms, based on the recommendations in the BAP specification.
In the BAP configuration tests, both the ACL and CIS connections use 2M PHY.

1M PHY and CODED PHY are tested with bidirectional data transfer with two CISes configured with longer ISO intervals to accommodate for longer packet durations.

Configurations for BIS
----------------------

All BIS BAP configurations are tested with two BISes.
The tests are executed with extended advertising interval and periodic advertising interval set to 300 ms.
The extended advertiser and periodic advertiser are both configured to use 100 bytes of advertising data.
Each BIS BAP configuration is tested using 2M PHY on the BISes and the periodic advertiser.

1M PHY and CODED PHY are tested with three BISes configured with longer ISO intervals to accommodate for longer packet durations.

Additionally, BIS source is tested together with the channel survey feature, see :c:func:`sdc_hci_cmd_vs_qos_channel_survey_enable`.

Configurations for parallel use of CIS and BIS
----------------------------------------------

Parallel use of one CIS and one BIS is tested.
However, there is no absolute maximum of BISes, CISes and ACLs that can be used concurrently.
Instead, the number of roles that can be used at the same time is limited by available memory and the on-air timings.

Tested topologies
*****************

Although the maximum number of BISes/CISes in a single BIG/CIG is limited in the `Bluetooth Core Specification`_, the actual number is subject to available memory and the on-air timings.
The following topologies are tested with data transfer, and can be used as references.

CIS
---
.. figure:: pic/iso_topology/tested_cis_topology.svg
   :alt: Alt text: A diagram showing the tested topologies for CIS
   :align: center
   :width: 80%

   Tested CIS topology - each arrow represents a bidirectional CIS

BIS
---
.. figure:: pic/iso_topology/tested_bis_topology.svg
   :alt: Alt text: A diagram showing the tested topologies for BIS
   :align: center
   :width: 80%

   Tested BIS topology - each arrow represents a BIS

.. _iso_parameter_selection:

Parameter selection
*******************

This section gives a high level overview of how the |controller| selects the values for the BIS and CIS parameters.

When the HCI LE Create BIG command or HCI LE Set CIG Parameters command is used, the |controller| will handle the selection of the ISO parameters.

The host command's input values affect the ISO parameters selected by the |controller|, which affect the resulting reliability and transport latency.
The selected parameters can result in:

* Maximum reliability with high latency.
* Minimal transport latency with low reliability.
* Something between these two extremes.

The parameters are selected based on the following rules:

* The transport latency of the selected configuration does not exceed the ``Max_Transport_Latency`` provided by the host.
* The number of retransmissions of the selected configuration does not exceed the ``RTN`` provided by the host, and matches it if possible.

In other words, ``Max_Transport_Latency`` and ``RTN`` are treated as upper limits for the configuration that the |controller| will use.
By using these properties, the host can influence the parameter selection.

To reduce transport latency, consider one or more of the following approaches:

* Use a smaller ``Max_SDU``
* Use a shorter ``SDU_Interval``
* Switch from LE 1M PHY to LE 2M PHY

``RTN`` also affects transport latency, and this is discussed further in the following sections.

.. note::
   The |controller| has certain limitations on the configurations it can support.
   These are listed in the :ref:`softdevice_controller_limitations` section.
   Unless mentioned otherwise, the |controller| supports the whole range of the allowed parameters.

BIS
-------------

When BIS is used, retransmissions can be achieved by using repetitions and pre-transmissions.
The |controller| will use at least one repetition and as many pre-transmissions as possible to achieve the desired retransmissions.

The following example shows how the host provided values affect the selected parameters.
The example assumes that the |controller| can produce a valid configuration with a mixture of pre-transmissions and repetitions for a given input.
The host can influence the parameters with any of the following actions:

* Increase the ``RTN`` while keeping the ``Max_Transport_Latency`` constant.
   This prioritizes reliability over transport latency, assuming there is room for new retransmissions.
* Decrease the ``RTN`` while keeping the ``Max_Transport_Latency`` constant.
   This prioritizes transport latency over reliability.
* Increase the ``Max_Transport_Latency`` while keeping the ``RTN`` constant.
   This makes the |controller| prioritize pre-transmissions over repetitions.
* Decrease the ``Max_Transport_Latency`` while keeping the ``RTN`` constant.
   This makes the |controller| prioritize repetitions over pre-transmissions.

.. note::
   The |controller| reserves 2.5 ms to allow time for periodic advertising.
   This in turn limits the number of subevents that can be fitted in a BIG event.
   This value can be configured with the :kconfig:option:`BT_CTLR_SDC_BIG_RESERVED_TIME_US` Kconfig option, or with the vendor-specific HCI command defined by :c:func:`sdc_hci_cmd_vs_big_reserved_time_set`.

CIS
-----------

When CIS is used, a PDU will be retransmitted if it is not acknowledged by the peer, provided there is room for retransmissions.
That means a greater ``RTN`` provided by the host will improve reliability, but will introduce higher transport latency.
Conversely, a lower ``Max_Transport_Latency`` will reduce reliability, as a PDU has fewer opportunities for retransmission.

.. _iso_providing_data:

Providing data
**************

Data is provided using SDUs using the HCI format for ISO data described in the `Bluetooth Core Specification`_ Version 5.4, Vol 4, Part E, Section 5.4.5.
Data should be provided in intervals of SDU interval.
The SDU interval is configured when the CIS or BIS is created and is a constant during the lifetime of the stream.

There are 3 modes that determine when the SDUs provided to the |controller| are sent:

Timestamps
   In the timestamp mode, timestamps must be provided in the ``Time_Stamp`` parameter in the HCI ISO data that is provided to the controller.
   This is the preferred way of providing data to the |controller| and guarantees the highest degree of control.

   The timestamp must be based on the controller's timings.
   The timestamp of a previous SDU can be retrieved using the HCI VS ISO Read TX Timestamp command.
   The next timestamp should be incremented by a multiple of the SDU interval.
   This means that, in the audio use case where SDUs are provided every SDU interval, the next timestamp should be incremented by one SDU interval.
   SDUs must be provided to the |controller| at least :c:macro:`HCI_ISO_SDU_PROCESSING_TIME_US` before the time indicated in the added timestamp.

   In ISO, the timing information is based on the central's clock.
   This means that for the CIS central and Broadcaster roles, it is sufficient to retrieve the timestamp from the controller only once.
   The CIS peripheral needs to compensate for drift between its clock and the central's clock.
   When running the CIS peripheral, the HCI VS ISO Read TX Timestamp needs to be called periodically, and should be called every time before new data is provided.

   When a timestamp is added to the HCI data, the |controller| ignores the SDU sequence numbers.

Time of arrival
   In the time of arrival mode, the |controller| records the time when the data is being processed inside the controller.
   The controller then tries to send the data in the next available CIS or BIS event where it does not yet have data to send.
   By doing this, the application does not need to keep track of the exact time, which leads to a higher probability that the SDU is sent and not dropped before being sent.
   The latency between when an SDU is provided and when it is sent depend on the configuration.
   There is a minimum of :c:macro:`HCI_ISO_SDU_PROCESSING_TIME_US` latency due to the processing overhead of the |controller| before sending the SDU.
   While the controller tries to minimize latency, there is some inherent jitter due to the asynchronous nature of the HCI interface.

   Expect a larger latency if data is not provided every SDU interval and the stream is configured with retransmissions.
   This is due to the fact that the controller first needs to send empty data packets for the data that was not provided.
   In case data is missing, the controller sends NULL data every ISO event.
   This also ensures that the data provided with the time of arrival mode is retransmitted the configured number of times.

   Use this mode if the exact time when an SDU is sent does not matter or if SDUs are only produced at a rate much smaller than the SDU interval.
   To use this mode, set the sequence number to 0 and do not add a timestamp to the HCI ISO data.

Sequence numbers
   In the sequence number mode, a sequence number must be provided in the ``Packet_Sequence_Number`` parameter in the HCI ISO data that is provided to the controller.
   When using this mode, an SDU should be provided every SDU interval, and the SDU sequence number must be increased by one for each SDU.
   If SDUs are provided more than one SDU interval apart, the SDU sequence number must be increased by a matching amount.
   It is not recommended to use the sequence number mode if SDUs are provided more than one SDU interval apart.

   The controller learns the initial sequence number, so there is no need to align the sequence number each time with the one that is returned when calling the HCI VS ISO Read TX Timestamp command.

   Pay special attention on the CIS peripheral side, because the timings of ISO are based on the central's clock.
   This means that you need to account for drift between the central's and the peripheral's clocks for the generation of SDUs.
   To do this, use the HCI VS ISO Read TX Timestamp command.
   The command provides a timestamp corresponding to the last possible point in time that the previous SDU could have been provided.
   When combined with the SDU interval, this gives an indication of the last possible time when an SDU can be provided.

   Due to the asynchronous nature of the HCI interface, even small jitter or drift can lead to an SDU being provided too late.
   In that case, the data might be dropped or only transmitted as a retransmission.

   If the provided sequence number does not make sense, the |controller| falls back to the time of arrival mode.

   To use this mode, set the sequence number field and do not add a timestamp to the HCI ISO data.


Synchronize data sent on multiple CISes or BISes
************************************************

The LE Isochronous Channels feature allows SDUs to be sent in a way that multiple receivers can process this data synchronously.
An example use case of this is playback of music that needs to be time-synchronized between a left and a right channel.
The application needs to inform the |controller| which SDUs should be time-synchronized on the receivers.

The recommended way to provide this information is using the timestamps mode.
Using the same timestamp for multiple SDUs guarantees that the SDUs are time-synchronized.
Synchronization cannot be reliably achieved using the time of arrival method.
See the :ref:`iso_providing_data` section for more information.

The following logical flow demonstrates how to send time-synchronized SDUs on multiple CISes or BISes:

1. Provide the controller with an SDU for one of the CISes or BISes using the time of arrival method.
#. Issue the HCI VS ISO Read TX Timestamp command on the CIS or BIS where the SDU was sent.
   The command obtains the timestamp that was assigned to that SDU.
#. Provide the controller with the SDUs for the remaining CISes or BISes using the timestamp method with the obtained timestamp.

.. note::
   Providing the same sequence number to different CISes or BISes does not time-synchronize the provided SDUs.

Only SDUs sent in the same CIG or BIG can be time-synchronized.
