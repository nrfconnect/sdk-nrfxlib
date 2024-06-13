/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file sdc_hci_vs.h
 *
 * @defgroup sdc_hci_vs SoftDevice Controller Vendor Specific HCI Interface
 * @ingroup sdc
 *
 * The SoftDevice Controller VS HCI APIs define additional functionality provided by
 * the SoftDevice Controller library.
 * All APIs in this header file are expected to be called from the
 * same execution priority as mpsl_low_priority_process.
 * Not doing so will lead to undefined behavior.
 *
 * @{
 */

#ifndef SDC_HCI_VS_H__
#define SDC_HCI_VS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <mdk/compiler_abstraction.h>

/**
 * @defgroup HCI_TYPES Types
 * @{
 */

/** @brief HCI VS OpCode Field values. */
enum sdc_hci_opcode_vs
{
    /** @brief See @ref sdc_hci_cmd_vs_zephyr_read_version_info(). */
    SDC_HCI_OPCODE_CMD_VS_ZEPHYR_READ_VERSION_INFO = 0xfc01,
    /** @brief See @ref sdc_hci_cmd_vs_zephyr_read_supported_commands(). */
    SDC_HCI_OPCODE_CMD_VS_ZEPHYR_READ_SUPPORTED_COMMANDS = 0xfc02,
    /** @brief See @ref sdc_hci_cmd_vs_zephyr_write_bd_addr(). */
    SDC_HCI_OPCODE_CMD_VS_ZEPHYR_WRITE_BD_ADDR = 0xfc06,
    /** @brief See @ref sdc_hci_cmd_vs_zephyr_read_static_addresses(). */
    SDC_HCI_OPCODE_CMD_VS_ZEPHYR_READ_STATIC_ADDRESSES = 0xfc09,
    /** @brief See @ref sdc_hci_cmd_vs_zephyr_read_key_hierarchy_roots(). */
    SDC_HCI_OPCODE_CMD_VS_ZEPHYR_READ_KEY_HIERARCHY_ROOTS = 0xfc0a,
    /** @brief See @ref sdc_hci_cmd_vs_zephyr_read_chip_temp(). */
    SDC_HCI_OPCODE_CMD_VS_ZEPHYR_READ_CHIP_TEMP = 0xfc0b,
    /** @brief See @ref sdc_hci_cmd_vs_zephyr_write_tx_power(). */
    SDC_HCI_OPCODE_CMD_VS_ZEPHYR_WRITE_TX_POWER = 0xfc0e,
    /** @brief See @ref sdc_hci_cmd_vs_zephyr_read_tx_power(). */
    SDC_HCI_OPCODE_CMD_VS_ZEPHYR_READ_TX_POWER = 0xfc0f,
    /** @brief See @ref sdc_hci_cmd_vs_read_supported_vs_commands(). */
    SDC_HCI_OPCODE_CMD_VS_READ_SUPPORTED_VS_COMMANDS = 0xfd00,
    /** @brief See @ref sdc_hci_cmd_vs_llpm_mode_set(). */
    SDC_HCI_OPCODE_CMD_VS_LLPM_MODE_SET = 0xfd01,
    /** @brief See @ref sdc_hci_cmd_vs_conn_update(). */
    SDC_HCI_OPCODE_CMD_VS_CONN_UPDATE = 0xfd02,
    /** @brief See @ref sdc_hci_cmd_vs_conn_event_extend(). */
    SDC_HCI_OPCODE_CMD_VS_CONN_EVENT_EXTEND = 0xfd03,
    /** @brief See @ref sdc_hci_cmd_vs_qos_conn_event_report_enable(). */
    SDC_HCI_OPCODE_CMD_VS_QOS_CONN_EVENT_REPORT_ENABLE = 0xfd04,
    /** @brief See @ref sdc_hci_cmd_vs_event_length_set(). */
    SDC_HCI_OPCODE_CMD_VS_EVENT_LENGTH_SET = 0xfd05,
    /** @brief See @ref sdc_hci_cmd_vs_periodic_adv_event_length_set(). */
    SDC_HCI_OPCODE_CMD_VS_PERIODIC_ADV_EVENT_LENGTH_SET = 0xfd06,
    /** @brief See @ref sdc_hci_cmd_vs_coex_scan_mode_config(). */
    SDC_HCI_OPCODE_CMD_VS_COEX_SCAN_MODE_CONFIG = 0xfd07,
    /** @brief See @ref sdc_hci_cmd_vs_coex_priority_config(). */
    SDC_HCI_OPCODE_CMD_VS_COEX_PRIORITY_CONFIG = 0xfd08,
    /** @brief See @ref sdc_hci_cmd_vs_peripheral_latency_mode_set(). */
    SDC_HCI_OPCODE_CMD_VS_PERIPHERAL_LATENCY_MODE_SET = 0xfd09,
    /** @brief See @ref sdc_hci_cmd_vs_write_remote_tx_power(). */
    SDC_HCI_OPCODE_CMD_VS_WRITE_REMOTE_TX_POWER = 0xfd0a,
    /** @brief See @ref sdc_hci_cmd_vs_set_adv_randomness(). */
    SDC_HCI_OPCODE_CMD_VS_SET_ADV_RANDOMNESS = 0xfd0c,
    /** @brief See @ref sdc_hci_cmd_vs_compat_mode_window_offset_set(). */
    SDC_HCI_OPCODE_CMD_VS_COMPAT_MODE_WINDOW_OFFSET_SET = 0xfd0d,
    /** @brief See @ref sdc_hci_cmd_vs_qos_channel_survey_enable(). */
    SDC_HCI_OPCODE_CMD_VS_QOS_CHANNEL_SURVEY_ENABLE = 0xfd0e,
    /** @brief See @ref sdc_hci_cmd_vs_set_power_control_request_params(). */
    SDC_HCI_OPCODE_CMD_VS_SET_POWER_CONTROL_REQUEST_PARAMS = 0xfd10,
    /** @brief See @ref sdc_hci_cmd_vs_read_average_rssi(). */
    SDC_HCI_OPCODE_CMD_VS_READ_AVERAGE_RSSI = 0xfd11,
    /** @brief See @ref sdc_hci_cmd_vs_central_acl_event_spacing_set(). */
    SDC_HCI_OPCODE_CMD_VS_CENTRAL_ACL_EVENT_SPACING_SET = 0xfd12,
    /** @brief See @ref sdc_hci_cmd_vs_set_conn_event_trigger(). */
    SDC_HCI_OPCODE_CMD_VS_SET_CONN_EVENT_TRIGGER = 0xfd13,
    /** @brief See @ref sdc_hci_cmd_vs_get_next_conn_event_counter(). */
    SDC_HCI_OPCODE_CMD_VS_GET_NEXT_CONN_EVENT_COUNTER = 0xfd14,
    /** @brief See @ref sdc_hci_cmd_vs_allow_parallel_connection_establishments(). */
    SDC_HCI_OPCODE_CMD_VS_ALLOW_PARALLEL_CONNECTION_ESTABLISHMENTS = 0xfd15,
    /** @brief See @ref sdc_hci_cmd_vs_min_val_of_max_acl_tx_payload_set(). */
    SDC_HCI_OPCODE_CMD_VS_MIN_VAL_OF_MAX_ACL_TX_PAYLOAD_SET = 0xfd16,
    /** @brief See @ref sdc_hci_cmd_vs_iso_read_tx_timestamp(). */
    SDC_HCI_OPCODE_CMD_VS_ISO_READ_TX_TIMESTAMP = 0xfd17,
    /** @brief See @ref sdc_hci_cmd_vs_big_reserved_time_set(). */
    SDC_HCI_OPCODE_CMD_VS_BIG_RESERVED_TIME_SET = 0xfd18,
    /** @brief See @ref sdc_hci_cmd_vs_cig_reserved_time_set(). */
    SDC_HCI_OPCODE_CMD_VS_CIG_RESERVED_TIME_SET = 0xfd19,
    /** @brief See @ref sdc_hci_cmd_vs_cis_subevent_length_set(). */
    SDC_HCI_OPCODE_CMD_VS_CIS_SUBEVENT_LENGTH_SET = 0xfd1a,
    /** @brief See @ref sdc_hci_cmd_vs_scan_channel_map_set(). */
    SDC_HCI_OPCODE_CMD_VS_SCAN_CHANNEL_MAP_SET = 0xfd1b,
    /** @brief See @ref sdc_hci_cmd_vs_scan_accept_ext_adv_packets_set(). */
    SDC_HCI_OPCODE_CMD_VS_SCAN_ACCEPT_EXT_ADV_PACKETS_SET = 0xfd1c,
    /** @brief See @ref sdc_hci_cmd_vs_set_role_priority(). */
    SDC_HCI_OPCODE_CMD_VS_SET_ROLE_PRIORITY = 0xfd1d,
};

/** @brief VS subevent Code values. */
enum sdc_hci_subevent_vs
{
    /** @brief See @ref sdc_hci_subevent_vs_qos_conn_event_report_t. */
    SDC_HCI_SUBEVENT_VS_QOS_CONN_EVENT_REPORT = 0x80,
    /** @brief See @ref sdc_hci_subevent_vs_qos_channel_survey_report_t. */
    SDC_HCI_SUBEVENT_VS_QOS_CHANNEL_SURVEY_REPORT = 0x81,
};

/** @brief Bluetooth roles that are recognized by the coexistence interface. */
enum sdc_hci_vs_coex_bt_role
{
    SDC_HCI_VS_COEX_BT_ROLE_ADVERTISER = 0x00,
    SDC_HCI_VS_COEX_BT_ROLE_SCANNER = 0x01,
    SDC_HCI_VS_COEX_BT_ROLE_CENTRAL = 0x02,
    SDC_HCI_VS_COEX_BT_ROLE_PERIPHERAL = 0x03,
};

/** @brief Scanner coexistence session request modes. */
enum sdc_hci_vs_coex_scan_mode
{
    /** @brief Request after receiving a valid access address. */
    SDC_HCI_VS_COEX_SCAN_MODE_REQUEST_ON_AA = 0x00,
    /** @brief Request before transmitting. */
    SDC_HCI_VS_COEX_SCAN_MODE_REQUEST_ON_TX = 0x01,
};

/** @brief Connection Event Trigger Role Selection. */
enum sdc_hci_vs_conn_event_trigger_role
{
    /** @brief Unused. */
    SDC_HCI_VS_CONN_EVENT_TRIGGER_ROLE_UNUSED = 0x00,
    /** @brief Connection event trigger for the Scanner. */
    SDC_HCI_VS_CONN_EVENT_TRIGGER_ROLE_SCAN = 0x01,
    /** @brief Connection event trigger for the Initiator. */
    SDC_HCI_VS_CONN_EVENT_TRIGGER_ROLE_INIT = 0x02,
    /** @brief Connection event trigger for connections (Central or Peripheral). */
    SDC_HCI_VS_CONN_EVENT_TRIGGER_ROLE_CONN = 0x03,
    /** @brief Connection event trigger for the Advertiser. */
    SDC_HCI_VS_CONN_EVENT_TRIGGER_ROLE_ADV = 0x04,
};

/** @brief Peripheral latency disable/enable modes. */
enum sdc_hci_vs_peripheral_latency_mode
{
    /** @brief Peripheral latency enable. */
    SDC_HCI_VS_PERIPHERAL_LATENCY_MODE_ENABLE = 0x00,
    /** @brief Peripheral latency disable. */
    SDC_HCI_VS_PERIPHERAL_LATENCY_MODE_DISABLE = 0x01,
    /** @brief Peripheral latency wait for ack. */
    SDC_HCI_VS_PERIPHERAL_LATENCY_MODE_WAIT_FOR_ACK = 0x02,
};

/** @brief Handle type for priority update. */
enum sdc_hci_vs_set_role_priority_handle_type
{
    /** @brief Handle of type initiator. Only affects secondary channel priority. */
    SDC_HCI_VS_SET_ROLE_PRIORITY_HANDLE_TYPE_INITIATOR_SECONDARY_CHANNEL = 0x04,
};

/** @brief TX power handle type. */
enum sdc_hci_vs_tx_power_handle_type
{
    /** @brief Handle of type Advertiser. */
    SDC_HCI_VS_TX_POWER_HANDLE_TYPE_ADV = 0x00,
    /** @brief Handle of type Scanner or Initiator. */
    SDC_HCI_VS_TX_POWER_HANDLE_TYPE_SCAN_INIT = 0x01,
    /** @brief Handle of type Connection. */
    SDC_HCI_VS_TX_POWER_HANDLE_TYPE_CONN = 0x02,
    /** @brief Handle of type Periodic Sync. */
    SDC_HCI_VS_TX_POWER_HANDLE_TYPE_SYNC = 0x03,
};

/** @brief Supported Vendor Specific HCI Commands. */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t read_supported_vs_commands : 1;
    uint8_t llpm_mode_set : 1;
    uint8_t conn_update : 1;
    uint8_t conn_event_extend : 1;
    uint8_t qos_conn_event_report_enable : 1;
    uint8_t event_length_set : 1;
    uint8_t periodic_adv_event_length_set : 1;
    uint8_t coex_priority_config : 1;
    uint8_t coex_scan_mode_config : 1;
    uint8_t peripheral_latency_mode_set : 1;
    uint8_t write_remote_tx_power : 1;
    uint8_t set_adv_randomness : 1;
    uint8_t qos_channel_survey_enable : 1;
    uint8_t set_power_control_request_params : 1;
    uint8_t read_average_rssi : 1;
    uint8_t central_acl_event_spacing_set : 1;
    uint8_t set_conn_event_trigger : 1;
    uint8_t get_next_conn_event_counter : 1;
    uint8_t allow_parallel_connection_establishments : 1;
    uint8_t min_val_of_max_acl_tx_payload_set : 1;
    uint8_t iso_read_tx_timestamp : 1;
    uint8_t big_reserved_time_set : 1;
    uint8_t cig_reserved_time_set : 1;
    uint8_t cis_subevent_length_set : 1;
    uint8_t scan_channel_map_set : 1;
    uint8_t scan_accept_ext_adv_packets_set : 1;
    uint8_t set_role_priority : 1;
} sdc_hci_vs_supported_vs_commands_t;

