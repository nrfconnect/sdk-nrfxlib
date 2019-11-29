/*
 * Copyright (c) 2019 - 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**
 * @file ble_controller_hci_vs.h
 *
 * @defgroup ble_controller_hci_vs BLE Controller Vendor Specific HCI Interface
 * @ingroup ble_controller
 *
 * The BLE Controller VS HCI APIs define additional functionality provided by
 * the BLE controller library.
 * All APIs in this header file are expected to be called from the
 * same execution priority as mpsl_low_priority_process.
 * Not doing so will lead to undefined behavior.
 * @{
 */

#ifndef BLE_CONTROLLER_HCI_VS_H__
#define BLE_CONTROLLER_HCI_VS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <cmsis_compiler.h>

/**
 * @defgroup HCI_VS_TYPES Types
 * @{
 */

/** @brief HCI OpCode Field values. */
enum HCI_VS_OPCODE
{
    /** @brief See @ref hci_vs_cmd_llpm_mode_set(). */
    HCI_VS_OPCODE_CMD_LLPM_MODE_SET = 0xfc01,
    /** @brief See @ref hci_vs_cmd_conn_update(). */
    HCI_VS_OPCODE_CMD_CONN_UPDATE = 0xfc02,
    /** @brief See @ref hci_vs_cmd_conn_event_extend(). */
    HCI_VS_OPCODE_CMD_CONN_EVENT_EXTEND = 0xfc03,
    /** @brief See @ref hci_vs_cmd_qos_conn_event_report_enable(). */
    HCI_VS_OPCODE_CMD_QOS_CONN_EVENT_REPORT_ENABLE = 0xfc04,
};

/** @brief Subevent Code values. */
enum HCI_VS_SUBEVENT_CODE
{
    /** @brief See @ref hci_vs_evt_qos_conn_event_report_t. */
    HCI_VS_SUBEVENT_CODE_QOS_CONN_EVENT_REPORT = 0x01,
};

/** @} end of HCI_VS_TYPES */

/**
 * @defgroup HCI_VS_EVENTS Events
 * @{
 */

/** @brief QoS Connection Event Report.
 *
 * A QoS Connection Event report gives information about the connection event.
 */
typedef __PACKED_STRUCT
{
    /** @brief Connnection handle corresponding to the connection event report. */
    uint16_t conn_handle;
    /** @brief Connection event counter corresponding to the connection event report. */
    uint16_t event_counter;
    /** @brief Data Channel Index used during the connection event (0-36). */
    uint8_t channel_index;
    /** @brief Number of packets received with good CRC during the connection event. */
    uint16_t crc_ok_count;
    /** @brief Number of packets received with bad CRC during the connection event. */
    uint8_t crc_error_count;
    /** @brief Indicates that the connection event was closed because a packet was not received. */
    uint8_t rx_timeout;
} hci_vs_evt_qos_conn_event_report_t;

/** @} end of HCI_VS_EVENTS */

/**
 * @defgroup HCI_VS_COMMANDS Commands
 * @{
 */

/** @brief Set Low Latency Packet Mode command parameter(s). */
typedef __PACKED_STRUCT
{
    /** @brief Set to 1 to enable LLPM. */
    uint8_t enable;
} hci_vs_cmd_llpm_mode_set_t;

/** @brief Connection Update command parameter(s). */
typedef __PACKED_STRUCT
{
    /** @brief Connection Handle. */
    uint16_t connection_handle;
    /** @brief Connection Interval in microseconds. Valid range is 7,500 us to 4,000,000 us in 1,250
     *         us steps.  If LLPM mode is enabled, parameters in the range 1,000 us to 7,000 us in
     *         1,000 us steps are also accepted.
     */
    uint32_t conn_interval_us;
    /** @brief Slave latency for the connection in number of connection events. */
    uint16_t conn_latency;
    /** @brief Supervision timeout for the LE Link in 10 ms units. Range 100 ms to 32 ms. */
    uint16_t supervision_timeout;
} hci_vs_cmd_conn_update_t;

/** @brief Enable or Disable Extended Connection Events command parameter(s). */
typedef __PACKED_STRUCT
{
    /** @brief Set to 0 for disabling, 1 for enabling, all other values are RFU. */
    uint8_t enable;
} hci_vs_cmd_conn_event_extend_t;

/** @brief QoS Connection Event Reports enable command parameter(s). */
typedef __PACKED_STRUCT
{
    /** @brief Set to 0 for disabling, 1 for enabling, all other values are RFU. */
    uint8_t enable;
} hci_vs_cmd_qos_conn_event_report_enable_t;

/** @} end of HCI_VS_COMMANDS */

/**
 * @defgroup HCI_VS_API API
 * @{
 */

/** @brief Set Low Latency Packet Mode.
 *
 * This command enables or disables Low Latency Packet Mode support.
 * When Low Latency Packet Mode is enabled, it is possible to switch to connection intervals in the
 * range 1-7 ms. Switch to short connection intervals by calling @ref hci_vs_cmd_conn_update().
 *
 * After HCI Reset, this feature is disabled.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t hci_vs_cmd_llpm_mode_set(const hci_vs_cmd_llpm_mode_set_t * p_params);

/** @brief Connection Update.
 *
 * This vendor specific command is used to change the Link Layer Connection parameters of a
 * connection. This command may be issued by the master only.
 *
 * The Supervision_Timeout in milliseconds shall be larger than (1 + Conn_Latency) *
 * Conn_Interval_Max * 2, where Conn_Interval_Max is given in milliseconds.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t hci_vs_cmd_conn_update(const hci_vs_cmd_conn_update_t * p_params);

/** @brief Enable or Disable Extended Connection Events.
 *
 * When Extended Connection Events are disabled, the maximum connection event length is set by @ref
 * ble_controller_cfg_event_length_t::event_length_us.
 * When Extended Connection Events are enabled, the controller will extend the connection event as
 * much as possible, if:
 * - Either of the peers has more data to send.
 *   See also: Core v5.1, Vol 6, Part B, Section 4.5.6
 * - There are no conflicts with other concurrent links.
 *
 * A connection event can not be extended beyond the connection interval.
 *
 * By default, that is after an HCI Reset, Extended Connection Events are enabled.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t hci_vs_cmd_conn_event_extend(const hci_vs_cmd_conn_event_extend_t * p_params);

/** @brief QoS Connection Event Reports enable.
 *
 * This vendor specific command is used to enable or disable generation of QoS Connection event
 * reports.
 * See @ref hci_vs_evt_qos_conn_event_report_t. When enabled, one report will be generated every
 * connection event.
 *
 * @note If the application does not pull a report in time, it will be overwritten.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t hci_vs_cmd_qos_conn_event_report_enable(const hci_vs_cmd_qos_conn_event_report_enable_t * p_params);

/** @} end of HCI_VS_API */


/** @} end of ble_controller_hci_vs */

#ifdef __cplusplus
}
#endif

#endif /* BLE_CONTROLLER_HCI_VS_H__ */

