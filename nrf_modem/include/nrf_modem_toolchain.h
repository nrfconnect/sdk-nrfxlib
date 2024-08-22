/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file nrf_modem_toolchain.h
 *
 * @defgroup nrf_modem_toolchain library compiler toolchain attributes
 * @{
 * @brief Modem library toolchain
 */
#ifndef NRF_MODEM_TOOLCHAIN_H__
#define NRF_MODEM_TOOLCHAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNUC__
#define __nrf_modem_attr_packed __attribute__((__packed__))
#define __nrf_modem_printf_like(f, a) __attribute__((format (__printf__, f, a)))
#define __nrf_modem_scanf_like(f, a) __attribute__((format (__scanf__, f, a)))
#else
#define __nrf_modem_attr_packed
#define __nrf_modem_printf_like(f, a)
#define __nrf_modem_scanf_like(f, a)
#endif

#ifdef __cplusplus
}
#endif

#endif /* NRF_MODEM_TOOLCHAIN_H__ */
/** @} */
