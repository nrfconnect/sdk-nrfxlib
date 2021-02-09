#
# Copyright (c) 2020 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#
# The purpose of this file is to provice a set of debug commands to nrf security
# module.

nrf_security_debug("######### Creating shared noglue library #########")

#
# mbedcrypto_shared is a library containing features that are all
# provided by vanilla backend, but is expressively enabled to allow
# single backend being enabled in the system and still have full access to
# features not provided by the specific backend
#
# All features added here can't be glued

nrf_security_shared_library(
  NAME mbedcrypto_shared
  FILES ${ARM_MBEDTLS_PATH}/library/chacha20.c INCLUDE VANILLA OBERON EXCLUDE CC3XX
  FILES ${ARM_MBEDTLS_PATH}/library/poly1305.c INCLUDE VANILLA OBERON EXCLUDE CC3XX
  FILES ${ARM_MBEDTLS_PATH}/library/chachapoly.c INCLUDE VANILLA EXCLUDE CC3XX
  FILES ${ARM_MBEDTLS_PATH}/library/rsa.c ${ARM_MBEDTLS_PATH}/library/rsa_internal.c INCLUDE VANILLA EXCLUDE CC3XX
  FILES ${ARM_MBEDTLS_PATH}/library/sha512.c
#  FILES dhm.c BACKEND OBERON
)
# ToDo DHM.
## Special case: Oberon only backend and enabled DHM
#if (CONFIG_MBEDTLS_DHM_C AND CONFIG_OBERON_BACKEND)
#
#  target_sources(mbedcrypto_shared PRIVATE
#    ${ARM_MBEDTLS_PATH}/library/dhm.c
#  )
#endif()
