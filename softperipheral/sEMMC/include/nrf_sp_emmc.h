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

#include "nrfx.h"                                     // Resolve nrfXX_types.h for the correct target to get definitions for __IO, __IOM etc.
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
    __IOM uint32_t READYTOTRANSFER;                                                               /*!< (@ 0x00000000) Indicates that SEMMC has parsed a configuration and it
                                                                                                   *                  is ready to transfer*/
    __IOM uint32_t CLKFREQHZ;                                                                     /*!< (@ 0x00000004) SEMMC Clock Frequency in Hz                           */
    __IOM uint32_t BUSWIDTH;                                                                      /*!< (@ 0x00000008) SEMMC Bus width                                       */
    __IOM uint32_t READDELAY;                                                                     /*!< (@ 0x0000000C) Number of FLPR clock cycles to offset for data sampling
                                                                                                   *                  point*/
    __IOM uint32_t POLLMODE;                                                                      /*!< (@ 0x00000010) sEMMC DAT0 poll function                              */
    __IOM uint32_t TIMEOUT;                                                                       /*!< (@ 0x00000014) Poll timeout                                          */
} NRF_SP_EMMC_CONFIG_Type;                                                                        /*!< Size = 24 (0x018)                                                    */

/* SP_EMMC_CONFIG_READYTOTRANSFER: Indicates that SEMMC has parsed a configuration and it is ready to transfer */
  #define SP_EMMC_CONFIG_READYTOTRANSFER_ResetValue (0x00000000UL)                                /*!< Reset value of READYTOTRANSFER register.               */

/* READYTOTRANSFER @Bit 0 : SEMMC is ready to transfer */
  #define SP_EMMC_CONFIG_READYTOTRANSFER_READYTOTRANSFER_Pos      (0UL)                           /*!< Position of READYTOTRANSFER field.                     */
  #define SP_EMMC_CONFIG_READYTOTRANSFER_READYTOTRANSFER_Msk                                                                       \
                                                                     (0x1UL << SP_EMMC_CONFIG_READYTOTRANSFER_READYTOTRANSFER_Pos) \
                                                                                                  /*!<
                                                                                                   * Bit mask of READYTOTRANSFER field.*/
  #define SP_EMMC_CONFIG_READYTOTRANSFER_READYTOTRANSFER_Min      (0x0UL)                         /*!< Min enumerator value of READYTOTRANSFER field.       */
  #define SP_EMMC_CONFIG_READYTOTRANSFER_READYTOTRANSFER_Max      (0x1UL)                         /*!< Max enumerator value of READYTOTRANSFER field.       */
  #define SP_EMMC_CONFIG_READYTOTRANSFER_READYTOTRANSFER_NotReady (0x0UL)                         /*!< SEMMC is not ready                              */
  #define SP_EMMC_CONFIG_READYTOTRANSFER_READYTOTRANSFER_Ready    (0x1UL)                         /*!< SEMMC is ready                                     */

/* SP_EMMC_CONFIG_CLKFREQHZ: SEMMC Clock Frequency in Hz */
  #define SP_EMMC_CONFIG_CLKFREQHZ_ResetValue (0x00000000UL)                                      /*!< Reset value of CLKFREQHZ register.                           */

/* CLKFREQHZ @Bits 0..31 : SEMMC Clock Frequency in Hz */
  #define SP_EMMC_CONFIG_CLKFREQHZ_CLKFREQHZ_Pos (0UL)                                            /*!< Position of CLKFREQHZ field.                                       */
  #define SP_EMMC_CONFIG_CLKFREQHZ_CLKFREQHZ_Msk                                                             \
                                                    (0xFFFFFFFFUL << SP_EMMC_CONFIG_CLKFREQHZ_CLKFREQHZ_Pos) \
                                                                                                  /*!< Bit mask of
                                                                                                   * CLKFREQHZ field.*/

/* SP_EMMC_CONFIG_BUSWIDTH: SEMMC Bus width */
  #define SP_EMMC_CONFIG_BUSWIDTH_ResetValue (0x00000001UL)                                       /*!< Reset value of BUSWIDTH register.                             */

