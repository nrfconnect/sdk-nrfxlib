/*
 * Copyright (c) 2026, Nordic Semiconductor ASA
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
 * @brief This file defines an API for the Binary Search Map (BSMAP) module of the
 *        nRF 802.15.4 Radio Driver.
 *
 * @details
 * The BSMAP module provides a key-to-value mapping using a binary search algorithm.
 * It is designed to look up data associated with a given key as fast as possible,
 * from a high priority interrupt.
 * Modification (add, remove, update value) is expected to be performed from
 * priorities lower or equal than lookups and it is thread-safe with respect
 * to high priority look ups.
 * Modifications are not thread-safe with respect to each other. It is the caller's
 * responsibility to ensure thread-safety for this case. They are not intended to
 * be used from high priority interrupts.
 */

#ifndef NRF_802154_BSMAP_H_
#define NRF_802154_BSMAP_H_

#include <nrfx.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 * @brief Forward declaration.
 *
 * @note Use @c nrf_802154_bsmap_t typedef.
 */
struct nrf_802154_bsmap_s;

/**
 * @brief Type for object representing the BSMAP.
 *
 * @note Do not modify the fields directly, use the API functions only.
 */
typedef struct nrf_802154_bsmap_s nrf_802154_bsmap_t;

/**
 * @brief Type of an iterator used to iterate over the BSMAP.
 *
 * @note Do not modify the fields directly, use the API functions only.
 */
typedef struct
{
    /**
     * @brief Index to an internal array.
     */
    int32_t index;
    /**
     * @brief Indicates if direction of iteration is forward.
     */
    bool forward;
    /**
     * @brief Indicates if entry at index has been deleted.
     */
    bool deleted;
    /**
     * @brief Pointer to the BSMAP the iterator is iterating over.
     */
    nrf_802154_bsmap_t * p_bsmap;
} nrf_802154_bsmap_iterator_t;

/**
 * @brief Type of a structure holding the BSMAP object.
 *
 * @note Use @c nrf_802154_bsmap_t typedef.
 */
struct nrf_802154_bsmap_s
{
    /**
     * @brief Size of each key, in bytes.
     */
    uint16_t key_size;

    /**
     * @brief Size of each value associated with a key, in bytes.
     */
    uint16_t value_size;

    /**
     * @brief Size of one stored entry (key and value combined), in bytes.
     */
    uint16_t entry_size;

    /**
     * @brief Maximum number of entries the map can hold.
     */
    uint16_t capacity;

    /**
     * @brief Backing memory for all entries (each entry is key and value combined).
     */
    uint16_t keys_count;

    /**
     * @brief Pointer to a memory storage for entries.
     */
    void * p_memory;

    /**
     * @brief Index of the entry staged in @c p_aux_memory, or a sentinel when no entry is staged.
     */
    nrfx_atomic_t entry_idx_in_aux;

    /**
     * @brief Pointer to a temporary buffer used while modifying entries.
     */
    void * p_aux_memory;

    /**
     * @brief Iterator registered for this map, or NULL when no iteration is active.
     */
    nrf_802154_bsmap_iterator_t * p_iter;
};

/**
 * @brief Calculate the size of a single entry (key + value) in the BSMAP.
 *
 * This macro is internal but provided for @ref NRF_802154_BSMAP_AUX_MEMORY_SIZE.
 *
 * @param[in] key_size   The size of the key in bytes.
 * @param[in] value_size The size of the value associated with the key in bytes.
 */
#define NRF_802154_BSMAP_ENTRY_SIZE(key_size, value_size) ((key_size) + (value_size))

/**
 * @brief Calculate the size of the memory needed to store keys in the BSMAP.
 *
 * @param[in] key_size   The size of the key in bytes.
 * @param[in] value_size The size of the value associated with the key in bytes.
 * @param[in] capacity   The capacity (maximum number of keys) of the BSMAP.
 */
#define NRF_802154_BSMAP_MEMORY_SIZE(key_size, value_size, capacity) \
    (NRF_802154_BSMAP_ENTRY_SIZE((key_size), (value_size)) * (capacity))

/**
 * @brief Calculate the size of the auxiliary memory needed for the BSMAP.
 *
 * @param[in] key_size   The size of the key in bytes.
 * @param[in] value_size The size of the value associated with the key in bytes.
 */
#define NRF_802154_BSMAP_AUX_MEMORY_SIZE(key_size, value_size) \
    NRF_802154_BSMAP_ENTRY_SIZE((key_size), (value_size))

/**
 * @brief Initialize the BSMAP object.
 *
 * @param[out] p_bsmap          Pointer to the BSMAP object to initialize.
 * @param[in]  key_size         The size of the key in bytes. Must be greater than 0
 *                              and no more than @c UINT16_MAX .
 * @param[in]  value_size       The size of the value associated with the key in bytes.
 *                              If 0, the BSMAP will store only keys.
 *                              Must not be more than @c UINT16_MAX . Sum of the
 *                              key size and value size must also not be more than @c UINT16_MAX .
 * @param[in]  capacity         The capacity (maximum number of keys) of the BSMAP.
 *                              Must not be more than @c UINT16_MAX .
 * @param[in]  p_memory         Pointer to a memory storage for the BSMAP.
 *                              Must be able to store @ref NRF_802154_BSMAP_MEMORY_SIZE bytes.
 */
