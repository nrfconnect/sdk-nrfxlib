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
 @brief This file contains all the type definitions that are used for the CryptoCell ECPKI APIs.
 */

/*!
 @defgroup cc_ecpki_types CryptoCell ECPKI type definitions
 @brief Contains CryptoCell ECPKI API type definitions.

 See cc_ecpki_types.h.

 @{
 @ingroup cc_ecpki
 @}
 */

#ifndef _CC_ECPKI_TYPES_H
#define _CC_ECPKI_TYPES_H


#include "cc_bitops.h"
#include "cc_pal_types_plat.h"
#include "cc_hash_defs.h"
#include "cc_pka_defs_hw.h"
#include "cc_pal_compiler.h"
#ifdef USE_MBEDTLS_CRYPTOCELL
#include "mbedtls/md.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif


/************************ Defines ******************************/
/*! The size of the internal buffer in words. */
#define CC_PKA_DOMAIN_LLF_BUFF_SIZE_IN_WORDS (10 + 3*CC_ECPKI_MODUL_MAX_LENGTH_IN_WORDS)

/**************************************************************************************
 *                Enumerators
 ***************************************************************************************/

/*------------------------------------------------------------------*/
/*! @brief EC domain idetifiers.

   For more information, see <em>Standards for Efficient Cryptography Group (SECG):
   SEC2 Recommended Elliptic Curve Domain Parameters, Version 1.0</em>.
*/
typedef enum
{
    /* For prime field */
    CC_ECPKI_DomainID_secp192k1,   /*!< EC secp192k1. */
    CC_ECPKI_DomainID_secp192r1,   /*!< EC secp192r1. */
    CC_ECPKI_DomainID_secp224k1,   /*!< EC secp224k1. */
    CC_ECPKI_DomainID_secp224r1,   /*!< EC secp224r1. */
    CC_ECPKI_DomainID_secp256k1,   /*!< EC secp256k1. */
    CC_ECPKI_DomainID_secp256r1,   /*!< EC secp256r1. */
    CC_ECPKI_DomainID_secp384r1,   /*!< EC secp384r1. */
    CC_ECPKI_DomainID_secp521r1,   /*!< EC secp521r1. */

    CC_ECPKI_DomainID_Builded,     /*!< User given, not identified. */
    CC_ECPKI_DomainID_OffMode,     /*!< Reserved.*/

    CC_ECPKI_DomainIDLast      = 0x7FFFFFFF, /*!< Reserved.*/

}CCEcpkiDomainID_t;


/*------------------------------------------------------------------*/
/*!
  @brief Hash operation mode.

  Defines hash modes according to <em>IEEE 1363-2000: IEEE Standard for
  Standard Specifications for Public-Key Cryptography</em>.
 */
typedef enum
{
    CC_ECPKI_HASH_SHA1_mode    = 0,     /*!< The message data will be hashed with SHA-1. */
    CC_ECPKI_HASH_SHA224_mode  = 1,     /*!< The message data will be hashed with SHA-224. */
    CC_ECPKI_HASH_SHA256_mode  = 2,     /*!< The message data will be hashed with SHA-256. */
    CC_ECPKI_HASH_SHA384_mode  = 3,     /*!< The message data will be hashed with SHA-384. */
    CC_ECPKI_HASH_SHA512_mode  = 4,     /*!< The message data will be hashed with SHA-512. */

    CC_ECPKI_AFTER_HASH_SHA1_mode    = 5,   /*!< The message data is a digest of SHA-1 and will not be hashed. */
    CC_ECPKI_AFTER_HASH_SHA224_mode  = 6,   /*!< The message data is a digest of SHA-224 and will not be hashed. */
    CC_ECPKI_AFTER_HASH_SHA256_mode  = 7,   /*!< The message data is a digest of SHA-256 and will not be hashed. */
    CC_ECPKI_AFTER_HASH_SHA384_mode  = 8,   /*!< The message data is a digest of SHA-384 and will not be hashed. */
    CC_ECPKI_AFTER_HASH_SHA512_mode  = 9,   /*!< The message data is a digest of SHA-512 and will not be hashed. */


    CC_ECPKI_HASH_NumOfModes,   /*!< The maximal number of hash modes. */
    CC_ECPKI_HASH_OpModeLast        = 0x7FFFFFFF, /*!< Reserved. */

}CCEcpkiHashOpMode_t;


/*---------------------------------------------------*/
/*! EC point-compression identifiers.
*/
typedef enum
{
    CC_EC_PointCompressed     = 2,  /*!< A compressed point. */
    CC_EC_PointUncompressed   = 4,  /*!< An uncompressed point. */
    CC_EC_PointContWrong      = 5,  /*!< An incorrect point-control value. */
    CC_EC_PointHybrid         = 6,  /*!< A hybrid point. */

    CC_EC_PointCompresOffMode = 8,  /*!< Reserved. */

    CC_ECPKI_PointCompressionLast= 0x7FFFFFFF,  /*!< Reserved. */

}CCEcpkiPointCompression_t;