/* BUSWIDTH @Bits 0..3 : Bus width SEMMC will use for data transfers */
  #define SP_EMMC_CONFIG_BUSWIDTH_BUSWIDTH_Pos        (0UL)                                       /*!< Position of BUSWIDTH field.                                          */
  #define SP_EMMC_CONFIG_BUSWIDTH_BUSWIDTH_Msk                                                           \
                                                         (0xFUL << SP_EMMC_CONFIG_BUSWIDTH_BUSWIDTH_Pos) \
                                                                                                  /*!< Bit mask of BUSWIDTH field.*/
  #define SP_EMMC_CONFIG_BUSWIDTH_BUSWIDTH_Min        (0x1UL)                                     /*!< Min enumerator value of BUSWIDTH field.                            */
  #define SP_EMMC_CONFIG_BUSWIDTH_BUSWIDTH_Max        (0x8UL)                                     /*!< Max enumerator value of BUSWIDTH field.                            */
  #define SP_EMMC_CONFIG_BUSWIDTH_BUSWIDTH_ONELANE    (0x1UL)                                     /*!< Data transfers will use one lane                               */
  #define SP_EMMC_CONFIG_BUSWIDTH_BUSWIDTH_FOURLANES  (0x4UL)                                     /*!< Data transfers will use four lanes                           */
  #define SP_EMMC_CONFIG_BUSWIDTH_BUSWIDTH_EIGHTLANES (0x8UL)                                     /*!< Data transfers will use eigth lanes                         */

/* SP_EMMC_CONFIG_READDELAY: Number of FLPR clock cycles to offset for data sampling point */
  #define SP_EMMC_CONFIG_READDELAY_ResetValue (0x00000000UL)                                      /*!< Reset value of READDELAY register.                           */

/* READDELAY @Bits 0..31 : Number of FLPR clock cycles to offset for data sampling point */
  #define SP_EMMC_CONFIG_READDELAY_READDELAY_Pos (0UL)                                            /*!< Position of READDELAY field.                                       */
  #define SP_EMMC_CONFIG_READDELAY_READDELAY_Msk                                                             \
                                                    (0xFFFFFFFFUL << SP_EMMC_CONFIG_READDELAY_READDELAY_Pos) \
                                                                                                  /*!< Bit mask of
                                                                                                   * READDELAY field.*/

/* SP_EMMC_CONFIG_POLLMODE: sEMMC DAT0 poll function */
  #define SP_EMMC_CONFIG_POLLMODE_ResetValue (0x00000000UL)                                       /*!< Reset value of POLLMODE register.                             */

/* POLLMODE @Bit 0 : sEMMC DAT0 poll function */
  #define SP_EMMC_CONFIG_POLLMODE_POLLMODE_Pos    (0UL)                                           /*!< Position of POLLMODE field.                                          */
  #define SP_EMMC_CONFIG_POLLMODE_POLLMODE_Msk    (0x1UL << SP_EMMC_CONFIG_POLLMODE_POLLMODE_Pos) /*!< Bit mask of POLLMODE field.*/
  #define SP_EMMC_CONFIG_POLLMODE_POLLMODE_Min    (0x0UL)                                         /*!< Min enumerator value of POLLMODE field.                            */
  #define SP_EMMC_CONFIG_POLLMODE_POLLMODE_Max    (0x1UL)                                         /*!< Max enumerator value of POLLMODE field.                            */
  #define SP_EMMC_CONFIG_POLLMODE_POLLMODE_NOPOLL (0x0UL)                                         /*!< No polling on DAT0                                              */
  #define SP_EMMC_CONFIG_POLLMODE_POLLMODE_POLL   (0x1UL)                                         /*!< Polling on DAT0                                                   */

/* SP_EMMC_CONFIG_TIMEOUT: Poll timeout */
  #define SP_EMMC_CONFIG_TIMEOUT_ResetValue (0x00000000UL)                                        /*!< Reset value of TIMEOUT register.                               */

/* TIMEOUT @Bits 0..31 : Number of micro seconds to wait for DAT0 to go high */
  #define SP_EMMC_CONFIG_TIMEOUT_TIMEOUT_Pos (0UL)                                                /*!< Position of TIMEOUT field.                                           */
  #define SP_EMMC_CONFIG_TIMEOUT_TIMEOUT_Msk (0xFFFFFFFFUL << SP_EMMC_CONFIG_TIMEOUT_TIMEOUT_Pos) /*!< Bit mask of TIMEOUT
                                                                                                   * field.*/

/* ================================================= Struct SP_EMMC_COMMAND ================================================== */
/**
 * @brief COMMAND [SP_EMMC_COMMAND] SEMMC command descriptor
 */
typedef struct
{
    __IOM uint32_t CMD;                                                                     /*!< (@ 0x00000000) Command                                               */
    __IOM uint32_t ARG;                                                                     /*!< (@ 0x00000004) Command argument                                      */
    __IOM uint32_t RESPONSEADDR;                                                            /*!< (@ 0x00000008) Command response buffer address                       */
    __IOM uint32_t RESPONSE[4];                                                             /*!< (@ 0x0000000C) Command response                                      */
} NRF_SP_EMMC_COMMAND_Type;                                                                 /*!< Size = 28 (0x01C)                                                    */

