/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef NRF_CAN_H__
#define NRF_CAN_H__

#include <nrfx.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SOFTPERIPHERAL_CAN (1)
#if SOFTPERIPHERAL_CAN
#include <nrf_sp_can.h>
#define NRF_CAN_Type NRF_SP_CAN_Type
#else
#define NRF_CAN_Type NRF_CAN_Type
#endif

/**
 * @defgroup nrf_can_hal CAN HAL
 * @{
 * @brief   Hardware access layer for managing the CAN peripheral.
 */

/** @brief CAN tasks. */
typedef enum
{
    NRF_CAN_TASK_START = offsetof(NRF_CAN_Type, TASKS_START), /**< Trigger this task to start a request (RX/TX). */
    NRF_CAN_TASK_RESET = offsetof(NRF_CAN_Type,
                                  TASKS_RESET),               /**< Trigger this task to reset SCAN to UNINITIALIZED state. */
} nrf_can_task_t;

/** @brief CAN events. */
typedef enum
{
    NRF_CAN_EVENT_TXCOMPLETE    = offsetof(NRF_CAN_Type, EVENTS_TXCOMPLETE),    /**< TX completed. */
    NRF_CAN_EVENT_RXCOMPLETE    = offsetof(NRF_CAN_Type, EVENTS_RXCOMPLETE),    /**< RX completed after ID match. */
    NRF_CAN_EVENT_ERRORDETECTED = offsetof(NRF_CAN_Type, EVENTS_ERRORDETECTED), /**< Error detected. */
    NRF_CAN_EVENT_STATECHANGED  = offsetof(NRF_CAN_Type, EVENTS_STATECHANGED),  /**< CAN controller state changed. */
} nrf_can_event_t;

/** @brief CAN interrupt masks. */
typedef enum
{
    NRF_CAN_INT_TXCOMPLETE_MASK    = SP_CAN_INTEN_TXCOMPLETE_Msk,    /**< Interrupt on TX complete. */
    NRF_CAN_INT_RXCOMPLETE_MASK    = SP_CAN_INTEN_RXCOMPLETE_Msk,    /**< Interrupt on RX complete. */
    NRF_CAN_INT_ERRORDETECTED_MASK = SP_CAN_INTEN_ERRORDETECTED_Msk, /**< Interrupt on error detected. */
    NRF_CAN_INT_STATECHANGED_MASK  = SP_CAN_INTEN_STATECHANGED_Msk,  /**< Interrupt on state change. */
} nrf_can_int_mask_t;

/** @brief CAN request types. */
typedef enum
{
    NRF_CAN_REQUEST_RX = SP_CAN_CONFIG_REQUEST_REQUEST_RX, /**< RX request. */
    NRF_CAN_REQUEST_TX = SP_CAN_CONFIG_REQUEST_REQUEST_TX, /**< TX request. */
} nrf_can_request_t;

typedef enum
{
    NRF_CAN_PARSING_NONE     = SP_CAN_CONFIG_PARSING_REGGROUP_None,
    NRF_CAN_PARSING_TIMING   = SP_CAN_CONFIG_PARSING_REGGROUP_Timing,
    NRF_CAN_PARSING_MODE     = SP_CAN_CONFIG_PARSING_REGGROUP_OperationMode,
    NRF_CAN_PARSING_RXFILTER = SP_CAN_CONFIG_PARSING_REGGROUP_RxFilters,
} nrf_can_parsing_t;

/** @brief CAN operation modes. */
typedef enum
{
    NRF_CAN_MODE_NORMAL     = SP_CAN_CONFIG_MODE_MODE_Normal,
    NRF_CAN_MODE_LOOPBACK   = SP_CAN_CONFIG_MODE_MODE_Loopback,
    NRF_CAN_MODE_LISTENONLY = SP_CAN_CONFIG_MODE_MODE_ListenOnly,
    NRF_CAN_MODE_ONESHOT    = SP_CAN_CONFIG_MODE_MODE_OneShot,
} nrf_can_mode_t;

