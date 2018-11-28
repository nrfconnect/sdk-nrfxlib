/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#ifndef BLECTLR_HCI_H__
#define BLECTLR_HCI_H__

#include <stdint.h>
#include <stdbool.h>

/** @defgroup blectlr_hci
 * @{
 **/

/** @brief The size of a command packet header. */
#define HCI_CMD_HEADER_SIZE   (3)

/** @brief The size of a data packet header. */
#define HCI_DATA_HEADER_SIZE  (4)

/** @brief The size of an event packet header. */
#define HCI_EVENT_HEADER_SIZE (2)

/** @brief The maximum size of a command. */
#define HCI_CMD_MAX_SIZE    (255)

/** @brief The maximum size of data. */
#define HCI_DATA_MAX_SIZE   (251)

/** @brief The maximum size of an event. */
#define HCI_EVENT_MAX_SIZE  (255)

/** @brief The maximum size of an HCI command packet. */
#define HCI_CMD_PACKET_MAX_SIZE         (HCI_CMD_MAX_SIZE + HCI_CMD_HEADER_SIZE)

/** @brief The maximum size of an HCI data packet. */
#define HCI_DATA_PACKET_MAX_SIZE        (HCI_DATA_MAX_SIZE + HCI_DATA_HEADER_SIZE)

/** @brief The maximum size of an HCI event packet. */
#define HCI_EVENT_PACKET_MAX_SIZE       (HCI_EVENT_MAX_SIZE + HCI_EVENT_HEADER_SIZE)

/** @brief The maximum size of an HCI packet. */
#define HCI_MSG_BUFFER_MAX_SIZE HCI_CMD_PACKET_MAX_SIZE

/** @brief HCI message packet element definition. */
typedef uint8_t hci_element_t;

/**
 * @brief      Send an HCI command packet to the Controller.
 *
 * @param      buffer  HCI command packet in octet format according the HCI specification.
 *
 * @return     True if the HCI command is valid and was sent to the Controller
 */
bool hci_cmd_packet_put(hci_element_t const * buffer);

/**
 * @brief      Send an HCI data packet to the Controller.
 *
 * @param      buffer  HCI data packet in octet format according to the HCI specification.
 *
 * @return     True if the HCI data is valid and was sent to the Controller
 */
bool hci_data_packet_put(hci_element_t const * buffer);

/**
 * @brief      Receive an HCI data packet from the Controller.
 *
 * @param      buffer  HCI data packet in octet format according to the HCI specification.
 *
 * @return     True if a data packet was received from the Controller.
 */
bool hci_data_packet_get(hci_element_t * buffer);

/**
 * @brief      Receive an HCI event packet from the Controller.
 *
 * @param      buffer  HCI event packet in octet format according to the HCI specification.
 *
 * @return     True if an event packet was received from the Controller.
 */
bool hci_event_packet_get(hci_element_t * buffer);

#endif // BLECTLR_HCI_H__

/**
  @}
*/
