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
/*  PURPOSE: Public legacy packet buffers API
*/
#ifndef ZBOSS_API_BUF_LEGACY_H
#define ZBOSS_API_BUF_LEGACY_H 1

/**
 * @addtogroup bufleg
 * @{
 *   @par Pass parameters via packet buffers
 *
 *   Usual sequence of the data send/receive primitives is aps-nwk-mac down and up packet pass.
 *   Packets passed not by direct function calls but via scheduler to limit used stack size.
 *   Callback has single parameter only.
 *   Pass packet buffer as parameter.
 *   When possible, pass parameters by filling lower layer header (it is possible for nwk-mac data primitive).
 *   But usually will need a place for some additional arguments.
 *   Will put it into packet tail. Does not formally 'allocate' space at packet tail - just use it.
 *   Packet must have size enough to keep biggest possible frame + parameters.
 *   Function for getting packet tail for parameters checks for overflow and move
 *   data left if it is possible to free space at packet buffer tail.
 *
 *   Management calls have no data. All parameters passed via packet buffers.
 *
 *   @par Blockable memory allocation, reaction on memory absence
 *
 *   Main limited resource is RAM: controller has 16k only.
 *
 *   RAM used for i/o (packet) buffers, routing tables, neighbor tables, (what else?)
 *   and smaller data structures.
 *
 *   Packet buffers pool is common for in and out but its usage for In and Out is
 *   limited (50/50 initially, maybe, will tune it).  The rationale is to prevent
 *   stack stop due to inability to send data due to buffers absence if all buffers
 *   "eaten" as input.
 *
 *   Packet buffer allocation could be asynchronous operation - it means, allocate
 *   call does not return directly but via scheduler (callback call).  It allows
 *   packet allocation to block. In general, all allocation inside stack are blocked.
 *
 *   Output packets allocates at some upper layers (ZD0, ZCL, application etc).  It
 *   packet pool is empty, it means some buffers are sitting in the output queue or
 *   indirect transmit queue waiting for send. When it will be sent and freed,
 *   allocation operation unblocks (callback calls) and goes thru.
 *
 *   Input packets are always allocated in the MAC. If no space available, MAC receive
 *   blocks. It does not begin packet read if no space for it. If it blocks for a
 *   short time, it will be ok. If it blocks for long time, transceiver will probably
 *   skip next packet receive. Anyway, we will not split the packet: MAC transport
 *   will either have memory for entire packet or have no memory at all.
 */

#ifdef ZB_LEGACY_BUFS

#define zb_buf_t zb_leg_buf_t


/**
   BIG Packet buffer header.

   Never used yet.
 */
typedef ZB_PACKED_PRE struct zb_bigbuf_hdr_s
{
  zb_uint16_t len;              /*!< current layer buffer length  */
  zb_uint16_t data_offset;      /*!< data offset in buffer buf*/
  zb_uint8_t handle;           /*!< The handle associated with the NSDU to be
                                * transmitted by the NWK layer entity.  */
  zb_bitfield_t is_in_buf:1;    /*!< if 1, this is input buffer */

  zb_bitfield_t encrypt_type:3; /*!< payload must be encrypted before send, if
                                 * !0. \see zb_secur_buf_encr_type_e.
                                 */
  zb_bitfield_t use_same_key:1;    /*!< if 1, use same nwk key# packet was
                                    * encrypted by */
  zb_bitfield_t zdo_cmd_no_resp:1; /*!< if 1, this is ZDO command with no
                                    * response - call callback at confirm  */
  zb_bitfield_t is_rx_buf:1;    /*!< if 1, this is buffer with received packet and
                                 * nwk_mac_addrs_t is at buffer tail */
  zb_bitfield_t has_aps_payload:1;   /*!< if 1, than packet comes from APS */

  zb_int16_t status;            /*!< some status to be passed with packet  */
} ZB_PACKED_STRUCT zb_bigbuf_hdr_t;

/**
   BIG Packet buffer
 */
