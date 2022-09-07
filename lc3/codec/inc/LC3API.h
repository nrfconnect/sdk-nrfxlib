/*
 * Copyright (c) 2018-2021 T2 Software, Inc. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-T2-LC3
 */

/** @file LC3API.h
 *  @brief This file contains the API functions for the LC3 Codec implementation.
 */
#ifndef _LC3APIH_
#define _LC3APIH_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
* Includes
******************************************************************************/
#include "osal.h"

/*! \addtogroup LC3
 *  \{ */

/******************************************************************************
* Definitions
******************************************************************************/

/** @name LC3 Result Codes
 *  @anchor LC3ResultCodes */
/**@{*/
#define LC3_ERROR_OFFSET                      (-5000)                 /*!< \brief Offset for LC3 error values. */
#define LC3_RESULT_NO_ERROR                   (0)                     /*!< \brief Success. */
#define LC3_RESULT_INVALID_PARAMETER          ((LC3_ERROR_OFFSET)-1)  /*!< \brief Error: invalid parameter detected. */
#define LC3_RESULT_INSUFFICIENT_RESOURCES     ((LC3_ERROR_OFFSET)-2)  /*!< \brief Error: not enough memory available. */
#define LC3_RESULT_NOT_INITIALIZED            ((LC3_ERROR_OFFSET)-3)  /*!< \brief Error: not initialized before use. */
#define LC3_RESULT_UNKNOWN_ERROR              ((LC3_ERROR_OFFSET)-4)  /*!< \brief Error: unknown error detected. */
#define LC3_RESULT_INVALID_BITRATE            ((LC3_ERROR_OFFSET)-5)  /*!< \brief Error: invalid bitrate value. */
#define LC3_RESULT_INPUT_BUFFER_TOO_SMALL     ((LC3_ERROR_OFFSET)-6)  /*!< \brief Error: passed buffer size too small for input data. */
#define LC3_RESULT_OUTPUT_BUFFER_TOO_SMALL    ((LC3_ERROR_OFFSET)-7)  /*!< \brief Error: passed buffer size too small for output data. */
#define LC3_RESULT_INVALID_BITSPERSAMPLE      ((LC3_ERROR_OFFSET)-8)  /*!< \brief Error: invalid bits per sample value. */
#define LC3_RESULT_INVALID_SAMPLERATE         ((LC3_ERROR_OFFSET)-9)  /*!< \brief Error: invalid sample rate value. */
#define LC3_RESULT_BITERRORCONDITION          ((LC3_ERROR_OFFSET)-10) /*!< \brief Error: bit error condition detected in bit stream data. */
#define LC3_RESULT_FEATURE_NOT_SUPPORTED      ((LC3_ERROR_OFFSET)-11) /*!< \brief Error: feature not supported in this build. */
#define LC3_RESULT_STILL_IN_USE               ((LC3_ERROR_OFFSET)-12) /*!< \brief Error: Sessions not closed, buffer still in use. */
#define LC3_RESULT_ALREADY_INITIALIZED        ((LC3_ERROR_OFFSET)-13) /*!< \brief Error: Codec already initialized, call LC3Deinitialize first. */
/**@}*/

/** @name LC3 Sample Rate Bits
 *  @anchor LC3SampleRateBits */
/**@{*/
#define LC3_SAMPLE_RATE_NONE       (0x00)                           /*!< \brief No sample rate selected. */
#define LC3_SAMPLE_RATE_8_KHZ      (0x01)                           /*!< \brief 8 kHz sample rate bit. */
#define LC3_SAMPLE_RATE_16_KHZ     (0x02)                           /*!< \brief 16 kHz sample rate bit. */
#define LC3_SAMPLE_RATE_24_KHZ     (0x04)                           /*!< \brief 24 kHz sample rate bit. */
#define LC3_SAMPLE_RATE_32_KHZ     (0x08)                           /*!< \brief 32 kHz sample rate bit. */
#define LC3_SAMPLE_RATE_441_KHZ    (0x10)                           /*!< \brief 44.1 kHz sample rate bit. */
#define LC3_SAMPLE_RATE_48_KHZ     (0x20)                           /*!< \brief 48 kHz sample rate bit. */
#define LC3_SAMPLE_RATE_ALL        (0x1F)                           /*!< \brief All sample rates selected. */
/**@}*/

/******************************************************************************
* Module Typedefs
******************************************************************************/

/*! \brief LC3 encoder handle type. */
typedef void * LC3EncoderHandle_t;

/*! \brief LC3 decoder handle type. */
typedef void * LC3DecoderHandle_t;

