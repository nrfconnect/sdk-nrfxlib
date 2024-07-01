.. _nrf_wifi_driver_layers_overview:

Wi-Fi driver layers
###################

.. contents::
   :local:
   :depth: 2

The nRF70 Series Wi-Fi driver consists of the following key layers:

UMAC_IF
*******

**UMAC IF** (Upper Media Access Control Interface) layer is the public interface to the nRF70 Series Wi-Fi driver.
It abstracts the access to the **UMAC** (Upper Media Access Control) firmware in the nRF70 Series Wi-Fi chipset.

HAL
***

**HAL** (Hardware Abstraction Layer) layer abstracts the access to the nRF70 Series Wi-Fi chipset hardware.
It provides a set of functions that the **UMAC IF** layer uses to access the hardware.

BUS_IF
******

**Bus IF** (Bus Interface) layer abstracts the access to the bus level communication to nRF70 Series Wi-Fi chipset
It provides a set of functions that the **HAL** layer uses to access the bus, (Q)SPI and PCIe BUSes are supported.

Wi-Fi chipset layers
####################

The nRF70 Series Wi-Fi chipset consists of the following key layers:

Firmware layers
***************

Only the key firmware layers that are relevant for the nRF70 Series Wi-Fi driver are listed here.

- **UMAC** (Upper Media Access Control) firmware layer implements the IEEE 802.11 MAC layer.
  It is responsible for managing the Wi-Fi STA MLME.

- **LMAC** (Lower Media Access Control) firmware layer implements the IEEE 802.11 MAC layer.
  It is responsible for managing all real-time operations of the Wi-Fi MAC layer.
  The **BB** (Baseband) and **RF** (Radio Frequency) layers are part of the **LMAC** firmware.
  The nRF70 Series Wi-Fi driver does not interact with the **LMAC** firmware directly.

Hardware blocks
***************

Only the key hardware blocks that are relevant for the nRF70 Series Wi-Fi driver are listed here.

- **HPQM** (Host Processor Queue Manager) hardware block is responsible for managing the control flow by providing a set of queues with addresses of commands and event buffers.
  The **HPQM** block is used by the **UMAC** firmware to communicate with the host and vice versa.

  **HPQM** queues:
   - ``cmd_avl_queue``: This queue is used by the **UMAC** firmware to notify the host about command buffers that are available for the host to send commands to the firmware.
   - ``cmd_busy_queue``: This queue is used by the host to send commands to the **UMAC** firmware.
   - ``event_busy_queue``: This queue is used by the **UMAC** firmware to send events to the host.
   - ``event_avl_queue``: This queue is used by the host to notify the **UMAC** firmware about event buffers that are available for the UMAC firmware to send events to the host.

   The queue depth is controlled by the **UMAC** firmware and is not configurable by the host.
   Default queue depth is ``10`` for all queues.

Memory blocks
*************

Only the key memory blocks that are relevant for the nRF70 Series Wi-Fi driver are listed here.

- **Packet RAM** memory block is used to store the Transmit and Receive frames as well as the Command and Event buffers.
  The size of the Packet RAM is fixed (``~192KB``) and is shared between the **UMAC** firmware and the host.
  The division of the memory between the Transmit and Receive paths is configurable and is controlled by the nRF70 Series Wi-Fi driver.


Control flow
############

Command
*******

A typical control flow for a **Command** for the nRF70 Series Wi-Fi driver is as follows:

1. **UMAC IF** prepares a **UMAC** command and sends it to the **HAL** layer.
#. **HAL** polls the **HPQM** queue (in this case ``cmd_avl_queue``) for a command buffer address with a timeout, .
#. **HAL** dequeues the command buffer address from the **HPQM** ``cmd_avl_queue``.
#. **HAL** writes the contents of the command to the buffer address provided in **Step 3**.
#. **HAL** notifies the **UMAC** that a command is written by writing the command buffer address to the **HPQM** ``cmd_busy_queue``.
#. **HAL** notifies the **UMAC** firmware that the command buffer is ready for consumption by raising an interrupt to the **UMAC** processor.


Event
*****

A typical control flow for an **Event** for the nRF70 Series Wi-Fi driver is as follows:

1. **UMAC** firmware generates an interrupt to the host processor.
#. **HAL** reads the event buffer address from the **HPQM** ``event_busy_queue``.
#. **HAL** reads the event buffer from the address provided in **Step 2**.
#. **HAL** writes the event buffer address to the **HPQM** ``event_avl_queue``.
#. **HAL** passes the event buffer to the **UMAC IF** layer.
#. **UMAC IF** processes the event and depending on the event type, handles it accordingly.

.. note::

   The command and event flow is a simplified version of the actual flow.
   Fragmented commands and events are not covered in this flow.

Data flow
#########

Transmit
********

**UMAC IF** implements a basic token bucket algorithm to flowcontrol TX frames to the nRF70 Series Wi-Fi chipset.

