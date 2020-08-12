/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#ifndef BLE_CONTROLLER_H__
#define BLE_CONTROLLER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "sdc.h"

#define BLE_CONTROLLER_DEFAULT_RESOURCE_CFG_TAG SDC_DEFAULT_RESOURCE_CFG_TAG
#define BLE_CONTROLLER_DEFAULT_SLAVE_COUNT      SDC_DEFAULT_SLAVE_COUNT
#define BLE_CONTROLLER_DEFAULT_MASTER_COUNT     SDC_DEFAULT_MASTER_COUNT
#define BLE_CONTROLLER_DEFAULT_TX_PACKET_SIZE   SDC_DEFAULT_TX_PACKET_SIZE
#define BLE_CONTROLLER_DEFAULT_RX_PACKET_SIZE   SDC_DEFAULT_RX_PACKET_SIZE
#define BLE_CONTROLLER_DEFAULT_TX_PACKET_COUNT  SDC_DEFAULT_TX_PACKET_COUNT
#define BLE_CONTROLLER_DEFAULT_RX_PACKET_COUNT  SDC_DEFAULT_RX_PACKET_COUNT
#define BLE_CONTROLLER_DEFAULT_EVENT_LENGTH_US  SDC_DEFAULT_EVENT_LENGTH_US
#define BLE_CONTROLLER_BUILD_REVISION_SIZE      SDC_BUILD_REVISION_SIZE

#define BLE_CONTROLLER_MEM_DEFAULT_MASTER_LINK_SIZE SDC_MEM_DEFAULT_MASTER_LINK_SIZE
#define BLE_CONTROLLER_MEM_DEFAULT_SLAVE_LINK_SIZE  SDC_MEM_DEFAULT_SLAVE_LINK_SIZE
#define BLE_CONTROLLER_MEM_BUFFER_OVERHEAD_SIZE     SDC_MEM_BUFFER_OVERHEAD_SIZE

#define BLE_CONTROLLER_MEM_ADDITIONAL_LINK_SIZE SDC_MEM_ADDITIONAL_LINK_SIZE
#define BLE_CONTROLLER_MEM_PER_MASTER_LINK      SDC_MEM_PER_MASTER_LINK
#define BLE_CONTROLLER_MEM_PER_SLAVE_LINK       SDC_MEM_PER_SLAVE_LINK

#define BLE_CONTROLLER_MEM_MASTER_LINKS_SHARED SDC_MEM_MASTER_LINKS_SHARED
#define BLE_CONTROLLER_MEM_SLAVE_LINKS_SHARED  SDC_MEM_SLAVE_LINKS_SHARED

#define ble_controller_fault_handler_t sdc_fault_handler_t
#define ble_controller_callback_t      sdc_callback_t

#define BLE_CONTROLLER_CFG_TYPE        sdc_cfg_type
#define BLE_CONTROLLER_CFG_TYPE_NONE   SDC_CFG_TYPE_NONE
#define BLE_CONTROLLER_CFG_TYPE_MASTER_COUNT SDC_CFG_TYPE_MASTER_COUNT
#define BLE_CONTROLLER_CFG_TYPE_SLAVE_COUNT  SDC_CFG_TYPE_SLAVE_COUNT
#define BLE_CONTROLLER_CFG_TYPE_BUFFER_CFG   SDC_CFG_TYPE_BUFFER_CFG
#define BLE_CONTROLLER_CFG_TYPE_EVENT_LENGTH SDC_CFG_TYPE_EVENT_LENGTH

#define ble_controller_cfg_role_count_t sdc_cfg_role_count_t
#define ble_controller_cfg_buffer_cfg_t sdc_cfg_event_length_t
#define ble_controller_cfg_t            sdc_cfg_t

#define ble_controller_init                 sdc_init
#define ble_controller_cfg_set              sdc_cfg_set
#define ble_controller_enable               sdc_enable
#define ble_controller_disable              sdc_disable
#define ble_controller_build_revision_get   sdc_build_revision_get
#define ble_controller_RNG_IRQHandler       sdc_RNG_IRQHandler
#define ble_controller_support_dle          sdc_support_dle
#define ble_controller_support_le_2m_phy    sdc_support_le_2m_phy
#define ble_controller_support_le_coded_phy sdc_support_le_coded_phy

#ifdef __cplusplus
}
#endif

#endif /* BLE_CONTROLLER_H__ */