/** \brief Frame Size for configuration.
 *
 * Defines the frame size used for global memory configuration purposes.
 * Select the specific frame size if only one will be used, otherwise select
 * both and the session open call will define which is used for a session.
 */
typedef enum
{
  LC3FrameSize10MsConfig,       /*!< \brief 10 msec frame size. */
  LC3FrameSize7_5MsConfig,      /*!< \brief 7.5 msec frame size. */
  LC3FrameSizeBothConfig        /*!< \brief 7.5 and 10 msec frame size. */
} LC3FrameSizeConfig_t;

/** \brief Session Frame Size.
 *
 * Defines the valid frame sizes for an LC3 codec session.
 */
typedef enum
{
  LC3FrameSize10Ms,       /*!< \brief 10 msec frame size. */
  LC3FrameSize7_5Ms       /*!< \brief 7.5 msec frame size. */
} LC3FrameSize_t;

/** \brief LC3 encoder input PCM data structure.
 *
 * Structure used to pass an audio frame to the encoder.
 * Incoming PCM samples must be linear with a bit depth rounded to
 * the nearest byte. A bit depth of 16 uses 2 bytes per sample, bit
 * depths of 17-24 use 3 bytes per sample, and bit depths of 25-32
 * use 4 bytes per sample. The number of PCM bytes needed per frame
 * can be calculated using LC3PCMBuffersize().
 */
typedef struct
{
  const void *PCMData;         /*!< \brief Pointer to the PCM data to encode. */
  uint16_t    PCMDataLength;   /*!< \brief Length of PCM data in bytes. */
  uint32_t    encodeBitrate;   /*!< \brief Bit rate, in bits per second, at which to encode frame. */
  uint16_t    bytesRead;       /*!< \brief Number of PCM data bytes read. */
} LC3EncodeInput_t;

/** \brief LC3 encoder output data structure.
 *
 * Structure that holds the LC3 encoder output. The output buffer is externally
 * allocated and its pointer and available byte length are passed into the LC3
 * Encoder. The number of output bit stream data bytes can be
 * calculated with LC3BitstreamBuffersize().
 */
typedef struct
{
  uint8_t *outputData;        /*!< \brief Output buffer pointer. */
  uint16_t outputDataLength;  /*!< \brief Output buffer size in bytes. */
  uint16_t bytesWritten;      /*!< \brief Number of bit stream bytes written. */
} LC3EncodeOutput_t;

/** \brief Bad Frame Indicator (BFI).
 *
 * Identifies a bit stream data frame's status as known by the device. A frame
 * known to contain bit errors or be otherwise corrupted should be passed with
 * the BadFrame value when passed to the LC3 decoder. The decoder will detect
 * bit errors internally and apply PLC automatically, but if the caller knows
 * that the frame is in error, such as from information indicated by a Bluetooth
 * controller, then this value is used to skip decoding and immediately execute
 * packet loss concealment.
 */
typedef enum
{
  GoodFrame,
  BadFrame
} LC3BFI_t;

/** \brief LC3 decoder input bit stream structure.
 *
 * Structure used to pass a frame of incoming bit stream data to the LC3 decoder.
 * If there is a known problem with the frame, such as an error condition
 * indicated by a Bluetooth controller, then badFrameIndicator should be set to
 * BadFrame. inputDataLength must represent the number of bytes exactly in the
 * frame to be decoded.
 */
typedef struct
{
  const uint8_t *inputData;           /*!< \brief Pointer to the input LC3 data to decode. */
  uint16_t       inputDataLength;     /*!< \brief Number of data bytes in the encoded frame. */
  LC3BFI_t       badFrameIndicator;   /*!< \brief Frame condition as detected by device. */
} LC3DecodeInput_t;

/** \brief LC3 decoder output PCM structure.
 *
 * This structure is used to hold the decoded PCM audio samples of an audio stream.
 * The buffer is externally allocated and its pointer and size are passed to the
 * LC3 decoder. bytesWritten contains the number of bytes written to the buffer.
 * The number of PCM bytes needed per frame can be calculated with
 * LC3PCMBuffersize(). If PLC has been applied to the frame, PLCCounter will
 * increment by one. A frame decoded without any bit errors resets PLCCounter
 * to zero.
 */
typedef struct
{
  void    *PCMData;         /*!< \brief Output buffer pointer. */
  uint16_t PCMDataLength;   /*!< \brief Output buffer size in bytes. */
  uint16_t bytesWritten;    /*!< \brief Number of bytes written to the output buffer. */
  uint16_t PLCCounter;      /*!< \brief Number of successive frames to which PLC has been applied. */
} LC3DecodeOutput_t;


