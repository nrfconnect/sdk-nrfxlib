/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef NRF_EMMC_H__
#define NRF_EMMC_H__

#include <nrfx.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SOFTPERIPHERAL_EMMC (1)
#if SOFTPERIPHERAL_EMMC
#include <nrf_sp_emmc.h>
#define NRF_EMMC_Type NRF_SP_EMMC_Type
#else
#define NRF_EMMC_Type NRF_EMMC_Type
#endif

/**
 * @defgroup nrf_emmc_hal EMMC HAL
 * @{
 * @brief   Hardware access layer for managing the EMMC peripheral.
 */

/** @brief EMMC tasks. */
typedef enum
{
    NRF_EMMC_TASK_START = offsetof(NRF_EMMC_Type, TASKS_START), /**< Trigger this task to send a command. */
    NRF_EMMC_TASK_RESET = offsetof(NRF_EMMC_Type,
                                   TASKS_RESET),                /**< Trigger this task to reset SEMMC to UNINITIALIZED state. */
} nrf_emmc_task_t;

/** @brief EMMC events. */
typedef enum
{
    NRF_EMMC_EVENT_XFERCOMPLETE    = offsetof(NRF_EMMC_Type, EVENTS_XFERCOMPLETE),    /**< Command or transfer completed. */
    NRF_EMMC_EVENT_ABORTED         = offsetof(NRF_EMMC_Type, EVENTS_ABORTED),         /**< Command/transfer aborted or timed out. */
    NRF_EMMC_EVENT_READYTOTRANSFER = offsetof(NRF_EMMC_Type, EVENTS_READYTOTRANSFER), /**< Ready to transfer. */
} nrf_emmc_event_t;

/** @brief EMMC interrupt masks. */
typedef enum
{
    NRF_EMMC_INT_XFERCOMPLETE_MASK    = SP_EMMC_INTEN_XFERCOMPLETE_Msk,    /**< Interrupt on transfer complete. */
    NRF_EMMC_INT_ABORTED_MASK         = SP_EMMC_INTEN_ABORTED_Msk,         /**< Interrupt on transfer aborted. */
    NRF_EMMC_INT_READYTOTRANSFER_MASK = SP_EMMC_INTEN_READYTOTRANSFER_Msk, /**< Interrupt on ready to transfer. */
} nrf_emmc_int_mask_t;

/** @brief EMMC response types. */
typedef enum
{
    NRF_EMMC_RESPONSE_NONE = SP_EMMC_COMMAND_CMD_RESPTYPE_NONE, /**< No response. */
    NRF_EMMC_RESPONSE_R1   = SP_EMMC_COMMAND_CMD_RESPTYPE_R1,   /**< R1 response. */
    NRF_EMMC_RESPONSE_R1B  = SP_EMMC_COMMAND_CMD_RESPTYPE_R1B,  /**< R1 response with busy signal. */
    NRF_EMMC_RESPONSE_R2   = SP_EMMC_COMMAND_CMD_RESPTYPE_R2,   /**< R2 response. */
    NRF_EMMC_RESPONSE_R3   = SP_EMMC_COMMAND_CMD_RESPTYPE_R3,   /**< R3 response. */
    NRF_EMMC_RESPONSE_R4   = SP_EMMC_COMMAND_CMD_RESPTYPE_R4,   /**< R4 response. */
    NRF_EMMC_RESPONSE_R5   = SP_EMMC_COMMAND_CMD_RESPTYPE_R5,   /**< R5 response. */
} nrf_emmc_response_type_t;

/** @brief EMMC response processing types. */
typedef enum
{
    NRF_EMMC_RESPONSE_PROC_PROCESS = SP_EMMC_COMMAND_CMD_RESPPROC_PROCESS, /**< Process the response. */
    NRF_EMMC_RESPONSE_PROC_IGNORE  = SP_EMMC_COMMAND_CMD_RESPPROC_IGNORE,  /**< Ignore the response. */
} nrf_emmc_response_proc_t;