/* SP_EMMC_COMMAND_CMD: Command */
  #define SP_EMMC_COMMAND_CMD_ResetValue (0x00000000UL)                                     /*!< Reset value of CMD register.                                      */

/* IDX @Bits 0..15 : Command index */
  #define SP_EMMC_COMMAND_CMD_IDX_Pos (0UL)                                                 /*!< Position of IDX field.                                               */
  #define SP_EMMC_COMMAND_CMD_IDX_Msk (0xFFFFUL << SP_EMMC_COMMAND_CMD_IDX_Pos)             /*!< Bit mask of IDX field.                    */

/* RESPTYPE @Bits 16..23 : Response type */
  #define SP_EMMC_COMMAND_CMD_RESPTYPE_Pos  (16UL)                                          /*!< Position of RESPTYPE field.                                          */
  #define SP_EMMC_COMMAND_CMD_RESPTYPE_Msk  (0xFFUL << SP_EMMC_COMMAND_CMD_RESPTYPE_Pos)    /*!< Bit mask of RESPTYPE field.       */
  #define SP_EMMC_COMMAND_CMD_RESPTYPE_Min  (0x00UL)                                        /*!< Min enumerator value of RESPTYPE field.                              */
  #define SP_EMMC_COMMAND_CMD_RESPTYPE_Max  (0x08UL)                                        /*!< Max enumerator value of RESPTYPE field.                              */
  #define SP_EMMC_COMMAND_CMD_RESPTYPE_NONE (0x00UL)                                        /*!< No response                                                          */
  #define SP_EMMC_COMMAND_CMD_RESPTYPE_R1   (0x01UL)                                        /*!< R1 response                                                          */
  #define SP_EMMC_COMMAND_CMD_RESPTYPE_R1B  (0x02UL)                                        /*!< R1 response with busy signal on DATA0 line                           */
  #define SP_EMMC_COMMAND_CMD_RESPTYPE_R2   (0x03UL)                                        /*!< R2 response                                                          */
  #define SP_EMMC_COMMAND_CMD_RESPTYPE_R3   (0x04UL)                                        /*!< R3 response                                                          */
  #define SP_EMMC_COMMAND_CMD_RESPTYPE_R4   (0x05UL)                                        /*!< R4 response                                                          */
  #define SP_EMMC_COMMAND_CMD_RESPTYPE_R5   (0x06UL)                                        /*!< R5 response                                                          */
  #define SP_EMMC_COMMAND_CMD_RESPTYPE_R6   (0x07UL)                                        /*!< R6 response                                                          */
  #define SP_EMMC_COMMAND_CMD_RESPTYPE_R7   (0x08UL)                                        /*!< R7 response                                                          */

/* RESPPROC @Bits 24..31 : Response process */
  #define SP_EMMC_COMMAND_CMD_RESPPROC_Pos     (24UL)                                       /*!< Position of RESPPROC field.                                          */
  #define SP_EMMC_COMMAND_CMD_RESPPROC_Msk     (0xFFUL << SP_EMMC_COMMAND_CMD_RESPPROC_Pos) /*!< Bit mask of RESPPROC field.       */
  #define SP_EMMC_COMMAND_CMD_RESPPROC_Min     (0x00UL)                                     /*!< Min enumerator value of RESPPROC field.                              */
  #define SP_EMMC_COMMAND_CMD_RESPPROC_Max     (0x01UL)                                     /*!< Max enumerator value of RESPPROC field.                              */
  #define SP_EMMC_COMMAND_CMD_RESPPROC_PROCESS (0x00UL)                                     /*!< Process the response                                              */
  #define SP_EMMC_COMMAND_CMD_RESPPROC_IGNORE  (0x01UL)                                     /*!< Ignore the response                                                */

/* SP_EMMC_COMMAND_ARG: Command argument */
  #define SP_EMMC_COMMAND_ARG_ResetValue (0x00000000UL)                                     /*!< Reset value of ARG register.                                      */

/* ARG @Bits 0..31 : Argument */
  #define SP_EMMC_COMMAND_ARG_ARG_Pos (0UL)                                                 /*!< Position of ARG field.                                               */
  #define SP_EMMC_COMMAND_ARG_ARG_Msk (0xFFFFFFFFUL << SP_EMMC_COMMAND_ARG_ARG_Pos)         /*!< Bit mask of ARG field.                */

