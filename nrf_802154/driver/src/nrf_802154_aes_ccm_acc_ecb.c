/*
 * Copyright (c) 2021, Nordic Semiconductor ASA
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

#if NRF_802154_ENCRYPTION_ACCELERATOR_ECB

#include "nrf_802154_aes_ccm.h"

#include "nrf_802154_assert.h"
#include <string.h>

#include "hal/nrf_ecb.h"
#include "nrf_802154_const.h"
#include "nrf_802154_config.h"
#include "nrf_802154_tx_work_buffer.h"
#include "platform/nrf_802154_irq.h"

#ifndef MIN
#define MIN(a, b)                                 ((a) < (b) ? (a) : (b)) ///< Leaves the minimum of the two arguments
#endif

#define NRF_802154_AES_CCM_BLOCK_SIZE             16                      // Annex B4 Specification of generic CCM* a)

#define NRF_802154_AES_CCM_ADATA_AUTH_FLAG        (0x40)                  // Annex B4.1.2 - Adata flag for authentication transform
#define NRF_802154_AES_CCM_M_BITS_AUTH_FLAG       3                       // Annex B4.1.2 - Nr of bits for MIC flag for authentication transform

#define NRF_802154_AES_CCM_AI_FIELD_FLAG_OCTET    0                       // AnnnexB4.1.3b) - Position of octet for flags in Ai field
#define NRF_802154_AES_CCM_AI_FIELD_NONCE_OCTET   1                       // AnnnexB4.1.3b) - Position of octet for nonce in Ai field
#define NRF_802154_AES_CCM_B0_FIELD_FLAG_OCTET    0                       // AnnnexB4.1.2b) - Position of octet for flags in B0 field
#define NRF_802154_AES_CCM_B0_FIELD_NONCE_OCTET   1                       // AnnnexB4.1.2b) - Position of octet for nonce in B0 field
#define NRF_802154_AES_CCM_AUTH_DATA_LENGTH_OCTET 0                       // AnnnexB4.1.1b) - Position of octet for length of auth data in AddAuthData
#define NRF_802154_AES_CCM_AUTH_DATA_OCTET        2                       // AnnnexB4.1.1b) - Position of octet for data of auth data in AddAuthData

/**
 * @brief Steps of AES-CCM* algorithm.
 */
typedef enum
{
    ADD_AUTH_DATA_AUTH,
    PLAIN_TEXT_AUTH,
    PLAIN_TEXT_ENCRYPT,
    CALCULATE_ENCRYPTED_TAG
} ccm_steps_t;

/**
 * @brief Actual state of perfomed AES-CCM* algorithm.
 */
typedef struct
{
    ccm_steps_t transformation;                                                    // Actual step of transformation
    uint8_t     iteration;                                                         // Iteration of actual step of transformation
} ccm_state_t;

static nrf_802154_aes_ccm_data_t m_aes_ccm_data;                                   ///< AES CCM Frame
static uint8_t                   m_x[NRF_802154_AES_CCM_BLOCK_SIZE];               ///< CBC-MAC value - Annex B4.1.2 d)
static uint8_t                   m_b[NRF_802154_AES_CCM_BLOCK_SIZE];               ///< B[i] octet for Authorization Transformatino - Annex B4.1.2 b)
static uint8_t                   m_m[NRF_802154_AES_CCM_BLOCK_SIZE];               ///< M[i] octet as parsed plaintext blocks - Annex B4.1.3 c)
static uint8_t                   m_a[NRF_802154_AES_CCM_BLOCK_SIZE];               ///< A[i] octet for Encryption Transformation - Annex B4.1.3 b)
static ccm_state_t               m_state;                                          ///< State of AES-CCM* transformation
static uint8_t                   m_auth_tag[MIC_128_SIZE];                         ///< Authorization Tag
static bool                      m_initialized;                                    ///< Flag that indicates whether the module has been initialized.
static uint8_t                 * mp_ciphertext;                                    ///< Pointer to ciphertext destination buffer.
static uint8_t                 * mp_work_buffer;                                   ///< Pointer to work buffer that stores the frame being transformed.

static const uint8_t m_mic_size[] = { 0, MIC_32_SIZE, MIC_64_SIZE, MIC_128_SIZE }; ///< Security level - 802.15.4-2015 Standard Table 9.6

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