/** @brief EMMC bus width configurations. */
typedef enum
{
    NRF_EMMC_BUS_WIDTH_1_LANE  = SP_EMMC_CONFIG_BUSWIDTH_BUSWIDTH_ONELANE,    /**< 1-lane data transfer. */
    NRF_EMMC_BUS_WIDTH_4_LANES = SP_EMMC_CONFIG_BUSWIDTH_BUSWIDTH_FOURLANES,  /**< 4-lane data transfer. */
    NRF_EMMC_BUS_WIDTH_8_LANES = SP_EMMC_CONFIG_BUSWIDTH_BUSWIDTH_EIGHTLANES, /**< 8-lane data transfer. */
} nrf_emmc_bus_width_t;

/** @brief EMMC configuration structure. */
typedef struct
{
    uint32_t             clkfreqhz;         /**< Clock frequency in Hz. */
    nrf_emmc_bus_width_t bus_width;         /**< Bus width configuration. */
    bool                 ready_to_transfer; /**< Ready to transfer flag. */
    uint32_t             read_delay;
} nrf_emmc_config_t;

/** @brief EMMC command structure. */
typedef struct
{
    uint16_t                 idx;       /**< Command index. */
    nrf_emmc_response_type_t resp_type; /**< Response type. */
    nrf_emmc_response_proc_t resp_proc; /**< Response processing. */
    uint32_t                 arg;       /**< Command argument. */
    uint32_t                 resp_addr; /**< Response address. */
} nrf_emmc_command_t;

/** @brief EMMC data transfer structure. */
typedef struct
{
    uint32_t buffer_addr; /**< Buffer address. */
    // uint32_t target_addr; /**< Target address. */
    uint32_t block_size;  /**< Block size. */
    uint32_t block_num;   /**< Number of blocks. */
} nrf_emmc_data_t;

NRF_STATIC_INLINE void nrf_emmc_task_trigger(NRF_EMMC_Type * p_reg, nrf_emmc_task_t task);

NRF_STATIC_INLINE void nrf_emmc_enable(NRF_EMMC_Type * p_reg);

NRF_STATIC_INLINE void nrf_emmc_disable(NRF_EMMC_Type * p_reg);

NRF_STATIC_INLINE bool nrf_emmc_enable_check(NRF_EMMC_Type const * p_reg);

NRF_STATIC_INLINE void nrf_emmc_event_clear(NRF_EMMC_Type * p_reg, nrf_emmc_event_t event);

NRF_STATIC_INLINE bool nrf_emmc_event_check(NRF_EMMC_Type const * p_reg, nrf_emmc_event_t event);

NRF_STATIC_INLINE uint32_t nrf_emmc_event_address_get(NRF_EMMC_Type const * p_reg,
                                                      nrf_emmc_event_t      event);

NRF_STATIC_INLINE void nrf_emmc_int_enable(NRF_EMMC_Type * p_reg, uint32_t mask);

NRF_STATIC_INLINE uint32_t nrf_emmc_int_enable_check(NRF_EMMC_Type const * p_reg, uint32_t mask);

NRF_STATIC_INLINE void nrf_emmc_int_disable(NRF_EMMC_Type * p_reg, uint32_t mask);

NRF_STATIC_INLINE void nrf_emmc_config_set(NRF_EMMC_Type *           p_reg,
                                           nrf_emmc_config_t const * p_config);

NRF_STATIC_INLINE void nrf_emmc_config_get(NRF_EMMC_Type const * p_reg,
                                           nrf_emmc_config_t *   p_config);

NRF_STATIC_INLINE void nrf_emmc_command_set(NRF_EMMC_Type *            p_reg,
                                            nrf_emmc_command_t const * p_cmd);

NRF_STATIC_INLINE void nrf_emmc_config_set_ready_to_transfer(NRF_EMMC_Type *           p_reg,
                                                             nrf_emmc_config_t const * p_config);

NRF_STATIC_INLINE void nrf_emmc_config_get_ready_to_transfer(NRF_EMMC_Type const * p_reg,
                                                             nrf_emmc_config_t *   p_config);

NRF_STATIC_INLINE void nrf_emmc_command_set_response(NRF_EMMC_Type *  p_reg,
                                                     const uint32_t * response);

