/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef NRF_SP_EMMC_H__
#define NRF_SP_EMMC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf.h"                                      // Resolve nrfXX_types.h for the correct target to get definitions for __IO, __IOM etc.
/* ===========================================================================================================================
 * ================                                          SP_EMMC                                          ================
 * ===========================================================================================================================*/

#if !defined(__ASSEMBLER__) && !defined(__ASSEMBLY__) /*!< Ignore C structs for assembly code.                                 */

/* ================================================== Struct SP_EMMC_CONFIG ================================================== */
/**
 * @brief CONFIG [SP_EMMC_CONFIG] SEMMC configuration
 */
typedef struct
{
    __IOM uint32_t READYTOTRANSFER;                                       /*!< (@ 0x00000000) Indicates that SEMMC has parsed a configuration and it
                                                                           *                  is ready to transfer*/
    __IOM uint32_t CLKFREQHZ;                                             /*!< (@ 0x00000004) SEMMC Clock Frequency in Hz                           */
    __IOM uint32_t BUSWIDTH;                                              /*!< (@ 0x00000008) SEMMC Bus width                                       */
    __IOM uint32_t NUMRETRIES;                                            /*!< (@ 0x0000000C) Number of retries for a transaction                   */
    __IOM uint32_t READDELAY;                                             /*!< (@ 0x00000010) Number of FLPR clock cycles to offset for data sampling
                                                                           *                  point*/
} NRF_SP_EMMC_CONFIG_Type;                                                /*!< Size = 20 (0x014)                                                    */

/* SP_EMMC_CONFIG_READYTOTRANSFER: Indicates that SEMMC has parsed a configuration and it is ready to transfer */
  #define SP_EMMC_CONFIG_READYTOTRANSFER_ResetValue (0x00000000UL)        /*!< Reset value of READYTOTRANSFER register.               */

/* READYTOTRANSFER @Bit 0 : SEMMC is ready to transfer */
  #define SP_EMMC_CONFIG_READYTOTRANSFER_READYTOTRANSFER_Pos      (0UL)   /*!< Position of READYTOTRANSFER field.                     */
  #define SP_EMMC_CONFIG_READYTOTRANSFER_READYTOTRANSFER_Msk                                                                       \
                                                                     (0x1UL << SP_EMMC_CONFIG_READYTOTRANSFER_READYTOTRANSFER_Pos) \
                                                                          /*!<
                                                                           * Bit mask of READYTOTRANSFER field.*/
  #define SP_EMMC_CONFIG_READYTOTRANSFER_READYTOTRANSFER_Min      (0x0UL) /*!< Min enumerator value of READYTOTRANSFER field.       */
  #define SP_EMMC_CONFIG_READYTOTRANSFER_READYTOTRANSFER_Max      (0x1UL) /*!< Max enumerator value of READYTOTRANSFER field.       */
  #define SP_EMMC_CONFIG_READYTOTRANSFER_READYTOTRANSFER_NotReady (0x0UL) /*!< SEMMC is not ready                              */
  #define SP_EMMC_CONFIG_READYTOTRANSFER_READYTOTRANSFER_Ready    (0x1UL) /*!< SEMMC is ready                                     */

/* SP_EMMC_CONFIG_CLKFREQHZ: SEMMC Clock Frequency in Hz */
  #define SP_EMMC_CONFIG_CLKFREQHZ_ResetValue (0x00000000UL)              /*!< Reset value of CLKFREQHZ register.                           */

/* CLKFREQHZ @Bits 0..31 : SEMMC Clock Frequency in Hz */
  #define SP_EMMC_CONFIG_CLKFREQHZ_CLKFREQHZ_Pos (0UL)                    /*!< Position of CLKFREQHZ field.                                       */
  #define SP_EMMC_CONFIG_CLKFREQHZ_CLKFREQHZ_Msk                                                             \
                                                    (0xFFFFFFFFUL << SP_EMMC_CONFIG_CLKFREQHZ_CLKFREQHZ_Pos) \
                                                                          /*!< Bit mask of
                                                                           * CLKFREQHZ field.*/

/* SP_EMMC_CONFIG_BUSWIDTH: SEMMC Bus width */
  #define SP_EMMC_CONFIG_BUSWIDTH_ResetValue (0x00000001UL)               /*!< Reset value of BUSWIDTH register.                             */

/* BUSWIDTH @Bits 0..3 : Bus width SEMMC will use for data transfers */
  #define SP_EMMC_CONFIG_BUSWIDTH_BUSWIDTH_Pos        (0UL)               /*!< Position of BUSWIDTH field.                                          */
  #define SP_EMMC_CONFIG_BUSWIDTH_BUSWIDTH_Msk                                                           \
                                                         (0xFUL << SP_EMMC_CONFIG_BUSWIDTH_BUSWIDTH_Pos) \
                                                                          /*!< Bit mask of BUSWIDTH field.*/
  #define SP_EMMC_CONFIG_BUSWIDTH_BUSWIDTH_Min        (0x1UL)             /*!< Min enumerator value of BUSWIDTH field.                            */
  #define SP_EMMC_CONFIG_BUSWIDTH_BUSWIDTH_Max        (0x8UL)             /*!< Max enumerator value of BUSWIDTH field.                            */
  #define SP_EMMC_CONFIG_BUSWIDTH_BUSWIDTH_ONELANE    (0x1UL)             /*!< Data transfers will use one lane                               */
  #define SP_EMMC_CONFIG_BUSWIDTH_BUSWIDTH_FOURLANES  (0x4UL)             /*!< Data transfers will use four lanes                           */
  #define SP_EMMC_CONFIG_BUSWIDTH_BUSWIDTH_EIGHTLANES (0x8UL)             /*!< Data transfers will use eigth lanes                         */

/* SP_EMMC_CONFIG_NUMRETRIES: Number of retries for a transaction */
  #define SP_EMMC_CONFIG_NUMRETRIES_ResetValue (0x00000000UL)             /*!< Reset value of NUMRETRIES register.                         */

