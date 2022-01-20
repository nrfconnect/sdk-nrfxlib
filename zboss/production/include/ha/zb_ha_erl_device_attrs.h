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
/* PURPOSE: ERL Interface Device common definitions
*/

#if defined ZB_HA_DEFINE_DEVICE_ERL_INTERFACE_DEVICE || defined DOXYGEN

#include "zcl/zb_zcl_basic.h"
#include "zcl/zb_zcl_daily_schedule.h"
#include "zcl/zb_zcl_identify.h"
#include "zcl/zb_zcl_time.h"
#include "zcl/zb_zcl_meter_identification.h"
#include "zcl/zb_zcl_el_measurement.h"
#include "zcl/zb_zcl_diagnostics.h"
#include "zcl/zb_zcl_metering.h"
#include "zcl/zb_zcl_daily_schedule.h"

/** @cond DOXYGEN_ERL_SECTION */

/** @defgroup ZB_ERL_INTERFACE_DEVICE_BASIC_CLUSTER_DEFINITIONS Basic cluster definitions for ERL device
 *  @{
 */

/** @brief Declare attribute list for Basic cluster
 * @param[in] attr_list - attribute list variable name
 * @param[in] zcl_version - pointer to variable to store @ref ZB_ZCL_ATTR_BASIC_ZCL_VERSION_ID
 * @param[in] application_version - pointer to variable to store @ref ZB_ZCL_ATTR_BASIC_APPLICATION_VERSION_ID
 * @param[in] stack_version - pointer to variable to store @ref ZB_ZCL_ATTR_BASIC_STACK_VERSION_ID
 * @param[in] hw_version - pointer to variable to store @ref ZB_ZCL_ATTR_BASIC_HW_VERSION_ID
 * @param[in] manufacture_name - pointer to variable to store @ref ZB_ZCL_ATTR_BASIC_MANUFACTURER_NAME_ID
 * @param[in] model_identifier - pointer to variable to store @ref ZB_ZCL_ATTR_BASIC_MODEL_IDENTIFIER_ID
 * @param[in] date_code - pointer to variable to store @ref ZB_ZCL_ATTR_BASIC_DATE_CODE_ID
 * @param[in] power_source - pointer to variable to store @ref ZB_ZCL_ATTR_BASIC_POWER_SOURCE_ID
 */
#define ZB_ERL_DECLARE_BASIC_ATTRIB_LIST(attr_list, zcl_version, application_version, stack_version, \
                                      hw_version, manufacture_name, model_identifier, date_code,     \
                                      power_source)                                                  \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                                        \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_BASIC_ZCL_VERSION_ID, (zcl_version), ZB_ZCL_ATTR_TYPE_U8, ZB_ZCL_ATTR_ACCESS_READ_WRITE)                     \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_BASIC_APPLICATION_VERSION_ID, (application_version), ZB_ZCL_ATTR_TYPE_U8, ZB_ZCL_ATTR_ACCESS_READ_WRITE)     \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_BASIC_STACK_VERSION_ID, (stack_version), ZB_ZCL_ATTR_TYPE_U8, ZB_ZCL_ATTR_ACCESS_READ_WRITE)                 \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_BASIC_HW_VERSION_ID, (hw_version), ZB_ZCL_ATTR_TYPE_U8, ZB_ZCL_ATTR_ACCESS_READ_WRITE)                       \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_BASIC_MANUFACTURER_NAME_ID, (manufacture_name), ZB_ZCL_ATTR_TYPE_CHAR_STRING, ZB_ZCL_ATTR_ACCESS_READ_WRITE) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_BASIC_MODEL_IDENTIFIER_ID, (model_identifier), ZB_ZCL_ATTR_TYPE_CHAR_STRING, ZB_ZCL_ATTR_ACCESS_READ_WRITE)  \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_BASIC_DATE_CODE_ID, (date_code), ZB_ZCL_ATTR_TYPE_CHAR_STRING, ZB_ZCL_ATTR_ACCESS_READ_WRITE)                \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_BASIC_POWER_SOURCE_ID, (power_source), ZB_ZCL_ATTR_TYPE_8BIT_ENUM, ZB_ZCL_ATTR_ACCESS_READ_WRITE)            \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST


/**
 * ERL Interface device Basic cluster attributes
 */
typedef struct zb_erl_basic_server_attrs_s
{
  /** @copydoc ZB_ZCL_ATTR_BASIC_ZCL_VERSION_ID
   * @see ZB_ZCL_ATTR_BASIC_ZCL_VERSION_ID
   */
  zb_uint8_t zcl_version;
  /** @copydoc ZB_ZCL_ATTR_BASIC_APPLICATION_VERSION_ID
   * @see ZB_ZCL_ATTR_BASIC_APPLICATION_VERSION_ID
   */
  zb_uint8_t application_version;
  /** @copydoc ZB_ZCL_ATTR_BASIC_STACK_VERSION_ID
   * @see ZB_ZCL_ATTR_BASIC_STACK_VERSION_ID
   */
  zb_uint8_t stack_version;
  /** @copydoc ZB_ZCL_ATTR_BASIC_HW_VERSION_ID
   * @see ZB_ZCL_ATTR_BASIC_HW_VERSION_ID
   */
  zb_uint8_t hw_version;
  /** @copydoc ZB_ZCL_ATTR_BASIC_MANUFACTURER_NAME_ID
   * @see ZB_ZCL_ATTR_BASIC_MANUFACTURER_NAME_ID
   */
  zb_uint8_t manufacture_name[33];
  /** @copydoc ZB_ZCL_ATTR_BASIC_MODEL_IDENTIFIER_ID
   * @see ZB_ZCL_ATTR_BASIC_MODEL_IDENTIFIER_ID
   */
  zb_uint8_t model_identifier[33];
  /** @copydoc ZB_ZCL_ATTR_BASIC_DATE_CODE_ID
   * @see ZB_ZCL_ATTR_BASIC_DATE_CODE_ID
   */
  zb_uint8_t date_code[17];
  /** @copydoc ZB_ZCL_ATTR_BASIC_POWER_SOURCE_ID
   * @see ZB_ZCL_ATTR_BASIC_POWER_SOURCE_ID
   */
  zb_uint8_t power_source;
} zb_erl_basic_server_attrs_t;

/** Initialize @ref erl_basic_server_attrs_s Basic cluster's attributes */
#define ZB_ERL_BASIC_ATTR_LIST_INIT    \
  (zb_erl_basic_server_attrs_t)        \
  { .zcl_version = ZB_ZCL_BASIC_ZCL_VERSION_DEFAULT_VALUE,                 \
    .application_version = ZB_ZCL_BASIC_APPLICATION_VERSION_DEFAULT_VALUE, \
    .stack_version = ZB_ZCL_BASIC_STACK_VERSION_DEFAULT_VALUE,             \
    .hw_version = ZB_ZCL_BASIC_HW_VERSION_DEFAULT_VALUE,                   \
    .manufacture_name = ZB_ZCL_BASIC_MANUFACTURER_NAME_DEFAULT_VALUE,      \
    .model_identifier = ZB_ZCL_BASIC_MODEL_IDENTIFIER_DEFAULT_VALUE,       \
    .date_code = ZB_ZCL_BASIC_DATE_CODE_DEFAULT_VALUE,                     \
    .power_source = ZB_ZCL_BASIC_POWER_SOURCE_DEFAULT_VALUE}

/** @brief Declare attribute list for Basic cluster
 *  @param[in] attr_list - attribute list variable name
 *  @param[in] attrs - pointer to @ref erl_basic_server_attrs_s structure
 */
#define ZB_ERL_DECLARE_BASIC_ATTR_LIST(attr_list, attrs)                                             \
  ZB_ERL_DECLARE_BASIC_ATTRIB_LIST(attr_list, &attrs.zcl_version, &attrs.application_version,        \
                                   &attrs.stack_version, &attrs.hw_version, &attrs.manufacture_name, \
                                   &attrs.model_identifier, &attrs.date_code, &attrs.power_source)

 /** @} */ /* ZB_ERL_INTERFACE_DEVICE_BASIC_CLUSTER_DEFINITIONS */

/** @defgroup ZB_ERL_INTERFACE_DEVICE_IDENTIFY_CLUSTER_DEFINITIONS Basic cluster definitions for ERL device
 *  @{
 */

/**
 * ERL Interface device Identify cluster attributes
 */

/** @brief Declare attribute list for Identify cluster
 * @param[in] attr_list - attribute list variable name
 * @param[in] identify_time - pointer to variable to store @ref ZB_ZCL_ATTR_IDENTIFY_IDENTIFY_TIME_ID
 */
#define ZB_ERL_DECLARE_IDENTIFY_ATTRIB_LIST(attr_list, identify_time)        \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_IDENTIFY_IDENTIFY_TIME_ID, (identify_time), ZB_ZCL_ATTR_TYPE_U16, ZB_ZCL_ATTR_ACCESS_READ_WRITE)  \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

typedef struct zb_erl_identify_server_attrs_s
{
  /** @copydoc ZB_ZCL_ATTR_IDENTIFY_IDENTIFY_TIME_ID
   * @see ZB_ZCL_ATTR_IDENTIFY_IDENTIFY_TIME_ID
   */
  zb_uint16_t identify_time;
} zb_erl_identify_server_attrs_t;

/** Initialize @ref erl_identify_server_attrs_s Identify cluster's attributes */
#define ZB_ERL_IDENTIFY_ATTR_LIST_INIT             \
  (zb_erl_identify_server_attrs_t)              \
  { .identify_time = ZB_ZCL_IDENTIFY_IDENTIFY_TIME_DEFAULT_VALUE}


/** @brief Declare attribute list for Identify cluster
*  @param[in] attr_list - attribute list variable name
*  @param[in] attrs - pointer to @ref erl_identify_server_attrs_s structure
*/
#define ZB_ERL_DECLARE_IDENTIFY_ATTR_LIST(attr_list, attrs)  \
  ZB_ERL_DECLARE_IDENTIFY_ATTRIB_LIST(attr_list, &attrs.identify_time)

/** @} */ /* ZB_ERL_IDENTIFY_CLUSTER_DEFINITIONS */

/** @defgroup ZB_ERL_TIME_CLUSTER_DEFINITIONS Time cluster definitions for ERL device
 *  @{
 */

/** @brief Declare attribute list for Time cluster
 * @param[in] attr_list - attribute list variable name
 * @param[in] time - pointer to variable to store @ref ZB_ZCL_ATTR_TIME_TIME_ID
 * @param[in] time_status - pointer to variable to store @ref ZB_ZCL_ATTR_TIME_TIME_STATUS_ID
 * @param[in] time_zone - pointer to variable to store @ref ZB_ZCL_ATTR_TIME_TIME_ZONE_ID
 * @param[in] dst_start - pointer to variable to store @ref ZB_ZCL_ATTR_TIME_DST_START_ID
 * @param[in] dst_end - pointer to variable to store @ref ZB_ZCL_ATTR_TIME_DST_END_ID
 * @param[in] dst_shift - pointer to variable to store @ref ZB_ZCL_ATTR_TIME_DST_SHIFT_ID
 * @param[in] standard_time - pointer to variable to store @ref ZB_ZCL_ATTR_TIME_STANDARD_TIME_ID
 * @param[in] local_time - pointer to variable to store @ref ZB_ZCL_ATTR_TIME_LOCAL_TIME_ID
 * @param[in] last_set_time - pointer to variable to store @ref ZB_ZCL_ATTR_TIME_LAST_SET_TIME_ID
 * @param[in] valid_until_time - pointer to variable to store @ref ZB_ZCL_ATTR_TIME_VALID_UNTIL_TIME_ID
 */
