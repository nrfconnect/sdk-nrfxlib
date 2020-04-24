/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2020 DSR Corporation, Denver CO, USA.
 * http://www.dsr-zboss.com
 * http://www.dsr-corporation.com
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
/*  PURPOSE: Channel page header file. All channel-related stuff.
*/
#ifndef ZB_CHANNEL_PAGE_H
#define ZB_CHANNEL_PAGE_H 1

#include "zb_types.h"
#include "zb_errors.h"

/*! \addtogroup channel_page */
/*! @{ */

#define ZB_CHANNEL_PAGE_PAGE_BITMASK           0xF8000000u
#define ZB_CHANNEL_PAGE_MASK_BITMASK           0x07FFFFFFu

/**
 * Represents the fields of the Channel Page structure (binary encoded channel
 * page and channels mask).
 */
typedef zb_uint32_t zb_channel_page_t;

/**
 * Channel list structure (ZBOSS internal)
 */
typedef zb_channel_page_t zb_channel_list_t[ZB_CHANNEL_PAGES_NUM];

/* Private API */
#define ZB_CHANNEL_LIST_PAGE0_IDX  0
#define ZB_CHANNEL_LIST_PAGE28_IDX 1
#define ZB_CHANNEL_LIST_PAGE29_IDX 2
#define ZB_CHANNEL_LIST_PAGE30_IDX 3
#define ZB_CHANNEL_LIST_PAGE31_IDX 4

#define ZB_PAGE0_2_4_GHZ_CHANNEL_FROM   11
#define ZB_PAGE0_2_4_GHZ_CHANNEL_TO     26

#define ZB_PAGE28_SUB_GHZ_CHANNEL_FROM  0
#define ZB_PAGE28_SUB_GHZ_CHANNEL_TO    26

#define ZB_PAGE29_SUB_GHZ_CHANNEL_FROM  27
#define ZB_PAGE29_SUB_GHZ_CHANNEL_TO    34

#define ZB_PAGE30_SUB_GHZ_CHANNEL_FROM  35
#define ZB_PAGE30_SUB_GHZ_CHANNEL_TO    61

#define ZB_PAGE31_SUB_GHZ_CHANNEL_FROM  0
#define ZB_PAGE31_SUB_GHZ_CHANNEL_TO    26

#define ZB_CHANNEL_PAGE_TO_IDX(channel_page) ((channel_page > 0) ? (channel_page - 27) : 0)
#define ZB_CHANNEL_PAGE_FROM_IDX(channel_page_idx) ((channel_page_idx > 0) ? (channel_page_idx + 27) : 0)

#ifdef ZB_PAGES_REMAP_TO_2_4GHZ
/**
   The following remap stuff allows to remap Sub GHz pages/channel to 2.4GHz.
   This is used to test Sub GHz without getting out from the town to desert
   island due to wide range of Sub GHz.

   Remap is based on using of channels range of 2.4GHz to imitate working with
   Sub GHz. The main idea is to devide channels range of 2.4GHz, where each
   channels area corresponds to particular channel page. We have 5 channel
   pages, so the range of channel numbers of 2.4GHz is devided into 5 areas:

   * 0-2   - PAGE0
   * 3-5   - PAGE28
   * 6-8   - PAGE29
   * 9-11  - PAGE30
   * 12-14 - PAGE31

   So whole range of used channel numbers starts from 0 and ends with 14
   (15 channels of 2.4GHz are used).

   The following formula is used to remap channel number into 2.4GHz channel number.
   * remap_channel_number = (channel_number % 3) + page_shift
 */

#define ZB_PAGES_REMAP_PAGE_CHANNELS_NUM 3

#define ZB_PAGES_REMAP_PAGE0_SHIFT  0
#define ZB_PAGES_REMAP_PAGE28_SHIFT (ZB_PAGES_REMAP_PAGE0_SHIFT + ZB_PAGES_REMAP_PAGE_CHANNELS_NUM)
#define ZB_PAGES_REMAP_PAGE29_SHIFT (ZB_PAGES_REMAP_PAGE28_SHIFT + ZB_PAGES_REMAP_PAGE_CHANNELS_NUM)
#define ZB_PAGES_REMAP_PAGE30_SHIFT (ZB_PAGES_REMAP_PAGE29_SHIFT + ZB_PAGES_REMAP_PAGE_CHANNELS_NUM)
#define ZB_PAGES_REMAP_PAGE31_SHIFT (ZB_PAGES_REMAP_PAGE30_SHIFT + ZB_PAGES_REMAP_PAGE_CHANNELS_NUM)

