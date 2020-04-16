.. _gnss_extension:

GNSS socket
###########

`Global navigation satellite system (GNSS)`_ socket is one of the socket types supported by the BSD library.
This socket type is used to configure and fetch GPS position fix data from the GPS module, and to write `A-GPS`_ data to the GPS module.

Creating a GNSS socket
**********************

The following code shows how to create a GNSS socket with the :c:type:`NRF_PROTO_GNSS` protocol family and the 
:c:type:`NRF_AF_LOCAL` proprietary address family.

.. code-block:: c

   gnss_fd = nrf_socket(NRF_AF_LOCAL, NRF_SOCK_DGRAM, NRF_PROTO_GNSS);

The socket type is :c:type:`NRF_SOCK_DGRAM`.
The code above creates a socket that can be used to communicate with the GPS module.
After the function call, the GPS module will be in the stopped mode (i.e. the module does not generate GPS fixes).

.. note:: Only one instance of the GNSS socket is supported.

Starting the GPS
****************

After the GNSS socket is created, the GPS module must be started for the module to start generating GPS fixes.
This is done using the :cpp:func:`nrf_setsockopt` function with the :c:type:`NRF_SOL_GNSS` socket option level and the :c:type:`NRF_SO_GNSS_START` socket option.

.. code-block:: c

   nrf_gnss_delete_mask_t  delete_mask  = 0;

   nrf_setsockopt(gnss_fd, NRF_SOL_GNSS, NRF_SO_GNSS_START, &delete_mask, sizeof(delete_mask));



During a session (when the GPS is running), the socket stores the information to the non-volatile memory.
This is done to generate a quick fix in a subsequent session and is termed as a hot or warm start (depending on the data that is used from the last session).
Such session data can be deleted using the delete mask that is supplied to the :cpp:func:`nrf_setsockopt` function call while starting the GPS module.
A value of 0 in the delete mask is an indication to keep all the previous data.
The bit masks for the different types of data that can be deleted is defined in the GNSS socket API documentation of :cpp:type:`nrf_gnss_delete_mask_t`.

Stopping the GPS
****************

The GPS module can be stopped through the GNSS socket by using the :c:type:`NRF_SO_GNSS_STOP` socket option. 
A delete mask must also be supplied when stopping the GPS module as shown in the following code:

.. code-block:: c

   nrf_gnss_delete_mask_t  delete_mask  = 0;

   nrf_setsockopt(gnss_fd, NRF_SOL_GNSS, NRF_SO_GNSS_STOP, &delete_mask, sizeof(delete_mask));


Configuring the GPS
*******************

There are various configuration parameters that can be sent to the GPS module.
Configuration parameters can only be sent to the GPS module when it is in the stopped state.
A configuration parameter is written to the GPS module through the :cpp:func:`nrf_setsockopt` function call.

Below example code shows how to set the fix interval:

.. code-block:: c

   nrf_gnss_fix_interval_t fix_interval = 5;

   nrf_setsockopt(gnss_fd, NRF_SOL_GNSS, NRF_SO_GNSS_FIX_INTERVAL, &fix_interval, sizeof(fix_interval));

A configuration can also be read by the application by using the :cpp:func:`nrf_getsockopt` function call as shown in the following code:

.. code-block:: c

   uint32_t   fix_interval_len = sizeof(fix_interval);

   nrf_getsockopt(gnss_fd, NRF_SOL_GNSS, NRF_SO_GNSS_FIX_INTERVAL, &fix_interval, &fix_interval_len);


Configuration parameters
========================

Fix interval
   | Socket option:  :c:type:`NRF_SO_GNSS_FIX_INTERVAL`
   | Datatype:       :cpp:type:`nrf_gnss_fix_interval_t`
   | Default:        1
   | Allowed values: 0, 1, 10..1800

The fix interval parameter controls the time between GNSS receiver-starts.
It also controls the stopping of the GNSS receiver after a valid PVT
estimate.
The fix interval parameter determines the mode of navigation.
There are three different navigation modes available: single-fix,
continuous, and periodic.

Single-fix navigation mode is engaged by setting the fix interval to 0.
In this mode, once a valid PVT estimate is produced, the GNSS receiver is turned off indefinitely.
It does not resume navigation without explicit actions by the application processor.
To do another single-fix, the application processor must first stop the GNSS module and then start the GNSS module again.

Continuous navigation mode is engaged by setting the fix interval to 1.
In this mode, the GNSS receiver continues to produce fixes at 1 Hz rate without any time limit.