/** @brief Zephyr Static Address type. */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Static device address. */
    uint8_t address[6];
    /** @brief Identity root key (IR) for static device address. All zero parameter value indicates
     *         missing identity root key.
     */
    uint8_t identity_root[16];
} sdc_hci_vs_zephyr_static_address_t;

/** @brief Zephyr supported commands.
 *
 * If the field is set to 1, it indicates that the underlying command and
 * feature is supported by the controller.
 */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Read Version Information. */
    uint8_t read_version_info : 1;
    /** @brief Read Supported Commands. */
    uint8_t read_supported_commands : 1;
    /** @brief Read Supported Features. */
    uint8_t read_supported_features : 1;
    /** @brief Set Event Mask. */
    uint8_t set_event_mask : 1;
    /** @brief Reset. */
    uint8_t reset : 1;
    /** @brief Write BD_ADDR. */
    uint8_t write_bd_addr : 1;
    /** @brief Set Trace Enable. */
    uint8_t set_trace_enable : 1;
    /** @brief Read Build Information. */
    uint8_t read_build_info : 1;
    /** @brief Read Static Addresses. */
    uint8_t read_static_addresses : 1;
    /** @brief Read Key Hierarchy Roots. */
    uint8_t read_key_hierarchy_roots : 1;
    /** @brief Read Chip Temperature. */
    uint8_t read_chip_temperature : 1;
    /** @brief Read Host Stack Commands. */
    uint8_t read_host_stack_commands : 1;
    /** @brief Set Scan Request Reports. */
    uint8_t set_scan_request_reports : 1;
    /** @brief Write Tx Power Level (per Role/Connection). */
    uint8_t write_tx_power_level : 1;
    /** @brief Read Tx Power Level (per Role/Connection). */
    uint8_t read_tx_power_level : 1;
} sdc_hci_vs_zephyr_supported_commands_t;

