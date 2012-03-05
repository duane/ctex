add_gtest(IO_test BinaryInputStream.cc CodecInputStream.cc Path.cc)
target_link_libraries(IO_test Diag IO Unicode Util)