void nrf_802154_bsmap_init(nrf_802154_bsmap_t * p_bsmap,
                           size_t               key_size,
                           size_t               value_size,
                           size_t               capacity,
                           void               * p_memory);

/**
 * @brief Search for a record associated with the key in the BSMAP and copy the associated value
 *        into @p p_value if found.
 *
 * This function is optimized for high priority execution and is thread-safe with
 * respect to low priority modifications of the BSMAP performed by
 * @ref nrf_802154_bsmap_rec_write and @ref nrf_802154_bsmap_rec_delete functions.
 *
 * @param[in]  p_bsmap      Pointer to the BSMAP object to search in.
 * @param[in]  p_key        Pointer to the key to search for. Must not be NULL.
 * @param[out] p_value      Pointer to a variable where the value associated with the key
 *                          will be stored if the key is found. If NULL or the map does not store
 *                          values, just a search is performed without retrieving
 *                          the value. If not NULL, must point to a storage large enough
 *                          to store @c value_size bytes as passed to the
 *                          @ref nrf_802154_bsmap_init function.
 *
 * @retval true  If the key was found in the BSMAP.
 *               If @p p_value is not @c NULL, the value associated with the key is stored in it.
 * @retval false If the key was not found in the BSMAP.
 */
bool nrf_802154_bsmap_rec_get(const nrf_802154_bsmap_t * p_bsmap,
                              const void               * p_key,
                              void                     * p_value);

/**
 * @brief Write a record associated with a key into the BSMAP.
 *
 * If the key is not found in the BSMAP, a new entry is inserted into the BSMAP with
 * provided key and value. If the key is found in the BSMAP, the value associated with
 * the key is updated with the provided value.
 *
 * This function is optimized for low priority execution. It is thread-safe with respect to
 * high priority lookups performed by the @ref nrf_802154_bsmap_rec_get function.
 * This function is not thread-safe with respect to itself or
 * @ref nrf_802154_bsmap_rec_delete.
 *
 * @param[in] p_bsmap  Pointer to the BSMAP object to write to.
 * @param[in] p_key    Pointer to the key to write. Must not be NULL.
 * @param[in] p_value  Pointer to the value to write. If the BSMAP is initialized with
 *                     @c value_size equal to 0, this parameter is ignored and can
 *                     be NULL. Otherwise this parameter must not be NULL and must point
 *                     to a storage containing @c value_size bytes as passed to the
 *                     @ref nrf_802154_bsmap_init function.
 * @param[in] p_aux_memory Pointer to an auxiliary memory storage used during modification.
 *                         It must be able to store @ref NRF_802154_BSMAP_AUX_MEMORY_SIZE bytes.
 *
 * @retval true  If the record was successfully written to the BSMAP.
 * @retval false If the record was not written to the BSMAP because BSMAP is full.
 */
bool nrf_802154_bsmap_rec_write(nrf_802154_bsmap_t * p_bsmap,
                                const void         * p_key,
                                const void         * p_value,
                                void               * p_aux_memory);

/**
 * @brief Delete a record associated with a key from the BSMAP.
 *
 * This function is optimized for low priority execution. It is thread-safe with respect to
 * high priority lookups performed by the @ref nrf_802154_bsmap_rec_get function.
 *
 * This function is not thread-safe with respect to itself or
 * @ref nrf_802154_bsmap_rec_write.
 *
 * @param[in] p_bsmap  Pointer to the BSMAP object to write to.
 * @param[in] p_key    Pointer to the key to write. Must not be NULL.
 * @param[in] p_aux_memory Pointer to an auxiliary memory storage used during modification.
 *                         It must be able to store @ref NRF_802154_BSMAP_AUX_MEMORY_SIZE bytes.
 *
 * @retval true  If the record was found and deleted from the BSMAP.
 * @retval false If the record was not found in the BSMAP. This result means also that the goal of
 *               the function (not having the key in the BSMAP) is achieved.
 *               This does not necessarily mean that there is an error.
 */
bool nrf_802154_bsmap_rec_delete(nrf_802154_bsmap_t * p_bsmap,
                                 const void         * p_key,
                                 void               * p_aux_memory);

/**
 * @brief Remove all entries from the BSMAP.
 *
 * @param[in] p_bsmap Pointer to the BSMAP object to clear.
 */
void nrf_802154_bsmap_clear(nrf_802154_bsmap_t * p_bsmap);

