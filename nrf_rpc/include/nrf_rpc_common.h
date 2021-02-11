/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef _NRF_RPC_COMMON_H_
#define _NRF_RPC_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup nrf_rpc_common Common nRF RPC utility
 * @{
 * @ingroup nrf_rpc
 *
 * @brief Common utility macros used internally in nRF RPC.
 */

/** @brief Internal macro used by @ref NRF_RPC_CONCAT to perform the expansion
 * in two steps.
 */
#define __NRF_RPC_CONCAT(_x, _y) _x ## _y

/**
 * @brief Macro for concatenating two tokens in macro expansion.
 *
 * @note This macro is expanded in two steps so that tokens given as macros
 *       themselves are fully expanded before they are merged.
 *
 * @param[in] _x First token.
 * @param[in] _y Second token.
 *
 * @return The two tokens merged into one, unless they cannot together form
 *         a valid token (in such case, the preprocessor issues a warning and
 *         does not perform the concatenation).
 */
#define NRF_RPC_CONCAT(_x, _y) \
	__NRF_RPC_CONCAT(_x, _y)

/** @brief Internal macro used by @ref NRF_RPC_STRINGIFY to perform the
 * expansion in two steps.
 */
#define __NRF_RPC_STRINGIFY(_x) #_x

/**
 * @brief Macro for converting macro argument into a string constans.
 *
 * @note This macro is expanded in two steps so that tokens given as macros
 *       themselves are fully expanded before they are merged.
 *
 * @param[in] _x Macro argument.
 *
 * @return String constant.
 */
#define NRF_RPC_STRINGIFY(_x) \
	__NRF_RPC_STRINGIFY(_x)

/**
 * @brief Retrieve pointer to parent structure.
 *
 * @param[in] _ptr Pointer to data.
 * @param[in] _type Parent structure data type.
 * @param[in] _field_name Data field name in parent structure.
 *
 * @return Pointer to parent structure.
 */
#define NRF_RPC_CONTAINER_OF(_ptr, _type, _field_name) \
	((_type *)(((uint8_t *)_ptr) - offsetof(_type, _field_name)))

#ifdef __GNUC__
#define __NRF_RPC_STATIC_ASSERT(_expr, _msg) \
	_Static_assert(_expr, _msg)
#else
#define __NRF_RPC_STATIC_ASSERT(_expr, _msg) \
	extern char (*_ignore(void))
#endif /* __GNUC__ */

/**
 * @brief Compile-time assert.
 *
 * @param _expr Condition that have to be met.
 * @param _msg  Message shown when condition is not met.
 */
#define NRF_RPC_STATIC_ASSERT(_expr, _msg) \
	__NRF_RPC_STATIC_ASSERT(_expr, _msg)

#if defined(__ASSERT)
#define __NRF_RPC_ASSERT(_expr) __ASSERT(_expr, "nRF RPC assertion failed")
#else
#include <assert.h>
#define __NRF_RPC_ASSERT assert
#endif

/** @brief Runtime assert.
 *
 * @param _expr Condition that have to be met.
 */
#define NRF_RPC_ASSERT(_expr) \
	__NRF_RPC_ASSERT(_expr)

/** @brief Creates new automatically registered array.
 *
 * @param _name      Name of the array. Can be used later for
 *                   @a NRF_RPC_AUTO_ARR_FOR.
 * @param _array_key String array unique key that will be used to add items with
 *                   @a NRF_RPC_AUTO_ARR_ITEM.
 */
#define NRF_RPC_AUTO_ARR(_name, _array_key)				       \
	const uint8_t NRF_RPC_CONCAT(_name, _auto_arr_end) __used	       \
	__attribute__((__section__(".nrf_rpc." _array_key ".c")));	       \
	const uint8_t *const _name __used				       \
	__attribute__((__section__(".nrf_rpc." _array_key ".a"))) =	       \
		&NRF_RPC_CONCAT(_name, _auto_arr_end)

/** @brief Adds new variable to the array.
 *
 * @param _type      Type of the variable.
 * @param _name      Name of the variable.
 * @param _array_key String array unique key.
 * @param _item_key  String item key.
 */
#define NRF_RPC_AUTO_ARR_ITEM(_type, _name, _array_key, _item_key)	       \
	_type _name __used						       \
	__attribute__((__section__(".nrf_rpc." _array_key ".b." _item_key)))

/** @brief Iterate over array items.
 *
 * This macro must be placed in `for` brackets.
 *
 * @param _it        Pointer variable of type `void *` for iterator.
 * @param _var       Pointer variable that holds each item of the array.
 * @param _array_ptr Pointer to array.
 * @param _type      Type of items in array.
 */
#define NRF_RPC_AUTO_ARR_FOR(_it, _var, _array_ptr, _type)		       \
	(_var) = (_type *)((const uint8_t *const *)(_array_ptr) + 1);	       \
		(const uint8_t *const)_var <				       \
			*(const uint8_t *const *)(_array_ptr);		       \
		(_var) = (_type *)(_var) + 1, (void)_it

/** @brief Get item from the array.
 *
 * @param _array_ptr Pointer to array.
 * @param _index     Index of the item.
 * @param _type      Type of items in array.
 */
#define NRF_RPC_AUTO_ARR_GET(_array_ptr, _index, _type)			       \
	(((_type *)((const uint8_t *const *)(_array_ptr) + 1))[_index])

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* _NRF_RPC_COMMON_H_ */
