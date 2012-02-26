add_executable(command command.cc)
target_link_libraries(command Diag IO State Type Parse Unicode Util)