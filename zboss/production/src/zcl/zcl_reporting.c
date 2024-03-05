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
/* PURPOSE: ZCL, reporting functions implementation
*/

#define ZB_TRACE_FILE_ID 2080

#include "zb_common.h"

#if defined(ZB_ENABLE_ZCL) && !(defined ZB_ZCL_DISABLE_REPORTING)

#include "zb_zcl.h"
#include "zb_aps.h"
#include "zb_zdo.h"
#include "zcl/zb_zcl_common.h"
#include "zcl/zb_zcl_reporting.h"
#include "zb_nvram.h"

#ifdef SWITCH_OFF_SOME_ZCL_TRACE
/* temporary switch trace off here */
#undef TRACE_ZCL1
#define TRACE_ZCL1 TRACE_MEM1
#undef TRACE_ZCL2
#define TRACE_ZCL2 TRACE_MEM2
#undef TRACE_ZCL3
#define TRACE_ZCL3 TRACE_MEM3
#endif

static zb_zcl_reporting_info_t* get_new_reporting_info(zb_uint8_t endpoint_id);

#ifdef EXCLUDED_FROM_BUILD
static zb_uint8_t zb_zcl_mark_all_for_reporting(zb_uint8_t flag);
#endif

static zb_bool_t check_delta_value(zb_zcl_reporting_info_t *rep_info);
static void start_wait_reporting_timer(zb_zcl_reporting_info_t *rep_info);
void zb_zcl_wait_reporting_timeout(zb_uint8_t param);
void zb_zcl_update_reporting_info(zb_zcl_reporting_info_t *rep_info);
void zb_zcl_adjust_reporting_timer(zb_uint8_t param);
void zb_zcl_reporting_timer_handler(zb_uint8_t param);
void zb_zcl_report_attr(zb_uint8_t param);

/*
  Reporting timeouts.

  Start 2 timers:
  - start_reporting timer - it will mark all attributes
  for reporting with max_interval timeout
  - allow_reporting timer - it will mark all attributes as allowed for
  reporting with min_interval timeout

  "allow_reporting" makes sense only for reporting caused by attribute
  value change and it is not taken into account for periodic reporting.

  Note: it is possible that some attribute is reported because of its
  value change and right after it - reported because of periodic
  reporting. Suppose, it is not very often occasion and network will
  not be overloaded because of it.


  Timeout for getting report.
  Functionality: check that some attribute is reported by remote with
  some timeout period; if attribute report for specific attribute is
  not received during <timeout period>, application is informed via
  user callback no_reporting_cb.
  On receiving attribute report, ZCL reporting logic is informed about
  it and in wait_reporting_timer is restarted. To inform about report
  receiving application should call zb_zcl_report_received(), it is
  also called from default report attribute handler zb_zcl_report_attr_cmd_handler()


  Sending reports
  To prevent ZB stack overloading with packets to send, use the
  following logic for sending reports:
  - send 1 report at a time
  - wait, until reporting command is sent (successfully or with error
  doesn't metter), zb_zcl_reporting_cb() is called from
  zb_apsde_data_acknowledged()
  - when command send is confirmed, send another command
  - to send next report, check reporting_info, find the first
  attribute that should be reported and send report for it

 */


/** @internal @brief Init reporting context - cancels all alarms and
 * initializes timeouts */
void zb_zcl_init_reporting_ctx()
{
  /* Allocate new buffer here... */
  zb_bufid_t buf;

  TRACE_MSG(TRACE_ZCL1, ">>zb_zcl_init_reporting_ctx", (FMT__0));

  ZB_BZERO(&ZCL_CTX().reporting_ctx, sizeof(zb_zcl_reporting_ctx_t));

  /* FIXME: Do not lock the buf if we do not need to report anything... */
  buf = zb_buf_get_any();
  if (buf)
  {
    TRACE_MSG(TRACE_ZCL1, "Use %hd buffer for reporting", (FMT__H, buf));

    ZCL_CTX().reporting_ctx.buf_ref = buf;
  }
  else
  {
    /* Empty buffer*/
    TRACE_MSG(TRACE_ERROR, "no free buf for reporting", (FMT__0));
    ZB_ASSERT(0);
  }

  ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_reporting_timer_handler, ZB_ALARM_ALL_CB);
  ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_wait_reporting_timeout, ZB_ALARM_ALL_CB);

  TRACE_MSG(TRACE_ZCL1, "<<zb_zcl_init_reporting_ctx", (FMT__0));
}

/** @internal @brief Reset reporting context - cancels all alarms and
 * initializes timeouts (without buffer allocation) */
void zb_zcl_reset_reporting_ctx()
{
  zb_uint8_t old_buf_ref = ZB_UNDEFINED_BUFFER;

  TRACE_MSG(TRACE_ZCL1, ">>zb_zcl_reset_reporting_ctx", (FMT__0));

  old_buf_ref = ZCL_CTX().reporting_ctx.buf_ref;
  ZB_BZERO(&ZCL_CTX().reporting_ctx, sizeof(zb_zcl_reporting_ctx_t));
  ZCL_CTX().reporting_ctx.buf_ref = old_buf_ref;

  TRACE_MSG(TRACE_ZCL1, "Use old %hd buf_ref for reporting", (FMT__H, old_buf_ref));

  ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_reporting_timer_handler, ZB_ALARM_ALL_CB);
  ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_wait_reporting_timeout, ZB_ALARM_ALL_CB);

  TRACE_MSG(TRACE_ZCL1, "<<zb_zcl_reset_reporting_ctx", (FMT__0));
}


/** @internal @brief Init reporting info data - mark it as unused */
void zb_zcl_init_reporting_info()
{
  zb_uindex_t i;
  TRACE_MSG(TRACE_ZCL1, "zb_zcl_init_reporting_info", (FMT__0));

  if (ZCL_CTX().device_ctx)
  {
    for (i = 0; i < ZCL_CTX().device_ctx->ep_count; i++)
    {
      if (ZCL_CTX().device_ctx->ep_desc_list[i]->reporting_info)
      {
        ZB_BZERO(ZCL_CTX().device_ctx->ep_desc_list[i]->reporting_info,
                 sizeof(zb_zcl_reporting_info_t) * ZCL_CTX().device_ctx->ep_desc_list[i]->rep_info_count);
      }
    }
  }
}

