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
/*  PURPOSE: Contexts for ZBOSS configurable memory
*/
#ifndef ZB_MEM_CONFIG_CONTEXT_H
#define ZB_MEM_CONFIG_CONTEXT_H 1


/*! @internal */
/*! @{ */

/**
   @defgroup configurable_mem_internals
   @{
   @details
   Design of configurable ZBOSS internal memory provides an ability to
   configure some storage parameters at application link time without
   recompiling ZBOSS libraries.
   If application does not configure storage parameters, default values will be used.

   ZBOSS uses static memory allocation technique.

   Initially (before introducing configurable memory) ZBOSS used static arrays inside data structures and defines for array's sizes.
   Defines are fixed at ZBOSS compile time. @see zb_buf_pool_t.pool as an example.

   To be able to configure memory at compile time, ZBOSS changes static buffers to pointers (@see zb_buf_pool_t.pool again).
   Size constants are redefined to variables (@see gc_pool_size).
   So ZBOSS code mostly not changed and can be compiled with or without configurable memory feature.

   This header file defines variables for buffers and variables for buffer sizes.
   That file can be included strictly once from the kernel (zb_init.c) or
   from the user's application (indirectly, from file zb_mem_config_xxx.h).
   When included form the kernel, ZB_CONFIG_DEFAULT_KERNEL_DEFINITION is defined, so all symbols are defined as weak.
   As a result, if application does not included any memory configuration includes,
   default buffers and its sizes (weak) are used.
   If application includes one of zb_mem_config_xxx.h, it overwrites weak symbols for buffers and its sizes.

   Pointers assignment (like @ref zb_buf_pool_t.pool) and arrays syzes
   initialization are done at ZBOSS start time from @ref zb_init_configurable_mem().

   Default storage settings are defined in zb_vendor.h file.
   @note Changing zb_vendor.h is meaningful only if you re-compile ZBOSS library.
   Do not change it when you compile the application.

   See also @ref configurable_mem.
   @}
*/

#if defined ZB_CONFIGURABLE_MEM || defined DOXYGEN

/*
  some data structures use 7 bits to address a buffer, so can have up to 127 buffers.
  Actually it is big enough because more than 48 are rarely used..
 */
#if ZB_CONFIG_IOBUF_POOL_SIZE > 127U
#error ZB_CONFIG_IOBUF_POOL_SIZE must be <= 127
#endif

/**@brief Maximum buffer index.
 *        This macro is just an abbreviation for a corresponding macro in ZBOSS sources and its value must not be changed.
 */
#define ZB_CONFIG_N_BUF_IDS (ZB_CONFIG_IOBUF_POOL_SIZE + 1U)

/**
   @par
   Define default configuration in the kernel lib as a weak symbol
 */
#if defined ZB_CONFIG_DEFAULT_KERNEL_DEFINITION || defined DOXYGEN

/**
 Kernel's default buffers. Define weak global variables.
*/
#define ZB_CONFIG_POST ZB_WEAK
#define ZB_CONFIG_PRE ZB_WEAK_PRE ZB_ALIGNED_PRE
#define ZB_CONFIG_USE_DEFAULTS 1U

#else

/* User's app. Just define global variables. */
#define ZB_CONFIG_POST
#define ZB_CONFIG_PRE ZB_ALIGNED_PRE
#define ZB_CONFIG_USE_DEFAULTS 0U

#endif


/**
   If user did not owerwrite default memory configuration, that variable value is 1, else 0.

   Currently used in trace only, but, maybe, implement user API for it if customer require it?
 */
ZB_CONFIG_PRE zb_uint_t gc_use_defaults ZB_CONFIG_POST = ZB_CONFIG_USE_DEFAULTS;

/* Can't put all that variables into the struct because its sizes will
 * be different for in-kernel default case and in the application
 * which overwritten some sizes. So declare variables one by one. */

/* If we are here from the user's app, here are buffers
   definition. Application must be here only once.

   If we are here from the kernel, it can be either default buffers
   definition or variables declaration (including variables storing
   sizes).
 */


/**
Buffer pool:
ZB_IOBUF_POOL_SIZE and derivatives.

Arrays directly defined by ZB_IOBUF_POOL_SIZE:

zb_zdo_globals.h
nwk_addr_req_pending_tsns
nwk_addr_req_pending_mask

zb_bufpool_globals.h

zb_buf_pool_t.pool
zb_buf_pool_t.bufs_busy_bitmap
zb_buf_pool_t.buf_in_use
 */