/*----------------------------------------------------*/
/*! EC key checks. */
typedef enum {
    CheckPointersAndSizesOnly = 0,   /*!< Check only preliminary input parameters. */
    ECpublKeyPartlyCheck      = 1,   /*!< Check preliminary input parameters and verify that the EC public-key point is on the curve. */
    ECpublKeyFullCheck        = 2,   /*!< Check preliminary input parameters, verify that the EC public-key point is on the curve,
                                          and verify that \c EC_GeneratorOrder*PubKey = 0 */

    PublKeyChecingOffMode,          /*! Reserved. */
    EC_PublKeyCheckModeLast  = 0x7FFFFFFF,  /*! Reserved. */
}ECPublKeyCheckMode_t;

/*----------------------------------------------------*/
/*! SW SCA protection type. */
typedef enum {
    SCAP_Inactive,  /*! SCA protection inactive.*/
    SCAP_Active,    /*! SCA protection active.*/
    SCAP_OFF_MODE,  /*! Reserved. */
    SCAP_LAST = 0x7FFFFFFF  /*! Reserved. */
}CCEcpkiScaProtection_t;


/**************************************************************************************
 *               EC Domain structure definition
 ***************************************************************************************/

/*!
 @brief The structure containing the EC domain parameters in little-endian form.

 EC equation: \c Y^2 = \c X^3 + \c A*X + \c B over prime field \p GFp.
 */
typedef  struct {

    uint32_t    ecP [CC_ECPKI_MODUL_MAX_LENGTH_IN_WORDS] /*!< EC modulus: P. */;
    uint32_t    ecA [CC_ECPKI_MODUL_MAX_LENGTH_IN_WORDS] /*!< EC equation parameter A. */;
    uint32_t    ecB [CC_ECPKI_MODUL_MAX_LENGTH_IN_WORDS] /*!< EC equation parameter B. */;
    uint32_t    ecR [CC_ECPKI_MODUL_MAX_LENGTH_IN_WORDS + 1] /*!< Order of generator. */;
    uint32_t    ecGx [CC_ECPKI_MODUL_MAX_LENGTH_IN_WORDS] /*!< EC cofactor EC_Cofactor_K.
        The coordinates of the EC base point generator in projective form. */;
    uint32_t    ecGy [CC_ECPKI_MODUL_MAX_LENGTH_IN_WORDS] /*!< EC cofactor EC_Cofactor_K.
        The coordinates of the EC base point generator in projective form. */;
    uint32_t    ecH /*!< EC cofactor EC_Cofactor_K.
        The coordinates of the EC base point generator in projective form. */;
    uint32_t      llfBuff[CC_PKA_DOMAIN_LLF_BUFF_SIZE_IN_WORDS] /*!< Specific fields that are used by the low-level functions.*/;
    uint32_t    modSizeInBits /*!< The size of fields in bits. */;
    uint32_t    ordSizeInBits /*!< The size of the order in bits. */;
    uint32_t    barrTagSizeInWords /*!< The size of each inserted Barret tag in words. 0 if not inserted.*/;
    CCEcpkiDomainID_t   DomainID /*!< The EC Domain identifier.*/;
    int8_t name[20] /*!< Internal buffer. */;
}CCEcpkiDomain_t;



/**************************************************************************************
 *               EC  point structures definitions
 ***************************************************************************************/

/*! The structure containing the EC point in affine coordinates
   and little endian form. */
typedef  struct
{
    uint32_t x[CC_ECPKI_MODUL_MAX_LENGTH_IN_WORDS] /*!< The X coordinate of the point. */;
    uint32_t y[CC_ECPKI_MODUL_MAX_LENGTH_IN_WORDS] /*!< The Y coordinate of the point. */;

}CCEcpkiPointAffine_t;


/**************************************************************************************
 *                ECPKI public and private key  Structures
 ***************************************************************************************/

/* --------------------------------------------------------------------- */
/* .................. The public key structures definitions ............ */
/* --------------------------------------------------------------------- */

/*! The structure containing the public key in affine coordinates.*/
typedef  struct
{
    uint32_t x[CC_ECPKI_MODUL_MAX_LENGTH_IN_WORDS] /*!< The X coordinate of the public key.*/;
    uint32_t y[CC_ECPKI_MODUL_MAX_LENGTH_IN_WORDS] /*!< The Y coordinate of the public key.*/;
    CCEcpkiDomain_t  domain /*!< The EC Domain.*/;
    uint32_t pointType /*!< The point type.*/;
} CCEcpkiPublKey_t;