#define ZB_ERL_DECLARE_TIME_ATTRIB_LIST(attr_list, time, time_status, time_zone,     \
                                     dst_start, dst_end, dst_shift, standard_time,   \
                                     local_time, last_set_time, valid_until_time)    \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                                    \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_TIME_TIME_ID, (time), ZB_ZCL_ATTR_TYPE_UTC_TIME, ZB_ZCL_ATTR_ACCESS_READ_WRITE) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_TIME_TIME_STATUS_ID, (time_status), ZB_ZCL_ATTR_TYPE_8BITMAP, ZB_ZCL_ATTR_ACCESS_READ_WRITE | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_TIME_TIME_ZONE_ID, (time_zone), ZB_ZCL_ATTR_TYPE_S32, ZB_ZCL_ATTR_ACCESS_READ_WRITE) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_TIME_DST_START_ID, (dst_start), ZB_ZCL_ATTR_TYPE_U32, ZB_ZCL_ATTR_ACCESS_READ_WRITE) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_TIME_DST_END_ID, (dst_end), ZB_ZCL_ATTR_TYPE_U32, ZB_ZCL_ATTR_ACCESS_READ_WRITE) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_TIME_DST_SHIFT_ID, (dst_shift), ZB_ZCL_ATTR_TYPE_S32, ZB_ZCL_ATTR_ACCESS_READ_WRITE) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_TIME_STANDARD_TIME_ID, (standard_time), ZB_ZCL_ATTR_TYPE_U32, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_TIME_LOCAL_TIME_ID, (local_time), ZB_ZCL_ATTR_TYPE_U32, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_TIME_LAST_SET_TIME_ID, (last_set_time), ZB_ZCL_ATTR_TYPE_UTC_TIME, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_TIME_VALID_UNTIL_TIME_ID, (valid_until_time), ZB_ZCL_ATTR_TYPE_UTC_TIME, ZB_ZCL_ATTR_ACCESS_READ_WRITE) \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST


#define ZB_ERL_TIME_REPORT_ATTR_COUNT 1

/**
 * Time cluster attributes
 */
typedef struct zb_erl_time_server_attrs_s
{
  /** @copydoc ZB_ZCL_ATTR_TIME_TIME_ID
   * @see ZB_ZCL_ATTR_TIME_TIME_ID
   */
  zb_uint32_t time;
  /** @copydoc ZB_ZCL_ATTR_TIME_TIME_STATUS_ID
   * @see ZB_ZCL_ATTR_TIME_TIME_STATUS_ID
   */
  zb_uint8_t time_status;
  /** @copydoc ZB_ZCL_ATTR_TIME_TIME_ZONE_ID
   * @see ZB_ZCL_ATTR_TIME_TIME_ZONE_ID
   */
  zb_int32_t time_zone;
  /** @copydoc ZB_ZCL_ATTR_TIME_DST_START_ID
   * @see ZB_ZCL_ATTR_TIME_DST_START_ID
   */
  zb_uint32_t dst_start;
  /** @copydoc ZB_ZCL_ATTR_TIME_DST_END_ID
   * @see ZB_ZCL_ATTR_TIME_DST_END_ID
   */
  zb_uint32_t dst_end;
  /** @copydoc ZB_ZCL_ATTR_TIME_DST_SHIFT_ID
   * @see ZB_ZCL_ATTR_TIME_DST_SHIFT_ID
   */
  zb_int32_t dst_shift;
  /** @copydoc ZB_ZCL_ATTR_TIME_STANDARD_TIME_ID
   * @see ZB_ZCL_ATTR_TIME_STANDARD_TIME_ID
   */
  zb_uint32_t standard_time;
  /** @copydoc ZB_ZCL_ATTR_TIME_LOCAL_TIME_ID
   * @see ZB_ZCL_ATTR_TIME_LOCAL_TIME_ID
   */
  zb_uint32_t local_time;
  /** @copydoc ZB_ZCL_ATTR_TIME_LAST_SET_TIME_ID
   * @see ZB_ZCL_ATTR_TIME_LAST_SET_TIME_ID
   */
  zb_uint32_t last_set_time;
  /** @copydoc ZB_ZCL_ATTR_TIME_VALID_UNTIL_TIME_ID
   * @see ZB_ZCL_ATTR_TIME_VALID_UNTIL_TIME_ID
   */
  zb_uint32_t valid_until_time;
} zb_erl_time_server_attrs_t;

/** Initialize @ref zb_erl_time_server_attrs_s Time cluster's attributes */
#define ZB_ERL_TIME_ATTR_LIST_INIT                                  \
  (zb_erl_time_server_attrs_t)                                      \
  { .time = ZB_ZCL_TIME_TIME_DEFAULT_VALUE,                         \
    .time_status = ZB_ZCL_TIME_TIME_STATUS_DEFAULT_VALUE,           \
    .time_zone = ZB_ZCL_TIME_TIME_ZONE_DEFAULT_VALUE,               \
    .dst_start = ZB_ZCL_TIME_DST_START_DEFAULT_VALUE,               \
    .dst_end = ZB_ZCL_TIME_DST_END_DEFAULT_VALUE,                   \
    .dst_shift = ZB_ZCL_TIME_DST_SHIFT_DEFAULT_VALUE,               \
    .standard_time = ZB_ZCL_TIME_STANDARD_TIME_DEFAULT_VALUE,       \
    .local_time = ZB_ZCL_TIME_LOCAL_TIME_DEFAULT_VALUE,             \
    .last_set_time = ZB_ZCL_TIME_LAST_SET_TIME_DEFAULT_VALUE,       \
    .valid_until_time = ZB_ZCL_TIME_VALID_UNTIL_TIME_DEFAULT_VALUE}


/** @brief Declare attribute list for Time cluster
*  @param[in] attr_list - attribute list variable name
*  @param[in] attrs - pointer to @ref zb_erl_time_server_attrs_s structure
*/
#define ZB_ERL_DECLARE_TIME_ATTR_LIST(attr_list, attrs)                                         \
  ZB_ERL_DECLARE_TIME_ATTRIB_LIST(attr_list, &attrs.time, &attrs.time_status, &attrs.time_zone, \
                                &attrs.dst_start, &attrs.dst_end, &attrs.dst_shift,             \
                                &attrs.standard_time, &attrs.local_time, &attrs.last_set_time,  \
                                &attrs.valid_until_time)


/** @} */ /* ZB_ERL_TIME_CLUSTER_DEFINITIONS */

/** @defgroup ZB_ERL_METER_IDENTIFICATION_CLUSTER_DEFINITIONS Meter Identification cluster definitions for ERL device
 *  @{
 */

/** @brief Declare attribute list for Meter Identification cluster
 *  @param[in] attr_list - attribute list variable name
 *  @param[in] company_name - pointer to variable to store @ref ZB_ZCl_ATTR_METER_IDENTIFICATION_COMPANY_NAME
 *  @param[in] meter_type_id - pointer to variable to store @ref ZB_ZCl_ATTR_METER_IDENTIFICATION_METER_TYPE_ID
 *  @param[in] data_quality_id - pointer to variable to store @ref ZB_ZCl_ATTR_METER_IDENTIFICATION_DATA_QUALITY_ID
 *  @param[in] model - pointer to variable to store @ref ZB_ZCl_ATTR_METER_IDENTIFICATION_MODEL
 *  @param[in] pod - pointer to variable to store @ref ZB_ZCl_ATTR_METER_IDENTIFICATION_POD
 *  @param[in] available_power - pointer to variable to store @ref ZB_ZCl_ATTR_METER_IDENTIFICATION_AVAILABLE_POWER
 *  @param[in] power_threshold - pointer to variable to store @ref ZB_ZCl_ATTR_METER_IDENTIFICATION_POWER_THRESHOLD
 *
 */
#define ZB_ERL_DECLARE_METER_IDENTIFICATION_ATTRIB_LIST(attr_list, company_name, meter_type_id, data_quality_id, \
                                                     model, pod, available_power, power_threshold)               \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                                                    \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCl_ATTR_METER_IDENTIFICATION_COMPANY_NAME, (company_name), ZB_ZCL_ATTR_TYPE_CHAR_STRING, ZB_ZCL_ATTR_ACCESS_READ_ONLY )                             \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCl_ATTR_METER_IDENTIFICATION_METER_TYPE_ID, (meter_type_id), ZB_ZCL_ATTR_TYPE_U16, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING)     \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCl_ATTR_METER_IDENTIFICATION_DATA_QUALITY_ID, (data_quality_id), ZB_ZCL_ATTR_TYPE_U16, ZB_ZCL_ATTR_ACCESS_READ_ONLY )                               \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCl_ATTR_METER_IDENTIFICATION_MODEL, (model), ZB_ZCL_ATTR_TYPE_ARRAY, ZB_ZCL_ATTR_ACCESS_READ_ONLY)                                                  \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCl_ATTR_METER_IDENTIFICATION_POD, (pod), ZB_ZCL_ATTR_TYPE_CHAR_STRING, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING)                 \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCl_ATTR_METER_IDENTIFICATION_AVAILABLE_POWER, (available_power), ZB_ZCL_ATTR_TYPE_S24, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCl_ATTR_METER_IDENTIFICATION_POWER_THRESHOLD, (power_threshold), ZB_ZCL_ATTR_TYPE_S24, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

#define ZB_ERL_METER_IDENTIFICATION_REPORT_ATTR_COUNT 4

/**
 * Meter Identification cluster attributes
 */
typedef struct zb_erl_meter_identification_server_attrs_s
{
  /** @copydoc ZB_ZCl_ATTR_METER_IDENTIFICATION_COMPANY_NAME
   * @see ZB_ZCl_ATTR_METER_IDENTIFICATION_COMPANY_NAME
   */
  zb_uint8_t company_name[1 + 16];
  /** @copydoc ZB_ZCl_ATTR_METER_IDENTIFICATION_METER_TYPE_ID
   * @see ZB_ZCl_ATTR_METER_IDENTIFICATION_METER_TYPE_ID
   */
  zb_uint16_t meter_type_id;
  /** @copydoc ZB_ZCl_ATTR_METER_IDENTIFICATION_DATA_QUALITY_ID
   * @see ZB_ZCl_ATTR_METER_IDENTIFICATION_DATA_QUALITY_ID
   */
  zb_uint16_t data_quality_id;
  /** @copydoc ZB_ZCl_ATTR_METER_IDENTIFICATION_MODEL
   * @see ZB_ZCl_ATTR_METER_IDENTIFICATION_MODEL
   */
  zb_uint8_t model[1 + 16];
  /** @copydoc ZB_ZCl_ATTR_METER_IDENTIFICATION_POD
   * @see ZB_ZCl_ATTR_METER_IDENTIFICATION_POD
   */
  zb_uint8_t pod[1 + 16];
  /** @copydoc ZB_ZCl_ATTR_METER_IDENTIFICATION_AVAILABLE_POWER
   * @see ZB_ZCl_ATTR_METER_IDENTIFICATION_AVAILABLE_POWER
   */
  zb_int24_t available_power;
  /** @copydoc ZB_ZCl_ATTR_METER_IDENTIFICATION_POWER_THRESHOLD
   * @see ZB_ZCl_ATTR_METER_IDENTIFICATION_POWER_THRESHOLD
   */
  zb_int24_t power_threshold;
} zb_erl_meter_identification_server_attrs_t;


