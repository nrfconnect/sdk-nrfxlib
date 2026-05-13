/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file sdc_hci_evt.h
 *
 * @defgroup sdc_hci_evt SoftDevice Controller HCI events
 * @ingroup sdc_hci
 *
 * @{
 */

#ifndef SDC_HCI_EVT_H__
#define SDC_HCI_EVT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <nrfx.h>

/**
 * @defgroup HCI_TYPES Types
 * @{
 */

/** @brief HCI Event Code values. */
enum sdc_hci_event
{
    /** @brief Disconnection Complete. */
    SDC_HCI_EVENT_DISCONN_COMPLETE = 0x5,
    /** @brief Encryption Change [v1]. */
    SDC_HCI_EVENT_ENCRYPTION_CHANGE = 0x8,
    /** @brief Read Remote Version Information Complete. */
    SDC_HCI_EVENT_READ_REMOTE_VERSION_INFORMATION_COMPLETE = 0xc,
    /** @brief Command Complete. */
    SDC_HCI_EVENT_COMMAND_COMPLETE = 0xe,
    /** @brief Command Status. */
    SDC_HCI_EVENT_COMMAND_STATUS = 0xf,
    /** @brief Number Of Completed Packets. */
    SDC_HCI_EVENT_NUMBER_OF_COMPLETED_PACKETS = 0x13,
    /** @brief Data Buffer Overflow. */
    SDC_HCI_EVENT_DATA_BUFFER_OVERFLOW = 0x1a,
    /** @brief Encryption Key Refresh Complete. */
    SDC_HCI_EVENT_ENCRYPTION_KEY_REFRESH_COMPLETE = 0x30,
    /** @brief LE Meta. */
    SDC_HCI_EVENT_LE_META = 0x3e,
    /** @brief Authenticated Payload Timeout Expired. */
    SDC_HCI_EVENT_AUTHENTICATED_PAYLOAD_TIMEOUT_EXPIRED = 0x57,
    /** @brief VS Meta. */
    SDC_HCI_EVENT_VS_META = 0xff,
};

/** @brief Number Of Completed Packets array parameters. */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint16_t num_completed_packets;
} sdc_hci_number_of_completed_packets_array_params_t;

/** @} end of HCI_TYPES */

/**
 * @defgroup HCI_EVENTS Events
 * @{
 */

/** @brief Disconnection Complete.
 *
 * The description below is extracted from Core_v6.3,
 * Vol 4, Part E, Section 7.7.5
 *
 * This event occurs when a connection is terminated. The status parameter indicates if
 * the disconnection was successful or not. The reason parameter indicates the reason
 * for the disconnection if the disconnection was successful. If the disconnection was not
 * successful, then the value of the reason parameter shall be ignored by the Host. For
 * example, this can be the case if the Host has issued the HCI_Disconnect command
 * and there was a parameter error, or the command was not presently allowed, or a
 * Connection_Handle that didn’t correspond to a connection was given.
 *
 * If Connection_Handle identifies a CIS on the Central, then the handle and the
 * associated data paths of that CIS shall remain valid (irrespective of whether the
 * disconnection was successful or not). If Connection_Handle identifies a CIS on the
 * Peripheral and Status is zero, then the handle and the associated data path of that
 * CIS shall be deleted. If Connection_Handle identifies any other kind of connection and
 * Status is zero, then the handle shall be deleted. If Connection_Handle identifies an LE
 * ACL connection and Status is zero, then any associated CS configurations shall also be
 * deleted.
 *
 * Note: When a physical link fails, one HCI_Disconnection_Complete event will be
 * returned for each logical channel on the physical link with the corresponding
 * Connection_Handle as a parameter.
 */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t status;
    uint16_t conn_handle;
    uint8_t reason;
} sdc_hci_event_disconn_complete_t;

/** @brief Encryption Change [v1].
 *
 * The description below is extracted from Core_v6.3,
 * Vol 4, Part E, Section 7.7.8
 *
 * This event is used to indicate that the change of the encryption mode has been
 * completed. The Connection_Handle parameter will be a Connection_Handle for an
 * ACL connection and is used to identify the remote device. The Encryption_Enabled
 * parameter specifies the new encryption state for the connection specified by
 * Connection_Handle. The Encryption_Key_Size parameter specifies the size, in octets,
 * of the key used to encrypt the link. This event will occur on both devices to notify the
 * Hosts when encryption has changed for all connections between the two devices. This
 * event shall not be generated if encryption is paused or resumed; during a role switch,
 * for example.
 */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t status;
    uint16_t conn_handle;
    uint8_t encryption_enabled;
} sdc_hci_event_encryption_change_t;