/* NUMRETRIES @Bits 0..31 : Number of retries for a transaction */
  #define SP_EMMC_CONFIG_NUMRETRIES_NUMRETRIES_Pos (0UL)                  /*!< Position of NUMRETRIES field.                                    */
  #define SP_EMMC_CONFIG_NUMRETRIES_NUMRETRIES_Msk                                                               \
                                                      (0xFFFFFFFFUL << SP_EMMC_CONFIG_NUMRETRIES_NUMRETRIES_Pos) \
                                                                          /*!< Bit mask of
                                                                           * NUMRETRIES field.*/

/* SP_EMMC_CONFIG_READDELAY: Number of FLPR clock cycles to offset for data sampling point */
  #define SP_EMMC_CONFIG_READDELAY_ResetValue (0x00000000UL)              /*!< Reset value of READDELAY register.                           */

/* READDELAY @Bits 0..31 : Number of FLPR clock cycles to offset for data sampling point */
  #define SP_EMMC_CONFIG_READDELAY_READDELAY_Pos (0UL)                    /*!< Position of READDELAY field.                                       */
  #define SP_EMMC_CONFIG_READDELAY_READDELAY_Msk                                                             \
                                                    (0xFFFFFFFFUL << SP_EMMC_CONFIG_READDELAY_READDELAY_Pos) \
                                                                          /*!< Bit mask of
                                                                           * READDELAY field.*/

/* ================================================= Struct SP_EMMC_COMMAND ================================================== */
/**
 * @brief COMMAND [SP_EMMC_COMMAND] SEMMC command descriptor
 */
typedef struct
{
    __IOM uint32_t CMD;                                                                     /*!< (@ 0x00000000) Command                                               */
    __IOM uint32_t ARG;                                                                     /*!< (@ 0x00000004) Command argument                                      */
    __IOM uint32_t RESPONSEADDR;                                                            /*!< (@ 0x00000008) Command response address                              */
    __IOM uint32_t RESPONSE[4];                                                             /*!< (@ 0x0000000C) Command response                                      */
    __IOM uint32_t SPISADDR;                                                                /*!< (@ 0x0000001C) SPI slave instance address                            */
} NRF_SP_EMMC_COMMAND_Type;                                                                 /*!< Size = 32 (0x020)                                                    */

/* SP_EMMC_COMMAND_CMD: Command */
  #define SP_EMMC_COMMAND_CMD_ResetValue (0x00000000UL)                                     /*!< Reset value of CMD register.                                      */

/* IDX @Bits 0..15 : Command index */
  #define SP_EMMC_COMMAND_CMD_IDX_Pos (0UL)                                                 /*!< Position of IDX field.                                               */
  #define SP_EMMC_COMMAND_CMD_IDX_Msk (0xFFFFUL << SP_EMMC_COMMAND_CMD_IDX_Pos)             /*!< Bit mask of IDX field.                    */

/* RESPTYPE @Bits 16..23 : Response type */
  #define SP_EMMC_COMMAND_CMD_RESPTYPE_Pos  (16UL)                                          /*!< Position of RESPTYPE field.                                          */
  #define SP_EMMC_COMMAND_CMD_RESPTYPE_Msk  (0xFFUL << SP_EMMC_COMMAND_CMD_RESPTYPE_Pos)    /*!< Bit mask of RESPTYPE field.       */
  #define SP_EMMC_COMMAND_CMD_RESPTYPE_Min  (0x0UL)                                         /*!< Min enumerator value of RESPTYPE field.                              */
  #define SP_EMMC_COMMAND_CMD_RESPTYPE_Max  (0x6UL)                                         /*!< Max enumerator value of RESPTYPE field.                              */
  #define SP_EMMC_COMMAND_CMD_RESPTYPE_NONE (0x00UL)                                        /*!< No response                                                          */
  #define SP_EMMC_COMMAND_CMD_RESPTYPE_R1   (0x01UL)                                        /*!< R1 response                                                          */
  #define SP_EMMC_COMMAND_CMD_RESPTYPE_R1B  (0x02UL)                                        /*!< R1 response with busy signal on DATA0 line                           */
  #define SP_EMMC_COMMAND_CMD_RESPTYPE_R2   (0x03UL)                                        /*!< R2 response                                                          */
  #define SP_EMMC_COMMAND_CMD_RESPTYPE_R3   (0x04UL)                                        /*!< R3 response                                                          */
  #define SP_EMMC_COMMAND_CMD_RESPTYPE_R4   (0x05UL)                                        /*!< R4 response                                                          */
  #define SP_EMMC_COMMAND_CMD_RESPTYPE_R5   (0x06UL)                                        /*!< R5 response                                                          */

/* RESPPROC @Bits 24..31 : Response process */
  #define SP_EMMC_COMMAND_CMD_RESPPROC_Pos     (24UL)                                       /*!< Position of RESPPROC field.                                          */
  #define SP_EMMC_COMMAND_CMD_RESPPROC_Msk     (0xFFUL << SP_EMMC_COMMAND_CMD_RESPPROC_Pos) /*!< Bit mask of RESPPROC field.       */
  #define SP_EMMC_COMMAND_CMD_RESPPROC_Min     (0x0UL)                                      /*!< Min enumerator value of RESPPROC field.                              */
  #define SP_EMMC_COMMAND_CMD_RESPPROC_Max     (0x2UL)                                      /*!< Max enumerator value of RESPPROC field.                              */
  #define SP_EMMC_COMMAND_CMD_RESPPROC_PROCESS (0x00UL)                                     /*!< Process the response                                              */
  #define SP_EMMC_COMMAND_CMD_RESPPROC_IGNORE  (0x01UL)                                     /*!< Ignore the response                                                */

/* SP_EMMC_COMMAND_ARG: Command argument */
  #define SP_EMMC_COMMAND_ARG_ResetValue (0x00000000UL)                                     /*!< Reset value of ARG register.                                      */

