/*
 * Copyright (c) 2020 - 2021, Nordic Semiconductor ASA
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

#include <assert.h>
#include <stdint.h>
#include <stddef.h>

#include "../spinel_base/spinel.h"
#include "nrf_802154_spinel_dec.h"
#include "nrf_802154_serialization_error.h"

nrf_802154_ser_err_t nrf_802154_spinel_decode_cmd(const void * p_packet_data,
                                                  size_t       packet_data_len)
{
    uint8_t          header;
    spinel_command_t cmd;
    const void     * p_cmd_data;
    size_t           cmd_data_len;

    spinel_ssize_t siz = spinel_datatype_unpack(p_packet_data,
                                                packet_data_len,
                                                SPINEL_DATATYPE_COMMAND_S SPINEL_DATATYPE_DATA_S,
                                                &header,
                                                &cmd,
                                                &p_cmd_data,
                                                &cmd_data_len);

    if (siz < 0)
    {
        assert(false);
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    return nrf_802154_spinel_dispatch_cmd(cmd, p_cmd_data, cmd_data_len);
}
