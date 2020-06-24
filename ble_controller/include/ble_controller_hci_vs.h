/*
 * Copyright (c) Nordic Semiconductor ASA
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
 *
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
 * @defgroup HCI_TYPES Types
 * @{
 */

/** @brief HCI VS OpCode Field values. */
enum HCI_VS_OPCODE
{
    /** @brief See @ref hci_vs_cmd_zephyr_read_version_info(). */
    HCI_VS_OPCODE_CMD_ZEPHYR_READ_VERSION_INFO = 0xfc01,
    /** @brief See @ref hci_vs_cmd_zephyr_read_supported_commands(). */
    HCI_VS_OPCODE_CMD_ZEPHYR_READ_SUPPORTED_COMMANDS = 0xfc02,
    /** @brief See @ref hci_vs_cmd_zephyr_read_static_addresses(). */
    HCI_VS_OPCODE_CMD_ZEPHYR_READ_STATIC_ADDRESSES = 0xfc09,
    /** @brief See @ref hci_vs_cmd_zephyr_write_tx_power(). */
    HCI_VS_OPCODE_CMD_ZEPHYR_WRITE_TX_POWER = 0xfc0e,
    /** @brief See @ref hci_vs_cmd_llpm_mode_set(). */
    HCI_VS_OPCODE_CMD_LLPM_MODE_SET = 0xfd01,
    /** @brief See @ref hci_vs_cmd_conn_update(). */
    HCI_VS_OPCODE_CMD_CONN_UPDATE = 0xfd02,
    /** @brief See @ref hci_vs_cmd_conn_event_extend(). */
    HCI_VS_OPCODE_CMD_CONN_EVENT_EXTEND = 0xfd03,
    /** @brief See @ref hci_vs_cmd_qos_conn_event_report_enable(). */
    HCI_VS_OPCODE_CMD_QOS_CONN_EVENT_REPORT_ENABLE = 0xfd04,
    /** @brief See @ref hci_vs_cmd_event_length_set(). */
    HCI_VS_OPCODE_CMD_EVENT_LENGTH_SET = 0xfd05,
};

/** @brief VS subevent Code values. */
enum HCI_VS_SUBEVENT
{
    /** @brief See @ref hci_vs_subevent_qos_conn_event_report_t. */
    HCI_VS_SUBEVENT_QOS_CONN_EVENT_REPORT = 0x80,
};

/** @brief TX power handle type. */
enum HCI_VS_TX_POWER_HANDLE_TYPE
{
    /** @brief Handle of type Advertiser. */
    HCI_VS_TX_POWER_HANDLE_TYPE_ADV = 0x00,
    /** @brief Handle of type Scanner or Initiator. */
    HCI_VS_TX_POWER_HANDLE_TYPE_SCAN_INIT = 0x01,
    /** @brief Handle of type Connection. */
    HCI_VS_TX_POWER_HANDLE_TYPE_CONN = 0x02,
};

/** @brief Zephyr Static Adress type. */
typedef __PACKED_STRUCT
{
    /** @brief Static device address. */
    uint8_t address[6];
    /** @brief Identity root key (IR) for static device address. All zero parameter value indicates
     *         missing identity root key.
     */
    uint8_t identity_root[16];
} hci_vs_zephyr_static_address_t;

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
} hci_vs_zephyr_supported_commands_t;

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
    uint8_t crc_error_count;
    /** @brief Indicates that the connection event was closed because a packet was not received. */
    uint8_t rx_timeout;
} hci_vs_subevent_qos_conn_event_report_t;

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
} hci_vs_cmd_zephyr_read_version_info_return_t;

/** @brief Zephyr Read Supported Commands return parameter(s). */
typedef __PACKED_UNION
{
    /** @brief Bit mask for each vendor command. If a bit is 1, the Controller supports the
     *         corresponding command and the features required for the command, unsupported or
     *         undefined commands shall be set to 0.
     */
    hci_vs_zephyr_supported_commands_t params;
    uint8_t raw[64];
} hci_vs_cmd_zephyr_read_supported_commands_return_t;