static uint8_t   m_ecb_data[48];    ///< ECB data structure for RNG peripheral to access.
static uint8_t * mp_ecb_key;        ///< Key:        Starts at ecb_data
static uint8_t * mp_ecb_cleartext;  ///< Cleartext:  Starts at ecb_data + 16 bytes.
static uint8_t * mp_ecb_ciphertext; ///< Ciphertext: Starts at ecb_data + 32 bytes.

static void nrf_ecb_init(void)
{
    mp_ecb_key        = m_ecb_data;
    mp_ecb_cleartext  = m_ecb_data + 16;
    mp_ecb_ciphertext = m_ecb_data + 32;

    nrf_ecb_data_pointer_set(NRF_ECB, m_ecb_data);
}

static void nrf_ecb_set_key(const uint8_t * p_key)
{
    memcpy(mp_ecb_key, p_key, 16);
}

static void ecb_irq_handler(void);

/**
 * @brief Initializes the ECB peripheral.
 */
static void ecb_init(void)
{
    if (!m_initialized)
    {
        nrf_802154_irq_init(nrfx_get_irq_number(NRF_ECB), NRF_802154_ECB_PRIORITY, ecb_irq_handler);
        m_initialized = true;
    }

    // TODO: ensure ECB initialization is handled by zephyr
    // TODO: what about ECB initialization in baremetal scenario?
    nrf_ecb_init();

    nrf_802154_irq_clear_pending(nrfx_get_irq_number(NRF_ECB));
    nrf_802154_irq_enable(nrfx_get_irq_number(NRF_ECB));
    nrf_ecb_int_enable(NRF_ECB, NRF_ECB_INT_ENDECB_MASK);
    nrf_ecb_int_enable(NRF_ECB, NRF_ECB_INT_ERRORECB_MASK);
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/**
 * @brief Calculates XOR of two blocks of data
 *
 * @param[inout] p_first  First block of data
 * @param[in]    p_second Second block of data
 * @param[in]    len      Length of blocks
 */
static void two_blocks_xor(uint8_t * p_first, const uint8_t * p_second, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        p_first[i] ^= p_second[i];
    }
}

/**
 * @brief   Forms 16-octet Ai field
 * IEEE std 802.15.4-2015, B.4.1.3 Encryption transformation
 *
 * @param[in]  p_frame pointer to AES CCM frame structure
 * @param[in]  iter    counter of actual iteration
 * @param[out] p_a     pointer to memory for Ai
 */
static void ai_format(const nrf_802154_aes_ccm_data_t * p_frame,
                      uint16_t                          iter,
                      uint8_t                         * p_a)
{
    uint8_t enc_flags = NRF_802154_AES_CCM_L_VALUE - 1;

    p_a[NRF_802154_AES_CCM_AI_FIELD_FLAG_OCTET] = enc_flags;
    memcpy(&p_a[NRF_802154_AES_CCM_AI_FIELD_NONCE_OCTET],
           p_frame->nonce,
           NRF_802154_AES_CCM_NONCE_SIZE);
    p_a[NRF_802154_AES_CCM_BLOCK_SIZE - 1] = iter;
    p_a[NRF_802154_AES_CCM_BLOCK_SIZE - 2] = iter >> 8;
}

/**
 * @brief   Forms 16-octet B0 field
 * IEEE std 802.15.4-2015, B.4.1.2b Encryption transformation
 *
 * @param[in]  p_frame pointer to AES CCM frame structure
 * @param[in]  flags   flags for injection to B0 field
 * @param[out] p_b     pointer to memory for B0
 */
static void b0_format(const nrf_802154_aes_ccm_data_t * p_frame,
                      const uint8_t                     flags,
                      uint8_t                         * p_b)
{
    p_b[NRF_802154_AES_CCM_B0_FIELD_FLAG_OCTET] = flags;
    memcpy(&p_b[NRF_802154_AES_CCM_B0_FIELD_NONCE_OCTET],
           p_frame->nonce,
           NRF_802154_AES_CCM_NONCE_SIZE);
    p_b[NRF_802154_AES_CCM_BLOCK_SIZE - 1] = (p_frame->plain_text_data_len & 0xFF);
    p_b[NRF_802154_AES_CCM_BLOCK_SIZE - 2] = 0;
}

/**
 * @brief   Forms authentication flag
 * IEEE std 802.15.4-2015, B.4.1.2 Authentication transformation
 *
 * @param[in] p_frame pointer to AES CCM frame structure
 *
 * @return Formatted authorization flags
 */
