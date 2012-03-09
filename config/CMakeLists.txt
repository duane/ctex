
# default configuration
if (NOT TEX_FONT_AREA)
  set(TEX_FONT_AREA ${tex_SOURCE_DIR}/support/fonts/)
  message("Font directory: ${TEX_FONT_AREA}")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in
               ${tex_BINARY_DIR}/Config.h)

include_directories(${tex_BINARY_DIR})