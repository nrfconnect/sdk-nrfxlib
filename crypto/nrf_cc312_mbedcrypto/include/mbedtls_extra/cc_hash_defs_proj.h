/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*!
 @addtogroup cc_hash_defs_proj
 @{
 */


/*!
 @file
 @brief This file contains the project-specific definitions of hash APIs.
 */

#ifndef CC_HASH_DEFS_PROJ_H
#define CC_HASH_DEFS_PROJ_H

#ifdef __cplusplus
extern "C"
{
#endif


/************************ Defines ******************************/

/*! The size of the context prototype of the user in words.
 * See ::CCHashUserContext_t. */
/* In our system this needs to have enough memory to hold
 * the biggest possible "struct psa_hash_operation_t" in sdk-nrf.
 * The psa_hash_operation_t in sdk-nrf includes the fields for the id and
 * the cc3xx_hash_operation_t and the oberon_hash_operation_t.
 * This needs to have enough memory to fit the whole psa_hash_operation_t struct
 * when both the drivers are enabled. */
#define CC_HASH_USER_CTX_SIZE_IN_WORDS 108


#ifdef __cplusplus
}
#endif

/*!
 @}
 */
#endif /* CC_HASH_DEFS_PROJ_H */
