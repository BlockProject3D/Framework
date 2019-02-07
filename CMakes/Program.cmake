set(BP_PROG_CMAKE_SELF ${CMAKE_CURRENT_LIST_DIR})
include("${BP_PROG_CMAKE_SELF}/Base.cmake")

function(bp_add_framework name)
    include(${BP_FRAMEWORK_SRC_DIR}/BPFramework.cmake)
    
    # Add definitions
    target_compile_definitions(${name} PRIVATE "BPF_API=${BP_SYMBOL_IMPORT_MACRO}")
    # Attempt at fixing templates problems under MSVC 2017
    target_compile_definitions(${name} PRIVATE "BP_TPL_API=${BP_SYMBOL_EXPORT_MACRO}")

    # Add includes and libs
    target_include_directories(${name} PRIVATE ${FRAMEWORK_INCLUDE_DIR})
    target_link_libraries(${name} PRIVATE debug ${FRAMEWORK_BIN_DEBUG})
    target_link_libraries(${name} PRIVATE optimized ${FRAMEWORK_BIN_RELEASE})
endfunction(bp_add_framework)

macro(bp_setup_program name incdir)
    add_executable(${name} ${SOURCES})
    target_compile_definitions(${name} PRIVATE "${BP_API_MACRO}=${BP_SYMBOL_EXPORT_MACRO}")
    bp_add_framework(${name})
    bp_setup_target(${name} ${incdir} ${SOURCES})
endmacro(bp_setup_program)
