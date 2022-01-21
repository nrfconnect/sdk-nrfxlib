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
/*  PURPOSE: NCP Dev API declarations
*/
#ifndef NCP_DEV_API_H
#define NCP_DEV_API_H 1

#include "zb_types.h"
#include "ncp_common_api.h"

/** @cond DOXYGEN_NCP_SECTION */

/** @addtogroup ncp_dev_api
  *  @{
  */

/** Return code which is raised if @ref zb_ncp_custom_response will be called later*/
#define NCP_RET_LATER (255U)


/** The custom request callback.
 *
 * @param param - the buffer with request data from host and tsn (@ref zb_uint8_t) as a parameter.
 *                It's freed by the stack afterwards.
 *
 * @return The length of a response returned by @ref zb_ncp_custom_response if it is called within callback.
 *         @ref NCP_RET_LATER if @ref zb_ncp_custom_response is called later.
 *
 * @note If returned length is equal to 0, the response is generated and sent automatically.
 */
typedef zb_uint16_t (*zb_ncp_custom_request_cb_t)(zb_uint8_t param);

/** Registers a callback, that is called once the zb_ncp_custom_request is called on the host side
 *  and the corresponding NCP command is received.
 *
 * @param cb - the callback. If NULL is passed, previously registered callback (if any) will be unregistered.
 */
void zb_ncp_custom_register_request_cb(zb_ncp_custom_request_cb_t cb);

/** Sends the indication to the Host.
 *
 * @param param - the ZBOSS buffer with payload. It's freed by the stack.
 */
void zb_ncp_custom_indication(zb_uint8_t param);

/** Sends a custom response.
 *
 * @param param - the ZBOSS buffer with response payload and @ref ncp_hl_custom_resp_t as a parameter.
 *                It's freed by the stack.
 *
 * @return Response length
 *
 * @note Should be called within @ref zb_ncp_custom_request_cb_t unless @ref NCP_RET_LATER is not returned from callback.
 */
zb_uint16_t zb_ncp_custom_response(zb_uint8_t param);

/** @} */ /* ncp_dev_api */

/** @endcond */ /* DOXYGEN_NCP_SECTION */

#endif /* NCP_DEV_API_H */
