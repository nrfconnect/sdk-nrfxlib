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
/* PURPOSE: Security services and routines - internals
*/

#ifndef ZB_SECUR_H
#define ZB_SECUR_H 1

#include "zb_common.h"
#include "zb_mac.h"

/*! @cond internals_doc */
/*! \addtogroup ZB_SECUR */
/*! @{ */


#define ZB_NONCE_LENGTH 13U
#define ZB_KEY_LENGTH 16U

/**
 * @name Secur key attributes
 * @anchor secur_key_attributes
 *
 * Note: These values were members of `enum zb_secur_key_attributes_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_SECUR_PROVISIONAL_KEY 0U
#define ZB_SECUR_UNVERIFIED_KEY  1U
#define ZB_SECUR_VERIFIED_KEY    2U
#define ZB_SECUR_APPLICATION_KEY 3U    /*!< non-standard - for quick check for !TCLK */
#define ZB_SECUR_ANY_KEY_ATTR    0xFFU /*!< Any key attribute - for zb_secur_get_link_key_by_address() */
/** @} */

/**
 * @name Secur link key type.
 * @anchor secur_aps_link_key_type
 *
 * Note: These values were members of `enum zb_secur_aps_link_key_type_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_SECUR_UNIQUE_KEY 0U
#define ZB_SECUR_GLOBAL_KEY 1U
/** @} */

/**
 * @name BDB node join link key type.
 * @anchor bdb_node_join_link_key_type
 *
 * Note: These values were members of `enum zb_bdb_node_join_link_key_type_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define BDB_DEFAULT_GLOBAL_TRUST_CENTER_LINK_KEY        0U
#define BDB_DISTRIBUTED_SECURITY_GLOBAL_LINK_KEY        1U
#define BDB_INSTALL_CODE_DERIVED_PRECONFIGURED_LINK_KEY 2U
/** @cond touchlink */
#define BDB_TOUCHLINK_PRECONFIGURED_LINK_KEY            3U
/** @endcond */ /* touchlink */
/** @} */

/**
 * @name Secur key sources.
 * @anchor secur_key_sources
 *
 * Note: These values were members of `enum zb_secur_key_sources_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_SECUR_KEY_SRC_UNKNOWN 0U /*!< key source is unknown/doesn't matter */
#define ZB_SECUR_KEY_SRC_CBKE    1U /*!< key was obtained via CBKE (key establishment) */
/* NOTE: if extended with more key sources, don't forget to adjust
 * - zb_aps_device_key_pair_set_t::key_source
 * - zb_zcl_parsed_hdr_t::key_source
 * - zb_aps_hdr_t::key_source
 */
/** @} */


/**
   APS Device Key Pair Set structure

   Structure contain record Application key with attributes (address, global/unique, etc.)
 */
/* Since that structire used to save into nvram, pack it! */
typedef ZB_PACKED_PRE struct zb_aps_secur_common_data_s
{
  zb_uint8_t coordinator_version;
  zb_uint8_t align[3];
} ZB_PACKED_STRUCT zb_aps_secur_common_data_t;

typedef ZB_PACKED_PRE struct zb_aps_device_key_pair_set_s
{
  zb_ieee_addr_t  device_address;               /*!< Partner address */
  zb_uint8_t      link_key[ZB_CCM_KEY_SIZE];         /*!< Link key, see Spec. Not use in current
                                                      * release */
  /* 4.4.1.1[2] Security Processing of Outgoing[Incoming] Frames */
/* Moved to zb_aps_device_key_pair_array_t */
/* This fields were commented out after discussion with EE.
 * ZB spec does not require to save APS key frame counters into NVRAM.
 */
#ifndef ZB_LITE_NO_GLOBAL_VS_UNIQUE_KEYS
  zb_bitfield_t   aps_link_key_type:1; /*!< @ref secur_aps_link_key_type  */
#endif
  zb_bitfield_t   key_source:1;        /*!< how the key was obtained @ref secur_key_sources */
  zb_bitfield_t   key_attributes:2;    /*!< attributes of the key @ref secur_key_attributes */
  zb_bitfield_t   reserved:4;
  zb_uint8_t      align[3];
} ZB_PACKED_STRUCT zb_aps_device_key_pair_set_t;

typedef zb_aps_device_key_pair_set_t zb_aps_device_key_pair_nvram_t;

ZB_ASSERT_IF_NOT_ALIGNED_TO_4(zb_aps_device_key_pair_nvram_t);

#define ZB_APS_DEVICE_KEY_PAIR_CACHED (zb_uint32_t)0x07ffffffU

/* zb_aps_device_key_pair_array_t moved to zboss_api_internal.h */

typedef struct zb_aps_device_key_pair_storage_s
{
  zb_uint8_t  nvram_page;
  zb_uint8_t cached_i;
  zb_aps_device_key_pair_set_t cached;
#ifndef ZB_CONFIGURABLE_MEM
  zb_aps_device_key_pair_array_t key_pair_set[ZB_N_APS_KEY_PAIR_ARR_MAX_SIZE]; /*!< APS Application Key pair table */
#else
  zb_aps_device_key_pair_array_t *key_pair_set;
#endif
} zb_aps_device_key_pair_storage_t;

typedef ZB_PACKED_PRE struct zb_aps_device_key_pair_set_nvram_1_0_s
{
  zb_ieee_addr_t  device_address;               /*!< Partner address */
  zb_uint8_t master_key[ZB_CCM_KEY_SIZE];       /*!< Master key */
  zb_uint8_t global_link_key;                   /*!< 1 - Global key, 0 - Unique key  */
#ifdef ZB_PLATFORM_CORTEX_M3
  zb_uint8_t  aligned[3];
#endif /* ZB_PLATFORM_CORTEX_M3 */
} ZB_PACKED_STRUCT zb_aps_device_key_pair_set_nvram_1_0_t;


/**
   APS Installcode structure for NVRAM
 */
typedef ZB_PACKED_PRE struct zb_aps_installcode_nvram_s
{
  zb_ieee_addr_t  device_address;               /*!< Partner address */
  /*AEV:Here data was a filler: align[2]; - remove it for storage of IC type for now.
    options lowest 2 bits [0-1]: 00-48, 01-64, 10-96, 11-128 bits ic type.*/
  zb_uint8_t      options;
  zb_uint8_t      align;
  zb_uint8_t      installcode[ZB_CCM_KEY_SIZE+ZB_CCM_KEY_CRC_SIZE];       /*!< 16b installcode +2b crc */
  /*hint:we can remove 2b crc at the end of installcode, but we must keep ic type, thus align to dword and don't change it*/
} ZB_PACKED_STRUCT zb_aps_installcode_nvram_t;

extern const zb_uint8_t zb_ic_size_by_type[ZB_IC_TYPE_MAX];

#define ZB_IC_TYPE_MAX_SIZE 16U