typedef enum
{
    NRF_CAN_STATE_ERRORACTIVE  = SP_CAN_STATUS_STATUS_STATE_ERRORACTIVE,
    NRF_CAN_STATE_ERRORWARNING = SP_CAN_STATUS_STATUS_STATE_ERRORWARNING,
    NRF_CAN_STATE_ERRORPASSIVE = SP_CAN_STATUS_STATUS_STATE_ERRORPASSIVE,
    NRF_CAN_STATE_BUSOFF       = SP_CAN_STATUS_STATUS_STATE_BUSOFF,
    NRF_CAN_STATE_STOPPED      = SP_CAN_STATUS_STATUS_STATE_STOPPED,
} nrf_can_state_t;

/** @brief CAN configuration structure. */
typedef struct
{
    uint32_t          timing;  /**< Data Bit Timing and Prescaler. */
    nrf_can_request_t request; /**< Request type (RX/TX). */
    nrf_can_mode_t    mode;    /**< Operation mode. */
} nrf_can_config_t;

typedef struct
{
    uint32_t filter;
    uint32_t id_mask;
} nrf_can_rxfilter_t;

/** @brief CAN frame structure. */
typedef struct
{
    uint32_t identifier;      /**< Frame identifier. */
    uint8_t  length;          /**< Data length. */
    uint8_t  data[8];         /**< Data buffer. */
    uint16_t crc;             /**< Frame CRC. */
    bool     extended_format; /**< Extended format flag. */
    bool     remote_request;  /**< Remote request flag. */
} nrf_can_frame_t;

NRF_STATIC_INLINE void nrf_can_task_trigger(NRF_CAN_Type * p_reg, nrf_can_task_t task);

NRF_STATIC_INLINE void nrf_can_task_untrigger(NRF_CAN_Type * p_reg, nrf_can_task_t task);

NRF_STATIC_INLINE void nrf_can_enable(NRF_CAN_Type * p_reg);

NRF_STATIC_INLINE void nrf_can_disable(NRF_CAN_Type * p_reg);

NRF_STATIC_INLINE bool nrf_can_enable_check(NRF_CAN_Type const * p_reg);

NRF_STATIC_INLINE void nrf_can_event_clear(NRF_CAN_Type * p_reg, nrf_can_event_t event);

NRF_STATIC_INLINE bool nrf_can_event_check(NRF_CAN_Type const * p_reg, nrf_can_event_t event);

NRF_STATIC_INLINE uint32_t nrf_can_event_address_get(NRF_CAN_Type const * p_reg,
                                                     nrf_can_event_t      event);

NRF_STATIC_INLINE void nrf_can_int_enable(NRF_CAN_Type * p_reg, uint32_t mask);

NRF_STATIC_INLINE uint32_t nrf_can_int_enable_check(NRF_CAN_Type const * p_reg, uint32_t mask);

NRF_STATIC_INLINE void nrf_can_int_disable(NRF_CAN_Type * p_reg, uint32_t mask);

NRF_STATIC_INLINE void nrf_can_rxfilter_set(NRF_CAN_Type *             p_reg,
                                            nrf_can_rxfilter_t const * p_rxfilter,
                                            uint8_t                    index);

NRF_STATIC_INLINE void nrf_can_rxfilter_get(NRF_CAN_Type const * p_reg,
                                            nrf_can_rxfilter_t * p_rxfilter,
                                            uint8_t              index);

NRF_STATIC_INLINE void nrf_can_rxfilter_usefilter_set(NRF_CAN_Type * p_reg,
                                                      uint8_t        index,
                                                      uint8_t        value);

NRF_STATIC_INLINE bool nrf_can_rxfilter_usefilter_get(NRF_CAN_Type const * p_reg, uint8_t index);

NRF_STATIC_INLINE void nrf_can_rxfilter_filtermatched_set(NRF_CAN_Type * p_reg,
                                                          uint8_t        index,
                                                          uint8_t        value);

NRF_STATIC_INLINE bool nrf_can_rxfilter_filtermatched_get(NRF_CAN_Type const * p_reg,
                                                          uint8_t              index);

NRF_STATIC_INLINE void nrf_can_rxfilter_inmailbox_set(NRF_CAN_Type * p_reg,
                                                      uint8_t        index,
                                                      uint8_t        value);

NRF_STATIC_INLINE bool nrf_can_rxfilter_inmailbox_get(NRF_CAN_Type const * p_reg, uint8_t index);