/**
 * @brief Start iteration over the BSMAP.
 *
 * @note There can be only one iteration started on a given instance pointed by
 * @p p_bsmap.
 *
 * To check if iterator is valid (end not reached), call the
 * @ref nrf_802154_bsmap_iterator_is_valid function.
 *
 * To retrieve data through iterator, call the @ref nrf_802154_bsmap_iterator_rec_key_get and
 * @ref nrf_802154_bsmap_iterator_rec_value_get functions.
 *
 * To manipulate the BSMAP through the iterator, call the functions
 * @ref nrf_802154_bsmap_iterator_rec_value_write and
 * @ref nrf_802154_bsmap_iterator_rec_delete.
 *
 * To move the iterator to the next entry, call the @ref nrf_802154_bsmap_iterator_next function.
 *
 * Iteration over the BSMAP must end with a call to the @ref nrf_802154_bsmap_iterator_finish
 * function.
 *
 * @param[in]  p_bsmap  Pointer to the BSMAP object to iterate over.
 * @param[out] p_iter   Pointer to an iterator that will be prepared for iteration.
 * @param[in]  forward  @c true to iterate forward. @c false to iterate backward.
 */
void nrf_802154_bsmap_iterator_begin(nrf_802154_bsmap_t          * p_bsmap,
                                     nrf_802154_bsmap_iterator_t * p_iter,
                                     bool                          forward);

/**
 * @brief Check if iterator points to an existing item in the BSMAP.
 *
 * @param[in] p_iter    Pointer to an iterator to check.
 *
 * @retval false     The iterator does not describe any valid BSMAP entry, "end" of iteration
 *                   reached.
 * @retval true      The iterator describes a valid BSMAP entry, "end" of iteration not reached
 *                   yet.
 */
bool nrf_802154_bsmap_iterator_is_valid(const nrf_802154_bsmap_iterator_t * p_iter);

/**
 * @brief Get the key part of record at given iterator.
 *
 * @note The iterator must be valid by @ref nrf_802154_bsmap_iterator_is_valid.
 *
 * @param[in]  p_iter  Pointer to an iterator at which to read the record.
 * @param[out] p_key   Pointer to the buffer where the key of record is to be read into.
 */
void nrf_802154_bsmap_iterator_rec_key_get(const nrf_802154_bsmap_iterator_t * p_iter,
                                           void                              * p_key);

/**
 * @brief Get the value part of record at given iterator.
 *
 * @note The iterator must be valid by @ref nrf_802154_bsmap_iterator_is_valid.
 *
 * @param[in]  p_iter  Pointer to an iterator at which to read the record.
 * @param[out] p_value Pointer to the buffer where the value of record is to be read into.
 *                     If @c value_size passed to the @ref nrf_802154_bsmap_init function is 0,
 *                     this parameter is ignored and can be NULL. However, there
 *                     is no point in calling this function if the map does not store values.
 */
void nrf_802154_bsmap_iterator_rec_value_get(const nrf_802154_bsmap_iterator_t * p_iter,
                                             void                              * p_value);

/**
 * @brief Write the value part of record at given iterator.
 *
 * @note The iterator must be valid by @ref nrf_802154_bsmap_iterator_is_valid.
 *
 * @param[in] p_iter        Pointer to an iterator at which the record value to write.
 * @param[in] p_value       Pointer to the buffer containing the value to write.
 *                          If @c value_size passed to the @ref nrf_802154_bsmap_init function is
 *                          zero, this parameter is ignored and can be NULL. However, there
 *                          is no point in calling this function if the map does not store values.
 * @param[in] p_aux_memory  Pointer to an auxiliary memory storage used during modification.
 *                          It must be able to store @ref NRF_802154_BSMAP_AUX_MEMORY_SIZE bytes.
 */
void nrf_802154_bsmap_iterator_rec_value_write(
    const nrf_802154_bsmap_iterator_t * p_iter,
    const void                        * p_value,
    void                              * p_aux_memory);

/**
 * @brief Delete a record at given iterator.
 *
 * @note The iterator must be valid by @ref nrf_802154_bsmap_iterator_is_valid.
 * After the entry is deleted, the only allowed next call is @ref nrf_802154_bsmap_iterator_next.
 *
 * @param[in]  p_iter       Pointer to an iterator at which the record is to be deleted.
 * @param[in]  p_aux_memory Pointer to an auxiliary memory storage used during modification.
 *                          It must be able to store @ref NRF_802154_BSMAP_AUX_MEMORY_SIZE bytes.
 */
void nrf_802154_bsmap_iterator_rec_delete(
    nrf_802154_bsmap_iterator_t * p_iter,
    void                        * p_aux_memory);

/**
 * @brief Move the BSMAP iterator to the next position.
 *
 * @param[inout] p_iter    Pointer to an iterator to move.
 */
void nrf_802154_bsmap_iterator_next(nrf_802154_bsmap_iterator_t * p_iter);

/**
 * @brief Finish the iteration over a BSMAP the iterator is bound to.
 *
 * @param[in] p_iter    Pointer to the iterator to check.
 */
void nrf_802154_bsmap_iterator_finish(nrf_802154_bsmap_iterator_t * p_iter);

#endif /* NRF_802154_BSMAP_H_ */
