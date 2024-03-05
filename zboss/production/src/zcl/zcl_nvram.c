/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2024 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: ZCL datasets functions
*/

#define ZB_TRACE_FILE_ID 2056

#include "zb_common.h"

#if defined ZB_USE_NVRAM && defined ZB_ENABLE_ZCL

#include "zb_zcl.h"

#if defined ZB_ENABLE_HA
/** @brief Read HA data from NVRAM */

void zb_nvram_read_ha_dataset(
  zb_uint8_t page, zb_uint32_t pos, zb_uint16_t len, zb_nvram_ver_t nvram_ver, zb_uint16_t ds_ver)
{
  zb_nvram_dataset_ha_t ds;
  zb_ret_t ret;

  TRACE_MSG(TRACE_ZCL1, "> zb_nvram_read_ha_dataset %hd pos %ld, len %d, nvram_ver %d",
      (FMT__H_L_D_D, page, pos, len, nvram_ver));

  ZVUNUSED(len);
  ZVUNUSED(nvram_ver);
  ZVUNUSED(ds_ver);

  /* If we fail, trace is given and assertion is triggered */
  ret = zb_nvram_read_data(page, pos, (zb_uint8_t*)&ds, sizeof(ds));

  if (ret == RET_OK)
  {
    zb_uint8_t endpoint;
    zb_zcl_attr_t *attr_desc;

#ifdef ZB_ZCL_SUPPORT_CLUSTER_IAS_ZONE
    /********** IAS Zone Data set *************/
    endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE);
    TRACE_MSG(TRACE_ZCL1, "ias zone ep %hd", (FMT__H, endpoint));

    if (endpoint)
    {
      attr_desc = zb_zcl_get_attr_desc_a(
        endpoint,
        ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_IAS_ZONE_IAS_CIE_ADDRESS_ID);
      ZB_ASSERT(attr_desc);

      ZB_IEEE_ADDR_COPY(attr_desc->data_p, ds.cie_address);

      attr_desc = zb_zcl_get_attr_desc_a(
        endpoint,
        ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_IAS_ZONE_ZONESTATE_ID);
      ZB_ASSERT(attr_desc);

      ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, ds.zone_state);

      attr_desc = zb_zcl_get_attr_desc_a(
       endpoint,
       ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_IAS_ZONE_ZONEID_ID);
      ZB_ASSERT(attr_desc);
      ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, ds.zone_id);

      attr_desc = zb_zcl_get_attr_desc_a(
        endpoint,
        ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_CUSTOM_CIE_EP);

      ZB_ASSERT(attr_desc);

      ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, ds.cie_ep);


      attr_desc = zb_zcl_get_attr_desc_a(
        endpoint,
        ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_CUSTOM_CIE_SHORT_ADDR);
      ZB_ASSERT(attr_desc);

      ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc, ds.cie_short_addr);

      attr_desc = zb_zcl_get_attr_desc_a(
        endpoint,
        ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_CUSTOM_CIE_ADDR_IS_SET);
      ZB_ASSERT(attr_desc);

      ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, ds.cie_address_is_set);

      if (ZB_U2B(ds.cie_address_is_set))
      {
        ret = zb_zcl_ias_zone_put_cie_address_to_binding_whitelist(endpoint);
        if (ret != RET_OK)
        {
          TRACE_MSG(TRACE_ERROR, "Put CIE address to binding whitelist failed ret %hu", (FMT__D, ret));
        }
      }
    }
#endif
#ifdef ZB_ZCL_SUPPORT_CLUSTER_POLL_CONTROL
    /********** Poll control Data set *************/
    endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE);
    TRACE_MSG(TRACE_ZCL1, "poll control ep %hd", (FMT__H, endpoint));
    if (endpoint)
    {
      zb_zcl_poll_control_srv_cfg_data_t *addr_data;

      attr_desc = zb_zcl_get_attr_desc_a(
        endpoint, ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE,
        ZB_ZCL_ATTR_POLL_CONTROL_ADDR_DATA_ID);
      ZB_ASSERT(attr_desc);

      addr_data = (zb_zcl_poll_control_srv_cfg_data_t*)attr_desc->data_p;
      addr_data->poll_addr = ds.poll_ctrl_short_addr;
      addr_data->poll_ep = ds.poll_ctrl_ep;
    }
#endif
    /********** Diagnostic Data set *************/
    /* Diagnostics counters were moved into the ZDO Diagnostics dataset, see zb_nvram_dataset_diagnostics_v1_t */
