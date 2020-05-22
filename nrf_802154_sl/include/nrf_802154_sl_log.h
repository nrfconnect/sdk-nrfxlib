/*
 * Copyright (c) 2020 Nordic Semiconductor ASA. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic Semiconductor ASA.
 * The use, copying, transfer or disclosure of such information is prohibited except by
 * express written agreement with Nordic Semiconductor ASA.
 */

#ifndef NRF_802154_SL_LOG_H__
#define NRF_802154_SL_LOG_H__

extern uint32_t g_nrf_802154_sl_log_buffer[];
extern uint32_t * gp_nrf_802154_sl_log_ptr;

#define nrf_802154_sl_log_function_enter(...)

#define nrf_802154_sl_log_function_exit(...)

#define nrf_802154_sl_log_local_event(...)

#define nrf_802154_sl_log_global_event(...)

#endif // NRF_802154_SL_LOG_H__