ZB_CONFIG_PRE zb_buf_ent_t gc_iobuf_pool[ZB_CONFIG_IOBUF_POOL_SIZE] ZB_CONFIG_POST = { 0 };
ZB_CONFIG_PRE zb_uint8_t gc_bufs_busy_bitmap[ZB_CONFIG_BUF_POOL_BITMAP_SIZE] ZB_CONFIG_POST = { 0 };
ZB_CONFIG_PRE zb_uint8_t gc_buf_pool_bitmap_size ZB_CONFIG_POST = ZB_CONFIG_BUF_POOL_BITMAP_SIZE;
#ifdef ZB_BUF_SHIELD
ZB_CONFIG_PRE zb_uint8_t gc_iobuf_buf_in_use[(ZB_CONFIG_IOBUF_POOL_SIZE + 7)/8] ZB_CONFIG_POST = { 0 };
#endif

/**
Input packets queue

zb_nwk_globals.h

zb_nwk_handle_t.input_q
*/
ZB_RING_BUFFER_DECLARE(zb_nwk_in_q_buf, zb_uint8_t, (ZB_CONFIG_IOBUF_POOL_SIZE/2));
ZB_CONFIG_PRE zb_nwk_in_q_buf_t gc_nwk_in_q ZB_CONFIG_POST = { 0 };

/**
TSNs pending for address request complete.

zb_zdo_globals.h
nwk_addr_req_pending_tsns
nwk_addr_req_pending_mask
*/
ZB_CONFIG_PRE zb_uint8_t gc_nwk_addr_req_pending_tsns[ZB_CONFIG_N_BUF_IDS] ZB_CONFIG_POST = { 0 };
ZB_CONFIG_PRE zb_uint8_t gc_nwk_addr_req_pending_mask[(ZB_CONFIG_N_BUF_IDS + 7) / 8] ZB_CONFIG_POST = { 0 };

/**
Node Desc feature: mask of pending pkt for every buffer

zb_zdo_globals.h
node_desc_req_pending_mask
*/
ZB_CONFIG_PRE zb_uint8_t gc_node_desc_req_pending_mask[(ZB_CONFIG_N_BUF_IDS + 7) / 8] ZB_CONFIG_POST = { 0 };

/**
MAC pending TX queue (FFDs).

zb_mac_globals.h
ZB_MAC_PENDING_QUEUE_SIZE (ZB_IOBUF_POOL_SIZE / 4)

 */
#ifdef ZB_ROUTER_ROLE
ZB_CONFIG_PRE zb_mac_pending_data_t gc_mac_pending_data_queue[ZB_CONFIG_MAC_PENDING_QUEUE_SIZE] ZB_CONFIG_POST = { 0 };
#endif

#ifdef ZB_MAC_SOFTWARE_PB_MATCHING
ZB_CONFIG_PRE zb_uint_t gc_child_hash_table_size ZB_CONFIG_POST = ZB_CONFIG_CHILD_HASH_TABLE_SIZE;

ZB_CONFIG_PRE zb_uint16_t gc_child_hash_table[ZB_CONFIG_CHILD_HASH_TABLE_SIZE] ZB_CONFIG_POST = { 0 };
ZB_CONFIG_PRE zb_uint32_t gc_pending_bitmap[ZB_CONFIG_PENDING_BITMAP_SIZE] ZB_CONFIG_POST = { 0 };

#ifdef ZB_MAC_POLL_INDICATION_CALLS_REDUCED
ZB_CONFIG_PRE zb_time_t gc_poll_timestamp_table[ZB_CONFIG_CHILD_HASH_TABLE_SIZE] ZB_CONFIG_POST = { 0 };
ZB_CONFIG_PRE zb_uint16_t gc_poll_timeout_table[ZB_CONFIG_CHILD_HASH_TABLE_SIZE] ZB_CONFIG_POST = { 0 };
#endif /* ZB_MAC_POLL_INDICATION_CALLS_REDUCED */

#endif  /* ZB_MAC_SOFTWARE_PB_MATCHING */

/**
  APS bound transmission buffers

  zb_aps_globals.h

  zb_aps_bind_dst_table_t.trans_index
  */

