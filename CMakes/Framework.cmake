include("${CMAKE_CURRENT_LIST_DIR}/Base.cmake")

function(bp_setup_module name)
    set(props
        API_MACRO
    )
    set(options
        PACKAGE
    )
    set(multiValueArgs)
    cmake_parse_arguments(MODULE_INFO "${options}" "${props}" "${multiValueArgs}" ${ARGN})

    if (NOT BP_ADDITIONAL_SOURCE_FILE)
        set(BP_ADDITIONAL_SOURCE_FILE "")
    endif(NOT BP_ADDITIONAL_SOURCE_FILE)

    add_library(${name} SHARED ${SOURCES} ${BP_ADDITIONAL_SOURCE_FILE})

    if (MODULE_INFO_API_MACRO)
        target_compile_definitions(${name} PRIVATE "${MODULE_INFO_API_MACRO}=${BP_SYMBOL_EXPORT_MACRO}")
        bp_write_module_descriptor(${name} ${MODULE_INFO_API_MACRO})
    endif (MODULE_INFO_API_MACRO)
    # Attempt at fixing templates problems under MSVC 2017
    target_compile_definitions(${name} PRIVATE "BP_TPL_API=${BP_SYMBOL_EXPORT_MACRO}")

    if (MSVC)
        target_compile_options(${name} PRIVATE /W4)
    else (MSVC)
        target_compile_options(${name} PRIVATE -Wall)
    endif (MSVC)
    target_compile_options(${name} PRIVATE "$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    target_compile_options(${name} PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

    if (NOT ${name} STREQUAL "BPF")
        bp_add_module(${name} "BPF")
    endif (NOT ${name} STREQUAL "BPF")

    bp_setup_target(${name} include ${SOURCES})

    if (MODULE_INFO_PACKAGE)
        if (MODULE_INFO_API_MACRO)
            bp_package_module(${name} true)
        else (MODULE_INFO_API_MACRO)
            bp_package_module(${name} false)
        endif (MODULE_INFO_API_MACRO)
    endif (MODULE_INFO_PACKAGE)
endfunction(bp_setup_module)

function(bp_add_module targetname modulename)
    set(MD_DESC "")
    set(MD_PATH "")
    foreach (PATH ${BP_MODULE_PATH})
        if (EXISTS "${PATH}/${modulename}.cmake")
            set(MD_DESC "${PATH}/${modulename}.cmake")
            set(MD_PATH "${PATH}")
        endif (EXISTS "${PATH}/${modulename}.cmake")
    endforeach (PATH ${BP_MODULE_PATH})
    if (MD_DESC STREQUAL "")
        message(FATAL_ERROR "Module ${modulename} could not be found in module path; did you forget to update BP_MODULE_PATH?")
    endif (MD_DESC STREQUAL "")
    include(${MD_DESC})

    # Add definitions
    target_compile_definitions(${targetname} PRIVATE "${API_MACRO}=${BP_SYMBOL_IMPORT_MACRO}")

    # Add includes and libs
    target_include_directories(${targetname} PRIVATE "${MD_PATH}/${INCLUDE_DIR}")

    # Handle linkage: identify what kind of Framework we are working with (pre-build vs source build) then link correct lib
    # This if block is designed to allow correct behaviour for Makefile and Ninja
    if (EXISTS "${MD_PATH}/${ROOT}/CMakeLists.txt") # We are a source build
        target_link_libraries(${targetname} PRIVATE debug ${modulename})
        target_link_libraries(${targetname} PRIVATE optimized ${modulename})
    else (EXISTS "${MD_PATH}/${ROOT}/CMakeLists.txt") # We are a pre-build
        target_link_libraries(${targetname} PRIVATE debug "${MD_PATH}/${BIN_DEBUG}")
        target_link_libraries(${targetname} PRIVATE optimized "${MD_PATH}/${BIN_RELEASE}")
    endif (EXISTS "${MD_PATH}/${ROOT}/CMakeLists.txt")
endfunction(bp_add_module)
