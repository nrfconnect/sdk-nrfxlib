/*
 * Copyright (c) 2019, Nordic Semiconductor ASA
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

#ifndef NRF_802154_DEBUG_LOG_H_
#define NRF_802154_DEBUG_LOG_H_

#include "nrf_802154_config.h"
#include "nrf_802154_sl_log.h"
#include "nrf_802154_debug_log_codes.h"

/**@brief Records log about entry to a function.
 * @param verbosity     Verbosity level of the module in which log is recorded required to emit log.
 */
#define nrf_802154_log_function_enter(verbosity) \
    nrf_802154_sl_log_function_enter(verbosity)

/**@brief Records log about exit from a function.
 * @param verbosity     Verbosity level of the module in which log is recorded required to emit log.
 */
#define nrf_802154_log_function_exit(verbosity) \
    nrf_802154_sl_log_function_exit(verbosity)

/**@brief Records log about event (with parameter) related to current module.
 * @param verbosity         Verbosity level of the module in which log is recorded required to emit log.
 * @param local_event_id    Event identifier whose meaning is defined within scope of the module
 *                          in which log is recorded. Possible values 0...63
 * @param param_u16         Additional parameter to be logged with event. Meaning
 *                          of the parameter is defined by the module in which
 *                          the log is recorded and event_id.
 */
#define nrf_802154_log_local_event(verbosity, local_event_id, param_u16) \
    nrf_802154_sl_log_local_event(verbosity, local_event_id, param_u16)

/**@brief Records log about event (with parameter) related to global resource.
 * @param verbosity         Verbosity level of the module in which log is recorded required to emit log.
 * @param global_event_id   Event identifier whose meaning is defined globally. Possible values 0...63
 * @param param_u16         Additional parameter to be logged with event. Meaning
 *                          of the parameter is defined by value of global_event_id.
 */
#define nrf_802154_log_global_event(verbosity, global_event_id, param_u16) \
    nrf_802154_sl_log_global_event(verbosity, global_event_id, param_u16)

#endif /* NRF_802154_DEBUG_LOG_H_ */
