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
/* PURPOSE: ZCL Continuous Value Change feature commands
*/
#define ZB_TRACE_FILE_ID 2064

#include "zb_common.h"

#if defined (ZB_CVC_FEATURE_SUPPORT)

#include "zcl/zb_zcl_cvc_commands.h"

/* 01/02/2013 CR:MAJOR
   General:
   The following functions should use CVC:
   1. Move to level command(3.10.2.3.1). Current value -> Target value, step - from technical
   practice. Direction from values.
   2. Move command(3.10.2.3.2). Rate and direction(or stop). On min/max values - stop.
   3. Step command(3.10.2.3.3). Target value, direction, transition time.
   4. Enhanced move to hue (6.8.1.3.2...). Target value, direction(or stop), transition time. App should care
   about reachable values!
   5. Enhanced move hue (6.8.1.3.3...). Rate and direction(or stop). On margin values - loop.
   6. Enhanced step hue (6.8.1.3.4...). Direction, target value, transition time. On margin values - loop.
   7. Move to XY (6.8.1.3.5). TargetX, TargetY, transition time.
   8. Color loop set. Out of scope.
   9. Stop move step. Out of scope.
   10. Move color temp. Rate, direction(or stop), min and max. After reach
   min/max/phyMin/phyMax - stop.
   11. Step color temp. Direction (or stop), target value, transition time, max and min.
   After reach min/max/phyMin/phyMax - stop.

   TODO: Continuous value change with loop should be implemented.

   05/02/2013 "with loop" means overlapped values?
   06/02/2013 Overlapping added. Value change scheme can be, for example, ... -> 1 -> 0 -> 255 ->
   254 -> ..., where 0 and 255 is MIN and MAX values. Tested on Level Control.
*/


#define ZB_ZCL_CVC_GET_TIME_IN_UNITS()  ZB_TIME_BEACON_INTERVAL_TO_MSEC(ZB_TIMER_GET()) / 100

void zb_zcl_cvc_correct_value(zb_zcl_cvc_input_variables_t* input_var);

