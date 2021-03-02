#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

#
# Add mbedcrypto, mbedx509 and mbedtls as dependencies for
# TF-M
#
add_dependencies(tfm mbedcrypto)

if (TARGET mbedx509)
  add_dependencies(tfm mbedx509)
endif()

if (TARGET mbedtls)
  add_dependencies(tfm mbedtls)
endif()