#define ZB_PAGES_REMAP_CHANNEL_PAGE ZB_CHANNEL_PAGE0_2_4_GHZ
#define ZB_PAGES_REMAP_CHANNEL_NUMBER(page, channel_number) (((channel_number % ZB_PAGES_REMAP_PAGE_CHANNELS_NUM)) + ZB_PAGE0_2_4_GHZ_START_CHANNEL_NUMBER + \
                                                             (page == ZB_CHANNEL_PAGE28_SUB_GHZ ? ZB_PAGES_REMAP_PAGE28_SHIFT : \
                                                              (page == ZB_CHANNEL_PAGE29_SUB_GHZ ? ZB_PAGES_REMAP_PAGE29_SHIFT : \
                                                               (page == ZB_CHANNEL_PAGE30_SUB_GHZ ? ZB_PAGES_REMAP_PAGE30_SHIFT : \
                                                                (page == ZB_CHANNEL_PAGE31_SUB_GHZ ? ZB_PAGES_REMAP_PAGE31_SHIFT : \
                                                                 ZB_PAGES_REMAP_PAGE0_SHIFT)))))

/**
  Remaps logical channel into correspondong logical channel in 2.4GHz range.

  @param channel_page - original channel page
  @param logical_channel - original logical channel in scope of original channel page
  @param remap_logical_channel [out] - remapped logical channel in scope of 2.4GHz

  @return remapped channel if success
          0xff in case of error
*/
zb_uint8_t zb_pages_remap_logical_channel(zb_uint8_t channel_page,
                                          zb_uint8_t logical_channel);

#define ZB_PAGES_REMAP_LOGICAL_CHANNEL(page, lchannel_number) zb_pages_remap_logical_channel(page, lchannel_number)

#else

#define ZB_PAGES_REMAP_LOGICAL_CHANNEL(page, lchannel_number) (lchannel_number)

#endif

/**
 * Gets channel page field from Channel page structure
 *
 * @param channel_page - binary encoded channel page and channels mask
 */
#define ZB_CHANNEL_PAGE_GET_PAGE(channel_page) (((channel_page) & ZB_CHANNEL_PAGE_PAGE_BITMASK) >> 27)

/**
 * Gets channels mask field from Channel page structure
 *
 * @param channel_page - binary encoded channel page and channels mask
 */
#define ZB_CHANNEL_PAGE_GET_MASK(channel_page) ((channel_page) & ZB_CHANNEL_PAGE_MASK_BITMASK)

/**
 * Sets channel page field for Channel page structure
 *
 * @param channel_page - binary encoded channel page and channels mask
 * @param page         - channel page to set
 */
#define ZB_CHANNEL_PAGE_SET_PAGE(channel_page, page) (channel_page) = ((channel_page) & ZB_CHANNEL_PAGE_MASK_BITMASK) | (((zb_uint32_t)(page)) << 27)

/**
 * Sets channels mask field for Channel page structure
 *
 * @param channel_page - binary encoded channel page and channels mask
 * @param mask         - channels mask to set
 */
#define ZB_CHANNEL_PAGE_SET_MASK(channel_page, mask) (channel_page) = ((channel_page) & ZB_CHANNEL_PAGE_PAGE_BITMASK) | ((mask) & ZB_CHANNEL_PAGE_MASK_BITMASK)

/* Numbers [28, 29, 30, 31] have common first three bits
 *
 * 28 = 0b11100
 * 29 = 0b11101
 * 30 = 0b11110
 * 31 = 0b11111
 *
 * So, can only compare with 0b11100 mask = 0x1C = 28 (dec)
 */
#define ZB_LOGICAL_PAGE_SUB_GHZ_PAGE_MASK ZB_CHANNEL_PAGE28_SUB_GHZ

#define ZB_LOGICAL_PAGE_IS_SUB_GHZ_EU_FSK(logical_page) \
  ((logical_page) & ZB_LOGICAL_PAGE_SUB_GHZ_PAGE_MASK)

#define ZB_LOGICAL_PAGE_IS_2_4GHZ(logical_page) \
  ((logical_page) == ZB_CHANNEL_PAGE0_2_4_GHZ)

#define ZB_CHANNEL_PAGE_IS_SUB_GHZ_EU_FSK(channel_page) \
  (ZB_LOGICAL_PAGE_IS_SUB_GHZ_EU_FSK(ZB_CHANNEL_PAGE_GET_PAGE(channel_page)))

#define ZB_CHANNEL_PAGE_IS_2_4GHZ(channel_page) \
  (ZB_LOGICAL_PAGE_IS_2_4GHZ(ZB_CHANNEL_PAGE_GET_PAGE(channel_page)))

#define ZB_CHANNEL_PAGE_IS_MASK_EMPTY(channel_page) \
  (ZB_CHANNEL_PAGE_GET_MASK(channel_page) == 0)

