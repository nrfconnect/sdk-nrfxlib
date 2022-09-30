/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * SPDX-License-Identifier: Nordic-5-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
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
 * 5. Any software provided in binary form under this license must not be
 * reverse engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <zephyr/kernel.h>
#include "sw_codec_lc3.h"
#include "LC3API.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(sw_codec_lc3);

#define ENC_BITRATE_WRN_LVL_LOW 24000
#define ENC_BITRATE_WRN_LVL_HIGH 160000

static uint16_t enc_pcm_bytes_req;
static uint32_t m_enc_bitrate;

static LC3EncoderHandle_t enc_handle_ch[CONFIG_LC3_ENC_CHAN_MAX];
static LC3DecoderHandle_t dec_handle_ch[CONFIG_LC3_DEC_CHAN_MAX];

static uint8_t enc_num_instances;
static uint8_t dec_num_instances;

int sw_codec_lc3_enc_run(void const *const pcm_data, uint32_t pcm_data_size, uint32_t enc_bitrate,
			 uint8_t audio_ch, uint16_t lc3_data_buf_size, uint8_t *const lc3_data,
			 uint16_t *const lc3_data_wr_size)
{
	int ret;
	/* Number of PCM bytes read */
	uint16_t bytes_read = 0;

	if (pcm_data_size < enc_pcm_bytes_req) {
		LOG_ERR("Too few PCM samples to encode. Bytes required %d, input is %d",
			enc_pcm_bytes_req, pcm_data_size);
		return -EINVAL;
	}

	if (audio_ch >= enc_num_instances) {
		LOG_ERR("Trying to use audio_ch outside of allowed range");
		return -EINVAL;
	}

	LC3EncodeInput_t LC3EncodeInput = { .PCMData = (void *)pcm_data,
					    .PCMDataLength = pcm_data_size,
					    .bytesRead = bytes_read };

	if (enc_bitrate == LC3_USE_BITRATE_FROM_INIT) {
		LC3EncodeInput.encodeBitrate = m_enc_bitrate;
	} else {
		LC3EncodeInput.encodeBitrate = enc_bitrate;
	}

	LC3EncodeOutput_t LC3EncodeOutput = { .outputData = lc3_data,
					      .outputDataLength = lc3_data_buf_size,
					      .bytesWritten = 0 };

	if (!enc_handle_ch[audio_ch]) {
		LOG_ERR("LC3 enc ch:%d is not initialized", audio_ch);
		return -EPERM;
	}
	ret = LC3EncodeSessionData(enc_handle_ch[audio_ch], &LC3EncodeInput, &LC3EncodeOutput);
	if (ret) {
		return ret;
	}

	*lc3_data_wr_size = LC3EncodeOutput.bytesWritten;

	return 0;
}

int sw_codec_lc3_dec_run(uint8_t const *const lc3_data, uint16_t lc3_data_size,
			 uint16_t pcm_data_buf_size, uint8_t audio_ch, void *const pcm_data,
			 uint16_t *const pcm_data_wr_size, bool bad_frame)
{
	int ret;
	LC3BFI_t frame_status;
	uint16_t plc_counter = 0;

	if (audio_ch >= dec_num_instances) {
		LOG_ERR("Trying to use audio_ch outside of allowed range");
		return -EINVAL;
	}

	if (bad_frame) {
		frame_status = BadFrame;
	} else {
		frame_status = GoodFrame;
	}

	LC3DecodeInput_t LC3DecodeInput = { .inputData = (uint8_t *)lc3_data,
					    .inputDataLength = lc3_data_size,
					    frame_status };
	LC3DecodeOutput_t LC3DecodeOutput = { .PCMData = pcm_data,
					      .PCMDataLength = pcm_data_buf_size,
					      .bytesWritten = 0,
					      .PLCCounter = plc_counter };

	if (!dec_handle_ch[audio_ch]) {
		LOG_ERR("LC3 dec ch:%d is not initialized", audio_ch);
		return -EPERM;
	}
	ret = LC3DecodeSessionData(dec_handle_ch[audio_ch], &LC3DecodeInput, &LC3DecodeOutput);
	if (ret) {
		return ret;
	}

	/* A non-zero PLCCounter indicates PLC has been applied to the current frame.
	 * Upon a successful decode, PLCCounter is reset.
	 */
	if (LC3DecodeOutput.PLCCounter > 0 && IS_ENABLED(CONFIG_LC3_PLC_DISABLED))
	{
		/* Write a blank frame (all 0) into buffer. */
		memset(pcm_data, 0, pcm_data_buf_size);
	}

	*pcm_data_wr_size = LC3DecodeOutput.bytesWritten;

	return 0;
}

