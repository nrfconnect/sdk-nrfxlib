/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef NRF_SP_QSPI_H__
#define NRF_SP_QSPI_H__

#include "nrfx.h" // Resolve nrfXX_types.h for the correct target to get definitions for __IO, __IOM etc.
/* ===========================================================================================================================
 * ================                                           QSPI                                           ================
 * ===========================================================================================================================*/

#if !defined(__ASSEMBLER__) \
    && !defined(__ASSEMBLY__) /*!< Ignore C structs for assembly code.                                 */

/* =========================================== Struct QSPI_EVENTS_DMA_EVENTS_DONE ============================================ */
/**
 * @brief EVENTS_DONE [QSPI_EVENTS_DMA_EVENTS_DONE] Peripheral events.
 */
typedef struct
{
    __IOM uint32_t LIST;                                         /*!<                Descriptor list is complete.                          */
    __IOM uint32_t LISTPART;                                     /*!<                Descriptor list is partially complete. Threshold in
                                                                  *                               CONFIG.LISTPARTTHRESH register has been passed.*/
    __IOM uint32_t SELECTJOB;                                    /*!<                Selected job is completed. Selection is done via
                                                                  *                               CONFIG.SELECTJOBENABLE.*/
    __IOM uint32_t DATA;                                         /*!<                A job has been completed, i.e. the job's data has been
                                                                  *                               completely transferred either to AXI or DMA bus,
                                                                  *                               depending on direction.*/
    __IOM uint32_t JOB;                                          /*!<                A job has been fetched from the joblist.              */
} NRF_QSPI_EVENTS_DMA_EVENTS_DONE_Type;                          /*!<                                                                      */

/* QSPI_EVENTS_DMA_EVENTS_DONE_LIST: Descriptor list is complete. */
#define QSPI_EVENTS_DMA_EVENTS_DONE_LIST_ResetValue \
    (0x00000000UL)                                               /*!< Reset value of LIST register.                        */

/* LIST @Bit 0 : Descriptor list is complete. */
#define QSPI_EVENTS_DMA_EVENTS_DONE_LIST_LIST_Pos \
    (0UL)                                                        /*!< Position of LIST field.                                         */
#define QSPI_EVENTS_DMA_EVENTS_DONE_LIST_LIST_Msk \
    (0x1UL << QSPI_EVENTS_DMA_EVENTS_DONE_LIST_LIST_Pos)         /*!< Bit mask of LIST
                                                                  *                 field.*/
#define QSPI_EVENTS_DMA_EVENTS_DONE_LIST_LIST_Min \
    (0x0UL)                                                      /*!< Min enumerator value of LIST field.                           */
#define QSPI_EVENTS_DMA_EVENTS_DONE_LIST_LIST_Max \
    (0x1UL)                                                      /*!< Max enumerator value of LIST field.                           */
#define QSPI_EVENTS_DMA_EVENTS_DONE_LIST_LIST_NotGenerated \
    (0x0UL)                                                      /*!< Event not generated                                  */
#define QSPI_EVENTS_DMA_EVENTS_DONE_LIST_LIST_Generated \
    (0x1UL)                                                      /*!< Event generated                                         */

/* QSPI_EVENTS_DMA_EVENTS_DONE_LISTPART: Descriptor list is partially complete. Threshold in CONFIG.LISTPARTTHRESH register has
 *                                        been passed. */

#define QSPI_EVENTS_DMA_EVENTS_DONE_LISTPART_ResetValue \
    (0x00000000UL)                                               /*!< Reset value of LISTPART register.                */

/* LISTPART @Bit 0 : Descriptor list is partially complete. Threshold in CONFIG.LISTPARTTHRESH register has been passed. */
#define QSPI_EVENTS_DMA_EVENTS_DONE_LISTPART_LISTPART_Pos \
    (0UL)                                                        /*!< Position of LISTPART field.                             */
#define QSPI_EVENTS_DMA_EVENTS_DONE_LISTPART_LISTPART_Msk \
    (0x1UL << QSPI_EVENTS_DMA_EVENTS_DONE_LISTPART_LISTPART_Pos) /*!<
                                                                  *         Bit mask of LISTPART field.*/
#define QSPI_EVENTS_DMA_EVENTS_DONE_LISTPART_LISTPART_Min \
    (0x0UL)                                                      /*!< Min enumerator value of LISTPART field.               */
#define QSPI_EVENTS_DMA_EVENTS_DONE_LISTPART_LISTPART_Max \
    (0x1UL)                                                      /*!< Max enumerator value of LISTPART field.               */
#define QSPI_EVENTS_DMA_EVENTS_DONE_LISTPART_LISTPART_NotGenerated \
    (0x0UL)                                                      /*!< Event not generated                          */
#define QSPI_EVENTS_DMA_EVENTS_DONE_LISTPART_LISTPART_Generated \
    (0x1UL)                                                      /*!< Event generated                                 */

/* QSPI_EVENTS_DMA_EVENTS_DONE_SELECTJOB: Selected job is completed. Selection is done via CONFIG.SELECTJOBENABLE. */
#define QSPI_EVENTS_DMA_EVENTS_DONE_SELECTJOB_ResetValue \
    (0x00000000UL)                                               /*!< Reset value of SELECTJOB register.              */

/* SELECTJOB @Bit 0 : Selected job is completed. Selection is done via CONFIG.SELECTJOBENABLE. */
#define QSPI_EVENTS_DMA_EVENTS_DONE_SELECTJOB_SELECTJOB_Pos \
    (0UL)                                                        /*!< Position of SELECTJOB field.                          */
#define QSPI_EVENTS_DMA_EVENTS_DONE_SELECTJOB_SELECTJOB_Msk \
    (0x1UL << QSPI_EVENTS_DMA_EVENTS_DONE_SELECTJOB_SELECTJOB_Pos)
/*!< Bit mask of SELECTJOB field.*/
#define QSPI_EVENTS_DMA_EVENTS_DONE_SELECTJOB_SELECTJOB_Min \
    (0x0UL)                                                      /*!< Min enumerator value of SELECTJOB field.            */
#define QSPI_EVENTS_DMA_EVENTS_DONE_SELECTJOB_SELECTJOB_Max \
    (0x1UL)                                                      /*!< Max enumerator value of SELECTJOB field.            */
#define QSPI_EVENTS_DMA_EVENTS_DONE_SELECTJOB_SELECTJOB_NotGenerated \
    (0x0UL)                                                      /*!< Event not generated                        */
#define QSPI_EVENTS_DMA_EVENTS_DONE_SELECTJOB_SELECTJOB_Generated \
    (0x1UL)                                                      /*!< Event generated                               */

/* QSPI_EVENTS_DMA_EVENTS_DONE_DATA: A job has been completed, i.e. the job's data has been completely transferred either to AXI
 *                                    or DMA bus, depending on direction. */

#define QSPI_EVENTS_DMA_EVENTS_DONE_DATA_ResetValue \
    (0x00000000UL)                                               /*!< Reset value of DATA register.                        */

/* DATA @Bit 0 : A job has been completed, i.e. the job's data has been completely transferred either to AXI or DMA bus,
 *               depending on direction. */

#define QSPI_EVENTS_DMA_EVENTS_DONE_DATA_DATA_Pos \
    (0UL)                                                        /*!< Position of DATA field.                                         */
#define QSPI_EVENTS_DMA_EVENTS_DONE_DATA_DATA_Msk \
    (0x1UL << QSPI_EVENTS_DMA_EVENTS_DONE_DATA_DATA_Pos)         /*!< Bit mask of DATA
                                                                  *                 field.*/
#define QSPI_EVENTS_DMA_EVENTS_DONE_DATA_DATA_Min \
    (0x0UL)                                                      /*!< Min enumerator value of DATA field.                           */
#define QSPI_EVENTS_DMA_EVENTS_DONE_DATA_DATA_Max \
    (0x1UL)                                                      /*!< Max enumerator value of DATA field.                           */
#define QSPI_EVENTS_DMA_EVENTS_DONE_DATA_DATA_NotGenerated \
    (0x0UL)                                                      /*!< Event not generated                                  */
#define QSPI_EVENTS_DMA_EVENTS_DONE_DATA_DATA_Generated \
    (0x1UL)                                                      /*!< Event generated                                         */

/* QSPI_EVENTS_DMA_EVENTS_DONE_JOB: A job has been fetched from the joblist. */
#define QSPI_EVENTS_DMA_EVENTS_DONE_JOB_ResetValue \
    (0x00000000UL)                                               /*!< Reset value of JOB register.                          */

/* JOB @Bit 0 : A job has been fetched from the joblist. */
#define QSPI_EVENTS_DMA_EVENTS_DONE_JOB_JOB_Pos \
    (0UL)                                                        /*!< Position of JOB field.                                            */
#define QSPI_EVENTS_DMA_EVENTS_DONE_JOB_JOB_Msk \
    (0x1UL << QSPI_EVENTS_DMA_EVENTS_DONE_JOB_JOB_Pos)           /*!< Bit mask of JOB
                                                                  *                   field.*/
#define QSPI_EVENTS_DMA_EVENTS_DONE_JOB_JOB_Min \
    (0x0UL)                                                      /*!< Min enumerator value of JOB field.                              */
#define QSPI_EVENTS_DMA_EVENTS_DONE_JOB_JOB_Max \
    (0x1UL)                                                      /*!< Max enumerator value of JOB field.                              */
#define QSPI_EVENTS_DMA_EVENTS_DONE_JOB_JOB_NotGenerated \
    (0x0UL)                                                      /*!< Event not generated                                    */
#define QSPI_EVENTS_DMA_EVENTS_DONE_JOB_JOB_Generated \
    (0x1UL)                                                      /*!< Event generated                                           */

/* ================================================= Struct QSPI_EVENTS_DMA ================================================== */
/**
 * @brief EVENTS_DMA [QSPI_EVENTS_DMA] Peripheral events.
 */
typedef struct
{
    __IOM NRF_QSPI_EVENTS_DMA_EVENTS_DONE_Type
                   EVENTS_DONE;                    /*!<                Peripheral events.                               */
    __IOM uint32_t ERROR;                          /*!<                AXI bus error received.                               */
    __IOM uint32_t PAUSED;                         /*!<                DMA paused with task TASKS_PAUSE.                     */
    __IOM uint32_t RESET;                          /*!<                DMA reset with task TASKS_RESET.                      */
    __IOM uint32_t DONE;                           /*!<                DMA transfer done                                     */
    __IOM uint32_t TXUNEXPECTEDIDLE;               /*!<                The QSPI core has gone into idle state at an unexpected
                                                    *                                  time, this can happen if the TX buffer underflows, this
                                                    *                                  event is followed by either DONE or ABORTED event.
                                                    *                                  Triggered by the CORE being idle while we still have
                                                    *                                  data to transfer to the CORE.*/
    __IOM uint32_t INTERNALBUSERROR;               /*!<                An error occured during the transfer of data internally
                                                    *                                  in the QSPI, followed by either DONE or ABORTED event.
                                                    *                                  This event is generated on an AHB error response on the
                                                    *                                  easyDMA towards the QSPI CORE. The BUSERROR address is
                                                    *                                  not given in a register since the target register
                                                    *                                  address is fixed.*/
    __IOM uint32_t ABORTED;                        /*!<                The QSPI DMA has aborted due to an error given in a
                                                    *                                  separate event, and has attempted to complete any
                                                    *                                  ongoing transactions, and stopped. The QSPI DMA is
                                                    *                                  ready for a new transaction.*/
} NRF_QSPI_EVENTS_DMA_Type;                        /*!<                                                                     */

/* QSPI_EVENTS_DMA_ERROR: AXI bus error received. */
#define QSPI_EVENTS_DMA_ERROR_ResetValue \
    (0x00000000UL)                                 /*!< Reset value of ERROR register.                                  */

/* ERROR @Bit 0 : AXI bus error received. */
#define QSPI_EVENTS_DMA_ERROR_ERROR_Pos \
    (0UL)                                          /*!< Position of ERROR field.                                             */
#define QSPI_EVENTS_DMA_ERROR_ERROR_Msk \
    (0x1UL << QSPI_EVENTS_DMA_ERROR_ERROR_Pos)     /*!< Bit mask of ERROR field.             */
#define QSPI_EVENTS_DMA_ERROR_ERROR_Min \
    (0x0UL)                                        /*!< Min enumerator value of ERROR field.                                 */
#define QSPI_EVENTS_DMA_ERROR_ERROR_Max \
    (0x1UL)                                        /*!< Max enumerator value of ERROR field.                                 */
#define QSPI_EVENTS_DMA_ERROR_ERROR_NotGenerated \
    (0x0UL)                                        /*!< Event not generated                                            */
#define QSPI_EVENTS_DMA_ERROR_ERROR_Generated \
    (0x1UL)                                        /*!< Event generated                                                   */

/* QSPI_EVENTS_DMA_PAUSED: DMA paused with task TASKS_PAUSE. */
#define QSPI_EVENTS_DMA_PAUSED_ResetValue \
    (0x00000000UL)                                 /*!< Reset value of PAUSED register.                                */

/* PAUSED @Bit 0 : DMA paused with task TASKS_PAUSE. */
#define QSPI_EVENTS_DMA_PAUSED_PAUSED_Pos \
    (0UL)                                          /*!< Position of PAUSED field.                                            */
#define QSPI_EVENTS_DMA_PAUSED_PAUSED_Msk \
    (0x1UL << QSPI_EVENTS_DMA_PAUSED_PAUSED_Pos)   /*!< Bit mask of PAUSED field.        */
#define QSPI_EVENTS_DMA_PAUSED_PAUSED_Min \
    (0x0UL)                                        /*!< Min enumerator value of PAUSED field.                                */
#define QSPI_EVENTS_DMA_PAUSED_PAUSED_Max \
    (0x1UL)                                        /*!< Max enumerator value of PAUSED field.                                */
#define QSPI_EVENTS_DMA_PAUSED_PAUSED_NotGenerated \
    (0x0UL)                                        /*!< Event not generated                                          */
#define QSPI_EVENTS_DMA_PAUSED_PAUSED_Generated \
    (0x1UL)                                        /*!< Event generated                                                 */

/* QSPI_EVENTS_DMA_RESET: DMA reset with task TASKS_RESET. */
#define QSPI_EVENTS_DMA_RESET_ResetValue \
    (0x00000000UL)                                 /*!< Reset value of RESET register.                                  */

/* RESET @Bit 0 : DMA reset with task TASKS_RESET. */
#define QSPI_EVENTS_DMA_RESET_RESET_Pos \
    (0UL)                                          /*!< Position of RESET field.                                             */
#define QSPI_EVENTS_DMA_RESET_RESET_Msk \
    (0x1UL << QSPI_EVENTS_DMA_RESET_RESET_Pos)     /*!< Bit mask of RESET field.             */
#define QSPI_EVENTS_DMA_RESET_RESET_Min \
    (0x0UL)                                        /*!< Min enumerator value of RESET field.                                 */
#define QSPI_EVENTS_DMA_RESET_RESET_Max \
    (0x1UL)                                        /*!< Max enumerator value of RESET field.                                 */
#define QSPI_EVENTS_DMA_RESET_RESET_NotGenerated \
    (0x0UL)                                        /*!< Event not generated                                            */
#define QSPI_EVENTS_DMA_RESET_RESET_Generated \
    (0x1UL)                                        /*!< Event generated                                                   */

/* QSPI_EVENTS_DMA_DONE: DMA transfer done */
#define QSPI_EVENTS_DMA_DONE_ResetValue \
    (0x00000000UL)                                 /*!< Reset value of DONE register.                                    */

/* DONE @Bit 0 : DMA transfer done */
#define QSPI_EVENTS_DMA_DONE_DONE_Pos (0UL)        /*!< Position of DONE field.                                              */
#define QSPI_EVENTS_DMA_DONE_DONE_Msk \
    (0x1UL << QSPI_EVENTS_DMA_DONE_DONE_Pos)       /*!< Bit mask of DONE field.                  */
#define QSPI_EVENTS_DMA_DONE_DONE_Min \
    (0x0UL)                                        /*!< Min enumerator value of DONE field.                                  */
#define QSPI_EVENTS_DMA_DONE_DONE_Max \
    (0x1UL)                                        /*!< Max enumerator value of DONE field.                                  */
#define QSPI_EVENTS_DMA_DONE_DONE_NotGenerated \
    (0x0UL)                                        /*!< Event not generated                                              */
#define QSPI_EVENTS_DMA_DONE_DONE_Generated \
    (0x1UL)                                        /*!< Event generated                                                     */

/* QSPI_EVENTS_DMA_TXUNEXPECTEDIDLE: The QSPI core has gone into idle state at an unexpected time, this can happen if the TX
 *                                    buffer underflows, this event is followed by either DONE or ABORTED event. Triggered by
 *                                    the CORE being idle while we still have data to transfer to the CORE. */

#define QSPI_EVENTS_DMA_TXUNEXPECTEDIDLE_ResetValue \
    (0x00000000UL)                                 /*!< Reset value of TXUNEXPECTEDIDLE register.            */

/* TXUNEXPECTEDIDLE @Bit 0 : The QSPI core has gone into idle state at an unexpected time, this can happen if the TX buffer
 *                           underflows, this event is followed by either DONE or ABORTED event. Triggered by the CORE being
 *                           idle while we still have data to transfer to the CORE. */

#define QSPI_EVENTS_DMA_TXUNEXPECTEDIDLE_TXUNEXPECTEDIDLE_Pos \
    (0UL)                                          /*!< Position of TXUNEXPECTEDIDLE field.                 */
#define QSPI_EVENTS_DMA_TXUNEXPECTEDIDLE_TXUNEXPECTEDIDLE_Msk \
    (0x1UL << QSPI_EVENTS_DMA_TXUNEXPECTEDIDLE_TXUNEXPECTEDIDLE_Pos)
/*!< Bit mask of TXUNEXPECTEDIDLE field.*/
#define QSPI_EVENTS_DMA_TXUNEXPECTEDIDLE_TXUNEXPECTEDIDLE_Min \
    (0x0UL)                                        /*!< Min enumerator value of TXUNEXPECTEDIDLE field.   */
#define QSPI_EVENTS_DMA_TXUNEXPECTEDIDLE_TXUNEXPECTEDIDLE_Max \
    (0x1UL)                                        /*!< Max enumerator value of TXUNEXPECTEDIDLE field.   */
#define QSPI_EVENTS_DMA_TXUNEXPECTEDIDLE_TXUNEXPECTEDIDLE_NotGenerated \
    (0x0UL)                                        /*!< Event not generated                      */
#define QSPI_EVENTS_DMA_TXUNEXPECTEDIDLE_TXUNEXPECTEDIDLE_Generated \
    (0x1UL)                                        /*!< Event generated                             */

/* QSPI_EVENTS_DMA_INTERNALBUSERROR: An error occured during the transfer of data internally in the QSPI, followed by either
 *                                    DONE or ABORTED event. This event is generated on an AHB error response on the easyDMA
 *                                    towards the QSPI CORE. The BUSERROR address is not given in a register since the target
 *                                    register address is fixed. */

#define QSPI_EVENTS_DMA_INTERNALBUSERROR_ResetValue \
    (0x00000000UL)                                 /*!< Reset value of INTERNALBUSERROR register.            */

/* INTERNALBUSERROR @Bit 0 : An error occured during the transfer of data internally in the QSPI, followed by either DONE or
 *                           ABORTED event. This event is generated on an AHB error response on the easyDMA towards the QSPI
 *                           CORE. The BUSERROR address is not given in a register since the target register address is fixed. */

#define QSPI_EVENTS_DMA_INTERNALBUSERROR_INTERNALBUSERROR_Pos \
    (0UL)                                          /*!< Position of INTERNALBUSERROR field.                 */
#define QSPI_EVENTS_DMA_INTERNALBUSERROR_INTERNALBUSERROR_Msk \
    (0x1UL << QSPI_EVENTS_DMA_INTERNALBUSERROR_INTERNALBUSERROR_Pos)
/*!< Bit mask of INTERNALBUSERROR field.*/
#define QSPI_EVENTS_DMA_INTERNALBUSERROR_INTERNALBUSERROR_Min \
    (0x0UL)                                        /*!< Min enumerator value of INTERNALBUSERROR field.   */
#define QSPI_EVENTS_DMA_INTERNALBUSERROR_INTERNALBUSERROR_Max \
    (0x1UL)                                        /*!< Max enumerator value of INTERNALBUSERROR field.   */
#define QSPI_EVENTS_DMA_INTERNALBUSERROR_INTERNALBUSERROR_NotGenerated \
    (0x0UL)                                        /*!< Event not generated                      */
#define QSPI_EVENTS_DMA_INTERNALBUSERROR_INTERNALBUSERROR_Generated \
    (0x1UL)                                        /*!< Event generated                             */

/* QSPI_EVENTS_DMA_ABORTED: The QSPI DMA has aborted due to an error given in a separate event, and has attempted to complete
 *                           any ongoing transactions, and stopped. The QSPI DMA is ready for a new transaction. */

#define QSPI_EVENTS_DMA_ABORTED_ResetValue \
    (0x00000000UL)                                 /*!< Reset value of ABORTED register.                              */

/* ABORTED @Bit 0 : The QSPI DMA has aborted due to an error given in a separate event, and has attempted to complete any
 *                  ongoing transactions, and stopped. The QSPI DMA is ready for a new transaction. */

#define QSPI_EVENTS_DMA_ABORTED_ABORTED_Pos \
    (0UL)                                          /*!< Position of ABORTED field.                                           */
#define QSPI_EVENTS_DMA_ABORTED_ABORTED_Msk \
    (0x1UL << QSPI_EVENTS_DMA_ABORTED_ABORTED_Pos) /*!< Bit mask of ABORTED field.   */
#define QSPI_EVENTS_DMA_ABORTED_ABORTED_Min \
    (0x0UL)                                        /*!< Min enumerator value of ABORTED field.                              */
#define QSPI_EVENTS_DMA_ABORTED_ABORTED_Max \
    (0x1UL)                                        /*!< Max enumerator value of ABORTED field.                              */
#define QSPI_EVENTS_DMA_ABORTED_ABORTED_NotGenerated \
    (0x0UL)                                        /*!< Event not generated                                        */
#define QSPI_EVENTS_DMA_ABORTED_ABORTED_Generated \
    (0x1UL)                                        /*!< Event generated                                               */

/* =================================================== Struct QSPI_CONFIG ==================================================== */
/**
 * @brief CONFIG [QSPI_CONFIG] (unspecified)
 */
typedef struct
{
    __IOM uint32_t TXBURSTLENGTH;                          /*!<                Transmit burst length                                 */
    __IOM uint32_t RXBURSTLENGTH;                          /*!<                Receive burst length                                  */
    __IOM uint32_t RXTRANSFERLENGTH;                       /*!<                Receive transfer full length                          */
    __IOM uint32_t STOPON;                                 /*!<                Stop the DMA if the CORE goes into any of the enabled
                                                            *                                  states.*/
    __IOM uint32_t AXIMODE;                                /*!<                Determines if AXI-lite or full AXI is used.           */
} NRF_QSPI_CONFIG_Type;                                    /*!<                                                                      */

/* QSPI_CONFIG_TXBURSTLENGTH: Transmit burst length */
#define QSPI_CONFIG_TXBURSTLENGTH_ResetValue \
    (0x00000008UL)                                         /*!< Reset value of TXBURSTLENGTH register.                      */

/* AMOUNT @Bits 0..4 : Number of buffer lines */
#define QSPI_CONFIG_TXBURSTLENGTH_AMOUNT_Pos \
    (0UL)                                                  /*!< Position of AMOUNT field.                                            */
#define QSPI_CONFIG_TXBURSTLENGTH_AMOUNT_Msk \
    (0x1FUL << QSPI_CONFIG_TXBURSTLENGTH_AMOUNT_Pos)       /*!< Bit mask of AMOUNT field. */

/* QSPI_CONFIG_RXBURSTLENGTH: Receive burst length */
#define QSPI_CONFIG_RXBURSTLENGTH_ResetValue \
    (0x00000008UL)                                         /*!< Reset value of RXBURSTLENGTH register.                      */

/* AMOUNT @Bits 0..4 : Number of buffer lines */
#define QSPI_CONFIG_RXBURSTLENGTH_AMOUNT_Pos \
    (0UL)                                                  /*!< Position of AMOUNT field.                                            */
#define QSPI_CONFIG_RXBURSTLENGTH_AMOUNT_Msk \
    (0x1FUL << QSPI_CONFIG_RXBURSTLENGTH_AMOUNT_Pos)       /*!< Bit mask of AMOUNT field. */

/* QSPI_CONFIG_RXTRANSFERLENGTH: Receive transfer full length */
#define QSPI_CONFIG_RXTRANSFERLENGTH_ResetValue \
    (0x00000010UL)                                         /*!< Reset value of RXTRANSFERLENGTH register.                */

/* AMOUNT @Bits 0..17 : Number of buffer lines */
#define QSPI_CONFIG_RXTRANSFERLENGTH_AMOUNT_Pos \
    (0UL)                                                  /*!< Position of AMOUNT field.                                         */
#define QSPI_CONFIG_RXTRANSFERLENGTH_AMOUNT_Msk \
    (0x3FFFFUL << QSPI_CONFIG_RXTRANSFERLENGTH_AMOUNT_Pos) /*!< Bit mask of AMOUNT
                                                            *               field.*/

/* QSPI_CONFIG_STOPON: Stop the DMA if the CORE goes into any of the enabled states. */
#define QSPI_CONFIG_STOPON_ResetValue \
    (0x00000003UL)                                         /*!< Reset value of STOPON register.                                    */

/* TXUNEXPECTEDIDLE @Bit 0 : The CORE will go into idle if the CORE buffer is emptied, if the DMA still has data to transfer the
 *                           CORE is not expected to go into idle state. This constitutes a buffer underflow. The CORE migth
 *                           also go into idle if an error occurs. */

#define QSPI_CONFIG_STOPON_TXUNEXPECTEDIDLE_Pos \
    (0UL)                                                  /*!< Position of TXUNEXPECTEDIDLE field.                               */
#define QSPI_CONFIG_STOPON_TXUNEXPECTEDIDLE_Msk \
    (0x1UL << QSPI_CONFIG_STOPON_TXUNEXPECTEDIDLE_Pos)     /*!< Bit mask of
                                                            *                   TXUNEXPECTEDIDLE field.*/
#define QSPI_CONFIG_STOPON_TXUNEXPECTEDIDLE_Min \
    (0x0UL)                                                /*!< Min enumerator value of TXUNEXPECTEDIDLE field.                 */
#define QSPI_CONFIG_STOPON_TXUNEXPECTEDIDLE_Max \
    (0x1UL)                                                /*!< Max enumerator value of TXUNEXPECTEDIDLE field.                 */
#define QSPI_CONFIG_STOPON_TXUNEXPECTEDIDLE_Disabled \
    (0x0UL)                                                /*!< Stop on tx unexpected idle disabled.                       */
#define QSPI_CONFIG_STOPON_TXUNEXPECTEDIDLE_Enabled \
    (0x1UL)                                                /*!< Stop on tx unexpected idle enabled.                         */

/* RXOVERFLOW @Bit 1 : This condition occurrs if the DMA attempts to write to the RX buffer in the CORE, but the buffer is full.
 *                     When this bit is enabled the DMA will stop when the CORE buffer experiences a rx buffer overflow */

#define QSPI_CONFIG_STOPON_RXOVERFLOW_Pos \
    (1UL)                                                  /*!< Position of RXOVERFLOW field.                                        */
#define QSPI_CONFIG_STOPON_RXOVERFLOW_Msk \
    (0x1UL << QSPI_CONFIG_STOPON_RXOVERFLOW_Pos)           /*!< Bit mask of RXOVERFLOW field.    */
#define QSPI_CONFIG_STOPON_RXOVERFLOW_Min \
    (0x0UL)                                                /*!< Min enumerator value of RXOVERFLOW field.                            */
#define QSPI_CONFIG_STOPON_RXOVERFLOW_Max \
    (0x1UL)                                                /*!< Max enumerator value of RXOVERFLOW field.                            */
#define QSPI_CONFIG_STOPON_RXOVERFLOW_Disabled \
    (0x0UL)                                                /*!< Stop on rx overflow disabled.                                    */
#define QSPI_CONFIG_STOPON_RXOVERFLOW_Enabled \
    (0x1UL)                                                /*!< Stop on rx overflow enabled.                                      */

/* INTERNALBUSERROR @Bit 2 : Terminate the transaction if a INTERNALBUSERROR event is detected. */
#define QSPI_CONFIG_STOPON_INTERNALBUSERROR_Pos \
    (2UL)                                                  /*!< Position of INTERNALBUSERROR field.                               */
#define QSPI_CONFIG_STOPON_INTERNALBUSERROR_Msk \
    (0x1UL << QSPI_CONFIG_STOPON_INTERNALBUSERROR_Pos)     /*!< Bit mask of
                                                            *                   INTERNALBUSERROR field.*/
#define QSPI_CONFIG_STOPON_INTERNALBUSERROR_Min \
    (0x0UL)                                                /*!< Min enumerator value of INTERNALBUSERROR field.                 */
#define QSPI_CONFIG_STOPON_INTERNALBUSERROR_Max \
    (0x1UL)                                                /*!< Max enumerator value of INTERNALBUSERROR field.                 */
#define QSPI_CONFIG_STOPON_INTERNALBUSERROR_Disabled \
    (0x0UL)                                                /*!< Disable                                                    */
#define QSPI_CONFIG_STOPON_INTERNALBUSERROR_Enabled \
    (0x1UL)                                                /*!< Enable                                                      */

/* DMABUSERROR @Bit 3 : Terminate the transaction if a DMA.ERROR event is detected. */
#define QSPI_CONFIG_STOPON_DMABUSERROR_Pos \
    (3UL)                                                  /*!< Position of DMABUSERROR field.                                       */
#define QSPI_CONFIG_STOPON_DMABUSERROR_Msk \
    (0x1UL << QSPI_CONFIG_STOPON_DMABUSERROR_Pos)          /*!< Bit mask of DMABUSERROR field. */
#define QSPI_CONFIG_STOPON_DMABUSERROR_Min \
    (0x0UL)                                                /*!< Min enumerator value of DMABUSERROR field.                           */
#define QSPI_CONFIG_STOPON_DMABUSERROR_Max \
    (0x1UL)                                                /*!< Max enumerator value of DMABUSERROR field.                           */
#define QSPI_CONFIG_STOPON_DMABUSERROR_Disabled \
    (0x0UL)                                                /*!< Disable                                                         */
#define QSPI_CONFIG_STOPON_DMABUSERROR_Enabled \
    (0x1UL)                                                /*!< Enable                                                           */

/* QSPI_CONFIG_AXIMODE: Determines if AXI-lite or full AXI is used. */
#define QSPI_CONFIG_AXIMODE_ResetValue \
    (0x00000000UL)                                         /*!< Reset value of AXIMODE register.                                  */

/* AXIMODE @Bit 4 : Type of AXI access. */
#define QSPI_CONFIG_AXIMODE_AXIMODE_Pos \
    (4UL)                                                  /*!< Position of AXIMODE field.                                           */
#define QSPI_CONFIG_AXIMODE_AXIMODE_Msk \
    (0x1UL << QSPI_CONFIG_AXIMODE_AXIMODE_Pos)             /*!< Bit mask of AXIMODE field.           */
#define QSPI_CONFIG_AXIMODE_AXIMODE_Min \
    (0x0UL)                                                /*!< Min enumerator value of AXIMODE field.                               */
#define QSPI_CONFIG_AXIMODE_AXIMODE_Max \
    (0x1UL)                                                /*!< Max enumerator value of AXIMODE field.                               */
#define QSPI_CONFIG_AXIMODE_AXIMODE_Lite \
    (0x0UL)                                                /*!< AXI Lite                                                             */
