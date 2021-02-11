#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

add_subdirectory_ifdef(CONFIG_ZIGBEE_LIBRARY_PRODUCTION  production)
add_subdirectory_ifdef(CONFIG_ZIGBEE_LIBRARY_DEVELOPMENT development)
