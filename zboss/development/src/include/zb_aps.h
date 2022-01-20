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
/* PURPOSE: APS layer API
*/

#ifndef ZB_APS_H
#define ZB_APS_H 1

#include "zb_config.h"

/*! \addtogroup aps_ib */
/*! @{ */

/* APS Information Base default values. */

/** @brief Default value for AIB non-member radius attribute */
#define ZB_APS_AIB_DEFAULT_NONMEMBER_RADIUS        0x02U

/** @brief Default value for AIB maximum non-member radius attribute.
  *
  * Possible values from 0x00 to 0x06.
  */
#define ZB_APS_AIB_DEFAULT_MAX_NONMEMBER_RADIUS    0x06U

/** @brief Value of non-member radius that means infinity non-member transmission. */
#define ZB_APS_AIB_INFINITY_NONMEMBER_RADIUS       0x07U

/** @cond internals_doc */

/** @brief APSME GET common message header. */
typedef ZB_PACKED_PRE struct zb_aps_command_header_s
{
  zb_uint8_t fc;                                    /*!< Frame control */
  zb_uint8_t aps_counter;                           /*!< APS counter  */
  zb_uint8_t aps_command_id;                        /*!< APS command */
} ZB_PACKED_STRUCT zb_aps_command_header_t;

/** @brief APSME GET common packet header. */
typedef ZB_PACKED_PRE struct zb_aps_command_pkt_header_s
{
  zb_uint8_t fc;                                    /*!< Frame control */
  zb_uint8_t aps_counter;                           /*!< APS counter  */
} ZB_PACKED_STRUCT zb_aps_command_pkt_header_t;

/** @endcond */ /* internals_doc */

/*! @} */

/*! \addtogroup aps_api */
/*! @{ */

/** @name APS data service structures and API.
  * @{
  */

/** @brief APSDE data request structure.
  *
  * This data structure passed to @ref zb_apsde_data_request() in the packet buffer (in its tail).
  */
typedef struct zb_apsde_data_req_s
{
  zb_addr_u dst_addr;  /*!< Destination address */
  zb_uint16_t  profileid;    /*!< The identifier of the profile for which this
                                  frame is intended.  */
  zb_uint16_t  clusterid;    /*!< The identifier of the object for which this
                                  frame is intended.  */
  zb_uint8_t   dst_endpoint; /*!< either the number of the individual endpoint
                                  of the entity to which the ASDU is being
                                  transferred or the broadcast endpoint (0xff).  */
  zb_uint8_t   src_endpoint; /*!< The individual endpoint of the entity from
                                  which the ASDU is being transferred. */
  zb_uint8_t   radius;       /*!< The distance, in hops, that a frame will be
                                  allowed to travel through the network. */
  zb_uint8_t   addr_mode;    /*!< The type of destination address supplied by
                                  the DstAddr parameter - see @ref aps_addr_mode  */
  zb_uint8_t   tx_options;   /*!< The transmission options for the ASDU to be
                                  transferred. These are a bitwise OR of one or
                                  more of the following:
                                  0x01 = Security enabled transmission
                                  0x02 = Use NWK key
                                  0x04 = Acknowledged transmission
                                  0x08 = Fragmentation permitted.
                                  See @ref apsde_tx_opt          */
  /*2.2.4.1.1.1 spec r21*/
  zb_uint8_t   use_alias;    /*!< The next higher layer may use the UseAlias parameter
			          to request alias usage by NWK layer for the current frame.
				  If the UseAlias parameter has a value of FALSE,
				  meaning no alias usage, then the parameters
				  AliasSrcAddr and AliasSeqNumb will be ignored.
				  Otherwise, a value of TRUE denotes that the values supplied in
				  AliasSrcAddr and AliasSeqNumb are to be used
				  Values: TRUE or FALSE */
  zb_uint16_t   alias_src_addr;/*!< The source address to be used for this NSDU.
				    If the UseAlias parameter has a value of FALSE,
				    the AliasSrcAddr parameter is ignored.
				    Values: Any valid device address except broadcast address */
  zb_uint8_t    alias_seq_num; /*!< The sequence number to be used for this NSDU.
				    If the UseAlias parameter has a value of FALSE,
				    the AliasSeqNumb parameter is ignored.
				    Values: 0x00-0xff */
#ifdef APS_FRAGMENTATION
#define NCP_APSDE_PARAM_SIZE ZB_OFFSETOF(zb_apsde_data_req_t, extended_fc)
  zb_uint8_t extended_fc;  /* Extended Frame Control - internal use only */
  zb_uint8_t block_num;    /* Fragmentation: for the first segment - total payload len */
#else
#define NCP_APSDE_PARAM_SIZE sizeof(zb_apsde_data_req_t)
#endif
} zb_apsde_data_req_t;