#define ZB_IC_SIZE_BY_TYPE(t) ( ( t ) < ZB_IC_TYPE_MAX ? zb_ic_size_by_type[( t )]:zb_ic_size_by_type[(ZB_IC_TYPE_MAX - 1U)] )

#define ZB_IC_GET_TYPE_FROM_OPT(opt) ( (opt) & 0x7U )
/* zb_aps_installcode_storage_t moved to zboss_api_internal.h */

typedef struct zb_secur_ic_add_s
{
  zb_uint8_t *address;
  zb_uint8_t *ic;
  zb_uint8_t do_update;
  zb_uint8_t type;
} zb_secur_ic_add_t;


/**
 * @name Network key types
 * @anchor secur_key_types
 * @see r20 4.4.3.1.1
 * @see r21 Table 4.9 StandardKeyType Parameter of the Transport-Key, Verify-Key, and Confirm-Key
 * Primitives
 *
 * Note: These values were members of `enum zb_secur_key_types_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_TC_MASTER_KEY_OBSOLETE          0U /*!< reserved */
#define ZB_STANDARD_NETWORK_KEY            1U /*!< NWK key */
#define ZB_APP_MASTER_KEY_OBSOLETE         2U /*!< reserved */
#define ZB_APP_LINK_KEY                    3U /*!< Application link key */
#define ZB_TC_LINK_KEY                     4U /*!< Trust-center link key */
#define ZB_HIGH_SECUR_NETWORK_KEY_OBSOLETE 5U /*!< reserved */
/** @} */

/**
 * @brief Type for Network key types values.
 *
 * Holds one of @ref secur_key_types. Kept only for backward compatibility as
 * they were declared previously as 'enum zb_secur_key_types_e'.
 */
typedef zb_uint8_t zb_secur_key_type_t;

/**
 * @name RequestKey type values
 * @anchor secur_request_key_types
 * @see r21 Table 4-19 RequestKeyType Values
 *
 * Note: These values were members of `enum zb_secur_request_key_types_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_REQUEST_APP_LINK_KEY 2U /*!< Application link key */
#define ZB_REQUEST_TC_LINK_KEY  4U /*!< Trust-center link key */
/** @} */

/**
 * @brief Type for RequestKey type values.
 *
 * Holds one of @ref secur_request_key_types. Kept only for backward compatibility as
 * they were declared previously as 'enum zb_secur_key_attributes_e'.
 */
typedef zb_uint8_t zb_secur_key_attributes_t;

/**
 * @name Key ID
 * @see r20 4.5.1.1.2, r21 Table 4.31 Encoding of the Key Identifier Sub-Field
 *      and 4.5.3 Cryptographic Key Hierarchy
 * @anchor secur_key_id
 */
/** @{ */
#define  ZB_SECUR_DATA_KEY           0U     /*!< Data key  - Link key without a hash */
#define  ZB_SECUR_NWK_KEY            1U     /*!< NWK key - use NWK key*/
#define  ZB_SECUR_KEY_TRANSPORT_KEY  2U     /*!< Transport key - use hash of TC Link key with 2. Used to protect NWK key send. */
#define  ZB_SECUR_KEY_LOAD_KEY       3U     /*!< Load key - use hash of TC key with 3. Used to protect APS LC send */
#define  ZB_SECUR_DATA_KEY_N_UNENCR  0xFEU  /*!< Send 2 copy: encrypted and not encrypted at APS */
#define  ZB_NOT_SECUR                0xFFU  /*!< Not use APS security */
/** @} */

/**
 * @brief Type for key ID.
 *
 * Holds one of @ref secur_key_id. Kept for backward compatibility as
 * @ref secur_key_id were declared previously as enum.
 */
typedef zb_uint8_t zb_secur_key_id_t;

/**
   Auxiliary frame header (4.5.1) for NWK frame and NWK key

   Extended nonce subfield set to 1 (4.3.1.1).
   source_address amd key_seq_number exist.
 */
typedef ZB_PACKED_PRE struct zb_nwk_aux_frame_hdr_s
{
  zb_uint8_t     secur_control;         /*!< Security Control Field */
  zb_uint32_t    frame_counter;         /*!< Frame Counter */
  zb_ieee_addr_t source_address;        /*!< Extended Source */
  zb_uint8_t     key_seq_number;        /*!< Key Sequence Number */
} ZB_PACKED_STRUCT zb_nwk_aux_frame_hdr_t;


/**
   Auxiliary frame header (4.5.1) for APS frame encrypted by NWK key

   Extended nonce subfield set to 0 (4.4.1.1).
   source_address absent, key_seq_number exists.
 */
typedef ZB_PACKED_PRE struct zb_aps_nwk_aux_frame_hdr_s
{
  zb_uint8_t     secur_control;      /*!< Security Control Field */
  zb_uint32_t    frame_counter;      /*!< Frame Counter */
  zb_uint8_t     key_seq_number;     /*!< Key Sequence Number */
} ZB_PACKED_STRUCT zb_aps_nwk_aux_frame_hdr_t;

/**
   Auxiliary frame header (4.5.1) for APS frame encrypted by Data key

   Extended nonce subfield set to 0 (4.4.1.1).
   source_address and key_seq_number are absent.
 */
typedef ZB_PACKED_PRE struct zb_aps_data_aux_frame_hdr_s
{
  zb_uint8_t     secur_control;     /*!< Security Control Field */
  zb_uint32_t    frame_counter;     /*!< Frame Counter */
} ZB_PACKED_STRUCT zb_aps_data_aux_frame_hdr_t;

/**
   Auxiliary frame header (4.5.1) for APS frame encrypted by Data key

   Extended nonce subfield set to 1
   source_address is present, key_seq_number is absent.
 */
typedef ZB_PACKED_PRE struct zb_aps_data_aux_nonce_frame_hdr_s
{
  zb_uint8_t     secur_control;     /*!< Security Control Field */
  zb_uint32_t    frame_counter;     /*!< Frame Counter */
  zb_ieee_addr_t source_address;    /*!< Extended Source */
} ZB_PACKED_STRUCT zb_aps_data_aux_nonce_frame_hdr_t;



/**
   CCM nonce (see 4.5.2.2)
 */
typedef ZB_PACKED_PRE struct zb_secur_ccm_nonce_s
{
  zb_ieee_addr_t source_address;    /*!< Extended Source */
  zb_uint32_t    frame_counter;     /*!< Frame Counter */
  zb_uint8_t     secur_control;     /*!< Security Control Field */
} ZB_PACKED_STRUCT zb_secur_ccm_nonce_t;

/**
 * See 4.5.1.1
 */

#define ZB_NWK_STD_SECUR_CONTROL \
  ( 5U /*security level */ | (ZB_SECUR_NWK_KEY << 3U) /* key identifier - NWK key */ | 1U << 5U /* ext nonce */)

