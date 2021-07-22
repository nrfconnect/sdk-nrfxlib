.. _nrf_modem_at:

AT interface
############

.. contents::
   :local:
   :depth: 2

The Modem library supports sending AT commands to the modem, reading responses, and receiving AT notifications using the :ref:`nrf_modem_at_api` found in :file:`nrfxlib/nrf_modem/include/nrf_modem_at.h`.

AT commands
***********

AT commands are essentially a set of modem instructions that are used to configure the modem, establish a network connection (or in general, execute operations), and retrieve modem and connection status information.
For the full set of supported AT commands, see the `AT Commands Reference Guide`_.

Initialization
**************

The AT interface can be used once the application initializes the Modem library in :c:enum:`NORMAL_MODE` by calling :c:func:`nrf_modem_init`.

Sending AT commands
*******************

The application can use :c:func:`nrf_modem_at_printf` to send a formatted AT command to the modem for generic result of the operation, for example, ``OK``, ``ERROR``, ``+CME ERROR`` or ``+CMS ERROR``.
This function works similarly to :c:func:`printf` from the standard C library, and it supports all the format specifiers supported by the :c:func:`printf` implementation of the selected C library.

The following snippet shows how to use :c:func:`nrf_modem_at_printf` to send a formatted AT command to the modem and check the result of the operation:

.. code-block:: c

	int cfun_control(int mode)
	{
		int err;

		err = nrf_modem_at_printf("AT+CFUN=%d", mode);
		if (err = 0) {
			/* OK, success */
		} else if (err < 0) {
			/* Failed to send command, err is an nrf_errno */
		} else if (err > 0) {
			/* Command was sent, but response is not "OK" */
			switch(nrf_modem_at_err_type(err)) {
			case NRF_MODEM_AT_ERROR:
				/* Modem returned "ERROR" */
				printf("error");
				break;
			case NRF_MODEM_AT_CME_ERROR:
				/* Modem returned "+CME ERROR" */
				printf("cme error: %d", nrf_modem_at_err(err));
				break;
			case NRF_MODEM_AT_CMS_ERROR:
				/* Modem returned "+CMS ERROR" */
				printf("cms error: %d", nrf_modem_at_err(err));
				break;
			}
		}
		return err;
	}

	int foo(void)
	{
		/* Send AT+CFUN=1 */
		cfun_control(1);
		/* Send AT+CFUN=4 */
		cfun_control(4);
	}

Any return value other than zero indicates an error.
Negative values indicate that the Modem library has failed to send the AT command, and they represent a nrf_errno code that indicates the reason for the failure.
Positive values indicate that the modem has received the AT command and has responded with an error.
When a positive value is returned, the error type can be retrieved using the :c:func:`nrf_modem_at_err_type` helper function, and the error value (in case of CME or CMS errors) can be retrieved with the :c:func:`nrf_modem_at_err` helper function.

Reading data from an AT response
********************************

Use :c:func:`nrf_modem_at_scanf` to send an AT command to the modem and parse the response according to a specified format.
This function works similarly to :c:func:`scanf` from the standard C library, and it supports all the format specifiers supported by the :c:func:`scanf` implementation of the selected C library.

The following snippet shows how to use :c:func:`nrf_modem_at_scanf` to read the modem network registration status using ``AT+CEREG?``

.. code-block:: c

	void cereg_read(void)
	{
		int rc;
		int status;

		/* The `*` sub-specifier discards the result of the match.
		 * The data is read but it is not stored in any argument.
		 */
		rc = nrf_modem_at_scanf("AT+CEREG?", "+CEREG: %*d,%d", &status);

		/* Upon returning, `rc` contains the number of matches */
		if (rc == 1) {
			/* We have matched one argument */
			printf("Network registration status: %d\n", status);
		} else {
			/* No arguments where matched */
		}
	}

Advanced usage
**************

The remaining two functions, :c:func:`nrf_modem_at_cmd` and its asynchronous version, :c:func:`nrf_modem_at_cmd_async`, cover more advanced use cases.

The application can use :c:func:`nrf_modem_at_cmd` to send a formatted AT command to the modem and copy the AT response into the buffer that is supplied to the function.
The application can then parse the buffer as necessary, for example, by using the C library function :c:func:`sscanf`, thus achieving the combined functionality of :c:func:`nrf_modem_at_printf` and :c:func:`nrf_modem_at_scanf`.
The following snippet shows how to use the :c:func:`nrf_modem_at_cmd` function to change the function mode by using the ``AT+CFUN`` command and read the modem response:

.. code-block:: c

	void foo(void)
	{
		int err;
		char response[64];

		err = nrf_modem_at_cmd(response, sizeof(response), "AT+CFUN=%d", 1);
		if (err) {
			/* error */
		}

		/* buffer contains the whole response */
		printf("Modem response:\n%s", response);
	}

The application can use :c:func:`nrf_modem_at_cmd_async` to send a formatted AT command and receive the whole response asynchronously through the provided callback function.
Only one asynchronous command can be pending at any time.

The following snippet shows how to use the :c:func:`nrf_modem_at_cmd_async` function to change the function mode by using the ``AT+CFUN`` command and read the modem response:

.. code-block:: c

	void resp_callback(const char *at_response)
	{
		printf("AT response received:\n%s", at_response);
	}

	void foo(void)
	{
		int err;

		err = nrf_modem_at_cmd_async(resp_callback, "AT+CFUN=%d", 1);
		if (err) {
			/* error */
		}
	}

.. note::
   The callback function is executed in an interrupt service routine.
   The user is responsible for rescheduling any processing of the response as appropriate.

   When there is a pending response, all other functions belonging to the AT API will block until the response is received in the callback function.

Receiving AT notifications
**************************

The Modem library can dispatch incoming AT notifications from the modem to a user-provided callback function set by :c:func:`nrf_modem_at_notif_handler_set`.
Only one callback function must be registered with the Modem library.
Registering a new callback function will override any callback previously set.
Multiple parts of an application might require receiving of AT notifications, thus AT notifications need to be dispatched as necessary.
In |NCS|, :ref:`at_monitor_readme` library takes care of the dispatching of notifications.

The following snippet shows how to setup an AT notification handler:

.. code-block:: c

	void notif_callback(const char *at_notification)
	{
		printf("AT notification received: %s\n", at_notification);
	}

	int foo(void)
	{
		int err;

		err = nrf_modem_at_notif_handler_set(notif_callback);
		if (err) {
			/* error */
		}

		return 0;
	}

.. note::

   The callback is invoked in an interrupt context.
   The user is responsible for rescheduling the processing of AT notifications as appropriate.
   In |NCS|, the :ref:`at_monitor_readme` library and the :ref:`nrf_modem_lib_readme` takes care of the rescheduling.

Thread safety
*************

The AT API is thread safe and can be used by multiple threads.

Use with legacy AT socket
*************************

The AT interface and the AT socket do not interfere with each other, and it is possible to use both in the same application.

.. note::
   Usage of the AT interface does not count towards the maximum number of AT and IP sockets.
