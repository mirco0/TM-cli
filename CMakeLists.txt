cmake_minimum_required(VERSION 3.10)
project(TM-cli C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(headers)
include_directories(headers/parser)

file(GLOB_RECURSE SOURCES "sources/*.c")

add_executable(TM-CLI ${SOURCES})
