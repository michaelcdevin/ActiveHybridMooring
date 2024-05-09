CMake_minimum_required (VERSION 3.10)
project (ahm)

find_package (MoorDyn REQUIRED)

add_executable (moordyn_fortran_test moordyn_fortran_test.f90)
target_link_libraries (moordyn_fortran_test MoorDyn::MoorDyn-F)