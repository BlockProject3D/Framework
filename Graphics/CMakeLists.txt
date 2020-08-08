cmake_minimum_required(VERSION 3.10)
project(BPF.Graphics)

include("${CMAKE_CURRENT_SOURCE_DIR}/../CMakes/Module.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/../Version.cmake")

set(SOURCES
    include/Framework/Graphics/PixelArray.hpp
    include/Framework/Graphics/Graphics2D.hpp
    src/Framework/Graphics/Graphics2D.cpp
)

bp_product_properties(BPF.Compression
    VERSION_MAJOR ${BP_VERSION_MAJOR}
    VERSION_MINOR ${BP_VERSION_MINOR}
    DESCRIPTION "BlockProject 3D Framework"
)

bp_setup_module(BPF.Graphics API_MACRO BPF_GRAPHICS_API PACKAGE)
