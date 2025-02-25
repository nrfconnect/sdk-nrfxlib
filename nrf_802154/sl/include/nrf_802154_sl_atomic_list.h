/*
 * Copyright (c) 2021, Nordic Semiconductor ASA
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

#ifndef NRF_802154_SL_ATOMIC_LIST_H__
#define NRF_802154_SL_ATOMIC_LIST_H__

#include <stddef.h>
#include <stdbool.h>

#include "nrf_802154_sl_atomics.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief Type representing a single-linked list */
typedef struct
{
    /**@brief Pointer to the head of the list. */
    void * volatile              p_head;
    /**@brief Counter incremented every time the list changes.*/
    nrf_802154_sl_atomic_uint8_t bump_counter;
} nrf_802154_sl_atomic_list_t;

/**@brief Structure that needs to be contained in every struct capable of being stored in a list.
 *
 * Pointers building a list are stored within @ref nrf_802154_sl_atomic_list_membership_capability_t .
 * Operations on a list are generic. To manipulate the list user must provide an offset
 * to field of type @ref nrf_802154_sl_atomic_list_membership_capability_t within an item structure
 * as a @c offsetof_membership_capability parameter.
 * See @ref nrf_802154_sl_atomic_list_insert_ordered for reference.
 * Items can be simultaneously member of many lists provided that the item structs have separate
 * fields of @ref nrf_802154_sl_atomic_list_membership_capability_t type for each list.
 */
typedef struct
{
    void * volatile p_next;
} nrf_802154_sl_atomic_list_membership_capability_t;

/**@brief Type of a function that compares two items with regard to it's own criteria.
 *
 * It is up to the implementation to decide what means less-than, equal and greater-than criteria.
 * Data required to perform comparison must be present within underlying type of items.
 *
 * @param[in] p_a   Pointer to left-hand operand of comparison.
 * @param[in] p_b   Pointer to right-hand operand of comparison.
 *
 * @retval -1       When *p_a < *p_b
 * @retval 0        When *p_a == *p_b
 * @retval 1        When *p_a > *p_b
 */
typedef int_fast8_t (* nrf_802154_sl_compare_func_t)(const void * p_a, const void * p_b);

/**@brief Initializes an empty list.
 *
 * Any API function for list manipulation is forbidden before call to this function.
 *
 * @param p_list    Pointer to the list to initialize.
 */
void nrf_802154_sl_atomic_list_init(nrf_802154_sl_atomic_list_t * p_list);

/**@brief Atomically inserts an item to an ordered list.
 *
 * @param p_list                         Pointer to the list the item is to be inserted.
 * @param p_item                         Pointer to the item to add.
 * @param offsetof_membership_capability Offset of @ref nrf_802154_sl_atomic_list_membership_capability_t
 *                                       structure to be used to build the list.
 * @param compare_func                   Pointer to a function to be called when iterating over list
 *                                       to select appropriate place to insert @p p_item. This
 *                                       function determines an order in the list.
 *                                       When inserting the list is traversed from the head until
 *                                       an @c entry if found for which compare_func(p_item, entry)
 *                                       returns -1. When such match is found the @p p_item is inserted
 *                                       before @c entry.
 */
void nrf_802154_sl_atomic_list_insert_ordered(
    nrf_802154_sl_atomic_list_t * p_list,
    void                        * p_item,
    size_t                        offsetof_membership_capability,
    nrf_802154_sl_compare_func_t  compare_func);

/**@brief Atomically removes an item from a list.
 *
 * @param p_list                    Pointer to the list from which the item is to be removed.
 * @param p_item                    Pointer to the item to be removed from the list.
 * @param offsetof_membership_capability Offset of @ref nrf_802154_sl_atomic_list_membership_capability_t
 *                                       structure to be used to build the list.
 *
 * @retval true     The item was found in the list and has been removed.
 * @retval false    The item was not a member of the list, has been not removed.
 */
bool nrf_802154_sl_atomic_list_remove(nrf_802154_sl_atomic_list_t * p_list,
                                      void                        * p_item,
                                      size_t                        offsetof_membership_capability);

/**@brief Peeks at the value of list head.
 *
 * @param p_list  Pointer to the list to peek at.
 *
 * @returns Pointer of list head.
 */
void * nrf_802154_sl_atomic_list_head_peek(nrf_802154_sl_atomic_list_t * p_list);

/**@brief Type of a function that checks if given item meets some user-defined criteria.
 *
 * @param p_item    Pointer to an item to check
 * @param p_param   Additional parameter provided to the function call. It is up to implementation
 *                  to provide a meaning of this parameter if any.
 * @retval true     User-defined criteria met.
 * @retval false    User-defined criteria not met.
 */
typedef bool (* nrf_802154_sl_checker_func_t)(const void * p_item, const void * p_param);

/**@brief Atomically removes the head item from a list if the head item meets user-defined criteria.
 *
 * @param p_list                         Pointer to a list from which the item is to be removed.
 * @param offsetof_membership_capability Offset of @ref nrf_802154_sl_atomic_list_membership_capability_t
 *                                       structure to be used to build the list.
 * @param checker_func                   Pointer to a function to be called to determine if an head
 *                                       item should be removed.
 * @param p_checker_func_param           Parameter passed to @p checker_func
 *
 * @retval NULL when there was no head item or the head item didn't pass criteria imposed by
 *              @p checker_func .
 * @retval other Pointer to the removed item.
 */
void * nrf_802154_sl_atomic_list_remove_head_if_criteria_met(
    nrf_802154_sl_atomic_list_t * p_list,
    size_t                        offsetof_membership_capability,
    nrf_802154_sl_checker_func_t  checker_func,
    const void                  * p_checker_func_param);

#ifdef __cplusplus
}
#endif

#endif // NRF_802154_SL_ATOMICS_H__
