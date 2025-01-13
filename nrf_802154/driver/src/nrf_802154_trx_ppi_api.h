/*
 * Copyright (c) 2020, Nordic Semiconductor ASA
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 * @brief Module that contains PPI management functions for
 *        the nRF IEEE 802.15.4 radio driver.
 */

#ifndef NRF_802154_TRX_PPI_H_
#define NRF_802154_TRX_PPI_H_

#include <stdbool.h>
#include <stdint.h>

#include "hal/nrf_egu.h"
#include "hal/nrf_radio.h"
#include "nrf_802154_trx.h"

/**
 * @brief Configures (D)PPI connections required for TRX operation.
 *
 * Actions performed by this function in DPPI variant:
 *   - RADIO_DISABLED event is published to DPPI designated by NRF_802154_DPPI_RADIO_DISABLED
 *   - NRF_RADIO_EVENT_CCAIDLE is published to DPPI designated by NRF_802154_DPPI_RADIO_CCAIDLE
 * The DPPIs mentioned above may be then subscribed to.
 */
void nrf_802154_trx_ppi_for_enable(void);

/**
 * @brief Clears (D)PPI connections required for TRX operation.
 */
void nrf_802154_trx_ppi_for_disable(void);

/**
 * @brief Set PPIs to connect trigger event with tasks needed to ramp up.
 *
 * When @p trigg_mode is TRX_RAMP_UP_SW_TRIGGER, the trigger event is RADIO_DISABLED and
 * PPI connections are made to it.
 * When @p trigg_mode is TRX_RAMP_UP_HW_TRIGGER, the trigger event is defined outside the module
 * and PPI connections are only partially created. To complete the connection creation, the trigger
 * event must be connected to the (D)PPI channel specified with
 * @ref nrf_802154_trx_ppi_for_ramp_up_channel_id_get.
 *
 * Connections created by this function in DPPI variant and TRX_RAMP_UP_SW_TRIGGER mode:
 *
 *       RADIO_DISABLED ----> EGU -----> ramp_up_task
 *                      |          \--> self disable
 *               if (start_timer)
 *                      \-------------> TIMER_START
 *
 * Connections created by this function in DPPI variant and TRX_RAMP_UP_HW_TRIGGER mode:
 *
 *       [DPPI] -----------------------> RADIO_TASK_DISABLE
 *
 *       RADIO_DISABLED ----> EGU -----> ramp_up_task
 *                      |           \--> self disable
 *               if (start_timer)
 *                      \--------------> TIMER_START
 *
 * Connections created by this function in PPI variant and TRX_RAMP_UP_SW_TRIGGER mode:
 *
 *      RADIO_DISABLED ----> EGU -----> ramp_up_task
 *                                 \--> self disable
 *
 *      EGU ---> if (start_timer) ----> TIMER_START
 *
 * Connections created by this function in PPI variant and TRX_RAMP_UP_HW_TRIGGER mode:
 *
 *      [PPI] --------> EGU ----------> ramp_up_task
 *                                 \--> self disable
 *
 *      EGU ---> if (start_timer) ----> TIMER_START
 *
 * @param[in]  ramp_up_task  Task triggered to start ramp up procedure.
 * @param[in]  trigg_mode    Trigger mode the connections must conform to.
 * @param[in]  start_timer   If timer is to be started on RADIO DISABLED event.
 */
void nrf_802154_trx_ppi_for_ramp_up_set(nrf_radio_task_t                      ramp_up_task,
                                        nrf_802154_trx_ramp_up_trigger_mode_t trigg_mode,
                                        bool                                  start_timer);

/**
 * @brief Set (D)PPIs to connect trigger event with tasks needed to ramp up for a transmission of a frame.
 *
 * @param[in] cca         Transmission with preceding CCA operation.
 * @param[in] trigg_mode  Trigger mode the connections must conform to.
 */
void nrf_802154_trx_ppi_for_txframe_ramp_up_set(
    bool                                  cca,
    nrf_802154_trx_ramp_up_trigger_mode_t trigg_mode);

/**
 * @brief Set (D)PPIs to perform CCA procedures back-to-back.
 */
void nrf_802154_trx_ppi_for_extra_cca_attempts_set(void);

/**
 * @brief Reconfigure (D)PPIs for the next steps in receiving or transmitting.
 *
 * Due to limited resources on some platforms, some PPIs have many uses: when starting an operation,
 * one is used to trigger the radio ramp up, then when the radio is "up", this PPI must be
 * reconfigured so it can be used to disable the radio when needed.
 * This is the PPI reconfiguration routine. It is intended to be called in the radio READY event
 * handler.
 */
