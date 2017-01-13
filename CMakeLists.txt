cmake_minimum_required(VERSION 3.4)

# === PROJECT INFORMATION ===
project(kplot C)

set(KPLOT_MAJOR_VERSION 0)
set(KPLOT_MINOR_VERSION 1)
set(KPLOT_PATCH_VERSION 15)

set(KPLOT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(KPLOT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(KPLOT_EXAMPLE_DIR ${CMAKE_SOURCE_DIR}/example)

# === DEPENDENCIES ===
find_path(CAIRO_INCLUDE_PATH
  NAMES
    cairo.h
  PATHS
    "${CMAKE_FIND_ROOT_PATH}/include/cairo"
    "C:/usr/include/cairo"
    "/usr/include/cairo"
    "/usr/local/include/cairo"
  )

find_library(CAIRO_LIBRARIES
  NAMES
    cairo
  PATHS
    "${CMAKE_FIND_ROOT_PATH}/lib"
    "C:/usr/lib/cairo"
    "/usr/lib/cairo"
    "/usr/local/lib/cairo"
  )

find_library(MATH_LIBRARIES
  NAMES
    m
  PATHS
    "/lib"
    "/usr/lib"
  )
if(NOT ${MATH_LIBRARIES})
  # When the math libraries can't be found they're usually implicitly linked
  set(MATH_LIBRARIES "")
endif()

include_directories(
  ${KPLOT_INCLUDE_DIR}
  ${CAIRO_INCLUDE_PATH}
  )

# Needs libbsd but for cross-compiling can't do compile-run based tests

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  # Link in libbsd for Linux
  find_library(BSD_LIBRARIES
    NAMES
      bsd
    )
  if(${BSD_LIBRARIES})
    set(HAVE_REALLOCARRAY 1)
  else()
    set(HAVE_REALLOCARRAY 0)
  endif()
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  # Mainly want for mingw cross-compilation
  set(HAVE_REALLOCARRAY 0)
else()
endif()
# TODO Run tests for determining the value of HAVE_REALLOCARRAY

configure_file(
  ${KPLOT_INCLUDE_DIR}/compat.h.in
  ${CMAKE_BINARY_DIR}/include/compat.h
  )

include_directories(${CMAKE_BINARY_DIR}/include)

# === LIBRARY ===
add_library(kplot STATIC
  src/array.c
  src/border.c
  src/bucket.c
  src/buffer.c
  src/colours.c
  src/draw.c
  src/grid.c
  src/hist.c
  src/label.c
  src/kdata.c
  src/kplot.c
  src/margin.c
  src/mean.c
  src/plotctx.c
  src/reallocarray.c
  src/stddev.c
  src/tic.c
  src/vector.c
  )

target_link_libraries(kplot
  ${CAIRO_LIBRARIES}
  ${MATH_LIBRARIES}
  ${BSD_LIBRARIES}
  )

# === EXAMPLES ===
set(EXAMPLES
  example0
  example1
  example2
  example3
  example4
  example5
  example6
  example7
  example8
  example9
  example10
  example11
  example12
  example13
  )

foreach(EXAMPLE IN ITEMS ${EXAMPLES})
  add_executable(${EXAMPLE}
    "${KPLOT_EXAMPLE_DIR}/${EXAMPLE}.c"
    )
  target_link_libraries(${EXAMPLE}
    kplot
    )
  add_custom_command(OUTPUT
      ${EXAMPLE}.png
    COMMAND
      ${EXAMPLE}
    )
  # Add custom commands for generating the pngs
endforeach(EXAMPLE)

# === MAN FILES ===
set(KPLOT_MANPAGES
  ${CMAKE_SOURCE_DIR}/man/kdata_array_alloc.3
  ${CMAKE_SOURCE_DIR}/man/kdata_array_fill.3
  ${CMAKE_SOURCE_DIR}/man/kdata_bucket_alloc.3
  ${CMAKE_SOURCE_DIR}/man/kdata_bucket_add.3
  ${CMAKE_SOURCE_DIR}/man/kdata_buffer_alloc.3
  ${CMAKE_SOURCE_DIR}/man/kdata_buffer_copy.3
  ${CMAKE_SOURCE_DIR}/man/kdata_destroy.3
  ${CMAKE_SOURCE_DIR}/man/kdata_get.3
  ${CMAKE_SOURCE_DIR}/man/kdata_hist_alloc.3
  ${CMAKE_SOURCE_DIR}/man/kdata_hist_add.3
  ${CMAKE_SOURCE_DIR}/man/kdata_mean_alloc.3
  ${CMAKE_SOURCE_DIR}/man/kdata_mean_attach.3
  ${CMAKE_SOURCE_DIR}/man/kdata_pmfmean.3
  ${CMAKE_SOURCE_DIR}/man/kdata_pmfstddev.3
  ${CMAKE_SOURCE_DIR}/man/kdata_pmfvar.3
  ${CMAKE_SOURCE_DIR}/man/kdata_stddev_alloc.3
  ${CMAKE_SOURCE_DIR}/man/kdata_stddev_attach.3
  ${CMAKE_SOURCE_DIR}/man/kdata_vector_append.3
  ${CMAKE_SOURCE_DIR}/man/kdata_vector_alloc.3
  ${CMAKE_SOURCE_DIR}/man/kdata_xmax.3
  ${CMAKE_SOURCE_DIR}/man/kdata_xmean.3
  ${CMAKE_SOURCE_DIR}/man/kdata_xmin.3
  ${CMAKE_SOURCE_DIR}/man/kdata_xstddev.3
  ${CMAKE_SOURCE_DIR}/man/kdatacfg_defaults.3
  ${CMAKE_SOURCE_DIR}/man/kplot.3
  ${CMAKE_SOURCE_DIR}/man/kplot_alloc.3
  ${CMAKE_SOURCE_DIR}/man/kplot_attach_data.3
  ${CMAKE_SOURCE_DIR}/man/kplot_attach_datas.3
  ${CMAKE_SOURCE_DIR}/man/kplot_attach_smooth.3
  ${CMAKE_SOURCE_DIR}/man/kplot_detach.3
  ${CMAKE_SOURCE_DIR}/man/kplot_draw.3
  ${CMAKE_SOURCE_DIR}/man/kplot_free.3
  ${CMAKE_SOURCE_DIR}/man/kplot_get_datacfg.3
  ${CMAKE_SOURCE_DIR}/man/kplot_get_plotcfg.3
  ${CMAKE_SOURCE_DIR}/man/kplotcfg_defaults.3
  )

set(KPLOT_HTMLGENPAGES
  ${CMAKE_BINARY_DIR}/man/kdata_array_alloc.3.html
  ${CMAKE_BINARY_DIR}/man/kdata_array_fill.3.html
  ${CMAKE_BINARY_DIR}/man/kdata_bucket_alloc.3.html
  ${CMAKE_BINARY_DIR}/man/kdata_bucket_add.3.html
  ${CMAKE_BINARY_DIR}/man/kdata_buffer_alloc.3.html
  ${CMAKE_BINARY_DIR}/man/kdata_buffer_copy.3.html
  ${CMAKE_BINARY_DIR}/man/kdata_destroy.3.html
  ${CMAKE_BINARY_DIR}/man/kdata_get.3.html
  ${CMAKE_BINARY_DIR}/man/kdata_hist_alloc.3.html
  ${CMAKE_BINARY_DIR}/man/kdata_hist_add.3.html
  ${CMAKE_BINARY_DIR}/man/kdata_mean_alloc.3.html
  ${CMAKE_BINARY_DIR}/man/kdata_mean_attach.3.html
  ${CMAKE_BINARY_DIR}/man/kdata_pmfmean.3.html
  ${CMAKE_BINARY_DIR}/man/kdata_pmfstddev.3.html
  ${CMAKE_BINARY_DIR}/man/kdata_pmfvar.3.html
  ${CMAKE_BINARY_DIR}/man/kdata_stddev_alloc.3.html
  ${CMAKE_BINARY_DIR}/man/kdata_stddev_attach.3.html
  ${CMAKE_BINARY_DIR}/man/kdata_vector_append.3.html
  ${CMAKE_BINARY_DIR}/man/kdata_vector_alloc.3.html
  ${CMAKE_BINARY_DIR}/man/kdata_xmax.3.html
  ${CMAKE_BINARY_DIR}/man/kdata_xmean.3.html
  ${CMAKE_BINARY_DIR}/man/kdata_xmin.3.html
  ${CMAKE_BINARY_DIR}/man/kdata_xstddev.3.html
  ${CMAKE_BINARY_DIR}/man/kdatacfg_defaults.3.html
  ${CMAKE_BINARY_DIR}/man/kplot.3.html
  ${CMAKE_BINARY_DIR}/man/kplot_alloc.3.html
  ${CMAKE_BINARY_DIR}/man/kplot_attach_data.3.html
  ${CMAKE_BINARY_DIR}/man/kplot_attach_datas.3.html
  ${CMAKE_BINARY_DIR}/man/kplot_attach_smooth.3.html
  ${CMAKE_BINARY_DIR}/man/kplot_detach.3.html
  ${CMAKE_BINARY_DIR}/man/kplot_draw.3.html
  ${CMAKE_BINARY_DIR}/man/kplot_free.3.html
  ${CMAKE_BINARY_DIR}/man/kplot_get_datacfg.3.html
  ${CMAKE_BINARY_DIR}/man/kplot_get_plotcfg.3.html
  ${CMAKE_BINARY_DIR}/man/kplotcfg_defaults.3.html
  ${CMAKE_BINARY_DIR}/mandoc.css
  ${CMAKE_BINARY_DIR}/index.html
  )

# === HTML FILES ===
# TODO perform mandoc HTML generation

find_program(MANDOC NAMES mandoc)
if(${MANDOC})
  add_custom_command(OUTPUT
      ${KPLOT_HTMLGENPAGES}
    COMMAND
      ${MANDOC} -Thtml -Ostyle=${CMAKE_BINARY_DIR}/mandoc.css,man=%N.%S.html .3.3.html
   )
endif(${MANDOC})

# === INSTALL ===
install(TARGETS
    kplot
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  )

install(FILES
    ${KPLOT_INCLUDE_DIR}/kplot.h
    ${CMAKE_BINARY_DIR}/include/compat.h
  DESTINATION
    include
  )

install(FILES
    ${KPLOT_MANPAGES}
  DESTINATION
    man/man3
  )