static uint8_t auth_flags_format(const nrf_802154_aes_ccm_data_t * p_frame)
{
    uint8_t auth_flags = 0;
    uint8_t m;

    auth_flags |= (p_frame->auth_data_len == 0) ? 0 : NRF_802154_AES_CCM_ADATA_AUTH_FLAG;

    m           = m_mic_size[p_frame->mic_level];
    m           = (m > 0) ? (m - 2) >> 1 : 0;
    auth_flags |= (m << NRF_802154_AES_CCM_M_BITS_AUTH_FLAG);

    auth_flags |= NRF_802154_AES_CCM_L_VALUE - 1; // l value

    return auth_flags;
}

/**
 * @brief   Forms additional authentication data from octet string a by 16-octet chunks
 * IEEE std 802.15.4-2015, B.4.1.1 Input transformation
 *
 * @param[in]  p_frame pointer to AES CCM frame structure
 * @param[in]  iter    number of chunk
 * @param[out] p_b     pointer to memory for Bi
 *
 * @retval true  Chunk was formated
 * @retval false Otherwise
 */
static bool add_auth_data_get(const nrf_802154_aes_ccm_data_t * p_frame,
                              uint8_t                           iter,
                              uint8_t                         * p_b)
{
    uint8_t offset = 0;
    uint8_t len;

    if (p_frame->auth_data_len == 0)
    {
        return false;
    }

    memset(p_b, 0, NRF_802154_AES_CCM_BLOCK_SIZE);

    if (iter == 0)
    {
        len = MIN(p_frame->auth_data_len, NRF_802154_AES_CCM_BLOCK_SIZE - sizeof(uint16_t));
        p_b[NRF_802154_AES_CCM_AUTH_DATA_LENGTH_OCTET]     = (p_frame->auth_data_len & 0xFF00) >> 8;
        p_b[NRF_802154_AES_CCM_AUTH_DATA_LENGTH_OCTET + 1] = (p_frame->auth_data_len & 0xFF);
        memcpy(&p_b[NRF_802154_AES_CCM_AUTH_DATA_OCTET], p_frame->auth_data, len);
        return true;
    }

    offset += NRF_802154_AES_CCM_BLOCK_SIZE - sizeof(uint16_t);
    offset += NRF_802154_AES_CCM_BLOCK_SIZE * (iter - 1);
    if (offset >= p_frame->auth_data_len)
    {
        return false;
    }

    len = MIN(p_frame->auth_data_len - offset, NRF_802154_AES_CCM_BLOCK_SIZE);
    memcpy(p_b, p_frame->auth_data + offset, len);
    return true;
}

/**
 * @brief   Forms plain/cipher text data from octet string m/c by 16-octet chunks
 * IEEE std 802.15.4-2015, B.4.1.1 Input transformation
 *
 * @param[in]  p_frame pointer to AES CCM frame structure
 * @param[in]  iter    number of chunk
 * @param[out] p_b     pointer to memory for Bi
 *
 * @retval true  Chunk was formated
 * @retval false Otherwise
 */
static bool plain_text_data_get(const nrf_802154_aes_ccm_data_t * p_frame,
                                uint8_t                           iter,
                                uint8_t                         * p_b)
{
    uint8_t offset = 0;
    uint8_t len;

    if (p_frame->plain_text_data_len == 0)
    {
        return false;
    }

    memset(p_b, 0, NRF_802154_AES_CCM_BLOCK_SIZE);

    offset += NRF_802154_AES_CCM_BLOCK_SIZE * iter;
    if (offset >= p_frame->plain_text_data_len)
    {
        return false;
    }

    len = MIN(p_frame->plain_text_data_len - offset, NRF_802154_AES_CCM_BLOCK_SIZE);
    memcpy(p_b, p_frame->plain_text_data + offset, len);

    return true;
}

/**
 * @brief Block of Authorization Transformation iteration
 */
static inline void process_ecb_auth_iteration(void)
{
    m_state.iteration++;
    two_blocks_xor(mp_ecb_ciphertext, m_b, NRF_802154_AES_CCM_BLOCK_SIZE);
    memcpy(mp_ecb_cleartext, mp_ecb_ciphertext, NRF_802154_AES_CCM_BLOCK_SIZE);
    nrf_ecb_task_trigger(NRF_ECB, NRF_ECB_TASK_STARTECB);
}

/**
 * @brief Block of Encryption Transformation iteration
 */
static inline void process_ecb_encrypt_iteration(void)
{
    ai_format(&m_aes_ccm_data, m_state.iteration, m_a);
    memcpy(mp_ecb_cleartext, m_a, NRF_802154_AES_CCM_BLOCK_SIZE);
    nrf_ecb_task_trigger(NRF_ECB, NRF_ECB_TASK_STARTECB);
}

