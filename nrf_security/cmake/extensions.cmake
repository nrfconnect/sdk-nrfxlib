#
# Copyright (c) 2021-2022 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

#
# This macro creates a variable for a base input if CONFIG_<base> is set
#
macro(kconfig_check_and_set_base base)
  if (CONFIG_${base})
    nrf_security_debug("Setting ${base} to TRUE")
    set(${base} TRUE)
  endif()
endmacro()


#
# This macro creates a variable with a value for a base input if CONFIG_<base> is set
#
macro(kconfig_check_and_set_base_val base val)
  if (CONFIG_${base})
    nrf_security_debug("Setting ${base} ${val}")
    set(${base} ${val})
  endif()
endmacro()

#
# This macro sets the value of base input to 1 if CONFIG_<base> is set
#
macro(kconfig_check_and_set_base_to_one base)
  kconfig_check_and_set_base_val(${base} 1)
endmacro()

#
# Internal macro which will create a variable base if dependent
# Kconfig variables in the ARGN list (stripped for CONFIG_ is given)
#
macro(kconfig_check_and_set_base_depends base)
  set(${base} TRUE)
  foreach(arg ${ARGN})
    if (NOT CONFIG_${arg})
      nrf_security_debug("Unsetting ${base} because ${arg} is not set")
      unset(${base})
      break()
    endif()
  endforeach()
endmacro()

#
# Internal macro to configure file if Kconfig config is set
#
# This needs some work
#
macro(nrf_security_configure_file config location file)
  if (${mbedtls_config})
    nrf_security_debug("Configure file: ${file}")
    get_filename_component(file_name ${file} NAME)
    configure_file(${file} ${location}/${file_name} COPYONLY)
  endif()
endmacro()

#
# Add common configurations/options from the zephyr interface libraries
#
# This includes
# Compile options
# Standard includes
# C flags/Linker flags
#
macro(nrf_security_add_zephyr_options lib_name)
  if(TARGET zephyr_interface)
    # Add compile options and includes from zephyr
    target_compile_options(${lib_name} PRIVATE $<TARGET_PROPERTY:zephyr_interface,INTERFACE_COMPILE_OPTIONS>)
    target_include_directories(${lib_name} PRIVATE $<TARGET_PROPERTY:zephyr_interface,INTERFACE_INCLUDE_DIRECTORIES>)
    target_include_directories(${lib_name} PRIVATE $<TARGET_PROPERTY:zephyr_interface,INTERFACE_SYSTEM_INCLUDE_DIRECTORIES>)

    # Unsure if these are needed any more
    target_compile_options(${lib_name} PRIVATE ${TOOLCHAIN_C_FLAGS})
    target_ld_options(${lib_name} PRIVATE ${TOOLCHAIN_LD_FLAGS})
  else()
    target_compile_options(${lib_name} PRIVATE "SHELL: -imacros ${ZEPHYR_AUTOCONF}")
    target_include_directories(${lib_name} PRIVATE
      $<$<TARGET_EXISTS:platform_cc3xx>:$<TARGET_PROPERTY:platform_cc3xx,INTERFACE_INCLUDE_DIRECTORIES>>
    )
  endif()
endmacro()