/** @brief Zephyr Read Static Addresses return parameter(s). */
typedef __PACKED_STRUCT
{
    /** @brief Number of static device addresses. */
    uint8_t num_addresses;
    /** @brief Zephyr Static Addresses. The number of addresses is specified in num_addresses. */
    hci_vs_zephyr_static_address_t addresses[];
} hci_vs_cmd_zephyr_read_static_addresses_return_t;

/** @brief Zephyr Write Tx Power Level (per Role/Connection) command parameter(s). */
typedef __PACKED_STRUCT
{
    /** @brief Handle type. See @ref HCI_VS_TX_POWER_HANDLE_TYPE. */
    uint8_t handle_type;
    /** @brief Handle of the selected handle_type that identifies the instance to set the power of.
     *         In case of Extended Advertising, the handle specifies the advertising set. In case of
     *         a connection, it specifies a Connection Handle. Otherwise this parameter is ignored.
     */
    uint16_t handle;
    /** @brief The desired Tx_Power_Level in dBm in signed 1 octet integer format. If set to 127,
     *         this indicates that the controller shall revert to using its default setting for Tx
     *         power. If the selected power level is not supported, an error is returned.
     */
    int8_t tx_power_level;
} hci_vs_cmd_zephyr_write_tx_power_t;

/** @brief Zephyr Write Tx Power Level (per Role/Connection) return parameter(s). */
typedef __PACKED_STRUCT
{
    /** @brief Handle type. See @ref HCI_VS_TX_POWER_HANDLE_TYPE. */
    uint8_t handle_type;
    /** @brief See @ref hci_vs_cmd_zephyr_write_tx_power_t. */
    uint16_t handle;
    /** @brief The selected Tx Power in dBm. */
    int8_t selected_tx_power;
} hci_vs_cmd_zephyr_write_tx_power_return_t;

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

/** @brief Set event length for connections command parameter(s). */
typedef __PACKED_STRUCT
{
    /** @brief Allocated event length in microseconds. */
    uint32_t event_length_us;
} hci_vs_cmd_event_length_set_t;

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
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t hci_vs_cmd_zephyr_read_version_info(hci_vs_cmd_zephyr_read_version_info_return_t * p_return);

/** @brief Zephyr Read Supported Commands.
 *
 * This command reads the list of vendor commands supported for the local Controller.
 *
 * This command shall return the Supported_Commands configuration parameter. It is
 * implied that if a command is listed as supported, the feature underlying that
 * command is also supported.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t hci_vs_cmd_zephyr_read_supported_commands(hci_vs_cmd_zephyr_read_supported_commands_return_t * p_return);

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
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t hci_vs_cmd_zephyr_read_static_addresses(hci_vs_cmd_zephyr_read_static_addresses_return_t * p_return);

/** @brief Zephyr Write Tx Power Level (per Role/Connection).
 *
 * This command dynamically modifies BLE Tx power level given a handle and a
 * handle type (advertiser, scanner, connection).
 *
 * The Tx power of the BLE radio interface is modified for any low-level link by
 * the controller with a high degree of flexibility. The BLE link whose power is
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
 * as Tx_Power_Level. The power setup and control can be performed dynamically
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
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t hci_vs_cmd_zephyr_write_tx_power(const hci_vs_cmd_zephyr_write_tx_power_t * p_params,
                                         hci_vs_cmd_zephyr_write_tx_power_return_t * p_return);

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
 * hci_vs_cmd_event_length_set().
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
 * See @ref hci_vs_subevent_qos_conn_event_report_t. When enabled, one report will be generated
 * every connection event.
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

/** @brief Set event length for connections.
 *
 * Set the event length for new connections. This API must be called before starting a connectable
 * advertiser or starting an initiator for the event length to applied to the connection once established.
 * The event length will not be changed for existing connections.
 *
 * The BLE controller will ensure that the anchor points of master link connections are spaced
 * event_length_us apart.
 *
 * The default event length is @ref BLE_CONTROLLER_DEFAULT_EVENT_LENGTH_US.
 *
 * See also @ref hci_vs_cmd_conn_event_extend().
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t hci_vs_cmd_event_length_set(const hci_vs_cmd_event_length_set_t * p_params);

/** @} end of HCI_VS_API */

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* BLE_CONTROLLER_HCI_VS_H__ */
