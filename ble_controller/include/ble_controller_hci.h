/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */


/**
 * @file ble_controller_hci.h
 *
 * @defgroup ble_controller_hci BLE Controller HCI Interface
 * @ingroup ble_controller
 *
 * The BLE Controller HCI APIs are used to send HCI commands/data and receive
 * events/data to and from the BLE Controller.
 * The HCI packet format is described in the Bluetooth Core Specification,
 * i.e. in Core v5. Vol 2, Part E.
 * All APIs in this header file are expected to be called from the
 * same execution priority as mpsl_low_priority_process.
 * Not doing so will lead to undefined behavior.
 * @{
 */


#ifndef BLE_CONTROLLER_HCI_H__
#define BLE_CONTROLLER_HCI_H__


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include "nrf_errno.h"

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


/** @brief Send an HCI command packet to the BLE Controller.
 *
 * @param[in] p_cmd_in  HCI Command packet. The first byte in the buffer should correspond to
 *                      OpCode, as specified by the Bluetooth Core Specification.
 *
 * @retval 0              Success
 * @retval -NRF_EINVAL    Invalid input
 */
int32_t hci_cmd_put(uint8_t const * p_cmd_in);


/** @brief Send an HCI data packet to the BLE Controller.
 *
 * @param[in] p_data_in  HCI Data packet. The first byte in the buffer should correspond to
 *                       Handle, as specified by the Bluetooth Core Specification.
 *
 * @retval 0              Success
 * @retval -NRF_EINVAL    Invalid input
 */
int32_t hci_data_put(uint8_t const * p_data_in);


/** @brief Retrieve an HCI event packet from the BLE Controller.
 *
 * This API is non-blocking.
 *
 * @note The application should ensure that the size of the provided buffer is at least
 *       @ref HCI_EVENT_PACKET_MAX_SIZE bytes.
 *
 * @param[in,out] p_evt_out Buffer where the HCI event will be stored.
 *                          If an event is retrieved, the first byte corresponds to Event Code,
 *                          as specified by the Bluetooth Core Specification.
 *
 * @retval 0              Success
 * @retval -NRF_EAGAIN  No event available
 * @retval -NRF_EINVAL  Invalid input
 */
int32_t hci_evt_get(uint8_t * p_evt_out);


/** @brief Retrieve an HCI data packet from the BLE Controller.
 *
 * This API is non-blocking.
 *
 * @note The application should ensure that the size of the provided buffer is at least
 *       @ref HCI_DATA_PACKET_MAX_SIZE bytes.
 *
 * @param[in,out] p_data_out Buffer where the HCI data packet will be stored.
 *                           If an data packet is retrieved, the first byte corresponds to Handle,
 *                           as specified by the Bluetooth Core Specification.
 *
 * @retval 0              Success
 * @retval -NRF_EAGAIN    No data available
 * @retval -NRF_EINVAL    Invalid input
 */
int32_t hci_data_get(uint8_t * p_data_out);


#ifdef __cplusplus
}
#endif

/** @} end of ble_controller_hci */

#endif /* BLE_CONTROLLER_HCI_H__ */