int sw_codec_lc3_enc_uninit_all(void)
{
	for (uint8_t i = 0; i < enc_num_instances; i++) {
		if (enc_handle_ch[i] != NULL) {
			LC3EncodeSessionClose(enc_handle_ch[i]);
			enc_handle_ch[i] = NULL;
		} else {
			LOG_ERR("Encoder:%d already unitialized", i);
			return -EALREADY;
		}
	}

	return 0;
}

int sw_codec_lc3_dec_uninit_all(void)
{
	for (uint8_t i = 0; i < dec_num_instances; i++) {
		if (dec_handle_ch[i] != NULL) {
			LC3DecodeSessionClose(dec_handle_ch[i]);
			dec_handle_ch[i] = NULL;
		} else {
			LOG_ERR("Decoder:%d already unitialized", i);
			return -EALREADY;
		}
	}

	return 0;
}

int sw_codec_lc3_init(uint8_t *sw_codec_lc3_buffer, uint32_t *sw_codec_lc3_buffer_size,
		      uint16_t framesize_us)
{
	int ret;
	uint8_t enc_sample_rates = 0;
	uint8_t dec_sample_rates = 0;

	/* Set unique session to 0 for using the default sharing memory setting.
	 *
	 * This could lead to higher heap consumption, but is able to manipulate
	 * different sample rate setting between encoder/decoder.
	 */
	uint8_t unique_session = 0;

	/* Check supported sample rates for encoder */
	if (IS_ENABLED(CONFIG_LC3_ENC_SAMPLE_RATE_8KHZ_SUPPORT)) {
		enc_sample_rates |= LC3_SAMPLE_RATE_8_KHZ;
	}
	if (IS_ENABLED(CONFIG_LC3_ENC_SAMPLE_RATE_16KHZ_SUPPORT)) {
		enc_sample_rates |= LC3_SAMPLE_RATE_16_KHZ;
	}
	if (IS_ENABLED(CONFIG_LC3_ENC_SAMPLE_RATE_24KHZ_SUPPORT)) {
		enc_sample_rates |= LC3_SAMPLE_RATE_24_KHZ;
	}
	if (IS_ENABLED(CONFIG_LC3_ENC_SAMPLE_RATE_32KHZ_SUPPORT)) {
		enc_sample_rates |= LC3_SAMPLE_RATE_32_KHZ;
	}
	if (IS_ENABLED(CONFIG_LC3_ENC_SAMPLE_RATE_441KHZ_SUPPORT)) {
		enc_sample_rates |= LC3_SAMPLE_RATE_441_KHZ;
	}
	if (IS_ENABLED(CONFIG_LC3_ENC_SAMPLE_RATE_48KHZ_SUPPORT)) {
		enc_sample_rates |= LC3_SAMPLE_RATE_48_KHZ;
	}

	/* Check supported sample rates for decoder */
	if (IS_ENABLED(CONFIG_LC3_DEC_SAMPLE_RATE_8KHZ_SUPPORT)) {
		dec_sample_rates |= LC3_SAMPLE_RATE_8_KHZ;
	}
	if (IS_ENABLED(CONFIG_LC3_DEC_SAMPLE_RATE_16KHZ_SUPPORT)) {
		dec_sample_rates |= LC3_SAMPLE_RATE_16_KHZ;
	}
	if (IS_ENABLED(CONFIG_LC3_DEC_SAMPLE_RATE_24KHZ_SUPPORT)) {
		dec_sample_rates |= LC3_SAMPLE_RATE_24_KHZ;
	}
	if (IS_ENABLED(CONFIG_LC3_DEC_SAMPLE_RATE_32KHZ_SUPPORT)) {
		dec_sample_rates |= LC3_SAMPLE_RATE_32_KHZ;
	}
	if (IS_ENABLED(CONFIG_LC3_DEC_SAMPLE_RATE_441KHZ_SUPPORT)) {
		dec_sample_rates |= LC3_SAMPLE_RATE_441_KHZ;
	}
	if (IS_ENABLED(CONFIG_LC3_DEC_SAMPLE_RATE_48KHZ_SUPPORT)) {
		dec_sample_rates |= LC3_SAMPLE_RATE_48_KHZ;
	}

	LC3FrameSize_t framesize;

	switch (framesize_us) {
	case 7500:
		framesize = LC3FrameSize7_5Ms;
		break;
	case 10000:
		framesize = LC3FrameSize10Ms;
		break;
	default:
		LOG_ERR("Unsupported framesize: %d", framesize_us);
		return -EINVAL;
	}

	ret = LC3Initialize(enc_sample_rates, dec_sample_rates, framesize, unique_session,
			    sw_codec_lc3_buffer, sw_codec_lc3_buffer_size);
	return ret;
}

