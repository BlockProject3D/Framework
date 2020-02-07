include("${CMAKE_CURRENT_LIST_DIR}/Base.cmake")

macro(bp_setup_module name apimacro)
    #Replace API macro by the correct one
    set(BP_API_MACRO ${apimacro})

    add_library(${name} SHARED ${SOURCES})

    target_compile_definitions(${name} PRIVATE "${BP_API_MACRO}=${BP_SYMBOL_EXPORT_MACRO}")
    # Attempt at fixing templates problems under MSVC 2017
    target_compile_definitions(${name} PRIVATE "BP_TPL_API=${BP_SYMBOL_EXPORT_MACRO}")

    if (NOT ${name} STREQUAL "BPF")
        bp_add_module(${name} "BPF")
    endif (NOT ${name} STREQUAL "BPF")

    bp_setup_target(${name} include ${SOURCES})
    file(WRITE ${CMAKE_CURRENT_LIST_DIR}/../${name}.cmake
        "set(INCLUDE_DIR \"${CMAKE_CURRENT_SOURCE_DIR}/include\")\n"
        "set(BIN_DEBUG \"${CMAKE_BINARY_DIR}/Debug/${BP_LIBRARY_PREFIX}${name}${BP_EXTENSION_LIB}\")\n"
        "set(BIN_RELEASE \"${CMAKE_BINARY_DIR}/Release/${BP_LIBRARY_PREFIX}${name}${BP_EXTENSION_LIB}\")\n"
        "set(ROOT \"${CMAKE_CURRENT_SOURCE_DIR}\")\n"
        "set(API_MACRO \"${BP_API_MACRO}\")\n"
    )
endmacro(bp_setup_module)

function(bp_add_module targetname modulename)
    include(${CMAKE_CURRENT_LIST_DIR}/../${modulename}.cmake)
    
    # Add definitions
    target_compile_definitions(${targetname} PRIVATE "${API_MACRO}=${BP_SYMBOL_IMPORT_MACRO}")

    # Add includes and libs
    target_include_directories(${targetname} PRIVATE ${INCLUDE_DIR})
    target_link_libraries(${targetname} PRIVATE debug ${BIN_DEBUG})
    target_link_libraries(${targetname} PRIVATE optimized ${BIN_RELEASE})
endfunction(bp_add_module)
