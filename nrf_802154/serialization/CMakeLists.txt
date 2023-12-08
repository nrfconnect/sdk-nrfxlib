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

target_include_directories(nrf-802154-serialization-interface
  INTERFACE
    include
    include/platform
    include/serialization
)

add_library(nrf-802154-serialization STATIC EXCLUDE_FROM_ALL)

target_include_directories(nrf-802154-serialization
  PRIVATE
    src/include
)

target_sources(nrf-802154-serialization
  PRIVATE
    spinel_base/spinel.c
    src/nrf_802154_buffer_allocator.c
    src/nrf_802154_buffer_mgr_dst.c
    src/nrf_802154_buffer_mgr_src.c
    src/nrf_802154_kvmap.c
    src/nrf_802154_spinel.c
    src/nrf_802154_spinel_dec.c
)

if(CONFIG_NATIVE_LIBRARY)
  # The spinel header defines the spinel API, with SPINEL_API_EXTERN, as externally linkable.
  # When building Zephyr for the native_simulator targets (including bsim targets) it would
  # cause the linker to find duplicates when building the final executable if multiple MCUs
  # include each this code. We fix this by having the native_simulator build make those symbols
  # local before linking the final executable.
  set_property(TARGET native_simulator APPEND PROPERTY
    LOCALIZE_EXTRA_OPTIONS "--localize-symbol=spinel*"
  )
endif()

if (SER_HOST)
  target_sources(nrf-802154-serialization
    PRIVATE
      src/nrf_802154_spinel_app.c
      src/nrf_802154_spinel_dec_app.c
  )
  target_compile_definitions(nrf-802154-serialization-interface
    INTERFACE
      # The core for which the code is compiled does not contain the 802.15.4 radio driver
      # It contains the serialized radio driver functions.
      NRF_802154_SERIALIZATION_HOST=1
  )
else ()
  target_sources(nrf-802154-serialization
    PRIVATE
      src/nrf_802154_spinel_net.c
      src/nrf_802154_spinel_dec_net.c
  )
endif()

target_link_libraries(nrf-802154-serialization-interface
  INTERFACE
    nrf-802154-common-interface
)

target_link_libraries(nrf-802154-serialization
  PRIVATE
    nrf-802154-serialization-interface
    nrf-802154-platform
    nrf-802154-common
)
