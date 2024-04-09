.. _nrf_modem_dect_phy:

DECT NR+ PHY
############

.. contents::
   :local:
   :depth: 2

The DECT PHY interface in the Modem library is used to control the DECT NR+ physical layer (PHY) in the DECT NR+ modem firmware.

.. important::
   The DECT NR+ modem firmware is a variant of the nRF9161 modem firmware with a different feature set than the cellular modem firmware.
   In particular, the DECT NR+ modem firmware does not support cellular operation or the Global Navigation Satellite System (GNSS).
   Contact the Nordic Semiconductor sales department for more information on how to obtain the DECT NR+ modem firmware.

For more details about the capabilities of the nRF9161 hardware with regards to the DECT NR+ standard, and of the DECT NR+ PHY modem firmware implementation, refer to the `nRF9161 DECT NR+ product specification`_.

Protocol stack overview
***********************

The physical layer is only one part of the DECT-2020 New Radio protocol stack (DECT NR+).

The whole DECT NR+ protocol stack consist of four layers:

* PHY or physical layer
* MAC or medium access layer, including co-existing and channel access operation
* DLC or data link control, including routing, segmentation and re-assembly of messages
* CVG or convergence layer, including message flow control, multiplexing and transmission service level control

The DECT NR+ modem firmware implements only the PHY layer of the protocol stack.

.. important::
   DECT NR+ operates on free but regulated radio channels.
   The availability of the channels and the exact regulation to use them varies in different countries.
   It is the responsibility of the developer to operate the devices according to the local regulation, at development site and at the installation locations of the devices.
   To pass the certification, medium access layer (MAC) implementation is required and its operation is verified during the certification phase.

General operation
*****************

This section describes the characteristics and general operation of the DECT PHY interface.

Initialization
==============

The DECT PHY interface in the Modem library requires explicit initialization.

Before initializing the DECT PHY interface of the Modem library, the application must:

#. Initialize the Modem library by calling the :c:func:`nrf_modem_init` function. This also turns on the modem SoC.
#. Register callbacks for DECT PHY operations by calling the :c:func:`nrf_modem_dect_phy_callback_set` function.

Afterwards, the application can initialize the DECT PHY interface by calling the :c:func:`nrf_modem_dect_phy_init` function.
Upon successful initialization, both the DECT PHY interface and DECT NR+ PHY stack in the modem are ready for operation.

The DECT PHY interface can be de-initialized, which in turn de-initializes the PHY stack in the modem, cancelling all scheduled operations.

.. note::
   De-initializing the DECT PHY interface does not unset any callbacks set by the application using the :c:func:`nrf_modem_dect_phy_callback_set` function,
   nor it affects the modem SoC power status (on/off), which is instead controlled by the :c:func:`nrf_modem_init` and :c:func:`nrf_modem_shutdown` functions.

Once de-initialized, the DECT PHY interface can be re-initialized by calling the :c:func:`nrf_modem_dect_phy_init` function only.

Radio states
============

The radio states have implications on latency and power consumption. During operation, the radio can be in one of several states.

When a radio operation is being executed, the stack is in one of the following states:

* TX active: the radio is transmitting, no reception is ongoing.
* RX active: the radio is receiving, no transmission is ongoing.

When no radio operations is being executed, the stack is in the following state:

* Radio Idle: the radio is powered on and able to start a radio operation with relatively low latency, at the cost of increased power consumption compared sleep or off states.

Currently, the DECT PHY stack in the DECT NR+ modem firmware does not support deep sleep states.

Scheduling operations
=====================

The DECT PHY interface allows to schedule radio operations for execution by the scheduler of the DECT NR+ physical layer in the DECT NR+ modem firmware.
Due to the nature of a radio scheduler, which allows radio operations to be executed at a specific time in the future, all radio operations in the DECT PHY interface are asynchronous and their completion is signalled to the application using callbacks.

.. note::
   All scheduling is final, meaning that it is not possible to unschedule a scheduled operation.

A radio operation may be scheduled to execute a specific time in the future, or immediately, if the radio is not currently executing any other operation.

