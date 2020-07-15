/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**
 * @file mpsl_fem_protocol_api.h
 *
 * @defgroup mpsl_fem MPSL Protocol interface for Power Amplifier (PA) and Low Noise Amplifier (LNA).
 * @ingroup  mpsl
 *
 * This module enables control of peripherals before and after the radio transmission and the radio reception
 * in order to control a Power Amplifier or a Low Noise Amplifier, or both.
 *
 * The application must first provide PA and LNA device-specific configuration parameters to this module.
 * The protocol must then provide PA and LNA protocol configuration parameters before it can use the functionality.
 *
 * When the PA/LNA module is configured, the stack can call the provided enable functions before radio activity
 * to enable the PA or LNA timer configurations for the upcoming radio activity.
 *
 * @{
 */

#ifndef MPSL_FEM_PROTOCOL_API_H__
#define MPSL_FEM_PROTOCOL_API_H__

#include <stdint.h>
#include <stdbool.h>

#include <nrfx.h>
#include "nrf_errno.h"

/**
 * @brief PA and LNA functionality types.
 */
typedef enum
{
    MPSL_FEM_PA  = 1 << 0,                     /**< PA Functionality. */
    MPSL_FEM_LNA = 1 << 1,                     /**< LMA Functionality. */
    MPSL_FEM_ALL = MPSL_FEM_PA | MPSL_FEM_LNA  /**< Both PA and LNA Functionalities. */
} mpsl_fem_functionality_t;

/**
 * @brief PA and LNA activation event types.
 */
typedef enum
{
    MPSL_FEM_EVENT_TYPE_TIMER,                 /**< Timer Event type. */
    MPSL_FEM_EVENT_TYPE_GENERIC                /**< Generic Event type. */
} mpsl_fem_event_type_t;

/**
 * @brief MPSL Front End Module event.
 *
 * The event can be a Timer Compare event or a Generic event (field @ref mpsl_fem_event_type_t type).
 * 
 * Timer Compare event represents an event which is to be scheduled in some near time. In order to schedule that event, the protocol
 * must provide:
 * - The instance of the timer, which the protocol has to start itself.
 * - Compare Channels mask, which tells the Front End Module which Compare Channels of the provided Timer are free to use.
 *   The one with the lowest ID of those provided in the mask shall be populated with the timestamp after which the PA/LNA set up is finished.
 *   The provided Timer shall be stopped after that by the short configured by the Front End Module.
 * - Start time, at which the Front End Module can start preparing the PA or LNA.
 * - End time, at which the Front End Module must be ready for the RF procedure.
 * 
 * The protocol can use these start and end times to plan the consecutive radio operations. For example, the protocol can schedule the 
 * activation of LNA to listen for some time and transmit a frame right after that. The enabling of PA and/or LNA usually takes time, so
 * the protocol can realize this scenario in the following way (for simplicity, deactivation of PA/LNA is skipped):
 * 
 * 1. The protocol sets the LNA to be activated by the event of type @ref mpsl_fem_event_type_t::MPSL_FEM_EVENT_TYPE_TIMER, with Timer TIMER0, using Compare Channel CC0 and start 0, end 40.
 * 2. Let's say the Front End Module requires 13 microseconds to enable it. The Front End Module shall set the Compare Channel to the value 40-13=27.
 * 3. The protocol can read the value of CC0 and determine that TIMER0 shall be stopped at the value 27.
 * 4. The protocol wires the Hardware Event which ends the listening with the TIMER0's TASK_START task via some PPI.
 * 5. The protocol sets the PA to be activated by the event of type @ref mpsl_fem_event_type_t::MPSL_FEM_EVENT_TYPE_TIMER, with Timer TIMER0, using Compare Channel CC1 and start 27, end 27+40=67.
 * 6. The protocol starts TIMER0.
 * 
 * The following chart illustrates the flow of Radio operations described in the above scenario:
 *
 *                                   +
 *                                   | Hardware Event
 *                                   |
 *                                   |
 *                                   v
 *  
 *            +---+----------------------+------------------------------------------------+
 *            |R  |                  |R  |                                                |
 *            |A  |                  |A  |                                                |
 *            |M  |  Rx              |M  |   Tx                                           |
 *            |P U|                  |P U|                                                |
 *            |  P|                  |  P|                                                |                                          Time
 *  +---------+----------------------+----------------------------------------------------+------------------------------------------------------>
 *            0   40               1000  1040                                           5000
 *  
 *  
 *                                                                                                                            Timer Value
 *  +-------------------------------------------------------------------------------------------------------------------------------------------->
 *            0  27                 27  54
 *  
 *  
 *            +--+                   +--+                                                                                     Timer Runs
 *  +-------------------------------------------------------------------------------------------------------------------------------------------->
 *  
 *  
 *  
 *                                        +------------------------------------------------+                                  PA
 *  +-------------------------------------------------------------------------------------------------------------------------------------------->
 *                +------------------+                                                                                        LNA
 *   
 * 
 * 
 * Generic Event represents the Generic Hardware event. In order to schedule that event, the protocol has to provide its address in the
 * register_address field.
 * 
 */
typedef struct
{
    mpsl_fem_event_type_t      type;                 /**< Type of event source. */
    union
    {
        struct
        {
            NRF_TIMER_Type *   p_timer_instance;     /**< Pointer to a 1-us resolution timer instance. */
            struct
            {
                uint32_t       start;                /**< Timer value when the Front End Module can start preparing PA/LNA. */
                uint32_t       end;                  /**< Timer value at which the PA/LNA have to be prepared. Radio operation shall start at this point. */
            } counter_period;                        /**< Time interval in which the timer should start and end. */
            uint8_t            compare_channel_mask; /**< Mask of the compare channels that can be used by the Front End Module to schedule its own tasks. */
        } timer;                                     /**< Event generated by the timer, used in case of type equal to @ref mpsl_fem_event_type_t::MPSL_FEM_EVENT_TYPE_TIMER. */
        struct
        {
            uint32_t           register_address;     /**< Address of event register. */
        } generic;                                   /**< Generic event, used in case of type equal to @ref mpsl_fem_event_type_t::MPSL_FEM_EVENT_TYPE_GENERIC. */
    }                          event;                /**< Implementation of the event. */
    bool                       override_ppi;         /**< False to ignore the PPI channel below and use the one set by application. True to use the PPI channel below. */
    uint8_t                    ppi_ch_id;            /**< PPI channel to be used for this event. */
} mpsl_fem_event_t;

/**
 * @brief Sets up PA using the provided events for the upcoming radio transmission.
 *
 * Multiple configurations can be provided by repeating calls to this function (that is, you can set the activate and the deactivate events in multiple calls,
 * and the configuration is preserved between calls).
 *
 * The order of calls of this function and its `lna` counterpart must match the order of radio operations.
 * I.e. if you want to listen first and then send the frame, you need first to issue @ref mpsl_fem_lna_configuration_set and only after @ref mpsl_fem_pa_configuration_set.
 *
 * If a @ref mpsl_fem_event_type_t::MPSL_FEM_EVENT_TYPE_TIMER timer event is provided, the PA will be configured to activate or deactivate at the application-configured time gap
 * before the timer instance reaches the given register_value. The time gap is set via the corresponding configuration setter of the selected Front End Module.
 *
 * If a @ref mpsl_fem_event_type_t::MPSL_FEM_EVENT_TYPE_GENERIC event is provided, the PA will be configured to activate or deactivate when an event occurs.
 *
 * The function sets up the PPIs and the GPIOTE channel to activate PA for the upcoming radio transmission.
 * The PA pin will be active until deactivated, which can happen either by encountering a configured deactivation event or by using @ref mpsl_fem_deactivate_now.
 *
 * @param[in] p_activate_event   Pointer to the activation event structure.
 * @param[in] p_deactivate_event Pointer to the deactivation event structure.
 *
 * @pre To activate PA, the corresponding configuration setter of the selected Front End Module must have been called first.
 *
 * @note If a timer event is provided, the caller of this function is responsible for starting the timer and configuring its shorts.
 *       Moreover, the caller is responsible for stopping the timer no earlier than the compare channel of the lowest ID among the provided ones does expire.
 *
 * @retval   0             PA activation setup is successful.
 * @retval   -NRF_EPERM    PA is currently disabled.
 * @retval   -NRF_EINVAL   PA activation setup could not be performed due to invalid or missing configuration parameters
 *                         in p_activate_event or p_deactivate_event, or both.
 */
int32_t mpsl_fem_pa_configuration_set(const mpsl_fem_event_t * const p_activate_event,
                                      const mpsl_fem_event_t * const p_deactivate_event);

/**
 * @brief Clears up the configuration provided by the @ref mpsl_fem_pa_configuration_set function.
 *
 * @retval   0             PA activation setup purge is successful.
 * @retval   -NRF_EPERM    PA is currently disabled.
 */
int32_t mpsl_fem_pa_configuration_clear(void);

/**
 * @brief Sets up LNA using the provided event for the upcoming radio reception.
 *
 * Multiple configurations can be provided by repeating calls to this function (that is, you can set the activate and the deactivate event in multiple calls,
 * and the configuration is preserved between calls).
 *
 * The order of calls of this function and its `pa` counterpart must match the order of radio operations.
 * I.e. if you want to listen first and then send the frame, you need first to issue @ref mpsl_fem_lna_configuration_set and only after @ref mpsl_fem_pa_configuration_set.
 *
 * If a @ref mpsl_fem_event_type_t::MPSL_FEM_EVENT_TYPE_TIMER timer event is provided, the LNA will be configured to activate or deactivate at the application-configured time gap
 * before the timer instance reaches the given register_value. The time gap is set via the corresponding configuration setter of the selected Front End Module.
 *
 * If a @ref mpsl_fem_event_type_t::MPSL_FEM_EVENT_TYPE_GENERIC event is provided, the LNA will be configured to activate or deactivate when an event occurs.
 *
 * The function sets up the PPIs and the GPIOTE channel to activate LNA for the upcoming radio transmission.
 * The LNA pin will be active until deactivated, which can happen either by encountering a configured deactivation event or by using @ref mpsl_fem_deactivate_now.
 *
 * @param[in] p_activate_event   Pointer to the activation event structure.
 * @param[in] p_deactivate_event Pointer to the deactivation event structure.
 *
 * @pre To activate LNA, the corresponding configuration setter of the selected Front End Module must have been called first.
 *
 * @note If a timer event is provided, the caller of this function is responsible for starting the timer and configuring its shorts.
 *       Moreover, the caller is responsible for stopping the timer no earlier than the compare channel of the lowest ID among the provided ones does expire.
 *
 * @retval   0             LNA activation setup is successful.
 * @retval   -NRF_EPERM    LNA is currently disabled.
 * @retval   -NRF_EINVAL   LNA activation setup could not be performed due to invalid or missing configuration parameters
 *                         in p_activate_event or p_deactivate_event, or both.
 */
int32_t mpsl_fem_lna_configuration_set(const mpsl_fem_event_t * const p_activate_event,
                                       const mpsl_fem_event_t * const p_deactivate_event);

/**
 * @brief Clears up the configuration provided by the @ref mpsl_fem_lna_configuration_set function.
 *
 *
 * @retval   0             LNA activate setup purge is successful.
 * @retval   -NRF_EPERM    LNA is currently disabled.
 */
int32_t mpsl_fem_lna_configuration_clear(void);

/**
 * @brief Deactivates PA/LNA with immediate effect - contrary to @ref mpsl_fem_lna_configuration_clear or @ref mpsl_fem_pa_configuration_clear,
 *        which both just set up the infrastructure for events which shall disable the PA/LNA.
 * 
 * @param[in] type Whether to deactivate imeediately PA, LNA, or both (see @ref mpsl_fem_functionality_t).
 */
void mpsl_fem_deactivate_now(mpsl_fem_functionality_t type);

/**
 * @brief Instruct Front End Module to disable PA and LNA as soon as possible using the group following the event.
 *
 * @param[in] event          Address of the event which is triggered when the abort condition occurs.
 * @param[in] group          PPI Group which shall be disabled when the abort event is triggered.
 *
 * @retval   0               Setting of the abort sequence path is successful.
 * @retval   -NRF_EPERM      Setting of the abort sequence path could not be performed.    
 */
int32_t mpsl_fem_abort_set(uint32_t event, uint32_t group);

/**
 * @brief Adds one more PPI channel to the PPI Group prepared by the @ref mpsl_fem_abort_set function.
 *
 * @param[in] channel_to_add PPI channel to add to the PPI group.
 * @param[in] group          The said PPI group.
 *
 * @retval    0              Setting of the abort sequence path is successful.
 * @retval    -NRF_EPERM     Setting of the abort sequence path could not be performed.
 */
int32_t mpsl_fem_abort_extend(uint32_t channel_to_add, uint32_t group);

/**
 * @brief Removes one PPI channel from the PPI Group prepared by the @ref mpsl_fem_abort_set function.
 *
 * @param[in] channel_to_remove PPI channel to remove from the PPI group.
 * @param[in] group             The said PPI group.
 *
 * @retval   0                  Setting of the abort sequence path is successful.
 * @retval   -NRF_EPERM         Setting of the abort sequence path could not be performed.
 */
int32_t mpsl_fem_abort_reduce(uint32_t channel_to_remove, uint32_t group);

/**
 * @brief Clears up the configuration provided by the @ref mpsl_fem_abort_set function.
 *
 * @retval   0               Clearing of the abort sequence path is successful.
 * @retval   -NRF_EPERM      Clearing was not done - the possible reason is that there was nothing to clear.
 */
int32_t mpsl_fem_abort_clear(void);

/**
 * @brief Cleans up the configured PA/LNA timer/radio instance and corresponding allocated hardware resources.
 * The function resets the hardware that has been set up for the PA/LNA activation. The PA and LNA module control configuration parameters are not deleted.
 * The function is intended to be called after the radio disable signal.
 */
void mpsl_fem_cleanup(void);

/**
 * @brief Checks if the PA signaling is configured and enabled, and gets the configured gain in dB.
 *
 * @param[out] p_gain The configured gain in dB if PA is configured and enabled.
 *                    If there is no PA present or the PA does not affect the signal gain, returns 0 dB.
 */
void mpsl_fem_pa_is_configured(int8_t * const p_gain);

/**
 * @brief Prepares the Front End Module module to switch to the Power Down state.
 *        The timer is owned by the protocol and must be started by the protocol.
 *        The timer stops after matching the provided compare channel (the call sets the short).
 *
 * @param[in] p_instance      Timer instance that is used to schedule the transition to the Power Down state.
 * @param[in] compare_channel Compare channel to hold a value for the timer.
 * @param[in] ppi_id          ID of the PPI channel used to switch to the Power Down state.
 * @param[in] event_addr      Address of the event which shall trigger the Timer start.
 *
 * @retval true               Whether the scheduling of the transition was successful.
 * @retval false              Whether the scheduling of the transition was not successful.
 */
bool mpsl_fem_prepare_powerdown(NRF_TIMER_Type * p_instance,
                                uint32_t         compare_channel,
                                uint32_t         ppi_id,
                                uint32_t         event_addr);

#endif // MPSL_FEM_PROTOCOL_API_H__

/**@} */
