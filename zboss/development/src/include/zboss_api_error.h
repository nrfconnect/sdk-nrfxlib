/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2022 DSR Corporation, Denver CO, USA.
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
/*  PURPOSE: ZBOSS error API header. Error handling API.
*/

#ifndef ZB_ZBOSS_API_ERROR_H
#define ZB_ZBOSS_API_ERROR_H 1

#define ZB_ERROR_SEVERITY_MINOR 0x01
#define ZB_ERROR_SEVERITY_MAJOR 0xFE
#define ZB_ERROR_SEVERITY_FATAL 0xFF

typedef struct zb_verify_additional_info_s {
  zb_uint16_t file_id;
  zb_int_t line_number;
  void *caller_additional_info;
} zb_verify_additional_info_t;

/**
 * @brief   Definition of error handler. It must be registered by application using
 *          @ref zb_error_register_app_handler .
 * 
 * @note    Severity @ref ZB_ERROR_SEVERITY_FATAL is excusive for errors raised by ZB_VERIFY
 *          or ZB_VERIFY_INFO
 * 
 * @note    If the registered callback is called with severity @ref ZB_ERROR_SEVERITY_FATAL ,
 *          the argument additional_info will point to a structure of type
 *          @ref zb_verify_additional_info_t , in which its parameter @ref caller_additional_info
 *          will point to the original additional info used by the caller in @ref  ZB_VERIFY_INFO
 *          (or NULL, in case of @ref ZB_VERIFY)
 * 
 * @param severity        severity level of the error
 * @param error_code      error code being raised
 * @param additional_info additional error-dependent info
 */
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
  Verify: Raise error and interrupt application

  @param file_id          source file id
  @param line_number      line in the source
  @param error_code       error code to be used
  @param additional_info  any additional error-dependent info
*/
ZB_NORETURN void zb_verify(zb_uint16_t file_id, zb_int_t line_number, zb_ret_t error_code, void *additional_info);
/**
   Check for expression in runtime and call zb_verify() if it is false.

  @param condition        expression to check
  @param error_code       error code to be used if the expression is false
  @param additional_info  any additional error-dependent info
*/
#define ZB_VERIFY_INFO(condition, error_code, additional_info) {if(!(condition)) { zb_verify(ZB_TRACE_FILE_ID, __LINE__, error_code, additional_info);} }

/**
  Check for expression in runtime and call zb_verify() if it is false.

  @param condition        expression to check
  @param error_code       error code to be used if the expression is false - @ref ERROR_CODE macro
*/
#define ZB_VERIFY(condition, error_code) ZB_VERIFY_INFO(condition, error_code, NULL)

#endif /* ZB_ZBOSS_API_ERROR_H */
