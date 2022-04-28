/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2022 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: Deprecated Definitions of SE's Keep-Alive cluster (SE spec, Annex A.3)
*/

#ifndef ZB_SE_KEEP_ALIVE_H
#define ZB_SE_KEEP_ALIVE_H 1

/** @cond DOXYGEN_SE_SECTION */

/** @addtogroup ZB_ZCL_KEEP_ALIVE_HEADER_DEPRECATED
 *  @{
 *    @details
 *    Using of this SE cluster header is deprecated. The header was moved from SE to ZCL in ZCL8. Use ZCL Keep Alive Cluster instead (include/zcl/zb_zcl_keep_alive.h).
 *    @deprecated
 *    This cluster header as a part of SE will be removed in February 2023 in favor of its heir ZCL Keep Alive Cluster.
 *
 */

/* Deprecated header ZB_SE_KEEP_ALIVE_H */

/*! @} */ /* SE Keep-Alive cluster definitions */

/** @endcond */ /* DOXYGEN_SE_SECTION */

#include "zcl/zb_zcl_keep_alive.h"

#endif /* ZB_SE_KEEP_ALIVE_H */
