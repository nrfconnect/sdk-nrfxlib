/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2020 DSR Corporation, Denver CO, USA.
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
/*  PURPOSE: The biggest possible configuration: ZC, big net, high load, high complexity
*/
#ifndef ZB_MEM_CONFIG_MAXIMUM_H
#define ZB_MEM_CONFIG_MAXIMUM_H 1

/**
   @addtogroup configurable_mem
   @{
*/
/**
   Configure memory sizes for ZC device role
 */
#define ZB_CONFIG_ROLE_ZC

#ifdef DOXYGEN 
/**
   Configure memory sizes for ZED device role
 */
#define ZB_CONFIG_ROLE_ZED
/**
   Configure memory sizes for ZR device role
 */
#define ZB_CONFIG_ROLE_ZR
#endif 

/*#define ZB_CONFIG_ROLE_ZR*/
/*#define ZB_CONFIG_ROLE_ZED*/
/**
   Max total number of devices in Zigbee network.

   From 2 to 200.
 */
#define ZB_CONFIG_OVERALL_NETWORK_SIZE 200U
/*#define ZB_CONFIG_OVERALL_NETWORK_SIZE 16*/
/*#define ZB_CONFIG_OVERALL_NETWORK_SIZE 32*/

/**
   High routing and application traffic from/to that device.
 */
#define ZB_CONFIG_HIGH_TRAFFIC
/*#define ZB_CONFIG_MODERATE_TRAFFIC*/
/*#define ZB_CONFIG_LIGHT_TRAFFIC*/

/**
   Complex user's application at that device: complex relations to other devices.
 */
#define ZB_CONFIG_APPLICATION_COMPLEX
/*#define ZB_CONFIG_APPLICATION_MODERATE*/
/*#define ZB_CONFIG_APPLICATION_SIMPLE*/

/**
    @}
*/

/* Now common logic derives numerical parameters from the defined configuration. */
#include "zb_mem_config_common.h"

/* Now if you REALLY know what you do, you can study zb_mem_config_common.h and redefine some configuration parameters, like:
#undef ZB_CONFIG_SCHEDULER_Q_SIZE
#define ZB_CONFIG_SCHEDULER_Q_SIZE 56
*/

/* Memory context definitions */
#include "zb_mem_config_context.h"

#endif /* ZB_MEM_CONFIG_MAXIMUM_H */
