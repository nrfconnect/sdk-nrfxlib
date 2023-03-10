/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_cx_abstract_interface.h
 *
 * @defgroup mpsl_cx_abstract_interface MPSL protocol interface abstraction for coexistence module
 * @ingroup  mpsl_cx
 *
 * This module provides the interface abstraction for radio coexistence modules.
 * Interface abstraction allows application developer to select one of available implementations of
 * MPSL Coexistence protocol API.
 *
 * @{
 */

#ifndef MPSL_CX_ABSTRACT_INTERFACE_H_
#define MPSL_CX_ABSTRACT_INTERFACE_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "protocol/mpsl_cx_protocol_api.h"

/** @brief Set of pointers to functions in MPSL Coexistence interface.
 *
 * This structure defines a set of pointers to functions which correspond the Public Protocol API defined in the
 * @ref mpsl_cx_protocol_api.h file.
 */
typedef struct
{
    /** Pointer to a function called when @ref mpsl_cx_request is called */
    int32_t  (*p_request)(const mpsl_cx_request_t * p_req_params);
    /** Pointer to a function called when @ref mpsl_cx_release is called */
    int32_t  (*p_release)(void);
    /** Pointer to a function called when @ref mpsl_cx_granted_ops_get is called */
    int32_t  (*p_granted_ops_get)(mpsl_cx_op_map_t * p_granted_ops);
    /** Pointer to a function called when @ref mpsl_cx_req_grant_delay_get is called */
    uint32_t (*p_req_grant_delay_get)(void);
    /** Pointer to a function called when @ref mpsl_cx_register_callback is called */
    int32_t  (*p_register_callback)(mpsl_cx_cb_t cb);
} mpsl_cx_interface_t;

/** @brief Connects to one of Coexistence PTA client implementation
 *
 * @param[in] p_methods   Pointer to structure of pointers.
 * this must be valid for the lifetime of the application.
 *
 * @note This must be used by Coexistence implementation "constructor" and this is the only case
 * in which it can be used.
 *
 * @retval   0               The "link" was successfuly created.
 * @retval   -NRF_EPERM      Some error occured (e.g. null pointer was passed).
 */
int32_t mpsl_cx_interface_set(const mpsl_cx_interface_t * p_methods);

#endif /* MPSL_CX_ABSTRACT_INTERFACE_ */

/**
  @}
*/
