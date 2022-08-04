cmake_minimum_required(VERSION 3.12)
project(test01 C)

set(CMAKE_C_STANDARD 99)

add_compile_options(-Wall -Os -s)

file(GLOB_RECURSE all_source_file src/*.c)

foreach(file ${all_source_file})
    message(STATUS "Adding source file ${file} to project.")
endforeach()

add_executable(test01 ${all_source_file})
