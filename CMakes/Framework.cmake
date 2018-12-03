set(BP_FRAMEWORK_CMAKE_SELF ${CMAKE_CURRENT_LIST_DIR})
include("${BP_FRAMEWORK_CMAKE_SELF}/Base.cmake")

bp_add_definition("BPF_API=${BP_SYMBOL_EXPORT_MACRO}")

macro(bp_setup)
    add_library(BPFramework SHARED ${SOURCES})
    bp_setup_target(BPFramework include ${SOURCES})
    file(WRITE ${CMAKE_SOURCE_DIR}/BPFramework.cmake
        "set(FRAMEWORK_INCLUDE_DIR \"${CMAKE_SOURCE_DIR}/include\")\n"
        "set(FRAMEWORK_BIN_DEBUG \"${CMAKE_BINARY_DIR}/Debug/${PREFIXLIB}BPFramework${EXTDLIB}\")\n"
        "set(FRAMEWORK_BIN_RELEASE \"${CMAKE_BINARY_DIR}/Release/${PREFIXLIB}BPFramework${EXTDLIB}\")\n"
    )
endmacro(bp_setup)
