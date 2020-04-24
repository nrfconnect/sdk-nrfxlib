/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2020 DSR Corporation, Denver CO, USA.
 * http://www.dsr-zboss.com
 * http://www.dsr-corporation.com
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
/*  PURPOSE: Error codes
*/

#ifndef ZB_ERRORS_H
#define ZB_ERRORS_H

/**
 * @addtogroup base_types
 * @{
 */
/** @brief Return type for ZB functions returning execution status. @see enum zb_ret_e. */
typedef zb_int_t zb_ret_t;
/** @} */

/*! @cond internals_doc */

/* categories */

#define ERROR_CATEGORY_INTERVAL 256

enum zb_error_category_e
{
  ERROR_CATEGORY_GENERIC,
  ERROR_CATEGORY_SYSTEM,
  ERROR_CATEGORY_MAC,
  ERROR_CATEGORY_NWK,
  ERROR_CATEGORY_APS,
  ERROR_CATEGORY_ZDO,
  ERROR_CATEGORY_CBKE,
};

/* Let's always return 0 for RET_OK - in any category. */
#define ERROR_CODE(category, code) ((zb_ret_t)((code) != 0 ? -(((category) * ERROR_CATEGORY_INTERVAL) + (code)) : 0))
#define GENERIC_ERROR_CODE(code)   ERROR_CODE(ERROR_CATEGORY_GENERIC, code)
#define SYSTEM_ERROR_CODE(code)    ERROR_CODE(ERROR_CATEGORY_SYSTEM, code)

#define ERROR_GET_CATEGORY(err) (((-(zb_int_t)(err))) / ERROR_CATEGORY_INTERVAL)
#define ERROR_GET_CODE(err) ((-(zb_int_t)(err)) % ERROR_CATEGORY_INTERVAL)

/** @endcond */ /* internals_doc */

/**
 *  @addtogroup error_codes
 *  @{
 */

/**
 *  @brief Error codes for non-void stack functions.
 *  In general, function can return OK, BLOCKED or some error. Errors are negative.
 *
 *  Error can be "generic" or some additional error code.
 */
enum zb_ret_e
{
  /* Most common return types: ok, generic error, BLOCKED, thread exit indication. */
  RET_OK              = 0,
  RET_ERROR           = ERROR_CODE(ERROR_CATEGORY_GENERIC, 1), /* -1 indeed */
  RET_BLOCKED         = ERROR_CODE(ERROR_CATEGORY_GENERIC, 2),
  RET_EXIT            = ERROR_CODE(ERROR_CATEGORY_GENERIC, 3),
  RET_BUSY            = ERROR_CODE(ERROR_CATEGORY_GENERIC, 4),
  RET_EOF             = ERROR_CODE(ERROR_CATEGORY_GENERIC, 5),
  RET_OUT_OF_RANGE    = ERROR_CODE(ERROR_CATEGORY_GENERIC, 6),
  RET_EMPTY           = ERROR_CODE(ERROR_CATEGORY_GENERIC, 7),
  RET_CANCELLED       = ERROR_CODE(ERROR_CATEGORY_GENERIC, 8),


