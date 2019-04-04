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
 @brief This file contains definitions of the CryptoCell hash APIs.
 */

/*!
 @defgroup cc_hash_defs CryptoCell hash API definitions
 @brief Contains CryptoCell hash-API definitions.

 See cc_hash_defs.h.
 @{
 @ingroup cc_hash
 @}
*/

#ifndef CC_HASH_DEFS_H
#define CC_HASH_DEFS_H


#ifdef __cplusplus
extern "C"
{
#endif

#include "cc_pal_types.h"
#include "cc_error.h"
#include "cc_hash_defs_proj.h"

/************************ Defines ******************************/

/*! The size of the hash result in words. The maximal size for SHA-512 is 512 bits. */
#define CC_HASH_RESULT_SIZE_IN_WORDS    16

/*! The size of the MD5 digest result in Bytes. */
#define CC_HASH_MD5_DIGEST_SIZE_IN_BYTES 16

/*! The size of the MD5 digest result in words. */
#define CC_HASH_MD5_DIGEST_SIZE_IN_WORDS 4

/*! The size of the SHA-1 digest result in Bytes. */
#define CC_HASH_SHA1_DIGEST_SIZE_IN_BYTES 20

/*! The size of the SHA-1 digest result in words. */
#define CC_HASH_SHA1_DIGEST_SIZE_IN_WORDS 5

/*! The size of the SHA-224 digest result in words. */
#define CC_HASH_SHA224_DIGEST_SIZE_IN_WORDS 7

/*! The size of the SHA-256 digest result in words. */
#define CC_HASH_SHA256_DIGEST_SIZE_IN_WORDS 8

/*! The size of the SHA-384 digest result in words. */
#define CC_HASH_SHA384_DIGEST_SIZE_IN_WORDS 12

/*! The size of the SHA-512 digest result in words. */
#define CC_HASH_SHA512_DIGEST_SIZE_IN_WORDS 16

/*! The size of the SHA-256 digest result in Bytes. */
#define CC_HASH_SHA224_DIGEST_SIZE_IN_BYTES 28

/*! The size of the SHA-256 digest result in Bytes. */
#define CC_HASH_SHA256_DIGEST_SIZE_IN_BYTES 32

/*! The size of the SHA-384 digest result in Bytes. */
#define CC_HASH_SHA384_DIGEST_SIZE_IN_BYTES 48

/*! The size of the SHA-512 digest result in Bytes. */
#define CC_HASH_SHA512_DIGEST_SIZE_IN_BYTES 64

/*! The size of the SHA-1 hash block in words. */
#define CC_HASH_BLOCK_SIZE_IN_WORDS 16

/*! The size of the SHA-1 hash block in Bytes. */
#define CC_HASH_BLOCK_SIZE_IN_BYTES 64

/*! The size of the SHA-2 hash block in words. */
#define CC_HASH_SHA512_BLOCK_SIZE_IN_WORDS  32

/*! The size of the SHA-2 hash block in Bytes. */
#define CC_HASH_SHA512_BLOCK_SIZE_IN_BYTES  128

/*! The maximal data size for the update operation. */
#define CC_HASH_UPDATE_DATA_MAX_SIZE_IN_BYTES (1 << 29)


/************************ Enums ********************************/

/*! The hash operation mode. */
typedef enum {
    CC_HASH_SHA1_mode          = 0,    /*!< SHA-1. */
    CC_HASH_SHA224_mode        = 1,    /*!< SHA-224. */
    CC_HASH_SHA256_mode        = 2,    /*!< SHA-256. */
    CC_HASH_SHA384_mode        = 3,    /*!< SHA-384. */
    CC_HASH_SHA512_mode        = 4,    /*!< SHA-512. */
    CC_HASH_MD5_mode           = 5,    /*!< MD5. */
    /*! The number of hash modes. */
    CC_HASH_NumOfModes,
    /*! Reserved. */
    CC_HASH_OperationModeLast= 0x7FFFFFFF,

}CCHashOperationMode_t;

/************************ Typedefs  *****************************/

/*! The hash result buffer. */
typedef uint32_t CCHashResultBuf_t[CC_HASH_RESULT_SIZE_IN_WORDS];

/************************ Structs  ******************************/
/*!
 The context prototype of the user.
 The argument type that is passed by the user to the hash APIs.
 The context saves the state of the operation, and must be saved by the user
 until the end of the API flow.
*/
typedef struct CCHashUserContext_t {
    uint32_t buff[CC_HASH_USER_CTX_SIZE_IN_WORDS] /*!< The internal buffer. */;
}CCHashUserContext_t;


#ifdef __cplusplus
}
#endif

#endif /* #ifndef CC_HASH_DEFS_H */
