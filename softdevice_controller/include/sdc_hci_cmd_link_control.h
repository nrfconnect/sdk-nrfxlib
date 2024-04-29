/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file sdc_hci_cmd_link_control.h
 *
 * @defgroup sdc_hci_lc SoftDevice Controller HCI Interface for Link Control commands
 * @ingroup sdc_hci
 *
 * All APIs in this header file are expected to be called from the
 * same execution priority as mpsl_low_priority_process.
 * Not doing so will lead to undefined behavior.
 *
 * @{
 */

#ifndef SDC_HCI_CMD_LINK_CONTROL_H__
#define SDC_HCI_CMD_LINK_CONTROL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <mdk/compiler_abstraction.h>

/**
 * @defgroup HCI_TYPES Types
 * @{
 */

/** @brief HCI LC OpCode Field values. */
enum sdc_hci_opcode_lc
{
    /** @brief See @ref sdc_hci_cmd_lc_disconnect(). */
    SDC_HCI_OPCODE_CMD_LC_DISCONNECT = 0x406,
    /** @brief See @ref sdc_hci_cmd_lc_read_remote_version_information(). */
    SDC_HCI_OPCODE_CMD_LC_READ_REMOTE_VERSION_INFORMATION = 0x41d,
};

/** @} end of HCI_TYPES */

/**
 * @defgroup HCI_COMMAND_PARAMETERS Parameters
 * @{
 */

/** @brief Disconnect command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint8_t reason;
} sdc_hci_cmd_lc_disconnect_t;

/** @brief Read Remote Version Information command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_lc_read_remote_version_information_t;

/** @} end of HCI_COMMAND_PARAMETERS */

/**
 * @defgroup HCI_VS_API API
 * @{
 */
/** @brief Disconnect.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.1.6
 *
 * The HCI_Disconnect command is used to terminate an existing connection.
 * The Connection_Handle command parameter indicates which connection is to
 * be disconnected. The Reason command parameter indicates the reason for
 * ending the connection and is copied into the error code field of the
 * LMP_DETACH PDU on a BR/EDR connection or the error code field of the
 * LL_TERMINATE_IND or LL_CIS_TERMINATE_IND PDU on an LE
 * connection. All SCO, eSCO, and CIS connections on a physical link should be
 * disconnected before the ACL connection on the same physical connection is
 * disconnected. If it does not, they will be implicitly disconnected as part of the
 * ACL disconnection.
 *
 * If, on the Central, the Host issues this command before issuing the
 * HCI_LE_Create_CIS command for the same CIS (including if a previous CIS
 * with the same CIS_ID in the same CIG has been terminated or considered
 * lost), then the Controller shall return the error code Command Disallowed
 * (0x0C).
 *
 * If, on the Peripheral, the Host issues this command before the Controller has
 * generated the HCI_LE_CIS_Established event for that CIS, then the Controller
 * shall return the error code Command Disallowed (0x0C).
 *
 * Note: As specified in Section 7.7.5, on the Central, the handle for a CIS
 * remains valid even after disconnection and, therefore, the Host can recreate a
 * disconnected CIS at a later point in time using the same connection handle.
 *
 * Event(s) generated (unless masked away):
 * When the Controller receives the HCI_Disconnect command, it shall send the
 * HCI_Command_Status event to the Host. The HCI_Disconnection_Complete
 * event will occur at each Host when the termination of the connection has
 * completed, and on the local Host also indicates that this command has been
 * completed. The Reason event parameter in the event on the local Host shall be
 * set to the value Connection Terminated by Local Host (0x16), while that on the
 * remote Host shall be set to the value of the Reason command parameter.
 * However, if the termination procedure completes because a timer expires and,
 * therefore, the local Controller cannot determine whether or not the Reason
 * command parameter was received by the remote Controller, the Reason event
 * parameter on the local Host should instead be set to the value LMP Response
 * Timeout / LL Response Timeout (0x22).
 *
 * If this command is issued for a CIS on the Central and the CIS is successfully
 * terminated before being established, then an HCI_LE_CIS_Established event
 * shall also be sent for this CIS with the Status Operation Cancelled by Host
 * (0x44).
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_lc_disconnect(const sdc_hci_cmd_lc_disconnect_t * p_params);

/** @brief Read Remote Version Information.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.1.23
 *
 * This command will obtain the values for the version information for the remote
 * device identified by the Connection_Handle parameter. The
 * Connection_Handle shall be a Connection_Handle for an ACL-U or LE-U
 * logical link.
 *
 * Event(s) generated (unless masked away):
 * When the Controller receives the HCI_Read_Remote_Version_Information
 * command, the Controller shall send the HCI_Command_Status event to the
 * Host. When the Link Manager or Link Layer has completed the sequence to
 * determine the remote version information, the local Controller shall send an
 * HCI_Read_Remote_Version_Information_Complete event to the Host. The
 * HCI_Read_Remote_Version_Information_Complete event contains the status
 * of this command, and parameters describing the version and subversion of the
 * LMP or Link Layer used by the remote device.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_lc_read_remote_version_information(const sdc_hci_cmd_lc_read_remote_version_information_t * p_params);

/** @} end of HCI_VS_API */

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* SDC_HCI_CMD_LINK_CONTROL_H__ */