/* SP_EMMC_COMMAND_RESPONSEADDR: Command response buffer address */
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

/* =================================================== Struct SP_EMMC_DATA =================================================== */
/**
 * @brief DATA [SP_EMMC_DATA] SEMMC data transfer descriptor
 */
typedef struct
{
    __IOM uint32_t BUFFERADDR;                                                                    /*!< (@ 0x00000000) Address of the data buffer                            */
    __IOM uint32_t BLOCKSIZE;                                                                     /*!< (@ 0x00000004) Block size                                            */
    __IOM uint32_t BLOCKNUM;                                                                      /*!< (@ 0x00000008) Number of blocks to transfer                          */
    __IOM uint32_t BLOCKSDONE;                                                                    /*!< (@ 0x0000000C) Number of completed blocks                            */
    __IOM uint32_t DIRECTION;                                                                     /*!< (@ 0x00000010) Direction of data transfer                            */
    __IOM uint32_t SKIPDATACRC;                                                                   /*!< (@ 0x00000014) Skip checking for valid data CRC                      */
} NRF_SP_EMMC_DATA_Type;                                                                          /*!< Size = 24 (0x018)                                                    */

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

/* BLOCKNUM @Bits 0..31 : Number of blocks */
  #define SP_EMMC_DATA_BLOCKNUM_BLOCKNUM_Pos (0UL)                                                /*!< Position of BLOCKNUM field.                                          */
  #define SP_EMMC_DATA_BLOCKNUM_BLOCKNUM_Msk (0xFFFFFFFFUL << SP_EMMC_DATA_BLOCKNUM_BLOCKNUM_Pos) /*!< Bit mask of BLOCKNUM
                                                                                                   * field.*/

/* SP_EMMC_DATA_BLOCKSDONE: Number of completed blocks */
  #define SP_EMMC_DATA_BLOCKSDONE_ResetValue (0x00000000UL)                                       /*!< Reset value of BLOCKSDONE register.                           */

/* BLOCKSDONE @Bits 0..31 : Number of completed blocks */
  #define SP_EMMC_DATA_BLOCKSDONE_BLOCKSDONE_Pos (0UL)                                            /*!< Position of BLOCKSDONE field.                                      */
  #define SP_EMMC_DATA_BLOCKSDONE_BLOCKSDONE_Msk                                                             \
                                                    (0xFFFFFFFFUL << SP_EMMC_DATA_BLOCKSDONE_BLOCKSDONE_Pos) \
                                                                                                  /*!< Bit mask of
                                                                                                   * BLOCKSDONE field.*/

/* SP_EMMC_DATA_DIRECTION: Direction of data transfer */
  #define SP_EMMC_DATA_DIRECTION_ResetValue (0x00000001UL)                                        /*!< Reset value of DIRECTION register.                             */

/* DIRECTION @Bit 0 : Direction of data transfer */
  #define SP_EMMC_DATA_DIRECTION_DIRECTION_Pos (0UL)                                              /*!< Position of DIRECTION field.                                         */
  #define SP_EMMC_DATA_DIRECTION_DIRECTION_Msk (0x1UL << SP_EMMC_DATA_DIRECTION_DIRECTION_Pos)    /*!< Bit mask of DIRECTION
                                                                                                   * field.*/
  #define SP_EMMC_DATA_DIRECTION_DIRECTION_Min (0x0UL)                                            /*!< Min enumerator value of DIRECTION field.                           */
  #define SP_EMMC_DATA_DIRECTION_DIRECTION_Max (0x1UL)                                            /*!< Max enumerator value of DIRECTION field.                           */
  #define SP_EMMC_DATA_DIRECTION_DIRECTION_WR  (0x0UL)                                            /*!< Write                                                               */
  #define SP_EMMC_DATA_DIRECTION_DIRECTION_RD  (0x1UL)                                            /*!< Read                                                                */

/* SP_EMMC_DATA_SKIPDATACRC: Skip checking for valid data CRC */
  #define SP_EMMC_DATA_SKIPDATACRC_ResetValue (0x00000000UL)                                      /*!< Reset value of SKIPDATACRC register.                         */