#define QSPI_CONFIG_AXIMODE_AXIMODE_Full \
    (0x1UL)                                                /*!< Full AXI                                                             */

/* MODE @Bit 5 : (unspecified) */
#define QSPI_CONFIG_AXIMODE_MODE_Pos (5UL)                 /*!< Position of MODE field.                                              */
#define QSPI_CONFIG_AXIMODE_MODE_Msk \
    (0x1UL << QSPI_CONFIG_AXIMODE_MODE_Pos)                /*!< Bit mask of MODE field.                    */

/* =================================================== Struct QSPI_FORMAT ==================================================== */
/**
 * @brief FORMAT [QSPI_FORMAT] (unspecified)
 */
typedef struct
{
    __IOM uint32_t DFS;                          /*!<                Data frame size                                       */
    __IOM uint32_t BPP;                          /*!<                Bits per pixel                                        */
    __IOM uint32_t PIXELS;                       /*!<                Number of pixels following the command                */
    __IOM uint32_t CILEN;                        /*!<                Command/Instruction length                            */
    __IOM uint32_t BITORDER;                     /*!<                Bit Order                                             */
} NRF_QSPI_FORMAT_Type;                          /*!<                                                                      */

/* QSPI_FORMAT_DFS: Data frame size */
#define QSPI_FORMAT_DFS_ResetValue \
    (0x00000000UL)                               /*!< Reset value of DFS register.                                         */

/* DFS @Bits 0..5 : (unspecified) */
#define QSPI_FORMAT_DFS_DFS_Pos (0UL)            /*!< Position of DFS field.                                               */
#define QSPI_FORMAT_DFS_DFS_Msk \
    (0x3FUL << QSPI_FORMAT_DFS_DFS_Pos)          /*!< Bit mask of DFS field.                              */

/* QSPI_FORMAT_BPP: Bits per pixel */
#define QSPI_FORMAT_BPP_ResetValue \
    (0x00000000UL)                               /*!< Reset value of BPP register.                                         */

/* BPP @Bits 0..5 : (unspecified) */
#define QSPI_FORMAT_BPP_BPP_Pos (0UL)            /*!< Position of BPP field.                                               */
#define QSPI_FORMAT_BPP_BPP_Msk \
    (0x3FUL << QSPI_FORMAT_BPP_BPP_Pos)          /*!< Bit mask of BPP field.                              */
#define QSPI_FORMAT_BPP_BPP_Min (0x0UL)          /*!< Min enumerator value of BPP field.                                   */
#define QSPI_FORMAT_BPP_BPP_Max (0x10UL)         /*!< Max enumerator value of BPP field.                                   */
#define QSPI_FORMAT_BPP_BPP_0   (0x00UL)         /*!< Reordering of pixels disabled                                        */
#define QSPI_FORMAT_BPP_BPP_4   (0x04UL)         /*!< 4 Bits per pixel                                                     */
#define QSPI_FORMAT_BPP_BPP_8   (0x08UL)         /*!< 8 Bits per pixel                                                     */
#define QSPI_FORMAT_BPP_BPP_16  (0x10UL)         /*!< 16 Bits per pixel                                                    */

/* QSPI_FORMAT_PIXELS: Number of pixels following the command */
#define QSPI_FORMAT_PIXELS_ResetValue \
    (0x00000000UL)                               /*!< Reset value of PIXELS register.                                    */

/* PIXELS @Bits 0..17 : Number of pixels */
#define QSPI_FORMAT_PIXELS_PIXELS_Pos (0UL)      /*!< Position of PIXELS field.                                            */
#define QSPI_FORMAT_PIXELS_PIXELS_Msk \
    (0x3FFFFUL << QSPI_FORMAT_PIXELS_PIXELS_Pos) /*!< Bit mask of PIXELS field.            */

/* QSPI_FORMAT_CILEN: Command/Instruction length */
#define QSPI_FORMAT_CILEN_ResetValue \
    (0x00000000UL)                               /*!< Reset value of CILEN register.                                      */

/* CILEN @Bits 0..1 : Number of words */
#define QSPI_FORMAT_CILEN_CILEN_Pos (0UL)        /*!< Position of CILEN field.                                             */
#define QSPI_FORMAT_CILEN_CILEN_Msk \
    (0x3UL << QSPI_FORMAT_CILEN_CILEN_Pos)       /*!< Bit mask of CILEN field.                     */

/* QSPI_FORMAT_BITORDER: Bit Order */
#define QSPI_FORMAT_BITORDER_ResetValue \
    (0x00000000UL)                               /*!< Reset value of BITORDER register.                                      */
#define QSPI_FORMAT_BITORDER_COMMAND_Pos (0UL)   /*!< Position of COMMAND field.                                             */
#define QSPI_FORMAT_BITORDER_COMMAND_Msk \
    (0x1UL << QSPI_FORMAT_BITORDER_COMMAND_Pos)  /*!< Bit mask of COMMAND field.                     */
#define QSPI_FORMAT_BITORDER_DATA_Pos    (1UL)   /*!< Position of DATA field.                                             */
#define QSPI_FORMAT_BITORDER_DATA_Msk \
    (0x1UL << QSPI_FORMAT_BITORDER_DATA_Pos)     /*!< Bit mask of DATA field.                     */

/* ================================================= Struct QSPI_DMA_STATUS ================================================== */
/**
 * @brief STATUS [QSPI_DMA_STATUS] EasyVDMA status registers.
 */
typedef struct
{
    __IM uint32_t BYTECOUNT;                                  /*!<                Number of bytes sent or received during processing of
                                                               *                                          current descriptor list*/
    __IM uint32_t ATTRIBUTE;                                  /*!<                Latest job attribute being processed.                 */
    __IM uint32_t ADDRESS;                                    /*!<                Latest address being accessed.                        */
    __IM uint32_t JOBCOUNT;                                   /*!<                Number of completed jobs in current descriptor list.  */
    __IM uint32_t BUSERROR;                                   /*!<                AXI bus error status.                                 */
    __IM uint32_t FIFO;                                       /*!<                Data fifo status.                                     */
    __IM uint32_t ACTIVE;                                     /*!<                DMA activity state.                                   */
} NRF_QSPI_DMA_STATUS_Type;                                   /*!<                                                                      */

/* QSPI_DMA_STATUS_BYTECOUNT: Number of bytes sent or received during processing of current descriptor list */
#define QSPI_DMA_STATUS_BYTECOUNT_ResetValue \
    (0x00000000UL)                                            /*!< Reset value of BYTECOUNT register.                          */

/* BYTECOUNT @Bits 0..31 : Number of bytes sent or received during processing of current descriptor list */
#define QSPI_DMA_STATUS_BYTECOUNT_BYTECOUNT_Pos \
    (0UL)                                                     /*!< Position of BYTECOUNT field.                                      */
#define QSPI_DMA_STATUS_BYTECOUNT_BYTECOUNT_Msk \
    (0xFFFFFFFFUL << QSPI_DMA_STATUS_BYTECOUNT_BYTECOUNT_Pos) /*!< Bit mask of
                                                               *            BYTECOUNT field.*/

/* QSPI_DMA_STATUS_ATTRIBUTE: Latest job attribute being processed. */
#define QSPI_DMA_STATUS_ATTRIBUTE_ResetValue \
    (0x00000000UL)                                            /*!< Reset value of ATTRIBUTE register.                          */

/* ATTRIBUTE @Bits 0..5 : Latest job attribute being processed. */
#define QSPI_DMA_STATUS_ATTRIBUTE_ATTRIBUTE_Pos \
    (0UL)                                                     /*!< Position of ATTRIBUTE field.                                      */
#define QSPI_DMA_STATUS_ATTRIBUTE_ATTRIBUTE_Msk \
    (0x3FUL << QSPI_DMA_STATUS_ATTRIBUTE_ATTRIBUTE_Pos)       /*!< Bit mask of ATTRIBUTE
                                                               *                  field.*/

/* QSPI_DMA_STATUS_ADDRESS: Latest address being accessed. */
#define QSPI_DMA_STATUS_ADDRESS_ResetValue \
    (0x00000000UL)                                            /*!< Reset value of ADDRESS register.                              */

/* ADDRESS @Bits 0..31 : Latest address being accessed. */
#define QSPI_DMA_STATUS_ADDRESS_ADDRESS_Pos \
    (0UL)                                                     /*!< Position of ADDRESS field.                                           */
#define QSPI_DMA_STATUS_ADDRESS_ADDRESS_Msk \
    (0xFFFFFFFFUL << QSPI_DMA_STATUS_ADDRESS_ADDRESS_Pos)     /*!< Bit mask of ADDRESS
                                                               *                field.*/

/* QSPI_DMA_STATUS_JOBCOUNT: Number of completed jobs in current descriptor list. */
#define QSPI_DMA_STATUS_JOBCOUNT_ResetValue \
    (0x00000000UL)                                            /*!< Reset value of JOBCOUNT register.                            */

/* JOBCOUNT @Bits 0..31 : Number of completed jobs in current descriptor list. */
#define QSPI_DMA_STATUS_JOBCOUNT_JOBCOUNT_Pos \
    (0UL)                                                     /*!< Position of JOBCOUNT field.                                         */
#define QSPI_DMA_STATUS_JOBCOUNT_JOBCOUNT_Msk \
    (0xFFFFFFFFUL << QSPI_DMA_STATUS_JOBCOUNT_JOBCOUNT_Pos)   /*!< Bit mask of
                                                               *              JOBCOUNT field.*/

/* QSPI_DMA_STATUS_BUSERROR: AXI bus error status. */
#define QSPI_DMA_STATUS_BUSERROR_ResetValue \
    (0x00000000UL)                                            /*!< Reset value of BUSERROR register.                            */

/* BUSERROR @Bits 0..2 : AXI bus error type */
#define QSPI_DMA_STATUS_BUSERROR_BUSERROR_Pos \
    (0UL)                                                     /*!< Position of BUSERROR field.                                         */
#define QSPI_DMA_STATUS_BUSERROR_BUSERROR_Msk \
    (0x7UL << QSPI_DMA_STATUS_BUSERROR_BUSERROR_Pos)          /*!< Bit mask of BUSERROR
                                                               *                     field.*/
#define QSPI_DMA_STATUS_BUSERROR_BUSERROR_Min \
    (0x0UL)                                                   /*!< Min enumerator value of BUSERROR field.                           */
#define QSPI_DMA_STATUS_BUSERROR_BUSERROR_Max \
    (0x4UL)                                                   /*!< Max enumerator value of BUSERROR field.                           */
#define QSPI_DMA_STATUS_BUSERROR_BUSERROR_NoError \
    (0x0UL)                                                   /*!< There are no errors.                                          */
#define QSPI_DMA_STATUS_BUSERROR_BUSERROR_ReadError \
    (0x1UL)                                                   /*!< Error related to memory when reading joblist, or error
                                                               *                                                     related to memory/register when reading data.*/
#define QSPI_DMA_STATUS_BUSERROR_BUSERROR_ReadDecodeError \
    (0x2UL)                                                   /*!< Error related to the joblist address when reading
                                                               *                                                           joblist, or error related to address when reading
                                                               *                                                           memory/register.*/
#define QSPI_DMA_STATUS_BUSERROR_BUSERROR_WriteError \
    (0x3UL)                                                   /*!< Error related to memory/register when writing data.        */
#define QSPI_DMA_STATUS_BUSERROR_BUSERROR_WriteDecodeError \
    (0x4UL)                                                   /*!< Error related to the memory/register address when
                                                               *                                                            writing data.*/

/* QSPI_DMA_STATUS_FIFO: Data fifo status. */
#define QSPI_DMA_STATUS_FIFO_ResetValue \
    (0x00000000UL)                                            /*!< Reset value of FIFO register.                                    */

/* RXFIFO @Bits 0..1 : Memory-to-Peripheral direction data fifo status */
#define QSPI_DMA_STATUS_FIFO_RXFIFO_Pos \
    (0UL)                                                     /*!< Position of RXFIFO field.                                            */
#define QSPI_DMA_STATUS_FIFO_RXFIFO_Msk \
    (0x3UL << QSPI_DMA_STATUS_FIFO_RXFIFO_Pos)                /*!< Bit mask of RXFIFO field.            */
#define QSPI_DMA_STATUS_FIFO_RXFIFO_Min \
    (0x0UL)                                                   /*!< Min enumerator value of RXFIFO field.                                */
#define QSPI_DMA_STATUS_FIFO_RXFIFO_Max \
    (0x2UL)                                                   /*!< Max enumerator value of RXFIFO field.                                */
#define QSPI_DMA_STATUS_FIFO_RXFIFO_Empty \
    (0x0UL)                                                   /*!< Fifo is empty.                                                       */
#define QSPI_DMA_STATUS_FIFO_RXFIFO_NotEmpty \
    (0x1UL)                                                   /*!< Fifo contains data.                                                */
#define QSPI_DMA_STATUS_FIFO_RXFIFO_Full \
    (0x2UL)                                                   /*!< Fifo is full.                                                        */

/* TXFIFO @Bits 2..3 : Peripheral-to-Memory direction data fifo status */
#define QSPI_DMA_STATUS_FIFO_TXFIFO_Pos \
    (2UL)                                                     /*!< Position of TXFIFO field.                                            */
#define QSPI_DMA_STATUS_FIFO_TXFIFO_Msk \
    (0x3UL << QSPI_DMA_STATUS_FIFO_TXFIFO_Pos)                /*!< Bit mask of TXFIFO field.            */
#define QSPI_DMA_STATUS_FIFO_TXFIFO_Min \
    (0x0UL)                                                   /*!< Min enumerator value of TXFIFO field.                                */
#define QSPI_DMA_STATUS_FIFO_TXFIFO_Max \
    (0x2UL)                                                   /*!< Max enumerator value of TXFIFO field.                                */
#define QSPI_DMA_STATUS_FIFO_TXFIFO_Empty \
    (0x0UL)                                                   /*!< Fifo is empty.                                                       */
#define QSPI_DMA_STATUS_FIFO_TXFIFO_NotEmpty \
    (0x1UL)                                                   /*!< Fifo contains data.                                                */
#define QSPI_DMA_STATUS_FIFO_TXFIFO_Full \
    (0x2UL)                                                   /*!< Fifo is full.                                                        */

/* QSPI_DMA_STATUS_ACTIVE: DMA activity state. */
#define QSPI_DMA_STATUS_ACTIVE_ResetValue \
    (0x00000000UL)                                            /*!< Reset value of ACTIVE register.                                */

/* ACTIVE @Bit 0 : DMA activity state. */
#define QSPI_DMA_STATUS_ACTIVE_ACTIVE_Pos \
    (0UL)                                                     /*!< Position of ACTIVE field.                                            */
#define QSPI_DMA_STATUS_ACTIVE_ACTIVE_Msk \
    (0x1UL << QSPI_DMA_STATUS_ACTIVE_ACTIVE_Pos)              /*!< Bit mask of ACTIVE field.        */
#define QSPI_DMA_STATUS_ACTIVE_ACTIVE_Min \
    (0x0UL)                                                   /*!< Min enumerator value of ACTIVE field.                                */
#define QSPI_DMA_STATUS_ACTIVE_ACTIVE_Max \
    (0x1UL)                                                   /*!< Max enumerator value of ACTIVE field.                                */
#define QSPI_DMA_STATUS_ACTIVE_ACTIVE_Idle \
    (0x0UL)                                                   /*!< DMA is in idle state.                                                */
#define QSPI_DMA_STATUS_ACTIVE_ACTIVE_Active \
    (0x1UL)                                                   /*!< Data is being transferred.                                         */

/* ================================================= Struct QSPI_DMA_CONFIG ================================================== */
/**
 * @brief CONFIG [QSPI_DMA_CONFIG] General config registers
 */
typedef struct
{
    __IOM uint32_t BUFFERFILL;                                      /*!<                Data to be used when data is sent with BufferFill
                                                                     *                                    attribute*/
    __IOM uint32_t LISTPTR;                                         /*!<                Start address of descriptor list                      */
    __IOM uint32_t LISTPARTTHRESH;                                  /*!<                Threshold value for generating event
                                                                     *                                    EVENTS_DONE.PARTLIST when descriptor list is partially
                                                                     *                                    completed. Value is number of jobs.*/
} NRF_QSPI_DMA_CONFIG_Type;                                         /*!<                                                                 */

/* QSPI_DMA_CONFIG_BUFFERFILL: Data to be used when data is sent with BufferFill attribute */
#define QSPI_DMA_CONFIG_BUFFERFILL_ResetValue \
    (0x00000000UL)                                                  /*!< Reset value of BUFFERFILL register.                        */

/* BUFFERFILL @Bits 0..7 : Data to be used when data is sent with BufferFill attribute */
#define QSPI_DMA_CONFIG_BUFFERFILL_BUFFERFILL_Pos \
    (0UL)                                                           /*!< Position of BUFFERFILL field.                                   */
#define QSPI_DMA_CONFIG_BUFFERFILL_BUFFERFILL_Msk \
    (0xFFUL << QSPI_DMA_CONFIG_BUFFERFILL_BUFFERFILL_Pos)           /*!< Bit mask of
                                                                     *                BUFFERFILL field.*/

/* QSPI_DMA_CONFIG_LISTPTR: Start address of descriptor list */
#define QSPI_DMA_CONFIG_LISTPTR_ResetValue \
    (0x00000000UL)                                                  /*!< Reset value of LISTPTR register.                              */

/* LISTPTR @Bits 0..31 : Start address of descriptor list */
#define QSPI_DMA_CONFIG_LISTPTR_LISTPTR_Pos \
    (0UL)                                                           /*!< Position of LISTPTR field.                                           */
#define QSPI_DMA_CONFIG_LISTPTR_LISTPTR_Msk \
    (0xFFFFFFFFUL << QSPI_DMA_CONFIG_LISTPTR_LISTPTR_Pos)           /*!< Bit mask of LISTPTR
                                                                     *                field.*/

/* QSPI_DMA_CONFIG_LISTPARTTHRESH: Threshold value for generating event EVENTS_DONE.PARTLIST when descriptor list is partially
 *                                  completed. Value is number of jobs. */

#define QSPI_DMA_CONFIG_LISTPARTTHRESH_ResetValue \
    (0x00000000UL)                                                  /*!< Reset value of LISTPARTTHRESH register.                */

/* LISTPARTTHRESH @Bits 0..15 : Threshold value. */
#define QSPI_DMA_CONFIG_LISTPARTTHRESH_LISTPARTTHRESH_Pos \
    (0UL)                                                           /*!< Position of LISTPARTTHRESH field.                       */
#define QSPI_DMA_CONFIG_LISTPARTTHRESH_LISTPARTTHRESH_Msk \
    (0xFFFFUL << QSPI_DMA_CONFIG_LISTPARTTHRESH_LISTPARTTHRESH_Pos) /*!<
                                                                     *      Bit mask of LISTPARTTHRESH field.*/

/* ===================================================== Struct QSPI_DMA ===================================================== */
/**
 * @brief DMA [QSPI_DMA] (unspecified)
 */
typedef struct
{
    __IOM NRF_QSPI_DMA_STATUS_Type STATUS; /*!<                EasyVDMA status registers.                            */
    __IOM NRF_QSPI_DMA_CONFIG_Type CONFIG; /*!<                General config registers                              */
} NRF_QSPI_DMA_Type;                       /*!<                                                                      */

/* ================================================== Struct QSPI_CORE_CORE ================================================== */
/**
 * @brief CORE [QSPI_CORE_CORE] (unspecified)
 */
typedef struct
{
    __IOM uint32_t CTRLR0;                                     /*!<                Control Register 0                                    */
    __IOM uint32_t CTRLR1;                                     /*!<                Control Register 1                                    */
    __IOM uint32_t SQSPIENR;                                   /*!<                SQSPI Enable Register                                   */
    __IOM uint32_t MWCR;                                       /*!<                Microwire Control Register                            */
    __IOM uint32_t SER;                                        /*!<                Target Enable Register                                */
    __IOM uint32_t BAUDR;                                      /*!<                Baud Rate Select                                      */
    __IOM uint32_t TXFTLR;                                     /*!<                Transmit FIFO Threshold Level                         */
    __IOM uint32_t RXFTLR;                                     /*!<                Receive FIFO Threshold Level                          */
    __IOM uint32_t TXFLR;                                      /*!<                Transmit FIFO Level Register                          */
    __IOM uint32_t RXFLR;                                      /*!<                Receive FIFO Level Register                           */
    __IOM uint32_t SR;                                         /*!<                Status Register                                       */
    __IOM uint32_t IMR;                                        /*!<                Interrupt Mask Register                               */
    __IOM uint32_t ISR;                                        /*!<                Interrupt Status Register                             */
    __IOM uint32_t RISR;                                       /*!<                Raw Interrupt Status Register                         */
    __IOM uint32_t TXEICR;                                     /*!<                Transmit FIFO Error Interrupt Clear Register Register */
    __IOM uint32_t RXOICR;                                     /*!<                Receive FIFO Overflow Interrupt Clear Register        */
    __IOM uint32_t RXUICR;                                     /*!<                Receive FIFO Underflow Interrupt Clear Register       */
    __IOM uint32_t MSTICR;                                     /*!<                Multi-Controller Interrupt Clear Register             */
    __IOM uint32_t ICR;                                        /*!<                Interrupt Clear Register                              */
    __IOM uint32_t DMACR;                                      /*!<                DMA Control Register                                  */
    __IOM uint32_t DMATDLR;                                    /*!<                DMA Transmit Data Level                               */
    __IOM uint32_t DMARDLR;                                    /*!<                DMA Receive Data Level                                */
    __IOM uint32_t IDR;                                        /*!<                Identification Register                               */
    __IOM uint32_t SQSPICVERSIONID;                            /*!<                QSPI CORE component version                           */
    __IOM uint32_t DR[36];                                     /*!<                QSPI CORE Data Register                               */
    __IOM uint32_t RXSAMPLEDELAY;                              /*!<                RX Sample Delay Register                              */
    __IOM uint32_t SPICTRLR0;                                  /*!<                SPI_CTRLR0 - SPI Control Register                     */
    __IOM uint32_t SPICTRLR1;                                  /*!<                SPI Control 1 register                                */
    __IOM uint32_t SPITECR;                                    /*!<                SPI Transmit Error Interrupt Clear Register           */
} NRF_QSPI_CORE_CORE_Type;                                     /*!<                                                                      */

/* QSPI_CORE_CORE_CTRLR0: Control Register 0 */
#define QSPI_CORE_CORE_CTRLR0_ResetValue \
    (0x00000007UL)                                             /*!< Reset value of CTRLR0 register.                                 */

/* DFS @Bits 0..4 : Data Frame Size. */
#define QSPI_CORE_CORE_CTRLR0_DFS_Pos (0UL)                    /*!< Position of DFS field.                                               */
#define QSPI_CORE_CORE_CTRLR0_DFS_Msk \
    (0x1FUL << QSPI_CORE_CORE_CTRLR0_DFS_Pos)                  /*!< Bit mask of DFS field.                  */
#define QSPI_CORE_CORE_CTRLR0_DFS_Min \
    (0x3UL)                                                    /*!< Min enumerator value of DFS field.                                   */
#define QSPI_CORE_CORE_CTRLR0_DFS_Max \
    (0x1FUL)                                                   /*!< Max enumerator value of DFS field.                                   */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS04BIT \
    (0x03UL)                                                   /*!< 04-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS05BIT \
    (0x04UL)                                                   /*!< 05-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS06BIT \
    (0x05UL)                                                   /*!< 06-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS07BIT \
    (0x06UL)                                                   /*!< 07-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS08BIT \
    (0x07UL)                                                   /*!< 08-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS09BIT \
    (0x08UL)                                                   /*!< 09-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS10BIT \
    (0x09UL)                                                   /*!< 10-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS11BIT \
    (0x0AUL)                                                   /*!< 11-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS12BIT \
    (0x0BUL)                                                   /*!< 12-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS13BIT \
    (0x0CUL)                                                   /*!< 13-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS14BIT \
    (0x0DUL)                                                   /*!< 14-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS15BIT \
    (0x0EUL)                                                   /*!< 15-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS16BIT \
    (0x0FUL)                                                   /*!< 16-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS17BIT \
    (0x10UL)                                                   /*!< 17-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS18BIT \
    (0x11UL)                                                   /*!< 18-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS19BIT \
    (0x12UL)                                                   /*!< 19-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS20BIT \
    (0x13UL)                                                   /*!< 20-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS21BIT \
    (0x14UL)                                                   /*!< 21-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS22BIT \
    (0x15UL)                                                   /*!< 22-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS23BIT \
    (0x16UL)                                                   /*!< 23-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS24BIT \
    (0x17UL)                                                   /*!< 24-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS25BIT \
    (0x18UL)                                                   /*!< 25-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS26BIT \
    (0x19UL)                                                   /*!< 26-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS27BIT \
    (0x1AUL)                                                   /*!< 27-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS28BIT \
    (0x1BUL)                                                   /*!< 28-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS29BIT \
    (0x1CUL)                                                   /*!< 29-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS30BIT \
    (0x1DUL)                                                   /*!< 30-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS31BIT \
    (0x1EUL)                                                   /*!< 31-bit serial data transfer                                         */
#define QSPI_CORE_CORE_CTRLR0_DFS_DFS32BIT \
    (0x1FUL)                                                   /*!< 32-bit serial data transfer                                         */

/* RSVDCTRLR05 @Bit 5 : (unspecified) */
#define QSPI_CORE_CORE_CTRLR0_RSVDCTRLR05_Pos \
    (5UL)                                                      /*!< Position of RSVDCTRLR05 field.                                      */
#define QSPI_CORE_CORE_CTRLR0_RSVDCTRLR05_Msk \
    (0x1UL << QSPI_CORE_CORE_CTRLR0_RSVDCTRLR05_Pos)           /*!< Bit mask of RSVDCTRLR05
                                                                *                     field.*/

/* FRF @Bits 6..7 : Frame Format. */
#define QSPI_CORE_CORE_CTRLR0_FRF_Pos (6UL)                    /*!< Position of FRF field.                                               */
#define QSPI_CORE_CORE_CTRLR0_FRF_Msk \
    (0x3UL << QSPI_CORE_CORE_CTRLR0_FRF_Pos)                   /*!< Bit mask of FRF field.                   */
#define QSPI_CORE_CORE_CTRLR0_FRF_Min \
    (0x0UL)                                                    /*!< Min enumerator value of FRF field.                                   */
#define QSPI_CORE_CORE_CTRLR0_FRF_Max \
    (0x2UL)                                                    /*!< Max enumerator value of FRF field.                                   */
#define QSPI_CORE_CORE_CTRLR0_FRF_SPI \
    (0x0UL)                                                    /*!< Motorola SPI Frame Format                                            */
#define QSPI_CORE_CORE_CTRLR0_FRF_SSP \
    (0x1UL)                                                    /*!< Texas Instruments SSP Frame Format                                   */
#define QSPI_CORE_CORE_CTRLR0_FRF_MICROWIRE \
    (0x2UL)                                                    /*!< National Semiconductors Microwire Frame Format                      */

/* SCPH @Bit 8 : Serial Clock Phase. */
#define QSPI_CORE_CORE_CTRLR0_SCPH_Pos \
    (8UL)                                                      /*!< Position of SCPH field.                                              */
#define QSPI_CORE_CORE_CTRLR0_SCPH_Msk \
    (0x1UL << QSPI_CORE_CORE_CTRLR0_SCPH_Pos)                  /*!< Bit mask of SCPH field.                */
#define QSPI_CORE_CORE_CTRLR0_SCPH_Min \
    (0x0UL)                                                    /*!< Min enumerator value of SCPH field.                                  */
#define QSPI_CORE_CORE_CTRLR0_SCPH_Max \
    (0x1UL)                                                    /*!< Max enumerator value of SCPH field.                                  */
#define QSPI_CORE_CORE_CTRLR0_SCPH_MIDDLEBIT \
    (0x0UL)                                                    /*!< Serial clock toggles in middle of first bit                        */
#define QSPI_CORE_CORE_CTRLR0_SCPH_STARTBIT \
    (0x1UL)                                                    /*!< Serial clock toggles at start of first bit                          */

/* SCPOL @Bit 9 : Serial Clock Polarity. */
#define QSPI_CORE_CORE_CTRLR0_SCPOL_Pos \
    (9UL)                                                      /*!< Position of SCPOL field.                                             */
#define QSPI_CORE_CORE_CTRLR0_SCPOL_Msk \
    (0x1UL << QSPI_CORE_CORE_CTRLR0_SCPOL_Pos)                 /*!< Bit mask of SCPOL field.             */
#define QSPI_CORE_CORE_CTRLR0_SCPOL_Min \
    (0x0UL)                                                    /*!< Min enumerator value of SCPOL field.                                 */
#define QSPI_CORE_CORE_CTRLR0_SCPOL_Max \
    (0x1UL)                                                    /*!< Max enumerator value of SCPOL field.                                 */
#define QSPI_CORE_CORE_CTRLR0_SCPOL_INACTIVEHIGH \
    (0x0UL)                                                    /*!< Inactive state of serial clock is low                          */
#define QSPI_CORE_CORE_CTRLR0_SCPOL_INACTIVELOW \
    (0x1UL)                                                    /*!< Inactive state of serial clock is high                          */

/* TMOD @Bits 10..11 : Transfer Mode. */
#define QSPI_CORE_CORE_CTRLR0_TMOD_Pos \
    (10UL)                                                     /*!< Position of TMOD field.                                              */
#define QSPI_CORE_CORE_CTRLR0_TMOD_Msk \
    (0x3UL << QSPI_CORE_CORE_CTRLR0_TMOD_Pos)                  /*!< Bit mask of TMOD field.                */
#define QSPI_CORE_CORE_CTRLR0_TMOD_Min \
    (0x0UL)                                                    /*!< Min enumerator value of TMOD field.                                  */
#define QSPI_CORE_CORE_CTRLR0_TMOD_Max \
    (0x3UL)                                                    /*!< Max enumerator value of TMOD field.                                  */
#define QSPI_CORE_CORE_CTRLR0_TMOD_TXANDRX \
    (0x0UL)                                                    /*!< Transmit & Receive; Not Applicable in enhanced SPI operating mode or
                                                                *                                            when SQSPIC_HAS_TX_RX_EN is set to 0*/
#define QSPI_CORE_CORE_CTRLR0_TMOD_TXONLY \
    (0x1UL)                                                    /*!< Transmit only mode; Or Write in enhanced SPI operating mode          */
#define QSPI_CORE_CORE_CTRLR0_TMOD_RXONLY \
    (0x2UL)                                                    /*!< Receive only mode; Or Read in enhanced SPI operating mode            */
#define QSPI_CORE_CORE_CTRLR0_TMOD_EEPROMREAD \
    (0x3UL)                                                    /*!< EEPROM Read mode; Not Applicable in enhanced SPI operating mode   */

/* SLVOE @Bit 12 : Target Output Enable. */
#define QSPI_CORE_CORE_CTRLR0_SLVOE_Pos \
    (12UL)                                                     /*!< Position of SLVOE field.                                             */
#define QSPI_CORE_CORE_CTRLR0_SLVOE_Msk \
    (0x1UL << QSPI_CORE_CORE_CTRLR0_SLVOE_Pos)                 /*!< Bit mask of SLVOE field.             */
#define QSPI_CORE_CORE_CTRLR0_SLVOE_Min \
    (0x0UL)                                                    /*!< Min enumerator value of SLVOE field.                                 */
#define QSPI_CORE_CORE_CTRLR0_SLVOE_Max \
    (0x1UL)                                                    /*!< Max enumerator value of SLVOE field.                                 */
#define QSPI_CORE_CORE_CTRLR0_SLVOE_ENABLED \
    (0x0UL)                                                    /*!< Target Output is enabled                                            */