/* ARG @Bits 0..31 : Argument */
  #define SP_EMMC_COMMAND_ARG_ARG_Pos (0UL)                                                 /*!< Position of ARG field.                                               */
  #define SP_EMMC_COMMAND_ARG_ARG_Msk (0xFFFFFFFFUL << SP_EMMC_COMMAND_ARG_ARG_Pos)         /*!< Bit mask of ARG field.                */

/* SP_EMMC_COMMAND_RESPONSEADDR: Command response address */
  #define SP_EMMC_COMMAND_RESPONSEADDR_ResetValue (0x00000000UL)                            /*!< Reset value of RESPONSEADDR register.                    */

/* RESPONSEADDR @Bits 0..31 : Memory address to which SEMMC will copy the response for a command This address is assumed to be
 *                            the beginning of an array of 4 32 bit words. Meant to be used for a pointer in red code */

  #define SP_EMMC_COMMAND_RESPONSEADDR_RESPONSEADDR_Pos (0UL)                               /*!< Position of RESPONSEADDR field.                             */
  #define SP_EMMC_COMMAND_RESPONSEADDR_RESPONSEADDR_Msk                                                                    \
                                                           (0xFFFFFFFFUL << SP_EMMC_COMMAND_RESPONSEADDR_RESPONSEADDR_Pos) \
                                                                                            /*!< Bit
                                                                                             * mask of RESPONSEADDR field.*/
/* SP_EMMC_COMMAND_RESPONSE: Command response */
  #define SP_EMMC_COMMAND_RESPONSE_MaxCount   (4UL)                                         /*!< Max size of RESPONSE[4] array.                                       */
  #define SP_EMMC_COMMAND_RESPONSE_MaxIndex   (3UL)                                         /*!< Max index of RESPONSE[4] array.                                      */
  #define SP_EMMC_COMMAND_RESPONSE_MinIndex   (0UL)                                         /*!< Min index of RESPONSE[4] array.                                      */
  #define SP_EMMC_COMMAND_RESPONSE_ResetValue (0x00000000UL)                                /*!< Reset value of RESPONSE[4] register.                         */

/* RESPONSE @Bits 0..31 : (unspecified) */
  #define SP_EMMC_COMMAND_RESPONSE_RESPONSE_Pos (0UL)                                       /*!< Position of RESPONSE field.                                         */
  #define SP_EMMC_COMMAND_RESPONSE_RESPONSE_Msk                                                            \
                                                   (0xFFFFFFFFUL << SP_EMMC_COMMAND_RESPONSE_RESPONSE_Pos) \
                                                                                            /*!< Bit mask of
                                                                                             * RESPONSE field.*/

/* SP_EMMC_COMMAND_SPISADDR: SPI slave instance address */
  #define SP_EMMC_COMMAND_SPISADDR_ResetValue (0x00000000UL)                                /*!< Reset value of SPISADDR register.                            */

/* SPISADDR @Bits 0..31 : Memory address that SEMMC will use to configure a SPI slave to process responses from eMMC This
 *                        address is meant to be used for a pointer in red code */

  #define SP_EMMC_COMMAND_SPISADDR_SPISADDR_Pos (0UL)                                       /*!< Position of SPISADDR field.                                         */
  #define SP_EMMC_COMMAND_SPISADDR_SPISADDR_Msk                                                            \
                                                   (0xFFFFFFFFUL << SP_EMMC_COMMAND_SPISADDR_SPISADDR_Pos) \
                                                                                            /*!< Bit mask of
                                                                                             * SPISADDR field.*/

/* =================================================== Struct SP_EMMC_DATA =================================================== */
/**
 * @brief DATA [SP_EMMC_DATA] SEMMC data transfer descriptor
 */
typedef struct
{
    __IOM uint32_t BUFFERADDR;                                                                    /*!< (@ 0x00000000) Address of the data buffer                            */
    __IOM uint32_t BLOCKSIZE;                                                                     /*!< (@ 0x00000004) Block size                                            */
    __IOM uint32_t BLOCKNUM;                                                                      /*!< (@ 0x00000008) Number of blocks to transfer                          */
} NRF_SP_EMMC_DATA_Type;                                                                          /*!< Size = 12 (0xC)                                                    */

/* SP_EMMC_DATA_BUFFERADDR: Address of the data buffer */
  #define SP_EMMC_DATA_BUFFERADDR_ResetValue (0x00000000UL)                                       /*!< Reset value of BUFFERADDR register.                           */

/* BUFFERADDR @Bits 0..31 : Address of the data buffer */
  #define SP_EMMC_DATA_BUFFERADDR_BUFFERADDR_Pos (0UL)                                            /*!< Position of BUFFERADDR field.                                      */
  #define SP_EMMC_DATA_BUFFERADDR_BUFFERADDR_Msk                                                             \
                                                    (0xFFFFFFFFUL << SP_EMMC_DATA_BUFFERADDR_BUFFERADDR_Pos) \
                                                                                                  /*!< Bit mask of
                                                                                                   * BUFFERADDR field.*/
/* SP_EMMC_DATA_BLOCKSIZE: Block size */
  #define SP_EMMC_DATA_BLOCKSIZE_ResetValue (0x00000200UL)                                        /*!< Reset value of BLOCKSIZE register.                             */

/* BLOCKSIZE @Bits 0..31 : Block size */
  #define SP_EMMC_DATA_BLOCKSIZE_BLOCKSIZE_Pos (0UL)                                              /*!< Position of BLOCKSIZE field.                                         */
  #define SP_EMMC_DATA_BLOCKSIZE_BLOCKSIZE_Msk                                                           \
                                                  (0xFFFFFFFFUL << SP_EMMC_DATA_BLOCKSIZE_BLOCKSIZE_Pos) \
                                                                                                  /*!< Bit mask of BLOCKSIZE
                                                                                                   * field.*/