/** @brief Declare attribute list for Meter Identification cluster
 *  @param[in] attr_list - attribute list variable name
 *  @param[in] attrs - pointer to @ref erl_meter_identification_server_attrs_s structure
 */
#define ZB_ERL_DECLARE_METER_IDENTIFICATION_ATTR_LIST(attr_list, attrs)                                  \
  ZB_ERL_DECLARE_METER_IDENTIFICATION_ATTRIB_LIST(attr_list, &attrs.company_name, &attrs.meter_type_id,  \
                                               &attrs.data_quality_id, &attrs.model, &attrs.pod,         \
                                               &attrs.available_power, &attrs.power_threshold)


/** @} */ /* ERL_METER_IDENTIFICATION_CLUSTER_DEFINITIONS */

/** @defgroup ZB_ERL_ELECTRICAL_MEASUREMENT_CLUSTER_DEFINITIONS Electrical Measurement cluster definitions for ERL device
 *  @{
 */

/** @brief Declare attribute list for Electrical Measurement cluster
 *  @param[in] attr_list - attribute list variable name
 *  @param[in] measurement_type - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_MEASUREMENT_TYPE_ID
 *  @param[in] total_active_power - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_TOTAL_ACTIVE_POWER_ID
 *  @param[in] total_apparent_power - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_TOTAL_APPARENT_POWER_ID
 *  @param[in] power_multiplier - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_POWER_MULTIPLIER_ID
 *  @param[in] power_divisor - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_POWER_DIVISOR_ID
 *  @param[in] rms_voltage - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_ID
 *  @param[in] rms_current - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_ID
 *  @param[in] active_power - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACTIVE_POWER_ID
 *  @param[in] apparent_power - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_APPARENT_POWER_ID
 *  @param[in] avrg_rmsvoltage_measur_period - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_AVERAGE_RMSVOLTAGE_MEASUREMENT_PERIOD_ID
 *  @param[in] acvoltage_multiplier - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACVOLTAGE_MULTIPLIER_ID
 *  @param[in] acvoltage_divisor - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACVOLTAGE_DIVISOR_ID
 *  @param[in] accurrent_multiplier - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACCURRENT_MULTIPLIER_ID
 *  @param[in] accurrent_divisor - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACCURRENT_DIVISOR_ID
 *  @param[in] acpower_multiplier - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACPOWER_MULTIPLIER_ID
 *  @param[in] acpower_divisor - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACPOWER_DIVISOR_ID
 *  @param[in] rms_voltage_phb - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_PHB_ID
 *  @param[in] rms_current_phb - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_PHB_ID
 *  @param[in] active_power_phb - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACTIVE_POWER_PHB_ID
 *  @param[in] apparent_power_phb - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_APPARENT_POWER_PHB_ID
 *  @param[in] avrg_rmsvoltage_measur_period_phb - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_AVERAGE_RMSVOLTAGE_MEASUREMENT_PERIOD_PHB_ID
 *  @param[in] rms_voltage_phc - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_PHC_ID
 *  @param[in] rms_current_phc - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_PHC_ID
 *  @param[in] active_power_phc - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACTIVE_POWER_PHC_ID
 *  @param[in] apparent_power_phc - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_APPARENT_POWER_PHC_ID
 *  @param[in] avrg_rmsvoltage_measur_period_phc - pointer to variable to store @ref ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_AVERAGE_RMSVOLTAGE_MEASUREMENT_PERIOD_PHC_ID
 */
#define ZB_ERL_DECLARE_EL_MEASUREMENT_ATTRIB_LIST(attr_list, measurement_type, total_active_power,          \
                                               total_apparent_power, power_multiplier, power_divisor,       \
                                               rms_voltage, rms_current,                                    \
                                               active_power, apparent_power, avrg_rmsvoltage_measur_period, \
                                               acvoltage_multiplier, acvoltage_divisor,                     \
                                               accurrent_multiplier, accurrent_divisor,                     \
                                               acpower_multiplier, acpower_divisor,                         \
                                               rms_voltage_phb, rms_current_phb, active_power_phb,          \
                                               apparent_power_phb, avrg_rmsvoltage_measur_period_phb,       \
                                               rms_voltage_phc, rms_current_phc, active_power_phc,          \
                                               apparent_power_phc, avrg_rmsvoltage_measur_period_phc)       \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                                               \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_MEASUREMENT_TYPE_ID, (measurement_type), ZB_ZCL_ATTR_TYPE_32BITMAP, ZB_ZCL_ATTR_ACCESS_READ_ONLY ) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_TOTAL_ACTIVE_POWER_ID, (total_active_power), ZB_ZCL_ATTR_TYPE_S32, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_TOTAL_APPARENT_POWER_ID, (total_apparent_power), ZB_ZCL_ATTR_TYPE_U32, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_POWER_MULTIPLIER_ID, (power_multiplier), ZB_ZCL_ATTR_TYPE_U32, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_POWER_DIVISOR_ID, (power_divisor), ZB_ZCL_ATTR_TYPE_U32, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_ID, (rms_voltage), ZB_ZCL_ATTR_TYPE_U16, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_ID, (rms_current), ZB_ZCL_ATTR_TYPE_U16, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACTIVE_POWER_ID, (active_power), ZB_ZCL_ATTR_TYPE_S16, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_APPARENT_POWER_ID, (apparent_power), ZB_ZCL_ATTR_TYPE_U16, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_AVERAGE_RMSVOLTAGE_MEASUREMENT_PERIOD_ID, (avrg_rmsvoltage_measur_period), ZB_ZCL_ATTR_TYPE_U16, ZB_ZCL_ATTR_ACCESS_READ_WRITE) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACVOLTAGE_MULTIPLIER_ID, (acvoltage_multiplier), ZB_ZCL_ATTR_TYPE_U16, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACVOLTAGE_DIVISOR_ID, (acvoltage_divisor), ZB_ZCL_ATTR_TYPE_U16, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACCURRENT_MULTIPLIER_ID, (accurrent_multiplier), ZB_ZCL_ATTR_TYPE_U16, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACCURRENT_DIVISOR_ID, (accurrent_divisor), ZB_ZCL_ATTR_TYPE_U16, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACPOWER_MULTIPLIER_ID, (acpower_multiplier), ZB_ZCL_ATTR_TYPE_U16, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACPOWER_DIVISOR_ID, (acpower_divisor), ZB_ZCL_ATTR_TYPE_U16, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_PHB_ID, (rms_voltage_phb), ZB_ZCL_ATTR_TYPE_U16, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_PHB_ID, (rms_current_phb), ZB_ZCL_ATTR_TYPE_U16, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACTIVE_POWER_PHB_ID, (active_power_phb), ZB_ZCL_ATTR_TYPE_S16, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_APPARENT_POWER_PHB_ID, (apparent_power_phb), ZB_ZCL_ATTR_TYPE_U16, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_AVERAGE_RMSVOLTAGE_MEASUREMENT_PERIOD_PHB_ID, (avrg_rmsvoltage_measur_period_phb), ZB_ZCL_ATTR_TYPE_U16, ZB_ZCL_ATTR_ACCESS_READ_WRITE) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_PHC_ID, (rms_voltage_phc), ZB_ZCL_ATTR_TYPE_U16, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_PHC_ID, (rms_current_phc), ZB_ZCL_ATTR_TYPE_U16, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACTIVE_POWER_PHC_ID, (active_power_phc),  ZB_ZCL_ATTR_TYPE_S16, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_APPARENT_POWER_PHC_ID, (apparent_power_phc), ZB_ZCL_ATTR_TYPE_U16, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_AVERAGE_RMSVOLTAGE_MEASUREMENT_PERIOD_PHC_ID, (avrg_rmsvoltage_measur_period_phc), ZB_ZCL_ATTR_TYPE_U16, ZB_ZCL_ATTR_ACCESS_READ_WRITE) \
ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

#define ZB_ERL_EL_MEASUREMENT_REPORT_ATTR_COUNT 14

/**
 * Electric Measurement cluster attributes
 */
typedef struct zb_erl_el_measurement_server_attrs_s
{
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_MEASUREMENT_TYPE_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_MEASUREMENT_TYPE_ID
   */
  zb_uint32_t measurement_type;
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_TOTAL_ACTIVE_POWER_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_TOTAL_ACTIVE_POWER_ID
   */
  zb_int32_t total_active_power;
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_TOTAL_APPARENT_POWER_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_TOTAL_APPARENT_POWER_ID
   */
  zb_uint32_t total_apparent_power;
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_POWER_MULTIPLIER_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_POWER_MULTIPLIER_ID
   */
  zb_uint32_t power_multiplier;
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_POWER_DIVISOR_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_POWER_DIVISOR_ID
   */
  zb_uint32_t power_divisor;
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_ID
   */
  zb_uint16_t rms_voltage;
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_ID
   */
  zb_uint16_t rms_current;
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACTIVE_POWER_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACTIVE_POWER_ID
   */
  zb_int16_t  active_power;
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_APPARENT_POWER_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_APPARENT_POWER_ID
   */
  zb_uint16_t apparent_power;
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_AVERAGE_RMSVOLTAGE_MEASUREMENT_PERIOD_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_AVERAGE_RMSVOLTAGE_MEASUREMENT_PERIOD_ID
   */
  zb_uint16_t avrg_rmsvoltage_measur_period;
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACVOLTAGE_MULTIPLIER_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACVOLTAGE_MULTIPLIER_ID
   */
  zb_uint16_t acvoltage_multiplier;
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACVOLTAGE_DIVISOR_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACVOLTAGE_DIVISOR_ID
   */
  zb_uint16_t acvoltage_divisor;
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACCURRENT_MULTIPLIER_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACCURRENT_MULTIPLIER_ID
   */
  zb_uint16_t accurrent_multiplier;
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACCURRENT_DIVISOR_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACCURRENT_DIVISOR_ID
   */
  zb_uint16_t accurrent_divisor;
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACPOWER_MULTIPLIER_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACPOWER_MULTIPLIER_ID
   */
  zb_uint16_t acpower_multiplier;
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACPOWER_DIVISOR_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACPOWER_DIVISOR_ID
   */
  zb_uint16_t acpower_divisor;
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_PHB_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_PHB_ID
   */
  zb_uint16_t rms_voltage_phb;
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_PHB_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_PHB_ID
   */
  zb_uint16_t rms_current_phb;
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACTIVE_POWER_PHB_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACTIVE_POWER_PHB_ID
   */
  zb_int16_t  active_power_phb;
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_APPARENT_POWER_PHB_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_APPARENT_POWER_PHB_ID
   */
  zb_uint16_t apparent_power_phb;
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_AVERAGE_RMSVOLTAGE_MEASUREMENT_PERIOD_PHB_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_AVERAGE_RMSVOLTAGE_MEASUREMENT_PERIOD_PHB_ID
   */
  zb_uint16_t avrg_rmsvoltage_measur_period_phb;
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_PHC_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_PHC_ID
   */
  zb_uint16_t rms_voltage_phc;
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_PHC_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_PHC_ID
   */
  zb_uint16_t rms_current_phc;
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACTIVE_POWER_PHC_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACTIVE_POWER_PHC_ID
   */
  zb_int16_t  active_power_phc;
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_APPARENT_POWER_PHC_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_APPARENT_POWER_PHC_ID
   */
  zb_uint16_t apparent_power_phc;
  /** @copydoc ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_AVERAGE_RMSVOLTAGE_MEASUREMENT_PERIOD_PHC_ID
   * @see ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_AVERAGE_RMSVOLTAGE_MEASUREMENT_PERIOD_PHC_ID
   */
  zb_uint16_t avrg_rmsvoltage_measur_period_phc;
} zb_erl_el_measurement_server_attrs_t;