NRF_STATIC_INLINE void nrf_can_set_parsing_timing(NRF_CAN_Type * p_reg);

NRF_STATIC_INLINE void nrf_can_set_parsing_mode(NRF_CAN_Type * p_reg);

NRF_STATIC_INLINE void nrf_can_set_parsing_rxfilter(NRF_CAN_Type * p_reg);

NRF_STATIC_INLINE void nrf_can_config_set(NRF_CAN_Type * p_reg, nrf_can_config_t const * p_config);

NRF_STATIC_INLINE void nrf_can_config_get(NRF_CAN_Type const * p_reg, nrf_can_config_t * p_config);

NRF_STATIC_INLINE void nrf_can_txframe_set(NRF_CAN_Type *          p_reg,
                                           uint8_t                 idx,
                                           nrf_can_frame_t const * p_frame);

NRF_STATIC_INLINE void nrf_can_txframe_get(NRF_CAN_Type const * p_reg,
                                           uint8_t              idx,
                                           nrf_can_frame_t *    p_frame);

NRF_STATIC_INLINE void nrf_can_rxframe_set(NRF_CAN_Type *          p_reg,
                                           uint8_t                 idx,
                                           nrf_can_frame_t const * p_frame);

NRF_STATIC_INLINE void nrf_can_rxframe_get(NRF_CAN_Type const * p_reg,
                                           uint8_t              idx,
                                           nrf_can_frame_t *    p_frame);

NRF_STATIC_INLINE nrf_can_state_t nrf_can_state_get(NRF_CAN_Type const * p_reg);

NRF_STATIC_INLINE uint32_t nrf_can_error_get(NRF_CAN_Type const * p_reg);

NRF_STATIC_INLINE bool nrf_can_status_error_check(uint32_t error_mask);

NRF_STATIC_INLINE void nrf_can_handshake_set(NRF_CAN_Type * p_reg, uint32_t val, uint8_t idx);

NRF_STATIC_INLINE uint32_t nrf_can_handshake_get(NRF_CAN_Type const * p_reg, uint8_t idx);

#ifndef NRF_DECLARE_ONLY

NRF_STATIC_INLINE void nrf_can_task_trigger(NRF_CAN_Type * p_reg, nrf_can_task_t task)
{
    *((volatile uint32_t *)((uint8_t *)p_reg + (uint32_t)task)) = 0x1UL;
}

NRF_STATIC_INLINE void nrf_can_task_untrigger(NRF_CAN_Type * p_reg, nrf_can_task_t task)
{
    *((volatile uint32_t *)((uint8_t *)p_reg + (uint32_t)task)) = 0x0UL;
}

NRF_STATIC_INLINE void nrf_can_enable(NRF_CAN_Type * p_reg)
{
    p_reg->ENABLE = SP_CAN_ENABLE_ENABLE_Enabled;
}

NRF_STATIC_INLINE void nrf_can_disable(NRF_CAN_Type * p_reg)
{
    p_reg->ENABLE = SP_CAN_ENABLE_ENABLE_Disabled;
}

NRF_STATIC_INLINE bool nrf_can_enable_check(NRF_CAN_Type const * p_reg)
{
    return (bool)(p_reg->ENABLE & SP_CAN_ENABLE_ENABLE_Msk);
}

NRF_STATIC_INLINE void nrf_can_event_clear(NRF_CAN_Type * p_reg, nrf_can_event_t event)
{
    *((volatile uint32_t *)((uint8_t *)p_reg + (uint32_t)event)) = 0x0UL;
    nrf_event_readback((uint8_t *)p_reg + (uint32_t)event);
}

NRF_STATIC_INLINE bool nrf_can_event_check(NRF_CAN_Type const * p_reg, nrf_can_event_t event)
{
    return nrf_event_check(p_reg, event);
}

NRF_STATIC_INLINE uint32_t nrf_can_event_address_get(NRF_CAN_Type const * p_reg,
                                                     nrf_can_event_t      event)
{
    return nrf_task_event_address_get(p_reg, event);
}

NRF_STATIC_INLINE void nrf_can_int_enable(NRF_CAN_Type * p_reg, uint32_t mask)
{
    p_reg->INTEN |= mask;
}