#ifdef ZB_BIGBUF
typedef struct zb_bigbuf_s
{
  union
  {
    zb_bigbuf_hdr_t hdr;                   /*!< Buffer header, if buffer is used */
    //struct zb_bigbuf_s *next;              /*!< Ref to next free buffer, if buffer is not used*/
  } u; /*!< Buffer header */
  //zb_uint8_t   *buf;                    /*!< Dynamically allocated buffer for payload */
  zb_uint8_t   *buf;                       /*!< Allocated buffer Data*/
  zb_uint16_t  buf_len;                   /*!< Allocated buffer Len*/
  //zb_uint8_t   flag;                      /*!< busy and other flags possible*/
#ifdef ZB_DEBUG_BUFFERS_EXT
  zb_buf_usage_t buf_allocation;
  zb_buf_usage_t buf_usages[ZB_DEBUG_BUFFERS_EXT_USAGES_COUNT];
#endif
} zb_bigbuf_t;
#endif /*ZB_BIGBUF*/

#define ZB_IS_BIGBUF(x) (((x)&0x80)>0)
#define ZB_BIGBUF_ID_FROM_REF(x) ((x)-0x80)
#define ZB_BIGBUF_REF_FROM_ID(x) ((x)+0x80)
/**
 * @brief Return pointer to the beginning of the buffer
 *
 * @param zbbuf - pointer to buffer @ref zb_buf_t
 *
 * @return pointer to beginning of buffer
 *
 * @note see @ref ZB_BUF_BEGIN(), which returns pointer to the beginning of data written
 */
#define ZB_BUF_GET_ARRAY_PTR(zbbuf) ((zbbuf)->buf)

#ifdef ZB_BUF_SHIELD

zb_bool_t zb_buf_check_full(zb_int_t file_id,zb_uint8_t param);


#define ZB_BUF_CHECK_INLINE(param) \
  ZB_INLINE_ASSERT(zb_buf_check_full(ZB_TRACE_FILE_ID, param))

#define ZB_BUF_CHECK(param) ZB_ASSERT(zb_buf_check_full(ZB_TRACE_FILE_ID, param))

#else

#define ZB_BUF_CHECK(param)
#define ZB_BUF_CHECK_INLINE(param)

#endif

/* Function prototypes.
   Do not use it directly. Use macro instead.
 */
#ifdef ZB_BIGBUF
zb_uint8_t zb_add_bigbuf(zb_uint8_t *data, uint16_t size);
zb_bigbuf_t *zb_get_bigbuf(
#ifdef ZB_DEBUG_BUFFERS
    zb_uint16_t from_file, zb_uint16_t from_line,
#endif
    zb_uint8_t priority);
#endif

/**
 * @copydoc ZB_BUF_BEGIN()
 *
 * @param file_id - TRACE_ID of the file from which function is invoked
 *
 * @note don't call it directly, use @ref ZB_BUF_BEGIN() instead
 */
zb_uint8_t* zb_buf_begin_leg(TRACE_PROTO zb_buf_t *zbbuf);


/**
 * @copydoc ZB_BUF_LEN()
 *
 * @param file_id - TRACE_ID of the file from which function is invoked
 *
 * @note don't call it directly, use @ref ZB_BUF_LEN() instead
 */
#ifdef ZB_BIGBUF
zb_uint16_t zb_buf_len_leg(TRACE_PROTO zb_buf_t *zbbuf);
#else
  zb_uint8_t zb_buf_len_leg(TRACE_PROTO zb_buf_t *zbbuf);
#endif

/**
   Get IN buffer from the buffers list.
   If no buffers available, does not block.

   @return pointer to the buffer or NULL if no buffer available.
 */
#ifdef ZB_DEBUG_BUFFERS
zb_buf_t *zb_get_in_buf(zb_uint16_t from_file, zb_uint16_t from_line);

/**
   Get IN buffer from the buffers list.
   If no buffers available, does not block.

   @return pointer to the buffer or NULL if no buffer available.
 */
#define ZB_GET_IN_BUF() zb_get_in_buf(ZB_TRACE_FILE_ID, __LINE__)
#else
zb_buf_t *zb_get_in_buf(void);

