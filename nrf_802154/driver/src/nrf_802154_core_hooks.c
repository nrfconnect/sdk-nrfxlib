/*
 * Copyright (c) 2017, Nordic Semiconductor ASA
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

/**
 * @file
 *   This file implements hooks for the 802.15.4 driver Core module.
 *
 * Hooks are used by optional driver features to modify way in which notifications are propagated
 * through the driver.
 *
 */

#include "nrf_802154_core_hooks.h"

#include <stdbool.h>

#include "mac_features/nrf_802154_ack_timeout.h"
#include "mac_features/nrf_802154_delayed_trx.h"
#include "mac_features/nrf_802154_ie_writer.h"
#include "mac_features/nrf_802154_security_writer.h"
#include "mac_features/nrf_802154_tx_timestamp_provider.h"
#include "nrf_802154_encrypt.h"
#include "nrf_802154_config.h"

typedef bool (* abort_hook)(nrf_802154_term_t term_lvl, req_originator_t req_orig);
typedef nrf_802154_tx_error_t (* tx_setup_hook)(nrf_802154_transmit_params_t * p_params);
typedef void (* transmitted_hook)(const nrf_802154_frame_t * p_frame);
typedef void (* tx_failed_hook)(uint8_t * p_frame, nrf_802154_tx_error_t error);
typedef void (* tx_ack_failed_hook)(uint8_t * p_ack, nrf_802154_tx_error_t error);
typedef void (* tx_started_hook)(uint8_t * p_frame);
typedef void (* rx_started_hook)(const nrf_802154_frame_t * p_frame);
typedef void (* rx_ack_started_hook)(void);
typedef void (* tx_ack_started_hook)(uint8_t * p_ack);

/* Since some compilers do not allow empty initializers for arrays with unspecified bounds,
 * NULL pointer is appended to below arrays if the compiler used is not GCC. It is intentionally
 * unused, but it prevents the arrays from being empty. GCC manages to optimize empty arrays away,
 * so such a solution is unnecessary. */

static const abort_hook m_abort_hooks[] =
{
#if NRF_802154_ACK_TIMEOUT_ENABLED
    nrf_802154_ack_timeout_abort,
#endif

#if NRF_802154_DELAYED_TRX_ENABLED
    nrf_802154_delayed_trx_abort,
#endif

    NULL,
};

static const tx_setup_hook m_tx_setup_hooks[] =
{
#if NRF_802154_TX_TIMESTAMP_PROVIDER_ENABLED
    nrf_802154_tx_timestamp_provider_tx_setup,
#endif
#if NRF_802154_IE_WRITER_ENABLED
    nrf_802154_ie_writer_tx_setup,
#endif
#if NRF_802154_SECURITY_WRITER_ENABLED
    nrf_802154_security_writer_tx_setup,
#endif
#if NRF_802154_ENCRYPTION_ENABLED
    nrf_802154_encrypt_tx_setup,
#endif
    NULL,
};

static const transmitted_hook m_transmitted_hooks[] =
{
#if NRF_802154_ACK_TIMEOUT_ENABLED
    nrf_802154_ack_timeout_transmitted_hook,
#endif
    NULL,
};

static const tx_failed_hook m_tx_failed_hooks[] =
{
#if NRF_802154_ACK_TIMEOUT_ENABLED
    nrf_802154_ack_timeout_tx_failed_hook,
#endif

#if NRF_802154_ENCRYPTION_ENABLED
    nrf_802154_encrypt_tx_failed_hook,
#endif

    NULL,
};

static const tx_ack_failed_hook m_tx_ack_failed_hooks[] =
{
#if NRF_802154_ENCRYPTION_ENABLED
    nrf_802154_encrypt_tx_ack_failed_hook,
#endif

    NULL,
};

static const tx_started_hook m_tx_started_hooks[] =
{
#if NRF_802154_ACK_TIMEOUT_ENABLED
    nrf_802154_ack_timeout_tx_started_hook,
#endif

#if NRF_802154_SECURITY_WRITER_ENABLED
    nrf_802154_security_writer_tx_started_hook,
#endif

#if NRF_802154_TX_TIMESTAMP_PROVIDER_ENABLED
    nrf_802154_tx_timestamp_provider_tx_started_hook,
#endif

#if NRF_802154_IE_WRITER_ENABLED
    nrf_802154_ie_writer_tx_started_hook,
#endif

#if NRF_802154_ENCRYPTION_ENABLED
    nrf_802154_encrypt_tx_started_hook,
#endif

    NULL,
};

static const rx_started_hook m_rx_started_hooks[] =
{
#if NRF_802154_DELAYED_TRX_ENABLED
    nrf_802154_delayed_trx_rx_started_hook,
#endif

    NULL,
};

