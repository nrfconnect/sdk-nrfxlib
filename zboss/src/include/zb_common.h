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
/* PURPOSE: Common include file for ZigBee
*/
#ifndef ZB_COMMON_H
#define ZB_COMMON_H 1

#include "zb_config.h"

#ifdef ZB_MINIMAL_CONTEXT

#include "zb_g_context_min.h"

#else

#include "zboss_api.h"
#include "zb_types.h"
#include "zb_errors.h"
#include "zb_debug.h"
#include "zb_g_context.h"
#include "zb_trace.h"
#include "zb_hash.h"
#ifdef ZB_TH_ENABLED
#include "zb_th_internal.h"
#else
#include "zb_th_dummy.h"
#endif /* ZB_TH_ENABLED */
#include "zb_common_u.h"

#include "zb_nvram.h"

/*! \addtogroup init_api */
/*! @{ */

void zb_globals_init(void);

#if defined ENABLE_USB_SERIAL_IMITATOR
#define USB_SERIAL_IMITATOR_RX_PIPE "usbc_rx"
#define USB_SERIAL_IMITATOR_TX_PIPE "usbc_tx"
#endif /* defined ENABLE_USB_SERIAL_IMITATOR */

/*! @} */

/*! \internal \addtogroup ZB_BASE */
/*! @{ */

/**
   Set Informational Bases defaults.

   @param rx_pipe - rx pipe name (for Unix) or node number (for ns build
                            in 8051 simulator)
 */
void zb_ib_set_defaults(zb_char_t *rx_pipe);

enum zb_dev_specific_e
{
  /*! Standard device */
  ZB_STANDARD_SPECIFIC_DEVICE = 0x00,
  /*! Manufactured device */
  ZB_MANUFACTURED_SPECIFIC_DEVICE = 0x01
};

/* Definitions for multiple tests in the same FW image / same Linux binary */
#ifdef ZB_MULTI_TEST
extern zb_int_t g_argc;
extern char** g_argv;
#define ZB_ARGV g_argv
#define ZB_ARGC g_argc
#undef MAIN
#undef ARGV_UNUSED
#define ARGV_UNUSED
#undef MAIN_RETURN
#define MAIN_RETURN(x)
#define TN_CAT0(x,y) x ## y
#define TN_CAT(x,y) TN_CAT0(x,y)
#define MAIN void TN_CAT(ZB_TEST_NAME,_main)
#define ZB_ZDO_STARTUP_COMPLETE void TN_CAT(ZB_TEST_NAME,_zb_zdo_startup_complete)
#define ZGPD_STARTUP_COMPLETE void TN_CAT(ZB_TEST_NAME,_zgpd_startup_complete)

/* List of MAC primitives' callbacks */
#define ZB_MLME_ASSOCIATE_INDICATION void TN_CAT(ZB_TEST_NAME,_zb_mlme_associate_indication)
#define ZB_MLME_ASSOCIATE_CONFIRM void TN_CAT(ZB_TEST_NAME,_zb_mlme_associate_confirm)
#define ZB_MLME_BEACON_NOTIFY_INDICATION void TN_CAT(ZB_TEST_NAME,_zb_mlme_beacon_notify_indication)
#define ZB_MLME_COMM_STATUS_INDICATION void TN_CAT(ZB_TEST_NAME,_zb_mlme_comm_status_indication)
#define ZB_MLME_ORPHAN_INDICATION void TN_CAT(ZB_TEST_NAME,_zb_mlme_orphan_indication)
#define ZB_MLME_RESET_CONFIRM void TN_CAT(ZB_TEST_NAME,_zb_mlme_reset_confirm)
#define ZB_MLME_SCAN_CONFIRM void TN_CAT(ZB_TEST_NAME,_zb_mlme_scan_confirm)
#define ZB_MLME_START_CONFIRM void TN_CAT(ZB_TEST_NAME,_zb_mlme_start_confirm)
#define ZB_MLME_POLL_CONFIRM void TN_CAT(ZB_TEST_NAME,_zb_mlme_poll_confirm)
#define ZB_MLME_PURGE_CONFIRM void TN_CAT(ZB_TEST_NAME,_zb_mlme_purge_confirm)
#define ZB_MCPS_DATA_INDICATION void TN_CAT(ZB_TEST_NAME,_zb_mcps_data_indication)
#define ZB_MCPS_DATA_CONFIRM void TN_CAT(ZB_TEST_NAME,_zb_mcps_data_confirm)
#define ZB_MLME_DUTY_CYCLE_MODE_INDICATION void TN_CAT(ZB_TEST_NAME,_zb_mlme_duty_cycle_mode_indication)
#define ZB_PLME_CCA_CONFIRM void TN_CAT(ZB_TEST_NAME,_zb_plme_cca_confirm)

