# disable RTTI.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTEST_HAS_RTTI=0")
add_subdirectory(gtest)

include_directories(gtest/include)

macro(add_gtest TEST_NAME)
  add_executable(${TEST_NAME} ${ARGN})
  target_link_libraries(${TEST_NAME} gtest gtest_main)
  add_test(${TEST_NAME} ${TEST_NAME})
endmacro(add_gtest)

add_subdirectory(Util)
add_subdirectory(Unicode)
