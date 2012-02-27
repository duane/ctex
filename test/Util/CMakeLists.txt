add_gtest(Util_test ByteBuffer.cc HashMap.cc SmallIntMap.cc SmallVector.cc UniquePtr.cc)
target_link_libraries(Util_test Diag Util)