NRF_STATIC_INLINE uint32_t nrf_can_int_enable_check(NRF_CAN_Type const * p_reg, uint32_t mask)
{
    return p_reg->INTEN & mask;
}

NRF_STATIC_INLINE void nrf_can_int_disable(NRF_CAN_Type * p_reg, uint32_t mask)
{
    p_reg->INTEN &= ~mask;
}

NRF_STATIC_INLINE void nrf_can_rxfilter_set(NRF_CAN_Type *             p_reg,
                                            nrf_can_rxfilter_t const * p_rxfilter,
                                            uint8_t                    index)
{
    p_reg->RXFILTER[index].IDFILTER = p_rxfilter->filter & SP_CAN_RXFILTER_IDFILTER_IDENTIFIER_Msk;
    p_reg->RXFILTER[index].IDMASK   = p_rxfilter->id_mask & SP_CAN_RXFILTER_IDMASK_MASK_Msk;
}

NRF_STATIC_INLINE void nrf_can_rxfilter_get(NRF_CAN_Type const * p_reg,
                                            nrf_can_rxfilter_t * p_rxfilter,
                                            uint8_t              index)
{
    p_rxfilter->filter  = p_reg->RXFILTER[index].IDFILTER & SP_CAN_RXFILTER_IDFILTER_IDENTIFIER_Msk;
    p_rxfilter->id_mask = p_reg->RXFILTER[index].IDMASK & SP_CAN_RXFILTER_IDMASK_MASK_Msk;
}

NRF_STATIC_INLINE void nrf_can_rxfilter_usefilter_set(NRF_CAN_Type * p_reg,
                                                      uint8_t        index,
                                                      uint8_t        value)
{
    p_reg->RXFILTER[index].IDFILTER = (p_reg->RXFILTER[index].IDFILTER &
                                       ~SP_CAN_RXFILTER_IDFILTER_USEFILTER_Msk) |
                                      ((value << SP_CAN_RXFILTER_IDFILTER_USEFILTER_Pos) &
                                       SP_CAN_RXFILTER_IDFILTER_USEFILTER_Msk);
}

NRF_STATIC_INLINE bool nrf_can_rxfilter_usefilter_get(NRF_CAN_Type const * p_reg, uint8_t index)
{
    if ((p_reg->RXFILTER[index].IDFILTER & SP_CAN_RXFILTER_IDFILTER_USEFILTER_Msk)
        >> SP_CAN_RXFILTER_IDFILTER_USEFILTER_Pos)
    {
        return true;
    }
    return false;
}

NRF_STATIC_INLINE void nrf_can_rxfilter_filtermatched_set(NRF_CAN_Type * p_reg,
                                                          uint8_t        index,
                                                          uint8_t        value)
{
    p_reg->RXFILTER[index].IDFILTER = (p_reg->RXFILTER[index].IDFILTER &
                                       ~SP_CAN_RXFILTER_IDFILTER_FILTERMATCHED_Msk) |
                                      ((value << SP_CAN_RXFILTER_IDFILTER_FILTERMATCHED_Pos) &
                                       SP_CAN_RXFILTER_IDFILTER_FILTERMATCHED_Msk);
}

NRF_STATIC_INLINE bool nrf_can_rxfilter_filtermatched_get(NRF_CAN_Type const * p_reg, uint8_t index)
{
    if ((p_reg->RXFILTER[index].IDFILTER & SP_CAN_RXFILTER_IDFILTER_FILTERMATCHED_Msk)
        >> SP_CAN_RXFILTER_IDFILTER_FILTERMATCHED_Pos)
    {
        return true;
    }
    return false;
}

NRF_STATIC_INLINE void nrf_can_rxfilter_inmailbox_set(NRF_CAN_Type * p_reg,
                                                      uint8_t        index,
                                                      uint8_t        value)
{
    p_reg->RXFILTER[index].IDFILTER = (p_reg->RXFILTER[index].IDFILTER &
                                       ~SP_CAN_RXFILTER_IDFILTER_INMAILBOX_Msk) |
                                      ((value << SP_CAN_RXFILTER_IDFILTER_INMAILBOX_Pos) &
                                       SP_CAN_RXFILTER_IDFILTER_INMAILBOX_Msk);
}

