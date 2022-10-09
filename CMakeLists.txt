cmake_minimum_required(VERSION 3.23)
project(polish_notation_for_big_numbers)

set(CMAKE_C_STANDARD 17)

file(GLOB_RECURSE SOURCES "src/*.c")
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
