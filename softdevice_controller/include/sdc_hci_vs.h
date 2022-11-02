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
#include <cmsis_compiler.h>

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
};

/** @brief VS subevent Code values. */
enum sdc_hci_subevent_vs
{
    /** @brief See @ref sdc_hci_subevent_vs_qos_conn_event_report_t. */
    SDC_HCI_SUBEVENT_VS_QOS_CONN_EVENT_REPORT = 0x80,
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

/** @brief TX power handle type. */
enum sdc_hci_vs_tx_power_handle_type
{
    /** @brief Handle of type Advertiser. */
    SDC_HCI_VS_TX_POWER_HANDLE_TYPE_ADV = 0x00,
    /** @brief Handle of type Scanner or Initiator. */
    SDC_HCI_VS_TX_POWER_HANDLE_TYPE_SCAN_INIT = 0x01,
    /** @brief Handle of type Connection. */
    SDC_HCI_VS_TX_POWER_HANDLE_TYPE_CONN = 0x02,
};

/** @brief Supported Vendor Specific HCI Commands. */
typedef __PACKED_STRUCT
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
} sdc_hci_vs_supported_vs_commands_t;

/** @brief Zephyr Static Address type. */
typedef __PACKED_STRUCT
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
typedef __PACKED_STRUCT
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
    uint16_t crc_error_count;
    /** @brief Number of received Negative Acknowledges from the peer during the connection event.
     */
    uint16_t nak_count;
    /** @brief Indicates that the connection event was closed because a packet was not received. */
    uint8_t rx_timeout : 1;
} sdc_hci_subevent_vs_qos_conn_event_report_t;

/** @} end of HCI_EVENTS */

/**
 * @defgroup HCI_COMMAND_PARAMETERS Parameters
 * @{
 */