Periodic navigation mode is engaged by setting the fix interval to a value other than 0 or 1.
In this mode the GNSS receiver is turned off after each valid PVT estimate, and turned back on periodically after each fix interval has passed.

Fix retry
   | Socket option:  :c:type:`NRF_SO_GNSS_FIX_RETRY`
   | Datatype:       :cpp:type:`nrf_gnss_fix_retry_t`
   | Default:        60
   | Allowed values: 0..65535

The fix retry parameter controls the maximum time the GNSS receiver can run while trying to produce a valid PVT estimate.
If the fix retry time is non-zero, the GNSS receiver is turned off after the fix retry time is up regardless of whether a valid PVT estimate was produced or not.
If the fix retry parameter is set to 0, the GNSS receiver can run indefinitely until a valid PVT estimate is produced.

NMEA mask
   | Socket option: :c:type:`NRF_SO_GNSS_NMEA_MASK`
   | Datatype:      :cpp:type:`nrf_gnss_nmea_mask_t`
   | Default:       0x0000
   | Allowed values:

   | :c:type:`NRF_GNSS_NMEA_GGA_MASK`
   | :c:type:`NRF_GNSS_NMEA_GLL_MASK`
   | :c:type:`NRF_GNSS_NMEA_GSA_MASK`
   | :c:type:`NRF_GNSS_NMEA_GSV_MASK`
   | :c:type:`NRF_GNSS_NMEA_RMC_MASK`

This mask is used to enable different `NMEA`_ string types.
Multiple NMEA string types can be enabled at the same time.

Elevation mask
   | Socket option:  :c:type:`NRF_SO_GNSS_ELEVATION_MASK`
   | Datatype:       :cpp:type:`nrf_gnss_elevation_mask_t`
   | Default         5
   | Allowed values: 0 (horizontal)..90

The elevation mask parameter controls the elevation threshold angle (in degrees).
Satellites with elevation angle less than the threshold are automatically excluded from the PVT estimation.

Use case
   | Socket option:  :c:type:`NRF_SO_GNSS_USE_CASE`
   | Datatype:       :c:type:`uint8_t`
   | Default         1
   | Allowed values: 0, 1

The use case parameter determines the target performance.
A value of 0 indicates single cold start and one indicates multiple hot starts.

Start GPS module
   | Socket option: :c:type:`NRF_SO_GNSS_START`
   | Datatype:      :cpp:type:`nrf_gnss_delete_mask_t`
   | Default:       NA

This parameter makes the GPS module start generating fixes.
The delete mask is used to delete the data that the GPS module has stored for any previous session.

Stop GPS module
   | Socket option: :c:type:`NRF_SO_GNSS_STOP`
   | Datatype:      :c:type:`nrf_gnss_delete_mask_t`
   | Default        NA

This parameter makes the GPS module stop generating fixes.
The delete mask is used to delete data that the GPS module has stored from any previous session.

Power save modes
   | Socket option: :c:type:`NRF_SO_GNSS_POWER_SAVE_MODE`
   | Datatype:      :cpp:type:`nrf_gnss_power_save_mode_t`
   | Default:       :c:type:`NRF_GNSS_PSM_DISABLED`
   | Allowed values:

   | :c:type:`NRF_GNSS_PSM_DISABLED`
   | :c:type:`NRF_GNSS_PSM_DUTY_CYCLING_PERFORMANCE`
   | :c:type:`NRF_GNSS_PSM_DUTY_CYCLING_POWER`

The GNSS receiver may track signals (i.e. run) either continuously or in duty cycles.

In continuous tracking, the received GNSS signal has no gaps.
Best performance is achieved with continuous tracking.

In duty-cycled tracking, the GNSS receiver operates in on-and-off cycles consisting of tracking and sleeping.
The received GNSS signal has therefore short gaps during the sleep periods.
This type of tracking reduces the power consumption at the expense of performance.

A duty cycle denotes the fraction of one cycle in which the GNSS receiver is tracking.
In duty-cycled tracking the tracking period is fixed to 100 ms, and the sleeping period is fixed to 400 ms.
Consequently, the total time needed to complete an on-and-off cycle is 500 ms.
These values give 20% duty cycle.
Since the GPS data bit length is 20 ms, and the smallest meaningful data primitive is 30-bit word (i.e. 600 ms), it is impossible to decode any data during duty-cycled tracking.
This has significant impact on the performance.
It means that any new GNSS signal cannot be utilized before any of the following conditions occur:

* Continuous tracking is resumed to enable the decoding of the navigation data
* A-GPS is used to provide relevant navigation data