/**
 * @brief helper function for plain text encryption in ECB IRQ
 */
static void perform_plain_text_encryption(void)
{
    memcpy(m_auth_tag, mp_ecb_ciphertext, m_mic_size[m_aes_ccm_data.mic_level]);

    m_state.iteration      = 0;
    m_state.transformation = PLAIN_TEXT_ENCRYPT;

    if (plain_text_data_get(&m_aes_ccm_data, m_state.iteration, m_m))
    {
        m_state.iteration++;
        process_ecb_encrypt_iteration();
    }
    else
    {
        if (m_mic_size[m_aes_ccm_data.mic_level] != 0)
        {
            process_ecb_encrypt_iteration();
            m_state.transformation = CALCULATE_ENCRYPTED_TAG;
        }
    }
}

/**
 * @brief helper function for plain text auth in ECB IRQ
 */
static void perform_plain_text_authorization(void)
{
    if (plain_text_data_get(&m_aes_ccm_data, m_state.iteration, m_b))
    {
        process_ecb_auth_iteration();
    }
    else
    {
        perform_plain_text_encryption();
    }
}

static void transformation_finished(void)
{
    nrf_802154_tx_work_buffer_is_secured_set();
    m_aes_ccm_data.raw_frame = NULL;
}

/**
 * @brief Handler to ECB Interrupt Routine
 *  Performs AES-CCM* calculation in pipeline
 */
static void ecb_irq_handler(void)
{
    uint8_t len = 0;
    uint8_t offset;

    if (nrf_ecb_int_enable_check(NRF_ECB, NRF_ECB_INT_ENDECB_MASK) &&
        nrf_ecb_event_check(NRF_ECB, NRF_ECB_EVENT_ENDECB))
    {
        nrf_ecb_event_clear(NRF_ECB, NRF_ECB_EVENT_ENDECB);

        switch (m_state.transformation)
        {
            case ADD_AUTH_DATA_AUTH:
                if (add_auth_data_get(&m_aes_ccm_data, m_state.iteration, m_b))
                {
                    process_ecb_auth_iteration();
                }
                else
                {
                    m_state.iteration      = 0;
                    m_state.transformation = PLAIN_TEXT_AUTH;
                    perform_plain_text_authorization();
                }
                break;

            case PLAIN_TEXT_AUTH:
                perform_plain_text_authorization();
                break;

            case PLAIN_TEXT_ENCRYPT:
                two_blocks_xor(m_m, mp_ecb_ciphertext, NRF_802154_AES_CCM_BLOCK_SIZE);

                offset = (m_state.iteration - 1) * NRF_802154_AES_CCM_BLOCK_SIZE;
                len    = MIN(m_aes_ccm_data.plain_text_data_len - offset,
                             NRF_802154_AES_CCM_BLOCK_SIZE);
                memcpy(mp_ciphertext + offset, m_m, len);
                if (plain_text_data_get(&m_aes_ccm_data, m_state.iteration, m_m))
                {
                    m_state.iteration++;
                    process_ecb_encrypt_iteration();
                }
                else
                {
                    if (m_mic_size[m_aes_ccm_data.mic_level] != 0)
                    {
                        m_state.iteration      = 0;
                        m_state.transformation = CALCULATE_ENCRYPTED_TAG;
                        process_ecb_encrypt_iteration();
                    }
                    else
                    {
                        transformation_finished();
                    }
                }
                break;

            case CALCULATE_ENCRYPTED_TAG:
                two_blocks_xor(m_auth_tag,
                               mp_ecb_ciphertext,
                               m_mic_size[m_aes_ccm_data.mic_level]);
                memcpy(mp_work_buffer +
                       (mp_work_buffer[PHR_OFFSET] - FCS_SIZE -
                        m_mic_size[m_aes_ccm_data.mic_level] +
                        PHR_SIZE),
                       m_auth_tag,
                       m_mic_size[m_aes_ccm_data.mic_level]);
                transformation_finished();
                break;

            default:
                break;
        }
    }

    if (nrf_ecb_int_enable_check(NRF_ECB, NRF_ECB_INT_ERRORECB_MASK) &&
        nrf_ecb_event_check(NRF_ECB, NRF_ECB_EVENT_ERRORECB))
    {
        /*
         * It is possible that the ERRORECB event is caused by the
         * AAR and CCM peripherals, which share the same hardware resources.
         * At this point it is assumed, that ECB, AAR and CCM peripherals
         * are not used by anything, except the 802.15.4 driver and
         * other MPSL clients and thus it is impossible that ECB was aborted
         * for any other reason, than the TX failed event caused by a terminated
         * 802.15.4 transmit operation or end of timeslot.
         *
         * Therefore no action is taken in this handler.
         */
        nrf_ecb_event_clear(NRF_ECB, NRF_ECB_EVENT_ERRORECB);
    }
}

