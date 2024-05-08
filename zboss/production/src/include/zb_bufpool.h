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
/* PURPOSE: Packet buffers pool - internal legacy API
*/

#ifndef ZB_BUFPOOL_H
#define ZB_BUFPOOL_H 1

#include "zb_config.h"
#include "zb_types.h"
#include "zboss_api_core.h"
#include "zboss_api_buf.h"

/* Definitions common to both implementations */

#define ZB_BPOOL_USAGES_RB_SIZE 2U

#define ZB_BUFS_LIMIT (ZB_IOBUF_POOL_SIZE / 2U)
#define ZB_BUFS_HI_PRIOR_RESERVE (2U)
#ifdef ZB_REDUCE_NWK_LOAD_ON_LOW_MEMORY
#define ZB_BUFS_RESERVE 3U
#endif

#define ZB_BUF_IS_OOM_STATE_IN()  (ZG->bpool.bufs_allocated[1] >= (ZB_BUFS_LIMIT - ZB_BUFS_HI_PRIOR_RESERVE))

#define ZB_BUF_IS_OOM_STATE_OUT() (ZG->bpool.bufs_allocated[0] >= (ZB_BUFS_LIMIT - ZB_BUFS_HI_PRIOR_RESERVE))

#define ZB_BUF_IS_OOM_STATE() (ZB_BUF_IS_OOM_STATE_IN() || ZB_BUF_IS_OOM_STATE_OUT())

#if !defined ZB_MACSPLIT_DEVICE && !defined ZB_MINIMAL_CONTEXT
#define ZB_NWK_UNLOCK_IN(bufid) zb_nwk_unlock_in((bufid))
#else
#define ZB_NWK_UNLOCK_IN(bufid)
#endif

#define ZB_BUF_GET_MULTIPLICITY(buf_hdr) ((buf_hdr)->multiplicity)

/* Differing definitions */

#if defined ZB_LEGACY_BUFS /* old buffer implementation */

#include "zb_bufpool_legacy.h"

#else /* new buffer implementation */

/* Storage functions */

/**
   Buffer storage initialization
 */
void zb_bufpool_storage_init(void);

/**
   Allocate a new buffer entry of specified multiplicity:
   the allocated buffer's maximum size will equal at least that of buf_cnt buffers

   @param buf_cnt - required buffer multiplicity
   @return pointer to the allocated buffer or NULL
 */
zb_buf_ent_t *zb_bufpool_storage_allocate(zb_uint8_t buf_cnt);

/**
   Release a previously allocated buffer back to the storage

   @param buf - pointer to the buffer
 */
void zb_bufpool_storage_free(zb_buf_ent_t *buf);

/* Even if we return buf ids from zb_bufpool_storage_allocate, we would still need the conversion
   from the buffer ptr to the buffer id: zb_buf_from_data0
*/
/**
   Convert a buffer pointer to its index in the global buffer array

   @param buf - pointer to the buffer
   @return zero-based numerical index of the buffer
 */
zb_bufid_t zb_bufpool_storage_buf_to_bufid(zb_buf_ent_t *buf);

/**
   Convert a zero-based buffer index to the corresponding pointer

   @param buf_id - zero-based buffer index
   @return pointer to the corresponding zb_buf_ent_t structure
 */
zb_buf_ent_t *zb_bufpool_storage_bufid_to_buf(zb_bufid_t buf_id);

/* we need this function from zb_bufpool_mult.c only since it needs to calculate and know
   the required number of buffers for each buffer allocation.
*/
/**
   Calculate minimal buffer multiplicity required to store packet of the specified size

   @param payload_size - target payload size
   @return required buffer multiplicity
 */
zb_uint8_t zb_bufpool_storage_calc_multiplicity(zb_uint16_t payload_size);

/**
   Calculate the maximum payload size that a buffer of the specified multiplicity can hold.

   @param buf_cnt - buffer multiplicity
   @return maximum payload size
 */
zb_size_t zb_bufpool_storage_calc_payload_size(zb_uint8_t buf_cnt);

#ifdef ZB_BUF_SHIELD

zb_bool_t zb_bufpool_storage_is_buf_corrupted(zb_buf_ent_t *buf);

#define ZB_BUF_IS_BUSY(param) (ZG->bpool.buf_in_use[(param) / 8U] & (1U<<((param) % 8U)))

#endif /* #ifdef ZB_BUF_SHIELD */

#endif /* #if defined ZB_LEGACY_BUFS */


/* API functions common to both buffer implementations */