#define ZB_APS_NWK_STD_SECUR_CONTROL \
  ( 5U /*security level */ | (ZB_SECUR_NWK_KEY << 3U) /* key identifier - NWK key */ | 0U /* ext nonce */)

#define ZB_APS_MASTEP_STD_SECUR_CONTROL \
  ( 5U /*security level */ | (ZB_SECUR_KEY_TRANSPORT_KEY << 3U) /* key identifier - APS master key */ | 0U /* ext nonce */)

#define ZB_APS_LINK_STD_SECUR_CONTROL \
  ( 5U /*security level */ | (ZB_SECUR_KEY_LOAD_KEY << 3U) /* key identifier - APS link key */ | 0U /* ext nonce */)

#define ZB_APS_DATA_STD_SECUR_CONTROL \
  ( 5U /*security level */ | (ZB_SECUR_DATA_KEY << 3U) /* key identifier - APS data key */ | 0U /* ext nonce */)

#define ZB_APS_SET_SECUR_CONTROL(security_level, key_identifier, ext_nonce) \
  ( (security_level) | ((key_identifier) << 3U) | ((((ext_nonce) != 0u) ? 1U : 0U) << 5U) )

#define ZB_SECUR_SET_ZEROED_LEVEL(ctrl)  (ctrl) = (ctrl) & (~0x07U)

#define ZB_SECUR_AUX_HDR_GET_KEY_TYPE(ctrl) (((ctrl) >> 3U) & 0x3U)

#define ZB_SECUR_AUX_HDR_SET_KEY_TYPE(ctrl, key_type) ( (ctrl) = ((ctrl) & ~(0x3U << 3U)) | (((key_type) & 0x3U) << 3U))

#define ZB_SECUR_GET_SECURITY_LEVEL(ctrl) ((ctrl) & 0x7U)

#define ZB_SECUR_SET_SECURITY_LEVEL(ctrl, level) ( (ctrl) = ((ctrl) & ~0x07U) | ((level) & 0x07U) )

#define ZB_SECUR_GET_SECURITY_NONCE(ctrl) (((ctrl) >> 5U) & 0x1U)


#if 0
#define ZB_SECUR_SET_BUF_MAC_ENCR(_pbuf) ((_pbuf)->u.hdr.encrypt_type |= ZB_SECUR_MAC_ENCR)

#define ZB_SECUR_SET_BUF_NWK_ENCR(_pbuf) ((_pbuf)->u.hdr.encrypt_type |= ZB_SECUR_NWK_ENCR)

#define ZB_SECUR_SET_BUF_APS_ENCR(_pbuf) ((_pbuf)->u.hdr.encrypt_type |= ZB_SECUR_APS_ENCR)

#define ZB_SECUR_CLR_BUF_MAC_ENCR(_pbuf) ((_pbuf)->u.hdr.encrypt_type &= ~ZB_SECUR_MAC_ENCR)

#define ZB_SECUR_CLR_BUF_NWK_ENCR(_pbuf) ((_pbuf)->u.hdr.encrypt_type &= ~ZB_SECUR_NWK_ENCR)

#define ZB_SECUR_CLR_BUF_APS_ENCR(_pbuf) ((_pbuf)->u.hdr.encrypt_type &= ~ZB_SECUR_APS_ENCR)

#define ZB_SECUR_GET_BUF_MAC_ENCR(_pbuf) (((_pbuf)->u.hdr.encrypt_type & ZB_SECUR_MAC_ENCR) >> 2U)

#define ZB_SECUR_GET_BUF_NWK_ENCR(_pbuf) ((_pbuf)->u.hdr.encrypt_type & ZB_SECUR_NWK_ENCR)

#define ZB_SECUR_GET_BUF_APS_ENCR(_pbuf) (((_pbuf)->u.hdr.encrypt_type & ZB_SECUR_APS_ENCR) >> 1U)
#endif


/**
   Initialize APS security data structures
 */
void zb_aps_secur_init(void);

/**
   Zigbee Keyed Hash Function. Described in Zigbee specification
   section B.1.4, and in FIPS Publication 198. Strictly speaking
   there is nothing about the Keyed Hash Function which restricts
   it to only a single byte input, but that's all Zigbee ever uses.

   The output of this function is an ep_alloced buffer containing
   the key-hashed output, and is guaranteed never to return NULL.

   @param key   - Zigbee Security Key (must be ZBEE_SEC_CONST_KEYSIZE) in length.
   @param input - key index
   @param hash_key - hash key
 */
void zb_cmm_key_hash(zb_uint8_t *key, zb_uint8_t input, zb_uint8_t *hash_key);


/**
 * Size of one hash block.
 * @note THe result of hashing is exactly one hash block.
 */
#define ZBEE_SEC_CONST_BLOCKSIZE 16U

/**
 * @brief Zigbee Cryptographic Hash Function, described in Zigbee
 *        specification sections B.1.3 and B.6.
 *
 * This is a Matyas-Meyer-Oseas hash function using the AES-128
 * cipher. The ECB mode of libgcrypt is used to get a raw block
 * cipher.
 *
 * Input may be any length, and the output must be exactly 1-block in length.
 *
 * Implements the function:
 *    Hash(text) = Hash[t];
 *    Hash[0] = 0^(blocksize).
 *    Hash[i] = E(Hash[i-1], M[i]) XOR M[j];
 *    M[i] = i'th block of text, with some padding and flags concatenated.
 *
 * @param input      Hash Input (any length).
 * @param input_len  Hash Input Length.
 * @param output     Hash Output (exactly one block in length).
 */
zb_bool_t zb_sec_b6_hash(zb_uint8_t *input, zb_uint32_t input_len, zb_uint8_t *output);

/**
   Search for link key by pair address and attribute.

   @note Up to 2 keys to the same address can exist: one is
   VERIFIED/PROVISIONING, another is UNVERIFIED.
   See BDB 10.3.3 Behavior when a known node joins.

   @param address - IEEE address
   @param attr - key attribute to search.
   @return Key Pair Set structure
 */
zb_aps_device_key_pair_set_t *zb_secur_get_link_key_by_address(zb_ieee_addr_t address,
                                                               zb_secur_key_attributes_t attr);

/**
   Search for link key index by pair address and attribute.

   @note Up to 2 keys to the same address can exist: one is
   VERIFIED/PROVISIONING, another is UNVERIFIED.
   See BDB 10.3.3 Behavior when a known node joins.

   @param address - IEEE address
   @param attr - key attribute to search.
   @return keypair index if link key exists, -1 otherwise
 */
zb_uint16_t zb_aps_keypair_get_index_by_addr(zb_ieee_addr_t dev_addr,
                                             zb_secur_key_attributes_t attr);

