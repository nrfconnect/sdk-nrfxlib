.. _limitations:

Limitations
###########

.. contents::
   :local:
   :depth: 2

The Modem DFU socket is not part of the generic sockets.
Therefore, operating on it using ``poll`` is not possible.

Modem firmware limitations
**************************

For a detailed description of limitations related to the modem firmware used, refer to the changelog of that specific version.
The changelog is distributed as part of the `nRF9160 modem firmware zip file`_.
