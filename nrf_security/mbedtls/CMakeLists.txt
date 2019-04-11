#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#
FUNCTION(PREPEND var prefix)
  SET(listVar "")
  FOREACH(f ${ARGN})
    LIST(APPEND listVar "${prefix}${f}")
  ENDFOREACH(f)
  SET(${var} "${listVar}" PARENT_SCOPE)
ENDFUNCTION(PREPEND)

# ----------  Original (unmodified) mbedTLS sources -------------

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
    ctr_drbg.c
    des.c
    dhm.c
    ecdh.c
    ecdsa.c
    ecjpake.c
    ecp.c
    ecp_curves.c
    # entropy.c - replaced in nrf_mbedtls
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

PREPEND(src_crypto_replacement ${NRF_MBEDTLS_ROOT}/src/backend/cc310/replacements/
    entropy.c
)

zephyr_include_directories(${ARM_MBEDTLS_ROOT}/include)
zephyr_include_directories(${ARM_MBEDTLS_ROOT}/include/mbedtls)

add_library(mbedcrypto_vanilla STATIC "")
set(ZEPHYR_CURRENT_LIBRARY mbedcrypto_vanilla)
zephyr_library_sources(${src_crypto} ${src_crypto_replacement})
zephyr_library_include_directories(${common_includes})
zephyr_library_link_libraries(zephyr_interface)
zephyr_library_compile_definitions_ifdef(CONFIG_MBEDTLS_CIPHER_AES_256_ECB_C MBEDTLS_CIPHER_AES_256_ECB_C)
zephyr_library_compile_definitions_ifdef(CONFIG_MBEDTLS_CIPHER_AES_256_CBC_C MBEDTLS_CIPHER_AES_256_CBC_C)
zephyr_library_compile_definitions_ifdef(CONFIG_MBEDTLS_CIPHER_AES_256_CTR_C MBEDTLS_CIPHER_AES_256_CTR_C)
zephyr_library_compile_definitions_ifdef(CONFIG_MBEDTLS_CIPHER_AES_256_CCM_C MBEDTLS_CIPHER_AES_256_CCM_C)
if(TARGET mbedcrypto_cc310)
  target_link_libraries(mbedcrypto_vanilla PRIVATE mbedcrypto_cc310)
endif()

add_library(mbedx509_vanilla STATIC "")
set(ZEPHYR_CURRENT_LIBRARY mbedx509_vanilla)
zephyr_library_sources(${src_x509})
zephyr_library_include_directories(${common_includes})
zephyr_library_compile_definitions(-DMBEDTLS_CONFIG_FILE=${MBEDTLS_CONFIGURATION})
zephyr_library_link_libraries(zephyr_interface)
if(TARGET mbedcrypto_cc310)
  zephyr_library_include_directories(
    $<TARGET_PROPERTY:mbedcrypto_cc310,INTERFACE_INCLUDE_DIRECTORIES>
  )
endif()

add_library(mbedtls_vanilla STATIC "")
set(ZEPHYR_CURRENT_LIBRARY mbedtls_vanilla)
zephyr_library_sources(${src_tls} ${src_tls_replacement} ${MBEDTLS_CONFIGURATION_FILE})
zephyr_library_include_directories(${common_includes})
zephyr_library_compile_definitions(-DMBEDTLS_CONFIG_FILE=${MBEDTLS_CONFIGURATION})
zephyr_library_link_libraries(zephyr_interface)
if(TARGET mbedcrypto_cc310)
  zephyr_library_include_directories(
    $<TARGET_PROPERTY:mbedcrypto_cc310,INTERFACE_INCLUDE_DIRECTORIES>
  )
endif()

if(NOT CONFIG_NRF_CRYPTO_BACKEND_COMBINATION_0)
  zephyr_append_cmake_library(mbedcrypto_vanilla)
endif()