NRF_STATIC_INLINE void nrf_emmc_command_get_response(NRF_EMMC_Type const * p_reg,
                                                     uint32_t *            response);

NRF_STATIC_INLINE void nrf_emmc_command_get(NRF_EMMC_Type const * p_reg,
                                            nrf_emmc_command_t *  p_cmd);

NRF_STATIC_INLINE void nrf_emmc_data_set(NRF_EMMC_Type * p_reg, nrf_emmc_data_t const * p_data);

NRF_STATIC_INLINE void nrf_emmc_data_get(NRF_EMMC_Type const * p_reg, nrf_emmc_data_t * p_data);

NRF_STATIC_INLINE void nrf_emmc_set_num_blocks(NRF_EMMC_Type *         p_reg,
                                               nrf_emmc_data_t const * p_data);

NRF_STATIC_INLINE void nrf_emmc_get_num_blocks(NRF_EMMC_Type const * p_reg,
                                               nrf_emmc_data_t *     p_data);

NRF_STATIC_INLINE void nrf_emmc_status_set(NRF_EMMC_Type * p_reg,
                                           uint32_t        error_mask);

NRF_STATIC_INLINE uint32_t nrf_emmc_status_get(NRF_EMMC_Type const * p_reg);

NRF_STATIC_INLINE bool nrf_emmc_status_error_check(uint32_t status);

NRF_STATIC_INLINE void nrf_emmc_handshake_set(NRF_EMMC_Type * p_reg, uint32_t val, uint8_t idx);

NRF_STATIC_INLINE uint32_t nrf_emmc_handshake_get(NRF_EMMC_Type const * p_reg, uint8_t idx);

#ifndef NRF_DECLARE_ONLY

NRF_STATIC_INLINE void nrf_emmc_task_trigger(NRF_EMMC_Type * p_reg, nrf_emmc_task_t task)
{
    *((volatile uint32_t *)((uint8_t *)p_reg + (uint32_t)task)) = 0x1UL;
}

NRF_STATIC_INLINE void nrf_emmc_enable(NRF_EMMC_Type * p_reg)
{
    p_reg->ENABLE = SP_EMMC_ENABLE_ENABLE_Enabled;
}

NRF_STATIC_INLINE void nrf_emmc_disable(NRF_EMMC_Type * p_reg)
{
    p_reg->ENABLE = SP_EMMC_ENABLE_ENABLE_Disabled;
}

NRF_STATIC_INLINE bool nrf_emmc_enable_check(NRF_EMMC_Type const * p_reg)
{
    return (bool)(p_reg->ENABLE & SP_EMMC_ENABLE_ENABLE_Msk);
}

NRF_STATIC_INLINE void nrf_emmc_event_clear(NRF_EMMC_Type * p_reg, nrf_emmc_event_t event)
{
    *((volatile uint32_t *)((uint8_t *)p_reg + (uint32_t)event)) = 0x0UL;
    nrf_event_readback((uint8_t *)p_reg + (uint32_t)event);
}

NRF_STATIC_INLINE bool nrf_emmc_event_check(NRF_EMMC_Type const * p_reg, nrf_emmc_event_t event)
{
    return nrf_event_check(p_reg, event);
}

NRF_STATIC_INLINE uint32_t nrf_emmc_event_address_get(NRF_EMMC_Type const * p_reg,
                                                      nrf_emmc_event_t      event)
{
    return nrf_task_event_address_get(p_reg, event);
}

NRF_STATIC_INLINE void nrf_emmc_int_enable(NRF_EMMC_Type * p_reg, uint32_t mask)
{
    p_reg->INTEN |= mask;
}

NRF_STATIC_INLINE uint32_t nrf_emmc_int_enable_check(NRF_EMMC_Type const * p_reg, uint32_t mask)
{
    return p_reg->INTEN & mask;
}

NRF_STATIC_INLINE void nrf_emmc_int_disable(NRF_EMMC_Type * p_reg, uint32_t mask)
{
    p_reg->INTEN &= ~mask;
}

