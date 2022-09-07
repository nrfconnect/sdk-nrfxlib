/*
 * Copyright (c) 2018-2021 T2 Software, Inc. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-T2-LC3
 */

#ifndef __OSAL_TYPES_H__
#define __OSAL_TYPES_H__

#include <stdint.h>
#include <limits.h>

/**@brief OSAL calling convention.
 */
#define OSALCALL

 /**@brief Defines packed message macros.
  */
#define PACKED_BEGIN
#define PACKED_END

/**@brief Defines the OSAL boolean value.
 */

typedef int                bool_t;

#ifndef TRUE
#define TRUE               (1 == 1)
#endif

#ifndef FALSE
#define FALSE              (1 == 2)
#endif

#ifndef NULL
#define NULL               (void *)0
#endif

#endif // __OSAL_TYPES_H__
