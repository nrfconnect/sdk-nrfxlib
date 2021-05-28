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
/* PURPOSE: HW crypto API
*/

#ifndef HW_CRYPTO_API_H
#define HW_CRYPTO_API_H 1

#ifndef ZB_SOFT_SECURITY

extern zb_ret_t
hw_encrypt_n_auth(
  zb_uint8_t *key,
  zb_uint8_t *nonce,
  zb_uint8_t *string_a,
  zb_ushort_t string_a_len,
  zb_uint8_t *string_m,
  zb_ushort_t string_m_len,
  zb_bufid_t crypted_text);

extern zb_ret_t
hw_decrypt_n_auth(
  zb_uint8_t *key,
  zb_uint8_t *nonce,
  zb_bufid_t  buf,
  zb_ushort_t string_a_len,
  zb_ushort_t string_c_len);


#define zb_ccm_encrypt_n_auth hw_encrypt_n_auth

#define zb_ccm_decrypt_n_auth hw_decrypt_n_auth

#define zb_ccm_decrypt_n_auth_stdsecur zb_ccm_decrypt_n_auth

#endif  /* ZB_SOFT_SECURITY */

#ifdef ZB_HW_ZB_AES128
void hw_aes128(zb_uint8_t *key, zb_uint8_t *msg, zb_uint8_t *c);

#define zb_aes128 hw_aes128
#endif

#ifdef ZB_HW_ZB_AES128_DEC
void hw_aes128_dec(zb_uint8_t *key, zb_uint8_t *msg, zb_uint8_t *c);

#define zb_aes128_dec hw_aes128_dec
#endif

#endif  /* HW_CRYPTO_API_H */
