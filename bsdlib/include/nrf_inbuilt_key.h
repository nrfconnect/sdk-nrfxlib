/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file nrf_inbuilt_key.h
 *
 * @defgroup nrf_inbuilt_key nRF91 Inbuilt Key Management
 * @{
 */
#ifndef NRF_INBUILT_KEY_H__
#define NRF_INBUILT_KEY_H__

#include <stdint.h>
#include <stdbool.h>

#include "nrf_socket.h"
#include "nrf_key_mgmt.h"

/**@brief Provision new or update credential in persistent storage.
 *
 * This function will store the credential referenced to be stored persistently.
 * The credential can later be referenced for use or managed by nrf_inbuilt_key module
 * by using the application defined sec_tag.
 *
 * @param[in]  sec_tag        Application defined tag for this credential to be referred
 *                            to in setting up a BSD Secure Socket or to manage the credential
 *                            using nrf_key_mgmt module to read/delete/search the key.
 * @param[in]  cred_type      Type of credential being created and stored for later use.
 * @param[in]  p_buffer       Buffer containing the credential data.
 * @param[in]  buffer_len     Length of the buffer holding the credential data.
 *
 * @retval 0            If create operation was successful.
 * @retval NRF_EIO      If operation was not successful due to internal error or uninitialized
 *                      module.
 * @retval NRF_ENOBUFS  If the operation could not be performed because it could not allocate
 *                      enough intermediate buffers to perform the operation.
 * @retval NRF_ENOENT   If the sec_tag indicated cannot be written.
 * @retval NRF_ENOMEM   If there was not memory enough to store the credential data.
 * @retval NRF_EPERM    If the application did not have permission to do the operation.
 * @retval NRF_EACCES   If the operation could not be performed while modem is in active state.
 * @retval NRF_EINVAL   If one or more of the provided parameters are not valid.
 */
int nrf_inbuilt_key_write(nrf_sec_tag_t            sec_tag,
                          nrf_key_mgnt_cred_type_t cred_type,
                          uint8_t                * p_buffer,
                          uint16_t                 buffer_len);


/**@brief Read back a credential from persistent storage.
 *
 * This function will read the credential from persistent memory referenced by a sec_tag.
 *
 * @param[in]     sec_tag       Application defined tag for this credential to read.
 * @param[in]     cred_type     Type of credential being read.
 * @param[in]     p_buffer      Output buffer containing where to write the read credential data.
 * @param[inout]  p_buffer_len  Length of the output buffer as input, and length used as output
 *                              parameter.
 *
 * @retval 0            If read operation was successful.
 * @retval NRF_EIO      If operation was not successful due to internal error or uninitialized
 *                      module.
 * @retval NRF_ENOBUFS  If the operation could not be performed because it could not allocate
 *                      enough intermediate buffers to perform the operation.
 * @retval NRF_ENOENT   If there was no credential associated with the sec_tag and cred_type.
 * @retval NRF_EPERM    If the application did not have permission to do the operation.
 * @retval NRF_EACCES   If the operation could not be performed while modem is in active state.
 * @retval NRF_EINVAL   If provided buffer is to small for result data. If failing with this error,
 *                      the size needed is provided as output parameter by reference in the output
 *                      p_buffer_len output parameter.
 */
int nrf_inbuilt_key_read(nrf_sec_tag_t            sec_tag,
                         nrf_key_mgnt_cred_type_t cred_type,
                         uint8_t                * p_buffer,
                         uint16_t               * p_buffer_len);


/**@brief Delete a credential from persistent storage.
 *
 * This function deletes a stored credential from the persistent storage.
 *
 * @param[in]     sec_tag     Application defined tag to delete.
 * @param[in]     cred_type   Type of credential being deleted.
 *
 * @retval 0            If delete operation was successful.
 * @retval NRF_EIO      If operation was not successful due to internal errors or uninitialized
 *                      module.
 * @retval NRF_ENOBUFS  If the operation could not be performed because it could not allocate
 *                      enough intermediate buffers to perform the operation.
 * @retval NRF_ENOENT   If there was no credential associated with the sec_tag and cred_type.
 * @retval NRF_EPERM    If the application did not have permission to do the operation.
 * @retval NRF_EACCES   If the operation could not be performed while modem is in active state.
 */
int nrf_inbuilt_key_delete(nrf_sec_tag_t sec_tag, nrf_key_mgnt_cred_type_t cred_type);


/**@brief Set permission on a credential in persistent storage.
 *
 * This function will set permissions on the credential referenced in
 * persistently storage by a sec_tag and credential type.
 *
 * @param[in]  sec_tag        Application defined tag for this credential apply the permissions to.
 * @param[in]  cred_type      Type of credential to apply the permissions to.
 * @param[in]  perm_flags     Permission flags. Not yet implemented/supported.
 *
 * @retval 0            If create operation was successful.
 * @retval NRF_EIO      If operation was not successful due to internal errors or uninitialized
 *                      module.
 * @retval NRF_ENOBUFS  If the operation could not be performed because it could not allocate
 *                      enough intermediate buffers to perform the operation.
 * @retval NRF_ENOENT   If there was no credential associated with the sec_tag and cred_type.
 * @retval NRF_EPERM    If the application did not have permission to do the operation.
 * @retval NRF_EACCES   If the operation could not be performed while modem is in active state.
 */
int nrf_inbuilt_key_permission_set(nrf_sec_tag_t            sec_tag,
                                   nrf_key_mgnt_cred_type_t cred_type,
                                   uint8_t                  perm_flags);


/**@brief Check if credential exists in persistent storage.
 *
 * This function do a check whether the credential exists in the persistent storage.
 *
 * @param[in]     sec_tag       Application defined tag for search for.
 * @param[in]     cred_type     Type of credential being searched read.
 * @param[out]    p_exists      Value by reference output parameter telling the existence
 *                              of the credential. Only valid if operation was successful.
 * @param[out]    p_perm_flags  Value by reference output parameter telling the permission
 *                              flags of the credential. Only valid if operation was successful
 *                              and @param p_exists is true. Not yet implemented/supported.
 *
 * @retval 0            If delete operation was successful.
 * @retval NRF_EIO      If operation was not successful due to internal errors or uninitialized
 *                      module.
 * @retval NRF_ENOBUFS  If the operation could not be performed because it could not allocate
 *                      enough intermediate buffers to perform the operation.
 * @retval NRF_ENOENT   If there was no credential associated with the sec_tag and cred_type.
 * @retval NRF_EPERM    If the application did not have permission to do the operation.
 * @retval NRF_EACCES   If the operation could not be performed while modem is in active state.
 */
int nrf_inbuilt_key_exists(nrf_sec_tag_t            sec_tag,
                           nrf_key_mgnt_cred_type_t cred_type,
                           bool *                   p_exists,
                           uint8_t *                p_perm_flags);

#endif // NRF_INBUILT_KEY_H__
/**@} */
