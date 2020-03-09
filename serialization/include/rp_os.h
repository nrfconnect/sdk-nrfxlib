/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef RP_OS_H_
#define RP_OS_H_

#include <rp_ser.h>
#include <rp_errors.h>

/**
 * @file
 * @defgroup rp_os Remote Procedures Serialization OS abstraction API
 * @{
 * @brief Remote Procedures OS-specific API
 */

#ifdef __cplusplus
extern "C" {
#endif

/**@brief Helper macro for getting signal instance.
 *
 * @param[in] _signal_type Signal mechanism data type.
 * @param[in] _rp_instance Remote procedure serialization instance.
 */
#define RP_OS_SIGNAL_GET(_signal_type, _rp_instance) \
	(_signal_type *)_rp_instance->rp_sem

/**@brief Function for initializing OS signal mechanism.
 *
 * @param[in] rp Remote Procedure serialization instance.
 *
 * @retval 0 If the operation was successful.
 *           Otherwise, a error code is returned.
 */
rp_err_t rp_os_signal_init(struct rp_ser *rp);

/**@brief Function to be called after requesting a remote procedure call. It
 *        should implement a 'Wait for signal' functionality in the OS.
 *
 * @param[in] rp Remote Procedure serialization instance.
 *
 * @retval 0 If the operation was successful.
 *           Otherwise, a error code is returned.
 */
rp_err_t rp_os_response_wait(struct rp_ser *rp);

/**@brief Function to be called after after remote procedure response reception.
 *        It should implement a 'Signal set' functionality in the OS.
 *
 * @param[in] rp Remote Procedure serialization instance.
 *
 * @retval 0 If the operation was successful.
 *           Otherwise, a error code is returned.
 */
rp_err_t rp_os_response_signal(struct rp_ser *rp);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* RP_OS_H_ */
