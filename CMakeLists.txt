cmake_minimum_required(VERSION 3.23)
project(polish_notation_for_big_numbers)

set(CMAKE_C_STANDARD 17)

file(GLOB_RECURSE SOURCES "src/*.c")
file(GLOB_RECURSE TEST_SOURCES "test/*.c")

add_executable(${PROJECT_NAME} ${SOURCES} ./main.c)
target_include_directories(${PROJECT_NAME} PUBLIC include)

add_executable(${PROJECT_NAME}-test ${SOURCES} ${TEST_SOURCES})
target_include_directories(${PROJECT_NAME}-test PUBLIC include)