/* SP_EMMC_DATA_BLOCKNUM: Number of blocks to transfer */
  #define SP_EMMC_DATA_BLOCKNUM_ResetValue (0x00000001UL)                                         /*!< Reset value of BLOCKNUM register.                               */

/* BLOCKNUM @Bits 0..31 : Number of blocks to transfer */
  #define SP_EMMC_DATA_BLOCKNUM_BLOCKNUM_Pos (0UL)                                                /*!< Position of BLOCKNUM field.                                          */
  #define SP_EMMC_DATA_BLOCKNUM_BLOCKNUM_Msk (0xFFFFFFFFUL << SP_EMMC_DATA_BLOCKNUM_BLOCKNUM_Pos) /*!< Bit mask of BLOCKNUM
                                                                                                   * field.*/

/* ================================================== Struct SP_EMMC_STATUS ================================================== */
/**
 * @brief STATUS [SP_EMMC_STATUS] SEMMC status
 */
typedef struct
{
    __IOM uint32_t STATUS;                                                                           /*!< (@ 0x00000000) SEMMC status register                                 */
} NRF_SP_EMMC_STATUS_Type;                                                                           /*!< Size = 4 (0x004)                                                     */

/* SP_EMMC_STATUS_STATUS: SEMMC status register */
  #define SP_EMMC_STATUS_STATUS_ResetValue (0x00000000UL)                                            /*!< Reset value of STATUS register.                                 */

/* CMDTIMEOUT @Bit 0 : Command timeout */
  #define SP_EMMC_STATUS_STATUS_CMDTIMEOUT_Pos (0UL)                                                 /*!< Position of CMDTIMEOUT field.                                        */
  #define SP_EMMC_STATUS_STATUS_CMDTIMEOUT_Msk (0x1UL << SP_EMMC_STATUS_STATUS_CMDTIMEOUT_Pos)       /*!< Bit mask of CMDTIMEOUT
                                                                                                      * field.*/

/* CMDCRCERROR @Bit 1 : CRC Error in the command response */
  #define SP_EMMC_STATUS_STATUS_CMDCRCERROR_Pos (1UL)                                                /*!< Position of CMDCRCERROR field.                                      */
  #define SP_EMMC_STATUS_STATUS_CMDCRCERROR_Msk (0x1UL << SP_EMMC_STATUS_STATUS_CMDCRCERROR_Pos)     /*!< Bit mask of CMDCRCERROR
                                                                                                      * field.*/

/* DATACRCERROR @Bit 2 : CRC Error during data transfer (read) */
  #define SP_EMMC_STATUS_STATUS_DATACRCERROR_Pos (2UL)                                               /*!< Position of DATACRCERROR field.                                    */
  #define SP_EMMC_STATUS_STATUS_DATACRCERROR_Msk (0x1UL << SP_EMMC_STATUS_STATUS_DATACRCERROR_Pos)   /*!< Bit mask of DATACRCERROR
                                                                                                      * field.*/

/* RETRYEXCEEDED @Bit 3 : Exceeded number in NUMRETRIES */
  #define SP_EMMC_STATUS_STATUS_RETRYEXCEEDED_Pos (3UL)                                              /*!< Position of RETRYEXCEEDED field.                                  */
  #define SP_EMMC_STATUS_STATUS_RETRYEXCEEDED_Msk (0x1UL << SP_EMMC_STATUS_STATUS_RETRYEXCEEDED_Pos) /*!< Bit mask of
                                                                                                      * RETRYEXCEEDED field.*/

/* PROTOCOLERR @Bit 4 : Protocol error, generic error response */
  #define SP_EMMC_STATUS_STATUS_PROTOCOLERR_Pos (4UL)                                                /*!< Position of PROTOCOLERR field.                                      */
  #define SP_EMMC_STATUS_STATUS_PROTOCOLERR_Msk (0x1UL << SP_EMMC_STATUS_STATUS_PROTOCOLERR_Pos)     /*!< Bit mask of PROTOCOLERR
                                                                                                      * field.*/

/* ================================================== Struct SP_EMMC_SPSYNC ================================================== */
/**
 * @brief SPSYNC [SP_EMMC_SPSYNC] Registers used to acknowledge API function calls
 */
typedef struct
{
    __IOM uint32_t AUX[6];                                                                          /*!< (@ 0x00000000) Auxiliary registers for XSB macro call handshaking    */
} NRF_SP_EMMC_SPSYNC_Type;                                                                          /*!< Size = 24 (0x018)                                                    */

/* SP_EMMC_SPSYNC_AUX: Auxiliary registers for XSB macro call handshaking */
  #define SP_EMMC_SPSYNC_AUX_MaxCount   (6UL)                                                       /*!< Max size of AUX[6] array.                                            */
  #define SP_EMMC_SPSYNC_AUX_MaxIndex   (5UL)                                                       /*!< Max index of AUX[6] array.                                           */
  #define SP_EMMC_SPSYNC_AUX_MinIndex   (0UL)                                                       /*!< Min index of AUX[6] array.                                           */
  #define SP_EMMC_SPSYNC_AUX_ResetValue (0x00000000UL)                                              /*!< Reset value of AUX[6] register.                                    */

/* AUX @Bits 0..31 : Auxiliary register */
  #define SP_EMMC_SPSYNC_AUX_AUX_Pos (0UL)                                                          /*!< Position of AUX field.                                               */
  #define SP_EMMC_SPSYNC_AUX_AUX_Msk (0xFFFFFFFFUL << SP_EMMC_SPSYNC_AUX_AUX_Pos)                   /*!< Bit mask of AUX field.                  */

/* ===================================================== Struct SP_EMMC ====================================================== */
/**
 * @brief Soft peripheral eMMC
 */