zb_ret_t zb_zcl_put_reporting_info(zb_zcl_reporting_info_t* rep_info_ptr, zb_bool_t override)
{
  zb_zcl_reporting_info_t *rep_info;
/* 05/28/15 NK:CR {*/
  zb_af_endpoint_desc_t *ep_desc;
  zb_zcl_cluster_desc_t *cluster_desc;
  zb_zcl_attr_t *attr_desc;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_put_reporting_info", (FMT__0));

  /* NK: Check if we support this attribute */
  ep_desc = zb_af_get_endpoint_desc(rep_info_ptr->ep);
  /* TRICKY: If role is not set, assume server by default. */
  if (!rep_info_ptr->cluster_role)
  {
    rep_info_ptr->cluster_role = ZB_ZCL_CLUSTER_SERVER_ROLE;
  }
  cluster_desc = get_cluster_desc(ep_desc, rep_info_ptr->cluster_id,
                                  rep_info_ptr->cluster_role);
  attr_desc = zb_zcl_get_attr_desc_manuf(cluster_desc, rep_info_ptr->attr_id, rep_info_ptr->manuf_code);

  if (attr_desc)
  {
    /* search for already registered configure reporting record */
    rep_info = zb_zcl_find_reporting_info_manuf(
      rep_info_ptr->ep,
      rep_info_ptr->cluster_id,
      rep_info_ptr->cluster_role,
      rep_info_ptr->attr_id,
      rep_info_ptr->manuf_code);

    /* New reporting configuration may overrule an existing reporting configuration (probably
     * configured by a peer). Check if overriding is allowed. */
    if (rep_info && !override)
    {
      /* not allowed - reporting already configured */
      return RET_ALREADY_EXISTS;
    }

    if (!rep_info)
    {
      rep_info = get_new_reporting_info(rep_info_ptr->ep);
    }

    if (rep_info)
    {
      if (rep_info->u.send_info.def_max_interval != 0 &&
           rep_info->u.send_info.def_max_interval < rep_info->u.send_info.def_min_interval)
      {
        return RET_INVALID_PARAMETER_1;
      }

      ZB_MEMCPY(rep_info, rep_info_ptr, sizeof(zb_zcl_reporting_info_t));
      ZB_ZCL_CLR_ALL_REPORTING_FLAGS(rep_info);

      rep_info->u.send_info.min_interval = rep_info->u.send_info.def_min_interval;
      rep_info->u.send_info.max_interval = rep_info->u.send_info.def_max_interval;

      ZB_ZCL_SET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_SLOT_BUSY);

      /* Set reported_value to 0xff - it is most likely not the default value and not the first change
         for the attribute.
         Rationale: we do not have a flag to check is it first or non-first report. Furthermore, we
         do reporting checks AFTER applying new value to attribute. Because of that, if first
         attribute change is to the value equal to initial reported_value (it is 0), this change
         will not be reportable.
         0xff value is invalid for the most of attributes, so looks like it is more safe to use.
      */
      ZB_MEMSET(&rep_info->u.send_info.reported_value, -1, sizeof(union zb_zcl_attr_var_u));

      if (rep_info->direction == ZB_ZCL_CONFIGURE_REPORTING_SEND_REPORT)
      {
        if (rep_info->u.send_info.max_interval == ZB_ZCL_REPORTING_NOT_NEEDED)
        {
          TRACE_MSG(TRACE_ZCL3, "clear reporting info", (FMT__0));

          /* Remember that we don't send reports by default */
          ZB_ZCL_CLR_ALL_REPORTING_FLAGS(rep_info);
          ZB_ZCL_SET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_SLOT_BUSY);
          ZB_ZCL_SET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_STOP);
        }
#ifdef ZB_USE_NVRAM
        /* WARNING: This function may be called from zb_zcl_put_default_reporting_info_for_cluster()
         * BEFORE NVRAM init, in such case we do not need to write anything to NVRAM.
         * This is default reporting configuration inited every startup, so it is ok NOT to store
         * it in NVRAM.
         * Note that it WILL be stored (or overwritten and stored) if application (locally or
         * remotely via ZCL API) will configure the reporting (this will be done AFTER NVRAM
         * init). This is also ok - default reporting will be CONFIGURED to defaults here (before
         * NVRAM init) and than OVERWRITTEN by NVRAM loading procedure. */
        if (ZB_NVRAM().inited)
        {
          /* If we fail, trace is given and assertion is triggered */
          (void)zb_nvram_write_dataset(ZB_NVRAM_ZCL_REPORTING_DATA);
        }
#endif
        if (!ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_STOP))
        {
          zb_zcl_update_reporting_info(rep_info);
          zb_zcl_adjust_reporting_timer(0);
          /* Mark attr for reporting - we need to send first report with min interval. */
          /* [AN] 3/3/2020 maybe we should report only in case, when maximum interval is non-zero*/
          if (rep_info->u.send_info.max_interval)
          {
            zb_zcl_mark_attr_for_reporting_manuf(rep_info->ep, rep_info->cluster_id, rep_info->cluster_role, rep_info->attr_id, rep_info->manuf_code);
          }
        }
      }
      else
      {
        TRACE_MSG(TRACE_ZCL3, "accept wait reporting timeout %d", (FMT__D, rep_info->u.recv_info.timeout));
        start_wait_reporting_timer(rep_info);

        if (rep_info->u.recv_info.timeout == ZB_ZCL_TIMEOUT_ZERO)
        {
          TRACE_MSG(TRACE_ZCL3, "cancel wait reporting", (FMT__0));
          ZB_ZCL_CLR_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_SLOT_BUSY);
        }
      }
    }
    else
    {
      TRACE_MSG(TRACE_ERROR, "Error, no free slots for reporting info", (FMT__0));
      return RET_NO_MEMORY/*  no memory */;
    }
  }
  else
  {
    TRACE_MSG(TRACE_ERROR, "Error, attribute not found", (FMT__0));
    return RET_NOT_FOUND;
  }
/* 05/28/15 NK:CR }*/

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_put_reporting_info", (FMT__0));
  return RET_OK /* success */;
}

void zb_zcl_put_default_reporting_info_for_cluster(zb_uint8_t endpoint, zb_uint16_t profile_id, struct zb_zcl_cluster_desc_s *cluster_desc)
{
  zb_zcl_reporting_info_t rep_info;
  zb_zcl_attr_t *attr_desc;
  zb_uint16_t i = 0;

  ZB_ASSERT(cluster_desc);

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_put_default_reporting_info_for_cluster cluster_id 0x%x", (FMT__D, cluster_desc->cluster_id));

  attr_desc = cluster_desc->attr_desc_list;

  if (attr_desc)
  {
    ZB_BZERO(&rep_info, sizeof(rep_info));

    rep_info.direction = ZB_ZCL_CONFIGURE_REPORTING_SEND_REPORT;
    rep_info.ep = endpoint;
    rep_info.cluster_id = cluster_desc->cluster_id;
    rep_info.cluster_role = cluster_desc->role_mask;
    rep_info.dst.profile_id = profile_id;

    rep_info.u.send_info.def_min_interval = ZB_ZCL_MIN_REPORTING_INTERVAL_DEFAULT;
    rep_info.u.send_info.def_max_interval = ZB_ZCL_MAX_REPORTING_INTERVAL_DEFAULT;

    ZB_BZERO(&rep_info.u.send_info.delta, sizeof(union zb_zcl_attr_var_u));

    while (attr_desc)
    {
      if (attr_desc->access & ZB_ZCL_ATTR_ACCESS_REPORTING)
      {
        TRACE_MSG(TRACE_ZCL3, "Attribute with id: 0x%x, manuf_code 0x%x, supports reporting, configure...",
                  (FMT__D_D, attr_desc->id, attr_desc->manuf_code));

        rep_info.attr_id = attr_desc->id;
        rep_info.manuf_code = attr_desc->manuf_code;

        if (zb_zcl_is_analog_data_type(attr_desc->type))
        {
          ZB_BZERO(&rep_info.u.send_info.delta,
                   zb_zcl_get_analog_attribute_size(attr_desc->type));
        }

        zb_zcl_put_reporting_info(&rep_info, ZB_FALSE);
      }

      i++;
      attr_desc++;
      if (i >= cluster_desc->attr_count)
      {
        attr_desc = NULL;
        break;
      }
    }
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_put_default_reporting_info_for_cluster", (FMT__0));
}

/** @internal @brief Put reporting information to device contexts
    @param cmd_info - command information
    @param config_rep_req - configure reporting command
    @return RET_OK on success, error code otherwise
*/

zb_ret_t zb_zcl_put_reporting_info_from_req(zb_zcl_configure_reporting_req_t *config_rep_req,
                                            zb_zcl_attr_addr_info_t* attr_addr_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_reporting_info_t *rep_info;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_put_reporting_info_from_req config_rep_req %p",
            (FMT__P, config_rep_req));

  ZB_ASSERT(config_rep_req);
  ZB_ASSERT(attr_addr_info);

  /* search for already registered configure reporting record */
  rep_info = zb_zcl_find_reporting_info_manuf(
      attr_addr_info->src_ep,
      attr_addr_info->cluster_id,
      attr_addr_info->cluster_role,
      config_rep_req->attr_id,
      attr_addr_info->manuf_code);

  if (!rep_info)
  {
    /* get free slot to store configure reporting record */
    rep_info = get_new_reporting_info(attr_addr_info->src_ep);
    if (rep_info)
    {
      ZB_BZERO(rep_info, sizeof(zb_zcl_reporting_info_t));
    }
  }

  TRACE_MSG(TRACE_ZCL3, "rep_info %p", (FMT__P, rep_info));
  if (rep_info)
  {
    /* Clear old reporting flags (if any) */
    ZB_ZCL_CLR_ALL_REPORTING_FLAGS(rep_info);

    ZB_ZCL_SET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_SLOT_BUSY);
    /* We can't use 0xff as reported value, it can be default value for reportable attribute.
       E.g RMSVoltage attribute of Electrical measurement cluster
       So, let's use ZB_ZCL_REPORT_IS_FIRST flag for this purpose
    */
    ZB_ZCL_SET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_IS_FIRST);
    rep_info->direction = config_rep_req->direction;
    rep_info->ep = attr_addr_info->src_ep;
      TRACE_MSG(TRACE_ZCL3, "put_reporting_info ep %i", (FMT__H, rep_info->ep));
    rep_info->cluster_id = attr_addr_info->cluster_id;
    rep_info->cluster_role = attr_addr_info->cluster_role;
    rep_info->attr_id = config_rep_req->attr_id;

    rep_info->dst.short_addr = attr_addr_info->dst_short_addr;
    rep_info->dst.endpoint = attr_addr_info->dst_ep;
    rep_info->dst.profile_id = attr_addr_info->profile_id;

    TRACE_MSG(TRACE_ZCL3, "direction %hd", (FMT__H, rep_info->direction));
    if (rep_info->direction == ZB_ZCL_CONFIGURE_REPORTING_SEND_REPORT)
    {
      /*
        ZCL 6.0 2.5.7.1.6: Maximum Reporting Interval Field
        If this value is set to 0x0000 and the minimum reporting interval field equals 0xffff,
        then the device SHALL revert back to its default reporting configuration.
        The reportable change field, if present, SHALL be set to zero.
      */
      /*
        Report records are not deleted now, when report is not needed,
        REPORT_STOP is set instead to save default configuration;
        that's why we suppose default configs to be actual here
      */
      if (config_rep_req->u.clnt.min_interval == ZB_ZCL_REPORTING_NOT_NEEDED
          && config_rep_req->u.clnt.max_interval == ZB_ZCL_TIMEOUT_ZERO)
      {
        TRACE_MSG(TRACE_ZCL3, "restore default reporting config", (FMT__0));

        rep_info->u.send_info.min_interval = rep_info->u.send_info.def_min_interval;
        rep_info->u.send_info.max_interval = rep_info->u.send_info.def_max_interval;

        if (zb_zcl_is_analog_data_type(config_rep_req->u.clnt.attr_type))
        {
          ZB_BZERO(&rep_info->u.send_info.delta,
                   zb_zcl_get_analog_attribute_size(config_rep_req->u.clnt.attr_type));
        }

        TRACE_MSG(TRACE_ZCL3, "min interval %d max interval %d", (FMT__D_D, rep_info->u.send_info.def_min_interval, rep_info->u.send_info.def_max_interval));
      }
      else
      {
        rep_info->u.send_info.min_interval = config_rep_req->u.clnt.min_interval;
        rep_info->u.send_info.max_interval = config_rep_req->u.clnt.max_interval;

        if (zb_zcl_is_analog_data_type(config_rep_req->u.clnt.attr_type))
        {
          ZB_MEMCPY(&rep_info->u.send_info.delta, config_rep_req->u.clnt.delta,
                    zb_zcl_get_analog_attribute_size(config_rep_req->u.clnt.attr_type));
        }
      }


      if (rep_info->u.send_info.max_interval == ZB_ZCL_REPORTING_NOT_NEEDED)
      {
        TRACE_MSG(TRACE_ZCL3, "clear reporting info", (FMT__0));
        ZB_ZCL_SET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_STOP);
      }
      else
      {
        ZB_ZCL_CLR_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_STOP);
        zb_zcl_update_reporting_info(rep_info);
        zb_zcl_adjust_reporting_timer(0);
        /* Mark attr for reporting - we need to send first report with min interval. */
        /* [AN] 3/3/2020 maybe we should report only in case, when maximum interval is non-zero*/
        if (rep_info->u.send_info.max_interval)
        {
          zb_zcl_mark_attr_for_reporting_manuf(rep_info->ep, rep_info->cluster_id, rep_info->cluster_role, rep_info->attr_id, rep_info->manuf_code);
        }
      }
