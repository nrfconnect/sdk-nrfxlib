/*
 * Copyright (c) 2020, Nordic Semiconductor ASA
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "nrf_802154_spinel.h"

#include <stdint.h>
#include <stddef.h>

#include "../spinel_base/spinel.h"
#include "nrf_802154_spinel_dec.h"
#include "nrf_802154_spinel_backend.h"
#include "nrf_802154_spinel_log.h"
#include "nrf_802154_spinel_response_notifier.h"
#include "nrf_802154_serialization_error.h"
#include "nrf_802154_serialization_error_helper.h"
#include "nrf_802154_buffer_mgr_dst.h"
#include "nrf_802154_buffer_mgr_src.h"
#include "nrf_802154_serialization_config.h"

#if CONFIG_NRF_802154_SER_HOST
NRF_802154_BUFFER_MGR_SRC_INST_DECL_STATIC(m_src_mgr, NRF_802154_TX_BUFFERS);
NRF_802154_BUFFER_MGR_DST_INST_DECL_STATIC(m_dst_mgr, NRF_802154_RX_BUFFERS);
#else // CONFIG_NRF_802154_SER_HOST
NRF_802154_BUFFER_MGR_DST_INST_DECL_STATIC(m_dst_mgr, NRF_802154_TX_BUFFERS);
NRF_802154_BUFFER_MGR_SRC_INST_DECL_STATIC(m_src_mgr, NRF_802154_RX_BUFFERS);
#endif  // CONFIG_NRF_802154_SER_HOST

static void buffer_mgr_init(void)
{
    NRF_802154_BUFFER_MGR_SRC_INIT(m_src_mgr);
    NRF_802154_BUFFER_MGR_DST_INIT(m_dst_mgr);
}

nrf_802154_buffer_mgr_dst_t * nrf_802154_spinel_dst_buffer_mgr_get(void)
{
    return &m_dst_mgr;
}

nrf_802154_buffer_mgr_src_t * nrf_802154_spinel_src_buffer_mgr_get(void)
{
    return &m_src_mgr;
}

void nrf_802154_serialization_init(void)
{
    SERIALIZATION_ERROR_INIT(error);

    buffer_mgr_init();
    nrf_802154_spinel_response_notifier_init();

    nrf_802154_ser_err_t ret = nrf_802154_backend_init();

    SERIALIZATION_ERROR_CHECK(ret, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

nrf_802154_ser_err_t nrf_802154_spinel_send(const char * p_fmt, ...)
{
    uint8_t        command_buff[NRF_802154_SPINEL_FRAME_BUFFER_SIZE];
    spinel_ssize_t siz;

    va_list args;

    va_start(args, p_fmt);

    siz = spinel_datatype_vpack(command_buff, sizeof(command_buff), p_fmt, args);

    va_end(args);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_ENCODING_FAILURE;
    }

    NRF_802154_SPINEL_LOG_RAW("Sending spinel frame\n");
    NRF_802154_SPINEL_LOG_BUFF_NAMED(command_buff, siz, "data");

    return nrf_802154_spinel_encoded_packet_send(command_buff, (size_t)siz);
}

void nrf_802154_spinel_encoded_packet_received(const void * p_data, size_t data_len)
{
    NRF_802154_SPINEL_LOG_RAW("Received spinel frame\n");
    NRF_802154_SPINEL_LOG_BUFF_NAMED(p_data, data_len, "data");

    SERIALIZATION_ERROR_INIT(error);

    nrf_802154_ser_err_t err = nrf_802154_spinel_decode_cmd(p_data, data_len);

    SERIALIZATION_ERROR_CHECK(err, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}
