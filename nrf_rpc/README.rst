.. _nrf_rpc:

Remote procedure call library (nRF RPC)
#######################################

nRF RPC is a remote procedure call library for enabling inter-processor communication on Nordic Semiconductor SoCs.
The library provides a framework for function call serialization.
It is RTOS-agnostic and designed to be used with an underlying transport layer, such as one based on `OpenAMP`_.

The nRF RPC library allows for calling a function on a remote processor from a local processor, in both synchronous and asynchronous way.
The library simplifies:

* The serialization of user APIs, such as those of a Bluetooth stack.
* The execution of functions implementing those APIs on a remote processor.

Depending on the transport layer, the local and remote processors are not confined to just one device.
The remote processor can be located on a separate device of any kind, such as a PC, or it can be another CPU core within the same device.

The library can be used with any operating system after porting the OS-dependent layers of the library.

The CBOR API layer on top of the core nRF RPC API uses the `zcbor`_ library for serializing the function arguments and return values.

.. toctree::
   :maxdepth: 2
   :caption: Subpages:

   doc/architecture
   doc/usage
   doc/protocol_specification
   CHANGELOG
   doc/api
