#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

# Legacy APIs missing driver support or APIs assumed to be
# deprecated with more PSA API awareness.
append_with_prefix(src_crypto_legacy ${ARM_MBEDTLS_PATH}/library
  aesni.c
  aria.c
  camellia.c
  cipher.c
  cipher_wrap.c
  des.c
  dhm.c
  md.c
  pk.c
  pk_wrap.c
)

# Add all files that are neccessary for nrf_oberon + builtin in legacy mode
# These files are used only as a convenience until PSA Crypto APIs are standarized
if(CONFIG_MBEDTLS_LEGACY_CRYPTO_C OR
  CONFIG_PSA_CRYPTO_DRIVER_BUILTIN OR
  CONFIG_PSA_CRYPTO_DRIVER_OBERON)
  append_with_prefix(src_crypto_legacy ${ARM_MBEDTLS_PATH}/library
    cmac.c
    ccm.c
    chachapoly.c
    ecdh.c
    ecdsa.c
    ecjpake.c
    ecp.c
    ecp_curves.c
    gcm.c
    hkdf.c
    hmac_drbg.c
    md5.c
    ripemd160.c
    sha1.c
    sha256.c
    sha512.c
    rsa.c
    rsa_alt_helpers.c
  )
endif()

# Legacy files should only be added in certain configurations
if(NOT COMPILE_PSA_APIS OR (COMPILE_PSA_APIS AND NOT CONFIG_CRYPTOCELL_USABLE))
  append_with_prefix(src_crypto_legacy ${ARM_MBEDTLS_PATH}/library
    ctr_drbg.c
    entropy.c
  )
endif()

# Legacy Files needed when Oberon is disabled
if(NOT CONFIG_OBERON_BACKEND)
  append_with_prefix(src_crypto_legacy ${ARM_MBEDTLS_PATH}/library
    chacha20.c
    poly1305.c
  )
endif()

# Add oberon-specific replacement files
if (CONFIG_OBERON_BACKEND OR PSA_CRYPTO_DRIVER_OBERON)
  list(APPEND src_crypto_legacy
    ${CMAKE_CURRENT_LIST_DIR}/aes.c
    ${CMAKE_CURRENT_LIST_DIR}/aes_oberon.c
    ${CMAKE_CURRENT_LIST_DIR}/ecjpake_oberon.c
  )
else()
  list(APPEND src_crypto_legacy
    ${ARM_MBEDTLS_PATH}/library/aes.c
  )
endif()

# Add legacy files to mbedcrypto_target
if(DEFINED src_crypto_legacy)
  target_sources(${mbedcrypto_target}
    PRIVATE
      ${src_crypto_legacy}
  )
endif()

if(CONFIG_MBEDTLS_LEGACY_CRYPTO_C AND CONFIG_OBERON_BACKEND)

  # Get the include paths from nrf_oberon libraries
  get_target_property(NRF_OBERON_MBEDTLS_INCLUDE_PATH mbedcrypto_oberon_mbedtls_imported
    INTERFACE_INCLUDE_DIRECTORIES
  )
  get_target_property(NRF_OBERON_INCLUDE_PATH mbedcrypto_oberon_imported
    INTERFACE_INCLUDE_DIRECTORIES
  )

  # Link to the nrf_oberon mbedcrypto library
  target_link_libraries(${mbedcrypto_target} PRIVATE
    mbedcrypto_oberon_mbedtls_imported
  )

  # Add nrf_oberon includes to mbedcrypto library
  target_include_directories(${mbedcrypto_target}
    INTERFACE
      ${NRF_OBERON_MBEDTLS_INCLUDE_PATH}
      ${NRF_OBERON_INCLUDE_PATH}
  )
elseif(CONFIG_MBEDTLS_LEGACY_CRYPTO_C AND CONFIG_CC3XX_BACKEND)

  # Link directly with cc3xx platform/mbedcrypto libraries
  target_link_libraries(${mbedcrypto_target} PRIVATE
    mbedcrypto_cc3xx_imported
  )

  # Add cc3xx platform/mbedcrypto includes to mbedcrypto library
  target_include_directories(${mbedcrypto_target}
    INTERFACE
      $<TARGET_PROPERTY:mbedcrypto_cc3xx_imported,INTERFACE_INCLUDE_DIRECTORIES>
  )

endif()

target_include_directories(${mbedcrypto_target}
INTERFACE
  $<$<TARGET_EXISTS:platform_cc3xx>:$<TARGET_PROPERTY:platform_cc3xx,INTERFACE_INCLUDE_DIRECTORIES>>
)