/******************************************************************************
* Function Prototypes
******************************************************************************/
/**@brief Initializes the LC3 Codec.
 *
 * This function initializes the LC3 codec for the sample rates(s) specified in
 * the encoder/decoder sample rate parameter bit masks. The sample rate bits are
 * defined in @ref LC3SampleRateBits. The user must provide all required
 * encoder/decoder sample rates to this function.  If the exact sample rates
 * are unknown, the LC3_SAMPLE_RATE_ALL value should be entered.
 *
 * frameSizeConfig defines if the codec will support 7.5 msec frames, 10 msec
 * frames or both.  If the frame size isn't known in advance, setting this to
 * both allows each session to use either 7.5 or 10 msec but will use more
 * memory.
 *
 * The uniqueSessions value is the number of unique (sample rates * frame sizes)
 * sessions expected to be used simultaneously; the sample rates of 48k and
 * 44.1k are considered a single sample rate for this calculation.  If nothing
 * is known of the expected sessions' sample rates and/or frame sizes, this
 * can be set to the number of simultaneous encode + decode sessions.  Setting
 * this to the total number of (unique sample rates * frame sizes) will
 * initialize data tables globally, otherwise they will be initialized as
 * needed when sessions are opened.
 *
 * When LC3_SAMPLE_RATE_ALL and/or LC3FrameSizeBothConfig are used and the
 * uniqueSessions value is less than the number of (sample rates * frame sizes),
 * the codec allocates the worse case shared memory required based on the
 * uniqueSessions value. The LC3EncodeSessionOpen and/or LC3DecodeSessionOpen
 * call initializes the tables required for each session's sample rate and
 * frame size.  In general, the smallest memory footprint will be achieved by
 * defining the sample rate bit fields and a single frame size.
 *
 * This function can allocate its own memory or take a user-provided buffer. It
 * can also return the amount of memory needed without initializing. These memory
 * options are performed in the following ways:
 *
 * <b>bufferSize = NULL:</b> Allocates memory needed. The buffer parameter may be NULL.
 *
 * <b>*bufferSize < memory required by codec:</b> Sets bufferSize's referenced value to
 * the total memory required and returns @ref LC3_RESULT_INSUFFICIENT_RESOURCES. The
 * caller can then call this function again with a buffer of at least bufferSize's
 * referenced value. A size of 0 will always trigger this condition. The buffer
 * parameter may be NULL.
 *
 * <b>*bufferSize >= memory required by codec:</b> Uses the user-provided buffer.
 * bufferSize's referenced value is set to the amount of memory used by the codec.
 * The buffer parameter may not be NULL.
 *
 * @param[in]     encoderSampleRates   Bitfield containing all encoder session sample rates
 *                                     required.
 * @param[in]     decoderSampleRates   Bitfield containing all decoder session sample rates
 *                                     required.
 * @param[in]     frameSizeConfig      Frame size of 7.5, 10 msec or both.
 * @param[in]     uniqueSessions       The number of unique simultaneous sample rate * frame
 *                                     size sessions.
 * @param[in]     buffer               Pointer to a memory buffer.
 * @param[in,out] bufferSize           Size of the memory buffer passed in, number of bytes
 *                                     used returned.
 *
 * @returns  Zero on success or one of the defined @ref LC3ResultCodes "LC3 result codes" on error.
 */
int32_t OSALCALL LC3Initialize(uint8_t encoderSampleRates, uint8_t decoderSampleRates, LC3FrameSizeConfig_t frameSizeConfig, uint8_t uniqueSessions, uint8_t *buffer, uint32_t *bufferSize);


/**@brief Deinitializes the LC3 Codec.
 *
 * This function deinitializes the LC3 Codec. All encoder and decoder sessions
 * should be closed before calling this function. Memory passed in the
 * LC3Initialize's buffer parameter can be freed only after this function
 * returns @ref LC3_RESULT_NO_ERROR. If LC3Initialize allocated memory internally,
 * that memory will be freed upon a successful call to this function.
 *
 * If the result code @ref LC3_RESULT_STILL_IN_USE is returned, there are still
 * open sessions. No related resources are freed until all sessions
 * associated with the handle are closed.
 *
 * @returns  Zero on success or one of the defined @ref LC3ResultCodes "LC3 result codes" on error.
 */
int32_t OSALCALL LC3Deinitialize(void);