ZB_CONFIG_PRE zb_uint8_t gc_trans_table_size ZB_CONFIG_POST = ZB_CONFIG_APS_BIND_TRANS_TABLE_SIZE;
ZB_CONFIG_PRE zb_uint8_t gc_single_trans_index_size ZB_CONFIG_POST = ZB_CONFIG_SINGLE_TRANS_INDEX_SIZE;
ZB_CONFIG_PRE zb_uint8_t gc_trans_index_buf[ZB_CONFIG_APS_DST_BINDING_TABLE_SIZE][ZB_CONFIG_SINGLE_TRANS_INDEX_SIZE] ZB_CONFIG_POST = { 0 };
ZB_CONFIG_PRE zb_uint8_t gc_trans_table[ZB_CONFIG_APS_BIND_TRANS_TABLE_SIZE] ZB_CONFIG_POST = { 0 };

/**
  APS bind tables
  
  zb_aps_globals.h

  ZG->aps.binding.zb_aps_bind_src_table_t
  ZG->aps.binding.zb_aps_bind_dst_table_t
 */
ZB_CONFIG_PRE zb_uint8_t gc_aps_bind_src_table_size ZB_CONFIG_POST = ZB_CONFIG_APS_SRC_BINDING_TABLE_SIZE;
ZB_CONFIG_PRE zb_uint8_t gc_aps_bind_dst_table_size ZB_CONFIG_POST = ZB_CONFIG_APS_DST_BINDING_TABLE_SIZE;
ZB_CONFIG_PRE zb_aps_bind_src_table_t gc_aps_bind_src_table[ZB_CONFIG_APS_SRC_BINDING_TABLE_SIZE] ZB_CONFIG_POST = { 0 };
ZB_CONFIG_PRE zb_aps_bind_dst_table_t gc_aps_bind_dst_table[ZB_CONFIG_APS_DST_BINDING_TABLE_SIZE] ZB_CONFIG_POST = { 0 };

/**
   APS retransmission buffer

  zb_aps_globals.h
  zb_aps_retrans_t.hash
 */

ZB_CONFIG_PRE zb_aps_retrans_ent_t gc_aps_retrans[ZB_CONFIG_N_APS_RETRANS_ENTRIES] ZB_CONFIG_POST = { 0 };

/*
  Declarations of variables defining parameters sizes.
 */

ZB_CONFIG_PRE zb_uint_t gc_pool_size ZB_CONFIG_POST = ZB_CONFIG_IOBUF_POOL_SIZE;
ZB_CONFIG_PRE zb_uint_t gc_mac_pending_queue_size ZB_CONFIG_POST = ZB_CONFIG_MAC_PENDING_QUEUE_SIZE;
ZB_CONFIG_PRE zb_uint_t gc_n_aps_retrans_entries ZB_CONFIG_POST = ZB_CONFIG_N_APS_RETRANS_ENTRIES;


/* ZB_CONFIG_SCHEDULER_Q_SIZE */
ZB_RING_BUFFER_DECLARE(zb_cb_q_buf, zb_cb_q_ent_t, ZB_CONFIG_SCHEDULER_Q_SIZE);
ZB_CONFIG_PRE zb_cb_q_buf_t gc_cb_q ZB_CONFIG_POST = { 0 };
ZB_CONFIG_PRE zb_tm_q_ent_t gc_tm_buf[ZB_CONFIG_SCHEDULER_Q_SIZE] ZB_CONFIG_POST = { 0 };
ZB_CONFIG_PRE zb_uint32_t gc_cb_flag_bm[(ZB_CONFIG_SCHEDULER_Q_SIZE + 31)/32] ZB_CONFIG_POST = { 0 };
ZB_CONFIG_PRE zb_uint_t gc_sched_q_size ZB_CONFIG_POST = ZB_CONFIG_SCHEDULER_Q_SIZE;
ZB_CONFIG_PRE zb_uint_t gc_sched_stack_unprotected_q_size ZB_CONFIG_POST  = ZB_CONFIG_SCHEDULER_Q_SIZE_PROTECTED_STACK_POOL;


/* ZB_N_APS_KEY_PAIR_ARR_MAX_SIZE */
#if defined ZB_COORDINATOR_ROLE && defined ZB_SECURITY_INSTALLCODES
ZB_CONFIG_PRE zb_aps_installcode_storage_t gc_installcodes_table[ZB_CONFIG_N_APS_KEY_PAIR_ARR_MAX_SIZE] ZB_CONFIG_POST = { 0 };
#endif
ZB_CONFIG_PRE zb_aps_device_key_pair_array_t gc_key_pair_set[ZB_CONFIG_N_APS_KEY_PAIR_ARR_MAX_SIZE] ZB_CONFIG_POST = { 0 };
ZB_CONFIG_PRE zb_uint_t gc_n_aps_key_pair ZB_CONFIG_POST = ZB_CONFIG_N_APS_KEY_PAIR_ARR_MAX_SIZE;


