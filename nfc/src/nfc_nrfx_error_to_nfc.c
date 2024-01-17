/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include "nrf_nfc_errno.h"
#include "nrfx.h"

/**@brief Translate nrfx nfct driver errors to nfc standard error codes.
 *        This function is required because nrfx drivers might return different
 *        error codes depending on build environment.
 *
 * @param[in] err returned nrfx error code.
 *
 * @retval Translated NFC error code.
 */
int nfc_nrfx_error_to_nfc(nrfx_err_t err)
{
    switch (err) {
    case NRFX_SUCCESS:
        return 0;
    case NRFX_ERROR_NO_MEM:
        return -NRF_ENOMEM;
    case NRFX_ERROR_NOT_SUPPORTED:
        return -NRF_EOPNOTSUPP;
    case NRFX_ERROR_INVALID_PARAM:
    case NRFX_ERROR_INVALID_LENGTH:
    case NRFX_ERROR_NULL:
        return -NRF_EINVAL;
    case NRFX_ERROR_INVALID_ADDR:
        return -NRF_EFAULT;
    case NRFX_ERROR_INVALID_STATE:
    case NRFX_ERROR_INTERNAL:
        return -NRF_EAGAIN;
    case NRFX_ERROR_TIMEOUT:
        return -NRF_ETIMEDOUT;
    case NRFX_ERROR_FORBIDDEN:
        return -NRF_EACCES;
    case NRFX_ERROR_BUSY:
        return -NRF_EALREADY;
    default:
        return -NRF_EFAULT;
    }
}
