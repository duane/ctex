add_gtest(IO_test BinaryInputStream.cc CodecInputStream.cc)
target_link_libraries(IO_test Diag IO Unicode Util)
add_dependencies(IO_test Util_test Unicode_test)
add_dependencies(check_IO_test check_Util_test check_Unicode_test)