/** @brief Read Remote Version Information Complete.
 *
 * The description below is extracted from Core_v6.3,
 * Vol 4, Part E, Section 7.7.12
 *
 * This event is used to indicate the completion of the process obtaining the version
 * information of the remote Controller specified by the Connection_Handle parameter.
 * Connection_Handle shall be for an ACL connection.
 *
 * The Version parameter defines the specification version of the BR/EDR or LE Controller.
 * The Company_Identifier parameter indicates the manufacturer of the remote Controller.
 * The Subversion parameter is controlled by the manufacturer and is vendor-specific.
 * These parameters shall contain the same values as the CompanyID and SubVersion
 * parameters in [Vol 2] Part C, Section 4.3.3 and [Vol 6] Part B, Section 2.4.2.13.
 */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t status;
    uint16_t conn_handle;
    uint8_t version;
    uint16_t company_identifier;
    uint16_t subversion;
} sdc_hci_event_read_remote_version_information_complete_t;

/** @brief Command Complete.
 *
 * The description below is extracted from Core_v6.3,
 * Vol 4, Part E, Section 7.7.14
 *
 * This event is used by the Controller for most commands to transmit return status of
 * a command and the other event parameters that are specified for the issued HCI
 * command.
 *
 * The Num_HCI_Command_Packets parameter allows the Controller to indicate the
 * number of HCI command packets the Host can send to the Controller. If the Controller
 * requires the Host to stop sending commands, Num_HCI_Command_Packets will be
 * set to zero. To indicate to the Host that the Controller is ready to receive HCI
 * command packets, the Controller generates an HCI_Command_Complete event with
 * the Command_Opcode parameter set to 0x0000 and Num_HCI_Command_Packets
 * set to 1 or more. Command_Opcode 0x0000 is a special value indicating that this
 * event is not associated with a command sent by the Host. The Controller can send
 * an HCI_Command_Complete event with Command Opcode 0x0000 at any time to
 * change the number of outstanding HCI command packets that the Host can send before
 * waiting. See each command for the parameters that are returned by this event.
 */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t num_hci_command_packets;
    uint16_t command_opcode;
    uint8_t status;
} sdc_hci_event_command_complete_t;

/** @brief Command Status.
 *
 * The description below is extracted from Core_v6.3,
 * Vol 4, Part E, Section 7.7.15
 *
 * This event is used to indicate that the command described by the Command_Opcode
 * parameter has been received, and that the Controller is currently performing the task
 * for this command. This event is needed to provide mechanisms for asynchronous
 * operation, which avoids the need for the Host to wait for a command to finish. If
 * the command cannot begin to execute (a parameter error may have occurred, or
 * the command may currently not be allowed), the Status parameter will contain the
 * corresponding error code, and no complete event will follow since the command was
 * not started. The Num_HCI_Command_Packets parameter allows the Controller to
 * indicate the number of HCI command packets the Host can send to the Controller. If the
 * Controller requires the Host to stop sending commands, Num_HCI_Command_Packets
 * will be set to zero. To indicate to the Host that the Controller is ready to receive
 * HCI command packets, the Controller generates an HCI_Command_Status event with
 * Status 0x00 and Command_Opcode 0x0000 and Num_HCI_Command_Packets set
 * to 1 or more. Command_Opcode 0x0000 is a special value indicating that this event
 * is not associated with a command sent by the Host. The Controller can send an
 * HCI_Command_Status event with Command Opcode 0x0000 at any time to change the
 * number of outstanding HCI command packets that the Host can send before waiting.
 */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t status;
    uint8_t num_hci_command_packets;
    uint16_t command_opcode;
} sdc_hci_event_command_status_t;

