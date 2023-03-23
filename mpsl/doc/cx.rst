.. _mpsl_cx:

Short-Range Protocols External Radio Coexistence
################################################

.. contents::
   :local:
   :depth: 2

The radio coexistence feature allows the application to interface with several types of packet traffic arbiters (PTAs).
This allows for better radio performance for devices using multiple interfering radios simultaneously, like a combination of IEEE 802.15.4 or Bluetooth LE and Wi-Fi.
The PTA arbitrates the requested radio operations between all radios to avoid interference, increasing the performance of the radio protocols enabled simultaneously.
The exact arbitration algorithm is dependent on the specific PTA used.

.. note::
  The radio coexistence feature is not needed for the arbitration between |BLE| and IEEE 802.15.4 in dynamic multiprotocol applications, as the dynamic multiprotocol does not use PTA for the arbitration.

Limitations
***********

This experimental feature has several limitations:

* It does not use radio operation priorities, as the radio protocol requests each operation with a priority value of ``1``.
* [IEEE 802.15.4] The PTA can deny only TX operations.
  Denying RX or idle listening has no effect.
* [IEEE 802.15.4] Aborting a TX operation after the access to medium is revoked takes a few dozens of microseconds.
* [BLE] Bluetooth 1-wire coex interface is only supported by nRF52 Series

Principle of operation
**********************

The radio coexistence featureallows short-range protocols (e.g. IEEE 802.15.4, Bluetooth LE) To communicate with the packet traffic arbiter (PTA) using ``mpsl_cx`` API.
To perform any radio operation, the protocol drivers must first request appropriate access to the medium from the arbiter. The radio operation can be executed only when the PTA grants it.
When the PTA revokes access to the medium during an ongoing radio operation, the radio protocol must abort this operation immediately.

The radio coexistence implementation translates the request from the radio protocol to hardware signals compatible with the PTA available in the system.
It also translates the hardware signals received from the PTA indicating which radio operations are allowed at that moment, and it passes this information to the radio protocol implementation.

The radio protocol must register a callback function in the radio coexistence module to be informed about allowed radio operations.

The details on how and when to request access to the medium from a given PTA for a specific operation and how the PTA grants this access depend on the implementation of the PTA. They are transparent to the user of ``mpsl_cx`` API. 

.. note::
  To learn more about the arbitration process during various transmission and reception modes for the IEEE 802.15.4 Radio Driver, see :ref:`Wi-Fi Coexistence module (PTA support) <rd_coex>`

Implementation
**************

To use the radio coexistence feature, you can either select the implementation available in nRF Connect SDK or replace it with another implementation compatible with your PTA.
Each implementation exposes the same API to allow for an easier replacement that does not require modifications to the radio protocols.

The radio coexistence implementation provided in nRF Connect SDK is the following:

*Bluetooth LE External Radio 1-wire Coexistence*
   This PTA interface is specifically designed for the coexistence interface of nRF9160. 

*nRF700x Coexistence*
   This PTA interface is compatible with the nRF700x Coexistence interface. 

*Thread Radio Coexistence*
   This PTA interface is compatible with the generic 3-wire PTA interface defined in the *Thread Radio Coexistence recommendations* document available to Thread Group members.
   It is also compatible with any IEEE 802.15.4 protocol (including Thread).

The support for these interfaces is provided by single-file plugins located in the sdk-nrf repository, in the :file:`subsys/mpsl/cx` directory.

Configuration
*************

To configure the Thread Radio Coexistence implementation, you must define each pin used in the 3-wire PTA interface, specifically:

* ``REQUEST``
* ``PRIORITY/STATUS``
* ``GRANT``

The pins need to be configured by adding a ``nrf_radio_coex`` node in the devicetree source file.

To enable Wi-Fi Coexistence, set the :kconfig:option:`CONFIG_MPSL_CX` Kconfig option to ``y`` and select a coexistence interface implementation appropriate for your application.

For more detailed information regarding necessary configuration and definition examples, please refer to: :ref:`Adding Wi-Fi Coexistence support to short-range radio applications <nrf:ug_radio_coex>`.

Implementation of a new radio coexistence module
************************************************
If the radio coexistence implementation provided in nRF Connect SDK is not compatible with the PTA used in your system, you can implement a software module compatible with your PTA that exposes the radio coexistence API.

Implement a software plugin
^^^^^^^^^^^^^^^^^^^^^^^^^^^
You can refer to the :file:`nrf/subsys/mpsl/cx/thread/mpsl_cx_thread.c` file as an example.

The radio coexistence implementation must implement the interface defined in the ``mpsl_cx_interface_t`` structure in the :file:`nrfxlib/mpsl/include/mpsl_cx_abstract_interface.h` file.
Each function from this interface is described in detail in the appropriate :ref:`API documentation <mpsl_api_rd_cx>`, as well as in the :file:`nrfxlib/mpsl/include/protocol/mpsl_cx_protocol_api.h` file.

Register your implementation in MPSL
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Any implementation of the radio coexistence interface must register the interface in MPSL during the system initialization using the :c:func:`mpsl_cx_interface_set()` function.

You can find an example how to configure a radio coexistence implementation, including the registration of the interface in MPSL, in the :c:func:`mpsl_cx_init()` function described in the :file:`nrf/subsys/mpsl/mpsl_init.c` and :file:`nrf/subsys/mpsl/mpsl_cx.c` files.
