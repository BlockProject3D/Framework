include("${CMAKE_CURRENT_LIST_DIR}/Base.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Framework.cmake")

macro(bp_setup_program name incdir)
    add_executable(${name} ${SOURCES})
    target_compile_definitions(${name} PRIVATE "${BP_API_MACRO}=${BP_SYMBOL_EXPORT_MACRO}")
    bp_add_module(${name} "BPFramework")
    bp_setup_target(${name} ${incdir} ${SOURCES})
endmacro(bp_setup_program)
