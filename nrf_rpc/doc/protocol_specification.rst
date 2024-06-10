.. _nrf_rpc_protocol_specification:

nRF RPC protocol specification
##############################

.. contents::
   :local:
   :depth: 2

Two processors that communicate with each other using the remote procedure call (nRF RPC) library follow the nRF RPC protocol.
The nRF RPC protocol specifies the binary format and rules for constructing packets that are exchanged within an nRF RPC communication.

The nRF RPC packets that are constructed by the nRF RPC core library are relayed to the selected transport layer, where they can be additionally encoded to ensure a reliable delivery of the packet to the other processor using the selected medium.
The nRF RPC transport's specification is outside the scope of this document.

The types of communication, threading model and other general concepts of the nRF RPC are described in :ref:`nrf_rpc_architecture`.

nRF RPC packet format
*********************

Each nRF RPC packet consists of a 5-byte header and an optional, variable-length payload:

.. table::
   :align: center

   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
   |0                              |1                              |
   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
   |0  |1  |2  |3  |4  |5  |6  |7  |0  |1  |2  |3  |4  |5  |6  |7  |
   +===+===+===+===+===+===+===+===+===+===+===+===+===+===+===+===+
   | Type [\| Source Context ID]   | Command ID                    |
   +-------------------------------+-------------------------------+
   | Destination Context ID        | Source Group ID               |
   +-------------------------------+-------------------------------+
   | Destination Group ID          | [Payload...]                  |
   +-------------------------------+-------------------------------+
   |                             [...]                             |
   +---------------------------------------------------------------+

A description of the various fields and their meaning:

``Type [| Source Context ID]``: 8 bits
  The packet type determines the function of the packet and it can be one of the following values:

  * ``0x00`` - event
  * ``0x01`` - response
  * ``0x02`` - event acknowledgment
  * ``0x03`` - error report
  * ``0x04`` - initialization packet
  * ``0x80`` - command

  If the packet type is ``0x80`` (command), this field is additionally bitwise ORed with the source context ID.

  The source context ID is a numeric identifier of the conversation to which the packet is associated, chosen by the packet sender.

  The source context ID is a feature of the nRF RPC protocol that facilitates concurrent conversations.
  When two threads on the local processor want to start an nRF RPC conversation at the same time, they shall use distinct source context IDs when constructing a packet to the remote processor.
  The remote processor is then obliged to use the source context ID as the destination context ID in the response packet.
  This ensures that responses and any packets that follow within each conversation are correctly routed to the initiating thread.

  The exact source context ID allocation pattern is implementation-defined, meaning that when the packet sender initiates a new conversation or responds to the initiating packet, it is free to allocate any unused source context ID for the new conversation.

``Command ID``: 8 bits
  Identifies an individual command or event within an nRF RPC group.

  If the packet is a **response** or an **initialization packet**, this field has no meaning and shall be set to ``0xff``.

``Destination Context ID``: 8 bits
  A numeric identifier of the conversation to which the packet is associated, chosen by the packet receiver.

  In a packet that starts a new conversation, this field shall be assigned the value ``0xff`` (indicating it is unknown).
  In all subsequent packets within the conversation, the sender of the packet shall carry over the source context ID that was included in the last packet received from the peer.

``Source Group ID``: 8 bits
  A numeric identifier of the nRF RPC group associated with the packet, chosen by the packet sender.

  Each processor that uses the nRF RPC protocol chooses unique numeric identifiers for all nRF RPC groups that it supports.
  During the nRF RPC protocol initialization, it then communicates its own mapping of the pre-shared string group identifiers to these unique numeric identifiers.

``Destination Group ID``: 8 bits
  A numeric identifier of the nRF RPC group associated with the packet, chosen by the packet receiver.

  The sender learns this identifier by receiving an **initialization packet** from the peer during the nRF RPC protocol initialization.

``Payload``: variable length
  The payload format depends on the packet type:

  * **event acknowledgment** - the payload is empty.
  * **error report** - the payload is a 32-bit integer representing an error code, in little-endian byte order.
  * **initialization packet** - the payload has the following format:

    .. table::
       :align: center

       +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
       |0                              |1                              |
       +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
       |0  |1  |2  |3  |4  |5  |6  |7  |0  |1  |2  |3  |4  |5  |6  |7  |
       +===+===+===+===+===+===+===+===+===+===+===+===+===+===+===+===+
       | Max Version   | Min Version   | Group name....                |
       +---------------+---------------+-------------------------------+
       |                              ...                              |
       +---------------------------------------------------------------+

    The ``Min Version`` and ``Max Version`` fields indicate the minimum and maximum version of the nRF RPC protocol supported by the sender.
    The ``Group name`` field has a variable length and contains the string identifier of the nRF RPC group to which this packet is associated with, without the null terminator.

  * **event**, **response**, **command** - the payload contains remote procedure call arguments or return values, represented in an implementation-defined format.

    If the nRF RPC protocol is used together with the CBOR encoding, then the arguments and return values are represented as a sequence of CBOR data items, terminated by the null data item (``0xf6``).

    For example, if a packet is an nRF RPC command that represents the C function call ``foo(100, "bar")``, the packet might look as follows:

    .. code-block:: none

       80 01 ff 00 00 18 64 63 62 61 72 f6

       80: Command | Source Context ID (0)
       01: Command ID (1)
       ff: Destination Context ID (unknown)
       00: Source Group ID (0)
       00: Destination Group ID (0)
       18 64: CBOR unsigned int (100)
       63 62 61 72: CBOR text string ("bar")
       f6: CBOR null