#ifdef ZB_USE_NVRAM
      /* If we fail, trace is given and assertion is triggered */
      (void)zb_nvram_write_dataset(ZB_NVRAM_ZCL_REPORTING_DATA);
#endif
    }
    else
    {
      TRACE_MSG(TRACE_ZCL3, "accept wait reporting timeout %d", (FMT__D, rep_info->u.recv_info.timeout));
      rep_info->u.recv_info.timeout = config_rep_req->u.srv.timeout;
      start_wait_reporting_timer(rep_info);

      if (config_rep_req->u.srv.timeout == ZB_ZCL_TIMEOUT_ZERO)
      {
        TRACE_MSG(TRACE_ZCL3, "cancel wait reporting", (FMT__0));
        ZB_ZCL_CLR_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_SLOT_BUSY);
      }
    }
    /* NK: TODO: Call reporting_cb here - configure some parameters on device if needed. */
  }
  else
  {
    TRACE_MSG(TRACE_ERROR, "Error, no free slots for reporting info", (FMT__0));
    ret = RET_NO_MEMORY;
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_put_reporting_info_from_req ret %hd", (FMT__H, ret));
  return ret;
}

/*! Returns slot number of the reporting info */
zb_uint8_t zb_zcl_reporting_slot_number(zb_zcl_reporting_info_t *rep_info)
{
  /* assign maxuint to return  ZB_ZCL_UNDEFINED_REPORTING_SLOT if no ctx/arg */
  zb_uindex_t i = (zb_uindex_t)~0;
  zb_uint8_t slot_num = 0;

  if (rep_info && ZCL_CTX().device_ctx)
  {
    for (i = 0; i < ZCL_CTX().device_ctx->ep_count; i++)
    {
      if (rep_info->ep != ZCL_CTX().device_ctx->ep_desc_list[i]->ep_id)
      {
        slot_num += ZB_ZCL_MAX_REPORTING_SLOTS_BY_EP; /* ZCL_CTX().device_ctx->ep_desc_list[i]->rep_info_count */
      }
      else
      {
        slot_num += rep_info - ZCL_CTX().device_ctx->ep_desc_list[i]->reporting_info;
        break;
      }
    }
  }

  return (i < ZCL_CTX().device_ctx->ep_count) ? slot_num : ZB_ZCL_UNDEFINED_REPORTING_SLOT;
}

/*! Returns reporting info that is stored in slot with specified number */
zb_zcl_reporting_info_t *zb_zcl_get_reporting_info(zb_uint8_t slot_number)
{
  zb_uindex_t i;

  if (slot_number != ZB_ZCL_UNDEFINED_REPORTING_SLOT && ZCL_CTX().device_ctx)
  {
    for (i = 0; i < ZCL_CTX().device_ctx->ep_count; i++)
    {
      if (slot_number >= ZB_ZCL_MAX_REPORTING_SLOTS_BY_EP)
      {
        slot_number -= ZB_ZCL_MAX_REPORTING_SLOTS_BY_EP;
      }
      else
      {
        return (zb_zcl_reporting_info_t *)(ZCL_CTX().device_ctx->ep_desc_list[i]->reporting_info + slot_number);
      }
    }
  }

  return NULL;
}

static void start_wait_reporting_timer(zb_zcl_reporting_info_t *rep_info)
{
  TRACE_MSG(TRACE_ZCL1, "start_wait_reporting_timer %p, timeout %d",
            (FMT__P_D, rep_info, rep_info->u.recv_info.timeout));

  ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_wait_reporting_timeout, zb_zcl_reporting_slot_number(rep_info));

  if (rep_info->u.recv_info.timeout != ZB_ZCL_TIMEOUT_ZERO)
  {
    ZB_SCHEDULE_ALARM(zb_zcl_wait_reporting_timeout, zb_zcl_reporting_slot_number(rep_info),
                      rep_info->u.recv_info.timeout * ZB_TIME_ONE_SECOND);
  }
}

void zb_zcl_update_reporting_info(zb_zcl_reporting_info_t *rep_info)
{
  zb_uint16_t delay_time = ZB_ZCL_INVALID_TIMEOUT;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_update_reporting_info rep_info %p, min_interval %d, max_interval %d ep %hd",
            (FMT__P_D_D_H, rep_info, rep_info->u.send_info.min_interval, rep_info->u.send_info.max_interval, rep_info->ep));

  /* Firstly, check minimal interval */
  if (rep_info->u.send_info.min_interval != ZB_ZCL_TIMEOUT_ZERO)
  {
    TRACE_MSG(TRACE_ZCL2, "delay for min_interval", (FMT__0));
    delay_time = rep_info->u.send_info.min_interval;
  }
  else
  {
    ZB_ZCL_SET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_IS_ALLOWED);

    /* Check ZB_ZCL_REPORT_ATTR flag -> it may be already set
     * while waiting for reporting buffer freeing */
    if (!ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_ATTR))
    {
      if (rep_info->u.send_info.max_interval != ZB_ZCL_TIMEOUT_ZERO)
      {
        TRACE_MSG(TRACE_ZCL2, "delay for max_interval", (FMT__0));
        delay_time = rep_info->u.send_info.max_interval;
      }
      else
      {
        /* Non-periodic reporting - do nothing */
        TRACE_MSG(TRACE_ZCL2, "Non-periodic rep_info", (FMT__0));
      }
    }
    else
    {
      /* Do not reschedule zb_zcl_report_attr() - it'll be rescheduled
       * in zb_zcl_reporting_cb */
      TRACE_MSG(TRACE_ZCL2, "Non-periodic rep_info", (FMT__0));
    }
  }

  TRACE_MSG(TRACE_ZCL1, "delay for %d seconds", (FMT__D, delay_time));
  if (delay_time != ZB_ZCL_INVALID_TIMEOUT)
  {
    zb_time_t t;

    t = ZB_TIMER_GET();
    ZB_ZCL_SET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_TIMER_STARTED);
    rep_info->run_time = ZB_TIME_ADD(t, delay_time*ZB_TIME_ONE_SECOND);
    TRACE_MSG(TRACE_ZCL1, "rep_info->run_time %ld", (FMT__L, rep_info->run_time));
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_update_reporting_info", (FMT__0));
}