/** @brief Parameters of the APSDE-DATA.confirm primitive. */
typedef ZB_PACKED_PRE struct zb_apsde_data_confirm_s
{
  zb_addr_u      dst_addr;        /*!< The destination address for the binding entry. */
  zb_uint8_t     dst_endpoint;    /*!< The destination endpoint for the binding entry. */
  zb_uint8_t     src_endpoint;    /*!< The source endpoint for the binding entry. */
  /* FIXME: Do we use this field?? */
  zb_time_t      tx_time;         /*!< A time indication for the transmitted packet based on the
                                       local clock, as provided by the NWK layer. */
  zb_uint8_t     addr_mode;       /*!< The type of destination address supplied by
                                       the DstAddr parameter - see @ref aps_addr_mode  */
#define FIRST_INTERNAL_APSDE_CONF_FIELD status
  zb_ret_t       status;          /*!< The status of corresponding request. */
  zb_bool_t      need_unlock;
} ZB_PACKED_STRUCT zb_apsde_data_confirm_t;

/**
 * @name Transmission options
 * @brief The transmission options for the ASDU to be transferred.
 *
 * Note: These values were members of `enum zb_apsde_tx_opt_e` type but were converted to a
 * set of macros due to MISRA violations.
 *
 * These are a bitwise OR of one or more.
 * @anchor apsde_tx_opt
 */
/** @{ */
#define ZB_APSDE_TX_OPT_SECURITY_ENABLED         0x01U /*!< Security enabled transmission */
#define ZB_APSDE_TX_OPT_USE_NWK_KEY_R21OBSOLETE  0x02U /*!< Use NWK key */
#define ZB_APSDE_TX_OPT_ACK_TX                   0x04U /*!< Acknowledged transmission */
#define ZB_APSDE_TX_OPT_FRAG_PERMITTED           0x08U /*!< Fragmentation permitted */
#define ZB_APSDE_TX_OPT_INC_EXT_NONCE            0x10U /*!< Include extended nonce in APS security frame */

#ifdef ZB_APSDE_REQ_ROUTING_FEATURES
#define ZB_APSDE_TX_OPT_FORCE_MESH_ROUTE_DISC    0x20U /* Non-spec extension: Force mesh route discovery */
#define ZB_APSDE_TX_OPT_FORCE_SEND_ROUTE_REC     0x40U /* Non-spec extension: Force send route record */
#endif /* ZB_APSDE_REQ_ROUTING_FEATURES */

/** @} */

/** @brief NLDE-DATA.request primitive

   This function can be called via scheduler, returns immediately.
   Later zb_nlde_data_confirm will be called to pass NLDE-DATA.request result up.

   @param param - packet to send (\ref zb_buf_t) and parameters at buffer tail
          \ref zb_nlde_data_req_t

   @b Example:
@code
  {
    zb_apsde_data_req_t *req;
    zb_ushort_t i;

    buf = ZB_BUF_FROM_REF(param);
    ZB_BUF_INITIAL_ALLOC(buf, 10, ptr);
    for (i = 0 ; i < 10 ; ++i)
    {
      ptr[i] = i % 32 + '0';
    }

    req = ZB_GET_BUF_TAIL(buf, sizeof(zb_apsde_data_req_t));
    req->dst_addr.addr_short = 0; /-- ZC --/
    req->addr_mode = ZB_APS_ADDR_MODE_16_ENDP_PRESENT;
    req->tx_options = ZB_APSDE_TX_OPT_ACK_TX;
    req->radius = 5;
    req->profileid = 2;
    req->src_endpoint = 10;
    req->dst_endpoint = 10;
    buf->u.hdr.handle = 0x11;
    TRACE_MSG(TRACE_APS3, "Sending apsde_data.request", (FMT__0));
    ZB_SCHEDULE_CALLBACK(zb_apsde_data_request, ZB_REF_FROM_BUF(buf));
  }
@endcode

 */
