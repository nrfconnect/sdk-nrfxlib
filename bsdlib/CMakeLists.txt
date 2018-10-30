if(CONFIG_FLOAT)
  if(CONFIG_FP_HARDABI)
    set(float_dir hard-float)
  elseif(CONFIG_FP_SOFTABI)
    set(float_dir softfp-float)
  else()
    assert(0 "Unreachable code")
  endif()
else()
  set(float_dir soft-float)
endif()

if (float_dir STREQUAL "softfp-float")
  assert(0 "softfp-float target not supported.")
endif()

set(BSD_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/${GCC_M_CPU}/${float_dir})

# Link in the bsd library, the oberon library, and lib C. Lib C and
# lib oberon are linked in because they are used by the bsd library
# and must therefore be placed after the bsd library on the linker's
# command line.
zephyr_link_libraries(
  ${BSD_LIB_PATH}/libbsd_nrf9160_xxaa.a
  ${BSD_LIB_PATH}/liboberon_2.0.5.a
  c
  )

zephyr_include_directories(include)
