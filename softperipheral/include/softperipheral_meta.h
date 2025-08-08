/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef SOFTPERIPHERAL_META_H__
#define SOFTPERIPHERAL_META_H__

#include <stdint.h>

#define SOFTPERIPHERAL_META_SOFTPERIPHERAL_ID_SQSPI 0x45b1

#if (SOFTPERIPHERAL_META_HEADER_VERSION == 1)

#define SOFTPERIPHERAL_META_COMM_ID_REGIF       (1)
#define SOFTPERIPHERAL_META_COMM_ID_JOBLIST     (2)

#define SOFTPERIPHERAL_META_PLATFORM_DEVICE_15  (15)
#define SOFTPERIPHERAL_META_PLATFORM_DEVICE_20  (20)

#define SOFTPERIPHERAL_META_PLATFORM_PLATFORM_L (0)
#define SOFTPERIPHERAL_META_PLATFORM_PLATFORM_H (1)

#define SOFTPERIPHERAL_META_PLATFORM_SERIES_54  (54)

#ifndef SOFTPERIPHERAL_META_SELF_BOOTING
#define SOFTPERIPHERAL_META_SELF_BOOTING (0)
#endif

typedef struct
{
    union
    {
        struct
        {
            uint32_t magic          : 16;
            uint32_t header_version : 4;
            uint32_t comm_id        : 8;
            uint32_t reserved0      : 3;
            uint32_t self_boot      : 1; // If True, set INITPC to NVM address.
                                         // If False, copy fw_code_size bytes from NVM to fw_ram_base_addr
        };

        uint32_t w0_raw;
    };

    union
    {
        struct
        {
            uint16_t softperiph_id;

            union
            {
                struct
                {
                    uint16_t device   : 7;
                    uint16_t platform : 2;
                    uint16_t series   : 7;
                };

                uint16_t raw;
            }        platform;
        };

        uint32_t w1_raw;
    };

    union
    {
        struct
        {
            uint32_t patch : 16;
            uint32_t minor : 8;
            uint32_t major : 8;
        }        version;

        uint32_t w2_raw;
    };

    union
    {
        struct
        {
            uint16_t fw_code_size;      // size / 16
            uint16_t
                     fw_ram_total_size; // size / 16. Must also include code (RAM CODE region), code RAM (.data/.bss/.stack/.heap), shared and/or allocatable RAM from the FW.
        };

        uint32_t w3_raw;
    };

    union
    {
        uint32_t fw_code_addr; // Address of where the code is compiled to run from.
        uint32_t w4_raw;
    };

    union
    {
        uint32_t
                 fw_ram_base_addr; // Dest address for code in case self_boot = False. Also, the RAM start address set by the FW in case of any RAM it needs.
        uint32_t w5_raw;
    };

    union
    {
        struct
        {
            uint16_t fw_shared_ram_size; // size / 16
            uint16_t fw_shared_ram_addr_offset;
        };

        uint32_t w6_raw;
    };

    union
    {
        uint32_t fw_vpr_save_ctx_addr;
        uint32_t w7_raw;
    };
} softperipheral_metadata_t;

#elif  (SOFTPERIPHERAL_META_HEADER_VERSION == 2)

#define SOFTPERIPHERAL_META_COMM_ID_REGIF         (1)
#define SOFTPERIPHERAL_META_COMM_ID_JOBLIST       (2)

#define SOFTPERIPHERAL_META_PLATFORM_DEVICE_05    (1)
#define SOFTPERIPHERAL_META_PLATFORM_DEVICE_09    (2)
#define SOFTPERIPHERAL_META_PLATFORM_DEVICE_10    (4)
#define SOFTPERIPHERAL_META_PLATFORM_DEVICE_15    (8)
#define SOFTPERIPHERAL_META_PLATFORM_DEVICE_20    (16)
#define SOFTPERIPHERAL_META_PLATFORM_DEVICE_ANY   (31)

#define SOFTPERIPHERAL_META_PLATFORM_PLATFORM_L   (1)
#define SOFTPERIPHERAL_META_PLATFORM_PLATFORM_H   (2)
#define SOFTPERIPHERAL_META_PLATFORM_PLATFORM_ANY (3)

#define SOFTPERIPHERAL_META_PLATFORM_SERIES_54    (1)
#define SOFTPERIPHERAL_META_PLATFORM_SERIES_ANY   (1)

#ifndef SOFTPERIPHERAL_META_SELF_BOOTING
#define SOFTPERIPHERAL_META_SELF_BOOTING (0)
#endif

typedef struct
{
    union
    {
        struct
        {
            uint32_t magic          : 16;
            uint32_t header_version : 4;
            uint32_t comm_id        : 8;
            uint32_t reserved0      : 3;
            uint32_t self_boot      : 1; // If True, set INITPC to NVM address.
                                         // If False, copy fw_code_size bytes from NVM to fw_ram_base_addr
        };

        uint32_t w0_raw;
    };

    union
    {
        struct
        {
            uint16_t softperiph_id;

            union
            {
                struct
                {
                    uint16_t device   : 9;
                    uint16_t platform : 4;
                    uint16_t series   : 3;
                };

                uint16_t raw;
            }        platform;
        };

        uint32_t w1_raw;
    };

    union
    {
        struct
        {
            uint32_t patch : 16;
            uint32_t minor : 8;
            uint32_t major : 8;
        }        version;

        uint32_t w2_raw;
    };

    union
    {
        struct
        {
            uint16_t fw_code_size;      // size / 16
            uint16_t
                     fw_ram_total_size; // size / 16. Must also include code (RAM CODE region), code RAM (.data/.bss/.stack/.heap), shared and/or allocatable RAM from the FW.
        };

        uint32_t w3_raw;
    };

    uint32_t w4_raw;

    union
    {
        uint32_t
                 fw_ram_base_addr; // Dest address for code in case self_boot = False. Also, the RAM start address set by the FW in case of any RAM it needs.
        uint32_t w5_raw;
    };

    union
    {
        struct
        {
            uint16_t fw_shared_ram_size; // size / 16
            uint16_t fw_shared_ram_addr_offset;
        };

        uint32_t w6_raw;
    };
    uint32_t w7_raw;
} softperipheral_metadata_t;

#endif //SOFTPERIPHERAL_META_HEADER_VERSION

#endif // SOFTPERIPHEREAL_META_H__