#else  /* ZB_MULTI_TEST */

#define ZB_ZDO_STARTUP_COMPLETE void zb_zdo_startup_complete
#define ZGPD_STARTUP_COMPLETE void zgpd_startup_complete

  /* List of MAC primitives' callbacks */
#define ZB_MLME_ASSOCIATE_INDICATION void zb_mlme_associate_indication
#define ZB_MLME_ASSOCIATE_CONFIRM void zb_mlme_associate_confirm
#define ZB_MLME_BEACON_NOTIFY_INDICATION void zb_mlme_beacon_notify_indication
#define ZB_MLME_COMM_STATUS_INDICATION void zb_mlme_comm_status_indication
#define ZB_MLME_ORPHAN_INDICATION void zb_mlme_orphan_indication
#define ZB_MLME_RESET_CONFIRM void zb_mlme_reset_confirm
#define ZB_MLME_SCAN_CONFIRM void zb_mlme_scan_confirm
#define ZB_MLME_START_CONFIRM void zb_mlme_start_confirm
#define ZB_MLME_POLL_CONFIRM void zb_mlme_poll_confirm
#define ZB_MLME_PURGE_CONFIRM void zb_mlme_purge_confirm
#define ZB_MCPS_DATA_INDICATION void zb_mcps_data_indication
#define ZB_MCPS_DATA_CONFIRM void zb_mcps_data_confirm
#define ZB_MLME_DUTY_CYCLE_MODE_INDICATION void zb_mlme_duty_cycle_mode_indication
#define ZB_PLME_CCA_CONFIRM void zb_plme_cca_confirm

#define ZB_ARGV argv
#define ZB_ARGC argc

#endif  /* ZB_MULTI_TEST */

/**
 * @brief Implements "if (a) then (b)" logical operation.
 *
 * @b Example:
 * Instead of:
 * @code
 * {
 *   if(ret == RET_OK)
 *     ZB_ASSERT(*p != NULL);
 * }
 * @endcode
 * It is possible to write as:
 * @code
 * {
 *   ZB_ASSERT(ZB_THEREFORE(ret == RET_OK, *p != NULL));
 * }
 *  @endcode
 * This approach avoid dead code when ZB_ASSERT is not defined
 */
#define ZB_THEREFORE(a, b) (!(a) || (b))

/**
 * @brief Assert using #ZB_THEREFORE macro
 */
#define ZB_ASSERT_IF(cond, assert) (ZB_ASSERT(ZB_THEREFORE((cond), (assert))))

#ifndef ZB_CONFIGURABLE_MEM
#define ZB_RESYNC_CFG_MEM()
#else /* ZB_CONFIGURABLE_MEM */
void zb_init_configurable_mem(int clear);
#define ZB_RESYNC_CFG_MEM() zb_init_configurable_mem(0)
#endif /* ZB_CONFIGURABLE_MEM */

