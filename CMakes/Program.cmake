set(BP_PROG_CMAKE_SELF ${CMAKE_CURRENT_LIST_DIR})
include("${BP_PROG_CMAKE_SELF}/Base.cmake")

bp_add_definition("BPF_API=${BP_SYMBOL_IMPORT_MACRO}")
# Attempt at fixing templates problems under MSVC 2017
bp_add_definition("BP_TPL_API=${BP_SYMBOL_EXPORT_MACRO}")

bp_add_definition("${BP_API_MACRO}=${BP_SYMBOL_EXPORT_MACRO}")

macro(bp_setup)
    include(${BP_FRAMEWORK_SRC_DIR}/BPFramework.cmake)
    add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
    bp_add_include_dir(${FRAMEWORK_INCLUDE_DIR})
    bp_setup_target(${CMAKE_PROJECT_NAME} include ${SOURCES})
    target_link_libraries(${BP_NAME} PRIVATE debug ${FRAMEWORK_BIN_DEBUG})
    target_link_libraries(${BP_NAME} PRIVATE optimized ${FRAMEWORK_BIN_RELEASE})
endmacro(bp_setup)
