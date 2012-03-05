add_gtest(Parse_test TokenInputStream.cc)
target_link_libraries(Parse_test Diag IO State Type Parse Unicode Util)
