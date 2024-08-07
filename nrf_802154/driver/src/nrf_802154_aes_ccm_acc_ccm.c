/*
 * Copyright (c) 2024, Nordic Semiconductor ASA
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

#include "nrf_802154_config.h"

#if NRF_802154_ENCRYPTION_ENABLED && !NRF_802154_ENCRYPTION_ACCELERATOR_ECB

/** Configures if the CCM's OUT.PTR pointer points to the same memory location as PACKETPTR register
 *  of the RADIO.
 *
 *  Note that with this option set, IEEE 802.15.4 Information Elements injection is not supported.
 */
#define CCM_OUTPTR_POINTS_TO_RADIO_PACKETPTR 0

#define CCM_ALEN_ATTR_ID                     11 ///< Attribute field that identifies the alen CCM job
#define CCM_MLEN_ATTR_ID                     12 ///< Attribute field that identifies the mlen CCM job
#define CCM_ADATA_ATTR_ID                    13 ///< Attribute field that identifies the adata CCM job
#define CCM_MDATA_ATTR_ID                    14 ///< Attribute field that identifies the mdata CCM job

#include "nrf_802154_aes_ccm.h"

#include <string.h>

#include "hal/nrf_ccm.h"
#include "hal/nrf_dppi.h"
#include "hal/nrf_ppib.h"
#include "helpers/nrf_vdma.h"

#include "nrf_802154_assert.h"
#include "nrf_802154_const.h"
#include "nrf_802154_tx_work_buffer.h"
#include "nrf_802154_peripherals.h"
#include "nrf_802154_utils_byteorder.h"
#include "platform/nrf_802154_irq.h"

#if defined(NRF54H_SERIES)
#include "hal/nrf_spu.h"
#define PPIB_RAD  NRF_PPIB020
#define PPIB_CCM  NRF_PPIB030
#ifndef NRF_DPPIC030
#define DPPIC_CCM ((NRF_DPPIC_Type *)0x53032000)
#else
#define DPPIC_CCM NRF_DPPIC030
#endif
#elif defined(NRF54L_SERIES)
#define PPIB_RAD  NRF_PPIB10
#define PPIB_CCM  NRF_PPIB00
#define DPPIC_CCM NRF_DPPIC00
#endif

typedef struct
{
    struct
    {
        uint16_t alen;
        uint16_t mlen;
    } in;

    struct
    {
        uint16_t alen;
        uint16_t mlen;
    } out;
} ccm_params_t;

__ALIGN(8) static nrf_vdma_job_t m_in_job_list[5];  ///< CCM DMA input job list array.
__ALIGN(8) static nrf_vdma_job_t m_out_job_list[5]; ///< CCM DMA output job list array.

static bool             m_setup;                    ///< Was encryption setup
static ccm_params_t     m_ccm_params;               ///< CCM peripheral parameters.
static bool             m_initialized;              ///< Module init status.
static nrf_ccm_config_t m_ccm_config;               ///< CCM configuration used during the next transmission.
static uint32_t         m_key[4];                   ///< Key used during the next transmission.
static uint32_t         m_nonce[4];                 ///< Nonce used during the next transmission.

static void ccm_disable(void)
{
    nrf_802154_irq_disable(nrfx_get_irq_number(NRF_802154_CCM_INSTANCE));
    nrf_ccm_subscribe_clear(NRF_802154_CCM_INSTANCE, NRF_CCM_TASK_START);
    nrf_ccm_subscribe_clear(NRF_802154_CCM_INSTANCE, NRF_CCM_TASK_STOP);
    nrf_ccm_int_disable(NRF_802154_CCM_INSTANCE, NRF_CCM_INT_ERROR_MASK | NRF_CCM_INT_END_MASK);
    nrf_ccm_disable(NRF_802154_CCM_INSTANCE);
}

