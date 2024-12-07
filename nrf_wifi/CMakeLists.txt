#
# Copyright (c) 2024 Nordic Semiconductor
#
# SPDX-License-Identifier: BSD-3-Clause
#

# Enable debug info by default
# Set the CPU architecture to armv8-m.main (Cortex-M33), default is armv4t
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -mcpu=cortex-m33 -mthumb")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -mcpu=cortex-m33 -mthumb")

add_library(nrf-wifi STATIC)

set(NRF_WIFI_DIR ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(
  nrf-wifi
  PUBLIC
  ${NRF_WIFI_DIR}/utils/inc
  ${NRF_WIFI_DIR}/os_if/inc
  ${NRF_WIFI_DIR}/bus_if/bus/qspi/inc
  ${NRF_WIFI_DIR}/bus_if/bal/inc
  ${NRF_WIFI_DIR}/fw_if/umac_if/inc
  ${NRF_WIFI_DIR}/fw_load/mips/fw/inc
  ${NRF_WIFI_DIR}/hw_if/hal/inc
  ${NRF_WIFI_DIR}/hw_if/hal/inc/fw
  ${NRF_WIFI_DIR}/fw_if/umac_if/inc/fw
)

target_include_directories_ifdef(CONFIG_NRF70_RADIO_TEST
  nrf-wifi
  PUBLIC
  ${NRF_WIFI_DIR}/fw_if/umac_if/inc/radio_test
)

target_include_directories_ifndef(CONFIG_NRF70_RADIO_TEST
  nrf-wifi
  PUBLIC
  ${NRF_WIFI_DIR}/fw_if/umac_if/inc/default
)

target_sources(
  nrf-wifi
  PRIVATE
  ${NRF_WIFI_DIR}/os_if/src/osal.c
  ${NRF_WIFI_DIR}/utils/src/list.c
  ${NRF_WIFI_DIR}/utils/src/queue.c
  ${NRF_WIFI_DIR}/utils/src/util.c
  ${NRF_WIFI_DIR}/hw_if/hal/src/hal_api.c
  ${NRF_WIFI_DIR}/hw_if/hal/src/hal_fw_patch_loader.c
  ${NRF_WIFI_DIR}/hw_if/hal/src/hal_interrupt.c
  ${NRF_WIFI_DIR}/hw_if/hal/src/hal_mem.c
  ${NRF_WIFI_DIR}/hw_if/hal/src/hal_reg.c
  ${NRF_WIFI_DIR}/hw_if/hal/src/hpqm.c
  ${NRF_WIFI_DIR}/hw_if/hal/src/pal.c
  ${NRF_WIFI_DIR}/bus_if/bal/src/bal.c
  ${NRF_WIFI_DIR}/bus_if/bus/qspi/src/qspi.c
  ${NRF_WIFI_DIR}/fw_if/umac_if/src/cmd.c
  ${NRF_WIFI_DIR}/fw_if/umac_if/src/event.c
  ${NRF_WIFI_DIR}/fw_if/umac_if/src/fmac_api_common.c
)

target_sources_ifndef(CONFIG_NRF70_RADIO_TEST
  nrf-wifi
  PRIVATE
  ${NRF_WIFI_DIR}/fw_if/umac_if/src/rx.c
  ${NRF_WIFI_DIR}/fw_if/umac_if/src/fmac_vif.c
  ${NRF_WIFI_DIR}/fw_if/umac_if/src/fmac_util.c
  ${NRF_WIFI_DIR}/fw_if/umac_if/src/default/fmac_api.c
)

target_sources_ifdef(CONFIG_NRF70_RADIO_TEST
  nrf-wifi
  PRIVATE
  ${NRF_WIFI_DIR}/fw_if/umac_if/src/radio_test/fmac_api.c
  ${NRF_WIFI_DIR}/fw_if/umac_if/src/fmac_util.c
)

target_sources_ifdef(CONFIG_NRF700X_DATA_TX
  nrf-wifi
  PRIVATE
  ${NRF_WIFI_DIR}/fw_if/umac_if/src/tx.c
  ${NRF_WIFI_DIR}/fw_if/umac_if/src/fmac_peer.c
)

target_sources_ifdef(CONFIG_NRF700X_STA_MODE
  nrf-wifi
  PRIVATE
  ${NRF_WIFI_DIR}/fw_if/umac_if/src/fmac_peer.c
  ${NRF_WIFI_DIR}/fw_if/umac_if/src/fmac_util.c
)

target_sources_ifdef(CONFIG_NRF700X_AP_MODE
  nrf-wifi
  PRIVATE
  ${NRF_WIFI_DIR}/fw_if/umac_if/src/fmac_ap.c
)

# Without WPA supplicant we only support scan
target_sources_ifdef(CONFIG_NRF700X_STA_MODE
  nrf-wifi
  PRIVATE
  ${NRF_WIFI_DIR}/fw_if/umac_if/src/fmac_peer.c
  ${NRF_WIFI_DIR}/fw_if/umac_if/src/fmac_util.c
)

target_compile_definitions_ifdef(CONFIG_NRF_WIFI_LOW_POWER
  nrf-wifi
  PRIVATE
  -DCONFIG_NRF_WIFI_LOW_POWER
)

target_compile_definitions_ifdef(CONFIG_NRF70_RADIO_TEST
  nrf-wifi
  PRIVATE
  -DCONFIG_NRF700X_RADIO_TEST
)

target_compile_definitions(
  nrf-wifi
  PRIVATE
  -DCONFIG_NRF700X_LOG_VERBOSE=${CONFIG_NRF700X_LOG_VERBOSE}
  -DCONFIG_NRF700X_SCAN_ONLY=y
  -DCONFIG_WIFI_NRF700X_LOG_LEVEL=${CONFIG_WIFI_NRF700X_SHIM_LOG_LEVEL}
  -DCONFIG_NRF700X_MAX_TX_TOKENS=10
  -DCONFIG_NRF700X_MAX_TX_AGGREGATION=12
  -DCONFIG_NRF700X_RX_NUM_BUFS=63
  -DCONFIG_NRF700X_RX_MAX_DATA_SIZE=1600
  -DCONFIG_WIFI_MGMT_RAW_SCAN_RESULTS
  -DCONFIG_NRF700X_ANT_GAIN_2G=0
  -DCONFIG_NRF700X_ANT_GAIN_5G_BAND1=0
  -DCONFIG_NRF700X_ANT_GAIN_5G_BAND2=0
  -DCONFIG_NRF700X_ANT_GAIN_5G_BAND3=0
  -DCONFIG_NRF700X_BAND_2G_LOWER_EDGE_BACKOFF_DSSS=0
  -DCONFIG_NRF700X_BAND_2G_LOWER_EDGE_BACKOFF_HT=0
  -DCONFIG_NRF700X_BAND_2G_LOWER_EDGE_BACKOFF_HE=0
  -DCONFIG_NRF700X_BAND_2G_UPPER_EDGE_BACKOFF_DSSS=0
  -DCONFIG_NRF700X_BAND_2G_UPPER_EDGE_BACKOFF_HT=0
  -DCONFIG_NRF700X_BAND_2G_UPPER_EDGE_BACKOFF_HE=0
  -DCONFIG_NRF700X_BAND_UNII_1_LOWER_EDGE_BACKOFF_HT=0
  -DCONFIG_NRF700X_BAND_UNII_1_LOWER_EDGE_BACKOFF_HE=0
  -DCONFIG_NRF700X_BAND_UNII_1_UPPER_EDGE_BACKOFF_HT=0
  -DCONFIG_NRF700X_BAND_UNII_1_UPPER_EDGE_BACKOFF_HE=0
  -DCONFIG_NRF700X_BAND_UNII_2A_LOWER_EDGE_BACKOFF_HT=0
  -DCONFIG_NRF700X_BAND_UNII_2A_LOWER_EDGE_BACKOFF_HE=0
  -DCONFIG_NRF700X_BAND_UNII_2A_UPPER_EDGE_BACKOFF_HT=0
  -DCONFIG_NRF700X_BAND_UNII_2A_UPPER_EDGE_BACKOFF_HE=0
  -DCONFIG_NRF700X_BAND_UNII_2C_LOWER_EDGE_BACKOFF_HT=0
  -DCONFIG_NRF700X_BAND_UNII_2C_LOWER_EDGE_BACKOFF_HE=0
  -DCONFIG_NRF700X_BAND_UNII_2C_UPPER_EDGE_BACKOFF_HT=0
  -DCONFIG_NRF700X_BAND_UNII_2C_UPPER_EDGE_BACKOFF_HE=0
  -DCONFIG_NRF700X_BAND_UNII_3_LOWER_EDGE_BACKOFF_HT=0
  -DCONFIG_NRF700X_BAND_UNII_3_LOWER_EDGE_BACKOFF_HE=0
  -DCONFIG_NRF700X_BAND_UNII_3_UPPER_EDGE_BACKOFF_HT=0
  -DCONFIG_NRF700X_BAND_UNII_3_UPPER_EDGE_BACKOFF_HE=0
  -DCONFIG_NRF700X_BAND_UNII_4_LOWER_EDGE_BACKOFF_HT=0
  -DCONFIG_NRF700X_BAND_UNII_4_LOWER_EDGE_BACKOFF_HE=0
  -DCONFIG_NRF700X_BAND_UNII_4_UPPER_EDGE_BACKOFF_HT=0
  -DCONFIG_NRF700X_BAND_UNII_4_UPPER_EDGE_BACKOFF_HE=0
  -DCONFIG_NRF700X_REG_DOMAIN=\"${CONFIG_NRF70_REG_DOMAIN}\"
  -DCONFIG_NRF700X_TX_MAX_DATA_SIZE=1600
  -DCONFIG_NRF_WIFI_IFACE_MTU=1500
  -DCONFIG_NRF700X_MAX_TX_PENDING_QLEN=48
  -DCONFIG_NRF700X_RPU_PS_IDLE_TIMEOUT_MS=10
  -DCONFIG_NRF700X_PCB_LOSS_2G=0
  -DCONFIG_NRF700X_PCB_LOSS_5G_BAND1=0
  -DCONFIG_NRF700X_PCB_LOSS_5G_BAND2=0
  -DCONFIG_NRF700X_PCB_LOSS_5G_BAND3=0
  -DCONFIG_NRF_WIFI_AP_DEAD_DETECT_TIMEOUT=20
)
