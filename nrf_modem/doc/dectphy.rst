.. _nrf_modem_dect_phy:

DECT NR+ physical layer
#######################

.. contents::
   :local:
   :depth: 2

The DECT physical layer (PHY) interface in the Modem library is used to control the DECT NR+ PHY in the DECT NR+ PHY firmware.

The DECT NR+ PHY firmware is a variant of the nRF91x1 firmware with a different radio technology than the cellular firmware.
In particular, the DECT NR+ PHY firmware does not support cellular operation or the Global Navigation Satellite System (GNSS).

For details about the key hardware capabilities of the nRF9161 and its DECT NR+ PHY firmware implementation, with regards to the DECT NR+ standard, refer to the `nRF9161 DECT NR+ product specification`_.

.. note::
   To obtain the DECT NR+ PHY firmware, you must contact the Nordic Semiconductor sales department.

For more details about which Modem library features are available for which firmware, see :ref:`nrf_modem_variants`.

Regulations
***********

DECT NR+ operates on free but regulated radio channels.
The availability of the channels and the exact regulation to use them varies in different countries.

.. note::
   It is your responsibility to operate the devices according to the local regulation, in all situations.

.. caution::
   The European Telecommunications Standards Institute (ETSI) has published European Normative document `ETSI EN 301 406-2`_, which restricts operation on specific absolute channel frequency numbers within the available range.
   If you plan to operate a device using the DECT NR+ PHY firmware in territories where ETSI EN 301 406-2 is applicable, you must refer to table 4.3.2.3-2 to see which channel frequency numbers you are allowed to use.

   The Federal Communication Commission (FCC) rules and regulations for radio frequency devices are located in `47 CFR Part 15`_.
   If you plan to operate a device using the DECT NR+ PHY firmware in territories where 47 CFR Part 15 is applicable, you must refer to `47 CFR Part 15.323`_ to see which channel frequency numbers you are allowed to use.

Protocol stack overview
***********************

The physical layer is only one part of the DECT-2020 New Radio protocol stack (DECT NR+).

The whole DECT NR+ protocol stack consists of four layers:

* Physical layer (PHY).
* Medium Access Control (MAC) layer, including co-existing and channel access operation.
* Data link control (DLC), including routing, segmentation and re-assembly of messages.
* Convergence layer (CVG), including message flow control, multiplexing and transmission service level control.

For more details about the DECT NR+ protocol stack, refer to the `nRF9161 DECT NR+ product specification`_ page.

The DECT NR+ PHY firmware implements only the physical layer of the protocol stack.

.. note::
   To pass the certification, a MAC layer implementation is required, and main parts of its operation are verified during the certification phase.

General operation
*****************

This section describes the characteristics and general operation of the DECT PHY interface.

Initialization
==============

The DECT PHY interface in the Modem library requires explicit initialization.

Before initializing the DECT PHY interface of the Modem library, the application must:

#. Initialize the Modem library by calling the :c:func:`nrf_modem_init` function. This also turns on the modem core.
#. Register callbacks for DECT PHY operations by calling the :c:func:`nrf_modem_dect_phy_callback_set` function.

Afterwards, the application can initialize the DECT PHY interface by calling the :c:func:`nrf_modem_dect_phy_init` function.
Upon successful initialization, both the DECT PHY interface and DECT NR+ physical layer in the modem are ready for operation.

The very first time the DECT PHY interface is initialized on a nRF9131 SiP, the DECT NR+ PHY firmware locks the calibration data preventing any changes to it.

The DECT PHY interface can be de-initialized, which in turn de-initializes the physical layer in the modem, cancelling all scheduled operations.

.. note::
   De-initializing the DECT PHY interface does not unset any callbacks set by the application using the :c:func:`nrf_modem_dect_phy_callback_set` function.
   It also does not affect the modem core power status (on/off), which is instead controlled by the :c:func:`nrf_modem_init` and :c:func:`nrf_modem_shutdown` functions.

Once de-initialized, the DECT PHY interface can be re-initialized by only calling the :c:func:`nrf_modem_dect_phy_init` function.