void zb_zcl_adjust_reporting_timer(zb_uint8_t param)
{
  zb_bool_t reschedule = ZB_FALSE;
  zb_zcl_reporting_info_t *rep_info = NULL;
  zb_time_t next_event_time = 0;
  zb_uindex_t i, j;

  ZVUNUSED(param);

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_adjust_reporting_timer", (FMT__0));

  for (j = 0; j < ZCL_CTX().device_ctx->ep_count; j++)
  {
    if (ZCL_CTX().device_ctx->ep_desc_list[j]->reporting_info)
    {
      rep_info = ZCL_CTX().device_ctx->ep_desc_list[j]->reporting_info;
      for (i = 0; i < ZCL_CTX().device_ctx->ep_desc_list[j]->rep_info_count; i++)
      {
        if (ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_SLOT_BUSY) &&
            !ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_STOP) &&
            ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_TIMER_STARTED))
        {
          if (!reschedule)
          {
            /* The easiest way to find first entry */
            next_event_time = rep_info->run_time;
            reschedule = ZB_TRUE;
          }

          /* Find the minimal schedule-time */
          if (ZB_TIME_GE(next_event_time, rep_info->run_time))
          {
            next_event_time = rep_info->run_time;
          }
        }
        rep_info++;
      }
    }
  }

  TRACE_MSG(TRACE_ZCL1, "reschedule %hd, next_event_time %ld", (FMT__H_L, reschedule, next_event_time));

  if (reschedule)
  {
    zb_time_t t;

    t = ZB_TIMER_GET();
    ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_reporting_timer_handler, ZB_ALARM_ALL_CB);
    /* If next_event_time is in the past, run immediately. */
    if (ZB_TIME_GE(t, next_event_time))
    {
      ZB_SCHEDULE_CALLBACK(zb_zcl_reporting_timer_handler, 0);
    }
    else
    {
      ZB_SCHEDULE_ALARM(zb_zcl_reporting_timer_handler, 0, ZB_TIME_SUBTRACT(next_event_time, t));
    }
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_adjust_reporting_timer", (FMT__0));
}

void zb_zcl_reporting_timer_handler(zb_uint8_t param)
{
  zb_zcl_reporting_info_t *rep_info = NULL;
  zb_time_t t;
  zb_uint8_t rep_count = 0;
  zb_uindex_t i, j;

  ZVUNUSED(param);

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_reporting_timer_handler", (FMT__0));

  t = ZB_TIMER_GET();

  /* We're here because some event has been occurred. Need to analyse it */
  for (j = 0; j < ZCL_CTX().device_ctx->ep_count; j++)
  {
    if (ZCL_CTX().device_ctx->ep_desc_list[j]->reporting_info)
    {
      rep_info = ZCL_CTX().device_ctx->ep_desc_list[j]->reporting_info;
      for (i = 0; i < ZCL_CTX().device_ctx->ep_desc_list[j]->rep_info_count; i++)
      {
        if (ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_SLOT_BUSY) &&
            !ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_STOP) &&
            ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_TIMER_STARTED) &&
            ZB_TIME_GE(t, rep_info->run_time))
        {
          /* Get the reason */
          if (!ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_IS_ALLOWED))
          {
            /* Minimal interval expired - mark attribute as allowed to
             * be reported */
            TRACE_MSG(TRACE_ZCL1, "min_interval expired rep_info %p", (FMT__P));

            ZB_ZCL_SET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_IS_ALLOWED);

            /* If pending attributes exists, mark to report and do not schedule
             * max_interval event */
            if (ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_ATTR))
            {
              TRACE_MSG(TRACE_ZCL1, "handle pending report", (FMT__0));
              rep_count++;
            }
            else
            {
              if (rep_info->u.send_info.max_interval != ZB_ZCL_TIMEOUT_ZERO)
              {
                zb_uint16_t delay_time;

                delay_time = rep_info->u.send_info.max_interval - rep_info->u.send_info.min_interval;
                rep_info->run_time = ZB_TIME_ADD(t, delay_time*ZB_TIME_ONE_SECOND);
                TRACE_MSG(TRACE_ZCL1, "Schedule max_interval timer, delay %d seconds, run_time %ld",
                          (FMT__D_L, delay_time, rep_info->run_time));
              }
              else
              {
                /* Clear timed callback - reporting flag */
                /* TODO: DEBUG: when minimum interval is expired BEFORE binding, this record will
                 * NEVER be reported! */
                TRACE_MSG(TRACE_ZCL1, "Zero max_interval time, skip", (FMT__0));
                ZB_ZCL_CLR_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_TIMER_STARTED);
              }
            }
          }
          else
          {
            /* Maximum interval expired - mark attribute to be reported */
            TRACE_MSG(TRACE_ZCL1, "max_interval expired rep_info %p", (FMT__P));

            ZB_ZCL_SET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_ATTR);
            ZB_ZCL_CLR_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_TIMER_STARTED);

            rep_count++;
          }
        }

        rep_info++;
      }
    }
  }

  TRACE_MSG(TRACE_ZCL1, "%hd entries to report", (FMT__H, rep_count));
  if (rep_count)
  {
    TRACE_MSG(TRACE_ZCL1, "Schedule next report attribute rep_count %hd", (FMT__H, rep_count));
    ZB_SCHEDULE_CALLBACK(zb_zcl_report_attr, 0);
  }

  zb_zcl_adjust_reporting_timer(0);

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_reporting_timer_handler", (FMT__0));
}

/**
   Returns reporting info for specified attribute
 */
zb_zcl_reporting_info_t* zb_zcl_find_reporting_info(  zb_uint8_t ep,
                                                      zb_uint16_t cluster_id,
                                                      zb_uint8_t cluster_role,
                                                      zb_uint16_t attr_id)
{
  zb_zcl_reporting_info_t *rep_info;
  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_find_reporting_info, ep %hd, cluster %d, attr_id 0x%x, cluster_role %hd",
            (FMT__H_D_D_D, ep, cluster_id, attr_id, cluster_role));
  rep_info = zb_zcl_find_reporting_info_manuf(ep, cluster_id, cluster_role, attr_id, ZB_ZCL_NON_MANUFACTURER_SPECIFIC);
  TRACE_MSG(TRACE_ZCL2, "<< zb_zcl_find_reporting_info %p", (FMT__P, rep_info));
  return rep_info;
}


zb_zcl_reporting_info_t* zb_zcl_find_reporting_info_manuf(  zb_uint8_t ep,
                                                            zb_uint16_t cluster_id,
                                                            zb_uint8_t cluster_role,
                                                            zb_uint16_t attr_id,
                                                            zb_uint16_t manuf_code)
{
  zb_zcl_reporting_info_t *rep_info = NULL;
  zb_uindex_t i, j;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_find_reporting_info_manuf, ep %hd, cluster %d, attr_id 0x%x, manuf_code 0x%x, cluster_role %hd",
            (FMT__H_D_D_D_D, ep, cluster_id, attr_id, manuf_code, cluster_role));
  if (ZCL_CTX().device_ctx)
  {
    for (j = 0; j < ZCL_CTX().device_ctx->ep_count; j++)
    {
      if (ZCL_CTX().device_ctx->ep_desc_list[j]->reporting_info)
      {
        TRACE_MSG(TRACE_ZCL1, ">>  ZCL_CTX().device_ctx->rep_info_count = %d",
                  (FMT__D, ZCL_CTX().device_ctx->ep_desc_list[j]->rep_info_count));
        rep_info = ZCL_CTX().device_ctx->ep_desc_list[j]->reporting_info;
        TRACE_MSG(TRACE_ZCL1, ">>  rep_info = %p",(FMT__P, rep_info));
        for (i = 0; i < ZCL_CTX().device_ctx->ep_desc_list[j]->rep_info_count; i++)
        {
          TRACE_MSG(TRACE_ZCL1, ">>  rep_info->ep %d, rep_info->cluster_id %d, rep_info->attr_id 0x%x, manuf_code 0x%x, cluster_role %d, rep_flags %d)",
                    (FMT__D_D_D_D_D_D, rep_info->ep,rep_info->cluster_id,rep_info->attr_id,rep_info->manuf_code,rep_info->cluster_role,rep_info->flags));
          TRACE_MSG(TRACE_ZCL1, "    max_interval %d min_interval %d",(FMT__D_D, rep_info->u.send_info.max_interval,
                                                                       rep_info->u.send_info.min_interval));

          if ((rep_info->ep == ep) &&
              (rep_info->cluster_id == cluster_id) &&
              (rep_info->cluster_role == cluster_role) &&
              (rep_info->attr_id == attr_id) &&
              (rep_info->manuf_code == manuf_code) &&
              ZB_ZCL_GET_REPORTING_FLAG(rep_info,ZB_ZCL_REPORTING_SLOT_BUSY))
          {
            TRACE_MSG(TRACE_ZCL1, "an entry is found! i == %hd, max %hd", (FMT__H_H, i, ZCL_CTX().device_ctx->ep_desc_list[j]->rep_info_count));
            break;
          }
          rep_info++;
        }
        if (i >= ZCL_CTX().device_ctx->ep_desc_list[j]->rep_info_count)
        {
          TRACE_MSG(TRACE_ZCL2, ">>  if (i >= ZCL_CTX().device_ctx->rep_info_count)",
                    (FMT__0));
          rep_info = NULL;
        }
        /* If rep_info was successfully found, break outer loop to return from the function. */
        else
        {
          break;
        }
      }
    }
  }

  TRACE_MSG(TRACE_ZCL2, "<< zb_zcl_find_reporting_info_manuf %p", (FMT__P, rep_info));
  return rep_info;
}