  RET_INVALID_PARAMETER_1                 = ERROR_CODE(ERROR_CATEGORY_GENERIC, 10),
  RET_INVALID_PARAMETER_2                 = ERROR_CODE(ERROR_CATEGORY_GENERIC, 11),
  RET_INVALID_PARAMETER_3                 = ERROR_CODE(ERROR_CATEGORY_GENERIC, 12),
  RET_INVALID_PARAMETER_4                 = ERROR_CODE(ERROR_CATEGORY_GENERIC, 13),
  RET_INVALID_PARAMETER_6                 = ERROR_CODE(ERROR_CATEGORY_GENERIC, 15),
  RET_PENDING                             = ERROR_CODE(ERROR_CATEGORY_GENERIC, 21),
  RET_NO_MEMORY                           = ERROR_CODE(ERROR_CATEGORY_GENERIC, 22),
  RET_INVALID_PARAMETER                   = ERROR_CODE(ERROR_CATEGORY_GENERIC, 23),
  RET_OPERATION_FAILED                    = ERROR_CODE(ERROR_CATEGORY_GENERIC, 24),
  RET_BUFFER_TOO_SMALL                    = ERROR_CODE(ERROR_CATEGORY_GENERIC, 25),
  RET_END_OF_LIST                         = ERROR_CODE(ERROR_CATEGORY_GENERIC, 26),
  RET_ALREADY_EXISTS                      = ERROR_CODE(ERROR_CATEGORY_GENERIC, 27),
  RET_NOT_FOUND                           = ERROR_CODE(ERROR_CATEGORY_GENERIC, 28),
  RET_OVERFLOW                            = ERROR_CODE(ERROR_CATEGORY_GENERIC, 29),
  RET_TIMEOUT                             = ERROR_CODE(ERROR_CATEGORY_GENERIC, 30),
  RET_NOT_IMPLEMENTED                     = ERROR_CODE(ERROR_CATEGORY_GENERIC, 31),
  RET_NO_RESOURCES                        = ERROR_CODE(ERROR_CATEGORY_GENERIC, 32),
  RET_UNINITIALIZED                       = ERROR_CODE(ERROR_CATEGORY_GENERIC, 33),
  RET_INVALID_STATE                       = ERROR_CODE(ERROR_CATEGORY_GENERIC, 35),
  RET_CONNECTION_FAILED                   = ERROR_CODE(ERROR_CATEGORY_GENERIC, 37),
  RET_CONNECTION_LOST                     = ERROR_CODE(ERROR_CATEGORY_GENERIC, 38),
  RET_UNAUTHORIZED                        = ERROR_CODE(ERROR_CATEGORY_GENERIC, 40),
  RET_CONFLICT                            = ERROR_CODE(ERROR_CATEGORY_GENERIC, 41),
  RET_INVALID_FORMAT                      = ERROR_CODE(ERROR_CATEGORY_GENERIC, 42),
  RET_NO_MATCH                            = ERROR_CODE(ERROR_CATEGORY_GENERIC, 43),
  RET_PROTOCOL_ERROR                      = ERROR_CODE(ERROR_CATEGORY_GENERIC, 44),
  RET_VERSION                             = ERROR_CODE(ERROR_CATEGORY_GENERIC, 45),
  RET_FILE_NOT_FOUND                      = ERROR_CODE(ERROR_CATEGORY_GENERIC, 48),
  RET_CONVERSION_ERROR                    = ERROR_CODE(ERROR_CATEGORY_GENERIC, 50),
  RET_FILE_CORRUPTED                      = ERROR_CODE(ERROR_CATEGORY_GENERIC, 56),
  RET_PAGE_NOT_FOUND                      = ERROR_CODE(ERROR_CATEGORY_GENERIC, 57),
  RET_ILLEGAL_REQUEST                     = ERROR_CODE(ERROR_CATEGORY_GENERIC, 62),
  RET_INVALID_GROUP                       = ERROR_CODE(ERROR_CATEGORY_GENERIC, 64),
  RET_TABLE_FULL                          = ERROR_CODE(ERROR_CATEGORY_GENERIC, 65),
  RET_IGNORE                              = ERROR_CODE(ERROR_CATEGORY_GENERIC, 69),
  RET_AGAIN                               = ERROR_CODE(ERROR_CATEGORY_GENERIC, 70),
  RET_DEVICE_NOT_FOUND                    = ERROR_CODE(ERROR_CATEGORY_GENERIC, 71),
  RET_OBSOLETE                            = ERROR_CODE(ERROR_CATEGORY_GENERIC, 72),
};

/** @} */ /* Error codes */

#endif /* ZB_ERRORS_H */