/** Initialize @ref zb_erl_el_measurement_server_attrs_s
 * Electrical Measurement cluster's attributes
 */
#define ZB_ERL_EL_MEASUREMENT_ATTR_LIST_INIT \
        (zb_erl_el_measurement_server_attrs_t)     \
        { .measurement_type                  = ZB_ZCL_ELECTRICAL_MEASUREMENT_MEASUREMENT_TYPE_DEFAULT_VALUE,                          \
          .power_multiplier                  = ZB_ZCL_ELECTRICAL_MEASUREMENT_POWER_MULTIPLIER_DEFAULT_VALUE,                          \
          .power_divisor                     = ZB_ZCL_ELECTRICAL_MEASUREMENT_POWER_DIVISOR_DEFAULT_VALUE,                             \
          .rms_voltage                       = ZB_ZCL_ELECTRICAL_MEASUREMENT_RMS_VOLTAGE_MAX_DEFAULT_VALUE,                           \
          .rms_current                       = ZB_ZCL_ELECTRICAL_MEASUREMENT_RMS_CURRENT_MAX_DEFAULT_VALUE,                           \
          .active_power                      = ZB_ZCL_ELECTRICAL_MEASUREMENT_ACTIVE_POWER_MAX_DEFAULT_VALUE,                          \
          .apparent_power                    = ZB_ZCL_ELECTRICAL_MEASUREMENT_APPARENT_POWER_DEFAULT_VALUE,                            \
          .avrg_rmsvoltage_measur_period     = ZB_ZCL_ELECTRICAL_MEASUREMENT_AVERAGE_RMSVOLTAGE_MEASUREMENT_PERIOD_DEFAULT_VALUE,     \
          .acvoltage_multiplier              = ZB_ZCL_ELECTRICAL_MEASUREMENT_ACVOLTAGE_MULTIPLIER_DEFAULT_VALUE,                      \
          .acvoltage_divisor                 = ZB_ZCL_ELECTRICAL_MEASUREMENT_ACVOLTAGE_DIVISOR_DEFAULT_VALUE,                         \
          .accurrent_multiplier              = ZB_ZCL_ELECTRICAL_MEASUREMENT_ACCURRENT_MULTIPLIER_DEFAULT_VALUE,                      \
          .accurrent_divisor                 = ZB_ZCL_ELECTRICAL_MEASUREMENT_ACCURRENT_DIVISOR_DEFAULT_VALUE,                         \
          .acpower_multiplier                = ZB_ZCL_ELECTRICAL_MEASUREMENT_ACPOWER_MULTIPLIER_DEFAULT_VALUE,                        \
          .acpower_divisor                   = ZB_ZCL_ELECTRICAL_MEASUREMENT_ACPOWER_DIVISOR_DEFAULT_VALUE,                           \
          .rms_voltage_phb                   = ZB_ZCL_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_PHB_DEFAULT_VALUE,                            \
          .rms_current_phb                   = ZB_ZCL_ELECTRICAL_MEASUREMENT_RMSCURRENT_PHB_DEFAULT_VALUE,                            \
          .active_power_phb                  = ZB_ZCL_ELECTRICAL_MEASUREMENT_ACTIVE_POWER_PHB_DEFAULT_VALUE,                          \
          .apparent_power_phb                = ZB_ZCL_ELECTRICAL_MEASUREMENT_APPARENT_POWER_PHB_DEFAULT_VALUE,                        \
          .avrg_rmsvoltage_measur_period_phb = ZB_ZCL_ELECTRICAL_MEASUREMENT_AVERAGE_RMSVOLTAGE_MEASUREMENT_PERIOD_PHB_DEFAULT_VALUE, \
          .rms_voltage_phc                   = ZB_ZCL_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_PHC_DEFAULT_VALUE,                            \
          .rms_current_phc                   = ZB_ZCL_ELECTRICAL_MEASUREMENT_RMSCURRENT_PHC_DEFAULT_VALUE,                            \
          .active_power_phc                  = ZB_ZCL_ELECTRICAL_MEASUREMENT_ACTIVE_POWER_PHC_DEFAULT_VALUE,                          \
          .apparent_power_phc                = ZB_ZCL_ELECTRICAL_MEASUREMENT_APPARENT_POWER_PHC_DEFAULT_VALUE,                        \
          .avrg_rmsvoltage_measur_period_phc = ZB_ZCL_ELECTRICAL_MEASUREMENT_AVERAGE_RMSVOLTAGE_MEASUREMENT_PERIOD_PHC_DEFAULT_VALUE}


/** @brief Declare attribute list for Electrical Measurement cluster
 *  @param[in] attr_list - attribute list variable name
 *  @param[in] attrs - pointer to @ref erl_el_measurement_server_attrs_s structure
 */
#define ZB_ERL_DECLARE_EL_MEASUREMENT_ATTR_LIST(attr_list, attrs)  \
  ZB_ERL_DECLARE_EL_MEASUREMENT_ATTRIB_LIST(attr_list, &attrs.measurement_type, &attrs.total_active_power,                      \
                                   &attrs.total_apparent_power, &attrs.power_multiplier, &attrs.power_divisor,                  \
                                   &attrs.rms_voltage, &attrs.rms_current,                                                      \
                                   &attrs.active_power, &attrs.apparent_power, &attrs.avrg_rmsvoltage_measur_period,            \
                                   &attrs.acvoltage_multiplier, &attrs.acvoltage_divisor, &attrs.accurrent_multiplier,          \
                                   &attrs.accurrent_divisor, &attrs.acpower_multiplier, &attrs.acpower_divisor,                 \
                                   &attrs.rms_voltage_phb, &attrs.rms_current_phb, &attrs.active_power_phb,                     \
                                   &attrs.apparent_power_phb, &attrs.avrg_rmsvoltage_measur_period_phb, &attrs.rms_voltage_phc, \
                                   &attrs.rms_current_phc, &attrs.active_power_phc, &attrs.apparent_power_phc,                  \
                                   &attrs.avrg_rmsvoltage_measur_period_phc)

/** @} */ /* ZB_ERL_ELECTRICAL_MEASUREMENT_CLUSTER_DEFINITIONS */

/** @defgroup ZB_ERL_DIAGNOSTICS_CLUSTER_DEFINITIONS Diagnostics cluster definitions for ERL device
 *  @{
 */

/** @brief Declare attribute list for Diagnostics cluster
 * @param[in] attr_list - attribute list variable name
 * @param[in] number_of_resets - pointer to variable to store @ref ZB_ZCL_ATTR_DIAGNOSTICS_NUMBER_OF_RESETS_ID
 * @param[in] last_msg_lqi - pointer to variable to store @ref ZB_ZCL_ATTR_DIAGNOSTICS_LAST_LQI_ID
 * @param[in] last_msg_rssi - pointer to variable to store @ref ZB_ZCL_ATTR_DIAGNOSTICS_LAST_RSSI_ID
 */

#define ZB_ERL_DECLARE_DIAGNOSTICS_ATTRIB_LIST(attr_list, number_of_resets, last_msg_lqi, last_msg_rssi) \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                                         \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_DIAGNOSTICS_NUMBER_OF_RESETS_ID, (number_of_resets), ZB_ZCL_ATTR_TYPE_U16, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_DIAGNOSTICS_LAST_LQI_ID, (last_msg_lqi), ZB_ZCL_ATTR_TYPE_U8, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_DIAGNOSTICS_LAST_RSSI_ID, (last_msg_rssi), ZB_ZCL_ATTR_TYPE_U8, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/**
* Diagnostics cluster attributes
*/
typedef struct zb_erl_diagnostics_server_attrs_s
{
  /** @copydoc ZB_ZCL_ATTR_DIAGNOSTICS_NUMBER_OF_RESETS_ID
   * @see ZB_ZCL_ATTR_DIAGNOSTICS_NUMBER_OF_RESETS_ID
   */
  zb_uint16_t number_of_resets;
  /** @copydoc ZB_ZCL_ATTR_DIAGNOSTICS_LAST_LQI_ID
   * @see ZB_ZCL_ATTR_DIAGNOSTICS_LAST_LQI_ID
   */
  zb_uint8_t last_msg_lqi;
  /** @copydoc ZB_ZCL_ATTR_DIAGNOSTICS_LAST_RSSI_ID
   * @see ZB_ZCL_ATTR_DIAGNOSTICS_LAST_RSSI_ID
   */
  zb_uint8_t last_msg_rssi;

} zb_erl_diagnostics_server_attrs_t;

/** Initialize @ref erl_diagnostics_server_attrs_s Diagnostics cluster's attributes */
#define ZB_ERL_DIAGNOSTICS_ATTR_LIST_INIT                                \
  (zb_erl_diagnostics_server_attrs_t)                                    \
  { .numberOfResets = ZB_ZCL_DIAGNOSTICS_NUMBER_OF_RESETS_DEFAULT_VALUE, \
    .lastMessageLQI = ZB_ZCL_DIAGNOSTICS_LAST_LQI_DEFAULT_VALUE,         \
    .lastMessageRSSI = ZB_ZCL_DIAGNOSTICS_LAST_RSSI_DEFAULT_VALUE}


/** @brief Declare attribute list for Diagnostics cluster
* @param[in] attr_list - attribute list variable name
* @param[in] attrs - pointer to @ref erl_diagnostics_server_attrs_s structure
*/
#define ZB_ERL_DECLARE_DIAGNOSTICS_ATTR_LIST(attr_list, attrs)  \
  ZB_ERL_DECLARE_DIAGNOSTICS_ATTRIB_LIST(attr_list, &attrs.number_of_resets, &attrs.last_msg_lqi, &attrs.last_msg_rssi)

/** @} */ /* ZB_ERL_DIAGNOSTICS_CLUSTER_DEFINITIONS */

/** @defgroup ZB_ERL_METERING_CLUSTER_DEFINITIONS  Metering cluster definitions for ERL device
 *  @{
 */

