CMake_minimum_required (VERSION 3.10)
project (ahm)

find_package (MoorDyn REQUIRED)

add_executable (moordyn_cpp_test moordyn_cpp_test.cpp)
target_link_libraries (moordyn_cpp_test MoorDyn::moordyn)