Radio states
============

The radio states have implications for latency and power consumption.
During operation, the radio can be in one of several states.

When a radio operation is being executed, the stack is in one of the following states:

* TX active- The radio is transmitting, no reception is ongoing.
* RX active- The radio is receiving, no transmission is ongoing.

When no radio operations are being executed, the stack is in the radio idle state.
In this state, the radio is powered on and able to start a radio operation with relatively low latency, at the cost of increased power consumption compared to sleep or off states.

Currently, the DECT PHY layer in the DECT NR+ PHY firmware does not support deep sleep states.

Scheduling operations
=====================

The DECT PHY interface allows to schedule radio operations for execution by the scheduler of the DECT NR+ physical layer in the DECT NR+ PHY firmware.
Due to the nature of a radio scheduler, which allows radio operations to be executed at a specific time in the future, all radio operations in the DECT PHY interface are asynchronous and their completion is signaled to the application using callbacks.

All scheduling is final, which means that it is not possible to unschedule operations.

A radio operation may be scheduled to execute at a specific time in the future, or immediately, if the radio is not currently executing any other operation.

All radio operations have an application-defined handle, which can be used to identify the operation and that is returned by the callback signaling the completion of an operation.
The operation handle is entirely opaque to all underlying layers, including the DECT PHY interface.

Modem time and operation latency
--------------------------------

Operation execution is scheduled by the application according to the modem time, which is a 64-bit counter kept by the modem.
All radio operation callbacks provide the value that the modem time counter had at the moment the callback was sent by the modem core to the application core.
This provides a way for the application to track the modem time without explicitly querying the modem for it.

If necessary, the application can retrieve the modem time counter value by calling the :c:func:`nrf_modem_dect_phy_time_get` function.

Both radio state transitions between operations in the modem core and the interprocessor communication (IPC) mechanism between the application and modem cores have latency.
The latency of both radio state transitions and IPC is measured and provided by the DECT PHY interface upon initialization, by the :c:member:`nrf_modem_dect_phy_callbacks.init` callback function, in the :c:member:`nrf_modem_dect_phy_modem_cfg.latency` parameter.

.. note::
   The application must account for operations' latency when scheduling operations.

Temperature monitoring
======================

The DECT PHY interface reports the current SiP temperature as measured by the DECT NR+ PHY firmware in all radio operation callbacks, and upon initialization.
This allows the application to track the changes in temperature and adjust further scheduling of operations accordingly, to ensure the device remains within safe operating temperatures.

The DECT NR+ PHY firmware has an internal temperature protection mechanism that prevents the SiP from operating above safe temperature limits.
The operating temperature limit is reported upon initialization by the :c:member:`nrf_modem_dect_phy_callbacks.init` callback function, in the :c:member:`nrf_modem_dect_phy_modem_cfg.temperature_limit` parameter.

If the temperature threshold is reached, the modem rejects further scheduling of radio operations with the :c:enum:`nrf_modem_dect_phy_err.NRF_MODEM_DECT_PHY_ERR_TEMP_HIGH` error.

In this event, the application must de-initialize the DECT PHY interface (and thus the firmware DECT NR+ physical layer) by calling the :c:func:`nrf_modem_dect_phy_deinit` function and allow the device to cool.
This will cancel all scheduled operations, with the :c:enum:`nrf_modem_dect_phy_err.NRF_MODEM_DECT_PHY_ERR_OP_CANCELED` error in their relative callbacks.

The application can then re-initialize the DECT PHY interface by calling :c:func:`nrf_modem_dect_phy_init`, and read the current measured temperature in the :c:member:`nrf_modem_dect_phy_callbacks.init` function parameters,
to ensure the temperature has decreased below the allowed threshold.

Physical layer capabilities
===========================

The application can retrieve the DECT NR+ PHY firmware physical layer capabilities by calling the :c:func:`nrf_modem_dect_phy_capability_get` function.
The list of supported capabilities is returned to the application in the :c:member:`nrf_modem_dect_phy_callbacks.capability_get` callback function.

Radio operations
****************

The DECT PHY interface offers the following three radio operations:

