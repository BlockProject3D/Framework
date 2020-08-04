include("${CMAKE_CURRENT_LIST_DIR}/Framework.cmake")

function(bp_setup_module name)
    set(props
            API_MACRO
            PACKAGE_NAME
            )
    set(options
            PACKAGE
            )
    set(multiValueArgs)
    cmake_parse_arguments(MODULE_INFO "${options}" "${props}" "${multiValueArgs}" ${ARGN})

    add_library(${name} SHARED ${SOURCES} ${BP_GENERATED_SOURCE_FILES})

    if (MODULE_INFO_API_MACRO)
        target_compile_definitions(${name} PRIVATE "${MODULE_INFO_API_MACRO}=${BP_SYMBOL_EXPORT_MACRO}")
        __bp_write_module_descriptor(${name} ${MODULE_INFO_API_MACRO})
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
        bp_use_module(${name} "BPF")
    endif (NOT ${name} STREQUAL "BPF")

    bp_setup_target(${name} include ${SOURCES})

    if (MODULE_INFO_PACKAGE)
        if (NOT MODULE_INFO_PACKAGE_NAME)
            set(MODULE_INFO_PACKAGE_NAME ${BP_PACKAGE_NAME})
        endif (NOT MODULE_INFO_PACKAGE_NAME)
        if (NOT MODULE_INFO_PACKAGE_NAME)
            set(MODULE_INFO_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
        endif (NOT MODULE_INFO_PACKAGE_NAME)
        if (MODULE_INFO_API_MACRO)
            __bp_package_module(${name} true ${MODULE_INFO_PACKAGE_NAME})
        else (MODULE_INFO_API_MACRO)
            __bp_package_module(${name} false ${MODULE_INFO_PACKAGE_NAME})
        endif (MODULE_INFO_API_MACRO)
    endif (MODULE_INFO_PACKAGE)
endfunction(bp_setup_module)
