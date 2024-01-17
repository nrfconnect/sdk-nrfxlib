/**
 * Copyright (c) 2015, Telit Communications Cyprus Ltd
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef NFC_T2T_LIB_H__
#define NFC_T2T_LIB_H__

/** @file nfc_t2t_lib.h
 *
 * @defgroup nfc_t2t NFC Type 2 Tag
 * @brief Implementation of NFC Type 2 Tag.
 *
 * @defgroup nfc_t2t_lib NFC tag 2 type emulation library
 * @{
 * @ingroup nfc_t2t
 * @brief The T2T emulation library interface.
 */

#include <string.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** T2T internal byte size.*/
#define NFC_T2T_SIZEOF_INTERNAL_BYTES 10
/** Maximum NDEF message size.*/
#define NFC_T2T_MAX_PAYLOAD_SIZE      988
/**< No NDEF-TLV and no implicit lock bytes at the end. */
#define NFC_T2T_MAX_PAYLOAD_SIZE_RAW  1008

/** @brief Events passed to the callback function. */
typedef enum
{
	/** Not used.*/
	NFC_T2T_EVENT_NONE,

	/** NFC tag has detected external NFC field and was selected by an NFC
	 *  polling device.
	 */
	NFC_T2T_EVENT_FIELD_ON,

	/** External NFC field has been removed.*/
	NFC_T2T_EVENT_FIELD_OFF,

	/** NFC polling device has read all tag data.*/
	/**
	 * Repeated reading in the same session i.e. before
	 * @ref NFC_T2T_EVENT_FIELD_OFF event, will not trigger another
	 * @ref NFC_T2T_EVENT_DATA_READ event.
	 */
	NFC_T2T_EVENT_DATA_READ,

	/** Reference to the application NFC callback has been released using
	 *  @ref nfc_t2t_done.
	 */
	NFC_T2T_EVENT_STOPPED
} nfc_t2t_event_t;

typedef enum
{
	/** Frame Delay Time Min parameter.
	 *  The parameter controls the frame transmission timing during collision resolution.
	 */
	NFC_T2T_PARAM_FDT_MIN,

	/** NFCID1 value, data can be 4, 7, or 10 bytes long (single, double,
	 * or triple size). To use default NFCID1 of specific length pass one
	 * byte containing requested length. Default 7-byte NFCID1 will be
	 * used if this parameter was not set. This parameter can be set
	 * before nfc_t2t_setup() to set initial NFCID1 and it can be changed
	 * later.
	 */
	NFC_T2T_PARAM_NFCID1,
} nfc_t2t_param_id_t;

/**@brief Callback to pass events from NFC T2T Library to application.
 *
 * @param context Application context for callback execution.
 * @param event The event that occurred.
 * @param data Data to send to the application (event specific).
 * @param data_length Length of the data.
 */
typedef void (*nfc_t2t_callback_t)(void *context,
				   nfc_t2t_event_t event,
				   const uint8_t *data,
				   size_t data_length);

/**@brief Register the application callback for event signaling.
 *
 * The callback will be called by NFC T2T Library to notify the application of
 * relevant events. It will be called from the HAL_NFC callback context.
 *
 * @param callback Function pointer to the callback.
 * @param context Pointer to a memory area used by the callback for
 * execution (optional).
 *
 * @retval 0 Success.
 * @retval -NRF_EINVAL Invalid argument (e.g. wrong data length, NULL pointer))
 * @retval -NRF_EOPNOTSUPP If emulation is in running state.
 */
int nfc_t2t_setup(nfc_t2t_callback_t callback, void *context);

/**@brief Set up an NFC parameter.
 *
 * This function allows to set an NFC configuration parameter.
 *
 * @param id ID of the parameter to set.
 * @param data Pointer to a buffer containing the data to set.
 * @param data_length Size of the buffer containing the data to set.
 *
 * @retval 0 Success.
 * @retval -NRF_EINVAL Invalid argument (e.g. wrong data length, NULL pointer).
 */
int nfc_t2t_parameter_set(nfc_t2t_param_id_t id,
			  void *data,
			  size_t data_length);

/**@brief Query an NFC parameter value.
 *
 * The queried value will be placed into the passed data buffer. If the buffer
 * is too small, max_data_length will contain the required buffer size. If the
 * buffer is big enough, max_data_length will contain the actual size of the
 * data.
 *
 * @param id ID of the parameter to query.
 * @param data Pointer to a buffer receiving the queried data.
 * @param max_data_length Size of the buffer, receives actual size of
 *	  queried data.
 *
 * @retval 0 Success.
 * @retval -NRF_EINVAL Invalid argument (e.g. wrong data length, NULL pointer).
 */