* Transmission (TX)
* Reception (RX)
* RSSI measurement

Each of these operations can be performed with different parameters.

The completion of a radio operation is signaled to the application by the invocation of the :c:member:`nrf_modem_dect_phy_callbacks.op_complete` callback function.
The callback receives the same handle that was specified by the application at the time the operation was scheduled.

Transmission
============

The application can schedule a transmission by calling the :c:func:`nrf_modem_dect_phy_tx` function.

The operation is asynchronous, and the successful completion of the :c:func:`nrf_modem_dect_phy_tx` function only signals that the request was sent to the modem.
When the operation has completed, its result is signaled to the application in the :c:member:`nrf_modem_dect_phy_callbacks.op_complete` callback function.
If any error has occurred in scheduling or executing the operation, it is returned in the callback.

The operation has several parameters, including Listen Before Talk (LBT) period and threshold.

Channel frequency
-----------------

The DECT radio band is divided into different channels, as described in chapter 5.2 of `ETSI TS 103 636-2`_.
The absolute radio channel frequency used for transmission is controlled by the :c:member:`nrf_modem_dect_phy_tx_params.carrier` field on the transmission parameters.

.. important::
   Follow the local regulations when transmitting.
   To see which frequency channels can be used:

   * In the European Union, you must refer to table 4.3.2.3-2 of `ETSI EN 301 406-2`_.
   * In the USA, you must refer to `47 CFR Part 15.323`_.
   * In all other territories, you must follow the local regulations.


Transmission length
-------------------

The length of a *transmission* is expressed in sub-slots (or half slots), where each sub-slot has a duration given by the subcarrier scaling factor.
A transmission may take up to 15 sub-slots, where the number of bits per sub-slot is decided by the chosen modulation scheme, as shown in the table below.
The length of the *data* being transmitted is expressed in bytes, and is specified by the :c:member:`nrf_modem_dect_phy_tx_params.data_size` parameter.

.. note::
   When transmitting, the length in bytes of the data being transmitted must match exactly the number of bits that can be transmitted during the length of the transmission in sub-slots.

For example, with modulation scheme 0 (MSC 0), a transmission with a data length of 17 bytes (136/8) would take exactly 2 sub-slots.
Using the same modulation scheme, a transmission of 3 sub-slots transmits 33 bytes (264/8), therefore, the length of the data being transmitted must be exactly 33 bytes.

.. table:: Bits per subslot index with given modulation scheme

   +---------------------+------+------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
   | MSC / Subslot-index | 0    | 1    | 2     | 3     | 4     | 5     | 6     | 7     | 8     | 9     | 10    | 11    | 12    | 13    | 14    | 15    |
   +---------------------+------+------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
   | MCS 0               | 0    | 136  | 264   | 400   | 536   | 664   | 792   | 920   | 1064  | 1192  | 1320  | 1448  | 1576  | 1704  | 1864  | 1992  |
   +---------------------+------+------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
   | MCS 1               | 32   | 296  | 552   | 824   | 1096  | 1352  | 1608  | 1864  | 2104  | 2360  | 2616  | 2872  | 3128  | 3384  | 3704  | 3960  |
   +---------------------+------+------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
   | MCS 2               | 56   | 456  | 856   | 1256  | 1640  | 2024  | 2360  | 2744  | 3192  | 3576  | 3960  | 4320  | 4768  | 5152  | 5536  | --    |
   +---------------------+------+------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
   | MCS 3               | 88   | 616  | 1128  | 1672  | 2168  | 2680  | 3192  | 3704  | 4256  | 4768  | 5280  | --    | --    | --    | --    | --    |
   +---------------------+------+------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
   | MCS 4               | 144  | 936  | 1736  | 2488  | 3256  | 4024  | 4832  | 5600  | --    | --    | --    | --    | --    | --    | --    | --    |
   +---------------------+------+------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+

.. note::
   For higher MCS levels, a better Signal-to-Noise Ratio (SNR) is needed to correctly decode the transmissions.
   Depending on transmission power and range, higher MCS levels may not be reached.

