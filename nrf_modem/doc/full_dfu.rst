.. _full_dfu:

Full firmware updates
#####################

.. contents::
   :local:
   :depth: 2

The modem library supports the full update of the modem firmware when the library is initialized in full DFU mode by the :file:`nrf_modem_full_dfu.h` interface.
Full firmware update is the only operation that can be performed in full DFU mode.
The sockets are not available to the application.

It is possible to switch between the full DFU and normal operation modes by reinitializing the library.

Memory requirements
*******************

The total size of all the shared memory regions must be at least 8220 bytes.
This is because programming requires 8 kB data buffer and some storage for structures used in RPC communication.

.. note::

   When using |NCS|, the library is initialized by the glue, which will configure the size and position of the shared memory regions in RAM.
   Use the Kconfig options belonging to the glue to ensure that the total size of the shared memory region is sufficiently large.

Modem library initialization
============================

To perform a full firmware update of the modem, the library must be initialized in ``FULL_DFU`` mode as shown in the following code:

.. code-block:: c

   /* Initialize modem to prepare for DFU */
   nrf_modem_init(init_params, FULL_DFU_MODE);

If the library has already been initialized in ``NORMAL_MODE`` mode, it must be shut down through the :c:func:`nrf_modem_shutdown` function and reinitialized as shown in the following code:

.. code-block:: c

   nrf_modem_init(init_params, NORMAL_MODE);
   /* Shutdown and re-initialize modem to prepare for DFU */
   nrf_modem_shutdown();
   nrf_modem_init(init_params, FULL_DFU_MODE);

Considerations for corrupted modem firmware
===========================================

When designing your application, you might have to consider an interrupted DFU process, which can lead to corrupted modem firmware.
There can be various reasons (for example, power outage) that can cause an interruption in a firmware update.
The modem does not have a back-up ROM and hence, an interruption in DFU can prevent the modem from further boot up.

As a workaround to the above scenario, either the application must tolerate the situation or another means of recovery must be provided  (for example, reprogramming the modem with PC tools).

You must then initialize the modem library manually by using the following code:

.. code-block:: c

	int main(void)
	{
		int rc = nrf_modem_init(init_params, NORMAL_MODE);

		if (rc < 0) {
			/* Recover the modem firmware from external flash */
			nrf_modem_init(init_params, FULL_DFU_MODE);
			/* Perform FULL modem firmware update. */
			nrf_modem_shutdown();
			nrf_modem_init(init_params, NORMAL_MODE);
		}
		/* Modem firmware updated, continue as normal */
	}


Full DFU API
************

A full firmware upgrade of the modem consists of the following steps:

1. Initialization
#. Programming the bootloader
#. Programming the modem firmware
#. Verification

Bootloader forms the first segment of the firmware package and it must be programmed initially.
If any failures happen, the sequence of steps must be restarted from the initialization phase.

Initialization
==============

To initialize the full firmware update process for the modem, call the following function:

.. code-block:: c

	int nrf_modem_full_dfu_init(struct nrf_modem_full_dfu_digest *digest_buffer);

Programming the bootloader
==========================

To program a bootloader, call the following function:

.. code-block:: c

	int nrf_modem_full_dfu_bl_write(uint32_t len, void *src)

The bootloader may be written in smaller chunks, which are internally appended together by the library.
When all pieces are written, call the following function:

.. code-block:: c

	int nrf_modem_full_dfu_apply(void)

After a successful call, the modem changes to the DFU mode.
At this stage, you may write firmware segments or issue any other DFU commands like ``verify``.

Programming the modem firmware
==============================

Firmware segments are written by using the following function call:

.. code-block:: c

	int nrf_modem_full_dfu_fw_write(uint32_t addr, uint32_t len, void *src)

The modem library buffers the data with the same destination address, until one of the following conditions occur:

* The buffered data reaches 8kb.
* The destination address changes.

At this point, the buffer is written to the flash.
When all the segments are written, you must call the following function:

.. code-block:: c

	int nrf_modem_full_dfu_apply(void)

Verification
============

To verify the content of the modem flash, use the following function:

.. code-block:: c

   nrf_modem_full_dfu_digest(uint32_t addr, uint32_t size, struct nrf_modem_full_dfu_digest *digest_buffer);

This function calculates SHA-256 hash over the given flash area.
Compare the hash to the precalculated value that comes with the modem firmware package, to ensure that the image is programmed successfully.