NRF_STATIC_INLINE void nrf_emmc_config_set(NRF_EMMC_Type *           p_reg,
                                           nrf_emmc_config_t const * p_config)
{
    p_reg->CONFIG.CLKFREQHZ = p_config->clkfreqhz;
    p_reg->CONFIG.BUSWIDTH  = ((uint32_t)p_config->bus_width) &
                              SP_EMMC_CONFIG_BUSWIDTH_BUSWIDTH_Msk;
    p_reg->CONFIG.READDELAY       = (uint32_t)p_config->read_delay;
    p_reg->CONFIG.READYTOTRANSFER = p_config->ready_to_transfer
                                        ? SP_EMMC_CONFIG_READYTOTRANSFER_READYTOTRANSFER_Ready
                                        : SP_EMMC_CONFIG_READYTOTRANSFER_READYTOTRANSFER_NotReady;
}

NRF_STATIC_INLINE void nrf_emmc_config_get(NRF_EMMC_Type const * p_reg,
                                           nrf_emmc_config_t *   p_config)
{
    p_config->clkfreqhz = p_reg->CONFIG.CLKFREQHZ;
    p_config->bus_width = (nrf_emmc_bus_width_t)(p_reg->CONFIG.BUSWIDTH &
                                                 SP_EMMC_CONFIG_BUSWIDTH_BUSWIDTH_Msk);
    p_config->read_delay        = p_reg->CONFIG.READDELAY;
    p_config->ready_to_transfer = p_reg->CONFIG.READYTOTRANSFER;
}

NRF_STATIC_INLINE void nrf_emmc_config_set_ready_to_transfer(NRF_EMMC_Type *           p_reg,
                                                             nrf_emmc_config_t const * p_config)
{
    p_reg->CONFIG.READYTOTRANSFER = p_config->ready_to_transfer
                                        ? SP_EMMC_CONFIG_READYTOTRANSFER_READYTOTRANSFER_Ready
                                        : SP_EMMC_CONFIG_READYTOTRANSFER_READYTOTRANSFER_NotReady;
}

NRF_STATIC_INLINE void nrf_emmc_config_get_ready_to_transfer(NRF_EMMC_Type const * p_reg,
                                                             nrf_emmc_config_t *   p_config)
{
    p_config->ready_to_transfer = (p_reg->CONFIG.READYTOTRANSFER &
                                   SP_EMMC_CONFIG_READYTOTRANSFER_READYTOTRANSFER_Msk) !=
                                  0;
}

NRF_STATIC_INLINE void nrf_emmc_command_set(NRF_EMMC_Type * p_reg, nrf_emmc_command_t const * p_cmd)
{
    p_reg->COMMAND.CMD = ((p_cmd->idx
                           << SP_EMMC_COMMAND_CMD_IDX_Pos) & SP_EMMC_COMMAND_CMD_IDX_Msk) |
                         ((p_cmd->resp_type
                           << SP_EMMC_COMMAND_CMD_RESPTYPE_Pos) &
                          SP_EMMC_COMMAND_CMD_RESPTYPE_Msk) |
                         ((p_cmd->resp_proc
                           << SP_EMMC_COMMAND_CMD_RESPPROC_Pos) & SP_EMMC_COMMAND_CMD_RESPPROC_Msk);

    p_reg->COMMAND.ARG          = p_cmd->arg;
    p_reg->COMMAND.RESPONSEADDR = p_cmd->resp_addr;
}

NRF_STATIC_INLINE void nrf_emmc_command_get(NRF_EMMC_Type const * p_reg, nrf_emmc_command_t * p_cmd)
{
    uint32_t cmd_reg = p_reg->COMMAND.CMD;

    p_cmd->idx = (uint16_t)((cmd_reg & SP_EMMC_COMMAND_CMD_IDX_Msk)
                            >> SP_EMMC_COMMAND_CMD_IDX_Pos);
    p_cmd->resp_type = (nrf_emmc_response_type_t)((cmd_reg & SP_EMMC_COMMAND_CMD_RESPTYPE_Msk)
                                                  >> SP_EMMC_COMMAND_CMD_RESPTYPE_Pos);
    p_cmd->resp_proc = (nrf_emmc_response_proc_t)((cmd_reg & SP_EMMC_COMMAND_CMD_RESPPROC_Msk)
                                                  >> SP_EMMC_COMMAND_CMD_RESPPROC_Pos);
    p_cmd->arg       = p_reg->COMMAND.ARG;
    p_cmd->resp_addr = p_reg->COMMAND.RESPONSEADDR;
}

