cmake_minimum_required(VERSION 3.10)
project(BPF.Compression)

include("${CMAKE_CURRENT_SOURCE_DIR}/../CMakes/Framework.cmake")

include("${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.in.txt")

set(SOURCES
    include/Framework/Compression/ZInflater.hpp
    include/Framework/Compression/ZDeflater.hpp
    src/Framework/Compression/ZInflater.cpp
    src/Framework/Compression/ZDeflater.cpp
)

bp_setup_module(BPF.Compression BPF_COMPRESSION_API)
target_link_libraries(BPF.Compression PRIVATE CONAN_PKG::zlib)