static zb_uint8_t zb_zcl_cvc_calc(
  zb_zcl_cvc_input_variables_t* input_variables,
  zb_uint8_t buf_id,
  zb_bool_t is_recalc)
{
  zb_bufid_t buf = 0;
#ifdef ZB_ZCL_ENABLE_CVC
  zb_uint8_t diff;
  zb_bool_t direction_decrement = ZB_FALSE;
#endif
  zb_zcl_cvc_variables_t* change_variables;
  zb_uint8_t ret = ZB_UNDEFINED_BUFFER;
  zb_zcl_cvc_input_variables_t* input_var;
  zb_time_t transition_time_q = 0;


  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_cvc_calc", (FMT__0));

  if (is_recalc == ZB_FALSE)
  {
    /* 1. Correct end level. */
    buf = zb_buf_get_out();
    zb_zcl_cvc_correct_value(input_variables);
  }
  else
  {
    buf = buf_id;
  }

  ZB_ASSERT(buf != 0);

  change_variables = ZB_BUF_GET_PARAM(buf, zb_zcl_cvc_variables_t);

  if (is_recalc == ZB_FALSE)
  {
    input_var = input_variables;
  }
  else
  {
    input_var = &(change_variables->input_var);
  }


/* 0. Check that transition_time is correct (> 0)! */
  if (input_var->transition_time == 0)
  {
  /* This function returns buffer id.
   * If input data is wrong, ZB_UNDEFINED_BUFFER will be returned.
   * So i think, no error codes from zb_error.h needed here, because this is only 2 cases: valid
   * buffer id or invalid buffer id (ZB_UNDEFINED_BUFFER).
   */
    TRACE_MSG(TRACE_ZCL1, "Error, transition_time is incorrect!", (FMT__0));
  }
  else
  {

/* 2. Set input variables. */
    if (&(change_variables->input_var) != input_var)
    {
      ZB_MEMCPY(&(change_variables->input_var), input_var, sizeof(zb_zcl_cvc_input_variables_t));
    }
/*    change_variables->input_var.current_value16 = input_var->current_value16;
    change_variables->input_var.end_value16 = input_var->end_value16;
    change_variables->input_var.min_value16 = input_var->min_value16;
    change_variables->input_var.max_value16 = input_var->max_value16;
    change_variables->input_var.overlap = input_var->overlap;
    change_variables->input_var.transition_time = input_var->transition_time;
    change_variables->transition_time_q = 0;
    change_variables->input_var.value_set_func = input_var->value_set_func;
    change_variables->input_var.buf_id = input_var->buf_id;
    change_variables->input_var.after_processing_cb = input_var->after_processing_cb;
*/
#ifndef ZB_ZCL_ENABLE_CVC
    input_var->transition_time = ZB_ZCL_CVC_TRANSITION_TIME_AS_FAST_AS_ABLE;
    change_variables->transition_time_q = 0;
    change_variables->steps_number = 1;
    change_variables->delta_time_q = 1;
#else

/* 3. Calculate direction_decrement */


/* 4. Calculate diff. */
//TODO: overlap
    if (input_var->overlap == ZB_TRUE)
    {
      if (input_var->end_value16 > input_var->current_value16)
      // end_value16 > current_value16 => overlap MIN value
      {
        diff = (input_var->current_value16 - input_var->min_value16) +
          (input_var->max_value16 - input_var->end_value16);
        direction_decrement = ZB_TRUE;
      }
      else
      // end_value16 <= current_value16 => overlap MAX value
      {
        diff = (input_var->max_value16 - input_var->current_value16) +
          (input_var->end_value16 - input_var->min_value16);
        direction_decrement = ZB_FALSE;
      }
    }
    else
    {
      diff = ZB_ABS((zb_int32_t)input_var->end_value16 - input_var->current_value16);
      direction_decrement = (zb_bool_t)(input_var->end_value16 < input_var->current_value16);
    }
    TRACE_MSG(TRACE_ZCL1, "diff = %i", (FMT__H,  diff));

    if (diff != 0)
    {
      if (input_var->transition_time == ZB_ZCL_CVC_TRANSITION_TIME_AS_FAST_AS_ABLE ||
          input_var->transition_time == ZB_ZCL_CVC_TRANSITION_TIME_MINIMAL)
      {
        TRACE_MSG(TRACE_ZCL1, "transition_time is 0xffff or 0x0001", (FMT__0));
        change_variables->steps_number = 1;
        change_variables->delta_time_q = 1;
      }
      else
      {
        transition_time_q = input_var->transition_time * ZB_ZCL_CVC_TRANSITION_TIME_UNIT_IN_QUANTS;
        change_variables->transition_time_q = 0;
      /* 7. */
/* 01/02/2013
  I don't understood this comparison.
  What they are compared for?

  05/02/2013 steps_number = min(diff*TRANSITION_TIME_UNITS, transition_time)
  Its needed for calculations.
*/

        /* Each step can be executed with the fastest time of
         * ZB_ZCL_CVC_TRANSITION_TIMER_QUANT_MS. Transition time is
         * calculated in units of 0.1 sec, that's why multiplication
         * is needed */
        if (transition_time_q > diff)
        {
          /* If transition time  */
          change_variables->steps_number = diff;
        }
        else
        {
          change_variables->steps_number = transition_time_q;
        }

        TRACE_MSG(TRACE_ERROR, "steps_number = %d", (FMT__D, change_variables->steps_number));

        if (change_variables->steps_number > 1)
        {
          change_variables->delta_time_q = transition_time_q / change_variables->steps_number;
        }
        else
        {
          change_variables->delta_time_q = transition_time_q;
        }

        TRACE_MSG(TRACE_ERROR,
                  "delta_time_q = %d",
                  (FMT__D, change_variables->delta_time_q));

/* 8. */
        if (change_variables->steps_number > 1)
        {
          change_variables->delta_value16 = diff / change_variables->steps_number;
        }
        else
        {
          change_variables->delta_value16 = diff;
        }

        TRACE_MSG(TRACE_ERROR, "delta_value = %d", (FMT__D, change_variables->delta_value16));

/* 9. */
/* 10. */
/* 11. */
        if (direction_decrement == ZB_TRUE)
        {
          change_variables->delta_value16 = -change_variables->delta_value16;
        }

/* 12. */
        change_variables->end_time_tu =
          (ZB_ZCL_CVC_GET_TIME_IN_UNITS() + input_var->transition_time);

        TRACE_MSG(TRACE_ERROR,
                  "end_time_tu = %d",
                  (FMT__D, change_variables->end_time_tu));
      }
    }
    else
    {
      change_variables->steps_number = 0;
//      change_variables->transition_time_q = 0;
//      change_variables->input_var.transition_time = ZB_ZCL_CVC_TRANSITION_TIME_AS_FAST_AS_ABLE;

    }
#endif
    ret = buf;
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_cvc_calc", (FMT__0));

  return ret;
}


zb_uint8_t zb_zcl_cvc_calculate_transition_values(zb_zcl_cvc_input_variables_t* input_var)
{
  return zb_zcl_cvc_calc(input_var, 0, ZB_FALSE);
}

#define ZB_ZCL_CVC_RECALC_TRANSITION_VALUES(buf_id)                     \
  zb_zcl_cvc_calc(                                                      \
    NULL,                                                               \
    buf_id,                                                             \
    ZB_TRUE);


void zb_zcl_cvc_correct_value(zb_zcl_cvc_input_variables_t* input_var)
{
  if (input_var->end_value16 > input_var->max_value16)
  {
    input_var->end_value16 = input_var->max_value16;
  }
  else if (input_var->end_value16 < input_var->min_value16)
  {
    input_var->end_value16 = input_var->min_value16;
  }

}

/*! Returns slot number of the reporting info */
static zb_uint8_t zb_zcl_cvc_get_alarm_id(zb_zcl_cvc_alarm_variables_t *alarm_var)
{
  zb_uindex_t i = 0xff;
  zb_uint8_t alarm_id = 0;

  if (alarm_var && ZCL_CTX().device_ctx)
  {
    for (i = 0; i < ZCL_CTX().device_ctx->ep_count; i++)
    {
      if (alarm_var->endpoint_id != ZCL_CTX().device_ctx->ep_desc_list[i]->ep_id)
      {
        alarm_id += ZB_ZCL_MAX_CVC_SLOTS_BY_EP;
      }
      else
      {
        alarm_id += alarm_var - ZCL_CTX().device_ctx->ep_desc_list[i]->cvc_alarm_info;
        break;
      }
    }
  }

  return (i < ZCL_CTX().device_ctx->ep_count) ? alarm_id : ZB_ZCL_CVC_INVALID_ALARM_ID;
}

/*! Returns reporting info that is stored in slot with specified number */
static zb_zcl_cvc_alarm_variables_t *zb_zcl_cvc_get_alarm_variables(zb_uint8_t alarm_id)
{
  zb_uindex_t i;

  if (alarm_id != ZB_ZCL_CVC_INVALID_ALARM_ID && ZCL_CTX().device_ctx)
  {
    for (i = 0; i < ZCL_CTX().device_ctx->ep_count; i++)
    {
      if (alarm_id >= ZB_ZCL_MAX_CVC_SLOTS_BY_EP)
      {
        alarm_id -= ZB_ZCL_MAX_CVC_SLOTS_BY_EP;
      }
      else
      {
        return (zb_zcl_cvc_alarm_variables_t *)(ZCL_CTX().device_ctx->ep_desc_list[i]->cvc_alarm_info + alarm_id);
      }
    }
  }

  return NULL;
}

static void zb_zcl_cvc_next_step_alarm(zb_uint8_t alarm_id)
{
  zb_zcl_cvc_variables_t* change_var;
  zb_zcl_cvc_alarm_variables_t* alarm_var;
  zb_time_t curr_time_tu;
  zb_time_t next_alarm = 0;
  zb_int32_t tmp;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_cvc_next_step_alarm alarm_id %i", (FMT__H, alarm_id));

/* 1. Get variables from buffer. */

  if (ZCL_CTX().device_ctx != NULL)
  {
    alarm_var = zb_zcl_cvc_get_alarm_variables(alarm_id);
    if (alarm_var != NULL)
    {
      change_var = ZB_BUF_GET_PARAM(alarm_var->alarm_buf_id, zb_zcl_cvc_variables_t);

      /* 2. Time calculation part */

      curr_time_tu = ZB_ZCL_CVC_GET_TIME_IN_UNITS();

      TRACE_MSG(TRACE_ZCL1, "curr_time_tu = %ld", (FMT__L, curr_time_tu));
      TRACE_MSG(TRACE_ZCL1, "transition_time = %d", (FMT__D, change_var->input_var.transition_time));
      TRACE_MSG(TRACE_ZCL1, "end_time_tu = %d", (FMT__D, change_var->end_time_tu));
      TRACE_MSG(TRACE_ZCL1, "delta_time_q = %d", (FMT__D, change_var->delta_time_q));
      TRACE_MSG(TRACE_ZCL1, "delta_value16 = %d", (FMT__D, change_var->delta_value16));
      TRACE_MSG(TRACE_ZCL1, "steps_number = %d", (FMT__D, change_var->steps_number));
      TRACE_MSG(TRACE_ZCL1, "current_value16 = %d", (FMT__D, change_var->input_var.current_value16));

      /* Check if recalc needed (in bigger or smaller side). If error is bigger than
         move_variables->err value, all move variables will be corrected (with
         new transition_time). */

      if (change_var->input_var.transition_time != ZB_ZCL_CVC_TRANSITION_TIME_AS_FAST_AS_ABLE &&
          change_var->input_var.transition_time != ZB_ZCL_CVC_TRANSITION_TIME_MINIMAL)
      {
        /* Check if end_time has already passed. */
        if ((change_var->end_time_tu + ZB_ZCL_CVC_TRANSITION_TIME_ERROR_TU) > curr_time_tu)
        {
          /* Planned end time */
          zb_uint32_t tmp_val_tu = curr_time_tu + change_var->input_var.transition_time - change_var->delta_time_q / ZB_ZCL_CVC_TRANSITION_TIME_UNIT_IN_QUANTS; 

          /* Difference between planned end time and initial end time */
          if (tmp_val_tu > change_var->end_time_tu)
          {
            tmp_val_tu -= change_var->end_time_tu;
          }
          else
          {
            tmp_val_tu = change_var->end_time_tu - tmp_val_tu;
          }

          if (tmp_val_tu > ZB_ZCL_CVC_TRANSITION_TIME_ERROR_TU)
          {
            TRACE_MSG(TRACE_ZCL1, "recalc", (FMT__0));

            if (change_var->end_time_tu > curr_time_tu)
            {
              /* (change_var->end_time_tu - curr_time_tu) < ZB_ZCL_CVC_TRANSITION_TIME_ERROR_TU */
              tmp_val_tu = change_var->end_time_tu - curr_time_tu;
            }
            else
            {
              tmp_val_tu = curr_time_tu - change_var->end_time_tu;
            }

            change_var->input_var.transition_time = tmp_val_tu;
            change_var->transition_time_q = 0;
            TRACE_MSG(TRACE_ZCL1, "new transition_time = %d", (FMT__D, change_var->input_var.transition_time));

            ZB_ZCL_CVC_RECALC_TRANSITION_VALUES(alarm_var->alarm_buf_id);
          }
          else
          {
            change_var->input_var.transition_time -= change_var->delta_time_q / ZB_ZCL_CVC_TRANSITION_TIME_UNIT_IN_QUANTS;

            if (change_var->transition_time_q < (change_var->delta_time_q % ZB_ZCL_CVC_TRANSITION_TIME_UNIT_IN_QUANTS))
            {
              change_var->input_var.transition_time--;
              change_var->transition_time_q += ZB_ZCL_CVC_TRANSITION_TIME_UNIT_IN_QUANTS;
            }

            change_var->transition_time_q -= change_var->delta_time_q % ZB_ZCL_CVC_TRANSITION_TIME_UNIT_IN_QUANTS;
          }
        }
        else
        {
          /* curr_time is greater then end_time_tu of all transition - go to last step */
          change_var->steps_number = 1;
        }
      }

      /* Time calculation part ended */

      if (change_var->steps_number != 0)
      {

        /* step_number == 1 means the LAST step */
        if (change_var->steps_number == 1)
        {
          change_var->input_var.current_value16 = change_var->input_var.end_value16;
        }
        else
        {
//TODO: overlap
          tmp = change_var->input_var.current_value16 + change_var->delta_value16;

          TRACE_MSG(TRACE_ZCL1, "tmp = %d", (FMT__D, tmp));

          if (tmp >= change_var->input_var.min_value16 &&
              tmp <= change_var->input_var.max_value16)
          {
            change_var->input_var.current_value16 = tmp;
          }
          else
          {
            if (tmp < change_var->input_var.min_value16)
            {
              change_var->input_var.current_value16 = change_var->input_var.max_value16 -
                ZB_ABS((zb_int32_t)change_var->input_var.min_value16 - tmp) + 1;
            }
            else
            {
              change_var->input_var.current_value16 = change_var->input_var.min_value16 +
                ZB_ABS((zb_int32_t)tmp - change_var->input_var.max_value16) - 1;
            }
          }
        }

        TRACE_MSG(TRACE_ZCL1, "next level is %d", (FMT__D, change_var->input_var.current_value16));

        if (change_var->input_var.value_set_func != NULL)
        {
          zb_uint16_t remaining_time = 0;

          if (change_var->steps_number > 1)
          {
            remaining_time = change_var->input_var.transition_time;
          }
          change_var->input_var.value_set_func(alarm_var->endpoint_id, &(change_var->input_var.current_value16), remaining_time);
        }
      }

      if (change_var->steps_number > 1)
      {
/* Checking if time incrmenting is needed */
        next_alarm = change_var->delta_time_q;
        ZB_SCHEDULE_ALARM(zb_zcl_cvc_next_step_alarm,
                          alarm_id,
                          ZB_ZCL_CVC_TRANSITION_TIMER_QUANT_TO_BE(next_alarm));
      }
      else
      {
        TRACE_MSG(TRACE_ZCL1, "transition with alarm_id %i finished", (FMT__H, alarm_id));
        ZB_SCHEDULE_CALLBACK(change_var->input_var.after_processing_cb, alarm_var->endpoint_id);
        alarm_var->is_used = ZB_FALSE;
        zb_buf_free(alarm_var->alarm_buf_id);
      }
/*
  if (change_var->input_var.current_value16 >= 20 &&
  change_var->input_var.current_value16 <= 50)
  {
  TRACE_MSG(TRACE_ZCL1, "sleep!", (FMT__0));
  sleep(1);
  }
*/
      if (change_var->steps_number > 0U)
      {
        --(change_var->steps_number);
      }
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_cvc_next_step_alarm", (FMT__0));
}


/** @internal @brief Init alarm info data  */
void zb_zcl_init_cvc_alarm_info()
{
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_init_cvc_alarm_info", (FMT__0));

  if (ZCL_CTX().device_ctx != NULL)
  {
    zb_uindex_t i;

    for (i = 0; i < ZCL_CTX().device_ctx->ep_count; i++)
    {
      if (ZCL_CTX().device_ctx->ep_desc_list[i]->cvc_alarm_info != NULL)
      {
        ZB_BZERO(ZCL_CTX().device_ctx->ep_desc_list[i]->cvc_alarm_info,
                 sizeof(zb_zcl_cvc_alarm_variables_t) * ZCL_CTX().device_ctx->ep_desc_list[i]->cvc_alarm_count);
      }
    }
  }
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_init_cvc_alarm_info", (FMT__0));
}

zb_uint8_t zb_zcl_cvc_check_transition_running(
  zb_uint8_t endpoint_id,
  zb_uint16_t cluster_id,
  zb_uint16_t attribute_id)
{
  zb_zcl_cvc_alarm_variables_t* alarm_info;
  zb_zcl_cvc_alarm_variables_t alarm_var;
  zb_uindex_t i, j;
  zb_uint8_t ret = ZB_ZCL_CVC_INVALID_ALARM_ID;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_cvc_check_transition_running, ep %hd, cluster %d, attr_id %d",
            (FMT__H_D_D, endpoint_id, cluster_id, attribute_id));

  alarm_var.endpoint_id = endpoint_id;
  alarm_var.cluster_id = cluster_id;
  alarm_var.attribute_id = attribute_id;
  alarm_var.is_used = ZB_TRUE;

  if (ZCL_CTX().device_ctx != NULL)
  {
    for (j = 0; j < ZCL_CTX().device_ctx->ep_count; j++)
    {
      if (ZCL_CTX().device_ctx->ep_desc_list[j]->ep_id == endpoint_id)
      {
        alarm_info = ZCL_CTX().device_ctx->ep_desc_list[j]->cvc_alarm_info;
        for (i = 0; i < ZCL_CTX().device_ctx->ep_desc_list[j]->cvc_alarm_count; i++)
        {
          if (alarm_info->endpoint_id == alarm_var.endpoint_id &&
              alarm_info->cluster_id == alarm_var.cluster_id &&
              alarm_info->attribute_id == alarm_var.attribute_id &&
              alarm_info->is_used == ZB_TRUE)
          {
            ret = zb_zcl_cvc_get_alarm_id(alarm_info);
            break;
          }
          alarm_info++;
        }
      }
    }
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_cvc_check_transition_running ret %i", (FMT__H, ret));
  return ret;
}

zb_uint8_t zb_zcl_cvc_stop_transition(zb_uint8_t alarm_id)
{
  zb_uint8_t old_buf_id = ZB_UNDEFINED_BUFFER;
  zb_bufid_t  alarm_buf;
  zb_zcl_cvc_alarm_variables_t* alarm_info;
  zb_zcl_cvc_variables_t* change_var;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_cvc_stop_transition, alarm_id %i",
            (FMT__H, alarm_id));
  if (ZCL_CTX().device_ctx != NULL)
  {
    alarm_info = zb_zcl_cvc_get_alarm_variables(alarm_id);

    if (alarm_info && alarm_info->is_used)
    {
      ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_cvc_next_step_alarm, alarm_id);

      alarm_buf = alarm_info->alarm_buf_id;
      change_var = ZB_BUF_GET_PARAM(alarm_buf, zb_zcl_cvc_variables_t);
      old_buf_id = change_var->input_var.buf_id;

      zb_buf_free(alarm_buf);
      alarm_info->is_used = ZB_FALSE;
    }
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_cvc_stop_transition", (FMT__0));
  return old_buf_id;
}


zb_uint8_t zb_zcl_cvc_start_alarm(zb_uint8_t endpoint_id,
                                  zb_uint16_t cluster_id,
                                  zb_uint16_t attribute_id,
                                  zb_uint8_t alarm_buf_id)
{
  zb_uint8_t alarm_id;
  zb_zcl_cvc_alarm_variables_t* alarm_info;
  zb_uindex_t i = 0, j = 0;
  zb_uint16_t ret = ZB_ZCL_CVC_INVALID_ALARM_ID;
  zb_bool_t next_step = ZB_TRUE;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_cvc_start_alarm", (FMT__0));

/* 01/02/2013 CR:MAJOR
  Use ASSERT for all cases when context is broken/NULL
  And t.b.h. I'm not sure that this check is usefull at all. Because it can be
  done once and after I doubt that it's really needed for any purpose except code size increasing.
  If it's overlapped somehow - it's a zero chance that it became zero. Most likely it will take
  some unapropriate value and will cause segmentation fault (or any prot.error) even if it pass
  NULL-check

  05/02/2013 OK, i can remove them if you think its useless. I was look on reporting when
  adding it.
*/
  if (ZCL_CTX().device_ctx != NULL)
  {
    for (j = 0; j < ZCL_CTX().device_ctx->ep_count; j++)
    {
      if (ZCL_CTX().device_ctx->ep_desc_list[j]->ep_id == endpoint_id)
      {
        alarm_info = ZCL_CTX().device_ctx->ep_desc_list[j]->cvc_alarm_info;

        if (alarm_info == NULL)
        {
          TRACE_MSG(TRACE_ERROR, "Continuous value change alarm slot is NULL for ep %hd, cluster %d, attr_id %d",
                    (FMT__H_D_D, endpoint_id, cluster_id, attribute_id));
          continue;
        }

        TRACE_MSG(TRACE_ZCL1, "ZCL_CTX().device_ctx && ZCL_CTX().device_ctx->cvc_alarm_info", (FMT__0));
        alarm_id = zb_zcl_cvc_check_transition_running(endpoint_id,
                                                       cluster_id,
                                                       attribute_id);
        if (alarm_id != ZB_ZCL_CVC_INVALID_ALARM_ID)
        {
          zb_zcl_cvc_stop_transition(alarm_id);
          alarm_info = zb_zcl_cvc_get_alarm_variables(alarm_id);
        }
        else
        {
/* We run zb_zcl_cvc_check_transition_running() for check. If other transition
   is in progress (alarm_id != ZB_ZCL_CVC_INVALID_ALARM_ID), we dont need to search and
   can use its alarm_info. Else we need to check, is there any free alarm_info's (while
   (alarm_info->is_used == ZB_TRUE) go to next alarm_info). If alarm_id >=
   ZCL_CTX().device_ctx->cvc_alarm_count, we checked all list and didn't
   found free alarm_info (need to exit with INVALID_ALARM_ID return), else - use founded alarm_info
   and start alarm.
*/
          i = 0;
          while (alarm_info->is_used == ZB_TRUE)
          {
            ++i;
            /* to prevent infinite loop */
            if (i >= ZCL_CTX().device_ctx->ep_desc_list[j]->cvc_alarm_count)
            {
              /* there are no free alarms */
              break;
            }
            else
            {
              ++alarm_info;
            }
          }

          if (i >= ZCL_CTX().device_ctx->ep_desc_list[j]->cvc_alarm_count)
          {
            TRACE_MSG(TRACE_ZCL1, "(i >= ZCL_CTX().device_ctx->cvc_alarm_count", (FMT__0));
            next_step = ZB_FALSE;
          }
          else
          {
            /* Set endpoint_id here to be able to find back alarm_id (next call) */
            alarm_info->endpoint_id = endpoint_id;
            alarm_id = zb_zcl_cvc_get_alarm_id(alarm_info);
          }
        }

        TRACE_MSG(TRACE_ZCL1, "next_step %i", (FMT__H, next_step));

        if (next_step == ZB_TRUE)
        {
          zb_zcl_cvc_variables_t* change_var;

          TRACE_MSG(TRACE_ZCL1, "alarm_info found, id %i", (FMT__H, alarm_id));

          ret = alarm_id;

          alarm_info->endpoint_id = endpoint_id;
          alarm_info->cluster_id = cluster_id;
          alarm_info->attribute_id = attribute_id;
          alarm_info->alarm_buf_id = alarm_buf_id;
          alarm_info->is_used = ZB_TRUE;

          change_var = ZB_BUF_GET_PARAM(alarm_info->alarm_buf_id, zb_zcl_cvc_variables_t);

          //scheduling
          switch (change_var->steps_number)
          {
            case 0:
              /* Can not call after_processing_cb immediately - let's do 1 iteration. */
              ZB_SCHEDULE_ALARM(zb_zcl_cvc_next_step_alarm,
                                alarm_id,
                                ZB_ZCL_CVC_TRANSITION_TIMER_QUANT_TO_BE(ZB_ZCL_CVC_TRANSITION_TIME_MINIMAL));
              break;

            case 1:
            {
              zb_uint16_t next_alarm;

              if (change_var->input_var.transition_time == ZB_ZCL_CVC_TRANSITION_TIME_AS_FAST_AS_ABLE)
              {
/*
   Why should it wait for the "minimal" interval when it's said as_fast_as_able?

   05/02/2013 It was our agreement that as_fast_as_able == at 1 minimal transition time unit.
*/
                next_alarm = ZB_ZCL_CVC_TRANSITION_TIME_MINIMAL;
              }
              else
              {
                next_alarm = change_var->delta_time_q;
              }

              ZB_SCHEDULE_ALARM(zb_zcl_cvc_next_step_alarm,
                                alarm_id,
                                ZB_ZCL_CVC_TRANSITION_TIMER_QUANT_TO_BE(next_alarm));
            }
            break;

            default:
              ZB_SCHEDULE_CALLBACK(zb_zcl_cvc_next_step_alarm, alarm_id);
              break;
          }
        }
      }
    }
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_cvc_start_alarm", (FMT__0));
  return ret;
}

zb_uint16_t zb_zcl_cvc_get_remaining_time(zb_uint8_t alarm_id)
{
  zb_zcl_cvc_alarm_variables_t* alarm_info;
  zb_uint16_t ret = ZB_ZCL_CVC_INVALID_REMAINING_TIME;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_cvc_get_remaining_time alarm_id %i", (FMT__H, alarm_id));

  if (ZCL_CTX().device_ctx != NULL)
  {
    alarm_info = zb_zcl_cvc_get_alarm_variables(alarm_id);

    if (alarm_info && alarm_info->is_used)
    {
      zb_zcl_cvc_variables_t* change_var =
        ZB_BUF_GET_PARAM(alarm_info->alarm_buf_id, zb_zcl_cvc_variables_t);

      ret = change_var->input_var.transition_time;
    }
  }

  return ret;
}
#endif /* defined ZB_CVC_FEATURE_SUPPORT */