/** @} end of HCI_TYPES */

/**
 * @defgroup HCI_EVENTS Events
 * @{
 */

/** @brief QoS Connection Event Report.
 *
 * A QoS Connection Event report gives information about the connection event.
 */
typedef struct __PACKED __ALIGN(1)
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
    uint16_t crc_error_count;
    /** @brief Number of received Negative Acknowledges from the peer during the connection event.
     */
    uint16_t nak_count;
    /** @brief Indicates that the connection event was closed because a packet was not received. */
    uint8_t rx_timeout : 1;
} sdc_hci_subevent_vs_qos_conn_event_report_t;

/** @brief QoS Channel Survey report event.
 *
 * QoS Channel Survey report event
 */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief The measured energy on the Bluetooth Low Energy channels, in dBm, indexed by Channel
     *         Index. If no measurement is available for the given channel, channel_energy is set to
     *         127.
     */
    int8_t channel_energy[40];
} sdc_hci_subevent_vs_qos_channel_survey_report_t;

/** @} end of HCI_EVENTS */

/**
 * @defgroup HCI_COMMAND_PARAMETERS Parameters
 * @{
 */

/** @brief Zephyr Read Version Information return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Assigned hardware manufacturer. Always 0x0002 indicating Nordic Semiconductor. */
    uint16_t hw_platform;
    /** @brief Assigned platform specific value. Set to 0x2 for nRF52, 0x3 for nRF53, 0x4 for
     *         nRF54H, 0x5 for nRF54L.
     */
    uint16_t hw_variant;
    /** @brief Firmware Variant. 0 indicates a standard Bluetooth controller. */
    uint8_t fw_variant;
    /** @brief Firmware Version. */
    uint8_t fw_version;
    /** @brief Firmware Revision. */
    uint16_t fw_revision;
    /** @brief Firware build revision. */
    uint32_t fw_build;
} sdc_hci_cmd_vs_zephyr_read_version_info_return_t;

/** @brief Zephyr Read Supported Commands return parameter(s). */
typedef union __PACKED __ALIGN(1)
{
    /** @brief Bit mask for each vendor command. If a bit is 1, the Controller supports the
     *         corresponding command and the features required for the command, unsupported or
     *         undefined commands shall be set to 0.
     */
    sdc_hci_vs_zephyr_supported_commands_t params;
    uint8_t raw[64];
} sdc_hci_cmd_vs_zephyr_read_supported_commands_return_t;

/** @brief Zephyr Write BD ADDR command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief BD_ADDR of the Device. */
    uint8_t bd_addr[6];
} sdc_hci_cmd_vs_zephyr_write_bd_addr_t;

/** @brief Zephyr Read Static Addresses return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Number of static device addresses. */
    uint8_t num_addresses;
    /** @brief Zephyr Static Addresses. The number of addresses is specified in num_addresses. */
    sdc_hci_vs_zephyr_static_address_t addresses[];
} sdc_hci_cmd_vs_zephyr_read_static_addresses_return_t;

/** @brief Zephyr Read KEY Hierarchy Roots return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Zephyr Identity Root Key. */
    uint8_t ir[16];
    /** @brief Zephyr Encryption Root Key. */
    uint8_t er[16];
} sdc_hci_cmd_vs_zephyr_read_key_hierarchy_roots_return_t;

/** @brief Zephyr Read Chip Temperature return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief The measured temperature in degrees Celsius. */
    int8_t temp;
} sdc_hci_cmd_vs_zephyr_read_chip_temp_return_t;

/** @brief Zephyr Write Tx Power Level (per Role/Connection) command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Handle type. See @ref sdc_hci_vs_tx_power_handle_type. */
    uint8_t handle_type;
    /** @brief Handle of the selected handle_type that identifies the instance to set the power of.
     *         In case of Extended Advertising, the handle specifies the advertising set. In case of
     *         a connection, it specifies a Connection Handle. Otherwise this parameter is ignored.
     */
    uint16_t handle;
    /** @brief The desired Tx_Power_Level at the antenna in dBm in signed 1 octet integer format. If
     *         set to 127, this indicates that the controller shall revert to using its default
     *         setting for Tx power. If the selected power level is not supported, an error is
     *         returned.
     */
    int8_t tx_power_level;
} sdc_hci_cmd_vs_zephyr_write_tx_power_t;

/** @brief Zephyr Write Tx Power Level (per Role/Connection) return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Handle type. See @ref sdc_hci_vs_tx_power_handle_type. */
    uint8_t handle_type;
    /** @brief See @ref sdc_hci_cmd_vs_zephyr_write_tx_power_t. */
    uint16_t handle;
    /** @brief The selected Tx Power in dBm. */
    int8_t selected_tx_power;
} sdc_hci_cmd_vs_zephyr_write_tx_power_return_t;

/** @brief Zephyr Read Tx Power Level (per Role/Connection) Command command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Handle type. See @ref sdc_hci_vs_tx_power_handle_type. */
    uint8_t handle_type;
    /** @brief Handle of the selected handle_type that identifies the instance to set the power of.
     *         In case of Extended Advertising, the handle specifies the advertising set. In case of
     *         a connection, it specifies a Connection Handle. Otherwise this parameter is ignored.
     */
    uint16_t handle;
} sdc_hci_cmd_vs_zephyr_read_tx_power_t;

/** @brief Zephyr Read Tx Power Level (per Role/Connection) Command return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Handle type. See @ref sdc_hci_vs_tx_power_handle_type. */
    uint8_t handle_type;
    /** @brief See @ref sdc_hci_cmd_vs_zephyr_read_tx_power_t. */
    uint16_t handle;
    /** @brief The selected Tx Power in dBm. */
    int8_t selected_tx_power;
} sdc_hci_cmd_vs_zephyr_read_tx_power_return_t;

/** @brief Read Supported Vendor Specific Commands return parameter(s). */
typedef union __PACKED __ALIGN(1)
{
    sdc_hci_vs_supported_vs_commands_t params;
    uint8_t raw[64];
} sdc_hci_cmd_vs_read_supported_vs_commands_return_t;

/** @brief Set Low Latency Packet Mode command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Set to 1 to enable LLPM. */
    uint8_t enable;
} sdc_hci_cmd_vs_llpm_mode_set_t;

/** @brief Connection Update command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Connection Handle. */
    uint16_t conn_handle;
    /** @brief Connection Interval in microseconds. Valid range is 7,500 us to 4,000,000 us in 1,250
     *         us steps. If LLPM mode is enabled, parameters in the range 1,000 us to 7,000 us in
     *         1,000 us steps are also accepted.
     */
    uint32_t conn_interval_us;
    /** @brief Slave latency for the connection in number of connection events. */
    uint16_t conn_latency;
    /** @brief Supervision timeout for the LE Link in 10 ms units. Range 100 ms to 32 s. */
    uint16_t supervision_timeout;
} sdc_hci_cmd_vs_conn_update_t;

