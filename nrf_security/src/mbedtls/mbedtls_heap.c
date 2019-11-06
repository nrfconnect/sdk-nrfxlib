/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#include <init.h>
#include <app_memory/app_memdomain.h>

#include "mbedtls/memory_buffer_alloc.h"

#if !defined(CONFIG_MBEDTLS_HEAP_SIZE) || CONFIG_MBEDTLS_HEAP_SIZE == 0
#error "CONFIG_MBEDTLS_HEAP_SIZE must be specified and greater than 0"
#endif

static unsigned char mbedtls_heap[CONFIG_MBEDTLS_HEAP_SIZE];

static int mbedtls_heap_init(struct device *dev)
{
	ARG_UNUSED(dev);

	mbedtls_memory_buffer_alloc_init(mbedtls_heap, sizeof(mbedtls_heap));

	return 0;
}

/* Hw cc310 is initialized with CONFIG_KERNEL_INIT_PRIORITY_DEFAULT and the
 * heap must be initialized afterwards.
 */
SYS_INIT(mbedtls_heap_init, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEVICE);