static zb_zcl_reporting_info_t* get_new_reporting_info(zb_uint8_t endpoint_id)
{
  zb_zcl_reporting_info_t *rep_info = NULL;
  zb_uindex_t i, j;

  TRACE_MSG(TRACE_ZCL1, ">> get_new_reporting_info",(FMT__0));
  if (ZCL_CTX().device_ctx)
  {
    for (j = 0; j < ZCL_CTX().device_ctx->ep_count; j++)
    {
      if (ZCL_CTX().device_ctx->ep_desc_list[j]->ep_id == endpoint_id &&
          ZCL_CTX().device_ctx->ep_desc_list[j]->reporting_info)
      {
        rep_info = ZCL_CTX().device_ctx->ep_desc_list[j]->reporting_info;
        for (i = 0; i < ZCL_CTX().device_ctx->ep_desc_list[j]->rep_info_count; i++)
        {
          if (!(rep_info->flags & ZB_ZCL_REPORTING_SLOT_BUSY))
          {
            break;
          }
          rep_info++;
        }
        if (i >= ZCL_CTX().device_ctx->ep_desc_list[j]->rep_info_count)
        {
          rep_info = NULL;
        }
      }
    }
  }

  TRACE_MSG(TRACE_ZCL1, "<< get_new_reporting_info %p", (FMT__P, rep_info));
  return rep_info;
}


void zb_zcl_report_attr(zb_uint8_t param)
{
  zb_uindex_t i, j;
  zb_bool_t report_sent = ZB_FALSE;
  zb_zcl_reporting_info_t *rep_info;

  ZVUNUSED(param);

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_report_attr buf_ref %hd", (FMT__H, ZCL_CTX().reporting_ctx.buf_ref));

  if (ZCL_CTX().device_ctx)
  {
    for (j = 0; j < ZCL_CTX().device_ctx->ep_count; j++)
    {
      if (ZCL_CTX().device_ctx->ep_desc_list[j]->reporting_info)
      {
        rep_info = ZCL_CTX().device_ctx->ep_desc_list[j]->reporting_info;
        for (i = 0; i < ZCL_CTX().device_ctx->ep_desc_list[j]->rep_info_count; i++)
        {
          TRACE_MSG(TRACE_ZCL1, "rep_info %p, flags %hx", (FMT__P_H, rep_info, rep_info->flags));
          TRACE_MSG(TRACE_ZCL1, "attr_id 0x%x, manuf_code 0x%x", (FMT__D_D, rep_info->attr_id, rep_info->manuf_code));

          if (ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_ATTR) &&
              ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_IS_ALLOWED) &&
              !ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_STOP))
          {
              if (ZCL_CTX().reporting_ctx.buf_ref != ZB_UNDEFINED_BUFFER)
              {
                TRACE_MSG(TRACE_ZCL1, "buffer is free, send report", (FMT__0));

                /* Moved to zb_zcl_send_report_attr_command() - to properly reset flags for each
                   rep_info independently (if there are 2 attributes to report in one cluster). */
                /* ZB_ZCL_CLR_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_ATTR); */
                /* ZB_ZCL_CLR_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_IS_ALLOWED); */
                /* ZB_ZCL_CLR_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_TIMER_STARTED); */
                /* ZB_ZCL_SET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_IS_SENT); */

                zb_zcl_send_report_attr_command(rep_info, ZCL_CTX().reporting_ctx.buf_ref);
                ZCL_CTX().reporting_ctx.buf_ref = ZB_UNDEFINED_BUFFER;

                report_sent = ZB_TRUE;

                break;
              }
              else
              {
                /* Report buffer is in use. Retry sending on cb */
                TRACE_MSG(TRACE_ZCL1, "buffer is in use, skip", (FMT__0));
              }
          }
          rep_info++;
        }
      }
    }
  }

  if (!report_sent)
  {
    TRACE_MSG(TRACE_ZCL1, "report is not sent, skip", (FMT__0));
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_report_attr", (FMT__0));
}


/* @internal @brief Function returns next reporting info that should be reported.
 *
 * Criteria: it should be sent to the same device and to the same cluster, as specified by
 * @ref rep_info_init
 */
zb_zcl_reporting_info_t* zb_zcl_get_next_reporting_info(zb_zcl_reporting_info_t *rep_info_init, zb_uint8_t is_manuf_spec)
{
  zb_zcl_reporting_info_t *rep_info = NULL;
  zb_zcl_attr_t *attr_desc;
  zb_uint8_t attr_manuf_spec;
  zb_uindex_t i, j;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_get_next_reporting_info, rep_info_init %p, is_manuf_spec %hd",
            (FMT__P_D, rep_info_init, is_manuf_spec));

  if (ZCL_CTX().device_ctx)
  {
    /* Should look here only on the same endpoint */
    j = 0;
    while (j < ZCL_CTX().device_ctx->ep_count
           && ZCL_CTX().device_ctx->ep_desc_list[j]->ep_id != rep_info_init->ep)
    {
      ++j;
    }
    ZB_ASSERT(j < ZCL_CTX().device_ctx->ep_count);

    if (ZCL_CTX().device_ctx->ep_desc_list[j]->reporting_info)
    {
      rep_info = ZCL_CTX().device_ctx->ep_desc_list[j]->reporting_info;

      for (i = 0; i < ZCL_CTX().device_ctx->ep_desc_list[j]->rep_info_count; i++)
      {
        TRACE_MSG(TRACE_ZCL3, "check next rep info %p", (FMT__P, rep_info));

        if (ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_SLOT_BUSY))
        {

          attr_desc =
            zb_zcl_get_attr_desc_manuf_a(rep_info->ep, rep_info->cluster_id, rep_info->cluster_role, rep_info->attr_id, rep_info->manuf_code);
          attr_manuf_spec = !!ZB_ZCL_IS_ATTR_MANUF_SPEC(attr_desc);

          if (ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_ATTR) &&
              ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_IS_ALLOWED) &&
              !ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_STOP) &&
              rep_info->dst.short_addr == rep_info_init->dst.short_addr &&
              rep_info->dst.endpoint == rep_info_init->dst.endpoint &&
              rep_info->dst.profile_id == rep_info_init->dst.profile_id &&
              rep_info->ep == rep_info_init->ep &&
              rep_info->cluster_id == rep_info_init->cluster_id &&
              !(attr_manuf_spec ^ is_manuf_spec))
          {
            TRACE_MSG(TRACE_ZCL3, "found new %p", (FMT__P, rep_info));
            ZB_ZCL_CLR_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_ATTR);
            ZB_ZCL_CLR_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_IS_ALLOWED);
            ZB_ZCL_SET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_IS_SENT);

            break;
          }
        }
        rep_info++;
      }
      if (i >= ZCL_CTX().device_ctx->ep_desc_list[j]->rep_info_count)
      {
        rep_info = NULL;
      }
    }
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_get_next_reporting_info, ret %p", (FMT__P, rep_info));
  return rep_info;
}

void zb_zcl_mark_report_not_sent(zb_zcl_reporting_info_t *rep_info)
{
  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_mark_report_not_sent, rep_info %p", (FMT__P, rep_info));

  if (rep_info)
  {
    /* Restore reporting flags - try to the attribute them next time */
    ZB_ZCL_SET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_ATTR);
    ZB_ZCL_SET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_IS_ALLOWED);
    ZB_ZCL_CLR_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_IS_SENT);
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_mark_report_not_sent", (FMT__0));
}

void zb_zcl_wait_reporting_timeout(zb_uint8_t param)
{
  zb_zcl_reporting_info_t *rep_info = zb_zcl_get_reporting_info(param);

  TRACE_MSG(TRACE_ZCL1, "zb_zcl_wait_reporting_timeout param %hd, rep_info %p",
            (FMT__H_P, param, rep_info));

  if (ZCL_CTX().no_reporting_cb)
  {
    TRACE_MSG(TRACE_ZCL3, "inform application - no report cb %p", (FMT__P, ZCL_CTX().no_reporting_cb));
    (*ZCL_CTX().no_reporting_cb)(rep_info->dst.endpoint, rep_info->cluster_id, rep_info->attr_id);
  }

  if (ZCL_CTX().reporting_ctx.min_interval != ZB_ZCL_TIMEOUT_ZERO)
  {
    TRACE_MSG(TRACE_ZCL3, "start wait reporting again", (FMT__0));
    ZB_SCHEDULE_ALARM(zb_zcl_wait_reporting_timeout, param,
                      rep_info->u.recv_info.timeout * ZB_TIME_ONE_SECOND);
  }
}


/** @internal @brief Function should be called by application on receiving attribute
    report to inform ZCL that report is received.
    @param ep - endpoint number
    @param cluster_id - cluster ID
    @param attr_id - attribute ID
 */
void zb_zcl_report_received(zb_uint8_t ep, zb_uint16_t cluster_id, zb_uint8_t cluster_role, zb_uint16_t attr_id)
{
  TRACE_MSG(TRACE_ZCL1, ">>zb_zcl_report_received ep %hd, cluster_id 0x%x, cluster_role %d, attr_id 0x%x",
            (FMT__H_D_D_D, ep, cluster_id, cluster_role, attr_id));
  zb_zcl_report_received_manuf(ep, cluster_id, cluster_role, attr_id, ZB_ZCL_NON_MANUFACTURER_SPECIFIC);
  TRACE_MSG(TRACE_ZCL1, "<<zb_zcl_report_received", (FMT__0));
}

