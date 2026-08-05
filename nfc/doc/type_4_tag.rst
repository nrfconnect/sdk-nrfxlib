.. _type_4_tag:

Type 4 Tag
##########

.. contents::
   :local:
   :depth: 2

The Type 4 Tag implementation is based on the NFC Forum document *Type 4 Tag Technical Specification Version 1.0 2017-08-28 [T4T]*.

Overview
********

A Type 4 Tag must contain at least the NFC Data Exchange Format (NDEF) tag application.
This application provides a file system that consists of at least two Elementary Files (EFs):

Capability container (CC)
  The CC is a read-only metafile that contains the version of the implemented specification, communication parameters of the tag, and properties of all the other EF files that are present on a Type 4 Tag platform.

NDEF file
  The NDEF file (see :ref:`t4t_format`) contains the NDEF message.
  Depending on the file properties defined in the CC file, the message can be read or rewritten.

The Type 4 Tag library uses Application Protocol Data Units (APDUs) to communicate with polling NFC devices over the ISO-DEP (ISO14443-4A) protocol.
As with the :ref:`type_2_tag`, the Type 4 Tag supports NFC-A listen mode.
In this mode, the tag waits for polling devices and does not actively initiate a connection.

Supported device families
=========================

The Type 4 Tag library supports the following device families:

* nRF52 Series
* nRF53 Series
* nRF54L Series
* nRF54H Series

Available emulation modes
=========================

The Type 4 Tag library supports the following emulation modes:

.. list-table:: Emulation modes
   :header-rows: 1

   * - Mode
     - Payload function
     - Buffer content
     - Length argument
     - Key callback events
     - Typical use case
   * - Read-only NDEF tag
     - :c:func:`nfc_t4t_ndef_staticpayload_set`
     - NDEF message
     - Encoded message size
     - :c:enumerator:`NFC_T4T_EVENT_FIELD_ON`, :c:enumerator:`NFC_T4T_EVENT_FIELD_OFF`, and :c:enumerator:`NFC_T4T_EVENT_NDEF_READ`
     - Static content, such as a URL or text, that readers cannot overwrite
   * - Read-write NDEF tag
     - :c:func:`nfc_t4t_ndef_rwpayload_set`
     - NDEF file (NLEN + message)
     - Buffer capacity
     - :c:enumerator:`NFC_T4T_EVENT_FIELD_ON`, :c:enumerator:`NFC_T4T_EVENT_FIELD_OFF`, :c:enumerator:`NFC_T4T_EVENT_NDEF_READ`, and :c:enumerator:`NFC_T4T_EVENT_NDEF_UPDATED`
     - Tags that a smartphone can reprogram (for example, with NFC Tools)
   * - Raw ISO-DEP
     - Not used (skip payload setup)
     - Not applicable
     - Not applicable
     - :c:enumerator:`NFC_T4T_EVENT_FIELD_ON`, :c:enumerator:`NFC_T4T_EVENT_FIELD_OFF`, :c:enumerator:`NFC_T4T_EVENT_DATA_IND`, and :c:enumerator:`NFC_T4T_EVENT_DATA_TRANSMITTED`
     - Custom ISO-DEP protocols that do not use NDEF tag emulation

See :ref:`t4t_mode_selection` for information about selecting an emulation mode.

.. _t4t_format:

NDEF file format
================

The NDEF file is a file type that can be present on a Type 4 Tag platform.
It consists of the following fields:

.. list-table::
   :header-rows: 1

   * - Field
     - Length
     - Description
   * - NDEF Length (NLEN)
     - 2 bytes
     - Length of the NDEF message in big-endian format.
   * - NDEF Message
     - NLEN bytes
     - NDEF message.
       See :ref:`nrf:ug_nfc_ndef`.

Compared to a raw NDEF message, the NDEF file adds the mandatory NLEN field.
This field encodes the total length of the NDEF message.

.. note::

   The NDEF file is required only when you implement the read-write NDEF tag mode or the raw ISO-DEP tag mode.
   The read-only NDEF tag mode does not require it.

Configuration
*************

Follow the information in this section to enable the library for your project.

