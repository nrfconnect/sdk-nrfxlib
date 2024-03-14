/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */


/**
 * @file sdc_hci.h
 *
 * @defgroup sdc_hci SoftDevice Controller HCI Interface
 * @ingroup sdc
 *
 * The SoftDevice Controller HCI APIs are used to send HCI commands/data and receive
 * events/data to and from the SoftDevice Controller.
 * The HCI packet format is described in the Bluetooth Core Specification,
 * i.e. in Core v5. Vol 2, Part E.
 * All APIs in this header file are expected to be called from the
 * same execution priority as @ref mpsl_low_priority_process.
 * Not doing so will lead to undefined behavior.
 * @{
 */


#ifndef SDC_HCI_H__
#define SDC_HCI_H__


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

/** @brief The arrival margin in microseconds the controller needs to receive an ISO SDU before it can be sent on air.
 *
 * For more details, see the SoftDevice Controller documentation that describes how to provide data for isochronous channels.
 */
#define HCI_ISO_TX_SDU_ARRIVAL_MARGIN_US 1000

typedef enum
{
  SDC_HCI_MSG_TYPE_DATA = 0x02,
  SDC_HCI_MSG_TYPE_EVT  = 0x04,
  SDC_HCI_MSG_TYPE_ISO  = 0x08,
} sdc_hci_msg_type_t;

/** @brief Send an HCI data packet to the SoftDevice Controller.
 *
 * @param[in] p_data_in  HCI Data packet. The first byte in the buffer should correspond to
 *                       Handle, as specified by the Bluetooth Core Specification.
 *
 * @retval 0              Success
 * @retval -NRF_EINVAL    Invalid input
 */
int32_t sdc_hci_data_put(uint8_t const * p_data_in);

/** @brief Send an HCI ISO data packet to the SoftDevice Controller.
 *
 * If an error is returned, the HCI ISO data packet is dropped by the SoftDevice Controller and will not be
 * further processed.
 *
 * @param[in] p_data_in  HCI Data packet. The first byte in the buffer should correspond to
 *                       Handle, as specified by the Bluetooth Core Specification.
 *
 * @retval 0              Success
 * @retval -NRF_EINVAL    Invalid input
 * @retval -NRF_EPERM     There is no data-path for the selected Handle
 * @retval -NRF_EAGAIN    The data is provided too late
 * @retval -NRF_EFAULT    Failure during processing of the data
 */
int32_t sdc_hci_iso_data_put(uint8_t const * p_data_in);

/** @brief Retrieve an HCI packet from the SoftDevice Controller.
 *
 * This API is non-blocking.
 *
 * @note The application should ensure that the size of the provided buffer is at least
 *       @ref HCI_MSG_BUFFER_MAX_SIZE bytes.
 *
 * @param[in,out] p_packet_out Buffer where the HCI packet will be stored.
 *                             If an event is retrieved, the first byte corresponds to the Event Code.
 *                             If a data packet is retrieved, the first byte corresponds to the Handle.
 * @param[out] p_msg_type_out  Type of HCI packet produced by the controller.
 *                             See @ref sdc_hci_msg_type_t for possible values.
 *
 * @retval 0            Success
 * @retval -NRF_EAGAIN  No event available
 * @retval -NRF_EINVAL  Invalid input
 */
int32_t sdc_hci_get(uint8_t * p_packet_out, uint8_t * p_msg_type_out);

#ifdef __cplusplus
}
#endif

/** @} end of sdc_hci */

#endif /* SDC_HCI_H__ */
