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
/* PURPOSE: Zigbee nvram internal management
*/

#ifndef ZB_NVRAM_H
#define ZB_NVRAM_H 1

#if defined ZB_USE_NVRAM || defined doxygen

/* Always use 32-bit time label. It implies 16-bit variable
 * for data length in header, which allows to store big datasets
 * like ZGP dataset */
#define ZB_NVRAM_32BIT_TL

/**
   NVRAM time label (initially): 16 bits, 0xffff is reserved (negative)
   Cortex M3 can write by 32-bit words only. Use 32-bit time.
 */
#ifdef ZB_PLATFORM_CORTEX_M3
#ifndef ZB_NVRAM_32BIT_TL
#define ZB_NVRAM_32BIT_TL
#endif
/** if defined, after erase memory contents is zero, else -1 */
#endif

#if (defined ZB_PLATFORM_XAP5 || defined ZB_PLATFORM_CORTEX_M3)
#define ZB_NVRAM_ERASE_TO_ZERO
#endif

#ifndef ZB_NVRAM_32BIT_TL
typedef zb_uint16_t zb_nvram_tl_t;
#else
typedef zb_uint32_t zb_nvram_tl_t;
#endif

#ifdef ZB_NVRAM_ERASE_TO_ZERO
#define ZB_NVRAM_TL_RESERVED 0U
#endif

#ifndef ZB_NVRAM_32BIT_TL

#ifndef ZB_NVRAM_ERASE_TO_ZERO
#define ZB_NVRAM_TL_RESERVED 0xffffU
#endif

#define ZB_NVRAM_TL_MASK 0x7fff

#else

#ifndef ZB_NVRAM_ERASE_TO_ZERO
#define ZB_NVRAM_TL_RESERVED  (zb_uint32_t)(~0u)
#endif

#define ZB_NVRAM_TL_MASK 0x7fffffffU
#endif

#define ZB_NVRAM_HALF_MAX_TL_VAL (ZB_NVRAM_TL_MASK / 2U)

#ifndef ZB_NVRAM_ERASE_TO_ZERO
#define ZB_NVRAM_TL_INC(a) (zb_nvram_tl_t)(((a) + 1U) & ZB_NVRAM_TL_MASK)
#else
/* Increment time label, with overflow. If 0 is reserved (produced by erase), jump over it. */
#define ZB_NVRAM_TL_INC(a) (zb_nvram_tl_t)((a) == ZB_NVRAM_TL_MASK ? 1U : (a) + 1U)
#endif

#define ZB_NVRAM_TL_GE(a, b) ((((a) - (b)) & ZB_NVRAM_TL_MASK) < ZB_NVRAM_HALF_MAX_TL_VAL)
#define ZB_NVRAM_TL_GT(a, b) ((a) != (b) && ZB_NVRAM_TL_GE(a, b))

#define ZB_NVRAM_TL_INITIAL 1U


/**
   Data set header in nvram segment
 */
typedef ZB_PACKED_PRE struct zb_nvram_dataset_hdr_1_0_s
{
  /* We must write time label separately from other fields.
     If it is ok to write 2 bytes into flash, can use 4 bytes header, else use
     4 bytes time label and 4 bytes for other fields. */
  zb_nvram_tl_t time_label;     /*!< Time index - not real or Zigbee time */
#ifndef ZB_NVRAM_32BIT_TL
#warning ("check maximum dataset length")
  zb_lbitfield_t data_len:11;    /*!< Record length. It equals to header length + dataset length  <= 2k - is it enough? */
  zb_bitfield_t data_set_type:5;    /*!< Dataset type. See @par zb_nvram_dataset_types_t  */
#else
  /* If must align anyway, need not use bitfields */
  zb_uint16_t data_len;    /*!< Record length. It equals to header length + dataset length  */
  zb_uint16_t data_set_type;    /*!< Dataset type. See @par zb_nvram_dataset_types_t  */
#endif /* 32 bit */

  /* FIXME: it looks like migration from old datasets-vistout-version is broken: there must be no data_set_version.
     Anyway, it is useful for one-time nvram migration of very old devices, so just ignore it now.
   */
  zb_uint16_t data_set_version;     /*!< Dataset version.
                                     * Unique per dataset: each dataset fills it with its own version. */
  zb_uint8_t reserved[2];  /*!< Alignment, reserved for future use. */
} ZB_PACKED_STRUCT
zb_nvram_dataset_hdr_1_0_t;

/**
 * @name NVRAM transaction state
 * @anchor nvram_transaction_state
 *
 * Note: These values were members of `enum zb_nvram_transaction_state_e` type but were converted to
 * a set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_NVRAM_TRANSACTION_AUTOCOMMIT 0U
#define ZB_NVRAM_TRANSACTION_ONGOING    1U
/** @} */

/**
   Data set header in nvram segment
 */
typedef ZB_PACKED_PRE struct zb_nvram_dataset_hdr_7_0_s
{
  /* We must write time label separately from other fields.
     If it is ok to write 2 bytes into flash, can use 4 bytes header, else use
     4 bytes time label and 4 bytes for other fields. */
  /* Note: to simplify migration keep that field in nvram v8 (with data set trailers). */
  zb_nvram_tl_t time_label;     /*!< Time index - not real or ZigBee time */
#ifndef ZB_NVRAM_32BIT_TL
#warning ("check maximum dataset length")
  zb_lbitfield_t data_len:11;    /*!< Record length. It equal header length + dataset length  <= 2k - is it enough? */
  zb_bitfield_t data_set_type:5;    /*!< Dataset type. See @par zb_nvram_dataset_types_t  */
#else
  /* If must align anyway, need not use bitfields */
  zb_uint16_t data_len;    /*!< Record length. It equal header length + dataset length  */
  zb_uint16_t data_set_type;    /*!< Dataset type. See @par zb_nvram_dataset_types_t  */
#endif /* 32 bit */
  zb_uint16_t data_set_version; /*!< Dataset version. Unique per each
                                 * dataset  */
  zb_uint8_t transaction_status; /*!< Current transaction status. @see @ref nvram_transaction_state  */
  zb_uint8_t reserved;       /*!< Alignment. Reserved for future
                                   use  */
} ZB_PACKED_STRUCT
zb_nvram_dataset_hdr_7_0_t;


/**
   Data set trailer
 */
typedef ZB_PACKED_PRE struct zb_nvram_dataset_tail_8_0_s
{
  zb_nvram_tl_t time_label;     /*!< Time index - not real or ZigBee time - kind of TSN */
} ZB_PACKED_STRUCT
zb_nvram_dataset_tail_8_0_t;

/* TODO: implement migration tail_8_0 -> tail_9_0 */
/**
   Data set trailer
 */
typedef ZB_PACKED_PRE struct zb_nvram_dataset_tail_9_0_s
{
  zb_nvram_tl_t time_label;     /*!< Time index - not real or ZigBee time - kind of TSN */
  zb_uint16_t   crc;            /*!< CRC16 field for validate dataset integrity */
  zb_uint16_t   reserved;       /*!< Reserved field for future needs */
} ZB_PACKED_STRUCT
zb_nvram_dataset_tail_9_0_t;

typedef zb_nvram_dataset_tail_9_0_t zb_nvram_dataset_tail_t;

/* Current dataset */
#define SIZE_DS_HEADER_1_0   sizeof(zb_nvram_dataset_hdr_1_0_t)
#define PAGE_HEADER_SIZE_1_0 (SIZE_DS_HEADER_1_0 + sizeof(zb_nvram_page_hdr_dataset_t))

typedef zb_nvram_dataset_hdr_7_0_t zb_nvram_dataset_hdr_t;

#define SIZE_DS_HEADER     sizeof(zb_nvram_dataset_hdr_t)
#define PAGE_HEADER_SIZE (SIZE_DS_HEADER + sizeof(zb_nvram_page_hdr_dataset_t) + sizeof(zb_nvram_dataset_tail_t))

#ifndef ZB_NVRAM_ERASE_TO_ZERO

#ifndef ZB_NVRAM_32BIT_TL
#define ZB_NVRAM_DATA_LEN_RESERVED 0x7ffU
#define ZB_NVRAM_DATA_SET_TYPE_RESERVED 0x1fU
#else
#define ZB_NVRAM_DATA_LEN_RESERVED 0xffffU
#define ZB_NVRAM_DATA_SET_TYPE_RESERVED 0xffffU
#endif

#define ZB_NVRAM_DATA_SET_VERSION_RESERVED 0xffffU

#else

