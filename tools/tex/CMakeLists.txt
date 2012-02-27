add_executable(tex tex.cc)
target_link_libraries(tex Diag IO Output Parse Render State Type Unicode Util)