#if 0

    endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_DIAGNOSTICS, ZB_ZCL_CLUSTER_SERVER_ROLE);
    TRACE_MSG(TRACE_ZCL1, "Diagnostic ep %hd", (FMT__H, endpoint));
    if (endpoint)
    {
      attr_desc = zb_zcl_get_attr_desc_a(
        endpoint,
        ZB_ZCL_CLUSTER_ID_DIAGNOSTICS, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_DIAGNOSTICS_NUMBER_OF_RESETS_ID);
      ZB_ASSERT(attr_desc);

      ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc, ds.number_of_resets);

      attr_desc = zb_zcl_get_attr_desc_a(
        endpoint,
        ZB_ZCL_CLUSTER_ID_DIAGNOSTICS, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_DIAGNOSTICS_JOIN_INDICATION_ID);
      ZB_ASSERT(attr_desc);

      ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc, ds.join_indication);

      attr_desc = zb_zcl_get_attr_desc_a(
        endpoint,
        ZB_ZCL_CLUSTER_ID_DIAGNOSTICS, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_DIAGNOSTICS_PACKET_BUFFER_ALLOCATE_FAILURES_ID);
      ZB_ASSERT(attr_desc);

      ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc, ds.packet_buffer_allocate_failures);
    }

#endif
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_nvram_read_ha_dataset", (FMT__0));
}


/** @brief Write HA data to NVRAM */
zb_ret_t zb_nvram_write_ha_dataset(zb_uint8_t page, zb_uint32_t pos)
{
  zb_nvram_dataset_ha_t ds;
  zb_uint8_t endpoint;
  zb_zcl_attr_t *attr_desc;

  TRACE_MSG(TRACE_ZCL1, "> zb_nvram_write_ha_dataset page %hd pos %ld",
      (FMT__H_L, page, pos));

  ZB_BZERO(&ds, sizeof(ds));

  /********** IAS Zone Data set *************/
#ifdef ZB_ZCL_SUPPORT_CLUSTER_IAS_ZONE
  endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE);
  TRACE_MSG(TRACE_ZCL1, "ias zone ep %hd", (FMT__H, endpoint));

  if (endpoint)
  {
    attr_desc = zb_zcl_get_attr_desc_a(
      endpoint,
      ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_IAS_ZONE_IAS_CIE_ADDRESS_ID);
    ZB_ASSERT(attr_desc);

    ZB_IEEE_ADDR_COPY(ds.cie_address, attr_desc->data_p);

    attr_desc = zb_zcl_get_attr_desc_a(
      endpoint,
      ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_IAS_ZONE_ZONESTATE_ID);
    ZB_ASSERT(attr_desc);

    ds.zone_state = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);

    attr_desc = zb_zcl_get_attr_desc_a(
      endpoint,
      ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_CUSTOM_CIE_EP);
    ZB_ASSERT(attr_desc);

    ds.cie_ep = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);

    attr_desc = zb_zcl_get_attr_desc_a(
      endpoint,
      ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_IAS_ZONE_ZONEID_ID);
    if (attr_desc)
    {
      ds.zone_id = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
    }
    else
    {
      ds.zone_id = 0;
    }

    attr_desc = zb_zcl_get_attr_desc_a(
      endpoint,
      ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_CUSTOM_CIE_SHORT_ADDR);
    ZB_ASSERT(attr_desc);

    ds.cie_short_addr = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);

    attr_desc = zb_zcl_get_attr_desc_a(
      endpoint,
      ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_CUSTOM_CIE_ADDR_IS_SET);
    ZB_ASSERT(attr_desc);

    ds.cie_address_is_set = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
  }
#endif
#ifdef ZB_ZCL_SUPPORT_CLUSTER_POLL_CONTROL
  /********** Poll control Data set *************/
  endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE);
  TRACE_MSG(TRACE_ZCL1, "poll control ep %hd", (FMT__H, endpoint));

  if (endpoint)
  {
    zb_zcl_poll_control_srv_cfg_data_t *addr_data;

    attr_desc = zb_zcl_get_attr_desc_a(
      endpoint, ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE,
      ZB_ZCL_ATTR_POLL_CONTROL_ADDR_DATA_ID);
    ZB_ASSERT(attr_desc);

    addr_data = (zb_zcl_poll_control_srv_cfg_data_t*)attr_desc->data_p;
    ds.poll_ctrl_short_addr = addr_data->poll_addr;
    ds.poll_ctrl_ep = addr_data->poll_ep;
  }
