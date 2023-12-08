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
 *   This file implements debug helpers for the nRF 802.15.4 radio driver.
 *
 */

#include <nrfx.h>

#include "nrf_802154_debug.h"
#include "nrf_802154_debug_log_codes.h"

void nrf_802154_debug_gpio_init(void);

void nrf_802154_debug_init(void)
{

#if NRF_802154_SL_ENABLE_DEBUG_LOG
    // dummy_local is needed for only one purpose - to ensure the presence of typedef-s
    // (used inside nrf_802154_drv_typedefs_to_save_in_elf_t union) in final .elf file.
    // The original reason is that the information about particular 'typdef' is included
    // in the .elf file only if this 'typdef' is used for the actual operation in the code.
    nrf_802154_drv_typedefs_to_save_in_elf_t volatile dummy_local;

    dummy_local.dummy00tag = 0;
    (void)dummy_local;
#endif

#if ENABLE_DEBUG_GPIO
    nrf_802154_debug_gpio_init();
#endif // ENABLE_DEBUG_GPIO

#if ENABLE_DEBUG_LOG
#warning attempt to use deprecated ENABLE_DEBUG_LOG switch
// When linking with SL use NRF_802154_SL_ENABLE_DEBUG_LOG instead
#endif // ENABLE_DEBUG_LOG

    nrf_802154_sl_log_init();
}