void zb_apsde_data_request(zb_uint8_t param);

/** @brief APSDE-DATA.confirm primitive.
  *
  * The primitive reports the results of a request to transfer a data PDU (ASDU) from NHLE.
  *
  * @note zb_apsde_data_confirm must be defined in the APS layer! APS layer just calls this
  * function.
  * @param param - sent packet - see @ref zb_buf_t. APS must free or reuse it. Following packet
  * fields are used:
  * @li status - The status of the corresponding request. Can take value of: INVALID_REQUEST,
  * MAX_FRM_COUNTER, NO_KEY, BAD_CCM_OUTPUT, ROUTE_ERROR, BT_TABLE_FULL, FRAME_NOT_BUFFERED or any
  * status values returned from security suite or the MCPS-DATA.confirm primitive.
  * @li handle - The handle associated with the NSDU being confirmed.
  */
void zb_apsde_data_confirm(zb_uint8_t param);

/** @brief NLDE-DATA.indication primitive.
  *
  * This function called via scheduler by the NWK layer to pass incoming data packet to the APS
  * layer.
  * @note zb_nlde_data_indication() must be defined in the APS layer! NWK layer just
  * calls this function.
  * @param param - The set of octets comprising the NSDU to be transferred (with length).
  *
  * Other fields got from MAC nsdu by macros.
  */
void zb_apsde_data_indication(zb_uint8_t param);

/** @} */ /* APS data service structures and API. */

/** @cond internals_doc */

/** @brief Remove APS header from the packet.

   @param packet - APS packet
   @param ptr - (out) pointer to the APS data begin

   @par Example
   @snippet tp_aps_bv_19_i_zr1.c data_indication
   @par

 */
#define ZB_APS_HDR_CUT_P(packet, ptr)                   \
  ptr = zb_buf_cut_left(packet, zb_aps_full_hdr_size(zb_buf_begin(packet)))


/** @brief Remove APS header from the packet.

   @param packet - APS packet

   @b Example:
@code
void data_indication(zb_uint8_t param)
{
  zb_ushort_t i;
  zb_bufid_t asdu = (zb_bufid_t )ZB_BUF_FROM_REF(param);

  ZB_APS_HDR_CUT(asdu);

  TRACE_MSG(TRACE_APS3, "data_indication: packet %p len %d handle 0x%x", (FMT__P_D_D,
                         asdu, (int)zb_buf_len(asdu), asdu->u.hdr.status));

  zb_free_buf(asdu);
}
@endcode
 */
#define ZB_APS_HDR_CUT(packet)                                 \
  (void)zb_buf_cut_left(packet, zb_aps_full_hdr_size(zb_buf_begin(packet)))

/** @endcond */ /* internals_doc */

/** @cond internals_doc */

/** @brief Structure for Get Group Membership Command.
  * @see ZCL8 spec, subclause 3.6.2.3.4.
  */
typedef ZB_PACKED_PRE struct zb_apsme_get_group_membership_req_s
{
  zb_callback_t  confirm_cb;    /*!< The callback to be called when the operation is completed. */
  zb_ushort_t    n_groups;      /*!< Group addresses amount. */
  zb_uint16_t    groups[1];     /*!< First element if list with group addresses. */
} ZB_PACKED_STRUCT zb_apsme_get_group_membership_req_t;

/*
 * 'groups' array member is passed as a function parameter (e.g. zb_aps_group_table_get_membership()), it should be
 * properly aligned.
 */
ZB_ASSERT_IF_NOT_ALIGNED(ZB_OFFSETOF(zb_apsme_get_group_membership_req_t, groups), sizeof(zb_uint16_t));

/** @brief Structure for Get Group Membership Response Command
  * @see ZCL spec, subclause 3.6.2.3.3.
  */
typedef struct zb_apsme_get_group_membership_conf_s
{
  zb_uint16_t groups[ZB_APS_GROUP_TABLE_SIZE];  /*!< List with group addresses. */
  zb_ushort_t n_groups;                         /*!< Group addresses amount. */
  zb_ushort_t capacity;                         /*!< Group table capacity */
} zb_apsme_get_group_membership_conf_t;

/** @endcond */ /* internals_doc */

/*! @} */


/*! @cond internals_doc */
/*! \addtogroup ZB_APS */
/*! @{ */

