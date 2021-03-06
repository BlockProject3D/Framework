cmake_minimum_required(VERSION 3.10)
project(BPF.Compression)

include("${CMAKE_CURRENT_SOURCE_DIR}/../CMakes/Module.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/../Version.cmake")
include(${CMAKE_CURRENT_SOURCE_DIR}/../CMakes/conan.cmake)

conan_cmake_run(
    REQUIRES zlib/1.2.11@conan/stable
    OPTIONS zlib:shared=False
    BASIC_SETUP CMAKE_TARGETS KEEP_RPATHS
    BUILD missing
)

set(SOURCES
    include/Framework/Compression/ZInflater.hpp
    include/Framework/Compression/ZDeflater.hpp
    src/Framework/Compression/ZInflater.cpp
    src/Framework/Compression/ZDeflater.cpp
)

bp_product_properties(BPF.Compression
    VERSION_MAJOR ${BP_VERSION_MAJOR}
    VERSION_MINOR ${BP_VERSION_MINOR}
    DESCRIPTION "BlockProject 3D Framework"
)

bp_setup_module(BPF.Compression API_MACRO BPF_COMPRESSION_API PACKAGE)
target_link_libraries(BPF.Compression PRIVATE CONAN_PKG::zlib)
