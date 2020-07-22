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

set(BP_MODULE_PATH "")
list(APPEND BP_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../")
set(BP_GENERATED_SOURCE_FILES "")

set(PLATFORM "Auto" CACHE STRING "Platform name")
set(CMAKE_INSTALL_DIR "" CACHE STRING "Install directory")

if (PLATFORM STREQUAL "Auto")
    if (WIN32)
        set(PLATFORM "Windows")
    elseif (UNIX)
        if (APPLE)
            set(PLATFORM "Mac")
        else (APPLE)
            set(PLATFORM "Linux")
        endif (APPLE)
        if (CMAKE_HOST_SYSTEM_NAME MATCHES "Android")
            set(PLATFORM "Termux")
        endif (CMAKE_HOST_SYSTEM_NAME MATCHES "Android")
    endif (WIN32)
endif (PLATFORM STREQUAL "Auto")

if (PLATFORM STREQUAL "Linux")
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release;DebugWithCoverage" CACHE STRING "" FORCE)
else (PLATFORM STREQUAL "Linux")
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
endif (PLATFORM STREQUAL "Linux")

if (NOT CMAKE_BUILD_TYPE IN_LIST CMAKE_CONFIGURATION_TYPES)
    message(FATAL_ERROR "Unsupported build type: ${CMAKE_BUILD_TYPE}")
endif (NOT CMAKE_BUILD_TYPE IN_LIST CMAKE_CONFIGURATION_TYPES)

if (NOT CMAKE_INSTALL_DIR STREQUAL "")
    set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_DIR} CACHE STRING "FUCK YOU MOTHER HOLY FUCK CMAKE!" FORCE)
endif (NOT CMAKE_INSTALL_DIR STREQUAL "")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DBUILD_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DBUILD_RELEASE")
#Remove block when reaching cmake_minimum 1.16
if (MSVC)
    string(REGEX REPLACE "/W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}) # Remove /W3
endif (MSVC)

include("${CMAKE_CURRENT_LIST_DIR}/Platforms/${PLATFORM}.cmake")

#Setup the main target name=name of target mainincdir=main include directory
function(bp_setup_target name mainincdir)
    target_include_directories(${name} PRIVATE ${mainincdir})
    set_target_properties(${name} PROPERTIES CXX_STANDARD 11)
    set_target_properties(${name} PROPERTIES CXX_STANDARD_REQUIRED TRUE)
    if (CMAKE_BUILD_TYPE STREQUAL DebugWithCoverage)
        set_target_properties(${name} PROPERTIES CXX_EXTENSIONS TRUE)
        target_compile_definitions(${name} PRIVATE COVERAGE)
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
            -Wno-unused
            -std=gnu++11)
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
            -Wno-unused
            -std=gnu++11)
        target_link_libraries(${name} PRIVATE gcov supc++)
    endif (CMAKE_BUILD_TYPE STREQUAL DebugWithCoverage)
    bp_target_created(${name})
    source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
                 PREFIX ""
                 FILES ${SOURCES})
    source_group(".generated" FILES ${BP_GENERATED_SOURCE_FILES})
endfunction(bp_setup_target)

include(CheckCXXCompilerFlag)

function(bp_check_and_flag target w)
    if (${w} MATCHES -Wno-self-assign-overloaded)
        if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang" AND ${CMAKE_CXX_COMPILER_VERSION} MATCHES "8.0.1")
            target_compile_options(${target} PRIVATE ${w})
        endif (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang" AND ${CMAKE_CXX_COMPILER_VERSION} MATCHES "8.0.1")
    else (${w} MATCHES -Wno-self-assign-overloaded)
        check_cxx_compiler_flag(${w} tmpvar)
        if (tmpvar)
            target_compile_options(${target} PRIVATE ${w})
        endif (tmpvar)
    endif (${w} MATCHES -Wno-self-assign-overloaded)
endfunction(bp_check_and_flag)

function(__bp_package_module name hasexports packagename)
    get_filename_component(PRJ_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)
    file(RELATIVE_PATH REL_BIN_DIR ${PRJ_ROOT} ${CMAKE_BINARY_DIR})
    install(TARGETS ${name} CONFIGURATIONS Debug DESTINATION ${packagename}/${REL_BIN_DIR}/Debug)
    install(TARGETS ${name} CONFIGURATIONS Release DESTINATION ${packagename}/${REL_BIN_DIR}/Release)
    if (hasexports)
        install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../${name}.cmake DESTINATION ${packagename})
        file(RELATIVE_PATH REL_SRC_DIR ${PRJ_ROOT} ${CMAKE_CURRENT_SOURCE_DIR})
        install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include DESTINATION ${packagename}/${REL_SRC_DIR})
    endif (hasexports)
endfunction(__bp_package_module)

function(__bp_write_module_descriptor name apimacro)
    message("Writing module descriptor \"${name}.cmake\"...")
    get_filename_component(PRJ_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)
    file(RELATIVE_PATH REL_SRC_DIR ${PRJ_ROOT} ${CMAKE_CURRENT_SOURCE_DIR})
    file(RELATIVE_PATH REL_BIN_DIR ${PRJ_ROOT} ${CMAKE_BINARY_DIR})
    file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/../${name}.cmake
        "set(INCLUDE_DIR \"${REL_SRC_DIR}/include\")\n"
        "set(LIB_DEBUG \"${REL_BIN_DIR}/Debug/${BP_LIBRARY_PREFIX}${name}${BP_EXTENSION_LIB}\")\n"
        "set(LIB_RELEASE \"${REL_BIN_DIR}/Release/${BP_LIBRARY_PREFIX}${name}${BP_EXTENSION_LIB}\")\n"
        "set(BIN_DEBUG \"${REL_BIN_DIR}/Debug/${BP_LIBRARY_PREFIX}${name}${BP_EXTENSION_DYNAMIC}\")\n"
        "set(BIN_RELEASE \"${REL_BIN_DIR}/Release/${BP_LIBRARY_PREFIX}${name}${BP_EXTENSION_DYNAMIC}\")\n"
        "set(ROOT \"${REL_SRC_DIR}\")\n"
        "set(API_MACRO \"${apimacro}\")\n"
        "set(DEPENDENCIES \"\")\n"
    )
endfunction(__bp_write_module_descriptor)