/** @brief Enable or Disable Extended Connection Events command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Set to 0 for disabling, 1 for enabling, all other values are RFU. */
    uint8_t enable;
} sdc_hci_cmd_vs_conn_event_extend_t;

/** @brief QoS Connection Event Reports enable command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Set to 0 for disabling, 1 for enabling, all other values are RFU. */
    uint8_t enable;
} sdc_hci_cmd_vs_qos_conn_event_report_enable_t;

/** @brief Set event length for ACL connections command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Allocated event length in microseconds. */
    uint32_t event_length_us;
} sdc_hci_cmd_vs_event_length_set_t;

/** @brief Set event length for periodic advertisers command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Allocated periodic advertising event length in microseconds. */
    uint32_t event_length_us;
} sdc_hci_cmd_vs_periodic_adv_event_length_set_t;

/** @brief Configure Coexistence Scan Request Mode command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Scanner request mode. See @ref sdc_hci_vs_coex_scan_mode. */
    uint8_t mode;
} sdc_hci_cmd_vs_coex_scan_mode_config_t;

/** @brief Configure Coexistence Per-Role Priority command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Current Bluetooth device role, see @ref sdc_hci_vs_coex_bt_role. */
    uint8_t role;
    /** @brief @ref MPSL_COEX_PRIORITY_HIGH or @ref MPSL_COEX_PRIORITY_LOW priority. */
    uint8_t priority;
    /** @brief When the number of denied requests to the PTA controller is larger than the
     *         escalation threshold, the priority will be escalated. The threshold is handled only
     *         if priority is @ref MPSL_COEX_PRIORITY_LOW. The priority will be reset to @ref
     *         MPSL_COEX_PRIORITY_LOW after the end of the current radio activity. This means after
     *         the end of the advertising event, scan window, or connection event. If an escalation
     *         is not required then use @ref MPSL_COEX_ESCALATION_THRESHOLD_OFF.
     */
    uint8_t escalation_threshold;
} sdc_hci_cmd_vs_coex_priority_config_t;

/** @brief Set peripheral latency mode command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Connection handle. */
    uint16_t conn_handle;
    /** @brief Peripheral latency mode. See @ref sdc_hci_vs_peripheral_latency_mode. */
    uint8_t mode;
} sdc_hci_cmd_vs_peripheral_latency_mode_set_t;

/** @brief Write remote transmit power level command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    /** @brief PHY bit number i.e. [1M, 2M, s8, s2] == [1, 2, 3, 4]. */
    uint8_t phy;
    /** @brief Requested adjustment (in dBm) for the remote to apply to its transmit power. The
     *         value can be 0 to utilize the response of the peer to update the information on the
     *         transmit power setting of the remote. Note that this is only a request to the peer,
     *         which is in control of how, if at all, to apply changes to its transmit power.
     */
    int8_t delta;
} sdc_hci_cmd_vs_write_remote_tx_power_t;

/** @brief Set advertising randomness command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Advertising Handle or 0xFF to set the behavior for the very first advertising event.
     */
    uint8_t adv_handle;
    /** @brief Maximum random delay in microseconds, 0 to disable randomness. */
    uint16_t rand_us;
} sdc_hci_cmd_vs_set_adv_randomness_t;

/** @brief Set Compatibility mode for window offset command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Set to 1 to enable this compatibility mode. */
    uint8_t enable;
} sdc_hci_cmd_vs_compat_mode_window_offset_set_t;

/** @brief Enable the Quality of Service (QoS) channel survey module. command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Set to 0 to disable, 1 to enable, all other values are RFU. */
    uint8_t enable;
    /** @brief Requested average interval for the measurements and reports. Valid range is from 7500
     *         to 4000000. If set to 0, the measurements will be scheduled at every available
     *         opportunity.
     */
    uint32_t interval_us;
} sdc_hci_cmd_vs_qos_channel_survey_enable_t;

/** @brief Set LE Power Control Request procedure parameters command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Enable or Disable controller initiated autonomous LE Power Control Request procedure.
     *         Disabled by default.
     */
    uint8_t auto_enable;
    /** @brief Enable or Disable received APR handling in controller during LE Power Control Request
     *         procedure. Disabled by default.
     */
    uint8_t apr_enable;
    /** @brief The valid range is [0, 4095]. Default value is 2048. */
    uint16_t beta;
    /** @brief The lower limit of the RSSI golden range. The RSSI golden range is explained in
     *         Core_v5.4, Vol 6, Part B, Section 5.1.17.1. Default value is -70 dBm.
     */
    int8_t lower_limit;
    /** @brief The upper limit of the RSSI golden range. Default value is -30 dBm. */
    int8_t upper_limit;
    /** @brief Target RSSI level in dBm units when the average RSSI level is less than the lower
     *         limit of RSSI Golden range. Default value is -65 dBm.
     */
    int8_t lower_target_rssi;
    /** @brief Target RSSI level in dBm units when the average RSSI level is greater than the upper
     *         limit of RSSI Golden range. Default value is -35 dBm.
     */
    int8_t upper_target_rssi;
    /** @brief Duration in milliseconds to wait before initiating a new LE Power Control Request
     *         procedure by the controller. Default value is 5000 milliseconds. 0 milliseconds value
     *         is an invalid value.
     */
    uint16_t wait_period_ms;
    /** @brief Margin between APR value received from peer in LL_POWER_CONTROL_RSP PDU and actual
     *         reduction in Transmit power that is applied locally. The applied decrease in local
     *         Transmit power will be (received_apr - apr_margin) if received_apr > apr_margin,
     *         otherwise no change. Default value is 5 dB.
     */
    uint8_t apr_margin;
} sdc_hci_cmd_vs_set_power_control_request_params_t;

/** @brief Read average RSSI command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Connection Handle to read the average RSSI for. */
    uint16_t conn_handle;
} sdc_hci_cmd_vs_read_average_rssi_t;

/** @brief Read average RSSI return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    /** @brief Average RSSI in dBm. */
    int8_t avg_rssi;
} sdc_hci_cmd_vs_read_average_rssi_return_t;

/** @brief Set Central ACL event spacing command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Central ACL event spacing in microseconds. */
    uint32_t central_acl_event_spacing_us;
} sdc_hci_cmd_vs_central_acl_event_spacing_set_t;

/** @brief Set Connection Event Trigger command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Connection handle to set up trigger for. In case @ref
     *         sdc_hci_vs_conn_event_trigger_role specifies the Scanner or Initiator, this parameter
     *         is ignored.
     */
    uint16_t conn_handle;
    /** @brief Selected role to set triggers for. See @ref sdc_hci_vs_conn_event_trigger_role. */
    uint8_t role;
    /** @brief (D)PPI channel to use. This channel should be regarded as reserved until the
     *         connection event (D)PPI task triggering is stopped.
     */
    uint8_t ppi_ch_id;
    /** @brief Task Endpoint to trigger. If this is 0, then the connection event triggering feature
     *         is disabled.
     */
    uint32_t task_endpoint;
    /** @brief Connection event counter when the task end point triggering should start. */
    uint16_t conn_evt_counter_start;
    /** @brief The period in events between triggering of the task end point. */
    uint16_t period_in_events;
} sdc_hci_cmd_vs_set_conn_event_trigger_t;

/** @brief Get Next Connection Event Counter command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Connection handle to get the connection event counter value for. */
    uint16_t conn_handle;
} sdc_hci_cmd_vs_get_next_conn_event_counter_t;