/*!
@brief The user structure prototype of the EC public key.

This structure must be saved by the user. It is used as input to ECC functions,
for example, CC_EcdsaVerify().
*/
typedef struct   CCEcpkiUserPublKey_t
{
    uint32_t    valid_tag /*!< The validation tag.*/;
    uint32_t    PublKeyDbBuff[(sizeof(CCEcpkiPublKey_t)+3)/4] /*!< The data of the public key. */;

} CCEcpkiUserPublKey_t;


/* --------------------------------------------------------------------- */
/* .................. The private key structures definitions ........... */
/* --------------------------------------------------------------------- */

/*! The structure containing the data of the private key. */
typedef  struct
{
    uint32_t  PrivKey[CC_ECPKI_MODUL_MAX_LENGTH_IN_WORDS + 1] /*!< The data of the private key. */;
    CCEcpkiDomain_t  domain /*!< The EC domain. */;
    CCEcpkiScaProtection_t  scaProtection /*!< The SCA protection mode. */;

}CCEcpkiPrivKey_t;


/*!
 @brief The user structure prototype of the EC private key.

 This structure must be saved by the user. It is used as input to ECC functions,
 for example, CC_EcdsaSign().
 */
typedef struct   CCEcpkiUserPrivKey_t
{

    uint32_t    valid_tag /*!< The validation tag. */;
    uint32_t    PrivKeyDbBuff[(sizeof(CCEcpkiPrivKey_t)+3)/4] /*!< The data of the private key. */;
}  CCEcpkiUserPrivKey_t;

/*! The type of the ECDH temporary data. */
typedef struct CCEcdhTempData_t
{
    uint32_t ccEcdhIntBuff[CC_PKA_ECDH_BUFF_MAX_LENGTH_IN_WORDS] /*!< Temporary buffers. */;
}CCEcdhTempData_t;

/*! EC build temporary data. */
typedef struct CCEcpkiBuildTempData_t
{
    uint32_t  ccBuildTmpIntBuff[CC_PKA_ECPKI_BUILD_TMP_BUFF_MAX_LENGTH_IN_WORDS] /*!< Temporary buffers. */;
}CCEcpkiBuildTempData_t;



/**************************************************************************
 *                CryptoCell ECDSA context structures
 **************************************************************************/

/* --------------------------------------------------------------------- */
/*                CryptoCell ECDSA Signing context structure                   */
/* --------------------------------------------------------------------- */
/*! The internal buffer used in the signing process. */
typedef uint32_t CCEcdsaSignIntBuff_t[CC_PKA_ECDSA_SIGN_BUFF_MAX_LENGTH_IN_WORDS];

/*! The context definition for the signing operation. */
typedef  struct
{
    CCEcpkiUserPrivKey_t     ECDSA_SignerPrivKey /*!< The data of the private key. */;

#ifdef USE_MBEDTLS_CRYPTOCELL
    mbedtls_md_context_t     hash_ctx /*!< The hash context. */;
#else
    CCHashUserContext_t      hashUserCtxBuff /*!< The hash context. */;
#endif
    CCHashResultBuf_t        hashResult /*!< The hash result buffer. */;
    uint32_t                 hashResultSizeWords /*!< The size of the hash result in words. */;
    CCEcpkiHashOpMode_t  hashMode /*!< The hash mode. */;
    CCEcdsaSignIntBuff_t     ecdsaSignIntBuff /*!< Internal buffer. */;
}EcdsaSignContext_t;


/* --------------------------------------------------------------------- */
/*                ECDSA  Signing User context database              */
/* --------------------------------------------------------------------- */

/*!
 @brief The context definition of the user for the signing operation.

 This context saves the state of the operation, and must be saved by the user
 until the end of the API flow.
 */
typedef struct  CCEcdsaSignUserContext_t
{
    uint32_t  context_buff [(sizeof(EcdsaSignContext_t)+3)/4] /*!< The data of the signing process. */;
    uint32_t  valid_tag /*!< The validation tag. */;
} CCEcdsaSignUserContext_t;



/****************************************************************************/

/* --------------------------------------------------------------------- */
/*                ECDSA Verifying context structure                 */
/* --------------------------------------------------------------------- */
/*! The internal buffer used in the verification process. */
typedef uint32_t CCEcdsaVerifyIntBuff_t[CC_PKA_ECDSA_VERIFY_BUFF_MAX_LENGTH_IN_WORDS];

