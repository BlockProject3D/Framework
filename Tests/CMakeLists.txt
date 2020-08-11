cmake_minimum_required(VERSION 3.10)
project(BPF.Tests)

include("${CMAKE_CURRENT_SOURCE_DIR}/../CMakes/Program.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/../Version.cmake")

configure_file(CMakeLists.in.txt googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif (result)
execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif (result)

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

include_directories("${gtest_SOURCE_DIR}/include")

set(SOURCES
    src/IO/File.cpp
    src/IO/MemoryMapper.cpp
    src/IO/FileStream.cpp
    src/IO/ByteBuf.cpp
    src/IO/BinaryReadWrite.cpp
    src/IO/TextReadWrite.cpp
    src/IO/Console.cpp
    src/IO/ConsoleWriter.cpp
    src/Math/MatrixDynamic.cpp
    src/Math/MatrixStatic.cpp
    src/Math/Quaternion.cpp
    src/Math/BoundingBox.cpp
    src/Math/Transform3.cpp
    src/Math/Transform2.cpp
    src/Math/Polygon3.cpp
    src/Math/Polygon2.cpp
    src/Math/VectorStatic.cpp
    src/Math/VectorDynamic.cpp
    src/Math/Math.cpp
    src/Math/Color.cpp
    src/Log/Logger.cpp
    src/Collection/List.cpp
    src/Collection/ArrayList.cpp
    src/Collection/HashMap.cpp
    src/Collection/Map.cpp
    src/Collection/ArrayDynamic.cpp
    src/Collection/ArrayStatic.cpp
    src/Collection/Stack.cpp
    src/Collection/Queue.cpp
    src/Collection/PriorityQueue.cpp
    src/System/DateTime.cpp
    src/System/TimeSpan.cpp
    src/System/Platform.cpp
    src/System/Timer.cpp
    src/System/Thread.cpp
    src/System/Process.cpp
    src/System/Plugins.cpp
    src/System/ThreadPool.cpp
    src/System/Application.cpp
    src/Json/Json.cpp
    src/Json/JsonWriter.cpp
    src/Memory/ObjectConstructor.cpp
    src/Memory/Casting.cpp
    src/BaseConvert.cpp
    src/Compression.cpp
    src/Tuple.cpp
    src/Dynamic.cpp
    src/Name.cpp
    src/String.cpp
    src/MathEval.cpp
    src/Scalar.cpp
    src/RuntimeException.cpp
    src/Delegate.cpp
    src/Event.cpp
    src/main.cpp
    src/LowLevelMain.cpp
)

bp_setup_program(${PROJECT_NAME})
target_compile_definitions(${PROJECT_NAME} PRIVATE "BP_VERSION_STRING=\"${BP_VERSION_STRING}\"")

bp_use_module(${PROJECT_NAME} BPF.Compression)
target_link_libraries(${PROJECT_NAME} PRIVATE gtest_main)