/** @brief Signals user that data is sent and acknowledged.
  * @param param - reference to the buffer containing ASDU sent.
  */
void zb_apsde_data_acknowledged(zb_uint8_t param);

/** @brief Initialize APS subsystem. */
void zb_aps_init(void);



/** @brief APS command codes constants.
  * @note APS_CMD_TUNNEL should be last one, or you should change logic.
  */

/**
 * @name APS command codes constants.
 * @note APS_CMD_TUNNEL should be last one, or you should change logic.
 * @anchor aps_commands
 *
 * Note: These values were members of `enum zb_aps_commands_e` type but were converted to a
 * set of macros due to MISRA violations.
 */
/** @{ */
#define APS_CMD_SKKE_1           0x01U
#define APS_CMD_SKKE_2           0x02U
#define APS_CMD_SKKE_3           0x03U
#define APS_CMD_SKKE_4           0x04U
#define APS_CMD_TRANSPORT_KEY    0x05U
#define APS_CMD_UPDATE_DEVICE    0x06U
#define APS_CMD_REMOVE_DEVICE    0x07U
#define APS_CMD_REQUEST_KEY      0x08U
#define APS_CMD_SWITCH_KEY       0x09U
#define APS_CMD_EA_INIT_CHLNG    0x0AU
#define APS_CMD_EA_RSP_CHLNG     0x0BU
#define APS_CMD_EA_INIT_MAC_DATA 0x0CU
#define APS_CMD_EA_RSP_MAC_DATA  0x0DU
#define APS_CMD_TUNNEL           0x0EU
#define APS_CMD_VERIFY_KEY       0x0FU
#define APS_CMD_CONFIRM_KEY      0x10U
/** @} */

zb_uint8_t aps_find_src_ref(
  zb_address_ieee_ref_t src_addr_ref, zb_uint8_t src_end, zb_uint16_t cluster_id);

#ifdef ZB_COORDINATOR_ROLE
/** @brief APSME-UPDATE-DEVICE.indication primitive.
  * @param param - reference to the buffer containing indication data.
  */
void zb_apsme_update_device_indication(zb_uint8_t param);
#endif /* ZB_COORDINATOR_ROLE */

/** @internal @brief Send APS command.
  *
  * Internal routine to be used to send APS command frame.
  * @see ZB spec, subclause 2.2.5.2.2 - APS Command Frame Format.
  * @param param - buffer with command body.
  * @param dest_addr - address of device to send to.
  * @param command - command id.
  * @param secure  - if true, secure transfer at NWK level.
  */
void zb_aps_send_command(zb_uint8_t param, zb_uint16_t dest_addr, zb_uint8_t command, zb_bool_t secure
    /** @param secure_aps_i - if true, secure APS level.*/
    , zb_uint8_t secure_aps_i
  );

/** \par Macros for APS FC parse-compose */

#define ZB_APS_FC_GET_FRAME_TYPE(fc)  ((fc) & 3U)
#define ZB_APS_FC_SET_FRAME_TYPE(fc, t)  ((fc) |= (t))

#define ZB_APS_FC_GET_DELIVERY_MODE(fc) (((fc)>>2U) & 3U)
#define ZB_APS_FC_SET_DELIVERY_MODE(fc, m) ((fc) |= (m) << 2U)

#define ZB_APS_FC_GET_ACK_FORMAT(fc) (((fc)>>4U) & 1U)
#define ZB_APS_FC_SET_ACK_FORMAT(fc, f) ((fc) |= ((f) << 4U))

#define ZB_APS_FC_GET_SECURITY(fc) (((fc)>>5U) & 1U)
#define ZB_APS_FC_IS_SECURE(fc) (ZB_APS_FC_GET_SECURITY(fc) != 0U)
#define ZB_APS_FC_SET_SECURITY(fc, f) ((fc) |= ((f) << 5U))

#define ZB_APS_FC_GET_ACK_REQUEST(fc) (((fc)>>6U) & 1U)
#define ZB_APS_FC_SET_ACK_REQUEST(fc, f) ((fc) |= ((f) << 6U))

#define ZB_APS_FC_GET_EXT_HDR_PRESENT(fc) (((fc)>>7U) & 1U)
#define ZB_APS_FC_SET_EXT_HDR_PRESENT(fc, f) ((fc) |= ((f) << 7U))