#ifndef ZB_HW_ZB_AES128
/*
  Encrypt a single 128-bit block by a 128-bit key using AES
  url: en.wikipedia.org/wiki/Advanced_Encryption_Standard
*/
void zb_aes128(zb_uint8_t *key, zb_uint8_t *msg, zb_uint8_t *c);
#endif


#ifndef ZB_HW_ZB_AES128_DEC
void zb_aes128_dec(zb_uint8_t *key, zb_uint8_t *msg, zb_uint8_t *c);
#endif


#ifdef ZB_SOFT_SECURITY

/**
   CCM* encryption and authentication procedure for Standard security

   This implementation may be not optimal. It done for debug only: be able to
   encrypt in Linux to be able to compare results with real HW.
   To be debugged using test vectors from the spec.


   @param key   - 16-bytes key
   @param nonce - 16-bytes nonce - see 4.5.2.2 "CCM nonce" for details
   @param string_a - 'a' parameter - authentication string -
                     NwkHeader||AuxiliaryHeader for Standard security. See 4.3.1.1 for details
   @param string_a_len - l(a)
   @param string_m - 'm' parameter - text data -
                     Payload for Standard security. See 4.3.1.1 for details
   @param string_m_len - l(m)
   @param crypted_text - encryption result - user supplied buffer. Must have size
          string_m_len + ccm_m rounded to 16.
          Result len is always string_m_len + ccm_m.


   @return RET_OK if success, RET_ERROR in case of error
 */
zb_ret_t
zb_ccm_encrypt_n_auth(
  zb_uint8_t *key,
  zb_uint8_t *nonce,
  zb_uint8_t *string_a,
  zb_uint32_t string_a_len,
  zb_uint8_t *string_m,
  zb_uint32_t string_m_len,
  zb_bufid_t crypted_text);


/**
   CCM* decryption and authentication procedure for Standard security

   @param key   - 16-bytes key
   @param nonce - 16-bytes nonce - see 4.5.2.2 "CCM nonce" for details
   @param buf   - buffer with encrypted_text [IN], for decryption result [OUT]
   @param string_a_len - length string_a - authentication string -
                     NwkHeader||AuxiliaryHeader for Standard security. See 4.3.1.1 for details
   @param string_c_len - length text data -
                     Payload for Standard security. See 4.3.1.1 for details

   @return RET_OK if success, RET_ERROR in case of error (authentication failure)
 */
zb_ret_t
zb_ccm_decrypt_n_auth(
  zb_uint8_t *key,
  zb_uint8_t *nonce,
  zb_bufid_t buf,
  zb_uint32_t string_a_len,
  zb_uint32_t string_c_len);


/**
   Decryption and authentication according to standard security requirements.

   Decrypt "on place". Use HW if possible,
 */
#define zb_ccm_decrypt_n_auth_stdsecur zb_ccm_decrypt_n_auth

#endif /* ZB_SOFT_SECURITY */


void zb_ccm_auth_trans(
  zb_ushort_t ccm_m,
  zb_uint8_t *key,
  zb_uint8_t *nonce,
  zb_uint8_t *string_a,
  zb_uint32_t string_a_len,
  zb_uint8_t *string_m,
  zb_uint32_t string_m_len,
  zb_uint8_t *t);

/*
void encrypt_trans(
  zb_ushort_t ccm_m,
  zb_uint8_t *key,
  zb_uint8_t *nonce,
  zb_uint8_t *string_m,
  zb_uint16_t string_m_len,
  zb_uint8_t *t,
  zb_uint8_t *encrypted);
*/

/**
   Secure frame at NWK level according to the current security mode
   This procedure secures frame 'on place' increasing its size.
   It is supposed that buffer tail has enough free space.

   @param src - nwk packet to secure
   @param mac_hdr_size - size of header placed before mac hdr
   @param dst - buffer for the secured frame


   @return RET_OK if ok, else error code
 */
zb_ret_t zb_nwk_secure_frame(zb_bufid_t src, zb_uint_t mac_hdr_size, zb_bufid_t dst);


/**
   Unsecure NWK frame at NWK level according to the current security mode
   This procedure secures frame 'on place' decreasing its size.

   @param param - ref to nwk packet to unsecure

   @return RET_OK if success or RET_ERROR if unsecure failed
   Attention: if frame unsecure failed, this function reuses packet buffer to
   send NWK status. Don't use the buffer if return code != RET_OK!
 */
zb_ret_t zb_nwk_unsecure_frame(zb_uint8_t param);


/**
   Allocate and fill space for auxiliary security header in the APS data or command frame.

   @param p - pointer to the aux hdr
   @param key_type - key type for aux header see 4.5.1.1.2, @see @ref secur_key_id
   @param ext_nonce - extended nonce flag

   If nwk_key is false and transport_key is false then use APS data key
 */
void zb_secur_aps_aux_hdr_fill(zb_uint8_t *p, zb_secur_key_id_t key_type, zb_bool_t ext_nonce);

/**
   Add security to the APS command frame. Allocate space for APS header. Fill APS Aux security header.

   @param buf - buffer with command payload
   @param command_id - command id to assign into APS command header
   @param secure_aps - secure type APS level
 */
void zb_aps_command_add_secur(zb_bufid_t buf, zb_uint8_t command_id, zb_secur_key_id_t secure_aps);

/**
   Encrypt APS frame before its send.

   Need to put secured frame into another buffer because original buffer will be
   passed back to APS in CONFIRM primitive and, potentially, will be
   retransmitter by APS layer, so must keep it unencrypted.

   @param src - source buffer with all headers filled, including MHR
   @param mac_hdr_size - size of MAC header
   @param dst - destination buffer (for secure result)
   @param is_tunnel - APS command has tunnel header

   @return RET_OK if successfully encrypted frame, error code otherwise
 */
zb_ret_t zb_aps_secure_frame(zb_bufid_t src, zb_uint_t mac_hdr_size, zb_bufid_t dst, zb_bool_t is_tunnel);


/**
   Decrypt APS frame "on place"

   @param buf - (in/out) buffer holding frame
   @param keypair_i_p - (out) index of APS keypair used to decrypt or (zb_uint16_t)-1 if used default TCLK
   @param key_id_p - pointer to variable that contains key used to protect the frame
   @param is_verified_tclk - (out) ZB_TRUE if the frame was decrypted successfully using TCLK, ZB_FALSE otherwise

   @return RET_OK if successfully decrypted frame, error code otherwise
 */
zb_ret_t zb_aps_unsecure_frame(zb_bufid_t buf, zb_uint16_t *keypair_i_p, zb_secur_key_id_t *key_id_p, zb_bool_t *is_verified_tclk);

/**
   Return size of APS aux secure header

   @param secur_control - aux APS header secur_control field
   @return aux header size if ok or -1 if error (unknown key type)
 */
zb_ushort_t zb_aps_secur_aux_size(zb_uint8_t secur_control);