NRF_STATIC_INLINE bool nrf_can_rxfilter_inmailbox_get(NRF_CAN_Type const * p_reg, uint8_t index)
{
    if ((p_reg->RXFILTER[index].IDFILTER & SP_CAN_RXFILTER_IDFILTER_INMAILBOX_Msk)
        >> SP_CAN_RXFILTER_IDFILTER_INMAILBOX_Pos)
    {
        return true;
    }
    return false;
}

NRF_STATIC_INLINE void nrf_can_set_parsing_timing(NRF_CAN_Type * p_reg)
{
    p_reg->CONFIG.PARSING = NRF_CAN_PARSING_TIMING;
}

NRF_STATIC_INLINE void nrf_can_set_parsing_mode(NRF_CAN_Type * p_reg)
{
    p_reg->CONFIG.PARSING = NRF_CAN_PARSING_MODE;
}

NRF_STATIC_INLINE void nrf_can_set_parsing_rxfilter(NRF_CAN_Type * p_reg)
{
    p_reg->CONFIG.PARSING = NRF_CAN_PARSING_RXFILTER;
}

NRF_STATIC_INLINE void nrf_can_config_set(NRF_CAN_Type * p_reg, nrf_can_config_t const * p_config)
{
    p_reg->CONFIG.TIMING  = p_config->timing;
    p_reg->CONFIG.REQUEST = ((uint32_t)p_config->request) &
                            SP_CAN_CONFIG_REQUEST_REQUEST_Msk;
    p_reg->CONFIG.MODE = (((uint32_t)p_config->mode) &
                          SP_CAN_CONFIG_MODE_MODE_Msk);
}

NRF_STATIC_INLINE void nrf_can_config_get(NRF_CAN_Type const * p_reg, nrf_can_config_t * p_config)
{
    p_config->timing  = p_reg->CONFIG.TIMING;
    p_config->request = (nrf_can_request_t)(p_reg->CONFIG.REQUEST &
                                            SP_CAN_CONFIG_REQUEST_REQUEST_Msk);
    p_config->mode = (nrf_can_mode_t)(p_reg->CONFIG.MODE &
                                      SP_CAN_CONFIG_MODE_MODE_Msk);
}

NRF_STATIC_INLINE void nrf_can_txframe_set(NRF_CAN_Type *          p_reg,
                                           uint8_t                 idx,
                                           nrf_can_frame_t const * p_frame)
{
    p_reg->TXFRAME[idx].IDENTIFIER = (p_frame->identifier
                                      << SP_CAN_TXFRAME_IDENTIFIER_IDENTIFIER_Pos) &
                                     SP_CAN_TXFRAME_IDENTIFIER_IDENTIFIER_Msk;

    p_reg->TXFRAME[idx].TXDATAL = ((uint32_t)p_frame->data[0] << 0) | ((uint32_t)p_frame->data[1]
                                                                       << 8) |
                                  ((uint32_t)p_frame->data[2]
                                   << 16) | ((uint32_t)p_frame->data[3] << 24);

    p_reg->TXFRAME[idx].TXDATAH = ((uint32_t)p_frame->data[4] << 0) | ((uint32_t)p_frame->data[5]
                                                                       << 8) |
                                  ((uint32_t)p_frame->data[6]
                                   << 16) | ((uint32_t)p_frame->data[7] << 24);

    p_reg->TXFRAME[idx].METADATA = (((uint32_t)p_frame->extended_format
                                     << SP_CAN_TXFRAME_METADATA_EXTENDEDFORMAT_Pos) &
                                    SP_CAN_TXFRAME_METADATA_EXTENDEDFORMAT_Msk) |
                                   (((uint32_t)p_frame->remote_request
                                     << SP_CAN_TXFRAME_METADATA_REMOTEREQUEST_Pos) &
                                    SP_CAN_TXFRAME_METADATA_REMOTEREQUEST_Msk) |
                                   (((uint32_t)p_frame->length
                                     << SP_CAN_TXFRAME_METADATA_DATALENGTH_Pos) &
                                    SP_CAN_TXFRAME_METADATA_DATALENGTH_Msk) |
                                   (((uint32_t)p_frame->crc << SP_CAN_TXFRAME_METADATA_CRC_Pos) &
                                    SP_CAN_TXFRAME_METADATA_CRC_Msk);
}