typedef struct                                                                                      /*!< SP_EMMC Structure                                                    */
{
    __OM uint32_t                  TASKS_START;                                                     /*!< (@ 0x00000000) Trigger this task to send a command (which starts a
                                                                                                     *                  data transfer, if applicable)*/
    __OM uint32_t                  TASKS_RESET;                                                     /*!< (@ 0x00000004) Trigger this task to reset SEMMC to UNINITIALIZED
                                                                                                     *                  state*/
    __IOM uint32_t                 SUBSCRIBE_START;                                                 /*!< (@ 0x00000008) Subscribe configuration for task START                */
    __IOM uint32_t                 SUBSCRIBE_RESET;                                                 /*!< (@ 0x0000000C) Subscribe configuration for task RESET                */
    __IOM uint32_t                 EVENTS_XFERCOMPLETE;                                             /*!< (@ 0x00000010) This event signals that SEMMC has completed a command
                                                                                                     *                  or transfer (if applicable)*/
    __IOM uint32_t                 EVENTS_ABORTED;                                                  /*!< (@ 0x00000014) This event signals that SEMMC has aborted in the middle
                                                                                                     *                  of a command/transfer or has timed out*/
    __IOM uint32_t                 EVENTS_READYTOTRANSFER;                                          /*!< (@ 0x00000018) This event signals that SEMMC is ready to start a
                                                                                                     *                  transfer*/
    __IOM uint32_t                 PUBLISH_XFERCOMPLETE;                                            /*!< (@ 0x0000001C) Publish configuration for event XFERCOMPLETE          */
    __IOM uint32_t                 PUBLISH_ABORTED;                                                 /*!< (@ 0x00000020) Publish configuration for event ABORTED               */
    __IOM uint32_t                 PUBLISH_READYTOTRANSFER;                                         /*!< (@ 0x00000024) Publish configuration for event READYTOTRANSFER       */
    __IOM uint32_t                 INTEN;                                                           /*!< (@ 0x00000028) Enable or disable interrupt                           */
    __IOM uint32_t                 ENABLE;                                                          /*!< (@ 0x0000002C) Enable the SEMMC peripheral                           */
    __IOM NRF_SP_EMMC_CONFIG_Type  CONFIG;                                                          /*!< (@ 0x00000030) SEMMC configuration                                   */
    __IOM NRF_SP_EMMC_COMMAND_Type COMMAND;                                                         /*!< (@ 0x00000044) SEMMC command descriptor                              */
    __IOM NRF_SP_EMMC_DATA_Type    DATA;                                                            /*!< (@ 0x00000064) SEMMC data transfer descriptor                        */
    __IOM NRF_SP_EMMC_STATUS_Type  STATUS;                                                          /*!< (@ 0x00000070) SEMMC status                                          */
    __IOM NRF_SP_EMMC_SPSYNC_Type  SPSYNC;                                                          /*!< (@ 0x00000074) Registers used to acknowledge API function calls      */
} NRF_SP_EMMC_Type;                                                                                 /*!< Size = 140 (0x08C)                                                   */

/* SP_EMMC_TASKS_START: Trigger this task to send a command (which starts a data transfer, if applicable) */
  #define SP_EMMC_TASKS_START_ResetValue (0x00000000UL)                                             /*!< Reset value of TASKS_START register.                              */

/* TASKS_START @Bit 0 : Trigger this task to send a command (which starts a data transfer, if applicable) */
  #define SP_EMMC_TASKS_START_TASKS_START_Pos     (0UL)                                             /*!< Position of TASKS_START field.                                       */
  #define SP_EMMC_TASKS_START_TASKS_START_Msk     (0x1UL << SP_EMMC_TASKS_START_TASKS_START_Pos)    /*!< Bit mask of TASKS_START
                                                                                                     * field.*/
  #define SP_EMMC_TASKS_START_TASKS_START_Min     (0x1UL)                                           /*!< Min enumerator value of TASKS_START field.                          */
  #define SP_EMMC_TASKS_START_TASKS_START_Max     (0x1UL)                                           /*!< Max enumerator value of TASKS_START field.                          */
  #define SP_EMMC_TASKS_START_TASKS_START_Trigger (0x1UL)                                           /*!< Trigger task                                                    */

/* SP_EMMC_TASKS_RESET: Trigger this task to reset SEMMC to UNINITIALIZED state */
  #define SP_EMMC_TASKS_RESET_ResetValue (0x00000000UL)                                             /*!< Reset value of TASKS_RESET register.                              */

/* TASKS_RESET @Bit 0 : Trigger this task to reset SEMMC to UNINITIALIZED state */
  #define SP_EMMC_TASKS_RESET_TASKS_RESET_Pos     (0UL)                                             /*!< Position of TASKS_RESET field.                                       */
  #define SP_EMMC_TASKS_RESET_TASKS_RESET_Msk     (0x1UL << SP_EMMC_TASKS_RESET_TASKS_RESET_Pos)    /*!< Bit mask of TASKS_RESET
                                                                                                     * field.*/
  #define SP_EMMC_TASKS_RESET_TASKS_RESET_Min     (0x1UL)                                           /*!< Min enumerator value of TASKS_RESET field.                          */
  #define SP_EMMC_TASKS_RESET_TASKS_RESET_Max     (0x1UL)                                           /*!< Max enumerator value of TASKS_RESET field.                          */
  #define SP_EMMC_TASKS_RESET_TASKS_RESET_Trigger (0x1UL)                                           /*!< Trigger task                                                    */

/* SP_EMMC_SUBSCRIBE_START: Subscribe configuration for task START */
  #define SP_EMMC_SUBSCRIBE_START_ResetValue (0x00000000UL)                                         /*!< Reset value of SUBSCRIBE_START register.                      */

/* CHIDX @Bits 0..7 : DPPI channel that task START will subscribe to */
  #define SP_EMMC_SUBSCRIBE_START_CHIDX_Pos (0UL)                                                   /*!< Position of CHIDX field.                                             */
  #define SP_EMMC_SUBSCRIBE_START_CHIDX_Msk (0xFFUL << SP_EMMC_SUBSCRIBE_START_CHIDX_Pos)           /*!< Bit mask of CHIDX field.        */
  #define SP_EMMC_SUBSCRIBE_START_CHIDX_Min (0x00UL)                                                /*!< Min value of CHIDX field.                                            */
  #define SP_EMMC_SUBSCRIBE_START_CHIDX_Max (0xFFUL)                                                /*!< Max size of CHIDX field.                                             */

