#
# Copyright (c) 2026 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

if(NOT CONFIG_WIFI_NRF71_PATCH)
  return()
endif()

if(CONFIG_WIFI_NRF71_PATCH_MANIFEST_FILE STREQUAL "")
  message(WARNING
    "No WIFI_NRF71_PATCH_MANIFEST_FILE default for the selected "
    "WIFI_NRF71_PATCH_VARIANT"
  )
  return()
endif()

# Paths and tools
set(manifest_file ${CONFIG_WIFI_NRF71_PATCH_MANIFEST_FILE})
get_filename_component(fw_dir ${manifest_file} DIRECTORY)
set(merge_script ${ZEPHYR_BASE}/scripts/build/mergehex.py)
set(zephyr_dir ${APPLICATION_BINARY_DIR}/zephyr)

function(nrf7120_wifi_patch_partition_end nodelabel output_var)
  dt_nodelabel(path NODELABEL ${nodelabel} QUIET)
  if(NOT path)
    set(${output_var} 0 PARENT_SCOPE)
    return()
  endif()

  dt_reg_addr(start PATH ${path})
  dt_reg_size(size PATH ${path})
  math(EXPR end "${start} + ${size}")
  set(${output_var} ${end} PARENT_SCOPE)
endfunction()

# Active application slot bounds from devicetree
dt_chosen(code_partition PROPERTY "zephyr,code-partition")
dt_reg_addr(patch_region_start PATH ${code_partition})
dt_reg_size(patch_region_size PATH ${code_partition})
math(EXPR patch_region_end "${patch_region_start} + ${patch_region_size}")

# Primary slot ends from devicetree (not hardcoded constants).
nrf7120_wifi_patch_partition_end(slot0_partition slot0_end)
nrf7120_wifi_patch_partition_end(slot0_ns_partition slot0_ns_end)

if(CONFIG_WIFI_NRF71_PATCH_AUTO)
  set(auto_slot_label "")
  set(auto_min_distance -1)

  foreach(slot_label IN ITEMS slot0_partition slot0_ns_partition)
    if(${slot_label} STREQUAL "slot0_partition")
      set(slot_end ${slot0_end})
    else()
      set(slot_end ${slot0_ns_end})
    endif()

    if(slot_end EQUAL 0)
      continue()
    endif()

    math(EXPR delta "${patch_region_end} - ${slot_end}")
    if(delta LESS 0)
      math(EXPR distance "0 - ${delta}")
    else()
      set(distance ${delta})
    endif()

    if(auto_min_distance LESS 0 OR distance LESS auto_min_distance)
      set(auto_min_distance ${distance})
      set(auto_slot_label ${slot_label})
    endif()
  endforeach()

  if(auto_slot_label STREQUAL "")
    message(STATUS
      "Wi-Fi patch: auto mode, code partition ends at ${patch_region_end} "
      "(no slot0 or slot0_ns node in devicetree)"
    )
  else()
    if(auto_slot_label STREQUAL "slot0_partition")
      set(auto_slot_end ${slot0_end})
    else()
      set(auto_slot_end ${slot0_ns_end})
    endif()

    if(auto_min_distance GREATER 0)
      message(WARNING
        "Wi-Fi patch: code partition end ${patch_region_end} does not match "
        "${auto_slot_label} end ${auto_slot_end}; closest distance "
        "${auto_min_distance} bytes"
      )
    else()
      message(STATUS "Wi-Fi patch: auto-selected ${auto_slot_label}")
    endif()
  endif()
endif()

# Manifest parsing
if(NOT EXISTS ${manifest_file})
  message(WARNING "Missing nRF7120 Wi-Fi patch manifest: ${manifest_file}")
  return()
endif()

file(READ ${manifest_file} manifest)
string(JSON patch_count LENGTH "${manifest}" "patches")
if(patch_count EQUAL 0)
  message(WARNING "Manifest does not define a Wi-Fi patch")
  return()
endif()

# Patch validation against Kconfig expectations
set(patches)
set(kconfig_origins lmac ${CONFIG_WIFI_NRF71_PATCH_LMAC_ORIGIN} umac ${CONFIG_WIFI_NRF71_PATCH_UMAC_ORIGIN})
set(kconfig_sizes lmac ${CONFIG_WIFI_NRF71_PATCH_LMAC_SIZE} umac ${CONFIG_WIFI_NRF71_PATCH_UMAC_SIZE})

