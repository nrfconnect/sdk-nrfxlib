#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

zephyr_library()
zephyr_include_directories(../../include/)
zephyr_library_sources(psa_eits.c)

if(CONFIG_PSA_EITS_BACKEND_ZEPHYR)
        zephyr_library_sources(psa_eits_zephyr_settings.c)
endif()