/* SKIPDATACRC @Bit 0 : Skip checking for valid data CRC */
  #define SP_EMMC_DATA_SKIPDATACRC_SKIPDATACRC_Pos          (0UL)                                 /*!< Position of SKIPDATACRC field.                                   */
  #define SP_EMMC_DATA_SKIPDATACRC_SKIPDATACRC_Msk                                                                 \
                                                               (0x1UL << SP_EMMC_DATA_SKIPDATACRC_SKIPDATACRC_Pos) \
                                                                                                  /*!< Bit mask of
                                                                                                   * SKIPDATACRC field.*/
  #define SP_EMMC_DATA_SKIPDATACRC_SKIPDATACRC_Min          (0x0UL)                               /*!< Min enumerator value of SKIPDATACRC field.                     */
  #define SP_EMMC_DATA_SKIPDATACRC_SKIPDATACRC_Max          (0x1UL)                               /*!< Max enumerator value of SKIPDATACRC field.                     */
  #define SP_EMMC_DATA_SKIPDATACRC_SKIPDATACRC_CHECKDATACRC (0x0UL)                               /*!< CRC will be checked in data phase                     */
  #define SP_EMMC_DATA_SKIPDATACRC_SKIPDATACRC_SKIPDATACRC  (0x1UL)                               /*!< CRC will not be checked in data phase                  */

/* ================================================== Struct SP_EMMC_STATUS ================================================== */
/**
 * @brief STATUS [SP_EMMC_STATUS] SEMMC status
 */
typedef struct
{
    __IOM uint32_t STATUS;                                                                         /*!< (@ 0x00000000) SEMMC status register                                 */
} NRF_SP_EMMC_STATUS_Type;                                                                         /*!< Size = 4 (0x004)                                                     */

/* SP_EMMC_STATUS_STATUS: SEMMC status register */
  #define SP_EMMC_STATUS_STATUS_ResetValue (0x00000000UL)                                          /*!< Reset value of STATUS register.                                 */

/* CMDTIMEOUT @Bit 0 : Command timeout */
  #define SP_EMMC_STATUS_STATUS_CMDTIMEOUT_Pos (0UL)                                               /*!< Position of CMDTIMEOUT field.                                        */
  #define SP_EMMC_STATUS_STATUS_CMDTIMEOUT_Msk (0x1UL << SP_EMMC_STATUS_STATUS_CMDTIMEOUT_Pos)     /*!< Bit mask of CMDTIMEOUT
                                                                                                    * field.*/

/* CMDCRCERROR @Bit 1 : CRC Error in the command response */
  #define SP_EMMC_STATUS_STATUS_CMDCRCERROR_Pos (1UL)                                              /*!< Position of CMDCRCERROR field.                                      */
  #define SP_EMMC_STATUS_STATUS_CMDCRCERROR_Msk (0x1UL << SP_EMMC_STATUS_STATUS_CMDCRCERROR_Pos)   /*!< Bit mask of CMDCRCERROR
                                                                                                    * field.*/

/* DATACRCERROR @Bit 2 : CRC Error during data transfer (read) */
  #define SP_EMMC_STATUS_STATUS_DATACRCERROR_Pos (2UL)                                             /*!< Position of DATACRCERROR field.                                    */
  #define SP_EMMC_STATUS_STATUS_DATACRCERROR_Msk (0x1UL << SP_EMMC_STATUS_STATUS_DATACRCERROR_Pos) /*!< Bit mask of DATACRCERROR
                                                                                                    * field.*/

/* PROTOCOLERR @Bit 3 : Protocol error, generic error response */
  #define SP_EMMC_STATUS_STATUS_PROTOCOLERR_Pos (3UL)                                              /*!< Position of PROTOCOLERR field.                                      */
  #define SP_EMMC_STATUS_STATUS_PROTOCOLERR_Msk (0x1UL << SP_EMMC_STATUS_STATUS_PROTOCOLERR_Pos)   /*!< Bit mask of PROTOCOLERR
                                                                                                    * field.*/

/* ================================================== Struct SP_EMMC_SPSYNC ================================================== */
/**
 * @brief SPSYNC [SP_EMMC_SPSYNC] Registers used to acknowledge API function calls
 */
typedef struct
{
    __IOM uint32_t AUX[6];                                                                          /*!< (@ 0x00000000) Auxiliary registers for XSB macro call handshaking    */
    __IOM uint32_t DPPIMAP;                                                                         /*!< (@ 0x00000018) Task to handler map and DPPI subscription permissions */
} NRF_SP_EMMC_SPSYNC_Type;                                                                          /*!< Size = 28 (0x01C)                                                    */