#endif
  /********** Diagnostic Data set *************/
  /* Diagnostics counters were moved into the ZDO Diagnostics dataset, see zb_nvram_dataset_diagnostics_v1_t */
#if 0

  endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_DIAGNOSTICS, ZB_ZCL_CLUSTER_SERVER_ROLE);
  TRACE_MSG(TRACE_ZCL1, "Diagnostic ep %hd", (FMT__H, endpoint));
  if (endpoint)
  {
    attr_desc = zb_zcl_get_attr_desc_a(
      endpoint,
      ZB_ZCL_CLUSTER_ID_DIAGNOSTICS, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_DIAGNOSTICS_NUMBER_OF_RESETS_ID);
    ZB_ASSERT(attr_desc);
    ds.number_of_resets = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);

    attr_desc = zb_zcl_get_attr_desc_a(
      endpoint,
      ZB_ZCL_CLUSTER_ID_DIAGNOSTICS, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_DIAGNOSTICS_JOIN_INDICATION_ID);
    ZB_ASSERT(attr_desc);
    ds.join_indication = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);

    attr_desc = zb_zcl_get_attr_desc_a(
      endpoint,
      ZB_ZCL_CLUSTER_ID_DIAGNOSTICS, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_DIAGNOSTICS_PACKET_BUFFER_ALLOCATE_FAILURES_ID);
    ZB_ASSERT(attr_desc);
    ds.packet_buffer_allocate_failures = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);

  }

#endif

  TRACE_MSG(TRACE_ZCL1, "< zb_nvram_write_ha_dataset", (FMT__0));

  return zb_nvram_write_data(page, pos, (zb_uint8_t*)&ds, sizeof(ds));
}

/**
 *  @} internal
*/
#endif /* defined ZB_ENABLE_HA*/

#if !defined ZB_ZCL_DISABLE_REPORTING
zb_uint16_t zb_nvram_zcl_reporting_dataset_length(void)
{
  zb_uindex_t i, j;
  zb_zcl_reporting_info_t *rep_info;
  zb_uint16_t counter = 0;

  TRACE_MSG(TRACE_ZCL1, ">> zb_nvram_zcl_reporting_dataset_length", (FMT__0));

  /* Iterate all reporting_info's */
  if (ZCL_CTX().device_ctx)
  {
    for (i = 0; i < ZCL_CTX().device_ctx->ep_count; i++)
    {
      if (ZCL_CTX().device_ctx->ep_desc_list[i]->reporting_info)
      {
        rep_info = ZCL_CTX().device_ctx->ep_desc_list[i]->reporting_info;
        for (j = 0; j < ZCL_CTX().device_ctx->ep_desc_list[i]->rep_info_count; j++)
        {
          if (ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_SLOT_BUSY))
          {
            counter++;
          }
          rep_info++;
        }
      }
    }
  }

  counter *= sizeof(zb_zcl_reporting_info_nvram_t);
  TRACE_MSG(TRACE_ZCL1, "<< zb_nvram_zcl_reporting_dataset_length %d", (FMT__D, counter));
  return counter;
}


