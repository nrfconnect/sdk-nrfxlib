/*
 * Copyright (c) 2019 - 2021, Nordic Semiconductor ASA
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
 * @brief Module that contains radio hardware-related functions
 *        of the nRF IEEE 802.15.4 radio driver.
 *
 * @details
 */

#ifndef NRF_802154_TRX_H_
#define NRF_802154_TRX_H_

#include <stdbool.h>
#include <stdint.h>

#include "nrf_802154_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief States of the FSM of the TRX module. */
typedef enum
{
    TRX_STATE_DISABLED = 0,
    TRX_STATE_IDLE,
    TRX_STATE_GOING_IDLE,
    TRX_STATE_RXFRAME,

    /* PPIS disabled deconfigured
     * RADIO is DISABLED, RXDISABLE
     * RADIO shorts are 0
     * TIMER is running
     * FEM is going to powered or is powered depending if RADIO reached DISABLED
     */
    TRX_STATE_RXFRAME_FINISHED,

    TRX_STATE_RXACK,
    TRX_STATE_TXFRAME,
    TRX_STATE_TXACK,
    TRX_STATE_STANDALONE_CCA,
    TRX_STATE_CONTINUOUS_CARRIER,
    TRX_STATE_MODULATED_CARRIER,
    TRX_STATE_ENERGY_DETECTION,

    /* PPIS disabled deconfigured
     * RADIO is DISABLED, TXDISABLE, RXDISABLE
     * RADIO shorts are 0
     * TIMER is stopped
     * FEM is going to powered or is powered depending if RADIO reached DISABLED
     */
    TRX_STATE_FINISHED
} trx_state_t;

/**@brief Notifications that can be enabled for @ref nrf_802154_trx_receive_frame operation. */
typedef enum
{
    /**@brief No notifications during frame receive provided. */
    TRX_RECEIVE_NOTIFICATION_NONE = 0U,

    /**@brief Notification @ref nrf_802154_trx_receive_frame_prestarted enabled. */
    TRX_RECEIVE_NOTIFICATION_PRESTARTED = (1U << 0),

    /**@brief Notification @ref nrf_802154_trx_receive_frame_started enabled. */
    TRX_RECEIVE_NOTIFICATION_STARTED = (1U << 1),
} nrf_802154_trx_receive_notifications_t;

/**@brief Notifications that can be enabled for @ref nrf_802154_trx_transmit_frame operation. */
typedef enum
{
    /**@brief No notifications during frame transmission provided. */
    TRX_TRANSMIT_NOTIFICATION_NONE = 0U,

    /**@brief Notification @ref nrf_802154_trx_transmit_frame_ccaidle */
    TRX_TRANSMIT_NOTIFICATION_CCAIDLE = (1U << 1),

    /**@brief Notification @ref nrf_802154_trx_transmit_frame_ccastarted */
    TRX_TRANSMIT_NOTIFICATION_CCASTARTED = (1U << 2)
} nrf_802154_trx_transmit_notifications_t;

/**@brief Initializes trx module.
 *
 * This function must be called exactly once, before any other API call. This function sets internal state
 * of trx module into @c DISABLED. It initializes also those peripherals that are used exclusively
 * by trx module and are not shared when trx module is in @c DISABLED state.
 */
void nrf_802154_trx_init(void);

/**@brief Enables trx module.
 *
 * Effects of call to this function:
 * - The RADIO peripheral is fully reset and configured into 802.15.4 mode.
 * - FEM module state is set to powered off.
 * - PPIs used by trx module are disabled.
 * - TIMER used by trx module is shutdown.
 * - Trx module is in @c IDLE state
 *
 * @warning This function may be called when:
 * - Trx module was in @c DISABLED state (after @ref nrf_802154_trx_init or after @ref nrf_802154_trx_disable).
 * - HFCLK clock is activated.
 * - Access to the RADIO peripheral is granted (applicable when the RADIO is shared with other drivers).
 *
 * @note When trx was in @c DISABLED state, the RADIO might have been controlled by other drivers. Thus
 * full reset of the RADIO peripheral is performed.
 */
void nrf_802154_trx_enable(void);