#define QSPI_CORE_CORE_CTRLR0_SLVOE_DISABLED \
    (0x1UL)                                                    /*!< Target Output is disabled                                          */

/* SRL @Bit 13 : Shift Register Loop. */
#define QSPI_CORE_CORE_CTRLR0_SRL_Pos \
    (13UL)                                                     /*!< Position of SRL field.                                               */
#define QSPI_CORE_CORE_CTRLR0_SRL_Msk \
    (0x1UL << QSPI_CORE_CORE_CTRLR0_SRL_Pos)                   /*!< Bit mask of SRL field.                   */
#define QSPI_CORE_CORE_CTRLR0_SRL_Min \
    (0x0UL)                                                    /*!< Min enumerator value of SRL field.                                   */
#define QSPI_CORE_CORE_CTRLR0_SRL_Max \
    (0x1UL)                                                    /*!< Max enumerator value of SRL field.                                   */
#define QSPI_CORE_CORE_CTRLR0_SRL_NORMALMODE \
    (0x0UL)                                                    /*!< Normal mode operation                                              */
#define QSPI_CORE_CORE_CTRLR0_SRL_TESTINGMODE \
    (0x1UL)                                                    /*!< Test Mode Operation                                               */

/* SSTE @Bit 14 : Target Select Toggle Enable. */
#define QSPI_CORE_CORE_CTRLR0_SSTE_Pos \
    (14UL)                                                     /*!< Position of SSTE field.                                              */
#define QSPI_CORE_CORE_CTRLR0_SSTE_Msk \
    (0x1UL << QSPI_CORE_CORE_CTRLR0_SSTE_Pos)                  /*!< Bit mask of SSTE field.                */
#define QSPI_CORE_CORE_CTRLR0_SSTE_Min \
    (0x0UL)                                                    /*!< Min enumerator value of SSTE field.                                  */
#define QSPI_CORE_CORE_CTRLR0_SSTE_Max \
    (0x1UL)                                                    /*!< Max enumerator value of SSTE field.                                  */
#define QSPI_CORE_CORE_CTRLR0_SSTE_TOGGLEDISABLE \
    (0x0UL)                                                    /*!< ss_*_n will stay low and sclk will run continuously for the
                                                                *                                                  duration of the transfer*/
#define QSPI_CORE_CORE_CTRLR0_SSTE_TOGGLEEN \
    (0x1UL)                                                    /*!< ss_*_n line will toggle between consecutive data frames, with the
                                                                *                                             serial clock (sclk) being held to its default value while ss_*_n is
                                                                *                                             high*/

/* RSVDCTRLR015 @Bit 15 : (unspecified) */
#define QSPI_CORE_CORE_CTRLR0_RSVDCTRLR015_Pos \
    (15UL)                                                     /*!< Position of RSVDCTRLR015 field.                                   */
#define QSPI_CORE_CORE_CTRLR0_RSVDCTRLR015_Msk \
    (0x1UL << QSPI_CORE_CORE_CTRLR0_RSVDCTRLR015_Pos)          /*!< Bit mask of RSVDCTRLR015
                                                               *                    field.*/

/* CFS @Bits 16..19 : Control Frame Size. */
#define QSPI_CORE_CORE_CTRLR0_CFS_Pos \
    (16UL)                                                     /*!< Position of CFS field.                                               */
#define QSPI_CORE_CORE_CTRLR0_CFS_Msk \
    (0xFUL << QSPI_CORE_CORE_CTRLR0_CFS_Pos)                   /*!< Bit mask of CFS field.                   */
#define QSPI_CORE_CORE_CTRLR0_CFS_Min \
    (0x0UL)                                                    /*!< Min enumerator value of CFS field.                                   */
#define QSPI_CORE_CORE_CTRLR0_CFS_Max \
    (0xFUL)                                                    /*!< Max enumerator value of CFS field.                                   */
#define QSPI_CORE_CORE_CTRLR0_CFS_SIZE01BIT \
    (0x0UL)                                                    /*!< 01-bit Control Word                                                 */
#define QSPI_CORE_CORE_CTRLR0_CFS_SIZE02BIT \
    (0x1UL)                                                    /*!< 02-bit Control Word                                                 */
#define QSPI_CORE_CORE_CTRLR0_CFS_SIZE03BIT \
    (0x2UL)                                                    /*!< 03-bit Control Word                                                 */
#define QSPI_CORE_CORE_CTRLR0_CFS_SIZE04BIT \
    (0x3UL)                                                    /*!< 04-bit Control Word                                                 */
#define QSPI_CORE_CORE_CTRLR0_CFS_SIZE05BIT \
    (0x4UL)                                                    /*!< 05-bit Control Word                                                 */
#define QSPI_CORE_CORE_CTRLR0_CFS_SIZE06BIT \
    (0x5UL)                                                    /*!< 06-bit Control Word                                                 */
#define QSPI_CORE_CORE_CTRLR0_CFS_SIZE07BIT \
    (0x6UL)                                                    /*!< 07-bit Control Word                                                 */
#define QSPI_CORE_CORE_CTRLR0_CFS_SIZE08BIT \
    (0x7UL)                                                    /*!< 08-bit Control Word                                                 */
#define QSPI_CORE_CORE_CTRLR0_CFS_SIZE09BIT \
    (0x8UL)                                                    /*!< 09-bit Control Word                                                 */
#define QSPI_CORE_CORE_CTRLR0_CFS_SIZE10BIT \
    (0x9UL)                                                    /*!< 10-bit Control Word                                                 */
#define QSPI_CORE_CORE_CTRLR0_CFS_SIZE11BIT \
    (0xAUL)                                                    /*!< 11-bit Control Word                                                 */
#define QSPI_CORE_CORE_CTRLR0_CFS_SIZE12BIT \
    (0xBUL)                                                    /*!< 12-bit Control Word                                                 */
#define QSPI_CORE_CORE_CTRLR0_CFS_SIZE13BIT \
    (0xCUL)                                                    /*!< 13-bit Control Word                                                 */
#define QSPI_CORE_CORE_CTRLR0_CFS_SIZE14BIT \
    (0xDUL)                                                    /*!< 14-bit Control Word                                                 */
#define QSPI_CORE_CORE_CTRLR0_CFS_SIZE15BIT \
    (0xEUL)                                                    /*!< 15-bit Control Word                                                 */
#define QSPI_CORE_CORE_CTRLR0_CFS_SIZE16BIT \
    (0xFUL)                                                    /*!< 16-bit Control Word                                                 */

/* RSVDCTRLR02021 @Bits 20..21 : (unspecified) */
#define QSPI_CORE_CORE_CTRLR0_RSVDCTRLR02021_Pos \
    (20UL)                                                     /*!< Position of RSVDCTRLR02021 field.                               */
#define QSPI_CORE_CORE_CTRLR0_RSVDCTRLR02021_Msk \
    (0x3UL << QSPI_CORE_CORE_CTRLR0_RSVDCTRLR02021_Pos)        /*!< Bit mask of
                                                                *                  RSVDCTRLR02021 field.*/

/* SPIFRF @Bits 22..23 : SPI Frame Format */
#define QSPI_CORE_CORE_CTRLR0_SPIFRF_Pos \
    (22UL)                                                     /*!< Position of SPIFRF field.                                            */
#define QSPI_CORE_CORE_CTRLR0_SPIFRF_Msk \
    (0x3UL << QSPI_CORE_CORE_CTRLR0_SPIFRF_Pos)                /*!< Bit mask of SPIFRF field.          */
#define QSPI_CORE_CORE_CTRLR0_SPIFRF_Min \
    (0x0UL)                                                    /*!< Min enumerator value of SPIFRF field.                                */
#define QSPI_CORE_CORE_CTRLR0_SPIFRF_Max \
    (0x3UL)                                                    /*!< Max enumerator value of SPIFRF field.                                */
#define QSPI_CORE_CORE_CTRLR0_SPIFRF_SPISTANDARD \
    (0x0UL)                                                    /*!< Standard SPI Format                                            */
#define QSPI_CORE_CORE_CTRLR0_SPIFRF_SPIDUAL \
    (0x1UL)                                                    /*!< Dual SPI Format                                                    */
#define QSPI_CORE_CORE_CTRLR0_SPIFRF_SPIQUAD \
    (0x2UL)                                                    /*!< Quad SPI Format                                                    */
#define QSPI_CORE_CORE_CTRLR0_SPIFRF_SPIOCTAL \
    (0x3UL)                                                    /*!< Octal SPI Format                                                  */

/* SPIHYPERBUSEN @Bit 24 : SPI Hyperbus Frame format enable. */
#define QSPI_CORE_CORE_CTRLR0_SPIHYPERBUSEN_Pos \
    (24UL)                                                     /*!< Position of SPIHYPERBUSEN field.                                 */
#define QSPI_CORE_CORE_CTRLR0_SPIHYPERBUSEN_Msk \
    (0x1UL << QSPI_CORE_CORE_CTRLR0_SPIHYPERBUSEN_Pos)         /*!< Bit mask of
                                                                *                   SPIHYPERBUSEN field.*/
#define QSPI_CORE_CORE_CTRLR0_SPIHYPERBUSEN_Min \
    (0x0UL)                                                    /*!< Min enumerator value of SPIHYPERBUSEN field.                    */
#define QSPI_CORE_CORE_CTRLR0_SPIHYPERBUSEN_Max \
    (0x1UL)                                                    /*!< Max enumerator value of SPIHYPERBUSEN field.                    */
#define QSPI_CORE_CORE_CTRLR0_SPIHYPERBUSEN_DISABLE \
    (0x0UL)                                                    /*!< Disable Hyperbus Format                                     */
#define QSPI_CORE_CORE_CTRLR0_SPIHYPERBUSEN_ENABLE \
    (0x1UL)                                                    /*!< Enable Hyperbus Format                                       */

/* SPIDWSEN @Bit 25 : Enable Dynamic wait states in SPI mode of operation. This field is only applicable when CTRLR0.FRF is set
 *                    to 0 (Motorola SPI Frame Format). */

#define QSPI_CORE_CORE_CTRLR0_SPIDWSEN_Pos \
    (25UL)                                                     /*!< Position of SPIDWSEN field.                                          */
#define QSPI_CORE_CORE_CTRLR0_SPIDWSEN_Msk \
    (0x1UL << QSPI_CORE_CORE_CTRLR0_SPIDWSEN_Pos)              /*!< Bit mask of SPIDWSEN field.    */
#define QSPI_CORE_CORE_CTRLR0_SPIDWSEN_Min \
    (0x0UL)                                                    /*!< Min enumerator value of SPIDWSEN field.                              */
#define QSPI_CORE_CORE_CTRLR0_SPIDWSEN_Max \
    (0x1UL)                                                    /*!< Max enumerator value of SPIDWSEN field.                              */
#define QSPI_CORE_CORE_CTRLR0_SPIDWSEN_DISABLE \
    (0x0UL)                                                    /*!< Disable SPI Dynamic Wait State                                   */
#define QSPI_CORE_CORE_CTRLR0_SPIDWSEN_ENABLE \
    (0x1UL)                                                    /*!< Enable SPI Dynamic Wait State                                     */

/* CLKLOOPEN @Bit 26 : Clock loop back enable bit. Once this bit is set to 1, QSPI CORE will use looped back clock (mst_sclk_in)
 *                     to capture read data */

#define QSPI_CORE_CORE_CTRLR0_CLKLOOPEN_Pos \
    (26UL)                                                     /*!< Position of CLKLOOPEN field.                                         */
#define QSPI_CORE_CORE_CTRLR0_CLKLOOPEN_Msk \
    (0x1UL << QSPI_CORE_CORE_CTRLR0_CLKLOOPEN_Pos)             /*!< Bit mask of CLKLOOPEN field. */
#define QSPI_CORE_CORE_CTRLR0_CLKLOOPEN_Min \
    (0x0UL)                                                    /*!< Min enumerator value of CLKLOOPEN field.                            */
#define QSPI_CORE_CORE_CTRLR0_CLKLOOPEN_Max \
    (0x1UL)                                                    /*!< Max enumerator value of CLKLOOPEN field.                            */
#define QSPI_CORE_CORE_CTRLR0_CLKLOOPEN_DISABLE \
    (0x0UL)                                                    /*!< Disable Clock Loop Back                                         */
#define QSPI_CORE_CORE_CTRLR0_CLKLOOPEN_ENABLE \
    (0x1UL)                                                    /*!< Enable Clock Loop Back                                           */

/* RSVDCTRLR02730 @Bits 27..30 : (unspecified) */
#define QSPI_CORE_CORE_CTRLR0_RSVDCTRLR02730_Pos \
    (27UL)                                                     /*!< Position of RSVDCTRLR02730 field.                               */
#define QSPI_CORE_CORE_CTRLR0_RSVDCTRLR02730_Msk \
    (0xFUL << QSPI_CORE_CORE_CTRLR0_RSVDCTRLR02730_Pos)        /*!< Bit mask of
                                                                *                  RSVDCTRLR02730 field.*/

/* SQSPIISMST @Bit 31 : This field selects if QSPI CORE is working in Controller or Target mode */
#define QSPI_CORE_CORE_CTRLR0_SQSPIISMST_Pos \
    (31UL)                                                     /*!< Position of SQSPIISMST field.                                          */
#define QSPI_CORE_CORE_CTRLR0_SQSPIISMST_Msk \
    (0x1UL << QSPI_CORE_CORE_CTRLR0_SQSPIISMST_Pos)            /*!< Bit mask of SQSPIISMST field.    */
#define QSPI_CORE_CORE_CTRLR0_SQSPIISMST_Min \
    (0x0UL)                                                    /*!< Min enumerator value of SQSPIISMST field.                              */
#define QSPI_CORE_CORE_CTRLR0_SQSPIISMST_Max \
    (0x1UL)                                                    /*!< Max enumerator value of SQSPIISMST field.                              */
#define QSPI_CORE_CORE_CTRLR0_SQSPIISMST_TARGET \
    (0x0UL)                                                    /*!< QSPI CORE is Target                                               */
#define QSPI_CORE_CORE_CTRLR0_SQSPIISMST_CONTROLLER \
    (0x1UL)                                                    /*!< QSPI CORE is Controller                                       */

/* QSPI_CORE_CORE_CTRLR1: Control Register 1 */
#define QSPI_CORE_CORE_CTRLR1_ResetValue \
    (0x00000000UL)                                             /*!< Reset value of CTRLR1 register.                                 */

/* NDF @Bits 0..15 : Number of Data Frames. */
#define QSPI_CORE_CORE_CTRLR1_NDF_Pos (0UL)                    /*!< Position of NDF field.                                               */
#define QSPI_CORE_CORE_CTRLR1_NDF_Msk \
    (0xFFFFUL << QSPI_CORE_CORE_CTRLR1_NDF_Pos)                /*!< Bit mask of NDF field.                */

/* RSVDCTRLR1 @Bits 16..31 : (unspecified) */
#define QSPI_CORE_CORE_CTRLR1_RSVDCTRLR1_Pos \
    (16UL)                                                     /*!< Position of RSVDCTRLR1 field.                                       */
#define QSPI_CORE_CORE_CTRLR1_RSVDCTRLR1_Msk \
    (0xFFFFUL << QSPI_CORE_CORE_CTRLR1_RSVDCTRLR1_Pos)         /*!< Bit mask of RSVDCTRLR1
                                                                *                   field.*/

/* QSPI_CORE_CORE_SQSPIENR: SQSPI Enable Register */
#define QSPI_CORE_CORE_SQSPIENR_ResetValue \
    (0x00000000UL)                                             /*!< Reset value of SQSPIENR register.                                 */

/* SQSPICEN @Bit 0 : SQSPI Enable. */
#define QSPI_CORE_CORE_SQSPIENR_SQSPICEN_Pos \
    (0UL)                                                      /*!< Position of SQSPICEN field.                                            */
#define QSPI_CORE_CORE_SQSPIENR_SQSPICEN_Msk \
    (0x1UL << QSPI_CORE_CORE_SQSPIENR_SQSPICEN_Pos)            /*!< Bit mask of SQSPICEN field.          */
#define QSPI_CORE_CORE_SQSPIENR_SQSPICEN_Min \
    (0x0UL)                                                    /*!< Min enumerator value of SQSPICEN field.                                */
#define QSPI_CORE_CORE_SQSPIENR_SQSPICEN_Max \
    (0x1UL)                                                    /*!< Max enumerator value of SQSPICEN field.                                */
#define QSPI_CORE_CORE_SQSPIENR_SQSPICEN_DISABLE \
    (0x0UL)                                                    /*!< Disables QSPI CORE                                                 */
#define QSPI_CORE_CORE_SQSPIENR_SQSPICEN_ENABLED \
    (0x1UL)                                                    /*!< Enables QSPI CORE                                                  */

/* RSVDSQSPIENR @Bits 1..31 : (unspecified) */
#define QSPI_CORE_CORE_SQSPIENR_RSVDSQSPIENR_Pos \
    (1UL)                                                      /*!< Position of RSVDSQSPIENR field.                                        */
#define QSPI_CORE_CORE_SQSPIENR_RSVDSQSPIENR_Msk \
    (0x7FFFFFFFUL << QSPI_CORE_CORE_SQSPIENR_RSVDSQSPIENR_Pos) /*!< Bit mask of
                                                                *           RSVDSQSPIENR field.*/

/* QSPI_CORE_CORE_MWCR: Microwire Control Register */
#define QSPI_CORE_CORE_MWCR_ResetValue \
    (0x00000000UL)                                             /*!< Reset value of MWCR register.                                     */

/* MWMOD @Bit 0 : Microwire Transfer Mode. */
#define QSPI_CORE_CORE_MWCR_MWMOD_Pos (0UL)                    /*!< Position of MWMOD field.                                             */
#define QSPI_CORE_CORE_MWCR_MWMOD_Msk \
    (0x1UL << QSPI_CORE_CORE_MWCR_MWMOD_Pos)                   /*!< Bit mask of MWMOD field.                 */
#define QSPI_CORE_CORE_MWCR_MWMOD_Min \
    (0x0UL)                                                    /*!< Min enumerator value of MWMOD field.                                 */
#define QSPI_CORE_CORE_MWCR_MWMOD_Max \
    (0x1UL)                                                    /*!< Max enumerator value of MWMOD field.                                 */
#define QSPI_CORE_CORE_MWCR_MWMOD_NONSEQUENTIAL \
    (0x0UL)                                                    /*!< Non-Sequential Transfer                                         */
#define QSPI_CORE_CORE_MWCR_MWMOD_SEQUENTIAL \
    (0x1UL)                                                    /*!< Sequential Transfer                                                */

/* MDD @Bit 1 : Microwire Control. */
#define QSPI_CORE_CORE_MWCR_MDD_Pos (1UL)                      /*!< Position of MDD field.                                               */
#define QSPI_CORE_CORE_MWCR_MDD_Msk \
    (0x1UL << QSPI_CORE_CORE_MWCR_MDD_Pos)                     /*!< Bit mask of MDD field.                       */
#define QSPI_CORE_CORE_MWCR_MDD_Min (0x0UL)                    /*!< Min enumerator value of MDD field.                                   */
#define QSPI_CORE_CORE_MWCR_MDD_Max (0x1UL)                    /*!< Max enumerator value of MDD field.                                   */
#define QSPI_CORE_CORE_MWCR_MDD_RECEIVE \
    (0x0UL)                                                    /*!< SQSPI receives data                                                    */
#define QSPI_CORE_CORE_MWCR_MDD_TRANSMIT \
    (0x1UL)                                                    /*!< SQSPI transmits data                                                   */

/* MHS @Bit 2 : Microwire Handshaking. */
#define QSPI_CORE_CORE_MWCR_MHS_Pos (2UL)                      /*!< Position of MHS field.                                               */
#define QSPI_CORE_CORE_MWCR_MHS_Msk \
    (0x1UL << QSPI_CORE_CORE_MWCR_MHS_Pos)                     /*!< Bit mask of MHS field.                       */
#define QSPI_CORE_CORE_MWCR_MHS_Min (0x0UL)                    /*!< Min enumerator value of MHS field.                                   */
#define QSPI_CORE_CORE_MWCR_MHS_Max (0x1UL)                    /*!< Max enumerator value of MHS field.                                   */
#define QSPI_CORE_CORE_MWCR_MHS_DISABLE \
    (0x0UL)                                                    /*!< handshaking interface is disabled                                    */
#define QSPI_CORE_CORE_MWCR_MHS_ENABLED \
    (0x1UL)                                                    /*!< handshaking interface is enabled                                     */

/* RSVDMWCR @Bits 3..31 : (unspecified) */
#define QSPI_CORE_CORE_MWCR_RSVDMWCR_Pos \
    (3UL)                                                      /*!< Position of RSVDMWCR field.                                          */
#define QSPI_CORE_CORE_MWCR_RSVDMWCR_Msk \
    (0x1FFFFFFFUL << QSPI_CORE_CORE_MWCR_RSVDMWCR_Pos)         /*!< Bit mask of RSVDMWCR field. */

/* QSPI_CORE_CORE_SER: Target Enable Register */
#define QSPI_CORE_CORE_SER_ResetValue \
    (0x00000000UL)                                             /*!< Reset value of SER register.                                       */

/* SER @Bits 0..3 : Chip Select Enable Flag. */
#define QSPI_CORE_CORE_SER_SER_Pos (0UL)                       /*!< Position of SER field.                                               */
#define QSPI_CORE_CORE_SER_SER_Msk \
    (0xFUL << QSPI_CORE_CORE_SER_SER_Pos)                      /*!< Bit mask of SER field.                         */

/* RSVDSER @Bits 4..31 : (unspecified) */
#define QSPI_CORE_CORE_SER_RSVDSER_Pos \
    (4UL)                                                      /*!< Position of RSVDSER field.                                           */
#define QSPI_CORE_CORE_SER_RSVDSER_Msk \
    (0xFFFFFFFUL << QSPI_CORE_CORE_SER_RSVDSER_Pos)            /*!< Bit mask of RSVDSER field.       */

/* QSPI_CORE_CORE_BAUDR: Baud Rate Select */
#define QSPI_CORE_CORE_BAUDR_ResetValue \
    (0x00000002UL)                                             /*!< Reset value of BAUDR register.                                   */

/* RSVDBAUDR0 @Bit 0 : (unspecified) */
#define QSPI_CORE_CORE_BAUDR_RSVDBAUDR0_Pos \
    (0UL)                                                      /*!< Position of RSVDBAUDR0 field.                                        */
#define QSPI_CORE_CORE_BAUDR_RSVDBAUDR0_Msk \
    (0x1UL << QSPI_CORE_CORE_BAUDR_RSVDBAUDR0_Pos)             /*!< Bit mask of RSVDBAUDR0 field.*/

/* SCKDV @Bits 1..15 : SQSPI Clock Divider. */
#define QSPI_CORE_CORE_BAUDR_SCKDV_Pos \
    (1UL)                                                      /*!< Position of SCKDV field.                                             */
#define QSPI_CORE_CORE_BAUDR_SCKDV_Msk \
    (0x7FFFUL << QSPI_CORE_CORE_BAUDR_SCKDV_Pos)               /*!< Bit mask of SCKDV field.            */

/* RSVDBAUDR1631 @Bits 16..31 : (unspecified) */
#define QSPI_CORE_CORE_BAUDR_RSVDBAUDR1631_Pos \
    (16UL)                                                     /*!< Position of RSVDBAUDR1631 field.                                  */
#define QSPI_CORE_CORE_BAUDR_RSVDBAUDR1631_Msk \
    (0xFFFFUL << QSPI_CORE_CORE_BAUDR_RSVDBAUDR1631_Pos)       /*!< Bit mask of
                                                                *                 RSVDBAUDR1631 field.*/

/* QSPI_CORE_CORE_TXFTLR: Transmit FIFO Threshold Level */
#define QSPI_CORE_CORE_TXFTLR_ResetValue \
    (0x00080000UL)                                             /*!< Reset value of TXFTLR register.                                 */

/* TFT @Bits 0..3 : Transmit FIFO Threshold. */
#define QSPI_CORE_CORE_TXFTLR_TFT_Pos (0UL)                    /*!< Position of TFT field.                                               */
#define QSPI_CORE_CORE_TXFTLR_TFT_Msk \
    (0xFUL << QSPI_CORE_CORE_TXFTLR_TFT_Pos)                   /*!< Bit mask of TFT field.                   */

/* RSVDTXFTLR @Bits 4..15 : (unspecified) */
#define QSPI_CORE_CORE_TXFTLR_RSVDTXFTLR_Pos \
    (4UL)                                                      /*!< Position of RSVDTXFTLR field.                                        */
#define QSPI_CORE_CORE_TXFTLR_RSVDTXFTLR_Msk \
    (0xFFFUL << QSPI_CORE_CORE_TXFTLR_RSVDTXFTLR_Pos)          /*!< Bit mask of RSVDTXFTLR
                                                                *                    field.*/

/* TXFTHR @Bits 16..19 : Transfer start FIFO level. */
#define QSPI_CORE_CORE_TXFTLR_TXFTHR_Pos \
    (16UL)                                                     /*!< Position of TXFTHR field.                                            */
#define QSPI_CORE_CORE_TXFTLR_TXFTHR_Msk \
    (0xFUL << QSPI_CORE_CORE_TXFTLR_TXFTHR_Pos)                /*!< Bit mask of TXFTHR field.          */

/* RSVDTXFTHR @Bits 20..31 : (unspecified) */
#define QSPI_CORE_CORE_TXFTLR_RSVDTXFTHR_Pos \
    (20UL)                                                     /*!< Position of RSVDTXFTHR field.                                       */
#define QSPI_CORE_CORE_TXFTLR_RSVDTXFTHR_Msk \
    (0xFFFUL << QSPI_CORE_CORE_TXFTLR_RSVDTXFTHR_Pos)          /*!< Bit mask of RSVDTXFTHR
                                                                *                    field.*/

/* QSPI_CORE_CORE_RXFTLR: Receive FIFO Threshold Level */
#define QSPI_CORE_CORE_RXFTLR_ResetValue \
    (0x00000000UL)                                             /*!< Reset value of RXFTLR register.                                 */

/* RFT @Bits 0..3 : Receive FIFO Threshold. */
#define QSPI_CORE_CORE_RXFTLR_RFT_Pos (0UL)                    /*!< Position of RFT field.                                               */
#define QSPI_CORE_CORE_RXFTLR_RFT_Msk \
    (0xFUL << QSPI_CORE_CORE_RXFTLR_RFT_Pos)                   /*!< Bit mask of RFT field.                   */

/* RSVDRXFTLR @Bits 4..31 : (unspecified) */
#define QSPI_CORE_CORE_RXFTLR_RSVDRXFTLR_Pos \
    (4UL)                                                      /*!< Position of RSVDRXFTLR field.                                        */
#define QSPI_CORE_CORE_RXFTLR_RSVDRXFTLR_Msk \
    (0xFFFFFFFUL << QSPI_CORE_CORE_RXFTLR_RSVDRXFTLR_Pos)      /*!< Bit mask of RSVDRXFTLR
                                                                *                field.*/

/* QSPI_CORE_CORE_TXFLR: Transmit FIFO Level Register */
#define QSPI_CORE_CORE_TXFLR_ResetValue \
    (0x00000000UL)                                             /*!< Reset value of TXFLR register.                                   */

/* TXTFL @Bits 0..4 : Transmit FIFO Level. */
#define QSPI_CORE_CORE_TXFLR_TXTFL_Pos \
    (0UL)                                                      /*!< Position of TXTFL field.                                             */
#define QSPI_CORE_CORE_TXFLR_TXTFL_Msk \
    (0x1FUL << QSPI_CORE_CORE_TXFLR_TXTFL_Pos)                 /*!< Bit mask of TXTFL field.              */

/* RSVDTXFLR @Bits 5..31 : (unspecified) */
#define QSPI_CORE_CORE_TXFLR_RSVDTXFLR_Pos \
    (5UL)                                                      /*!< Position of RSVDTXFLR field.                                         */
#define QSPI_CORE_CORE_TXFLR_RSVDTXFLR_Msk \
    (0x7FFFFFFUL << QSPI_CORE_CORE_TXFLR_RSVDTXFLR_Pos)        /*!< Bit mask of RSVDTXFLR
                                                                *                  field.*/

/* QSPI_CORE_CORE_RXFLR: Receive FIFO Level Register */
#define QSPI_CORE_CORE_RXFLR_ResetValue \
    (0x00000000UL)                                             /*!< Reset value of RXFLR register.                                   */

/* RXTFL @Bits 0..4 : Receive FIFO Level. */
#define QSPI_CORE_CORE_RXFLR_RXTFL_Pos \
    (0UL)                                                      /*!< Position of RXTFL field.                                             */
#define QSPI_CORE_CORE_RXFLR_RXTFL_Msk \
    (0x1FUL << QSPI_CORE_CORE_RXFLR_RXTFL_Pos)                 /*!< Bit mask of RXTFL field.              */

/* RSVDRXFLR @Bits 5..31 : (unspecified) */
#define QSPI_CORE_CORE_RXFLR_RSVDRXFLR_Pos \
    (5UL)                                                      /*!< Position of RSVDRXFLR field.                                         */
#define QSPI_CORE_CORE_RXFLR_RSVDRXFLR_Msk \
    (0x7FFFFFFUL << QSPI_CORE_CORE_RXFLR_RSVDRXFLR_Pos)        /*!< Bit mask of RSVDRXFLR
                                                                *                  field.*/

/* QSPI_CORE_CORE_SR: Status Register */
#define QSPI_CORE_CORE_SR_ResetValue \
    (0x00000006UL)                                             /*!< Reset value of SR register.                                         */

/* BUSY @Bit 0 : SQSPI Busy Flag. */
#define QSPI_CORE_CORE_SR_BUSY_Pos (0UL)                       /*!< Position of BUSY field.                                              */
#define QSPI_CORE_CORE_SR_BUSY_Msk \
    (0x1UL << QSPI_CORE_CORE_SR_BUSY_Pos)                      /*!< Bit mask of BUSY field.                        */
#define QSPI_CORE_CORE_SR_BUSY_Min (0x0UL)                     /*!< Min enumerator value of BUSY field.                                  */
#define QSPI_CORE_CORE_SR_BUSY_Max (0x1UL)                     /*!< Max enumerator value of BUSY field.                                  */
#define QSPI_CORE_CORE_SR_BUSY_INACTIVE \
    (0x0UL)                                                    /*!< QSPI CORE is idle or disabled                                        */
#define QSPI_CORE_CORE_SR_BUSY_ACTIVE \
    (0x1UL)                                                    /*!< QSPI CORE is actively transferring data                              */

/* TFNF @Bit 1 : Transmit FIFO Not Full. */
#define QSPI_CORE_CORE_SR_TFNF_Pos  (1UL)                      /*!< Position of TFNF field.                                              */
#define QSPI_CORE_CORE_SR_TFNF_Msk \
    (0x1UL << QSPI_CORE_CORE_SR_TFNF_Pos)                      /*!< Bit mask of TFNF field.                        */
#define QSPI_CORE_CORE_SR_TFNF_Min  (0x0UL)                    /*!< Min enumerator value of TFNF field.                                  */
#define QSPI_CORE_CORE_SR_TFNF_Max  (0x1UL)                    /*!< Max enumerator value of TFNF field.                                  */
#define QSPI_CORE_CORE_SR_TFNF_FULL (0x0UL)                    /*!< Tx FIFO is full                                                      */
#define QSPI_CORE_CORE_SR_TFNF_NOTFULL \
    (0x1UL)                                                    /*!< Tx FIFO is not Full                                                  */