/**@brief Opens and initializes an LC3 Encoder session.
 *
 * This function initializes the LC3 encoder in preparation for encoding one stream
 * of audio. It allocates the memory needed for the session's processing, including
 * any used for tracking progress from frame to frame, and initializes the allocated
 * memory.
 *
 * A non-NULL handle to the encoder session is returned when successful. The handle
 * value is passed to future calls of LC3EncodeSessionData() and
 * LC3EncodeSessionClose() for this session.
 *
 * This function can allocate its own memory or take a user-provided buffer. It
 * can also return the amount of memory needed without initializing. These memory
 * options are performed in the following ways:
 *
 * <b>bufferSize = NULL:</b> Allocates memory needed. The buffer parameter may be NULL.
 *
 * <b>*bufferSize < memory required by encoder:</b> Sets bufferSize's referenced value
 * to the total memory required and returns @ref LC3_RESULT_INSUFFICIENT_RESOURCES. The
 * caller can then call this function again with a buffer of at least bufferSize's
 * referenced value. A size of 0 will always trigger this condition. The buffer
 * parameter may be NULL.
 *
 * <b>*bufferSize >= memory required by encoder:</b> Uses the user-provided buffer.
 * bufferSize's referenced value is set to the amount of memory used by the encoder.
 * The buffer parameter may not be NULL.
 *
 * @param[in]     sampleRate    Input PCM sample rate in Hz.
 * @param[in]     bitsPerSample Input PCM bits per sample.
 * @param[in]     frameSize     Frame size of 7.5 or 10 msec.
 * @param[in]     buffer        Pointer to a memory buffer.
 * @param[in,out] bufferSize    Size of the memory buffer passed in, number of bytes used returned.
 * @param[out]    result        Pointer to an LC3 Return Code integer.
 *
 * @returns  Non-NULL LC3EncoderHandle_t on success or NULL on failure.
 */
LC3EncoderHandle_t OSALCALL LC3EncodeSessionOpen(uint16_t sampleRate, uint8_t bitsPerSample, LC3FrameSize_t frameSize, uint8_t *buffer, uint16_t *bufferSize, int32_t *result);


/**@brief Encodes a frame of data using LC3.
 *
 * This function processes the audio frame data passed in the encodeInput structure.
 * It returns the encoded data in the encodeOutput structure.
 *
 * @param[in] encodeHandle  Handle to an encoder instance.
 * @param[in] encodeInput   Pointer to a structure pointing to the frame's audio
 *                          samples to encode.
 * @param[in] encodeOutput  Pointer to a structure to receive the frame's encoded data.
 *
 * @returns  Zero on success or one of the defined @ref LC3ResultCodes "LC3 result codes" on error.
 */
int32_t OSALCALL LC3EncodeSessionData(LC3EncoderHandle_t encodeHandle, LC3EncodeInput_t *encodeInput, LC3EncodeOutput_t *encodeOutput);


/**@brief Closes an LC3 Encoder session.
 *
 * This function closes the encodeHandle's session and releases allocated session memory.
 * Memory passed in the LC3EncodeSessionOpen's buffer parameter can be freed after
 * this returns.  The encodeHandle is no longer valid and should not be reused.
 *
 * @param[in] encodeHandle  Handle to the encoder instance to close.
 *
 * @returns  None.
 */
void OSALCALL LC3EncodeSessionClose(LC3EncoderHandle_t encodeHandle);


/**@brief Opens and initializes an LC3 Decoder session.
 *
 * This function initializes the LC3 decoder in preparation for decoding one stream
 * of audio. It allocates the memory needed for the session's processing, including
 * any used for tracking progress from frame to frame, and initializes the allocated
 * memory.
 *
 * A non-NULL handle to the decoder session is returned when successful. The handle
 * value is passed to future calls of LC3DecodeSessionData() and
 * LC3DecodeSessionClose() for this session.
 *
 * This function can allocate its own memory or take a user-provided buffer. It
 * can also return the amount of memory needed without initializing. These memory
 * options are performed in the following ways:
 *
 * <b>bufferSize = NULL:</b> Allocates memory needed. The buffer parameter may be NULL.
 *
 * <b>*bufferSize < memory required by decoder:</b> Sets bufferSize's referenced value
 * to the total memory required and returns @ref LC3_RESULT_INSUFFICIENT_RESOURCES. The
 * caller can then call this function again with a buffer of at least bufferSize's
 * referenced value. A size of 0 will always trigger this condition. The buffer
 * parameter may be NULL.
 *
 * <b>*bufferSize >= memory required by decoder:</b> Uses the user-provided buffer.
 * bufferSize's referenced value is set to the amount of memory used by the decoder.
 * The buffer parameter may not be NULL.
 *
 * @param[in]      sampleRate    Output PCM sample rate in Hz.
 * @param[in]      bitsPerSample Output PCM bits per sample.
 * @param[in]      frameSize     Frame size of 7.5 or 10 msec.
 * @param[in]      buffer        Pointer to a memory buffer.
 * @param[in, out] bufferSize    Size of the memory buffer passed in, number of bytes used returned.
 * @param[out]     result        Pointer to an LC3 Return Code integer value.
 *
 * @returns  Non-NULL LC3DecoderHandle_t on success or NULL on failure.
 */