#define ZB_APS_FIRST_FRAGMENT 0x01U
#define ZB_APS_NOT_FIRST_FRAGMENT 0x02U
/* Limit to 4 fragments as maximum available.
   If need to use more, extend bitfield in zb_aps_dup_tbl_ent_t. */
#define ZB_APS_MAX_FRAGMENT_BLOCK_NUM ZB_APS_MAX_FRAGMENT_NUM - 1
#define ZB_APS_FC_GET_EXT_HDR_FRAG(fc) ((fc) & 3U)
#define ZB_APS_FC_SET_EXT_HDR_FRAG(fc, f) ((fc) |= (f))

/** @brief Setup FC for APS command: frame type, ack request, ack format. */
#define ZB_APS_FC_SET_COMMAND(fc, need_ack)      \
  (fc) |= (   /*frame type*/ZB_APS_FRAME_COMMAND \
           |  /*ack req*/(((need_ack)) << 6U)     \
           | /*ack format*/(1U << 4U))

/**
 * @name APS frame type
 * @anchor aps_frame_type
 *
 * Note: These values were members of `enum zb_aps_frame_type_e` type but were converted to a
 * set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_APS_FRAME_DATA    0U
#define ZB_APS_FRAME_COMMAND 1U
#define ZB_APS_FRAME_ACK     2U
/** @} */

/**
 * @name APS frame delivery mode values
 * @anchor aps_frame_delivery_mode
 *
 * Note: These values were members of `enum zb_aps_frame_delivery_mode_e` type but were converted to
 * a set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_APS_DELIVERY_UNICAST   0U /*!< Unicast frame delivery. */
/*! Reserved value, see Zigbee spec, subclause 2.2.5.1.1.2 */
#define ZB_APS_DELIVERY_RESERVED  1U
#define ZB_APS_DELIVERY_BROADCAST 2U /*!< Broadcast frame delivery. */
#define ZB_APS_DELIVERY_GROUP     3U /*!< Group frame delivery. */
/** @} */


/** @brief Ger APS header size (not include AUX security hdr). */
zb_uint8_t zb_aps_hdr_size(zb_uint8_t fc);
#define ZB_APS_HDR_SIZE(fc)                                             \
  zb_aps_hdr_size(fc)
/* #define ZB_APS_HDR_SIZE(fc)                                             \ */
/*   (2 +                          /\* fc + aps counter *\/                  \ */
/*    ((ZB_APS_FC_GET_FRAME_TYPE(fc) == ZB_APS_FRAME_COMMAND) ? 0 :  (     \ */
/*      /\* Packet either has dest endpoint (1) or group address (2) *\/     \ */
/*      1 +                                                                \ */
/*      (ZB_APS_FC_GET_DELIVERY_MODE(fc) == ZB_APS_DELIVERY_GROUP) +       \ */
/*      /\* cluster id, profile id *\/                                       \ */
/*      2 + 2 +                                                            \ */
/*      /\* src endpoint *\/                                                 \ */
/*      1 +                                                                \ */
/*      (ZB_APS_FC_GET_EXT_HDR_PRESENT(fc) ?                               \ */
/*       /\* extended hdr + block number (+ block ack) *\/                   \ */
/*       ((ZB_APS_FC_GET_FRAME_TYPE(fc) == ZB_APS_FRAME_ACK) ? 3 : 2)      \ */
/*       : 0)                                                              \ */
/*      ))) */


/** @brief Return APS header + APS aux hdr size.
  * @param pkt - packet for calculate size.
  * @return packet APS full head size.
  */
zb_ushort_t zb_aps_full_hdr_size(const zb_uint8_t *pkt);


/* #define APS_CONFIRM_STATUS(buf, s, func)                \ */
/* do                                                      \ */
/* {                                                       \ */
/*   (buf)->u.hdr.status = (s);                            \ */
/*   ZB_SCHEDULE_CALLBACK((func), ZB_REF_FROM_BUF((buf))); \ */
/* }                                                       \ */
/* while(0) */


/** @brief Get Group Membership Command (For Zigbee cluster library).
  *
  * @param param - buffer with parameter. See @ref zb_apsme_get_group_membership_req_s.
  * @see ZCL spec, subclause 3.6.2.2.5.
  */