/* TFE @Bit 2 : Transmit FIFO Empty. */
#define QSPI_CORE_CORE_SR_TFE_Pos   (2UL)                      /*!< Position of TFE field.                                               */
#define QSPI_CORE_CORE_SR_TFE_Msk \
    (0x1UL << QSPI_CORE_CORE_SR_TFE_Pos)                       /*!< Bit mask of TFE field.                           */
#define QSPI_CORE_CORE_SR_TFE_Min   (0x0UL)                    /*!< Min enumerator value of TFE field.                                   */
#define QSPI_CORE_CORE_SR_TFE_Max   (0x1UL)                    /*!< Max enumerator value of TFE field.                                   */
#define QSPI_CORE_CORE_SR_TFE_NOTEMPTY \
    (0x0UL)                                                    /*!< Transmit FIFO is not empty                                           */
#define QSPI_CORE_CORE_SR_TFE_EMPTY (0x1UL)                    /*!< Transmit FIFO is empty                                               */

/* RFNE @Bit 3 : Receive FIFO Not Empty. */
#define QSPI_CORE_CORE_SR_RFNE_Pos (3UL)                       /*!< Position of RFNE field.                                              */
#define QSPI_CORE_CORE_SR_RFNE_Msk \
    (0x1UL << QSPI_CORE_CORE_SR_RFNE_Pos)                      /*!< Bit mask of RFNE field.                        */
#define QSPI_CORE_CORE_SR_RFNE_Min (0x0UL)                     /*!< Min enumerator value of RFNE field.                                  */
#define QSPI_CORE_CORE_SR_RFNE_Max (0x1UL)                     /*!< Max enumerator value of RFNE field.                                  */
#define QSPI_CORE_CORE_SR_RFNE_EMPTY \
    (0x0UL)                                                    /*!< Receive FIFO is empty                                                */
#define QSPI_CORE_CORE_SR_RFNE_NOTEMPTY \
    (0x1UL)                                                    /*!< Receive FIFO is not empty                                            */

/* RFF @Bit 4 : Receive FIFO Full. */
#define QSPI_CORE_CORE_SR_RFF_Pos  (4UL)                       /*!< Position of RFF field.                                               */
#define QSPI_CORE_CORE_SR_RFF_Msk \
    (0x1UL << QSPI_CORE_CORE_SR_RFF_Pos)                       /*!< Bit mask of RFF field.                           */
#define QSPI_CORE_CORE_SR_RFF_Min  (0x0UL)                     /*!< Min enumerator value of RFF field.                                   */
#define QSPI_CORE_CORE_SR_RFF_Max  (0x1UL)                     /*!< Max enumerator value of RFF field.                                   */
#define QSPI_CORE_CORE_SR_RFF_NOTFULL \
    (0x0UL)                                                    /*!< Receive FIFO is not full                                             */
#define QSPI_CORE_CORE_SR_RFF_FULL (0x1UL)                     /*!< Receive FIFO is full                                                 */

/* TXE @Bit 5 : Transmission Error. */
#define QSPI_CORE_CORE_SR_TXE_Pos (5UL)                        /*!< Position of TXE field.                                               */
#define QSPI_CORE_CORE_SR_TXE_Msk \
    (0x1UL << QSPI_CORE_CORE_SR_TXE_Pos)                       /*!< Bit mask of TXE field.                           */
#define QSPI_CORE_CORE_SR_TXE_Min (0x0UL)                      /*!< Min enumerator value of TXE field.                                   */
#define QSPI_CORE_CORE_SR_TXE_Max (0x1UL)                      /*!< Max enumerator value of TXE field.                                   */
#define QSPI_CORE_CORE_SR_TXE_NOERROR \
    (0x0UL)                                                    /*!< No Error                                                             */
#define QSPI_CORE_CORE_SR_TXE_TXERROR \
    (0x1UL)                                                    /*!< Transmission Error                                                   */

/* DCOL @Bit 6 : Data Collision Error. */
#define QSPI_CORE_CORE_SR_DCOL_Pos (6UL)                       /*!< Position of DCOL field.                                              */
#define QSPI_CORE_CORE_SR_DCOL_Msk \
    (0x1UL << QSPI_CORE_CORE_SR_DCOL_Pos)                      /*!< Bit mask of DCOL field.                        */
#define QSPI_CORE_CORE_SR_DCOL_Min (0x0UL)                     /*!< Min enumerator value of DCOL field.                                  */
#define QSPI_CORE_CORE_SR_DCOL_Max (0x1UL)                     /*!< Max enumerator value of DCOL field.                                  */
#define QSPI_CORE_CORE_SR_DCOL_NOERRORCONDITION \
    (0x0UL)                                                    /*!< No Error                                                        */
#define QSPI_CORE_CORE_SR_DCOL_TXCOLLISIONERROR \
    (0x1UL)                                                    /*!< Transmit Data Collision Error                                   */

/* RSVDSR @Bits 7..14 : (unspecified) */
#define QSPI_CORE_CORE_SR_RSVDSR_Pos (7UL)                     /*!< Position of RSVDSR field.                                            */
#define QSPI_CORE_CORE_SR_RSVDSR_Msk \
    (0xFFUL << QSPI_CORE_CORE_SR_RSVDSR_Pos)                   /*!< Bit mask of RSVDSR field.                 */

/* CMPLTDDF @Bits 15..31 : Completed Data frames */
#define QSPI_CORE_CORE_SR_CMPLTDDF_Pos \
    (15UL)                                                     /*!< Position of CMPLTDDF field.                                          */
#define QSPI_CORE_CORE_SR_CMPLTDDF_Msk \
    (0x1FFFFUL << QSPI_CORE_CORE_SR_CMPLTDDF_Pos)              /*!< Bit mask of CMPLTDDF field.        */

/* QSPI_CORE_CORE_IMR: Interrupt Mask Register */
#define QSPI_CORE_CORE_IMR_ResetValue \
    (0x0000043FUL)                                             /*!< Reset value of IMR register.                                       */

/* TXEIM @Bit 0 : Transmit FIFO Empty Interrupt Mask */
#define QSPI_CORE_CORE_IMR_TXEIM_Pos (0UL)                     /*!< Position of TXEIM field.                                             */
#define QSPI_CORE_CORE_IMR_TXEIM_Msk \
    (0x1UL << QSPI_CORE_CORE_IMR_TXEIM_Pos)                    /*!< Bit mask of TXEIM field.                   */
#define QSPI_CORE_CORE_IMR_TXEIM_Min \
    (0x0UL)                                                    /*!< Min enumerator value of TXEIM field.                                 */
#define QSPI_CORE_CORE_IMR_TXEIM_Max \
    (0x1UL)                                                    /*!< Max enumerator value of TXEIM field.                                 */
#define QSPI_CORE_CORE_IMR_TXEIM_MASKED \
    (0x0UL)                                                    /*!< sqspitxe_intr interrupt is masked                                     */
#define QSPI_CORE_CORE_IMR_TXEIM_UNMASKED \
    (0x1UL)                                                    /*!< sqspitxe_intr interrupt is not masked                                 */

/* TXOIM @Bit 1 : Transmit FIFO Overflow Interrupt Mask */
#define QSPI_CORE_CORE_IMR_TXOIM_Pos (1UL)                     /*!< Position of TXOIM field.                                             */
#define QSPI_CORE_CORE_IMR_TXOIM_Msk \
    (0x1UL << QSPI_CORE_CORE_IMR_TXOIM_Pos)                    /*!< Bit mask of TXOIM field.                   */
#define QSPI_CORE_CORE_IMR_TXOIM_Min \
    (0x0UL)                                                    /*!< Min enumerator value of TXOIM field.                                 */
#define QSPI_CORE_CORE_IMR_TXOIM_Max \
    (0x1UL)                                                    /*!< Max enumerator value of TXOIM field.                                 */
#define QSPI_CORE_CORE_IMR_TXOIM_MASKED \
    (0x0UL)                                                    /*!< sqspitxo_intr interrupt is masked                                     */
#define QSPI_CORE_CORE_IMR_TXOIM_UNMASKED \
    (0x1UL)                                                    /*!< sqspitxo_intr interrupt is not masked                                 */

/* RXUIM @Bit 2 : Receive FIFO Underflow Interrupt Mask */
#define QSPI_CORE_CORE_IMR_RXUIM_Pos (2UL)                     /*!< Position of RXUIM field.                                             */
#define QSPI_CORE_CORE_IMR_RXUIM_Msk \
    (0x1UL << QSPI_CORE_CORE_IMR_RXUIM_Pos)                    /*!< Bit mask of RXUIM field.                   */
#define QSPI_CORE_CORE_IMR_RXUIM_Min \
    (0x0UL)                                                    /*!< Min enumerator value of RXUIM field.                                 */
#define QSPI_CORE_CORE_IMR_RXUIM_Max \
    (0x1UL)                                                    /*!< Max enumerator value of RXUIM field.                                 */
#define QSPI_CORE_CORE_IMR_RXUIM_MASKED \
    (0x0UL)                                                    /*!< sqspirxu_intr interrupt is masked                                     */
#define QSPI_CORE_CORE_IMR_RXUIM_UNMASKED \
    (0x1UL)                                                    /*!< sqspirxu_intr interrupt is not masked                                 */

/* RXOIM @Bit 3 : Receive FIFO Overflow Interrupt Mask */
#define QSPI_CORE_CORE_IMR_RXOIM_Pos (3UL)                     /*!< Position of RXOIM field.                                             */
#define QSPI_CORE_CORE_IMR_RXOIM_Msk \
    (0x1UL << QSPI_CORE_CORE_IMR_RXOIM_Pos)                    /*!< Bit mask of RXOIM field.                   */
#define QSPI_CORE_CORE_IMR_RXOIM_Min \
    (0x0UL)                                                    /*!< Min enumerator value of RXOIM field.                                 */
#define QSPI_CORE_CORE_IMR_RXOIM_Max \
    (0x1UL)                                                    /*!< Max enumerator value of RXOIM field.                                 */
#define QSPI_CORE_CORE_IMR_RXOIM_MASKED \
    (0x0UL)                                                    /*!< sqspirxo_intr interrupt is masked                                     */
#define QSPI_CORE_CORE_IMR_RXOIM_UNMASKED \
    (0x1UL)                                                    /*!< sqspirxo_intr interrupt is not masked                                 */

/* RXFIM @Bit 4 : Receive FIFO Full Interrupt Mask */
#define QSPI_CORE_CORE_IMR_RXFIM_Pos (4UL)                     /*!< Position of RXFIM field.                                             */
#define QSPI_CORE_CORE_IMR_RXFIM_Msk \
    (0x1UL << QSPI_CORE_CORE_IMR_RXFIM_Pos)                    /*!< Bit mask of RXFIM field.                   */
#define QSPI_CORE_CORE_IMR_RXFIM_Min \
    (0x0UL)                                                    /*!< Min enumerator value of RXFIM field.                                 */
#define QSPI_CORE_CORE_IMR_RXFIM_Max \
    (0x1UL)                                                    /*!< Max enumerator value of RXFIM field.                                 */
#define QSPI_CORE_CORE_IMR_RXFIM_MASKED \
    (0x0UL)                                                    /*!< sqspirxf_intr interrupt is masked                                     */
#define QSPI_CORE_CORE_IMR_RXFIM_UNMASKED \
    (0x1UL)                                                    /*!< sqspirxf_intr interrupt is not masked                                 */

/* MSTIM @Bit 5 : Multi-Controller Contention Interrupt Mask. This bit field is not present if the QSPI CORE is configured as a
 *                serial-controller device. */

#define QSPI_CORE_CORE_IMR_MSTIM_Pos (5UL)                     /*!< Position of MSTIM field.                                             */
#define QSPI_CORE_CORE_IMR_MSTIM_Msk \
    (0x1UL << QSPI_CORE_CORE_IMR_MSTIM_Pos)                    /*!< Bit mask of MSTIM field.                   */
#define QSPI_CORE_CORE_IMR_MSTIM_Min \
    (0x0UL)                                                    /*!< Min enumerator value of MSTIM field.                                 */
#define QSPI_CORE_CORE_IMR_MSTIM_Max \
    (0x1UL)                                                    /*!< Max enumerator value of MSTIM field.                                 */
#define QSPI_CORE_CORE_IMR_MSTIM_MASKED \
    (0x0UL)                                                    /*!< sqspimst_intr interrupt is masked                                     */
#define QSPI_CORE_CORE_IMR_MSTIM_UNMASKED \
    (0x1UL)                                                    /*!< sqspimst_intr interrupt is not masked                                 */

/* XRXOIM @Bit 6 : XIP Receive FIFO Overflow Interrupt Mask */
#define QSPI_CORE_CORE_IMR_XRXOIM_Pos (6UL)                    /*!< Position of XRXOIM field.                                            */
#define QSPI_CORE_CORE_IMR_XRXOIM_Msk \
    (0x1UL << QSPI_CORE_CORE_IMR_XRXOIM_Pos)                   /*!< Bit mask of XRXOIM field.                */
#define QSPI_CORE_CORE_IMR_XRXOIM_Min \
    (0x0UL)                                                    /*!< Min enumerator value of XRXOIM field.                                */
#define QSPI_CORE_CORE_IMR_XRXOIM_Max \
    (0x1UL)                                                    /*!< Max enumerator value of XRXOIM field.                                */
#define QSPI_CORE_CORE_IMR_XRXOIM_MASKED \
    (0x0UL)                                                    /*!< sqspixrxo_intr interrupt is masked                                    */
#define QSPI_CORE_CORE_IMR_XRXOIM_UNMASKED \
    (0x1UL)                                                    /*!< sqspixrxo_intr interrupt is not masked                                */

/* TXUIM @Bit 7 : Transmit FIFO Underflow Interrupt Mask */
#define QSPI_CORE_CORE_IMR_TXUIM_Pos (7UL)                     /*!< Position of TXUIM field.                                             */
#define QSPI_CORE_CORE_IMR_TXUIM_Msk \
    (0x1UL << QSPI_CORE_CORE_IMR_TXUIM_Pos)                    /*!< Bit mask of TXUIM field.                   */
#define QSPI_CORE_CORE_IMR_TXUIM_Min \
    (0x0UL)                                                    /*!< Min enumerator value of TXUIM field.                                 */
#define QSPI_CORE_CORE_IMR_TXUIM_Max \
    (0x1UL)                                                    /*!< Max enumerator value of TXUIM field.                                 */
#define QSPI_CORE_CORE_IMR_TXUIM_MASKED \
    (0x0UL)                                                    /*!< sqspitxu_intr interrupt is masked                                     */
#define QSPI_CORE_CORE_IMR_TXUIM_UNMASKED \
    (0x1UL)                                                    /*!< sqspitxu_intr interrupt is not masked                                 */

/* AXIEM @Bit 8 : AXI Error Interrupt Mask */
#define QSPI_CORE_CORE_IMR_AXIEM_Pos (8UL)                     /*!< Position of AXIEM field.                                             */
#define QSPI_CORE_CORE_IMR_AXIEM_Msk \
    (0x1UL << QSPI_CORE_CORE_IMR_AXIEM_Pos)                    /*!< Bit mask of AXIEM field.                   */
#define QSPI_CORE_CORE_IMR_AXIEM_Min \
    (0x0UL)                                                    /*!< Min enumerator value of AXIEM field.                                 */
#define QSPI_CORE_CORE_IMR_AXIEM_Max \
    (0x1UL)                                                    /*!< Max enumerator value of AXIEM field.                                 */
#define QSPI_CORE_CORE_IMR_AXIEM_MASKED \
    (0x0UL)                                                    /*!< sqspiaxie_intr interrupt is masked                                    */
#define QSPI_CORE_CORE_IMR_AXIEM_UNMASKED \
    (0x1UL)                                                    /*!< sqspiaxie_intr interrupt is not masked                                */

/* RSVD9IMR @Bit 9 : (unspecified) */
#define QSPI_CORE_CORE_IMR_RSVD9IMR_Pos \
    (9UL)                                                      /*!< Position of RSVD9IMR field.                                          */
#define QSPI_CORE_CORE_IMR_RSVD9IMR_Msk \
    (0x1UL << QSPI_CORE_CORE_IMR_RSVD9IMR_Pos)                 /*!< Bit mask of RSVD9IMR field.          */

/* SPITEM @Bit 10 : SPI Transmit Error Interrupt Mask */
#define QSPI_CORE_CORE_IMR_SPITEM_Pos \
    (10UL)                                                     /*!< Position of SPITEM field.                                            */
#define QSPI_CORE_CORE_IMR_SPITEM_Msk \
    (0x1UL << QSPI_CORE_CORE_IMR_SPITEM_Pos)                   /*!< Bit mask of SPITEM field.                */
#define QSPI_CORE_CORE_IMR_SPITEM_Min \
    (0x0UL)                                                    /*!< Min enumerator value of SPITEM field.                                */
#define QSPI_CORE_CORE_IMR_SPITEM_Max \
    (0x1UL)                                                    /*!< Max enumerator value of SPITEM field.                                */
#define QSPI_CORE_CORE_IMR_SPITEM_MASKED \
    (0x0UL)                                                    /*!< sqspispite_intr interrupt is masked                                   */
#define QSPI_CORE_CORE_IMR_SPITEM_UNMASKED \
    (0x1UL)                                                    /*!< sqspispite_intr interrupt is not masked                               */

/* DONEM @Bit 11 : SQSPI Done Interrupt Mask */
#define QSPI_CORE_CORE_IMR_DONEM_Pos (11UL)                    /*!< Position of DONEM field.                                             */
#define QSPI_CORE_CORE_IMR_DONEM_Msk \
    (0x1UL << QSPI_CORE_CORE_IMR_DONEM_Pos)                    /*!< Bit mask of DONEM field.                   */
#define QSPI_CORE_CORE_IMR_DONEM_Min \
    (0x0UL)                                                    /*!< Min enumerator value of DONEM field.                                 */
#define QSPI_CORE_CORE_IMR_DONEM_Max \
    (0x1UL)                                                    /*!< Max enumerator value of DONEM field.                                 */
#define QSPI_CORE_CORE_IMR_DONEM_MASKED \
    (0x0UL)                                                    /*!< sqspidone_intr interrupt is masked                                    */
#define QSPI_CORE_CORE_IMR_DONEM_UNMASKED \
    (0x1UL)                                                    /*!< sqspidone_intr interrupt is not masked                                */

/* RSVD1231IMR @Bits 12..31 : (unspecified) */
#define QSPI_CORE_CORE_IMR_RSVD1231IMR_Pos \
    (12UL)                                                     /*!< Position of RSVD1231IMR field.                                       */
#define QSPI_CORE_CORE_IMR_RSVD1231IMR_Msk \
    (0xFFFFFUL << QSPI_CORE_CORE_IMR_RSVD1231IMR_Pos)          /*!< Bit mask of RSVD1231IMR
                                                                *                    field.*/

/* QSPI_CORE_CORE_ISR: Interrupt Status Register */
#define QSPI_CORE_CORE_ISR_ResetValue \
    (0x00000000UL)                                             /*!< Reset value of ISR register.                                       */

/* TXEIS @Bit 0 : Transmit FIFO Empty Interrupt Status */
#define QSPI_CORE_CORE_ISR_TXEIS_Pos (0UL)                     /*!< Position of TXEIS field.                                             */
#define QSPI_CORE_CORE_ISR_TXEIS_Msk \
    (0x1UL << QSPI_CORE_CORE_ISR_TXEIS_Pos)                    /*!< Bit mask of TXEIS field.                   */
#define QSPI_CORE_CORE_ISR_TXEIS_Min \
    (0x0UL)                                                    /*!< Min enumerator value of TXEIS field.                                 */
#define QSPI_CORE_CORE_ISR_TXEIS_Max \
    (0x1UL)                                                    /*!< Max enumerator value of TXEIS field.                                 */
#define QSPI_CORE_CORE_ISR_TXEIS_INACTIVE \
    (0x0UL)                                                    /*!< sqspitxe_intr interrupt is not active after masking                   */
#define QSPI_CORE_CORE_ISR_TXEIS_ACTIVE \
    (0x1UL)                                                    /*!< sqspitxe_intr interrupt is active after masking                       */

/* TXOIS @Bit 1 : Transmit FIFO Overflow Interrupt Status */
#define QSPI_CORE_CORE_ISR_TXOIS_Pos (1UL)                     /*!< Position of TXOIS field.                                             */
#define QSPI_CORE_CORE_ISR_TXOIS_Msk \
    (0x1UL << QSPI_CORE_CORE_ISR_TXOIS_Pos)                    /*!< Bit mask of TXOIS field.                   */
#define QSPI_CORE_CORE_ISR_TXOIS_Min \
    (0x0UL)                                                    /*!< Min enumerator value of TXOIS field.                                 */
#define QSPI_CORE_CORE_ISR_TXOIS_Max \
    (0x1UL)                                                    /*!< Max enumerator value of TXOIS field.                                 */
#define QSPI_CORE_CORE_ISR_TXOIS_INACTIVE \
    (0x0UL)                                                    /*!< sqspitxo_intr interrupt is not active after masking                   */
#define QSPI_CORE_CORE_ISR_TXOIS_ACTIVE \
    (0x1UL)                                                    /*!< sqspitxo_intr interrupt is active after masking                       */

/* RXUIS @Bit 2 : Receive FIFO Underflow Interrupt Status */
#define QSPI_CORE_CORE_ISR_RXUIS_Pos (2UL)                     /*!< Position of RXUIS field.                                             */
#define QSPI_CORE_CORE_ISR_RXUIS_Msk \
    (0x1UL << QSPI_CORE_CORE_ISR_RXUIS_Pos)                    /*!< Bit mask of RXUIS field.                   */
#define QSPI_CORE_CORE_ISR_RXUIS_Min \
    (0x0UL)                                                    /*!< Min enumerator value of RXUIS field.                                 */
#define QSPI_CORE_CORE_ISR_RXUIS_Max \
    (0x1UL)                                                    /*!< Max enumerator value of RXUIS field.                                 */
#define QSPI_CORE_CORE_ISR_RXUIS_INACTIVE \
    (0x0UL)                                                    /*!< sqspirxu_intr interrupt is not active after masking                   */
#define QSPI_CORE_CORE_ISR_RXUIS_ACTIVE \
    (0x1UL)                                                    /*!< sqspirxu_intr interrupt is active after masking                       */

/* RXOIS @Bit 3 : Receive FIFO Overflow Interrupt Status */
#define QSPI_CORE_CORE_ISR_RXOIS_Pos (3UL)                     /*!< Position of RXOIS field.                                             */
#define QSPI_CORE_CORE_ISR_RXOIS_Msk \
    (0x1UL << QSPI_CORE_CORE_ISR_RXOIS_Pos)                    /*!< Bit mask of RXOIS field.                   */
#define QSPI_CORE_CORE_ISR_RXOIS_Min \
    (0x0UL)                                                    /*!< Min enumerator value of RXOIS field.                                 */
#define QSPI_CORE_CORE_ISR_RXOIS_Max \
    (0x1UL)                                                    /*!< Max enumerator value of RXOIS field.                                 */
#define QSPI_CORE_CORE_ISR_RXOIS_INACTIVE \
    (0x0UL)                                                    /*!< sqspirxo_intr interrupt is not active after masking                   */
#define QSPI_CORE_CORE_ISR_RXOIS_ACTIVE \
    (0x1UL)                                                    /*!< sqspirxo_intr interrupt is active after masking                       */

/* RXFIS @Bit 4 : Receive FIFO Full Interrupt Status */
#define QSPI_CORE_CORE_ISR_RXFIS_Pos (4UL)                     /*!< Position of RXFIS field.                                             */
#define QSPI_CORE_CORE_ISR_RXFIS_Msk \
    (0x1UL << QSPI_CORE_CORE_ISR_RXFIS_Pos)                    /*!< Bit mask of RXFIS field.                   */
#define QSPI_CORE_CORE_ISR_RXFIS_Min \
    (0x0UL)                                                    /*!< Min enumerator value of RXFIS field.                                 */
#define QSPI_CORE_CORE_ISR_RXFIS_Max \
    (0x1UL)                                                    /*!< Max enumerator value of RXFIS field.                                 */
#define QSPI_CORE_CORE_ISR_RXFIS_INACTIVE \
    (0x0UL)                                                    /*!< sqspirxf_intr interrupt is not active after masking                   */
#define QSPI_CORE_CORE_ISR_RXFIS_ACTIVE \
    (0x1UL)                                                    /*!< sqspirxf_intr interrupt is active after masking                       */

/* MSTIS @Bit 5 : Multi-Controller Contention Interrupt Status. This bit field is not present */
#define QSPI_CORE_CORE_ISR_MSTIS_Pos (5UL)                     /*!< Position of MSTIS field.                                             */
#define QSPI_CORE_CORE_ISR_MSTIS_Msk \
    (0x1UL << QSPI_CORE_CORE_ISR_MSTIS_Pos)                    /*!< Bit mask of MSTIS field.                   */
#define QSPI_CORE_CORE_ISR_MSTIS_Min \
    (0x0UL)                                                    /*!< Min enumerator value of MSTIS field.                                 */
#define QSPI_CORE_CORE_ISR_MSTIS_Max \
    (0x1UL)                                                    /*!< Max enumerator value of MSTIS field.                                 */
#define QSPI_CORE_CORE_ISR_MSTIS_INACTIVE \
    (0x0UL)                                                    /*!< sqspimst_intr interrupt is not active after masking                   */
#define QSPI_CORE_CORE_ISR_MSTIS_ACTIVE \
    (0x1UL)                                                    /*!< sqspimst_intr interrupt is active after masking                       */

/* XRXOIS @Bit 6 : XIP Receive FIFO Overflow Interrupt Status */
#define QSPI_CORE_CORE_ISR_XRXOIS_Pos (6UL)                    /*!< Position of XRXOIS field.                                            */
#define QSPI_CORE_CORE_ISR_XRXOIS_Msk \
    (0x1UL << QSPI_CORE_CORE_ISR_XRXOIS_Pos)                   /*!< Bit mask of XRXOIS field.                */
#define QSPI_CORE_CORE_ISR_XRXOIS_Min \
    (0x0UL)                                                    /*!< Min enumerator value of XRXOIS field.                                */
#define QSPI_CORE_CORE_ISR_XRXOIS_Max \
    (0x1UL)                                                    /*!< Max enumerator value of XRXOIS field.                                */
#define QSPI_CORE_CORE_ISR_XRXOIS_INACTIVE \
    (0x0UL)                                                    /*!< sqspixrxo_intr interrupt is not active after masking                  */
#define QSPI_CORE_CORE_ISR_XRXOIS_ACTIVE \
    (0x1UL)                                                    /*!< sqspixrxo_intr interrupt is active after masking                      */

/* TXUIS @Bit 7 : Transmit FIFO Underflow Interrupt Status */
#define QSPI_CORE_CORE_ISR_TXUIS_Pos (7UL)                     /*!< Position of TXUIS field.                                             */
#define QSPI_CORE_CORE_ISR_TXUIS_Msk \
    (0x1UL << QSPI_CORE_CORE_ISR_TXUIS_Pos)                    /*!< Bit mask of TXUIS field.                   */
#define QSPI_CORE_CORE_ISR_TXUIS_Min \
    (0x0UL)                                                    /*!< Min enumerator value of TXUIS field.                                 */
#define QSPI_CORE_CORE_ISR_TXUIS_Max \
    (0x1UL)                                                    /*!< Max enumerator value of TXUIS field.                                 */
#define QSPI_CORE_CORE_ISR_TXUIS_INACTIVE \
    (0x0UL)                                                    /*!< sqspitxu_intr interrupt is not active after masking                   */
#define QSPI_CORE_CORE_ISR_TXUIS_ACTIVE \
    (0x1UL)                                                    /*!< sqspitxu_intr interrupt is active after masking                       */

/* AXIES @Bit 8 : AXI Error Interrupt Status */
#define QSPI_CORE_CORE_ISR_AXIES_Pos (8UL)                     /*!< Position of AXIES field.                                             */
#define QSPI_CORE_CORE_ISR_AXIES_Msk \
    (0x1UL << QSPI_CORE_CORE_ISR_AXIES_Pos)                    /*!< Bit mask of AXIES field.                   */
#define QSPI_CORE_CORE_ISR_AXIES_Min \
    (0x0UL)                                                    /*!< Min enumerator value of AXIES field.                                 */
#define QSPI_CORE_CORE_ISR_AXIES_Max \
    (0x1UL)                                                    /*!< Max enumerator value of AXIES field.                                 */
#define QSPI_CORE_CORE_ISR_AXIES_INACTIVE \
    (0x0UL)                                                    /*!< sqspiaxie_intr interrupt is not active after masking                  */
#define QSPI_CORE_CORE_ISR_AXIES_ACTIVE \
    (0x1UL)                                                    /*!< sqspiaxie_intr interrupt is active after masking                      */

/* RSVD9RISR @Bit 9 : (unspecified) */
#define QSPI_CORE_CORE_ISR_RSVD9RISR_Pos \
    (9UL)                                                      /*!< Position of RSVD9RISR field.                                         */
#define QSPI_CORE_CORE_ISR_RSVD9RISR_Msk \
    (0x1UL << QSPI_CORE_CORE_ISR_RSVD9RISR_Pos)                /*!< Bit mask of RSVD9RISR field.       */

/* SPITES @Bit 10 : SPI Transmit Error Interrupt */
#define QSPI_CORE_CORE_ISR_SPITES_Pos \
    (10UL)                                                     /*!< Position of SPITES field.                                            */
#define QSPI_CORE_CORE_ISR_SPITES_Msk \
    (0x1UL << QSPI_CORE_CORE_ISR_SPITES_Pos)                   /*!< Bit mask of SPITES field.                */
#define QSPI_CORE_CORE_ISR_SPITES_Min \
    (0x0UL)                                                    /*!< Min enumerator value of SPITES field.                                */
#define QSPI_CORE_CORE_ISR_SPITES_Max \
    (0x1UL)                                                    /*!< Max enumerator value of SPITES field.                                */
#define QSPI_CORE_CORE_ISR_SPITES_INACTIVE \
    (0x0UL)                                                    /*!< sqspispite_intr interrupt is not active after masking                 */
#define QSPI_CORE_CORE_ISR_SPITES_ACTIVE \
    (0x1UL)                                                    /*!< sqspispite_intr interrupt is active after masking                     */

/* DONES @Bit 11 : SQSPI Done Interrupt Status */
#define QSPI_CORE_CORE_ISR_DONES_Pos (11UL)                    /*!< Position of DONES field.                                             */
#define QSPI_CORE_CORE_ISR_DONES_Msk \
    (0x1UL << QSPI_CORE_CORE_ISR_DONES_Pos)                    /*!< Bit mask of DONES field.                   */
#define QSPI_CORE_CORE_ISR_DONES_Min \
    (0x0UL)                                                    /*!< Min enumerator value of DONES field.                                 */
#define QSPI_CORE_CORE_ISR_DONES_Max \
    (0x1UL)                                                    /*!< Max enumerator value of DONES field.                                 */
#define QSPI_CORE_CORE_ISR_DONES_INACTIVE \
    (0x0UL)                                                    /*!< sqspidone_intr interrupt is not active after masking                  */
#define QSPI_CORE_CORE_ISR_DONES_ACTIVE \
    (0x1UL)                                                    /*!< sqspidone_intr interrupt is active after masking                      */

/* RSVD1231RISR @Bits 12..31 : (unspecified) */
#define QSPI_CORE_CORE_ISR_RSVD1231RISR_Pos \
    (12UL)                                                     /*!< Position of RSVD1231RISR field.                                      */
#define QSPI_CORE_CORE_ISR_RSVD1231RISR_Msk \
    (0xFFFFFUL << QSPI_CORE_CORE_ISR_RSVD1231RISR_Pos)         /*!< Bit mask of RSVD1231RISR
                                                                *                   field.*/