/* SP_EMMC_SPSYNC_AUX: Auxiliary registers for XSB macro call handshaking */
  #define SP_EMMC_SPSYNC_AUX_MaxCount   (6UL)                                                       /*!< Max size of AUX[6] array.                                            */
  #define SP_EMMC_SPSYNC_AUX_MaxIndex   (5UL)                                                       /*!< Max index of AUX[6] array.                                           */
  #define SP_EMMC_SPSYNC_AUX_MinIndex   (0UL)                                                       /*!< Min index of AUX[6] array.                                           */
  #define SP_EMMC_SPSYNC_AUX_ResetValue (0x00000000UL)                                              /*!< Reset value of AUX[6] register.                                    */

/* AUX @Bits 0..31 : Auxiliary register */
  #define SP_EMMC_SPSYNC_AUX_AUX_Pos (0UL)                                                          /*!< Position of AUX field.                                               */
  #define SP_EMMC_SPSYNC_AUX_AUX_Msk (0xFFFFFFFFUL << SP_EMMC_SPSYNC_AUX_AUX_Pos)                   /*!< Bit mask of AUX field.                  */

/* SP_EMMC_SPSYNC_DPPIMAP: Task to handler map and DPPI subscription permissions */
  #define SP_EMMC_SPSYNC_DPPIMAP_ResetValue (0x03204015UL)                                          /*!< Reset value of DPPIMAP register.                               */

/* VALID @Bits 0..3 : Marks this register as carrying a map the soft peripheral should apply */
  #define SP_EMMC_SPSYNC_DPPIMAP_VALID_Pos   (0UL)                                                  /*!< Position of VALID field.                                             */
  #define SP_EMMC_SPSYNC_DPPIMAP_VALID_Msk   (0xFUL << SP_EMMC_SPSYNC_DPPIMAP_VALID_Pos)            /*!< Bit mask of VALID field.           */
  #define SP_EMMC_SPSYNC_DPPIMAP_VALID_Min   (0x5UL)                                                /*!< Min enumerator value of VALID field.                                 */
  #define SP_EMMC_SPSYNC_DPPIMAP_VALID_Max   (0x5UL)                                                /*!< Max enumerator value of VALID field.                                 */
  #define SP_EMMC_SPSYNC_DPPIMAP_VALID_Valid (0x5UL)                                                /*!< Register contents are a valid map                                    */

/* ROLE0 @Bits 4..7 : Handler invoked by task entry 0 (DPPI channel 0) */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE0_Pos    (4UL)                                                 /*!< Position of ROLE0 field.                                             */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE0_Msk    (0xFUL << SP_EMMC_SPSYNC_DPPIMAP_ROLE0_Pos)           /*!< Bit mask of ROLE0 field.           */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE0_Min    (0x0UL)                                               /*!< Min enumerator value of ROLE0 field.                                 */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE0_Max    (0x4UL)                                               /*!< Max enumerator value of ROLE0 field.                                 */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE0_NONE   (0x0UL)                                               /*!< Task dispatches nothing                                              */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE0_DPPI0  (0x1UL)                                               /*!< Start a prepared request (green0_fsm_handle_dppi_0)                  */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE0_CONFIG (0x2UL)                                               /*!< Parse a new configuration (green0_fsm_handle_config), target of the
                                                                                                     *   __CSB barrier*/
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE0_ACTION (0x3UL)                                               /*!< Run a prepared action (green0_fsm_handle_action), target of the __ASB
                                                                                                     *   barrier*/
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE0_STOP   (0x4UL)                                               /*!< Abort what is running (green0_fsm_handle_stop), target of the __SSB
                                                                                                     *   barrier*/

/* ROLE1 @Bits 8..11 : Handler invoked by task entry 1 (DPPI channel 1) */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE1_Pos    (8UL)                                                 /*!< Position of ROLE1 field.                                             */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE1_Msk    (0xFUL << SP_EMMC_SPSYNC_DPPIMAP_ROLE1_Pos)           /*!< Bit mask of ROLE1 field.           */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE1_Min    (0x0UL)                                               /*!< Min enumerator value of ROLE1 field.                                 */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE1_Max    (0x4UL)                                               /*!< Max enumerator value of ROLE1 field.                                 */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE1_NONE   (0x0UL)                                               /*!< Task dispatches nothing                                              */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE1_DPPI0  (0x1UL)                                               /*!< Start a prepared request (green0_fsm_handle_dppi_0)                  */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE1_CONFIG (0x2UL)                                               /*!< Parse a new configuration (green0_fsm_handle_config), target of the
                                                                                                     *   __CSB barrier*/
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE1_ACTION (0x3UL)                                               /*!< Run a prepared action (green0_fsm_handle_action), target of the __ASB
                                                                                                     *   barrier*/
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE1_STOP   (0x4UL)                                               /*!< Abort what is running (green0_fsm_handle_stop), target of the __SSB
                                                                                                     *   barrier*/

