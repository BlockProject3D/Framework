set(BP_FRAMEWORK_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/../")
set(BP_BASICS_CMAKE_SELF ${CMAKE_CURRENT_LIST_DIR})
set(PLATFORM "Auto" CACHE STRING "Platform name")
option(RELEASE "Enable release build" OFF)

string(TOUPPER "${CMAKE_PROJECT_NAME}_API" BP_API_MACRO)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

set(BP_DEFINITIONS "")
set(BP_INCLUDES "")
set(BP_MODULES "")

if (PLATFORM STREQUAL "Auto")
    if (WIN32)
        set(PLATFORM "Windows")
    elseif (UNIX)
        if (APPLE)
            set(PLATFORM "Mac")
        else (APPLE)
            set(PLATFORM "Linux")
        endif (APPLE)
    endif (WIN32)
endif (PLATFORM STREQUAL "Auto")

macro(bp_add_definition name)
    list(APPEND BP_DEFINITIONS ${name})
endmacro(bp_add_definition)

macro(bp_add_include_dir name)
    list(APPEND BP_INCLUDES ${name})
endmacro(bp_add_include_dir)

if (RELEASE)
    set(CMAKE_BUILD_TYPE Release)
else (RELEASE)
    set(CMAKE_BUILD_TYPE Debug)
endif (RELEASE)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DBUILD_DEBUG")

include("${BP_BASICS_CMAKE_SELF}/Platforms/${PLATFORM}.cmake")

macro(bp_setup_target name mainincdir)
    set(BP_NAME ${name})
    target_include_directories(${name} PUBLIC ${mainincdir})
    target_include_directories(${name} PRIVATE ${BP_INCLUDES})
    target_compile_definitions(${name} PUBLIC ${BP_DEFINITIONS})
    target_link_libraries(${name} PUBLIC ${BP_MODULES})
    set_property(TARGET ${name} PROPERTY CXX_STANDARD 11)
    bp_target_created(${name})
    source_group(TREE "${CMAKE_SOURCE_DIR}"
                 PREFIX ""
                 FILES ${SOURCES})
endmacro(bp_setup_target)