#define ZB_NVRAM_DATA_LEN_RESERVED 0U
#define ZB_NVRAM_DATA_SET_TYPE_RESERVED 0U

#define ZB_NVRAM_DATA_SET_VERSION_RESERVED 0U

#endif  /* erase to zero */

#define ZB_NVRAM_DATA_SET_VERSION_NOT_AVAILABLE 0xffffU

/**
   Return 1 if data set header is empty.

   No more data in the nvram segment after empty header. If empty header is at
   nvram segment begin, entire segment is empty. It usually means segment is
   just erased.
 */

#define ZB_NVRAM_DATA_SET_HDR_IS_EMPTY(hdrp) (                           \
    (hdrp)->time_label == ZB_NVRAM_TL_RESERVED                  \
    && (hdrp)->data_len == ZB_NVRAM_DATA_LEN_RESERVED           \
    && (hdrp)->data_set_type == ZB_NVRAM_DATA_SET_TYPE_RESERVED \
    && (hdrp)->data_set_version == ZB_NVRAM_DATA_SET_VERSION_RESERVED \
    )

#define ZB_NVRAM_DATA_SET_HDR_1_0_IS_EMPTY(hdrp) (                      \
    (hdrp)->time_label == ZB_NVRAM_TL_RESERVED                          \
    && (hdrp)->data_len == ZB_NVRAM_DATA_LEN_RESERVED                   \
    && (hdrp)->data_set_type == ZB_NVRAM_DATA_SET_TYPE_RESERVED         \
    )

typedef struct zb_nvram_position_s
{
  zb_nvram_tl_t time_label;
  zb_lbitfield_t pos:30;
  zb_lbitfield_t page:2;
  zb_uint16_t payload_length;
  zb_uint16_t version;
} zb_nvram_position_t;


/**
 * @name NVRAM versioning
 * @anchor nvram_versions
 *
 * Format of stored datasets in NVRAM has a version. It is called NVRAM version.
 * The version is common for all datasets. If structure of some dataset
 * is changed, then the version should be updated (incremented).
 *
 * Current NVRAM version is determined at compile time and almost all functions
 * should not bother about NVRAM version in use. The main problem is to convert
 * datasets stored in older format to current datasets format.
 *
 * It is responsibility of specific datasets read functions like
 * \ref zb_nvram_read_common_dataset, \ref zb_nvram_read_ha_dataset, etc.
 * Version number and length of dataset to be read are passed to these
 * functions. They should analyze them in order to convert old dataset
 * representation to the new one. The only exception is user application
 * dataset read function, where no version is provided. Application should
 * use own dataset versioning scheme if necessary.
 *
 * The easiest change in format to handle is adding fields to the end.
 * In that case dataset read function can analyze only dataset length parameter
 * and read data of this length from nvram to dataset of current version.
 * All new fields should be set to some default values in this case.
 *
 * NVRAM version is page-wide. Datasets with different versions can't be mixed
 * on one page. Version is stored in separate dataset \ref zb_nvram_page_hdr_dataset_t.
 * This dataset is always stored as first dataset on page and is written
 * after page erase procedure.
 *
 * Since different dataset versions can't be mixed on one page, migration of
 * datasets is needed if attempt to write to old version page was made.
 * This is done in \ref migrate_curr_page_to_new_version.
 *
 * Note: These values were members of `enum zb_nvram_versions_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
/* First version should be 0. New versions should be added to the end, before ZB_NVRAM_VER_COUNT */
#define ZB_NVRAM_VER_1_0  0U
#define ZB_NVRAM_VER_2_0  1U
#define ZB_NVRAM_VER_3_0  2U
#define ZB_NVRAM_VER_4_0  3U
#define ZB_NVRAM_VER_5_0  4U
#define ZB_NVRAM_VER_6_0  5U
#define ZB_NVRAM_VER_7_0  6U /* Used for ZBOSS SDK: stub for the next versions */
#define ZB_NVRAM_VER_8_0  7U /* Used before introducing dataset tails */
#define ZB_NVRAM_VER_9_0  8U /* Version of NVRAM having additional dataset trailers */
#define ZB_NVRAM_VER_10_0 9U /* Version with unused CRC field in tail */
#define ZB_NVRAM_VER_11_0 10U /* Version with used CRC field in tail */
#define ZB_NVRAM_VER_12_0 11U /* Version with guarantee that all dataset sizes are forcefully aligned to 4 */
#define ZB_NVRAM_LAST_VER ZB_NVRAM_VER_12_0 /* Should always be equal to the highest version */
/*------------------*/
#define ZB_NVRAM_VER_COUNT (ZB_NVRAM_LAST_VER + 1U)
/** @} */


#define ZB_MIN_NVRAM_VER_WITH_DS_TRAILERS ZB_NVRAM_VER_9_0
#define ZB_MIN_NVRAM_VER_WITH_DS_CRC_IN_TAIL ZB_NVRAM_VER_11_0

/* Initnial value for crc calculation */
#define ZB_NVRAM_CRC_DEFAULT_VALUE 0xFFFFU

typedef zb_uint16_t zb_nvram_ver_t;

#define ZB_NVRAM_MAX_VERSION (ZB_NVRAM_VER_COUNT - 1U)
/* Always assume that current NVRAM version in use is the last version.
 * That means support of reading NVRAM datasets in older versions,
 * but write datasets only in newest format */
#define ZB_NVRAM_VERSION ZB_NVRAM_MAX_VERSION

/**
   Return 1 if data set type is valid.
 */
#define ZB_NVRAM_DATA_SET_HDR_IS_VALID(hdrp) (  \
    (hdrp)->data_set_type < (zb_uint16_t)ZB_NVRAM_DATASET_NUMBER \
    )

typedef struct zb_nvram_migration_info_s
{
  zb_uint32_t src_pos;
  zb_uint32_t dst_pos;
  zb_uint8_t src_page;
  zb_uint8_t dst_page;
  zb_uint8_t length;
  zb_nvram_ver_t nvram_ver;
}
zb_nvram_migration_info_t;


/**
 * Common NVRAM dataset. See spec 2.2.8, 3.6.8, 4.3.3, 4.4.10
 */
typedef ZB_PACKED_PRE struct zb_nvram_dataset_common_ver_1_0_s
{
  zb_bitfield_t   aps_designated_coordinator : 1; /*!< This boolean flag indicates whether the
                                              device should assume on startup that it must
                                              become a Zigbee coordinator.  */
  zb_bitfield_t   aps_insecure_join : 1; /*!< A boolean flag, which defaults to TRUE and
                                           indicates whether it is OK to use insecure
                                           join on startup.  */
  zb_bitfield_t   stack_profile:4;            /*!< Stack profile identifier  */
  zb_bitfield_t   use_tc_alternative_key:1;   /*!< if 1, use alternative TC key */
  zb_bitfield_t   reserved:1;
  zb_uint8_t      depth;                     /*!< current node depth */
  zb_uint16_t     nwk_manager_addr;             /*!< NWK manager address */
  zb_uint16_t     panId;                        /*!< Pan ID */
  zb_uint16_t     network_address;              /*!< Current network address */

  zb_uint32_t     aps_channel_mask;     /*!< This is the mask containing allowable
                                           channels on which the device may attempt
                                           to form or join a network at startup time.  */
  zb_ext_pan_id_t aps_use_extended_pan_id;
  zb_ext_pan_id_t nwk_extended_pan_id;  /*!< Extended Pan ID for the PAN of which the device is a member */
  zb_ieee_addr_t parent_address;            /*!< Parent address */
  zb_ieee_addr_t trust_center_address;      /*!< Trust Center IEEE address */
  zb_uint8_t nwk_key[ZB_CCM_KEY_SIZE];      /*!< Network Key */
  zb_uint8_t nwk_key_seq;
  zb_uint8_t tc_standard_key[ZB_CCM_KEY_SIZE];      /*!< Trust Center Standard Key */
  zb_uint8_t tc_alternative_key[ZB_CCM_KEY_SIZE];   /*!< Trust Center Alternative Key */

  /* Custom fields*/
  zb_uint8_t channel;                       /*!< Current channel. Custom field */
  zb_uint8_t aligned[2];

} ZB_PACKED_STRUCT
zb_nvram_dataset_common_ver_1_0_t;