/**
   Get IN buffer from the buffers list.
   If no buffers available, does not block.

   @return pointer to the buffer or NULL if no buffer available.
 */
#define ZB_GET_IN_BUF() zb_get_in_buf()
#endif

/**
   Get OUT buffer from the buffers list.
   If no buffers available, does not block.
   @return pointer to the buffer.
 */
#ifdef ZB_DEBUG_BUFFERS
zb_buf_t *zb_get_out_buf(zb_uint16_t from_file, zb_uint16_t from_line);

/**
   Get OUT buffer from the buffers list.
   If no buffers available, does not block.
   @return pointer to the buffer.
 */
#define ZB_GET_OUT_BUF() zb_get_out_buf(ZB_TRACE_FILE_ID, __LINE__)
#else
zb_buf_t *zb_get_out_buf(void);

/**
   Get OUT buffer from the buffers list.
   If no buffers available, does not block.
   @return pointer to the buffer.
 */
#define ZB_GET_OUT_BUF() zb_get_out_buf()
#endif
/** @endcond */ /* internals_doc */


/**
 * @copydoc ZB_BUF_FROM_REF()
 *
 * @note Don't call it directly, use @ref ZB_BUF_FROM_REF() instead.
 *
 * @param from_file - TRACE_ID of the file from which function is invoked
 * @param from_line - line from which function is invoked
 */
zb_buf_t* zb_buf_from_ref(zb_uint8_t ref, zb_uint16_t from_file, zb_uint16_t from_line);

/**
 * Calculate pointer of the buffer by buffer reference.
 *
 * @param ref - buffer reference
 *
 * @return pointer to buffer header
*/
#ifdef ZB_USE_STD_MALLOC
#define ZB_BUF_FROM_REF(ref) (ZG->bpool.pool[ref])
#else
#define ZB_BUF_FROM_REF(ref) (zb_buf_from_ref((ref), ZB_TRACE_FILE_ID, __LINE__))
#endif

#ifdef ZB_USE_STD_MALLOC
#define ZB_REF_FROM_BUF(buf) get_buf_ref(buf)
#else

/**
 * @copydoc ZB_REF_FROM_BUF()
 *
 * @note don't call it directly, use @ref ZB_REF_FROM_BUF() instead
 */
zb_uint8_t zb_ref_from_buf(zb_buf_t *buf);

/**
 * @brief get buffer reference which can be passed to scheduler callback
 *
 * @param buf - buffer of interest
 *
 * @return reference to buffer
 */
#define ZB_REF_FROM_BUF(buf) zb_ref_from_buf(buf)

#endif


/**
 * @brief get buffer reference  which can be passed to scheduler callback
 *
 * @param ptr - direct pointer to buffer's content
 *
 * @return reference to buffer
 */
#if defined ZB_USE_STD_MALLOC && defined ZB_USE_ALLOC_FOR_PAYLOAD_DATA
#error This macro still needs to be implemented!
#else
#define ZB_REF_FROM_BUF_PTR(ptr) ZB_REF_FROM_BUF((zb_buf_t*)(void*)((zb_int8_t *)(ptr) - ZB_OFFSETOF(zb_buf_t, buf)))
#endif


/**
 * Return pointer to data stored in buffer
 *
 * @param zbbuf - buffer
 *
 * @return pointer to beginning of data in buffer
 *
 * @note see @ref ZB_BUF_GET_ARRAY_PTR() which returns begining of buffer
 */
#define ZB_BUF_BEGIN(zbbuf) zb_buf_begin_leg(TRACE_CALL zbbuf)

/**
 * Return current buffer length
 *
 * @param zbbuf - pointer to buffer
 *
 * @return size of data placed in buffer
 */

#define ZB_BUF_LEN(zbbuf) zb_buf_len_leg(TRACE_CALL zbbuf)

/**
   Return current buffer length by buffer reference.
 */
#define ZB_BUF_LEN_BY_REF(param) ZB_BUF_LEN(ZB_BUF_FROM_REF(param))