/* ZB_IEEE_ADDR_TABLE_SIZE */
ZB_CONFIG_PRE zb_address_map_t gc_addr_map[ZB_CONFIG_IEEE_ADDR_TABLE_SIZE] ZB_CONFIG_POST = { 0 };
ZB_CONFIG_PRE zb_uint8_t gc_short_sorted[ZB_CONFIG_IEEE_ADDR_TABLE_SIZE] ZB_CONFIG_POST = { 0 };
ZB_CONFIG_PRE zb_uint8_t gc_addr_to_neighbor[ZB_CONFIG_IEEE_ADDR_TABLE_SIZE] ZB_CONFIG_POST = { 0 };
ZB_CONFIG_PRE zb_uint_t gc_addr_table_size ZB_CONFIG_POST = ZB_CONFIG_IEEE_ADDR_TABLE_SIZE;

/* ZB_NEIGHBOR_TABLE_SIZE */
#ifdef ZB_ROUTER_ROLE
ZB_CONFIG_PRE zb_uint8_t gc_passive_ack[ZB_NWK_BRR_TABLE_SIZE][((ZB_CONFIG_NEIGHBOR_TABLE_SIZE + 7) / 8)] ZB_CONFIG_POST = { 0 };
#endif

#ifdef ZB_ZCL_SUPPORT_CLUSTER_SUBGHZ
ZB_CONFIG_PRE zb_address_ieee_ref_t gc_subghz_dev[ZB_CONFIG_NEIGHBOR_TABLE_SIZE] ZB_CONFIG_POST = { 0 };
#endif

ZB_CONFIG_PRE zb_neighbor_tbl_ent_t gc_neighbor[ZB_CONFIG_NEIGHBOR_TABLE_SIZE] ZB_CONFIG_POST = { 0 };
ZB_CONFIG_PRE zb_uint_t gc_neighbor_table_size ZB_CONFIG_POST = ZB_CONFIG_NEIGHBOR_TABLE_SIZE;

/* ZB_NWK_ROUTING_TABLE_SIZE */
#ifdef ZB_ROUTER_ROLE
ZB_CONFIG_PRE zb_nwk_routing_t gc_routing_table[ZB_CONFIG_NWK_ROUTING_TABLE_SIZE] ZB_CONFIG_POST = { 0 };
ZB_CONFIG_PRE zb_uint_t gc_routing_table_size ZB_CONFIG_POST = ZB_CONFIG_NWK_ROUTING_TABLE_SIZE;
/* ZB_NWK_SOURCE_ROUTING_TABLE_SIZE */
#if defined ZB_PRO_STACK && !defined ZB_LITE_NO_SOURCE_ROUTING
/* 10/21/2019 EE CR:MAJOR ZC only! Not ZB_ROUTER_ROLE but ZB_COORDINATOR_ROLE */
ZB_CONFIG_PRE zb_nwk_rrec_t gc_src_routing_table[ZB_CONFIG_NWK_MAX_SOURCE_ROUTES] ZB_CONFIG_POST = { 0 };
ZB_CONFIG_PRE zb_uint8_t gc_nwk_max_source_routes ZB_CONFIG_POST = ZB_CONFIG_NWK_MAX_SOURCE_ROUTES;
#endif
#endif

/* ZB_APS_DUPS_TABLE_SIZE */
ZB_CONFIG_PRE zb_aps_dup_tbl_ent_t gc_dups_table[ZB_CONFIG_APS_DUPS_TABLE_SIZE] ZB_CONFIG_POST = { 0 };
ZB_CONFIG_PRE zb_uint_t gc_aps_dups_table_size ZB_CONFIG_POST = ZB_CONFIG_APS_DUPS_TABLE_SIZE;

#if defined NCP_MODE && !defined NCP_MODE_HOST
ZB_CONFIG_PRE zb_ncp_pending_calls_t gc_ncp_pending_calls[ZB_CONFIG_N_BUF_IDS] ZB_CONFIG_POST = { 0 };
#endif

#endif  /* ZB_CONFIGURABLE_MEM */

/*! @} */
/*! @} */

#endif /* ZB_MEM_CONFIG_CONTEXT_H */
