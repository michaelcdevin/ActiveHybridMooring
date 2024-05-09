CMake_minimum_required (VERSION 3.10)
project (ahm)

find_package (MoorDyn REQUIRED)

add_executable (moordyn_c_test moordyn_c_test.c)
target_link_libraries (moordyn_c_test MoorDyn::moordyn)