/** @brief Declare attribute list for Metering cluster
 * @param[in] attr_list - attribute list variable name
 * @param[in] curr_summ_delivered - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_CURRENT_SUMMATION_DELIVERED_ID
 * @param[in] curr_summ_received - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_CURRENT_SUMMATION_RECEIVED_ID
 * @param[in] fast_poll_update_period - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_FAST_POLL_UPDATE_PERIOD_ID
 * @param[in] active_register_tier_delivered - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_ACTIVE_REGISTER_TIER_DELIVERED_ID
 * @param[in] number_of_tiers_in_use - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_NUMBER_OF_TIERS_IN_USE
 * @param[in] curr_tier1_summ_delivered - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_CURRENT_TIER1_SUMMATION_DELIVERED_ID
 * @param[in] curr_tier1_summ_received - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_CURRENT_TIER1_SUMMATION_RECEIVED_ID
 * @param[in] curr_tier2_summ_delivered - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_CURRENT_TIER2_SUMMATION_DELIVERED_ID
 * @param[in] curr_tier3_summ_delivered - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_CURRENT_TIER3_SUMMATION_DELIVERED_ID
 * @param[in] curr_tier4_summ_delivered - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_CURRENT_TIER4_SUMMATION_DELIVERED_ID
 * @param[in] curr_tier5_summ_delivered - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_CURRENT_TIER5_SUMMATION_DELIVERED_ID
 * @param[in] curr_tier6_summ_delivered - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_CURRENT_TIER6_SUMMATION_DELIVERED_ID
 * @param[in] curr_tier7_summ_delivered - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_CURRENT_TIER7_SUMMATION_DELIVERED_ID
 * @param[in] curr_tier8_summ_delivered - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_CURRENT_TIER8_SUMMATION_DELIVERED_ID
 * @param[in] curr_tier9_summ_delivered - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_CURRENT_TIER9_SUMMATION_DELIVERED_ID
 * @param[in] curr_tier10_summ_delivered - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_CURRENT_TIER10_SUMMATION_DELIVERED_ID
 * @param[in] status - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_STATUS_ID
 * @param[in] extended_status - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_EXTENDED_STATUS_ID
 * @param[in] curr_meter - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_CURRENT_METER_ID_ID
 * @param[in] service_disconnect_reason - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_SERVICE_DISCONNECT_REASON_ID
 * @param[in] linky_mode_of_operation - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_LINKY_MODE_OF_OPERATION_ID
 * @param[in] unit_of_measure - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_UNIT_OF_MEASURE_ID
 * @param[in] multiplier - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_MULTIPLIER_ID
 * @param[in] divisor - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_DIVISOR_ID
 * @param[in] summation_formatting - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_SUMMATION_FORMATTING_ID
 * @param[in] demand_format - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_DEMAND_FORMATTING_ID
 * @param[in] historical_consumption_format - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_HISTORICAL_CONSUMPTION_FORMATTING_ID
 * @param[in] device_type - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_METERING_DEVICE_TYPE_ID
 * @param[in] site - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_SITE_ID_ID
 * @param[in] meter_serial_number - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_METER_SERIAL_NUMBER_ID
 * @param[in] module_serial_number - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_MODULE_SERIAL_NUMBER_ID
 * @param[in] instantaneous_demand - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_INSTANTANEOUS_DEMAND_ID
 * @param[in] curr_day_max_demand_delivered - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_CURRENT_DAY_MAX_DEMAND_DELIVERED_ID
 * @param[in] curr_day_max_demand_delivered_time - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_CURRENT_DAY_MAX_DEMAND_DELIVERED_TIME_ID
 * @param[in] curr_day_max_demand_received - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_CURRENT_DAY_MAX_DEMAND_RECEIVED_ID
 * @param[in] curr_day_max_demand_received_time - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_CURRENT_DAY_MAX_DEMAND_RECEIVED_TIME_ID
 * @param[in] prev_day_max_demand_delivered - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_PREVIOUS_DAY_MAX_DEMAND_DELIVERED_ID
 * @param[in] prev_day_max_demand_delivered_time - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_PREVIOUS_DAY_MAX_DEMAND_DELIVERED_TIME_ID
 * @param[in] prev_day_max_demand_received - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_PREVIOUS_DAY_MAX_DEMAND_RECEIVED_ID
 * @param[in] prev_day_max_demand_received_time - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_PREVIOUS_DAY_MAX_DEMAND_RECEIVED_TIME_ID
 * @param[in] max_number_of_periods_delivered - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_MAX_NUMBER_OF_PERIODS_DELIVERED_ID
 * @param[in] curr_reactive_summ_q1 - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_CURRENT_REACTIVE_SUMMATION_Q1_ID
 * @param[in] curr_reactive_summ_q2 - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_CURRENT_REACTIVE_SUMMATION_Q2_ID
 * @param[in] curr_reactive_summ_q3 - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_CURRENT_REACTIVE_SUMMATION_Q3_ID
 * @param[in] curr_reactive_summ_q4 - pointer to variable to store @ref ZB_ZCL_ATTR_METERING_CURRENT_REACTIVE_SUMMATION_Q4_ID
 */
#define ZB_ERL_DECLARE_METERING_ATTRIB_LIST(attr_list, curr_summ_delivered, curr_summ_received, fast_poll_update_period,         \
                                                   active_register_tier_delivered, number_of_tiers_in_use, curr_tier1_summ_delivered, curr_tier1_summ_received, \
                                                   curr_tier2_summ_delivered, curr_tier3_summ_delivered, curr_tier4_summ_delivered,     \
                                                   curr_tier5_summ_delivered, curr_tier6_summ_delivered, curr_tier7_summ_delivered,     \
                                                   curr_tier8_summ_delivered, curr_tier9_summ_delivered, curr_tier10_summ_delivered,    \
                                                   status, extended_status, curr_meter, service_disconnect_reason,                      \
                                                   linky_mode_of_operation, unit_of_measure, multiplier, divisor,                       \
                                                   summation_formatting, demand_format, historical_consumption_format,                  \
                                                   device_type, site, meter_serial_number, module_serial_number,                        \
                                                   instantaneous_demand, curr_day_max_demand_delivered, curr_day_max_demand_delivered_time, \
                                                   curr_day_max_demand_received, curr_day_max_demand_received_time,                     \
                                                   prev_day_max_demand_delivered, prev_day_max_demand_delivered_time,                   \
                                                   prev_day_max_demand_received, prev_day_max_demand_received_time,                     \
                                                   max_number_of_periods_delivered,                                                     \
                                                   curr_reactive_summ_q1, curr_reactive_summ_q2, curr_reactive_summ_q3, curr_reactive_summ_q4) \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                                                                     \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_CURRENT_SUMMATION_DELIVERED_ID, (curr_summ_delivered), ZB_ZCL_ATTR_TYPE_U48, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_CURRENT_SUMMATION_RECEIVED_ID, (curr_summ_received), ZB_ZCL_ATTR_TYPE_U48, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_FAST_POLL_UPDATE_PERIOD_ID, (fast_poll_update_period), ZB_ZCL_ATTR_TYPE_U8, ZB_ZCL_ATTR_ACCESS_READ_ONLY ) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_ACTIVE_REGISTER_TIER_DELIVERED_ID, (active_register_tier_delivered), ZB_ZCL_ATTR_TYPE_8BIT_ENUM, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_NUMBER_OF_TIERS_IN_USE, (number_of_tiers_in_use), ZB_ZCL_ATTR_TYPE_U8, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_CURRENT_TIER1_SUMMATION_DELIVERED_ID, (curr_tier1_summ_delivered), ZB_ZCL_ATTR_TYPE_U48, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_CURRENT_TIER1_SUMMATION_RECEIVED_ID, (curr_tier1_summ_received), ZB_ZCL_ATTR_TYPE_U48, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_CURRENT_TIER2_SUMMATION_DELIVERED_ID, (curr_tier2_summ_delivered), ZB_ZCL_ATTR_TYPE_U48, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_CURRENT_TIER3_SUMMATION_DELIVERED_ID, (curr_tier3_summ_delivered), ZB_ZCL_ATTR_TYPE_U48, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_CURRENT_TIER4_SUMMATION_DELIVERED_ID, (curr_tier4_summ_delivered), ZB_ZCL_ATTR_TYPE_U48, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_CURRENT_TIER5_SUMMATION_DELIVERED_ID, (curr_tier5_summ_delivered), ZB_ZCL_ATTR_TYPE_U48, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_CURRENT_TIER6_SUMMATION_DELIVERED_ID, (curr_tier6_summ_delivered), ZB_ZCL_ATTR_TYPE_U48, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_CURRENT_TIER7_SUMMATION_DELIVERED_ID, (curr_tier7_summ_delivered), ZB_ZCL_ATTR_TYPE_U48, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_CURRENT_TIER8_SUMMATION_DELIVERED_ID, (curr_tier8_summ_delivered), ZB_ZCL_ATTR_TYPE_U48, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_CURRENT_TIER9_SUMMATION_DELIVERED_ID, (curr_tier9_summ_delivered), ZB_ZCL_ATTR_TYPE_U48, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_CURRENT_TIER10_SUMMATION_DELIVERED_ID, (curr_tier10_summ_delivered), ZB_ZCL_ATTR_TYPE_U48, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_STATUS_ID, (status), ZB_ZCL_ATTR_TYPE_8BITMAP, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_EXTENDED_STATUS_ID, (extended_status), ZB_ZCL_ATTR_TYPE_64BITMAP, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_CURRENT_METER_ID_ID, (curr_meter), ZB_ZCL_ATTR_TYPE_ARRAY, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_SERVICE_DISCONNECT_REASON_ID, (service_disconnect_reason), ZB_ZCL_ATTR_TYPE_8BIT_ENUM, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_LINKY_MODE_OF_OPERATION_ID, (linky_mode_of_operation), ZB_ZCL_ATTR_TYPE_8BITMAP, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_UNIT_OF_MEASURE_ID, (unit_of_measure), ZB_ZCL_ATTR_TYPE_8BIT_ENUM, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_MULTIPLIER_ID, (multiplier), ZB_ZCL_ATTR_TYPE_U24, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_DIVISOR_ID, (divisor), ZB_ZCL_ATTR_TYPE_U24, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_SUMMATION_FORMATTING_ID, (summation_formatting), ZB_ZCL_ATTR_TYPE_8BITMAP,ZB_ZCL_ATTR_ACCESS_READ_ONLY ) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_DEMAND_FORMATTING_ID, (demand_format), ZB_ZCL_ATTR_TYPE_8BITMAP, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_HISTORICAL_CONSUMPTION_FORMATTING_ID, (historical_consumption_format), ZB_ZCL_ATTR_TYPE_8BITMAP, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_METERING_DEVICE_TYPE_ID, (device_type), ZB_ZCL_ATTR_TYPE_8BITMAP, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_SITE_ID_ID, (site), ZB_ZCL_ATTR_TYPE_ARRAY, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_METER_SERIAL_NUMBER_ID, (meter_serial_number), ZB_ZCL_ATTR_TYPE_ARRAY, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_MODULE_SERIAL_NUMBER_ID, (module_serial_number), ZB_ZCL_ATTR_TYPE_ARRAY, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_INSTANTANEOUS_DEMAND_ID, (instantaneous_demand), ZB_ZCL_ATTR_TYPE_S24, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_CURRENT_DAY_MAX_DEMAND_DELIVERED_ID, (curr_day_max_demand_delivered), ZB_ZCL_ATTR_TYPE_U48, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_CURRENT_DAY_MAX_DEMAND_DELIVERED_TIME_ID, (curr_day_max_demand_delivered_time), ZB_ZCL_ATTR_TYPE_UTC_TIME, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_CURRENT_DAY_MAX_DEMAND_RECEIVED_ID, (curr_day_max_demand_received), ZB_ZCL_ATTR_TYPE_U48, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_CURRENT_DAY_MAX_DEMAND_RECEIVED_TIME_ID, (curr_day_max_demand_received_time), ZB_ZCL_ATTR_TYPE_UTC_TIME, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_PREVIOUS_DAY_MAX_DEMAND_DELIVERED_ID, (prev_day_max_demand_delivered), ZB_ZCL_ATTR_TYPE_U48, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_PREVIOUS_DAY_MAX_DEMAND_DELIVERED_TIME_ID, (prev_day_max_demand_delivered_time), ZB_ZCL_ATTR_TYPE_UTC_TIME, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_PREVIOUS_DAY_MAX_DEMAND_RECEIVED_ID, (prev_day_max_demand_received), ZB_ZCL_ATTR_TYPE_U48, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_PREVIOUS_DAY_MAX_DEMAND_RECEIVED_TIME_ID, (prev_day_max_demand_received_time), ZB_ZCL_ATTR_TYPE_UTC_TIME, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_MAX_NUMBER_OF_PERIODS_DELIVERED_ID, (max_number_of_periods_delivered), ZB_ZCL_ATTR_TYPE_U8, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_CURRENT_REACTIVE_SUMMATION_Q1_ID, (curr_reactive_summ_q1), ZB_ZCL_ATTR_TYPE_U48, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_CURRENT_REACTIVE_SUMMATION_Q2_ID, (curr_reactive_summ_q2), ZB_ZCL_ATTR_TYPE_U48, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_CURRENT_REACTIVE_SUMMATION_Q3_ID, (curr_reactive_summ_q3), ZB_ZCL_ATTR_TYPE_U48, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_METERING_CURRENT_REACTIVE_SUMMATION_Q4_ID, (curr_reactive_summ_q4), ZB_ZCL_ATTR_TYPE_U48, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

