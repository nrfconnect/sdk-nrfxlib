/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file nrf_apn_class.h
 *
 * @defgroup nrf_apn_class nRF91 APN Class Management
 * @{
 */
#ifndef NRF_APN_CLASS_H__
#define NRF_APN_CLASS_H__

#include <stdint.h>

/**@brief Update Class with new APN.
 *
 * This function will write a new APN to the specified Class.
 *
 * @param[in]  apn_class      Class to update.
 * @param[in]  p_apn          Buffer containing the APN to set.
 * @param[in]  apn_len        Length of the APN pointed to by p_apn.
 *
 * @retval 0            If create operation was successful.
 * @retval NRF_EIO      If operation was not successful due to internal error.
 * @retval NRF_ENOBUFS  If the operation could not be performed because it could not allocate
 *                      enough intermediate buffers to perform the operation.
 * @retval NRF_ENOENT   If the Class indicated cannot be written.
 * @retval NRF_ENOMEM   If there was not memory enough to store the APN name.
 * @retval NRF_EPERM    If the application did not have permission to do the operation.
 * @retval NRF_EINVAL   If one or more of the provided parameters are not valid.
 */
int nrf_apn_class_update(uint8_t         apn_class,
                         const uint8_t * p_apn,
                         uint16_t        apn_len);


/**@brief Read APN based on Class.
 *
 * This function will read the APN referenced by a Class.
 *
 * @param[in]     apn_class     Class to read.
 * @param[out]    p_apn         Output buffer where to write the APN.
 * @param[inout]  p_apn_len     Length of the p_apn buffer as input, and length of the APN as
 *                              output.
 *
 * @retval 0            If read operation was successful.
 * @retval NRF_EIO      If operation was not successful due to internal error.
 * @retval NRF_ENOBUFS  If the operation could not be performed because it could not allocate
 *                      enough intermediate buffers to perform the operation.
 * @retval NRF_ENOENT   If there is no APN associated with the Class.
 * @retval NRF_EPERM    If the application did not have permission to do the operation.
 * @retval NRF_EINVAL   If provided buffer is to small for result data. If failing with this error,
 *                      the size needed is provided as output parameter by reference in the output
 *                      p_apn_len output parameter.
 */
int nrf_apn_class_read(uint8_t    apn_class,
                       uint8_t  * p_apn,
                       uint16_t * p_apn_len);
#endif // NRF_APN_CLASS_H__
/**@} */
