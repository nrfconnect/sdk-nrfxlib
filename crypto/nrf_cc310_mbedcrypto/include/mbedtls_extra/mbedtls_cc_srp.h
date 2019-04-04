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
 @brief This file contains all of the CryptoCell SRP APIs, their enums and definitions.
 */

 /*!
 @defgroup cc_srp CryptoCell SRP APIs
 @brief Contains CryptoCell SRP APIs. See mbedtls_cc_srp.h.

 @{
 @ingroup cryptocell_api
 @}
 */

#ifndef _MBEDTLS_CC_SRP_H
#define _MBEDTLS_CC_SRP_H

#include "cc_pal_types.h"
#include "cc_error.h"
#include "cc_pka_defs_hw.h"
#include "cc_hash_defs.h"
#include "cc_rnd_common.h"


#ifdef __cplusplus
extern "C"
{
#endif

/*!\internal The following describes the SRP APIs usage for the Device and the Accessory :*

        Device (User)                       Accessory (Host)
*      --------------                       -----------------

  1.    CC_SRP_HK_INIT(CC_SRP_USER, .......)        CC_SRP_HK_INIT(CC_SRP_HOST, .....)

  2.                                CC_SrpPwdVerCreate(..)

  3.    CC_SrpUserPubKeyCreate(..)              CC_SrpHostPubKeyCreate(..)

  4.    CC_SrpUserProofCalc(..)

  5.                                CC_SrpHostProofVerifyAndCalc(..)

  6.    CC_SrpUserProofVerify(..)

  7.    CC_SrpClear(..)                 CC_SrpClear(..)

 */

/************************ Defines ******************************/
/* The SRP modulus sizes. */
/*! SRP modulus size of 1024 bits. */
#define CC_SRP_MODULUS_SIZE_1024_BITS   1024
/*! SRP modulus size of 1536 bits. */
#define CC_SRP_MODULUS_SIZE_1536_BITS   1536
/*! SRP modulus size of 2048 bits. */
#define CC_SRP_MODULUS_SIZE_2048_BITS   2048
/*! SRP modulus size of 3072 bits. */
#define CC_SRP_MODULUS_SIZE_3072_BITS   3072

/*! The maximal size of the SRP modulus in bits. */
#define CC_SRP_MAX_MODULUS_IN_BITS      CC_SRP_MODULUS_SIZE_3072_BITS
/*! The maximal size of the SRP modulus in Bytes. */
#define CC_SRP_MAX_MODULUS          (CC_SRP_MAX_MODULUS_IN_BITS/CC_BITS_IN_BYTE)
/*! The maximal size of the SRP modulus in words. */
#define CC_SRP_MAX_MODULUS_IN_WORDS         (CC_SRP_MAX_MODULUS_IN_BITS/CC_BITS_IN_32BIT_WORD)

/* SRP private number size range. */
/*! The minimal size of the SRP private number in bits. */
#define CC_SRP_PRIV_NUM_MIN_SIZE_IN_BITS        (256)
/*! The minimal size of the SRP private number in Bytes. */
#define CC_SRP_PRIV_NUM_MIN_SIZE            (CC_SRP_PRIV_NUM_MIN_SIZE_IN_BITS/CC_BITS_IN_BYTE)
/*! The minimal size of the SRP private number in words. */
#define CC_SRP_PRIV_NUM_MIN_SIZE_IN_WORDS       (CC_SRP_PRIV_NUM_MIN_SIZE_IN_BITS/CC_BITS_IN_32BIT_WORD)
/*! The maximal size of the SRP private number in bits. */
#define CC_SRP_PRIV_NUM_MAX_SIZE_IN_BITS        (CC_SRP_MAX_MODULUS_IN_BITS)
/*! The maximal size of the SRP private number in Bytes. */
#define CC_SRP_PRIV_NUM_MAX_SIZE            (CC_SRP_PRIV_NUM_MAX_SIZE_IN_BITS/CC_BITS_IN_BYTE)
/*! The maximal size of the SRP private number in words. */
#define CC_SRP_PRIV_NUM_MAX_SIZE_IN_WORDS       (CC_SRP_PRIV_NUM_MAX_SIZE_IN_BITS/CC_BITS_IN_32BIT_WORD)

/*! The maximal size of the SRP hash digest in words. */
#define CC_SRP_MAX_DIGEST_IN_WORDS      CC_HASH_RESULT_SIZE_IN_WORDS
/*! The maximal size of the SRP hash digest in Bytes. */
#define CC_SRP_MAX_DIGEST           (CC_SRP_MAX_DIGEST_IN_WORDS*CC_32BIT_WORD_SIZE)

/*! The minimal size of the salt in Bytes. */
#define CC_SRP_MIN_SALT_SIZE            (8)
/*! The minimal size of the salt in words. */
#define CC_SRP_MIN_SALT_SIZE_IN_WORDS       (CC_SRP_MIN_SALT_SIZE/CC_32BIT_WORD_SIZE)
/*! The maximal size of the salt in Bytes. */
#define CC_SRP_MAX_SALT_SIZE            (64)
/*! The maximal size of the salt in words. */
#define CC_SRP_MAX_SALT_SIZE_IN_WORDS       (CC_SRP_MAX_SALT_SIZE/CC_32BIT_WORD_SIZE)

/************************ Typedefs  ****************************/
/*! The definition of the SRP modulus buffer. */
typedef uint8_t mbedtls_srp_modulus[CC_SRP_MAX_MODULUS];

/*! The definition of the SRP digest buffer. */
typedef uint8_t mbedtls_srp_digest[CC_SRP_MAX_DIGEST];

/*! The definition of the SRP session key. */
typedef uint8_t mbedtls_srp_sessionKey[2*CC_SRP_MAX_DIGEST];

/************************ Enums ********************************/

/*! Supported SRP versions. */
typedef enum {
    CC_SRP_VER_3    = 0,   /*!< SRP version 3. */
    CC_SRP_VER_6   = 1,    /*!< SRP version 6. */
    CC_SRP_VER_6A  = 2,    /*!< SRP version 6A. */
    CC_SRP_VER_HK  = 3,    /*!< SRP version HK. */

    CC_SRP_NumOfVersions, /*!< The maximal number of supported versions. */
    CC_SRP_VersionLast= 0x7FFFFFFF, /*!< Reserved.*/
}mbedtls_srp_version_t;

/*! SRP entity types. */
typedef enum {
    CC_SRP_HOST = 1,    /*!< The host entity, also known as server, verifier, or accessory. */
    CC_SRP_USER   = 2,    /*!< The user entity, also known as client, or device. */

    CC_SRP_NumOfEntityType, /*!< The maximal number of entities types. */
    CC_SRP_EntityLast= 0x7FFFFFFF, /*!< Reserved. */
}mbedtls_srp_entity_t;

/************************ Structs  ******************************/

/*!
 @brief Group parameters for the SRP.

 Defines the modulus and the generator used.
 */
typedef struct mbedtls_srp_group_param {
    mbedtls_srp_modulus modulus; /*!< The SRP modulus. */
    uint8_t         gen; /*!< The SRP generator. */
    size_t          modSizeInBits; /*!< The size of the SRP modulus in bits. */
    uint32_t        validNp; /*!< The valid SRP Np. */
    uint32_t        Np[CC_PKA_BARRETT_MOD_TAG_BUFF_SIZE_IN_WORDS]; /*!< The SRP Np buffer. */
}mbedtls_srp_group_param;

/************************ context Structs  ******************************/
/*! The SRP context prototype */
typedef struct mbedtls_srp_context {
    mbedtls_srp_entity_t            srpType; /*!< The SRP entitiy type. */
    mbedtls_srp_version_t   srpVer; /*!< The SRP version. */
    mbedtls_srp_group_param     groupParam;  /*!< The group parameter including the modulus information. */// N, g, Np
    CCHashOperationMode_t   hashMode; /*!< The hash mode. */
    size_t          hashDigestSize; /*!< The hash digest size. */
    size_t          sessionKeySize; /*!< The session key size. */
    CCRndContext_t      *pRndCtx; /*!< A pointer to the RND context. */
    mbedtls_srp_modulus     ephemPriv;  /*!< The modulus. */ // a or b
    size_t          ephemPrivSize; /*!< The modulus size. */
    mbedtls_srp_digest      userNameDigest;  /*!< The user-name digest. */// M
    mbedtls_srp_digest      credDigest;  /*!< The cred digest. */ // p
    mbedtls_srp_digest      kMult; /*!< The SRP K multiplier. */      // k multiplier
}mbedtls_srp_context;


/************************ SRP common Functions **********************/
/****************************************************************************************************/
/*!
 @brief This function initiates the SRP context.

 @return \c CC_OK on success.
 @return A non-zero value on failure as defined in mbedtls_cc_srp_error.h or cc_hash_error.h.
 */
CIMPORT_C CCError_t  mbedtls_srp_init(
                mbedtls_srp_entity_t    srpType,    /*!< [in] The SRP entity type. */
                mbedtls_srp_version_t   srpVer,     /*!< [in] The SRP version. */
                mbedtls_srp_modulus srpModulus, /*!< [in] A pointer to the SRP modulus, BE Byte buffer. */
                uint8_t         srpGen,     /*!< [in] The SRP generator param. */
                size_t          modSizeInBits,  /*!< [in] The size of the SRP modulus in bits. Valid values are: 1024bits, 1536bits, 2048bits, or 3072bits. */
                CCHashOperationMode_t   hashMode,       /*!< [in] The hash mode. */
                uint8_t         *pUserName, /*!< [in] A pointer to the username. */
                size_t                  userNameSize,   /*!< [in] The size of the username buffer. Must be larger than 0. */
                uint8_t         *pPwd,      /*!< [in] A pointer to the user password. */
                size_t                  pwdSize,    /*!< [in] The size of the user-password buffer. Must be larger than 0 if \p pPwd is valid. */
                CCRndContext_t  *pRndCtx,   /*!< [in] A pointer to the RND context. */
                mbedtls_srp_context *pCtx       /*!< [out] A pointer to the SRP host context. */
);

/*! Macro definition for a specific SRP-initialization function. */
#define CC_SRP_HK_INIT(srpType, srpModulus, srpGen, modSizeInBits, pUserName, userNameSize, pPwd, pwdSize, pRndCtx, pCtx) \
        mbedtls_srp_init(srpType, CC_SRP_VER_HK, srpModulus, srpGen, modSizeInBits, CC_HASH_SHA512_mode, pUserName, userNameSize, pPwd, pwdSize, pRndCtx, pCtx)


/****************************************************************************************************/
/*!
 @brief This function calculates \p pSalt and \p pwdVerifier.

 @return \c CC_OK on success.
 @return A non-zero value on failure, as defined in mbedtls_cc_srp_error.h, cc_rnd_error.h or cc_hash_error.h.
 */
CIMPORT_C CCError_t  mbedtls_srp_pwd_ver_create(
                size_t                  saltSize,   /*!< [in] The size of the random salt to generate.
                The range is between #CC_SRP_MIN_SALT_SIZE and #CC_SRP_MAX_SALT_SIZE. */
                uint8_t         *pSalt,     /*!< [out] A pointer to the \p pSalt number (s). */
                mbedtls_srp_modulus         pwdVerifier,    /*!< [out] A pointer to the password verifier (v). */
                mbedtls_srp_context *pCtx       /*!< [out] A pointer to the SRP context. */
);


/****************************************************************************************************/
/*!
 @brief This function clears the SRP context.

 @return \c CC_OK on success.
 @return A non-zero value on failure, as defined in mbedtls_cc_srp_error.h.
 */
CIMPORT_C CCError_t  mbedtls_srp_clear(
        mbedtls_srp_context *pCtx       /*!< [in/out] A pointer to the SRP context. */
);


/************************ SRP Host Functions **********************/
/****************************************************************************************************/
/*!
 @brief This function generates the public and private host ephemeral keys, known as B and b in
 <em>RFC 5054 Using the Secure Remote Password (SRP) Protocol for TLS Authentication</em>.

 @return \c CC_OK on success.
 @return A non-zero value on failure, as defined in mbedtls_cc_srp_error.h or cc_rnd_error.h.
 */
CIMPORT_C CCError_t  mbedtls_srp_host_pub_key_create(
                size_t                  ephemPrivSize,   /*!< [in] The size of the generated ephemeral private key (b).
                                                                   The range is between #CC_SRP_PRIV_NUM_MIN_SIZE and
                                                                   #CC_SRP_PRIV_NUM_MAX_SIZE */
                mbedtls_srp_modulus     pwdVerifier,     /*!< [in] A pointer to the verifier (v). */
                mbedtls_srp_modulus     hostPubKeyB,     /*!< [out] A pointer to the host ephemeral public key (B). */
                mbedtls_srp_context     *pCtx            /*!< [in/out] A pointer to the SRP context. */
);


/*!
 @brief This function verifies the user proof, and calculates the host-message proof.

 @return \c CC_OK on success.
 @return A non-zero value on failure, as defined in mbedtls_cc_srp_error.h or cc_hash_error.h.
 */
CIMPORT_C CCError_t  mbedtls_srp_host_proof_verify_and_calc(
                size_t                  saltSize,     /*!< [in] The size of the random salt.
                                                                The range is between #CC_SRP_MIN_SALT_SIZE
                                                                and #CC_SRP_MAX_SALT_SIZE. */
                uint8_t                 *pSalt,       /*!< [in] A pointer to the pSalt number. */
                mbedtls_srp_modulus     pwdVerifier,  /*!< [in] A pointer to the password verifier (v). */
                mbedtls_srp_modulus     userPubKeyA,  /*!< [in] A pointer to the ephemeral public key of the user (A). */
                mbedtls_srp_modulus     hostPubKeyB,  /*!< [in] A pointer to the ephemeral public key of the host (B). */
                mbedtls_srp_digest      userProof,    /*!< [in] A pointer to the SRP user-proof buffer (M1). */
                mbedtls_srp_digest      hostProof,    /*!< [out] A pointer to the SRP host-proof buffer (M2). */
                mbedtls_srp_sessionKey   sessionKey,  /*!< [out] A pointer to the SRP session key (K). */
                mbedtls_srp_context     *pCtx         /*!< [in] A pointer to the SRP context. */
);



/************************ SRP User Functions **********************/
/****************************************************************************************************/
/*!
 @brief This function generates public and private user ephemeral keys, known as A and a in
 <em>RFC 5054 Using the Secure Remote Password (SRP) Protocol for TLS Authentication</em>.

 @return \c CC_OK on success.
 @return A non-zero value on failure, as defined in mbedtls_cc_srp_error.h or cc_rnd_error.h.
 */
CIMPORT_C CCError_t  mbedtls_srp_user_pub_key_create(
                size_t                  ephemPrivSize,  /*!< [in] The size of the generated ephemeral
                                                                  private key (a).
                                                                  The range is between #CC_SRP_PRIV_NUM_MIN_SIZE
                                                                  and #CC_SRP_PRIV_NUM_MAX_SIZE.
                                                                  The size must be 32 bit aligned */
                mbedtls_srp_modulus     userPubKeyA,    /*!< [out] A pointer to the user ephemeral public key (A). */
                mbedtls_srp_context     *pCtx           /*!< [in/out] A pointer to the SRP context. */
);


/****************************************************************************************************/
/*!
 @brief This function calculates the user proof.

 @return \c CC_OK on success.
 @return A non-zero value on failure, as defined in mbedtls_cc_srp_error.h or cc_hash_error.h.
 */
CIMPORT_C CCError_t  mbedtls_srp_user_proof_calc(
                size_t                  saltSize,     /*!< [in] The size of the random salt.
                                                                The range is between #CC_SRP_MIN_SALT_SIZE
                                                                and #CC_SRP_MAX_SALT_SIZE. */
                uint8_t                 *pSalt,       /*!< [in] A pointer to the pSalt number. */
                mbedtls_srp_modulus     userPubKeyA,  /*!< [in] A pointer to the public ephmeral key of the user (A). */
                mbedtls_srp_modulus     hostPubKeyB,  /*!< [in] A pointer to the public ephmeral key of the host (B). */
                mbedtls_srp_digest      userProof,    /*!< [out] A pointer to the SRP user proof buffer (M1). */
                mbedtls_srp_sessionKey      sessionKey, /*!< [out] A pointer to the SRP session key (K). */
                mbedtls_srp_context     *pCtx         /*!< [out] A pointer to the SRP context. */
);

/****************************************************************************************************/
/*!
 @brief This function verifies the host proof.

 @return \c CC_OK on success.
 @return A non-zero value on failure, as defined in mbedtls_cc_srp_error.h or cc_hash_error.h.
 */
CIMPORT_C CCError_t  mbedtls_srp_user_proof_verify(
                mbedtls_srp_sessionKey    sessionKey,  /*!< [in] A pointer to the SRP session key (K). */
                mbedtls_srp_modulus   userPubKeyA,   /*!< [in] A pointer to the public ephmeral key of the user (A). */
                mbedtls_srp_digest    userProof,     /*!< [in] A pointer to the SRP user proof buffer (M1). */
                mbedtls_srp_digest    hostProof,     /*!< [in] A pointer to the SRP host proof buffer (M2). */
                mbedtls_srp_context   *pCtx          /*!< [out] A pointer to the SRP user context. */
);


#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MBEDTLS_CC_SRP_H */