typedef ZB_PACKED_PRE struct zb_nvram_dataset_common_ver_2_0_s
{
  zb_bitfield_t   aps_designated_coordinator : 1; /*!< This boolean flag indicates whether the
                                              device should assume on startup that it must
                                              become a Zigbee coordinator.  */
  zb_bitfield_t   aps_insecure_join : 1; /*!< A boolean flag, which defaults to TRUE and
                                           indicates whether it is OK to use insecure
                                           join on startup.  */
  zb_bitfield_t   stack_profile:4;            /*!< Stack profile identifier  */
  zb_bitfield_t   reserved:2;
  zb_uint8_t      depth;                     /*!< current node depth */
  zb_uint16_t     nwk_manager_addr;             /*!< NWK manager address */
  zb_uint16_t     panId;                        /*!< Pan ID */
  zb_uint16_t     network_address;              /*!< Current network address */

  zb_uint32_t     aps_channel_mask;     /*!< This is the mask containing allowable
                                           channels on which the device may attempt
                                           to form or join a network at startup time.  */
  zb_ext_pan_id_t aps_use_extended_pan_id;
  zb_ext_pan_id_t nwk_extended_pan_id;  /*!< Extended Pan ID for the PAN of which the device is a member */
  zb_ieee_addr_t parent_address;            /*!< Parent address */
  zb_ieee_addr_t trust_center_address;      /*!< Trust Center IEEE address */
  zb_uint8_t nwk_key[ZB_CCM_KEY_SIZE];      /*!< Network Key */
  zb_uint8_t nwk_key_seq;
  zb_uint8_t tc_standard_key[ZB_CCM_KEY_SIZE];      /*!< Trust Center Standard Key */

  /* Custom fields*/
  zb_uint8_t channel;                       /*!< Current channel. Custom field */
  zb_uint8_t aligned[2];

} ZB_PACKED_STRUCT
zb_nvram_dataset_common_ver_2_0_t;


typedef ZB_PACKED_PRE struct zb_nvram_dataset_common_ver_4_0_s
{
  zb_bitfield_t   aps_designated_coordinator : 1; /*!< This boolean flag indicates whether the
                                              device should assume on startup that it must
                                              become a Zigbee coordinator.  */
  zb_bitfield_t   aps_insecure_join : 1; /*!< A boolean flag, which defaults to TRUE and
                                           indicates whether it is OK to use insecure
                                           join on startup.  */
  zb_bitfield_t   stack_profile:4;            /*!< Stack profile identifier  */
  /*zb_bitfield_t   reserved:2;*/
  zb_bitfield_t   device_type:2;    /*!< NIB device type */
  zb_uint8_t      depth;                     /*!< current node depth */
  zb_uint16_t     nwk_manager_addr;             /*!< NWK manager address */
  zb_uint16_t     panId;                        /*!< Pan ID */
  zb_uint16_t     network_address;              /*!< Current network address */

  zb_channel_list_t aps_channel_mask_list; /*!< This is the masks list containing allowable
                                            * channels on which the device may attempt
                                            * to form or join a network at startup time. */
  zb_ext_pan_id_t aps_use_extended_pan_id;
  zb_ext_pan_id_t nwk_extended_pan_id;  /*!< The extended PAN identifier for the PAN of which the device is a member */
  zb_ieee_addr_t parent_address;            /*!< Parent address */
  zb_ieee_addr_t trust_center_address;      /*!< Trust Center IEEE address */
  zb_uint8_t nwk_key[ZB_CCM_KEY_SIZE];      /*!< Network Key */
  zb_uint8_t nwk_key_seq;
  zb_uint8_t tc_standard_key[ZB_CCM_KEY_SIZE];      /*!< Trust Center Standard Key */

  /* Custom fields*/
  /* In default configuration ZB_NWK_MAC_IFACE_TBL_SIZE is 1, so the dataset is compatible with previous stack versions.
   * When multple interfaces are enabled, the dataset will become incompatible with default configuration and
   * migrations between single-MAC and multi-MAC NVRAM are not supported now */
  zb_uint8_t channel[ZB_NWK_MAC_IFACE_TBL_SIZE]; /*!< Current channel. Custom field */
  zb_uint8_t page[ZB_NWK_MAC_IFACE_TBL_SIZE];    /*!< Current page. Custom field */
  zb_nwk_mac_iface_tbl_ent_t mac_iface_tbl[ZB_NWK_MAC_IFACE_TBL_SIZE]; /*!< nwkMacInterfaceTable from NWK NIB()*/
  /* There was 1 reserved byte */
  zb_bitfield_t   hub_connectivity:1; /*!< gub connectivity for WWAH and r23 all hubs  */
  zb_bitfield_t   rx_on:1;            /*!< rx-on-when-idle for ZED  */
  zb_bitfield_t   tc_swapped:1;     /*!< TC swapout is in progress for ZC (not
                                     * all known devices came back) / Joiner
                                     * detected TC swapout, not all precessing
                                     * completed. */
  zb_bitfield_t   reserved:5;
#if ZB_NWK_MAC_IFACE_TBL_SIZE == 1
#elif ZB_NWK_MAC_IFACE_TBL_SIZE == 2
  /* sizeof(zb_nvram_dataset_common_ver_3_0_t) == 106 (without additional reserved bytes) */
  zb_uint8_t mm_reserved[2];
#else
  #error Current Multi-MAC configuration is not supported
#endif

} ZB_PACKED_STRUCT
zb_nvram_dataset_common_ver_4_0_t;

/* The new dataset does not introduce new fields, but renames the previously reserved fields,
 * so we can use the new dataset definition while loading legacy structure.
 */
typedef zb_nvram_dataset_common_ver_4_0_t zb_nvram_dataset_common_ver_3_0_t;

typedef zb_nvram_dataset_common_ver_4_0_t zb_nvram_dataset_common_t;

/**
 * @name NVRAM dataset common versions
 * @anchor nvram_dataset_common_versions
 *
 * Note: These values were members of `enum zb_nvram_dataset_common_versions_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_NVRAM_COMMON_DATA_DS_VER_1 0U
#define ZB_NVRAM_COMMON_DATA_DS_VER_2 1U
#define ZB_NVRAM_COMMON_DATA_DS_VER_3 2U
#define ZB_NVRAM_COMMON_DATA_DS_VER_4 3U
/** @} */

#define ZB_NVRAM_COMMON_DATA_DS_VER ZB_NVRAM_COMMON_DATA_DS_VER_4


/* Check dataset alignment for IAR compiler and ARM Cortex target platfrom */
ZB_ASSERT_IF_NOT_ALIGNED_TO_4(zb_nvram_dataset_common_t);

#ifdef ZB_STORE_COUNTERS

typedef ZB_PACKED_PRE struct zb_nvram_dataset_counters_ver_1_0_s
{
  zb_uint32_t nib_counter;
} ZB_PACKED_STRUCT
zb_nvram_dataset_counters_ver_1_0_t;

typedef ZB_PACKED_PRE struct zb_nvram_dataset_counters_ver_2_0_s
{
  zb_uint32_t nib_counter;
  zb_uint32_t aib_counter;
} ZB_PACKED_STRUCT
zb_nvram_dataset_counters_ver_2_0_t;

/**
 * @name NVRAM dataset counters versions
 * @anchor nvram_dataset_binding_versions
 */
/** @{ */
#define ZB_IB_COUNTERS_DS_VER_1 0U
#define ZB_IB_COUNTERS_DS_VER_2 1U
/** @} */

typedef zb_nvram_dataset_counters_ver_2_0_t zb_nvram_dataset_counters_t;
#define ZB_IB_COUNTERS_DS_VER ZB_IB_COUNTERS_DS_VER_2

/* Check dataset alignment for IAR compiler and ARM Cortex target platfrom */
ZB_ASSERT_IF_NOT_ALIGNED_TO_4(zb_nvram_dataset_counters_t);

#endif

#if defined ZB_ENABLE_HA

/**
 * HA NVRAM dataset. See HA spec IAS Zone Cluster
 */
typedef ZB_PACKED_PRE struct zb_nvram_dataset_ha_s
{
  /* IAS Zone data */
  zb_ieee_addr_t cie_address;   /*!< CIE address */

  zb_uint8_t cie_ep;            /*!< CIE EP used for notifications*/
  zb_uint8_t zone_state;        /*!< Zone State */
  zb_uint16_t cie_short_addr;   /*!< CIE short address used for notifications*/

  zb_uint8_t zone_id;           /*!< ZoneID value */
  /* Poll control data */
  zb_uint8_t poll_ctrl_ep; /*!< poll control client endpoint */
  zb_uint16_t poll_ctrl_short_addr; /*!< poll control client short addr*/

  /* Reserved data */
  zb_uint8_t reserved; /* Reserved */

  /*Diagnostic cluster data */
  zb_uint16_t number_of_resets; /*Number of resets*/
  zb_uint16_t join_indication; /*Number of join attempts*/
  zb_uint16_t packet_buffer_allocate_failures; /*Number of buffer allocation errors*/

  zb_uint8_t   cie_address_is_set;  /*!< Is CIE address set */
} ZB_PACKED_STRUCT
zb_nvram_dataset_ha_t;