#define ZB_ERL_METERING_REPORT_ATTR_COUNT 25

#define ZB_ZCL_ATTR_METERING_CURRENT_METER_ID_MAX_LENGTH 20

/**
 * Metering cluster attributes
 */

/* SummationFormatting: XXXXXX.Y, do not suppress leading zeroes */
#define ZB_ERL_INTERFACE_DEV_SUMM_FMT_LEFT 8
#define ZB_ERL_INTERFACE_DEV_SUMM_FMT_RIGHT 3
#define ZB_ERL_INTERFACE_DEV_SUMM_FMT_SUPPR 0
#define ZB_ERL_INTERFACE_DEV_SUMMATION_FORMATTING \
  ZB_ZCL_METERING_FORMATTING_SET(ZB_ERL_INTERFACE_DEV_SUMM_FMT_SUPPR, \
                                 ZB_ERL_INTERFACE_DEV_SUMM_FMT_LEFT, \
                                 ZB_ERL_INTERFACE_DEV_SUMM_FMT_RIGHT)

#define ZB_ZCL_ATTR_METERING_NUMBER_OF_TIRES_IN_USE_DEFAULT 0

typedef struct zb_erl_metering_server_attrs_s
{
  /** @copydoc ZB_ZCL_ATTR_METERING_CURRENT_SUMMATION_DELIVERED_ID
   * @see ZB_ZCL_ATTR_METERING_CURRENT_SUMMATION_DELIVERED_ID
   */
  zb_uint48_t curr_summ_delivered;
  /** @copydoc ZB_ZCL_ATTR_METERING_CURRENT_SUMMATION_RECEIVED_ID
   * @see ZB_ZCL_ATTR_METERING_CURRENT_SUMMATION_RECEIVED_ID
   */
  zb_uint48_t curr_summ_received;
  /** @copydoc ZB_ZCL_ATTR_METERING_FAST_POLL_UPDATE_PERIOD_ID
   * @see ZB_ZCL_ATTR_METERING_FAST_POLL_UPDATE_PERIOD_ID
   */
  zb_uint8_t fast_poll_update_period;
  /** @copydoc ZB_ZCL_ATTR_METERING_ACTIVE_REGISTER_TIER_DELIVERED_ID
   * @see ZB_ZCL_ATTR_METERING_ACTIVE_REGISTER_TIER_DELIVERED_ID
   */
  zb_uint8_t active_register_tier_delivered;
  /** @copydoc ZB_ZCL_ATTR_METERING_NUMBER_OF_TIERS_IN_USE
   * @see ZB_ZCL_ATTR_METERING_NUMBER_OF_TIERS_IN_USE
   */
  zb_uint8_t  number_of_tiers_in_use;
  /** @copydoc ZB_ZCL_ATTR_METERING_CURRENT_TIER1_SUMMATION_DELIVERED_ID
   * @see ZB_ZCL_ATTR_METERING_CURRENT_TIER1_SUMMATION_DELIVERED_ID
   */
  zb_uint48_t curr_tier1_summ_delivered;
  /** @copydoc ZB_ZCL_ATTR_METERING_CURRENT_TIER1_SUMMATION_RECEIVED_ID
   * @see ZB_ZCL_ATTR_METERING_CURRENT_TIER1_SUMMATION_RECEIVED_ID
   */
  zb_uint48_t curr_tier1_summ_received;
  /** @copydoc ZB_ZCL_ATTR_METERING_CURRENT_TIER2_SUMMATION_DELIVERED_ID
   * @see ZB_ZCL_ATTR_METERING_CURRENT_TIER2_SUMMATION_DELIVERED_ID
   */
  zb_uint48_t curr_tier2_summ_delivered;
  /** @copydoc ZB_ZCL_ATTR_METERING_CURRENT_TIER3_SUMMATION_DELIVERED_ID
   * @see ZB_ZCL_ATTR_METERING_CURRENT_TIER3_SUMMATION_DELIVERED_ID
   */
  zb_uint48_t curr_tier3_summ_delivered;
  /** @copydoc ZB_ZCL_ATTR_METERING_CURRENT_TIER4_SUMMATION_DELIVERED_ID
   * @see ZB_ZCL_ATTR_METERING_CURRENT_TIER4_SUMMATION_DELIVERED_ID
   */
  zb_uint48_t curr_tier4_summ_delivered;
  /** @copydoc ZB_ZCL_ATTR_METERING_CURRENT_TIER5_SUMMATION_DELIVERED_ID
   * @see ZB_ZCL_ATTR_METERING_CURRENT_TIER5_SUMMATION_DELIVERED_ID
   */
  zb_uint48_t curr_tier5_summ_delivered;
  /** @copydoc ZB_ZCL_ATTR_METERING_CURRENT_TIER6_SUMMATION_DELIVERED_ID
   * @see ZB_ZCL_ATTR_METERING_CURRENT_TIER6_SUMMATION_DELIVERED_ID
   */
  zb_uint48_t curr_tier6_summ_delivered;
  /** @copydoc ZB_ZCL_ATTR_METERING_CURRENT_TIER7_SUMMATION_DELIVERED_ID
   * @see ZB_ZCL_ATTR_METERING_CURRENT_TIER7_SUMMATION_DELIVERED_ID
   */
  zb_uint48_t curr_tier7_summ_delivered;
  /** @copydoc ZB_ZCL_ATTR_METERING_CURRENT_TIER8_SUMMATION_DELIVERED_ID
   * @see ZB_ZCL_ATTR_METERING_CURRENT_TIER8_SUMMATION_DELIVERED_ID
   */
  zb_uint48_t curr_tier8_summ_delivered;
  /** @copydoc ZB_ZCL_ATTR_METERING_CURRENT_TIER9_SUMMATION_DELIVERED_ID
   * @see ZB_ZCL_ATTR_METERING_CURRENT_TIER9_SUMMATION_DELIVERED_ID
   */
  zb_uint48_t curr_tier9_summ_delivered;
  /** @copydoc ZB_ZCL_ATTR_METERING_CURRENT_TIER10_SUMMATION_DELIVERED_ID
   * @see ZB_ZCL_ATTR_METERING_CURRENT_TIER10_SUMMATION_DELIVERED_ID
   */
  zb_uint48_t curr_tier10_summ_delivered;
  /** @copydoc ZB_ZCL_ATTR_METERING_STATUS_ID
   * @see ZB_ZCL_ATTR_METERING_STATUS_ID
   */
  zb_uint8_t status;
  /** @copydoc ZB_ZCL_ATTR_METERING_EXTENDED_STATUS_ID
   * @see ZB_ZCL_ATTR_METERING_EXTENDED_STATUS_ID
   */
  zb_uint64_t extended_status;
  /** @copydoc ZB_ZCL_ATTR_METERING_CURRENT_METER_ID_ID
   * @see ZB_ZCL_ATTR_METERING_CURRENT_METER_ID_ID
   */
  zb_uint8_t curr_meter_id[ZB_ZCL_ATTR_METERING_CURRENT_METER_ID_MAX_LENGTH];
  /** @copydoc ZB_ZCL_ATTR_METERING_SERVICE_DISCONNECT_REASON_ID
   * @see ZB_ZCL_ATTR_METERING_SERVICE_DISCONNECT_REASON_ID
   */
  zb_uint8_t service_disconnect_reason;
  /** @copydoc ZB_ZCL_ATTR_METERING_LINKY_MODE_OF_OPERATION_ID
   * @see ZB_ZCL_ATTR_METERING_LINKY_MODE_OF_OPERATION_ID
   */
  zb_uint8_t linky_mode_of_operation;
  /** @copydoc ZB_ZCL_ATTR_METERING_UNIT_OF_MEASURE_ID
   * @see ZB_ZCL_ATTR_METERING_UNIT_OF_MEASURE_ID
   */
  zb_uint8_t unit_of_measure;
  /** @copydoc ZB_ZCL_ATTR_METERING_MULTIPLIER_ID
   * @see ZB_ZCL_ATTR_METERING_MULTIPLIER_ID
   */
  zb_uint24_t multiplier;
  /** @copydoc ZB_ZCL_ATTR_METERING_DIVISOR_ID
   * @see ZB_ZCL_ATTR_METERING_DIVISOR_ID
   */
  zb_uint24_t divisor;
  /** @copydoc ZB_ZCL_ATTR_METERING_SUMMATION_FORMATTING_ID
   * @see ZB_ZCL_ATTR_METERING_SUMMATION_FORMATTING_ID
   */
  zb_uint8_t summation_formatting;
  /** @copydoc ZB_ZCL_ATTR_METERING_DEMAND_FORMATTING_ID
   * @see ZB_ZCL_ATTR_METERING_DEMAND_FORMATTING_ID
   */
  zb_uint8_t demand_formatting;
  /** @copydoc ZB_ZCL_ATTR_METERING_HISTORICAL_CONSUMPTION_FORMATTING_ID
   * @see ZB_ZCL_ATTR_METERING_HISTORICAL_CONSUMPTION_FORMATTING_ID
   */
  zb_uint8_t historical_consumption_formatting;
  /** @copydoc ZB_ZCL_ATTR_METERING_METERING_DEVICE_TYPE_ID
   * @see ZB_ZCL_ATTR_METERING_METERING_DEVICE_TYPE_ID
   */
  zb_uint8_t device_type;
  /** @copydoc ZB_ZCL_ATTR_METERING_SITE_ID_ID
   * @see ZB_ZCL_ATTR_METERING_SITE_ID_ID
   */
  zb_uint8_t site_id[1 + 32];
  /** @copydoc ZB_ZCL_ATTR_METERING_METER_SERIAL_NUMBER_ID
   * @see ZB_ZCL_ATTR_METERING_METER_SERIAL_NUMBER_ID
   */
  zb_uint8_t meter_serial_number[1 + 24];
  /** @copydoc ZB_ZCL_ATTR_METERING_MODULE_SERIAL_NUMBER_ID
   * @see ZB_ZCL_ATTR_METERING_MODULE_SERIAL_NUMBER_ID
   */
  zb_uint8_t module_serial_number[1 + 24];
  /** @copydoc ZB_ZCL_ATTR_METERING_INSTANTANEOUS_DEMAND_ID
   * @see ZB_ZCL_ATTR_METERING_INSTANTANEOUS_DEMAND_ID
   */
  zb_int24_t instantaneous_demand;
  /** @copydoc ZB_ZCL_ATTR_METERING_CURRENT_DAY_MAX_DEMAND_DELIVERED_ID
   * @see ZB_ZCL_ATTR_METERING_CURRENT_DAY_MAX_DEMAND_DELIVERED_ID
   */
  zb_uint48_t curr_day_max_demand_delivered;
  /** @copydoc ZB_ZCL_ATTR_METERING_CURRENT_DAY_MAX_DEMAND_DELIVERED_TIME_ID
   * @see ZB_ZCL_ATTR_METERING_CURRENT_DAY_MAX_DEMAND_DELIVERED_TIME_ID
   */
  zb_uint32_t curr_day_max_demand_delivered_time;
  /** @copydoc ZB_ZCL_ATTR_METERING_CURRENT_DAY_MAX_DEMAND_RECEIVED_ID
   * @see ZB_ZCL_ATTR_METERING_CURRENT_DAY_MAX_DEMAND_RECEIVED_ID
   */
  zb_uint48_t curr_day_max_demand_received;
  /** @copydoc ZB_ZCL_ATTR_METERING_CURRENT_DAY_MAX_DEMAND_RECEIVED_TIME_ID
   * @see ZB_ZCL_ATTR_METERING_CURRENT_DAY_MAX_DEMAND_RECEIVED_TIME_ID
   */
  zb_uint32_t curr_day_max_demand_received_time;
  /** @copydoc ZB_ZCL_ATTR_METERING_PREVIOUS_DAY_MAX_DEMAND_DELIVERED_ID
   * @see ZB_ZCL_ATTR_METERING_PREVIOUS_DAY_MAX_DEMAND_DELIVERED_ID
   */
  zb_uint48_t prev_day_max_demand_delivered;
  /** @copydoc ZB_ZCL_ATTR_METERING_PREVIOUS_DAY_MAX_DEMAND_DELIVERED_TIME_ID
   * @see ZB_ZCL_ATTR_METERING_PREVIOUS_DAY_MAX_DEMAND_DELIVERED_TIME_ID
   */
  zb_uint32_t prev_day_max_demand_delivered_time;
  /** @copydoc ZB_ZCL_ATTR_METERING_PREVIOUS_DAY_MAX_DEMAND_RECEIVED_ID
   * @see ZB_ZCL_ATTR_METERING_PREVIOUS_DAY_MAX_DEMAND_RECEIVED_ID
   */
  zb_uint48_t prev_day_max_demand_received;
  /** @copydoc ZB_ZCL_ATTR_METERING_PREVIOUS_DAY_MAX_DEMAND_RECEIVED_TIME_ID
   * @see ZB_ZCL_ATTR_METERING_PREVIOUS_DAY_MAX_DEMAND_RECEIVED_TIME_ID
   */
  zb_uint32_t prev_day_max_demand_received_time;
  /** @copydoc ZB_ZCL_ATTR_METERING_MAX_NUMBER_OF_PERIODS_DELIVERED_ID
   * @see ZB_ZCL_ATTR_METERING_MAX_NUMBER_OF_PERIODS_DELIVERED_ID
   */
  zb_uint8_t max_number_of_periods_delivered;
  /** @copydoc ZB_ZCL_ATTR_METERING_CURRENT_REACTIVE_SUMMATION_Q1_ID
   * @see ZB_ZCL_ATTR_METERING_CURRENT_REACTIVE_SUMMATION_Q1_ID
   */
  zb_uint48_t curr_reactive_summ_q1;
  /** @copydoc ZB_ZCL_ATTR_METERING_CURRENT_REACTIVE_SUMMATION_Q2_ID
   * @see ZB_ZCL_ATTR_METERING_CURRENT_REACTIVE_SUMMATION_Q2_ID
   */
  zb_uint48_t curr_reactive_summ_q2;
  /** @copydoc ZB_ZCL_ATTR_METERING_CURRENT_REACTIVE_SUMMATION_Q3_ID
   * @see ZB_ZCL_ATTR_METERING_CURRENT_REACTIVE_SUMMATION_Q3_ID
   */
  zb_uint48_t curr_reactive_summ_q3;
  /** @copydoc ZB_ZCL_ATTR_METERING_CURRENT_REACTIVE_SUMMATION_Q4_ID
   * @see ZB_ZCL_ATTR_METERING_CURRENT_REACTIVE_SUMMATION_Q4_ID
   */
  zb_uint48_t curr_reactive_summ_q4;
}zb_erl_metering_server_attrs_t;