#ifdef ZB_MAC_SECURITY
/**
   Encrypt MAC frame before its send.

   Need to put secured frame into another buffer because original buffer will be
   passed back to APS in CONFIRM primitive and, potentially, will be
   retransmitter by APS layer, so must keep it unencrypted.

   @param src - source buffer with all headers filled, including MHR
   @param mac_hdr_size - size of MAC header
   @param dst - destination buffer (for secure result)

   @return RET_OK if successfully encrypted frame, error code otherwise
 */
zb_ret_t zb_mac_secure_frame(zb_bufid_t src, zb_uint_t mac_hdr_size, zb_bufid_t dst);

/**
   Encrypt MAC frame "on place"

   @param param - (in/out) buffer holding frame

   @return RET_OK if successfully decrypted frame, error code otherwise
 */
zb_ret_t zb_mac_unsecure_frame(zb_uint8_t param);
#endif

/**
   Parameters for APSME-TRANSPORT-KEY.request primitive
 */
typedef struct zb_apsme_transport_key_req_s
{
  zb_addr_u dest_address;  /*!< destination address  */
  zb_uint8_t addr_mode;    /*!< The type of destination address supplied by
                             the DstAddr parameter - see @ref address_modes.
                             This field is non-standard: according to table 4.11
                             dest_address can be 64-bit only.
                             But, according to 4.6.3.4.1  Trust Center Operation,
                             TC must issue APSME-TRANSPORT-KEY.request with
                             broadcast address - means, need 16-bit address here!
                           */
  zb_uint8_t key_type;          /*!< @see @ref secur_key_types  */
  union key_holder_u {
    struct apsme_transport_key_nwk_s
    {
      zb_uint8_t key[ZB_CCM_KEY_SIZE];     /*!< Key */
      zb_uint8_t key_seq_number;           /*!< Sequence Number */
      zb_uint8_t use_parent;               /*!< Use parent - see 4.6.3 */
      zb_ieee_addr_t parent_address;       /*!< Parent Address */
    } nwk;                                 /*!< Use for transmit NWK key */
    struct apsme_transport_key_app_s
    {
      zb_uint8_t key[ZB_CCM_KEY_SIZE];      /*!< Key */
      zb_ieee_addr_t partner_address;       /*!< Partner address */
      zb_uint8_t initiator;                 /*!< Is initiator.
                                                 @note Exists in r20, no in r21, seems, still necessary. */
    } app;                                  /*!< Use for transmit Application key */
  } key;
  /* mega-hack: zb_aps_secure_frame uses dest_address to seek APS key. But,
     zb_aps_secure_frame is called from MAC when NWK also put parameter into
     buffer tail. Let's reserve a space for mcps data request parameters.
     Agree, this is ugly solution. The alternative is to parse packet contents,
     but is is more complex: must analyze for tunnel/direct send.
 */
  zb_uint8_t filler[sizeof(zb_mcps_data_req_params_t) > sizeof(union key_holder_u) ? sizeof(zb_mcps_data_req_params_t) - sizeof(union key_holder_u) : 4];
} zb_apsme_transport_key_req_t;

/**
   Variable part of Transport key command for translate NWK key
 */
typedef ZB_PACKED_PRE struct zb_transport_key_nwk_pkt_s
{
  zb_uint8_t     seq_number;        /*!< Sequence Number */
  zb_ieee_addr_t dest_address;      /*!< Destination address */
  zb_ieee_addr_t source_address;    /*!< Source address */
} ZB_PACKED_STRUCT zb_transport_key_nwk_pkt;

/**
   Variable part of Transport key command for translate TC key
 */
typedef ZB_PACKED_PRE struct zb_transport_key_tc_pkt_s
{
  zb_ieee_addr_t dest_address;      /*!< Destination address */
  zb_ieee_addr_t source_address;    /*!< Source address */
} ZB_PACKED_STRUCT zb_transport_key_tc_pkt;

/**
   Variable part of Transport key command for translate Application key
 */
typedef ZB_PACKED_PRE struct zb_transport_key_app_pkt_s
{
  zb_ieee_addr_t partner_address;   /*!< Partner address */
  zb_uint8_t initiator;             /*!< Is initiator */
}  ZB_PACKED_STRUCT zb_transport_key_app_pkt;

/**
   Transport key command data structure


 */
typedef ZB_PACKED_PRE struct zb_transport_key_dsc_pkt_s
{
  zb_uint8_t     key_type;       /*!< indeed, not part of the nwk key descriptor,
                                  * but it simplifies code  */

  zb_uint8_t     key[ZB_CCM_KEY_SIZE];  /*!< Key*/
  ZB_PACKED_PRE union {
    zb_transport_key_nwk_pkt nwk;       /*!< Variable part for translate NWK key */
    zb_transport_key_tc_pkt tc;         /*!< Variable part for translate TC key */
    zb_transport_key_app_pkt app;       /*!< Variable part for translate Application key */
  } key_data;
} ZB_PACKED_STRUCT zb_transport_key_dsc_pkt_t;

/**
   Update device command data structure
 */
typedef ZB_PACKED_PRE struct zb_apsme_update_device_pkt_s
{
  zb_ieee_addr_t device_address;            /*!< 64-bit address of update device */
  zb_uint16_t    device_short_address;      /*!< 16-bit address of update device */
  zb_uint8_t     status;                    /*!< @see @ref secur_upd_device_status  */
} ZB_PACKED_STRUCT zb_apsme_update_device_pkt_t;

#ifdef ZB_FORMATION
/**
   APSME-TRANSPORT-KEY.request primitive

   @param param - packet buffer filled be \see zb_transport_key_dsc_pkt_t
 */
void zb_apsme_transport_key_request(zb_uint8_t param);
#endif

/**
   Parameter for APSME-UPDATE-DEVICE.request
 */
typedef struct zb_apsme_update_device_req_s
{
  zb_ieee_addr_t dest_address;               /*!< 64-bit Destination address */
  zb_ieee_addr_t device_address;             /*!< 64-bit address of update device */
  zb_uint16_t    device_short_address;       /*!< 16-bit address of update device */
  zb_uint8_t     status;                     /*!< @see @ref secur_upd_device_status  */
} zb_apsme_update_device_req_t;



/**
   Parameter for APSME-UPDATE-DEVICE.indication
 */
typedef struct zb_apsme_update_device_ind_s
{
  zb_ieee_addr_t src_address;               /*!< 64-bit Destination address */
  zb_ieee_addr_t device_address;            /*!< 64-bit address of update device */
  zb_uint16_t    device_short_address;      /*!< 16-bit address of update device */
  zb_uint8_t     status;                    /*!< @see @ref secur_upd_device_status  */
} zb_apsme_update_device_ind_t;


/**
   Parameter for APSME-SWITCH-KEY.request

   Note about addr_mode: it is spec violation, but it necessary to satisfy TC.
 */
