cmake_minimum_required(VERSION 3.10)
project(BPF.Tests.Console)

include("${CMAKE_CURRENT_SOURCE_DIR}/../CMakes/Program.cmake")

set(SOURCES
    src/main.cpp
    src/LowLevelMain.cpp
)

bp_setup_program(${PROJECT_NAME})
