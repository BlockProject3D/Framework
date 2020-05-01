include("${CMAKE_CURRENT_LIST_DIR}/Base.cmake")

function(bp_write_module_descriptor name)
    message("Writing module descriptor \"${name}.cmake\"...")
    get_filename_component(PRJ_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)
    file(RELATIVE_PATH REL_SRC_DIR ${PRJ_ROOT} ${CMAKE_CURRENT_SOURCE_DIR})
    file(RELATIVE_PATH REL_BIN_DIR ${PRJ_ROOT} ${CMAKE_BINARY_DIR})
    file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/../${name}.cmake
        "set(INCLUDE_DIR \"${REL_SRC_DIR}/include\")\n"
        "set(BIN_DEBUG \"${REL_BIN_DIR}/Debug/${BP_LIBRARY_PREFIX}${name}${BP_EXTENSION_LIB}\")\n"
        "set(BIN_RELEASE \"${REL_BIN_DIR}/Release/${BP_LIBRARY_PREFIX}${name}${BP_EXTENSION_LIB}\")\n"
        "set(ROOT \"${REL_SRC_DIR}\")\n"
        "set(API_MACRO \"${BP_API_MACRO}\")\n"
    )
endfunction(bp_write_module_descriptor)

macro(bp_setup_module name apimacro)
    #Replace API macro by the correct one
    set(BP_API_MACRO ${apimacro})

    if (NOT BP_ADDITIONAL_SOURCE_FILE)
        set(BP_ADDITIONAL_SOURCE_FILE "")
    endif(NOT BP_ADDITIONAL_SOURCE_FILE)

    add_library(${name} SHARED ${SOURCES} ${BP_ADDITIONAL_SOURCE_FILE})

    target_compile_definitions(${name} PRIVATE "${BP_API_MACRO}=${BP_SYMBOL_EXPORT_MACRO}")
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

    bp_write_module_descriptor(${name})
endmacro(bp_setup_module)

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
    if (EXISTS "${MD_PATH}/${BIN_DEBUG}")
        target_link_libraries(${targetname} PRIVATE debug "${MD_PATH}/${BIN_DEBUG}")
    else (EXISTS "${MD_PATH}/${BIN_DEBUG}")
        target_link_libraries(${targetname} PRIVATE debug ${modulename})
    endif (EXISTS "${MD_PATH}/${BIN_DEBUG}")
    if (EXISTS "${MD_PATH}/${BIN_RELEASE}")
        target_link_libraries(${targetname} PRIVATE optimized "${MD_PATH}/${BIN_RELEASE}")
    else (EXISTS "${MD_PATH}/${BIN_RELEASE}")
        target_link_libraries(${targetname} PRIVATE optimized ${modulename})
    endif (EXISTS "${MD_PATH}/${BIN_RELEASE}")
endfunction(bp_add_module)
