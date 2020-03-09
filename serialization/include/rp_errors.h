/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef RP_ERRORS_H_
#define RP_ERRORS_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	RP_SUCCESS              = 0,
	RP_ERROR_NO_MEM         = 1,
	RP_ERROR_INVALID_PARAM  = 2,
	RP_ERROR_NULL           = 3,
	RP_ERROR_NOT_SUPPORTED  = 4,
	RP_ERROR_INTERNAL       = 5,
	RP_ERROR_OS_ERROR       = 6,
	RP_ERROR_INVALID_STATE  = 7,
	RP_ERROR_BUSY           = 8
} rp_err_t;

#ifdef __cplusplus
}
#endif

#endif /* RP_ERRORS_H_ */