.. _t4t_mode_selection:

Choosing an emulation mode
==========================

The payload function that you call during initialization determines the emulation mode.
The selected mode takes effect when you call :c:func:`nfc_t4t_emulation_start`.

Choosing read-only NDEF tag mode
--------------------------------

Start with the read-only NDEF tag mode if you are new to NFC tag emulation.

This mode requires the least setup:

1. Encode an NDEF message.
2. Pass the message to :c:func:`nfc_t4t_ndef_staticpayload_set`.
3. Start the emulation.

Because the library never modifies the buffer, it can reside in flash memory.


Choosing read-write NDEF tag mode
---------------------------------

Use read-write NDEF tag mode when an external reader must be able to update the tag content.

This mode requires the following elements:

* A RAM buffer that remains valid for the entire duration of the emulation.
* Initial content in the NDEF file format (see :ref:`t4t_format`).
* Handling of :c:enumerator:`NFC_T4T_EVENT_NDEF_UPDATED` in the callback.

See :ref:`t4t_read_write`.

Choosing raw ISO-DEP tag mode
-----------------------------

Use the raw ISO-DEP mode only when you need full control over the APDU exchange and do not want the library to emulate an NDEF tag file system.


.. _t4t_configuring:

Configuring tag emulation
=========================

If you use the supplied library, you do not need to apply the :ref:`t4t_format` to get started with the read-only mode.
To emulate a Type 4 Tag using one of the modes, complete the following steps:

1. Implement a callback function to handle events from the Type 4 Tag library:

   .. code-block:: c

      int err;
      /* Callback for NFC events */
      static void nfc_callback(void *context,
			       nfc_t4t_event_t event,
			       const uint8_t *data,
			       size_t data_length,
			       uint32_t flags)
      {
	      switch (event) {
	      case NFC_T4T_EVENT_FIELD_ON:
		      /* NFC field detected */
		      break;
	      case NFC_T4T_EVENT_FIELD_OFF:
		      /* NFC field removed */
		      break;
	      case NFC_T4T_EVENT_NDEF_READ:
		      /* NDEF data has been read */
		      break;
	      default:
		      break;
	      }
      }

#. Register the function by calling :c:func:`nfc_t4t_setup`:

   .. code-block:: c

      /* Set up NFC and register the application callback for NFC events. */
      err = nfc_t4t_setup(nfc_callback, NULL);
      if (err) {
	      printk("Cannot set up NFC T4T library!\n");
	      return err;
      }

#. Configure the tag data.

   The payload function that you call determines the emulation mode (see :ref:`t4t_mode_selection`):

   * For the read-only NDEF tag emulation, use :c:func:`nfc_t4t_ndef_staticpayload_set`.
     Pass a buffer that contains the NDEF message and its length (see :ref:`nrf:ug_nfc_ndef`).
     The library adds the NDEF file structure internally, so you do not need to add the NLEN field.

     .. code-block:: c

        uint8_t ndef_msg_buf[256];
        uint32_t len;

        /* Encode the NDEF message into ndef_msg_buf and set len to the encoded size. */

        /* Set the encoded message as the NFC payload. */
        err = nfc_t4t_ndef_staticpayload_set(ndef_msg_buf, len);
        if (err) {
        	printk("Cannot set payload!\n");
        	return err;
        }

   * For the read-write NDEF tag emulation, use :c:func:`nfc_t4t_ndef_rwpayload_set`.
     See :ref:`t4t_read_write` for details.

   * For the raw ISO-DEP mode, skip this step.
     The library delivers all APDUs through the callback as :c:enumerator:`NFC_T4T_EVENT_DATA_IND` events.
     The application must respond by calling :c:func:`nfc_t4t_response_pdu_send`.

#. Activate the NFC tag so that it starts sensing the NFC field and responds when a reader is present:

   .. code-block:: c

      /* Start sensing NFC field. */
      err = nfc_t4t_emulation_start();
      if (err) {
           printk("Cannot start emulation!\n");
           return err;
      }

   The selected emulation mode takes effect when you call :c:func:`nfc_t4t_emulation_start`.