NRF_STATIC_INLINE void nrf_can_txframe_get(NRF_CAN_Type const * p_reg,
                                           uint8_t              idx,
                                           nrf_can_frame_t *    p_frame)
{
    uint32_t identifier = p_reg->TXFRAME[idx].IDENTIFIER;

    p_frame->identifier = (identifier & SP_CAN_TXFRAME_IDENTIFIER_IDENTIFIER_Msk)
                          >> SP_CAN_TXFRAME_IDENTIFIER_IDENTIFIER_Pos;

    uint32_t datal = p_reg->TXFRAME[idx].TXDATAL;
    p_frame->data[0] = (uint8_t)(datal >> 0);
    p_frame->data[1] = (uint8_t)(datal >> 8);
    p_frame->data[2] = (uint8_t)(datal >> 16);
    p_frame->data[3] = (uint8_t)(datal >> 24);

    uint32_t datah = p_reg->TXFRAME[idx].TXDATAH;
    p_frame->data[4] = (uint8_t)(datah >> 0);
    p_frame->data[5] = (uint8_t)(datah >> 8);
    p_frame->data[6] = (uint8_t)(datah >> 16);
    p_frame->data[7] = (uint8_t)(datah >> 24);

    uint32_t metadata = p_reg->TXFRAME[idx].METADATA;
    p_frame->extended_format = (bool)((metadata & SP_CAN_TXFRAME_METADATA_EXTENDEDFORMAT_Msk)
                                      >> SP_CAN_TXFRAME_METADATA_EXTENDEDFORMAT_Pos);
    p_frame->remote_request = (bool)((metadata & SP_CAN_TXFRAME_METADATA_REMOTEREQUEST_Msk)
                                     >> SP_CAN_TXFRAME_METADATA_REMOTEREQUEST_Pos);
    p_frame->length = (uint8_t)((metadata & SP_CAN_TXFRAME_METADATA_DATALENGTH_Msk)
                                >> SP_CAN_TXFRAME_METADATA_DATALENGTH_Pos);
    p_frame->crc = (uint16_t)((metadata & SP_CAN_TXFRAME_METADATA_CRC_Msk)
                              >> SP_CAN_TXFRAME_METADATA_CRC_Pos);
}

NRF_STATIC_INLINE void nrf_can_rxframe_set(NRF_CAN_Type *          p_reg,
                                           uint8_t                 idx,
                                           nrf_can_frame_t const * p_frame)
{
    p_reg->RXFRAME[idx].IDENTIFIER = (p_frame->identifier
                                      << SP_CAN_RXFRAME_IDENTIFIER_IDENTIFIER_Pos) &
                                     SP_CAN_RXFRAME_IDENTIFIER_IDENTIFIER_Msk;

    p_reg->RXFRAME[idx].RXDATAL = ((uint32_t)p_frame->data[0] << 0) | ((uint32_t)p_frame->data[1]
                                                                       << 8) |
                                  ((uint32_t)p_frame->data[2]
                                   << 16) | ((uint32_t)p_frame->data[3] << 24);

    p_reg->RXFRAME[idx].RXDATAH = ((uint32_t)p_frame->data[4] << 0) | ((uint32_t)p_frame->data[5]
                                                                       << 8) |
                                  ((uint32_t)p_frame->data[6]
                                   << 16) | ((uint32_t)p_frame->data[7] << 24);

    p_reg->RXFRAME[idx].METADATA = (((uint32_t)p_frame->extended_format
                                     << SP_CAN_RXFRAME_METADATA_EXTENDEDFORMAT_Pos) &
                                    SP_CAN_RXFRAME_METADATA_EXTENDEDFORMAT_Msk) |
                                   (((uint32_t)p_frame->remote_request
                                     << SP_CAN_RXFRAME_METADATA_REMOTEREQUEST_Pos) &
                                    SP_CAN_RXFRAME_METADATA_REMOTEREQUEST_Msk) |
                                   (((uint32_t)p_frame->length
                                     << SP_CAN_RXFRAME_METADATA_DATALENGTH_Pos) &
                                    SP_CAN_RXFRAME_METADATA_DATALENGTH_Msk) |
                                   (((uint32_t)p_frame->crc << SP_CAN_RXFRAME_METADATA_CRC_Pos) &
                                    SP_CAN_RXFRAME_METADATA_CRC_Msk);
}