typedef struct zb_apsme_switch_key_req_s
{
  zb_ieee_addr_t dest_address;   /*!< Destination address  */
  zb_uint8_t key_seq_number;     /*!< Sequence Number */
} zb_apsme_switch_key_req_t;


#ifdef ZB_COORDINATOR_ROLE
/**
   APSME-SWITCH-KEY.request primitive

   @param param - packet buffer filled be \see zb_apsme_switch_key_req_t
 */
void zb_apsme_switch_key_request(zb_uint8_t param);
#endif

/**
   APSME-SWITCH-KEY APS command body
 */
typedef ZB_PACKED_PRE struct zb_apsme_switch_key_pkt_s
{
  zb_uint8_t key_seq_number;     /*!< Key Number */
} ZB_PACKED_STRUCT zb_apsme_switch_key_pkt_t;


/**
   APSME-SWITCH-KEY.indication
 */
typedef struct zb_apsme_switch_key_ind_s
{
  zb_ieee_addr_t src_address;       /*!< Source Address*/
  zb_uint8_t key_seq_number;        /*!< Key Number */
} zb_apsme_switch_key_ind_t;


void zb_aps_in_switch_key(zb_uint8_t param);


/**
   APSME-REMOVE-DEVICE.request primitive parameters structure
 */
typedef ZB_PACKED_PRE struct zb_apsme_remove_device_req_s
{
  zb_ieee_addr_t parent_address;    /*!< Parent Address*/
  zb_ieee_addr_t child_address;     /*!< Child Address*/
} ZB_PACKED_STRUCT zb_apsme_remove_device_req_t;


/**
   Remove device command data structure
 */
typedef ZB_PACKED_PRE struct zb_apsme_remove_device_pkt_s
{
  zb_ieee_addr_t child_address;     /*!< Child Address*/
} ZB_PACKED_STRUCT zb_apsme_remove_device_pkt_t;


/**
   APSME-REMOVE-DEVICE.indication primitive parameters structure
 */
typedef ZB_PACKED_PRE struct zb_apsme_remove_device_ind_s
{
  zb_ieee_addr_t src_address;       /*!< Source Address */
  zb_ieee_addr_t child_address;     /*!< Child Address */
} ZB_PACKED_STRUCT zb_apsme_remove_device_ind_t;

#ifdef ZB_COORDINATOR_ROLE
/**
   APSME-REMOVE-DEVICE.request command

   @param param - packet buffer filled be \see zb_apsme_request_key_req_t

   @snippet tp_sec_bv_28_zc.c zb_secur_apsme_remove_device
 */
void zb_secur_apsme_remove_device(zb_uint8_t param);
#endif

/**
   APSME-REMOVE-DEVICE.indication primitive

   @param param - packet buffer filled be \see zb_apsme_remove_device_pkt_t
 */
void zb_aps_in_remove_device(zb_uint8_t param);

/**
   APSME-REQUEST-KEY.request primitive parameters structure
 */
typedef ZB_PACKED_PRE struct zb_apsme_request_key_req_s
{
  zb_ieee_addr_t dest_address;      /*!< Destination address */
  zb_uint8_t     key_type;          /*!< \see secur_request_key_types  */
  zb_ieee_addr_t partner_address;   /*!< Partner address */
} ZB_PACKED_STRUCT zb_apsme_request_key_req_t;

typedef ZB_PACKED_PRE struct zb_apsme_request_key_pkt_s
{
  zb_uint8_t     key_type;          /*!< \see secur_request_key_types  */
  zb_ieee_addr_t partner_address;   /*!< Partner address */
} ZB_PACKED_STRUCT zb_apsme_request_key_pkt_t;

/**
   APSME-REQUEST-KEY.indication primitive parameters structure
 */
typedef ZB_PACKED_PRE struct zb_apsme_request_key_ind_s
{
  zb_ieee_addr_t src_address;       /*!< Source address */
  zb_uint8_t     key_type;          /*!< \see secur_request_key_types  */
  zb_ieee_addr_t partner_address;   /*!< Partner address */

  zb_uint16_t    keypair_i;     /*!< index of keypair used to decrypt cmd */
} ZB_PACKED_STRUCT zb_apsme_request_key_ind_t;

/**
   APSME-VERIFY-KEY.request

   See 4.4.7.1 APSME-VERIFY-KEY.request
 */
typedef struct zb_apsme_verify_key_req_s
{
  zb_ieee_addr_t dest_address;  /*!< Destination address. ONly TC is valid  */
  zb_uint8_t key_type;          /*!< key type. Only ZB_TC_LINK_KEY is legal.  */
} zb_apsme_verify_key_req_t;


/**
4.4.10.7 Verify-Key Command
 */
typedef ZB_PACKED_PRE struct zb_apsme_verify_key_pkt_s
{
  zb_uint8_t key_type;          /*!< key type. Only ZB_TC_LINK_KEY is legal.  */
  zb_ieee_addr_t src_address;  /*!< Source address. */
  zb_uint8_t key_hash[ZB_CCM_KEY_SIZE]; /*!< Initiator Verify-Key Hash Value  */
} ZB_PACKED_STRUCT zb_apsme_verify_key_pkt_t;


/**
4.4.7.2 APSME-VERIFY-KEY.indication
 */
typedef struct zb_apsme_verify_key_ind_s
{
  zb_ieee_addr_t src_address;  /*!< Source address. */
  zb_uint8_t key_type;          /*!< key type. Only ZB_TC_LINK_KEY is legal.  */
  zb_uint8_t key_hash[ZB_CCM_KEY_SIZE]; /*!< Initiator Verify-Key Hash Value  */
} ZB_PACKED_STRUCT zb_apsme_verify_key_ind_t;

/**
4.4.8.1 APSME-CONFIRM-KEY.request
 */
typedef struct zb_apsme_confirm_key_req_s
{
  zb_uint8_t status;          /*!< A value indicating the success or failure of a previous attempt to verify the trust center link key. See Table 2.27*/
  zb_ieee_addr_t dest_address;  /*!< Destination address.   */
  zb_uint8_t key_type;          /*!< key type. Only ZB_TC_LINK_KEY is legal.  */
} zb_apsme_confirm_key_req_t;


/**
4.4.10.8 Confirm-Key Command
 */
typedef ZB_PACKED_PRE struct zb_apsme_confirm_key_pkt_s
{
  zb_uint8_t status; /*!< A value indicating the success or failure of a previous attempt to
                             verify the trust center link key. See Table 2.27*/
  zb_uint8_t key_type;    /*!< key type. Only ZB_TC_LINK_KEY is legal.  */
  zb_ieee_addr_t dest_address; /*!< Dest address. */
} ZB_PACKED_STRUCT zb_apsme_confirm_key_pkt_t;


/**
4.4.8.2 APSME-CONFIRM-KEY.indication
 */
