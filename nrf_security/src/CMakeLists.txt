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
# Add mbedcrypto_glue if enabled
#
if (CONFIG_NRF_SECURITY_MULTI_BACKEND)
  # Create glued library if multiple backends enable the same gluable
  # features.
  add_subdirectory(mbedcrypto_glue)
  set(glue mbedcrypto_glue)
endif()

add_library(mbedcrypto STATIC)
nrf_security_target_embed_objects(TARGET mbedcrypto
  DIRECTORIES mbedtls ${glue}
  )

nrf_security_target_embed_libraries(TARGET mbedcrypto
  DIRECTORIES mbedtls ${glue}
)

#
# Create generic libraries for TF-M or Zephyr
#
if (CONFIG_BUILD_WITH_TFM)
  # Generate libraries for TF-M
  # Remember: generated configs for Zephyr scope
  add_subdirectory(tfm)
else()
  # Generate libraries for Zephyr
  add_subdirectory(zephyr)
endif()

add_subdirectory(psa)
