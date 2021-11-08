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
/* PURPOSE:
*/

#ifndef ZDO_WWAH_SURVEY_BEACONS_H
#define ZDO_WWAH_SURVEY_BEACONS_H 1

#include "zb_config.h"
#include "zb_common.h"

/* 2019-02-26: CR:MINOR TODO:Split WWAH-specific and non-WWAH-specific parts of Survey Beacons. */
#if defined ZB_BEACON_SURVEY && defined ZB_ZCL_ENABLE_WWAH_SERVER

/** @cond DOXYGEN_ZCL_SECTION */

/**
 *  @addtogroup ZB_ZCL_WWAH
 *  @{
 */

/**
 *  @addtogroup ZDO_WWAH_SURVEY_BEACONS
 *  @{
 */

typedef zb_zcl_wwah_beacon_survey_t zb_zdo_beacon_survey_parent_info_t;

/**
 *  @brief R23, I.3.3 Beacon Survey Configuration TLV
 *
 *  This TLV is 5-bytes in length and contains information
 *  about the channels and scan configuration that is used
 *  when performing a beacon survey.
 */
typedef ZB_PACKED_PRE struct zb_zdo_beacon_survey_configuration_t
{
  zb_nlme_beacon_survey_scan_request_t params;
}
ZB_PACKED_STRUCT zb_zdo_beacon_survey_configuration_t;

/**
 *  @brief R23, I.3.4 Beacon Survey Results TLV
 *
 *  This TLV is 4-bytes in length and contains information about the
 *  channels, scan configuration, and counted devices or beacons.
 */
typedef ZB_PACKED_PRE struct zb_zdo_beacon_survey_results_t
{
  /*!< Number of potential parents from the current Zigbee Network */
  zb_uint8_t num_potential_parents_current_zbn;

  /*!< Number of detected non-Zigbee networks */
  zb_uint8_t num_non_zbn;

  /*!< Number of detected ZigBee networks */
  zb_uint8_t num_zbn;

  /*!< Total Beacons Surveyed */
  zb_uint8_t total_beacons_surveyed;
}
ZB_PACKED_STRUCT zb_zdo_beacon_survey_results_t;

/**
 *  @brief R23, I.3.5 Potential Parents TLV
 *
 *  This TLV is 3 to 13 bytes in length and indicates the number
 *  of available parents in radio range. A maximum number of 5
 *  parents is supported for this TLV. The list of potential
 *  parents SHALL be ordered as described in 3.6.1.3.2.
 */
typedef ZB_PACKED_PRE struct zb_zdo_beacon_survey_potential_parents_t
{
  /*!< Pointer to the @zb_zdo_beacon_survey_parent_info_t list */
  zb_uint8_t *parents_info_ptr;
  /*!< Short address of the our parent (0xffff for Routers) */
  zb_uint16_t current_parent;
  /*!< Count of potential parents (valid range [0;5]) */
  zb_uint8_t count_potential_parents;
}
ZB_PACKED_STRUCT zb_zdo_beacon_survey_potential_parents_t;

/**
 *  @brief Structure to store information when 'Beacon Survey' procedure is executing
 *
 *  We reuse a buffer which contain 'Beacon Survey Request' command
 *  and store all necessary information into it. Format of the
 *  'Beacon Survey Response' command is different between ZCL WWAH
 *  and r23, so we need to use two functions to parse results and
 *  send response
 */
typedef ZB_PACKED_PRE struct zb_zdo_beacon_survey_resp_params_s
{
  /*!< R23, I.3.5 Potential Parents TLV */
  zb_zdo_beacon_survey_potential_parents_t parents;
  /*!< I.3.3 Beacon Survey Configuration TLV */
  zb_zdo_beacon_survey_configuration_t configuration;
  /*!< R23, I.3.4 Beacon Survey Results TLV */
  zb_zdo_beacon_survey_results_t results;
}
ZB_PACKED_STRUCT zb_zdo_beacon_survey_resp_params_t;

/**
 *  @brief Start Survey Beacons procedure
 *
 *  @param param - reference to a buffer which contain
 *         @zb_zdo_survey_beacons_params_t
 *
 *  @return @RET_ERROR if failed;
 *          @RET_BUSY otherwise - in this case stack doesn't free
 *          the buffer with Survey Beacons command, because this buffer
 *          will be reused to send Survey Beacons Response command.
 */
zb_ret_t zdo_wwah_start_survey_beacons(zb_uint8_t param);

/**
 *  @brief Process a received beacons and put information from it
 *         into the 'Survey Beacons Response' buffer
 *
 *  @param beacon_noti_ind_ptr - pointer to the @zb_mac_beacon_notify_indication_t
 *  @param beacon_payload_ptr - pointer to the @zb_mac_beacon_payload_t
 */
void zdo_wwah_process_beacon_info(
  zb_mac_beacon_notify_indication_t *beacon_noti_ind_ptr,
  zb_mac_beacon_payload_t *beacon_payload_ptr);

/**
 *  @brief Send 'Survey Beacons Response' command
 *
 *  @todo need to implement in r23
 */
void zdo_send_survey_beacons_response(void);

#if (defined ZB_ZCL_SUPPORT_CLUSTER_WWAH && defined ZB_ZCL_ENABLE_WWAH_SERVER)

/* 4 bytes for: fcf, seq_num, command_type and beacons_number */
#define ZDO_WWAH_MAX_BEACON_SURVEY() \
  ((ZB_ZCL_HI_WO_IEEE_MAX_PAYLOAD_SIZE - 4)     \
   / sizeof(zb_zdo_beacon_survey_parent_info_t))

#define ZDO_WWAH_MAX_BEACON_SURVEY_BYTES()    \
  (sizeof(zb_zdo_beacon_survey_parent_info_t) \
   * (ZDO_WWAH_MAX_BEACON_SURVEY()))          \

void zb_zcl_wwah_send_survey_beacons_response(zb_bufid_t buf, zb_uint16_t zcl_buf);

#define ZDO_SEND_SURVEY_BEACONS_RESPONSE_FUNC(buf) \
  zb_zcl_wwah_send_survey_beacons_response(0, buf)

#else /* ZB_ZCL_SUPPORT_CLUSTER_WWAH */

/* see R23, I.3.5 Potential Parents TLV, 'Count of Potential Parents' field */
#define ZDO_WWAH_MAX_BEACON_SURVEY() 5U

#define ZDO_WWAH_MAX_BEACON_SURVEY_BYTES() \
  (ZDO_WWAH_MAX_BEACON_SURVEY() * sizeof(zb_uint16_t))

#define ZDO_SEND_SURVEY_BEACONS_RESPONSE_FUNC() \
  zdo_send_survey_beacons_response(buf)

#endif /* (defined ZB_ZCL_SUPPORT_CLUSTER_WWAH && defined ZB_ZCL_ENABLE_WWAH_SERVER) */

#endif /* defined ZB_BEACON_SURVEY && defined ZB_ZCL_ENABLE_WWAH_SERVER */

/** @} */ /* @addtogroup ZDO_WWAH_SURVEY_BEACONS */

/** @} */ /* @addtogroup ZB_ZCL_WWAH */

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

#endif /* ZDO_WWAH_SURVEY_BEACONS_H */