static void ccm_irq_handler(void)
{
    if (nrf_ccm_int_enable_check(NRF_802154_CCM_INSTANCE, NRF_CCM_INT_ERROR_MASK) &&
        nrf_ccm_event_check(NRF_802154_CCM_INSTANCE, NRF_CCM_EVENT_ERROR))
    {
        nrf_ccm_event_clear(NRF_802154_CCM_INSTANCE, NRF_CCM_EVENT_ERROR);
        NRF_802154_ASSERT(false);
    }

    if (nrf_ccm_int_enable_check(NRF_802154_CCM_INSTANCE, NRF_CCM_INT_END_MASK) &&
        nrf_ccm_event_check(NRF_802154_CCM_INSTANCE, NRF_CCM_EVENT_END))
    {
        nrf_ccm_event_clear(NRF_802154_CCM_INSTANCE, NRF_CCM_EVENT_END);
        nrf_802154_tx_work_buffer_is_secured_set();
        ccm_disable();
    }
}

static void u8_string_to_u32x4_big_endian(const uint8_t * p_input,
                                          uint8_t         input_len,
                                          uint32_t      * p_output)
{
    const uint8_t unaligned = input_len % sizeof(uint32_t);
    const uint8_t len       = unaligned > 0 ? unaligned : sizeof(uint32_t);
    const uint8_t shift     = (sizeof(uint32_t) - len) * 8;
    uint32_t      data      = big_32_to_host((uint8_t *)p_input);

    p_input    += len;
    data      >>= shift;
    p_output[3] = data;

    for (int i = 2; i >= 0; i--)
    {
        data        = big_32_to_host((uint8_t *)p_input);
        p_input    += sizeof(uint32_t);
        p_output[i] = data;
    }
}

static void ccm_peripheral_configure(void)
{
    nrf_802154_irq_clear_pending(nrfx_get_irq_number(NRF_802154_CCM_INSTANCE));
    nrf_802154_irq_enable(nrfx_get_irq_number(NRF_802154_CCM_INSTANCE));

    nrf_ccm_enable(NRF_802154_CCM_INSTANCE);
    nrf_ccm_configure(NRF_802154_CCM_INSTANCE, &m_ccm_config);
    nrf_ccm_key_set(NRF_802154_CCM_INSTANCE, m_key);
    nrf_ccm_nonce_set(NRF_802154_CCM_INSTANCE, m_nonce);
    nrf_ccm_in_ptr_set(NRF_802154_CCM_INSTANCE, m_in_job_list);
    nrf_ccm_out_ptr_set(NRF_802154_CCM_INSTANCE, m_out_job_list);
    nrf_ccm_event_clear(NRF_802154_CCM_INSTANCE, NRF_CCM_EVENT_ERROR);
    nrf_ccm_event_clear(NRF_802154_CCM_INSTANCE, NRF_CCM_EVENT_END);
    nrf_ccm_int_enable(NRF_802154_CCM_INSTANCE, NRF_CCM_INT_ERROR_MASK | NRF_CCM_INT_END_MASK);
    nrf_ccm_adatamask_set(NRF_802154_CCM_INSTANCE, 0xff);
}

static void ppi_configure(void)
{
    uint32_t dppi_ch_to_enable = (1 << NRF_802154_DPPI_RADIO_DISABLED);

    nrf_ppib_subscribe_set(PPIB_RAD,
                           nrf_ppib_send_task_get(NRF_802154_DPPI_RADIO_DISABLED),
                           NRF_802154_DPPI_RADIO_DISABLED);
    nrf_ppib_publish_set(PPIB_CCM,
                         nrf_ppib_receive_event_get(NRF_802154_DPPI_RADIO_DISABLED),
                         NRF_802154_DPPI_RADIO_DISABLED);

    nrf_ccm_subscribe_set(NRF_802154_CCM_INSTANCE,
                          NRF_CCM_TASK_STOP,
                          NRF_802154_DPPI_RADIO_DISABLED);

    nrf_dppi_channels_enable(DPPIC_CCM, dppi_ch_to_enable);
}