/**
   Set length and data_offset fields in the buffer's header

   @param buf - buffer id
   @param len - new length
   @param offset - new offset
 */
void zb_buf_set_len_and_offset_func(TRACE_PROTO zb_bufid_t buf, zb_uint16_t len, zb_uint16_t offset);
#define zb_buf_set_len_and_offset(a,b,c) zb_buf_set_len_and_offset_func(TRACE_CALL (a), (b), (c))

/**
   Return pointer to the first element of buffer array.

   @param buf - buffer id

   @return pointer to the buffer array
 */
zb_uint8_t *zb_buf_data0_func(TRACE_PROTO zb_bufid_t buf);
#define zb_buf_data0(a) zb_buf_data0_func(TRACE_CALL (a))

/**
   Convert buffer pointer into bufid.
   ptr can be obtained from zb_buf_data0() function.

   @param ptr - pointer to the buffer array
   @return buffer id
 */
zb_bufid_t zb_buf_from_data0_func(TRACE_PROTO void *ptr);
#define zb_buf_from_data0(a) zb_buf_from_data0_func(TRACE_CALL (a));

#ifdef ZB_MACSPLIT
/**
   Serialize internal buffer structure into linear array.
   Output structure is: | header | body | parameter w/o leading zeros |

   @param buf - buffer id
   @param ptr - pointer for data output
   @return serialized data size
 */
zb_uint8_t zb_buf_serialize_func(TRACE_PROTO zb_bufid_t buf, zb_uint8_t *ptr);
#define zb_buf_serialize(a, b) zb_buf_serialize_func(TRACE_CALL (a), (b))

/**
   Deserialize linear array into ZBOSS buffer.
   Array structure must be the same as in @see zb_buf_serialize_func.

   @param buf - buffer id
   @param ptr - pointer to array
   @param payload_size - array size
 */
void zb_buf_deserialize_func(TRACE_PROTO zb_bufid_t buf, zb_uint8_t *ptr, zb_uint8_t payload_size);
#define zb_buf_deserialize(a, b, c) zb_buf_deserialize_func(TRACE_CALL (a),(b),(c))

/**
   Partial deserialize linear array.
   Calculate body pointer and its offset from ptr.

   @param ptr - pointer to array
   @param size - header size
   @return pointer to serialized body
 */
zb_uint8_t *zb_buf_partial_deserialize_func(TRACE_PROTO zb_uint8_t *ptr, zb_uint8_t *size);
#define zb_buf_partial_deserialize(a, b) zb_buf_partial_deserialize_func(TRACE_CALL (a),(b))
#endif /* ZB_MACSPLIT*/

#ifdef ZB_TH_ENABLED
/**
   Set length field in the buffer's header

   @param buf - buffer id
   @param len - new length
 */
void zb_buf_set_len_func(TRACE_PROTO zb_bufid_t buf, zb_uint16_t len);
#define zb_buf_set_len(a,b) zb_buf_set_len_func(TRACE_CALL (a),(b))
#endif
/**
   Return pointer to the data by offset off from data section start.

   @param buf - buffer id
   @param off - offset from the data section begin

   @return pointer to the data
 */
void *zb_buf_data_func(TRACE_PROTO zb_bufid_t buf, zb_uint_t off);
#define zb_buf_data(a, b) zb_buf_data_func(TRACE_CALL (a),(b))

#ifdef ZB_TH_ENABLED
/**
   Get 'data_offset' field of the buffer's header

   @param buf - buffer id
   @return data_offset field value
 */
zb_uint16_t zb_buf_get_offset_func(TRACE_PROTO zb_bufid_t buf);
#define zb_buf_get_offset(a) zb_buf_get_offset_func(TRACE_CALL (a))
#endif /* ZB_TH_ENABLED */


/**
 * @brief Changes buffer type from "in" to "out" when "in" buffers count is greater then "out" buffers count.
 *
 * Buffer type requalification can be needed in cases when "in" buffers is reused to send response for some incoming
 * packet.
 *
 * @param buf - buffer id to change type.
 * @return RET_OK or error code.
 */
zb_ret_t zb_buf_requalify_in_to_out_func(TRACE_PROTO zb_bufid_t buf);
#define zb_buf_requalify_in_to_out(buf) zb_buf_requalify_in_to_out_func(TRACE_CALL (buf))

/**
   Initialize packet buffers pool.

   To be called at start time.

   @return nothing

   Note: internal call.
 */
void zb_init_buffers(void);

#endif /* ZB_BUFPOOL_H */