typedef enum zb_nvram_dataset_ha_versions_e
{
  ZB_NVRAM_HA_DATA_DS_VER_1 = 0,
}
zb_nvram_dataset_ha_versions_t;

#define ZB_NVRAM_HA_DATA_DS_VER ZB_NVRAM_HA_DATA_DS_VER_1

/* Check dataset alignment for IAR compiler and ARM Cortex target platfrom */
ZB_ASSERT_IF_NOT_ALIGNED_TO_4(zb_nvram_dataset_ha_t);

#endif /*defined ZB_ENABLE_HA*/

#ifdef ZDO_DIAGNOSTICS

/**
 * Dataset of the ZDO Diagnostics.
 */
typedef ZB_PACKED_PRE struct zb_nvram_dataset_diagnostics_v1_s
{
  /*! @brief An attribute that is incremented
   *         each time the device resets. */
  zb_uint16_t number_of_resets;

  /*! @brief A counter that is incremented each time
   *  an entry is added to the neighbor table. */
  zb_uint16_t nwk_neighbor_added;

  /*! @brief A counter that is incremented each time
   *  an entry is removed from the neighbor table. */
  zb_uint16_t nwk_neighbor_removed;

  /*! @brief A counter that is incremented each time a neighbor table entry
   *  becomes stale because the neighbor has not been heard from. */
  zb_uint16_t nwk_neighbor_stale;

  /*! @brief A counter that is incremented each time
   *         a node joins or rejoins the network via this node. */
  zb_uint16_t join_indication;

  /*! @brief A counter that is incremented each time an entry
   *  is removed from the child table. */
  zb_uint16_t childs_removed;

  /*! @brief A counter that is incremented each time
   *         the stack failed to allocate a packet buffers. */
  zb_uint16_t packet_buffer_allocate_failures;

  zb_uint8_t alignment[2]; /* total size == 16 bytes */
} ZB_PACKED_STRUCT
zb_nvram_dataset_diagnostics_v1_t;

/**
 * @name NVRAM dataset diagnostics versions
 * @anchor nvram_dataset_diagnostics_versions
 */
/** @{ */
#define ZB_NVRAM_DIAGNOSTICS_DATA_DS_VER_1 0U
/** @} */

#define ZB_NVRAM_DIAGNOSTICS_DATA_DS_VER ZB_NVRAM_DIAGNOSTICS_DATA_DS_VER_1

/* Check dataset alignment for IAR compiler and ARM Cortex target platfrom */
ZB_ASSERT_IF_NOT_ALIGNED_TO_4(zb_nvram_dataset_diagnostics_v1_t);

#endif /* ZDO_DIAGNOSTICS */

#ifdef ZB_CONFIGURABLE_MEM

#define ZB_NVRAM_APS_BINDING_TABLE_SPECIFIC_MIGRATION

#ifndef ZB_ED_ROLE
#define ZB_APS_SRC_BINDING_TABLE_SIZE_OLD 16U
#define ZB_APS_DST_BINDING_TABLE_SIZE_OLD 32U
#else
#define ZB_APS_SRC_BINDING_TABLE_SIZE_OLD 8U
#define ZB_APS_DST_BINDING_TABLE_SIZE_OLD 16U
#endif  /* ZB_ED_ROLE */

#else /* defined ZB_CONFIGURABLE_MEM */

#define ZB_APS_SRC_BINDING_TABLE_SIZE_OLD ZB_APS_SRC_BINDING_TABLE_SIZE
#define ZB_APS_DST_BINDING_TABLE_SIZE_OLD ZB_APS_DST_BINDING_TABLE_SIZE

#define ZB_SINGLE_TRANS_INDEX_SIZE_OLD ((ZB_APS_BIND_TRANS_TABLE_SIZE + 7U) / 8U)

#endif /* defined ZB_CONFIGURABLE_MEM */

typedef ZB_PACKED_PRE struct zb_aps_bind_dst_table_old_s
{
  ZB_PACKED_PRE union
  {
    zb_uint16_t group_addr;                /*!< group address */
    zb_aps_bind_long_dst_addr_t long_addr; /*!< @see zb_asp_long_dst_addr_t */
  } u;
#ifndef ZB_CONFIGURABLE_MEM
  zb_uint8_t            trans_index[ZB_SINGLE_TRANS_INDEX_SIZE_OLD];
#else
  zb_uint8_t            *trans_index;
#endif

  zb_bitfield_t         dst_addr_mode:3;   /*!< destination address mode flag, 0
                                            * - group address, otherwise long
                                            * address plus dest endpoint */
  zb_bitfield_t         src_table_index:5; /*!< index from zb_asp_src_table_t */
} ZB_PACKED_STRUCT zb_aps_bind_dst_table_old_t;

typedef ZB_PACKED_PRE struct zb_aps_binding_table_old_s
{
  zb_aps_bind_src_table_t src_table[ZB_APS_SRC_BINDING_TABLE_SIZE_OLD]; /*!< Source table */
  zb_aps_bind_dst_table_old_t dst_table[ZB_APS_DST_BINDING_TABLE_SIZE_OLD]; /*!< Destination table */
  zb_uint8_t              src_n_elements;                               /*!< Count elements in source table */
  zb_uint8_t              dst_n_elements;                               /*!< Count elements in destination table */
#ifdef SNCP_MODE
  zb_uint8_t              remote_bind_offset;                           /*!< Offset to attribute id's to remote binding requests */
  zb_uint8_t              align;
#else
  /* FIXME: why align here? */
  zb_uint8_t              align[2];
#endif
#ifndef ZB_CONFIGURABLE_MEM
  zb_uint8_t              trans_table[ZB_APS_BIND_TRANS_TABLE_SIZE];    /*!< Buffers for simultaneous sendings */
#else
  zb_uint8_t              *trans_table;
#endif
} ZB_PACKED_STRUCT zb_aps_binding_table_old_t;

typedef zb_aps_binding_table_old_t zb_nvram_dataset_binding_v1_t;

typedef ZB_PACKED_PRE struct zb_aps_bind_dst_table_v2_s
{
#ifdef ZB_CONFIGURABLE_MEM
  /* WARNING: this field will be rewritten if APS binding dataset is present in NVRAM */
  zb_uint8_t            *trans_index;
#endif /* defined ZB_CONFIGURABLE_MEM */

  ZB_PACKED_PRE union
  {
    zb_uint16_t group_addr;                /*!< group address */
    zb_aps_bind_long_dst_addr_t long_addr; /*!< @see zb_asp_long_dst_addr_t */
  } u;

#ifndef ZB_CONFIGURABLE_MEM
  zb_uint8_t            trans_index[ZB_SINGLE_TRANS_INDEX_SIZE];
#endif /* defined ZB_CONFIGURABLE_MEM */
  zb_uint8_t            align;

  zb_bitfield_t         dst_addr_mode:3;   /*!< destination address mode flag, 0
                                            * - group address, otherwise long
                                            * address plus dest endpoint */
  zb_bitfield_t         src_table_index:5; /*!< index from zb_asp_src_table_t */
} ZB_PACKED_STRUCT zb_aps_bind_dst_table_v2_t;

/**
 * @brief APS binding NVRAM dataset.
 */
typedef ZB_PACKED_PRE struct zb_nvram_dataset_binding_v3_s
{
  zb_uint8_t src_n_elements;  /*!< Count elements in source table */
  zb_uint8_t dst_n_elements;  /*!< Count elements in destination table */
#ifdef SNCP_MODE
  zb_uint8_t remote_bind_offset;  /*!< Offset to attribute id's to remote binding requests */
  zb_uint8_t align;
#else
  zb_uint8_t align[2];
#endif
} ZB_PACKED_STRUCT
zb_nvram_dataset_binding_v3_t;

/* The new dataset header does not introduce new fields, but renames the previously reserved fields,
 * so we can use the new dataset header definition while loading legacy structure.
 * The binding dataset contents have changed, because the zb_aps_bind_dst_table_t has a different size.
 * The v2 loading function uses the zb_aps_bind_dst_table_v2_t type instead.
 */
typedef zb_nvram_dataset_binding_v3_t zb_nvram_dataset_binding_v2_t;

