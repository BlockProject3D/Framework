#Utility function used to prepend a list with a prefix
function(bp_prepend lst prefix)
    set(listVar "")
    foreach(f ${ARGN})
        list(APPEND listVar "${prefix}/${f}")
    endforeach(f)
    set(${lst} "${listVar}" PARENT_SCOPE)
endfunction(bp_prepend)

#Utility function used to fix list of sources
#Will reject all sources in order to only keep headers and will add missing quotes to paths
function(bp_fixheaderlist lst)
    set(listVar "")
    foreach(f ${ARGN})
        if (NOT ${f} MATCHES ".cpp$"
            AND NOT ${f} MATCHES ".cxx$"
            AND NOT ${f} MATCHES ".cc$"
            AND NOT ${f} MATCHES ".c$")
            list(APPEND listVar "\"${f}\"")
        endif ()
    endforeach(f)
    set(${lst} "${listVar}" PARENT_SCOPE)
endfunction(bp_fixheaderlist)

set(BP_FRAMEWORK_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/../")
set(BP_BASICS_CMAKE_SELF ${CMAKE_CURRENT_LIST_DIR})
set(PLATFORM "Auto" CACHE STRING "Platform name")
option(RELEASE "Enable release build" OFF)
option(COVERAGE "Enable coverage" OFF)

string(TOUPPER "${CMAKE_PROJECT_NAME}_API" BP_API_MACRO)

if (CMAKE_COMPILER_IS_GNUCC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif (CMAKE_COMPILER_IS_GNUCC)

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
endif (MSVC)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

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

#Setup the main target name=name of target mainincdir=main include directory
macro(bp_setup_target name mainincdir)
    #set(BP_NAME ${name})
    target_include_directories(${name} PRIVATE ${mainincdir})
    #target_include_directories(${name} PRIVATE ${BP_INCLUDES})
    target_compile_definitions(${name} PRIVATE ${BP_PLATFORM_DEF})
    if (COVERAGE)
        target_compile_options(${name}
            PRIVATE
            -g -O0
            --coverage
            -fno-inline
            -fno-inline-small-functions
            -fno-default-inline
            -fprofile-arcs
            -ftest-coverage
            -fkeep-inline-functions
            -Wno-unused)
        target_link_libraries(${name}
            PRIVATE
            -g -O0
            --coverage
            -fno-inline
            -fno-inline-small-functions
            -fno-default-inline
            -fprofile-arcs
            -ftest-coverage
            -fkeep-inline-functions
            -Wno-unused)
        target_link_libraries(${name} PRIVATE gcov supc++)
    endif (COVERAGE)
    #target_link_libraries(${name} PRIVATE ${BP_MODULES})
    set_property(TARGET ${name} PROPERTY CXX_STANDARD 11)
    bp_target_created(${name})
    source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
                 PREFIX ""
                 FILES ${SOURCES})
endmacro(bp_setup_target)
