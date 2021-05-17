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
  @defgroup nrf_errno_api Global Error Codes
  @{

  @brief Global Error definitions
*/

#ifndef MPSL_ERROR_H__
#define MPSL_ERROR_H__

#ifdef __cplusplus
extern "C" {
#endif

#define NRF_EPERM            1  ///< Not permitted, not possible / safe
#define NRF_ENOENT           2  ///< No such entry (file or directory)
#define NRF_EIO              5  ///< Input / output error
#define NRF_ENOMEM          12  ///< Not enough memory
#define NRF_EACCES          13  ///< Access error, not accessible using current privileges
#define NRF_EFAULT          14  ///< Bad address
#define NRF_EINVAL          22  ///< Invalid argument
#define NRF_EAGAIN          35  ///< Resource temporary unavailable, try again later
#define NRF_EPROTOTYPE      41  ///< Protocol type wrong for socket
#define NRF_ENOPROTOOPT     42  ///< Protocol not available
#define NRF_EPROTONOSUPPORT 43  ///< Protocol not supported
#define NRF_ESOCKTNOSUPPORT 44  ///< Socket type not supported
#define NRF_EOPNOTSUPP      45  ///< Operation not supported on socket
#define NRF_EAFNOSUPPORT    47  ///< Address family not supported by protocol family
#define NRF_EADDRINUSE      48  ///< Address already in use
#define NRF_ENETDOWN        50  ///< Network is down
#define NRF_ENETUNREACH     51  ///< Network is unreachable
#define NRF_ECONNRESET      54  ///< Connection reset by peer
#define NRF_EISCONN         56  ///< Connection is already connected
#define NRF_ENOTCONN        57  ///< Connection is not connected
#define NRF_ETIMEDOUT       60  ///< Connection timed out
#define NRF_ENOBUFS         105 ///< No buffer space available

#define NRF_EHOSTDOWN       112 ///< Host is down
#define NRF_EINPROGRESS     115 ///< Operation in progress
#define NRF_ECANCELED       125 ///< Operation canceled

#define NRF_ENOKEY          126 ///< Required key not available
#define NRF_EKEYEXPIRED     127 ///< Key has expired
#define NRF_EKEYREVOKED     128 ///< Key has been revoked
#define NRF_EKEYREJECTED    129 ///< Key was rejected by service

#ifdef __cplusplus
}
#endif

#endif // MPSL_ERROR_H__

/**
  @}
*/