typedef zb_nvram_dataset_binding_v3_t zb_nvram_dataset_binding_t;

/**
 * @name NVRAM dataset binding versions
 * @anchor nvram_dataset_binding_versions
 *
 * Note: These values were members of `enum zb_nvram_dataset_binding_versions_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_NVRAM_APS_BINDING_DATA_DS_VER_1 0U
#define ZB_NVRAM_APS_BINDING_DATA_DS_VER_2 1U
#define ZB_NVRAM_APS_BINDING_DATA_DS_VER_3 2U
/** @} */

#define ZB_NVRAM_APS_BINDING_DATA_DS_VER ZB_NVRAM_APS_BINDING_DATA_DS_VER_3

/* Check dataset alignment for IAR compiler and ARM Cortex target platfrom */
ZB_ASSERT_IF_NOT_ALIGNED_TO_4(zb_nvram_dataset_binding_v3_t);

typedef ZB_PACKED_PRE struct zb_nvram_dataset_groups_hdr_s
{
  zb_uint8_t n_groups;                        /*!< # of entries in APS group table */
  zb_uint8_t aligned[3];
} ZB_PACKED_STRUCT
zb_nvram_dataset_groups_hdr_t;

ZB_ASSERT_IF_NOT_ALIGNED_TO_4(zb_nvram_dataset_groups_hdr_t);
ZB_ASSERT_IF_NOT_ALIGNED_TO_4(zb_aps_group_table_ent_t);

/**
 * @name NVRAM dataset groups versions
 * @anchor nvram_dataset_groups_versions
 *
 * Note: These values were members of `enum zb_nvram_dataset_groups_versions_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_NVRAM_APS_GROUPS_DATA_DS_VER_1 0U
/** @} */

#define ZB_NVRAM_APS_GROUPS_DATA_DS_VER ZB_NVRAM_APS_GROUPS_DATA_DS_VER_1


#if defined ZB_HA_ENABLE_POLL_CONTROL_SERVER || defined DOXYGEN
/**
 * HA POLL CONTROL NVRAM dataset.
 */
typedef ZB_PACKED_PRE struct zb_nvram_dataset_poll_control_s
{
  zb_uint32_t checkin_interval;
  zb_uint32_t long_poll_interval;
  zb_uint16_t short_poll_interval;
  zb_uint16_t fast_poll_timeout;
  zb_uint32_t checkin_interval_min;
  zb_uint32_t long_poll_interval_min;
  zb_uint16_t fast_poll_timeout_max;
  zb_uint8_t  aligned[2];
} ZB_PACKED_STRUCT
zb_nvram_dataset_poll_control_t;

/* Check dataset alignment for IAR compiler and ARM Cortex target platfrom */
ZB_ASSERT_IF_NOT_ALIGNED_TO_4(zb_nvram_dataset_poll_control_t);

typedef enum zb_nvram_dataset_poll_control_versions_e
{
  ZB_NVRAM_HA_POLL_CONTROL_DATA_DS_VER_1 = 0,
}
zb_nvram_dataset_poll_control_versions_t;

#define ZB_NVRAM_HA_POLL_CONTROL_DATA_DS_VER ZB_NVRAM_HA_POLL_CONTROL_DATA_DS_VER_1

#endif /* defined ZB_HA_ENABLE_POLL_CONTROL_SERVER || defined DOXYGEN */

#if defined ZB_ZCL_SUPPORT_CLUSTER_WWAH || defined DOXYGEN
/**
 * WWAH NVRAM dataset.
 */
typedef ZB_PACKED_PRE struct zb_nvram_dataset_wwah_s
{
  /* >>> WWAH CTX TODO: clarify what is needed to store */
  zb_uint8_t wwah_behavior;
  zb_uint16_t time_server_addr;
  zb_uint8_t time_server_endpoint;

  zb_uint8_t aps_ack_exempt_table_cnt;
  zb_uint8_t aps_link_key_enabled_by_default;

  zb_uint8_t aps_link_key_authorization_table_cnt;
  zb_uint8_t use_trust_center_for_cluster_table_cnt;
  zb_uint8_t reserved;          /* there was classification mask - no longer needed, may be ignored while loading old datasets. */
  zb_zcl_wwah_periodic_checkins_data_t periodic_checkins;
  /* <<< WWAH CTX */

  /* >>> WWAH attributes */
  zb_uint16_t cluster_revision;
  zb_uint8_t nwk_retry_count;
  zb_uint8_t mac_retry_count;
  zb_uint8_t wwah_app_event_retry_queue_size;
  zb_uint8_t mac_poll_failure_wait_time;
  zb_uint8_t current_debug_report_id;
  zb_uint8_t pending_network_update_channel;
  zb_uint16_t pending_network_update_panid;
  zb_uint16_t ota_max_offline_duration; /* 79b */

  zb_bitfield_t disable_ota_downgrades:1;
  zb_bitfield_t mgmt_leave_without_rejoin_enabled:1;
  zb_bitfield_t router_check_in_enabled:1;
  zb_bitfield_t touchlink_interpan_enabled:1;
  zb_bitfield_t wwah_parent_classification_enabled:1;
  zb_bitfield_t wwah_app_event_retry_enabled:1;
  zb_bitfield_t wwah_rejoin_enabled:1;
  zb_bitfield_t configuration_mode_enabled:1;
  zb_bitfield_t tc_security_on_nwk_key_rotation_enabled:1;
  zb_bitfield_t wwah_bad_parent_recovery_enabled:1;
  zb_bitfield_t align_bitfield:6;
  /* <<< WWAH attributes */

  zb_uint8_t align_struct[1];
} ZB_PACKED_STRUCT
zb_nvram_dataset_wwah_t;

ZB_ASSERT_IF_NOT_ALIGNED_TO_4(zb_nvram_dataset_wwah_t);

typedef enum zb_nvram_dataset_wwah_versions_e
{
  ZB_NVRAM_ZCL_WWAH_DATA_DS_VER_1 = 0,
}
zb_nvram_dataset_wwah_versions_t;

#define ZB_NVRAM_ZCL_WWAH_DATA_DS_VER ZB_NVRAM_ZCL_WWAH_DATA_DS_VER_1

#endif /* defined ZB_ZCL_SUPPORT_CLUSTER_WWAH || defined DOXYGEN */

/****** GrPw dataset *****/

#ifdef ZB_ENABLE_ZGP

typedef ZB_PACKED_PRE struct zb_nvram_zgp_transl_tbl_entry_ver_1_0_s
{
  zb_ieee_addr_t         zgpd_id;
  zb_uint16_t            Zigbee_profile_id;
  zb_uint16_t            cluster_id;

  zb_uint8_t             Zigbee_cmd_id;
  zb_uint8_t             options;
  zb_uint8_t             zgpd_cmd_id;
  zb_uint8_t             endpoint;

  zb_uint8_t             payload_len;
  zb_uint8_t             payload_data[ZB_ZGP_TRANSL_CMD_PLD_MAX_SIZE];
  zb_uint8_t             aligned[2];
}
ZB_PACKED_STRUCT
zb_nvram_zgp_transl_tbl_entry_ver_1_0_t;

typedef zb_nvram_zgp_transl_tbl_entry_ver_1_0_t zb_nvram_zgp_transl_tbl_t;

typedef ZB_PACKED_PRE struct zb_nvram_zgp_sink_tbl_entry_ver_1_0_s
{
  zb_ieee_addr_t   zgpd_id;

  zb_uint16_t      options;
  zb_uint16_t      zgpd_assigned_alias;
  zb_uint32_t      zgpd_sec_frame_counter;

  zb_uint8_t       device_id;
  zb_uint8_t       groupcast_radius;
  zb_uint8_t       sec_options;
  zb_uint8_t       zgpd_key[ZB_CCM_KEY_SIZE];
  zb_uint8_t       dup_counter_expired;
}
ZB_PACKED_STRUCT
zb_nvram_zgp_sink_tbl_entry_ver_1_0_t;

/* Check dataset alignment for IAR compiler and ARM Cortex target platfrom */
ZB_ASSERT_IF_NOT_ALIGNED_TO_4(zb_nvram_zgp_sink_tbl_entry_ver_1_0_t);