/**
   Return pointer to data stored in buffer by buffer reference.
 */
#define ZB_BUF_BEGIN_FROM_REF(_ref) ZB_BUF_BEGIN(ZB_BUF_FROM_REF(_ref))

/**
 * @copydoc ZB_BUF_COPY()
 *
 * @note don't call it directly, use @ref ZB_BUF_COPY() instead
 */
void zb_buf_copy_leg(TRACE_PROTO zb_buf_t *dst_buf, zb_buf_t *src_buf);

/**
   Copy one buffer to another

   @param src_buf - source buffer
   @param dst_buf - destination buffer
 */
#define ZB_BUF_COPY(dst_buf, src_buf) zb_buf_copy_leg(TRACE_CALL dst_buf, src_buf)

/**
 * @copydoc ZB_BUF_OFFSET()
 *
 * @param file_id - TRACE_ID of the file from which function is invoked
 *
 * @note don't call it directly, use @ref ZB_BUF_OFFSET() instead
 */
#ifdef ZB_BIGBUF
zb_uint16_t zb_buf_offset(zb_int_t file_id, zb_buf_t *zbbuf);
#else
zb_uint8_t zb_buf_offset(zb_int_t file_id, zb_buf_t *zbbuf);
#endif

/**
 * Return current buffer offset
 *
 * @param zbbuf - pointer to buffer
 *
 * @return offset of the data in the buffer
 */
#define ZB_BUF_OFFSET(zbbuf) zb_buf_offset(ZB_TRACE_FILE_ID, zbbuf)


void zb_buf_clear_param(zb_buf_t *zbbuf);

/**
 * Clear param area in buffer
 *
 * @param zbbuf - pointer to buffer
 */
#define ZB_BUF_CLEAR_PARAM(zbbuf) zb_buf_clear_param(zbbuf)

/**
 * @copybrief ZB_BUF_INITIAL_ALLOC()
 *
 * @param zbbuf - buffer
 * @param size  - size to allocate
 *
 * @note don't call it directly, use @ref ZB_BUF_INITIAL_ALLOC() instead
 */
#ifdef ZB_BIGBUF
void *zb_buf_initial_alloc_leg(TRACE_PROTO zb_buf_t *zbbuf, zb_uint16_t size);
#else
void *zb_buf_initial_alloc_leg(TRACE_PROTO zb_buf_t *zbbuf, zb_uint8_t size);
#endif

/**
   @brief Initial space allocation in buffer.

   Allocate space in the buffer center (keep space in both buffer head
   and tail). Use it at upper layers before filling buffer by data.
   Old buffer contents is lost.

   @param zbbuf - buffer
   @param size  - size to allocate
   @param ptr   - (out) pointer to the new buffer begin

   @return pointer to the allocated space
 */
#define ZB_BUF_INITIAL_ALLOC(zbbuf, size, ptr) (ptr) = zb_buf_initial_alloc_leg(TRACE_CALL (zbbuf), (size))

/**
 * @copybrief ZB_BUF_ALLOC_LEFT()
 *
 * @param zbbuf - buffer
 * @param size  - size to allocate
 *
 * @note don't call it directly, use @ref ZB_BUF_ALLOC_LEFT() instead
*/
#ifdef ZB_BIGBUF
void *zb_buf_smart_alloc_left(zb_buf_t *zbbuf, zb_uint16_t size);
#else
void *zb_buf_smart_alloc_left(zb_buf_t *zbbuf, zb_uint8_t size);
#endif

/**
 * Allocate space at the beginning of buffer
 *
 * @param zbbuf - buffer
 * @param size  - size to allocate
 * @param ptr   - (out) pointer to the new buffer begin
 */
#define ZB_BUF_ALLOC_LEFT(zbbuf, size, ptr) (ptr) = zb_buf_smart_alloc_left((zbbuf), (size))

/**
 * @copybrief ZB_BUF_ALLOC_RIGHT()
 *
 * @param zbbuf - buffer
 * @param size  - size to allocate
 *
 * @note don't call it directly, use @ref ZB_BUF_ALLOC_RIGHT() instead
 */