/** @brief Get Next Connection Event Counter return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint16_t next_conn_event_counter;
} sdc_hci_cmd_vs_get_next_conn_event_counter_return_t;

/** @brief Allow Parallel Connection Establishment command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t enable;
} sdc_hci_cmd_vs_allow_parallel_connection_establishments_t;

/** @brief Set the minimum value that will be used as maximum Tx octets for ACL connections command
 *         parameter(s).
 */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Minimum value of maximum ACL TX payload. */
    uint8_t min_val_of_max_acl_tx_payload;
} sdc_hci_cmd_vs_min_val_of_max_acl_tx_payload_set_t;

/** @brief Iso Read Tx Timestamp command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_vs_iso_read_tx_timestamp_t;

/** @brief Iso Read Tx Timestamp return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint16_t packet_sequence_number;
    /** @brief Synchronization reference of the sent SDU. */
    uint32_t tx_time_stamp;
} sdc_hci_cmd_vs_iso_read_tx_timestamp_return_t;

/** @brief Set the default BIG reserved time command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint32_t reserved_time_us;
} sdc_hci_cmd_vs_big_reserved_time_set_t;

/** @brief Set the default CIG reserved time command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint32_t reserved_time_us;
} sdc_hci_cmd_vs_cig_reserved_time_set_t;

/** @brief Set the CIS subevent length in microseconds command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief The requested CIS subevent length in microseconds. */
    uint32_t cis_subevent_length_us;
} sdc_hci_cmd_vs_cis_subevent_length_set_t;

/** @brief Set the channel map for scanning and initiating. command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief This parameter contains 40 1-bit fields. The nth such field (in the range 0 to 39)
     *         contains the value for the Link Layer channel index n. Set the bit to 1 to enable the
     *         channel.  Only channel index 37, 38 and 39 are supported. The remaining bits shall be
     *         set to 1.
     */
    uint8_t channel_map[5];
} sdc_hci_cmd_vs_scan_channel_map_set_t;

/** @brief Scan accept extended advertising packets set command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Set to 1 to accept or 0 to ignore extended advertising packets. */
    uint8_t accept_ext_adv_packets;
} sdc_hci_cmd_vs_scan_accept_ext_adv_packets_set_t;

/** @brief Set priority of a BT role command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief Handle type for which to change the priority. See @ref
     *         sdc_hci_vs_set_role_priority_handle_type for valid values.
     */
    uint8_t handle_type;
    /** @brief Handle of the selected handle_type that identifies the instance to set the priority
     *         of. This parameter is ignored for scanner and initiator roles.
     */
    uint16_t handle;
    /** @brief The new priority for the role. Values from 1 to 5 set the new priority for the role,
     *         see https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrfxlib/softdevice_co
     *         ntroller/doc/scheduling.html A value of 0xFF resets the priority to the default
     *         selected by the controller.
     */
    uint8_t priority;
} sdc_hci_cmd_vs_set_role_priority_t;

/** @} end of HCI_COMMAND_PARAMETERS */

/**
 * @defgroup HCI_VS_API API
 * @{
 */