math(EXPR last_idx "${patch_count} - 1")
foreach(idx RANGE ${last_idx})
  string(JSON name GET "${manifest}" "patches" ${idx} "name")
  string(JSON file GET "${manifest}" "patches" ${idx} "file")
  string(JSON manifest_origin GET "${manifest}" "patches" ${idx} "origin")
  string(JSON manifest_size GET "${manifest}" "patches" ${idx} "size")

  list(FIND kconfig_origins ${name} origin_idx)
  if(origin_idx LESS 0)
    message(WARNING "Manifest defines unknown Wi-Fi patch '${name}'")
    return()
  endif()

  math(EXPR origin_idx "${origin_idx} + 1")
  list(GET kconfig_origins ${origin_idx} kconfig_origin)
  math(EXPR kconfig_origin_addr "${kconfig_origin}")

  list(FIND kconfig_sizes ${name} size_idx)
  math(EXPR size_idx "${size_idx} + 1")
  list(GET kconfig_sizes ${size_idx} kconfig_size)

  math(EXPR manifest_origin_addr "${manifest_origin}")
  math(EXPR manifest_size_int "${manifest_size}")

  if(NOT manifest_origin_addr EQUAL kconfig_origin_addr)
    message(WARNING
      "${name} patch origin mismatch: manifest=${manifest_origin}, "
      "Kconfig=${kconfig_origin}"
    )
    return()
  endif()

  if(NOT manifest_size_int EQUAL kconfig_size)
    message(WARNING
      "${name} patch size mismatch: manifest=${manifest_size}, "
      "Kconfig=${kconfig_size}"
    )
    return()
  endif()

  if(manifest_size_int LESS_EQUAL 0)
    message(WARNING "${name} patch size must be greater than zero")
    return()
  endif()

  set(bin ${fw_dir}/${file})
  if(NOT IS_ABSOLUTE ${bin})
    set(bin ${fw_dir}/${file})
  endif()

  if(NOT EXISTS ${bin})
    message(WARNING "${name} patch binary not found: ${bin}")
    return()
  endif()

  file(SIZE ${bin} bin_size)
  if(NOT bin_size EQUAL manifest_size_int)
    message(WARNING
      "${name} patch binary size mismatch: manifest=${manifest_size_int}, "
      "file=${bin_size}"
    )
    return()
  endif()

  math(EXPR addr "${manifest_origin}")
  math(EXPR end "${addr} + ${manifest_size_int}")

  if(addr LESS patch_region_start OR end GREATER patch_region_end)
    message(WARNING
      "${name} patch [${manifest_origin}, ${end}) is outside code partition "
      "[${patch_region_start}, ${patch_region_end})"
    )
    return()
  endif()

  set(${name}_bin ${bin})
  set(${name}_origin ${manifest_origin})
  set(${name}_addr ${addr})
  set(${name}_size ${manifest_size_int})
  list(APPEND patches ${name})
endforeach()

# Linker reservations
if(NOT CONFIG_CMAKE_LINKER_GENERATOR)
  set(linker_dir ${CMAKE_CURRENT_BINARY_DIR}/linker)
  file(MAKE_DIRECTORY ${linker_dir})
endif()

foreach(name IN LISTS patches)
  if(CONFIG_CMAKE_LINKER_GENERATOR)
    zephyr_linker_section(
      NAME .wifi_${name}_patch
      VMA FLASH
      ADDRESS ${${name}_addr}
      TYPE NOLOAD
      NOINPUT
      MIN_SIZE ${${name}_size}
    )
  else()
    set(section_name wifi_${name}_patch)
    set(patch_origin ${${name}_addr})
    set(patch_size ${${name}_size})
    configure_file(
      ${CMAKE_CURRENT_LIST_DIR}/wifi_patch_section.ld.in
      ${linker_dir}/wifi_${name}_patch.ld
      @ONLY
    )
    zephyr_linker_sources(SECTIONS ${linker_dir}/wifi_${name}_patch.ld)
  endif()
endforeach()

# Post-build merge
set(merged_prefix ${zephyr_dir}/${KERNEL_NAME}.nrf7120)
set(merged_hex ${merged_prefix}.hex)
set(merged_bin ${merged_prefix}.bin)
set(merge_byproducts ${merged_hex} ${merged_bin})

set(merge_inputs ${zephyr_dir}/${KERNEL_HEX_NAME})
if(CONFIG_BUILD_WITH_TFM)
  set(merge_inputs "$<TARGET_PROPERTY:tfm,TFM_S_HEX_FILE>")
  if(CONFIG_TFM_USE_NS_APP)
    list(APPEND merge_inputs "$<TARGET_PROPERTY:tfm,TFM_NS_HEX_FILE>")
  else()
    list(APPEND merge_inputs ${zephyr_dir}/${KERNEL_HEX_NAME})
  endif()
endif()

set(merge_command)
set(merge_depends ${merge_inputs})
set(patch_hexes)
foreach(name IN LISTS patches)
  set(patch_hex ${zephyr_dir}/${name}.hex)
  list(APPEND merge_command
    COMMAND ${CMAKE_OBJCOPY}
      --input-target=binary
      --output-target=ihex
      --change-addresses ${${name}_origin}
      ${${name}_bin}
      ${patch_hex}
  )
  list(APPEND patch_hexes ${patch_hex})
  list(APPEND merge_byproducts ${patch_hex})
  list(APPEND merge_depends ${${name}_bin})
  set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${${name}_bin})
endforeach()

list(APPEND merge_command
  COMMAND ${PYTHON_EXECUTABLE}
    ${merge_script}
    -o ${merged_hex}
    ${merge_inputs}
    ${patch_hexes}
  COMMAND ${CMAKE_OBJCOPY}
    --input-target=ihex
    --output-target=binary
    --gap-fill 0xFF
    ${merged_hex}
    ${merged_bin}
)

if(CONFIG_BOOTLOADER_MCUBOOT)
  set_property(GLOBAL APPEND PROPERTY extra_post_build_commands ${merge_command})
  set_property(GLOBAL APPEND PROPERTY extra_post_build_byproducts ${merge_byproducts})
else()
  add_custom_command(
    OUTPUT ${merge_byproducts}
    DEPENDS ${merge_depends}
    ${merge_command}
    COMMENT "Generating nRF7120 merged hex: ${merged_hex}"
    COMMAND_EXPAND_LISTS
    VERBATIM
  )
  add_custom_target(nrf71_wifi_patch_merge ALL DEPENDS ${merge_byproducts})
endif()

if(NOT CONFIG_BOOTLOADER_MCUBOOT)
  set_target_properties(runners_yaml_props_target PROPERTIES
    hex_file ${merged_hex}
    bin_file ${merged_bin}
  )
endif()

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
  ${manifest_file}
  ${merge_script}
)