.. _t4t_read_write:

Configuring read-write NDEF tag data
------------------------------------

To allow an external reader to overwrite the NDEF content, configure the tag in the read-write mode instead of the read-only mode.
This means calling :c:func:`nfc_t4t_ndef_rwpayload_set` instead of :c:func:`nfc_t4t_ndef_staticpayload_set`.
The library handles read and update procedures internally.
The application provides the buffer and responds to update events.

Complete the following steps when you are :ref:`t4t_configuring`, before calling :c:func:`nfc_t4t_emulation_start`:

1. Allocate a writable buffer for the NDEF file.

   The buffer must remain valid for the entire lifetime of the emulation.
   Its size defines the maximum NDEF file size, including the 2-byte NLEN field.

   .. code-block:: c

      static uint8_t ndef_file_buf[1024]; /* Buffer for the NDEF file. */

#. Prepare the initial NDEF file content.

   This means encoding the NDEF message in the area after NLEN, then writing the message size to NLEN.

   When using the |NCS|, you can use the :ref:`nrf:nfc_t4t_ndef_file_readme` library:

   .. code-block:: c

      uint32_t ndef_size = nfc_t4t_ndef_file_msg_size_get(sizeof(ndef_file_buf));

      /* Encode the NDEF message after the NLEN field. */
      err = nfc_ndef_uri_msg_encode(NFC_URI_HTTP_WWW, url, url_len,
				    nfc_t4t_ndef_file_msg_get(ndef_file_buf),
				    &ndef_size);
      if (err) {
	      return err;
      }

      /* Write the NLEN field to complete the NDEF file. */
      err = nfc_t4t_ndef_file_encode(ndef_file_buf, &ndef_size);
      if (err) {
	      return err;
      }

#. Register the buffer for read-write emulation.

   This means passing the full buffer capacity to :c:func:`nfc_t4t_ndef_rwpayload_set` (not the current NDEF file size):

   .. code-block:: c

      err = nfc_t4t_ndef_rwpayload_set(ndef_file_buf, sizeof(ndef_file_buf));
      if (err) {
	      printk("Cannot set payload!\n");
	      return err;
      }

#. Extend the callback to handle write events.

   .. code-block:: c

      case NFC_T4T_EVENT_NDEF_UPDATED:
	      if (data_length > 0) {
		      /* The NDEF file was updated; ndef_file_buf contains the new content.
		       * Persist or process the updated data here.
		       */
	      }
	      break;

   When a reader updates the NDEF file, the library signals :c:enumerator:`NFC_T4T_EVENT_NDEF_UPDATED`.
   The ``data_length`` parameter contains the current value of the NLEN field.
   A non-zero value indicates that the write operation has completed and that the buffer contains the updated NDEF file.
   The library may also signal this event with ``data_length`` set to zero while the reader erases the file at the start of an update.

After you start emulation with :c:func:`nfc_t4t_emulation_start`, the tag accepts write operations from NFC reader devices, such as the NFC Tools mobile application.


Read-only implementation template
=================================

The following example shows a minimal end-to-end read-only setup that you can use as a template for your own implementation:

.. code-block:: c

   static void nfc_callback(void *context, nfc_t4t_event_t event,
			    const uint8_t *data, size_t data_length,
			    uint32_t flags)
   {
	   ARG_UNUSED(context);
	   ARG_UNUSED(data);
	   ARG_UNUSED(flags);
   }
   int start_nfc_tag(const uint8_t *ndef_msg, size_t ndef_len)
   {
	   int err;
	   err = nfc_t4t_setup(nfc_callback, NULL);
	   if (err) {
		   return err;
	   }
	   err = nfc_t4t_ndef_staticpayload_set(ndef_msg, ndef_len);
	   if (err) {
		   return err;
	   }
	   return nfc_t4t_emulation_start();
   }


Samples using the library
*************************

See the :ref:`nrf:writable_ndef_msg` sample for a complete application that stores updated NDEF files in flash.

.. _nfc_api_type4:

API documentation
*****************

.. doxygengroup:: nfc_t4t_lib