/* EN @Bit 31 : (unspecified) */
  #define SP_EMMC_SUBSCRIBE_START_EN_Pos      (31UL)                                                /*!< Position of EN field.                                                */
  #define SP_EMMC_SUBSCRIBE_START_EN_Msk      (0x1UL << SP_EMMC_SUBSCRIBE_START_EN_Pos)             /*!< Bit mask of EN field.                  */
  #define SP_EMMC_SUBSCRIBE_START_EN_Min      (0x0UL)                                               /*!< Min enumerator value of EN field.                                    */
  #define SP_EMMC_SUBSCRIBE_START_EN_Max      (0x1UL)                                               /*!< Max enumerator value of EN field.                                    */
  #define SP_EMMC_SUBSCRIBE_START_EN_Disabled (0x0UL)                                               /*!< Disable subscription                                                */
  #define SP_EMMC_SUBSCRIBE_START_EN_Enabled  (0x1UL)                                               /*!< Enable subscription                                                  */

/* SP_EMMC_SUBSCRIBE_RESET: Subscribe configuration for task RESET */
  #define SP_EMMC_SUBSCRIBE_RESET_ResetValue (0x00000000UL)                                         /*!< Reset value of SUBSCRIBE_RESET register.                      */

/* CHIDX @Bits 0..7 : DPPI channel that task RESET will subscribe to */
  #define SP_EMMC_SUBSCRIBE_RESET_CHIDX_Pos (0UL)                                                   /*!< Position of CHIDX field.                                             */
  #define SP_EMMC_SUBSCRIBE_RESET_CHIDX_Msk (0xFFUL << SP_EMMC_SUBSCRIBE_RESET_CHIDX_Pos)           /*!< Bit mask of CHIDX field.        */
  #define SP_EMMC_SUBSCRIBE_RESET_CHIDX_Min (0x00UL)                                                /*!< Min value of CHIDX field.                                            */
  #define SP_EMMC_SUBSCRIBE_RESET_CHIDX_Max (0xFFUL)                                                /*!< Max size of CHIDX field.                                             */

/* EN @Bit 31 : (unspecified) */
  #define SP_EMMC_SUBSCRIBE_RESET_EN_Pos      (31UL)                                                /*!< Position of EN field.                                                */
  #define SP_EMMC_SUBSCRIBE_RESET_EN_Msk      (0x1UL << SP_EMMC_SUBSCRIBE_RESET_EN_Pos)             /*!< Bit mask of EN field.                  */
  #define SP_EMMC_SUBSCRIBE_RESET_EN_Min      (0x0UL)                                               /*!< Min enumerator value of EN field.                                    */
  #define SP_EMMC_SUBSCRIBE_RESET_EN_Max      (0x1UL)                                               /*!< Max enumerator value of EN field.                                    */
  #define SP_EMMC_SUBSCRIBE_RESET_EN_Disabled (0x0UL)                                               /*!< Disable subscription                                                */
  #define SP_EMMC_SUBSCRIBE_RESET_EN_Enabled  (0x1UL)                                               /*!< Enable subscription                                                  */

/* SP_EMMC_EVENTS_XFERCOMPLETE: This event signals that SEMMC has completed a command or transfer (if applicable) */
  #define SP_EMMC_EVENTS_XFERCOMPLETE_ResetValue (0x00000000UL)                                     /*!< Reset value of EVENTS_XFERCOMPLETE register.              */

/* EVENTS_XFERCOMPLETE @Bit 0 : This event signals that SEMMC has completed a command or transfer (if applicable) */
  #define SP_EMMC_EVENTS_XFERCOMPLETE_EVENTS_XFERCOMPLETE_Pos (0UL)                                 /*!< Position of EVENTS_XFERCOMPLETE field.                */
  #define SP_EMMC_EVENTS_XFERCOMPLETE_EVENTS_XFERCOMPLETE_Msk \
                                                                          (0x1UL << SP_EMMC_EVENTS_XFERCOMPLETE_EVENTS_XFERCOMPLETE_Pos)
                                                                                                    /*!< Bit mask of EVENTS_XFERCOMPLETE field.*/
  #define SP_EMMC_EVENTS_XFERCOMPLETE_EVENTS_XFERCOMPLETE_Min          (0x0UL)                      /*!< Min enumerator value of EVENTS_XFERCOMPLETE field.  */
  #define SP_EMMC_EVENTS_XFERCOMPLETE_EVENTS_XFERCOMPLETE_Max          (0x1UL)                      /*!< Max enumerator value of EVENTS_XFERCOMPLETE field.  */
  #define SP_EMMC_EVENTS_XFERCOMPLETE_EVENTS_XFERCOMPLETE_NotGenerated (0x0UL)                      /*!< Event not generated                        */
  #define SP_EMMC_EVENTS_XFERCOMPLETE_EVENTS_XFERCOMPLETE_Generated    (0x1UL)                      /*!< Event generated                               */

/* SP_EMMC_EVENTS_ABORTED: This event signals that SEMMC has aborted in the middle of a command/transfer or has timed out */
  #define SP_EMMC_EVENTS_ABORTED_ResetValue (0x00000000UL)                                          /*!< Reset value of EVENTS_ABORTED register.                        */