/** @internal @brief Function should be called by application on receiving attribute
    report to inform ZCL that report is received.
    @param ep - endpoint number
    @param cluster_id - cluster ID
    @param attr_id - attribute ID
 */
void zb_zcl_report_received_manuf(zb_uint8_t ep, zb_uint16_t cluster_id, zb_uint8_t cluster_role, zb_uint16_t attr_id, zb_uint16_t manuf_code)
{
  zb_zcl_reporting_info_t *rep_info;

  TRACE_MSG(TRACE_ZCL1, ">>zb_zcl_report_received_manuf ep %hd, cluster_id 0x%x, cluster_role %d, attr_id 0x%x, manuf_code 0x%x",
            (FMT__H_D_D_D_D, ep, cluster_id, cluster_role, attr_id, manuf_code));
  rep_info = zb_zcl_find_reporting_info_manuf(ep, cluster_id, cluster_role, attr_id, manuf_code);
  if (rep_info)
  {
    TRACE_MSG(TRACE_ZCL3, "restart wait report timer", (FMT__0));
    start_wait_reporting_timer(rep_info);
  }
  TRACE_MSG(TRACE_ZCL1, "<<zb_zcl_report_received_manuf", (FMT__0));
}


#ifdef EXCLUDED_FROM_BUILD
/* Function sets flags: report-is-needed or report-is-allowed for all
 * active reporting info records */
static zb_uint8_t zb_zcl_mark_all_for_reporting(zb_uint8_t flag)
{
  zb_uindex_t i;
  zb_zcl_reporting_info_t *rep_info;
  zb_uint8_t counter = 0;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_mark_all_for_reporting, flag %hx", (FMT__H, flag));

  if (ZCL_CTX().device_ctx && ZCL_CTX().device_ctx->reporting_info)
  {
    rep_info = ZCL_CTX().device_ctx->reporting_info;
    for (i = 0; i < ZCL_CTX().device_ctx->rep_info_count; i++)
    {
      TRACE_MSG(TRACE_ZCL3, "next rep info %p, flags %hx", (FMT__P_H, rep_info, rep_info->flags));
      if (ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_SLOT_BUSY))
      {
        /* DO NOT check for min_interval here, check it only when
         * setting report-is-needed because of attr value change */

        TRACE_MSG(TRACE_ZCL3, "set flag value", (FMT__0));
        ZB_ZCL_SET_REPORTING_FLAG(rep_info, flag);
        counter++;
      }
      rep_info++;
    }
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_mark_all_for_reporting ret %hd", (FMT__H, counter));
  return counter;
}
#endif


void zb_zcl_save_reported_value(zb_zcl_reporting_info_t *rep_info, zb_zcl_attr_t* attr_desc)
{
  zb_ushort_t attr_size;

  TRACE_MSG(TRACE_ZCL1, ">> save_reported_value rep_info %p, attr_desc %p",
            (FMT__P_P, rep_info, attr_desc));

  TRACE_MSG(TRACE_ZCL1, "attr type %hd", (FMT__H, attr_desc->type));

  if (zb_zcl_is_analog_data_type(attr_desc->type))
  {
    switch (attr_desc->type)
    {
      case ZB_ZCL_ATTR_TYPE_U8:
      case ZB_ZCL_ATTR_TYPE_S8:
      {
        rep_info->u.send_info.reported_value.u8 = *(zb_uint8_t*)attr_desc->data_p;
        TRACE_MSG(TRACE_ZCL3, "reported U8 %hd", (FMT__H, rep_info->u.send_info.reported_value.u8));
        break;
      }

      case ZB_ZCL_ATTR_TYPE_U16:
      {
        rep_info->u.send_info.reported_value.u16 = *(zb_uint16_t*)attr_desc->data_p;
        TRACE_MSG(TRACE_ZCL3, "reported U16 %d", (FMT__H, rep_info->u.send_info.reported_value.u16));
        break;
      }

      case ZB_ZCL_ATTR_TYPE_S16:
      {
        rep_info->u.send_info.reported_value.s16 = *(zb_int16_t*)attr_desc->data_p;
        TRACE_MSG(TRACE_ZCL3, "reported S16 %d", (FMT__H, rep_info->u.send_info.reported_value.s16));
        break;
      }

      case ZB_ZCL_ATTR_TYPE_U24:
      {
        rep_info->u.send_info.reported_value.u24 = *(zb_uint24_t*)attr_desc->data_p;
        TRACE_MSG(TRACE_ZCL3, "reported U24 %d %d", (FMT__D_D, rep_info->u.send_info.reported_value.u24.high, rep_info->u.send_info.reported_value.u24.low));
        break;
      }

      case ZB_ZCL_ATTR_TYPE_S24:
      {
        rep_info->u.send_info.reported_value.s24 = *(zb_int24_t*)attr_desc->data_p;
        TRACE_MSG(TRACE_ZCL3, "reported S24 %d %d", (FMT__D_D, rep_info->u.send_info.reported_value.s24.high, rep_info->u.send_info.reported_value.s24.low));
        break;
      }

      case ZB_ZCL_ATTR_TYPE_U32:
      {
        rep_info->u.send_info.reported_value.u32 = *(zb_uint32_t*)attr_desc->data_p;
        TRACE_MSG(TRACE_ZCL3, "reported U32 [%hd %hd %hd %hd]",
                (FMT__H_H_H_H,
                 rep_info->u.send_info.reported_value.data_buf[0], rep_info->u.send_info.reported_value.data_buf[1],
                 rep_info->u.send_info.reported_value.data_buf[2], rep_info->u.send_info.reported_value.data_buf[3]));
        break;
      }

      case ZB_ZCL_ATTR_TYPE_S32:
      {
        rep_info->u.send_info.reported_value.s32 = *(zb_int32_t*)attr_desc->data_p;
        TRACE_MSG(TRACE_ZCL3, "reported S32 [%hd %hd %hd %hd]",
                (FMT__H_H_H_H,
                 rep_info->u.send_info.reported_value.data_buf[0], rep_info->u.send_info.reported_value.data_buf[1],
                 rep_info->u.send_info.reported_value.data_buf[2], rep_info->u.send_info.reported_value.data_buf[3]));
        break;
      }

      case ZB_ZCL_ATTR_TYPE_U48:
      {
        rep_info->u.send_info.reported_value.u48 = *(zb_uint48_t*)attr_desc->data_p;
        TRACE_MSG(TRACE_ZCL3, "reported U48 %d %d", (FMT__D_D, rep_info->u.send_info.reported_value.u48.high, rep_info->u.send_info.reported_value.u48.low));
        break;
      }

      default:
        break;
    }
  } /* is analog type */
  else
  {
    TRACE_MSG(TRACE_ZCL3, "descrete attr type", (FMT__0));
    attr_size = zb_zcl_get_attribute_size(attr_desc->type, attr_desc->data_p);
    rep_info->u.send_info.reported_value.data_buf_crc32 = zb_crc32(attr_desc->data_p, attr_size);
    TRACE_MSG(TRACE_ZCL3, "reported attr size %hd, data crc32 = 0x%08x",
              (FMT__H_D, attr_size, rep_info->u.send_info.reported_value.data_buf_crc32));
  }

  TRACE_MSG(TRACE_ZCL1, "<< save_reported_value", (FMT__0));
}