#ifdef ZB_BIGBUF
void *zb_buf_smart_alloc_right(zb_buf_t *zbbuf, zb_uint16_t size);
#else
void *zb_buf_smart_alloc_right(zb_buf_t *zbbuf, zb_uint8_t size);
#endif

/**
   Allocate space at buffer end

   @param zbbuf - buffer
   @param size  - size to allocate
   @param ptr   - (out) pointer to the space allocated
 */
#define ZB_BUF_ALLOC_RIGHT(zbbuf, size, ptr) (ptr) = zb_buf_smart_alloc_right((zbbuf), (size))

/**
 * @copybrief ZB_BUF_CUT_LEFT()
 *
 * @param zbbuf - buffer
 * @param size  - size to cut
 *
 * @note don't call it directly, use @ref ZB_BUF_CUT_LEFT() instead
 */
#ifdef ZB_BIGBUF
void *zb_buf_cut_left_leg(zb_buf_t *zbbuf, zb_uint16_t size);
#else
void *zb_buf_cut_left_leg(zb_buf_t *zbbuf, zb_uint8_t size);
#endif

/**
 * Cut space at the beginning of buffer
 *
 * @param zbbuf - buffer
 * @param size  - size to cut
 * @param ptr   - (out) pointer to the new buffer begin
*/
#define ZB_BUF_CUT_LEFT(zbbuf, size, ptr)  (ptr) = zb_buf_cut_left_leg((zbbuf), (size))

#ifdef ZB_BIGBUF
void zb_buf_cut_left2(zb_int_t file_id, zb_buf_t *zbbuf, zb_uint16_t size);
#else
void zb_buf_cut_left2(zb_int_t file_id, zb_buf_t *zbbuf, zb_uint8_t size);
#endif
#define ZB_BUF_CUT_LEFT2(zbbuf, size) zb_buf_cut_left2(ZB_TRACE_FILE_ID, zbbuf, size)

/**
 * @copybrief ZB_BUF_CUT_RIGHT()
 *
 * @param zbbuf - buffer
 * @param size  - size to cut
 *
 * @note don't call it directly, use @ref ZB_BUF_CUT_RIGHT() instead
 */
#ifdef ZB_BIGBUF
void zb_buf_cut_right_leg(zb_buf_t *zbbuf, zb_uint16_t size);
#else
void zb_buf_cut_right_leg(zb_buf_t *zbbuf, zb_uint8_t size);
#endif

/**
 * Cut space at the end of buffer
 *
 * @param zbbuf - buffer
 * @param size  - size to cut
 */
#define ZB_BUF_CUT_RIGHT(zbbuf, size) zb_buf_cut_right_leg((zbbuf), (size))

/**
 * @copydoc ZB_GET_BUF_TAIL()
 *
 * @note don't call directly, use @ref ZB_GET_BUF_TAIL() instead
 */
#ifdef ZB_BIGBUF
void *zb_get_buf_tail(zb_buf_t *zbbuf, zb_uint16_t size);
#else
void *zb_get_buf_tail(zb_buf_t *zbbuf, zb_uint8_t size);
#endif

/**
   Get buffer tail of size 'size'

   Usually used to place external information (some parameters) to the buffer.

   @param zbbuf - buffer
   @param size - requested size

   @return pointer to the buffer tail
 */
#define ZB_GET_BUF_TAIL(zbbuf, size) zb_get_buf_tail(zbbuf, size)

/**
   Get buffer tail of size sizeof(type)

   Usually used to place external information (some parameters) to the buffer

   @param zbbuf - buffer
   @param type - data type that will be placed at the buffer end

   @return pointer to the buffer tail casted to (type*)

   @b Example
   @code
     zb_zdo_nwk_addr_req_param_t *req = ZB_GET_BUF_PARAM(buf, zb_zdo_nwk_addr_req_param_t);

     req->dst_addr = 0xffff;
     ZB_IEEE_ADDR_COPY(req->ieee_addr, g_ieee_addr_r4);
     req->start_index  = 0;
     req->request_type = 0x00;
     zb_zdo_nwk_addr_req(ZB_REF_FROM_BUF(buf), NULL);
   @endcode
 */