/* EVENTS_ABORTED @Bit 0 : This event signals that SEMMC has aborted in the middle of a command/transfer or has timed out */
  #define SP_EMMC_EVENTS_ABORTED_EVENTS_ABORTED_Pos          (0UL)                                  /*!< Position of EVENTS_ABORTED field.                               */
  #define SP_EMMC_EVENTS_ABORTED_EVENTS_ABORTED_Msk                                                                  \
                                                                (0x1UL << SP_EMMC_EVENTS_ABORTED_EVENTS_ABORTED_Pos) \
                                                                                                    /*!< Bit mask of
                                                                                                     * EVENTS_ABORTED field.*/
  #define SP_EMMC_EVENTS_ABORTED_EVENTS_ABORTED_Min          (0x0UL)                                /*!< Min enumerator value of EVENTS_ABORTED field.                 */
  #define SP_EMMC_EVENTS_ABORTED_EVENTS_ABORTED_Max          (0x1UL)                                /*!< Max enumerator value of EVENTS_ABORTED field.                 */
  #define SP_EMMC_EVENTS_ABORTED_EVENTS_ABORTED_NotGenerated (0x0UL)                                /*!< Event not generated                                  */
  #define SP_EMMC_EVENTS_ABORTED_EVENTS_ABORTED_Generated    (0x1UL)                                /*!< Event generated                                         */

/* SP_EMMC_EVENTS_READYTOTRANSFER: This event signals that SEMMC is ready to start a transfer */
  #define SP_EMMC_EVENTS_READYTOTRANSFER_ResetValue (0x00000000UL)                                  /*!< Reset value of EVENTS_READYTOTRANSFER register.        */

/* EVENTS_READYTOTRANSFER @Bit 0 : This event signals that SEMMC is ready to start a transfer */
  #define SP_EMMC_EVENTS_READYTOTRANSFER_EVENTS_READYTOTRANSFER_Pos (0UL)                           /*!< Position of EVENTS_READYTOTRANSFER field.       */
  #define SP_EMMC_EVENTS_READYTOTRANSFER_EVENTS_READYTOTRANSFER_Msk \
                                                                              (0x1UL << SP_EMMC_EVENTS_READYTOTRANSFER_EVENTS_READYTOTRANSFER_Pos)
                                                                                                    /*!< Bit mask of EVENTS_READYTOTRANSFER field.*/
  #define SP_EMMC_EVENTS_READYTOTRANSFER_EVENTS_READYTOTRANSFER_Min          (0x0UL)                /*!< Min enumerator value of EVENTS_READYTOTRANSFER
                                                                                                     * field.*/
  #define SP_EMMC_EVENTS_READYTOTRANSFER_EVENTS_READYTOTRANSFER_Max          (0x1UL)                /*!< Max enumerator value of EVENTS_READYTOTRANSFER
                                                                                                     * field.*/
  #define SP_EMMC_EVENTS_READYTOTRANSFER_EVENTS_READYTOTRANSFER_NotGenerated (0x0UL)                /*!< Event not generated                  */
  #define SP_EMMC_EVENTS_READYTOTRANSFER_EVENTS_READYTOTRANSFER_Generated    (0x1UL)                /*!< Event generated                         */

/* SP_EMMC_PUBLISH_XFERCOMPLETE: Publish configuration for event XFERCOMPLETE */
  #define SP_EMMC_PUBLISH_XFERCOMPLETE_ResetValue (0x00000000UL)                                    /*!< Reset value of PUBLISH_XFERCOMPLETE register.            */

/* CHIDX @Bits 0..7 : DPPI channel that event XFERCOMPLETE will publish to */
  #define SP_EMMC_PUBLISH_XFERCOMPLETE_CHIDX_Pos (0UL)                                              /*!< Position of CHIDX field.                                           */
  #define SP_EMMC_PUBLISH_XFERCOMPLETE_CHIDX_Msk (0xFFUL << SP_EMMC_PUBLISH_XFERCOMPLETE_CHIDX_Pos) /*!< Bit mask of CHIDX
                                                                                                     * field.*/
  #define SP_EMMC_PUBLISH_XFERCOMPLETE_CHIDX_Min (0x00UL)                                           /*!< Min value of CHIDX field.                                       */
  #define SP_EMMC_PUBLISH_XFERCOMPLETE_CHIDX_Max (0xFFUL)                                           /*!< Max size of CHIDX field.                                        */

/* EN @Bit 31 : (unspecified) */
  #define SP_EMMC_PUBLISH_XFERCOMPLETE_EN_Pos      (31UL)                                           /*!< Position of EN field.                                                */
  #define SP_EMMC_PUBLISH_XFERCOMPLETE_EN_Msk      (0x1UL << SP_EMMC_PUBLISH_XFERCOMPLETE_EN_Pos)   /*!< Bit mask of EN field.        */
  #define SP_EMMC_PUBLISH_XFERCOMPLETE_EN_Min      (0x0UL)                                          /*!< Min enumerator value of EN field.                                   */
  #define SP_EMMC_PUBLISH_XFERCOMPLETE_EN_Max      (0x1UL)                                          /*!< Max enumerator value of EN field.                                   */
  #define SP_EMMC_PUBLISH_XFERCOMPLETE_EN_Disabled (0x0UL)                                          /*!< Disable publishing                                             */
  #define SP_EMMC_PUBLISH_XFERCOMPLETE_EN_Enabled  (0x1UL)                                          /*!< Enable publishing                                               */

/* SP_EMMC_PUBLISH_ABORTED: Publish configuration for event ABORTED */
  #define SP_EMMC_PUBLISH_ABORTED_ResetValue (0x00000000UL)                                         /*!< Reset value of PUBLISH_ABORTED register.                      */

/* CHIDX @Bits 0..7 : DPPI channel that event ABORTED will publish to */
  #define SP_EMMC_PUBLISH_ABORTED_CHIDX_Pos (0UL)                                                   /*!< Position of CHIDX field.                                             */
  #define SP_EMMC_PUBLISH_ABORTED_CHIDX_Msk (0xFFUL << SP_EMMC_PUBLISH_ABORTED_CHIDX_Pos)           /*!< Bit mask of CHIDX field.        */
  #define SP_EMMC_PUBLISH_ABORTED_CHIDX_Min (0x00UL)                                                /*!< Min value of CHIDX field.                                            */
  #define SP_EMMC_PUBLISH_ABORTED_CHIDX_Max (0xFFUL)                                                /*!< Max size of CHIDX field.                                             */

