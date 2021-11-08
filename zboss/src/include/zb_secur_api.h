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
/* PURPOSE: Security - API to be used from the applications level
*/

#ifndef ZB_SECUR_API_H
#define ZB_SECUR_API_H 1
/** @cond (!DOXYGEN_ERL_SECTION) */
/*! \addtogroup secur_api */
/*! @{ */

#ifdef ZB_COORDINATOR_ROLE
/**
   Send new network key to all devices in the net via broadcast

   4.6.3.4  Network Key Update
   4.6.3.4.1  Trust Center Operation

   @param param - buffer with single parameter - short broadcast address. Valid
          values are 0xffff, 0xfffd

   @snippet tp_pro_bv-64_zc.c zb_secur_send_nwk_key_update_br
 */
void zb_secur_send_nwk_key_update_br(zb_uint8_t param);



/**
   Generate switch key.

   According to test 14.24TP/SEC/BV-01-I Security NWK Key Switch (No Pre-
   configured Key)-ZR, this command can be send either to broadcast or unicast
   to all rx-on-when-idle from the neighbor.

   When send unicast, it encrypted by the new (!) key, when send proadcast - by
   the old key.

   That mean, switch our key _after_ this frame transfer and secure - in the
   command send confirm.

   @param param - packet buffer with single parameter - broadcast address. If 0,
          send unicast.
 */
void zb_secur_send_nwk_key_switch(zb_uint8_t param);
#endif /* ZB_COORDINATOR_ROLE */

/*! @} */
/** @endcond */ /* (!DOXYGEN_ERL_SECTION) */
#endif /* ZB_SECUR_API_H */