/**@brief Disables trx module.
 *
 * This function has no effects when the trx module is in @c DISABLED state.
 * Otherwise following effects apply:
 * - The RADIO peripheral is fully reset.
 * - FEM module state is set to powered off.
 * - all PPSs used by trx module are disabled and cleared.
 * - TIMER used by trx module is shutdown.
 * - Trx module is in @c DISABLED state
 *
 * @note On call to this function any activity of trx module is stopped. If any operation was executing, it
 * will be stopped and no handler will be called. After call HFCLK may be deactivated and
 * the RADIO peripheral may be passed to other driver.
 */
void nrf_802154_trx_disable(void);

/**
 * @brief Updates currently used antenna.
 *
 * This function sets the antenna to be used based on antenna diversity interface
 * configuration and TRX state. See @ref nrf_802154_sl_ant_div_mode_set,
 * @ref nrf_802154_sl_ant_div_antenna_set.
 */
void nrf_802154_trx_antenna_update(void);

/**@brief Sets radio channel to use.
 *
 * @param[in] channel   Channel number to set (11-26).
 */
void nrf_802154_trx_channel_set(uint8_t channel);

/**@brief Updates CCA configuration in the RADIO peripheral according to PIB. */
void nrf_802154_trx_cca_configuration_update(void);

/**@brief Puts the trx module into receive frame mode.
 *
 * The frame will be received into buffer set by @ref nrf_802154_trx_receive_buffer_set.
 *
 * When NRF_802154_DISABLE_BCC_MATCHING == 0
 * - during receive @ref nrf_802154_trx_receive_frame_started handler is called when
 *   SHR field of a frame being received is received (only when @p notifications_mask contained @ref TRX_RECEIVE_NOTIFICATION_STARTED flag)
 * - during receive @ref nrf_802154_trx_receive_frame_bcmatched handler is called when
 *   @p bcc octets are received.
 * - when a frame is received with correct crc, @ref nrf_802154_trx_receive_frame_received is called
 * - when a frame is received with incorrect crc, @ref nrf_802154_trx_receive_frame_crcerror is called
 *
 * When NRF_802154_DISABLE_BCC_MATCHING != 0
 * - @ref nrf_802154_trx_receive_frame_prestarted handler is called when the RADIO initially syncs on received frame.
 *   This handler is called when @p notifications_mask contains @ref TRX_RECEIVE_NOTIFICATION_PRESTARTED.
 * - @ref nrf_802154_trx_receive_frame_started handler is called when
 *   SHR field of a frame being received is received (only when @p notifications_mask contained @ref TRX_RECEIVE_NOTIFICATION_STARTED flag)
 * - after the frame is received with correct crc, @ref nrf_802154_trx_receive_frame_received is called
 * - after the frame is received with incorrect crc:
 *     - when NRF_802154_NOTIFY_CRCERROR == 0:
 *         - the hardware restarts receive automatically
 *         - no handler is called
 *     - when NRF_802154_NOTIFY_CRCERROR != 0:
 *         - the hardware restarts receive automatically
 *         - @ref nrf_802154_trx_receive_frame_crcerror is called
 *
 * When in @ref nrf_802154_trx_receive_frame_received, the TIMER is running allowing sending response (e.g. ACK frame)
 * in time regime by a call to nrf_802154_trx_transmit_after_frame_received.
 *
 * @note To receive ACK use @ref nrf_802154_trx_receive_ack
 *
 * @param[in] bcc   Number of received bytes after which @ref nrf_802154_trx_receive_frame_bcmatched will be called.
 *                  This must not be zero if @ref NRF_802154_DISABLE_BCC_MATCHING == 0.
 *                  When @ref NRF_802154_DISABLE_BCC_MATCHING != 0, this value must be zero.
 * @param[in] notifications_mask Selects additional notifications generated during a frame reception.
 *                  It is bitwise combination of @ref nrf_802154_trx_receive_notifications_t values.
 *                  When NRF_802154_DISABLE_BCC_MATCHING != 0, flag @ref TRX_RECEIVE_NOTIFICATION_PRESTARTED is forbidden.
 */
void nrf_802154_trx_receive_frame(uint8_t                                bcc,
                                  nrf_802154_trx_receive_notifications_t notifications_mask);

