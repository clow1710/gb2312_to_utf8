cmake_minimum_required(VERSION 3.12)
project(test01 C)

set(CMAKE_C_STANDARD 99)

add_compile_options(-Wall -Os -s)

file(GLOB_RECURSE SOURCE_CODE_FILE src/*.c)
foreach(file ${SOURCE_CODE_FILE})
    message(STATUS "Adding source code file ${file} to project.")
endforeach()

file(GLOB_RECURSE TEST_CODE_FILE test/*.c)
foreach(file ${TEST_CODE_FILE})
    message(STATUS "Adding test code file ${file} to project.")
endforeach()

add_executable(gb2312_test
        ${SOURCE_CODE_FILE}
        ${TEST_CODE_FILE}
        )
