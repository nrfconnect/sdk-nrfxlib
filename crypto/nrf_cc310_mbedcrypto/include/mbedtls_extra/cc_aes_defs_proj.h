/**************************************************************************************
* Copyright (c) 2016-2018, Arm Limited (or its affiliates). All rights reserved       *
*                                                                                     *
* This file and the related binary are licensed under the following license:          *
*                                                                                     *
* ARM Object Code and Header Files License, v1.0 Redistribution.                      *
*                                                                                     *
* Redistribution and use of object code, header files, and documentation, without     *
* modification, are permitted provided that the following conditions are met:         *
*                                                                                     *
* 1) Redistributions must reproduce the above copyright notice and the                *
*    following disclaimer in the documentation and/or other materials                 *
*    provided with the distribution.                                                  *
*                                                                                     *
* 2) Unless to the extent explicitly permitted by law, no reverse                     *
*    engineering, decompilation, or disassembly of is permitted.                      *
*                                                                                     *
* 3) Redistribution and use is permitted solely for the purpose of                    *
*    developing or executing applications that are targeted for use                   *
*    on an ARM-based product.                                                         *
*                                                                                     *
* DISCLAIMER. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND                  *
* CONTRIBUTORS "AS IS." ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT             *
* NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT,        *
* AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE          *
* COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,   *
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED            *
* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR              *
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF              *
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING                *
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS                  *
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                        *
**************************************************************************************/

/*!
 @file
 @brief This file contains definitions that are used for CryptoCell AES APIs.
 */

/*!
 @addtogroup cc_aes_defs CryptoCell AES APIs definitions
 @brief Contains definitions that are used for CryptoCell AES APIs.

 See cc_aes_def_proj.h.

 @{
 @ingroup cc_aes
 @}
 */

#ifndef CC_AES_DEFS_PROJ_H
#define CC_AES_DEFS_PROJ_H

#include "cc_pal_types.h"


#ifdef __cplusplus
extern "C"
{
#endif

/************************ Defines ******************************/

/*! The size of the context prototype of the user in words. See ::CCAesUserContext_t.*/
#define CC_AES_USER_CTX_SIZE_IN_WORDS (4+8+8+4)

/*! The maximal size of the AES key in words. */
#define CC_AES_KEY_MAX_SIZE_IN_WORDS 8
/*! The maximal size of the AES key in bytes. */
#define CC_AES_KEY_MAX_SIZE_IN_BYTES (CC_AES_KEY_MAX_SIZE_IN_WORDS * sizeof(uint32_t))


#ifdef __cplusplus
}
#endif

#endif /* #ifndef CC_AES_DEFS_PROJ_H */