/**
 * @brief Start AES-CCM* Authorization Transformation
 */
static void start_ecb_auth_transformation(void)
{
    memcpy((uint8_t *)nrf_ecb_data_pointer_get(NRF_ECB) + 16, m_x, 16);
    m_state.iteration      = 0;
    m_state.transformation = ADD_AUTH_DATA_AUTH;
    nrf_ecb_event_clear(NRF_ECB, NRF_ECB_EVENT_ENDECB);
    nrf_ecb_task_trigger(NRF_ECB, NRF_ECB_TASK_STARTECB);
}

void nrf_802154_aes_ccm_transform_reset(void)
{
    m_aes_ccm_data.raw_frame = NULL;
}

bool nrf_802154_aes_ccm_transform_prepare(const nrf_802154_aes_ccm_data_t * p_aes_ccm_data)
{
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

    // Store the encryption data for future use
    memcpy(&m_aes_ccm_data, p_aes_ccm_data, sizeof(nrf_802154_aes_ccm_data_t));

    ptrdiff_t offset = p_aes_ccm_data->raw_frame[PHR_OFFSET] + PHR_SIZE;

    if (p_aes_ccm_data->plain_text_data)
    {
        offset = p_aes_ccm_data->plain_text_data - p_aes_ccm_data->raw_frame;
    }

    NRF_802154_ASSERT((offset >= 0) && (offset <= MAX_PACKET_SIZE + PHR_SIZE));

    nrf_802154_tx_work_buffer_plain_text_offset_set(offset);
    mp_work_buffer = nrf_802154_tx_work_buffer_enable_for(p_aes_ccm_data->raw_frame);
    mp_ciphertext  = mp_work_buffer + offset;

    memcpy(mp_work_buffer, p_aes_ccm_data->raw_frame, offset);
    memset(mp_ciphertext, 0, p_aes_ccm_data->raw_frame[PHR_OFFSET] + PHR_SIZE - offset);

    return true;
}

void nrf_802154_aes_ccm_transform_start(uint8_t * p_frame)
{
    // Verify that the algorithm's inputs were prepared properly
    if ((p_frame != m_aes_ccm_data.raw_frame) || (m_aes_ccm_data.raw_frame == NULL))
    {
        return;
    }

    uint8_t   auth_flags = auth_flags_format(&m_aes_ccm_data);
    uint8_t * p_x        = m_x;
    uint8_t * p_b        = m_b;
    ptrdiff_t offset     = mp_ciphertext - mp_work_buffer;

    // Copy updated part of the frame
    memcpy(mp_work_buffer, p_frame, offset);

    // initial settings
    memset(p_x, 0, NRF_802154_AES_CCM_BLOCK_SIZE);
    b0_format(&m_aes_ccm_data, auth_flags, p_b);

    two_blocks_xor(p_x, p_b, NRF_802154_AES_CCM_BLOCK_SIZE);
    ecb_init();
    memset(mp_ecb_key, 0, 48);
    nrf_ecb_set_key(m_aes_ccm_data.key);
    start_ecb_auth_transformation();
}

void nrf_802154_aes_ccm_transform_abort(uint8_t * p_frame)
{
    // Verify that the encryption of the correct frame is being aborted.
    if (p_frame != m_aes_ccm_data.raw_frame)
    {
        return;
    }

    /*
     * Temporarily disable ENDECB interrupt, trigger STOPECB task
     * to stop encryption in case it is still running and clear
     * the ENDECB event in case the encryption has completed.
     */
    nrf_ecb_int_disable(NRF_ECB, NRF_ECB_INT_ENDECB_MASK);
    nrf_ecb_task_trigger(NRF_ECB, NRF_ECB_TASK_STOPECB);
    nrf_ecb_event_clear(NRF_ECB, NRF_ECB_EVENT_ENDECB);
    nrf_ecb_int_enable(NRF_ECB, NRF_ECB_INT_ENDECB_MASK);

    m_aes_ccm_data.raw_frame = NULL;
}

#endif /* NRF_802154_ENCRYPTION_ACCELERATOR_ECB */