typedef ZB_PACKED_PRE struct zb_nvram_zgp_sink_tbl_entry_ver_6_0_s
{
  zb_uint32_t      src_id;
  zb_ieee_addr_t   ieee_addr;

  zb_uint32_t      zgpd_sec_frame_counter;

/**
 * Options field of sink table entry
 *
 * [0-2]        app_id          ZGPD Application ID
 * [3]          sn_cap          Sequence number capability
 * [4]          sn_expired      Sequence number expired
 * [5]          is_manuf_id     Is manufactured device id
 * [6]          is_used_entry   Is used entry
 * [7-8]        sec_lvl         Security level
 * [9-11]       sec_key_type    Security key type
 *
 */
  zb_uint16_t      options;

  zb_uint8_t       match_dev_tbl_idx;
  zb_uint8_t       zgpd_key[ZB_CCM_KEY_SIZE];
  zb_uint8_t       aligned[1];
}
ZB_PACKED_STRUCT
zb_nvram_zgp_sink_tbl_entry_ver_6_0_t;

/* Check dataset alignment for IAR compiler and ARM Cortex target platfrom */
ZB_ASSERT_IF_NOT_ALIGNED_TO_4(zb_nvram_zgp_sink_tbl_entry_ver_6_0_t);

typedef zb_nvram_zgp_sink_tbl_entry_ver_6_0_t zb_nvram_zgp_sink_tbl_t;

/* Format of old ZGP dataset used before ZB_NVRAM_VER_2_0 */
#define ZB_NVRAM_ZGP_TRANSL_TBL_SIZE_1_0  64U
#define ZB_NVRAM_ZGP_SINK_TBL_SIZE_1_0    32U
typedef ZB_PACKED_PRE struct zb_nvram_zgp_dataset_ver_1_0_s
{
  zb_nvram_zgp_transl_tbl_t zgp_translate_tbl[ZB_NVRAM_ZGP_TRANSL_TBL_SIZE_1_0];
  zb_nvram_zgp_sink_tbl_t   zgp_sink_tbl[ZB_NVRAM_ZGP_SINK_TBL_SIZE_1_0];
  zb_uint32_t               zgp_sink_tbl_used_entries;
}
ZB_PACKED_STRUCT
zb_nvram_zgp_dataset_ver_1_0_t;

typedef ZB_PACKED_PRE struct zb_nvram_zgp_dataset_info_ver_2_0_s
{
  zb_uint16_t               zgp_sink_tbl_size;
  zb_uint32_t               zgp_sink_tbl_used_entries;
  zb_uint16_t               zgp_transl_tbl_size;
}
ZB_PACKED_STRUCT
zb_nvram_zgp_dataset_info_ver_2_0_t;

typedef ZB_PACKED_PRE struct zb_nvram_zgp_dataset_info_ver_6_0_s
{
  zb_uint16_t               zgp_sink_tbl_size;
  zb_uint32_t               immed_tx_frame_counter;
}
ZB_PACKED_STRUCT
zb_nvram_zgp_dataset_info_ver_6_0_t;

#ifdef ZB_ENABLE_ZGP_PROXY
typedef ZB_PACKED_PRE struct zb_nvram_zgp_dataset_ver_6_0_s
{
  zb_nvram_zgp_dataset_info_ver_6_0_t zgp_info;
  zb_nvram_zgp_sink_tbl_t             zgp_sink_tbl[ZB_ZGP_SINK_TBL_SIZE];
}
ZB_PACKED_STRUCT
zb_nvram_zgp_dataset_ver_6_0_t;

typedef zb_nvram_zgp_dataset_info_ver_6_0_t zb_nvram_zgp_dataset_info_t;
typedef zb_nvram_zgp_dataset_ver_6_0_t zb_nvram_zgp_dataset_t;
#endif  /* ZB_ENABLE_ZGP_PROXY */

typedef enum zb_nvram_dataset_grpw_data_versions_e
{
  ZB_NVRAM_DATASET_GRPW_DATA_DS_VER_1 = 0,
  ZB_NVRAM_DATASET_GRPW_DATA_DS_VER_2 = 1,
  ZB_NVRAM_DATASET_GRPW_DATA_DS_VER_3 = 2,
  ZB_NVRAM_DATASET_GRPW_DATA_DS_VER_4 = 3,
  ZB_NVRAM_DATASET_GRPW_DATA_DS_VER_5 = 4,
  ZB_NVRAM_DATASET_GRPW_DATA_DS_VER_6 = 5,
}
zb_nvram_dataset_grpw_data_versions_t;

#define ZB_NVRAM_DATASET_GRPW_DATA_DS_VER ZB_NVRAM_DATASET_GRPW_DATA_DS_VER_6

typedef enum zb_nvram_dataset_gr_sinkt_versions_e
{
  ZB_NVRAM_DATASET_GP_SINKT_DS_VER_1 = 0,
}
zb_nvram_dataset_gr_sinkt_versions_t;

#define ZB_NVRAM_DATASET_GP_SINKT_DS_VER ZB_NVRAM_DATASET_GP_SINKT_DS_VER_1

typedef enum zb_nvram_dataset_gr_proxyt_versions_e
{
  ZB_NVRAM_DATASET_GP_PRPOXYT_DS_VER_1 = 0,
}
zb_nvram_dataset_gr_proxyt_versions_t;

#define ZB_NVRAM_DATASET_GP_PRPOXYT_DS_VER ZB_NVRAM_DATASET_GP_PRPOXYT_DS_VER_1

typedef enum zb_nvram_dataset_gp_cluster_versions_e
{
  ZB_NVRAM_DATASET_GP_CLUSTER_DS_VER_1 = 0,
}
zb_nvram_dataset_gp_cluster_versions_t;

#define ZB_NVRAM_DATASET_GP_CLUSTER_DS_VER ZB_NVRAM_DATASET_GP_CLUSTER_DS_VER_1

typedef enum zb_nvram_dataset_gp_app_tbl_versions_e
{
  ZB_NVRAM_DATASET_GP_APP_TBL_DS_VER_1 = 0,
}
zb_nvram_dataset_gp_app_tbl_versions_t;

#define ZB_NVRAM_DATASET_GP_APP_TBL_DS_VER ZB_NVRAM_DATASET_GP_APP_TBL_DS_VER_1

#endif  /* #ifdef ZB_ENABLE_ZGP */

/* CR: Discussion: MM: I decided to use internal dataset versioning
 * due to its dynamical size */

/**
  ZB_NVRAM_ADDR_MAP
  Address map dataset is stored in NVM using 2 structures:
  zb_nvram_addr_map_hdr_t
  Array of zb_nvram_addr_map_rec_t records

  If header store 0 as array size, no record follows
*/

/**
 * @name NVRAM address map dataset version
 * @anchor nvram_addr_map_ds_versions
 *
 * Note: These values were members of `enum zb_nvram_addr_map_ds_versions_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
/* First(0) version should be 0.
 * New versions should be added to the end, before ZB_NVRAM_ADDR_MAP_DS_VER.
 * After, ZB_NVRAM_ADDR_MAP_DS_VER should be updated. */
#define ZB_NVRAM_ADDR_MAP_DS_VER_0     0U
#define ZB_NVRAM_ADDR_MAP_DS_VER_1     1U
#define ZB_NVRAM_ADDR_MAP_DS_VER_2     2U
#define ZB_NVRAM_ADDR_MAP_DS_VER       ZB_NVRAM_ADDR_MAP_DS_VER_2 /* Should be equal to latest (highest) version */
#define ZB_NVRAM_ADDR_MAP_DS_VER_COUNT (ZB_NVRAM_ADDR_MAP_DS_VER + 1U)
/** @} */

typedef ZB_PACKED_PRE struct zb_nvram_addr_map_hdr_v0_s
{
  zb_uint8_t addr_map_num;   /*!< Stores number of addr maps - see zb_address_map_t */
  zb_uint8_t version;        /*!< Stores version of the dataset */
}
ZB_PACKED_STRUCT
zb_nvram_addr_map_hdr_v0_t;

typedef ZB_PACKED_PRE struct zb_nvram_addr_map_hdr_s
{
  zb_uint8_t addr_map_num;   /*!< Stores number of addr maps - see zb_address_map_t */
  zb_uint8_t version;        /*!< Stores version of the dataset */
  zb_uint8_t aligned[2];
}
ZB_PACKED_STRUCT
zb_nvram_addr_map_hdr_t;

/** Store a NVM record describing address map, see zb_address_map_t */
typedef ZB_PACKED_PRE struct zb_nvram_addr_map_rec_v0_s
{
  zb_ieee_addr_t  ieee_addr; /*!< Uncompressed IEEE address */
  zb_uint16_t     addr;      /*!< 16-bit device address */
  zb_uint8_t      index;     /*!< Reference for neighbor table */
}
ZB_PACKED_STRUCT
zb_nvram_addr_map_rec_v0_t;

