/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef RP_COMMON_H_
#define RP_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup rp_common Common RP serialization module
 * @{
 * @brief Common RP serialization API.
 */

/** @brief Internal macro used by @ref RP_CONCAT to perform the expansion in two steps. */
#define __RP_CONCAT(_x, _y) _x ## _y

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
#define RP_CONCAT(_x, _y) \
	__RP_CONCAT(_x, _y)

/** @brief Internal macro used by @ref RP_STRINGIFY to perform the expansion in two steps. */
#define __RP_STRINGIFY(_x) #_x

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
#define RP_STRINGIFY(_x) \
	__RP_STRINGIFY(_x)

/**
 * @brief Retrieve pointer to parent structure.
 *
 * @param[in] _ptr Pointer to data.
 * @param[in] _type Parent structure data type.
 * @param[in] _field_name Data field name in parent structure.
 *
 * @return Pointer to parent structure.
 */
#define RP_CONTAINER_OF(_ptr, _type, _field_name) \
	((_type *)(((uint8_t *)_ptr) - offsetof(_type, _field_name)))

#ifdef __GNUC__
#define __RP_STATIC_ASSERT(_expr, _msg) \
	_Static_assert(_expr, _msg)
#else
#define __RP_STATIC_ASSERT(_expr, _msg) \
	extern char (*_ignore(void))
#endif /* __GNUC__ */

#define RP_STATIC_ASSERT(...)           \
	__RP_STATIC_ASSERT(__VA_ARGS__)
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* RP_COMMON_H_ */