/**
 * @brief Copies source channel page list to the destination. Destination list
 * must be allocated.
 *
 * @note Number of elements in both lists must be equal to ZB_CHANNEL_PAGES_NUM.
 *
 * @param dst - destination channel page list
 * @param src - source channel page list
 */
void zb_channel_page_list_copy(zb_channel_list_t dst,
                                    zb_channel_list_t src);

/**
 * @brief Sets channels mask for specified element of channel page list.
 *
 * @param list - channel page list
 * @param idx  - index of the list element
 * @param mask - channels mask to set
 */
void zb_channel_page_list_set_mask(zb_channel_list_t list,
                                        zb_uint8_t         idx,
                                        zb_uint32_t        mask);

/**
 * @brief Gets channels mask of specified element of channel page list.
 *
 * @param list - channel page list
 * @param idx  - index of the list element
 *
 * @return Channels mask.
 */
zb_uint32_t zb_channel_page_list_get_mask(zb_channel_list_t list,
                                          zb_uint8_t         idx);

/**
 * @brief Sets channel page for specified element of channel page list.
 *
 * @param list - channel page list
 * @param idx  - index of the list element
 * @param page - channel page to set
 */
void zb_channel_page_list_set_page(zb_channel_list_t list,
                                        zb_uint8_t         idx,
                                        zb_uint8_t         page);

/**
 * @brief Gets channel page of specified element of channel page list.
 *
 * @param list - channel page list
 * @param idx  - index of the list element
 *
 * @return Channel page.
 */
zb_uint8_t zb_channel_page_list_get_page(zb_channel_list_t list,
                                         zb_uint8_t         idx);


/**
 * @brief Gets channel page index in channel lists.
 *
 * @param page - channel page
 * @param idx [out] - index of the channel page in channel lists
 *
 * @return RET_OK or RET_NOT_FOUND.
 */
zb_ret_t zb_channel_page_list_get_page_idx(zb_uint8_t page, zb_uint8_t *idx);


/**
 * @brief Gets page number by channel page index.
 *
 * @param idx  - index of the channel page in channel lists
 * @param page[out] - channel page
 *
 * @return RET_OK or RET_NOT_FOUND.
 */
zb_ret_t zb_channel_page_get_page_by_idx(zb_uint8_t idx, zb_uint8_t *page);

/**
 * @brief Sets channels mask for the list element of channel page list that
 * corresponds to 2.4GHz.
 *
 * @param list - channel page list
 * @param mask - channels mask to set
 */
void zb_channel_page_list_set_2_4GHz_mask(zb_channel_list_t list,
                                               zb_uint32_t        mask);

/**
 * @brief Gets channels mask of the list element of channel page list that
 * corresponds to 2.4GHz.
 *
 * @param list - channel page list
 *
 * @return 2.4GHz channels mask.
 */
zb_uint32_t zb_channel_page_list_get_2_4GHz_mask(zb_channel_list_t list);

/**
 * @brief Sets channels mask for specified element of the AIB channel page list.
 *
 * @param idx  - index of the list element
 * @param mask - channels mask to set
 */
void zb_aib_channel_page_list_set_mask(zb_uint8_t  idx,
                                            zb_uint32_t mask);

/**
 * @brief Gets channels mask of specified element of the AIB channel page list.
 *
 * @param idx  - index of the list element
 *
 * @return Channels mask.
 */
zb_uint32_t zb_aib_channel_page_list_get_mask(zb_uint8_t idx);

/**
 * @brief Sets channel page for specified element of the AIB channel page list.
 *
 * @param idx  - index of the list element
 * @param page - channel page to set
 */
void zb_aib_channel_page_list_set_page(zb_uint8_t idx,
                                            zb_uint8_t page);

/**
 * @brief Gets channels page of specified element of the AIB channel page list.
 *
 * @param idx  - index of the list element
 *
 * @return Channel page.
 */
zb_uint8_t zb_aib_channel_page_list_get_page(zb_uint8_t idx);

/**
 * @brief Sets channels mask for the list element of the AIB channel page list that
 * corresponds to 2.4GHz.
 *
 * @param mask - channels mask to set
 */
void zb_aib_channel_page_list_set_2_4GHz_mask(zb_uint32_t mask);

/**
 * @brief Gets channels mask of the list element of the AIB channel page list that
 * corresponds to 2.4GHz.
 *
 * @return 2.4GHz channels mask.
 */
zb_uint32_t zb_aib_channel_page_list_get_2_4GHz_mask(void);

/**
 * @brief Gets the first channel number for specified channel page.
 *
 * @note The channel number is returned in inner representation as channels bits
 *       stored in channel mask internally, it may differ from real channel numbers
 *       specified by spec.
 *
 * @param page - channel page
 * @param channel_number [out] - channel number
 *
 * @return RET_OK or RET_NOT_FOUND.
 */