/** @brief Zephyr Read Version Information return parameter(s). */
typedef __PACKED_STRUCT
{
    /** @brief Assigned hardware manufacturer. Always 0x0002 indicating Nordic Semiconductor. */
    uint16_t hw_platform;
    /** @brief Assigned platform specific value. Set to 0x2 for nRF52, 0x3 for nRF53. */
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
typedef __PACKED_UNION
{
    /** @brief Bit mask for each vendor command. If a bit is 1, the Controller supports the
     *         corresponding command and the features required for the command, unsupported or
     *         undefined commands shall be set to 0.
     */
    sdc_hci_vs_zephyr_supported_commands_t params;
    uint8_t raw[64];
} sdc_hci_cmd_vs_zephyr_read_supported_commands_return_t;

/** @brief Zephyr Write BD ADDR command parameter(s). */
typedef __PACKED_STRUCT
{
    /** @brief BD_ADDR of the Device. */
    uint8_t bd_addr[6];
} sdc_hci_cmd_vs_zephyr_write_bd_addr_t;

/** @brief Zephyr Read Static Addresses return parameter(s). */
typedef __PACKED_STRUCT
{
    /** @brief Number of static device addresses. */
    uint8_t num_addresses;
    /** @brief Zephyr Static Addresses. The number of addresses is specified in num_addresses. */
    sdc_hci_vs_zephyr_static_address_t addresses[];
} sdc_hci_cmd_vs_zephyr_read_static_addresses_return_t;

/** @brief Zephyr Read KEY Hierarchy Roots return parameter(s). */
typedef __PACKED_STRUCT
{
    /** @brief Zephyr Identity Root Key. */
    uint8_t ir[16];
    /** @brief Zephyr Encryption Root Key. */
    uint8_t er[16];
} sdc_hci_cmd_vs_zephyr_read_key_hierarchy_roots_return_t;

/** @brief Zephyr Read Chip Temperature return parameter(s). */
typedef __PACKED_STRUCT
{
    /** @brief The measured temperature in degrees Celsius. */
    int8_t temp;
} sdc_hci_cmd_vs_zephyr_read_chip_temp_return_t;

/** @brief Zephyr Write Tx Power Level (per Role/Connection) command parameter(s). */
typedef __PACKED_STRUCT
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
typedef __PACKED_STRUCT
{
    /** @brief Handle type. See @ref sdc_hci_vs_tx_power_handle_type. */
    uint8_t handle_type;
    /** @brief See @ref sdc_hci_cmd_vs_zephyr_write_tx_power_t. */
    uint16_t handle;
    /** @brief The selected Tx Power in dBm. */
    int8_t selected_tx_power;
} sdc_hci_cmd_vs_zephyr_write_tx_power_return_t;

/** @brief Zephyr Read Tx Power Level (per Role/Connection) Command command parameter(s). */
typedef __PACKED_STRUCT
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
typedef __PACKED_STRUCT
{
    /** @brief Handle type. See @ref sdc_hci_vs_tx_power_handle_type. */
    uint8_t handle_type;
    /** @brief See @ref sdc_hci_cmd_vs_zephyr_read_tx_power_t. */
    uint16_t handle;
    /** @brief The selected Tx Power in dBm. */
    int8_t selected_tx_power;
} sdc_hci_cmd_vs_zephyr_read_tx_power_return_t;

/** @brief Read Supported Vendor Specific Commands return parameter(s). */
typedef __PACKED_UNION
{
    sdc_hci_vs_supported_vs_commands_t params;
    uint8_t raw[64];
} sdc_hci_cmd_vs_read_supported_vs_commands_return_t;

/** @brief Set Low Latency Packet Mode command parameter(s). */
typedef __PACKED_STRUCT
{
    /** @brief Set to 1 to enable LLPM. */
    uint8_t enable;
} sdc_hci_cmd_vs_llpm_mode_set_t;

/** @brief Connection Update command parameter(s). */
typedef __PACKED_STRUCT
{
    /** @brief Connection Handle. */
    uint16_t connection_handle;
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
typedef __PACKED_STRUCT
{
    /** @brief Set to 0 for disabling, 1 for enabling, all other values are RFU. */
    uint8_t enable;
} sdc_hci_cmd_vs_conn_event_extend_t;

/** @brief QoS Connection Event Reports enable command parameter(s). */
typedef __PACKED_STRUCT
{
    /** @brief Set to 0 for disabling, 1 for enabling, all other values are RFU. */
    uint8_t enable;
} sdc_hci_cmd_vs_qos_conn_event_report_enable_t;

/** @brief Set event length for connections command parameter(s). */
typedef __PACKED_STRUCT
{
    /** @brief Allocated event length in microseconds. */
    uint32_t event_length_us;
} sdc_hci_cmd_vs_event_length_set_t;

/** @brief Set event length for periodic advertisers command parameter(s). */
typedef __PACKED_STRUCT
{
    /** @brief Allocated periodic advertising event length in microseconds. */
    uint32_t event_length_us;
} sdc_hci_cmd_vs_periodic_adv_event_length_set_t;

/** @brief Configure Coexistence Scan Request Mode command parameter(s). */
typedef __PACKED_STRUCT
{
    /** @brief Scanner request mode. See @ref sdc_hci_vs_coex_scan_mode. */
    uint8_t mode;
} sdc_hci_cmd_vs_coex_scan_mode_config_t;

/** @brief Configure Coexistence Per-Role Priority command parameter(s). */
typedef __PACKED_STRUCT
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
typedef __PACKED_STRUCT
{
    /** @brief Connection handle. */
    uint16_t conn_handle;
    /** @brief Peripheral latency mode. See @ref sdc_hci_vs_peripheral_latency_mode. */
    uint8_t mode;
} sdc_hci_cmd_vs_peripheral_latency_mode_set_t;

/** @brief Write remote transmit power level command parameter(s). */
typedef __PACKED_STRUCT
{
    uint16_t conn_handle;
    /** @brief Phy value to apply transmit power level adjustment. */
    uint8_t phy;
    /** @brief The transmit power level adjustment to request in dBm unit. */
    int8_t delta;
} sdc_hci_cmd_vs_write_remote_tx_power_t;

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
 * handle type (advertiser, scanner, connection).
 *
 * The Tx power of the Bluetooth LE radio interface is modified for any low-level link by
 * the controller with a high degree of flexibility. The Bluetooth LE link whose power is
 * set is identified based on a handle type (advertiser, scanner, connection) and
 * handle pair.
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
 * It gets the BLE Tx power level for any given handle type (advertiser, scanner, connection) and
 * handle.
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
 * This vendor specific command is used to change the Link Layer Connection parameters of a
 * connection.
 * This command may be issued by the master only.
 *
 * The Supervision_Timeout in milliseconds shall be larger than (1 + Conn_Latency) *
 * Conn_Interval_Max * 2,
 * where Conn_Interval_Max is given in milliseconds.
 *
 * Event(s) generated (unless masked away):
 * When the Controller receives the command, the Controller sends the HCI_Command_Complete
 * event to the Host. The HCI_LE_Connection_Update_Complete event shall be generated after
 * the connection parameters have been applied by the Controller or if the command
 * subsequently fails.
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

/** @brief Set event length for connections.
 *
 * Set the event length for new connections. This API must be called before starting a connectable
 * advertiser or starting an initiator for the event length to applied to the connection once
 * established.
 *
 * The SoftDevice Controller will ensure that the anchor points of master link connections are
 * spaced
 * event_length_us apart.
 *
 * The default event length is @ref SDC_DEFAULT_EVENT_LENGTH_US.
 *
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
 * The default event length is @ref SDC_DEFAULT_EVENT_LENGTH_US.
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

/** @} end of HCI_VS_API */

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* SDC_HCI_VS_H__ */