The available power modes determine whether duty-cycled tracking is allowed.
If it is allowed, the target performance is determined.

In the performance duty-cycled power mode, duty-cycled tracking is engaged and run when conditions allow it without significant performance degradation.
In the power duty-cycled power mode, duty-cycled tracking is engaged and run whenever it is possible with acceptable performance degradation.

The GNSS receiver continues to produce PVT estimates at the configured rate regardless of whether the tracking is continuous or in duty cycles.
However, a failure to produce a valid PVT estimate during duty-cycled tracking may cause the GNSS receiver to resume continuous tracking.

Enable priority
   | Socket option: :c:type:`NRF_SO_GNSS_ENABLE_PRIORITY`
   | Datatype:      NA (:c:type:`void`)
   | Default:       NA

Disable priority
   | Socket option: :c:type:`NRF_SO_GNSS_DISABLE_PRIORITY`
   | Datatype:      NA (:c:type:`void`)
   | Default:       NA

Reading a fix
*************

To read a data fix, the socket read function :cpp:func:`nrf_recv` is used with the GNSS socket descriptor as the function argument.
This is shown in the following code:

.. code-block:: c

  nrf_gnss_data_frame_t gps_data;
  int                   ret;

  ret = nrf_recv(gnss_fd, &gps_data, sizeof(nrf_gnss_data_frame_t), 0)

As this is a datagram socket, each successful read contains a complete frame of data.
The :cpp:func:`nrf_recv` read function can return three types of data frames.
The data type is identified by the ``data_id`` parameter in the received frame.
The three datatypes that are currently supported are :c:type:`NRF_GNSS_PVT_DATA_ID`, :c:type:`NRF_GNSS_AGPS_DATA_ID` and :c:type:`NRF_GNSS_NMEA_DATA_ID`.
The following code shows how the the position data is displayed based on the :c:type:`NRF_GNSS_PVT_DATA_ID` and :c:type:`NRF_GNSS_NMEA_DATA_ID` datatypes:

.. code-block:: c

   if (ret > 0)
   {
       switch (gps_data.data_id)
       {
           case NRF_GNSS_PVT_DATA_ID:
               if ((gps_data.pvt.flags &
                       NRF_GNSS_PVT_FLAG_FIX_VALID_BIT)
                       == NRF_GNSS_PVT_FLAG_FIX_VALID_BIT)
               {
                   printf("Longitude: %f\n", gps_data.pvt.longitude);
                   printf("Latitude:  %f\n", gps_data.pvt.latitude);
                   printf("Alitude:   %f\n", gps_data.pvt.altitude);
               }
               break;
           case NRF_GNSS_NMEA_DATA_ID:
               printf("NMEA string: %s\n", gps_data.nmea);
               break;
           default:
               break;
       }
   }


Fixes are always received in the ``pvt`` format.
The format of this frame is defined in the GNSS API documentation of :cpp:type:`nrf_gnss_pvt_data_frame_t`.

A-GPS data
**********

The GPS module automatically requests A-GPS data when the module determines that the existing data is outdated.
The A-GPS data frame is described in GNSS API documentation of :cpp:type:`nrf_gnss_agps_data_frame_t`.
This request data frame type can be read with the :cpp:func:`nrf_recv` function with the :c:type:`NRF_GNSS_AGPS_DATA_ID` id as shown in the following code:


.. code-block:: c

   nrf_gnss_data_frame_t gps_data;

   ret = nrf_recv(gnss_fd, &gps_data, sizeof(nrf_gnss_data_frame_t), 0);

   if ((ret > 0) && (gps_data.data_id == NRF_GNSS_AGPS_DATA_ID))
   {
       // Process APGS request
   }


When the A-GPS data is downloaded, it can be written to the GPS module using the :cpp:func:`nrf_sendto` function.
The ``servaddr`` parameter is used to select the A-GPS data model to which the data should be written to.
The available A-GPS data models are listed in the API documentation for :ref:`gnss_socket_agps_df`.

The following code shows how the A-GPS data is written to the GPS module.

.. code-block:: c

   nrf_gnss_agps_data_utc_t  utc_data;
   nrf_gnss_agps_data_type_t apgs_data_id = NRF_GNSS_AGPS_UTC_PARAMETERS;

   nrf_sendto(gnss_fd, utc_data, sizeof(utc_data), agps_data_id, sizeof(agps_data_id));

Closing a GNSS socket
*********************

The :cpp:func:`nrf_close` function is used to close a GNSS socket.
This function is called with the file descriptor associated with the GNSS socket that was created.

.. code-block:: c

   nrf_close(gnss_fd);