/* ROLE2 @Bits 12..15 : Handler invoked by task entry 2 (DPPI channel 2) */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE2_Pos    (12UL)                                                /*!< Position of ROLE2 field.                                             */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE2_Msk    (0xFUL << SP_EMMC_SPSYNC_DPPIMAP_ROLE2_Pos)           /*!< Bit mask of ROLE2 field.           */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE2_Min    (0x0UL)                                               /*!< Min enumerator value of ROLE2 field.                                 */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE2_Max    (0x4UL)                                               /*!< Max enumerator value of ROLE2 field.                                 */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE2_NONE   (0x0UL)                                               /*!< Task dispatches nothing                                              */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE2_DPPI0  (0x1UL)                                               /*!< Start a prepared request (green0_fsm_handle_dppi_0)                  */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE2_CONFIG (0x2UL)                                               /*!< Parse a new configuration (green0_fsm_handle_config), target of the
                                                                                                     *   __CSB barrier*/
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE2_ACTION (0x3UL)                                               /*!< Run a prepared action (green0_fsm_handle_action), target of the __ASB
                                                                                                     *   barrier*/
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE2_STOP   (0x4UL)                                               /*!< Abort what is running (green0_fsm_handle_stop), target of the __SSB
                                                                                                     *   barrier*/

/* ROLE3 @Bits 16..19 : Handler invoked by task entry 3 (DPPI channel 3) */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE3_Pos    (16UL)                                                /*!< Position of ROLE3 field.                                             */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE3_Msk    (0xFUL << SP_EMMC_SPSYNC_DPPIMAP_ROLE3_Pos)           /*!< Bit mask of ROLE3 field.           */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE3_Min    (0x0UL)                                               /*!< Min enumerator value of ROLE3 field.                                 */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE3_Max    (0x4UL)                                               /*!< Max enumerator value of ROLE3 field.                                 */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE3_NONE   (0x0UL)                                               /*!< Task dispatches nothing                                              */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE3_DPPI0  (0x1UL)                                               /*!< Start a prepared request (green0_fsm_handle_dppi_0)                  */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE3_CONFIG (0x2UL)                                               /*!< Parse a new configuration (green0_fsm_handle_config), target of the
                                                                                                     *   __CSB barrier*/
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE3_ACTION (0x3UL)                                               /*!< Run a prepared action (green0_fsm_handle_action), target of the __ASB
                                                                                                     *   barrier*/
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE3_STOP   (0x4UL)                                               /*!< Abort what is running (green0_fsm_handle_stop), target of the __SSB
                                                                                                     *   barrier*/

/* ROLE4 @Bits 20..23 : Handler invoked by task entry 4 (no DPPI channel) */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE4_Pos    (20UL)                                                /*!< Position of ROLE4 field.                                             */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE4_Msk    (0xFUL << SP_EMMC_SPSYNC_DPPIMAP_ROLE4_Pos)           /*!< Bit mask of ROLE4 field.           */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE4_Min    (0x0UL)                                               /*!< Min enumerator value of ROLE4 field.                                 */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE4_Max    (0x4UL)                                               /*!< Max enumerator value of ROLE4 field.                                 */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE4_NONE   (0x0UL)                                               /*!< Task dispatches nothing                                              */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE4_DPPI0  (0x1UL)                                               /*!< Start a prepared request (green0_fsm_handle_dppi_0)                  */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE4_CONFIG (0x2UL)                                               /*!< Parse a new configuration (green0_fsm_handle_config), target of the
                                                                                                     *   __CSB barrier*/
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE4_ACTION (0x3UL)                                               /*!< Run a prepared action (green0_fsm_handle_action), target of the __ASB
                                                                                                     *   barrier*/
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE4_STOP   (0x4UL)                                               /*!< Abort what is running (green0_fsm_handle_stop), target of the __SSB
                                                                                                     *   barrier*/