/** Store a NVM record describing address map, see zb_address_map_t */
typedef ZB_PACKED_PRE struct zb_nvram_addr_map_rec_v1_s
{
  zb_ieee_addr_t  ieee_addr; /*!< Uncompressed IEEE address */
  zb_uint16_t     addr;      /*!< 16-bit device address */
  zb_uint8_t      index;     /*!< Reference for neighbor table */
  zb_uint8_t      redirect_type; /*!< Redirect type */
  zb_uint8_t      redirect_ref; /*!< reference to regular/redirect
                                 * entry */
}
ZB_PACKED_STRUCT
zb_nvram_addr_map_rec_v1_t;

/** Store a NVM record describing address map, see zb_address_map_t */
typedef ZB_PACKED_PRE struct zb_nvram_addr_map_rec_v2_s
{
  zb_ieee_addr_t  ieee_addr; /*!< Uncompressed IEEE address */
  zb_uint16_t     addr;      /*!< 16-bit device address */
  zb_uint8_t      index;     /*!< Reference for neighbor table */
  zb_uint8_t      redirect_type; /*!< Redirect type */
  zb_uint8_t      redirect_ref; /*!< reference to regular/redirect
                                 * entry */
  zb_uint8_t aligned[3];
}
ZB_PACKED_STRUCT
zb_nvram_addr_map_rec_v2_t;

ZB_ASSERT_IF_NOT_ALIGNED_TO_4(zb_nvram_addr_map_rec_v2_t);

/**
  ZB_NVRAM_NEIGHBOUR_TBL
  Neighbour table dataset is stored in NVM using 2 structures:
  zb_nvram_neighbour_hdr_t
  Array of zb_nvram_neighbour_rec_t records.

  If array size specified in header is 0, no record follows.
*/

/**
 * @name NVRAM neighbor table dataset versions
 * @anchor nvram_neighbor_tbl_ds_versions
 *
 * Note: These values were members of `enum zb_nvram_neighbor_tbl_ds_versions_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
/* First(0) version should be 0.
 * New versions should be added to the end, before ZB_NVRAM_NEIGHBOUR_TBL_DS_VER.
 * After, ZB_NVRAM_NEIGHBOUR_TBL_DS_VER should be updated. */
#define ZB_NVRAM_NEIGHBOR_TBL_DS_VER_0 0U
#define ZB_NVRAM_NEIGHBOR_TBL_DS_VER_1 1U
#define ZB_NVRAM_NEIGHBOR_TBL_DS_VER_2 2U
#define ZB_NVRAM_NEIGHBOR_TBL_DS_VER_3 3U
#define ZB_NVRAM_NEIGHBOUR_TBL_DS_VER ZB_NVRAM_NEIGHBOR_TBL_DS_VER_3 /* Should be equal to latest (highest) version */
#define ZB_NVRAM_NEIGHBOR_TBL_DS_VER_COUNT (ZB_NVRAM_NEIGHBOUR_TBL_DS_VER + 1U)
/** @} */

typedef ZB_PACKED_PRE struct zb_nvram_neighbour_hdr_v0_s
{
  zb_uint8_t nbr_rec_num; /*!< Strores number of stored neighbour devices */
  zb_uint8_t version;     /*!< Stores version of the dataset */
}
ZB_PACKED_STRUCT
zb_nvram_neighbour_hdr_v0_t;

typedef ZB_PACKED_PRE struct zb_nvram_neighbour_hdr_v1_s
{
  zb_uint8_t nbr_rec_num; /*!< Strores number of stored neighbour devices */
  zb_uint8_t version;     /*!< Stores version of the dataset */
  zb_uint8_t aligned[2];
}
ZB_PACKED_STRUCT
zb_nvram_neighbour_hdr_v1_t;

typedef zb_nvram_neighbour_hdr_v1_t zb_nvram_neighbour_hdr_t;


ZB_ASSERT_IF_NOT_ALIGNED_TO_4(zb_nvram_neighbour_hdr_t);

typedef ZB_PACKED_PRE struct zb_nvram_neighbour_rec_v0_s
{
  zb_address_ieee_ref_t     addr_ref;          /*!< Neighbor address */
  zb_bitfield_t             depth:4;           /*!< Device depth  */
  zb_bitfield_t             rx_on_when_idle:1; /*!< Store device rx On when idle state */
  zb_bitfield_t             relationship:3;    /*!< The relationship with nbr device, see @ref nwk_relationship */
  zb_bitfield_t             device_type:2;     /*!< Neighbor device type - @see @ref nwk_device_type */
  zb_bitfield_t             reserved:6;
  zb_uint8_t                key_seq_number;    /*!< key number */
}
ZB_PACKED_STRUCT
zb_nvram_neighbour_rec_v0_t;


typedef ZB_PACKED_PRE struct zb_nvram_neighbour_rec_v1_s
{
  zb_address_ieee_ref_t     addr_ref;          /*!< Neighbor address */
  zb_bitfield_t             depth:4;           /*!< Device depth  */
  zb_bitfield_t             rx_on_when_idle:1; /*!< Store device rx On when idle state */
  zb_bitfield_t             relationship:3;    /*!< The relationship with nbr device, see @ref nwk_relationship */
  zb_bitfield_t             device_type:2;     /*!< Neighbor device type - @see @ref nwk_device_type */
  zb_bitfield_t             nwk_ed_timeout:4;  /*!< ED timeout value (for Child Aging) */
  zb_bitfield_t             reserved:2;
  zb_uint8_t                key_seq_number;    /*!< key number */
}
ZB_PACKED_STRUCT
zb_nvram_neighbour_rec_v1_t;

ZB_ASSERT_IF_NOT_ALIGNED_TO_4(zb_nvram_neighbour_rec_v1_t);

typedef ZB_PACKED_PRE struct zb_nvram_neighbour_rec_v2_s
{
  zb_address_ieee_ref_t     addr_ref;          /*!< Neighbor address */
  zb_bitfield_t             depth:4;           /*!< Device depth  */
  zb_bitfield_t             rx_on_when_idle:1; /*!< Store device rx On when idle state */
  zb_bitfield_t             relationship:3;    /*!< The relationship with nbr device, see @ref nwk_relationship */
  zb_bitfield_t             device_type:2;     /*!< Neighbor device type - @see @ref nwk_device_types */
  zb_bitfield_t             nwk_ed_timeout:4;  /*!< ED timeout value (for Child Aging) */
  zb_bitfield_t             reserved:2;
  zb_uint8_t                key_seq_number;    /*!< key number */
  zb_uint8_t                mac_iface_idx;     /*!< Mac Interface Table index */
  zb_uint8_t                align[3];          /*!< Reserved for future use */
}
ZB_PACKED_STRUCT
zb_nvram_neighbour_rec_v2_t;

ZB_ASSERT_IF_NOT_ALIGNED_TO_4(zb_nvram_neighbour_rec_v2_t);

typedef ZB_PACKED_PRE struct zb_nvram_page_hdr_dataset_s
{
   /*!< Version of dataset structures stored in NVRAM (simple counter)
   * One version for all datasets. */
  zb_nvram_ver_t version;
  zb_uint8_t  aligned[4-sizeof(zb_nvram_ver_t)];
}
zb_nvram_page_hdr_dataset_t;

ZB_ASSERT_IF_NOT_ALIGNED_TO_4(zb_nvram_page_hdr_dataset_t);

typedef enum zb_nvram_zcl_reporting_data_ds_versions_e
{
  ZB_NVRAM_ZCL_REPORTING_DATA_DS_VER_1 = 0,
}
zb_nvram_zcl_reporting_data_ds_versions_t;

#define ZB_NVRAM_ZCL_REPORTING_DATA_DS_VER ZB_NVRAM_ZCL_REPORTING_DATA_DS_VER_1

/**
 * @name NVRAM APS secure data dataset version
 * @anchor nvram_aps_secure_data_ds_versions
 *
 * Note: These values were members of `enum zb_nvram_aps_secure_data_ds_versions_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_NVRAM_APS_SECURE_DATA_DS_VER_1 0U
/** @} */

#define ZB_NVRAM_APS_SECURE_DATA_DS_VER ZB_NVRAM_APS_SECURE_DATA_DS_VER_1

typedef enum zb_nvram_installcodes_ds_versions_e
{
  ZB_NVRAM_INSTALLCODES_DS_VER_1 = 0,
}
zb_nvram_installcodes_ds_versions_t;

#define ZB_NVRAM_INSTALLCODES_DS_VER ZB_NVRAM_INSTALLCODES_DS_VER_1