static zb_bool_t check_delta_value(zb_zcl_reporting_info_t *rep_info)
{
  zb_zcl_attr_t* attr_desc;
  zb_ret_t ret = RET_EMPTY;
  zb_ushort_t attr_size;

  TRACE_MSG(TRACE_ZCL1, ">> check_delta_value %p", (FMT__P, rep_info));

  attr_desc = zb_zcl_get_attr_desc_manuf_a(rep_info->ep, rep_info->cluster_id, rep_info->cluster_role, rep_info->attr_id, rep_info->manuf_code);
  if (attr_desc)
  {
    if (zb_zcl_is_analog_data_type(attr_desc->type))
    {
      if (ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_IS_FIRST))
      {
        /* Special reported_value indicates that it is first check_delta_value() call - report any
         * value first time. */
        TRACE_MSG(TRACE_ZCL3, "First report!", (FMT__0));
        ZB_ZCL_CLR_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_IS_FIRST);
        ret = RET_OK;
      }
      else
      {
        switch (attr_desc->type)
        {
          case ZB_ZCL_ATTR_TYPE_U8:
          case ZB_ZCL_ATTR_TYPE_S8:
          {
            zb_uint8_t delta;

            delta = ZB_ABS((zb_int16_t)*(zb_uint8_t*)(attr_desc->data_p) - rep_info->u.send_info.reported_value.u8);

            TRACE_MSG(TRACE_ZCL3, "U8 delta %hd, min delta %hd", (FMT__H_H, delta, rep_info->u.send_info.delta.u8));
            ret = (delta >= rep_info->u.send_info.delta.u8)?(RET_OK ):(RET_IGNORE );
            break;
          }

          case ZB_ZCL_ATTR_TYPE_U16:
          {
            zb_uint16_t delta;

            delta = (zb_uint16_t)ZB_ABS((zb_int32_t)(*(zb_uint16_t*)(attr_desc->data_p)) - rep_info->u.send_info.reported_value.u16);

            TRACE_MSG(TRACE_ZCL3, "U16 delta %d, min delta %d", (FMT__D_D, delta, rep_info->u.send_info.delta.u16));
            ret = (delta >= rep_info->u.send_info.delta.u16)?(RET_OK ):(RET_IGNORE );
            break;
          }

          case ZB_ZCL_ATTR_TYPE_S16:
          {
            zb_int16_t delta;

            delta = ZB_ABS(*(zb_int16_t*)(attr_desc->data_p) - rep_info->u.send_info.reported_value.s16);

            TRACE_MSG(TRACE_ZCL3, "S16 delta %d, min delta %d", (FMT__D_D, delta, rep_info->u.send_info.delta.s16));
            ret = (delta >= rep_info->u.send_info.delta.s16)?(RET_OK ):(RET_IGNORE );
            break;
          }

          case ZB_ZCL_ATTR_TYPE_U24:
          {
            zb_uint32_t delta = 0,
                        delta_u24 = ZB_GET_UINT32_FROM_UINT24(rep_info->u.send_info.delta.u24);
            zb_uint32_t n1 = ZB_GET_UINT32_FROM_UINT24(*(zb_uint24_t*)(attr_desc->data_p)),
                        n2 = ZB_GET_UINT32_FROM_UINT24(rep_info->u.send_info.reported_value.u24);

            delta = (n1 > n2) ? (n1 - n2) : (n2 - n1);

            TRACE_MSG(TRACE_ZCL3, "U24 delta %ld, min delta %ld",
                      (FMT__L_L, delta, delta_u24));
            ret = (delta >= delta_u24) ? (RET_OK ):(RET_IGNORE );
            break;
          }

          case ZB_ZCL_ATTR_TYPE_S24:
          {
            zb_int32_t delta =
              ZB_ABS(ZB_GET_INT32_FROM_INT24(*(zb_int24_t*)(attr_desc->data_p)) -
                     ZB_GET_INT32_FROM_INT24(rep_info->u.send_info.reported_value.s24));

            TRACE_MSG(TRACE_ZCL3, "S24 delta %ld, min delta %ld",
                      (FMT__L_L, delta, ZB_GET_INT32_FROM_INT24(rep_info->u.send_info.delta.s24)));
            ret = (delta >= ZB_GET_INT32_FROM_INT24(rep_info->u.send_info.delta.s24)) ? (RET_OK ):(RET_IGNORE );
            break;
          }

          case ZB_ZCL_ATTR_TYPE_U32:
          {
            zb_uint32_t delta;

            if (*(zb_uint32_t*)(attr_desc->data_p) > rep_info->u.send_info.reported_value.u32)
            {
              delta = *(zb_uint32_t*)(attr_desc->data_p) - rep_info->u.send_info.reported_value.u32;
            }
            else
            {
              delta = rep_info->u.send_info.reported_value.u32 - *(zb_uint32_t*)(attr_desc->data_p);
            }

            TRACE_MSG(TRACE_ZCL3, "reported U32 [%hd %hd %hd %hd]",
                      (FMT__H_H_H_H,
                       rep_info->u.send_info.reported_value.data_buf[0], rep_info->u.send_info.reported_value.data_buf[1],
                       rep_info->u.send_info.reported_value.data_buf[2], rep_info->u.send_info.reported_value.data_buf[3]));
            ret = (delta >= rep_info->u.send_info.delta.u32)?(RET_OK ):(RET_IGNORE );
            break;
          }

          case ZB_ZCL_ATTR_TYPE_S32:
          {
            zb_int32_t delta = ZB_ABS(*(zb_int32_t*)(attr_desc->data_p) -
                                      rep_info->u.send_info.reported_value.s32);

            TRACE_MSG(TRACE_ZCL3, "reported S32 [%hd %hd %hd %hd]",
                      (FMT__H_H_H_H,
                       rep_info->u.send_info.reported_value.data_buf[0], rep_info->u.send_info.reported_value.data_buf[1],
                       rep_info->u.send_info.reported_value.data_buf[2], rep_info->u.send_info.reported_value.data_buf[3]));
            ret = (delta >= rep_info->u.send_info.delta.s32)?(RET_OK ):(RET_IGNORE );
            break;
          }


          case ZB_ZCL_ATTR_TYPE_U48:
          {
            zb_uint48_t delta = *(zb_uint48_t*)(attr_desc->data_p); /* initial value */

            TRACE_MSG(TRACE_ZCL3, "delta_cur %x %lx", (FMT__D_L, delta.high, delta.low));

            ZB_UINT48_SUB(delta, rep_info->u.send_info.reported_value.u48);

            TRACE_MSG(TRACE_ZCL3, "delta_cur %x %lx, delta_u48 %x %lx", (FMT__D_L_L_D, delta.high, delta.low, rep_info->u.send_info.delta.u48.high, rep_info->u.send_info.delta.u48.low));

            ret = ZB_UINT48_IS_GREAT_OR_EQUAL(delta, rep_info->u.send_info.delta.u48)?(RET_OK ):(RET_IGNORE );
            break;
          }

          default:
            break;
        }
      }
    } /* is analog type */

    /* Allow to work with unknown analog data types as with discrete - report any change if we can not
     * check the delta. */
    if (ret == RET_EMPTY)
    {
      TRACE_MSG(TRACE_ZCL3, "discrete/unknown analog attr type, check if value was changed or not", (FMT__0));
      attr_size = zb_zcl_get_attribute_size(attr_desc->type, attr_desc->data_p);
      if(rep_info->u.send_info.reported_value.data_buf_crc32 != zb_crc32(attr_desc->data_p, attr_size))
      {
          TRACE_MSG(TRACE_ZCL1, "Value was changed, report it", (FMT__0));
          ret = RET_OK;
      }
    }
  }
  TRACE_MSG(TRACE_ZCL1, "<< check_delta_value ret %hd", (FMT__H, ret));
  return (ret == RET_OK) ? (ZB_TRUE) : (ZB_FALSE);
}

/* marks attribute for reporting, after its value was changed */
void zb_zcl_mark_attr_for_reporting(zb_uint8_t ep, zb_uint16_t cluster_id, zb_uint8_t cluster_role, zb_uint16_t attr_id)
{
  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_mark_attr_for_reporting, ep %hd, cluster_id 0x%x, attr_id 0x%x",
            (FMT__H_D_D_D, ep, cluster_id, attr_id));
  zb_zcl_mark_attr_for_reporting_manuf(ep, cluster_id, cluster_role, attr_id, ZB_ZCL_NON_MANUFACTURER_SPECIFIC);
  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_mark_attr_for_reporting", (FMT__0));
}

void zb_zcl_mark_attr_for_reporting_manuf(zb_uint8_t ep, zb_uint16_t cluster_id, zb_uint8_t cluster_role, zb_uint16_t attr_id, zb_uint16_t manuf_code)
{
  zb_zcl_reporting_info_t *rep_info;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_mark_attr_for_reporting_manuf, ep %hd, cluster_id 0x%x, attr_id 0x%x, manuf_code 0x%x",
            (FMT__H_D_D_D, ep, cluster_id, attr_id, manuf_code));

  rep_info = zb_zcl_find_reporting_info_manuf(ep, cluster_id, cluster_role, attr_id, manuf_code);
  if (rep_info)
  {
    TRACE_MSG(TRACE_ZCL3, "min_inerval %hd, flags %hd",
              (FMT__H_H, rep_info->u.send_info.min_interval, rep_info->flags));

    /* Check for delta value */
    if (check_delta_value(rep_info))
    {
      if (ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_IS_ALLOWED))
      {
        TRACE_MSG(TRACE_ZCL3, "set reporting is needed", (FMT__0));

        /* Debug if needed, but in current circumstances it works incorrectly: if
         * max_interval expired before bind, we will have ZB_ZCL_REPORT_ATTR flag and NEVER run
         * zb_zcl_report_attr().
         * Note that zb_zcl_report_attr() has its own checks for free buffer.
         */
        if (1/* !ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_ATTR) */)
        {
          ZB_ZCL_SET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_ATTR);
          ZB_ZCL_CLR_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_TIMER_STARTED);

          TRACE_MSG(TRACE_ZCL3, "schedule reporting", (FMT__0));
          ZB_SCHEDULE_CALLBACK(zb_zcl_report_attr, 0);

          zb_zcl_adjust_reporting_timer(0);
        }
        else
        {
          /* Attribute is waiting for free buffer to be reported. No need
           * to retrigger zb_zcl_reporting_attr() function */
          TRACE_MSG(TRACE_ZCL3, "Already pending and waiting for free buffer", (FMT__0));
        }
      }
      else
      {
        TRACE_MSG(TRACE_ZCL3, "set reporting is pending", (FMT__0));
        ZB_ZCL_SET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_ATTR);
      }
    }
  }
  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_mark_attr_for_reporting_manuf", (FMT__0));
}