static void ccm_configure(void)
{
    if (!m_initialized)
    {
        // DMASEC is set to non-secure by default, which prevents the CCM from accessing
        // secure memory. Change the DMASEC to secure.
#if defined(NRF54H_SERIES)
        nrf_spu_periph_perm_dmasec_set(NRF_SPU030, 10, true);
#endif
        nrf_802154_irq_init(nrfx_get_irq_number(NRF_802154_CCM_INSTANCE),
                            NRF_802154_ECB_PRIORITY,
                            ccm_irq_handler);
        m_initialized = true;
    }

    m_setup = true;
}

static void vdma_jobs_fill(const nrf_802154_aes_ccm_data_t * p_aes_ccm_data,
                           uint8_t                         * p_adata,
                           uint8_t                         * p_mdata,
                           uint8_t                           macsize)
{
    // CCM peripheral job list integer params pointed by p_job_ptr must have big-endian byte order.
    host_16_to_little(p_aes_ccm_data->auth_data_len, (uint8_t *)&m_ccm_params.in.alen);
    host_16_to_little(p_aes_ccm_data->plain_text_data_len, (uint8_t *)&m_ccm_params.in.mlen);

    nrf_vdma_job_fill(&m_in_job_list[0],
                      &m_ccm_params.in.alen,
                      sizeof(m_ccm_params.in.alen),
                      CCM_ALEN_ATTR_ID);
    nrf_vdma_job_fill(&m_in_job_list[1],
                      &m_ccm_params.in.mlen,
                      sizeof(m_ccm_params.in.mlen),
                      CCM_MLEN_ATTR_ID);
    nrf_vdma_job_fill(&m_in_job_list[2],
                      p_aes_ccm_data->auth_data,
                      p_aes_ccm_data->auth_data_len,
                      CCM_ADATA_ATTR_ID);
    nrf_vdma_job_fill(&m_in_job_list[3],
                      p_aes_ccm_data->plain_text_data,
                      p_aes_ccm_data->plain_text_data_len,
                      CCM_MDATA_ATTR_ID);
    nrf_vdma_job_terminate(&m_in_job_list[4]);

    nrf_vdma_job_fill(&m_out_job_list[0],
                      &m_ccm_params.out.alen,
                      sizeof(m_ccm_params.out.alen),
                      CCM_ALEN_ATTR_ID);
    nrf_vdma_job_fill(&m_out_job_list[1],
                      &m_ccm_params.out.mlen,
                      sizeof(m_ccm_params.out.mlen),
                      CCM_MLEN_ATTR_ID);
    nrf_vdma_job_fill(&m_out_job_list[2],
                      p_adata,
                      p_aes_ccm_data->auth_data_len,
                      CCM_ADATA_ATTR_ID);
    nrf_vdma_job_fill(&m_out_job_list[3],
                      p_mdata,
                      p_aes_ccm_data->plain_text_data_len + macsize,
                      CCM_MDATA_ATTR_ID);
    nrf_vdma_job_terminate(&m_out_job_list[4]);
}

static bool ccm_data_fill(const nrf_802154_aes_ccm_data_t * p_aes_ccm_data,
                          uint8_t                         * p_adata,
                          uint8_t                         * p_mdata)
{
    nrf_ccm_maclen_t maclen;
    uint8_t          macsize;

    switch (p_aes_ccm_data->mic_level)
    {
        case SECURITY_LEVEL_MIC_32:
        case SECURITY_LEVEL_ENC_MIC_32:
            maclen  = NRF_CCM_MODE_MACLEN_M4;
            macsize = 4;
            break;

        case SECURITY_LEVEL_MIC_64:
        case SECURITY_LEVEL_ENC_MIC_64:
            maclen  = NRF_CCM_MODE_MACLEN_M8;
            macsize = 8;
            break;

        case SECURITY_LEVEL_MIC_128:
        case SECURITY_LEVEL_ENC_MIC_128:
            maclen  = NRF_CCM_MODE_MACLEN_M16;
            macsize = 16;
            break;

        default:
            return false;
    }

    m_ccm_config.mode       = NRF_CCM_MODE_ENCRYPTION;
    m_ccm_config.datarate   = NRF_CCM_DATARATE_250K;
    m_ccm_config.protocol   = NRF_CCM_MODE_PROTOCOL_IEEE802154;
    m_ccm_config.mac_length = maclen;

    u8_string_to_u32x4_big_endian(p_aes_ccm_data->key, sizeof(p_aes_ccm_data->key), m_key);
    u8_string_to_u32x4_big_endian(p_aes_ccm_data->nonce, sizeof(p_aes_ccm_data->nonce), m_nonce);

    vdma_jobs_fill(p_aes_ccm_data, p_adata, p_mdata, macsize);

    return true;
}