/**@brief Puts the trx module into receive ACK mode.
 *
 * The ack frame will be received into buffer set by @ref nrf_802154_trx_receive_buffer_set.
 *
 * During receive of an ack:
 * - @ref nrf_802154_trx_receive_ack_started is called when a frame has just started being received.
 * - when a frame is received with correct crc, @ref nrf_802154_trx_receive_ack_received is called.
 * - when a frame is received with incorrect crc, @ref nrf_802154_trx_receive_ack_crcerror is called.
 * - no bcmatch events are generated.
 */
void nrf_802154_trx_receive_ack(void);

/**@brief Starts RSSI measurement.
 *
 * @note This function succeeds when TRX module is in receive frame state only (started with @ref nrf_802154_trx_receive_frame)
 *
 * @retval true     When RSSI measurement has been started.
 * @retval false    When TRX state didn't allow start of RSSI measurement.
 */
bool nrf_802154_trx_rssi_measure(void);

/**@brief Checks if RSSI measurement is currently started.
 *
 * @retval true     When RSSI measurement is currently started. In this case user can
 *                  check if RSSI sample is already available by call to @ref nrf_802154_trx_rssi_sample_is_available.
 * @retval false    When RSSI measurement is not started.
 */
bool nrf_802154_trx_rssi_measure_is_started(void);

/**@brief Checks if RSSI sample is available.
 *
 * @retval true    When RSSI sample is available.  The sample may be read by a call to @ref nrf_802154_trx_rssi_last_sample_get
 * @retval false   When RSSI sample is unavailable.
 */
bool nrf_802154_trx_rssi_sample_is_available(void);

/**@brief Returns last measured RSSI sample.
 *
 * @return RSSI sample. Returned value must be scaled using API provided by nrf_802154_rssi.h.
 */
uint8_t nrf_802154_trx_rssi_last_sample_get(void);

/**@brief Check if PSDU is currently being received.
 *
 * @retval true     If trx is in receive mode triggered by @ref nrf_802154_trx_receive_frame and
 *                  a frame receive on air has been started but not finished yet.
 * @retval false    Otherwise.
 *
 * @note This function returns false when receive has been triggered by @ref nrf_802154_trx_receive_ack
 *       regardless of the fact if the frame on air has been started or not.
 */
bool nrf_802154_trx_psdu_is_being_received(void);

/**@brief Checks if current receive operation was started without providing receive buffer.
 *
 * It may happen that @ref nrf_802154_trx_receive_frame or @ref nrf_802154_trx_receive_ack have been started
 * when there was no receive buffer set. The RADIO peripheral will start ramping up, but it will remain
 * in @c RXIDLE state, because of missing receive buffer. This function allows to check if such situation
 * occurred.
 *
 * Usually this function may be called by buffer management subsystem when buffer becomes available.
 * Consider following code snippet:
 * @code
 * void buffer_is_available_callback(void * p_buffer)
 * {
 *     if (nrf_802154_trx_receive_is_buffer_missing())
 *     {
 *         nrf_802154_trx_receive_buffer_set(p_buffer);
 *     }
 * }
 * @endcode
 *
 * @retval true When in receive mode and receive buffer is missing
 * @retval false Otherwise.
 */
bool nrf_802154_trx_receive_is_buffer_missing(void);

/**@brief Sets pointer to a receive buffer.
 *
 * @param p_receive_buffer If NULL the next call to @ref nrf_802154_trx_receive_frame or
 *                         @ref nrf_802154_trx_receive_ack will not be able to receive.
 *                         If not NULL it must point to MAX_PACKET_SIZE + 1 (see nrf_802154_const.h)
 *                         buffer where received frame will be stored.
 *
 * @retval true  If operation solved missing buffer condition (see @ref nrf_802154_trx_receive_is_buffer_missing)
 *               and provided buffer will be used in current receive operation.
 * @retval false If operation didn't solve missing buffer condition (either no missing buffer or currently
 *               not in receive mode). Provided buffer will be used in next receive operation.
 */
bool nrf_802154_trx_receive_buffer_set(void * p_receive_buffer);

