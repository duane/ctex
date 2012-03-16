# disable RTTI.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTEST_HAS_RTTI=0")
add_subdirectory(gtest)
include_directories(gtest/include)
add_subdirectory(gtest_driver)

add_custom_target(test)
add_custom_target(check)

macro(add_gtest TEST_NAME)
  add_executable(${TEST_NAME} ${ARGN})
  target_link_libraries(${TEST_NAME} gtest ctex_gtest_main)
  add_dependencies(test ${TEST_NAME})
  add_custom_target(check_${TEST_NAME}
                    COMMAND ${TEST_NAME})
  add_dependencies(check check_${TEST_NAME})
endmacro(add_gtest)

add_subdirectory(IO)
add_subdirectory(State)
add_subdirectory(Parse)
add_subdirectory(Util)
add_subdirectory(Unicode)
