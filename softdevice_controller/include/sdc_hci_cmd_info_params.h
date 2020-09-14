/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**
 * @file sdc_hci_cmd_info_params.h
 *
 * @defgroup sdc_hci_ip SoftDevice Controller HCI Interface for Informational parameters
 * @ingroup sdc_hci
 *
 * All APIs in this header file are expected to be called from the
 * same execution priority as mpsl_low_priority_process.
 * Not doing so will lead to undefined behavior.
 *
 * @{
 */

#ifndef SDC_HCI_CMD_INFO_PARAMS_H__
#define SDC_HCI_CMD_INFO_PARAMS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <cmsis_compiler.h>

/**
 * @defgroup HCI_TYPES Types
 * @{
 */

/** @brief HCI IP OpCode Field values. */
enum sdc_hci_opcode_ip
{
    /** @brief See @ref sdc_hci_cmd_ip_read_local_version_information(). */
    SDC_HCI_OPCODE_CMD_IP_READ_LOCAL_VERSION_INFORMATION = 0x1001,
    /** @brief See @ref sdc_hci_cmd_ip_read_local_supported_commands(). */
    SDC_HCI_OPCODE_CMD_IP_READ_LOCAL_SUPPORTED_COMMANDS = 0x1002,
    /** @brief See @ref sdc_hci_cmd_ip_read_local_supported_features(). */
    SDC_HCI_OPCODE_CMD_IP_READ_LOCAL_SUPPORTED_FEATURES = 0x1003,
    /** @brief See @ref sdc_hci_cmd_ip_read_bd_addr(). */
    SDC_HCI_OPCODE_CMD_IP_READ_BD_ADDR = 0x1009,
};

/** @brief LMP features.
 *
 * See Core_v5.2, Vol 2, Part C, Section 3.3
 */
typedef __PACKED_STRUCT
{
    uint8_t slot3_packets : 1;
    uint8_t slot5_packets : 1;
    uint8_t encryption : 1;
    uint8_t slot_offset : 1;
    uint8_t timing_accuracy : 1;
    uint8_t role_switch : 1;
    uint8_t hold_mode : 1;
    uint8_t sniff_mode : 1;
    uint8_t previously_used : 1;
    uint8_t power_control_requests : 1;
    uint8_t cqddr : 1;
    uint8_t sco_link : 1;
    uint8_t hv2_packets : 1;
    uint8_t hv3_packets : 1;
    uint8_t u_law_log_sync_data : 1;
    uint8_t a_law_log_sync_data : 1;
    uint8_t cvsd_sync_data : 1;
    uint8_t paging_param_negiotiation : 1;
    uint8_t power_control : 1;
    uint8_t transparent_sync_data : 1;
    uint8_t flow_control_lag : 3;
    uint8_t broadcast_encryption : 1;
    uint8_t rfu1 : 1;
    uint8_t enhanced_data_rate_acl_2mbps_mode : 1;
    uint8_t enhanced_data_rate_acl_3mbps_mode : 1;
    uint8_t enhanced_inquiry_scan : 1;
    uint8_t interlaced_inquiry_scan : 1;
    uint8_t interlaced_page_scan : 1;
    uint8_t rssi_with_inquiry_results : 1;
    uint8_t ext_sco_link : 1;
    uint8_t ev4_packets : 1;
    uint8_t ev5_packets : 1;
    uint8_t rfu2 : 1;
    uint8_t afh_capable_slave : 1;
    uint8_t afh_classification_slave : 1;
    uint8_t bdedr_not_supported : 1;
    uint8_t le_supported : 1;
    uint8_t slot3_enhanced_data_rate : 1;
    uint8_t slot5_enhanced_data_rate : 1;
    uint8_t sniff_subrating : 1;
    uint8_t pause_encryption : 1;
    uint8_t afh_capable_master : 1;
    uint8_t afh_classification_master : 1;
    uint8_t enhanced_data_rate_esco_2mbps_mode : 1;
    uint8_t enhanced_data_rate_esco_3mbps_mode : 1;
    uint8_t slot3_enhanced_data_rate_esco : 1;
    uint8_t ext_inquiry_response : 1;
    uint8_t simultanious_le_and_bdedr : 1;
    uint8_t rfu3 : 1;
    uint8_t secure_simple_pairing : 1;
    uint8_t encapsulated_pdu : 1;
    uint8_t errorneous_data_reporting : 1;
    uint8_t non_flushable_packet_boundary : 1;
    uint8_t rfu4 : 1;
    uint8_t hci_link_supervision_timeout_changed_event : 1;
    uint8_t variable_inquiry_tx_power_level : 1;
    uint8_t enhanced_power_control : 1;
    uint8_t rfu5 : 1;
    uint8_t rfu6 : 1;
    uint8_t rfu7 : 1;
    uint8_t rfu8 : 1;
    uint8_t ext_features : 1;
} sdc_hci_ip_lmp_features_t;

/** @brief Supported HCI commands.
 *
 * See Core_v5.2, Vol 4, Part E, Section 6.27
 */
typedef __PACKED_STRUCT
{
    uint8_t HCI_Inquiry : 1;
    uint8_t HCI_Inquiry_Cancel : 1;
    uint8_t HCI_Periodic_Inquiry_Mode : 1;
    uint8_t HCI_Exit_Periodic_Inquiry_Mode : 1;
    uint8_t HCI_Create_Connection : 1;
    uint8_t HCI_Disconnect : 1;
    uint8_t HCI_Add_SCO_Connection : 1;
    uint8_t HCI_Create_Connection_Cancel : 1;
    uint8_t HCI_Accept_Connection_Request : 1;
    uint8_t HCI_Reject_Connection_Request : 1;
    uint8_t HCI_Link_Key_Request_Reply : 1;
    uint8_t HCI_Link_Key_Request_Negative_Reply : 1;
    uint8_t HCI_PIN_Code_Request_Reply : 1;
    uint8_t HCI_PIN_Code_Request_Negative_Reply : 1;
    uint8_t HCI_Change_Connection_Packet_Type : 1;
    uint8_t HCI_Authentication_Requested : 1;
    uint8_t HCI_Set_Connection_Encryption : 1;
    uint8_t HCI_Change_Connection_Link_Key : 1;
    uint8_t HCI_Master_Link_Key : 1;
    uint8_t HCI_Remote_Name_Request : 1;
    uint8_t HCI_Remote_Name_Request_Cancel : 1;
    uint8_t HCI_Read_Remote_Supported_Features : 1;
    uint8_t HCI_Read_Remote_Extended_Features : 1;
    uint8_t HCI_Read_Remote_Version_Information : 1;
    uint8_t HCI_Read_Clock_Offset : 1;
    uint8_t HCI_Read_LMP_Handle : 1;
    uint8_t rfu1 : 1;
    uint8_t rfu2 : 1;
    uint8_t rfu3 : 1;
    uint8_t rfu4 : 1;
    uint8_t rfu5 : 1;
    uint8_t rfu6 : 1;
    uint8_t rfu7 : 1;
    uint8_t HCI_Hold_Mode : 1;
    uint8_t HCI_Sniff_Mode : 1;
    uint8_t HCI_Exit_Sniff_Mode : 1;
    uint8_t previously_used1 : 1;
    uint8_t previously_used2 : 1;
    uint8_t HCI_QoS_Setup : 1;
    uint8_t HCI_Role_Discovery : 1;
    uint8_t HCI_Switch_Role : 1;
    uint8_t HCI_Read_Link_Policy_Settings : 1;
    uint8_t HCI_Write_Link_Policy_Settings : 1;
    uint8_t HCI_Read_Default_Link_Policy_Settings : 1;
    uint8_t HCI_Write_Default_Link_Policy_Settings : 1;
    uint8_t HCI_Flow_Specification : 1;
    uint8_t HCI_Set_Event_Mask : 1;
    uint8_t HCI_Reset : 1;
    uint8_t HCI_Set_Event_Filter : 1;
    uint8_t HCI_Flush : 1;
    uint8_t HCI_Read_PIN_Type : 1;
    uint8_t HCI_Write_PIN_Type : 1;
    uint8_t previously_used3 : 1;
    uint8_t HCI_Read_Stored_Link_Key : 1;
    uint8_t HCI_Write_Stored_Link_Key : 1;
    uint8_t HCI_Delete_Stored_Link_Key : 1;
    uint8_t HCI_Write_Local_Name : 1;
    uint8_t HCI_Read_Local_Name : 1;
    uint8_t HCI_Read_Connection_Accept_Timeout : 1;
    uint8_t HCI_Write_Connection_Accept_Timeout : 1;
    uint8_t HCI_Read_Page_Timeout : 1;
    uint8_t HCI_Write_Page_Timeout : 1;
    uint8_t HCI_Read_Scan_Enable : 1;
    uint8_t HCI_Write_Scan_Enable : 1;
    uint8_t HCI_Read_Page_Scan_Activity : 1;
    uint8_t HCI_Write_Page_Scan_Activity : 1;
    uint8_t HCI_Read_Inquiry_Scan_Activity : 1;
    uint8_t HCI_Write_Inquiry_Scan_Activity : 1;
    uint8_t HCI_Read_Authentication_Enable : 1;
    uint8_t HCI_Write_Authentication_Enable : 1;
    uint8_t HCI_Read_Encryption_Mode : 1;
    uint8_t HCI_Write_Encryption_Mode : 1;
    uint8_t HCI_Read_Class_Of_Device : 1;
    uint8_t HCI_Write_Class_Of_Device : 1;
    uint8_t HCI_Read_Voice_Setting : 1;
    uint8_t HCI_Write_Voice_Setting : 1;
    uint8_t HCI_Read_Automatic_Flush_Timeout : 1;
    uint8_t HCI_Write_Automatic_Flush_Timeout : 1;
    uint8_t HCI_Read_Num_Broadcast_Retransmissions : 1;
    uint8_t HCI_Write_Num_Broadcast_Retransmissions : 1;
    uint8_t HCI_Read_Hold_Mode_Activity : 1;
    uint8_t HCI_Write_Hold_Mode_Activity : 1;
    uint8_t HCI_Read_Transmit_Power_Level : 1;
    uint8_t HCI_Read_Synchronous_Flow_Control_Enable : 1;
    uint8_t HCI_Write_Synchronous_Flow_Control_Enable : 1;
    uint8_t HCI_Set_Controller_To_Host_Flow_Control : 1;
    uint8_t HCI_Host_Buffer_Size : 1;
    uint8_t HCI_Host_Number_Of_Completed_Packets : 1;
    uint8_t HCI_Read_Link_Supervision_Timeout : 1;
    uint8_t HCI_Write_Link_Supervision_Timeout : 1;
    uint8_t HCI_Read_Number_Of_Supported_IAC : 1;
    uint8_t HCI_Read_Current_IAC_LAP : 1;
    uint8_t HCI_Write_Current_IAC_LAP : 1;
    uint8_t HCI_Read_Page_Scan_Mode_Period : 1;
    uint8_t HCI_Write_Page_Scan_Mode_Period : 1;
    uint8_t HCI_Read_Page_Scan_Mode : 1;
    uint8_t HCI_Write_Page_Scan_Mode : 1;
    uint8_t HCI_Set_AFH_Host_Channel_Classification : 1;
    uint8_t rfu8 : 1;
    uint8_t rfu9 : 1;
    uint8_t HCI_Read_Inquiry_Scan_Type : 1;
    uint8_t HCI_Write_Inquiry_Scan_Type : 1;
    uint8_t HCI_Read_Inquiry_Mode : 1;
    uint8_t HCI_Write_Inquiry_Mode : 1;
    uint8_t HCI_Read_Page_Scan_Type : 1;
    uint8_t HCI_Write_Page_Scan_Type : 1;
    uint8_t HCI_Read_AFH_Channel_Assessment_Mode : 1;
    uint8_t HCI_Write_AFH_Channel_Assessment_Mode : 1;
    uint8_t rfu10 : 1;
    uint8_t rfu11 : 1;
    uint8_t rfu12 : 1;
    uint8_t rfu13 : 1;
    uint8_t rfu14 : 1;
    uint8_t rfu15 : 1;
    uint8_t rfu16 : 1;
    uint8_t HCI_Read_Local_Version_Information : 1;
    uint8_t rfu17 : 1;
    uint8_t HCI_Read_Local_Supported_Features : 1;
    uint8_t HCI_Read_Local_Extended_Features : 1;
    uint8_t HCI_Read_Buffer_Size : 1;
    uint8_t HCI_Read_Country_Code : 1;
    uint8_t HCI_Read_BD_ADDR : 1;
    uint8_t HCI_Read_Failed_Contact_Counter : 1;
    uint8_t HCI_Reset_Failed_Contact_Counter : 1;
    uint8_t HCI_Read_Link_Quality : 1;
    uint8_t HCI_Read_RSSI : 1;
    uint8_t HCI_Read_AFH_Channel_Map : 1;
    uint8_t HCI_Read_Clock : 1;
    uint8_t HCI_Read_Loopback_Mode : 1;
    uint8_t HCI_Write_Loopback_Mode : 1;
    uint8_t HCI_Enable_Device_Under_Test_Mode : 1;
    uint8_t HCI_Setup_Synchronous_Connection_Request : 1;
    uint8_t HCI_Accept_Synchronous_Connection_Request : 1;
    uint8_t HCI_Reject_Synchronous_Connection_Request : 1;
    uint8_t rfu18 : 1;
    uint8_t rfu19 : 1;
    uint8_t HCI_Read_Extended_Inquiry_Response : 1;
    uint8_t HCI_Write_Extended_Inquiry_Response : 1;
    uint8_t HCI_Refresh_Encryption_Key : 1;
    uint8_t rfu20 : 1;
    uint8_t HCI_Sniff_Subrating : 1;
    uint8_t HCI_Read_Simple_Pairing_Mode : 1;
    uint8_t HCI_Write_Simple_Pairing_Mode : 1;
    uint8_t HCI_Read_Local_OOB_Data : 1;
    uint8_t HCI_Read_Inquiry_Response_Transmit_Power_Level : 1;
    uint8_t HCI_Write_Inquiry_Transmit_Power_Level : 1;
    uint8_t HCI_Read_Default_Erroneous_Data_Reporting : 1;
    uint8_t HCI_Write_Default_Erroneous_Data_Reporting : 1;
    uint8_t rfu21 : 1;
    uint8_t rfu22 : 1;
    uint8_t rfu23 : 1;
    uint8_t HCI_IO_Capability_Request_Reply : 1;
    uint8_t HCI_User_Confirmation_Request_Reply : 1;
    uint8_t HCI_User_Confirmation_Request_Negative_Reply : 1;
    uint8_t HCI_User_Passkey_Request_Reply : 1;
    uint8_t HCI_User_Passkey_Request_Negative_Reply : 1;
    uint8_t HCI_Remote_OOB_Data_Request_Reply : 1;
    uint8_t HCI_Write_Simple_Pairing_Debug_Mode : 1;
    uint8_t HCI_Enhanced_Flush : 1;
    uint8_t HCI_Remote_OOB_Data_Request_Negative_Reply : 1;
    uint8_t rfu24 : 1;
    uint8_t rfu25 : 1;
    uint8_t HCI_Send_Keypress_Notification : 1;
    uint8_t HCI_IO_Capability_Request_Negative_Reply : 1;
    uint8_t HCI_Read_Encryption_Key_Size : 1;
    uint8_t rfu26 : 1;
    uint8_t rfu27 : 1;
    uint8_t rfu28 : 1;
    uint8_t HCI_Create_Physical_Link : 1;
    uint8_t HCI_Accept_Physical_Link : 1;
    uint8_t HCI_Disconnect_Physical_Link : 1;
    uint8_t HCI_Create_Logical_Link : 1;
    uint8_t HCI_Accept_Logical_Link : 1;
    uint8_t HCI_Disconnect_Logical_Link : 1;
    uint8_t HCI_Logical_Link_Cancel : 1;
    uint8_t HCI_Flow_Spec_Modify : 1;
    uint8_t HCI_Read_Logical_Link_Accept_Timeout : 1;
    uint8_t HCI_Write_Logical_Link_Accept_Timeout : 1;
    uint8_t HCI_Set_Event_Mask_Page_2 : 1;
    uint8_t HCI_Read_Location_Data : 1;
    uint8_t HCI_Write_Location_Data : 1;
    uint8_t HCI_Read_Local_AMP_Info : 1;
    uint8_t HCI_Read_Local_AMP_ASSOC : 1;
    uint8_t HCI_Write_Remote_AMP_ASSOC : 1;
    uint8_t HCI_Read_Flow_Control_Mode : 1;
    uint8_t HCI_Write_Flow_Control_Mode : 1;
    uint8_t HCI_Read_Data_Block_Size : 1;
    uint8_t rfu29 : 1;
    uint8_t rfu30 : 1;
    uint8_t HCI_Enable_AMP_Receiver_Reports : 1;
    uint8_t HCI_AMP_Test_End : 1;
    uint8_t HCI_AMP_Test : 1;
    uint8_t HCI_Read_Enhanced_Transmit_Power_Level : 1;
    uint8_t rfu31 : 1;
    uint8_t HCI_Read_Best_Effort_Flush_Timeout : 1;
    uint8_t HCI_Write_Best_Effort_Flush_Timeout : 1;
    uint8_t HCI_Short_Range_Mode : 1;
    uint8_t HCI_Read_LE_Host_Support : 1;
    uint8_t HCI_Write_LE_Host_Support : 1;
    uint8_t rfu32 : 1;
    uint8_t HCI_LE_Set_Event_Mask : 1;
    uint8_t HCI_LE_Read_Buffer_Size_v1 : 1;
    uint8_t HCI_LE_Read_Local_Supported_Features : 1;
    uint8_t rfu33 : 1;
    uint8_t HCI_LE_Set_Random_Address : 1;
    uint8_t HCI_LE_Set_Advertising_Parameters : 1;
    uint8_t HCI_LE_Read_Advertising_Physical_Channel_Tx_Power : 1;
    uint8_t HCI_LE_Set_Advertising_Data : 1;
    uint8_t HCI_LE_Set_Scan_Response_Data : 1;
    uint8_t HCI_LE_Set_Advertising_Enable : 1;
    uint8_t HCI_LE_Set_Scan_Parameters : 1;
    uint8_t HCI_LE_Set_Scan_Enable : 1;
    uint8_t HCI_LE_Create_Connection : 1;
    uint8_t HCI_LE_Create_Connection_Cancel : 1;
    uint8_t HCI_LE_Read_White_List_Size : 1;
    uint8_t HCI_LE_Clear_White_List : 1;
    uint8_t HCI_LE_Add_Device_To_White_List : 1;
    uint8_t HCI_LE_Remove_Device_From_White_List : 1;
    uint8_t HCI_LE_Connection_Update : 1;
    uint8_t HCI_LE_Set_Host_Channel_Classification : 1;
    uint8_t HCI_LE_Read_Channel_Map : 1;
    uint8_t HCI_LE_Read_Remote_Features : 1;
    uint8_t HCI_LE_Encrypt : 1;
    uint8_t HCI_LE_Rand : 1;
    uint8_t HCI_LE_Enable_Encryption : 1;
    uint8_t HCI_LE_Long_Term_Key_Request_Reply : 1;
    uint8_t HCI_LE_Long_Term_Key_Request_Negative_Reply : 1;
    uint8_t HCI_LE_Read_Supported_States : 1;
    uint8_t HCI_LE_Receiver_Test_v1 : 1;
    uint8_t HCI_LE_Transmitter_Test_v1 : 1;
    uint8_t HCI_LE_Test_End : 1;
    uint8_t rfu34 : 1;
    uint8_t rfu35 : 1;
    uint8_t rfu36 : 1;
    uint8_t rfu37 : 1;
    uint8_t HCI_Enhanced_Setup_Synchronous_Connection : 1;
    uint8_t HCI_Enhanced_Accept_Synchronous_Connection : 1;
    uint8_t HCI_Read_Local_Supported_Codecs : 1;
    uint8_t HCI_Set_MWS_Channel_Parameters : 1;
    uint8_t HCI_Set_External_Frame_Configuration : 1;
    uint8_t HCI_Set_MWS_Signaling : 1;
    uint8_t HCI_Set_MWS_Transport_Layer : 1;
    uint8_t HCI_Set_MWS_Scan_Frequency_Table : 1;
    uint8_t HCI_Get_MWS_Transport_Layer_Configuration : 1;
    uint8_t HCI_Set_MWS_PATTERN_Configuration : 1;
    uint8_t HCI_Set_Triggered_Clock_Capture : 1;
    uint8_t HCI_Truncated_Page : 1;
    uint8_t HCI_Truncated_Page_Cancel : 1;
    uint8_t HCI_Set_Connectionless_Slave_Broadcast : 1;
    uint8_t HCI_Set_Connectionless_Slave_Broadcast_Receive : 1;
    uint8_t HCI_Start_Synchronization_Train : 1;
    uint8_t HCI_Receive_Synchronization_Train : 1;
    uint8_t HCI_Set_Reserved_LT_ADDR : 1;
    uint8_t HCI_Delete_Reserved_LT_ADDR : 1;
    uint8_t HCI_Set_Connectionless_Slave_Broadcast_Data : 1;
    uint8_t HCI_Read_Synchronization_Train_Parameters : 1;
    uint8_t HCI_Write_Synchronization_Train_Parameters : 1;
    uint8_t HCI_Remote_OOB_Extended_Data_Request_Reply : 1;
    uint8_t HCI_Read_Secure_Connections_Host_Support : 1;
    uint8_t HCI_Write_Secure_Connections_Host_Support : 1;
    uint8_t HCI_Read_Authenticated_Payload_Timeout : 1;
    uint8_t HCI_Write_Authenticated_Payload_Timeout : 1;
    uint8_t HCI_Read_Local_OOB_Extended_Data : 1;
    uint8_t HCI_Write_Secure_Connections_Test_Mode : 1;
    uint8_t HCI_Read_Extended_Page_Timeout : 1;
    uint8_t HCI_Write_Extended_Page_Timeout : 1;
    uint8_t HCI_Read_Extended_Inquiry_Length : 1;
    uint8_t HCI_Write_Extended_Inquiry_Length : 1;
    uint8_t HCI_LE_Remote_Connection_Parameter_Request_Reply : 1;
    uint8_t HCI_LE_Remote_Connection_Parameter_Request_Negative_Reply : 1;
    uint8_t HCI_LE_Set_Data_Length : 1;
    uint8_t HCI_LE_Read_Suggested_Default_Data_Length : 1;
    uint8_t HCI_LE_Write_Suggested_Default_Data_Length : 1;
    uint8_t HCI_LE_Read_Local_P256_Public_Key : 1;
    uint8_t HCI_LE_Generate_DHKey_v1 : 1;
    uint8_t HCI_LE_Add_Device_To_Resolving_List : 1;
    uint8_t HCI_LE_Remove_Device_From_Resolving_List : 1;
    uint8_t HCI_LE_Clear_Resolving_List : 1;
    uint8_t HCI_LE_Read_Resolving_List_Size : 1;
    uint8_t HCI_LE_Read_Peer_Resolvable_Address : 1;
    uint8_t HCI_LE_Read_Local_Resolvable_Address : 1;
    uint8_t HCI_LE_Set_Address_Resolution_Enable : 1;
    uint8_t HCI_LE_Set_Resolvable_Private_Address_Timeout : 1;
    uint8_t HCI_LE_Read_Maximum_Data_Length : 1;
    uint8_t HCI_LE_Read_PHY : 1;
    uint8_t HCI_LE_Set_Default_PHY : 1;
    uint8_t HCI_LE_Set_PHY : 1;
    uint8_t HCI_LE_Receiver_Test_v2 : 1;
    uint8_t HCI_LE_Transmitter_Test_v2 : 1;
    uint8_t HCI_LE_Set_Advertising_Set_Random_Address : 1;
    uint8_t HCI_LE_Set_Extended_Advertising_Parameters : 1;
    uint8_t HCI_LE_Set_Extended_Advertising_Data : 1;
    uint8_t HCI_LE_Set_Extended_Scan_Response_Data : 1;
    uint8_t HCI_LE_Set_Extended_Advertising_Enable : 1;
    uint8_t HCI_LE_Read_Maximum_Advertising_Data_Length : 1;
    uint8_t HCI_LE_Read_Number_of_Supported_Advertising_Sets : 1;
    uint8_t HCI_LE_Remove_Advertising_Set : 1;
    uint8_t HCI_LE_Clear_Advertising_Sets : 1;
    uint8_t HCI_LE_Set_Periodic_Advertising_Parameters : 1;
    uint8_t HCI_LE_Set_Periodic_Advertising_Data : 1;
    uint8_t HCI_LE_Set_Periodic_Advertising_Enable : 1;
    uint8_t HCI_LE_Set_Extended_Scan_Parameters : 1;
    uint8_t HCI_LE_Set_Extended_Scan_Enable : 1;
    uint8_t HCI_LE_Extended_Create_Connection : 1;
    uint8_t HCI_LE_Periodic_Advertising_Create_Sync : 1;
    uint8_t HCI_LE_Periodic_Advertising_Create_Sync_Cancel : 1;
    uint8_t HCI_LE_Periodic_Advertising_Terminate_Sync : 1;
    uint8_t HCI_LE_Add_Device_To_Periodic_Advertiser_List : 1;
    uint8_t HCI_LE_Remove_Device_From_Periodic_Advertiser_List : 1;
    uint8_t HCI_LE_Clear_Periodic_Advertiser_List : 1;
    uint8_t HCI_LE_Read_Periodic_Advertiser_List_Size : 1;
    uint8_t HCI_LE_Read_Transmit_Power : 1;
    uint8_t HCI_LE_Read_RF_Path_Compensation : 1;
    uint8_t HCI_LE_Write_RF_Path_Compensation : 1;
    uint8_t HCI_LE_Set_Privacy_Mode : 1;
    uint8_t HCI_LE_Receiver_Test_v3 : 1;
    uint8_t HCI_LE_Transmitter_Test_v3 : 1;
    uint8_t HCI_LE_Set_Connectionless_CTE_Transmit_Parameters : 1;
    uint8_t HCI_LE_Set_Connectionless_CTE_Transmit_Enable : 1;
    uint8_t HCI_LE_Set_Connectionless_IQ_Sampling_Enable : 1;
    uint8_t HCI_LE_Set_Connection_CTE_Receive_Parameters : 1;
    uint8_t HCI_LE_Set_Connection_CTE_Transmit_Parameters : 1;
    uint8_t HCI_LE_Connection_CTE_Request_Enable : 1;
    uint8_t HCI_LE_Connection_CTE_Response_Enable : 1;
    uint8_t HCI_LE_Read_Antenna_Information : 1;
    uint8_t HCI_LE_Set_Periodic_Advertising_Receive_Enable : 1;
    uint8_t HCI_LE_Periodic_Advertising_Sync_Transfer : 1;
    uint8_t HCI_LE_Periodic_Advertising_Set_Info_Transfer : 1;
    uint8_t HCI_LE_Set_Periodic_Advertising_Sync_Transfer_Parameters : 1;
    uint8_t HCI_LE_Set_Default_Periodic_Advertising_Sync_Transfer_Parameters : 1;
    uint8_t HCI_LE_Generate_DHKey_v2 : 1;
    uint8_t HCI_Read_Local_Simple_Pairing_Options : 1;
    uint8_t HCI_LE_Modify_Sleep_Clock_Accuracy : 1;
    uint8_t HCI_LE_Read_Buffer_Size_v2 : 1;
    uint8_t HCI_LE_Read_ISO_TX_Sync : 1;
    uint8_t HCI_LE_Set_CIG_Parameters : 1;
    uint8_t HCI_LE_Set_CIG_Parameters_Test : 1;
    uint8_t HCI_LE_Create_CIS : 1;
    uint8_t HCI_LE_Remove_CIG : 1;
    uint8_t HCI_LE_Accept_CIS_Request : 1;
    uint8_t HCI_LE_Reject_CIS_Request : 1;
    uint8_t HCI_LE_Create_BIG : 1;
    uint8_t HCI_LE_Create_BIG_Test : 1;
    uint8_t HCI_LE_Terminate_BIG : 1;
    uint8_t HCI_LE_BIG_Create_Sync : 1;
    uint8_t HCI_LE_BIG_Terminate_Sync : 1;
    uint8_t HCI_LE_Request_Peer_SCA : 1;
    uint8_t HCI_LE_Setup_ISO_Data_Path : 1;
    uint8_t HCI_LE_Remove_ISO_Data_Path : 1;
    uint8_t HCI_LE_ISO_Transmit_Test : 1;
    uint8_t HCI_LE_ISO_Receive_Test : 1;
    uint8_t HCI_LE_ISO_Read_Test_Counters : 1;
    uint8_t HCI_LE_ISO_Test_End : 1;
    uint8_t HCI_LE_Set_Host_Feature : 1;
    uint8_t HCI_LE_Read_ISO_Link_Quality : 1;
    uint8_t HCI_LE_Enhanced_Read_Transmit_Power_Level : 1;
    uint8_t HCI_LE_Read_Remote_Transmit_Power_Level : 1;
    uint8_t HCI_LE_Set_Path_Loss_Reporting_Parameters : 1;
    uint8_t HCI_LE_Set_Path_Loss_Reporting_Enable : 1;
    uint8_t HCI_LE_Set_Transmit_Power_Reporting_Enable : 1;
    uint8_t HCI_LE_Transmitter_Test_v4 : 1;
    uint8_t HCI_Set_Ecosystem_Base_Interval : 1;
    uint8_t HCI_Read_Local_Supported_Codecs_v2 : 1;
    uint8_t HCI_Read_Local_Supported_Codec_Capabilities : 1;
    uint8_t HCI_Read_Local_Supported_Controller_Delay : 1;
    uint8_t HCI_Configure_Data_Path : 1;
    uint8_t rfu38 : 1;
    uint8_t rfu39 : 1;
} sdc_hci_ip_supported_commands_t;

/** @} end of HCI_TYPES */

/**
 * @defgroup HCI_COMMAND_PARAMETERS Parameters
 * @{
 */

/** @brief Read Local Version Information return parameter(s). */
typedef __PACKED_STRUCT
{
    uint8_t hci_version;
    uint16_t hci_revision;
    uint8_t lmp_pal_version;
    uint16_t manufacturer_name;
    uint16_t lmp_pal_subversion;
} sdc_hci_cmd_ip_read_local_version_information_return_t;

/** @brief Read Local Supported Commands return parameter(s). */
typedef __PACKED_UNION
{
    sdc_hci_ip_supported_commands_t params;
    uint8_t raw[64];
} sdc_hci_cmd_ip_read_local_supported_commands_return_t;

/** @brief Read Local Supported Features return parameter(s). */
typedef __PACKED_UNION
{
    sdc_hci_ip_lmp_features_t params;
    uint8_t raw[8];
} sdc_hci_cmd_ip_read_local_supported_features_return_t;

/** @brief Read BD_ADDR return parameter(s). */
typedef __PACKED_STRUCT
{
    uint8_t bd_addr[6];
} sdc_hci_cmd_ip_read_bd_addr_return_t;

/** @} end of HCI_COMMAND_PARAMETERS */

/**
 * @defgroup HCI_VS_API API
 * @{
 */
/** @brief Read Local Version Information.
 *
 * The description below is extracted from Core_v5.2,
 * Vol 4, Part E, Section 7.4.1
 *
 * This command reads the values for the version information for the local
 * Controller.
 *
 * The HCI_Version information defines the version information of the HCI layer.
 * The LMP_PAL_Version information defines the version of the LMP or PAL. The
 * Manufacturer_Name information indicates the manufacturer of the local device.
 *
 * The HCI_Revision and LMP_PAL_Subversion are implementation dependent.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_Read_Local_Version_Information command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_ip_read_local_version_information(sdc_hci_cmd_ip_read_local_version_information_return_t * p_return);

/** @brief Read Local Supported Commands.
 *
 * The description below is extracted from Core_v5.2,
 * Vol 4, Part E, Section 7.4.2
 *
 * This command reads the list of HCI commands supported for the local
 * Controller.
 *
 * This command shall return the Supported_Commands configuration
 * parameter. It is implied that if a command is listed as supported, the feature
 * underlying that command is also supported.
 *
 * See Section 6.27 for more information.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_Read_Local_Supported_Commands command has completed,
 * an HCI_Command_Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_ip_read_local_supported_commands(sdc_hci_cmd_ip_read_local_supported_commands_return_t * p_return);

/** @brief Read Local Supported Features.
 *
 * The description below is extracted from Core_v5.2,
 * Vol 4, Part E, Section 7.4.3
 *
 * This command requests a list of the supported features for the local BR/EDR
 * Controller. This command will return a list of the LMP features. For details see
 * [Vol 2] Part C, Link Manager Protocol Specification.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_Read_Local_Supported_Features command has completed,
 * an HCI_Command_Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_ip_read_local_supported_features(sdc_hci_cmd_ip_read_local_supported_features_return_t * p_return);

/** @brief Read BD_ADDR.
 *
 * The description below is extracted from Core_v5.2,
 * Vol 4, Part E, Section 7.4.6
 *
 * On a BR/EDR Controller, this command reads the Bluetooth Controller address
 * (BD_ADDR). (See [Vol 2] Part B, Section 1.2 and [Vol 3] Part C, Section 3.2.1).
 *
 * On an LE Controller, this command shall read the Public Device Address as
 * defined in [Vol 6] Part B, Section 1.3. If this Controller does not have a Public
 * Device Address, the value 0x000000000000 shall be returned.
 *
 * On a BR/EDR/LE Controller, the public address shall be the same as the
 * BD_ADDR.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_Read_BD_ADDR command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_ip_read_bd_addr(sdc_hci_cmd_ip_read_bd_addr_return_t * p_return);

/** @} end of HCI_VS_API */

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* SDC_HCI_CMD_INFO_PARAMS_H__ */