/**@brief Begins frame transmit operation.
 *
 * This operation performs differently according to cca parameter.
 * When cca==false:
 * - The RADIO starts ramp up in transmit mode.
 * - The RADIO starts sending synchronization header (SHR).
 * - @ref nrf_802154_trx_transmit_frame_started handler is called from an ISR just after SHR is sent
 *   (only when @ref NRF_802154_TX_STARTED_NOTIFY_ENABLED == 1).
 * - @ref nrf_802154_trx_transmit_frame_transmitted handler is called from an ISR after full frame is sent on air.
 *
 * When cca==true:
 * - The radio starts ramp up in receive mode, then it starts cca procedure.
 *     - The @ref nrf_802154_trx_transmit_frame_ccastarted handler is called from an ISR when
 *       the RADIO started CCA procedure and @p notifications_mask contained
 *       @ref TRX_TRANSMIT_NOTIFICATION_CCASTARTED flag.
 * - If cca succeded (channel was idle):
 *     - The RADIO switches to transmit mode (disables receive mode, starts ramp up in transmit mode).
 *     - The RADIO starts sending sending synchronization header (SHR).
 *     - If @ref TRX_TRANSMIT_NOTIFICATION_CCAIDLE was present in notifications_mask,
 *       the @ref nrf_802154_trx_transmit_frame_ccaidle is called.
 *     - @ref nrf_802154_trx_transmit_frame_started handler is called from an ISR just after SHR is sent
 *       (only when @ref NRF_802154_TX_STARTED_NOTIFY_ENABLED == 1).
 *     - @ref nrf_802154_trx_transmit_frame_transmitted handler is called from an ISR after full frame is sent on air.
 * - If cca failed (channel was busy):
 *     - The RADIO disables receive mode
 *     - @ref nrf_802154_trx_transmit_frame_ccabusy from an ISR handler is called
 *
 * @param p_transmit_buffer  Pointer to a buffer containing frame to transmit.
 *                           Must not be NULL. p_transmit_buffer[0] is the number of
 *                           bytes following p_transmit_buffer[0] to send.
 *                           The number of bytes pointed by p_transmit buffer must
 *                           be at least 1 and not less than p_transmit_buffer[0] + 1.
 *
 * @param cca                Selects if CCA procedure should be performed prior to
 *                           real transmission. If false no cca will be performed.
 *                           If true, cca will be performed.
 *
 * @param notifications_mask Selects additional notifications generated during a frame transmission.
 *                           It is bitwise combination of @ref nrf_802154_trx_transmit_notifications_t values.
 * @note To transmit ack after frame is received use @ref nrf_802154_trx_transmit_ack.
 */
void nrf_802154_trx_transmit_frame(const void                            * p_transmit_buffer,
                                   bool                                    cca,
                                   nrf_802154_trx_transmit_notifications_t notifications_mask);

/**@brief Puts the trx module into transmit ACK mode.
 *
 * @note This function may be called from @ref nrf_802154_trx_receive_received handler only.
 *       This is because in this condition only the TIMER peripheral is running and allows timed transmission.
 *
 * @param[in] p_transmit_buffer     Pointer to a buffer containing ACK frame to be transmitted.
 *                                  Caller is responsible for preparing an ACK frame according to the 802.15.4 protocol.
 * @param[in] delay_us              Delay (in microseconds)
 *
 * @retval true     If the function was called in time and ACK frame is scheduled for transmission.
 *                  When transmission starts and @ref NRF_802154_TX_STARTED_NOTIFY_ENABLED != 0
 *                  the function @ref nrf_802154_trx_transmit_ack_started will be called.
 *                  When transmission is finished the function @ref nrf_802154_trx_transmit_ack_transmitted
 *                  will be called.
 * @retval false    If the function was called too late and given delay_us time gap
 *                  between received frame and ACK frame transmission could not be hold.
 *                  The TIMER peripheral is stopped and it is not possible to trigger @ref nrf_802154_trx_transmit_ack
 *                  again without receiving another frame again. No handlers will be called.
 */
bool nrf_802154_trx_transmit_ack(const void * p_transmit_buffer, uint32_t delay_us);

/**@brief Puts trx module into IDLE mode.
 *
 * @retval true     If entering into IDLE mode started, @ref nrf_802154_trx_go_idle_finished will be called
 *                  (if not aborted by call to @ref nrf_802154_trx_abort or nrf_802154_trx_disable).
 * @retval false    If already in IDLE mode or just requested @ref nrf_802154_trx_go_idle. There will be
 *                  no @ref nrf_802154_trx_go_idle_finished handler being result of this function.
 */
bool nrf_802154_trx_go_idle(void);

/**@brief Starts standalone cca operation.
 *
 * Operation ends with a call to @ref nrf_802154_trx_standalone_cca_finished handler from an ISR.
 */