/** Initialize @ref zb_erl_metering_server_attrs_s Metering cluster's attributes */
#define ZB_ERL_METERING_ATTR_LIST_INIT  (zb_erl_metering_server_attrs_t)                                   \
  {.status                               = ZB_ZCL_METERING_STATUS_DEFAULT_VALUE,                           \
   .linky_mode_of_operation              = ZB_ZCL_METERING_LINKY_MODE_ON_OPERATION_DEFAULT_VALUE,          \
   .unit_of_measure                      = ZB_ZCL_METERING_UNIT_OF_MEASURE_DEFAULT_VALUE,                  \
   .instantaneous_demand                 = ZB_ZCL_METERING_INSTANTANEOUS_DEMAND_DEFAULT_VALUE,             \
   .max_number_of_periods_delivered      = ZB_ZCL_METERING_MAX_NUMBER_OF_PERIODS_DELIVERED_DEFAULT_VALUE,  \
   .summation_formatting                 = ZB_ERL_INTERFACE_DEV_SUMMATION_FORMATTING,                      \
   .device_type                          = ZB_ZCL_METERING_ELECTRIC_METERING,                              \
   .fast_poll_update_period              = ZB_ZCL_METERING_FAST_POLL_UPDATE_PERIOD_DEFAULT_VALUE,          \
   .number_of_tiers_in_use               = ZB_ZCL_ATTR_METERING_NUMBER_OF_TIRES_IN_USE_DEFAULT}

/** @brief Declare attribute list for Metering cluster
 *  @param[in] attr_list - attribute list variable name
 *  @param[in] attrs - pointer to @ref erl_metering_server_attrs_s structure
 */
#define ZB_ERL_DECLARE_METERING_ATTR_LIST(attr_list, attrs)  \
  ZB_ERL_DECLARE_METERING_ATTRIB_LIST(attr_list, &attrs.curr_summ_delivered, &attrs.curr_summ_received, &attrs.fast_poll_update_period,          \
                                      &attrs.active_register_tier_delivered, &attrs.number_of_tiers_in_use, &attrs.curr_tier1_summ_delivered, &attrs.curr_tier1_summ_received,  \
                                      &attrs.curr_tier2_summ_delivered, &attrs.curr_tier3_summ_delivered, &attrs.curr_tier4_summ_delivered,      \
                                      &attrs.curr_tier5_summ_delivered, &attrs.curr_tier6_summ_delivered, &attrs.curr_tier7_summ_delivered,      \
                                      &attrs.curr_tier8_summ_delivered, &attrs.curr_tier9_summ_delivered, &attrs.curr_tier10_summ_delivered,     \
                                      &attrs.status, &attrs.extended_status, &attrs.curr_meter_id, &attrs.service_disconnect_reason,             \
                                      &attrs.linky_mode_of_operation, &attrs.unit_of_measure, &attrs.multiplier, &attrs.divisor,                 \
                                      &attrs.summation_formatting, &attrs.demand_formatting, &attrs.historical_consumption_formatting,           \
                                      &attrs.device_type, &attrs.site_id, &attrs.meter_serial_number, &attrs.module_serial_number,               \
                                      &attrs.instantaneous_demand, &attrs.curr_day_max_demand_delivered, &attrs.curr_day_max_demand_delivered_time,\
                                      &attrs.curr_day_max_demand_received, &attrs.curr_day_max_demand_received_time,                             \
                                      &attrs.prev_day_max_demand_delivered, &attrs.prev_day_max_demand_delivered_time,                           \
                                      &attrs.prev_day_max_demand_received, &attrs.prev_day_max_demand_received_time,                             \
                                      &attrs.max_number_of_periods_delivered,                                                                    \
                                      &attrs.curr_reactive_summ_q1, &attrs.curr_reactive_summ_q2, &attrs.curr_reactive_summ_q3, &attrs.curr_reactive_summ_q4)

/** @} */ /* ZB_ERL_METERING_CLUSTER_DEFINITIONS */

/** @defgroup ZB_ERL_DAILY_SCHEDULE_CLUSTER_DEFINITIONS Daily schedule cluster definitions for ERL device
 *  @{
 */

/** @brief Declare attribute list for Daily schedule cluster
 *  @param[in] attr_list - attribute list variable name
 *  @param[in] aux_switch_1_label  - pointer to variable to store @ref ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_1_LABEL
 *  @param[in] aux_switch_2_label  - pointer to variable to store @ref ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_2_LABEL
 *  @param[in] aux_switch_3_label  - pointer to variable to store @ref ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_3_LABEL
 *  @param[in] aux_switch_4_label  - pointer to variable to store @ref ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_4_LABEL
 *  @param[in] aux_switch_5_label  - pointer to variable to store @ref ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_5_LABEL
 *  @param[in] aux_switch_6_label  - pointer to variable to store @ref ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_6_LABEL
 *  @param[in] aux_switch_7_label  - pointer to variable to store @ref ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_7_LABEL
 *  @param[in] aux_switch_8_label  - pointer to variable to store @ref ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_8_LABEL
 *  @param[in] curr_aux_load_switch_state - pointer to variable to store @ref ZB_ZCL_ATTR_DAILY_SCHEDULE_CURR_AUX_LOAD_SWITCH_STATE
 *  @param[in] curr_delivered_tier - pointer to variable to store @ref ZB_ZCL_ATTR_DAILY_SCHEDULE_CURR_DELIVERED_TIER
 *  @param[in] curr_tier_label     - pointer to variable to store @ref ZB_ZCL_ATTR_DAILY_SCHEDULE_CURR_TIER_LABEL
 *  @param[in] linky_peak_period_status - pointer to variable to store @ref ZB_ZCL_ATTR_DAILY_SCHEDULE_LINKY_PEAK_PERIOD_STATUS
 *  @param[in] peak_start_time     - pointer to variable to store @ref ZB_ZCL_ATTR_DAILY_SCHEDULE_PEAK_START_TIME
 *  @param[in] peak_end_time       - pointer to variable to store @ref ZB_ZCL_ATTR_DAILY_SCHEDULE_PEAK_END_TIME
 *  @param[in] curr_tariff_label   - pointer to variable to store @ref ZB_ZCL_ATTR_DAILY_SCHEDULE_CURR_TARIFF_LABEL
 */