/* stop attribute for reporting */
zb_ret_t zb_zcl_stop_attr_reporting(zb_uint8_t ep, zb_uint16_t cluster_id, zb_uint8_t cluster_role, zb_uint16_t attr_id)
{
  zb_ret_t ret;
  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_stop_attr_for_reporting, ep %hd, cluster_id %d, attr_id %d",
            (FMT__H_D_D, ep, cluster_id, attr_id));
  ret = zb_zcl_stop_attr_reporting_manuf(ep, cluster_id, cluster_role, attr_id, ZB_ZCL_NON_MANUFACTURER_SPECIFIC);
  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_stop_attr_for_reporting %d", (FMT__H, ret));
  return ret;
}

/* stop attribute for reporting */
zb_ret_t zb_zcl_stop_attr_reporting_manuf(zb_uint8_t ep, zb_uint16_t cluster_id, zb_uint8_t cluster_role, zb_uint16_t attr_id, zb_uint16_t manuf_code)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_reporting_info_t *rep_info;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_stop_attr_for_reporting_manuf, ep %hd, cluster_id 0x%x, attr_id 0x%x, manuf_code 0x%x",
            (FMT__H_D_D_D, ep, cluster_id, attr_id, manuf_code));

  rep_info = zb_zcl_find_reporting_info_manuf(ep, cluster_id, cluster_role, attr_id, manuf_code);
  if (rep_info)
  {
    ZB_ZCL_SET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_STOP);
  }
  else
  {
    ret = RET_NOT_FOUND;
  }
  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_stop_attr_for_reporting %d", (FMT__H, ret));
  return ret;
}

/* start attribute for reporting */
zb_ret_t zb_zcl_start_attr_reporting(zb_uint8_t ep, zb_uint16_t cluster_id, zb_uint8_t cluster_role, zb_uint16_t attr_id)
{
  zb_ret_t ret;
  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_start_attr_for_reporting, ep %hd, cluster_id 0x%x, attr_id 0x%x",
            (FMT__H_D_D, ep, cluster_id, attr_id));
  ret = zb_zcl_start_attr_reporting_manuf(ep, cluster_id, cluster_role, attr_id, ZB_ZCL_NON_MANUFACTURER_SPECIFIC);
  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_start_attr_for_reporting %d", (FMT__H, ret));
  return ret;
}

/* start attribute for reporting */
zb_ret_t zb_zcl_start_attr_reporting_manuf(zb_uint8_t ep, zb_uint16_t cluster_id, zb_uint8_t cluster_role, zb_uint16_t attr_id, zb_uint16_t manuf_code)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_reporting_info_t *rep_info;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_start_attr_for_reporting, ep %hd, cluster_id 0x%x, attr_id 0x%x, manuf_code 0x%x",
            (FMT__H_D_D_D, ep, cluster_id, attr_id, manuf_code));

  rep_info = zb_zcl_find_reporting_info_manuf(ep, cluster_id, cluster_role, attr_id, manuf_code);
  if (rep_info)
  {
    ZB_ZCL_CLR_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_STOP);
  }
  else
  {
    ret = RET_NOT_FOUND;
  }
  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_start_attr_for_reporting %d", (FMT__H, ret));
  return ret;
}

/** Reporting command send callback

   Reports are sent 1-by-1, zb_zcl_reporting_cb() is called when previouse report is sent
 */
void zb_zcl_reporting_cb(zb_uint8_t param)
{
  zb_uindex_t i, j;
  zb_zcl_reporting_info_t *rep_info;
  zb_zcl_command_send_status_t *cmd_send_status =
    ZB_BUF_GET_PARAM(param, zb_zcl_command_send_status_t);

  ZVUNUSED(cmd_send_status);
  TRACE_MSG(TRACE_ZCL1, "zb_zcl_reporting_cb param %hd, send status %hx",
            (FMT__H_H, param, cmd_send_status->status));

  for (j = 0; j < ZCL_CTX().device_ctx->ep_count; j++)
  {
    if (ZCL_CTX().device_ctx->ep_desc_list[j]->reporting_info)
    {
      rep_info = ZCL_CTX().device_ctx->ep_desc_list[j]->reporting_info;
      for (i = 0; i < ZCL_CTX().device_ctx->ep_desc_list[j]->rep_info_count; i++)
      {
        if (ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_SLOT_BUSY) &&
            ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_IS_SENT) &&
            !ZB_ZCL_GET_REPORTING_FLAG(rep_info, ZB_ZCL_REPORTING_STOP))
        {
          ZB_ZCL_CLR_REPORTING_FLAG(rep_info, ZB_ZCL_REPORT_IS_SENT);
          zb_zcl_update_reporting_info(rep_info);
        }
        rep_info++;
      }
    }
  }

  zb_zcl_adjust_reporting_timer(0);

  /* Send next reporting info */
  zb_buf_reuse(param);
  ZCL_CTX().reporting_ctx.buf_ref = param;
  ZB_SCHEDULE_CALLBACK(zb_zcl_report_attr, 0);
}

zb_bool_t zcl_is_attr_reported(
  zb_uint8_t ep, zb_uint16_t cluster_id, zb_uint8_t cluster_role, zb_uint16_t attr_id)
{
  zb_bool_t ret;
  TRACE_MSG(TRACE_ZCL1, ">> zcl_is_attr_reported", (FMT__0));
  ret = zcl_is_attr_reported_manuf(ep, cluster_id, cluster_role, attr_id, ZB_ZCL_NON_MANUFACTURER_SPECIFIC);
  TRACE_MSG(TRACE_ZCL1, "<< zcl_is_attr_reported ret %hd", (FMT__H, ret));
  return ret;
}

zb_bool_t zcl_is_attr_reported_manuf(
  zb_uint8_t ep, zb_uint16_t cluster_id, zb_uint8_t cluster_role, zb_uint16_t attr_id, zb_uint16_t manuf_code)
{
  zb_bool_t ret;
  zb_zcl_reporting_info_t *rep_info;

  TRACE_MSG(TRACE_ZCL1, ">> zcl_is_attr_reported_manuf", (FMT__0));

  rep_info = zb_zcl_find_reporting_info_manuf(ep, cluster_id, cluster_role, attr_id, manuf_code);
  ret = (zb_bool_t)!!rep_info;

  TRACE_MSG(TRACE_ZCL1, "<< zcl_is_attr_reported_manuf ret %hd", (FMT__H, ret));
  return ret;
}

#ifdef ZB_ZCL_ENABLE_DEFAULT_REPORT_ATTR_PROCESSING
/**
   Default report attribute command handler
   @param cmd_buf - pointer to received buffer with command
   @param cmd_info - pointer to a parsed command header
 */
void zb_zcl_report_attr_cmd_handler(zb_uint8_t param)
{
  zb_zcl_report_attr_req_t *rep_attr_req;
  zb_zcl_parsed_hdr_t cmd_info;

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_report_attr_cmd_handler %hd",
            (FMT__H, param));

  do
  {
    ZB_ZCL_GENERAL_GET_NEXT_REPORT_ATTR_REQ(param, rep_attr_req);
    TRACE_MSG(TRACE_ZCL3, "rep_attr_req %p", (FMT__P, rep_attr_req));
    if (rep_attr_req)
    {
      TRACE_MSG(TRACE_ZCL3, "attr dump: attr id %hd, type %hd",
                (FMT__H_H, rep_attr_req->attr_id, rep_attr_req->attr_type));

      /* inform ZCL that report is received to prevent timeout alarm */
      zb_zcl_report_received_manuf(
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
          cmd_info.cluster_id,
          (cmd_info.cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV) ?
          ZB_ZCL_CLUSTER_SERVER_ROLE : ZB_ZCL_CLUSTER_CLIENT_ROLE,
          rep_attr_req->attr_id,
          (cmd_info.is_manuf_specific) ? cmd_info.manuf_specific : ZB_ZCL_NON_MANUFACTURER_SPECIFIC);

      if (ZCL_CTX().report_attr_cb)
      {
        TRACE_MSG(TRACE_ZCL3, "inform user about reporting", (FMT__0));

        (*ZCL_CTX().report_attr_cb)(&ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source,
                                ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
                                    cmd_info.cluster_id,
                                    rep_attr_req->attr_id,
                                    rep_attr_req->attr_type,
                                    rep_attr_req->attr_value);
      }
    }
  }
  while(rep_attr_req);

  if (!cmd_info.disable_default_response)
  {
    ZB_ZCL_SEND_DEFAULT_RESP(param,
                             ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
                             ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                             ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
                             ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
                             cmd_info.profile_id,
                             cmd_info.cluster_id,
                             cmd_info.seq_number,
                             cmd_info.cmd_id,
                             ZB_ZCL_STATUS_SUCCESS);
  }
  else
  {
    zb_buf_free(param);
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_report_attr_cmd_handler", (FMT__0));
}
#endif /* ZB_ZCL_ENABLE_DEFAULT_REPORT_ATTR_PROCESSING */

#endif /* defined(ZB_ENABLE_ZCL) && !(defined ZB_ZCL_DISABLE_REPORTING) */