void nrf_802154_trx_standalone_cca(void);

/**@brief Starts generating continuous carrier.
 *
 * Generation of a continuous carrier generates no handlers. It may be terminated by a call to
 * @ref nrf_802154_trx_abort or @ref nrf_802154_trx_disable.
 */
void nrf_802154_trx_continuous_carrier(void);

/**@brief Restarts generating continuous carrier
 *
 * When continuous carrier was being generated and channel change was requested by a call to @ref nrf_802154_trx_channel_set.
 * The frequency is not changed automatically. Use @ref nrf_802154_trx_continuous_carrier_restart to
 * stop generating continuous carrier on old frequency and start this operation on a new frequency.
 * @ref nrf_802154_trx_continuous_carrier_restart is usually faster than
 * call to @ref nrf_802154_trx_abort @ref nrf_802154_trx_continuous_carrier
 */
void nrf_802154_trx_continuous_carrier_restart(void);

/**@brief Starts generating modulated carrier with given buffer.
 *
 * @param[in] p_transmit_buffer Pointer to a buffer used for modulating the carrier wave.
 */
void nrf_802154_trx_modulated_carrier(const void * p_transmit_buffer);

/** @brief Restarts generating modulated carrier.*/
void nrf_802154_trx_modulated_carrier_restart(void);

/**@brief Puts trx module into energy detection mode.
 *
 * Operation ends up with a call to @ref nrf_802154_trx_energy_detection_finished handler.
 *
 * Operation can be terminated with a call to @ref nrf_802154_trx_abort or @ref nrf_802154_trx_disable.
 * In this case no handler is called.
 *
 * @param ed_count  Number of iterations to perform. Must be in range 1..2097152.
 *                  One iteration takes 128 microseconds.
 */
void nrf_802154_trx_energy_detection(uint32_t ed_count);

/**@brief Aborts currently performed operation.
 *
 * When trx module is in @c DISABLED, @c IDLE or @c FINISHED state, this function has no effect.
 * Otherwise current operation is terminated and no handler will be generated by the operation
 * regardless of its state. In this case trx module will be in @c FINISHED state allowing
 * commencement of a next operation.
 */
void nrf_802154_trx_abort(void);

/**@brief Gets current state of the TRX module.
 *
 * @return Current state of the TRX module.*/
trx_state_t nrf_802154_trx_state_get(void);

/**@brief Handler called at the beginning of a ACK reception.
 *
 * This handler is called from an ISR when receive of an ack has been started, and
 * the RADIO received synchronization header (SHR).
 */
extern void nrf_802154_trx_receive_ack_started(void);

/**@brief Handler called at the beginning of frame reception (earliest possible moment).
 *
 * This handler is called from an ISR when:
 * - receive operation has been started with a call to @ref nrf_802154_trx_receive_frame
 * - the RADIO peripheral detected energy on channel or started synchronizing to it.
 *
 * When this handler is called following holds:
 * - trx module is in @c RXFRAME state.
 *
 * Implementation of @ref nrf_802154_trx_receive_frame_started may call:
 * - @ref nrf_802154_trx_abort if current receive frame needs to be aborted
 *   (usually followed by a call moving the trx module out of @c FINISHED state)
 * - @ref nrf_802154_trx_disable
 *
 * @note This handler may be triggered several times during receive of a preamble
 *       of a frame. It can be followed by call to @ref nrf_802154_trx_receive_frame_started
 *       (if enabled) and then by @ref nrf_802154_trx_receive_frame_crcok or
 *       @ref nrf_802154_trx_receive_frame_crcerror, but there is possibility
 *       that it will not be followed by these calls (In case when the RADIO didn't found
 *       full preamble.). If implementation of this handler starts some
 *       activity that must be terminated by a further call, it must implement
 *       its own timeout feature.
 */
extern void nrf_802154_trx_receive_frame_prestarted(void);

/**@brief Handler called at the beginning of frame reception.
 *
 * This handler is called from an ISR when:
 * - receive operation has been started with a call to @ref nrf_802154_trx_receive_frame
 * - the RADIO started receiving a frame and has just received SHR field of the frame.
 *
 * When this handler is called following holds:
 * - trx module is in @c RXFRAME state.
 *
 * Implementation of @ref nrf_802154_trx_receive_frame_started may call:
 * - @ref nrf_802154_trx_abort if current receive frame needs to be aborted
 *   (usually followed by a call moving the trx module out of @c FINISHED state)
 * - @ref nrf_802154_trx_disable
 */