#define ZB_GET_BUF_PARAM(zbbuf, type) ((type *)ZB_GET_BUF_TAIL((zbbuf), sizeof(type)))

/**
   Place data of size sizeof(type) at the end of buffer

   Macro used to copy external information (some data) at the
   end of buffer

   @param zbbuf - buffer
   @param param - data to be placed in the end of buffer
   @param type - data type that will be placed at the buffer end

   @b Example
   @code
     zb_zdo_nwk_addr_req_param_t req;

     req.dst_addr = 0xffff;
     ZB_IEEE_ADDR_COPY(req.ieee_addr, g_ieee_addr_r4);
     req.start_index  = 0;
     req.request_type = 0x00;
     ZB_SET_BUF_PARAM(buf, req, zb_zdo_nwk_addr_req_param_t);
     zb_zdo_nwk_addr_req(ZB_REF_FROM_BUF(buf), NULL);
   @endcode
 */
 #define ZB_SET_BUF_PARAM(zbbuf, param, type) (ZB_BUF_IS_BUSY_INLINE(ZB_REF_FROM_BUF(zbbuf)) *((type *)ZB_GET_BUF_TAIL(zbbuf, sizeof(type))) = (param) )

/**
   Place data of size sizeof(type) at the end of buffer

   Macro used to copy external information (some data) at the end
   of buffer

   @param zbbuf - buffer
   @param param - data to be placed in the end of buffer
   @param type - data type that will be placed at the buffer end
 */
#define ZB_SET_BUF_PARAM_PTR(zbbuf, param, type) ( ZB_MEMCPY((type *)ZB_GET_BUF_TAIL(zbbuf, sizeof(type)), (param), sizeof(type)) )

/**
 * @copydoc ZB_BUF_REUSE()
 *
 * @note Don't call directly, use ZB_BUF_REUSE() instead
 */
void zb_buf_reuse_leg(
#ifdef ZB_DEBUG_BUFFERS
    zb_uint16_t from_file, zb_uint16_t from_line,
#endif
    zb_buf_t *zbbuf);

/**
 * @brief Reuse previously used buffer
 *
 * @param zbbuf - buffer
 */
#ifdef ZB_DEBUG_BUFFERS
#define ZB_BUF_REUSE(zbbuf) zb_buf_reuse_leg(ZB_TRACE_FILE_ID, __LINE__, zbbuf)
#else
#define ZB_BUF_REUSE(zbbuf) zb_buf_reuse_leg(zbbuf)
#endif


/**
 * @copydoc ZB_FREE_BUF()
 *
 * @note Don't call it directly, use @ref ZB_FREE_BUF() instead
 */
void zb_free_buf(TRACE_PROTO zb_buf_t *buf);

/**
 * @brief Free packet buffer and put it into freelist.
 *
 * Can be called from the main loop.
 *
 * @param buf - packet buffer.
 *
 */
#define ZB_FREE_BUF(buf) zb_free_buf_leg(buf)

#define ZB_FREE_BUF_BY_REF(param)         \
do {                                      \
  if(param)                               \
  {                                       \
    zb_free_buf(ZB_BUF_FROM_REF(param));  \
  }                                       \
} while(0)

/**
 * @copydoc ZB_GET_IN_BUF_DELAYED()
 *
 * @note Don't call it directly, use @ref ZB_GET_IN_BUF_DELAYED() instead
 */
zb_ret_t zb_get_in_buf_delayed(
#ifdef ZB_DEBUG_BUFFERS
  zb_uint16_t from_file, zb_uint16_t from_line,
#endif
    zb_callback_t callback);