/** @brief Zephyr Read Version Information.
 *
 * Reads the values for the vendor version information for the local Controller.
 *
 * The Hardware_Platform information defines the hardware manufacturer
 * information. The Hardware_Variant is manufacturer specific and defines the
 * hardware platform from that manufacturer.
 *
 * The Firmware_Variant defines the type of firmware. It is possible to provide
 * HCI firmware with limited functionality for example for bootloader operation.
 * The Firmware_Version and Firmware_Revision define version information of the
 * Firmware_Variant that is currently active. The Firmware_Build defines an
 * additional counter for incremental builds.
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_zephyr_read_version_info(sdc_hci_cmd_vs_zephyr_read_version_info_return_t * p_return);

/** @brief Zephyr Read Supported Commands.
 *
 * This command reads the list of vendor commands supported for the local Controller.
 *
 * This command shall return the Supported_Commands configuration parameter. It is
 * implied that if a command is listed as supported, the feature underlying that
 * command is also supported.
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_zephyr_read_supported_commands(sdc_hci_cmd_vs_zephyr_read_supported_commands_return_t * p_return);

/** @brief Zephyr Write BD ADDR.
 *
 * This command writes the BD_ADDR (Bluetooth public device address) value
 * to the volatile memory. The address does not change during an HCI Reset but is
 * reset during a System Reset.
 * The address can be read out using the Read_BD_ADDR command.
 *
 * When the Write_BD_ADDR command has completed, a Command Complete event shall
 * be generated.
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_zephyr_write_bd_addr(const sdc_hci_cmd_vs_zephyr_write_bd_addr_t * p_params);

/** @brief Zephyr Read Static Addresses.
 *
 * This commands reads the controller specific static addresses.
 *
 * This command shall return the static addresses programmed by the vendor at
 * manufacturing time.
 *
 * Each returned static address shall confirm to the Static Device Address
 * definition. The two most significant bits of the address shall be equal to 1.
 * At least one bit of the random part of the address shall be 0. At least one bit
 * of the random part of the address shall be 1.
 *
 * The Identity_Root parameter may be all zeros to indicate no identity root key
 * being available for a given static address. The identity root key returned from
 * Read_Key_Hierarchy_Roots command shall not be returned from this command.
 *
 * Note: If no public address is provided and a static address is available, then
 * it is recommended to return an identity root key (if available) from this
 * command. In case a public address is provided, then it is recommended to use
 * the Read_Key_Hierarchy_Roots command to return the identity root key (if only
 * one is available).
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_zephyr_read_static_addresses(sdc_hci_cmd_vs_zephyr_read_static_addresses_return_t * p_return);

/** @brief Zephyr Read KEY Hierarchy Roots.
 *
 * This commands reads the controller specific identify and encryption root keys.
 *
 * This command shall return the identity root key and encryption root key
 * programmed by the vendor at manufacturing time. If a key is set to all zeros,
 * then the associated key is not available and it should not be used in the key
 * hierarchy.
 *
 * The identity root key and encryption root key may be used for the controllers
 * public device address or a static random address generated by the host. It
 * shall not be used for static addresses returned by Read_Static_Addresses
 * command that have its dedicated identity root key assigned.
 *
 * Note: For addresses returned by Read_Static_Addresses with an all zeros
 * identity root key, the returned Identity_Root value may be used. It is however
 * important that it only gets assigned to a single address (either public or
 * static random).
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_zephyr_read_key_hierarchy_roots(sdc_hci_cmd_vs_zephyr_read_key_hierarchy_roots_return_t * p_return);

/** @brief Zephyr Read Chip Temperature.
 *
 * This commands reads the controller chip temperature.
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_zephyr_read_chip_temp(sdc_hci_cmd_vs_zephyr_read_chip_temp_return_t * p_return);

/** @brief Zephyr Write Tx Power Level (per Role/Connection).
 *
 * This command dynamically modifies Bluetooth LE Tx power level at the antenna given a handle and a
 * handle type.
 *
 * The Tx power of the Bluetooth LE radio interface is modified for any low-level link by
 * the controller with a high degree of flexibility. The Bluetooth LE link whose power is
 * set is identified based on a handle type and handle pair.
 *
 * The role/state defining input parameter is the Handle_Type, whereas its
 * corresponding handle is provided by the Handle input parameter. Note that
 * for Advertisements, the Handle input parameter is ignored in the case that
 * Advertising Extensions are not configured, whereas Advertising Sets are to be
 * identified by their corresponding Handle in case Advertising Extensions are
 * enabled.
 *
 * The desired transmitter power level for the selected link instance is inputted
 * as Tx_Power_Level. This value represents the actual power level fed to the antenna.
 * When a Front-End Module is used, gain values for the SoC and FEM are calculated
 * automatically to guarantee closest possible match to the value requested by the user
 * at the RF output. The power setup and control can be performed dynamically
 * without the need of restarting the advertiser and scanner role/states. In case
 * of connections, the Tx power changes take effect only if the connections are
 * in a connected state.
 *
 * The inputs Handle_Type and Handle are passed through as outputs to aid the
 * asynchronous service of the command as well. In addition, the command returns
 * also with the Selected_Tx_Power by the controller which addresses and corrects
 * the possible mismatches between the desired Tx_Power_Level and the achievable
 * Tx powers given each individual controller capabilities.
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_zephyr_write_tx_power(const sdc_hci_cmd_vs_zephyr_write_tx_power_t * p_params,
                                             sdc_hci_cmd_vs_zephyr_write_tx_power_return_t * p_return);

/** @brief Zephyr Read Tx Power Level (per Role/Connection) Command.
 *
 * This command reads the BLE Tx power level.
 *
 * In contrast to the standardized HCI command, i.e. Read_Transmit_Power_Level,
 * which returns the transmitted power level only for a specified connection handle,
 * this command operates for both connected and unconnected states.
 * It gets the BLE Tx power level for any given handle type and handle.
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_zephyr_read_tx_power(const sdc_hci_cmd_vs_zephyr_read_tx_power_t * p_params,
                                            sdc_hci_cmd_vs_zephyr_read_tx_power_return_t * p_return);

/** @brief Read Supported Vendor Specific Commands.
 *
 * This command reads the list of vendor specific HCI commands supported
 * for the local Controller.
 *
 * This command shall return a bitmap of the supported vendor specific
 * commands.
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_read_supported_vs_commands(sdc_hci_cmd_vs_read_supported_vs_commands_return_t * p_return);

/** @brief Set Low Latency Packet Mode.
 *
 * This command enables or disables Low Latency Packet Mode support.
 * When Low Latency Packet Mode is enabled, it is possible to switch to connection intervals in the
 * range 1-7 ms. Switch to short connection intervals by calling @ref sdc_hci_cmd_vs_conn_update().
 *
 * After HCI Reset, this feature is disabled.
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_llpm_mode_set(const sdc_hci_cmd_vs_llpm_mode_set_t * p_params);

/** @brief Connection Update.
 *
 * This vendor specific command is used instead of HCI_LE_Connection_Update when
 * it is desirable to provide Connection Interval in microseconds instead of units.
 * See @ref sdc_hci_cmd_le_conn_update for description of behavior.
 *
 * Event(s) generated (unless masked away):
 * When the Controller receives the command, the Controller sends the
 * HCI_Command_Status event to the Host. The HCI_VS_Connection_Update_Complete
 * event shall be generated after the connection parameters have been applied
 * by the Controller or if the command subsequently fails.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_conn_update(const sdc_hci_cmd_vs_conn_update_t * p_params);

/** @brief Enable or Disable Extended Connection Events.
 *
 * When Extended Connection Events are disabled, the maximum connection event length is set
 * by @ref sdc_hci_cmd_vs_event_length_set(). When Extended Connection Events are enabled, the
 * controller
 * will extend the connection event as much as possible, if:
 * - Either of the peers has more data to send.
 *   See also: Core v5.1, Vol 6, Part B, Section 4.5.6
 * - There are no conflicts with other concurrent links.
 *
 * A connection event can not be extended beyond the connection interval.
 *
 * By default, that is after an HCI Reset, Extended Connection Events are enabled.
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_conn_event_extend(const sdc_hci_cmd_vs_conn_event_extend_t * p_params);

/** @brief QoS Connection Event Reports enable.
 *
 * This vendor specific command is used to enable or disable generation of QoS Connection event
 * reports.
 * See @ref sdc_hci_subevent_vs_qos_conn_event_report_t. When enabled, one report will be generated
 * every connection event.
 *
 * @note If the application does not pull a report in time, it will be overwritten.
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_qos_conn_event_report_enable(const sdc_hci_cmd_vs_qos_conn_event_report_enable_t * p_params);

/** @brief Set event length for ACL connections.
 *
 * Set the event length for new ACL connections. The event length is the time available for
 * transmission and reception in a single connection event.
 *
 * The maximum data length capabilities will be set based upon this value.
 *
 * This API must be called before issuing a command to create a connection,
 * or before starting a connectable advertiser.
 *
 * The default event length is 7500 us.
 *
 * The event length may be set to a value that is shorter than the time needed
 * for a single packet pair on a given PHY.
 * In that case the controller will reserve time for receiving 27 bytes and transmitting
 * the number of bytes configured with @ref sdc_hci_cmd_vs_min_val_of_max_acl_tx_payload_set().
 *
 * See also @ref sdc_hci_cmd_vs_conn_event_extend().
 * See also @ref sdc_hci_cmd_vs_central_acl_event_spacing_set().
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_event_length_set(const sdc_hci_cmd_vs_event_length_set_t * p_params);

/** @brief Set event length for periodic advertisers.
 *
 * Set the allocated event length for new periodic advertisers.
 * The SoftDevice Controller will ensure that the anchor points of periodic advertising events are
 * spaced
 * event_length_us apart. If the advertiser requires less time to transmit all the data, the
 * distance to
 * the next scheduling activity will still be equal to the configured event length. If the
 * advertiser
 * requires more time to transmit all the data, scheduling conflicts may occur.
 *
 * This API must be called before configuring a periodic advertiser for the event length to be
 * applied.
 *
 * The default event length is 7500 us.
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_periodic_adv_event_length_set(const sdc_hci_cmd_vs_periodic_adv_event_length_set_t * p_params);

/** @brief Configure Coexistence Scan Request Mode.
 *
 * This vendor specific command is used to configure the way the scanner requests a coexistence
 * session.
 * Either the scanner requests a coex session as soon as it has received a valid access address, or
 * it
 * only requests before transmitting.
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_coex_scan_mode_config(const sdc_hci_cmd_vs_coex_scan_mode_config_t * p_params);

/** @brief Configure Coexistence Per-Role Priority.
 *
 * This vendor specific command is used to configure the external radio coexistence
 * priorities depending on the Bluetooth device role.
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_coex_priority_config(const sdc_hci_cmd_vs_coex_priority_config_t * p_params);

/** @brief Set peripheral latency mode.
 *
 * This vendor specific command is used to disable/enable peripheral latency.
 * Three modes of disable/enable are supported.
 *
 * In enable mode, the peripheral will wake up every time it has data to send,
 * and/or every peripheral latency number of connection events.
 *
 * In disable mode, the peripheral will wake up on every connection event
 * regardless of the requested peripheral latency. This option consumes the most power.
 *
 * In wait_for_ack mode, the peripheral will wake up on every connection event if
 * it has not received an ACK from the master for at least peripheral latency events.
 * This configuration may increase the power consumption in environments with a lot of radio
 * activity.
 *
 * @note Shall only be called on peripheral links.
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_peripheral_latency_mode_set(const sdc_hci_cmd_vs_peripheral_latency_mode_set_t * p_params);

/** @brief Write remote transmit power level.
 *
 * This command requests adjustment of radio transmit power level for a connected peer device.
 *
 * When this command is issued, the controller initiates Power Control Request procedure
 * (Core_v5.3, Vol 6, Part B, Section 5.1.17) to request the change from the peer. The
 * peer responds with the actual transmit power level change and the controller reports
 * the change to the host with an event.
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Status event shall be generated.
 * When the controller has completed the power control request and if the request has resulted
 * in a change in the peer's transmit power level, an HCI_LE_Transmit_Power_Reporting event shall
 * be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_write_remote_tx_power(const sdc_hci_cmd_vs_write_remote_tx_power_t * p_params);

/** @brief Set advertising randomness.
 *
 * This vendor specific command is used to change the randomness of advertisers.
 * The setting applies to all subsequent advertising events of a given set.
 *
 * Event(s) generated (unless masked away):
 * When the Controller receives the command, the Controller sends the HCI_Command_Complete
 * event to the Host.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_set_adv_randomness(const sdc_hci_cmd_vs_set_adv_randomness_t * p_params);

/** @brief Set Compatibility mode for window offset.
 *
 * This compatibility mode enables interoperability with devices that do not support a value of 0
 * for the WinOffset parameter in the Link Layer CONNECT_IND packet.
 * This applies to a limited set of legacy peripheral devices from a limited set of vendors.
 * Enabling this compatibility mode will only have an effect if the local device will act as a
 * central device and initiate a connection to a peripheral device.
 * In that case it may lead to the connection creation taking up to one
 * connection interval longer to complete for all connections.
 *
 * By default this mode is disabled.
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_compat_mode_window_offset_set(const sdc_hci_cmd_vs_compat_mode_window_offset_set_t * p_params);

/** @brief Enable the Quality of Service (QoS) channel survey module.
 *
 * This vendor specific command is used to enable or disable the QoS channel survey module.
 *
 * The QoS channel survey module provides measurements of the energy levels on
 * the Bluetooth Low Energy channels. When the module is enabled, @ref
 * sdc_hci_subevent_vs_qos_channel_survey_report_t
 * events will periodically report the measured energy levels for each channel.
 *
 * The measurements are scheduled with low priority.
 *
 * The QoS channel survey module will attempt to do measurements so that the average interval
 * between measurements will be interval_us. However due to low scheduling priority, this may
 * not be possible. In that case fewer than expected reports may be given.
 *
 * In order to use the QoS channel survey module, funcref:sdc_support_qos_channel_survey
 * must be called.
 *
 * Event(s) geneated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_qos_channel_survey_enable(const sdc_hci_cmd_vs_qos_channel_survey_enable_t * p_params);

/** @brief Set LE Power Control Request procedure parameters.
 *
 * This command sets the parameters used in LE Power Control Request
 * procedure by the Link Layer.
 *
 * beta parameter is used to determine the weight of the previous average of RSSI values.
 * A higher value lowers how much the current RSSI weighs into the average, flattening peaks,
 * which also means the controller reacts slower on RSSI changes.
 * The average RSSI is calculated using an exponential weighted averaging in a
 * 12-bit fixed point fraction.
 * avg[n] = gamma * avg[n - 1] + (1 - gamma) * rssi[n]
 * Here, gamma equals beta/4096, and rssi[n] equals the current RSSI.
 * For example, for gamma to be 0.25, set the beta parameter in the command to 1024.
 *
 * Average RSSI and lower_limit parameter are used to calculate APR value the controller sends
 * in LL_POWER_CONTROL_RSP.
 *
 * When auto_enable parameter is set, the controller will keep average RSSI within
 * [lower_limit, upper_limit] bounds. When the average RSSI goes out of these bounds, the
 * controller will autonomously send LL_POWER_CONTROL_REQ requesting to adjust the
 * peer's TX power so average RSSI becomes either lower_target_rssi or upper_target_rssi.
 * The controller will not send such requests more often than specified by wait_period_ms parameter.
 * The wait_period_ms parameter is needed to not repeat send requests for transmit power change
 * without the remote having had the chance to react, as well as to avoid a busy controller.
 * This value should be set depending on needs.
 *
 * When apr_enable parameter is set, the controller will adjust local TX power according to
 * APR value received from the peer in LL_POWER_CONTROL_RSP and the apr_margin parameter.
 *
 * When this command is issued, the controller stores the parameters and
 * uses them for the subsequent LE Power Control Request procedures across all the connections.
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_set_power_control_request_params(const sdc_hci_cmd_vs_set_power_control_request_params_t * p_params);

/** @brief Read average RSSI.
 *
 * This command reads the average Received Signal Strength Indication (RSSI) value
 * calculated by LE Power Control.
 *
 * The average RSSI is calculated as an exponential weighted average according to
 * the formula given in @ref sdc_hci_cmd_vs_set_power_control_request_params().
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_read_average_rssi(const sdc_hci_cmd_vs_read_average_rssi_t * p_params,
                                         sdc_hci_cmd_vs_read_average_rssi_return_t * p_return);

/** @brief Set Central ACL event spacing.
 *
 * On the central, sets the time ACL connections are spaced apart, assuming they are
 * using the same connection interval.
 *
 * This API must be called before issuing a command to create a connection.
 *
 * The default event spacing is 7500 us.
 * The configured value is retained after issuing a HCI Reset command.
 *
 * Note: The time available for transmission and reception is not configured using this API
 *
 * See also @ref sdc_hci_cmd_vs_event_length_set().
 * See also @ref sdc_hci_cmd_vs_conn_event_extend().
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_central_acl_event_spacing_set(const sdc_hci_cmd_vs_central_acl_event_spacing_set_t * p_params);

/** @brief Set Connection Event Trigger.
 *
 * Start triggering a given task on radio event start.
 *
 * When enabled, this feature will trigger a (D)PPI task at the start of radio events.
 *
 * When used for connections, the connection event trigger can be configured to trigger
 * every N connection events starting from a given connection event counter.
 *
 * Disabling scanning, removing the advertising set, or disconnecting the connection will reset the
 * connection event trigger configuration.
 *
 * If the selected (D)PPI channel is reserved by the controller, the controller will
 * return the error code Invalid HCI Command Parameters (0x12).
 *
 * If enabling/disabling the connection event trigger and the trigger is already
 * enabled/disabled, the controller will return the error code Command Disallowed (0x0C).
 *
 * If the role is 0x1, 0x2, or 0x4, and the role is not currently active,
 * the controller will return the error code Command Disallowed (0x0C).
 *
 * If the role is 0x3 and conn_handle does not refer to an active connection, the controller
 * will return the error code Unknown Connection Identifier (0x02).
 *
 * If the role is 0x4 and legacy advertising is used, conn_handle must be set to 0.
 * If extended advertising is used, conn_handle must refer to an active advertising set.
 * Otherwise, the controller will return the error code Unknown Advertising Identifier (0x42).
 *
 * If the role is 0x3 and conn_evt_counter_start has already passed, the controller will return
 * the error code Command Disallowed (0x0C).
 *
 * If the role is 0x3 and period_in_events is zero, the controller will return the error code
 * Invalid HCI Command Parameters (0x12).
 *
 * If the role is 0x1, 0x2, or 0x4 and conn_evt_counter_start or period_in_events is non-zero,
 * the controller will return the error code Invalid HCI Command Parameters (0x12).
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_set_conn_event_trigger(const sdc_hci_cmd_vs_set_conn_event_trigger_t * p_params);

/** @brief Get Next Connection Event Counter.
 *
 * This command can be used to fetch the upcoming connection event counter value for Centrals or
 * Peripherals.
 *
 * If conn_handle doesn't match an existing connection, the Unknown Connection Identifier (0x02)
 * error code will be returned.
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_get_next_conn_event_counter(const sdc_hci_cmd_vs_get_next_conn_event_counter_t * p_params,
                                                   sdc_hci_cmd_vs_get_next_conn_event_counter_return_t * p_return);

/** @brief Allow Parallel Connection Establishment.
 *
 * This command enables the controller to accept establishing connections through
 * the initiator and a periodic advertiser with responses simultaneously.
 *
 * When enabled, HCI commands to create connections may then be used to
 * connect to a synchronized peer while the initiator is running, or to start
 * initiating before a connection establishment to a synchronized device
 * has been completed.
 *
 * By default this functionality is disabled.
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_allow_parallel_connection_establishments(const sdc_hci_cmd_vs_allow_parallel_connection_establishments_t * p_params);

/** @brief Set the minimum value that will be used as maximum Tx octets for ACL connections.
 *
 * This command sets the minimum value of maximum ACL payload length that can be sent
 * in each packet. If the configured event length is shorter than what is required to
 * send a packet pair of 27 bytes in each direction, the controller will use this value to
 * determine how much it can reduce the payload size to satisfy the event length requirements.
 * LL Control PDUs are not affected by this API.
 *
 * Together with @ref sdc_hci_cmd_vs_event_length_set(), this API allows the controller to schedule
 * ACLs events closer together with other activities.
 *
 * This API must be called before issuing a command to create a connection,
 * or before starting a connectable advertiser.
 *
 * The default and maximum value of this parameter is 27. The minimum supported value
 * of this parameter is 10.
 *
 * Setting this parameter to a lower value will result in more link layer fragmentation,
 * reducing the maximum throughput.
 *
 * Setting this parameter to a value lower than 27 bytes may result in interoperability
 * issues with older Bluetooth products. It is therefore not recommended to use this API
 * for applications interacting with devices qualified for Bluetooth Specification 5.1 or
 * older.
 *
 * The value is preserved when issuing the HCI Reset command.
 *
 * See also @ref sdc_hci_cmd_vs_event_length_set().
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_min_val_of_max_acl_tx_payload_set(const sdc_hci_cmd_vs_min_val_of_max_acl_tx_payload_set_t * p_params);

/** @brief Iso Read Tx Timestamp.
 *
 * The controller operates on a timeline that determines when an SDU provided
 * by the host is scheduled for transmission. This command is used to return the
 * tx_time_stamp and packet_sequence_number that determines where on this timeline
 * the previously provided SDU was scheduled. The packet_sequence_number is a
 * quantization of the tx_time_stamp. The SDU is provided for transmission on a
 * CIS or BIS as identified by the conn_handle parameter on the
 * Central or Peripheral.
 *
 * This command is made to simplify sending SDUs on different ISO streams
 * in the same ISO event.
 *
 * The returned timestamp corresponds to the SDU synchronization reference
 * as defined in Core_v5.4, Vol 6, Part G, Section 3.2.
 * If the provided handle identifies a CIS, the returned timestamp corresponds to
 * the SDU synchronization reference for the central to peripheral direction.
 *
 * The returned timestamp can be used to make the application provide SDUs to the
 * controller right before they are sent on air. The returned value
 * can also be used to synchronize the transmitter and receiver.
 *
 * If the Host issues this command with a connection handle that does not exist,
 * or the connection handle is not associated with a CIS or BIS, the Controller
 * shall return the error code Unknown Connection Identifier (0x02).
 *
 * If the Host issues this command on an existing connection handle for a CIS or
 * BIS where a ISO data path is not enabled, the Controller shall return the
 * error code Command Disallowed (0x0C).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Read_ISO_TX_Sync command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_iso_read_tx_timestamp(const sdc_hci_cmd_vs_iso_read_tx_timestamp_t * p_params,
                                             sdc_hci_cmd_vs_iso_read_tx_timestamp_return_t * p_return);

/** @brief Set the default BIG reserved time.
 *
 * This vendor specific command changes the time reserved at the end of a BIG event for other roles.
 * This applies to all BIGs created after calling this command.
 *
 * The default value is 1600 us, but can be set to between 0 us and 4,000,000 us. Changes persist
 * after an HCI_Reset command.
 *
 * If the value is set such that it cannot be satisfied for a given set of BIG parameters, BIG
 * creation will fail with error code UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE (0x11).
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_big_reserved_time_set(const sdc_hci_cmd_vs_big_reserved_time_set_t * p_params);

/** @brief Set the default CIG reserved time.
 *
 * This vendor specific command changes the time reserved each ISO interval for other roles.
 * This applies to all CIGs created after calling this command.
 * For example, if the reserved time is set to 2.5 ms and the ISO interval is 10 ms,
 * the controller may select CIG parameters such that 2.5 ms is reserved for an ACL connection
 * and the remaining 7.5 ms is used for the CIG.
 * Note, when multiple CIGs are used the user needs to ensure that the initial CIG reserves time
 * for the remaining CIGs.
 *
 * The default value is 1300 us, but can be set to between 0 us and 4,000,000 us. Changes persist
 * after an HCI_Reset command.
 *
 * If the value is set such that it cannot be satisfied for a given set of CIG parameters, the
 * actual
 * reserved time may be smaller than the requested value.
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_cig_reserved_time_set(const sdc_hci_cmd_vs_cig_reserved_time_set_t * p_params);

/** @brief Set the CIS subevent length in microseconds.
 *
 * This command sets the subevent length to be used for CISes
 * in microseconds.
 * If the configured subevent length is too small, the
 * HCI command used to configure the CIG will fail.
 * If this parameter is set to zero, the subevent length
 * is chosen by the controller.
 *
 * The default value is 0. That is, the minimum required subevent length
 * will be used.
 *
 * This API must be called before creating a CIG.
 *
 * The configured value is preserved when issuing the HCI Reset command.
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_cis_subevent_length_set(const sdc_hci_cmd_vs_cis_subevent_length_set_t * p_params);

/** @brief Set the channel map for scanning and initiating.
 *
 * This command sets the RF channels that should be used for scanning and initiating
 * on the primary advertising channels. The channel map will be used for subsequent
 * commands to start scanning or to create connections. Scanning and initiating
 * that was started before issuing this command is not affected.
 *
 * The default behavior is to listan on all primary advertising channels.
 * The default behavior is restored when issuing the HCI Reset command.
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_scan_channel_map_set(const sdc_hci_cmd_vs_scan_channel_map_set_t * p_params);

/** @brief Scan accept extended advertising packets set.
 *
 * This command enables or disables reception of extended advertising packets
 * when extended scanner or extended initiator HCI commands are used.
 *
 * When reception of extended advertising packets is disabled,
 * the scanner may be able to receive more legacy advertising packets.
 * Reception of extended advertising packets should only be disabled
 * when the application knows it is not interested in reports from extended advertisers.
 *
 * After HCI Reset, reception of extended advertising packets is enabled.
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_scan_accept_ext_adv_packets_set(const sdc_hci_cmd_vs_scan_accept_ext_adv_packets_set_t * p_params);

/** @brief Set priority of a BT role.
 *
 * This vendor specific command changes the scheduling priority of a role running in the controller.
 *
 * Note that unless documented otherwise, any non-default priorities are not tested.
 * This means that there is no guarantee that the controller works as intended when non-tested
 * priorities are used.
 * The default values for the priorities of the different roles are listed in
 * https://docs.nordicsemi.com/bundle/ncs-
 * latest/page/nrfxlib/softdevice_controller/doc/scheduling.html
 *
 * Priority changes configured using this API are not persisted on resets.
 *
 * If the handle is not associated with an instance of the role_id type,
 * the error code Unknown Connection Identifier (0x02) is returned.
 *
 * If the role_id is set to an invalid or unsupported role,
 * the error code Invalid HCI Command Parameters (0x12) is returned.
 *
 * If the priority is set to an invalid or unsupported value,
 * the error code Invalid HCI Command Parameters (0x12) is returned.
 *
 * Event(s) generated (unless masked away):
 * When the command has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_vs_set_role_priority(const sdc_hci_cmd_vs_set_role_priority_t * p_params);

/** @} end of HCI_VS_API */

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* SDC_HCI_VS_H__ */