Buckets
=======

Both per-AC buckets and global buckets are implemented as follows:

* **Per-AC buckets**: The nRF70 Series Wi-Fi chipset supports five Access Categories (ACs) (four as per IEEE 802.11e and one for management frames).
  The nRF70 Series Wi-Fi driver divides the tokens :kconfig:option:`CONFIG_NRF700X_MAX_TX_TOKENS` into equal parts for each AC.
* **Global bucket** (a.k.a **Spare bucket**): The nRF70 Series Wi-Fi driver implements a global bucket that is shared between all ACs.
  The remaining tokens after dividing the tokens for per-AC buckets are added to the global bucket.

For example, if :kconfig:option:`CONFIG_NRF700X_MAX_TX_TOKENS` is set to ``12``, the driver divides the tokens as follows:

* Two tokens for each AC
* Two tokens for the global bucket

Queues
=======

The nRF70 Series Wi-Fi driver implements the following queues:

* **TX queue** is used to store the frames that are ready to be transmitted.
  It maps to frames that can be transmitted in a single TXOP (Transmission Opportunity), for example a single A-MPDU.
  The depth of this queue is controlled by the :kconfig:option:`CONFIG_NRF700X_MAX_TX_AGGREGATION` Kconfig option and also limited by the maximum size of memory for a single token.

* **TX pending queue** is used to store the frames that are waiting for the tokens to be available in the buckets.
   The depth of this queue is controlled by the :kconfig:option:`CONFIG_NRF700X_MAX_TX_PENDING_QLEN` Kconfig option.


Flow
====

The nRF70 Series Wi-Fi driver implements the following flow for transmitting a frame:

1. **UMAC IF** receives a frame from the networking stack.
#. **UMAC IF** fetches the peer, TID and AC for the frame.
   Depending on these the transmit pending queue is selected and the frame is enqueued.
#. **UMAC IF** checks if the frame can be transmitted immediately, by checking
   * If the frame meets the aggregation criterion, which means the maximum aggregation count is reached or maximum aggregation size is reached or frame cannot be aggregated.
   * If the frame meets the token availability criterion, which means the frame can be transmitted based on the availability of the per-AC and global tokens.
#. If the frame can be transmitted, **UMAC IF** dequeues the frame from the transmit pending queue and enqueues it to the transmit queue.
#. **UMAC IF** processes the transmit queue:
   * It prepares the necessary information for **UMAC** firmware in the form of a ``struct nrf_wifi_tx_buff`` command.
   * It `copies` (this can be a memory copy or a DMA depending on underlying **BAL** implementation) each frame to the appropriate memory location in the Packet RAM
#. **UMAC IF** sends the ``struct nrf_wifi_tx_buff`` command to the **HAL** layer using the typical command flow, as described in the previous section.

Receive
*******

The key difference between the transmit and receive flow is that the RX packet buffers need to be pre-programmed to the nRF70 Series Wi-Fi chipset as packets can be received at any time.
Unlike the transmit flow, the receive flow does not have a token bucket mechanism to control the flow of packets.

Queues
======

The nRF70 Series Wi-Fi driver does not implement any queues for the receive path.
It is a circular buffer with a configurable size :kconfig:option:`CONFIG_NRF700X_RX_NUM_BUFS`.

Flow: initialization
====================

The nRF70 Series Wi-Fi driver implements the following flow during initialization:

1. **FMAC** allocates a set of buffers in the Packet RAM for the receive path.
    The number of buffers is controlled by the :kconfig:option:`CONFIG_NRF700X_RX_NUM_BUFS` Kconfig option.
#. **FMAC** prepares a ``struct nrf_wifi_rx_buff`` command and sends it to the **HAL** layer.
#. **HAL** sends the ``struct nrf_wifi_rx_buff`` command to the **UMAC** firmware using the typical command flow, as described in the previous section.

Flow: Receive
=============

The nRF70 Series Wi-Fi driver implements the following flow for receiving a frame:

1. **UMAC** firmware receives a frame from the air, processes it and stores it in the Packet RAM.
    Prepares ``struct nrf_wifi_rx_buff`` and generates an interrupt to the host processor.
#. **UMAC IF** receives the interrupt, processes ``struct nrf_wifi_rx_buff`` event, and copies the frame from the Packet RAM to the receive buffer.
#. **UMAC IF** performs 802.11 decapsulation and passes the frame to the networking stack.

Flow: Refill
============

The nRF70 Series Wi-Fi driver implements a refill mechanism to ensure that the Packet RAM is always filled with receive buffers.

1. **UMAC IF** allocates a new buffer for the received frame.
#. **UMAC IF** prepares a ``struct nrf_wifi_rx_buff`` command and sends it to the **HAL** layer.
#. **HAL** sends the ``struct nrf_wifi_rx_buff`` command to the **UMAC** firmware using the typical command flow, as described in the previous section.
