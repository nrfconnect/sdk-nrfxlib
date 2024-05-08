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
/*  PURPOSE: Header file for NWK MultiMAC
*/
#ifndef ZB_NWK_MM_H
#define ZB_NWK_MM_H 1

#define ZB_NWK_MAC_IFACE_SCAN_TYPE_ACTIVE     0U
#define ZB_NWK_MAC_IFACE_SCAN_TYPE_ENH_ACTIVE 1U

void zb_nwk_mm_mac_iface_table_init(void);

zb_uint8_t zb_nwk_mm_get_freq_band(void);

void zb_nlme_set_interface_request(zb_uint8_t param);

void zb_nlme_get_interface_request(zb_uint8_t param);

zb_uint8_t zb_multimac_get_iface_id_by_channel_page(zb_uint8_t page_idx, zb_uint8_t channel_idx);

/* Multi-MAC proxy functions */
void zb_nwk_handle_mlme_start_confirm(zb_uint8_t param);
void zb_nwk_handle_mlme_reset_confirm(zb_uint8_t param);
void zb_nwk_handle_mcps_data_confirm(zb_uint8_t param);

void zb_multimac_mlme_start_request_proxy(zb_uint8_t param);
void zb_multimac_mlme_start_confirm_proxy(zb_uint8_t param);
void zb_multimac_mcps_data_request_proxy(zb_uint8_t param);
void zb_multimac_mlme_set_request_proxy(zb_uint8_t param);
void zb_multimac_mlme_reset_request_proxy(zb_uint8_t param);
void zb_multimac_mlme_reset_confirm_proxy(zb_uint8_t param);
void zb_multimac_mlme_get_request_proxy(zb_uint8_t param);

void zb_multimac_mlme_scan_request_proxy(zb_uint8_t param);
void zb_multimac_mlme_scan_confirm_proxy(zb_uint8_t param);

void zb_multimac_mcps_data_confirm_proxy(zb_uint8_t param);

#ifdef ZB_MACSPLIT_HOST
void zb_mac_init_macsplit(void);
#endif /* ZB_MACSPLIT_HOST */
void zb_multimac_mac_init(void);

/* ZB_MACSPLIT_DEVICE just uses monolithic MAC now */
#if defined(ZB_MAC_INTERFACE_SINGLE) && defined(ZB_MACSPLIT_HOST)
  #define ZB_MAC_INIT() zb_mac_init_macsplit()
#elif defined(ZB_MACSPLIT_DEVICE)
  ZB_ASSERT_COMPILE_DECL(ZB_NWK_MAC_IFACE_TBL_SIZE == 1U);
  #define ZB_MAC_INIT() zb_mac_init()
#elif !defined(ZB_MAC_INTERFACE_SINGLE)
  #define ZB_MAC_INIT() zb_multimac_mac_init()
#else
  #define ZB_MAC_INIT() zb_mac_init()
#endif

/* MAC split interface API */
#ifdef ZB_MACSPLIT_HOST
void zb_mcps_data_request_macsplit(zb_uint8_t param);
void zb_mlme_get_request_macsplit(zb_uint8_t param);
void zb_mlme_set_request_macsplit(zb_uint8_t param);
void zb_mlme_reset_request_macsplit(zb_uint8_t param);
void zb_mlme_scan_request_macsplit(zb_uint8_t param);
void zb_mlme_associate_request_macsplit(zb_uint8_t param);
void zb_mlme_associate_response_macsplit(zb_uint8_t param);
void zb_mlme_poll_request_macsplit(zb_uint8_t param);
void zb_mlme_orphan_response_macsplit(zb_uint8_t param);
void zb_mlme_start_request_macsplit(zb_uint8_t param);
void zb_mac_resp_by_empty_frame_macsplit(zb_uint8_t param);

zb_ret_t zb_mac_logic_iteration_macsplit();
#endif /* ZB_MACSPLIT_HOST */

/**
 * @brief Synchronizes association parameters (MAC short address, coordinator extended address)
 * from primary interface with other interfaces
 *
 * @param param
 */
void zb_nwk_mm_sync_assoc(zb_uint8_t param);


/**
   Get the primary MAC interface.
*/
zb_uint8_t zb_get_primary_interface(void);

#endif /* ZB_NWK_MM_H */