void zb_nvram_read_zcl_reporting_dataset(
  zb_uint8_t page, zb_uint32_t pos, zb_uint16_t length, zb_nvram_ver_t nvram_ver, zb_uint16_t ds_ver)
{
  zb_zcl_reporting_info_nvram_t rec;
  zb_zcl_reporting_info_t *rep_info;
  zb_uindex_t i, j, k, l, count;
  zb_ret_t ret;

  TRACE_MSG(TRACE_ZCL1, "> zb_nvram_read_zcl_reporting_dataset %hd pos %ld length %d nvram_ver %d",
      (FMT__H_L_D_D, page, pos, length, nvram_ver));

  ZVUNUSED(nvram_ver);
  ZVUNUSED(ds_ver);

  count = length / sizeof(rec);

  /* Erase reporting_info for every ep */
  for (k = 0; k < ZCL_CTX().device_ctx->ep_count; k++)
  {
    if (ZCL_CTX().device_ctx->ep_desc_list[k]->reporting_info)
    {
      ZB_BZERO(ZCL_CTX().device_ctx->ep_desc_list[k]->reporting_info,
               sizeof(zb_zcl_reporting_info_t) *
               ZCL_CTX().device_ctx->ep_desc_list[k]->rep_info_count);
    }
  }

  for( i=0, j=0; i<count; i++)
  {
      /* If we fail, trace is given and assertion is triggered */
      ret = zb_nvram_read_data(page, pos, (zb_uint8_t*)&rec, sizeof(rec));
      pos += sizeof(rec);
      if (ret == RET_OK)
      {
      /* Try to find endpoint with this id */
      for (k = 0; k < ZCL_CTX().device_ctx->ep_count; k++)
      {
        if (ZCL_CTX().device_ctx->ep_desc_list[k]->reporting_info &&
            ZCL_CTX().device_ctx->ep_desc_list[k]->ep_id == rec.ep)
        {
          break;
        }
      }
      if (k < ZCL_CTX().device_ctx->ep_count)
      {
        rep_info = ZCL_CTX().device_ctx->ep_desc_list[k]->reporting_info;
        /* Skip BUSY slots */
        l = 0;
        while (l < ZCL_CTX().device_ctx->ep_desc_list[k]->rep_info_count &&
               ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_SLOT_BUSY))
        {
          ++l;
          ++rep_info;
        }

        if (l < ZCL_CTX().device_ctx->ep_desc_list[k]->rep_info_count)
        {
        ZB_ZCL_SET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_SLOT_BUSY);

        TRACE_MSG(TRACE_ZCL3, "rec info flags %hx, ep %hd, cluster_id %d, attr_id %d",
                  (FMT__H_H_D_D, rec.flags, rec.ep, rec.cluster_id, rec.attr_id));

        rep_info->direction = rec.direction;
        rep_info->ep = rec.ep;
        rep_info->cluster_id = rec.cluster_id;
          rep_info->cluster_role = rec.cluster_role;
        rep_info->attr_id = rec.attr_id;
        rep_info->manuf_code = rec.manuf_code;
        rep_info->flags = rec.flags;

        if (rep_info->direction == ZB_ZCL_CONFIGURE_REPORTING_SEND_REPORT)
        {
          rep_info->u.send_info.min_interval = rec.min_interval;
          rep_info->u.send_info.max_interval = rec.max_interval;
          rep_info->u.send_info.delta.u48.low = rec.delta;
        }
        else
        {
          rep_info->u.recv_info.timeout = rec.min_interval;
        }
        rep_info->dst.short_addr = rec.short_addr_dest;
        rep_info->dst.endpoint = rec.endpoint_dest;
        rep_info->dst.profile_id = rec.profile_id_dest;

        /* Set reported_value to 0xff - it is most likely not the default value and not the first
           change for the attribute.
           Rationale: we do not have a flag to check is it first or non-first report. Furthermore,
           we do reporting checks AFTER applying new value to attribute. Because of that, if first
           attribute change is to the value equal to initial reported_value (it is 0), this change
           will not be reportable. 0xff value is invalid for the most of attributes, so looks like
           it is more safe to use.
        */
        ZB_MEMSET(&rep_info->u.send_info.reported_value, -1, sizeof(union zb_zcl_attr_var_u));

        TRACE_MSG(TRACE_ZCL3, "next rep info %p, flags %hx, ep %hd, cluster_id %d, attr_id %d",
                  (FMT__P_H_H_D_D, rep_info, rep_info->flags, rep_info->ep, rep_info->cluster_id, rep_info->attr_id));

        zb_zcl_update_reporting_info(rep_info);
#if 0                           /* obsolete? */
        /* Mark attr for reporting - we need to send first report with min interval. */
        zb_zcl_mark_attr_for_reporting(rep_info->ep, rep_info->cluster_id, rep_info->attr_id);
        //rep_info++;
#endif
        j++;
      }
    }

    if(j>0)    // run timers
    {
      zb_zcl_adjust_reporting_timer(0);
    }
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_nvram_read_zcl_reporting_dataset", (FMT__0));
}


