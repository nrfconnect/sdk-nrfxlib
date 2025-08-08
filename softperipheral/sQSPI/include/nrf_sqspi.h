/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef NRF_SQSPI_H__
#define NRF_SQSPI_H__

#include <nrfx.h>
#include <nrf_config_sqspi.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup nrf_sqspi sQSPI driver
 * @{
 *
 * @brief Quad Serial Peripheral Interface soft peripheral (sQSPI) driver
 *
 * This API describes the driver operation of a QSPI peripheral or similar
 * peripherals capable of SPI-like serial data transfers (SPI, multi-line SPI,
 * SSP, Microwire, Hyperbus, etc).
 *
 * This API can be used by both controller and target device in the serial
 * communication bus.
 *
 * The functions in this driver are not enttrant if they operate on the same
 * QSPI peripheral. It is the responsibility of the API user to ensure the
 * functions are not preempted by each other. An exception is the callback
 * function context. Multiple functions from this API can be called from the
 * callback.
 *
 * @note The QSPI peripheral in nRF5340 is not a generic multi-line SPI
 * peripheral. The nrf_sqspi API does not support this peripheral. Use the
 * nrfx_qspi driver for the nRF5340 QSPI.
 */

/** @brief QSPI2 driver instance structure. */
typedef struct
{
    void * p_reg;        ///< Pointer to a structure with QSPI registers.
    uint8_t
           drv_inst_idx; ///< Index of the driver instance. For internal use only.
} nrf_sqspi_t;

/** @brief Asynchronous QSPI events. */
typedef enum
{
    NRF_SQSPI_EVT_XFER_DONE,    ///< Transfer operation has finished
    NRF_SQSPI_EVT_XFER_STARTED, ///< The operation has been accepted and scheduled to be started.
} nrf_sqspi_evt_type_t;

/** @brief Results of the QSPI transfer reported when the transfer is done. */
typedef enum
{
    NRF_SQSPI_RESULT_OK,      ///< Operation finished successfully
    NRF_SQSPI_RESULT_ABORTED, ///< Operation was aborted
} nrf_sqspi_evt_xfer_done_t;

/** @brief QSPI event reported by a @ref nrf_sqspi_callback_t function. */
typedef struct
{
    nrf_sqspi_evt_type_t type; ///< Event type

    union
    {
        nrf_sqspi_evt_xfer_done_t
            xfer_done;         ///< Data for @ref NRF_SQSPI_EVT_XFER_DONE event
    }                    data; ///< Event-type-specific data
} nrf_sqspi_evt_t;

/** @brief Callback function called when an asynchronous operation ends */
typedef void (* nrf_sqspi_callback_t)(nrf_sqspi_t const * p_qspi,
                                      nrf_sqspi_evt_t *   p_event,
                                      void *              p_context);

/**
 * @brief Pins selection for the QSPI peripheral.
 *
 * Pins set to @ref NRF_SQSPI_PINS_UNUSED are not configured and unused by
 * the peripheral.
 *
 * CSN / SS pins are intentionally missing. They are configured for devices in
 * @ref nrf_sqspi_dev_cfg_t.
 */
typedef struct
{
    uint32_t sck;    ///< Clock signal output in the controller mode or input in the
                     ///< target mode
    uint32_t strobe; ///< Clock signal input in the controller mode or output in
                     ///< the target mode

  /**< Useful when a receive data strobe signal or a clock loop-back feature
   *   is available. */
  /** @union @anonymous
   *  @brief Data lines configuration
   */
    union
    {
    /** @struct @anonymous
     *  @brief Data lines in the basic (MOSI/MISO) SPI protocol
     */
        struct
        {
            uint32_t mosi;                    ///< SPI MOSI line
            uint32_t miso;                    ///< SPI MISO line
            uint32_t spi_reserved[NRFX_MAX(
                                      NRF_SQSPI_MAX_NUM_DATA_LINES - 2,
                                      0)];    ///< Set all array elements to @ref NRF_SQSPI_PINS_UNUSED
        };

        uint32_t
            io[NRF_SQSPI_MAX_NUM_DATA_LINES]; ///< Data lines in multi-line
                                              ///< protocols like QSPI, Octal-SPI

    /** @struct @anonymous
     *  @brief Data lines in one-line protocols like Microwire or SSP
     */
        struct
        {
            uint32_t data;                    ///< Data line in one-line protocols like Microwire or SSP
            uint32_t one_line_reserved[NRF_SQSPI_MAX_NUM_DATA_LINES -
                                       1];    ///< Reserved data lines set to @ref
                                              ///< NRF_SQSPI_PINS_UNUSED to indicate
                                              ///< they are not used
        };
    };
} nrf_sqspi_pins_t;

#define NRF_SQSPI_PINS_UNUSED \
  UINT32_MAX                        ///< Value indicating that the pin is not used

/** @brief QSPI driver instance configuration structure. */
typedef struct
{
    nrf_sqspi_pins_t pins;          ///< Pin configuration structure
    bool             skip_gpio_cfg; ///< Skip GPIO configuration of pins
                                    /**< When set to true, the driver does not modify
                                     *   any GPIO parameters of the used pins. Those
                                     *   parameters are supposed to be configured
                                     *   externally before the driver is initialized.
                                     *   When set to false, the driver configures the
                                     *   GPIO parameters of the used pins according
                                     *   to the QSPI requirements defined in the
                                     *   Product Specification. If any additional
                                     *   pin configuration is required, the API user
                                     *   should configure GPIO using nrf_gpio_hal
                                     *   functions after the driver performs its
                                     *   initialization. */
    bool             skip_pmux_cfg; ///< Skip pin multiplexing configuration
                                    /**< When set to true, the driver does not modify
                                     *   PSEL (pin selection) or CTRLSEL registers
                                     *   in the QSPI handling peripheral or GPIO.
                                     *   Those registers are supposed to be set up
                                     *   externally before the driver is initialized.
                                     *   When set to false, the driver configures
                                     *   the PSEL registers in the QSPI-handling
                                     *   peripheral or the CTRLSEL regisers in the
                                     *   GPIO peripheral, whichever is relevant for
                                     *   the SoC.
                                     *   @note When both GPIO configuration and pin
                                     *   selection are to be skipped, the structure
                                     *   fields that specify pins can be omitted,
                                     *   as they are ignored anyway. */

    uint8_t          irq_priority;  ///< Interrupt priority.
  /**< If the @ref nrf_sqspi_irq_handler is not directly registered in the
   *   interrupt vector table, this value is ignored. The external code
   *   calling @ref nrf_sqspi_irq_handler configures the IRQ priority by
   * itself. */
} nrf_sqspi_cfg_t;

/** @brief Modes of the timing synchronization of sampling the input signal. */
typedef enum
{
    NRF_SQSPI_SAMPLE_SYNC_SCK,    ///< Input signal sampling is synchronized with
                                  ///< the SCK line (default)
    NRF_SQSPI_SAMPLE_SYNC_DELAY,  ///< Input signal is sampled configured number
                                  ///< of clock cycles after the SCK line edge
  /**< Used to compensate for the clock signal propagation latency. */
    NRF_SQSPI_SAMPLE_SYNC_CLK_IN, ///< Input signal sampling is synchronized with
                                  ///< a clock input signal
  /**< Clock input is generated by the target device or it is an SCK loop with
   * signal propagation properties close to the data lines. This is used to
   * compensate the clock signal propagation latency. */
} nrf_sqspi_sample_sync_t;

/** @brief Modes of clock polairty and phase for the SPI protocol */
typedef enum
{
    NRF_SQSPI_SPI_CPOLPHA_0, ///< Clock signal low when inactive, data shifted
                             ///< out on falling SCK, data sampled on rising SCK
    NRF_SQSPI_SPI_CPOLPHA_1, ///< Clock signal high when inactive, data shifted
                             ///< out on rising SCK, data sampled on falling SCK
    NRF_SQSPI_SPI_CPOLPHA_2, ///< Clock signal low when inactive, data shifted
                             ///< out on rising SCK, data sampled on falling SCK
    NRF_SQSPI_SPI_CPOLPHA_3, ///< Clock signal high when inactive, data shifted
                             ///< out on falling SCK, data sampled on rising SCK
} nrf_sqspi_spi_cpolpha_t;

/** @brief Protocols and roles of this device */
typedef enum
{
    NRF_SQSPI_PROTO_SPI_C, ///< SPI controller, including multi-line SPI
                           ///< protocols (quad, octal, etc)
    NRF_SQSPI_PROTO_SPI_T, ///< SPI target, including multi-line SPI protocols
    NRF_SQSPI_PROTO_SSP_C, ///< Synchronous Serial Protocol controller
    NRF_SQSPI_PROTO_SSP_T, ///< Synchronous Serial Protocol target
    NRF_SQSPI_PROTO_MW_C,  ///< Microwire controller
    NRF_SQSPI_PROTO_MW_T,  ///< Microwire target
} nrf_sqspi_proto_t;

/** @brief Number of data lines used during each part of an SPI transfer */
typedef enum
{
    NRF_SQSPI_SPI_LINES_SINGLE,      ///< Classic SPI
    NRF_SQSPI_SPI_LINES_DUAL_2_2_2,  ///< Two-line SPI used for the operation,
                                     ///< address, and data phases
    NRF_SQSPI_SPI_LINES_DUAL_1_1_2,  ///< Single line used to transfer the
                                     ///< operation and address, two lines for the
                                     ///< data transfer
    NRF_SQSPI_SPI_LINES_DUAL_1_2_2,  ///< Single line used to transfer the
                                     ///< operation, two lines for the address and
                                     ///< data
    NRF_SQSPI_SPI_LINES_QUAD_4_4_4,  ///< Four-line SPI used for the operation,
                                     ///< address, and data phases
    NRF_SQSPI_SPI_LINES_QUAD_1_1_4,  ///< Single line used to transfer the
                                     ///< operation and address, four lines for
                                     ///< the data transfer
    NRF_SQSPI_SPI_LINES_QUAD_1_4_4,  ///< Single line used to transfer the
                                     ///< operation, four lines for the address
                                     ///< and data
    NRF_SQSPI_SPI_LINES_OCTAL_8_8_8, ///< Eight-line SPI used for the operation,
                                     ///< address, and data phases
    NRF_SQSPI_SPI_LINES_OCTAL_1_1_8, ///< Single line used to transfer the
                                     ///< operation and address, eight lines for
                                     ///< the data transfer
    NRF_SQSPI_SPI_LINES_OCTAL_1_8_8, ///< Single line used to transfer the
                                     ///< operation, eight lines for the address
                                     ///< and data
} nrf_sqspi_spi_lines_t;

/** @brief Single or dual data rate modes used during each part of an SPI
 * transfer */
typedef enum
{
    NRF_SQSPI_SPI_DDR_SINGLE, ///< Classic SPI, single data rate
    NRF_SQSPI_SPI_DDR_S_S_D,  ///< The operation and address are transferred with
                              ///< single data-rate, data with dual
    NRF_SQSPI_SPI_DDR_S_D_D,  ///< The operation is transferred with single
                              ///< data-rate, the address and data with dual
    NRF_SQSPI_SPI_DDR_DUAL,   ///< The operation, address, and data are transferred
                              ///< with dual data-rate (on both clock edges)
} nrf_sqspi_spi_ddr_t;

/** @brief Allowed access modes for XIP (read or write). */
typedef enum
{
    NRF_SQSPI_SPI_XIP_MODE_DISABLED,   ///< XIP feature is disabled
    NRF_SQSPI_SPI_XIP_MODE_READ_ONLY,  ///< XIP feature is enabled for read-only
                                       ///< access
    NRF_SQSPI_SPI_XIP_MODE_READ_WRITE, ///< XIP feature is enabled for read and
                                       ///< write access
} nrf_sqspi_spi_xip_mode_t;

/** @brief Execute-in-place (XIP) configuration
 *
 *  This configures mapping the external memory addresses to an internal memory
 * bus address space. It also configures commands sent to the external memory to
 * read or write on the memory-mapped accesses.
 */
typedef struct
{
    nrf_sqspi_spi_xip_mode_t mode;           ///< Required state of the XIP feature
    uint32_t                 address_offset; ///< Addresses offset between the internal memory bus
                                             ///< and the serial bus
  /**< This address is the address on the memory bus driving the MSPI peripheral
   *   that corresponds to address 0 of the memory device attached to the serial
   *   bus. */
    uint32_t                 size;           ///< Size of the memory mapped region

    uint32_t                 read_cmd;       ///< SPI command to read data from the memory
    uint32_t                 write_cmd;      ///< SPI command to write data to the memory, ignored if
                                             ///< @ref NRF_SQSPI_SPI_XIP_MODE_READ_ONLY
    uint8_t                  cmd_size;       ///< Size of the SPI command, in bits
    uint8_t                  address_size;   ///< Size of the address, in bits
} nrf_sqspi_spi_xip_cfg_t;

/** @brief QSPI device configuration structure */
typedef struct
{
    uint32_t          csn_pin;          ///< CSN pin connected to the device being configured
    uint32_t          sck_freq_khz;     ///< SCK clock frequency in kHz
    nrf_sqspi_proto_t protocol;         ///< Serial protocol for this device
    nrf_sqspi_sample_sync_t
                      sample_sync;      ///< Synchronization of the input signal sampling
    uint16_t          sample_delay_cyc; ///< Delay from the SCK signal edge to the input
                                        ///< signal sampling

  /**< Used only with @ref NRF_SQSPI_SAMPLE_SYNC_DELAY
   *   The unit is the number of cycles clocking the hardware peripheral.
   *   This clock is usually faster than SCK frequency. */

  /** @union @anonymous
   *  @brief Configuration specific to the @c protocol field value
   */
    union
    {
    /** @struct @anonymous
     *  @brief Configuration of the (multi-line) SPI protocol
     */
        struct
        {
            nrf_sqspi_spi_cpolpha_t
                                spi_cpolpha;     ///< Clock line polarization and phase
            nrf_sqspi_spi_lines_t
                                mspi_lines;      ///< Number of data lines used for operation, address, and
                                                 ///< data transfers
            nrf_sqspi_spi_ddr_t mspi_ddr;        ///< Dual data rate configuration for
                                                 ///< operation, address, and data transfers
            bool                spi_clk_stretch; ///< Enable clock stretching when there is not
                                                 ///< enough data to transmit
      /**< Clock stretching is applied when the transmit FIFO gets empty or
       *   when DMA can't fetch enough data in time.
       *   If this feature is disabled and any of the clock stretching
       * conditions occur, the CSN pin gets unasserted. When the missing data
       * becomes available, the CSN pin gets asserted again and the data is
       *   transferred as a new transaction. */
            nrf_sqspi_spi_xip_cfg_t
                                xip_cfg;         ///< Configuration of the memory-mapped accesses
        };

    /** @struct @anonymous
     *  @brief Configuration of the SSP protocol
     */
        struct
        {
      /* Intentionally empty: no additional configuration for SSP */
            char ssp_dummy_field; ///< Dummy field to make the union non-empty
        };

    /** @struct @anonymous
     *  @brief Configuration of the Microwire protocol
     */
        struct
        {
      /* Intentionally empty: no additional configuration for MW */
            char mw_dummy_field; ///< Dummy field to make the union non-empty
        };
    };
} nrf_sqspi_dev_cfg_t;

/** @brief Direction of a single data transfer. */
typedef enum
{
    NRF_SQSPI_XFER_DIR_TXRX, ///< Transmit data and receive data in the same
                             ///< buffer
    NRF_SQSPI_XFER_DIR_TX,   ///< Transmit data only
    NRF_SQSPI_XFER_DIR_RX,   ///< Receive data only (after the command and address
                             ///< is transmitted)
} nrf_sqspi_xfer_dir_t;

/** @brief Configuration of a single data transfer.
 *
 *  @note Single transaction can be created by combining multiple data
 * transfers. This can be used to send data from multiple memory locations in a
 * single transaction.
 */
typedef struct
{
    uint32_t             cmd;          ///< Command to be sent (only for controller mode).
    uint64_t             address;      ///< Address to be accessed (only for controller mode).
    void *               p_data;       ///< Pointer to the data buffer for transmission or reception.
                                       /**< For @ref NRF_SQSPI_XFER_DIR_TXRX, the received data
                                        *   overrides the transmitted data in the same buffer. */
    size_t               data_length;  ///< Length of the data buffer in bytes.
    uint8_t
                         cmd_length;   ///< Length of the command in bits (only for controller mode).
    uint8_t              addr_length;  ///< Length of the address in bits (only for controller
                                       ///< mode).
    uint8_t              dummy_length; ///< Length of the dummy data in SCK cycles (only for
                                       ///< controller mode).
  /**< Dummy cycles is the delay between the address and data part
   *   of the transfer. */
    nrf_sqspi_xfer_dir_t dir;          ///< Direction of the transfer (TX, RX, or TXRX).
} nrf_sqspi_xfer_t;

/** @brief Flag indicating that the transfer is prepared but not started */
#define NRF_SQSPI_FLAG_HOLD_XFER (1UL << 0)

/** @brief Bit order on the serial protocol interface. */
typedef enum
{
    NRF_SQSPI_DATA_FMT_BIT_ORDER_MSB_FIRST, ///< Command bits are sent MSB first
                                            ///< (default)
    NRF_SQSPI_DATA_FMT_BIT_ORDER_LSB_FIRST, ///< Command bits are sent LSB first
} nrf_sqspi_data_fmt_bit_order_t;

/** @brief Configuration of data padding in the memory and serial protocol
 * interface. */
typedef enum
{
    NRF_SQSPI_DATA_FMT_PAD_RAW,              ///< Data is transferred as is to/from the memory
    NRF_SQSPI_DATA_FMT_PAD_MEM_32B_WIRE_8B,  ///< Each transferred 8-bit byte is
                                             ///< 32-bit aligned in the memory
                                             ///< buffer (24 LSBs are unused)
    NRF_SQSPI_DATA_FMT_PAD_MEM_4B_WIRE_32B,  ///< Each 4 bits from the memory are
                                             ///< transferred as a 32-bit word
                                             ///< (remaining LSBs are zeros)
    NRF_SQSPI_DATA_FMT_PAD_MEM_8B_WIRE_32B,  ///< Each 8 bits from the memory are
                                             ///< transferred as a 32-bit word
                                             ///< (remaining LSBs are zeros)
    NRF_SQSPI_DATA_FMT_PAD_MEM_16B_WIRE_32B, ///< Each 16 bits from the memory
                                             ///< are transferred as a 32-bit
                                             ///< word (remaining LSBs are zeros)
} nrf_sqspi_data_fmt_pad_t;

/** @brief Data formatting configuration. */
typedef struct
{
    nrf_sqspi_data_fmt_bit_order_t cmd_bit_order;         ///< Bit order for the command
    nrf_sqspi_data_fmt_bit_order_t addr_bit_order;        ///< Bit order for the address
    nrf_sqspi_data_fmt_bit_order_t data_bit_order;        ///< Bit order for the data
    uint8_t                        data_bit_reorder_unit; ///< Size in bits of a unit in which the data
                                                          ///< bits are reordered
  /**< The data bits are reordered in the units of this size.
   *   The size must be a divisor of 32. */

    uint8_t                        data_container;        ///< Size in bits of the container that gets
                                                          ///< transffered at once
  /**< The size of the data container affects the bit ordering, depending on the
   *   memory buffer endianness and the data bit order.
   *
   *   When the memory endianness is little (default) and the @ref
   * data_bit_order is MSB first, the data is transferred like in this example:
   *   @code
   *    Memory:
   *          |     container     |     container     |
   *    MSB  0bvuts_rqpo_nmlk_jihg_fedc_ba98_7654_3210  LSB 0x2000_0004
   *    MSB  0bVUTS_RQPO_NMLK_JIHG_FEDC_BA98_7654_3210  LSB 0x2000_0000
   *
   *    Serial lines:
   *    bits in order: FEDC_BA98_7654_3210_VUTS_RQPO_NMLK_JIHG
   *                   fedc_ba98_7654_3210_vuts_rqpo_nmlk_jihg
   *   @endcode
   *
   *   When the memory endianness is little (default) and the @ref
   * data_bit_order is LSB first, the container size doesn't matter. Data is
   * transferred like in this example:
   *   @code
   *    Memory:
   *    MSB  0bvuts_rqpo_nmlk_jihg_fedc_ba98_7654_3210  LSB 0x2000_0004
   *    MSB  0bVUTS_RQPO_NMLK_JIHG_FEDC_BA98_7654_3210  LSB 0x2000_0000
   *
   *    Serial lines:
   *    bits in order: 0123_4567_89AB_CDEF_GHIJ_KLMN_OPQR_STUV
   *                   0123_4567_89ab_cdef_ghij_klmn_opqr_stuv
   *   @endcode
   */
    uint8_t                        data_swap_unit;        ///< Size in bits of a unit that is swapped with
                                                          ///< another unit in a data container
  /**< The size must be a divisor of @ref data_container or 0.
   *   If the size is 0, the swapping is disabled.
   *   If the size is non-zero, the data units are swapped in the containers of
   *   @ref data_container size.
   *
   *   When the container size is 24 and the unit size is 4, the swapping is
   *   performed like in this example:
   *
   *   @code
   *    Memory:
   *                                                    |cont 3 ...
   *                      |    |    |    |    |    |    |unit|unit|
   *    0x2000_0008 MSB  0b????_????_????_????_????_????_????_????  LSB
   *
   *                       ... container 3    |    container 2 ...
   *                      |unit|unit|unit|unit|    |    |    |    |
   *    0x2000_0004 MSB  0bvuts_rqpo_nmlk_jihg_fedc_ba)(_*&^%_$#@!  LSB
   *
   *                      ... cont 2|          container 1        |
   *                      |    |    |unit|unit|unit|unit|unit|unit|
   *    0x2000_0000 MSB  0bVUTS_RQPO_NMLK_JIHG_FEDC_BA98_7654_3210  LSB
   *                                          ↓
   *    Swapped data:
   *                                                    |cont 3 ...
   *                MSB  0b    _    _    _    _    _    _jihg_nmlk  LSB
   *
   *                       ... container 3    |    container 2 ...
   *                MSB  0brqpo_vuts_????_????_RQPO_VUTS_$#@!_*&^%  LSB
   *
   *                      ... cont 2|          container 1        |
   *                MSB  0bba)(_fedc_3210_7654_BA98_FEDC_JIHG_NMLK  LSB
   *                                          ↓
   *    Serial lines (with the container size of 24 bits):
   *    bits in order:     3210_7654_BA98_FEDC_JIHG_NMLK_RQPO_VUTS
   *                       $#@!_*&^%_ba)(_fedc_jihg_nmlk_rqpo_vuts
   *                       ????_????
   *   @endcode
   *
   *   The question marks are filled with data units from the next word. */

    nrf_sqspi_data_fmt_pad_t       data_padding;          ///< Padding of the data in the memory
                                                          ///< and in the serial protocol
} nrf_sqspi_data_fmt_t;

/**
 * @brief  Initialize the QSPI driver instance.
 *
 * This function configures the peripheral.
 *
 * @note The function does not activate the peripheral instance. The activation
 * is done during the first transfer after initialization or when calling @ref
 * nrf_sqspi_activate function. The activation process starts the clocks and
 * ensures registers retention of the peripheral. An active peripheral can start
 * transfers with minimal latency.
 *
 * This function configures the SoC peripheral attached to the serial bus.
 * To configure the devices on the bus use @ref nrf_sqspi_dev_cfg.
 *
 * @param[in] p_qspi   Identifier of the QSPI instance to initialize.
 * @param[in] p_config Pointer to the structure with the initial configuration.
 *
 * @retval NRFX_SUCCESS             Initialization was successful.
 * @retval NRFX_ERROR_ALREADY       The driver is already initialized.
 * @retval NRFX_ERROR_INVALID_PARAM The pin configuration is incorrect.
 */
nrfx_err_t nrf_sqspi_init(nrf_sqspi_t const *     p_qspi,
                          nrf_sqspi_cfg_t const * p_config);

/**
 * @brief  Check if the QSPI driver instance is initialized.
 *
 * This function checks whether the specified QSPI driver instance has been
 * initialized.
 *
 * @param[in] p_qspi Identifier of the QSPI instance to check.
 *
 * @retval true  The driver instance is initialized.
 * @retval false The driver instance is not initialized.
 */
bool nrf_sqspi_init_check(nrf_sqspi_t const * p_qspi);

/**
 * @brief  Reconfigure the QSPI driver instance.
 *
 * This function allows changing the configuration of an already initialized
 * QSPI instance.
 *
 * This function must be called when the driver is deactivated.
 * Calling this function on an activated driver instance (e.g. during a
 * transfer) results in an error.
 *
 * @param[in] p_qspi   Identifier of the QSPI instance to reconfigure.
 * @param[in] p_config Pointer to the structure with the new configuration.
 *
 * @retval NRFX_SUCCESS             Reconfiguration was successful.
 * @retval NRFX_ERROR_INVALID_STATE The driver is not initialized or is
 * activated.
 * @retval NRFX_ERROR_INVALID_PARAM The pin configuration is incorrect.
 */
nrfx_err_t nrf_sqspi_reconfigure(nrf_sqspi_t const *     p_qspi,
                                 nrf_sqspi_cfg_t const * p_config);

/**
 * @brief  Uninitialize the QSPI driver instance.
 *
 * This function deconfigures the peripheral.
 *
 * @note The function deactivates the peripheral instance if it is active.
 *       The deactivation process releases the clock requests and the power
 * states restrictions.
 *
 * @param[in] p_qspi Identifier of the QSPI instance to uninitialize.
 */
void nrf_sqspi_uninit(nrf_sqspi_t const * p_qspi);

/**
 * @brief Configure a device attached to the QSPI-controlled bus
 *
 * @param[in]    p_qspi    Identifier of the QSPI instance to which this device
 * is connected.
 * @param[in]    p_config  Pointer to the structure with the device
 * configuration.
 * @param[in]    callback  Function called when an asynchronous operation ends.
 * @param[in]    p_context User-defined context passed as is to the callback
 * function.
 *
 * @retval NRFX_SUCCESS             Device configured successfully
 * @retval NRFX_ERROR_BUSY          The device is during a transfer. Can't
 * reconfigure
 * @retval NRFX_ERROR_NOMEM         Not enough memory to support more serial
 * devices
 * @retval NRFX_ERROR_INVALID_PARAM At least one configuration option has
 * invalid value
 * @retval NRFX_ERROR_NOT_SUPPORTED At least one configuration option is not
 * supported by this driver
 */
nrfx_err_t nrf_sqspi_dev_cfg(nrf_sqspi_t const *         p_qspi,
                             nrf_sqspi_dev_cfg_t const * p_config,
                             nrf_sqspi_callback_t        callback,
                             void *                      p_context);

/**
 * @brief Configure the data format for the device
 *
 * This function configures the data format for the device. The data format is
 * used to convert the memory layout to the serial protocol and vice versa.
 *
 * If the data format is not configured, the driver uses the @ref
 * NRF_SQSPI_DATA_FMT_PAD_RAW data padding format by default, bits are in sent
 * MSB first, no data units swapping is performed.
 *
 * @param[in] p_qspi     Identifier of the QSPI instance for which the data
 * formatting is configured.
 * @param[in] p_data_fmt Data format to set.
 *
 * @retval NRFX_SUCCESS             Data format configured successfully
 * @retval NRFX_ERROR_BUSY          The device is during a transfer. Can't
 * reconfigure
 * @retval NRFX_ERROR_NOT_SUPPORTED The data format is not supported by this
 * driver
 */
nrfx_err_t nrf_sqspi_dev_data_fmt_set(nrf_sqspi_t const *    p_qspi,
                                      nrf_sqspi_data_fmt_t * p_data_fmt);

/**
 * @brief Function for activating the QSPI driver instance.
 *
 * The peripheral controls its GPIOs in the active state. It controls the
 * direction and the polarity of the pins.
 *
 * @param[in] p_qspi Identifier of the QSPI instance to activate.
 *
 * @retval NRFX_SUCCESS       The peripheral is scheduled to be activated.
 * @retval NRFX_ERROR_ALREADY The peripheral is already activated.
 */
nrfx_err_t nrf_sqspi_activate(nrf_sqspi_t const * p_qspi);

/**
 * @brief Function for deactivating the QSPI driver instance.
 *
 * The peripheral may stop controlling its GPIOs when deactivated.
 *
 * If the driver instance is configured with skip_gpio_cfg, the caller of this
 * function should configure GPIOs to avoid glitches when the peripheral gets
 * deactivated.
 *
 * @note If a transfer is ongoing when the function is called, the transfer is
 * aborted.
 *
 * @param[in] p_qspi Identifier of the QSPI instance to deactivate.
 *
 * @retval NRFX_SUCCESS  The driver instance has been activated.
 */
nrfx_err_t nrf_sqspi_deactivate(nrf_sqspi_t const * p_qspi);

/**
 * @brief Transfer data using the serial interface
 *
 * The caller of this function is responsible to ensure that the clock and power
 * trees are prepared for the expected peripheral operations (clock accuracy,
 * blocking sleep states that could result in powering down the peripheral,
 * buses, or GPIO).
 *
 * If @p xfer_count is greater than 1 for the SPI transfer, only the first
 * transfer in the array should have the command and address fields set. The
 * rest of the transfers should have these fields set to 0.
 *
 * The caller of this function must ensure that the memory buffers pointed in
 * the passed arguments are ready for DMA operations (they are in a correct
 * memory region, cache is managed if needed). The caller of this function is
 * not allowed to access the memory buffers (even peek its content) until the
 * requested operation finishes.
 *
 * If the controller mode transfer is in progress, a new transfer can be
 * requested. If the driver is capable of queueing the requested transfer, the
 * function returns NRFX_SUCCESS. If not, it returns NRFX_ERROR_BUSY. If the
 * target mode transfer is idle waiting for the remote controller, new transfer
 * requests abort the idling transfer and the @ref nrf_sqspi_callback_t is
 * called to indicate aborted transfer. If the target mode transfer is activated
 * by the remote controller, a new requested transfer can be queued by the
 * driver. If the driver is capable of queuing the new transfer, the function
 * returns NRFX_SUCCESS. Otherwise, it returns NRFX_ERROR_BUSY.
 *
 * By default, when the @ref NRF_SQSPI_FLAG_HOLD_XFER flag is not set, the
 * transfer starts immediately if the device is configured in a controller mode.
 * If the device is configured in a target mode, the transfer starts when a
 * remote controller initializes it.
 *
 * If the @ref NRF_SQSPI_FLAG_HOLD_XFER flag is set, the transfer is prepared
 * but not started. It must be started by triggering a task obtained from the
 * @ref nrf_sqspi_start_task_address_get function. The task can be started by
 * writing a non-zero value to the task address or by using the DPPI subsystem.
 * If the device is configured in a target mode, the transfer starts after the
 * start task is triggered, followed by the remote controller transfer
 * initialization.
 *
 * This function is asynchronous. When operation is finished, @ref
 * nrf_sqspi_callback_t is called.
 *
 * @param[in] p_qspi     Identifier of the QSPI instance transferring data.
 * @param[in] p_xfer     Pointer to an array of structures describing the data
 * transfer.
 * @param[in] xfer_count Number of transfers in the array pointed by @p p_xfer.
 * @param[in] flags      Transfer options (0 for default settings).
 *
 * @retval NRFX_SUCCESS    The transfer is started
 * @retval NRFX_ERROR_BUSY There is ongoing transfer or XIP mode blocks other
 * transfers
 */
nrfx_err_t nrf_sqspi_xfer(nrf_sqspi_t const *      p_qspi,
                          nrf_sqspi_xfer_t const * p_xfer,
                          size_t                   xfer_count,
                          uint32_t                 flags);

/**
 * @brief Prepare the serial interface to transfer the data.
 *
 * This function is an equivalent of @ref nrf_sqspi_xfer with @ref
 * NRF_SQSPI_FLAG_HOLD_XFER flag set.
 *
 * The start of the prepared transfer must be triggered outside of this
 * function.
 * @sa nrf_sqspi_start_task_address_get
 *
 * If the controller mode transfer is in progress, a new transfer can be
 * requested. If the driver is capable of queueing the requested transfer, the
 * function returns NRFX_SUCCESS. If not, it returns NRFX_ERROR_BUSY. If the
 * target mode transfer is idle waiting for the remote controller, new transfer
 * requests abort the idling transfer and the @ref nrf_sqspi_callback_t is
 * called to indicate aborted transfer. If the target mode transfer is activated
 * by the remote controller, a new requested transfer can be queued by the
 * driver. If the driver is capable of queuing the new transfer, the function
 * returns NRFX_SUCCESS. Otherwise, it returns NRFX_ERROR_BUSY.
 *
 * @param[in] p_qspi     Identifier of the QSPI instance transferring data.
 * @param[in] p_xfer     Pointer to a structure describing the data transfer.
 * @param[in] xfer_count Number of transfers in the array pointed by @p p_xfer.
 *
 * @retval NRFX_SUCCESS    The transfer is prepared
 * @retval NRFX_ERROR_BUSY There is ongoing transfer
 */
nrfx_err_t nrf_sqspi_xfer_prepare(nrf_sqspi_t const *      p_qspi,
                                  nrf_sqspi_xfer_t const * p_xfer,
                                  size_t                   xfer_count);

/**
 * @brief Get an address of the task register to start the prepared transfer.
 *
 * The transfer to start must be prepared with @ref nrf_sqspi_xfer_prepare.
 *
 * To trigger a start task write a non-zero value to the returned address or use
 * the DPPI subsystem.
 *
 * If the device is configured in controller mode, the transfer starts
 * immediately when the task is triggered. If the device is configured in target
 * mode, the task must be triggered first, and then the transfer starts when a
 * remote controller initializes it.
 *
 * @param[in] p_qspi Identifier of the QSPI instance transferring data.
 *
 * @returns Address of the task register to start the prepared transfer, or NULL
 * if there is no such task.
 */
uint32_t * nrf_sqspi_start_task_address_get(nrf_sqspi_t const * p_qspi);

/**
 * @brief IRQ handler function for the QSPI driver.
 *
 * This function might be registered directly in the interrupt vector table by
 * one of the header files in the soc/ directory. If it is not, this function
 * must be called from the QSPI interrupt service routine.
 */
void nrf_sqspi_irq_handler(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* NRF_SQSPI_H__ */