static const rx_ack_started_hook m_rx_ack_started_hooks[] =
{
#if NRF_802154_ACK_TIMEOUT_ENABLED
    nrf_802154_ack_timeout_rx_ack_started_hook,
#endif

    NULL,
};

static const tx_ack_started_hook m_tx_ack_started_hooks[] =
{
#if NRF_802154_IE_WRITER_ENABLED
    nrf_802154_ie_writer_tx_ack_started_hook,
#endif

#if NRF_802154_ENCRYPTION_ENABLED
    nrf_802154_encrypt_tx_ack_started_hook,
#endif

    NULL,
};

bool nrf_802154_core_hooks_terminate(nrf_802154_term_t term_lvl, req_originator_t req_orig)
{
    bool result = true;

    for (uint32_t i = 0; i < sizeof(m_abort_hooks) / sizeof(m_abort_hooks[0]); i++)
    {
        if (m_abort_hooks[i] == NULL)
        {
            break;
        }

        result = m_abort_hooks[i](term_lvl, req_orig);

        if (!result)
        {
            break;
        }
    }

    return result;
}

nrf_802154_tx_error_t nrf_802154_core_hooks_tx_setup(
    nrf_802154_transmit_params_t * p_params)
{
    nrf_802154_tx_error_t result = NRF_802154_TX_ERROR_NONE;

    for (uint32_t i = 0; i < sizeof(m_tx_setup_hooks) / sizeof(m_tx_setup_hooks[0]);
         i++)
    {
        if (m_tx_setup_hooks[i] == NULL)
        {
            break;
        }

        result = m_tx_setup_hooks[i](p_params);

        if (result != NRF_802154_TX_ERROR_NONE)
        {
            break;
        }
    }

    return result;
}

void nrf_802154_core_hooks_transmitted(const nrf_802154_frame_t * p_frame)
{
    for (uint32_t i = 0; i < sizeof(m_transmitted_hooks) / sizeof(m_transmitted_hooks[0]); i++)
    {
        if (m_transmitted_hooks[i] == NULL)
        {
            break;
        }

        m_transmitted_hooks[i](p_frame);
    }
}

void nrf_802154_core_hooks_tx_failed(uint8_t * p_frame, nrf_802154_tx_error_t error)
{
    for (uint32_t i = 0; i < sizeof(m_tx_failed_hooks) / sizeof(m_tx_failed_hooks[0]); i++)
    {
        if (m_tx_failed_hooks[i] == NULL)
        {
            break;
        }

        m_tx_failed_hooks[i](p_frame, error);
    }
}

void nrf_802154_core_hooks_tx_ack_failed(uint8_t * p_ack, nrf_802154_tx_error_t error)
{
    for (uint32_t i = 0; i < sizeof(m_tx_ack_failed_hooks) / sizeof(m_tx_ack_failed_hooks[0]); i++)
    {
        if (m_tx_ack_failed_hooks[i] == NULL)
        {
            break;
        }

        m_tx_ack_failed_hooks[i](p_ack, error);
    }
}

void nrf_802154_core_hooks_tx_started(uint8_t * p_frame)
{
    for (uint32_t i = 0; i < sizeof(m_tx_started_hooks) / sizeof(m_tx_started_hooks[0]); i++)
    {
        if (m_tx_started_hooks[i] == NULL)
        {
            break;
        }

        m_tx_started_hooks[i](p_frame);
    }
}

void nrf_802154_core_hooks_rx_started(const nrf_802154_frame_t * p_frame)
{
    for (uint32_t i = 0; i < sizeof(m_rx_started_hooks) / sizeof(m_rx_started_hooks[0]); i++)
    {
        if (m_rx_started_hooks[i] == NULL)
        {
            break;
        }

        m_rx_started_hooks[i](p_frame);
    }
}

void nrf_802154_core_hooks_rx_ack_started(void)
{
    for (uint32_t i = 0; i < sizeof(m_rx_ack_started_hooks) / sizeof(m_rx_ack_started_hooks[0]);
         i++)
    {
        if (m_rx_ack_started_hooks[i] == NULL)
        {
            break;
        }

        m_rx_ack_started_hooks[i]();
    }
}

void nrf_802154_core_hooks_tx_ack_started(uint8_t * p_ack)
{
    for (uint32_t i = 0; i < sizeof(m_tx_ack_started_hooks) / sizeof(m_tx_ack_started_hooks[0]);
         i++)
    {
        if (m_tx_ack_started_hooks[i] == NULL)
        {
            break;
        }

        m_tx_ack_started_hooks[i](p_ack);
    }
}
