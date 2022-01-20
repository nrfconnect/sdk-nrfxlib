/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2021 DSR Corporation, Denver CO, USA.
 * www.dsr-zboss.com
 * www.dsr-corporation.com
 * All rights reserved.
 *
 *
 * Use in source and binary forms, redistribution in binary form only, with
 * or without modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 2. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 3. This software, with or without modification, must only be used with a Nordic
 *    Semiconductor ASA integrated circuit.
 *
 * 4. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/* PURPOSE: Error indication globals definition
*/

#ifndef ZB_ERROR_INDICATION_H
#define ZB_ERROR_INDICATION_H 1

#include "zb_types.h"

#include "zb_errors.h"

#define ZB_ERROR_SEVERITY_MINOR 0x01
#define ZB_ERROR_SEVERITY_FATAL 0xFF

typedef zb_bool_t (*zb_error_handler_t)(zb_uint8_t severity,
                                        zb_ret_t error_code,
                                        void *additional_info);

typedef struct zb_error_indication_ctx_s
{
  zb_error_handler_t app_err_handler;
} zb_error_indication_ctx_t;

/**
 * Set application error handler.
 *
 * @param cb - an error handler to setAddr
 */
void zb_error_register_app_handler(zb_error_handler_t cb);


/**
 * Raise an error. Use ZB_ERROR_RAISE macro instead of this function.
 *
 * @param severity - a severity level of the error
 * @param err_code - an error code (@see zb_ret_t), consists of an error category and code,
 *                   @see ERROR_CODE.
 * @param additional_info - any additional error-dependent info
 */
void zb_error_raise(zb_uint8_t severity, zb_ret_t err_code, void *additional_info);

#ifdef HAVE_TOP_LEVEL_ERROR_HANDLER
/**
 * Top-level handler which is called before an application handler
 * if HAVE_TOP_LEVEL_ERROR_HANDLER is defined.
 */
zb_bool_t zb_error_top_level_handler(zb_uint8_t severity, zb_ret_t err_code, void *additional_info);
#endif /* HAVE_TOP_LEVEL_ERROR_HANDLER */

#ifdef ZB_USE_ERROR_INDICATION
#define ZB_ERROR_RAISE(severity, err_code, additional_info) \
  zb_error_raise((severity), (err_code), (additional_info))
#else
#define ZB_ERROR_RAISE(severity, err_code, additional_info)
#endif /* ZB_USE_ERROR_INDICATION */

#endif  /* ZB_ERROR_INDICATION_H */