NRF_STATIC_INLINE void nrf_emmc_command_set_response(NRF_EMMC_Type *  p_reg,
                                                     const uint32_t * response)
{
    for (uint8_t i = SP_EMMC_COMMAND_RESPONSE_MinIndex; i < SP_EMMC_COMMAND_RESPONSE_MaxCount; i++)
    {
        p_reg->COMMAND.RESPONSE[i] = response[i];
    }
}

NRF_STATIC_INLINE void nrf_emmc_command_get_response(NRF_EMMC_Type const * p_reg,
                                                     uint32_t *            response)
{
    for (uint8_t i = SP_EMMC_COMMAND_RESPONSE_MinIndex; i < SP_EMMC_COMMAND_RESPONSE_MaxCount; i++)
    {
        response[i] = p_reg->COMMAND.RESPONSE[i];
    }
}

NRF_STATIC_INLINE void nrf_emmc_data_set(NRF_EMMC_Type * p_reg, nrf_emmc_data_t const * p_data)
{
    p_reg->DATA.BUFFERADDR = p_data->buffer_addr;
    // p_reg->DATA.TARGETADDR = p_data->target_addr;
    p_reg->DATA.BLOCKSIZE = p_data->block_size;
    p_reg->DATA.BLOCKNUM  = p_data->block_num;
}

NRF_STATIC_INLINE void nrf_emmc_data_get(NRF_EMMC_Type const * p_reg, nrf_emmc_data_t * p_data)
{
    p_data->buffer_addr = p_reg->DATA.BUFFERADDR;
    // p_data->target_addr = p_reg->DATA.TARGETADDR;
    p_data->block_size = p_reg->DATA.BLOCKSIZE;
    p_data->block_num  = p_reg->DATA.BLOCKNUM;
}

NRF_STATIC_INLINE void nrf_emmc_set_num_blocks(NRF_EMMC_Type *         p_reg,
                                               nrf_emmc_data_t const * p_data)
{
    p_reg->DATA.BLOCKNUM = p_data->block_num;
}

NRF_STATIC_INLINE void nrf_emmc_get_num_blocks(NRF_EMMC_Type const * p_reg,
                                               nrf_emmc_data_t *     p_data)
{
    p_data->block_num = p_reg->DATA.BLOCKNUM;
}

NRF_STATIC_INLINE void nrf_emmc_status_set(NRF_EMMC_Type * p_reg,
                                           uint32_t        error_mask)
{
    p_reg->STATUS.STATUS = error_mask;
}

NRF_STATIC_INLINE uint32_t nrf_emmc_status_get(NRF_EMMC_Type const * p_reg)
{
    return p_reg->STATUS.STATUS;
}

NRF_STATIC_INLINE bool nrf_emmc_status_error_check(uint32_t status)
{
    return (status &
            (SP_EMMC_STATUS_STATUS_CMDTIMEOUT_Msk | SP_EMMC_STATUS_STATUS_CMDCRCERROR_Msk |
             SP_EMMC_STATUS_STATUS_DATACRCERROR_Msk |
             SP_EMMC_STATUS_STATUS_RETRYEXCEEDED_Msk | SP_EMMC_STATUS_STATUS_PROTOCOLERR_Msk)) !=
           0;
}

NRF_STATIC_INLINE void nrf_emmc_handshake_set(NRF_EMMC_Type * p_reg, uint32_t val, uint8_t idx)
{
    p_reg->SPSYNC.AUX[idx] = val;
}

NRF_STATIC_INLINE uint32_t nrf_emmc_handshake_get(NRF_EMMC_Type const * p_reg, uint8_t idx)
{
    return p_reg->SPSYNC.AUX[idx];
}

#endif // NRF_DECLARE_ONLY

/** @} */

#ifdef __cplusplus
}
#endif

#endif // NRF_EMMC_H__