All radio operations have an application-defined handle which can be used to identify the operation and that is returned by the callback signalling the completion of an operation.
The operation handle is entirely opaque to all underlying layers, including the DECT PHY interface.

Modem time and operation latency
--------------------------------

Operation execution is scheduled by the application according to the modem time, which is a 64 bit counter kept by the modem.
All radio operation callbacks provide the value that the modem time counter had at the moment the callback was sent by the modem core to the application core, thus providing a way for the application to track the modem time without explicitly querying the modem for it.

If necessary, the application can retrieve the modem time counter value by calling the :c:func:`nrf_modem_dect_phy_time_get` function.

Both radio state transitions between operations in the modem core and the inter-processor communication (IPC) mechanism between the application and more cores have latency.
The latency of both radio state transitions and IPC is measured and provided by the DECT PHY interface upon initialization, by the :c:member:`nrf_modem_dect_phy_callbacks.init` callback function, in the :c:member:`nrf_modem_dect_phy_modem_cfg.latency` parameter.

.. note::
   The application must account for operations' latency when scheduling operations.

Temperature monitoring
======================

The DECT PHY interface reports the current SoC temperature as measured by the DECT NR+ modem firmware in all radio operation callbacks, and upon initialization.
This allows the application to track the changes in temperature and adjust further scheduling of operations accordingly, to ensure the device remains within safe operating temperatures.

The DECT NR+ modem firmware has an internal temperature protection mechanism that prevents the SoC from operating above safe temperature limits.
The operating temperature limit is reported upon initialization by the :c:member:`nrf_modem_dect_phy_callbacks.init` callback function, in the :c:member:`nrf_modem_dect_phy_modem_cfg.temperature_limit` parameter.

If the temperature threshold is reached, the modem cancels all scheduled radio operations and reports the :c:enum:`nrf_modem_dect_phy_err.NRF_MODEM_DECT_PHY_ERR_OP_CANCELED` error in their relative callbacks.
Additionally, it reject further scheduling of radio operations with the :c:enum:`nrf_modem_dect_phy_err.NRF_MODEM_DECT_PHY_ERR_TEMP_HIGH` error.

In this event, the application must de-initialize the Modem library PHY interface (and thus the Modem firmware DECT NR+ PHY stack) by calling :c:func:`nrf_modem_dect_phy_deinit` and allow the device to cool.

The application can then re-initialize the DECT PHY interface by calling :c:func:`nrf_modem_dect_phy_init`, and read the current measured temperature in the :c:member:`nrf_modem_dect_phy_callbacks.init` function parameters,
to ensure the temperature has decreased below the allowed threshold.

Physical layer capabilities
===========================

The application can retrieve the DECT NR+ modem firmware physical layer capabilities, as defined in annex B of the DECT NR+ physical layer standard, by calling the :c:func:`nrf_modem_dect_phy_capability_get` function.
The list of supported capabilities is returned to the application in the :c:member:`nrf_modem_dect_phy_callbacks.capability_get` callback function.

Radio operations
****************

The DECT PHY interface offers three radio operations: transmission (TX), reception (RX), and RSSI measurement.
Each of these operations can be performed with different parameters.

The completion of a radio operation is signalled to the application by the invocation of the :c:member:`nrf_modem_dect_phy_callbacks.op_complete` callback function, with the same handle that was specified by the application at the time the operation was scheduled.

Transmission
============

The application can schedule a transmission by calling the :c:func:`nrf_modem_dect_phy_tx` function.

The operation is asynchronous, and the successful completion of the :c:func:`nrf_modem_dect_phy_tx` function only signals that the request was sent to the modem.
When the operation has completed, its result is signalled to the application in the :c:member:`nrf_modem_dect_phy_callbacks.op_complete` callback function.
If any error has occurred in scheduling or executing the operation, it is returned in the callback.

The operation has several parameters, including listen-before-talk (LBT) period and threshold.

Hybrid ARC
----------