typedef struct zb_apsme_confirm_key_ind_s
{
  zb_uint8_t status; /*!< A value indicating the success or failure of a previous attempt to
                             verify the trust center link key. See Table 2.27*/
  zb_uint8_t key_type;    /*!< key type. Only ZB_TC_LINK_KEY is legal.  */
  zb_ieee_addr_t src_address; /*!< The extended 64-bit address of the device that sent the
                                 verify-key command. */
} zb_apsme_confirm_key_ind_t;


/**
   APSME-REQUEST-KEY.request command

   @param param - packet buffer filled be \see zb_apsme_request_key_req_t
 */
void zb_secur_apsme_request_key(zb_uint8_t param);

/**
   APSME-REQUEST-KEY.indication primitive

   @param param - packet buffer filled be \see zb_apsme_request_key_pkt_t
   @param keypair_i - index of APS key used to decrypt APS packet of -1 if default TCLK
   @param key_id - the key used to protect the frame
 */
#ifdef ZB_COORDINATOR_ROLE
void zb_aps_in_request_key(zb_uint8_t param, zb_uint16_t keypair_i
                           , zb_secur_key_id_t key_id
                          );
#endif

/**
   APSME-UPDATE-DEVICE.request command

   @param param - packet buffer filled be \see zb_apsme_update_device_req_t
 */
void zb_apsme_update_device_request(zb_uint8_t param);

#ifdef ZB_FORMATION
/**
   Initialize Trust Center functionality
 */
void secur_tc_init(void);


/**
   Generate NWK keys as random numbers
 */
void secur_nwk_generate_keys(void);
#endif

/**
   Authenticate child after join
 */
void secur_authenticate_child(zb_uint8_t param);

/**
   Remove from NBT unauthenticated device
 */
void secur_forget_unauth_dev(zb_uint8_t addr_ref);


/**
   Generate NWK key by key sequence number

   @param key_seq_number - key sequence number
   @return ref to NWK key from NWK keys table
 */
zb_uint8_t *secur_nwk_key_by_seq(zb_ushort_t key_seq_number);

/**
  Update AIB Key-Pair table

  @param address - partner address
  @param key - application key
  @param key_type - @see @ref secur_key_types
  @param key_attr - @see secur_key_attributes
  @param key_source - @see secur_key_sources

  @return pointer to the keypair updated/created or NULL in case of error

  @snippet tp_pro_bv-56_zr.c tp_pro_bv-56_zr
 */
zb_aps_device_key_pair_set_t * zb_secur_update_key_pair(zb_ieee_addr_t address,
                                                        zb_uint8_t* key,
                                                        zb_uint8_t key_type,
                                                        zb_uint8_t key_attr,
                                                        zb_uint8_t key_source
                                                        );

/**
   Get address from link key pair by specified index.

   @param idx - index from link key pair set
   @param dev_addr - pointer to store device address (addr from key pair if found, zero addr otherwise)
 */
void zb_aps_keypair_get_addr_by_idx(zb_uint_t idx, zb_uint8_t *dev_addr);

/**
   Get link key pair by specified index.

   @param idx - index from link key pair set
   @return pointer to the keypair if it is found and NULL otherwise
 */
zb_aps_device_key_pair_set_t* zb_aps_keypair_get_ent_by_idx(zb_uint_t idx);

/**
  Write/add key pair to AIB Key-Pair table

  @param ent - pointer to key pair entry
  @param idx - index from link key pair set to write (or -1 to add new element)

  @return status of operation - RET_OK for success, RET_ERROR otherwise
 */
zb_ret_t zb_aps_keypair_write(zb_aps_device_key_pair_set_t *ent, zb_uint32_t idx);

/**
   Load link key pair by specified index.

   @param idx - index from link key pair set
   @return status of operation - RET_OK for success, RET_NOT_FOUND if key pair is not found
 */
zb_ret_t zb_aps_keypair_load_by_idx(zb_uint_t idx);

/**
   Read link key pair by specified index.

   @param idx - index from link key pair set
   @param ent - pointer to the loaded key pair
   @return status of operation - RET_OK for success, RET_NOT_FOUND if key pair is not found
 */
zb_ret_t zb_aps_keypair_read_by_idx(zb_uint_t idx, zb_aps_device_key_pair_nvram_t *ent);

/**
   Parameters for APSME-TRANSPORT-KEY.indication primitive
 */
typedef struct zb_apsme_transport_key_indication_s
{
  zb_ieee_addr_t src_address;               /*!< Source address */
  zb_uint8_t key_type;                      /*!< @see @ref secur_key_types  */
  union {
    struct apsme_transport_key_nwk_ind_s
    {
      zb_uint8_t key[ZB_CCM_KEY_SIZE];      /*!< Key */
      zb_uint8_t key_seq_number;            /*!< Sequence number */
    } nwk;
    struct apsme_transport_key_app_ind_s
    {
      zb_uint8_t key[ZB_CCM_KEY_SIZE];      /*!< app link Key */
      zb_ieee_addr_t partner_address;       /*!< The extended 64-bit address of the device that was also sent this link key. */
    } app;
    struct apsme_transport_key_tc_ind_s
    {
      zb_uint8_t key[ZB_CCM_KEY_SIZE];      /*!< TC LiKey */
    } tc;
  } key;
} zb_apsme_transport_key_indication_t;



/**
   APSME-TRANSPORT-KEY.indication primitive

   @param param - packet buffer filled be \see zb_apsme_transport_key_indication_t
 */
void zb_apsme_transport_key_indication(zb_uint8_t param);


/**
   Switch NWK key

   @param key_number - NWK key number
 */
void secur_nwk_key_switch(zb_uint8_t key_number);

/**
   Switch NWK key and send it via broadcast
 */
#ifdef ZB_COORDINATOR_ROLE
void zb_secur_switch_nwk_key_br(zb_uint8_t param);
#endif

/**
   Return true if network key is empty (all zeroes).

   @param key - key
   @return ZB_TRUE if key is empty, ZB_FALSE otherwise
 */
zb_bool_t secur_nwk_key_is_empty(zb_uint8_t *key);


/**
   APSME-REMOVE-DEVICE.indication primitive

   @param param - packet buffer filled be \see zb_apsme_remove_device_ind_t
 */
void zb_apsme_remove_device_indication(zb_uint8_t param);


/**
   APSME-REQUEST-KEY.indication primitive

   @param param - packet buffer filled be \see zb_apsme_request_key_ind_t
 */
#ifdef ZB_COORDINATOR_ROLE
void zb_apsme_request_key_indication(zb_uint8_t param);
#endif

/**
   APSME-SWITCH-KEY.indication primitive

   @param param - packet buffer filled be \see zb_apsme_switch_key_ind_t
 */
void zb_apsme_switch_key_indication(zb_uint8_t param);


