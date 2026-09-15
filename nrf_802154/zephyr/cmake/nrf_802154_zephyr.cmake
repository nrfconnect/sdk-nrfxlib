#
# Copyright (c) 2026 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

function(nrf_802154_zephyr_add_platform_glue platform_dir)
  if(CONFIG_NRF_802154_SL)
    if(CONFIG_SOC_COMPATIBLE_NRF52X OR CONFIG_SOC_COMPATIBLE_NRF53X)
      target_sources(nrf-802154-platform
        PRIVATE
          ${platform_dir}/nrf_802154_hp_timer.c
          ${platform_dir}/nrf_802154_platform_sl_lptimer_zephyr.c
      )
    elseif(CONFIG_SOC_SERIES_NRF54H OR CONFIG_SOC_SERIES_NRF54L)
      target_sources(nrf-802154-platform
        PRIVATE
          ${platform_dir}/nrf_802154_platform_timestamper.c
          ${platform_dir}/nrf_802154_platform_sl_lptimer_grtc.c
          ${platform_dir}/nrf_802154_platform_sl_lptimer_grtc_hw_task.c
      )
    endif()
  endif()
endfunction()

function(nrf_802154_zephyr_apply_interface_defs)
  if(CONFIG_MPSL)
    target_compile_definitions(nrf-802154-platform PUBLIC NRF_802154_VERIFY_PERIPHS_ALLOC_AGAINST_MPSL=1)
  endif()

  target_compile_definitions(zephyr-802154-interface
    INTERFACE
      NRF_802154_ECB_PRIORITY=-1
      NRF_802154_SWI_PRIORITY=1
      NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US=${CONFIG_NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US}
      NRF_802154_CSMA_CA_MIN_BE_DEFAULT=${CONFIG_NRF_802154_CSMA_CA_MIN_BE_DEFAULT}
      NRF_802154_CSMA_CA_MAX_BE_DEFAULT=${CONFIG_NRF_802154_CSMA_CA_MAX_BE_DEFAULT}
      NRF_802154_CSMA_CA_MAX_CSMA_BACKOFFS_DEFAULT=${CONFIG_NRF_802154_CSMA_CA_MAX_CSMA_BACKOFFS_DEFAULT}
  )

  if(CONFIG_NRF_802154_TX_DIAGNOSTIC_MODE)
    target_compile_definitions(zephyr-802154-interface
      INTERFACE
        NRF_802154_TX_DIAGNOSTIC_MODE=1
    )
  endif()

  if(CONFIG_NRF_802154_ACK_TIMEOUT_CUSTOM_US)
    target_compile_definitions(zephyr-802154-interface
      INTERFACE
        NRF_802154_PRECISE_ACK_TIMEOUT_DEFAULT_TIMEOUT=${CONFIG_NRF_802154_ACK_TIMEOUT_CUSTOM_US}
    )
  endif()

  if(CONFIG_NRF_802154_DRV_REINIT_ENABLED)
    target_compile_definitions(zephyr-802154-interface
      INTERFACE
        NRF_802154_DRV_REINIT_ENABLED=1
    )
  endif()
endfunction()
