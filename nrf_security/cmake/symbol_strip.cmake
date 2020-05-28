#
# Copyright (c) 2020 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

#
# This file contain a function which allow symbols in a library that implements
# the arm mbed TLS API to be strip unwanted symbols
# This allows nRF Connect SDK Security to support multiple crypto backends
# that share the same front-end API.
#

#
# This macro will create a string to remove objects
# if the given configuration is not enabled in the backend
#
macro(remove_objects mbedtls_config backend_name object_list object_name)
  if (NOT CONFIG_${backend_name}_${mbedtls_config})
    nrf_security_debug("${backend_name}: Removing object ${object_name}")
    set(${object_list} ${${object_list}} ${object_name})
  else ()
    nrf_security_debug("Not removing: CONFIG_${backend_name}_${mbedtls_config} is set to TRUE")
  endif()
endmacro()

#
# This function will create a symbol renaming script for any library
# named mbedcrypto_<backend> corresponding to the parameter "backend"
# If is_stripped is "TRUE" this function will strip symbols from any library
# named mbedcrypto_<backend> corresponding to the parameter "backend"
#
function(symbol_strip_func backend)
  nrf_security_debug("========== Running symbol_strip_function for ${backend} ==========")
  string(TOUPPER "${backend}" BACKEND_NAME_UPPER)

  remove_objects("${BACKEND_NAME_UPPER}_ENABLED"   ${BACKEND_NAME_UPPER} remove_line "empty_file.c.obj")
  remove_objects("MBEDTLS_AES_C"        ${BACKEND_NAME_UPPER} remove_line "aes.c.obj")
  remove_objects("MBEDTLS_AES_C"        ${BACKEND_NAME_UPPER} remove_line "aes_alt.c.obj")
  remove_objects("MBEDTLS_CCM_C"        ${BACKEND_NAME_UPPER} remove_line "ccm.c.obj")
  remove_objects("MBEDTLS_CCM_C"        ${BACKEND_NAME_UPPER} remove_line "ccm_alt.c.obj")
  remove_objects("MBEDTLS_CMAC_C"       ${BACKEND_NAME_UPPER} remove_line "cmac.c.obj")
  remove_objects("MBEDTLS_CMAC_C"       ${BACKEND_NAME_UPPER} remove_line "cmac_alt.c.obj")
  remove_objects("MBEDTLS_ECP_C"        ${BACKEND_NAME_UPPER} remove_line "ecp.c.obj")
  remove_objects("MBEDTLS_ECP_C"        ${BACKEND_NAME_UPPER} remove_line "ecp_common.c.obj")
  remove_objects("MBEDTLS_ECP_C"        ${BACKEND_NAME_UPPER} remove_line "ecp_curves.c.obj")
  remove_objects("MBEDTLS_ECP_C"        ${BACKEND_NAME_UPPER} remove_line "ecp_alt_cc310.c.obj")
  remove_objects("MBEDTLS_ECP_C"        ${BACKEND_NAME_UPPER} remove_line "ecp_curves_alt.c.obj")
  remove_objects("MBEDTLS_ECDH_C"       ${BACKEND_NAME_UPPER} remove_line "ecdh.c.obj")
  remove_objects("MBEDTLS_ECDH_C"       ${BACKEND_NAME_UPPER} remove_line "ecdh_alt.c.obj")
  remove_objects("MBEDTLS_ECDSA_C"      ${BACKEND_NAME_UPPER} remove_line "ecdsa.c.obj")
  remove_objects("MBEDTLS_ECDSA_C"      ${BACKEND_NAME_UPPER} remove_line "ecdsa_alt.c.obj")
  remove_objects("MBEDTLS_ECJPAKE_C"    ${BACKEND_NAME_UPPER} remove_line "ecjpake.c.obj")
  remove_objects("MBEDTLS_ECJPAKE_C"    ${BACKEND_NAME_UPPER} remove_line "ecjpake_alt.c.obj")
  remove_objects("MBEDTLS_CHACHA20_C"   ${BACKEND_NAME_UPPER} remove_line "chacha20.c.obj")
  remove_objects("MBEDTLS_CHACHA20_C"   ${BACKEND_NAME_UPPER} remove_line "chacha20_alt.c.obj")
  remove_objects("MBEDTLS_CHACHAPOLY_C" ${BACKEND_NAME_UPPER} remove_line "chachapoly.c.obj")
  remove_objects("MBEDTLS_CHACHAPOLY_C" ${BACKEND_NAME_UPPER} remove_line "chachapoly_alt.c.obj")
  remove_objects("MBEDTLS_POLY1305_C"   ${BACKEND_NAME_UPPER} remove_line "poly1305_alt.c.obj")
  remove_objects("MBEDTLS_POLY1305_C"   ${BACKEND_NAME_UPPER} remove_line "poly1305.c.obj")
  remove_objects("MBEDTLS_POLY1305_C"   ${BACKEND_NAME_UPPER} remove_line "poly.c.obj")
  remove_objects("MBEDTLS_SHA1_C"       ${BACKEND_NAME_UPPER} remove_line "sha1.c.obj")
  remove_objects("MBEDTLS_SHA1_C"       ${BACKEND_NAME_UPPER} remove_line "sha1_alt.c.obj")
  remove_objects("MBEDTLS_SHA256_C"     ${BACKEND_NAME_UPPER} remove_line "sha256.c.obj")
  remove_objects("MBEDTLS_SHA256_C"     ${BACKEND_NAME_UPPER} remove_line "sha256_alt.c.obj")

  set(BACKEND_LIBRARY libmbedcrypto_${backend}.a)

  if (remove_line)
    set(remove_object_command ${CMAKE_AR} d ${BACKEND_LIBRARY} ${remove_line})
    nrf_security_debug("Objects stripped from mbedcrypto_${backend}: ${remove_line}")
  else()
    nrf_security_debug("No remove_line from mbedcrypto_${backend}")
  endif()

  add_custom_command(
    TARGET mbedcrypto_${backend}
    POST_BUILD
    COMMAND ${remove_object_command}
  )
endfunction()