extern void nrf_802154_trx_receive_frame_started(void);

/**@brief  Handler called during reception of a frame, when given number of bytes is received.
 *
 * This handler is called from an ISR when given number of bytes (see @ref nrf_802154_trx_receive)
 * have been just received.
 *
 * @note If the handler decides to abort receive by a call to @ref nrf_802154_trx_abort or
 *       @ref nrf_802154_trx_disable it must return value equal to original bcc parameter passed.
 *
 * @param[in]   bcc   Number of bytes that have been already received.
 *
 * @return  Value greater than original value of bcc parameter will cause @ref nrf_802154_trx_receive_frame_bcmatched
 *          to be called again when further data arrive. Value less than or equal to original bcc value will not cause this
 *          behavior.
 */
extern uint8_t nrf_802154_trx_receive_frame_bcmatched(uint8_t bcc);

/**@brief Handler called when a frame is received with correct crc.
 *
 * This handler is called from an ISR when:
 * - receive operation has been started with a call to @ref nrf_802154_trx_receive_frame
 * - the RADIO received a frame on air with correct crc
 *
 * When this handler is called following holds:
 * - trx module is in @c RXFRAME_FINISHED state.
 * - the RADIO peripheral started ramping down (or it ramped down already)
 * - TIMER peripheral started counting allowing @ref nrf_802154_trx_transmit_ack
 *
 * Implementation of @ref nrf_802154_trx_receive_frame_received is responsible for
 * leaving @c RXFRAME_FINISHED state. It may do this by call to:
 * - @ref nrf_802154_trx_transmit_ack,
 * - @ref nrf_802154_trx_receive_frame,
 * - @ref nrf_802154_trx_transmit_frame,
 * - @ref nrf_802154_trx_go_idle,
 * - @ref nrf_802154_trx_disable.
 */
extern void nrf_802154_trx_receive_frame_received(void);

/**@brief Handler called when a frame is received with incorrect crc.
 *
 * This handler is called from an ISR when:
 * - receive operation has been started with a call to @ref nrf_802154_trx_receive_frame
 * - the RADIO received a frame on air with incorrect crc
 *
 * If NRF_802154_DISABLE_BCC_MATCHING == 0:
 * When this handler is called following holds:
 * - the RADIO peripheral started ramping down (or it ramped down already)
 * - trx module is in @c FINISHED state.
 *
 * Implementation of @ref nrf_802154_trx_receive_frame_crcerror is responsible for
 * leaving @c FINISHED state. It may do this by call to:
 * - @ref nrf_802154_trx_receive_frame,
 * - @ref nrf_802154_trx_transmit_frame,
 * - @ref nrf_802154_trx_go_idle,
 * - @ref nrf_802154_trx_disable.
 *
 * If NRF_802154_DISABLE_BCC_MATCHING != 0:
 * - the RADIO peripheral is restarted automatically (by hardware) in receive mode
 * - trx module stays in @c RXFRAME state
 * Implementation of @ref nrf_802154_trx_receive_frame_crcerror should not call
 * @ref nrf_802154_trx_receive_frame as receive is restarted automatically by the hardware.
 * If the implementation wishes to change state it should call
 * @ref nrf_802154_trx_abort first.
 */
extern void nrf_802154_trx_receive_frame_crcerror(void);

/**@brief Handler called when an ack is received with correct crc.
 *
 * This handler is called from an ISR when:
 * - receive ack operation has been started with a call to @ref nrf_802154_trx_receive_ack
 * - the RADIO received a frame on air with correct crc
 *
 * When this handler is called following holds:
 * - the RADIO peripheral started ramping down (or it ramped down already)
 * - trx module is in @c FINISHED state.
 *
 * Implementation is responsible for:
 * - checking if received frame is an ack and matches previously transmitted frame.
 * - leaving @c FINISHED state. It may do this by call to:
 *     - @ref nrf_802154_trx_receive_frame,
 *     - @ref nrf_802154_trx_transmit_frame,
 *     - @ref nrf_802154_trx_go_idle,
 *     - @ref nrf_802154_trx_disable.
 */
