add_gtest(Parse_test TokenInputStream.cc)
target_link_libraries(Parse_test Diag IO State Type Parse Unicode Util)
add_dependencies(Parse_test IO_test State_test Util_test Unicode_test)
