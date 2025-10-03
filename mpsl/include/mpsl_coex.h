/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_coex.h
 *
 * @defgroup mpsl_coex MPSL interface for Bluetooth External Radio Coexistence
 * @ingroup  mpsl
 *
 * This module provides the necessary interface for communication between the Bluetooth stack and a Packet Traffic Arbitrator.
 * @{
 */


#ifndef MPSL_COEX_H__
#define MPSL_COEX_H__

#include <stdint.h>
#include <stdbool.h>

#include "nrf.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MPSL_COEX_ESCALATION_THRESHOLD_OFF  UINT8_MAX   /**< Setting for coexistence interface API to switch off escalation. */

/**
 * @brief Possible priorities to handle related BLE event.
 */
enum MPSL_COEX_PRIORITY
{
  MPSL_COEX_PRIORITY_LOW,    /**< Low priority for the related coexistence interface event. */
  MPSL_COEX_PRIORITY_HIGH,   /**< High priority for the related coexistence interface event. */
  MPSL_COEX_PRIORITY_NUMBER  /**< Number of priorities in the coexistence interface. */
};

/**
 * @brief Ids of the supported coexistence interfaces.
 */
typedef enum
{
  MPSL_COEX_802152_3WIRE_GPIOTE_ID = 0, /**< 3-wire GPIO coexistence interface
                                             based on IEEE 802.15.2-2003, subclause 6.
                                             To support this interface,
                                             @ref mpsl_coex_support_802152_3wire_gpiote_if()
                                             must be called. */
  MPSL_COEX_1WIRE_GPIOTE_ID        = 1, /**< 1-wire GPIO coexistence interface.
                                             To support this interface,
                                             @ref mpsl_coex_support_1wire_gpiote_if()
                                             must be called. */
} mpsl_coex_if_id_t;

/**
 * @brief Configuration parameters for one wire of the 3-wire coexistence interface GPIOTE based interface.
 */
typedef struct
{
  uint8_t gpio_pin;        /**< The GPIO pin number. */
  uint8_t active_high;     /**< Active pin level: 1 - high level, 0 - low level. */
  uint8_t ppi_ch_id;       /**< PPI channel number. */
  uint8_t gpiote_ch_id;    /**< GPIOTE channel number. */
} mpsl_coex_gpiote_cfg_t;

/**
 * @brief GPIOs configuration for 3-wire coexistence arbitrator.
 *
 * 3-wire coexistence protocol applies the packet traffic arbitration recommendation
 * based on IEEE Std 802.15.2-2003, subclause 6.
 * 3-wire interface is exposed by following signals:
 * REQUEST  -   Asserted whenever the user of MPSL wants to transmit or receive.
 *              The signal must remain asserted till the end of TX/RX or the request is rejected by PTA controller.
 * PRI_STATUS - Asserted to indicate the priority (high or low) of the intended
 *              traffic immediately following REQUEST. After that, it is changed to indicate TX or RX.
 * GRANT      - Asserted to indicate whether the request is granted or rejected.
 *
 * The request is made at @p type_delay_us + @p radio_delay_us before the radio activity starts. The Priority and Status line will
 * first indicate the priority of the request, then after @p type_delay_us, it'll switch to showing the type of transaction (RX/TX).
 * The radio will start transmitting or receiving @p radio_delay_us after this.
 * Sum of @p type_delay_us + @p radio_delay_us cannot be more than 40us due to radio ramp-up time.
 */
typedef struct
{
  mpsl_coex_gpiote_cfg_t request_cfg;   /**< Request line configuration. */
  mpsl_coex_gpiote_cfg_t priority_cfg;  /**< Priority and Status line configuration. */
  mpsl_coex_gpiote_cfg_t grant_cfg;     /**< Grant line configuration. */

  uint8_t  additional_ppi_ch_id;     /**< PPI channel used to set the priority level at the correct time.
                                          Since Priority and Status are combined functional, it requires separate PPI channel to handle both.  */
  uint8_t  is_rx_active_level : 1;   /**< Since Priority and Status are combined functional, the signal of the high priority is active level on the pin.
                                          Which active level to use for the RX/TX signal is set by this variable. */
  uint32_t type_delay_us;            /**< Delay in us from the request pin is raised until the Priority and Status line shows the type of transaction (RX/TX). */
  uint32_t radio_delay_us;           /**< Delay in us from the Priority and Status line shows the type of transaction, to the radio starts its on-air activity. */
  NRF_TIMER_Type *p_timer_instance;  /**< Pointer to a timer instance. Timer should not be shared with any other functionality. */
} mpsl_coex_802152_3wire_gpiote_if_t;

/**
 * @brief Concurrency mode with the external modem supported by 1-wire coexistence.
 */
typedef enum
{
  MPSL_COEX_1WIRE_CONCURRENCY_NONE    = 0, /**< 1-wire configuration to allow no concurrency with the external modem. */
  MPSL_COEX_1WIRE_CONCURRENCY_RX_ONLY = 1, /**< 1-wire configuration to allow RX only concurrency with the external modem. */
} mpsl_coex_1wire_concurrency_mode_t;

/**
 * @brief GPIOs configuration for 1-wire coexistence arbitrator.
 *
 * 1-wire interface is exposed by following signals:
 * GRANT - Asserted to indicate whether the external modem is active.
 *
 */
typedef struct
{
  mpsl_coex_gpiote_cfg_t grant_cfg;                    /**< Grant line configuration. */
  mpsl_coex_1wire_concurrency_mode_t concurrency_mode; /**< Concurrency mode configuration. */
} mpsl_coex_1wire_gpiote_if_t;

/**
 * @brief Configuration of the coexistence interface.
 *
 * This option configures MPSL to support interface for requesting and granting access to the media
 * and status notification in case of coexistence with another wireless standard.
 */
typedef struct
{
  mpsl_coex_if_id_t if_id;                                   /**<  Identification number of configured interface. */
  union
  {
    mpsl_coex_802152_3wire_gpiote_if_t coex_3wire_gpiote;    /**<  Configuration parameters of 3-wire GPIO based coexistence. */
    mpsl_coex_1wire_gpiote_if_t coex_1wire_gpiote;           /**<  Configuration parameters of 1-wire GPIO based coexistence. */
  } interfaces;                                              /**<  Union with supported coexistence interfaces. */
} mpsl_coex_if_t;

/** @brief Type of callback invoked from coexistence interface upon completion
 *         of enable or disable procedure.
 * The callback must accept calls in both high- and low-priority threads.
 */
typedef void mpsl_coex_on_complete_callback_t(void);

/** @brief Enable support for the 802.15.2 3-Wire coexistence interface.
 *
 * After this API is called, it is possible to configure MPSL with the coexistence interface
 * @ref MPSL_COEX_802152_3WIRE_GPIOTE_ID.
 */
void mpsl_coex_support_802152_3wire_gpiote_if(void);

/** @brief Enable support for the 1-Wire coexistence interface.
 *
 * After this API is called, it is possible to configure MPSL with the coexistence interface
 * @ref MPSL_COEX_1WIRE_GPIOTE_ID.
 */
void mpsl_coex_support_1wire_gpiote_if(void);

/** @brief Configures and enables the coexistence interface.
 *
 *  @note The function configures the interface and maps
 *        the other interface functions on exact coexistence implementation
 *
 *  @param[in] p_cfg
 *      Pointer to coexistence interface configuration structure. Any
 *      configuration with if_id set to MPSL_COEX_IDS_NUMBER is invalid.
 *  @param[in] enable_complete_callback
 *      Callback to be invoked when the coexistence interface completes the
 *      enabling procedure. The callback must accept calls in both high- and
 *      low-priority threads.
 *
 *  @retval  0              The configuration for the coexistence interface was
 *                          successfully set and the enabling procedure operation
 *                          was initiated. The supplied callback will be called
 *                          when the enabling procedure is completed.
 *  @retval -NRF_EINVAL     Invalid configuration or NULL callback provided.
 *  @retval -NRF_EAGAIN     Enabling was not allowed at the time because the
 *                          coexistence interface was already enabled or in an
 *                          ongoing enable or disable procedure. The call aborted
 *                          without any effect.
 *  @retval -NRF_EOPNOTSUPP The provided interface is not supported.
 *                          All interfaces must be explicitly enabled. See @ref mpsl_coex_if_id_t.
 */
int32_t mpsl_coex_enable(const mpsl_coex_if_t *p_cfg,
                         mpsl_coex_on_complete_callback_t enable_complete_callback);

/** @brief Disables the coexistence interface.
 *
 *  This releases all peripherals supplied to @ref mpsl_coex_enable for
 *  application use. The released GPIOs will be reset to high-impedance mode
 *  with no pull.
 *
 * @param[in] disable_complete_callback
 *      Callback to be invoked when coexistence interface completes the
 *      disabling procedure. The callback must accept calls in both high- and
 *      low-priority threads.
 *
 * @retval 0            The disabling procedure for the coexistence interface
 *                      was initiated. The supplied callback will be called when
 *                      the disable procedure is completed.
 * @retval -NRF_EAGAIN  Disabling was not allowed at the time because the
 *                      coexistence interface was already disabled or in an
 *                      ongoing enable or disable procedure. The call aborted
 *                      without any effect.
 * @retval -NRF_EINVAL  Invalid NULL callback provided.
 */
int32_t mpsl_coex_disable(mpsl_coex_on_complete_callback_t disable_complete_callback);

#ifdef __cplusplus
}
#endif
#endif // MPSL_COEX_H__

/**
  @}
 */