/**
 * @brief Allocate IN buffer, call a callback when the buffer is available.
 *
 * If buffer is available, schedules callback for execution immediately. If no buffers are available now,
 * schedule callback later, when buffer will be available.
 *
 * @param callback to call.
 * @return RET_OK or error code.
 *
 * @code
 *     ZB_GET_IN_BUF_DELAYED(new_buffer_allocated);
 * @endcode
 */
#ifdef ZB_DEBUG_BUFFERS
#define ZB_GET_IN_BUF_DELAYED(callback) zb_get_in_buf_delayed(ZB_TRACE_FILE_ID, __LINE__, callback)
#else
#define ZB_GET_IN_BUF_DELAYED(callback) zb_get_in_buf_delayed(callback)
#endif

/**
 * @copydoc ZB_GET_OUT_BUF_DELAYED()
 *
 * @note Don't call it directly, use @ref ZB_GET_OUT_BUF_DELAYED() instead
 */
zb_ret_t zb_get_out_buf_delayed(
#ifdef ZB_DEBUG_BUFFERS
  zb_uint16_t from_file, zb_uint16_t from_line,
#endif
    zb_callback_t callback);

/**
 * @brief Allocate OUT buffer, call a callback when the buffer is available.
 *
 * If buffer is available, schedules callback for execution immediately. If no buffers are available now,
 * schedule callback later, when buffer will be available.
 *
 * @param callback to call.
 * @return RET_OK or error code.
 *
 * @code
 *     ZB_GET_OUT_BUF_DELAYED(new_buffer_allocated);
 * @endcode
 */
#ifdef ZB_DEBUG_BUFFERS
#define ZB_GET_OUT_BUF_DELAYED(callback) zb_get_out_buf_delayed(ZB_TRACE_FILE_ID, __LINE__, callback)
#else
#define ZB_GET_OUT_BUF_DELAYED(callback) zb_get_out_buf_delayed(callback)
#endif

/**
 * @brief Allocate buffer of given type and  call a callback with extra user parameter
 * when buffer of this type is available.
 *
 * If buffer is available, schedules callback for execution immediately. If no buffers are available now,
 * schedule callback later, when buffer will be available.
 *
 * @note Don't call it directly, use @ref ZB_GET_OUT_BUF_DELAYED2() (for out buffers) instead
 *
 * @param callback - callback to call
 * @param buf_type - buffer type (either input or output)
 * @param use_2_param - whether additional param is used
 * @param user_param - will be passed to callback.
 *
 * @return RET_OK or error code.
 */
zb_ret_t zb_get_buf_delayed_2param(
#ifdef ZB_DEBUG_BUFFERS
  zb_uint16_t from_file, zb_uint16_t from_line,
#endif
  zb_callback2_t callback,
  zb_uint8_t buf_type,
  zb_bool_t use_2_param,
  zb_uint16_t user_param);

/**
 * @brief Allocate out buffer, call a callback with extra user parameter when the buffer is available.
 *
 * If buffer is available, schedules callback for execution immediately. If no buffers are available now,
 * schedule callback later, when buffer will be available.
 *
 * @param callback - callback to call
 * @param user_param - will be passed to callback.
 *
 * @return RET_OK or error code.
 *
 *  @code
 *      ZB_GET_OUT_BUF_DELAYED2(new_buffer_allocated, user_data);
 *  @endcode
 */
#ifdef ZB_DEBUG_BUFFERS
#define ZB_GET_OUT_BUF_DELAYED2(callback, user_param) zb_get_buf_delayed_2param(ZB_TRACE_FILE_ID, __LINE__, callback, ZB_OUT_BUFFER, ZB_TRUE, user_param)
#else
#define ZB_GET_OUT_BUF_DELAYED2(callback, user_param) zb_get_buf_delayed_2param(callback, ZB_OUT_BUFFER, ZB_TRUE, user_param)
#endif
/** @cond internals_doc */


/**
   Do nothing in ZB_SWITCH_BUF
 */
#define ZB_SWITCH_BUF(buf, to_in)


/*! @} */

#endif /* #ifdef ZB_LEGACY_BUFS */

#endif /* ZBOSS_API_BUF_LEGACY_H */
