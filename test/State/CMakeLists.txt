add_gtest(State_test State.cc)
target_link_libraries(State_test Diag State Unicode Util)
add_dependencies(State_test Util_test Unicode_test)
add_dependencies(check_State_test check_Util_test check_Unicode_test)
