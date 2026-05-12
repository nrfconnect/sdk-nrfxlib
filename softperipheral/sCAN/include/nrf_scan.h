/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef NRF_SCAN_H__
#define NRF_SCAN_H__

#include <stdbool.h>
#include <stdint.h>
// #include "nrf_config_scan.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup nrf_scan Soft CAN
 * @{
 *
 * @brief Soft CAN driver
 *
 * This API describes the driver operation of a Software CAN emulated in a VPR CPU. Based on CAN2.0A/B standard.
 *
 * The functions in this driver are not reentrant if they operate on the same sCAN peripheral. It is
 * the responsibility of the API user to ensure the functions are not preempted by each other.
 * An exception is the callback function context. Multiple functions from this API can be called
 * from the callback.
 *
 * @note The nrf_scan_irq_handler() function must be called from an external IRQ handler that has
 *       been properly registered with the interrupt controller system.
 *
 * @note The GPIO pins required for sCAN operation must be properly configured by the user before
 *       initializing the driver with nrf_scan_init().
 */

/** @brief sCAN event definitions */
typedef enum
{
    NRF_SCAN_EVT_TX_COMPLETE,   ///< TX has finished, if mode is ONESHOT this happens even if there are errors or arbitration was lost
    NRF_SCAN_EVT_RX_COMPLETE,   ///< RX has finished after an ID match
    NRF_SCAN_EVT_ERROR,         ///< sCAN has detected an error
    NRF_SCAN_EVT_STATE_CHANGED, ///< sCAN state has changed
} nrf_scan_event_type_t;

/** @brief sCAN operation mode definitions */
typedef enum
{
    NRF_SCAN_MODE_NORMAL = 0, ///< Normal mode of operation, RX/TX, uses ID filters, TX will be attemted until success
    NRF_SCAN_MODE_LOOPBACK,   ///< To test API, moves TX data to RX data via SW
    NRF_SCAN_MODE_LISTENONLY, ///< Listen only mode of operation, can't send dominant
                              ///< bits
    NRF_SCAN_MODE_ONESHOT,    ///< Same as normal mode of operation but TX attempts happen only
                              ///< once
} nrf_scan_mode_type_t;

/** @brief sCAN error code definitions */
typedef enum
{
    NRF_SCAN_SUCCESS = 0,
    NRF_SCAN_ERROR_PROTOCOL,           ///< Generic error response.
    NRF_SCAN_ERROR_BIT_STUFFING_ERROR, ///< RX. Node receives 6 consecutive dominant/recessive bits between SOF and CRC.
    NRF_SCAN_ERROR_BIT0_ERROR,         ///< TX. Node can't send dominant bit.
    NRF_SCAN_ERROR_BIT1_ERROR,         ///< TX. Node can't send recessive bit.
    NRF_SCAN_ERROR_FORM_ERROR,         ///< RX. Node receives an invalid value in a bit that belongs to SOF/EOF fields or ACK/CRC delimiters.
    NRF_SCAN_ERROR_ACK_ERROR,          ///< TX. Node transmits message but ACK slot is not made dominant by receivers
    NRF_SCAN_ERROR_CRC_ERROR,          ///< RX. Node calculates message CRC that differs from the one sent
    NRF_SCAN_ERROR_BUSY,               ///< API level error. sCAN is busy
    NRF_SCAN_ERROR_UNSUPPORTED,        ///< API level error. Config parameter combination is not supported
    NRF_SCAN_ERROR_INVALID_PARAM,      ///< API level error. Invalid config parameter
    NRF_SCAN_ERROR_INVALID_STATE,      ///< API level error. nrfx driver in invalid state
} nrf_scan_error_t;

/** @brief sCAN state definitions */
typedef enum
{
    NRF_SCAN_STATE_ERROR_ACTIVE = 0, ///< TEC/REC < 96
    NRF_SCAN_STATE_ERROR_WARNING,    ///< TEC/REC < 128
    NRF_SCAN_STATE_ERROR_PASSIVE,    ///< TEC/REC < 256
    NRF_SCAN_STATE_BUS_OFF,          ///< TEC/REC >= 256
    NRF_SCAN_STATE_STOPPED,          ///< No participation in CAN communication
} nrf_scan_state_t;

/** @brief sCAN event handler function pointer */
typedef void (* nrf_scan_event_handler_t)(nrf_scan_event_type_t const * p_event,
                                          void *                        p_context);

/**
 * @brief sCAN context structure.
 */
typedef struct
{
    uint16_t sp_identifier;        ///< Soft peripheral identifier
    uint16_t
             last_updated_mailbox; ///< index corresponding to the last updated mailbox, populated when NRF_SCAN_EVT_RX_COMPLETE happens
} nrf_scan_context_t;

/**
 * @brief sCAN timing configuration structure.
 */
typedef struct
{
    uint16_t sjw;        ///< (Re)synchronization jump width in quanta units
    uint16_t prop_seg;   ///< Propagation segment length in quanta units
    uint16_t phase_seg1; ///< Phase segment 1 length in quanta units
    uint16_t phase_seg2; ///< Phase segment 2 length in quanta units
    uint16_t prescaler;  ///< Prescaler, number of clock cycles per quanta unit
} nrf_scan_timing_t;

/**
 * @brief sCAN TX/RX frame structure.
 */
typedef struct
{
    uint32_t identifier;  ///< ID, 11 or 29 bits depending on ide
    uint8_t  data_length; ///< Data length in bytes [0,8]
    uint8_t  ide;         ///< Identifier Extension bit
    uint8_t  rtr;         ///< Remote Transmission Request bit
    uint8_t  data[8];
} nrf_scan_frame_t;

/**
 * @brief sCAN RX filter structure.
 */
typedef struct
{
    uint32_t id;   ///< ID, can be standard or extended
    uint32_t mask; ///< Filter mask
} nrf_scan_rx_filter_t;

/**
 * @brief sCAN RX mailbox structure.
 */
typedef struct
{
    nrf_scan_frame_t     rx_frame;  ///< Received frame
    nrf_scan_rx_filter_t rx_filter; ///< Filter
    bool                 ready;     ///< Indicator for new data to be processed
} nrf_scan_rx_mailbox_t;

/**
 * @brief sCAN RX mailbox. 
 * @note This is populated by the sCAN API, user should only read its data after NRF_SCAN_EVT_RX_COMPLETE.
 */
extern nrf_scan_rx_mailbox_t m_scan_rx_mailbox[];

/**
 * @brief sCAN driver instance structure.
 */
typedef struct
{
    void * p_reg;        ///< Pointer to a structure with sCAN virtual register interface.
    uint8_t
           drv_inst_idx; ///< Index of the driver instance. For internal use only.
} nrf_scan_t;

/**
 * @brief sCAN status structure.
 */
typedef struct
{
    nrf_scan_error_t error; ///< CAN errors.
    nrf_scan_state_t state; ///< CAN controller state.
} nrf_scan_status_t;

/**
 * @brief Get sCAN state and errors.
 *
 * @note Typically called only after an event has been received.
 *
 * @param[in] p_scan     Pointer to the sCAN driver instance to initialize.
 */
nrf_scan_status_t nrf_scan_get_status(nrf_scan_t const * p_scan);

/**
 * @brief Initialize the sCAN driver for a given instance.
 *
 * @note The GPIO pins required for sCAN operation must be properly configured by the API user
 *       before calling this function.
 *
 * @param[in] p_scan     Pointer to the sCAN driver instance to initialize.
 * @param[in]  handler   Event handler callback function.
 * @param[in]  p_context User context passed to the event handler.
 *
 * @retval NRF_SCAN_SUCCESS          Initialization successful.
 * @retval NRF_SCAN_UNSUPPORTED      feature unsupported.
 * @retval NRF_SCAN_ERROR_INVALID_PARAM Invalid or NULL parameter provided.
 * @retval NRF_SCAN_ERROR_INVALID_STATE nrfx driver state is invalid.
 * @retval NRF_SCAN_ERROR_BUSY        sCAN driver is busy
 */
nrf_scan_error_t nrf_scan_init(nrf_scan_t const *       p_scan,
                               nrf_scan_event_handler_t handler,
                               void *                   p_context);

/**
 * @brief  Uninitialize the sCAN driver instance.
 *
 * This function deconfigures the peripheral.
 *
 * @note The function deactivates the peripheral instance if it is active.
 *       The deactivation process releases the clock requests and the power
 * states restrictions related to VPR.
 *
 * @param[in] p_scan Identifier of the sCAN instance to uninitialize.
 */
void nrf_scan_uninit(nrf_scan_t const * p_scan);

/**
 * @brief Explicitly power up and configure the controller.
 *
 * Optional: Recommended around sequences of operations to avoid repeated power toggling latency.
 *
 * @param[in] p_scan Pointer to the sCAN driver instance.
 *
 * @retval NRF_SCAN_SUCCESS          Controller powered up successfully.
 * @retval NRF_SCAN_ERROR_INVALID_STATE nrfx driver state is invalid.
 */
nrf_scan_error_t nrf_scan_enable(nrf_scan_t const * p_scan);

/**
 * @brief Explicitly power down the controller.
 *
 * This function will abort any ongoing operations by calling @ref nrf_scan_abort
 * Optional: Call after a series of operations to save power.
 *
 * @param[in] p_scan Pointer to the sCAN driver instance.
 *
 * @retval NRF_SCAN_SUCCESS          Controller powered down successfully.
 * @retval NRF_SCAN_ERROR_INVALID_STATE nrfx driver state is invalid.
 */
nrf_scan_error_t nrf_scan_disable(nrf_scan_t const * p_scan);

/**
 * @brief Configure sCAN timing.
 *
 * This function configures timing parameters for sCAN
 *
 * @param[in] p_scan Pointer to the sCAN driver instance.
 * @param[in] p_timing Pointer to the sCAN driver timing configuration structure.
 *
 * @note This function must be called after sCAN is in STOPPED state:
 * - after @ref nrf_scan_enable
 * - after @ref nrf_scan_abort
 *
 * @note From a cold start, if this function is called after @ref nrf_can_enable, sCAN will immediately start operation with no RX filter
 *
 * @retval NRF_SCAN_SUCCESS          Controller timing configured successfully.
 * @retval NRF_SCAN_ERROR_INVALID_STATE nrfx driver state is invalid.
 * @retval NRF_SCAN_ERROR_INVALID_PARAM sCAN timing configuration contains an invalid parameter.
 */
nrf_scan_error_t nrf_scan_timing(nrf_scan_t const *        p_scan,
                                 nrf_scan_timing_t const * p_timing);

/**
 * @brief Configure sCAN mode.
 *
 * This function configures mode of operation for sCAN
 *
 * @param[in] p_scan Pointer to the sCAN driver instance.
 * @param[in] p_mode Pointer to the sCAN driver mode configuration.
 *
 * @retval NRF_SCAN_SUCCESS          Controller mode configured successfully.
 * @retval NRF_SCAN_ERROR_INVALID_STATE nrfx driver state is invalid.
 * @retval NRF_SCAN_ERROR_INVALID_PARAM sCAN mode configuration contains an invalid parameter.
 */
nrf_scan_error_t nrf_scan_mode(nrf_scan_t const * p_scan, nrf_scan_mode_type_t * p_mode);

/**
 * @brief Configure a sCAN RX filter.
 *
 * This function configures a filter for RX mailbox[index]
 *
 * @param[in] p_scan Pointer to the sCAN driver instance.
 * @param[in] p_rxfilter Pointer to the RX filter to apply for mailbox[index].
 * @param[in] index Index for @ref m_scan_rx_mailbox.
 *
 * @retval NRF_SCAN_SUCCESS          RX filter for mailbox[index] configured successfully.
 * @retval NRF_SCAN_ERROR_INVALID_STATE nrfx driver state is invalid.
 * @retval NRF_SCAN_ERROR_INVALID_PARAM index is an invalid parameter.
 */
nrf_scan_error_t nrf_scan_set_rx_filter(nrf_scan_t const *           p_scan,
                                        nrf_scan_rx_filter_t const * p_rxfilter,
                                        uint8_t                      index);

/**
 * @brief Disable a sCAN RX filter.
 *
 * This function disables a filter for RX mailbox[index]
 *
 * @note This function disables the filter but doesn't change ID or Mask values.
 *
 * @param[in] p_scan Pointer to the sCAN driver instance.
 * @param[in] index Index for @ref m_scan_rx_mailbox.
 *
 * @retval NRF_SCAN_SUCCESS          RX filter for mailbox[index] disabled successfully.
 * @retval NRF_SCAN_ERROR_INVALID_STATE nrfx driver state is invalid.
 * @retval NRF_SCAN_ERROR_INVALID_PARAM index is an invalid parameter.
 */
nrf_scan_error_t nrf_scan_disable_rx_filter(nrf_scan_t const * p_scan, uint8_t index);

/**
 * @brief Enable a sCAN RX filter.
 *
 * This function enables a filter for RX mailbox[index]
 *
 * @note This function enables the filter but doesn't change ID or Mask values.
 *
 * @param[in] p_scan Pointer to the sCAN driver instance.
 * @param[in] index Index for @ref m_scan_rx_mailbox.
 *
 * @retval NRF_SCAN_SUCCESS          RX filter for mailbox[index] enabled successfully.
 * @retval NRF_SCAN_ERROR_INVALID_STATE nrfx driver state is invalid.
 * @retval NRF_SCAN_ERROR_INVALID_PARAM index is an invalid parameter.
 */
nrf_scan_error_t nrf_scan_enable_rx_filter(nrf_scan_t const * p_scan, uint8_t index);

/**
 * @brief Unlock sCAN RX mailbox[index].
 *
 * @note This function should be called when the data at RX mailbox[index] has been processed
 * and sCAN can overwrite it with new data. The data stored in the mailbox will also be cleared
 *
 * @param[in] p_scan Pointer to the sCAN driver instance.
 * @param[in] index Index for @ref m_scan_rx_mailbox.
 *
 * @retval NRF_SCAN_SUCCESS          RX mailbox[index] unlocked successfully.
 * @retval NRF_SCAN_ERROR_INVALID_STATE nrfx driver state is invalid.
 * @retval NRF_SCAN_ERROR_INVALID_PARAM index is an invalid parameter.
 */
nrf_scan_error_t nrf_scan_unlock_rx_mailbox(nrf_scan_t const * p_scan, uint8_t index);

/**
 * @brief sCAN send function.
 *
 * This function configures the mode of operation and attempts to send a frame on the CAN bus.
 *
 * @param[in] p_scan Pointer to the sCAN driver instance.
 * @param[in] p_tx_frame Pointer to the TX frame structure that will be sent on the bus.
 *
 * @note This function must not be called before @ref nrf_scan_timing has been called at least once. This function can't be called when mode of operation is LISTENONLY.
 *
 * @retval NRF_SCAN_SUCCESS          TX transaction has been dispatched successfully (does not imply correct reception).
 * @retval NRF_SCAN_ERROR_BUSY       There is another TX transaction in the queue.
 * @retval NRF_SCAN_ERROR_INVALID_STATE nrfx driver state is invalid.
 * @retval NRF_SCAN_ERROR_INVALID_PARAM sCAN configuration contains an invalid parameter.
 */
nrf_scan_error_t nrf_scan_send(nrf_scan_t const * p_scan,
                               nrf_scan_frame_t * p_tx_frame);

/**
 * @brief Abort any ongoing sCAN operation.
 *
 * This function forcibly terminates any pending sCAN TX/RX
 * operations. It should be used as a recovery mechanism when a command has
 * timed out or the driver appears to be in a stuck state.
 *
 * After calling this function, the driver will attempt to reset internal state
 * and hardware, making it ready to accept new TX/RX operations.
 * This operation will set sCAN state to STOPPED and reset its internal REC and TEC.
 *
 * This function is called by @ref nrf_scan_disable
 *
 * @param p_scan Pointer to the sCAN driver instance.
 *
 * @retval NRF_SCAN_SUCCESS          sCAN aborted successfully.
 * @retval NRF_SCAN_ERROR_INVALID_STATE nrfx driver state is invalid.
 */
nrf_scan_error_t nrf_scan_abort(nrf_scan_t const * p_scan);

/**
 * @brief IRQ handler to be called from the sCAN interrupt vector.
 */
void nrf_scan_irq_handler(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif // NRF_SCAN_H__