/* EN @Bit 31 : (unspecified) */
  #define SP_EMMC_PUBLISH_ABORTED_EN_Pos      (31UL)                                                /*!< Position of EN field.                                                */
  #define SP_EMMC_PUBLISH_ABORTED_EN_Msk      (0x1UL << SP_EMMC_PUBLISH_ABORTED_EN_Pos)             /*!< Bit mask of EN field.                  */
  #define SP_EMMC_PUBLISH_ABORTED_EN_Min      (0x0UL)                                               /*!< Min enumerator value of EN field.                                    */
  #define SP_EMMC_PUBLISH_ABORTED_EN_Max      (0x1UL)                                               /*!< Max enumerator value of EN field.                                    */
  #define SP_EMMC_PUBLISH_ABORTED_EN_Disabled (0x0UL)                                               /*!< Disable publishing                                                  */
  #define SP_EMMC_PUBLISH_ABORTED_EN_Enabled  (0x1UL)                                               /*!< Enable publishing                                                    */

/* SP_EMMC_INTEN: Enable or disable interrupt */
  #define SP_EMMC_INTEN_ResetValue (0x00000000UL)                                                   /*!< Reset value of INTEN register.                                       */

/* XFERCOMPLETE @Bit 0 : Enable or disable interrupt for event XFERCOMPLETE */
  #define SP_EMMC_INTEN_XFERCOMPLETE_Pos      (0UL)                                                 /*!< Position of XFERCOMPLETE field.                                      */
  #define SP_EMMC_INTEN_XFERCOMPLETE_Msk      (0x1UL << SP_EMMC_INTEN_XFERCOMPLETE_Pos)             /*!< Bit mask of XFERCOMPLETE field.        */
  #define SP_EMMC_INTEN_XFERCOMPLETE_Min      (0x0UL)                                               /*!< Min enumerator value of XFERCOMPLETE field.                          */
  #define SP_EMMC_INTEN_XFERCOMPLETE_Max      (0x1UL)                                               /*!< Max enumerator value of XFERCOMPLETE field.                          */
  #define SP_EMMC_INTEN_XFERCOMPLETE_Disabled (0x0UL)                                               /*!< Disable                                                             */
  #define SP_EMMC_INTEN_XFERCOMPLETE_Enabled  (0x1UL)                                               /*!< Enable                                                               */

/* ABORTED @Bit 1 : Enable or disable interrupt for event ABORTED */
  #define SP_EMMC_INTEN_ABORTED_Pos      (1UL)                                                      /*!< Position of ABORTED field.                                           */
  #define SP_EMMC_INTEN_ABORTED_Msk      (0x1UL << SP_EMMC_INTEN_ABORTED_Pos)                       /*!< Bit mask of ABORTED field.                       */
  #define SP_EMMC_INTEN_ABORTED_Min      (0x0UL)                                                    /*!< Min enumerator value of ABORTED field.                               */
  #define SP_EMMC_INTEN_ABORTED_Max      (0x1UL)                                                    /*!< Max enumerator value of ABORTED field.                               */
  #define SP_EMMC_INTEN_ABORTED_Disabled (0x0UL)                                                    /*!< Disable                                                              */
  #define SP_EMMC_INTEN_ABORTED_Enabled  (0x1UL)                                                    /*!< Enable                                                               */

/* READYTOTRANSFER @Bit 2 : Enable or disable interrupt for event READYTOTRANSFER */
  #define SP_EMMC_INTEN_READYTOTRANSFER_Pos      (2UL)                                              /*!< Position of READYTOTRANSFER field.                                   */
  #define SP_EMMC_INTEN_READYTOTRANSFER_Msk      (0x1UL << SP_EMMC_INTEN_READYTOTRANSFER_Pos)       /*!< Bit mask of READYTOTRANSFER
                                                                                                     * field.*/
  #define SP_EMMC_INTEN_READYTOTRANSFER_Min      (0x0UL)                                            /*!< Min enumerator value of READYTOTRANSFER field.                       */
  #define SP_EMMC_INTEN_READYTOTRANSFER_Max      (0x1UL)                                            /*!< Max enumerator value of READYTOTRANSFER field.                       */
  #define SP_EMMC_INTEN_READYTOTRANSFER_Disabled (0x0UL)                                            /*!< Disable                                                          */
  #define SP_EMMC_INTEN_READYTOTRANSFER_Enabled  (0x1UL)                                            /*!< Enable                                                            */

/* SP_EMMC_ENABLE: Enable the SEMMC peripheral */
  #define SP_EMMC_ENABLE_ResetValue (0x00000000UL)                                                  /*!< Reset value of ENABLE register.                                      */

/* ENABLE @Bit 0 : Enable or disable SEMMC */
  #define SP_EMMC_ENABLE_ENABLE_Pos      (0UL)                                                      /*!< Position of ENABLE field.                                            */
  #define SP_EMMC_ENABLE_ENABLE_Msk      (0x1UL << SP_EMMC_ENABLE_ENABLE_Pos)                       /*!< Bit mask of ENABLE field.                        */
  #define SP_EMMC_ENABLE_ENABLE_Min      (0x0UL)                                                    /*!< Min enumerator value of ENABLE field.                                */
  #define SP_EMMC_ENABLE_ENABLE_Max      (0x1UL)                                                    /*!< Max enumerator value of ENABLE field.                                */
  #define SP_EMMC_ENABLE_ENABLE_Disabled (0x0UL)                                                    /*!< Disable SEMMC                                                        */
  #define SP_EMMC_ENABLE_ENABLE_Enabled  (0x1UL)                                                    /*!< Enable SEMMC                                                         */

#endif  /*!< !defined(__ASSEMBLER__) && !defined(__ASSEMBLY__)                    */

#endif /* NRF_SP_EMMC_H__ */
