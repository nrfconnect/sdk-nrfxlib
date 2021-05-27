#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

#
# Add mbed TLS source and backends
#
add_subdirectory(mbedtls)

#
# Add include folder for glued libraries
#
set(mbedcrypto_glue_include_path
  "${NRF_SECURITY_ROOT}/include/mbedcrypto_glue/mbedtls"
)

#
# Add mbedcrypto_glue if enabled
#
if (CONFIG_NRF_SECURITY_MULTI_BACKEND)
  # Create glued library if multiple backends enable the same gluable
  # features.
  add_subdirectory(mbedcrypto_glue)
  set(glue mbedcrypto_glue)
endif()

# When not building for TF-M we create a static mbedcrypto library.
add_library(${mbedcrypto_target} STATIC)
nrf_security_target_embed_objects(TARGET ${mbedcrypto_target}
  DIRECTORIES mbedtls ${glue}
)

nrf_security_target_embed_libraries(TARGET ${mbedcrypto_target}
  DIRECTORIES mbedtls ${glue}
)

# This allows other targets to fetch the MBEDTLS_CONFIG_FILE
# property without fetching all other target settings.
set_property(TARGET ${mbedcrypto_target}
  PROPERTY
    MBEDTLS_CONFIG_FILE ${CONFIG_MBEDTLS_CFG_FILE}
)

set_property(TARGET ${mbedcrypto_target}
  APPEND PROPERTY PRIVATE_LINK_LIBRARIES
    # In TF-M build, a psa_interface lib is created and we must inherit its settings
    $<TARGET_NAME_IF_EXISTS:psa_interface>
)

# Add generated includes (configs and alt-files for mbed TLS)
target_link_libraries(${mbedcrypto_target}
  INTERFACE
    mbedcrypto_generated_includes
)

# Add regular mbed TLS includes
target_link_libraries(${mbedcrypto_target}
  INTERFACE
    mbedcrypto_includes
)

# NEWLIB requires linking with the C library
if (CONFIG_NEWLIB_LIBC)
  target_link_libraries(${mbedcrypto_target} PRIVATE -lc)
endif()

target_compile_definitions(${mbedcrypto_target}
  INTERFACE
    PSA_CRYPTO_SECURE
)

if(TARGET platform_cc3xx)
  target_link_libraries(${mbedcrypto_target} PRIVATE platform_cc3xx)
  target_include_directories(${mbedcrypto_target}
    INTERFACE ${mbedcrypto_glue_include_path}/threading
      $<TARGET_PROPERTY:platform_cc3xx,INTERFACE_INCLUDE_DIRECTORIES>
  )
endif()

# Generate libraries for Zephyr
if (TARGET zephyr_interface)
  add_subdirectory(zephyr)
endif()

# Add PSA APIs
add_subdirectory(psa)