void zb_apsme_get_group_membership_request(zb_uint8_t param);

#define ZDO_MGMT_APS_LEAVE_RESP_CLID    0x8034U /* it is from zdo for leave callback*/

/** @internal
 * @brief Parse APS header
 *
 * @param packet - buffer with APS command
 * @param aps_hdr - APS header see @ref zb_aps_hdr_s
 * @param cut_nwk_hdr - flag "cut HWK header before parse"
 * */
void zb_aps_hdr_parse(zb_bufid_t packet, zb_aps_hdr_t *aps_hdr, zb_bool_t cut_nwk_hdr);

/** @internal
 * @brief Check element in the group array by GroupID.
 *
 * @par Example
 * @snippet tp_pro_bv-52_zr1.c tp_pro_bv-52_zr1
 * @par
 * */
zb_bool_t zb_aps_is_in_group(zb_uint16_t grp_id);

zb_aps_group_table_ent_t* zb_aps_get_group_table_entry(zb_uint16_t group_addr);

/** @brief Filling Nlde request.
  * @param param - buffer for fill request.
  * @param addr - destination address.
  * @param secure - Is ASP secure.
  */
void fill_nldereq(zb_uint8_t param, zb_uint16_t addr, zb_uint8_t secure);

/** @brief Set predefined TC key. */
void zb_aps_set_preconfigure_security_key(zb_uint8_t *key);

zb_ret_t zb_aps_get_ieee_source_from_cmd_frame(zb_uint8_t cmd_id, zb_uint8_t cmd_buf_param, zb_ieee_addr_t ieee_addr);

void zb_aps_clear_after_leave(zb_uint16_t address);

void zb_apsme_remove_all_groups_internal(void);
void apsme_forget_device(void);
void aps_send_fail_confirm(zb_uint8_t param, zb_ret_t status);

/*! @} */
/*! @endcond */

/** @cond internals_doc */

void zb_apsme_unbind_by_ref(zb_address_ieee_ref_t addr_ref);

zb_ret_t zb_check_binding_table_whitelist(zb_apsme_binding_req_t *apsreq);

/* Functions for accessing and modifying a group table */
zb_ret_t zb_aps_group_table_add(zb_aps_group_table_t *table, zb_uint16_t group, zb_uint8_t ep);
zb_ret_t zb_aps_group_table_remove(zb_aps_group_table_t *table, zb_uint16_t group, zb_uint8_t ep);
void zb_aps_group_table_remove_all(zb_aps_group_table_t *table);
zb_bool_t zb_aps_group_table_is_endpoint_in_group(zb_aps_group_table_t *table,
                                                  zb_uint16_t group_id,
                                                  zb_uint8_t endpoint);
void zb_apsme_internal_get_group_membership_request(zb_aps_group_table_t *table, zb_uint8_t param);


/* Internal function for binding APS-groups to ZCL endpoints,
 * called from zb_apsme_add_group_request or elsewhere from any layer
 */
zb_ret_t zb_apsme_add_group_internal(zb_uint16_t group, zb_uint8_t ep);

/* Internal function for unbinding APS-groups to ZCL endpoints,
 * called from zb_apsme_remove_group_request or elsewhere from any layer
 */
zb_ret_t zb_apsme_remove_group_internal(zb_uint16_t group, zb_uint8_t ep);

/** @endcond */ /* internals_doc */

/** @brief APSME GET request primitive.
  * @param param reference to the buffer containing request data.
  *
  * @par Example
  * @snippet start_ze.c zb_apsme_get_request
  * @par
  */
void zb_apsme_get_request(zb_uint8_t param);

/** @brief APSME SET request primitive.
  * @param param reference to the buffer containing request data.
  *
  * @par Example
  * @snippet start_ze.c zb_apsme_set_request
  * @par
  *
  */
void zb_apsme_set_request(zb_uint8_t param);

/*!
 * @brief Add entry to binding whitelist table
 *
 * @param ep_id - endpoint ID
 * @param cluster_id - endpoint ID
 * @param cluster_role - endpoint ID
 * @param ieee_addr - IEEE addr
 */
zb_ret_t zb_aps_add_binding_whitelist_entry(zb_uint8_t ep_id, zb_uint16_t cluster_id, zb_uint8_t cluster_role, zb_ieee_addr_t ieee_addr);

#endif /* ZB_APS_H */