extern void nrf_802154_trx_receive_ack_received(void);

/**@brief Handler called when an ack is received with incorrect crc.
 *
 * This handler is called from an ISR when:
 * - receive ack operation has been started with a call to @ref nrf_802154_trx_receive_ack
 * - the RADIO received a frame on air with incorrect crc
 *
 * When this handler is called following holds:
 * - the RADIO peripheral started ramping down (or it ramped down already)
 * - trx module is in @c FINISHED state.
 *
 * Implementation is responsible for:
 * - leaving @c FINISHED state. It may do this by call to:
 *     - @ref nrf_802154_trx_receive_frame,
 *     - @ref nrf_802154_trx_transmit_frame,
 *     - @ref nrf_802154_trx_go_idle,
 *     - @ref nrf_802154_trx_disable.
 */
extern void nrf_802154_trx_receive_ack_crcerror(void);

/**@brief Handler called when a cca operation during transmit attempt started.
 *
 * This handler is called from an ISR when:
 * - transmit operation with cca has been started with a call to @ref nrf_802154_transmit_frame(cca=true).
 * - transmit operation was started with parameter @c notifications_mask containing
 *   TRX_TRANSMIT_NOTIFICATION_CCASTARTED
 * - the RADIO peripheral started CCA operation.
 *
 * Implementation is not responsible for anything related to the trx module since it serves as
 * a pure notification of the channel assessment started event during transmission.
 */
extern void nrf_802154_trx_transmit_frame_ccastarted(void);

/**@brief Handler called when a cca operation during transmit attempt was successful.
 *
 * This handler is called from an ISR when:
 * - transmit operation with cca has been started with a call to @ref nrf_802154_transmit_frame(cca=true).
 * - the RADIO detected that channel was free.
 *
 * When this handler is called following holds:
 * - the RADIO peripheral started ramping up (or it ramped up already)
 * - trx module is in @c TXFRAME state.
 *
 * Implementation is not responsible for anything since it serves as
 * a pure notification of the successful channel assessment during transmission.
 */
extern void nrf_802154_trx_transmit_frame_ccaidle(void);

/**@brief Handler called when a cca operation during transmit attempt failed.
 *
 * This handler is called from an ISR when:
 * - transmit operation with cca has been started with a call to @ref nrf_802154_transmit_frame(cca=true).
 * - the RADIO detected that channel was busy.
 *
 * When this handler is called following holds:
 * - the RADIO peripheral started ramping down (or it ramped down already)
 * - trx module is in @c FINISHED state.
 *
 * Implementation is responsible for:
 * - leaving @c FINISHED state. It may do this by call to:
 *     - @ref nrf_802154_trx_receive_frame,
 *     - @ref nrf_802154_trx_transmit_frame,
 *     - @ref nrf_802154_trx_go_idle,
 *     - @ref nrf_802154_trx_disable.
 */
extern void nrf_802154_trx_transmit_frame_ccabusy(void);

/**@brief Handler called when frame transmission has just started.
 *
 * This handler is called from an ISR when:
 * - @ref NRF_802154_TX_STARTED_NOTIFY_ENABLED == 1 (see nrf_802154_config.h)
 * - transmit operation was started by a call to @ref nrf_802154_trx_transmit_frame.
 * - the RADIO peripheral sent synchronization header
 *
 * When this handler is called following holds:
 * - trx module is in @c TXFRAME state
 *
 * Implementation may (but does not need to) terminate transmission if it wishes by a call to:
 * - @ref nrf_802154_trx_abort,
 * - @ref nrf_802154_trx_disable.
 */
extern void nrf_802154_trx_transmit_frame_started(void);

/**@brief Handler called when frame transmission has just been finished.
 *
 * This handler is called from an ISR when:
 * - transmit operation was started by a call to @ref nrf_802154_trx_transmit_frame.
 * - the RADIO peripheral sent whole frame on air
 *
 * When this handler is called following holds:
 * - the RADIO peripheral started ramping down (or it ramped down already)
 * - trx module is in @c FINISHED state
 *
 * Implementation is responsible for leaving @c FINISHED state by a call to:
 * - @ref nrf_802154_trx_receive_frame,
 * - @ref nrf_802154_trx_transmit_frame,
 * - @ref nrf_802154_trx_go_idle,
 * - @ref nrf_802154_trx_disable.
 */