zb_ret_t zb_nvram_write_zcl_reporting_dataset(zb_uint8_t page, zb_uint32_t pos)
{
  zb_zcl_reporting_info_nvram_t rec;
  zb_zcl_reporting_info_t *rep_info;
  zb_zcl_reporting_info_nvram_t empty_rec;
  zb_ret_t ret = RET_OK;
  zb_uindex_t i, j;
  zb_uint8_t n_entries = 0;

  TRACE_MSG(TRACE_ZCL1, "> zb_nvram_write_zcl_reporting_dataset %hd pos %ld",
            (FMT__H_L, page, pos));

  if (ZCL_CTX().device_ctx)
  {
    /* Calculate number of entries to store */
    for (j = 0; j < ZCL_CTX().device_ctx->ep_count; j++)
    {
      if (ZCL_CTX().device_ctx->ep_desc_list[j]->reporting_info)
      {
        rep_info = ZCL_CTX().device_ctx->ep_desc_list[j]->reporting_info;
        for (i = 0; i < ZCL_CTX().device_ctx->ep_desc_list[j]->rep_info_count; i++)
        {
          if (ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_SLOT_BUSY))
          {
            n_entries++;
          }
          rep_info++;
        }
      }
    }

    TRACE_MSG(TRACE_ZCL3, "%hd entries to store", (FMT__H, n_entries));

    if (!n_entries)
    {
      TRACE_MSG(TRACE_ZCL3, "store empty entry", (FMT__0));

      ZB_BZERO(&empty_rec, sizeof(zb_zcl_reporting_info_nvram_t));

      ret = zb_nvram_write_data(page, pos, (zb_uint8_t*)&empty_rec, sizeof(empty_rec));
    }
    else
    {
      for (j = 0; j < ZCL_CTX().device_ctx->ep_count; j++)
      {
        if (ZCL_CTX().device_ctx->ep_desc_list[j]->reporting_info)
        {
          rep_info = ZCL_CTX().device_ctx->ep_desc_list[j]->reporting_info;
          for (i = 0; i < ZCL_CTX().device_ctx->ep_desc_list[j]->rep_info_count; i++)
          {
            TRACE_MSG(TRACE_ZCL3, "next rep info %p, flags %hx, ep %hd, cluster_id %d, attr_id %d",
                      (FMT__P_H_H_D_D, rep_info, rep_info->flags, rep_info->ep, rep_info->cluster_id, rep_info->attr_id));

            if (ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_SLOT_BUSY))
            {
              TRACE_MSG(TRACE_ZCL1, "save %hd rec", (FMT__H, i));

              ZB_BZERO(&rec, sizeof(rec));

              rec.direction = rep_info->direction;
              rec.ep = rep_info->ep;
              rec.cluster_id = rep_info->cluster_id;
              rec.cluster_role = rep_info->cluster_role;
              rec.attr_id = rep_info->attr_id;
              rec.manuf_code = rep_info->manuf_code;
              rec.flags = rep_info->flags;

              if (rep_info->direction == ZB_ZCL_CONFIGURE_REPORTING_SEND_REPORT)
              {
                rec.min_interval = rep_info->u.send_info.min_interval;
                rec.max_interval = rep_info->u.send_info.max_interval;
                rec.delta = rep_info->u.send_info.delta.u48.low;
              }
              else
              {
                rec.min_interval = rep_info->u.recv_info.timeout;
              }
              rec.short_addr_dest = rep_info->dst.short_addr;
              rec.endpoint_dest = rep_info->dst.endpoint;
              rec.profile_id_dest = rep_info->dst.profile_id;

              TRACE_MSG(TRACE_ZCL1, "rec info flags %hx, ep %hd, cluster_id %d, attr_id %d",
                        (FMT__H_H_D_D, rec.flags, rec.ep, rec.cluster_id, rec.attr_id));

              ret = zb_nvram_write_data(page, pos, (zb_uint8_t*)&rec, sizeof(rec));
              pos += sizeof(rec);
            }

            rep_info++;
          }
        }
      }
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_nvram_write_zcl_reporting_dataset %hd",
      (FMT__D, ret));

  return ret;
}
#endif /* !defined ZB_ZCL_DISABLE_REPORTING */