int nfc_t2t_parameter_get(nfc_t2t_param_id_t id,
			  void *data,
			  size_t *max_data_length);

/** @brief Register the payload to send on reception of a READ request.
 *
 * The payload is considered to only contain the NDEF message to deliver to a
 * reader. The required NDEF TLV will be created implicitly by NFC T2T Library.
 *
 * The pointer to the payload must stay valid for the duration of the library
 * execution, or until it is explicitly released.
 *
 * If the pointer is not NULL, but the length is zero, the payload is
 * considered to be an empty NDEF message.
 *
 * If a new payload is registered, the previously registered one is considered
 * released.
 *
 * Passing a NULL pointer releases the current payload without registering a
 * new one.
 *
 * If an invalid size is given (too big), the function returns with an error
 * and the currently registered payload is left unchanged.
 *
 * @note Provided pointer must point to RAM region.
 *
 * @param payload Pointer to the memory area in RAM containing the
 * payload to send.
 * @param payload_length Size of the payload in bytes.
 *
 * @retval 0 Success.
 * @retval -NRF_EINVAL Invalid @p payload_length.
 * @retval -NRF_EOPNOTSUPP Payload has been already set.
 */
int nfc_t2t_payload_set(const uint8_t *payload, size_t payload_length);

/** @brief Register the raw payload to send on reception of a READ request.
 *
 * The payload will be delivered directly as-is to the reader, without
 * implicitly adding an NDEF TLV container. This can be used if the
 * application wants to define the TLVs itself, for example, to provide a
 * different memory layout.
 *
 * The pointer to the payload must stay valid for the duration of the library
 * execution, or until it is explicitly released.
 *
 * If a new payload is registered, the previously registered one is considered
 * released.
 *
 * Passing a NULL pointer releases the current payload, without registering a
 * new one.
 *
 * If an invalid size is given (too big), the function returns with an error
 * and the currently registered payload is left unchanged.
 *
 * @note Provided pointer must points to RAM region.
 *
 * @param payload Pointer to the memory area in RAM containing the
 * payload to send.
 * @param payload_length Size of the payload in bytes.
 *
 * @retval 0 Success.
 * @retval -NRF_EINVAL Invalid @p payload_length.
 * @retval -NRF_EOPNOTSUPP Payload has been already set.
 */
int nfc_t2t_payload_raw_set(const uint8_t *payload,
			    size_t payload_length);

/** @brief Register the sequence of internal bytes.
 *
 * This refers to the first 10 bytes of the tag memory. The library will set
 * a sensible default for these bytes. The application can use this function
 * to override the default.
 *
 * Passing a NULL pointer reverts back to the default sequence.
 * The data will be copied by NFC T2T Library, so the memory does not have to
 * remain valid after the function returns.
 *
 * @note When modifying the internal bytes, remember that they must be
 *	 consistent with the NFC hardware register settings.
 *
 * @param data Pointer to the memory area containing the data.
 * @param data_length Size of the data in bytes.
 *
 * @retval 0 Success.
 * @retval -NRF_EINVAL Invalid argument (e.g. wrong data length, NULL pointer).
 */
int nfc_t2t_internal_set(const uint8_t *data, size_t data_length);

/** @brief Activate the NFC frontend.
 *
 * You must call this function so that events are posted to the application
 * callback.
 *
 * @retval 0 Success.
 * @retval -NRF_EOPNOTSUPP Already started.
 */
int nfc_t2t_emulation_start(void);

/** @brief Deactivate the NFC frontend.
 *
 * After calling this function, no more events will be posted to the
 * application callback.
 *
 * @retval 0 Success.
 * @retval -NRF_EOPNOTSUPP Emulation has been already stopped.
 */
int nfc_t2t_emulation_stop(void);

/** @brief Release the reference to the application callback.
 *
 * After calling this function, the passed callback pointer is no longer
 * considered valid.
 *
 * @retval 0 Success.
 * @retval -NRF_EOPNOTSUPP The NFC T4T has been de-initialized already.
 */
int nfc_t2t_done(void);

#ifdef __cplusplus
}
#endif

/** @} */
#endif /* NFC_T2T_LIB_H__ */