void nrf_802154_trx_ppi_for_ramp_up_reconfigure(void);

/**
 * @brief Clear (D)PPIs that are configured for ramp up procedure.
 *
 * @param[in]  ramp_up_task  Task triggered to start ramp up procedure.
 * @param[in]  start_timer   If timer start on RADIO DISABLED event is to be deconfigured as well. See @ref nrf_802154_trx_ppi_for_ramp_up_set.
 */
void nrf_802154_trx_ppi_for_ramp_up_clear(nrf_radio_task_t ramp_up_task, bool start_timer);

/**
 * @brief Clear (D)PPIs that are configured for ramp up procedure for a transmission of a frame.
 *
 * @note Complementary to @ref nrf_802154_trx_ppi_for_txframe_ramp_up_set.
 *
 * @param cca   Transmission with preceding CCA operation.
 */
void nrf_802154_trx_ppi_for_txframe_ramp_up_clear(bool cca);

/**
 * @brief Clear (D)PPIs to perform CCA procedures back-to-back.
 */
void nrf_802154_trx_ppi_for_extra_cca_attempts_clear(void);

/**
 * @brief Get (D)PPI channel used to trigger ramp up procedure start.
 *
 * @retval  NRF_802154_DPPI_EGU_TO_RADIO_RAMP_UP in DPPI variant.
 * @retval  NRF_802154_PPI_RADIO_RAMP_UP_TRIGG in PPI variant.
 */
uint8_t nrf_802154_trx_ppi_for_ramp_up_channel_id_get(void);

/**
 * @brief Wait until PPIs configured to ramp up radio are propagated through PPI system.
 *
 * During detection if trigger of DISABLED event caused start of hardware procedure, detecting
 * function needs to wait until event is propagated from RADIO through PPI to EGU. This delay is
 * required to make sure EGU event is set if hardware was prepared before DISABLED event was
 * triggered.
 */
void nrf_802154_trx_ppi_for_ramp_up_propagation_delay_wait(void);

/**
 * @brief Detect if PPIs configured to start radio operation were triggered.
 *
 * In TRX_RAMP_UP_SW_TRIGGER mode, radio ramp up starts by design from RADIO DISABLED event.
 * This function verifies occurrence of this event and PPIs status.
 *
 * The function is intended to be used only when all of the following conditions apply:
 *   - the connections are already made with @ref nrf_802154_trx_ppi_for_ramp_up_set
 *   - TRX_RAMP_UP_SW_TRIGGER mode was used to create connections
 *   - connections have not yet been cleared
 *
 * @retval  true   PPIs were triggered.
 * @retval  false  PPIs were not triggered. To trigger them, the caller must trigger RADIO DISABLE task.
 */
bool nrf_802154_trx_ppi_for_ramp_up_was_triggered(void);

/**
 * @brief Set PPIs to connect TIMER event with radio TXEN task, needed to ramp up for ACK TX.
 */
void nrf_802154_trx_ppi_for_ack_tx_set(void);

/**
 * @brief Clear PPIs to connect TIMER event with radio TXEN task, needed to ramp up for ACK TX. See @ref nrf_802154_trx_ppi_for_ack_tx_set
 */
void nrf_802154_trx_ppi_for_ack_tx_clear(void);

/**
 * @brief Configure PPIs needed for external LNA or PA. Radio DISABLED event will be connected to timer START task.
 * As a result, FEM ramp-up will be scheduled during the radio ramp-up period, with timing based on FEM implementation used.
 */
void nrf_802154_trx_ppi_for_fem_set(void);

/**
 * @brief Deconfigure PPIs needed for external LNA or PA. See @ref nrf_802154_trx_ppi_for_fem_set
 */
void nrf_802154_trx_ppi_for_fem_clear(void);

/**
 * @brief Get PPI group id used for disabling radio operations by an external event.
 */
uint32_t nrf_802154_trx_ppi_group_for_abort_get(void);

/**
 * @brief Configure PPIs needed to trigger IRQ from RADIO event SYNC.
 *
 * @param[in] task EGU task used for triggering IRQ on radio SYNC event.
 */
void nrf_802154_trx_ppi_for_radio_sync_set(nrf_egu_task_t task);

/**
 * @brief Unconfigure PPIs needed to trigger IRQ from RADIO event SYNC.
 *
 * @param[in] task EGU task used for triggering IRQ on radio SYNC event. See @ref nrf_802154_trx_ppi_for_radio_sync_set
 */
void nrf_802154_trx_ppi_for_radio_sync_clear(nrf_egu_task_t task);

#endif /* NRF_802154_TRX_PPI_H_ */
