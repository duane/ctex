cmake_minimum_required (VERSION 2.8) 
project (tex) 

# C flags
set(CMAKE_CXX_FLAGS
    "-g -O0 -Wall -Wextra -Werror -Wno-unused-parameter -fno-rtti")
include_directories("include")

# configure
add_subdirectory(config)
include_directories(${tex_BINARY_DIR})

find_package(Doxygen)
if(DOXYGEN_FOUND)
  add_custom_target(doc ${DOXYGEN_EXECUTABLE} doc/Doxyfile COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif(DOXYGEN_FOUND)

add_subdirectory(libs)
add_subdirectory(tools)

add_subdirectory(test EXCLUDE_FROM_ALL)