/* QSPI_CORE_CORE_RISR: Raw Interrupt Status Register */
#define QSPI_CORE_CORE_RISR_ResetValue \
    (0x00000000UL)                                             /*!< Reset value of RISR register.                                     */

/* TXEIR @Bit 0 : Transmit FIFO Empty Raw Interrupt Status */
#define QSPI_CORE_CORE_RISR_TXEIR_Pos (0UL)                    /*!< Position of TXEIR field.                                             */
#define QSPI_CORE_CORE_RISR_TXEIR_Msk \
    (0x1UL << QSPI_CORE_CORE_RISR_TXEIR_Pos)                   /*!< Bit mask of TXEIR field.                 */
#define QSPI_CORE_CORE_RISR_TXEIR_Min \
    (0x0UL)                                                    /*!< Min enumerator value of TXEIR field.                                 */
#define QSPI_CORE_CORE_RISR_TXEIR_Max \
    (0x1UL)                                                    /*!< Max enumerator value of TXEIR field.                                 */
#define QSPI_CORE_CORE_RISR_TXEIR_INACTIVE \
    (0x0UL)                                                    /*!< sqspitxe_intr interrupt is not active prior masking                   */
#define QSPI_CORE_CORE_RISR_TXEIR_ACTIVE \
    (0x1UL)                                                    /*!< sqspitxe_intr interrupt is active prior to masking                    */

/* TXOIR @Bit 1 : Transmit FIFO Overflow Raw Interrupt Status */
#define QSPI_CORE_CORE_RISR_TXOIR_Pos (1UL)                    /*!< Position of TXOIR field.                                             */
#define QSPI_CORE_CORE_RISR_TXOIR_Msk \
    (0x1UL << QSPI_CORE_CORE_RISR_TXOIR_Pos)                   /*!< Bit mask of TXOIR field.                 */
#define QSPI_CORE_CORE_RISR_TXOIR_Min \
    (0x0UL)                                                    /*!< Min enumerator value of TXOIR field.                                 */
#define QSPI_CORE_CORE_RISR_TXOIR_Max \
    (0x1UL)                                                    /*!< Max enumerator value of TXOIR field.                                 */
#define QSPI_CORE_CORE_RISR_TXOIR_INACTIVE \
    (0x0UL)                                                    /*!< sqspitxo_intr interrupt is not active prior masking                   */
#define QSPI_CORE_CORE_RISR_TXOIR_ACTIVE \
    (0x1UL)                                                    /*!< sqspitxo_intr interrupt is active prior to masking                    */

/* RXUIR @Bit 2 : Receive FIFO Underflow Raw Interrupt Status */
#define QSPI_CORE_CORE_RISR_RXUIR_Pos (2UL)                    /*!< Position of RXUIR field.                                             */
#define QSPI_CORE_CORE_RISR_RXUIR_Msk \
    (0x1UL << QSPI_CORE_CORE_RISR_RXUIR_Pos)                   /*!< Bit mask of RXUIR field.                 */
#define QSPI_CORE_CORE_RISR_RXUIR_Min \
    (0x0UL)                                                    /*!< Min enumerator value of RXUIR field.                                 */
#define QSPI_CORE_CORE_RISR_RXUIR_Max \
    (0x1UL)                                                    /*!< Max enumerator value of RXUIR field.                                 */
#define QSPI_CORE_CORE_RISR_RXUIR_INACTIVE \
    (0x0UL)                                                    /*!< sqspirxu_intr interrupt is not active prior masking                   */
#define QSPI_CORE_CORE_RISR_RXUIR_ACTIVE \
    (0x1UL)                                                    /*!< sqspirxu_intr interrupt is active prior to masking                    */

/* RXOIR @Bit 3 : Receive FIFO Overflow Raw Interrupt Status */
#define QSPI_CORE_CORE_RISR_RXOIR_Pos (3UL)                    /*!< Position of RXOIR field.                                             */
#define QSPI_CORE_CORE_RISR_RXOIR_Msk \
    (0x1UL << QSPI_CORE_CORE_RISR_RXOIR_Pos)                   /*!< Bit mask of RXOIR field.                 */
#define QSPI_CORE_CORE_RISR_RXOIR_Min \
    (0x0UL)                                                    /*!< Min enumerator value of RXOIR field.                                 */
#define QSPI_CORE_CORE_RISR_RXOIR_Max \
    (0x1UL)                                                    /*!< Max enumerator value of RXOIR field.                                 */
#define QSPI_CORE_CORE_RISR_RXOIR_INACTIVE \
    (0x0UL)                                                    /*!< sqspirxo_intr interrupt is not active prior masking                   */
#define QSPI_CORE_CORE_RISR_RXOIR_ACTIVE \
    (0x1UL)                                                    /*!< sqspirxo_intr interrupt is active prior to masking                    */

/* RXFIR @Bit 4 : Receive FIFO Full Raw Interrupt Status */
#define QSPI_CORE_CORE_RISR_RXFIR_Pos (4UL)                    /*!< Position of RXFIR field.                                             */
#define QSPI_CORE_CORE_RISR_RXFIR_Msk \
    (0x1UL << QSPI_CORE_CORE_RISR_RXFIR_Pos)                   /*!< Bit mask of RXFIR field.                 */
#define QSPI_CORE_CORE_RISR_RXFIR_Min \
    (0x0UL)                                                    /*!< Min enumerator value of RXFIR field.                                 */
#define QSPI_CORE_CORE_RISR_RXFIR_Max \
    (0x1UL)                                                    /*!< Max enumerator value of RXFIR field.                                 */
#define QSPI_CORE_CORE_RISR_RXFIR_INACTIVE \
    (0x0UL)                                                    /*!< sqspirxf_intr interrupt is not active prior masking                   */
#define QSPI_CORE_CORE_RISR_RXFIR_ACTIVE \
    (0x1UL)                                                    /*!< sqspirxf_intr interrupt is active prior to masking                    */

/* MSTIR @Bit 5 : Multi-Controller Contention Raw Interrupt Status. This bit field is not present if the QSPI CORE is configured
 *                as a serial-target device. */

#define QSPI_CORE_CORE_RISR_MSTIR_Pos (5UL)                    /*!< Position of MSTIR field.                                             */
#define QSPI_CORE_CORE_RISR_MSTIR_Msk \
    (0x1UL << QSPI_CORE_CORE_RISR_MSTIR_Pos)                   /*!< Bit mask of MSTIR field.                 */
#define QSPI_CORE_CORE_RISR_MSTIR_Min \
    (0x0UL)                                                    /*!< Min enumerator value of MSTIR field.                                 */
#define QSPI_CORE_CORE_RISR_MSTIR_Max \
    (0x1UL)                                                    /*!< Max enumerator value of MSTIR field.                                 */
#define QSPI_CORE_CORE_RISR_MSTIR_INACTIVE \
    (0x0UL)                                                    /*!< sqspimst_intr interrupt is not active prior masking                   */
#define QSPI_CORE_CORE_RISR_MSTIR_ACTIVE \
    (0x1UL)                                                    /*!< sqspimst_intr interrupt is active prior to masking                    */

/* XRXOIR @Bit 6 : XIP Receive FIFO Overflow Raw Interrupt Status */
#define QSPI_CORE_CORE_RISR_XRXOIR_Pos \
    (6UL)                                                      /*!< Position of XRXOIR field.                                            */
#define QSPI_CORE_CORE_RISR_XRXOIR_Msk \
    (0x1UL << QSPI_CORE_CORE_RISR_XRXOIR_Pos)                  /*!< Bit mask of XRXOIR field.              */
#define QSPI_CORE_CORE_RISR_XRXOIR_Min \
    (0x0UL)                                                    /*!< Min enumerator value of XRXOIR field.                                */
#define QSPI_CORE_CORE_RISR_XRXOIR_Max \
    (0x1UL)                                                    /*!< Max enumerator value of XRXOIR field.                                */
#define QSPI_CORE_CORE_RISR_XRXOIR_INACTIVE \
    (0x0UL)                                                    /*!< sqspixrxo_intr interrupt is not active prior masking                 */
#define QSPI_CORE_CORE_RISR_XRXOIR_ACTIVE \
    (0x1UL)                                                    /*!< sqspixrxo_intr interrupt is active prior to masking                   */

/* TXUIR @Bit 7 : Transmit FIFO Underflow Interrupt Raw Status */
#define QSPI_CORE_CORE_RISR_TXUIR_Pos (7UL)                    /*!< Position of TXUIR field.                                             */
#define QSPI_CORE_CORE_RISR_TXUIR_Msk \
    (0x1UL << QSPI_CORE_CORE_RISR_TXUIR_Pos)                   /*!< Bit mask of TXUIR field.                 */
#define QSPI_CORE_CORE_RISR_TXUIR_Min \
    (0x0UL)                                                    /*!< Min enumerator value of TXUIR field.                                 */
#define QSPI_CORE_CORE_RISR_TXUIR_Max \
    (0x1UL)                                                    /*!< Max enumerator value of TXUIR field.                                 */
#define QSPI_CORE_CORE_RISR_TXUIR_INACTIVE \
    (0x0UL)                                                    /*!< sqspitxu_intr interrupt is not active prior masking                   */
#define QSPI_CORE_CORE_RISR_TXUIR_ACTIVE \
    (0x1UL)                                                    /*!< sqspitxu_intr interrupt is active prior to masking                    */

/* AXIER @Bit 8 : AXI Error Interrupt Raw Status */
#define QSPI_CORE_CORE_RISR_AXIER_Pos (8UL)                    /*!< Position of AXIER field.                                             */
#define QSPI_CORE_CORE_RISR_AXIER_Msk \
    (0x1UL << QSPI_CORE_CORE_RISR_AXIER_Pos)                   /*!< Bit mask of AXIER field.                 */
#define QSPI_CORE_CORE_RISR_AXIER_Min \
    (0x0UL)                                                    /*!< Min enumerator value of AXIER field.                                 */
#define QSPI_CORE_CORE_RISR_AXIER_Max \
    (0x1UL)                                                    /*!< Max enumerator value of AXIER field.                                 */
#define QSPI_CORE_CORE_RISR_AXIER_INACTIVE \
    (0x0UL)                                                    /*!< sqspiaxie_intr interrupt is not active prior masking                  */
#define QSPI_CORE_CORE_RISR_AXIER_ACTIVE \
    (0x1UL)                                                    /*!< sqspiaxie_intr interrupt is active prior to masking                   */

/* RSVD9RISR @Bit 9 : (unspecified) */
#define QSPI_CORE_CORE_RISR_RSVD9RISR_Pos \
    (9UL)                                                      /*!< Position of RSVD9RISR field.                                         */
#define QSPI_CORE_CORE_RISR_RSVD9RISR_Msk \
    (0x1UL << QSPI_CORE_CORE_RISR_RSVD9RISR_Pos)               /*!< Bit mask of RSVD9RISR field.     */

/* SPITER @Bit 10 : SPI Transmit Error Interrupt status. */
#define QSPI_CORE_CORE_RISR_SPITER_Pos \
    (10UL)                                                     /*!< Position of SPITER field.                                            */
#define QSPI_CORE_CORE_RISR_SPITER_Msk \
    (0x1UL << QSPI_CORE_CORE_RISR_SPITER_Pos)                  /*!< Bit mask of SPITER field.              */
#define QSPI_CORE_CORE_RISR_SPITER_Min \
    (0x0UL)                                                    /*!< Min enumerator value of SPITER field.                                */
#define QSPI_CORE_CORE_RISR_SPITER_Max \
    (0x1UL)                                                    /*!< Max enumerator value of SPITER field.                                */
#define QSPI_CORE_CORE_RISR_SPITER_INACTIVE \
    (0x0UL)                                                    /*!< sqspispite_intr interrupt is not active prior to masking             */
#define QSPI_CORE_CORE_RISR_SPITER_ACTIVE \
    (0x1UL)                                                    /*!< sqspispite_intr interrupt is active prior to masking                  */

/* DONER @Bit 11 : SQSPI Done Interrupt Raw Status */
#define QSPI_CORE_CORE_RISR_DONER_Pos \
    (11UL)                                                     /*!< Position of DONER field.                                             */
#define QSPI_CORE_CORE_RISR_DONER_Msk \
    (0x1UL << QSPI_CORE_CORE_RISR_DONER_Pos)                   /*!< Bit mask of DONER field.                 */
#define QSPI_CORE_CORE_RISR_DONER_Min \
    (0x0UL)                                                    /*!< Min enumerator value of DONER field.                                 */
#define QSPI_CORE_CORE_RISR_DONER_Max \
    (0x1UL)                                                    /*!< Max enumerator value of DONER field.                                 */
#define QSPI_CORE_CORE_RISR_DONER_INACTIVE \
    (0x0UL)                                                    /*!< sqspidone_intr interrupt is not active prior masking                  */
#define QSPI_CORE_CORE_RISR_DONER_ACTIVE \
    (0x1UL)                                                    /*!< sqspidone_intr interrupt is active prior to masking                   */

/* RSVD1231RISR @Bits 12..31 : (unspecified) */
#define QSPI_CORE_CORE_RISR_RSVD1231RISR_Pos \
    (12UL)                                                     /*!< Position of RSVD1231RISR field.                                     */
#define QSPI_CORE_CORE_RISR_RSVD1231RISR_Msk \
    (0xFFFFFUL << QSPI_CORE_CORE_RISR_RSVD1231RISR_Pos)        /*!< Bit mask of RSVD1231RISR
                                                                *                  field.*/

/* QSPI_CORE_CORE_TXEICR: Transmit FIFO Error Interrupt Clear Register Register */
#define QSPI_CORE_CORE_TXEICR_ResetValue \
    (0x00000000UL)                                             /*!< Reset value of TXEICR register.                                 */

/* TXEICR @Bit 0 : Clear Transmit FIFO Overflow/Underflow Interrupt. */
#define QSPI_CORE_CORE_TXEICR_TXEICR_Pos \
    (0UL)                                                      /*!< Position of TXEICR field.                                            */
#define QSPI_CORE_CORE_TXEICR_TXEICR_Msk \
    (0x1UL << QSPI_CORE_CORE_TXEICR_TXEICR_Pos)                /*!< Bit mask of TXEICR field.          */

/* RSVDTXEICR @Bits 1..31 : (unspecified) */
#define QSPI_CORE_CORE_TXEICR_RSVDTXEICR_Pos \
    (1UL)                                                      /*!< Position of RSVDTXEICR field.                                        */
#define QSPI_CORE_CORE_TXEICR_RSVDTXEICR_Msk \
    (0x7FFFFFFFUL << QSPI_CORE_CORE_TXEICR_RSVDTXEICR_Pos)     /*!< Bit mask of
                                                                *               RSVDTXEICR field.*/

/* QSPI_CORE_CORE_RXOICR: Receive FIFO Overflow Interrupt Clear Register */
#define QSPI_CORE_CORE_RXOICR_ResetValue \
    (0x00000000UL)                                             /*!< Reset value of RXOICR register.                                 */

/* RXOICR @Bit 0 : Clear Receive FIFO Overflow Interrupt. */
#define QSPI_CORE_CORE_RXOICR_RXOICR_Pos \
    (0UL)                                                      /*!< Position of RXOICR field.                                            */
#define QSPI_CORE_CORE_RXOICR_RXOICR_Msk \
    (0x1UL << QSPI_CORE_CORE_RXOICR_RXOICR_Pos)                /*!< Bit mask of RXOICR field.          */

/* RSVDRXOICR @Bits 1..31 : (unspecified) */
#define QSPI_CORE_CORE_RXOICR_RSVDRXOICR_Pos \
    (1UL)                                                      /*!< Position of RSVDRXOICR field.                                        */
#define QSPI_CORE_CORE_RXOICR_RSVDRXOICR_Msk \
    (0x7FFFFFFFUL << QSPI_CORE_CORE_RXOICR_RSVDRXOICR_Pos)     /*!< Bit mask of
                                                                *               RSVDRXOICR field.*/

/* QSPI_CORE_CORE_RXUICR: Receive FIFO Underflow Interrupt Clear Register */
#define QSPI_CORE_CORE_RXUICR_ResetValue \
    (0x00000000UL)                                             /*!< Reset value of RXUICR register.                                 */

/* RXUICR @Bit 0 : Clear Receive FIFO Underflow Interrupt. */
#define QSPI_CORE_CORE_RXUICR_RXUICR_Pos \
    (0UL)                                                      /*!< Position of RXUICR field.                                            */
#define QSPI_CORE_CORE_RXUICR_RXUICR_Msk \
    (0x1UL << QSPI_CORE_CORE_RXUICR_RXUICR_Pos)                /*!< Bit mask of RXUICR field.          */

/* RSVDRXUICR @Bits 1..31 : (unspecified) */
#define QSPI_CORE_CORE_RXUICR_RSVDRXUICR_Pos \
    (1UL)                                                      /*!< Position of RSVDRXUICR field.                                        */
#define QSPI_CORE_CORE_RXUICR_RSVDRXUICR_Msk \
    (0x7FFFFFFFUL << QSPI_CORE_CORE_RXUICR_RSVDRXUICR_Pos)     /*!< Bit mask of
                                                                *               RSVDRXUICR field.*/

/* QSPI_CORE_CORE_MSTICR: Multi-Controller Interrupt Clear Register */
#define QSPI_CORE_CORE_MSTICR_ResetValue \
    (0x00000000UL)                                             /*!< Reset value of MSTICR register.                                 */

/* MSTICR @Bit 0 : Clear Multi-Controller Contention Interrupt. */
#define QSPI_CORE_CORE_MSTICR_MSTICR_Pos \
    (0UL)                                                      /*!< Position of MSTICR field.                                            */
#define QSPI_CORE_CORE_MSTICR_MSTICR_Msk \
    (0x1UL << QSPI_CORE_CORE_MSTICR_MSTICR_Pos)                /*!< Bit mask of MSTICR field.          */

/* RSVDMSTICR @Bits 1..31 : (unspecified) */
#define QSPI_CORE_CORE_MSTICR_RSVDMSTICR_Pos \
    (1UL)                                                      /*!< Position of RSVDMSTICR field.                                        */
#define QSPI_CORE_CORE_MSTICR_RSVDMSTICR_Msk \
    (0x7FFFFFFFUL << QSPI_CORE_CORE_MSTICR_RSVDMSTICR_Pos)     /*!< Bit mask of
                                                                *               RSVDMSTICR field.*/

/* QSPI_CORE_CORE_ICR: Interrupt Clear Register */
#define QSPI_CORE_CORE_ICR_ResetValue \
    (0x00000000UL)                                             /*!< Reset value of ICR register.                                       */

/* ICR @Bit 0 : Clear Interrupts. */
#define QSPI_CORE_CORE_ICR_ICR_Pos (0UL)                       /*!< Position of ICR field.                                               */
#define QSPI_CORE_CORE_ICR_ICR_Msk \
    (0x1UL << QSPI_CORE_CORE_ICR_ICR_Pos)                      /*!< Bit mask of ICR field.                         */

/* RSVDICR @Bits 1..31 : (unspecified) */
#define QSPI_CORE_CORE_ICR_RSVDICR_Pos \
    (1UL)                                                      /*!< Position of RSVDICR field.                                           */
#define QSPI_CORE_CORE_ICR_RSVDICR_Msk \
    (0x7FFFFFFFUL << QSPI_CORE_CORE_ICR_RSVDICR_Pos)           /*!< Bit mask of RSVDICR field.      */

/* QSPI_CORE_CORE_DMACR: DMA Control Register */
#define QSPI_CORE_CORE_DMACR_ResetValue \
    (0x00000000UL)                                             /*!< Reset value of DMACR register.                                   */

/* RDMAE @Bit 0 : Receive DMA Enable. This bit enables/disables the receive FIFO DMA channel. */
#define QSPI_CORE_CORE_DMACR_RDMAE_Pos \
    (0UL)                                                      /*!< Position of RDMAE field.                                             */
#define QSPI_CORE_CORE_DMACR_RDMAE_Msk \
    (0x1UL << QSPI_CORE_CORE_DMACR_RDMAE_Pos)                  /*!< Bit mask of RDMAE field.               */
#define QSPI_CORE_CORE_DMACR_RDMAE_Min \
    (0x0UL)                                                    /*!< Min enumerator value of RDMAE field.                                 */
#define QSPI_CORE_CORE_DMACR_RDMAE_Max \
    (0x1UL)                                                    /*!< Max enumerator value of RDMAE field.                                 */
#define QSPI_CORE_CORE_DMACR_RDMAE_DISABLE \
    (0x0UL)                                                    /*!< Receive DMA disabled                                                 */
#define QSPI_CORE_CORE_DMACR_RDMAE_ENABLED \
    (0x1UL)                                                    /*!< Receive DMA enabled                                                  */

/* TDMAE @Bit 1 : Transmit DMA Enable. This bit enables/disables the transmit FIFO DMA channel. */
#define QSPI_CORE_CORE_DMACR_TDMAE_Pos \
    (1UL)                                                      /*!< Position of TDMAE field.                                             */
#define QSPI_CORE_CORE_DMACR_TDMAE_Msk \
    (0x1UL << QSPI_CORE_CORE_DMACR_TDMAE_Pos)                  /*!< Bit mask of TDMAE field.               */
#define QSPI_CORE_CORE_DMACR_TDMAE_Min \
    (0x0UL)                                                    /*!< Min enumerator value of TDMAE field.                                 */
#define QSPI_CORE_CORE_DMACR_TDMAE_Max \
    (0x1UL)                                                    /*!< Max enumerator value of TDMAE field.                                 */
#define QSPI_CORE_CORE_DMACR_TDMAE_DISABLE \
    (0x0UL)                                                    /*!< Transmit DMA disabled                                                */
#define QSPI_CORE_CORE_DMACR_TDMAE_ENABLED \
    (0x1UL)                                                    /*!< Transmit DMA enabled                                                 */

/* IDMAE @Bit 2 : Internal DMA Enable. This bit should be enabled only when CTRLR0.FRF = 0 (Motorola SPI) and CTRLR0.SPI_FRF >
 *                0. */

#define QSPI_CORE_CORE_DMACR_IDMAE_Pos \
    (2UL)                                                      /*!< Position of IDMAE field.                                             */
#define QSPI_CORE_CORE_DMACR_IDMAE_Msk \
    (0x1UL << QSPI_CORE_CORE_DMACR_IDMAE_Pos)                  /*!< Bit mask of IDMAE field.               */

/* ATW @Bits 3..4 : AXI transfer width for DMA transfers mapped to arsize/awsize. This value must be less than or equal to
 *                  SQSPIC_AXI_DW. Values: */

#define QSPI_CORE_CORE_DMACR_ATW_Pos (3UL)                     /*!< Position of ATW field.                                               */
#define QSPI_CORE_CORE_DMACR_ATW_Msk \
    (0x3UL << QSPI_CORE_CORE_DMACR_ATW_Pos)                    /*!< Bit mask of ATW field.                     */

/* RSVDDMACR5 @Bit 5 : (unspecified) */
#define QSPI_CORE_CORE_DMACR_RSVDDMACR5_Pos \
    (5UL)                                                      /*!< Position of RSVDDMACR5 field.                                        */
#define QSPI_CORE_CORE_DMACR_RSVDDMACR5_Msk \
    (0x1UL << QSPI_CORE_CORE_DMACR_RSVDDMACR5_Pos)             /*!< Bit mask of RSVDDMACR5 field.*/

/* AINC @Bit 6 : Address Increment. Indicates whether to increment the AXI address on every transfer. */
#define QSPI_CORE_CORE_DMACR_AINC_Pos (6UL)                    /*!< Position of AINC field.                                              */
#define QSPI_CORE_CORE_DMACR_AINC_Msk \
    (0x1UL << QSPI_CORE_CORE_DMACR_AINC_Pos)                   /*!< Bit mask of AINC field.                  */

/* RSVDDMACR7 @Bit 7 : (unspecified) */
#define QSPI_CORE_CORE_DMACR_RSVDDMACR7_Pos \
    (7UL)                                                      /*!< Position of RSVDDMACR7 field.                                        */
#define QSPI_CORE_CORE_DMACR_RSVDDMACR7_Msk \
    (0x1UL << QSPI_CORE_CORE_DMACR_RSVDDMACR7_Pos)             /*!< Bit mask of RSVDDMACR7 field.*/

/* ACACHE @Bits 8..11 : AXI arcache/awcache signal value. */
#define QSPI_CORE_CORE_DMACR_ACACHE_Pos \
    (8UL)                                                      /*!< Position of ACACHE field.                                            */
#define QSPI_CORE_CORE_DMACR_ACACHE_Msk \
    (0xFUL << QSPI_CORE_CORE_DMACR_ACACHE_Pos)                 /*!< Bit mask of ACACHE field.            */

/* APROT @Bits 12..14 : AXI arprot/awprot signal value. */
#define QSPI_CORE_CORE_DMACR_APROT_Pos \
    (12UL)                                                     /*!< Position of APROT field.                                             */
#define QSPI_CORE_CORE_DMACR_APROT_Msk \
    (0x7UL << QSPI_CORE_CORE_DMACR_APROT_Pos)                  /*!< Bit mask of APROT field.               */

/* AID @Bits 15..20 : AXI awid/arid signal value. */
#define QSPI_CORE_CORE_DMACR_AID_Pos (15UL)                    /*!< Position of AID field.                                               */
#define QSPI_CORE_CORE_DMACR_AID_Msk \
    (0x3FUL << QSPI_CORE_CORE_DMACR_AID_Pos)                   /*!< Bit mask of AID field.                    */

/* RSVDDMACR @Bits 21..31 : (unspecified) */
#define QSPI_CORE_CORE_DMACR_RSVDDMACR_Pos \
    (21UL)                                                     /*!< Position of RSVDDMACR field.                                         */
#define QSPI_CORE_CORE_DMACR_RSVDDMACR_Msk \
    (0x7FFUL << QSPI_CORE_CORE_DMACR_RSVDDMACR_Pos)            /*!< Bit mask of RSVDDMACR field. */

/* QSPI_CORE_CORE_DMATDLR: DMA Transmit Data Level */
#define QSPI_CORE_CORE_DMATDLR_ResetValue \
    (0x00000000UL)                                             /*!< Reset value of DMATDLR register.                               */

/* DMATDL @Bits 0..3 : Transmit Data Level. This bit field controls the level at which a DMA request is made by the transmit
 *                     logic. It is equal to the watermark level; that is, the dma_tx_req signal is generated when the number of
 *                     valid data entries in the transmit FIFO is equal to or below this field value, and TDMAE = 1. */

#define QSPI_CORE_CORE_DMATDLR_DMATDL_Pos \
    (0UL)                                                      /*!< Position of DMATDL field.                                            */
#define QSPI_CORE_CORE_DMATDLR_DMATDL_Msk \
    (0xFUL << QSPI_CORE_CORE_DMATDLR_DMATDL_Pos)               /*!< Bit mask of DMATDL field.        */

/* RSVDDMATDLR @Bits 4..31 : (unspecified) */
#define QSPI_CORE_CORE_DMATDLR_RSVDDMATDLR_Pos \
    (4UL)                                                      /*!< Position of RSVDDMATDLR field.                                     */
#define QSPI_CORE_CORE_DMATDLR_RSVDDMATDLR_Msk \
    (0xFFFFFFFUL << QSPI_CORE_CORE_DMATDLR_RSVDDMATDLR_Pos)    /*!< Bit mask of
                                                                *              RSVDDMATDLR field.*/

/* QSPI_CORE_CORE_DMARDLR: DMA Receive Data Level */
#define QSPI_CORE_CORE_DMARDLR_ResetValue \
    (0x00000000UL)                                             /*!< Reset value of DMARDLR register.                               */

/* DMARDL @Bits 0..3 : Receive Data Level. This bit field controls the level at which a DMA request is made by the receive
 *                     logic. The watermark level = DMARDL+1; that is, dma_rx_req is generated when the number of valid data
 *                     entries in the receive FIFO is equal to or above this field value + 1, and RDMAE=1. */

#define QSPI_CORE_CORE_DMARDLR_DMARDL_Pos \
    (0UL)                                                      /*!< Position of DMARDL field.                                            */
#define QSPI_CORE_CORE_DMARDLR_DMARDL_Msk \
    (0xFUL << QSPI_CORE_CORE_DMARDLR_DMARDL_Pos)               /*!< Bit mask of DMARDL field.        */

/* RSVDDMARDLR @Bits 4..31 : (unspecified) */
#define QSPI_CORE_CORE_DMARDLR_RSVDDMARDLR_Pos \
    (4UL)                                                      /*!< Position of RSVDDMARDLR field.                                     */
#define QSPI_CORE_CORE_DMARDLR_RSVDDMARDLR_Msk \
    (0xFFFFFFFUL << QSPI_CORE_CORE_DMARDLR_RSVDDMARDLR_Pos)    /*!< Bit mask of
                                                                *              RSVDDMARDLR field.*/

/* QSPI_CORE_CORE_IDR: Identification Register */
#define QSPI_CORE_CORE_IDR_ResetValue \
    (0x51535049UL)                                             /*!< Reset value of IDR register.                                       */

/* IDCODE @Bits 0..31 : Identification code. The register contains the identification code of the peripheral, which is written
 *                      into the register at configuration time using CoreConsultant. */

#define QSPI_CORE_CORE_IDR_IDCODE_Pos (0UL)                    /*!< Position of IDCODE field.                                            */
#define QSPI_CORE_CORE_IDR_IDCODE_Msk \
    (0xFFFFFFFFUL << QSPI_CORE_CORE_IDR_IDCODE_Pos)            /*!< Bit mask of IDCODE field.         */

/* QSPI_CORE_CORE_SQSPICVERSIONID: QSPI CORE component version */
#define QSPI_CORE_CORE_SQSPICVERSIONID_ResetValue \
    (0x00000000UL)                                             /*!< Reset value of SQSPICVERSIONID register.                   */

/* SQSPICCOMPVERSION @Bits 0..31 : Contains the hex representation of the sQSPI version. */

#define QSPI_CORE_CORE_SQSPICVERSIONID_SQSPICCOMPVERSION_Pos \
    (0UL)                                                      /*!< Position of SQSPICCOMPVERSION field.                       */
#define QSPI_CORE_CORE_SQSPICVERSIONID_SQSPICCOMPVERSION_Msk \
    (0xFFFFFFFFUL << QSPI_CORE_CORE_SQSPICVERSIONID_SQSPICCOMPVERSION_Pos)
/*!< Bit mask of SQSPICCOMPVERSION field.*/

/* QSPI_CORE_CORE_DR: QSPI CORE Data Register */
#define QSPI_CORE_CORE_DR_MaxCount (36UL)            /*!< Max size of DR[36] array.                                            */
#define QSPI_CORE_CORE_DR_MaxIndex (35UL)            /*!< Max index of DR[36] array.                                           */
#define QSPI_CORE_CORE_DR_MinIndex (0UL)             /*!< Min index of DR[36] array.                                           */
#define QSPI_CORE_CORE_DR_ResetValue \
    (0x00000000UL)                                   /*!< Reset value of DR[36] register.                                     */

/* DR @Bits 0..31 : Data Register. When writing to this register, you must right-justify the data. Read data are automatically
 *                  right-justified. */

#define QSPI_CORE_CORE_DR_DR_Pos (0UL)               /*!< Position of DR field.                                                */
#define QSPI_CORE_CORE_DR_DR_Msk \
    (0xFFFFFFFFUL << QSPI_CORE_CORE_DR_DR_Pos)       /*!< Bit mask of DR field.                       */

/* QSPI_CORE_CORE_RXSAMPLEDELAY: RX Sample Delay Register */
#define QSPI_CORE_CORE_RXSAMPLEDELAY_ResetValue \
    (0x00000000UL)                                   /*!< Reset value of RXSAMPLEDELAY register.                   */

/* RSD @Bits 0..7 : Receive Data (rxd) Sample Delay. This register is used to delay the sample of the rxd input port. Each value
 */