bool nrf_802154_aes_ccm_transform_prepare(const nrf_802154_aes_ccm_data_t * p_aes_ccm_data)
{
    uint8_t * p_work_buffer;
    uint8_t * p_ciphertext;

    m_setup = false;

    // Verify that all necessary data is available
    if (p_aes_ccm_data->raw_frame == NULL)
    {
        return false;
    }

    // Verify that the optional data, if exists, is complete
    if (((p_aes_ccm_data->auth_data_len != 0) && (p_aes_ccm_data->auth_data == NULL)) ||
        ((p_aes_ccm_data->plain_text_data_len != 0) && (p_aes_ccm_data->plain_text_data == NULL)))
    {
        return false;
    }

    // Verify that the MIC level is valid
    if (p_aes_ccm_data->mic_level > SECURITY_LEVEL_MIC_LEVEL_MASK)
    {
        return false;
    }

    ptrdiff_t offset;

    if (p_aes_ccm_data->auth_data)
    {
        offset = p_aes_ccm_data->auth_data_len + PHR_SIZE;
    }
    else
    {
        offset = p_aes_ccm_data->raw_frame[PHR_OFFSET] + PHR_SIZE;
    }

    NRF_802154_ASSERT((offset >= 0) && (offset <= MAX_PACKET_SIZE + PHR_SIZE));

    nrf_802154_tx_work_buffer_plain_text_offset_set(offset);
    p_work_buffer = nrf_802154_tx_work_buffer_enable_for(p_aes_ccm_data->raw_frame);
    p_ciphertext  = p_work_buffer + offset;

#if CCM_OUTPTR_POINTS_TO_RADIO_PACKETPTR
    memcpy(p_work_buffer, p_aes_ccm_data->raw_frame, PHR_SIZE);
    memset(&p_work_buffer[PSDU_OFFSET], 0, p_aes_ccm_data->raw_frame[PHR_OFFSET]);
#else
    memcpy(p_work_buffer, p_aes_ccm_data->raw_frame, offset);
    memset(p_ciphertext, 0, p_aes_ccm_data->raw_frame[PHR_OFFSET] + PHR_SIZE - offset);
#endif // CCM_OUTPTR_POINTS_TO_RADIO_PACKETPTR

    if (ccm_data_fill(p_aes_ccm_data, p_work_buffer + PHR_SIZE, p_ciphertext))
    {
        ccm_configure();
        m_setup = true;
    }

    return m_setup;
}

void nrf_802154_aes_ccm_transform_start(uint8_t * p_frame)
{
    if (m_setup == false)
    {
        return;
    }

    ccm_peripheral_configure();
    ppi_configure();

    nrf_ccm_task_trigger(NRF_802154_CCM_INSTANCE, NRF_CCM_TASK_START);
}

void nrf_802154_aes_ccm_transform_abort(uint8_t * p_frame)
{
    ccm_disable();
    m_setup = false;
    memset(m_key, 0, sizeof(m_key));
    memset(m_nonce, 0, sizeof(m_nonce));
}

void nrf_802154_aes_ccm_transform_reset(void)
{
    m_setup = false;
    memset(m_key, 0, sizeof(m_key));
    memset(m_nonce, 0, sizeof(m_nonce));
}

#endif // NRF_802154_ENCRYPTION_ENABLED && !NRF_802154_ENCRYPTION_ACCELERATOR_ECB