/*! The context definition for verification operation. */
typedef  struct
{
    CCEcpkiUserPublKey_t        ECDSA_SignerPublKey /*!< The data of the public key. */;


#ifdef USE_MBEDTLS_CRYPTOCELL
    mbedtls_md_context_t        hash_ctx /*!< The hash context. */;
#else
    CCHashUserContext_t         hashUserCtxBuff /*!< The hash context. */;
#endif
    CCHashResultBuf_t           hashResult /*!< The hash result. */;
    uint32_t                    hashResultSizeWords /*!< The size of the hash result in words. */;
    CCEcpkiHashOpMode_t         hashMode /*!< The hash mode. */;
    CCEcdsaVerifyIntBuff_t      ccEcdsaVerIntBuff /*!< Internal buffer. */;
}EcdsaVerifyContext_t;


/* --------------------------------------------------------------------- */
/*                ECDSA Verifying User context database             */
/* --------------------------------------------------------------------- */
/*!
 @brief The context definition of the user for the verification operation.

 The context saves the state of the operation, and must be saved by the user
 until the end of the API flow.
 */
typedef struct  CCEcdsaVerifyUserContext_t
{
    /*! The data of the verification process. */
    uint32_t    context_buff[(sizeof(EcdsaVerifyContext_t)+3)/4];
    /*! The validation tag. */
    uint32_t    valid_tag;
}CCEcdsaVerifyUserContext_t;


/* --------------------------------------------------------------------- */
/* .................. key generation temp buffer   ........... */
/* --------------------------------------------------------------------- */

/*! The temporary data type of the ECPKI KG. */
typedef struct CCEcpkiKgTempData_t
{
    uint32_t ccKGIntBuff[CC_PKA_KG_BUFF_MAX_LENGTH_IN_WORDS] /*!< Internal buffer. */;
}CCEcpkiKgTempData_t;

/*! The temporary data definition of the ECIES. */
typedef struct CCEciesTempData_t {
    CCEcpkiUserPrivKey_t   PrivKey /*!< The data of the private key. */;
    CCEcpkiUserPublKey_t   PublKey /*!< The data of the public key. */;
    CCEcpkiUserPublKey_t   ConvPublKey /*!< The public-key data used by convertion from Mbed TLS to CryptoCell. */;
    uint32_t  zz[3*CC_ECPKI_MODUL_MAX_LENGTH_IN_WORDS + 1] /*!< Internal buffer. */;
    union {
        CCEcpkiBuildTempData_t buildTempbuff;
        CCEcpkiKgTempData_t    KgTempBuff;
        CCEcdhTempData_t       DhTempBuff;
    } tmp /*!< Internal buffers. */;

}CCEciesTempData_t;


/* --------------------------------------------------------------------- */
/* .................. defines for FIPS      ........... */
/* --------------------------------------------------------------------- */

/*! The order length for FIPS ECC tests. */
#define CC_ECPKI_FIPS_ORDER_LENGTH (256/CC_BITS_IN_BYTE)  // the order of secp256r1 in bytes

/*! ECPKI data structures for FIPS certification. */
typedef struct CCEcpkiKgFipsContext_t
{
    union {
        CCEcdsaSignUserContext_t    signCtx;
        CCEcdsaVerifyUserContext_t  verifyCtx;
    }operationCtx /*!< Signing and verification data. */;

    uint32_t    signBuff[2*CC_ECPKI_ORDER_MAX_LENGTH_IN_WORDS] /*!< Internal buffer. */;
}CCEcpkiKgFipsContext_t;

/*! ECDSA KAT data structures for FIPS certification.
    The ECDSA KAT tests are defined for domain 256r1.     */
typedef struct CCEcdsaFipsKatContext_t{
    union {
        struct {
            CCEcpkiUserPrivKey_t    PrivKey;
            CCEcdsaSignUserContext_t    signCtx;
        }userSignData /*!< The data of the private key. */;
        /*! The data of the public key. */
        struct {
            CCEcpkiUserPublKey_t    PublKey;
            union {
                CCEcdsaVerifyUserContext_t  verifyCtx;
                CCEcpkiBuildTempData_t  tempData;
            }buildOrVerify;
        }userVerifyData;
    }keyContextData /*!< The data of the key. */;
    /*! Internal buffer. */
    uint8_t         signBuff[2*CC_ECPKI_FIPS_ORDER_LENGTH];
}CCEcdsaFipsKatContext_t;

/*! ECDH KAT data structures for FIPS certification. */
typedef struct CCEcdhFipsKatContext_t{
    CCEcpkiUserPublKey_t  pubKey /*!< The data of the public key. */;
    CCEcpkiUserPrivKey_t  privKey /*!< The data of the private key. */;
    union {
        CCEcpkiBuildTempData_t  ecpkiTempData;
        CCEcdhTempData_t      ecdhTempBuff;
    }tmpData /*!< Internal buffers. */;

    uint8_t           secretBuff[CC_ECPKI_FIPS_ORDER_LENGTH] /*!< The buffer for the secret key. */;
}CCEcdhFipsKatContext_t;

#ifdef __cplusplus
}
#endif

#endif