It is recommended to use transmission lengths of at most 4-6 sub-slots.

Hybrid ARQ
----------

The application can schedule a hybrid ARQ response transmission (HARQ feedback) by calling the :c:func:`nrf_modem_dect_phy_tx_harq` function.
A HARQ response transmission can be scheduled after a reception on the physical control channel (PCC).

Scheduling a HARQ response is time critical and therefore it must be done directly from the :c:member:`nrf_modem_dect_phy_callbacks.pcc` callback.
The MAC must have prepared data towards possible HARQ recipients in advance, so that it can directly call this function without further delays.

.. note::
   The application must always generate the HARQ feedback as NACK.

Because at the time the HARQ feedback transmission is scheduled the PDC is still being processed, the modem automatically adjusts the feedback to ACK afterwards, based on the PDC checksum data calculation.

Reception
=========

The application can schedule a reception by calling the :c:func:`nrf_modem_dect_phy_rx` function.

The operation is asynchronous, and the completion of the :c:func:`nrf_modem_dect_phy_rx` function only signals that the request was sent to the modem.

During reception, data received on the physical control channel (PCC) and on the physical data channel (PDC) is sent to the application in the :c:member:`nrf_modem_dect_phy_callbacks.pcc` and :c:member:`nrf_modem_dect_phy_callbacks.pdc` callback functions, respectively.
Any CRC errors on the physical control channel and on the physical data channel are sent to the application in the :c:member:`nrf_modem_dect_phy_callbacks.pcc_crc_err` and :c:member:`nrf_modem_dect_phy_callbacks.pdc_crc_err` callback functions, respectively.

When the operation has completed, its result is signaled to the application in the :c:member:`nrf_modem_dect_phy_callbacks.op_complete` callback function.
If any error has occurred in scheduling or executing the operation, it is returned in the callback.

Reception modes
---------------

A reception operation can have the following three different reception modes:

* Continuous- The reception is continued after PDC reception, up to its ``duration``.
* Semi-continuous- The reception is continued after PDC reception, unless it is a unicast PDC.
* One-shot- The reception is terminated after PDC reception.

In all reception modes, the reception will not extend past the operation duration specified in the :c:member:`nrf_modem_dect_phy_rx_params.duration` parameter.

Reception with RSSI measurement
-------------------------------

A reception operation may be combined with an RSSI measurement operation by configuring the :c:member:`nrf_modem_dect_phy_rx_params.rssi_interval` parameter.

Stopping reception
------------------

It is possible to stop the execution of reception operations by calling the :c:func:`nrf_modem_dect_phy_rx_stop` function.

A reception operation may only be stopped when it is currently being executed.

It is not possible to unschedule the execution of reception operations (or any others).

RSSI measurement
================

The application can schedule an RSSI measurement by operation by calling the :c:func:`nrf_modem_dect_phy_rssi` function.

The operation is asynchronous, and the completion of the :c:func:`nrf_modem_dect_phy_rssi` function only signals that the request was sent to the modem.

RSSI measurements are sent to the application in the :c:member:`nrf_modem_dect_phy_callbacks.rssi` callback function at a configurable interval, as specified by the :c:member:`nrf_modem_dect_phy_rssi_params.reporting_interval` field in the operation parameters.

When the operation has completed, its result is signaled to the application in the :c:member:`nrf_modem_dect_phy_callbacks.op_complete` callback function.
If any error has occurred in scheduling or executing the operation, it is returned in the callback.

Combined TX and RX operation
============================

You can schedule a combined TX and RX operation by calling the :c:func:`nrf_modem_dect_phy_tx_rx` function.
Scheduling two operations at once has the advantage of being faster, that is, it has a lower latency than scheduling the two halves of the operation separately.

The operations will be executed one after the other, starting with the TX operation.
The RX operation will be executed only if the TX operation has been completed successfully.

The start time of the RX operation is relative to the completion of the TX operation.

When the TX or RX part of the operation has completed, either successfully or unsuccessfully, its result is sent to the :c:member:`nrf_modem_dect_phy_callbacks.op_complete` callback function.