The application can schedule a hybrid ARC response transmission (HARQ feedback) by calling the :c:func:`nrf_modem_dect_phy_tx_harq` function.
A HARQ transmission response can be scheduled as a response to a reception (or reception failure) on the physical control channel (PCC).

Scheduling a HARQ response is time critical and therefore it must be done directly from the :c:member:`nrf_modem_dect_phy_callbacks.pcc` or :c:member:`nrf_modem_dect_phy_callbacks.pcc_crc_err` callbacks.
The MAC must have prepared data towards possible HARQ recipients in advance, so that it can directly call this function without further delays.

.. important::
   The application must always generate the HARQ feedback as NACK.

Because at the time the HARQ feedback transmission is scheduled the PDC is still being processed, the modem automatically adjusts the feedback to ACK afterwards, based on the PDC checksum data calculation.

Reception
=========

The application can schedule a reception by calling the :c:func:`nrf_modem_dect_phy_rx` function.

The operation is asynchronous, and the completion of the :c:func:`nrf_modem_dect_phy_rx` function only signals that the request was sent to the modem.

During reception, data received on the physical control channel (PCC) and on the physical data channel (PDC) is sent to the application in the :c:member:`nrf_modem_dect_phy_callbacks.pcc` and :c:member:`nrf_modem_dect_phy_callbacks.pdc` callback functions, respectively.
Any CRC errors on the physical control channel and on the physical data channel are sent to the application in the :c:member:`nrf_modem_dect_phy_callbacks.pcc_crc_err` and :c:member:`nrf_modem_dect_phy_callbacks.pdc_crc_err` callback functions, respectively.

When the operation has completed, its result is signalled to the application in the :c:member:`nrf_modem_dect_phy_callbacks.op_complete` callback function.
If any error has occurred in scheduling or executing the operation, it is returned in the callback.

Reception modes
---------------

A reception operation can have three different reception modes:

* Continuous: the reception is continued after PDC reception, up to its ``duration``.
* Semi-continuous: the reception is continued after PDC reception, unless it's a unicast PDC.
* One-shot: the reception is terminated after PDC reception.

In all reception modes, the reception will not extend past the operation duration, specified in the :c:member:`nrf_modem_dect_phy_rx_params.duration` parameter.

Reception with RSSI measurement
-------------------------------

A reception operation may be combined with an RSSI measurement operation by configuring the :c:member:`nrf_modem_dect_phy_rx_params.rssi_interval` parameter.

Stopping reception
------------------

It is possible to stop the execution of reception operations by calling the :c:func:`nrf_modem_dect_phy_rx_stop` function.

.. note::
   A reception operation may only be stopped when it is currently being executed.

It is not possible to unschedule the execution of reception operations (or any others).

RSSI measurement
================

The application can schedule an RSSI measurement by operation by calling the :c:func:`nrf_modem_dect_phy_rssi` function.

The operation is asynchronous, and the completion of the :c:func:`nrf_modem_dect_phy_rssi` function only signals that the request was sent to the modem.

RSSI measurements are sent to the application in the :c:member:`nrf_modem_dect_phy_callbacks.rssi` callback function at a configurable interval,
as specified by the :c:member:`nrf_modem_dect_phy_rssi_params.reporting_interval` field in the operation parameters.

When the operation has completed, its result is signalled to the application in the :c:member:`nrf_modem_dect_phy_callbacks.op_complete` callback function.
If any error has occurred in scheduling or executing the operation, it is returned in the callback.

Combined TX/RX operations
=========================

It is possible to schedule a combined TX and RX operation by calling the :c:func:`nrf_modem_dect_phy_tx_rx` function.
Scheduling two operations at once has the advantage of being faster, that is, has a lower latency than scheduling the two halves of the operation separately.

The operations will be executed one after the other, starting with the TX operation.
The RX operation will be executed only if the TX operation has completed successfully.

The start time of the RX operation is relative to the completion of the TX operation.

When the TX or RX part of the operation has completed, either successfully or unsuccessfully, its result is sent to the :c:member:`nrf_modem_dect_phy_callbacks.op_complete` callback function.
