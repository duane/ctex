add_gtest(Unicode_test ASCII.cc MutableUString.cc UString.cc UTF8.cc)
target_link_libraries(Unicode_test Unicode)