zb_ret_t zb_channel_page_get_start_channel_number(zb_uint8_t page, zb_uint8_t *channel_number);

/**
 * @brief Gets the last channel number for specified channel page.
 *
 * @note The channel number is returned in inner representation as channels bits
 *       stored in channel mask internally, it may differ from real channel numbers
 *       specified by spec.
 *
 * @param page - channel page
 * @param channel_number [out] - channel number
 *
 * @return RET_OK or RET_NOT_FOUND.
 */
zb_ret_t zb_channel_page_get_max_channel_number(zb_uint8_t page, zb_uint8_t *channel_number);

/**
 * @brief Converts logical channel to channel number within channel mask for
 *        specified channel page. For example:
 *        - page [in] = 30
 *        - logical channel [in] = 36
 *        - channel number [out] = 1
 *
 * @param page [in] - channel page
 * @param logical_channel [in] - logical channel (e.g. simply channel)
 * @param channel_number [out] - channel number within channel mask (inner representation)
 *
 * @return RET_OK - success,
 *         RET_NOT_FOUND - unknown channel page,
 *         RET_INVALID_PARAMETER - unacceptable logical channel for specified channel page.
 */
zb_ret_t zb_channel_page_channel_logical_to_number(zb_uint8_t page,
                                                   zb_uint8_t logical_channel,
                                                   zb_uint8_t *channel_number);

/**
 * @brief Converts channel number within channel mask to logical channel for
 *        specified channel page. For example:
 *        - page [in] = 30
 *        - channel number [in] = 1
 *        - logical channel [out] = 36
 *
 * @param page - channel page
 * @param channel_number - channel number within channel mask (inner representation)
 * @param logical_channel [out] - logical channel (e.g. simply channel)
 *
 * @return RET_OK - success,
 *         RET_NOT_FOUND - unknown channel page,
 *         RET_INVALID_PARAMETER - unacceptable logical channel for specified channel page.
 */
zb_ret_t zb_channel_page_channel_number_to_logical(zb_uint8_t page,
                                                   zb_uint8_t channel_number,
                                                   zb_uint8_t *logical_channel);

/**
 * @brief Sets bit in channel mask corresponding to specified channel number.
 *
 * @param list - channel list
 * @param idx - index of channel page which channel mask will be modified
 * @param channel_number - channel number within channel mask (inner representation)
 *
 * @return RET_OK - success,
 *         RET_INVALID_PARAMETER - unacceptable channel number for specified channel page.
 */
zb_ret_t zb_channel_page_list_set_channel(zb_channel_list_t list,
                                          zb_uint8_t         idx,
                                          zb_uint8_t         channel_number);

/**
 * @brief Sets bit in channel mask corresponding to specified channel number.
 *
 * @param list - channel list
 * @param page - logicl page number
 * @param channel_number - logical channel number
 *
 * @return RET_OK - success,
 *         RET_INVALID_PARAMETER - unacceptable channel number for specified channel page.
 */
zb_ret_t zb_channel_page_list_set_logical_channel(zb_channel_list_t list,
                                                  zb_uint8_t         page,
                                                  zb_uint8_t         channel_number);



/**
 * @brief Unsets bit in channel mask corresponding to specified channel number.
 *
 * @param list - channel list
 * @param idx - index of channel page which channel mask will be modified
 * @param channel_number - channel number within channel mask (inner representation)
 *
 * @return RET_OK - success,
 *         RET_INVALID_PARAMETER - unacceptable channel number for specified channel page.
 */
zb_ret_t zb_channel_page_list_unset_channel(zb_channel_list_t list,
                                            zb_uint8_t         idx,
                                            zb_uint8_t         channel_number);

zb_uint8_t zb_channel_page_list_get_channels_num(zb_channel_list_t list,
                                                 zb_uint8_t         idx);


#ifdef ZB_MAC_CONFIGURABLE_TX_POWER
/**
 * @brief Get offsets for MAC TX power stored in the production config for
 * specified channel and page
 *
 * @param page - logicl page number
 * @param channel_number - logical channel number
 * @param array_idx [out] - pointer to store array id. The value -1 is an invalid value.
 * @param array_ofs [out] - pointer to store array offset. The value -1 is an invalid value.
 *
 */
void zb_channel_get_tx_power_offset(zb_uint8_t page, zb_uint8_t channel,
                                    zb_uint8_t *array_idx, zb_uint8_t *array_ofs);

#endif /* ZB_MAC_CONFIGURABLE_TX_POWER */


/*! @} */

#endif /* ZB_CHANNEL_PAGE_H */
