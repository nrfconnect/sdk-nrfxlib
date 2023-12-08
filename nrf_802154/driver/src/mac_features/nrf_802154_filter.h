/*
 * Copyright (c) 2018, Nordic Semiconductor ASA
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
 * @brief Module that implements the incoming frame filter API.
 *
 */

#ifndef NRF_802154_FILTER_H_
#define NRF_802154_FILTER_H_

#include <stdbool.h>
#include <stdint.h>

#include "nrf_802154_types.h"
#include "mac_features/nrf_802154_frame_parser.h"

/**
 * @defgroup nrf_802154_filter Incoming frame filter API
 * @{
 * @ingroup nrf_802154
 * @brief Procedures used to discard the incoming frames that contain unexpected data in PHR or MHR.
 */

/**@brief Type representing the scope of frame filtering.
 *
 * Possible values:
 * @ref NRF_802154_FILTER_MODE_FCF
 * @ref NRF_802154_FILTER_MODE_DST_ADDR
 */
typedef uint32_t nrf_802154_filter_mode_t;

/**@brief Filter the frame based on the content of the FCF field. */
#define NRF_802154_FILTER_MODE_FCF      (1UL << 0)

/**@brief Filter the frame based on the content of the destination addressing fields. */
#define NRF_802154_FILTER_MODE_DST_ADDR (1UL << 1)

/**@brief Perform all filtering tests. */
#define NRF_802154_FILTER_MODE_ALL      (NRF_802154_FILTER_MODE_FCF | \
                                         NRF_802154_FILTER_MODE_DST_ADDR)

/**
 * @brief Verifies if the given part of the frame is valid.
 *
 * This function filters the given frame within the scope specified by @p filter_mode argument.
 * In the @c NRF_802154_FILTER_MODE_FCF filter mode the function will reject all frames that do
 * not satisfy requirements that can be determined by parsing the FCF field, such as unsupported
 * frame versions, unsupported frame types, or when destination addressing is not present in the
 * frame and the node is not a coordinator.
 * In the @c NRF_802154_FILTER_MODE_DST_ADDR filter mode the function will only check if the
 * destination address matches the address of the current node. If the incoming frame has no address
 * and the node is a coordinator the frame will be accepted. No validation of FCF is performend.
 *
 * To perform a full filtering procedure it is necessary to call this function on the incoming frame
 * with both @c NRF_802154_FILTER_MODE_FCF and @c NRF_802154_FILTER_MODE_DST_ADDR scopes in
 * two iterations, or both at once using @c NRF_802154_FILTER_MODE_ALL.
 *
 * @param[in] p_frame_data Pointer to a frame parser data of the frame to be filtered.
 * @param[in] filter_mode  The filtering scope that should be performed by the function.
 *
 * @retval NRF_802154_RX_ERROR_NONE               Verified part of the incoming frame is valid.
 * @retval NRF_802154_RX_ERROR_INVALID_FRAME      Verified part of the incoming frame is invalid.
 * @retval NRF_802154_RX_ERROR_INVALID_DEST_ADDR  Incoming frame has destination address that
 *                                                mismatches the address of this node.
 * @retval NRF_802154_RX_ERROR_INVALID_LENGTH     Received a frame with invalid length.
 */
nrf_802154_rx_error_t nrf_802154_filter_frame_part(
    const nrf_802154_frame_parser_data_t * p_frame_data,
    nrf_802154_filter_mode_t               filter_mode);

/**
 *@}
 **/

#endif /* NRF_802154_FILTER_H_ */
