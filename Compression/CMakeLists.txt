cmake_minimum_required(VERSION 3.10)
project(BPF.Compression)

include("${CMAKE_CURRENT_SOURCE_DIR}/../CMakes/Framework.cmake")

include("${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.in.txt")

set(SOURCES
    include/Framework/Compression/IInflater.hpp
    include/Framework/Compression/IDeflater.hpp
    include/Framework/Compression/ZInflater.hpp
    include/Framework/Compression/ZDeflater.hpp
    src/Framework/Compression/ZInflater.cpp
    src/Framework/Compression/ZDeflater.cpp
)

bp_setup_module(BPF.Compression BPF_COMPRESSION_API)
add_dependencies(BPF.Compression staticzlib)
find_library(ZLIB_DBG zlibstatic ${CMAKE_BINARY_DIR}/zlib/Debug)
find_library(ZLIB_REL zlibstatic ${CMAKE_BINARY_DIR}/zlib/Release)
target_include_directories(BPF.Compression PRIVATE ${ZLIB_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR2})
target_link_libraries(BPF.Compression PRIVATE debug ${ZLIB_DBG})
target_link_libraries(BPF.Compression PRIVATE optimized ${ZLIB_REL})
