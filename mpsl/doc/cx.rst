.. _mpsl_cx:

Radio coexistence feature
#########################

.. contents::
   :local:
   :depth: 2

The radio coexistence feature allows the application to interface with several types of packet traffic arbiters (PTAs).
This allows for better radio performance for devices using multiple interfering radios simultaneously, like a combination of |BLE| and Wi-Fi, or IEEE 802.15.4 and Wi-Fi.
The PTA arbitrates the requested radio operations between all radios to avoid interference, increasing the performance of the radio protocols enabled simultaneously.
The exact arbitration algorithm is dependent on the specific PTA used.

.. note::
  The radio coexistence feature is not needed for the arbitration between |BLE| and IEEE 802.15.4 in dynamic multiprotocol applications, as the dynamic multiprotocol does not use PTA for the arbitration.

Limitations
***********

This experimental feature has several limitations:

* It supports only the IEEE 802.15.4 protocol.
* It supports only the nRF52 SoC family.
* It does not use radio operation priorities, as the radio protocol requests each operation with a priority value of ``1``.
* The PTA can deny only TX operations.
  Denying RX or idle listening has no effect.
* Aborting a TX operation takes a few dozens of microseconds.

Implementation
**************

To use the radio coexistence feature, you can either select the implementation available in NCS or replace it with another implementation compatible with your PTA.
Each implementation exposes the same API to allow for an easier replacement that does not require modifications to the radio protocols.

The radio coexistence implementation provided in NCS is the following:

* *Thread Radio Coexistence*.
  This PTA interface is compatible with the 3-wire PTA interface defined in the *Thread Radio Coexistence recommendations* document.
  It is also compatible with any IEEE 802.15.4 protocol (including Thread), with |BLE|, and with other radio protocols.

Configuration
*************

To configure the Thread Radio Coexistence implementation you must define each pin used in the 3-wire PTA interface, specifically:

* ``REQUEST``
* ``PRIORITY``
* ``GRANT``

General usage
*************

Each radio protocol using the radio coexistence feature (like IEEE 802.15.4 or |BLE|) can enable a transceiver only when the PTA allows a given radio operation.
When the PTA denies the ongoing radio operation, the radio protocol must abort this operation immediately.

The radio protocol implementation must inform the radio coexistence module about which radio operations it wants to perform at that moment or shortly after, and what is the priority of each radio operation.

The radio coexistence implementation translates the request from the radio protocol to hardware signals compatible with the PTA available in the system.
It also translates the hardware signals received from the PTA indicating which radio operations are allowed at that moment, and it passes this information to the radio protocol implementation.

The radio protocol must register a callback function in the radio coexistence module to be informed about allowed radio operations.

Implementation of a new radio coexistence module
************************************************

If the radio coexistence implementation provided in NCS is not compatible with the PTA used in your system, you can implement a software module compatible with your PTA that exposes the radio coexistence API, using the :file:`nrf/subsys/mpsl/cx/thread/mpsl_cx_thread.c` file as an example.

The radio coexistence implementation must implement the interface defined in the ``mpsl_cx_interface_t`` structure in the :file:`nrfxlib/mpsl/include/mpsl_cx_abstract_interface.h` file.
Each function from this interface is described in details in the :file:`nrfxlib/mpsl/include/protocol/mpsl_cx_protocol_api.h` file.

Any implementation of the radio coexistence interface must register the interface in MPSL during the system initialization using the :c:func:`mpsl_cx_interface_set()` function.
You can find an example how to configure a radio coexistence implementation, including the registration of the interface in MPSL, in the :c:func:`mpsl_cx_init()` function described in the :file:`nrf/subsys/mpsl/mpsl_init.c` and :file:`nrf/subsys/mpsl/mpsl_cx.c` files.
