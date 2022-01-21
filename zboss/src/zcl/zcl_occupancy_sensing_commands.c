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
/* PURPOSE: ZCL Occupancy Sensing cluster specific commands handling
*/

#define ZB_TRACE_FILE_ID 6

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_OCCUPANCY_SENSING)

#include "zcl/zb_zcl_occupancy_sensing.h"

/** @internal
    @{
*/

zb_ret_t check_value_occupancy_sensing_server(zb_uint16_t attr_id, zb_uint8_t endpont, zb_uint8_t *value);

void zb_zcl_occupancy_sensing_init_server()
{
    zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_OCCUPANCY_SENSING,
                                ZB_ZCL_CLUSTER_SERVER_ROLE,
                                check_value_occupancy_sensing_server,
                                (zb_zcl_cluster_write_attr_hook_t)NULL,
                                (zb_zcl_cluster_handler_t)NULL);
}

void zb_zcl_occupancy_sensing_init_client()
{
    zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_OCCUPANCY_SENSING,
                                ZB_ZCL_CLUSTER_CLIENT_ROLE,
                                (zb_zcl_cluster_check_value_t)NULL,
                                (zb_zcl_cluster_write_attr_hook_t)NULL,
                                (zb_zcl_cluster_handler_t)NULL);
}

zb_ret_t check_value_occupancy_sensing_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
    zb_ret_t ret = RET_ERROR;

    zb_uint16_t new_delay;
    zb_uint16_t max_delay;

    zb_uint8_t min_threshold;
    zb_uint8_t max_threshold;

    TRACE_MSG(TRACE_ZCL1, "> check_value_occupancy_sensing_server attr_id %d, ep %hd, val %hd",
              (FMT__H_H_D, attr_id, endpoint, value));

    if (value)
    {
        if (attr_id == ZB_ZCL_ATTR_OCCUPANCY_SENSING_PIR_OCC_TO_UNOCC_DELAY_ID ||
            attr_id == ZB_ZCL_ATTR_OCCUPANCY_SENSING_PIR_UNOCC_TO_OCC_DELAY_ID ||
            attr_id == ZB_ZCL_ATTR_OCCUPANCY_SENSING_ULTRASONIC_OCCUPIED_TO_UNOCCUPIED_DELAY_ID ||
            attr_id == ZB_ZCL_ATTR_OCCUPANCY_SENSING_ULTRASONIC_UNOCCUPIED_TO_OCCUPIED_DELAY_ID ||
            attr_id == ZB_ZCL_ATTR_OCCUPANCY_SENSING_PHYSICAL_CONTACT_OCCUPIED_TO_UNOCCUPIED_DELAY_ID ||
            attr_id == ZB_ZCL_ATTR_OCCUPANCY_SENSING_PHYSICAL_CONTACT_UNOCCUPIED_TO_OCCUPIED_DELAY_ID)
        {
            max_delay = ZB_ZCL_OCCUPANCY_SENSING_PHYSICAL_CONTACT_OCCUPIED_TO_UNOCCUPIED_DELAY_NO_REPORTING_VALUE;
            switch (attr_id)
            {
                case ZB_ZCL_ATTR_OCCUPANCY_SENSING_PIR_OCC_TO_UNOCC_DELAY_ID:
                {
                    max_delay = ZB_ZCL_OCCUPANCY_SENSING_PIR_OCC_TO_UNOCC_DELAY_MAX_VALUE;
                }break;
                case ZB_ZCL_ATTR_OCCUPANCY_SENSING_PIR_UNOCC_TO_OCC_DELAY_ID:
                {
                    max_delay = ZB_ZCL_OCCUPANCY_SENSING_PIR_UNOCC_TO_OCC_DELAY_MAX_VALUE;
                }break;
                case ZB_ZCL_ATTR_OCCUPANCY_SENSING_ULTRASONIC_OCCUPIED_TO_UNOCCUPIED_DELAY_ID:
                {
                    max_delay = ZB_ZCL_OCCUPANCY_SENSING_ULTRASONIC_OCCUPIED_TO_UNOCCUPIED_DELAY_MAX_VALUE;
                }break;
                case ZB_ZCL_ATTR_OCCUPANCY_SENSING_ULTRASONIC_UNOCCUPIED_TO_OCCUPIED_DELAY_ID:
                {
                    max_delay = ZB_ZCL_OCCUPANCY_SENSING_ULTRASONIC_UNOCCUPIED_TO_OCCUPIED_DELAY_MAX_VALUE;
                }
            }
            new_delay = ZB_ZCL_ATTR_GET16(value);

            if (new_delay > max_delay)
            {
                ret = RET_OUT_OF_RANGE;
            }
            else
            {
                ret = RET_OK;
            }
        }
        else if (attr_id == ZB_ZCL_ATTR_OCCUPANCY_SENSING_PIR_UNOCC_TO_OCC_THRESHOLD_ID ||
                 attr_id == ZB_ZCL_ATTR_OCCUPANCY_SENSING_ULTRASONIC_UNOCCUPIED_TO_OCCUPIED_THRESHOLD_ID ||
                 attr_id == ZB_ZCL_ATTR_OCCUPANCY_SENSING_PHYSICAL_CONTACT_UNOCCUPIED_TO_OCCUPIED_THRESHOLD_ID)
        {
            switch (attr_id)
            {
                case ZB_ZCL_ATTR_OCCUPANCY_SENSING_PIR_UNOCC_TO_OCC_THRESHOLD_ID:
                {
                    min_threshold = ZB_ZCL_OCCUPANCY_SENSING_PIR_UNOCC_TO_OCC_THRESHOLD_MIN_VALUE;
                    max_threshold = ZB_ZCL_OCCUPANCY_SENSING_PIR_UNOCC_TO_OCC_THRESHOLD_MAX_VALUE;
                }break;
                case ZB_ZCL_ATTR_OCCUPANCY_SENSING_ULTRASONIC_UNOCCUPIED_TO_OCCUPIED_THRESHOLD_ID:
                {
                    min_threshold = ZB_ZCL_OCCUPANCY_SENSING_ULTRASONIC_UNOCCUPIED_TO_OCCUPIED_THRESHOLD_MIN_VALUE;
                    max_threshold = ZB_ZCL_OCCUPANCY_SENSING_ULTRASONIC_UNOCCUPIED_TO_OCCUPIED_THRESHOLD_MAX_VALUE;
                }break;
                case ZB_ZCL_ATTR_OCCUPANCY_SENSING_PHYSICAL_CONTACT_UNOCCUPIED_TO_OCCUPIED_THRESHOLD_ID:
                {
                    min_threshold = ZB_ZCL_OCCUPANCY_SENSING_PHYSICAL_CONTACT_UNOCCUPIED_TO_OCCUPIED_THRESHOLD_MIN_VALUE;
                    max_threshold = ZB_ZCL_OCCUPANCY_SENSING_PHYSICAL_CONTACT_UNOCCUPIED_TO_OCCUPIED_THRESHOLD_MAX_VALUE;
                }break;
                default:
                {
                    TRACE_MSG(TRACE_ERROR, "zcl_occ_sens_check_threshold_value: Unsupported attribute ID!", (FMT__0));
                    ZB_ASSERT(0); /*Unsupported attribute*/
                }break;
            }

            if (((*value) < min_threshold) || ((*value) > max_threshold))
            {
                ret = RET_OUT_OF_RANGE;
            }
            else
            {
                ret = RET_OK;
            }
        }
    }
    else
    {
        TRACE_MSG(TRACE_ERROR, "check_value_occupancy_sensing_server: new value pointer is NULL!", (FMT__0));
    }
    TRACE_MSG(TRACE_ZCL1, "< check_value_occupancy_sensing_server ret %hd", (FMT__H, ret));
    return ret;
}

#endif
