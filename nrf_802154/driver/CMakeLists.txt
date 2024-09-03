#
# Copyright (c) 2021, Nordic Semiconductor ASA
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# 3. Neither the name of Nordic Semiconductor ASA nor the names of its
#    contributors may be used to endorse or promote products derived from this
#    software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
#

target_include_directories(nrf-802154-driver-interface INTERFACE include)

add_library(nrf-802154-driver STATIC EXCLUDE_FROM_ALL)

target_include_directories(nrf-802154-driver
  PRIVATE
    src
)

target_sources(nrf-802154-driver
  PRIVATE
    src/nrf_802154.c
    src/nrf_802154_aes_ccm_acc_ccm.c
    src/nrf_802154_aes_ccm_acc_ecb.c
    src/nrf_802154_bsim_utils.c
    src/nrf_802154_co.c
    src/nrf_802154_core.c
    src/nrf_802154_core_hooks.c
    src/nrf_802154_critical_section.c
    src/nrf_802154_debug.c
    src/nrf_802154_encrypt.c
    src/nrf_802154_notification_direct.c
    src/nrf_802154_notification_swi.c
    src/nrf_802154_pib.c
    src/nrf_802154_peripherals_alloc.c
    src/nrf_802154_queue.c
    src/nrf_802154_request_direct.c
    src/nrf_802154_request_swi.c
    src/nrf_802154_rssi.c
    src/nrf_802154_rx_buffer.c
    src/nrf_802154_stats.c
    src/nrf_802154_swi.c
    src/nrf_802154_trx.c
    src/nrf_802154_trx_dppi.c
    src/nrf_802154_trx_ppi.c
    src/nrf_802154_tx_work_buffer.c
    src/nrf_802154_tx_power.c
    src/mac_features/nrf_802154_csma_ca.c
    src/mac_features/nrf_802154_delayed_trx.c
    src/mac_features/nrf_802154_filter.c
    src/mac_features/nrf_802154_frame_parser.c
    src/mac_features/nrf_802154_ie_writer.c
    src/mac_features/nrf_802154_ifs.c
    src/mac_features/nrf_802154_security_pib_ram.c
    src/mac_features/nrf_802154_security_writer.c
    src/mac_features/nrf_802154_precise_ack_timeout.c
    src/mac_features/ack_generator/nrf_802154_ack_data.c
    src/mac_features/ack_generator/nrf_802154_ack_generator.c
    src/mac_features/ack_generator/nrf_802154_enh_ack_generator.c
    src/mac_features/ack_generator/nrf_802154_imm_ack_generator.c
)

if (SL_OPENSOURCE)
  target_compile_definitions(nrf-802154-driver-interface
    INTERFACE
      # Disable Frame Timestamps
      NRF_802154_FRAME_TIMESTAMP_ENABLED=0
      # Disable DTRX
      NRF_802154_DELAYED_TRX_ENABLED=0
      # Disable IFS
      NRF_802154_IFS_ENABLED=0
      # Disable CSMA-CA
      NRF_802154_CSMA_CA_ENABLED=0
      # Use nrf_802154_notification_direct.c implementation for "notification" module
      NRF_802154_NOTIFICATION_IMPL=NRF_802154_NOTIFICATION_IMPL_DIRECT
      # Use nrf_802154_request_direct.c implementation for "request" module
      NRF_802154_REQUEST_IMPL=NRF_802154_REQUEST_IMPL_DIRECT
  )
endif()

if (NRF_802154_PROJECT_CONFIG)
  target_compile_definitions(nrf-802154-driver-interface
    INTERFACE
      # Propagate project config from build system defines
      NRF_802154_PROJECT_CONFIG="${NRF_802154_PROJECT_CONFIG}"
  )
endif()

target_compile_definitions(nrf-802154-driver-interface
  INTERFACE
    # The core for which the code is compiled contains the 802.15.4 radio driver
    NRF_802154_SERIALIZATION_HOST=0
)

target_link_libraries(nrf-802154-driver-interface
  INTERFACE
    nrf-802154-common-interface
)

target_link_libraries(nrf-802154-driver
  PUBLIC
    nrf-802154-driver-interface
    nrf-802154-platform
    nrf-802154-sl
    nrf-802154-common
)