#if !defined ZB_LEGACY_BUFS
#ifdef zb_buf_t
#undef zb_buf_t
#undef ZB_BUF_ALLOC_LEFT
#undef ZB_BUF_ALLOC_RIGHT
#undef ZB_BUF_BEGIN_FROM_REF
#undef ZB_BUF_BEGIN
#undef ZB_BUF_CLEAR_PARAM
#undef ZB_BUF_COPY
#undef ZB_BUF_CUT_LEFT2
#undef ZB_BUF_CUT_LEFT
#undef ZB_BUF_CUT_RIGHT
#undef ZB_BUF_FROM_REF
#undef ZB_BUF_GET_ARRAY_PTR
#undef ZB_BUF_INITIAL_ALLOC
#undef ZB_BUF_LEN_BY_REF
#undef ZB_BUF_LEN
#undef ZB_BUF_OFFSET
#undef ZB_BUF_REUSE
#undef ZB_FREE_BUF
#undef ZB_FREE_BUF_BY_REF
#undef ZB_GET_BUF_PARAM
#undef ZB_GET_BUF_TAIL
#undef ZB_GET_IN_BUF
#undef ZB_GET_IN_BUF_DELAYED
#undef ZB_GET_OUT_BUF
#undef ZB_GET_OUT_BUF_DELAYED2
#undef ZB_GET_OUT_BUF_DELAYED
#undef ZB_REF_FROM_BUF
#undef ZB_REF_FROM_BUF_PTR
#undef ZB_SET_BUF_PARAM_PTR
#undef ZB_SET_BUF_PARAM
#undef ZB_SWITCH_BUF

#endif /*zb_buf_t */

#define ZB_BUF_ALLOC_LEFT(zbbuf, size, ptr)           No old buf API
#define ZB_BUF_ALLOC_RIGHT(zbbuf, size, ptr)          No old buf API
#define ZB_BUF_BEGIN_FROM_REF(_ref)                   No old buf API
#define ZB_BUF_BEGIN(zbbuf)                           No old buf API
#define ZB_BUF_CLEAR_PARAM(zbbuf)                     No old buf API
#define ZB_BUF_COPY(dst_buf, src_buf)                 No old buf API
#define ZB_BUF_CUT_LEFT2(zbbuf, size)                 No old buf API
#define ZB_BUF_CUT_LEFT(zbbuf, size, ptr)             No old buf API
#define ZB_BUF_CUT_RIGHT(zbbuf, size)                 No old buf API
#define ZB_BUF_FROM_REF(ref)                          No old buf API
#define ZB_BUF_GET_ARRAY_PTR(zbbuf)                   No old buf API
#define ZB_BUF_INITIAL_ALLOC(zbbuf, size, ptr)        No old buf API
#define ZB_BUF_LEN_BY_REF(param)                      No old buf API
#define ZB_BUF_LEN(zbbuf)                             No old buf API
#define ZB_BUF_OFFSET(zbbuf)                          No old buf API
#define ZB_BUF_REUSE(zbbuf)                           No old buf API
#define ZB_FREE_BUF(buf)                              No old buf API
#define ZB_FREE_BUF_BY_REF(param)                     No old buf API
#define ZB_GET_BUF_PARAM(zbbuf, type)                 No old buf API
#define ZB_GET_BUF_TAIL(zbbuf, size)                  No old buf API
#define ZB_GET_IN_BUF()                               No old buf API
#define ZB_GET_IN_BUF_DELAYED(callback)               No old buf API
#define ZB_GET_OUT_BUF()                              No old buf API
#define ZB_GET_OUT_BUF_DELAYED2(callback, user_param) No old buf API
#define ZB_GET_OUT_BUF_DELAYED(callback)              No old buf API
#define ZB_REF_FROM_BUF(buf)                          No old buf API
#define ZB_REF_FROM_BUF_PTR(ptr)                      No old buf API
#define ZB_SET_BUF_PARAM_PTR(zbbuf, param, type)      No old buf API
#define ZB_SET_BUF_PARAM(zbbuf, param, type)          No old buf API
#define ZB_SWITCH_BUF(buf, to_in)                     No old buf API
#endif /* !defined ZB_LEGACY_BUFS */

/*
 * Static compile time assertion to make sure that 'zb_bool_t' has the size
 * of exactly one byte, regardless of the used platform.
 */
ZB_ASSERT_TYPE_SIZE_NOT_GREATER(zb_bool_t, 1);

#endif /* ZB_MINIMAL_CONTEXT */

#endif /* ZB_COMMON_H */