#define ZB_ERL_DECLARE_DAILY_SCHEDULE_ATTRIB_LIST(attr_list, aux_switch_1_label, aux_switch_2_label,         \
                                        aux_switch_3_label, aux_switch_4_label, aux_switch_5_label, \
                                        aux_switch_6_label, aux_switch_7_label, aux_switch_8_label, \
                                        curr_aux_load_switch_state, curr_delivered_tier, curr_tier_label, \
                                        linky_peak_period_status, peak_start_time, peak_end_time, \
                                        curr_tariff_label) \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                                       \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_1_LABEL, (aux_switch_1_label), ZB_ZCL_ATTR_TYPE_ARRAY, ZB_ZCL_ATTR_ACCESS_READ_WRITE) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_2_LABEL, (aux_switch_2_label), ZB_ZCL_ATTR_TYPE_ARRAY, ZB_ZCL_ATTR_ACCESS_READ_WRITE) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_3_LABEL, (aux_switch_3_label), ZB_ZCL_ATTR_TYPE_ARRAY, ZB_ZCL_ATTR_ACCESS_READ_WRITE) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_4_LABEL, (aux_switch_4_label), ZB_ZCL_ATTR_TYPE_ARRAY, ZB_ZCL_ATTR_ACCESS_READ_WRITE) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_5_LABEL, (aux_switch_5_label), ZB_ZCL_ATTR_TYPE_ARRAY, ZB_ZCL_ATTR_ACCESS_READ_WRITE) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_6_LABEL, (aux_switch_6_label), ZB_ZCL_ATTR_TYPE_ARRAY, ZB_ZCL_ATTR_ACCESS_READ_WRITE) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_7_LABEL, (aux_switch_7_label), ZB_ZCL_ATTR_TYPE_ARRAY, ZB_ZCL_ATTR_ACCESS_READ_WRITE) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_8_LABEL, (aux_switch_8_label), ZB_ZCL_ATTR_TYPE_ARRAY, ZB_ZCL_ATTR_ACCESS_READ_WRITE) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_DAILY_SCHEDULE_CURR_AUX_LOAD_SWITCH_STATE, (curr_aux_load_switch_state), ZB_ZCL_ATTR_TYPE_8BITMAP, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_DAILY_SCHEDULE_CURR_DELIVERED_TIER, (curr_delivered_tier), ZB_ZCL_ATTR_TYPE_8BIT_ENUM, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_DAILY_SCHEDULE_CURR_TIER_LABEL, (curr_tier_label), ZB_ZCL_ATTR_TYPE_ARRAY, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_DAILY_SCHEDULE_LINKY_PEAK_PERIOD_STATUS, (linky_peak_period_status), ZB_ZCL_ATTR_TYPE_8BITMAP, ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_DAILY_SCHEDULE_PEAK_START_TIME, (peak_start_time), ZB_ZCL_ATTR_TYPE_UTC_TIME, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_DAILY_SCHEDULE_PEAK_END_TIME, (peak_end_time), ZB_ZCL_ATTR_TYPE_UTC_TIME, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_DAILY_SCHEDULE_CURR_TARIFF_LABEL, (curr_tariff_label), ZB_ZCL_ATTR_TYPE_ARRAY, ZB_ZCL_ATTR_ACCESS_READ_ONLY) \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

#define ZB_ERL_DAILY_SCHEDULE_REPORT_ATTR_COUNT 3

/**
 * Daily Schedule cluster attributes
 */
typedef struct zb_erl_daily_schedule_server_attrs_s
{
  /** @copydoc ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_1_LABEL
   * @see ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_1_LABEL
   */
  zb_uint8_t aux_switch_1_label[23];
  /** @copydoc ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_2_LABEL
   * @see ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_2_LABEL
   */
  zb_uint8_t aux_switch_2_label[23];
  /** @copydoc ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_3_LABEL
   * @see ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_3_LABEL
   */
  zb_uint8_t aux_switch_3_label[23];
  /** @copydoc ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_4_LABEL
   * @see ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_4_LABEL
   */
  zb_uint8_t aux_switch_4_label[23];
  /** @copydoc ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_5_LABEL
   * @see ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_5_LABEL
   */
  zb_uint8_t aux_switch_5_label[23];
  /** @copydoc ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_6_LABEL
   * @see ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_6_LABEL
   */
  zb_uint8_t aux_switch_6_label[23];
  /** @copydoc ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_7_LABEL
   * @see ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_7_LABEL
   */
  zb_uint8_t aux_switch_7_label[23];
  /** @copydoc ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_8_LABEL
   * @see ZB_ZCL_ATTR_DAILY_SCHEDULE_AUX_SWITCH_8_LABEL
   */
  zb_uint8_t aux_switch_8_label[23];
  /** @copydoc ZB_ZCL_ATTR_DAILY_SCHEDULE_CURR_AUX_LOAD_SWITCH_STATE
   * @see ZB_ZCL_ATTR_DAILY_SCHEDULE_CURR_AUX_LOAD_SWITCH_STATE
   */
  zb_uint8_t curr_aux_load_switch_state;
  /** @copydoc ZB_ZCL_ATTR_DAILY_SCHEDULE_CURR_DELIVERED_TIER
   * @see ZB_ZCL_ATTR_DAILY_SCHEDULE_CURR_DELIVERED_TIER
   */
  zb_uint8_t curr_delivered_tier;
  /** @copydoc ZB_ZCL_ATTR_DAILY_SCHEDULE_CURR_TIER_LABEL
   * @see ZB_ZCL_ATTR_DAILY_SCHEDULE_CURR_TIER_LABEL
   */
  zb_uint8_t curr_tier_label[19];
  /** @copydoc ZB_ZCL_ATTR_DAILY_SCHEDULE_LINKY_PEAK_PERIOD_STATUS
   * @see ZB_ZCL_ATTR_DAILY_SCHEDULE_LINKY_PEAK_PERIOD_STATUS
   */
  zb_uint8_t linky_peak_period_status;
  /** @copydoc ZB_ZCL_ATTR_DAILY_SCHEDULE_PEAK_START_TIME
   * @see ZB_ZCL_ATTR_DAILY_SCHEDULE_PEAK_START_TIME
   */
  zb_uint32_t peak_start_time;
  /** @copydoc ZB_ZCL_ATTR_DAILY_SCHEDULE_PEAK_END_TIME
   * @see ZB_ZCL_ATTR_DAILY_SCHEDULE_PEAK_END_TIME
   */
  zb_uint32_t peak_end_time;
  /** @copydoc ZB_ZCL_ATTR_DAILY_SCHEDULE_CURR_TARIFF_LABEL
   * @see ZB_ZCL_ATTR_DAILY_SCHEDULE_CURR_TARIFF_LABEL
   */
  zb_uint8_t curr_tariff_label[27];
} zb_erl_daily_schedule_server_attrs_t;

#define ZB_ERL_AUX_SWITCH_N_LABEL_ATTR_INIT(n_label) \
  {sizeof("Auxiliary 0") - 1, 'A', 'u', 'x', 'i', 'l', 'i', 'a', 'r', 'y', ' ', '0' + n_label}

/** Initialize @ref zb_erl_daily_schedule_server_attrs_s Daily schedule cluster's attributes */
#define ZB_ERL_DAILY_SCHEDULE_ATTR_LIST_INIT             \
        (zb_erl_daily_schedule_server_attrs_t)           \
        { .aux_switch_1_label = ZB_ERL_AUX_SWITCH_N_LABEL_ATTR_INIT(1), \
          .aux_switch_2_label = ZB_ERL_AUX_SWITCH_N_LABEL_ATTR_INIT(2), \
          .aux_switch_3_label = ZB_ERL_AUX_SWITCH_N_LABEL_ATTR_INIT(3), \
          .aux_switch_4_label = ZB_ERL_AUX_SWITCH_N_LABEL_ATTR_INIT(4), \
          .aux_switch_5_label = ZB_ERL_AUX_SWITCH_N_LABEL_ATTR_INIT(5), \
          .aux_switch_6_label = ZB_ERL_AUX_SWITCH_N_LABEL_ATTR_INIT(6), \
          .aux_switch_7_label = ZB_ERL_AUX_SWITCH_N_LABEL_ATTR_INIT(7), \
          .aux_switch_8_label = ZB_ERL_AUX_SWITCH_N_LABEL_ATTR_INIT(8), \
          .curr_aux_load_switch_state = ZB_ZCL_DAILY_SCHEDULE_CURRENT_AUXILLARY_LOAD_SWITCH_STATE_DEFAULT_VALUE, \
          .linky_peak_period_status = ZB_ZCL_DAILY_SCHEDULE_LINKY_PEAK_PERIOD_STATUS_DEFAULT_VALUE, \
          .curr_tariff_label = ZB_ZCL_DAILY_SCHEDULE_CURRENT_TARIFF_LABEL_DEFAULT_VALUE \
}

/** @brief Declare attribute list for Daily schedule cluster
 *  @param[in] attr_list - attribute list variable name
 *  @param[in] attrs - pointer to @ref erl_daily_schedule_server_attrs_s structure
 */
#define ZB_ERL_DECLARE_DAILY_SCHEDULE_ATTR_LIST(attr_list, attrs)  \
  ZB_ERL_DECLARE_DAILY_SCHEDULE_ATTRIB_LIST(attr_list, &attrs.aux_switch_1_label, &attrs.aux_switch_2_label, \
                                         &attrs.aux_switch_3_label, &attrs.aux_switch_4_label, &attrs.aux_switch_5_label, \
                                         &attrs.aux_switch_6_label, &attrs.aux_switch_7_label, &attrs.aux_switch_8_label, \
                                         &attrs.curr_aux_load_switch_state, &attrs.curr_delivered_tier, \
                                         &attrs.curr_tier_label, &attrs.linky_peak_period_status, &attrs.peak_start_time, \
                                         &attrs.peak_end_time, &attrs.curr_tariff_label)

/** @} */ /* ZB_ERL_DAILY_SCHEDULE_CLUSTER_DEFINITIONS */

/** @endcond */ /* DOXYGEN_ERL_SECTION */

#endif /* defined ZB_HA_DEFINE_DEVICE_ERL_INTERFACE_DEVICE || defined DOXYGEN */