#define QSPI_CORE_CORE_RXSAMPLEDELAY_RSD_Pos \
    (0UL)                                            /*!< Position of RSD field.                                               */
#define QSPI_CORE_CORE_RXSAMPLEDELAY_RSD_Msk \
    (0xFFUL << QSPI_CORE_CORE_RXSAMPLEDELAY_RSD_Pos) /*!< Bit mask of RSD field.    */

/* RSVD0RXSAMPLEDELAY @Bits 8..15 : (unspecified) */
#define QSPI_CORE_CORE_RXSAMPLEDELAY_RSVD0RXSAMPLEDELAY_Pos \
    (8UL)                                            /*!< Position of RSVD0RXSAMPLEDELAY field.                 */
#define QSPI_CORE_CORE_RXSAMPLEDELAY_RSVD0RXSAMPLEDELAY_Msk \
    (0xFFUL << QSPI_CORE_CORE_RXSAMPLEDELAY_RSVD0RXSAMPLEDELAY_Pos)
/*!< Bit mask of RSVD0RXSAMPLEDELAY field.*/

/* SE @Bit 16 : Receive Data (rxd) Sampling Edge. This register is used to decide the sampling edge for RXD signal with sqspiclk.
 *              Then this bit is set to 1 then negative edge of sqspiclk will be used to sample the incoming data, otherwise
 *              positive edge will be used for sampling. */

#define QSPI_CORE_CORE_RXSAMPLEDELAY_SE_Pos \
    (16UL)                                         /*!< Position of SE field.                                                */
#define QSPI_CORE_CORE_RXSAMPLEDELAY_SE_Msk \
    (0x1UL << QSPI_CORE_CORE_RXSAMPLEDELAY_SE_Pos) /*!< Bit mask of SE field.        */

/* RSVD1RXSAMPLEDELAY @Bits 17..31 : (unspecified) */
#define QSPI_CORE_CORE_RXSAMPLEDELAY_RSVD1RXSAMPLEDELAY_Pos \
    (17UL)                                         /*!< Position of RSVD1RXSAMPLEDELAY field.                */
#define QSPI_CORE_CORE_RXSAMPLEDELAY_RSVD1RXSAMPLEDELAY_Msk \
    (0x7FFFUL << QSPI_CORE_CORE_RXSAMPLEDELAY_RSVD1RXSAMPLEDELAY_Pos)
/*!< Bit mask of RSVD1RXSAMPLEDELAY field.*/

/* QSPI_CORE_CORE_SPICTRLR0: SPI_CTRLR0 - SPI Control Register */
#define QSPI_CORE_CORE_SPICTRLR0_ResetValue \
    (0x40000200UL)                                              /*!< Reset value of SPICTRLR0 register.                           */

/* TRANSTYPE @Bits 0..1 : Address and instruction transfer format. */
#define QSPI_CORE_CORE_SPICTRLR0_TRANSTYPE_Pos \
    (0UL)                                                       /*!< Position of TRANSTYPE field.                                       */
#define QSPI_CORE_CORE_SPICTRLR0_TRANSTYPE_Msk \
    (0x3UL << QSPI_CORE_CORE_SPICTRLR0_TRANSTYPE_Pos)           /*!< Bit mask of TRANSTYPE
                                                                 *                    field.*/
#define QSPI_CORE_CORE_SPICTRLR0_TRANSTYPE_Min \
    (0x0UL)                                                     /*!< Min enumerator value of TRANSTYPE field.                         */
#define QSPI_CORE_CORE_SPICTRLR0_TRANSTYPE_Max \
    (0x3UL)                                                     /*!< Max enumerator value of TRANSTYPE field.                         */
#define QSPI_CORE_CORE_SPICTRLR0_TRANSTYPE_TT0 \
    (0x0UL)                                                     /*!< Instruction and Address will be sent in Standard SPI Mode.       */
#define QSPI_CORE_CORE_SPICTRLR0_TRANSTYPE_TT1 \
    (0x1UL)                                                     /*!< Instruction will be sent in Standard SPI Mode and Address will be
                                                                 *                                                sent in the mode specified by CTRLR0.SPI_FRF.*/
#define QSPI_CORE_CORE_SPICTRLR0_TRANSTYPE_TT2 \
    (0x2UL)                                                     /*!< Both Instruction and Address will be sent in the mode specified by
                                                                 *                                                SPI_FRF.*/
#define QSPI_CORE_CORE_SPICTRLR0_TRANSTYPE_TT3 \
    (0x3UL)                                                     /*!< Dual Octal mode, the address and instruction are transferred in
                                                                 *                                                octal mode and data is transferred on 16 data lines.*/

/* ADDRL @Bits 2..5 : This bit defines Length of Address to be transmitted. Only after this much bits are programmed in to the
 *                    FIFO the transfer can begin. */

#define QSPI_CORE_CORE_SPICTRLR0_ADDRL_Pos \
    (2UL)                                                       /*!< Position of ADDRL field.                                             */
#define QSPI_CORE_CORE_SPICTRLR0_ADDRL_Msk \
    (0xFUL << QSPI_CORE_CORE_SPICTRLR0_ADDRL_Pos)               /*!< Bit mask of ADDRL field.       */
#define QSPI_CORE_CORE_SPICTRLR0_ADDRL_Min \
    (0x0UL)                                                     /*!< Min enumerator value of ADDRL field.                                 */
#define QSPI_CORE_CORE_SPICTRLR0_ADDRL_Max \
    (0xFUL)                                                     /*!< Max enumerator value of ADDRL field.                                 */
#define QSPI_CORE_CORE_SPICTRLR0_ADDRL_ADDRL0 \
    (0x0UL)                                                     /*!< No Address                                                        */
#define QSPI_CORE_CORE_SPICTRLR0_ADDRL_ADDRL4 \
    (0x1UL)                                                     /*!< 4 bit Address length                                              */
#define QSPI_CORE_CORE_SPICTRLR0_ADDRL_ADDRL8 \
    (0x2UL)                                                     /*!< 8 bit Address length                                              */
#define QSPI_CORE_CORE_SPICTRLR0_ADDRL_ADDRL12 \
    (0x3UL)                                                     /*!< 12 bit Address length                                            */
#define QSPI_CORE_CORE_SPICTRLR0_ADDRL_ADDRL16 \
    (0x4UL)                                                     /*!< 16 bit Address length                                            */
#define QSPI_CORE_CORE_SPICTRLR0_ADDRL_ADDRL20 \
    (0x5UL)                                                     /*!< 20 bit Address length                                            */
#define QSPI_CORE_CORE_SPICTRLR0_ADDRL_ADDRL24 \
    (0x6UL)                                                     /*!< 24 bit Address length                                            */
#define QSPI_CORE_CORE_SPICTRLR0_ADDRL_ADDRL28 \
    (0x7UL)                                                     /*!< 28 bit Address length                                            */
#define QSPI_CORE_CORE_SPICTRLR0_ADDRL_ADDRL32 \
    (0x8UL)                                                     /*!< 32 bit Address length                                            */
#define QSPI_CORE_CORE_SPICTRLR0_ADDRL_ADDRL36 \
    (0x9UL)                                                     /*!< 36 bit Address length                                            */
#define QSPI_CORE_CORE_SPICTRLR0_ADDRL_ADDRL40 \
    (0xAUL)                                                     /*!< 40 bit Address length                                            */
#define QSPI_CORE_CORE_SPICTRLR0_ADDRL_ADDRL44 \
    (0xBUL)                                                     /*!< 44 bit Address length                                            */
#define QSPI_CORE_CORE_SPICTRLR0_ADDRL_ADDRL48 \
    (0xCUL)                                                     /*!< 48 bit Address length                                            */
#define QSPI_CORE_CORE_SPICTRLR0_ADDRL_ADDRL52 \
    (0xDUL)                                                     /*!< 52 bit Address length                                            */
#define QSPI_CORE_CORE_SPICTRLR0_ADDRL_ADDRL56 \
    (0xEUL)                                                     /*!< 56 bit Address length                                            */
#define QSPI_CORE_CORE_SPICTRLR0_ADDRL_ADDRL60 \
    (0xFUL)                                                     /*!< 60 bit Address length                                            */

/* RSVDSPICTRLR06 @Bit 6 : (unspecified) */
#define QSPI_CORE_CORE_SPICTRLR0_RSVDSPICTRLR06_Pos \
    (6UL)                                                       /*!< Position of RSVDSPICTRLR06 field.                             */
#define QSPI_CORE_CORE_SPICTRLR0_RSVDSPICTRLR06_Msk \
    (0x1UL << QSPI_CORE_CORE_SPICTRLR0_RSVDSPICTRLR06_Pos)      /*!< Bit mask of
                                                                 *               RSVDSPICTRLR06 field.*/

/* XIPMDBITEN @Bit 7 : Mode bits enable in XIP mode. If this bit is set to 1, then in XIP mode of operation QSPI CORE will
 *                     insert mode bits after the address phase. These bits are set in register XIP_MODE_BITS register. */

#define QSPI_CORE_CORE_SPICTRLR0_XIPMDBITEN_Pos \
    (7UL)                                                       /*!< Position of XIPMDBITEN field.                                     */
#define QSPI_CORE_CORE_SPICTRLR0_XIPMDBITEN_Msk \
    (0x1UL << QSPI_CORE_CORE_SPICTRLR0_XIPMDBITEN_Pos)          /*!< Bit mask of XIPMDBITEN
                                                                 *                   field.*/

/* INSTL @Bits 8..9 : Enhanced SPI mode instruction length in bits. */
#define QSPI_CORE_CORE_SPICTRLR0_INSTL_Pos \
    (8UL)                                                       /*!< Position of INSTL field.                                             */
#define QSPI_CORE_CORE_SPICTRLR0_INSTL_Msk \
    (0x3UL << QSPI_CORE_CORE_SPICTRLR0_INSTL_Pos)               /*!< Bit mask of INSTL field.       */
#define QSPI_CORE_CORE_SPICTRLR0_INSTL_Min \
    (0x0UL)                                                     /*!< Min enumerator value of INSTL field.                                 */
#define QSPI_CORE_CORE_SPICTRLR0_INSTL_Max \
    (0x3UL)                                                     /*!< Max enumerator value of INSTL field.                                 */
#define QSPI_CORE_CORE_SPICTRLR0_INSTL_INSTL0 \
    (0x0UL)                                                     /*!< No Instruction                                                    */
#define QSPI_CORE_CORE_SPICTRLR0_INSTL_INSTL4 \
    (0x1UL)                                                     /*!< 4 bit Instruction length                                          */
#define QSPI_CORE_CORE_SPICTRLR0_INSTL_INSTL8 \
    (0x2UL)                                                     /*!< 8 bit Instruction length                                          */
#define QSPI_CORE_CORE_SPICTRLR0_INSTL_INSTL16 \
    (0x3UL)                                                     /*!< 16 bit Instruction length                                        */

/* RSVDSPICTRLR010 @Bit 10 : (unspecified) */
#define QSPI_CORE_CORE_SPICTRLR0_RSVDSPICTRLR010_Pos \
    (10UL)                                                      /*!< Position of RSVDSPICTRLR010 field.                          */
#define QSPI_CORE_CORE_SPICTRLR0_RSVDSPICTRLR010_Msk \
    (0x1UL << QSPI_CORE_CORE_SPICTRLR0_RSVDSPICTRLR010_Pos)     /*!< Bit mask of
                                                                 *              RSVDSPICTRLR010 field.*/

/* WAITCYCLES @Bits 11..15 : Wait cycles in Enhanced SPI mode between control frames transmit and data reception. Specified as
 *                           number of SPI clock cycles. */

#define QSPI_CORE_CORE_SPICTRLR0_WAITCYCLES_Pos \
    (11UL)                                                      /*!< Position of WAITCYCLES field.                                    */
#define QSPI_CORE_CORE_SPICTRLR0_WAITCYCLES_Msk \
    (0x1FUL << QSPI_CORE_CORE_SPICTRLR0_WAITCYCLES_Pos)         /*!< Bit mask of
                                                                 *                  WAITCYCLES field.*/

/* SPIDDREN @Bit 16 : SPI DDR Enable bit. This will enable Dual-data rate transfers in Enhanced SPI frame formats of SPI. */
#define QSPI_CORE_CORE_SPICTRLR0_SPIDDREN_Pos \
    (16UL)                                                      /*!< Position of SPIDDREN field.                                        */
#define QSPI_CORE_CORE_SPICTRLR0_SPIDDREN_Msk \
    (0x1UL << QSPI_CORE_CORE_SPICTRLR0_SPIDDREN_Pos)            /*!< Bit mask of SPIDDREN
                                                                 *                     field.*/

/* INSTDDREN @Bit 17 : Instruction DDR Enable bit. This will enable Dual-data rate transfer for Instruction phase. */
#define QSPI_CORE_CORE_SPICTRLR0_INSTDDREN_Pos \
    (17UL)                                                      /*!< Position of INSTDDREN field.                                      */
#define QSPI_CORE_CORE_SPICTRLR0_INSTDDREN_Msk \
    (0x1UL << QSPI_CORE_CORE_SPICTRLR0_INSTDDREN_Pos)           /*!< Bit mask of INSTDDREN
                                                                 *                    field.*/

/* SPIRXDSEN @Bit 18 : Read data strobe enable bit. Once this bit is set to 1 QSPI CORE will use Read data strobe (rxds) to
 *                     capture read data. */

#define QSPI_CORE_CORE_SPICTRLR0_SPIRXDSEN_Pos \
    (18UL)                                                      /*!< Position of SPIRXDSEN field.                                      */
#define QSPI_CORE_CORE_SPICTRLR0_SPIRXDSEN_Msk \
    (0x1UL << QSPI_CORE_CORE_SPICTRLR0_SPIRXDSEN_Pos)           /*!< Bit mask of SPIRXDSEN
                                                                 *                    field.*/

/* XIPDFSHC @Bit 19 : Fix DFS for XIP transfers. If this bit is set to 1 then data frame size for XIP transfers will be fixed to
 *                    the programmed value in CTRLR0.DFS. The number of data frames to fetch will be determined by HSIZE and
 *                    HBURST signals. If this bit is set to 0 then data frame size and number of data frames to fetch will be
 *                    determined by HSIZE and HBURST signals */

#define QSPI_CORE_CORE_SPICTRLR0_XIPDFSHC_Pos \
    (19UL)                                                      /*!< Position of XIPDFSHC field.                                        */
#define QSPI_CORE_CORE_SPICTRLR0_XIPDFSHC_Msk \
    (0x1UL << QSPI_CORE_CORE_SPICTRLR0_XIPDFSHC_Pos)            /*!< Bit mask of XIPDFSHC
                                                                 *                     field.*/

/* XIPINSTEN @Bit 20 : XIP instruction enable bit. If this bit is set to 1 then XIP transfers will also have instruction phase.
 *                     The instruction op-codes will be chosen from XIP_INCR_INST or XIP_WRAP_INST registers bases on AHB
 *                     transfer type. */

#define QSPI_CORE_CORE_SPICTRLR0_XIPINSTEN_Pos \
    (20UL)                                                      /*!< Position of XIPINSTEN field.                                      */
#define QSPI_CORE_CORE_SPICTRLR0_XIPINSTEN_Msk \
    (0x1UL << QSPI_CORE_CORE_SPICTRLR0_XIPINSTEN_Pos)           /*!< Bit mask of XIPINSTEN
                                                                 *                    field.*/

/* SQSPICXIPCONTXFEREN @Bit 21 : Enable continuous transfer in XIP mode. If this bit is set to 1 then continuous transfer mode in
 *                             XIP will be enabled, in this mode QSPI CORE will keep target selected until a non-XIP transfer is
 *                             detected on the AHB interface. */

#define QSPI_CORE_CORE_SPICTRLR0_SQSPICXIPCONTXFEREN_Pos \
    (21UL)                                                      /*!< Position of SQSPICXIPCONTXFEREN field.                      */
#define QSPI_CORE_CORE_SPICTRLR0_SQSPICXIPCONTXFEREN_Msk \
    (0x1UL << QSPI_CORE_CORE_SPICTRLR0_SQSPICXIPCONTXFEREN_Pos) /*!< Bit mask
                                                                 *          of SQSPICXIPCONTXFEREN field.*/

/* RSVDSPICTRLR022 @Bit 22 : (unspecified) */
#define QSPI_CORE_CORE_SPICTRLR0_RSVDSPICTRLR022_Pos \
    (22UL)                                                      /*!< Position of RSVDSPICTRLR022 field.                          */
#define QSPI_CORE_CORE_SPICTRLR0_RSVDSPICTRLR022_Msk \
    (0x1UL << QSPI_CORE_CORE_SPICTRLR0_RSVDSPICTRLR022_Pos)     /*!< Bit mask of
                                                                 *              RSVDSPICTRLR022 field.*/

/* RXDSVLEN @Bit 23 : RXDS variable latency enable bit. */
#define QSPI_CORE_CORE_SPICTRLR0_RXDSVLEN_Pos \
    (23UL)                                                      /*!< Position of RXDSVLEN field.                                        */
#define QSPI_CORE_CORE_SPICTRLR0_RXDSVLEN_Msk \
    (0x1UL << QSPI_CORE_CORE_SPICTRLR0_RXDSVLEN_Pos)            /*!< Bit mask of RXDSVLEN
                                                                 *                     field.*/

/* SPIDMEN @Bit 24 : SPI data mask enable bit. */
#define QSPI_CORE_CORE_SPICTRLR0_SPIDMEN_Pos \
    (24UL)                                                      /*!< Position of SPIDMEN field.                                          */
#define QSPI_CORE_CORE_SPICTRLR0_SPIDMEN_Msk \
    (0x1UL << QSPI_CORE_CORE_SPICTRLR0_SPIDMEN_Pos)             /*!< Bit mask of SPIDMEN field. */

/* SPIRXDSSIGEN @Bit 25 : Enable rxds signaling during address and command phase of Hyperbus transfer. */
#define QSPI_CORE_CORE_SPICTRLR0_SPIRXDSSIGEN_Pos \
    (25UL)                                                      /*!< Position of SPIRXDSSIGEN field.                                */
#define QSPI_CORE_CORE_SPICTRLR0_SPIRXDSSIGEN_Msk \
    (0x1UL << QSPI_CORE_CORE_SPICTRLR0_SPIRXDSSIGEN_Pos)        /*!< Bit mask of
                                                                 *                 SPIRXDSSIGEN field.*/

/* XIPMBL @Bits 26..27 : XIP Mode bits length. Sets the length of mode bits in XIP mode of operation. These bits are valid only
 *                       when SPI_CTRLR0.XIP_MD_BIT_EN is set to 1. */

#define QSPI_CORE_CORE_SPICTRLR0_XIPMBL_Pos \
    (26UL)                                                      /*!< Position of XIPMBL field.                                            */
#define QSPI_CORE_CORE_SPICTRLR0_XIPMBL_Msk \
    (0x3UL << QSPI_CORE_CORE_SPICTRLR0_XIPMBL_Pos)              /*!< Bit mask of XIPMBL field.    */
#define QSPI_CORE_CORE_SPICTRLR0_XIPMBL_Min \
    (0x0UL)                                                     /*!< Min enumerator value of XIPMBL field.                               */
#define QSPI_CORE_CORE_SPICTRLR0_XIPMBL_Max \
    (0x3UL)                                                     /*!< Max enumerator value of XIPMBL field.                               */
#define QSPI_CORE_CORE_SPICTRLR0_XIPMBL_MBL2 \
    (0x0UL)                                                     /*!< Mode bits length equal to 2                                        */
#define QSPI_CORE_CORE_SPICTRLR0_XIPMBL_MBL4 \
    (0x1UL)                                                     /*!< Mode bits length equal to 4                                        */
#define QSPI_CORE_CORE_SPICTRLR0_XIPMBL_MBL8 \
    (0x2UL)                                                     /*!< Mode bits length equal to 8                                        */
#define QSPI_CORE_CORE_SPICTRLR0_XIPMBL_MBL16 \
    (0x3UL)                                                     /*!< Mode bits length equal to 16                                      */

/* RSVDSPICTRLR028 @Bit 28 : (unspecified) */
#define QSPI_CORE_CORE_SPICTRLR0_RSVDSPICTRLR028_Pos \
    (28UL)                                                      /*!< Position of RSVDSPICTRLR028 field.                          */
#define QSPI_CORE_CORE_SPICTRLR0_RSVDSPICTRLR028_Msk \
    (0x1UL << QSPI_CORE_CORE_SPICTRLR0_RSVDSPICTRLR028_Pos)     /*!< Bit mask of
                                                                 *              RSVDSPICTRLR028 field.*/

/* XIPPREFETCHEN @Bit 29 : Enables XIP pre-fetch functionality in QSPI CORE. */
#define QSPI_CORE_CORE_SPICTRLR0_XIPPREFETCHEN_Pos \
    (29UL)                                                      /*!< Position of XIPPREFETCHEN field.                              */
#define QSPI_CORE_CORE_SPICTRLR0_XIPPREFETCHEN_Msk \
    (0x1UL << QSPI_CORE_CORE_SPICTRLR0_XIPPREFETCHEN_Pos)       /*!< Bit mask of
                                                                 *                XIPPREFETCHEN field.*/

/* CLKSTRETCHEN @Bit 30 : Enables clock stretching capability in SPI transfers. */
#define QSPI_CORE_CORE_SPICTRLR0_CLKSTRETCHEN_Pos \
    (30UL)                                                      /*!< Position of CLKSTRETCHEN field.                                */
#define QSPI_CORE_CORE_SPICTRLR0_CLKSTRETCHEN_Msk \
    (0x1UL << QSPI_CORE_CORE_SPICTRLR0_CLKSTRETCHEN_Pos)        /*!< Bit mask of
                                                                 *                 CLKSTRETCHEN field.*/

/* RSVDSPICTRLR0 @Bit 31 : (unspecified) */
#define QSPI_CORE_CORE_SPICTRLR0_RSVDSPICTRLR0_Pos \
    (31UL)                                                      /*!< Position of RSVDSPICTRLR0 field.                              */
#define QSPI_CORE_CORE_SPICTRLR0_RSVDSPICTRLR0_Msk \
    (0x1UL << QSPI_CORE_CORE_SPICTRLR0_RSVDSPICTRLR0_Pos)       /*!< Bit mask of
                                                                 *                RSVDSPICTRLR0 field.*/

/* QSPI_CORE_CORE_SPICTRLR1: SPI Control 1 register */
#define QSPI_CORE_CORE_SPICTRLR1_ResetValue \
    (0x00000000UL)                                              /*!< Reset value of SPICTRLR1 register.                           */

/* DYNWS @Bits 0..2 : SPI Dynamic Wait states field. */
#define QSPI_CORE_CORE_SPICTRLR1_DYNWS_Pos \
    (0UL)                                                       /*!< Position of DYNWS field.                                             */
#define QSPI_CORE_CORE_SPICTRLR1_DYNWS_Msk \
    (0x7UL << QSPI_CORE_CORE_SPICTRLR1_DYNWS_Pos)               /*!< Bit mask of DYNWS field.       */

/* RSVDSPICTRLR137 @Bits 3..7 : (unspecified) */
#define QSPI_CORE_CORE_SPICTRLR1_RSVDSPICTRLR137_Pos \
    (3UL)                                                       /*!< Position of RSVDSPICTRLR137 field.                           */
#define QSPI_CORE_CORE_SPICTRLR1_RSVDSPICTRLR137_Msk \
    (0x1FUL << QSPI_CORE_CORE_SPICTRLR1_RSVDSPICTRLR137_Pos)    /*!< Bit mask of
                                                                 *             RSVDSPICTRLR137 field.*/

/* MAXWS @Bits 8..11 : Maximum wait cycles allowed per transaction. */
#define QSPI_CORE_CORE_SPICTRLR1_MAXWS_Pos \
    (8UL)                                                       /*!< Position of MAXWS field.                                             */
#define QSPI_CORE_CORE_SPICTRLR1_MAXWS_Msk \
    (0xFUL << QSPI_CORE_CORE_SPICTRLR1_MAXWS_Pos)               /*!< Bit mask of MAXWS field.       */

/* RSVDSPICTRLR11215 @Bits 12..15 : (unspecified) */
#define QSPI_CORE_CORE_SPICTRLR1_RSVDSPICTRLR11215_Pos \
    (12UL)                                                      /*!< Position of RSVDSPICTRLR11215 field.                      */
#define QSPI_CORE_CORE_SPICTRLR1_RSVDSPICTRLR11215_Msk \
    (0xFUL << QSPI_CORE_CORE_SPICTRLR1_RSVDSPICTRLR11215_Pos)   /*!< Bit mask
                                                                 *            of RSVDSPICTRLR11215 field.*/

/* CSMINHIGH @Bits 16..19 : Chip-Select Minimum HIGH period. */
#define QSPI_CORE_CORE_SPICTRLR1_CSMINHIGH_Pos \
    (16UL)                                                      /*!< Position of CSMINHIGH field.                                      */
#define QSPI_CORE_CORE_SPICTRLR1_CSMINHIGH_Msk \
    (0xFUL << QSPI_CORE_CORE_SPICTRLR1_CSMINHIGH_Pos)           /*!< Bit mask of CSMINHIGH
                                                                 *                    field.*/

/* RSVDSPICTRLR12031 @Bits 20..31 : (unspecified) */
#define QSPI_CORE_CORE_SPICTRLR1_RSVDSPICTRLR12031_Pos \
    (20UL)                                                      /*!< Position of RSVDSPICTRLR12031 field.                      */
#define QSPI_CORE_CORE_SPICTRLR1_RSVDSPICTRLR12031_Msk \
    (0xFFFUL << QSPI_CORE_CORE_SPICTRLR1_RSVDSPICTRLR12031_Pos) /*!< Bit
                                                                 *          mask of RSVDSPICTRLR12031 field.*/

/* QSPI_CORE_CORE_SPITECR: SPI Transmit Error Interrupt Clear Register */
#define QSPI_CORE_CORE_SPITECR_ResetValue \
    (0x00000000UL)                                              /*!< Reset value of SPITECR register.                               */

/* SPITECR @Bit 0 : Clear SPI Transmit Error interrupt. */
#define QSPI_CORE_CORE_SPITECR_SPITECR_Pos \
    (0UL)                                                       /*!< Position of SPITECR field.                                           */
#define QSPI_CORE_CORE_SPITECR_SPITECR_Msk \
    (0x1UL << QSPI_CORE_CORE_SPITECR_SPITECR_Pos)               /*!< Bit mask of SPITECR field.     */

/* RSVDSPITECR @Bits 1..31 : (unspecified) */
#define QSPI_CORE_CORE_SPITECR_RSVDSPITECR_Pos \
    (1UL)                                                       /*!< Position of RSVDSPITECR field.                                     */
#define QSPI_CORE_CORE_SPITECR_RSVDSPITECR_Msk \
    (0x7FFFFFFFUL << QSPI_CORE_CORE_SPITECR_RSVDSPITECR_Pos)    /*!< Bit mask of
                                                                 *             RSVDSPITECR field.*/

/* ==================================================== Struct QSPI_CORE ===================================================== */
/**
 * @brief CORE [QSPI_CORE] (unspecified)
 */
typedef struct
{
    __IOM NRF_QSPI_CORE_CORE_Type CORE; /*!<                (unspecified)                                         */
} NRF_QSPI_CORE_Type;                   /*!<                                                                      */

/* ================================================== Struct QSPI_SPSYNC ================================================== */
/**
 * @brief SPSYNC [QSPI_SPSYNC] Registers used to acknowledge API function calls
 */
typedef struct
{
    __IOM uint32_t AUX[4];                                                  /*!< (@ 0x00000000) Auxiliary registers for XSB macro call handshaking    */
} NRF_QSPI_SPSYNC_Type;                                                     /*!< Size = 16 (0x010)                                                    */

/* QSPI_SPSYNC_AUX: Auxiliary registers for XSB macro call handshaking */
  #define QSPI_SPSYNC_AUX_MaxCount   (4UL)                                  /*!< Max size of AUX[4] array.                                            */
  #define QSPI_SPSYNC_AUX_MaxIndex   (3UL)                                  /*!< Max index of AUX[4] array.                                           */
  #define QSPI_SPSYNC_AUX_MinIndex   (0UL)                                  /*!< Min index of AUX[4] array.                                           */
  #define QSPI_SPSYNC_AUX_ResetValue (0x00000000UL)                         /*!< Reset value of AUX[4] register.                                    */

/* AUX @Bits 0..31 : Auxiliary register */
  #define QSPI_SPSYNC_AUX_AUX_Pos (0UL)                                     /*!< Position of AUX field.                                               */
  #define QSPI_SPSYNC_AUX_AUX_Msk (0xFFFFFFFFUL << QSPI_SPSYNC_AUX_AUX_Pos) /*!< Bit mask of AUX field.                  */

/* ======================================================= Struct QSPI ======================================================= */
/**
 * @brief Quad serial peripheral interface
 */
typedef struct                                                              /*!< QSPI Structure                                                       */
{
    __OM uint32_t              TASKS_START;                                 /*!<                Start operation.                                      */
    __OM uint32_t              TASKS_RESET;                                 /*!<                Reset the QSPI                                        */
    __IOM uint32_t             EVENTS_CORE;                                 /*!<                Interrupt from the QSPI core                          */
    __IOM NRF_QSPI_EVENTS_DMA_Type
                               EVENTS_DMA;                                  /*!<                Peripheral events.                                    */
    __IOM uint32_t             EVENTS_IDLE;                                 /*!<                This event signifies that the QSPI core is no longer
                                                                             *                                busy*/
    __IOM uint32_t             SHORTS;                                      /*!<                Shortcuts between local events and tasks              */
    __IOM uint32_t             INTEN;                                       /*!<                Enable or disable interrupt                           */
    __IOM uint32_t             INTENSET;                                    /*!<                Enable interrupt                                      */
    __IOM uint32_t             INTENCLR;                                    /*!<                Disable interrupt                                     */
    __IM uint32_t              INTPEND;                                     /*!<                Pending interrupts                                    */
    __IOM uint32_t             ENABLE;                                      /*!<                Enables the QSPI This requests clock for the IP core  */
    __IOM NRF_QSPI_CONFIG_Type CONFIG;                                      /*!<                (unspecified)                                         */
    __IOM NRF_QSPI_FORMAT_Type FORMAT;                                      /*!<                (unspecified)                                         */
    __IOM NRF_QSPI_DMA_Type    DMA;                                         /*!<                (unspecified)                                         */
    __IOM NRF_QSPI_CORE_Type   CORE;                                        /*!<                (unspecified)                                         */
    __IOM NRF_QSPI_SPSYNC_Type SPSYNC;                                      /*!<                (unspecified)                                         */
} NRF_SP_QSPI_Type;                                                         /*!<                                                                      */

/* QSPI_TASKS_START: Start operation. */
#define QSPI_TASKS_START_ResetValue \
    (0x00000000UL)                                                          /*!< Reset value of TASKS_START register.                                 */

/* TASKS_START @Bit 0 : Start operation. */
#define QSPI_TASKS_START_TASKS_START_Pos \
    (0UL)                                                                   /*!< Position of TASKS_START field.                                       */
#define QSPI_TASKS_START_TASKS_START_Msk \
    (0x1UL << QSPI_TASKS_START_TASKS_START_Pos)                             /*!< Bit mask of TASKS_START field.     */
#define QSPI_TASKS_START_TASKS_START_Min \
    (0x1UL)                                                                 /*!< Min enumerator value of TASKS_START field.                           */
#define QSPI_TASKS_START_TASKS_START_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of TASKS_START field.                           */
#define QSPI_TASKS_START_TASKS_START_Trigger \
    (0x1UL)                                                                 /*!< Trigger task                                                       */

/* QSPI_TASKS_RESET: Reset the QSPI */
#define QSPI_TASKS_RESET_ResetValue \
    (0x00000000UL)                                                          /*!< Reset value of TASKS_RESET register.                                 */

/* TASKS_RESET @Bit 0 : Reset the QSPI */
#define QSPI_TASKS_RESET_TASKS_RESET_Pos \
    (0UL)                                                                   /*!< Position of TASKS_RESET field.                                       */
#define QSPI_TASKS_RESET_TASKS_RESET_Msk \
    (0x1UL << QSPI_TASKS_RESET_TASKS_RESET_Pos)                             /*!< Bit mask of TASKS_RESET field.     */
#define QSPI_TASKS_RESET_TASKS_RESET_Min \
    (0x1UL)                                                                 /*!< Min enumerator value of TASKS_RESET field.                           */
#define QSPI_TASKS_RESET_TASKS_RESET_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of TASKS_RESET field.                           */
#define QSPI_TASKS_RESET_TASKS_RESET_Trigger \
    (0x1UL)                                                                 /*!< Trigger task                                                       */

/* QSPI_EVENTS_CORE: Interrupt from the QSPI core */
#define QSPI_EVENTS_CORE_ResetValue \
    (0x00000000UL)                                                          /*!< Reset value of EVENTS_CORE register.                                 */

/* EVENTS_CORE @Bit 0 : Interrupt from the QSPI core */
#define QSPI_EVENTS_CORE_EVENTS_CORE_Pos \
    (0UL)                                                                   /*!< Position of EVENTS_CORE field.                                       */
#define QSPI_EVENTS_CORE_EVENTS_CORE_Msk \
    (0x1UL << QSPI_EVENTS_CORE_EVENTS_CORE_Pos)                             /*!< Bit mask of EVENTS_CORE field.     */
#define QSPI_EVENTS_CORE_EVENTS_CORE_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of EVENTS_CORE field.                           */
#define QSPI_EVENTS_CORE_EVENTS_CORE_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of EVENTS_CORE field.                           */
#define QSPI_EVENTS_CORE_EVENTS_CORE_NotGenerated \
    (0x0UL)                                                                 /*!< Event not generated                                           */
#define QSPI_EVENTS_CORE_EVENTS_CORE_Generated \
    (0x1UL)                                                                 /*!< Event generated                                                  */

/* QSPI_EVENTS_IDLE: This event signifies that the QSPI core is no longer busy */
#define QSPI_EVENTS_IDLE_ResetValue \
    (0x00000000UL)                                                          /*!< Reset value of EVENTS_IDLE register.                                 */

/* EVENTS_IDLE @Bit 0 : This event signifies that the QSPI core is no longer busy */
#define QSPI_EVENTS_IDLE_EVENTS_IDLE_Pos \
    (0UL)                                                                   /*!< Position of EVENTS_IDLE field.                                       */
#define QSPI_EVENTS_IDLE_EVENTS_IDLE_Msk \
    (0x1UL << QSPI_EVENTS_IDLE_EVENTS_IDLE_Pos)                             /*!< Bit mask of EVENTS_IDLE field.     */
#define QSPI_EVENTS_IDLE_EVENTS_IDLE_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of EVENTS_IDLE field.                           */
#define QSPI_EVENTS_IDLE_EVENTS_IDLE_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of EVENTS_IDLE field.                           */
#define QSPI_EVENTS_IDLE_EVENTS_IDLE_NotGenerated \
    (0x0UL)                                                                 /*!< Event not generated                                           */
#define QSPI_EVENTS_IDLE_EVENTS_IDLE_Generated \
    (0x1UL)                                                                 /*!< Event generated                                                  */

/* QSPI_SHORTS: Shortcuts between local events and tasks */
#define QSPI_SHORTS_ResetValue \
    (0x00000000UL)                                                          /*!< Reset value of SHORTS register.                                      */

/* DMA_DONE_START @Bit 0 : Shortcut between event DMA.DONE and task START */
#define QSPI_SHORTS_DMA_DONE_START_Pos \
    (0UL)                                                                   /*!< Position of DMA_DONE_START field.                                    */
#define QSPI_SHORTS_DMA_DONE_START_Msk \
    (0x1UL << QSPI_SHORTS_DMA_DONE_START_Pos)                               /*!< Bit mask of DMA_DONE_START field.      */
#define QSPI_SHORTS_DMA_DONE_START_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMA_DONE_START field.                        */
#define QSPI_SHORTS_DMA_DONE_START_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMA_DONE_START field.                        */
#define QSPI_SHORTS_DMA_DONE_START_Disabled \
    (0x0UL)                                                                 /*!< Disable shortcut                                                    */
#define QSPI_SHORTS_DMA_DONE_START_Enabled \
    (0x1UL)                                                                 /*!< Enable shortcut                                                      */

/* QSPI_INTEN: Enable or disable interrupt */
#define QSPI_INTEN_ResetValue \
    (0x00000000UL)                                                          /*!< Reset value of INTEN register.                                       */

/* CORE @Bit 0 : Enable or disable interrupt for event CORE */
#define QSPI_INTEN_CORE_Pos      (0UL)                                      /*!< Position of CORE field.                                              */
#define QSPI_INTEN_CORE_Msk \
    (0x1UL << QSPI_INTEN_CORE_Pos)                                          /*!< Bit mask of CORE field.                                      */
#define QSPI_INTEN_CORE_Min      (0x0UL)                                    /*!< Min enumerator value of CORE field.                                  */
#define QSPI_INTEN_CORE_Max      (0x1UL)                                    /*!< Max enumerator value of CORE field.                                  */
#define QSPI_INTEN_CORE_Disabled (0x0UL)                                    /*!< Disable                                                              */
#define QSPI_INTEN_CORE_Enabled  (0x1UL)                                    /*!< Enable                                                               */

/* DMADONELIST @Bit 1 : Enable or disable interrupt for event DMADONELIST */
#define QSPI_INTEN_DMADONELIST_Pos (1UL)                                    /*!< Position of DMADONELIST field.                                       */
#define QSPI_INTEN_DMADONELIST_Msk \
    (0x1UL << QSPI_INTEN_DMADONELIST_Pos)                                   /*!< Bit mask of DMADONELIST field.                 */
#define QSPI_INTEN_DMADONELIST_Min (0x0UL)                                  /*!< Min enumerator value of DMADONELIST field.                           */
#define QSPI_INTEN_DMADONELIST_Max (0x1UL)                                  /*!< Max enumerator value of DMADONELIST field.                           */
#define QSPI_INTEN_DMADONELIST_Disabled \
    (0x0UL)                                                                 /*!< Disable                                                              */
#define QSPI_INTEN_DMADONELIST_Enabled \
    (0x1UL)                                                                 /*!< Enable                                                               */

/* DMADONELISTPART @Bit 2 : Enable or disable interrupt for event DMADONELISTPART */
#define QSPI_INTEN_DMADONELISTPART_Pos \
    (2UL)                                                                   /*!< Position of DMADONELISTPART field.                                   */
#define QSPI_INTEN_DMADONELISTPART_Msk \
    (0x1UL << QSPI_INTEN_DMADONELISTPART_Pos)                               /*!< Bit mask of DMADONELISTPART field.     */
#define QSPI_INTEN_DMADONELISTPART_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMADONELISTPART field.                       */
#define QSPI_INTEN_DMADONELISTPART_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMADONELISTPART field.                       */
#define QSPI_INTEN_DMADONELISTPART_Disabled \
    (0x0UL)                                                                 /*!< Disable                                                             */
#define QSPI_INTEN_DMADONELISTPART_Enabled \
    (0x1UL)                                                                 /*!< Enable                                                               */

/* DMADONESELECTJOB @Bit 3 : Enable or disable interrupt for event DMADONESELECTJOB */
#define QSPI_INTEN_DMADONESELECTJOB_Pos \
    (3UL)                                                                   /*!< Position of DMADONESELECTJOB field.                                  */
#define QSPI_INTEN_DMADONESELECTJOB_Msk \
    (0x1UL << QSPI_INTEN_DMADONESELECTJOB_Pos)                              /*!< Bit mask of DMADONESELECTJOB field.  */
#define QSPI_INTEN_DMADONESELECTJOB_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMADONESELECTJOB field.                      */
#define QSPI_INTEN_DMADONESELECTJOB_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMADONESELECTJOB field.                      */
#define QSPI_INTEN_DMADONESELECTJOB_Disabled \
    (0x0UL)                                                                 /*!< Disable                                                            */
#define QSPI_INTEN_DMADONESELECTJOB_Enabled \
    (0x1UL)                                                                 /*!< Enable                                                              */

/* DMADONEDATA @Bit 4 : Enable or disable interrupt for event DMADONEDATA */
#define QSPI_INTEN_DMADONEDATA_Pos (4UL)                                    /*!< Position of DMADONEDATA field.                                       */
#define QSPI_INTEN_DMADONEDATA_Msk \
    (0x1UL << QSPI_INTEN_DMADONEDATA_Pos)                                   /*!< Bit mask of DMADONEDATA field.                 */
#define QSPI_INTEN_DMADONEDATA_Min (0x0UL)                                  /*!< Min enumerator value of DMADONEDATA field.                           */
#define QSPI_INTEN_DMADONEDATA_Max (0x1UL)                                  /*!< Max enumerator value of DMADONEDATA field.                           */
#define QSPI_INTEN_DMADONEDATA_Disabled \
    (0x0UL)                                                                 /*!< Disable                                                              */
#define QSPI_INTEN_DMADONEDATA_Enabled \
    (0x1UL)                                                                 /*!< Enable                                                               */

/* DMADONEJOB @Bit 5 : Enable or disable interrupt for event DMADONEJOB */
#define QSPI_INTEN_DMADONEJOB_Pos (5UL)                                     /*!< Position of DMADONEJOB field.                                        */
#define QSPI_INTEN_DMADONEJOB_Msk \
    (0x1UL << QSPI_INTEN_DMADONEJOB_Pos)                                    /*!< Bit mask of DMADONEJOB field.                    */
#define QSPI_INTEN_DMADONEJOB_Min (0x0UL)                                   /*!< Min enumerator value of DMADONEJOB field.                            */
#define QSPI_INTEN_DMADONEJOB_Max (0x1UL)                                   /*!< Max enumerator value of DMADONEJOB field.                            */
#define QSPI_INTEN_DMADONEJOB_Disabled \
    (0x0UL)                                                                 /*!< Disable                                                              */
#define QSPI_INTEN_DMADONEJOB_Enabled \
    (0x1UL)                                                                 /*!< Enable                                                               */

/* DMAERROR @Bit 6 : Enable or disable interrupt for event DMAERROR */
#define QSPI_INTEN_DMAERROR_Pos     (6UL)                                   /*!< Position of DMAERROR field.                                          */
#define QSPI_INTEN_DMAERROR_Msk \
    (0x1UL << QSPI_INTEN_DMAERROR_Pos)                                      /*!< Bit mask of DMAERROR field.                          */
#define QSPI_INTEN_DMAERROR_Min     (0x0UL)                                 /*!< Min enumerator value of DMAERROR field.                              */
#define QSPI_INTEN_DMAERROR_Max     (0x1UL)                                 /*!< Max enumerator value of DMAERROR field.                              */
#define QSPI_INTEN_DMAERROR_Disabled \
    (0x0UL)                                                                 /*!< Disable                                                              */
#define QSPI_INTEN_DMAERROR_Enabled (0x1UL)                                 /*!< Enable                                                               */

/* DMAPAUSED @Bit 7 : Enable or disable interrupt for event DMAPAUSED */
#define QSPI_INTEN_DMAPAUSED_Pos (7UL)                                      /*!< Position of DMAPAUSED field.                                         */
#define QSPI_INTEN_DMAPAUSED_Msk \
    (0x1UL << QSPI_INTEN_DMAPAUSED_Pos)                                     /*!< Bit mask of DMAPAUSED field.                       */
#define QSPI_INTEN_DMAPAUSED_Min (0x0UL)                                    /*!< Min enumerator value of DMAPAUSED field.                             */
#define QSPI_INTEN_DMAPAUSED_Max (0x1UL)                                    /*!< Max enumerator value of DMAPAUSED field.                             */
#define QSPI_INTEN_DMAPAUSED_Disabled \
    (0x0UL)                                                                 /*!< Disable                                                              */
#define QSPI_INTEN_DMAPAUSED_Enabled \
    (0x1UL)                                                                 /*!< Enable                                                               */

/* DMARESET @Bit 8 : Enable or disable interrupt for event DMARESET */
#define QSPI_INTEN_DMARESET_Pos     (8UL)                                   /*!< Position of DMARESET field.                                          */
#define QSPI_INTEN_DMARESET_Msk \
    (0x1UL << QSPI_INTEN_DMARESET_Pos)                                      /*!< Bit mask of DMARESET field.                          */
#define QSPI_INTEN_DMARESET_Min     (0x0UL)                                 /*!< Min enumerator value of DMARESET field.                              */
#define QSPI_INTEN_DMARESET_Max     (0x1UL)                                 /*!< Max enumerator value of DMARESET field.                              */
#define QSPI_INTEN_DMARESET_Disabled \
    (0x0UL)                                                                 /*!< Disable                                                              */
#define QSPI_INTEN_DMARESET_Enabled (0x1UL)                                 /*!< Enable                                                               */

/* DMADONE @Bit 9 : Enable or disable interrupt for event DMADONE */
#define QSPI_INTEN_DMADONE_Pos      (9UL)                                   /*!< Position of DMADONE field.                                           */
#define QSPI_INTEN_DMADONE_Msk \
    (0x1UL << QSPI_INTEN_DMADONE_Pos)                                       /*!< Bit mask of DMADONE field.                             */
#define QSPI_INTEN_DMADONE_Min      (0x0UL)                                 /*!< Min enumerator value of DMADONE field.                               */
#define QSPI_INTEN_DMADONE_Max      (0x1UL)                                 /*!< Max enumerator value of DMADONE field.                               */
#define QSPI_INTEN_DMADONE_Disabled (0x0UL)                                 /*!< Disable                                                              */
#define QSPI_INTEN_DMADONE_Enabled  (0x1UL)                                 /*!< Enable                                                               */

/* DMATXUNEXPECTEDIDLE @Bit 10 : Enable or disable interrupt for event DMATXUNEXPECTEDIDLE */
#define QSPI_INTEN_DMATXUNEXPECTEDIDLE_Pos \
    (10UL)                                                                  /*!< Position of DMATXUNEXPECTEDIDLE field.                               */
#define QSPI_INTEN_DMATXUNEXPECTEDIDLE_Msk \
    (0x1UL << QSPI_INTEN_DMATXUNEXPECTEDIDLE_Pos)                           /*!< Bit mask of DMATXUNEXPECTEDIDLE
                                                                             *                        field.*/
#define QSPI_INTEN_DMATXUNEXPECTEDIDLE_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMATXUNEXPECTEDIDLE field.                   */
#define QSPI_INTEN_DMATXUNEXPECTEDIDLE_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMATXUNEXPECTEDIDLE field.                   */
#define QSPI_INTEN_DMATXUNEXPECTEDIDLE_Disabled \
    (0x0UL)                                                                 /*!< Disable                                                         */
#define QSPI_INTEN_DMATXUNEXPECTEDIDLE_Enabled \
    (0x1UL)                                                                 /*!< Enable                                                           */

/* DMAINTERNALBUSERROR @Bit 11 : Enable or disable interrupt for event DMAINTERNALBUSERROR */
#define QSPI_INTEN_DMAINTERNALBUSERROR_Pos \
    (11UL)                                                                  /*!< Position of DMAINTERNALBUSERROR field.                               */
#define QSPI_INTEN_DMAINTERNALBUSERROR_Msk \
    (0x1UL << QSPI_INTEN_DMAINTERNALBUSERROR_Pos)                           /*!< Bit mask of DMAINTERNALBUSERROR
                                                                             *                        field.*/
#define QSPI_INTEN_DMAINTERNALBUSERROR_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMAINTERNALBUSERROR field.                   */
#define QSPI_INTEN_DMAINTERNALBUSERROR_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMAINTERNALBUSERROR field.                   */
#define QSPI_INTEN_DMAINTERNALBUSERROR_Disabled \
    (0x0UL)                                                                 /*!< Disable                                                         */
#define QSPI_INTEN_DMAINTERNALBUSERROR_Enabled \
    (0x1UL)                                                                 /*!< Enable                                                           */

/* DMAABORTED @Bit 12 : Enable or disable interrupt for event DMAABORTED */
#define QSPI_INTEN_DMAABORTED_Pos (12UL)                                    /*!< Position of DMAABORTED field.                                        */
#define QSPI_INTEN_DMAABORTED_Msk \
    (0x1UL << QSPI_INTEN_DMAABORTED_Pos)                                    /*!< Bit mask of DMAABORTED field.                    */
#define QSPI_INTEN_DMAABORTED_Min (0x0UL)                                   /*!< Min enumerator value of DMAABORTED field.                            */
#define QSPI_INTEN_DMAABORTED_Max (0x1UL)                                   /*!< Max enumerator value of DMAABORTED field.                            */
#define QSPI_INTEN_DMAABORTED_Disabled \
    (0x0UL)                                                                 /*!< Disable                                                              */
#define QSPI_INTEN_DMAABORTED_Enabled \
    (0x1UL)                                                                 /*!< Enable                                                               */

/* IDLE @Bit 13 : Enable or disable interrupt for event IDLE */
#define QSPI_INTEN_IDLE_Pos      (13UL)                                     /*!< Position of IDLE field.                                              */
#define QSPI_INTEN_IDLE_Msk \
    (0x1UL << QSPI_INTEN_IDLE_Pos)                                          /*!< Bit mask of IDLE field.                                      */
#define QSPI_INTEN_IDLE_Min      (0x0UL)                                    /*!< Min enumerator value of IDLE field.                                  */
#define QSPI_INTEN_IDLE_Max      (0x1UL)                                    /*!< Max enumerator value of IDLE field.                                  */
#define QSPI_INTEN_IDLE_Disabled (0x0UL)                                    /*!< Disable                                                              */
#define QSPI_INTEN_IDLE_Enabled  (0x1UL)                                    /*!< Enable                                                               */

/* QSPI_INTENSET: Enable interrupt */
#define QSPI_INTENSET_ResetValue \
    (0x00000000UL)                                                          /*!< Reset value of INTENSET register.                                    */

/* CORE @Bit 0 : Write '1' to enable interrupt for event CORE */
#define QSPI_INTENSET_CORE_Pos      (0UL)                                   /*!< Position of CORE field.                                              */
#define QSPI_INTENSET_CORE_Msk \
    (0x1UL << QSPI_INTENSET_CORE_Pos)                                       /*!< Bit mask of CORE field.                                */
#define QSPI_INTENSET_CORE_Min      (0x0UL)                                 /*!< Min enumerator value of CORE field.                                  */
#define QSPI_INTENSET_CORE_Max      (0x1UL)                                 /*!< Max enumerator value of CORE field.                                  */
#define QSPI_INTENSET_CORE_Set      (0x1UL)                                 /*!< Enable                                                               */
#define QSPI_INTENSET_CORE_Disabled (0x0UL)                                 /*!< Read: Disabled                                                       */
#define QSPI_INTENSET_CORE_Enabled  (0x1UL)                                 /*!< Read: Enabled                                                        */

/* DMADONELIST @Bit 1 : Write '1' to enable interrupt for event DMADONELIST */
#define QSPI_INTENSET_DMADONELIST_Pos (1UL)                                 /*!< Position of DMADONELIST field.                                       */
#define QSPI_INTENSET_DMADONELIST_Msk \
    (0x1UL << QSPI_INTENSET_DMADONELIST_Pos)                                /*!< Bit mask of DMADONELIST field.           */
#define QSPI_INTENSET_DMADONELIST_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMADONELIST field.                           */
#define QSPI_INTENSET_DMADONELIST_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMADONELIST field.                           */
#define QSPI_INTENSET_DMADONELIST_Set \
    (0x1UL)                                                                 /*!< Enable                                                               */
#define QSPI_INTENSET_DMADONELIST_Disabled \
    (0x0UL)                                                                 /*!< Read: Disabled                                                       */
#define QSPI_INTENSET_DMADONELIST_Enabled \
    (0x1UL)                                                                 /*!< Read: Enabled                                                        */

/* DMADONELISTPART @Bit 2 : Write '1' to enable interrupt for event DMADONELISTPART */
#define QSPI_INTENSET_DMADONELISTPART_Pos \
    (2UL)                                                                   /*!< Position of DMADONELISTPART field.                                   */
#define QSPI_INTENSET_DMADONELISTPART_Msk \
    (0x1UL << QSPI_INTENSET_DMADONELISTPART_Pos)                            /*!< Bit mask of DMADONELISTPART
                                                                             *                         field.*/
#define QSPI_INTENSET_DMADONELISTPART_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMADONELISTPART field.                       */
#define QSPI_INTENSET_DMADONELISTPART_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMADONELISTPART field.                       */
#define QSPI_INTENSET_DMADONELISTPART_Set \
    (0x1UL)                                                                 /*!< Enable                                                               */
#define QSPI_INTENSET_DMADONELISTPART_Disabled \
    (0x0UL)                                                                 /*!< Read: Disabled                                                   */
#define QSPI_INTENSET_DMADONELISTPART_Enabled \
    (0x1UL)                                                                 /*!< Read: Enabled                                                     */

/* DMADONESELECTJOB @Bit 3 : Write '1' to enable interrupt for event DMADONESELECTJOB */
#define QSPI_INTENSET_DMADONESELECTJOB_Pos \
    (3UL)                                                                   /*!< Position of DMADONESELECTJOB field.                                  */
#define QSPI_INTENSET_DMADONESELECTJOB_Msk \
    (0x1UL << QSPI_INTENSET_DMADONESELECTJOB_Pos)                           /*!< Bit mask of DMADONESELECTJOB
                                                                            *                        field.*/
#define QSPI_INTENSET_DMADONESELECTJOB_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMADONESELECTJOB field.                      */
#define QSPI_INTENSET_DMADONESELECTJOB_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMADONESELECTJOB field.                      */
#define QSPI_INTENSET_DMADONESELECTJOB_Set \
    (0x1UL)                                                                 /*!< Enable                                                               */
#define QSPI_INTENSET_DMADONESELECTJOB_Disabled \
    (0x0UL)                                                                 /*!< Read: Disabled                                                  */
#define QSPI_INTENSET_DMADONESELECTJOB_Enabled \
    (0x1UL)                                                                 /*!< Read: Enabled                                                    */

/* DMADONEDATA @Bit 4 : Write '1' to enable interrupt for event DMADONEDATA */
#define QSPI_INTENSET_DMADONEDATA_Pos (4UL)                                 /*!< Position of DMADONEDATA field.                                       */
#define QSPI_INTENSET_DMADONEDATA_Msk \
    (0x1UL << QSPI_INTENSET_DMADONEDATA_Pos)                                /*!< Bit mask of DMADONEDATA field.           */
#define QSPI_INTENSET_DMADONEDATA_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMADONEDATA field.                           */
#define QSPI_INTENSET_DMADONEDATA_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMADONEDATA field.                           */
#define QSPI_INTENSET_DMADONEDATA_Set \
    (0x1UL)                                                                 /*!< Enable                                                               */
#define QSPI_INTENSET_DMADONEDATA_Disabled \
    (0x0UL)                                                                 /*!< Read: Disabled                                                       */
#define QSPI_INTENSET_DMADONEDATA_Enabled \
    (0x1UL)                                                                 /*!< Read: Enabled                                                        */

/* DMADONEJOB @Bit 5 : Write '1' to enable interrupt for event DMADONEJOB */
#define QSPI_INTENSET_DMADONEJOB_Pos (5UL)                                  /*!< Position of DMADONEJOB field.                                        */
#define QSPI_INTENSET_DMADONEJOB_Msk \
    (0x1UL << QSPI_INTENSET_DMADONEJOB_Pos)                                 /*!< Bit mask of DMADONEJOB field.              */
#define QSPI_INTENSET_DMADONEJOB_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMADONEJOB field.                            */
#define QSPI_INTENSET_DMADONEJOB_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMADONEJOB field.                            */
#define QSPI_INTENSET_DMADONEJOB_Set \
    (0x1UL)                                                                 /*!< Enable                                                               */
#define QSPI_INTENSET_DMADONEJOB_Disabled \
    (0x0UL)                                                                 /*!< Read: Disabled                                                       */
#define QSPI_INTENSET_DMADONEJOB_Enabled \
    (0x1UL)                                                                 /*!< Read: Enabled                                                        */

/* DMAERROR @Bit 6 : Write '1' to enable interrupt for event DMAERROR */
#define QSPI_INTENSET_DMAERROR_Pos (6UL)                                    /*!< Position of DMAERROR field.                                          */
#define QSPI_INTENSET_DMAERROR_Msk \
    (0x1UL << QSPI_INTENSET_DMAERROR_Pos)                                   /*!< Bit mask of DMAERROR field.                    */
#define QSPI_INTENSET_DMAERROR_Min (0x0UL)                                  /*!< Min enumerator value of DMAERROR field.                              */
#define QSPI_INTENSET_DMAERROR_Max (0x1UL)                                  /*!< Max enumerator value of DMAERROR field.                              */
#define QSPI_INTENSET_DMAERROR_Set (0x1UL)                                  /*!< Enable                                                               */
#define QSPI_INTENSET_DMAERROR_Disabled \
    (0x0UL)                                                                 /*!< Read: Disabled                                                       */
#define QSPI_INTENSET_DMAERROR_Enabled \
    (0x1UL)                                                                 /*!< Read: Enabled                                                        */

/* DMAPAUSED @Bit 7 : Write '1' to enable interrupt for event DMAPAUSED */
#define QSPI_INTENSET_DMAPAUSED_Pos (7UL)                                   /*!< Position of DMAPAUSED field.                                         */
#define QSPI_INTENSET_DMAPAUSED_Msk \
    (0x1UL << QSPI_INTENSET_DMAPAUSED_Pos)                                  /*!< Bit mask of DMAPAUSED field.                 */
#define QSPI_INTENSET_DMAPAUSED_Min (0x0UL)                                 /*!< Min enumerator value of DMAPAUSED field.                             */
#define QSPI_INTENSET_DMAPAUSED_Max (0x1UL)                                 /*!< Max enumerator value of DMAPAUSED field.                             */
#define QSPI_INTENSET_DMAPAUSED_Set (0x1UL)                                 /*!< Enable                                                               */
#define QSPI_INTENSET_DMAPAUSED_Disabled \
    (0x0UL)                                                                 /*!< Read: Disabled                                                       */
#define QSPI_INTENSET_DMAPAUSED_Enabled \
    (0x1UL)                                                                 /*!< Read: Enabled                                                        */

/* DMARESET @Bit 8 : Write '1' to enable interrupt for event DMARESET */
#define QSPI_INTENSET_DMARESET_Pos (8UL)                                    /*!< Position of DMARESET field.                                          */
#define QSPI_INTENSET_DMARESET_Msk \
    (0x1UL << QSPI_INTENSET_DMARESET_Pos)                                   /*!< Bit mask of DMARESET field.                    */
#define QSPI_INTENSET_DMARESET_Min (0x0UL)                                  /*!< Min enumerator value of DMARESET field.                              */
#define QSPI_INTENSET_DMARESET_Max (0x1UL)                                  /*!< Max enumerator value of DMARESET field.                              */
#define QSPI_INTENSET_DMARESET_Set (0x1UL)                                  /*!< Enable                                                               */
#define QSPI_INTENSET_DMARESET_Disabled \
    (0x0UL)                                                                 /*!< Read: Disabled                                                       */
#define QSPI_INTENSET_DMARESET_Enabled \
    (0x1UL)                                                                 /*!< Read: Enabled                                                        */

/* DMADONE @Bit 9 : Write '1' to enable interrupt for event DMADONE */
#define QSPI_INTENSET_DMADONE_Pos (9UL)                                     /*!< Position of DMADONE field.                                           */
#define QSPI_INTENSET_DMADONE_Msk \
    (0x1UL << QSPI_INTENSET_DMADONE_Pos)                                    /*!< Bit mask of DMADONE field.                       */
#define QSPI_INTENSET_DMADONE_Min (0x0UL)                                   /*!< Min enumerator value of DMADONE field.                               */
#define QSPI_INTENSET_DMADONE_Max (0x1UL)                                   /*!< Max enumerator value of DMADONE field.                               */
#define QSPI_INTENSET_DMADONE_Set (0x1UL)                                   /*!< Enable                                                               */
#define QSPI_INTENSET_DMADONE_Disabled \
    (0x0UL)                                                                 /*!< Read: Disabled                                                       */
#define QSPI_INTENSET_DMADONE_Enabled \
    (0x1UL)                                                                 /*!< Read: Enabled                                                        */

/* DMATXUNEXPECTEDIDLE @Bit 10 : Write '1' to enable interrupt for event DMATXUNEXPECTEDIDLE */
#define QSPI_INTENSET_DMATXUNEXPECTEDIDLE_Pos \
    (10UL)                                                                  /*!< Position of DMATXUNEXPECTEDIDLE field.                             */
#define QSPI_INTENSET_DMATXUNEXPECTEDIDLE_Msk \
    (0x1UL << QSPI_INTENSET_DMATXUNEXPECTEDIDLE_Pos)                        /*!< Bit mask of
                                                                             *                     DMATXUNEXPECTEDIDLE field.*/
#define QSPI_INTENSET_DMATXUNEXPECTEDIDLE_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMATXUNEXPECTEDIDLE field.                */
#define QSPI_INTENSET_DMATXUNEXPECTEDIDLE_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMATXUNEXPECTEDIDLE field.                */
#define QSPI_INTENSET_DMATXUNEXPECTEDIDLE_Set \
    (0x1UL)                                                                 /*!< Enable                                                            */
#define QSPI_INTENSET_DMATXUNEXPECTEDIDLE_Disabled \
    (0x0UL)                                                                 /*!< Read: Disabled                                               */
#define QSPI_INTENSET_DMATXUNEXPECTEDIDLE_Enabled \
    (0x1UL)                                                                 /*!< Read: Enabled                                                 */

/* DMAINTERNALBUSERROR @Bit 11 : Write '1' to enable interrupt for event DMAINTERNALBUSERROR */
#define QSPI_INTENSET_DMAINTERNALBUSERROR_Pos \
    (11UL)                                                                  /*!< Position of DMAINTERNALBUSERROR field.                             */
#define QSPI_INTENSET_DMAINTERNALBUSERROR_Msk \
    (0x1UL << QSPI_INTENSET_DMAINTERNALBUSERROR_Pos)                        /*!< Bit mask of
                                                                             *                     DMAINTERNALBUSERROR field.*/
#define QSPI_INTENSET_DMAINTERNALBUSERROR_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMAINTERNALBUSERROR field.                */
#define QSPI_INTENSET_DMAINTERNALBUSERROR_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMAINTERNALBUSERROR field.                */
#define QSPI_INTENSET_DMAINTERNALBUSERROR_Set \
    (0x1UL)                                                                 /*!< Enable                                                            */
#define QSPI_INTENSET_DMAINTERNALBUSERROR_Disabled \
    (0x0UL)                                                                 /*!< Read: Disabled                                               */
#define QSPI_INTENSET_DMAINTERNALBUSERROR_Enabled \
    (0x1UL)                                                                 /*!< Read: Enabled                                                 */

/* DMAABORTED @Bit 12 : Write '1' to enable interrupt for event DMAABORTED */
#define QSPI_INTENSET_DMAABORTED_Pos (12UL)                                 /*!< Position of DMAABORTED field.                                        */
#define QSPI_INTENSET_DMAABORTED_Msk \
    (0x1UL << QSPI_INTENSET_DMAABORTED_Pos)                                 /*!< Bit mask of DMAABORTED field.              */
#define QSPI_INTENSET_DMAABORTED_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMAABORTED field.                            */
#define QSPI_INTENSET_DMAABORTED_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMAABORTED field.                            */
#define QSPI_INTENSET_DMAABORTED_Set \
    (0x1UL)                                                                 /*!< Enable                                                               */
#define QSPI_INTENSET_DMAABORTED_Disabled \
    (0x0UL)                                                                 /*!< Read: Disabled                                                       */
#define QSPI_INTENSET_DMAABORTED_Enabled \
    (0x1UL)                                                                 /*!< Read: Enabled                                                        */

/* IDLE @Bit 13 : Write '1' to enable interrupt for event IDLE */
#define QSPI_INTENSET_IDLE_Pos      (13UL)                                  /*!< Position of IDLE field.                                              */
#define QSPI_INTENSET_IDLE_Msk \
    (0x1UL << QSPI_INTENSET_IDLE_Pos)                                       /*!< Bit mask of IDLE field.                                */
#define QSPI_INTENSET_IDLE_Min      (0x0UL)                                 /*!< Min enumerator value of IDLE field.                                  */
#define QSPI_INTENSET_IDLE_Max      (0x1UL)                                 /*!< Max enumerator value of IDLE field.                                  */
#define QSPI_INTENSET_IDLE_Set      (0x1UL)                                 /*!< Enable                                                               */
#define QSPI_INTENSET_IDLE_Disabled (0x0UL)                                 /*!< Read: Disabled                                                       */
#define QSPI_INTENSET_IDLE_Enabled  (0x1UL)                                 /*!< Read: Enabled                                                        */

/* QSPI_INTENCLR: Disable interrupt */
#define QSPI_INTENCLR_ResetValue \
    (0x00000000UL)                                                          /*!< Reset value of INTENCLR register.                                    */

/* CORE @Bit 0 : Write '1' to disable interrupt for event CORE */
#define QSPI_INTENCLR_CORE_Pos      (0UL)                                   /*!< Position of CORE field.                                              */
#define QSPI_INTENCLR_CORE_Msk \
    (0x1UL << QSPI_INTENCLR_CORE_Pos)                                       /*!< Bit mask of CORE field.                                */
#define QSPI_INTENCLR_CORE_Min      (0x0UL)                                 /*!< Min enumerator value of CORE field.                                  */
#define QSPI_INTENCLR_CORE_Max      (0x1UL)                                 /*!< Max enumerator value of CORE field.                                  */
#define QSPI_INTENCLR_CORE_Clear    (0x1UL)                                 /*!< Disable                                                              */
#define QSPI_INTENCLR_CORE_Disabled (0x0UL)                                 /*!< Read: Disabled                                                       */
#define QSPI_INTENCLR_CORE_Enabled  (0x1UL)                                 /*!< Read: Enabled                                                        */

/* DMADONELIST @Bit 1 : Write '1' to disable interrupt for event DMADONELIST */
#define QSPI_INTENCLR_DMADONELIST_Pos (1UL)                                 /*!< Position of DMADONELIST field.                                       */
#define QSPI_INTENCLR_DMADONELIST_Msk \
    (0x1UL << QSPI_INTENCLR_DMADONELIST_Pos)                                /*!< Bit mask of DMADONELIST field.           */
#define QSPI_INTENCLR_DMADONELIST_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMADONELIST field.                           */
#define QSPI_INTENCLR_DMADONELIST_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMADONELIST field.                           */
#define QSPI_INTENCLR_DMADONELIST_Clear \
    (0x1UL)                                                                 /*!< Disable                                                              */
#define QSPI_INTENCLR_DMADONELIST_Disabled \
    (0x0UL)                                                                 /*!< Read: Disabled                                                       */
#define QSPI_INTENCLR_DMADONELIST_Enabled \
    (0x1UL)                                                                 /*!< Read: Enabled                                                        */

/* DMADONELISTPART @Bit 2 : Write '1' to disable interrupt for event DMADONELISTPART */
#define QSPI_INTENCLR_DMADONELISTPART_Pos \
    (2UL)                                                                   /*!< Position of DMADONELISTPART field.                                   */
#define QSPI_INTENCLR_DMADONELISTPART_Msk \
    (0x1UL << QSPI_INTENCLR_DMADONELISTPART_Pos)                            /*!< Bit mask of DMADONELISTPART
                                                                             *                         field.*/
#define QSPI_INTENCLR_DMADONELISTPART_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMADONELISTPART field.                       */
#define QSPI_INTENCLR_DMADONELISTPART_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMADONELISTPART field.                       */
#define QSPI_INTENCLR_DMADONELISTPART_Clear \
    (0x1UL)                                                                 /*!< Disable                                                             */
#define QSPI_INTENCLR_DMADONELISTPART_Disabled \
    (0x0UL)                                                                 /*!< Read: Disabled                                                   */
#define QSPI_INTENCLR_DMADONELISTPART_Enabled \
    (0x1UL)                                                                 /*!< Read: Enabled                                                     */

/* DMADONESELECTJOB @Bit 3 : Write '1' to disable interrupt for event DMADONESELECTJOB */
#define QSPI_INTENCLR_DMADONESELECTJOB_Pos \
    (3UL)                                                                   /*!< Position of DMADONESELECTJOB field.                                  */
#define QSPI_INTENCLR_DMADONESELECTJOB_Msk \
    (0x1UL << QSPI_INTENCLR_DMADONESELECTJOB_Pos)                           /*!< Bit mask of DMADONESELECTJOB
                                                                            *                        field.*/
#define QSPI_INTENCLR_DMADONESELECTJOB_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMADONESELECTJOB field.                      */
#define QSPI_INTENCLR_DMADONESELECTJOB_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMADONESELECTJOB field.                      */
#define QSPI_INTENCLR_DMADONESELECTJOB_Clear \
    (0x1UL)                                                                 /*!< Disable                                                            */
#define QSPI_INTENCLR_DMADONESELECTJOB_Disabled \
    (0x0UL)                                                                 /*!< Read: Disabled                                                  */
#define QSPI_INTENCLR_DMADONESELECTJOB_Enabled \
    (0x1UL)                                                                 /*!< Read: Enabled                                                    */

/* DMADONEDATA @Bit 4 : Write '1' to disable interrupt for event DMADONEDATA */
#define QSPI_INTENCLR_DMADONEDATA_Pos (4UL)                                 /*!< Position of DMADONEDATA field.                                       */
#define QSPI_INTENCLR_DMADONEDATA_Msk \
    (0x1UL << QSPI_INTENCLR_DMADONEDATA_Pos)                                /*!< Bit mask of DMADONEDATA field.           */
#define QSPI_INTENCLR_DMADONEDATA_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMADONEDATA field.                           */
#define QSPI_INTENCLR_DMADONEDATA_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMADONEDATA field.                           */
#define QSPI_INTENCLR_DMADONEDATA_Clear \
    (0x1UL)                                                                 /*!< Disable                                                              */
#define QSPI_INTENCLR_DMADONEDATA_Disabled \
    (0x0UL)                                                                 /*!< Read: Disabled                                                       */
#define QSPI_INTENCLR_DMADONEDATA_Enabled \
    (0x1UL)                                                                 /*!< Read: Enabled                                                        */

/* DMADONEJOB @Bit 5 : Write '1' to disable interrupt for event DMADONEJOB */
#define QSPI_INTENCLR_DMADONEJOB_Pos (5UL)                                  /*!< Position of DMADONEJOB field.                                        */
#define QSPI_INTENCLR_DMADONEJOB_Msk \
    (0x1UL << QSPI_INTENCLR_DMADONEJOB_Pos)                                 /*!< Bit mask of DMADONEJOB field.              */
#define QSPI_INTENCLR_DMADONEJOB_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMADONEJOB field.                            */
#define QSPI_INTENCLR_DMADONEJOB_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMADONEJOB field.                            */
#define QSPI_INTENCLR_DMADONEJOB_Clear \
    (0x1UL)                                                                 /*!< Disable                                                              */
#define QSPI_INTENCLR_DMADONEJOB_Disabled \
    (0x0UL)                                                                 /*!< Read: Disabled                                                       */
#define QSPI_INTENCLR_DMADONEJOB_Enabled \
    (0x1UL)                                                                 /*!< Read: Enabled                                                        */

/* DMAERROR @Bit 6 : Write '1' to disable interrupt for event DMAERROR */
#define QSPI_INTENCLR_DMAERROR_Pos (6UL)                                    /*!< Position of DMAERROR field.                                          */
#define QSPI_INTENCLR_DMAERROR_Msk \
    (0x1UL << QSPI_INTENCLR_DMAERROR_Pos)                                   /*!< Bit mask of DMAERROR field.                    */
#define QSPI_INTENCLR_DMAERROR_Min (0x0UL)                                  /*!< Min enumerator value of DMAERROR field.                              */
#define QSPI_INTENCLR_DMAERROR_Max (0x1UL)                                  /*!< Max enumerator value of DMAERROR field.                              */
#define QSPI_INTENCLR_DMAERROR_Clear \
    (0x1UL)                                                                 /*!< Disable                                                              */
#define QSPI_INTENCLR_DMAERROR_Disabled \
    (0x0UL)                                                                 /*!< Read: Disabled                                                       */
#define QSPI_INTENCLR_DMAERROR_Enabled \
    (0x1UL)                                                                 /*!< Read: Enabled                                                        */

/* DMAPAUSED @Bit 7 : Write '1' to disable interrupt for event DMAPAUSED */
#define QSPI_INTENCLR_DMAPAUSED_Pos (7UL)                                   /*!< Position of DMAPAUSED field.                                         */
#define QSPI_INTENCLR_DMAPAUSED_Msk \
    (0x1UL << QSPI_INTENCLR_DMAPAUSED_Pos)                                  /*!< Bit mask of DMAPAUSED field.                 */
#define QSPI_INTENCLR_DMAPAUSED_Min (0x0UL)                                 /*!< Min enumerator value of DMAPAUSED field.                             */
#define QSPI_INTENCLR_DMAPAUSED_Max (0x1UL)                                 /*!< Max enumerator value of DMAPAUSED field.                             */
#define QSPI_INTENCLR_DMAPAUSED_Clear \
    (0x1UL)                                                                 /*!< Disable                                                              */
#define QSPI_INTENCLR_DMAPAUSED_Disabled \
    (0x0UL)                                                                 /*!< Read: Disabled                                                       */
#define QSPI_INTENCLR_DMAPAUSED_Enabled \
    (0x1UL)                                                                 /*!< Read: Enabled                                                        */

/* DMARESET @Bit 8 : Write '1' to disable interrupt for event DMARESET */
#define QSPI_INTENCLR_DMARESET_Pos (8UL)                                    /*!< Position of DMARESET field.                                          */
#define QSPI_INTENCLR_DMARESET_Msk \
    (0x1UL << QSPI_INTENCLR_DMARESET_Pos)                                   /*!< Bit mask of DMARESET field.                    */
#define QSPI_INTENCLR_DMARESET_Min (0x0UL)                                  /*!< Min enumerator value of DMARESET field.                              */
#define QSPI_INTENCLR_DMARESET_Max (0x1UL)                                  /*!< Max enumerator value of DMARESET field.                              */
#define QSPI_INTENCLR_DMARESET_Clear \
    (0x1UL)                                                                 /*!< Disable                                                              */
#define QSPI_INTENCLR_DMARESET_Disabled \
    (0x0UL)                                                                 /*!< Read: Disabled                                                       */
#define QSPI_INTENCLR_DMARESET_Enabled \
    (0x1UL)                                                                 /*!< Read: Enabled                                                        */

/* DMADONE @Bit 9 : Write '1' to disable interrupt for event DMADONE */
#define QSPI_INTENCLR_DMADONE_Pos   (9UL)                                   /*!< Position of DMADONE field.                                           */
#define QSPI_INTENCLR_DMADONE_Msk \
    (0x1UL << QSPI_INTENCLR_DMADONE_Pos)                                    /*!< Bit mask of DMADONE field.                       */
#define QSPI_INTENCLR_DMADONE_Min   (0x0UL)                                 /*!< Min enumerator value of DMADONE field.                               */
#define QSPI_INTENCLR_DMADONE_Max   (0x1UL)                                 /*!< Max enumerator value of DMADONE field.                               */
#define QSPI_INTENCLR_DMADONE_Clear (0x1UL)                                 /*!< Disable                                                              */
#define QSPI_INTENCLR_DMADONE_Disabled \
    (0x0UL)                                                                 /*!< Read: Disabled                                                       */
#define QSPI_INTENCLR_DMADONE_Enabled \
    (0x1UL)                                                                 /*!< Read: Enabled                                                        */

/* DMATXUNEXPECTEDIDLE @Bit 10 : Write '1' to disable interrupt for event DMATXUNEXPECTEDIDLE */
#define QSPI_INTENCLR_DMATXUNEXPECTEDIDLE_Pos \
    (10UL)                                                                  /*!< Position of DMATXUNEXPECTEDIDLE field.                             */
#define QSPI_INTENCLR_DMATXUNEXPECTEDIDLE_Msk \
    (0x1UL << QSPI_INTENCLR_DMATXUNEXPECTEDIDLE_Pos)                        /*!< Bit mask of
                                                                             *                     DMATXUNEXPECTEDIDLE field.*/
#define QSPI_INTENCLR_DMATXUNEXPECTEDIDLE_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMATXUNEXPECTEDIDLE field.                */
#define QSPI_INTENCLR_DMATXUNEXPECTEDIDLE_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMATXUNEXPECTEDIDLE field.                */
#define QSPI_INTENCLR_DMATXUNEXPECTEDIDLE_Clear \
    (0x1UL)                                                                 /*!< Disable                                                         */
#define QSPI_INTENCLR_DMATXUNEXPECTEDIDLE_Disabled \
    (0x0UL)                                                                 /*!< Read: Disabled                                               */
#define QSPI_INTENCLR_DMATXUNEXPECTEDIDLE_Enabled \
    (0x1UL)                                                                 /*!< Read: Enabled                                                 */

/* DMAINTERNALBUSERROR @Bit 11 : Write '1' to disable interrupt for event DMAINTERNALBUSERROR */
#define QSPI_INTENCLR_DMAINTERNALBUSERROR_Pos \
    (11UL)                                                                  /*!< Position of DMAINTERNALBUSERROR field.                             */
#define QSPI_INTENCLR_DMAINTERNALBUSERROR_Msk \
    (0x1UL << QSPI_INTENCLR_DMAINTERNALBUSERROR_Pos)                        /*!< Bit mask of
                                                                             *                     DMAINTERNALBUSERROR field.*/
#define QSPI_INTENCLR_DMAINTERNALBUSERROR_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMAINTERNALBUSERROR field.                */
#define QSPI_INTENCLR_DMAINTERNALBUSERROR_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMAINTERNALBUSERROR field.                */
#define QSPI_INTENCLR_DMAINTERNALBUSERROR_Clear \
    (0x1UL)                                                                 /*!< Disable                                                         */
#define QSPI_INTENCLR_DMAINTERNALBUSERROR_Disabled \
    (0x0UL)                                                                 /*!< Read: Disabled                                               */
#define QSPI_INTENCLR_DMAINTERNALBUSERROR_Enabled \
    (0x1UL)                                                                 /*!< Read: Enabled                                                 */

/* DMAABORTED @Bit 12 : Write '1' to disable interrupt for event DMAABORTED */
#define QSPI_INTENCLR_DMAABORTED_Pos (12UL)                                 /*!< Position of DMAABORTED field.                                        */
#define QSPI_INTENCLR_DMAABORTED_Msk \
    (0x1UL << QSPI_INTENCLR_DMAABORTED_Pos)                                 /*!< Bit mask of DMAABORTED field.              */
#define QSPI_INTENCLR_DMAABORTED_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMAABORTED field.                            */
#define QSPI_INTENCLR_DMAABORTED_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMAABORTED field.                            */
#define QSPI_INTENCLR_DMAABORTED_Clear \
    (0x1UL)                                                                 /*!< Disable                                                              */
#define QSPI_INTENCLR_DMAABORTED_Disabled \
    (0x0UL)                                                                 /*!< Read: Disabled                                                       */
#define QSPI_INTENCLR_DMAABORTED_Enabled \
    (0x1UL)                                                                 /*!< Read: Enabled                                                        */

/* IDLE @Bit 13 : Write '1' to disable interrupt for event IDLE */
#define QSPI_INTENCLR_IDLE_Pos      (13UL)                                  /*!< Position of IDLE field.                                              */
#define QSPI_INTENCLR_IDLE_Msk \
    (0x1UL << QSPI_INTENCLR_IDLE_Pos)                                       /*!< Bit mask of IDLE field.                                */
#define QSPI_INTENCLR_IDLE_Min      (0x0UL)                                 /*!< Min enumerator value of IDLE field.                                  */
#define QSPI_INTENCLR_IDLE_Max      (0x1UL)                                 /*!< Max enumerator value of IDLE field.                                  */
#define QSPI_INTENCLR_IDLE_Clear    (0x1UL)                                 /*!< Disable                                                              */
#define QSPI_INTENCLR_IDLE_Disabled (0x0UL)                                 /*!< Read: Disabled                                                       */
#define QSPI_INTENCLR_IDLE_Enabled  (0x1UL)                                 /*!< Read: Enabled                                                        */

/* QSPI_INTPEND: Pending interrupts */
#define QSPI_INTPEND_ResetValue \
    (0x00000000UL)                                                          /*!< Reset value of INTPEND register.                                     */

/* CORE @Bit 0 : Read pending status of interrupt for event CORE */
#define QSPI_INTPEND_CORE_Pos     (0UL)                                     /*!< Position of CORE field.                                              */
#define QSPI_INTPEND_CORE_Msk \
    (0x1UL << QSPI_INTPEND_CORE_Pos)                                        /*!< Bit mask of CORE field.                                  */
#define QSPI_INTPEND_CORE_Min     (0x0UL)                                   /*!< Min enumerator value of CORE field.                                  */
#define QSPI_INTPEND_CORE_Max     (0x1UL)                                   /*!< Max enumerator value of CORE field.                                  */
#define QSPI_INTPEND_CORE_NotPending \
    (0x0UL)                                                                 /*!< Read: Not pending                                                    */
#define QSPI_INTPEND_CORE_Pending (0x1UL)                                   /*!< Read: Pending                                                        */

/* DMADONELIST @Bit 1 : Read pending status of interrupt for event DMADONELIST */
#define QSPI_INTPEND_DMADONELIST_Pos (1UL)                                  /*!< Position of DMADONELIST field.                                       */
#define QSPI_INTPEND_DMADONELIST_Msk \
    (0x1UL << QSPI_INTPEND_DMADONELIST_Pos)                                 /*!< Bit mask of DMADONELIST field.             */
#define QSPI_INTPEND_DMADONELIST_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMADONELIST field.                           */
#define QSPI_INTPEND_DMADONELIST_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMADONELIST field.                           */
#define QSPI_INTPEND_DMADONELIST_NotPending \
    (0x0UL)                                                                 /*!< Read: Not pending                                                   */
#define QSPI_INTPEND_DMADONELIST_Pending \
    (0x1UL)                                                                 /*!< Read: Pending                                                        */

/* DMADONELISTPART @Bit 2 : Read pending status of interrupt for event DMADONELISTPART */
#define QSPI_INTPEND_DMADONELISTPART_Pos \
    (2UL)                                                                   /*!< Position of DMADONELISTPART field.                                   */
#define QSPI_INTPEND_DMADONELISTPART_Msk \
    (0x1UL << QSPI_INTPEND_DMADONELISTPART_Pos)                             /*!< Bit mask of DMADONELISTPART field. */
#define QSPI_INTPEND_DMADONELISTPART_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMADONELISTPART field.                       */
#define QSPI_INTPEND_DMADONELISTPART_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMADONELISTPART field.                       */
#define QSPI_INTPEND_DMADONELISTPART_NotPending \
    (0x0UL)                                                                 /*!< Read: Not pending                                               */
#define QSPI_INTPEND_DMADONELISTPART_Pending \
    (0x1UL)                                                                 /*!< Read: Pending                                                      */

/* DMADONESELECTJOB @Bit 3 : Read pending status of interrupt for event DMADONESELECTJOB */
#define QSPI_INTPEND_DMADONESELECTJOB_Pos \
    (3UL)                                                                   /*!< Position of DMADONESELECTJOB field.                                  */
#define QSPI_INTPEND_DMADONESELECTJOB_Msk \
    (0x1UL << QSPI_INTPEND_DMADONESELECTJOB_Pos)                            /*!< Bit mask of DMADONESELECTJOB
                                                                             *                         field.*/
#define QSPI_INTPEND_DMADONESELECTJOB_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMADONESELECTJOB field.                      */
#define QSPI_INTPEND_DMADONESELECTJOB_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMADONESELECTJOB field.                      */
#define QSPI_INTPEND_DMADONESELECTJOB_NotPending \
    (0x0UL)                                                                 /*!< Read: Not pending                                              */
#define QSPI_INTPEND_DMADONESELECTJOB_Pending \
    (0x1UL)                                                                 /*!< Read: Pending                                                     */

/* DMADONEDATA @Bit 4 : Read pending status of interrupt for event DMADONEDATA */
#define QSPI_INTPEND_DMADONEDATA_Pos (4UL)                                  /*!< Position of DMADONEDATA field.                                       */
#define QSPI_INTPEND_DMADONEDATA_Msk \
    (0x1UL << QSPI_INTPEND_DMADONEDATA_Pos)                                 /*!< Bit mask of DMADONEDATA field.             */
#define QSPI_INTPEND_DMADONEDATA_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMADONEDATA field.                           */
#define QSPI_INTPEND_DMADONEDATA_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMADONEDATA field.                           */
#define QSPI_INTPEND_DMADONEDATA_NotPending \
    (0x0UL)                                                                 /*!< Read: Not pending                                                   */
#define QSPI_INTPEND_DMADONEDATA_Pending \
    (0x1UL)                                                                 /*!< Read: Pending                                                        */

/* DMADONEJOB @Bit 5 : Read pending status of interrupt for event DMADONEJOB */
#define QSPI_INTPEND_DMADONEJOB_Pos (5UL)                                   /*!< Position of DMADONEJOB field.                                        */
#define QSPI_INTPEND_DMADONEJOB_Msk \
    (0x1UL << QSPI_INTPEND_DMADONEJOB_Pos)                                  /*!< Bit mask of DMADONEJOB field.                */
#define QSPI_INTPEND_DMADONEJOB_Min (0x0UL)                                 /*!< Min enumerator value of DMADONEJOB field.                            */
#define QSPI_INTPEND_DMADONEJOB_Max (0x1UL)                                 /*!< Max enumerator value of DMADONEJOB field.                            */
#define QSPI_INTPEND_DMADONEJOB_NotPending \
    (0x0UL)                                                                 /*!< Read: Not pending                                                    */
#define QSPI_INTPEND_DMADONEJOB_Pending \
    (0x1UL)                                                                 /*!< Read: Pending                                                        */

/* DMAERROR @Bit 6 : Read pending status of interrupt for event DMAERROR */
#define QSPI_INTPEND_DMAERROR_Pos (6UL)                                     /*!< Position of DMAERROR field.                                          */
#define QSPI_INTPEND_DMAERROR_Msk \
    (0x1UL << QSPI_INTPEND_DMAERROR_Pos)                                    /*!< Bit mask of DMAERROR field.                      */
#define QSPI_INTPEND_DMAERROR_Min (0x0UL)                                   /*!< Min enumerator value of DMAERROR field.                              */
#define QSPI_INTPEND_DMAERROR_Max (0x1UL)                                   /*!< Max enumerator value of DMAERROR field.                              */
#define QSPI_INTPEND_DMAERROR_NotPending \
    (0x0UL)                                                                 /*!< Read: Not pending                                                    */
#define QSPI_INTPEND_DMAERROR_Pending \
    (0x1UL)                                                                 /*!< Read: Pending                                                        */

/* DMAPAUSED @Bit 7 : Read pending status of interrupt for event DMAPAUSED */
#define QSPI_INTPEND_DMAPAUSED_Pos (7UL)                                    /*!< Position of DMAPAUSED field.                                         */
#define QSPI_INTPEND_DMAPAUSED_Msk \
    (0x1UL << QSPI_INTPEND_DMAPAUSED_Pos)                                   /*!< Bit mask of DMAPAUSED field.                   */
#define QSPI_INTPEND_DMAPAUSED_Min (0x0UL)                                  /*!< Min enumerator value of DMAPAUSED field.                             */
#define QSPI_INTPEND_DMAPAUSED_Max (0x1UL)                                  /*!< Max enumerator value of DMAPAUSED field.                             */
#define QSPI_INTPEND_DMAPAUSED_NotPending \
    (0x0UL)                                                                 /*!< Read: Not pending                                                    */
#define QSPI_INTPEND_DMAPAUSED_Pending \
    (0x1UL)                                                                 /*!< Read: Pending                                                        */

/* DMARESET @Bit 8 : Read pending status of interrupt for event DMARESET */
#define QSPI_INTPEND_DMARESET_Pos (8UL)                                     /*!< Position of DMARESET field.                                          */
#define QSPI_INTPEND_DMARESET_Msk \
    (0x1UL << QSPI_INTPEND_DMARESET_Pos)                                    /*!< Bit mask of DMARESET field.                      */
#define QSPI_INTPEND_DMARESET_Min (0x0UL)                                   /*!< Min enumerator value of DMARESET field.                              */
#define QSPI_INTPEND_DMARESET_Max (0x1UL)                                   /*!< Max enumerator value of DMARESET field.                              */
#define QSPI_INTPEND_DMARESET_NotPending \
    (0x0UL)                                                                 /*!< Read: Not pending                                                    */
#define QSPI_INTPEND_DMARESET_Pending \
    (0x1UL)                                                                 /*!< Read: Pending                                                        */

/* DMADONE @Bit 9 : Read pending status of interrupt for event DMADONE */
#define QSPI_INTPEND_DMADONE_Pos (9UL)                                      /*!< Position of DMADONE field.                                           */
#define QSPI_INTPEND_DMADONE_Msk \
    (0x1UL << QSPI_INTPEND_DMADONE_Pos)                                     /*!< Bit mask of DMADONE field.                         */
#define QSPI_INTPEND_DMADONE_Min (0x0UL)                                    /*!< Min enumerator value of DMADONE field.                               */
#define QSPI_INTPEND_DMADONE_Max (0x1UL)                                    /*!< Max enumerator value of DMADONE field.                               */
#define QSPI_INTPEND_DMADONE_NotPending \
    (0x0UL)                                                                 /*!< Read: Not pending                                                    */
#define QSPI_INTPEND_DMADONE_Pending \
    (0x1UL)                                                                 /*!< Read: Pending                                                        */

/* DMATXUNEXPECTEDIDLE @Bit 10 : Read pending status of interrupt for event DMATXUNEXPECTEDIDLE */
#define QSPI_INTPEND_DMATXUNEXPECTEDIDLE_Pos \
    (10UL)                                                                  /*!< Position of DMATXUNEXPECTEDIDLE field.                              */
#define QSPI_INTPEND_DMATXUNEXPECTEDIDLE_Msk \
    (0x1UL << QSPI_INTPEND_DMATXUNEXPECTEDIDLE_Pos)                         /*!< Bit mask of
                                                                             *                      DMATXUNEXPECTEDIDLE field.*/
#define QSPI_INTPEND_DMATXUNEXPECTEDIDLE_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMATXUNEXPECTEDIDLE field.                 */
#define QSPI_INTPEND_DMATXUNEXPECTEDIDLE_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMATXUNEXPECTEDIDLE field.                 */
#define QSPI_INTPEND_DMATXUNEXPECTEDIDLE_NotPending \
    (0x0UL)                                                                 /*!< Read: Not pending                                           */
#define QSPI_INTPEND_DMATXUNEXPECTEDIDLE_Pending \
    (0x1UL)                                                                 /*!< Read: Pending                                                  */

/* DMAINTERNALBUSERROR @Bit 11 : Read pending status of interrupt for event DMAINTERNALBUSERROR */
#define QSPI_INTPEND_DMAINTERNALBUSERROR_Pos \
    (11UL)                                                                  /*!< Position of DMAINTERNALBUSERROR field.                              */
#define QSPI_INTPEND_DMAINTERNALBUSERROR_Msk \
    (0x1UL << QSPI_INTPEND_DMAINTERNALBUSERROR_Pos)                         /*!< Bit mask of
                                                                             *                      DMAINTERNALBUSERROR field.*/
#define QSPI_INTPEND_DMAINTERNALBUSERROR_Min \
    (0x0UL)                                                                 /*!< Min enumerator value of DMAINTERNALBUSERROR field.                 */
#define QSPI_INTPEND_DMAINTERNALBUSERROR_Max \
    (0x1UL)                                                                 /*!< Max enumerator value of DMAINTERNALBUSERROR field.                 */
#define QSPI_INTPEND_DMAINTERNALBUSERROR_NotPending \
    (0x0UL)                                                                 /*!< Read: Not pending                                           */
#define QSPI_INTPEND_DMAINTERNALBUSERROR_Pending \
    (0x1UL)                                                                 /*!< Read: Pending                                                  */

/* DMAABORTED @Bit 12 : Read pending status of interrupt for event DMAABORTED */
#define QSPI_INTPEND_DMAABORTED_Pos (12UL)                                  /*!< Position of DMAABORTED field.                                        */
#define QSPI_INTPEND_DMAABORTED_Msk \
    (0x1UL << QSPI_INTPEND_DMAABORTED_Pos)                                  /*!< Bit mask of DMAABORTED field.                */
#define QSPI_INTPEND_DMAABORTED_Min (0x0UL)                                 /*!< Min enumerator value of DMAABORTED field.                            */
#define QSPI_INTPEND_DMAABORTED_Max (0x1UL)                                 /*!< Max enumerator value of DMAABORTED field.                            */
#define QSPI_INTPEND_DMAABORTED_NotPending \
    (0x0UL)                                                                 /*!< Read: Not pending                                                    */
#define QSPI_INTPEND_DMAABORTED_Pending \
    (0x1UL)                                                                 /*!< Read: Pending                                                        */

/* IDLE @Bit 13 : Read pending status of interrupt for event IDLE */
#define QSPI_INTPEND_IDLE_Pos     (13UL)                                    /*!< Position of IDLE field.                                              */
#define QSPI_INTPEND_IDLE_Msk \
    (0x1UL << QSPI_INTPEND_IDLE_Pos)                                        /*!< Bit mask of IDLE field.                                  */
#define QSPI_INTPEND_IDLE_Min     (0x0UL)                                   /*!< Min enumerator value of IDLE field.                                  */
#define QSPI_INTPEND_IDLE_Max     (0x1UL)                                   /*!< Max enumerator value of IDLE field.                                  */
#define QSPI_INTPEND_IDLE_NotPending \
    (0x0UL)                                                                 /*!< Read: Not pending                                                    */
#define QSPI_INTPEND_IDLE_Pending (0x1UL)                                   /*!< Read: Pending                                                        */

/* QSPI_ENABLE: Enables the QSPI This requests clock for the IP core */
#define QSPI_ENABLE_ResetValue \
    (0x00000000UL)                                                          /*!< Reset value of ENABLE register.                                      */

/* ENABLE @Bit 0 : Enable the QSPI */
#define QSPI_ENABLE_ENABLE_Pos (0UL)                                        /*!< Position of ENABLE field.                                            */
#define QSPI_ENABLE_ENABLE_Msk \
    (0x1UL << QSPI_ENABLE_ENABLE_Pos)                                       /*!< Bit mask of ENABLE field.                              */

#endif /*!< !defined(__ASSEMBLER__) && !defined(__ASSEMBLY__)                    */

#endif // NRF_SP_QSPI_H__