NRF_STATIC_INLINE void nrf_can_rxframe_get(NRF_CAN_Type const * p_reg,
                                           uint8_t              idx,
                                           nrf_can_frame_t *    p_frame)
{
    uint32_t identifier = p_reg->RXFRAME[idx].IDENTIFIER;

    p_frame->identifier = (identifier & SP_CAN_RXFRAME_IDENTIFIER_IDENTIFIER_Msk)
                          >> SP_CAN_RXFRAME_IDENTIFIER_IDENTIFIER_Pos;

    uint32_t datal = p_reg->RXFRAME[idx].RXDATAL;
    p_frame->data[0] = (uint8_t)(datal >> 0);
    p_frame->data[1] = (uint8_t)(datal >> 8);
    p_frame->data[2] = (uint8_t)(datal >> 16);
    p_frame->data[3] = (uint8_t)(datal >> 24);

    uint32_t datah = p_reg->RXFRAME[idx].RXDATAH;
    p_frame->data[4] = (uint8_t)(datah >> 0);
    p_frame->data[5] = (uint8_t)(datah >> 8);
    p_frame->data[6] = (uint8_t)(datah >> 16);
    p_frame->data[7] = (uint8_t)(datah >> 24);

    uint32_t metadata = p_reg->RXFRAME[idx].METADATA;
    p_frame->extended_format = (bool)((metadata & SP_CAN_RXFRAME_METADATA_EXTENDEDFORMAT_Msk)
                                      >> SP_CAN_RXFRAME_METADATA_EXTENDEDFORMAT_Pos);
    p_frame->remote_request = (bool)((metadata & SP_CAN_RXFRAME_METADATA_REMOTEREQUEST_Msk)
                                     >> SP_CAN_RXFRAME_METADATA_REMOTEREQUEST_Pos);
    p_frame->length = (uint8_t)((metadata & SP_CAN_RXFRAME_METADATA_DATALENGTH_Msk)
                                >> SP_CAN_RXFRAME_METADATA_DATALENGTH_Pos);
    p_frame->crc = (uint16_t)((metadata & SP_CAN_RXFRAME_METADATA_CRC_Msk)
                              >> SP_CAN_RXFRAME_METADATA_CRC_Pos);
}

NRF_STATIC_INLINE nrf_can_state_t nrf_can_state_get(NRF_CAN_Type const * p_reg)
{
    return (p_reg->STATUS.STATUS & SP_CAN_STATUS_STATUS_STATE_Msk)
           >> SP_CAN_STATUS_STATUS_STATE_Pos;
}

NRF_STATIC_INLINE uint32_t nrf_can_error_get(NRF_CAN_Type const * p_reg)
{
    return (p_reg->STATUS.STATUS & 0x1FF);
}

NRF_STATIC_INLINE bool nrf_can_status_error_check(uint32_t error_mask)
{
    return (error_mask &
            (SP_CAN_STATUS_STATUS_BITERROR_Msk | SP_CAN_STATUS_STATUS_BITSTUFFINGERROR_Msk |
             SP_CAN_STATUS_STATUS_FORMERROR_Msk | SP_CAN_STATUS_STATUS_ACKERROR_Msk |
             SP_CAN_STATUS_STATUS_CRCERROR_Msk | SP_CAN_STATUS_STATUS_PROTOCOLERROR_Msk)) !=
           0;
}

NRF_STATIC_INLINE void nrf_can_handshake_set(NRF_CAN_Type * p_reg, uint32_t val, uint8_t idx)
{
    p_reg->SPSYNC.AUX[idx] = val;
}

NRF_STATIC_INLINE uint32_t nrf_can_handshake_get(NRF_CAN_Type const * p_reg, uint8_t idx)
{
    return p_reg->SPSYNC.AUX[idx];
}

#endif // NRF_DECLARE_ONLY

/** @} */

#ifdef __cplusplus
}
#endif

#endif // NRF_CAN_H__
