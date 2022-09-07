/*
 * Copyright (c) 2018-2021 T2 Software, Inc. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-T2-LC3
 */

#ifndef __OSAL_PACKED_H__
#define __OSAL_PACKED_H__

#include "osal.h"

#define READ_LE_8(_x)               (((uint8_t )(((uint8_t *)(_x))[0] << 0x00)))
#define READ_LE_16(_x)              (((uint16_t)(((uint8_t *)(_x))[0] << 0x00))   | \
                                       ((uint16_t)(((uint8_t *)(_x))[1] << 0x08)))
#define READ_LE_32(_x)              (((uint32_t)(((uint8_t *)(_x))[0] << 0x00))   | \
                                       ((uint32_t)(((uint8_t *)(_x))[1] << 0x08)) | \
                                       ((uint32_t)(((uint8_t *)(_x))[2] << 0x10)) | \
                                       ((uint32_t)(((uint8_t *)(_x))[3] << 0x18)))
#define READ_LE_64(_x)              (((uint64_t)(((uint8_t *)(_x))[0] << 0x00))   | \
                                       ((uint64_t)(((uint8_t *)(_x))[1] << 0x08)) | \
                                       ((uint64_t)(((uint8_t *)(_x))[2] << 0x10)) | \
                                       ((uint64_t)(((uint8_t *)(_x))[3] << 0x18)) | \
                                       ((uint64_t)(((uint8_t *)(_x))[4] << 0x20)) | \
                                       ((uint64_t)(((uint8_t *)(_x))[5] << 0x28)) | \
                                       ((uint64_t)(((uint8_t *)(_x))[6] << 0x30)) | \
                                       ((uint64_t)(((uint8_t *)(_x))[7] << 0x38)))

#define WRITE_LE_8(_x, _y)          do { \
                                       ((uint8_t *)(_x))[0] = (((uint8_t )(_y)) >> 0x00) & 0xFF; \
                                    } while(0)
#define WRITE_LE_16(_x, _y)         do { \
                                       ((uint8_t *)(_x))[0] = (((uint16_t)(_y)) >> 0x00) & 0xFF; \
                                       ((uint8_t *)(_x))[1] = (((uint16_t)(_y)) >> 0x08) & 0xFF; \
                                    } while(0)
#define WRITE_LE_32(_x, _y)         do { \
                                       ((uint8_t *)(_x))[0] = (((uint32_t)(_y)) >> 0x00) & 0xFF; \
                                       ((uint8_t *)(_x))[1] = (((uint32_t)(_y)) >> 0x08) & 0xFF; \
                                       ((uint8_t *)(_x))[2] = (((uint32_t)(_y)) >> 0x10) & 0xFF; \
                                       ((uint8_t *)(_x))[3] = (((uint32_t)(_y)) >> 0x18) & 0xFF; \
                                    } while(0)
#define WRITE_LE_64(_x, _y)         do { \
                                       ((uint8_t *)(_x))[0] = (((uint32_t)(_y)) >> 0x00) & 0xFF; \
                                       ((uint8_t *)(_x))[1] = (((uint32_t)(_y)) >> 0x08) & 0xFF; \
                                       ((uint8_t *)(_x))[2] = (((uint32_t)(_y)) >> 0x10) & 0xFF; \
                                       ((uint8_t *)(_x))[3] = (((uint32_t)(_y)) >> 0x18) & 0xFF; \
                                       ((uint8_t *)(_x))[4] = (((uint32_t)(_y)) >> 0x20) & 0xFF; \
                                       ((uint8_t *)(_x))[5] = (((uint32_t)(_y)) >> 0x28) & 0xFF; \
                                       ((uint8_t *)(_x))[6] = (((uint32_t)(_y)) >> 0x30) & 0xFF; \
                                       ((uint8_t *)(_x))[7] = (((uint32_t)(_y)) >> 0x38) & 0xFF; \
                                    } while(0)

#endif // __OSAL_PACKED_H__