/**
   Reaction on TRANSPORT-KEY APS command (indication)

   @param param - buffer index with transport key command data
   @param keypair_i - index of APS key used to decrypt APS packet or -1 if used default TCLK
   @param key_id - the key used to protect the frame
   @return nothing
 */
void zb_aps_in_transport_key(zb_uint8_t param, zb_uint16_t keypair_i, zb_secur_key_id_t key_id);

/**
   Analyze trust frame by command ID, security level and key type

    @param cmd_id - command ID
    @param secured - frame is crypt
    @param is_global_key - key is global

    @return 'frame is trust'
 */
zb_bool_t zb_secur_aps_accept_policy(zb_uint8_t cmd_id, zb_bool_t secured, zb_uint16_t keypair_i);

/**
   APSME-TUNNEL.indication primitive
   Wrap APS command from tunnel command

   @param param - packet buffer filled be and APS command
 */
void zb_aps_in_tunnel_cmd(zb_uint8_t param);

/**

 */
void zb_secur_rejoin_after_security_failure(zb_uint8_t param);


#if defined ZB_HW_ZB_AES128 || defined ZB_HW_ZB_AES128 || !defined ZB_SOFT_SECURITY
#include "hw_crypto_api.h"
#endif

#ifdef ZB_COORDINATOR_ROLE
void zb_aps_in_verify_key(zb_uint8_t param);
#endif

void zb_aps_in_confirm_key(zb_uint8_t param);

void zb_apsme_verify_key_req(zb_uint8_t param);

void zb_zdo_update_tclk(zb_uint8_t param);

void zb_zdo_verify_tclk(zb_uint8_t param);

#ifdef ZB_COORDINATOR_ROLE
void zb_apsme_verify_key_indication(zb_uint8_t param);

void zb_apsme_confirm_key_request(zb_uint8_t param);
#endif

void zb_apsme_confirm_key_indication(zb_uint8_t param);

void zdo_secur_init(void);

void bdb_initiate_key_exchange(zb_uint8_t param);

void bdb_initiate_key_verify(zb_uint8_t param);
zb_bool_t bdb_verify_tclk_in_progress(void);

#if defined ZB_COORDINATOR_ROLE && !defined ZB_LITE_NO_TRUST_CENTER_REQUIRE_KEY_EXCHANGE
void bdb_link_key_transport_with_alarm(zb_uint8_t param2, zb_uint16_t param);

zb_uint8_t bdb_cancel_link_key_refresh_alarm(zb_callback_t func, zb_uint8_t param);

void bdb_link_key_refresh_alarm(zb_uint8_t param);
#endif

void zdo_authentication_failed(zb_uint8_t param);

/* CR : 04/19/2016 [VP]: Start: put it here to call this functions from apsme_secur.c */
void bdb_request_tclk_alarm(zb_uint8_t param);

void bdb_update_tclk_failed(zb_uint8_t param);
/* CR : 04/19/2016 [VP]: Stop */

void zb_secur_delete_link_keys_by_addr_ref(zb_address_ieee_ref_t addr_ref);

void zb_secur_delete_link_keys_by_long_addr(zb_ieee_addr_t ieee_address);

void zb_secur_delete_link_key_by_idx(zb_uint16_t idx);

zb_ret_t zb_secur_changing_tc_policy_check(void);

zb_aps_device_key_pair_set_t *zb_secur_get_link_key_pair_set(zb_ieee_addr_t address, zb_bool_t valid_only);

zb_aps_device_key_pair_set_t *zb_secur_get_verified_or_provisional_link_key(zb_ieee_addr_t address);

zb_aps_device_key_pair_set_t *zb_secur_create_best_suitable_link_key_pair_set(zb_ieee_addr_t address);

zb_ret_t zb_secur_ic_get_key_by_address(zb_ieee_addr_t address, zb_uint8_t *key);

#if defined ZB_COORDINATOR_ROLE && defined ZB_SECURITY_INSTALLCODES && defined ZB_USE_NVRAM && !defined APP_ONLY_NVRAM && !defined NCP_MODE_HOST
zb_int8_t zb_secur_ic_get_from_tc_storage(zb_ieee_addr_t address, zb_uint8_t *ic_type, zb_uint8_t *buf);
#endif

zb_uint8_t *zb_secur_ic_get_from_client_storage(zb_uint8_t* ic_type);

zb_ret_t zb_secur_ic_check_crc(zb_uint8_t ic_type, zb_uint8_t *ic);

zb_ret_t zb_secur_ic_from_string(char *ic_str, zb_uint8_t *ic_type, zb_uint8_t *ic);

zb_bool_t zb_secur_aps_cmd_is_encrypted_by_good_key(zb_uint8_t cmd_id, zb_uint16_t src_addr, zb_uint16_t keypair_i);

void zdo_secur_update_tclk_done(zb_uint8_t param);

#ifdef ZB_FORMATION
void secur_generate_key(zb_uint8_t *key);
#endif

zb_bool_t zb_secur_has_verified_key_by_short(zb_uint16_t addr_short);

void zb_secur_trace_all_key_pairs(zb_uint8_t param);

#ifdef DEBUG
#define ZB_SECUR_TRACE_ALL_KEY_PAIRS() zb_secur_trace_all_key_pairs(0)
#else
#define ZB_SECUR_TRACE_ALL_KEY_PAIRS()
#endif

#ifdef ZB_SE_KE_WHITELIST
void zb_secur_ke_whitelist_add(zb_ieee_addr_t addr);

void zb_secur_ke_whitelist_del(zb_ieee_addr_t addr);

void zb_secur_ke_whitelist_del_all(void);

zb_int16_t zb_secur_search_ke_whitelist(zb_ieee_addr_t addr);
#endif /* ZB_SE_KE_WHITELIST */

/*! @} */
/*! @endcond */

#ifdef ZB_USE_OSIF_OTA_ROUTINES
zb_bool_t zb_sec_b6_hash_iter_start(void *dev, zb_uint32_t input_addr, zb_uint32_t input_len);
void zb_sec_b6_hash_iter_done(void *dev, zb_uint32_t input_len, zb_uint8_t *calc_hash);
#endif

void zdo_initiate_tclk_gen_over_aps(zb_uint8_t param);

zb_uint8_t zb_secur_gen_upd_dev_status(zb_ushort_t rejoin_network, zb_ushort_t secure_rejoin);

#ifdef ZB_FORMATION
void secur_nwk_generate_key(zb_uint8_t i, zb_uint_t key_seq);

void secur_authenticate_child_directly(zb_uint8_t param);
#endif

#ifdef ZB_COORDINATOR_ROLE
zb_ret_t zb_authenticate_dev(zb_bufid_t param, zb_apsme_update_device_ind_t *ind);
#endif

zb_uint8_t zb_secur_aps_send_policy(zb_uint_t command, zb_uint16_t dest_addr, zb_uint8_t key_type);

#endif /* ZB_SECUR_H */
