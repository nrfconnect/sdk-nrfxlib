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

#ifndef NRF_802154_CSMA_CA_H__
#define NRF_802154_CSMA_CA_H__

#include <stdbool.h>
#include <stdint.h>

#include "nrf_802154_const.h"
#include "nrf_802154_types.h"
#include "mac_features/nrf_802154_frame.h"

/**
 * @defgroup nrf_802154_csma_ca 802.15.4 driver CSMA-CA support
 * @{
 * @ingroup nrf_802154
 * @brief CSMA-CA procedure.
 */

/**
 * @brief Starts the CSMA-CA procedure for the transmission of a given frame.
 *
 * If the CSMA-CA procedure is successful and the frame is transmitted,
 * the @ref nrf_802154_transmitted_raw function is called. If the procedure
 * failed and the frame cannot be transmitted due to busy channel,
 * the @ref nrf_802154_transmit_failed function is called.
 *
 * @param[in]  p_frame     Pointer to a frame data structure.
 * @param[in]  p_metadata  Pointer to metadata structure. Contains detailed properties of data
 *                         to transmit.
 *
 * @retval true   The function always returns true for compatibility reasons
 */
bool nrf_802154_csma_ca_start(const nrf_802154_frame_t                     * p_frame,
                              const nrf_802154_transmit_csma_ca_metadata_t * p_metadata);

/**
 *@}
 **/

#endif // NRF_802154_CSMA_CA_H__
