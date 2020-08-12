/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#ifndef BLE_CONTROLLER_HCI_VS_H__
#define BLE_CONTROLLER_HCI_VS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "sdc_hci_vs.h"

#define HCI_VS_OPCODE                                    sdc_hci_vs_opcode
#define HCI_VS_OPCODE_CMD_ZEPHYR_READ_VERSION_INFO       SDC_HCI_VS_OPCODE_CMD_ZEPHYR_READ_VERSION_INFO
#define HCI_VS_OPCODE_CMD_ZEPHYR_READ_SUPPORTED_COMMANDS SDC_HCI_VS_OPCODE_CMD_ZEPHYR_READ_SUPPORTED_COMMANDS
#define HCI_VS_OPCODE_CMD_ZEPHYR_WRITE_BD_ADDR           SDC_HCI_VS_OPCODE_CMD_ZEPHYR_WRITE_BD_ADDR
#define HCI_VS_OPCODE_CMD_ZEPHYR_READ_STATIC_ADDRESSES   SDC_HCI_VS_OPCODE_CMD_ZEPHYR_READ_STATIC_ADDRESSES
#define HCI_VS_OPCODE_CMD_ZEPHYR_WRITE_TX_POWER          SDC_HCI_VS_OPCODE_CMD_ZEPHYR_WRITE_TX_POWER
#define HCI_VS_OPCODE_CMD_LLPM_MODE_SET                  SDC_HCI_VS_OPCODE_CMD_LLPM_MODE_SET
#define HCI_VS_OPCODE_CMD_CONN_UPDATE                    SDC_HCI_VS_OPCODE_CMD_CONN_UPDATE
#define HCI_VS_OPCODE_CMD_CONN_EVENT_EXTEND              SDC_HCI_VS_OPCODE_CMD_CONN_EVENT_EXTEND
#define HCI_VS_OPCODE_CMD_QOS_CONN_EVENT_REPORT_ENABLE   SDC_HCI_VS_OPCODE_CMD_QOS_CONN_EVENT_REPORT_ENABLE
#define HCI_VS_OPCODE_CMD_EVENT_LENGTH_SET               SDC_HCI_VS_OPCODE_CMD_EVENT_LENGTH_SET

#define HCI_VS_SUBEVENT                       sdc_hci_vs_subevent
#define HCI_VS_SUBEVENT_QOS_CONN_EVENT_REPORT SDC_HCI_VS_SUBEVENT_QOS_CONN_EVENT_REPORT

#define HCI_VS_TX_POWER_HANDLE_TYPE           sdc_hci_vs_tx_power_handle_type
#define HCI_VS_TX_POWER_HANDLE_TYPE_ADV       SDC_HCI_VS_TX_POWER_HANDLE_TYPE_ADV
#define HCI_VS_TX_POWER_HANDLE_TYPE_SCAN_INIT SDC_HCI_VS_TX_POWER_HANDLE_TYPE_SCAN_INIT
#define HCI_VS_TX_POWER_HANDLE_TYPE_CONN      SDC_HCI_VS_TX_POWER_HANDLE_TYPE_CONN

#define hci_vs_zephyr_static_address_t                     sdc_hci_vs_zephyr_static_address_t
#define hci_vs_zephyr_supported_commands_t                 sdc_hci_vs_zephyr_supported_commands_t
#define hci_vs_subevent_qos_conn_event_report_t            sdc_hci_vs_subevent_qos_conn_event_report_t
#define hci_vs_cmd_zephyr_read_version_info_return_t       sdc_hci_vs_cmd_zephyr_read_version_info_return_t
#define hci_vs_cmd_zephyr_read_supported_commands_return_t sdc_hci_vs_cmd_zephyr_read_supported_commands_return_t
#define hci_vs_cmd_zephyr_write_bd_addr_t                  sdc_hci_vs_cmd_zephyr_write_bd_addr_t
#define hci_vs_cmd_zephyr_read_static_addresses_return_t   sdc_hci_vs_cmd_zephyr_read_static_addresses_return_t
#define hci_vs_cmd_zephyr_write_tx_power_t                 sdc_hci_vs_cmd_zephyr_write_tx_power_t
#define hci_vs_cmd_zephyr_write_tx_power_return_t          sdc_hci_vs_cmd_zephyr_write_tx_power_return_t
#define hci_vs_cmd_llpm_mode_set_t                         sdc_hci_vs_cmd_llpm_mode_set_t
#define hci_vs_cmd_conn_update_t                           sdc_hci_vs_cmd_conn_update_t
#define hci_vs_cmd_conn_event_extend_t                     sdc_hci_vs_cmd_conn_event_extend_t
#define hci_vs_cmd_qos_conn_event_report_enable_t          sdc_hci_vs_cmd_qos_conn_event_report_enable_t
#define hci_vs_cmd_event_length_set_t                      sdc_hci_vs_cmd_event_length_set_t

#define hci_vs_cmd_zephyr_read_version_info       sdc_hci_vs_cmd_zephyr_read_version_info
#define hci_vs_cmd_zephyr_read_supported_commands sdc_hci_vs_cmd_zephyr_read_supported_commands
#define hci_vs_cmd_zephyr_write_bd_addr           sdc_hci_vs_cmd_zephyr_write_bd_addr
#define hci_vs_cmd_zephyr_read_static_addresses   sdc_hci_vs_cmd_zephyr_read_static_addresses
#define hci_vs_cmd_zephyr_write_tx_power          sdc_hci_vs_cmd_zephyr_write_tx_power
#define hci_vs_cmd_llpm_mode_set                  sdc_hci_vs_cmd_llpm_mode_set
#define hci_vs_cmd_conn_update                    sdc_hci_vs_cmd_conn_update
#define hci_vs_cmd_conn_event_extend              sdc_hci_vs_cmd_conn_event_extend
#define hci_vs_cmd_qos_conn_event_report_enable   sdc_hci_vs_cmd_qos_conn_event_report_enable
#define hci_vs_cmd_event_length_set               sdc_hci_vs_cmd_event_length_set

#ifdef __cplusplus
}
#endif

#endif /* BLE_CONTROLLER_HCI_VS_H__ */
