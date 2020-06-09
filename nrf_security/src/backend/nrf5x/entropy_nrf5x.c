/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */
#include <assert.h>
#include <zephyr.h>
#include <device.h>
#include <drivers/entropy.h>
#include <mbedtls/entropy.h>

int mbedtls_hardware_poll(void *data,
                          unsigned char *output,
                          size_t len,
                          size_t *olen )
{
    struct device *dev;
    u8_t buffer[MBEDTLS_ENTROPY_MAX_GATHER] = { 0 };
    int ret;
    
    (void)data;

    if (output == NULL)
    {
        return -1;
    }

    if (olen == NULL)
    {
        return -1;
    }

    if (len == 0)
    {
        return -1;
    }

    if (len != MBEDTLS_ENTROPY_MAX_GATHER)
    {
        return -1;
    }

    dev = device_get_binding(DT_CHOSEN_ZEPHYR_ENTROPY_LABEL);

    if (!dev)
    {
        return MBEDTLS_ERR_ENTROPY_NO_SOURCES_DEFINED;
    }

    ret = entropy_get_entropy(dev, buffer, MBEDTLS_ENTROPY_MAX_GATHER);
    if (ret)
    {
        return MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;
    }

    return 0;
}
