/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2024 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: Zigbee nvram management
*/

#ifndef ZB_NVRAM_GLOBAL_H
#define ZB_NVRAM_GLOBAL_H 1

#if defined ZB_USE_NVRAM || defined doxygen

#include "zb_nvram.h"

typedef struct zb_nvram_pos_payload_checker_s
{
  zb_lbitfield_t pos:30;
  zb_lbitfield_t page:2;
  zb_uint16_t payload_length;
  zb_uint16_t crc; /* < CRC field is needed to accumulate crc while writing */
} zb_nvram_pos_payload_checker_t;

typedef ZB_PACKED_PRE struct zb_nvram_page_state_s
{
  zb_nvram_ver_t nvram_ver;
  zb_bitfield_t erased:1;
  zb_bitfield_t erase_in_progress:1;
  zb_bitfield_t reserved:6;
  zb_bitfield_t alignment:8;
} ZB_PACKED_STRUCT zb_nvram_page_state_t;

#define ZB_NVRAM_MAX_N_PAGES 2U

#ifdef ZB_NVRAM_ENABLE_DIRECT_API

typedef struct zb_nvram_ds_pos_s
{
  zb_uint32_t pos;
  zb_uint8_t page;
} zb_nvram_ds_pos_t;

#endif /* ZB_NVRAM_ENABLE_DIRECT_API */

typedef struct zb_nvram_custom_ds_entry_s
{
  zb_nvram_ds_filter_cb_t filter;
  zb_nvram_ds_get_length_cb_t get_length;
  zb_nvram_ds_get_version_cb_t get_version;
  zb_nvram_ds_read_cb_t read;
  zb_nvram_ds_write_cb_t write;
} zb_nvram_custom_ds_entry_t;

#define ZB_NVRAM_CUSTOM_DS_MAX_NUMBER 2U

typedef struct zb_nvram_custom_ds_ctx_s
{
  zb_nvram_custom_ds_entry_t handlers[ZB_NVRAM_CUSTOM_DS_MAX_NUMBER];
  zb_size_t ds_cnt;
} zb_nvram_custom_ds_ctx_t;


/**
   NVRAM global context
*/
typedef struct zb_nvram_globals_s
{
  zb_nvram_read_app_data_t      read_app_data_cb[ZB_NVRAM_APP_DATASET_NUMBER];
  zb_nvram_write_app_data_t     write_app_data_cb[ZB_NVRAM_APP_DATASET_NUMBER];
  zb_nvram_get_app_data_size_t  get_app_data_size_cb[ZB_NVRAM_APP_DATASET_NUMBER];
  zb_nvram_custom_ds_ctx_t      custom_ds_ctx;

  zb_nvram_ds_filter_cb_t ds_filter_cb; /*!< If assigned function to be called to filter out some datasets at load  */
  zb_nvram_tl_t current_time_label;     /*!< Current time label. Max from wrote NVRAM dataset header */
  zb_nvram_page_state_t pages[ZB_NVRAM_MAX_N_PAGES];
  zb_uint32_t current_pos;              /*!< Index of first free byte in Current Page NVRAM */
  zb_uint8_t current_page;              /*!< Current page NVRAM */

  zb_bitfield_t refresh_flag:1;              /*! flag indicating that device need to store counters as soon as it increment it */
  zb_bitfield_t transaction_ongoing:1; /*!< @see @ref nvram_transaction_state  */
  zb_bitfield_t empty:1;             /*!< if true, NVRAM is empty (maybe, except NWK
                                 * counter), so device is factory new  */
  zb_bitfield_t inited:1;            /*!< if true, NVRAM is inited (read/write operations are
                                      * allowed) */
  zb_bitfield_t alignment:4;

#ifdef ZB_NVRAM_ENABLE_DIRECT_API
  /* positions of datasets; updated on each write operation; used for read operations */
  zb_nvram_ds_pos_t ds_pos[ZB_NVRAM_DATASET_NUMBER];
#endif /* ZB_NVRAM_ENABLE_DIRECT_API */

  /* Actually useful at Nordic only. Move it out of there? */
  volatile zb_uint_t op_in_progress;

  /* Checker for nvram read/write calls */
  zb_nvram_pos_payload_checker_t gs_nvram_write_checker;
  zb_nvram_pos_payload_checker_t gs_nvram_read_checker;
  zb_nvram_pos_payload_checker_t gs_nvram_read_checker_cache;

} zb_nvram_globals_t;

#define ZB_NVRAM() ZG->nvram

#endif


#endif /* ZB_NVRAM_GLOBAL_H */
