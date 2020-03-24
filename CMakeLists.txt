cmake_minimum_required(VERSION 3.10)
project(BlockProject)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory("${CMAKE_SOURCE_DIR}/Base/")
add_subdirectory("${CMAKE_SOURCE_DIR}/Tests/")
add_subdirectory("${CMAKE_SOURCE_DIR}/Tests.Console/")
