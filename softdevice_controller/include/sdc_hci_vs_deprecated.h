/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**
 * @file sdc_hci_vs_deprecated.h
 *
 * This files contains defines mapping deprecated names to new names.
 *
 */

#ifndef SDC_HCI_VS_DEPRECATED_H__
#define SDC_HCI_VS_DEPRECATED_H__

#ifdef __cplusplus
extern "C" {
#endif

#define sdc_hci_vs_opcode sdc_hci_opcode_vs

#define SDC_HCI_VS_OPCODE_CMD_ZEPHYR_READ_VERSION_INFO       SDC_HCI_OPCODE_CMD_VS_ZEPHYR_READ_VERSION_INFO
#define SDC_HCI_VS_OPCODE_CMD_ZEPHYR_READ_SUPPORTED_COMMANDS SDC_HCI_OPCODE_CMD_VS_ZEPHYR_READ_SUPPORTED_COMMANDS
#define SDC_HCI_VS_OPCODE_CMD_ZEPHYR_WRITE_BD_ADDR           SDC_HCI_OPCODE_CMD_VS_ZEPHYR_WRITE_BD_ADDR
#define SDC_HCI_VS_OPCODE_CMD_ZEPHYR_READ_STATIC_ADDRESSES   SDC_HCI_OPCODE_CMD_VS_ZEPHYR_READ_STATIC_ADDRESSES
#define SDC_HCI_VS_OPCODE_CMD_ZEPHYR_WRITE_TX_POWER          SDC_HCI_OPCODE_CMD_VS_ZEPHYR_WRITE_TX_POWER
#define SDC_HCI_VS_OPCODE_CMD_LLPM_MODE_SET                  SDC_HCI_OPCODE_CMD_VS_LLPM_MODE_SET
#define SDC_HCI_VS_OPCODE_CMD_CONN_UPDATE                    SDC_HCI_OPCODE_CMD_VS_CONN_UPDATE
#define SDC_HCI_VS_OPCODE_CMD_CONN_EVENT_EXTEND              SDC_HCI_OPCODE_CMD_VS_CONN_EVENT_EXTEND
#define SDC_HCI_VS_OPCODE_CMD_QOS_CONN_EVENT_REPORT_ENABLE   SDC_HCI_OPCODE_CMD_VS_QOS_CONN_EVENT_REPORT_ENABLE
#define SDC_HCI_VS_OPCODE_CMD_EVENT_LENGTH_SET               SDC_HCI_OPCODE_CMD_VS_EVENT_LENGTH_SET

#define sdc_hci_vs_subevent sdc_hci_subevent_vs

#define SDC_HCI_VS_SUBEVENT_QOS_CONN_EVENT_REPORT SDC_HCI_SUBEVENT_VS_QOS_CONN_EVENT_REPORT

#define sdc_hci_vs_subevent_qos_conn_event_report_t            sdc_hci_subevent_vs_qos_conn_event_report_t
#define sdc_hci_vs_cmd_zephyr_read_version_info_return_t       sdc_hci_cmd_vs_zephyr_read_version_info_return_t
#define sdc_hci_vs_cmd_zephyr_read_supported_commands_return_t sdc_hci_cmd_vs_zephyr_read_supported_commands_return_t
#define sdc_hci_vs_cmd_zephyr_write_bd_addr_t                  sdc_hci_cmd_vs_zephyr_write_bd_addr_t
#define sdc_hci_vs_cmd_zephyr_read_static_addresses_return_t   sdc_hci_cmd_vs_zephyr_read_static_addresses_return_t
#define sdc_hci_vs_cmd_zephyr_write_tx_power_t                 sdc_hci_cmd_vs_zephyr_write_tx_power_t
#define sdc_hci_vs_cmd_zephyr_write_tx_power_return_t          sdc_hci_cmd_vs_zephyr_write_tx_power_return_t
#define sdc_hci_vs_cmd_llpm_mode_set_t                         sdc_hci_cmd_vs_llpm_mode_set_t
#define sdc_hci_vs_cmd_conn_update_t                           sdc_hci_cmd_vs_conn_update_t
#define sdc_hci_vs_cmd_conn_event_extend_t                     sdc_hci_cmd_vs_conn_event_extend_t
#define sdc_hci_vs_cmd_qos_conn_event_report_enable_t          sdc_hci_cmd_vs_qos_conn_event_report_enable_t
#define sdc_hci_vs_cmd_event_length_set_t                      sdc_hci_cmd_vs_event_length_set_t

#define sdc_hci_vs_cmd_zephyr_read_version_info       sdc_hci_cmd_vs_zephyr_read_version_info
#define sdc_hci_vs_cmd_zephyr_read_supported_commands sdc_hci_cmd_vs_zephyr_read_supported_commands
#define sdc_hci_vs_cmd_zephyr_write_bd_addr           sdc_hci_cmd_vs_zephyr_write_bd_addr
#define sdc_hci_vs_cmd_zephyr_read_static_addresses   sdc_hci_cmd_vs_zephyr_read_static_addresses
#define sdc_hci_vs_cmd_zephyr_write_tx_power          sdc_hci_cmd_vs_zephyr_write_tx_power
#define sdc_hci_vs_cmd_llpm_mode_set                  sdc_hci_cmd_vs_llpm_mode_set
#define sdc_hci_vs_cmd_conn_update                    sdc_hci_cmd_vs_conn_update
#define sdc_hci_vs_cmd_conn_event_extend              sdc_hci_cmd_vs_conn_event_extend
#define sdc_hci_vs_cmd_qos_conn_event_report_enable   sdc_hci_cmd_vs_qos_conn_event_report_enable
#define sdc_hci_vs_cmd_event_length_set               sdc_hci_cmd_vs_event_length_set

#ifdef __cplusplus
}
#endif

#endif /* SDC_HCI_VS_DEPRECATED_H__ */