/* ROLE5 @Bits 24..27 : Handler invoked by task entry 5 (no DPPI channel) */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE5_Pos    (24UL)                                                /*!< Position of ROLE5 field.                                             */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE5_Msk    (0xFUL << SP_EMMC_SPSYNC_DPPIMAP_ROLE5_Pos)           /*!< Bit mask of ROLE5 field.           */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE5_Min    (0x0UL)                                               /*!< Min enumerator value of ROLE5 field.                                 */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE5_Max    (0x4UL)                                               /*!< Max enumerator value of ROLE5 field.                                 */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE5_NONE   (0x0UL)                                               /*!< Task dispatches nothing                                              */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE5_DPPI0  (0x1UL)                                               /*!< Start a prepared request (green0_fsm_handle_dppi_0)                  */
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE5_CONFIG (0x2UL)                                               /*!< Parse a new configuration (green0_fsm_handle_config), target of the
                                                                                                     *   __CSB barrier*/
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE5_ACTION (0x3UL)                                               /*!< Run a prepared action (green0_fsm_handle_action), target of the __ASB
                                                                                                     *   barrier*/
  #define SP_EMMC_SPSYNC_DPPIMAP_ROLE5_STOP   (0x4UL)                                               /*!< Abort what is running (green0_fsm_handle_stop), target of the __SSB
                                                                                                     *   barrier*/

/* PERMIT @Bits 28..31 : Per slot permission to subscribe the DPPI channel, bit n for task entry n */
  #define SP_EMMC_SPSYNC_DPPIMAP_PERMIT_Pos (28UL)                                                  /*!< Position of PERMIT field.                                            */
  #define SP_EMMC_SPSYNC_DPPIMAP_PERMIT_Msk (0xFUL << SP_EMMC_SPSYNC_DPPIMAP_PERMIT_Pos)            /*!< Bit mask of PERMIT field.        */

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
    __IOM NRF_SP_EMMC_COMMAND_Type COMMAND;                                                         /*!< (@ 0x00000048) SEMMC command descriptor                              */
    __IOM NRF_SP_EMMC_DATA_Type    DATA;                                                            /*!< (@ 0x00000064) SEMMC data transfer descriptor                        */
    __IOM NRF_SP_EMMC_STATUS_Type  STATUS;                                                          /*!< (@ 0x0000007C) SEMMC status                                          */
    __IOM NRF_SP_EMMC_SPSYNC_Type  SPSYNC;                                                          /*!< (@ 0x00000080) Registers used to acknowledge API function calls      */
} NRF_SP_EMMC_Type;                                                                                 /*!< Size = 156 (0x09C)                                                   */

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

/* SP_EMMC_PUBLISH_READYTOTRANSFER: Publish configuration for event READYTOTRANSFER */
  #define SP_EMMC_PUBLISH_READYTOTRANSFER_ResetValue (0x00000000UL)                                 /*!< Reset value of PUBLISH_READYTOTRANSFER register.      */

/* CHIDX @Bits 0..7 : DPPI channel that event READYTOTRANSFER will publish to */
  #define SP_EMMC_PUBLISH_READYTOTRANSFER_CHIDX_Pos (0UL)                                           /*!< Position of CHIDX field.                                        */
  #define SP_EMMC_PUBLISH_READYTOTRANSFER_CHIDX_Msk                                                          \
                                                       (0xFFUL << SP_EMMC_PUBLISH_READYTOTRANSFER_CHIDX_Pos) \
                                                                                                    /*!< Bit mask of CHIDX
                                                                                                     * field.*/
  #define SP_EMMC_PUBLISH_READYTOTRANSFER_CHIDX_Min (0x00UL)                                        /*!< Min value of CHIDX field.                                    */
  #define SP_EMMC_PUBLISH_READYTOTRANSFER_CHIDX_Max (0xFFUL)                                        /*!< Max size of CHIDX field.                                     */

/* EN @Bit 31 : (unspecified) */
  #define SP_EMMC_PUBLISH_READYTOTRANSFER_EN_Pos      (31UL)                                        /*!< Position of EN field.                                             */
  #define SP_EMMC_PUBLISH_READYTOTRANSFER_EN_Msk                                                           \
                                                         (0x1UL << SP_EMMC_PUBLISH_READYTOTRANSFER_EN_Pos) \
                                                                                                    /*!< Bit mask of EN field.  */
  #define SP_EMMC_PUBLISH_READYTOTRANSFER_EN_Min      (0x0UL)                                       /*!< Min enumerator value of EN field.                                */
  #define SP_EMMC_PUBLISH_READYTOTRANSFER_EN_Max      (0x1UL)                                       /*!< Max enumerator value of EN field.                                */
  #define SP_EMMC_PUBLISH_READYTOTRANSFER_EN_Disabled (0x0UL)                                       /*!< Disable publishing                                          */
  #define SP_EMMC_PUBLISH_READYTOTRANSFER_EN_Enabled  (0x1UL)                                       /*!< Enable publishing                                            */

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

#endif \
    /*!< !defined(__ASSEMBLER__) && !defined(__ASSEMBLY__)                    */

#endif /* NRF_SP_EMMC_H__ */
