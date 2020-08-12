/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#ifndef BLE_CONTROLLER_HCI_H__
#define BLE_CONTROLLER_HCI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "sdc_hci.h"

#define hci_cmd_put  sdc_hci_cmd_put
#define hci_data_put sdc_hci_data_put
#define hci_evt_get  sdc_hci_evt_get
#define hci_data_get sdc_hci_data_get

#ifdef __cplusplus
}
#endif

#endif /* BLE_CONTROLLER_HCI_H__ */
