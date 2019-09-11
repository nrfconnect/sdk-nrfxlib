#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

#
# Internal function to append all unnamed parameters with 'prefix'
# and add it to the list 'var'
#
function(append_with_prefix var prefix)
  set(listVar ${${var}})
  foreach(f ${ARGN})
    kconfig_mbedtls_debug("Adding: ${f} to ${var}")
    list(APPEND listVar "${prefix}${f}")
  endforeach(f)
  set(${var} ${listVar} PARENT_SCOPE)
endfunction(append_with_prefix)

#
# Internal macro to append all unnamed parameters with 'prefix' if condition
# is met
#
macro(append_with_prefix_ifdef var cond prefix)
  if (cond)
    append_with_prefix(prefix ${ARGN})
  endif()
endmacro()


PREPEND(src_crypto ${ARM_MBEDTLS_ROOT}/library/
    aes.c
    aesni.c
    arc4.c
    aria.c
    asn1parse.c
    asn1write.c
    base64.c
    bignum.c
    blowfish.c
    camellia.c
    ccm.c
    chacha20.c
    chachapoly.c
    cipher.c
    # cipher_wrap.c - replaced in nrf_mbedtls
    cmac.c
    # ctr_drbg.c # Placed in dedicated mbeddrbg lib for correct symbol resolving during linking
    des.c
    dhm.c
    ecdh.c
    ecdsa.c
    ecjpake.c
    ecp.c
    ecp_curves.c
    # entropy.c - replaced in nrf_mbedtls for cc310
    entropy_poll.c
    error.c
    gcm.c
    havege.c
    hkdf.c
    hmac_drbg.c
    md.c
    md2.c
    md4.c
    md5.c
    md_wrap.c
    memory_buffer_alloc.c
    nist_kw.c
    oid.c
    padlock.c
    # pem.c - replaced in nrf_mbedtls
    pk.c
    pk_wrap.c
    pkcs12.c
    pkcs5.c
    pkparse.c
    pkwrite.c
    platform.c
    platform_util.c
    poly1305.c
    ripemd160.c
    rsa.c
    rsa_internal.c
    sha1.c
    sha256.c
    sha512.c
    threading.c
    timing.c
    version.c
    version_features.c
    xtea.c
)

PREPEND(src_x509 ${ARM_MBEDTLS_ROOT}/library/
    certs.c
    pkcs11.c
    x509.c
    x509_create.c
    x509_crl.c
    x509_crt.c
    x509_csr.c
    x509write_crt.c
    x509write_csr.c
)

PREPEND(src_tls ${ARM_MBEDTLS_ROOT}/library/
    debug.c
    net_sockets.c
    ssl_cache.c
    # ssl_ciphersuites.c - replaced in nrf_mbedtls
    ssl_cli.c
    ssl_cookie.c
    ssl_srv.c
    ssl_ticket.c
    ssl_tls.c
)

# ----------  Replacement sources - modified mbedTLS sources  -------------

PREPEND(src_crypto_replacement ${NRF_MBEDTLS_ROOT}/src/mbedtls/replacements/
    cipher_wrap.c
    pem.c
)

PREPEND(src_tls_replacement ${NRF_MBEDTLS_ROOT}/src/mbedtls/replacements/
    ssl_ciphersuites.c
)

zephyr_include_directories(${ARM_MBEDTLS_ROOT}/include)
zephyr_include_directories(${ARM_MBEDTLS_ROOT}/include/mbedtls)

# When using multiple backends this library should not be linked directly as
# there might be name collisions with other libs, e.g. mbedcrypto_cc310
# When multiple backends are enabled, unique symbols are guaranteed through
# symbol renaming and glue layer.
# Therefore add_library is used instead of zephyr_library
add_library(${IMAGE}mbedcrypto_vanilla STATIC "")
set(ZEPHYR_CURRENT_LIBRARY ${IMAGE}mbedcrypto_vanilla)
zephyr_library_sources(${src_crypto} ${src_crypto_replacement})
zephyr_library_include_directories(${common_includes})
zephyr_library_link_libraries(${IMAGE}zephyr_interface)
zephyr_library_compile_definitions_ifdef(CONFIG_MBEDTLS_CIPHER_AES_256_ECB_C MBEDTLS_CIPHER_AES_256_ECB_C)
zephyr_library_compile_definitions_ifdef(CONFIG_MBEDTLS_CIPHER_AES_256_CBC_C MBEDTLS_CIPHER_AES_256_CBC_C)
zephyr_library_compile_definitions_ifdef(CONFIG_MBEDTLS_CIPHER_AES_256_CTR_C MBEDTLS_CIPHER_AES_256_CTR_C)
zephyr_library_compile_definitions_ifdef(CONFIG_MBEDTLS_CIPHER_AES_256_CCM_C MBEDTLS_CIPHER_AES_256_CCM_C)
if(TARGET ${IMAGE}mbedcrypto_cc310)
  target_link_libraries(${IMAGE}mbedcrypto_vanilla PRIVATE ${IMAGE}mbedcrypto_cc310)
endif()

# mbeddrbg library.
# In order to ensure correct linking to glue functions from ctr_drbg this file
# must be placed in independent library.
zephyr_library_named(mbeddrbg)
zephyr_library_sources(${ARM_MBEDTLS_ROOT}/library/ctr_drbg.c)
if(CONFIG_GLUE_MBEDTLS_AES_C)
zephyr_library_compile_definitions(-DMBEDTLS_AES_ALT)
endif()
if(CONFIG_SOC_NRF52840 OR CONFIG_SOC_NRF9160)
  zephyr_library_sources(${NRF_MBEDTLS_ROOT}/src/backend/cc310/replacements/entropy.c)
else()
  zephyr_library_sources(${NRF_MBEDTLS_ROOT}/src/backend/nrf5x/replacements/entropy_nrf5x.c)
endif()

if (CONFIG_MBED_X509_LIBRARY)
  zephyr_library_named(mbedx509_vanilla)
  zephyr_library_sources(${src_x509})
  zephyr_library_include_directories(${common_includes})
  if(TARGET ${IMAGE}mbedcrypto_cc310)
    zephyr_library_include_directories(
      $<TARGET_PROPERTY:${IMAGE}mbedcrypto_cc310,INTERFACE_INCLUDE_DIRECTORIES>
    )
  endif()
endif()

if (CONFIG_MBED_TLS_LIBRARY)
  zephyr_library_named(mbedtls_vanilla)
  zephyr_library_sources(${src_tls} ${src_tls_replacement})
  zephyr_library_include_directories(${common_includes})
  if(TARGET ${IMAGE}mbedcrypto_cc310)
    zephyr_library_include_directories(
      $<TARGET_PROPERTY:${IMAGE}mbedcrypto_cc310,INTERFACE_INCLUDE_DIRECTORIES>
    )
  endif()
endif()

if(NOT CONFIG_NRF_CRYPTO_BACKEND_COMBINATION_0)
  # Only vanilla backend is chosen, thus append it to zephyr libraries.
  zephyr_append_cmake_library(${IMAGE}mbedcrypto_vanilla)
endif()

#
# Give the main application access to the generated includes
# NOTE: This library corresponds to APP_LINK_WITH_MBEDCRYPTO_INCLUDES in Kconfig
#
zephyr_interface_library_named(mbedcrypto_includes)
target_compile_definitions(app PRIVATE -DMBEDTLS_CONFIG_FILE="${CONFIG_MBEDTLS_CFG_FILE}")
target_include_directories(${IMAGE}mbedcrypto_includes INTERFACE
  ${common_includes}
  ${generated_includes}
)
