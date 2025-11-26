/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef NRF_SEMMC_H__
#define NRF_SEMMC_H__

#include <stdbool.h>
#include <stdint.h>
#include "nrf_config_semmc.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup nrf_semmc Soft eMMC
 * @{
 *
 * @brief Soft eMMC driver
 *
 * This API describes the driver operation of a Software eMMC emulated in a VPR CPU. Based on JEDEC Standard No. 84-B51A.
 *
 * The functions in this driver are not reentrant if they operate on the same eMMC peripheral. It is
 * the responsibility of the API user to ensure the functions are not preempted by each other.
 * An exception is the callback function context. Multiple functions from this API can be called
 * from the callback.
 *
 * @note The nrf_semmc_irq_handler() function must be called from an external IRQ handler that has
 *       been properly registered with the interrupt controller system.
 *
 * @note The GPIO pins required for eMMC operation must be properly configured by the user before
 *       initializing the driver with nrf_semmc_init().
 */

/** @brief eMMC command index definitions */
typedef enum
{
    NRF_SEMMC_CMD0_GO_IDLE_STATE         = 0,
    NRF_SEMMC_CMD1_SEND_OP_COND          = 1,
    NRF_SEMMC_CMD2_ALL_SEND_CID          = 2,
    NRF_SEMMC_CMD3_SET_RELATIVE_ADDR     = 3,
    NRF_SEMMC_CMD4_SET_DSR               = 4,
    NRF_SEMMC_CMD5_SLEEP_AWAKE           = 5,
    NRF_SEMMC_CMD6_SWITCH                = 6,
    NRF_SEMMC_CMD7_SELECT_DESELECT       = 7,
    NRF_SEMMC_CMD8_SEND_EXT_CSD          = 8,
    NRF_SEMMC_CMD9_SEND_CSD              = 9,
    NRF_SEMMC_CMD10_SEND_CID             = 10,
    NRF_SEMMC_CMD12_STOP_TRANSMISSION    = 12,
    NRF_SEMMC_CMD13_SEND_STATUS          = 13,
    NRF_SEMMC_CMD14_BUSTEST_R            = 14,
    NRF_SEMMC_CMD15_GO_INACTIVE_STATE    = 15,
    NRF_SEMMC_CMD16_SET_BLOCKLEN         = 16,
    NRF_SEMMC_CMD17_READ_SINGLE          = 17,
    NRF_SEMMC_CMD18_READ_MULTIPLE        = 18,
    NRF_SEMMC_CMD19_BUSTEST_W            = 19,
    NRF_SEMMC_CMD21_SEND_TUNING_BLOCK    = 21,
    NRF_SEMMC_CMD23_SET_BLOCK_COUNT      = 23,
    NRF_SEMMC_CMD24_WRITE_BLOCK          = 24,
    NRF_SEMMC_CMD25_WRITE_MULTIPLE       = 25,
    NRF_SEMMC_CMD26_PROGRAM_CID          = 26,
    NRF_SEMMC_CMD27_PROGRAM_CSD          = 27,
    NRF_SEMMC_CMD28_SET_WRITE_PROT       = 28,
    NRF_SEMMC_CMD29_CLR_WRITE_PROT       = 29,
    NRF_SEMMC_CMD30_SEND_WRITE_PROT      = 30,
    NRF_SEMMC_CMD31_SEND_WRITE_PROT_TYPE = 31,
    NRF_SEMMC_CMD35_ERASE_GROUP_START    = 35,
    NRF_SEMMC_CMD36_ERASE_GROUP_END      = 36,
    NRF_SEMMC_CMD38_ERASE                = 38,
    NRF_SEMMC_CMD42_LOCK_UNLOCK          = 42,
    NRF_SEMMC_CMD44_QUEUED_TASK_PARAMS   = 44,
    NRF_SEMMC_CMD45_QUEUED_TASK_ADDRESS  = 45,
    NRF_SEMMC_CMD46_EXECUTE_READ_TASK    = 46,
    NRF_SEMMC_CMD47_EXECUTE_WRITE_TASK   = 47,
    NRF_SEMMC_CMD48_CMDQ_TASK_MGMT       = 48,
    NRF_SEMMC_CMD49_SET_TIME             = 49,
} nrf_semmc_cmd_t;

typedef enum
{
    NRF_SEMMC_RESP_NONE,
    NRF_SEMMC_RESP_R1,
    NRF_SEMMC_RESP_R1B,
    NRF_SEMMC_RESP_R2,
    NRF_SEMMC_RESP_R3,
    NRF_SEMMC_RESP_R4,
    NRF_SEMMC_RESP_R5,
} nrf_semmc_resp_type_t;

typedef enum
{
    NRF_SEMMC_SUCCESS = 0,
    NRF_SEMMC_ERROR_TIMEOUT,
    NRF_SEMMC_ERROR_CMDTIMEOUT,
    NRF_SEMMC_ERROR_CMDCRCERROR,
    NRF_SEMMC_ERROR_DATACRCERROR,
    NRF_SEMMC_ERROR_ILLEGAL_CMD,
    NRF_SEMMC_ERROR_DEVICE_REJECTED,
    NRF_SEMMC_ERROR_INTERNAL_OR_PROTOCOL,
    NRF_SEMMC_ERROR_BUSY,
    NRF_SEMMC_ERROR_UNSUPPORTED,
    NRF_SEMMC_ERROR_INVALID_PARAM,
    NRF_SEMMC_ERROR_INVALID_STATE,
    NRF_SEMMC_ERROR_HW_FAULT,
} nrf_semmc_error_t;

typedef enum
{
    NRF_SEMMC_BUS_WIDTH_1 = 1,
    NRF_SEMMC_BUS_WIDTH_4 = 4,
    NRF_SEMMC_BUS_WIDTH_8 = 8,
} nrf_semmc_bus_width_t;

typedef enum
{
    NRF_SEMMC_EVT_XFER_DONE,                   ///< Transfer operation has finished
    NRF_SEMMC_EVT_XFER_STARTED,                ///< The operation has been accepted and scheduled to be started.
} nrf_semmc_event_type_t;

typedef enum
{
    NRF_SEMMC_RESULT_OK,                       ///< Operation finished successfully
    NRF_SEMMC_RESULT_ABORTED,                  ///< Operation was aborted
} nrf_semmc_event_xfer_done_t;

typedef struct
{
    nrf_semmc_event_type_t type;               ///< Event type
    union
    {
        nrf_semmc_event_xfer_done_t xfer_done; ///< Data for @ref NRF_SQSPI_EVT_XFER_DONE event
    }                      data;               ///< Event-type-specific data
} nrf_semmc_event_t;

/**
 * @brief Callback function type for eMMC driver events.
 *
 * This handler is invoked when an asynchronous operation (such as a command or data transfer)
 * completes or when an error or device interrupt occurs.
 *
 * The function may be called from interrupt context.
 *
 * @param p_event   Pointer to the event structure describing the event.
 * @param p_context Pointer to user-provided context data (as supplied during initialization).
 */
typedef void (* nrf_semmc_event_handler_t)(nrf_semmc_event_t const * p_event, void * p_context);

/**
 * @brief Configuration for initializing an eMMC driver instance.
 */
typedef struct
{
    void *                spis_instance;                     ///< Hardware instance pointer or base address of optional SPI slave(implementation-defined).
    uint32_t              process_response;                  ///< How to process a command's response
    uint32_t              clk_freq_hz;                       ///< Clock frequency in Hz to set during initialization.
    uint32_t              read_delay;                        ///< FLPR clock cycle number to delay data sampling.
    nrf_semmc_bus_width_t bus_width;                         ///< 1, 4, or 8 bits.
} nrf_semmc_config_t;

typedef struct
{
    uint8_t               cmd;                               ///< Command code. @ref nrf_semmc_cmd_t can be used to set this value.
    uint32_t              arg;                               ///< Command argument
    nrf_semmc_resp_type_t resp_type;                         ///< Response type. @ref nrf_semmc_resp_type_t can be used to set this value
    void *                resp_buffer;                       ///< Pointer to the response buffer for transfer. Used for raw response sampling, VPR must have access tho this memory location.
    uint32_t              response[NRF_SEMMC_RESPONSE_SIZE]; ///< Array to hold processed response. Application logic uses this
    nrf_semmc_error_t     err;                               ///< Command error code. It will be set by API after a transfer. A subset of @ref nrf_semmc_error_t
} nrf_semmc_cmd_desc_t;

/**
 * @brief Descriptor for data transfer operations.
 *
 * Note that the eMMC device address for an addressed transfer is specified in the 'arg' field
 * of the associated nrf_semmc_cmd_desc_t structure, so there's no need for a separate
 * block_addr field in this structure.
 */
typedef struct
{
    void *   buffer;     ///< MCU bus address - pointer to data buffer for transfer
    uint32_t block_size; ///< Size of each block in bytes
    uint32_t num_blocks; ///< Number of blocks to transfer
    /*
     * The block_addr is intentionally not included here as it's supplied in the
     * 'arg' field of the associated command descriptor (nrf_semmc_cmd_desc_t)
     */
} nrf_semmc_transfer_desc_t;

/** @brief Flag indicating that the transfer is prepared but not started */
#define NRF_SEMMC_FLAG_HOLD_XFER (1UL << 0)

/**
 * @brief sEMMC driver instance structure.
 */
typedef struct
{
    void * p_reg;        ///< Pointer to a structure with sEMMC virtual register interface.
    uint8_t
           drv_inst_idx; ///< Index of the driver instance. For internal use only.
} nrf_semmc_t;

/**
 * @brief Initialize the eMMC controller driver for a given instance.
 *
 * The data buffer pointed by @p p_semmc is managed by the API user. It must be valid as long
 * as the device driver is initialized and used. After the device driver finishes all its
 * operations and the controller is disabled, the buffer can be freed.
 *
 * @note The GPIO pins required for eMMC operation must be properly configured by the API user
 *       before calling this function.
 *
 * @param[out] p_semmc   Pointer to the eMMC driver instance to initialize.
 * @param[in]  handler   Event handler callback function.
 * @param[in]  p_context User context passed to the event handler.
 *
 * @retval NRF_SEMMC_SUCCESS          Initialization successful.
 * @retval NRF_SEMMC_ERROR_INVALID_PARAM Invalid or NULL parameter provided.
 * @retval NRF_SEMMC_ERROR_HW_FAULT   Hardware initialization failed.
 * @retval NRF_SEMMC_ERROR_INTERNAL   Internal error during initialization.
 */
nrf_semmc_error_t nrf_semmc_init(nrf_semmc_t const *       p_semmc,
                                 nrf_semmc_event_handler_t handler,
                                 void *                    p_context);

/**
 * @brief  Uninitialize the sEMMC driver instance.
 *
 * This function deconfigures the peripheral.
 *
 * @note The function deactivates the peripheral instance if it is active.
 *       The deactivation process releases the clock requests and the power
 * states restrictions related to VPR.
 *
 * @param[in] p_semmc Identifier of the sEMMC instance to uninitialize.
 */
void nrf_semmc_uninit(nrf_semmc_t const * p_semmc);

/**
 * @brief Explicitly power up and configure the controller.
 *
 * Optional: Recommended around sequences of operations to avoid repeated power toggling latency.
 *
 * @param[in] p_semmc Pointer to the eMMC driver instance.
 *
 * @retval NRF_SEMMC_SUCCESS          Controller powered up successfully.
 * @retval NRF_SEMMC_ERROR_INVALID_PARAM Invalid or NULL parameter provided.
 * @retval NRF_SEMMC_ERROR_HW_FAULT   Hardware power-up failed.
 * @retval NRF_SEMMC_ERROR_INTERNAL   Internal error during power-up.
 */
nrf_semmc_error_t nrf_semmc_enable(nrf_semmc_t const * p_semmc);

/**
 * @brief Explicitly power down the controller.
 *
 * Optional: Call after a series of operations to save power.
 *
 * @param[in] p_semmc Pointer to the eMMC driver instance.
 *
 * @retval NRF_SEMMC_SUCCESS          Controller powered down successfully.
 * @retval NRF_SEMMC_ERROR_INVALID_PARAM Invalid or NULL parameter provided.
 * @retval NRF_SEMMC_ERROR_BUSY       Controller is busy with an operation.
 * @retval NRF_SEMMC_ERROR_HW_FAULT   Hardware power-down failed.
 */
nrf_semmc_error_t nrf_semmc_disable(nrf_semmc_t const * p_semmc);

/**
 * @brief Start an asynchronous command.
 *
 * May return immediately with error. The user callback will be invoked on completion.
 * No other driver calls should be made on this instance until the callback fires.
 *
 * It is the caller's responsibility to implement a timeout mechanism to detect
 * if the device becomes unresponsive. The driver itself does not implement timeouts
 * for command completion.
 *
 * For commands that require a STOP_TRANSMISSION command (CMD12) and cleanup afterwards,
 * the caller is responsible for issuing the stop command and
 * performing the necessary cleanup. This applies to the following commands:
 * - NRF_SEMMC_CMD18_READ_MULTIPLE (CMD18)
 * - NRF_SEMMC_CMD25_WRITE_MULTIPLE (CMD25)
 *
 * The caller is responsible for proper cache management of any memory buffers referenced
 * in the p_transfer structure. This includes flushing the cache before writing data to
 * the eMMC device (for data being read by the DMA) and invalidating the cache before
 * reading data received from the eMMC device (for data written by the DMA). Additionally,
 * it is the caller's responsibility to ensure that buffers are properly aligned and padded
 * according to cache line boundaries to prevent data corruption when invalidating the cache.
 *
 * @param p_semmc    Pointer to the eMMC driver instance.
 * @param p_cmd      Pointer to the command descriptor structure.
 * @param p_config   Pointer to the configuration structure.
 * @param p_transfer Pointer to the transfer descriptor structure.
 * @param cmd_count   Number of transfers in the array pointed by @p p_cmd.
 * @param flags      Transfer options (0 for default settings).
 *
 * @retval NRF_SEMMC_SUCCESS         Command started successfully; completion will be reported via callback.
 * @retval NRF_SEMMC_ERROR_BUSY      Driver is busy with another operation.
 * @retval NRF_SEMMC_ERROR_INVALID_PARAM Invalid parameter(s) provided.
 * @retval NRF_SEMMC_ERROR_UNSUPPORTED  Command or operation not supported.
 * @retval NRF_SEMMC_ERROR_INTERNAL  Internal error occurred.
 */
nrf_semmc_error_t nrf_semmc_cmd(nrf_semmc_t const *         p_semmc,
                                nrf_semmc_cmd_desc_t *      p_cmd,
                                nrf_semmc_config_t const *  p_config,
                                nrf_semmc_transfer_desc_t * p_transfer,
                                size_t                      cmd_count,
                                uint32_t                    flags);

/**
 * @brief prepare to start an asynchronous command.
 *
 * This function is an equivalent of @ref nrf_semmc_cmd with @ref
 * NRF_SEMMC_FLAG_HOLD_XFER flag set.
 *
 * The start of the prepared transfer must be triggered outside of this
 * function.
 * @sa nrf_semmc_start_task_address_get
 *
 * If the controller mode transfer is in progress, a new transfer can be
 * requested. If the driver is capable of queueing the requested transfer, the
 * function returns NRF_SEMMC_SUCCESS. If not, it returns NRF_SEMMC_ERROR_BUSY. If the
 * target mode transfer is idle waiting for the remote controller, new transfer
 * requests abort the idling transfer and the @ref nrf_semmc_event_handler_t is
 * called to indicate aborted transfer. If the target mode transfer is activated
 * by the remote controller, a new requested transfer can be queued by the
 * driver. If the driver is capable of queuing the new transfer, the function
 * returns NRF_SEMMC_SUCCESS. Otherwise, it returns NRF_SEMMC_ERROR_BUSY.
 *
 * @param p_semmc    Pointer to the eMMC driver instance.
 * @param p_cmd      Pointer to the command descriptor structure.
 * @param p_config   Pointer to the configuration structure.
 * @param p_transfer Pointer to the transfer descriptor structure.
 * @param cmd_count   Number of transfers in the array pointed by @p p_cmd.
 *
 * @retval NRF_SEMMC_SUCCESS    The transfer is prepared
 * @retval NRF_SEMMC_ERROR_BUSY There is ongoing transfer
 */

nrf_semmc_error_t nrf_semmc_cmd_prepare(nrf_semmc_t const *         p_semmc,
                                        nrf_semmc_cmd_desc_t *      p_cmd,
                                        nrf_semmc_config_t const *  p_config,
                                        nrf_semmc_transfer_desc_t * p_transfer,
                                        size_t                      cmd_count);

/**
 * @brief Get an address of the task register to start the prepared transfer.
 *
 * The transfer to start must be prepared with @ref nrf_semmc_cmd_prepare.
 *
 * To trigger a start task write a non-zero value to the returned address or use
 * the DPPI subsystem.
 *
 * If the device is configured in controller mode, the transfer starts
 * immediately when the task is triggered. If the device is configured in target
 * mode, the task must be triggered first, and then the transfer starts when a
 * remote controller initializes it.
 *
 * @param[in] p_semmc Identifier of the sEMMC instance transferring data.
 *
 * @returns Address of the task register to start the prepared transfer, or NULL
 * if there is no such task.
 */
uint32_t * nrf_semmc_start_task_address_get(nrf_semmc_t const * p_semmc);

/**
 * @brief Abort any ongoing eMMC operation.
 *
 * This function forcibly terminates any pending eMMC command or data transfer
 * operations. It should be used as a recovery mechanism when a command has
 * timed out or the driver appears to be in a stuck state.
 *
 * After calling this function, the driver will attempt to reset internal state
 * and hardware, making it ready to accept new commands.
 *
 * Called by @ref nrf_semmc_disable if there is an ongoing transfer
 *
 * @param p_semmc Pointer to the eMMC driver instance.
 *
 * @retval NRF_SEMMC_SUCCESS          Operation aborted successfully.
 * @retval NRF_SEMMC_ERROR_INVALID_PARAM Invalid or NULL parameter provided.
 * @retval NRF_SEMMC_ERROR_HW_FAULT   Hardware reset failed.
 * @retval NRF_SEMMC_ERROR_INTERNAL   Internal error during abort.
 */
nrf_semmc_error_t nrf_semmc_abort(nrf_semmc_t const * p_semmc);

/**
 * @brief Returns true if an operation is still in progress.
 */
bool nrf_semmc_is_busy(nrf_semmc_t const * p_semmc);

/**
 * @brief IRQ handler to be called from the eMMC interrupt vector.
 */
void nrf_semmc_irq_handler(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif // NRF_SEMMC_H__