/** @brief Number Of Completed Packets.
 *
 * The description below is extracted from Core_v6.3,
 * Vol 4, Part E, Section 7.7.19
 *
 * This event is used by the Controller to indicate to the Host how many HCI Data
 * packets or HCI ISO Data packets have been completed for each Handle since the
 * previous HCI_Number_Of_Completed_Packets event was sent to the Host. This means
 * that the corresponding buffer space has been freed in the Controller and is available
 * for new packets to be sent. Based on this information and the return parameters of
 * the HCI_Read_Buffer_Size and HCI_LE_Read_Buffer_Size commands, the Host can
 * determine for which Handles the following HCI packets should be sent to the Controller.
 * The HCI_Number_Of_Completed_Packets event shall not specify a given Handle
 * before the Controller has sent the event indicating that the corresponding connection
 * or BIG has been created or after it has sent the event indicating disconnection of
 * the corresponding connection or indicating that the BIG has been terminated. While
 * the Controller has HCI Data packets or HCI ISO Data packets in its buffer, it shall
 * keep sending the HCI_Number_Of_Completed_Packets event to the Host at least
 * periodically, until it finally reports that all the pending packets have been completed.
 * The rate with which this event is sent is manufacturer specific.
 *
 * Note: HCI_Number_Of_Completed_Packets events will not report on synchronous
 * Connection_Handles if synchronous Flow Control is disabled. (See Section 7.3.36 and
 * Section 7.3.37.)
 */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t num_handles;
    sdc_hci_number_of_completed_packets_array_params_t handles[];
} sdc_hci_event_number_of_completed_packets_t;

/** @brief Data Buffer Overflow.
 *
 * The description below is extracted from Core_v6.3,
 * Vol 4, Part E, Section 7.7.26
 *
 * This event is used to indicate that the Controller’s data buffers have been overflowed.
 * This can occur if the Host has sent more packets than allowed. The Link_Type
 * parameter is used to indicate the type of data whose buffers overflowed.
 */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t link_type;
} sdc_hci_event_data_buffer_overflow_t;

/** @brief Encryption Key Refresh Complete.
 *
 * The description below is extracted from Core_v6.3,
 * Vol 4, Part E, Section 7.7.39
 *
 * This event is used to indicate to the Host that the encryption key was refreshed on
 * the given Connection_Handle any time encryption is paused and then resumed. The
 * Controller shall send this event when the encryption key has been refreshed due to
 * encryption being started or resumed.
 *
 * If the HCI_Encryption_Key_Refresh_Complete event was generated due to an
 * encryption pause and resume operation embedded within a change connection link key
 * procedure, the HCI_Encryption_Key_Refresh_Complete event shall be sent prior to the
 * HCI_Change_Connection_Link_Key_Complete event.
 *
 * If the HCI_Encryption_Key_Refresh_Complete event was generated due to an
 * encryption pause and resume operation embedded within a role switch procedure,
 * the HCI_Encryption_Key_Refresh_Complete event shall be sent prior to the
 * HCI_Role_Change event.
 */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t status;
    uint16_t conn_handle;
} sdc_hci_event_encryption_key_refresh_complete_t;

/** @brief LE Meta.
 *
 * The description below is extracted from Core_v6.3,
 * Vol 4, Part E, Section 7.7.65
 *
 * The LE Meta event is used to encapsulate all LE Controller specific events. The Event
 * Code of all LE Meta events shall be 0x3E. The Subevent_Code is the first octet of the
 * event parameters. The Subevent_Code shall be set to one of the valid Subevent_Codes
 * from an LE specific event. All other parameters are defined in the LE Controller specific
 * events.
 */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t subevent_code;
    uint8_t params[];
} sdc_hci_event_le_meta_t;

/** @brief Authenticated Payload Timeout Expired.
 *
 * The description below is extracted from Core_v6.3,
 * Vol 4, Part E, Section 7.7.75
 *
 * This event is used to indicate that a packet containing a valid MIC on the
 * Connection_Handle was not received within the authenticatedPayloadTO (see [Vol
 * 2] Part B, Appendix B for the BR/EDR and [Vol 6] Part B, Section 5.4 for the LE
 * connection). The Host may disconnect the link when this occurs.
 */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_event_authenticated_payload_timeout_expired_t;

/** @brief VS Meta.
 *
 * The VS Meta event is used to encapsulate all vendor-specific events. The Event
 * Code of all VS Meta events shall be 0xFF. The Subevent_Code is the first octet of the
 * event parameters. The Subevent_Code shall be set to one of the valid Subevent_Codes
 * from a vendor-specific event. All other parameters are defined in the vendor-specific
 * events.
 */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t subevent_code;
    uint8_t params[];
} sdc_hci_event_vs_meta_t;

/** @} end of HCI_EVENTS */

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* SDC_HCI_EVT_H__ */