LC3DecoderHandle_t OSALCALL LC3DecodeSessionOpen(uint16_t sampleRate, uint8_t bitsPerSample, LC3FrameSize_t frameSize, uint8_t *buffer, uint16_t *bufferSize, int32_t *result);


/**@brief Decodes a frame of data using LC3.
 *
 * Processes the frame of encoded bit stream data passed by the decodeInput structure.
 * Returns the decoded audio PCM data via the decodeOutput structure.
 *
 * This function will automatically apply packet loss concealment (PLC) to any frame where
 * the decode input's badFrameIndicator is set to BadFrame or if the bit stream data contains
 * errors. The caller should set badFrameIndicator to BadFrame if the caller knows in advance
 * of bit stream errors; this allows the decoder to skip decoding  and directly apply PLC. If
 * the caller is unaware of bit stream errors, but they are present, the decoder will detect
 * them regardless. When PLC is applied successfully, this function still returns
 * @ref LC3_RESULT_NO_ERROR as when a frame is decoded successfully, but the decode output's
 * PLCCounter parameter is incremented. Upon a successful decode, PLCCounter is reset.
 * Therefore, a non-zero PLCCounter indicates PLC has been applied to the current frame.
 *
 * @param[in] decodeHandle  Handle to a decoder instance.
 * @param[in] decodeInput   Pointer to a structure pointing to the encoded bit stream frame
 *                          to decode.
 * @param[in] decodeOutput  Pointer to a structure to receive the frame's decoded PCM data.
 *
 * @returns  Zero on success or one of the defined @ref LC3ResultCodes "LC3 result codes" on error.
 */
int32_t OSALCALL LC3DecodeSessionData(LC3DecoderHandle_t decodeHandle, LC3DecodeInput_t *decodeInput, LC3DecodeOutput_t *decodeOutput);


/**@brief Closes an LC3 Decoder session.
 *
 * This function closes the decodeHandle's session and and releases allocated session memory.
 * Memory passed in the LC3DecodeSessionOpen's buffer parameter can be freed after
 * this returns.  The decodeHandle is no longer valid and should not be reused.
 *
 * @param[in] decodeHandle  Handle to the decoder instance to close.
 *
 * @returns  None.
 */
void OSALCALL LC3DecodeSessionClose(LC3DecoderHandle_t decodeHandle);


/**@brief Calculates the buffer size required for bit stream data.
 *
 * Returns the minimum buffer size in bytes needed to hold a frame of bit stream data
 * for the specified sample rate and bit rate.
 *
 * The maxBitRate parameter is the maximum bitrate value that will be passed
 * to LC3EncodeSessionData() at any point during the session's encoding.
 *
 * @param[in]  sampleRate  Sample rate.
 * @param[in]  maxBitRate  Session's maximum encoding bit rate.
 * @param[in]  frameSize   Frame size of 7.5 or 10 msec.
 * @param[out] result      Pointer to an LC3 Return Code integer.
 *
 * @returns  Bit stream buffer size on success or 0 on error.
 */
uint16_t OSALCALL LC3BitstreamBuffersize(uint16_t sampleRate, uint32_t maxBitRate, LC3FrameSize_t frameSize, int32_t *result);


/**@brief Calculates the buffer size required for PCM data.
 *
 * Returns the minimum buffer size in bytes needed to hold a frame of PCM data
 * for the specified sample rate and bit depth. A bitDepth value in the range
 * (16,24] uses a 3 byte/sample PCM output while (24,32] uses 4 bytes/sample.
 *
 * @param[in]  sampleRate  Sample rate.
 * @param[in]  bitDepth    Bit depth of generated PCM data.
 * @param[in]  frameSize   Frame size of 7.5 or 10 msec.
 * @param[out] result      Pointer to an LC3 Return Code integer.
 *
 * @returns  PCM buffer size on success or 0 on error.
 */
uint16_t OSALCALL LC3PCMBuffersize(uint16_t sampleRate, uint8_t bitDepth, LC3FrameSize_t frameSize, int32_t *result);

/*! \} */    /* LC3 */

#ifdef __cplusplus
};
#endif

#endif /* _LC3APIH_ */
