.. _serialization:

Remote Procedure Serialization library
######################################

The Remote Procedure Serialization library is RTOS-agnostic serialization library for Nordic Semicoductor SoCs.

The Remote Procedure Serialization module provides an possibility to calling function
on the Remote processors and receiving an asynchronous events from it.

The remote processor could be a separate device of any type (including e.g. a PC), or another core on the same system.

The Remote Procedure Serialization libraries simplify the serialization of user API such as Bluetooth stack and executing it on a remote CPU.
The libraries are operating system independent so it can be used with default operating system or bare-metal application
after porting it.

The module uses `TinyCBOR <https://intel.github.io/tinycbor/current/>`_ library as serializator.

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   doc/architecture
   doc/transport
   doc/parallel_remote_procedure
   doc/porting

API documentation
=================

.. doxygengroup:: rp_ser
   :project: nrfxlib
   :members:
