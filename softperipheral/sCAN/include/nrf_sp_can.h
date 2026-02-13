/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef NRF_SP_CAN_H__
#define NRF_SP_CAN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "nrfx.h"                                     // Resolve nrfXX_types.h for the correct target to get definitions for __IO, __IOM etc.

/* ===========================================================================================================================
 * ================                                          SP_CAN                                          ================
 * ===========================================================================================================================*/

#if !defined(__ASSEMBLER__) && !defined(__ASSEMBLY__) /*!< Ignore C structs for assembly code.                                 */

/* ================================================== Struct SP_CAN_CONFIG =================================================== */
/**
 * @brief CONFIG [SP_CAN_CONFIG] SCAN configuration
 */
typedef struct
{
    __IOM uint32_t PARSING;                                                                          /*!< (@ 0x00000000) SCAN parsing options                                  */
    __IOM uint32_t TIMING;                                                                           /*!< (@ 0x00000004) SCAN Timing parameters                                */
    __IOM uint32_t REQUEST;                                                                          /*!< (@ 0x00000008) (unspecified)                                         */
    __IOM uint32_t MODE;                                                                             /*!< (@ 0x0000000C) Operation mode                                        */
} NRF_SP_CAN_CONFIG_Type;                                                                            /*!< Size = 16 (0x010)                                                    */

/* SP_CAN_CONFIG_PARSING: SCAN parsing options */
  #define SP_CAN_CONFIG_PARSING_ResetValue (0x00000000UL)                                            /*!< Reset value of PARSING register.                                */

/* REGGROUP @Bits 0..2 : Which register group to parse */
  #define SP_CAN_CONFIG_PARSING_REGGROUP_Pos           (0UL)                                         /*!< Position of REGGROUP field.                                          */
  #define SP_CAN_CONFIG_PARSING_REGGROUP_Msk           (0x7UL << SP_CAN_CONFIG_PARSING_REGGROUP_Pos) /*!< Bit mask of REGGROUP field.    */
  #define SP_CAN_CONFIG_PARSING_REGGROUP_Min           (0x0UL)                                       /*!< Min enumerator value of REGGROUP field.                              */
  #define SP_CAN_CONFIG_PARSING_REGGROUP_Max           (0x3UL)                                       /*!< Max enumerator value of REGGROUP field.                              */
  #define SP_CAN_CONFIG_PARSING_REGGROUP_None          (0x0UL)                                       /*!< No parsing                                                          */
  #define SP_CAN_CONFIG_PARSING_REGGROUP_Timing        (0x1UL)                                       /*!< Parse the timing registers                                        */
  #define SP_CAN_CONFIG_PARSING_REGGROUP_OperationMode (0x2UL)                                       /*!< Parse the operation mode                                   */
  #define SP_CAN_CONFIG_PARSING_REGGROUP_RxFilters     (0x3UL)                                       /*!< Parse the RX Filters                                           */

/* SP_CAN_CONFIG_TIMING: SCAN Timing parameters */
  #define SP_CAN_CONFIG_TIMING_ResetValue (0x00000000UL)                                             /*!< Reset value of TIMING register.                                  */

/* RSYNCJUMPWIDTH @Bits 0..7 : Re-synchronization jump width */
  #define SP_CAN_CONFIG_TIMING_RSYNCJUMPWIDTH_Pos    (0UL)                                           /*!< Position of RSYNCJUMPWIDTH field.                                 */
  #define SP_CAN_CONFIG_TIMING_RSYNCJUMPWIDTH_Msk                                                        \
                                                     (0xFFUL << SP_CAN_CONFIG_TIMING_RSYNCJUMPWIDTH_Pos) \
                                                                                                     /*!< Bit mask of
                                                                                                      * RSYNCJUMPWIDTH field.*/

/* PHASESEG1 @Bits 8..15 : Number of time quanta in phase 1 */
  #define SP_CAN_CONFIG_TIMING_PHASESEG1_Pos (8UL)                                                   /*!< Position of PHASESEG1 field.                                         */
  #define SP_CAN_CONFIG_TIMING_PHASESEG1_Msk (0xFFUL << SP_CAN_CONFIG_TIMING_PHASESEG1_Pos)          /*!< Bit mask of PHASESEG1 field.  */

/* PHASESEG2 @Bits 16..23 : Number of time quanta in phase 2 */
  #define SP_CAN_CONFIG_TIMING_PHASESEG2_Pos (16UL)                                                  /*!< Position of PHASESEG2 field.                                         */
  #define SP_CAN_CONFIG_TIMING_PHASESEG2_Msk (0xFFUL << SP_CAN_CONFIG_TIMING_PHASESEG2_Pos)          /*!< Bit mask of PHASESEG2 field.  */

/* PRESCALER @Bits 24..31 : Data Bit Rate Prescaler */
  #define SP_CAN_CONFIG_TIMING_PRESCALER_Pos (24UL)                                                  /*!< Position of PRESCALER field.                                         */
  #define SP_CAN_CONFIG_TIMING_PRESCALER_Msk (0xFFUL << SP_CAN_CONFIG_TIMING_PRESCALER_Pos)          /*!< Bit mask of PRESCALER field.  */

/* SP_CAN_CONFIG_REQUEST: (unspecified) */
  #define SP_CAN_CONFIG_REQUEST_ResetValue (0x00000000UL)                                            /*!< Reset value of REQUEST register.                                */

/* REQUEST @Bit 0 : Request either TX or RX */
  #define SP_CAN_CONFIG_REQUEST_REQUEST_Pos (0UL)                                                    /*!< Position of REQUEST field.                                           */
  #define SP_CAN_CONFIG_REQUEST_REQUEST_Msk (0x1UL << SP_CAN_CONFIG_REQUEST_REQUEST_Pos)             /*!< Bit mask of REQUEST field.       */
  #define SP_CAN_CONFIG_REQUEST_REQUEST_Min (0x0UL)                                                  /*!< Min enumerator value of REQUEST field.                               */
  #define SP_CAN_CONFIG_REQUEST_REQUEST_Max (0x1UL)                                                  /*!< Max enumerator value of REQUEST field.                               */
  #define SP_CAN_CONFIG_REQUEST_REQUEST_RX  (0x0UL)                                                  /*!< Only listen                                                          */
  #define SP_CAN_CONFIG_REQUEST_REQUEST_TX  (0x1UL)                                                  /*!< Attempt TX                                                           */

/* SP_CAN_CONFIG_MODE: Operation mode */
  #define SP_CAN_CONFIG_MODE_ResetValue (0x00000000UL)                                               /*!< Reset value of MODE register.                                      */

/* MODE @Bits 0..1 : Operation mode */
  #define SP_CAN_CONFIG_MODE_MODE_Pos        (0UL)                                                   /*!< Position of MODE field.                                              */
  #define SP_CAN_CONFIG_MODE_MODE_Msk        (0x3UL << SP_CAN_CONFIG_MODE_MODE_Pos)                  /*!< Bit mask of MODE field.                      */
  #define SP_CAN_CONFIG_MODE_MODE_Min        (0x0UL)                                                 /*!< Min enumerator value of MODE field.                                  */
  #define SP_CAN_CONFIG_MODE_MODE_Max        (0x3UL)                                                 /*!< Max enumerator value of MODE field.                                  */
  #define SP_CAN_CONFIG_MODE_MODE_Normal     (0x0UL)                                                 /*!< Normal mode                                                          */
  #define SP_CAN_CONFIG_MODE_MODE_Loopback   (0x1UL)                                                 /*!< Loopback mode                                                        */
  #define SP_CAN_CONFIG_MODE_MODE_ListenOnly (0x2UL)                                                 /*!< Listen only mode                                                     */
  #define SP_CAN_CONFIG_MODE_MODE_OneShot    (0x3UL)                                                 /*!< One shot mode                                                        */

/* ================================================= Struct SP_CAN_RXFILTER ================================================== */
/**
 * @brief RXFILTER [SP_CAN_RXFILTER] sCAN RX filters
 */
typedef struct
{
    __IOM uint32_t IDFILTER;                                                                /*!< (@ 0x00000000) ID filter for RX                                      */
    __IOM uint32_t IDMASK;                                                                  /*!< (@ 0x00000004) ID mask filter for RX                                 */
} NRF_SP_CAN_RXFILTER_Type;                                                                 /*!< Size = 8 (0x008)                                                     */
  #define SP_CAN_RXFILTER_MaxCount (4UL)                                                    /*!< Size of RXFILTER[4] array.                                           */
  #define SP_CAN_RXFILTER_MaxIndex (3UL)                                                    /*!< Max index of RXFILTER[4] array.                                      */
  #define SP_CAN_RXFILTER_MinIndex (0UL)                                                    /*!< Min index of RXFILTER[4] array.                                      */

/* SP_CAN_RXFILTER_IDFILTER: ID filter for RX */
  #define SP_CAN_RXFILTER_IDFILTER_ResetValue (0x00000000UL)                                /*!< Reset value of IDFILTER register.                            */

/* IDENTIFIER @Bits 0..28 : ID to match for RX */
  #define SP_CAN_RXFILTER_IDFILTER_IDENTIFIER_Pos    (0UL)                                  /*!< Position of IDENTIFIER field.                                     */
  #define SP_CAN_RXFILTER_IDFILTER_IDENTIFIER_Msk                                                              \
                                                     (0x1FFFFFFFUL << SP_CAN_RXFILTER_IDFILTER_IDENTIFIER_Pos) \
                                                                                            /*!< Bit mask of
                                                                                             * IDENTIFIER field.*/

/* USEFILTER @Bit 29 : Enable the filter to match against a received message */
  #define SP_CAN_RXFILTER_IDFILTER_USEFILTER_Pos         (29UL)                             /*!< Position of USEFILTER field.                                      */
  #define SP_CAN_RXFILTER_IDFILTER_USEFILTER_Msk                                                           \
                                                         (0x1UL << SP_CAN_RXFILTER_IDFILTER_USEFILTER_Pos) \
                                                                                            /*!< Bit mask of USEFILTER
                                                                                             * field.*/
  #define SP_CAN_RXFILTER_IDFILTER_USEFILTER_Min         (0x0UL)                            /*!< Min enumerator value of USEFILTER field.                         */
  #define SP_CAN_RXFILTER_IDFILTER_USEFILTER_Max         (0x1UL)                            /*!< Max enumerator value of USEFILTER field.                         */
  #define SP_CAN_RXFILTER_IDFILTER_USEFILTER_Disabled    (0x0UL)                            /*!< Disable                                                     */
  #define SP_CAN_RXFILTER_IDFILTER_USEFILTER_Enabled     (0x1UL)                            /*!< Enable                                                       */

/* INMAILBOX @Bit 30 : sCAN driver has put the data in a mailbox */
  #define SP_CAN_RXFILTER_IDFILTER_INMAILBOX_Pos         (30UL)                             /*!< Position of INMAILBOX field.                                      */
  #define SP_CAN_RXFILTER_IDFILTER_INMAILBOX_Msk                                                           \
                                                         (0x1UL << SP_CAN_RXFILTER_IDFILTER_INMAILBOX_Pos) \
                                                                                            /*!< Bit mask of INMAILBOX
                                                                                             * field.*/
  #define SP_CAN_RXFILTER_IDFILTER_INMAILBOX_Min         (0x0UL)                            /*!< Min enumerator value of INMAILBOX field.                         */
  #define SP_CAN_RXFILTER_IDFILTER_INMAILBOX_Max         (0x1UL)                            /*!< Max enumerator value of INMAILBOX field.                         */
  #define SP_CAN_RXFILTER_IDFILTER_INMAILBOX_Disabled    (0x0UL)                            /*!< Disable                                                     */
  #define SP_CAN_RXFILTER_IDFILTER_INMAILBOX_Enabled     (0x1UL)                            /*!< Enable                                                       */

/* FILTERMATCHED @Bit 31 : Indicates that the filter has been matched */
  #define SP_CAN_RXFILTER_IDFILTER_FILTERMATCHED_Pos        (31UL)                          /*!< Position of FILTERMATCHED field.                              */
  #define SP_CAN_RXFILTER_IDFILTER_FILTERMATCHED_Msk                                                              \
                                                            (0x1UL << SP_CAN_RXFILTER_IDFILTER_FILTERMATCHED_Pos) \
                                                                                            /*!< Bit mask of
                                                                                             * FILTERMATCHED field.*/
  #define SP_CAN_RXFILTER_IDFILTER_FILTERMATCHED_Min        (0x0UL)                         /*!< Min enumerator value of FILTERMATCHED field.                 */
  #define SP_CAN_RXFILTER_IDFILTER_FILTERMATCHED_Max        (0x1UL)                         /*!< Max enumerator value of FILTERMATCHED field.                 */
  #define SP_CAN_RXFILTER_IDFILTER_FILTERMATCHED_NoMatch    (0x0UL)                         /*!< No match                                                 */
  #define SP_CAN_RXFILTER_IDFILTER_FILTERMATCHED_Match      (0x1UL)                         /*!< Match                                                      */

/* SP_CAN_RXFILTER_IDMASK: ID mask filter for RX */
  #define SP_CAN_RXFILTER_IDMASK_ResetValue (0x00000000UL)                                  /*!< Reset value of IDMASK register.                                */

/* MASK @Bits 0..28 : ID mask filter for RX */
  #define SP_CAN_RXFILTER_IDMASK_MASK_Pos (0UL)                                             /*!< Position of MASK field.                                              */
  #define SP_CAN_RXFILTER_IDMASK_MASK_Msk (0x1FFFFFFFUL << SP_CAN_RXFILTER_IDMASK_MASK_Pos) /*!< Bit mask of MASK field.       */

/* ================================================== Struct SP_CAN_TXFRAME ================================================== */
/**
 * @brief TXFRAME [SP_CAN_TXFRAME] sCAN frame configuration
 */
typedef struct
{
    __IOM uint32_t IDENTIFIER;                                                                     /*!< (@ 0x00000000) Frame Identifier                                      */
    __IOM uint32_t TXDATAL;                                                                        /*!< (@ 0x00000004) TX data buffer LSB                                    */
    __IOM uint32_t TXDATAH;                                                                        /*!< (@ 0x00000008) TX data buffer MSB                                    */
    __IOM uint32_t METADATA;                                                                       /*!< (@ 0x0000000C) Frame's metadata                                      */
} NRF_SP_CAN_TXFRAME_Type;                                                                         /*!< Size = 16 (0x010)                                                    */
  #define SP_CAN_TXFRAME_MaxCount (4UL)                                                            /*!< Size of TXFRAME[4] array.                                            */
  #define SP_CAN_TXFRAME_MaxIndex (3UL)                                                            /*!< Max index of TXFRAME[4] array.                                       */
  #define SP_CAN_TXFRAME_MinIndex (0UL)                                                            /*!< Min index of TXFRAME[4] array.                                       */

/* SP_CAN_TXFRAME_IDENTIFIER: Frame Identifier */
  #define SP_CAN_TXFRAME_IDENTIFIER_ResetValue (0x00000000UL)                                      /*!< Reset value of IDENTIFIER register.                         */

/* IDENTIFIER @Bits 0..28 : Frame Identifier */
  #define SP_CAN_TXFRAME_IDENTIFIER_IDENTIFIER_Pos    (0UL)                                        /*!< Position of IDENTIFIER field.                                    */
  #define SP_CAN_TXFRAME_IDENTIFIER_IDENTIFIER_Msk                                                               \
                                                      (0x1FFFFFFFUL << SP_CAN_TXFRAME_IDENTIFIER_IDENTIFIER_Pos) \
                                                                                                   /*!< Bit mask of
                                                                                                    * IDENTIFIER field.*/

/* SP_CAN_TXFRAME_TXDATAL: TX data buffer LSB */
  #define SP_CAN_TXFRAME_TXDATAL_ResetValue (0x00000000UL)                                         /*!< Reset value of TXDATAL register.                               */

/* TXDATA0 @Bits 0..7 : Data */
  #define SP_CAN_TXFRAME_TXDATAL_TXDATA0_Pos (0UL)                                                 /*!< Position of TXDATA0 field.                                           */
  #define SP_CAN_TXFRAME_TXDATAL_TXDATA0_Msk (0xFFUL << SP_CAN_TXFRAME_TXDATAL_TXDATA0_Pos)        /*!< Bit mask of TXDATA0 field.    */

/* TXDATA1 @Bits 8..15 : Data */
  #define SP_CAN_TXFRAME_TXDATAL_TXDATA1_Pos (8UL)                                                 /*!< Position of TXDATA1 field.                                           */
  #define SP_CAN_TXFRAME_TXDATAL_TXDATA1_Msk (0xFFUL << SP_CAN_TXFRAME_TXDATAL_TXDATA1_Pos)        /*!< Bit mask of TXDATA1 field.    */

/* TXDATA2 @Bits 16..23 : Data */
  #define SP_CAN_TXFRAME_TXDATAL_TXDATA2_Pos (16UL)                                                /*!< Position of TXDATA2 field.                                           */
  #define SP_CAN_TXFRAME_TXDATAL_TXDATA2_Msk (0xFFUL << SP_CAN_TXFRAME_TXDATAL_TXDATA2_Pos)        /*!< Bit mask of TXDATA2 field.    */

/* TXDATA3 @Bits 24..31 : Data */
  #define SP_CAN_TXFRAME_TXDATAL_TXDATA3_Pos (24UL)                                                /*!< Position of TXDATA3 field.                                           */
  #define SP_CAN_TXFRAME_TXDATAL_TXDATA3_Msk (0xFFUL << SP_CAN_TXFRAME_TXDATAL_TXDATA3_Pos)        /*!< Bit mask of TXDATA3 field.    */

/* SP_CAN_TXFRAME_TXDATAH: TX data buffer MSB */
  #define SP_CAN_TXFRAME_TXDATAH_ResetValue (0x00000000UL)                                         /*!< Reset value of TXDATAH register.                               */

/* TXDATA4 @Bits 0..7 : Data */
  #define SP_CAN_TXFRAME_TXDATAH_TXDATA4_Pos (0UL)                                                 /*!< Position of TXDATA4 field.                                           */
  #define SP_CAN_TXFRAME_TXDATAH_TXDATA4_Msk (0xFFUL << SP_CAN_TXFRAME_TXDATAH_TXDATA4_Pos)        /*!< Bit mask of TXDATA4 field.    */

/* TXDATA5 @Bits 8..15 : Data */
  #define SP_CAN_TXFRAME_TXDATAH_TXDATA5_Pos (8UL)                                                 /*!< Position of TXDATA5 field.                                           */
  #define SP_CAN_TXFRAME_TXDATAH_TXDATA5_Msk (0xFFUL << SP_CAN_TXFRAME_TXDATAH_TXDATA5_Pos)        /*!< Bit mask of TXDATA5 field.    */

/* TXDATA6 @Bits 16..23 : Data */
  #define SP_CAN_TXFRAME_TXDATAH_TXDATA6_Pos (16UL)                                                /*!< Position of TXDATA6 field.                                           */
  #define SP_CAN_TXFRAME_TXDATAH_TXDATA6_Msk (0xFFUL << SP_CAN_TXFRAME_TXDATAH_TXDATA6_Pos)        /*!< Bit mask of TXDATA6 field.    */

/* TXDATA7 @Bits 24..31 : Data */
  #define SP_CAN_TXFRAME_TXDATAH_TXDATA7_Pos (24UL)                                                /*!< Position of TXDATA7 field.                                           */
  #define SP_CAN_TXFRAME_TXDATAH_TXDATA7_Msk (0xFFUL << SP_CAN_TXFRAME_TXDATAH_TXDATA7_Pos)        /*!< Bit mask of TXDATA7 field.    */

/* SP_CAN_TXFRAME_METADATA: Frame's metadata */
  #define SP_CAN_TXFRAME_METADATA_ResetValue (0x00000000UL)                                        /*!< Reset value of METADATA register.                             */

/* EXTENDEDFORMAT @Bit 0 : Frame's IDE (Identifier Extension bit) */
  #define SP_CAN_TXFRAME_METADATA_EXTENDEDFORMAT_Pos         (0UL)                                 /*!< Position of EXTENDEDFORMAT field.                              */
  #define SP_CAN_TXFRAME_METADATA_EXTENDEDFORMAT_Msk                                                               \
                                                             (0x1UL << SP_CAN_TXFRAME_METADATA_EXTENDEDFORMAT_Pos) \
                                                                                                   /*!< Bit mask of
                                                                                                    * EXTENDEDFORMAT field.*/
  #define SP_CAN_TXFRAME_METADATA_EXTENDEDFORMAT_Min         (0x0UL)                               /*!< Min enumerator value of EXTENDEDFORMAT field.                */
  #define SP_CAN_TXFRAME_METADATA_EXTENDEDFORMAT_Max         (0x1UL)                               /*!< Max enumerator value of EXTENDEDFORMAT field.                */
  #define SP_CAN_TXFRAME_METADATA_EXTENDEDFORMAT_Disabled    (0x0UL)                               /*!< Disable                                                 */
  #define SP_CAN_TXFRAME_METADATA_EXTENDEDFORMAT_Enabled     (0x1UL)                               /*!< Enable                                                   */

/* REMOTEREQUEST @Bit 1 : Frame's RTR (Remote Transmission request bit) */
  #define SP_CAN_TXFRAME_METADATA_REMOTEREQUEST_Pos         (1UL)                                  /*!< Position of REMOTEREQUEST field.                                */
  #define SP_CAN_TXFRAME_METADATA_REMOTEREQUEST_Msk                                                              \
                                                            (0x1UL << SP_CAN_TXFRAME_METADATA_REMOTEREQUEST_Pos) \
                                                                                                   /*!< Bit mask of
                                                                                                    * REMOTEREQUEST field.*/
  #define SP_CAN_TXFRAME_METADATA_REMOTEREQUEST_Min         (0x0UL)                                /*!< Min enumerator value of REMOTEREQUEST field.                  */
  #define SP_CAN_TXFRAME_METADATA_REMOTEREQUEST_Max         (0x1UL)                                /*!< Max enumerator value of REMOTEREQUEST field.                  */
  #define SP_CAN_TXFRAME_METADATA_REMOTEREQUEST_Disabled    (0x0UL)                                /*!< Disable                                                  */
  #define SP_CAN_TXFRAME_METADATA_REMOTEREQUEST_Enabled     (0x1UL)                                /*!< Enable                                                    */

/* DATALENGTH @Bits 2..5 : Frame's DLC (Data Length Code) */
  #define SP_CAN_TXFRAME_METADATA_DATALENGTH_Pos (2UL)                                             /*!< Position of DATALENGTH field.                                      */
  #define SP_CAN_TXFRAME_METADATA_DATALENGTH_Msk (0xFUL << SP_CAN_TXFRAME_METADATA_DATALENGTH_Pos) /*!< Bit mask of DATALENGTH
                                                                                                    * field.*/

/* CRC @Bits 16..30 : Frame's CRC */
  #define SP_CAN_TXFRAME_METADATA_CRC_Pos (16UL)                                                   /*!< Position of CRC field.                                               */
  #define SP_CAN_TXFRAME_METADATA_CRC_Msk (0x7FFFUL << SP_CAN_TXFRAME_METADATA_CRC_Pos)            /*!< Bit mask of CRC field.            */

/* ================================================== Struct SP_CAN_RXFRAME ================================================== */
/**
 * @brief RXFRAME [SP_CAN_RXFRAME] sCAN frame configuration
 */
typedef struct
{
    __IOM uint32_t IDENTIFIER;                                                                     /*!< (@ 0x00000000) Frame Identifier                                      */
    __IOM uint32_t RXDATAL;                                                                        /*!< (@ 0x00000004) RX data buffer LSB                                    */
    __IOM uint32_t RXDATAH;                                                                        /*!< (@ 0x00000008) RX data buffer MSB                                    */
    __IOM uint32_t METADATA;                                                                       /*!< (@ 0x0000000C) Frame's metadata                                      */
} NRF_SP_CAN_RXFRAME_Type;                                                                         /*!< Size = 16 (0x010)                                                    */
  #define SP_CAN_RXFRAME_MaxCount (4UL)                                                            /*!< Size of RXFRAME[4] array.                                            */
  #define SP_CAN_RXFRAME_MaxIndex (3UL)                                                            /*!< Max index of RXFRAME[4] array.                                       */
  #define SP_CAN_RXFRAME_MinIndex (0UL)                                                            /*!< Min index of RXFRAME[4] array.                                       */

/* SP_CAN_RXFRAME_IDENTIFIER: Frame Identifier */
  #define SP_CAN_RXFRAME_IDENTIFIER_ResetValue (0x00000000UL)                                      /*!< Reset value of IDENTIFIER register.                         */

/* IDENTIFIER @Bits 0..28 : Frame Identifier */
  #define SP_CAN_RXFRAME_IDENTIFIER_IDENTIFIER_Pos    (0UL)                                        /*!< Position of IDENTIFIER field.                                    */
  #define SP_CAN_RXFRAME_IDENTIFIER_IDENTIFIER_Msk                                                               \
                                                      (0x1FFFFFFFUL << SP_CAN_RXFRAME_IDENTIFIER_IDENTIFIER_Pos) \
                                                                                                   /*!< Bit mask of
                                                                                                    * IDENTIFIER field.*/

/* SP_CAN_RXFRAME_RXDATAL: RX data buffer LSB */
  #define SP_CAN_RXFRAME_RXDATAL_ResetValue (0x00000000UL)                                         /*!< Reset value of RXDATAL register.                               */

/* RXDATA0 @Bits 0..7 : Data */
  #define SP_CAN_RXFRAME_RXDATAL_RXDATA0_Pos (0UL)                                                 /*!< Position of RXDATA0 field.                                           */
  #define SP_CAN_RXFRAME_RXDATAL_RXDATA0_Msk (0xFFUL << SP_CAN_RXFRAME_RXDATAL_RXDATA0_Pos)        /*!< Bit mask of RXDATA0 field.    */

/* RXDATA1 @Bits 8..15 : Data */
  #define SP_CAN_RXFRAME_RXDATAL_RXDATA1_Pos (8UL)                                                 /*!< Position of RXDATA1 field.                                           */
  #define SP_CAN_RXFRAME_RXDATAL_RXDATA1_Msk (0xFFUL << SP_CAN_RXFRAME_RXDATAL_RXDATA1_Pos)        /*!< Bit mask of RXDATA1 field.    */

/* RXDATA2 @Bits 16..23 : Data */
  #define SP_CAN_RXFRAME_RXDATAL_RXDATA2_Pos (16UL)                                                /*!< Position of RXDATA2 field.                                           */
  #define SP_CAN_RXFRAME_RXDATAL_RXDATA2_Msk (0xFFUL << SP_CAN_RXFRAME_RXDATAL_RXDATA2_Pos)        /*!< Bit mask of RXDATA2 field.    */

/* RXDATA3 @Bits 24..31 : Data */
  #define SP_CAN_RXFRAME_RXDATAL_RXDATA3_Pos (24UL)                                                /*!< Position of RXDATA3 field.                                           */
  #define SP_CAN_RXFRAME_RXDATAL_RXDATA3_Msk (0xFFUL << SP_CAN_RXFRAME_RXDATAL_RXDATA3_Pos)        /*!< Bit mask of RXDATA3 field.    */

/* SP_CAN_RXFRAME_RXDATAH: RX data buffer MSB */
  #define SP_CAN_RXFRAME_RXDATAH_ResetValue (0x00000000UL)                                         /*!< Reset value of RXDATAH register.                               */

/* RXDATA4 @Bits 0..7 : Data */
  #define SP_CAN_RXFRAME_RXDATAH_RXDATA4_Pos (0UL)                                                 /*!< Position of RXDATA4 field.                                           */
  #define SP_CAN_RXFRAME_RXDATAH_RXDATA4_Msk (0xFFUL << SP_CAN_RXFRAME_RXDATAH_RXDATA4_Pos)        /*!< Bit mask of RXDATA4 field.    */

/* RXDATA5 @Bits 8..15 : Data */
  #define SP_CAN_RXFRAME_RXDATAH_RXDATA5_Pos (8UL)                                                 /*!< Position of RXDATA5 field.                                           */
  #define SP_CAN_RXFRAME_RXDATAH_RXDATA5_Msk (0xFFUL << SP_CAN_RXFRAME_RXDATAH_RXDATA5_Pos)        /*!< Bit mask of RXDATA5 field.    */

/* RXDATA6 @Bits 16..23 : Data */
  #define SP_CAN_RXFRAME_RXDATAH_RXDATA6_Pos (16UL)                                                /*!< Position of RXDATA6 field.                                           */
  #define SP_CAN_RXFRAME_RXDATAH_RXDATA6_Msk (0xFFUL << SP_CAN_RXFRAME_RXDATAH_RXDATA6_Pos)        /*!< Bit mask of RXDATA6 field.    */

/* RXDATA7 @Bits 24..31 : Data */
  #define SP_CAN_RXFRAME_RXDATAH_RXDATA7_Pos (24UL)                                                /*!< Position of RXDATA7 field.                                           */
  #define SP_CAN_RXFRAME_RXDATAH_RXDATA7_Msk (0xFFUL << SP_CAN_RXFRAME_RXDATAH_RXDATA7_Pos)        /*!< Bit mask of RXDATA7 field.    */

/* SP_CAN_RXFRAME_METADATA: Frame's metadata */
  #define SP_CAN_RXFRAME_METADATA_ResetValue (0x00000000UL)                                        /*!< Reset value of METADATA register.                             */

/* EXTENDEDFORMAT @Bit 0 : Frame's IDE (Identifier Extension bit) */
  #define SP_CAN_RXFRAME_METADATA_EXTENDEDFORMAT_Pos         (0UL)                                 /*!< Position of EXTENDEDFORMAT field.                              */
  #define SP_CAN_RXFRAME_METADATA_EXTENDEDFORMAT_Msk                                                               \
                                                             (0x1UL << SP_CAN_RXFRAME_METADATA_EXTENDEDFORMAT_Pos) \
                                                                                                   /*!< Bit mask of
                                                                                                    * EXTENDEDFORMAT field.*/
  #define SP_CAN_RXFRAME_METADATA_EXTENDEDFORMAT_Min         (0x0UL)                               /*!< Min enumerator value of EXTENDEDFORMAT field.                */
  #define SP_CAN_RXFRAME_METADATA_EXTENDEDFORMAT_Max         (0x1UL)                               /*!< Max enumerator value of EXTENDEDFORMAT field.                */
  #define SP_CAN_RXFRAME_METADATA_EXTENDEDFORMAT_Disabled    (0x0UL)                               /*!< Disable                                                 */
  #define SP_CAN_RXFRAME_METADATA_EXTENDEDFORMAT_Enabled     (0x1UL)                               /*!< Enable                                                   */

/* REMOTEREQUEST @Bit 1 : Frame's RTR (Remote Transmission request bit) */
  #define SP_CAN_RXFRAME_METADATA_REMOTEREQUEST_Pos         (1UL)                                  /*!< Position of REMOTEREQUEST field.                                */
  #define SP_CAN_RXFRAME_METADATA_REMOTEREQUEST_Msk                                                              \
                                                            (0x1UL << SP_CAN_RXFRAME_METADATA_REMOTEREQUEST_Pos) \
                                                                                                   /*!< Bit mask of
                                                                                                    * REMOTEREQUEST field.*/
  #define SP_CAN_RXFRAME_METADATA_REMOTEREQUEST_Min         (0x0UL)                                /*!< Min enumerator value of REMOTEREQUEST field.                  */
  #define SP_CAN_RXFRAME_METADATA_REMOTEREQUEST_Max         (0x1UL)                                /*!< Max enumerator value of REMOTEREQUEST field.                  */
  #define SP_CAN_RXFRAME_METADATA_REMOTEREQUEST_Disabled    (0x0UL)                                /*!< Disable                                                  */
  #define SP_CAN_RXFRAME_METADATA_REMOTEREQUEST_Enabled     (0x1UL)                                /*!< Enable                                                    */

/* DATALENGTH @Bits 2..5 : Frame's DLC (Data Length Code) */
  #define SP_CAN_RXFRAME_METADATA_DATALENGTH_Pos (2UL)                                             /*!< Position of DATALENGTH field.                                      */
  #define SP_CAN_RXFRAME_METADATA_DATALENGTH_Msk (0xFUL << SP_CAN_RXFRAME_METADATA_DATALENGTH_Pos) /*!< Bit mask of DATALENGTH
                                                                                                    * field.*/

/* CRC @Bits 16..30 : Frame's CRC */
  #define SP_CAN_RXFRAME_METADATA_CRC_Pos (16UL)                                                   /*!< Position of CRC field.                                               */
  #define SP_CAN_RXFRAME_METADATA_CRC_Msk (0x7FFFUL << SP_CAN_RXFRAME_METADATA_CRC_Pos)            /*!< Bit mask of CRC field.            */

/* ================================================== Struct SP_CAN_STATUS =================================================== */
/**
 * @brief STATUS [SP_CAN_STATUS] SCAN status
 */
typedef struct
{
    __IOM uint32_t STATUS;                                                                         /*!< (@ 0x00000000) SCAN status register                                  */
} NRF_SP_CAN_STATUS_Type;                                                                          /*!< Size = 4 (0x004)                                                     */

/* SP_CAN_STATUS_STATUS: SCAN status register */
  #define SP_CAN_STATUS_STATUS_ResetValue (0x80000000UL)                                           /*!< Reset value of STATUS register.                                  */

/* BITERROR @Bit 0 : Transmitter sends dominant/recessive but reads back recessive/dominant. Unable to transmit
 *                   dominant/recessive */

  #define SP_CAN_STATUS_STATUS_BITERROR_Pos (0UL)                                                  /*!< Position of BITERROR field.                                          */
  #define SP_CAN_STATUS_STATUS_BITERROR_Msk (0x1UL << SP_CAN_STATUS_STATUS_BITERROR_Pos)           /*!< Bit mask of BITERROR field.      */

/* BITSTUFFINGERROR @Bit 1 : Receiver detects a sequence of 6 dominant or recessive bits */
  #define SP_CAN_STATUS_STATUS_BITSTUFFINGERROR_Pos    (1UL)                                       /*!< Position of BITSTUFFINGERROR field.                             */
  #define SP_CAN_STATUS_STATUS_BITSTUFFINGERROR_Msk                                                         \
                                                       (0x1UL << SP_CAN_STATUS_STATUS_BITSTUFFINGERROR_Pos) \
                                                                                                   /*!< Bit mask of
                                                                                                    * BITSTUFFINGERROR field.*/

/* FORMERROR @Bit 2 : Receiver detects a bit with invalid logical level in SOF/EOF fields or ACK/CRC delimiters */
  #define SP_CAN_STATUS_STATUS_FORMERROR_Pos (2UL)                                                 /*!< Position of FORMERROR field.                                         */
  #define SP_CAN_STATUS_STATUS_FORMERROR_Msk (0x1UL << SP_CAN_STATUS_STATUS_FORMERROR_Pos)         /*!< Bit mask of FORMERROR field.   */

/* ACKERROR @Bit 3 : Transmitter sends a message but ACK slot is not maide dominant by receiver(s) */
  #define SP_CAN_STATUS_STATUS_ACKERROR_Pos (3UL)                                                  /*!< Position of ACKERROR field.                                          */
  #define SP_CAN_STATUS_STATUS_ACKERROR_Msk (0x1UL << SP_CAN_STATUS_STATUS_ACKERROR_Pos)           /*!< Bit mask of ACKERROR field.      */

/* CRCERROR @Bit 4 : Receiver calculates a CRC that differs fomr the transmitted CRC field value */
  #define SP_CAN_STATUS_STATUS_CRCERROR_Pos (4UL)                                                  /*!< Position of CRCERROR field.                                          */
  #define SP_CAN_STATUS_STATUS_CRCERROR_Msk (0x1UL << SP_CAN_STATUS_STATUS_CRCERROR_Pos)           /*!< Bit mask of CRCERROR field.      */

/* BIT0ERROR @Bit 5 : Transmitter sends dominant but reads back recessive. Unable to transmit dominant */
  #define SP_CAN_STATUS_STATUS_BIT0ERROR_Pos (5UL)                                                 /*!< Position of BIT0ERROR field.                                         */
  #define SP_CAN_STATUS_STATUS_BIT0ERROR_Msk (0x1UL << SP_CAN_STATUS_STATUS_BIT0ERROR_Pos)         /*!< Bit mask of BIT0ERROR field.   */

/* BIT1ERROR @Bit 6 : Transmitter sends recessive but reads back dominant. Unable to transmit recessive */
  #define SP_CAN_STATUS_STATUS_BIT1ERROR_Pos (6UL)                                                 /*!< Position of BIT1ERROR field.                                         */
  #define SP_CAN_STATUS_STATUS_BIT1ERROR_Msk (0x1UL << SP_CAN_STATUS_STATUS_BIT1ERROR_Pos)         /*!< Bit mask of BIT1ERROR field.   */

/* PROTOCOLERROR @Bit 7 : Generic error response */
  #define SP_CAN_STATUS_STATUS_PROTOCOLERROR_Pos (7UL)                                             /*!< Position of PROTOCOLERROR field.                                   */
  #define SP_CAN_STATUS_STATUS_PROTOCOLERROR_Msk (0x1UL << SP_CAN_STATUS_STATUS_PROTOCOLERROR_Pos) /*!< Bit mask of
                                                                                                    * PROTOCOLERROR field.*/

/* ARBITRATIONLOST @Bit 8 : TX attempt lost arbitration */
  #define SP_CAN_STATUS_STATUS_ARBITRATIONLOST_Pos    (8UL)                                        /*!< Position of ARBITRATIONLOST field.                               */
  #define SP_CAN_STATUS_STATUS_ARBITRATIONLOST_Msk                                                        \
                                                      (0x1UL << SP_CAN_STATUS_STATUS_ARBITRATIONLOST_Pos) \
                                                                                                   /*!< Bit mask of
                                                                                                    * ARBITRATIONLOST field.*/

/* ARBITRATIONLOSTRXOK @Bit 9 : TX attempt lost arbitration, but there was a match for rx filter */
  #define SP_CAN_STATUS_STATUS_ARBITRATIONLOSTRXOK_Pos    (9UL)                                    /*!< Position of ARBITRATIONLOSTRXOK field.                       */
  #define SP_CAN_STATUS_STATUS_ARBITRATIONLOSTRXOK_Msk                                                            \
                                                          (0x1UL << SP_CAN_STATUS_STATUS_ARBITRATIONLOSTRXOK_Pos) \
                                                                                                   /*!< Bit mask of
                                                                                                    * ARBITRATIONLOSTRXOK field.*/

/* STATE @Bits 29..31 : sCAN state */
  #define SP_CAN_STATUS_STATUS_STATE_Pos          (29UL)                                           /*!< Position of STATE field.                                             */
  #define SP_CAN_STATUS_STATUS_STATE_Msk          (0x7UL << SP_CAN_STATUS_STATUS_STATE_Pos)        /*!< Bit mask of STATE field.               */
  #define SP_CAN_STATUS_STATUS_STATE_Min          (0x0UL)                                          /*!< Min enumerator value of STATE field.                                 */
  #define SP_CAN_STATUS_STATUS_STATE_Max          (0x4UL)                                          /*!< Max enumerator value of STATE field.                                 */
  #define SP_CAN_STATUS_STATUS_STATE_ERRORACTIVE  (0x0UL)                                          /*!< Error-active state (REC/TEC < 96)                                */
  #define SP_CAN_STATUS_STATUS_STATE_ERRORWARNING (0x1UL)                                          /*!< Error-warning state (REC/TEC < 128)                             */
  #define SP_CAN_STATUS_STATUS_STATE_ERRORPASSIVE (0x2UL)                                          /*!< Error-passive state (REC/TEC < 256)                             */
  #define SP_CAN_STATUS_STATUS_STATE_BUSOFF       (0x3UL)                                          /*!< Bus-off state (REC/TEC >= 256)                                       */
  #define SP_CAN_STATUS_STATUS_STATE_STOPPED      (0x4UL)                                          /*!< Stopped state, sCAN doesn't participate in communication             */

/* ================================================== Struct SP_CAN_SPSYNC =================================================== */
/**
 * @brief SPSYNC [SP_CAN_SPSYNC] Registers used to acknowledge API function calls
 */
typedef struct
{
    __IOM uint32_t AUX[6];                                                                          /*!< (@ 0x00000000) Auxiliary registers for XSB macro call handshaking    */
} NRF_SP_CAN_SPSYNC_Type;                                                                           /*!< Size = 24 (0x018)                                                    */

/* SP_CAN_SPSYNC_AUX: Auxiliary registers for XSB macro call handshaking */
  #define SP_CAN_SPSYNC_AUX_MaxCount   (6UL)                                                        /*!< Max size of AUX[6] array.                                            */
  #define SP_CAN_SPSYNC_AUX_MaxIndex   (5UL)                                                        /*!< Max index of AUX[6] array.                                           */
  #define SP_CAN_SPSYNC_AUX_MinIndex   (0UL)                                                        /*!< Min index of AUX[6] array.                                           */
  #define SP_CAN_SPSYNC_AUX_ResetValue (0x00000000UL)                                               /*!< Reset value of AUX[6] register.                                     */

/* AUX @Bits 0..31 : Auxiliary register */
  #define SP_CAN_SPSYNC_AUX_AUX_Pos (0UL)                                                           /*!< Position of AUX field.                                               */
  #define SP_CAN_SPSYNC_AUX_AUX_Msk (0xFFFFFFFFUL << SP_CAN_SPSYNC_AUX_AUX_Pos)                     /*!< Bit mask of AUX field.                    */

/* ====================================================== Struct SP_CAN ====================================================== */
/**
 * @brief Soft peripheral CAN
 */
typedef struct                                                                                      /*!< SP_CAN Structure                                                     */
{
    __OM uint32_t                  TASKS_START;                                                     /*!< (@ 0x00000000) Trigger this task to start a request (RX/TX)          */
    __OM uint32_t                  TASKS_RESET;                                                     /*!< (@ 0x00000004) Trigger this task to reset SCAN to UNINITIALIZED state*/
    __IOM uint32_t                 SUBSCRIBE_START;                                                 /*!< (@ 0x00000008) Subscribe configuration for task START                */
    __IOM uint32_t                 SUBSCRIBE_RESET;                                                 /*!< (@ 0x0000000C) Subscribe configuration for task RESET                */
    __IOM uint32_t                 EVENTS_TXCOMPLETE;                                               /*!< (@ 0x00000010) This event signals that SCAN has completed a TX
                                                                                                     *                  operation*/
    __IOM uint32_t                 EVENTS_RXCOMPLETE;                                               /*!< (@ 0x00000014) This event signals that SCAN has finished RX after an
                                                                                                     *                  ID match*/
    __IOM uint32_t                 EVENTS_ERRORDETECTED;                                            /*!< (@ 0x00000018) This event signals that SCAN has detected an error.   */
    __IOM uint32_t                 EVENTS_STATECHANGED;                                             /*!< (@ 0x0000001C) This event signals that SCAN's state has changed.     */
    __IOM uint32_t                 PUBLISH_TXCOMPLETE;                                              /*!< (@ 0x00000020) Publish configuration for event TXCOMPLETE            */
    __IOM uint32_t                 PUBLISH_RXCOMPLETE;                                              /*!< (@ 0x00000024) Publish configuration for event RXCOMPLETE            */
    __IOM uint32_t                 PUBLISH_ERRORDETECTED;                                           /*!< (@ 0x00000028) Publish configuration for event ERRORDETECTED         */
    __IOM uint32_t                 PUBLISH_STATECHANGED;                                            /*!< (@ 0x0000002C) Publish configuration for event STATECHANGED          */
    __IOM uint32_t                 INTEN;                                                           /*!< (@ 0x00000030) Enable or disable interrupt                           */
    __IOM uint32_t                 ENABLE;                                                          /*!< (@ 0x00000034) Enable the SCAN peripheral                            */
    __IOM NRF_SP_CAN_CONFIG_Type   CONFIG;                                                          /*!< (@ 0x00000038) SCAN configuration                                    */
    __IOM NRF_SP_CAN_RXFILTER_Type RXFILTER[4];                                                     /*!< (@ 0x00000048) sCAN RX filters                                       */
    __IOM NRF_SP_CAN_TXFRAME_Type  TXFRAME[4];                                                      /*!< (@ 0x00000068) sCAN frame configuration                              */
    __IOM NRF_SP_CAN_RXFRAME_Type  RXFRAME[4];                                                      /*!< (@ 0x000000A8) sCAN frame configuration                              */
    __IOM NRF_SP_CAN_STATUS_Type   STATUS;                                                          /*!< (@ 0x000000E8) SCAN status                                           */
    __IOM NRF_SP_CAN_SPSYNC_Type   SPSYNC;                                                          /*!< (@ 0x000000EC) Registers used to acknowledge API function calls      */
} NRF_SP_CAN_Type;                                                                                  /*!< Size = 260 (0x104)                                                   */

/* SP_CAN_TASKS_START: Trigger this task to start a request (RX/TX) */
  #define SP_CAN_TASKS_START_ResetValue (0x00000000UL)                                              /*!< Reset value of TASKS_START register.                               */

/* TASKS_START @Bit 0 : Trigger this task to start a request (RX/TX) */
  #define SP_CAN_TASKS_START_TASKS_START_Pos     (0UL)                                              /*!< Position of TASKS_START field.                                       */
  #define SP_CAN_TASKS_START_TASKS_START_Msk     (0x1UL << SP_CAN_TASKS_START_TASKS_START_Pos)      /*!< Bit mask of TASKS_START field. */
  #define SP_CAN_TASKS_START_TASKS_START_Min     (0x1UL)                                            /*!< Min enumerator value of TASKS_START field.                           */
  #define SP_CAN_TASKS_START_TASKS_START_Max     (0x1UL)                                            /*!< Max enumerator value of TASKS_START field.                           */
  #define SP_CAN_TASKS_START_TASKS_START_Trigger (0x1UL)                                            /*!< Trigger task                                                     */

/* SP_CAN_TASKS_RESET: Trigger this task to reset SCAN to UNINITIALIZED state */
  #define SP_CAN_TASKS_RESET_ResetValue (0x00000000UL)                                              /*!< Reset value of TASKS_RESET register.                               */

/* TASKS_RESET @Bit 0 : Trigger this task to reset SCAN to UNINITIALIZED state */
  #define SP_CAN_TASKS_RESET_TASKS_RESET_Pos     (0UL)                                              /*!< Position of TASKS_RESET field.                                       */
  #define SP_CAN_TASKS_RESET_TASKS_RESET_Msk     (0x1UL << SP_CAN_TASKS_RESET_TASKS_RESET_Pos)      /*!< Bit mask of TASKS_RESET field. */
  #define SP_CAN_TASKS_RESET_TASKS_RESET_Min     (0x1UL)                                            /*!< Min enumerator value of TASKS_RESET field.                           */
  #define SP_CAN_TASKS_RESET_TASKS_RESET_Max     (0x1UL)                                            /*!< Max enumerator value of TASKS_RESET field.                           */
  #define SP_CAN_TASKS_RESET_TASKS_RESET_Trigger (0x1UL)                                            /*!< Trigger task                                                     */

/* SP_CAN_SUBSCRIBE_START: Subscribe configuration for task START */
  #define SP_CAN_SUBSCRIBE_START_ResetValue (0x00000000UL)                                          /*!< Reset value of SUBSCRIBE_START register.                       */

/* CHIDX @Bits 0..7 : DPPI channel that task START will subscribe to */
  #define SP_CAN_SUBSCRIBE_START_CHIDX_Pos (0UL)                                                    /*!< Position of CHIDX field.                                             */
  #define SP_CAN_SUBSCRIBE_START_CHIDX_Msk (0xFFUL << SP_CAN_SUBSCRIBE_START_CHIDX_Pos)             /*!< Bit mask of CHIDX field.          */
  #define SP_CAN_SUBSCRIBE_START_CHIDX_Min (0x00UL)                                                 /*!< Min value of CHIDX field.                                            */
  #define SP_CAN_SUBSCRIBE_START_CHIDX_Max (0xFFUL)                                                 /*!< Max size of CHIDX field.                                             */

/* EN @Bit 31 : (unspecified) */
  #define SP_CAN_SUBSCRIBE_START_EN_Pos      (31UL)                                                 /*!< Position of EN field.                                                */
  #define SP_CAN_SUBSCRIBE_START_EN_Msk      (0x1UL << SP_CAN_SUBSCRIBE_START_EN_Pos)               /*!< Bit mask of EN field.                    */
  #define SP_CAN_SUBSCRIBE_START_EN_Min      (0x0UL)                                                /*!< Min enumerator value of EN field.                                    */
  #define SP_CAN_SUBSCRIBE_START_EN_Max      (0x1UL)                                                /*!< Max enumerator value of EN field.                                    */
  #define SP_CAN_SUBSCRIBE_START_EN_Disabled (0x0UL)                                                /*!< Disable subscription                                                 */
  #define SP_CAN_SUBSCRIBE_START_EN_Enabled  (0x1UL)                                                /*!< Enable subscription                                                  */

/* SP_CAN_SUBSCRIBE_RESET: Subscribe configuration for task RESET */
  #define SP_CAN_SUBSCRIBE_RESET_ResetValue (0x00000000UL)                                          /*!< Reset value of SUBSCRIBE_RESET register.                       */

/* CHIDX @Bits 0..7 : DPPI channel that task RESET will subscribe to */
  #define SP_CAN_SUBSCRIBE_RESET_CHIDX_Pos (0UL)                                                    /*!< Position of CHIDX field.                                             */
  #define SP_CAN_SUBSCRIBE_RESET_CHIDX_Msk (0xFFUL << SP_CAN_SUBSCRIBE_RESET_CHIDX_Pos)             /*!< Bit mask of CHIDX field.          */
  #define SP_CAN_SUBSCRIBE_RESET_CHIDX_Min (0x00UL)                                                 /*!< Min value of CHIDX field.                                            */
  #define SP_CAN_SUBSCRIBE_RESET_CHIDX_Max (0xFFUL)                                                 /*!< Max size of CHIDX field.                                             */

/* EN @Bit 31 : (unspecified) */
  #define SP_CAN_SUBSCRIBE_RESET_EN_Pos      (31UL)                                                 /*!< Position of EN field.                                                */
  #define SP_CAN_SUBSCRIBE_RESET_EN_Msk      (0x1UL << SP_CAN_SUBSCRIBE_RESET_EN_Pos)               /*!< Bit mask of EN field.                    */
  #define SP_CAN_SUBSCRIBE_RESET_EN_Min      (0x0UL)                                                /*!< Min enumerator value of EN field.                                    */
  #define SP_CAN_SUBSCRIBE_RESET_EN_Max      (0x1UL)                                                /*!< Max enumerator value of EN field.                                    */
  #define SP_CAN_SUBSCRIBE_RESET_EN_Disabled (0x0UL)                                                /*!< Disable subscription                                                 */
  #define SP_CAN_SUBSCRIBE_RESET_EN_Enabled  (0x1UL)                                                /*!< Enable subscription                                                  */

/* SP_CAN_EVENTS_TXCOMPLETE: This event signals that SCAN has completed a TX operation */
  #define SP_CAN_EVENTS_TXCOMPLETE_ResetValue (0x00000000UL)                                        /*!< Reset value of EVENTS_TXCOMPLETE register.                   */

/* EVENTS_TXCOMPLETE @Bit 0 : This event signals that SCAN has completed a TX operation */
  #define SP_CAN_EVENTS_TXCOMPLETE_EVENTS_TXCOMPLETE_Pos             (0UL)                          /*!< Position of EVENTS_TXCOMPLETE field.                       */
  #define SP_CAN_EVENTS_TXCOMPLETE_EVENTS_TXCOMPLETE_Msk                                                                       \
                                                                     (0x1UL << SP_CAN_EVENTS_TXCOMPLETE_EVENTS_TXCOMPLETE_Pos) \
                                                                                                    /*!< Bit mask
                                                                                                     * of EVENTS_TXCOMPLETE field.*/
  #define SP_CAN_EVENTS_TXCOMPLETE_EVENTS_TXCOMPLETE_Min             (0x0UL)                        /*!< Min enumerator value of EVENTS_TXCOMPLETE field.         */
  #define SP_CAN_EVENTS_TXCOMPLETE_EVENTS_TXCOMPLETE_Max             (0x1UL)                        /*!< Max enumerator value of EVENTS_TXCOMPLETE field.         */
  #define SP_CAN_EVENTS_TXCOMPLETE_EVENTS_TXCOMPLETE_NotGenerated    (0x0UL)                        /*!< Event not generated                             */
  #define SP_CAN_EVENTS_TXCOMPLETE_EVENTS_TXCOMPLETE_Generated       (0x1UL)                        /*!< Event generated                                    */

/* SP_CAN_EVENTS_RXCOMPLETE: This event signals that SCAN has finished RX after an ID match */
  #define SP_CAN_EVENTS_RXCOMPLETE_ResetValue (0x00000000UL)                                        /*!< Reset value of EVENTS_RXCOMPLETE register.                   */

/* EVENTS_RXCOMPLETE @Bit 0 : This event signals that SCAN has finished RX after an ID match */
  #define SP_CAN_EVENTS_RXCOMPLETE_EVENTS_RXCOMPLETE_Pos             (0UL)                          /*!< Position of EVENTS_RXCOMPLETE field.                       */
  #define SP_CAN_EVENTS_RXCOMPLETE_EVENTS_RXCOMPLETE_Msk                                                                       \
                                                                     (0x1UL << SP_CAN_EVENTS_RXCOMPLETE_EVENTS_RXCOMPLETE_Pos) \
                                                                                                    /*!< Bit mask
                                                                                                     * of EVENTS_RXCOMPLETE field.*/
  #define SP_CAN_EVENTS_RXCOMPLETE_EVENTS_RXCOMPLETE_Min             (0x0UL)                        /*!< Min enumerator value of EVENTS_RXCOMPLETE field.         */
  #define SP_CAN_EVENTS_RXCOMPLETE_EVENTS_RXCOMPLETE_Max             (0x1UL)                        /*!< Max enumerator value of EVENTS_RXCOMPLETE field.         */
  #define SP_CAN_EVENTS_RXCOMPLETE_EVENTS_RXCOMPLETE_NotGenerated    (0x0UL)                        /*!< Event not generated                             */
  #define SP_CAN_EVENTS_RXCOMPLETE_EVENTS_RXCOMPLETE_Generated       (0x1UL)                        /*!< Event generated                                    */

/* SP_CAN_EVENTS_ERRORDETECTED: This event signals that SCAN has detected an error. */
  #define SP_CAN_EVENTS_ERRORDETECTED_ResetValue (0x00000000UL)                                     /*!< Reset value of EVENTS_ERRORDETECTED register.             */

/* EVENTS_ERRORDETECTED @Bit 0 : This event signals that SCAN has detected an error. */
  #define SP_CAN_EVENTS_ERRORDETECTED_EVENTS_ERRORDETECTED_Pos             (0UL)                    /*!< Position of EVENTS_ERRORDETECTED field.              */
  #define SP_CAN_EVENTS_ERRORDETECTED_EVENTS_ERRORDETECTED_Msk \
                                                                           (0x1UL << SP_CAN_EVENTS_ERRORDETECTED_EVENTS_ERRORDETECTED_Pos)
                                                                                                    /*!< Bit mask of EVENTS_ERRORDETECTED field.*/
  #define SP_CAN_EVENTS_ERRORDETECTED_EVENTS_ERRORDETECTED_Min             (0x0UL)                  /*!< Min enumerator value of EVENTS_ERRORDETECTED field.*/
  #define SP_CAN_EVENTS_ERRORDETECTED_EVENTS_ERRORDETECTED_Max             (0x1UL)                  /*!< Max enumerator value of EVENTS_ERRORDETECTED field.*/
  #define SP_CAN_EVENTS_ERRORDETECTED_EVENTS_ERRORDETECTED_NotGenerated    (0x0UL)                  /*!< Event not generated                       */
  #define SP_CAN_EVENTS_ERRORDETECTED_EVENTS_ERRORDETECTED_Generated       (0x1UL)                  /*!< Event generated                              */

/* SP_CAN_EVENTS_STATECHANGED: This event signals that SCAN's state has changed. */
  #define SP_CAN_EVENTS_STATECHANGED_ResetValue (0x00000000UL)                                      /*!< Reset value of EVENTS_STATECHANGED register.               */

/* EVENTS_STATECHANGED @Bit 0 : This event signals that SCAN's state has changed. */
  #define SP_CAN_EVENTS_STATECHANGED_EVENTS_STATECHANGED_Pos             (0UL)                      /*!< Position of EVENTS_STATECHANGED field.                 */
  #define SP_CAN_EVENTS_STATECHANGED_EVENTS_STATECHANGED_Msk                                                                           \
                                                                         (0x1UL << SP_CAN_EVENTS_STATECHANGED_EVENTS_STATECHANGED_Pos) \
                                                                                                    /*!<
                                                                                                     * Bit mask of EVENTS_STATECHANGED field.*/
  #define SP_CAN_EVENTS_STATECHANGED_EVENTS_STATECHANGED_Min             (0x0UL)                    /*!< Min enumerator value of EVENTS_STATECHANGED field.   */
  #define SP_CAN_EVENTS_STATECHANGED_EVENTS_STATECHANGED_Max             (0x1UL)                    /*!< Max enumerator value of EVENTS_STATECHANGED field.   */
  #define SP_CAN_EVENTS_STATECHANGED_EVENTS_STATECHANGED_NotGenerated    (0x0UL)                    /*!< Event not generated                         */
  #define SP_CAN_EVENTS_STATECHANGED_EVENTS_STATECHANGED_Generated       (0x1UL)                    /*!< Event generated                                */

/* SP_CAN_PUBLISH_TXCOMPLETE: Publish configuration for event TXCOMPLETE */
  #define SP_CAN_PUBLISH_TXCOMPLETE_ResetValue (0x00000000UL)                                       /*!< Reset value of PUBLISH_TXCOMPLETE register.                 */

/* CHIDX @Bits 0..7 : DPPI channel that event TXCOMPLETE will publish to */
  #define SP_CAN_PUBLISH_TXCOMPLETE_CHIDX_Pos (0UL)                                                 /*!< Position of CHIDX field.                                             */
  #define SP_CAN_PUBLISH_TXCOMPLETE_CHIDX_Msk (0xFFUL << SP_CAN_PUBLISH_TXCOMPLETE_CHIDX_Pos)       /*!< Bit mask of CHIDX field.    */
  #define SP_CAN_PUBLISH_TXCOMPLETE_CHIDX_Min (0x00UL)                                              /*!< Min value of CHIDX field.                                          */
  #define SP_CAN_PUBLISH_TXCOMPLETE_CHIDX_Max (0xFFUL)                                              /*!< Max size of CHIDX field.                                           */

/* EN @Bit 31 : (unspecified) */
  #define SP_CAN_PUBLISH_TXCOMPLETE_EN_Pos      (31UL)                                              /*!< Position of EN field.                                                */
  #define SP_CAN_PUBLISH_TXCOMPLETE_EN_Msk      (0x1UL << SP_CAN_PUBLISH_TXCOMPLETE_EN_Pos)         /*!< Bit mask of EN field.              */
  #define SP_CAN_PUBLISH_TXCOMPLETE_EN_Min      (0x0UL)                                             /*!< Min enumerator value of EN field.                                    */
  #define SP_CAN_PUBLISH_TXCOMPLETE_EN_Max      (0x1UL)                                             /*!< Max enumerator value of EN field.                                    */
  #define SP_CAN_PUBLISH_TXCOMPLETE_EN_Disabled (0x0UL)                                             /*!< Disable publishing                                                */
  #define SP_CAN_PUBLISH_TXCOMPLETE_EN_Enabled  (0x1UL)                                             /*!< Enable publishing                                                  */

/* SP_CAN_PUBLISH_RXCOMPLETE: Publish configuration for event RXCOMPLETE */
  #define SP_CAN_PUBLISH_RXCOMPLETE_ResetValue (0x00000000UL)                                       /*!< Reset value of PUBLISH_RXCOMPLETE register.                 */

/* CHIDX @Bits 0..7 : DPPI channel that event RXCOMPLETE will publish to */
  #define SP_CAN_PUBLISH_RXCOMPLETE_CHIDX_Pos (0UL)                                                 /*!< Position of CHIDX field.                                             */
  #define SP_CAN_PUBLISH_RXCOMPLETE_CHIDX_Msk (0xFFUL << SP_CAN_PUBLISH_RXCOMPLETE_CHIDX_Pos)       /*!< Bit mask of CHIDX field.    */
  #define SP_CAN_PUBLISH_RXCOMPLETE_CHIDX_Min (0x00UL)                                              /*!< Min value of CHIDX field.                                          */
  #define SP_CAN_PUBLISH_RXCOMPLETE_CHIDX_Max (0xFFUL)                                              /*!< Max size of CHIDX field.                                           */

/* EN @Bit 31 : (unspecified) */
  #define SP_CAN_PUBLISH_RXCOMPLETE_EN_Pos      (31UL)                                              /*!< Position of EN field.                                                */
  #define SP_CAN_PUBLISH_RXCOMPLETE_EN_Msk      (0x1UL << SP_CAN_PUBLISH_RXCOMPLETE_EN_Pos)         /*!< Bit mask of EN field.              */
  #define SP_CAN_PUBLISH_RXCOMPLETE_EN_Min      (0x0UL)                                             /*!< Min enumerator value of EN field.                                    */
  #define SP_CAN_PUBLISH_RXCOMPLETE_EN_Max      (0x1UL)                                             /*!< Max enumerator value of EN field.                                    */
  #define SP_CAN_PUBLISH_RXCOMPLETE_EN_Disabled (0x0UL)                                             /*!< Disable publishing                                                */
  #define SP_CAN_PUBLISH_RXCOMPLETE_EN_Enabled  (0x1UL)                                             /*!< Enable publishing                                                  */

/* SP_CAN_PUBLISH_ERRORDETECTED: Publish configuration for event ERRORDETECTED */
  #define SP_CAN_PUBLISH_ERRORDETECTED_ResetValue (0x00000000UL)                                    /*!< Reset value of PUBLISH_ERRORDETECTED register.           */

/* CHIDX @Bits 0..7 : DPPI channel that event ERRORDETECTED will publish to */
  #define SP_CAN_PUBLISH_ERRORDETECTED_CHIDX_Pos (0UL)                                              /*!< Position of CHIDX field.                                           */
  #define SP_CAN_PUBLISH_ERRORDETECTED_CHIDX_Msk (0xFFUL << SP_CAN_PUBLISH_ERRORDETECTED_CHIDX_Pos) /*!< Bit mask of CHIDX
                                                                                                     * field.*/
  #define SP_CAN_PUBLISH_ERRORDETECTED_CHIDX_Min (0x00UL)                                           /*!< Min value of CHIDX field.                                       */
  #define SP_CAN_PUBLISH_ERRORDETECTED_CHIDX_Max (0xFFUL)                                           /*!< Max size of CHIDX field.                                        */

/* EN @Bit 31 : (unspecified) */
  #define SP_CAN_PUBLISH_ERRORDETECTED_EN_Pos      (31UL)                                           /*!< Position of EN field.                                                */
  #define SP_CAN_PUBLISH_ERRORDETECTED_EN_Msk      (0x1UL << SP_CAN_PUBLISH_ERRORDETECTED_EN_Pos)   /*!< Bit mask of EN field.        */
  #define SP_CAN_PUBLISH_ERRORDETECTED_EN_Min      (0x0UL)                                          /*!< Min enumerator value of EN field.                                   */
  #define SP_CAN_PUBLISH_ERRORDETECTED_EN_Max      (0x1UL)                                          /*!< Max enumerator value of EN field.                                   */
  #define SP_CAN_PUBLISH_ERRORDETECTED_EN_Disabled (0x0UL)                                          /*!< Disable publishing                                             */
  #define SP_CAN_PUBLISH_ERRORDETECTED_EN_Enabled  (0x1UL)                                          /*!< Enable publishing                                               */

/* SP_CAN_PUBLISH_STATECHANGED: Publish configuration for event STATECHANGED */
  #define SP_CAN_PUBLISH_STATECHANGED_ResetValue (0x00000000UL)                                     /*!< Reset value of PUBLISH_STATECHANGED register.             */

/* CHIDX @Bits 0..7 : DPPI channel that event STATECHANGED will publish to */
  #define SP_CAN_PUBLISH_STATECHANGED_CHIDX_Pos (0UL)                                               /*!< Position of CHIDX field.                                            */
  #define SP_CAN_PUBLISH_STATECHANGED_CHIDX_Msk (0xFFUL << SP_CAN_PUBLISH_STATECHANGED_CHIDX_Pos)   /*!< Bit mask of CHIDX field.*/
  #define SP_CAN_PUBLISH_STATECHANGED_CHIDX_Min (0x00UL)                                            /*!< Min value of CHIDX field.                                        */
  #define SP_CAN_PUBLISH_STATECHANGED_CHIDX_Max (0xFFUL)                                            /*!< Max size of CHIDX field.                                         */

/* EN @Bit 31 : (unspecified) */
  #define SP_CAN_PUBLISH_STATECHANGED_EN_Pos      (31UL)                                            /*!< Position of EN field.                                                */
  #define SP_CAN_PUBLISH_STATECHANGED_EN_Msk      (0x1UL << SP_CAN_PUBLISH_STATECHANGED_EN_Pos)     /*!< Bit mask of EN field.          */
  #define SP_CAN_PUBLISH_STATECHANGED_EN_Min      (0x0UL)                                           /*!< Min enumerator value of EN field.                                    */
  #define SP_CAN_PUBLISH_STATECHANGED_EN_Max      (0x1UL)                                           /*!< Max enumerator value of EN field.                                    */
  #define SP_CAN_PUBLISH_STATECHANGED_EN_Disabled (0x0UL)                                           /*!< Disable publishing                                              */
  #define SP_CAN_PUBLISH_STATECHANGED_EN_Enabled  (0x1UL)                                           /*!< Enable publishing                                                */

/* SP_CAN_INTEN: Enable or disable interrupt */
  #define SP_CAN_INTEN_ResetValue (0x00000000UL)                                                    /*!< Reset value of INTEN register.                                       */

/* TXCOMPLETE @Bit 0 : Enable or disable interrupt for event TXATTEMPTCOMPLETE */
  #define SP_CAN_INTEN_TXCOMPLETE_Pos      (0UL)                                                    /*!< Position of TXCOMPLETE field.                                        */
  #define SP_CAN_INTEN_TXCOMPLETE_Msk      (0x1UL << SP_CAN_INTEN_TXCOMPLETE_Pos)                   /*!< Bit mask of TXCOMPLETE field.                */
  #define SP_CAN_INTEN_TXCOMPLETE_Min      (0x0UL)                                                  /*!< Min enumerator value of TXCOMPLETE field.                            */
  #define SP_CAN_INTEN_TXCOMPLETE_Max      (0x1UL)                                                  /*!< Max enumerator value of TXCOMPLETE field.                            */
  #define SP_CAN_INTEN_TXCOMPLETE_Disabled (0x0UL)                                                  /*!< Disable                                                              */
  #define SP_CAN_INTEN_TXCOMPLETE_Enabled  (0x1UL)                                                  /*!< Enable                                                               */

/* RXCOMPLETE @Bit 1 : Enable or disable interrupt for event RXCOMPLETE */
  #define SP_CAN_INTEN_RXCOMPLETE_Pos      (1UL)                                                    /*!< Position of RXCOMPLETE field.                                        */
  #define SP_CAN_INTEN_RXCOMPLETE_Msk      (0x1UL << SP_CAN_INTEN_RXCOMPLETE_Pos)                   /*!< Bit mask of RXCOMPLETE field.                */
  #define SP_CAN_INTEN_RXCOMPLETE_Min      (0x0UL)                                                  /*!< Min enumerator value of RXCOMPLETE field.                            */
  #define SP_CAN_INTEN_RXCOMPLETE_Max      (0x1UL)                                                  /*!< Max enumerator value of RXCOMPLETE field.                            */
  #define SP_CAN_INTEN_RXCOMPLETE_Disabled (0x0UL)                                                  /*!< Disable                                                              */
  #define SP_CAN_INTEN_RXCOMPLETE_Enabled  (0x1UL)                                                  /*!< Enable                                                               */

/* ERRORDETECTED @Bit 2 : Enable or disable interrupt for event ERRORDETECTED */
  #define SP_CAN_INTEN_ERRORDETECTED_Pos      (2UL)                                                 /*!< Position of ERRORDETECTED field.                                     */
  #define SP_CAN_INTEN_ERRORDETECTED_Msk      (0x1UL << SP_CAN_INTEN_ERRORDETECTED_Pos)             /*!< Bit mask of ERRORDETECTED field.       */
  #define SP_CAN_INTEN_ERRORDETECTED_Min      (0x0UL)                                               /*!< Min enumerator value of ERRORDETECTED field.                         */
  #define SP_CAN_INTEN_ERRORDETECTED_Max      (0x1UL)                                               /*!< Max enumerator value of ERRORDETECTED field.                         */
  #define SP_CAN_INTEN_ERRORDETECTED_Disabled (0x0UL)                                               /*!< Disable                                                             */
  #define SP_CAN_INTEN_ERRORDETECTED_Enabled  (0x1UL)                                               /*!< Enable                                                               */

/* STATECHANGED @Bit 3 : Enable or disable interrupt for event STATECHANGED */
  #define SP_CAN_INTEN_STATECHANGED_Pos      (3UL)                                                  /*!< Position of STATECHANGED field.                                      */
  #define SP_CAN_INTEN_STATECHANGED_Msk      (0x1UL << SP_CAN_INTEN_STATECHANGED_Pos)               /*!< Bit mask of STATECHANGED field.          */
  #define SP_CAN_INTEN_STATECHANGED_Min      (0x0UL)                                                /*!< Min enumerator value of STATECHANGED field.                          */
  #define SP_CAN_INTEN_STATECHANGED_Max      (0x1UL)                                                /*!< Max enumerator value of STATECHANGED field.                          */
  #define SP_CAN_INTEN_STATECHANGED_Disabled (0x0UL)                                                /*!< Disable                                                              */
  #define SP_CAN_INTEN_STATECHANGED_Enabled  (0x1UL)                                                /*!< Enable                                                               */

/* SP_CAN_ENABLE: Enable the SCAN peripheral */
  #define SP_CAN_ENABLE_ResetValue (0x00000000UL)                                                   /*!< Reset value of ENABLE register.                                      */

/* ENABLE @Bit 0 : Enable or disable SCAN */
  #define SP_CAN_ENABLE_ENABLE_Pos      (0UL)                                                       /*!< Position of ENABLE field.                                            */
  #define SP_CAN_ENABLE_ENABLE_Msk      (0x1UL << SP_CAN_ENABLE_ENABLE_Pos)                         /*!< Bit mask of ENABLE field.                          */
  #define SP_CAN_ENABLE_ENABLE_Min      (0x0UL)                                                     /*!< Min enumerator value of ENABLE field.                                */
  #define SP_CAN_ENABLE_ENABLE_Max      (0x1UL)                                                     /*!< Max enumerator value of ENABLE field.                                */
  #define SP_CAN_ENABLE_ENABLE_Disabled (0x0UL)                                                     /*!< Disable SCAN                                                         */
  #define SP_CAN_ENABLE_ENABLE_Enabled  (0x1UL)                                                     /*!< Enable SCAN                                                          */

#endif \
    /*!< !defined(__ASSEMBLER__) && !defined(__ASSEMBLY__)                    */

/* ========================================== End of section using anonymous unions ========================================== */
#endif /* NRF_SP_CAN_H */
