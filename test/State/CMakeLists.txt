add_gtest(State_test State.cc)
target_link_libraries(State_test Diag IO State Type Unicode Util)
add_dependencies(State_test Util_test Unicode_test)
