include("${CMAKE_CURRENT_LIST_DIR}/Base.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Framework.cmake")

macro(bp_setup_program name)
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

    if ("${ARGV2}" STREQUAL GUI)
        add_executable(${name} WIN32 MACOSX_BUNDLE ${SOURCES} ${BP_ADDITIONAL_SOURCE_FILE})
    else ("${ARGV2}" STREQUAL GUI)
        add_executable(${name} ${SOURCES} ${BP_ADDITIONAL_SOURCE_FILE})
    endif ("${ARGV2}" STREQUAL GUI)

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

    bp_add_module(${name} "BPF")
    bp_setup_target(${name} include ${SOURCES})

   if (MODULE_INFO_PACKAGE)
        if (MODULE_INFO_API_MACRO)
            bp_package_module(${name} true)
        else (MODULE_INFO_API_MACRO)
            bp_package_module(${name} false)
        endif (MODULE_INFO_API_MACRO)
    endif (MODULE_INFO_PACKAGE)
endmacro(bp_setup_program)
