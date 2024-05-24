CMake_minimum_required (VERSION 3.10)
project (ahm)

find_package (MoorDyn REQUIRED)

add_executable (moordyn_cpp_test moordyn_cpp_test.cpp)
target_link_libraries (moordyn_cpp_test MoorDyn::moordyn)

add_executable (ahm_3dof ahm_3dof.cpp)
target_link_libraries (ahm_3dof MoorDyn::moordyn)

add_executable (ahm_6dof ahm_6dof.cpp)
target_link_libraries (ahm_6dof MoorDyn::moordyn)