extern void nrf_802154_trx_transmit_frame_transmitted(void);

/**@brief Handler called when ack transmission has just been started.
 *
 * This handler is called from an ISR when:
 * - @ref NRF_802154_TX_STARTED_NOTIFY_ENABLED == 1 (see nrf_802154_config.h)
 * - transmit operation was started by a call to @ref nrf_802154_trx_transmit_ack.
 * - the RADIO peripheral sent synchronization header
 *
 * When this handler is called following holds:
 * - trx module is in @c TXACK state
 *
 * Implementation may (but does not need to) terminate transmission if it wishes by a call to:
 * - @ref nrf_802154_trx_abort,
 * - @ref nrf_802154_trx_disable.
 */
extern void nrf_802154_trx_transmit_ack_started(void);

/**@brief Handler called when ack transmission has just been finished.
 *
 * This handler is called from an ISR when:
 * - transmit operation was started by a call to @ref nrf_802154_trx_transmit_ack.
 * - the RADIO peripheral sent whole ack frame on air
 *
 * When this handler is called following holds:
 * - the RADIO peripheral started ramping down (or it ramped down already)
 * - trx module is in @c FINISHED state
 *
 * Implementation is responsible for leaving @c FINISHED state by a call to:
 * - @ref nrf_802154_trx_receive_frame,
 * - @ref nrf_802154_trx_transmit_frame,
 * - @ref nrf_802154_trx_go_idle,
 * - @ref nrf_802154_trx_disable.
 */
extern void nrf_802154_trx_transmit_ack_transmitted(void);

/**@brief Handler called when trx module reached @c IDLE state.
 *
 * This handler is called from an ISR when:
 * - transition to idle state was successfully requested by a call to @ref nrf_802154_trx_go_idle.
 * - the RADIO peripheral reached DISABLED state
 * - the FEM module has been powered off
 *
 * When this handler is called following holds:
 * - the RADIO is in @c DISABLED state
 * - the FEM is powered off
 * - trx module is in @c IDLE state
 *
 * Implementation may leave trx in @c IDLE state or it may request:
 * - @ref nrf_802154_trx_receive_frame,
 * - @ref nrf_802154_trx_transmit_frame,
 * - @ref nrf_802154_trx_disable.
 */
extern void nrf_802154_trx_go_idle_finished(void);

/**@brief Handler called when standalone cca operaion has been just finished.
 *
 *  This handler is called from an ISR when:
 *  - standalone cca operation was requested by a call to @ref nrf_802154_trx_standalone_cca
 *  - the RADIO peripheral finished cca operation
 *
 * When this handler is called following holds:
 * - the RADIO peripheral started ramping down (or it ramped down already)
 * - trx module is in @c FINISHED state
 *
 * Implementation is responsible for leaving @c FINISHED state by a call to:
 * - @ref nrf_802154_trx_receive_frame,
 * - @ref nrf_802154_trx_transmit_frame,
 * - @ref nrf_802154_trx_go_idle,
 * - @ref nrf_802154_trx_disable.
 *
 * @param channel_was_idle  Informs implementation of the handler if channel was idle.
 *                          true means the channel was idle, false means the channel was busy.
 */
extern void nrf_802154_trx_standalone_cca_finished(bool channel_was_idle);

/**@brief Handler called when energy detection operation has been just finished.
 *
 *  This handler is called from an ISR when:
 *  - energy detection operation was requested by a call to @ref nrf_802154_trx_energy_detection
 *  - the RADIO peripheral finished the operation
 *
 * When this handler is called following holds:
 * - the RADIO peripheral started ramping down (or it ramped down already)
 * - trx module is in @c FINISHED state
 *
 * Implementation is responsible for leaving @c FINISHED state by a call to:
 * - @ref nrf_802154_trx_receive_frame,
 * - @ref nrf_802154_trx_transmit_frame,
 * - @ref nrf_802154_trx_go_idle,
 * - @ref nrf_802154_trx_disable.
 *
 * @param channel_was_idle  Informs implementation of the handler if channel was idle.
 *                          true means the channel was idle, false means the channel was busy.
 */
extern void nrf_802154_trx_energy_detection_finished(uint8_t ed_sample);

#ifdef __cplusplus
}
#endif

#endif /* NRF_802154_TRX_H_ */