#ifdef ZB_ENABLE_ZGP

extern zb_ret_t    zb_nvram_write_zgp_proxy_table_dataset(zb_uint8_t page, zb_uint32_t pos);
extern void        zb_nvram_read_zgp_proxy_table_dataset(zb_uint8_t page, zb_uint32_t pos, zb_uint16_t length,
                                                          zb_nvram_ver_t nvram_ver, zb_uint16_t ds_ver);
extern zb_uint16_t zb_nvram_zgp_proxy_table_length(void);
extern void        zb_nvram_update_zgp_proxy_tbl_offset(zb_uint8_t page, zb_uint32_t dataset_pos, zb_uint32_t pos);

extern zb_ret_t    zb_nvram_write_zgp_sink_table_dataset(zb_uint8_t page, zb_uint32_t pos);
extern void        zb_nvram_read_zgp_sink_table_dataset(zb_uint8_t page, zb_uint32_t pos, zb_uint16_t length,
                                                          zb_nvram_ver_t nvram_ver, zb_uint16_t ds_ver);
extern zb_uint16_t zb_nvram_zgp_sink_table_length(void);
extern void        zb_nvram_update_zgp_sink_tbl_offset(zb_uint8_t page, zb_uint32_t dataset_pos, zb_uint32_t pos);

extern zb_ret_t    zb_zgp_nvram_write_app_tbl_dataset(zb_uint8_t page, zb_uint32_t pos);
extern void        zb_zgp_nvram_read_app_tbl_dataset(zb_uint8_t page, zb_uint32_t pos, zb_uint16_t length,
                                                      zb_nvram_ver_t nvram_ver, zb_uint16_t ds_ver);
extern zb_uint16_t zb_zgp_nvram_app_tbl_length(void);

extern zb_ret_t    zb_nvram_write_zgp_cluster_dataset(zb_uint8_t page, zb_uint32_t pos);
extern void        zb_nvram_read_zgp_cluster_dataset(
                    zb_uint8_t page, zb_uint32_t pos, zb_uint16_t length, zb_nvram_ver_t nvram_ver, zb_uint16_t ds_ver);
extern zb_uint16_t zb_nvram_zgp_cluster_length(void);

#endif

/****************************/

#ifdef APP_ONLY_NVRAM
/**
   Initialize local version of nvram.

   @note This function is for utilities which uses nvram but not ZBOSS.
   Never call it from your applications linked with ZBOSS.
 */
void zb_nvram_local_init(void *id);
#endif /* APP_ONLY_NVRAM */

/**
   Load Informational Bases from NVRAM or file
 */
void zb_nvram_load(void);

#ifdef ZB_ZBOSS_DEINIT
/**
   Denit nvram
 */
void zb_nvram_deinit(void);
#endif

/**
   Save Informational Bases to NVRAM or other persistent storage
 */
void zb_nvram_save_all(void);

#ifdef ZB_USE_INTERNAL_HEADERS
/**
   Erase Informational Bases to NVRAM or other persistent storage
 */
void zb_nvram_erase(void);
#endif

zb_ret_t zb_nvram_write_start(zb_nvram_dataset_hdr_t *hdr);
zb_ret_t zb_nvram_write_end(zb_nvram_dataset_hdr_t *hdr);

void zb_nvram_erase_finished(zb_uint8_t page);

void zb_nvram_write_empty_dataset(zb_nvram_dataset_types_t t);

zb_ret_t zb_osif_erase_nvram_trace_sector(void);

void zb_nvram_store_addr_n_nbt(void);


#ifdef ZB_NVRAM_ENABLE_DIRECT_API
zb_ret_t zb_nvram_write_dataset_direct(zb_nvram_dataset_types_t dataset_type,
                                       zb_uint16_t dataset_version,
                                       zb_uint8_t *buf, zb_uint16_t data_len);


zb_ret_t zb_nvram_read_dataset_direct(zb_nvram_dataset_types_t dataset_type,
                                      zb_uint8_t *buf,
                                      zb_size_t buf_len,
                                      zb_uint16_t *dataset_version,
                                      zb_uint16_t *dataset_length,
                                      zb_uint16_t *nvram_version);

#endif /* defined ZB_NVRAM_ENABLE_DIRECT_API */


#if defined(ZB_ENABLE_ZCL) && !(defined ZB_ZCL_DISABLE_REPORTING)

void zb_nvram_read_zcl_reporting_dataset(
  zb_uint8_t page, zb_uint32_t pos, zb_uint16_t length, zb_nvram_ver_t nvram_ver, zb_uint16_t ds_ver);

zb_ret_t zb_nvram_write_zcl_reporting_dataset(zb_uint8_t page, zb_uint32_t pos);

zb_uint16_t zb_nvram_zcl_reporting_dataset_length(void);

#endif /* defined(ZB_ENABLE_ZCL) && !(defined ZB_ZCL_DISABLE_REPORTING) */


#if defined ZB_ENABLE_HA

void zb_nvram_read_ha_dataset(
zb_uint8_t page, zb_uint32_t pos, zb_uint16_t len, zb_nvram_ver_t nvram_ver, zb_uint16_t ds_ver);

zb_ret_t zb_nvram_write_ha_dataset(zb_uint8_t page, zb_uint32_t pos);

#endif /* defined ZB_ENABLE_HA*/


#if defined ZB_HA_ENABLE_POLL_CONTROL_SERVER || defined DOXYGEN

void zb_nvram_read_poll_control_dataset(
  zb_uint8_t page, zb_uint32_t pos, zb_uint16_t len, zb_nvram_ver_t nvram_ver, zb_uint16_t ds_ver);

zb_ret_t zb_nvram_write_poll_control_dataset(zb_uint8_t page, zb_uint32_t pos);

#endif /* defined ZB_HA_ENABLE_POLL_CONTROL_SERVER || defined DOXYGEN */

#define ZB_ERROR_NVRAM_READ_FAILURE 1
#define ZB_ERROR_NVRAM_WRITE_FAILURE 2
#define ZB_ERROR_NVRAM_WRITE_VALIDATION_FAILURE 3

#if defined(ZDO_DIAGNOSTICS)

zb_uint16_t zb_nvram_diagnostics_dataset_length(void);

void zb_nvram_read_diagnostics_dataset(
  zb_uint8_t page, zb_uint32_t pos, zb_uint16_t len, zb_nvram_ver_t nvram_ver, zb_uint16_t ds_ver);

zb_ret_t zb_nvram_write_diagnostics_dataset(zb_uint8_t page, zb_uint32_t pos);

#endif /* ZDO_DIAGNOSTICS */

typedef zb_bool_t (*zb_nvram_ds_filter_cb_t)(zb_uint16_t ds_type);
typedef zb_size_t (*zb_nvram_ds_get_length_cb_t)(zb_uint16_t ds_type);
typedef zb_uint16_t (*zb_nvram_ds_get_version_cb_t)(zb_uint16_t ds_type);
typedef void (*zb_nvram_ds_read_cb_t)(zb_uint16_t ds_type,
                                      zb_uint8_t page,
                                      zb_uint32_t pos,
                                      zb_uint16_t len,
                                      zb_nvram_ver_t nvram_ver,
                                      zb_uint16_t ds_ver);
typedef zb_ret_t (*zb_nvram_ds_write_cb_t)(zb_uint16_t ds_type,
                                           zb_uint8_t page,
                                           zb_uint32_t pos);

zb_ret_t zb_nvram_custom_ds_try_read(zb_uint16_t ds_type,
                                     zb_uint8_t page,
                                     zb_uint32_t pos,
                                     zb_uint16_t len,
                                     zb_nvram_ver_t nvram_ver,
                                     zb_uint16_t ds_ver);

zb_ret_t zb_nvram_custom_ds_try_write(zb_uint16_t ds_type,
                                      zb_uint8_t page,
                                      zb_uint32_t pos);

zb_ret_t zb_nvram_custom_ds_try_get_length(zb_uint16_t ds_type,
                                           zb_size_t *len);

zb_bool_t zb_nvram_custom_ds_is_supported(zb_uint16_t ds_type);


zb_ret_t zb_nvram_custom_ds_register(zb_nvram_ds_filter_cb_t filter,
                                     zb_nvram_ds_get_length_cb_t get_length,
                                     zb_nvram_ds_get_version_cb_t get_version,
                                     zb_nvram_ds_read_cb_t read,
                                     zb_nvram_ds_write_cb_t write);

#endif  /* ZB_USE_NVRAM */

#endif /* ZB_NVRAM_H */