int sw_codec_lc3_enc_init(uint16_t pcm_sample_rate, uint8_t pcm_bit_depth, uint16_t framesize_us,
			  uint32_t enc_bitrate, uint8_t num_channels, uint16_t *const pcm_bytes_req)
{
	int ret;
	LC3FrameSize_t framesize;

	switch (framesize_us) {
	case 7500:
		framesize = LC3FrameSize7_5Ms;
		break;
	case 10000:
		framesize = LC3FrameSize10Ms;
		break;
	default:
		LOG_ERR("Unsupported framesize: %d", framesize_us);
		return -EINVAL;
	}

	if (enc_bitrate == 0) {
		LOG_ERR("LC3 enc_bitrate is 0");
		return -EINVAL;
	} else if (enc_bitrate <= ENC_BITRATE_WRN_LVL_LOW) {
		LOG_WRN("LC3 enc_bitrate: %d : likely too low", enc_bitrate);
	} else if (enc_bitrate >= ENC_BITRATE_WRN_LVL_HIGH) {
		LOG_WRN("LC3 enc_bitrate: %d : likely too high", enc_bitrate);
	}

	enc_pcm_bytes_req = LC3PCMBuffersize(pcm_sample_rate, pcm_bit_depth, framesize, &ret);
	*pcm_bytes_req = enc_pcm_bytes_req;

	if (enc_pcm_bytes_req == 0) {
		LOG_ERR("Required PCM bytes to encode LC3 is zero.");
		return -EPERM;
	}

	for (uint8_t i = 0; i < num_channels; i++) {
		if (enc_handle_ch[i]) {
			LOG_ERR("LC3 enc ch: %d already initialized", i);
			return -EALREADY;
		}
		enc_handle_ch[i] = LC3EncodeSessionOpen(pcm_sample_rate, pcm_bit_depth, framesize,
							NULL, NULL, &ret);
		if (ret) {
			return ret;
		}
	}

	m_enc_bitrate = enc_bitrate;

	enc_num_instances = num_channels;

	return ret;
}

int sw_codec_lc3_dec_init(uint16_t pcm_sample_rate, uint8_t pcm_bit_depth, uint16_t framesize_us,
			  uint8_t num_channels)
{
	int ret;

	LC3FrameSize_t framesize;

	switch (framesize_us) {
	case 7500:
		framesize = LC3FrameSize7_5Ms;
		break;
	case 10000:
		framesize = LC3FrameSize10Ms;
		break;
	default:
		LOG_ERR("Unsupported framesize: %d", framesize_us);
		return -EINVAL;
	}

	for (uint8_t i = 0; i < num_channels; i++) {
		if (dec_handle_ch[i]) {
			LOG_ERR("LC3 dec ch: %d already initialized", i);
			return -EALREADY;
		}
		dec_handle_ch[i] = LC3DecodeSessionOpen(pcm_sample_rate, pcm_bit_depth, framesize,
							NULL, NULL, &ret);
		if (ret) {
			return ret;
		}
	}

	dec_num_instances = num_channels;

	return 0;
}
