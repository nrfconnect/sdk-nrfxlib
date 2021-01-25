#
# Copyright (c) 2020 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

# Define a separate interface library for Nordic's extensions to ZBOSS stack
zephyr_interface_library_named(zboss-extensions)

# Common includes, not related to ZBOSS source code.
target_include_directories(zboss-extensions INTERFACE
  src/zb_error
  include/addons
)

# Suppress bitfield compatibility warning.
# This has to be included in all files, that uses ZBOSS headers.
# Those options will be inherited by the ZBOSS target as well as everything
# that links to it.
target_compile_options(zboss-extensions INTERFACE
  -Wno-packed-bitfield-compat
)

# Add compile-time definition, indicating which ZBOSS library will be linked
# This definition adjusts ZBOSS headers, by making non-applicable API for
# a given Zigbee role invisible.
# Those options will be inherited by the ZBOSS target as well as everything
# that links to it.
if (CONFIG_ZIGBEE_ROLE_END_DEVICE)
  target_compile_definitions(zboss-extensions INTERFACE
    ZB_ED_ROLE
  )
endif()


# Create a library for Nordic's extensions to ZBOSS stack.
# The target name is generated based on the path and is not meant to be
# explicitly linked in other modules.
# It will be pulled if the module links to the ZBOSS interface library.
zephyr_library()

# Add source files
zephyr_library_sources(src/zb_error/zb_error_to_string.c)

# Link with ZBOSS extensions interface library, which is linked to the main
# ZBOSS interface library.
zephyr_library_link_libraries(zboss)

# Precompiled libraries -only part.
# Use the CONFIG_ZBOSS_SOURCES_AVAILABLE Kconfig option is defined in ZBOSS
# platform module to check if user uses ZOI repositories for building
# application.
# If so, the user still may select libraries from nrfxlib to link with
# verified state of ZBOSS (CONFIG_ZBOSS_LIBRARY_PRECOMPILED symbol).
if ((NOT DEFINED CONFIG_ZBOSS_SOURCES_AVAILABLE) OR
    (CONFIG_ZBOSS_LIBRARY_PRECOMPILED))

  nrfxlib_calculate_lib_path(lib_path)
  set(ZBOSS_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${lib_path})
  if(NOT EXISTS ${ZBOSS_LIB_PATH})
    message(WARNING "This combination of SoC and floating point ABI is not supported by the ZBOSS lib."
                    "(${ZBOSS_LIB_PATH} doesn't exist.)")
  endif()

  # Define an interface library for Nordic's ZBOSS stack.
  # This library contains official ZBOSS headers, platform-specific configuration
  # and links to Nordic's extensions of ZBOSS API.
  # This target definition is defined in both - nrfxlib and ZBOSS platform for
  # NCS due to the lack of control on the order of Zephyr module inclusion.
  zephyr_interface_library_named(zboss)

  # Extend ZBOSS interface libraries by headers placed in nrfxlib.
  target_include_directories(zboss INTERFACE
    include
    include/zcl
    include/ha
    include/se
    include/osif
  )

  # Resolve Zigbee role to library variant
  if (CONFIG_ZIGBEE_ROLE_COORDINATOR OR CONFIG_ZIGBEE_ROLE_ROUTER)
    set(LIB_VARIANT )
  else()
    set(LIB_VARIANT ".ed")
  endif()

  # Include the static library to the ZBOSS interface definition and select
  # ZBOSS configuration header file by defining compile-time definition.
  target_link_libraries(zboss INTERFACE ${ZBOSS_LIB_PATH}/libzboss${LIB_VARIANT}.a)
  target_compile_definitions(zboss INTERFACE LIBZBOSS_CONFIG_FILE="libzboss_config${LIB_VARIANT}.h")

  # Include the selected subset of Zigbee Green Power functionalities
  if (CONFIG_ZIGBEE_GP_PB)
    target_link_libraries(zboss INTERFACE ${ZBOSS_LIB_PATH}/libgppb${LIB_VARIANT}.a)
  elseif (CONFIG_ZIGBEE_GP_CB)
    target_link_libraries(zboss INTERFACE ${ZBOSS_LIB_PATH}/libgpcb${LIB_VARIANT}.a)
  else()
    message( FATAL_ERROR "Unsupported Zigbee Green Power feature set")
  endif()

  # Include NCP protocol and commands implementation.
  # Since the libncp-dev contains the application code, all object files has to
  # be included while linking.
  if (CONFIG_ZIGBEE_LIBRARY_NCP_DEV)
    zephyr_library_import(zboss_libncp-dev ${ZBOSS_LIB_PATH}/libncp-dev${LIB_VARIANT}.a)
  elseif (CONFIG_ZIGBEE_LIBRARY_SOC)
  else()
    message(FATAL_ERROR "Unsupported Zigbee platform design")
  endif()

  # Link with Nordic's ZBOSS extensions as well as common compile time options
  # and definitions.
  target_link_libraries(zboss INTERFACE zboss-extensions)
endif()