#if defined ZB_HA_ENABLE_POLL_CONTROL_SERVER || defined DOXYGEN
/** @brief Read POOL CONTROL data from NVRAM */
void zb_nvram_read_poll_control_dataset(
  zb_uint8_t page, zb_uint32_t pos, zb_uint16_t len, zb_nvram_ver_t nvram_ver, zb_uint16_t ds_ver)
{
  zb_nvram_dataset_poll_control_t ds;
  zb_ret_t ret;

  ZVUNUSED(len);
  ZVUNUSED(nvram_ver);
  ZVUNUSED(ds_ver);

  TRACE_MSG(TRACE_ZCL1, "> zb_nvram_read_poll_control_dataset %d pos %ld len %d nvram_ver %d",
      (FMT__H_L_D_D, page, pos, len, nvram_ver));

  /* If we fail, trace is given and assertion is triggered */
  ret = zb_nvram_read_data(page, pos, (zb_uint8_t*)&ds, sizeof(ds));

  if (ret == RET_OK)
  {
    zb_uint8_t endpoint;
    zb_zcl_attr_t *attr_desc;

    endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE);
    ZB_ASSERT(endpoint);

    attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_CHECKIN_INTERVAL_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL32(attr_desc, ds.checkin_interval);
    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_LONG_POLL_INTERVAL_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL32(attr_desc, ds.long_poll_interval);
    zb_zdo_pim_set_long_poll_interval(ZB_QUARTERECONDS_TO_MSEC(ds.long_poll_interval));
    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_SHORT_POLL_INTERVAL_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc, ds.short_poll_interval);
    zb_zdo_pim_set_fast_poll_interval(ZB_QUARTERECONDS_TO_MSEC(ds.short_poll_interval));
    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_FAST_POLL_TIMEOUT_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc, ds.fast_poll_timeout);
    zb_zdo_pim_set_fast_poll_timeout(ZB_QUARTERECONDS_TO_MSEC(ds.fast_poll_timeout));
    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_MIN_CHECKIN_INTERVAL_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL32(attr_desc, ds.checkin_interval_min);
    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_LONG_POLL_MIN_INTERVAL_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL32(attr_desc, ds.long_poll_interval_min);
    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_FAST_POLL_MAX_TIMEOUT_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc, ds.fast_poll_timeout_max);
  }
  TRACE_MSG(TRACE_ZCL1, "< zb_nvram_read_poll_control_dataset", (FMT__0));
}


/** @brief Write POOL CONTROL data to NVRAM */
zb_ret_t zb_nvram_write_poll_control_dataset(zb_uint8_t page, zb_uint32_t pos)
{
  zb_nvram_dataset_poll_control_t ds;
  zb_uint8_t endpoint;
  zb_zcl_attr_t *attr_desc;

  TRACE_MSG(TRACE_ZCL1, "> zb_nvram_write_poll_control_dataset page %d pos %ld",
      (FMT__H_L, page, pos));

  endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE);
  ZB_ASSERT(endpoint);

  ZB_BZERO(&ds, sizeof(ds));

  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint,
  ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_CHECKIN_INTERVAL_ID);
  ZB_ASSERT(attr_desc);
  ds.checkin_interval = ZB_ZCL_GET_ATTRIBUTE_VAL_32(attr_desc);
  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint,
  ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_LONG_POLL_INTERVAL_ID);
  ZB_ASSERT(attr_desc);
  ds.long_poll_interval = ZB_ZCL_GET_ATTRIBUTE_VAL_32(attr_desc);
  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint,
  ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_SHORT_POLL_INTERVAL_ID);
  ZB_ASSERT(attr_desc);
  ds.short_poll_interval = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);
  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint,
  ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_FAST_POLL_TIMEOUT_ID);
  ZB_ASSERT(attr_desc);
  ds.fast_poll_timeout = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);
  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint,
  ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_MIN_CHECKIN_INTERVAL_ID);
  ZB_ASSERT(attr_desc);
  ds.checkin_interval_min = ZB_ZCL_GET_ATTRIBUTE_VAL_32(attr_desc);
  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint,
  ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_LONG_POLL_MIN_INTERVAL_ID);
  ZB_ASSERT(attr_desc);
  ds.long_poll_interval_min = ZB_ZCL_GET_ATTRIBUTE_VAL_32(attr_desc);
  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint,
  ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_FAST_POLL_MAX_TIMEOUT_ID);
  ZB_ASSERT(attr_desc);
  ds.fast_poll_timeout_max = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);

  TRACE_MSG(TRACE_ZCL1, "< zb_nvram_write_poll_control_dataset", (FMT__0));

  return zb_nvram_write_data(page, pos, (zb_uint8_t*)&ds, sizeof(ds));
}
#endif /* defined ZB_HA_ENABLE_POLL_CONTROL_SERVER || defined DOXYGEN */


#endif /* defined ZB_USE_NVRAM && defined ZB_ENABLE_ZCL */
