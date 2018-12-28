set(BP_FRAMEWORK_CMAKE_SELF ${CMAKE_CURRENT_LIST_DIR})
include("${BP_FRAMEWORK_CMAKE_SELF}/Base.cmake")

bp_add_definition("BPF_API=${BP_SYMBOL_EXPORT_MACRO}")
# Attempt at fixing templates problems under MSVC 2017
bp_add_definition("BP_TPL_API=${BP_SYMBOL_EXPORT_MACRO}")

macro(bp_setup)
    add_library(BPFramework SHARED ${SOURCES})
    bp_setup_target(BPFramework include ${SOURCES})
    bp_prepend(BP_SOURCES_CVT ${CMAKE_SOURCE_DIR} ${SOURCES})
    bp_fixheaderlist(BP_SOURCES_CVT ${BP_SOURCES_CVT})
    string(REPLACE ";" "\n" BP_SOURCES_CVT "${BP_SOURCES_CVT}")
    file(WRITE ${CMAKE_SOURCE_DIR}/BPFramework.cmake
        "set(FRAMEWORK_INCLUDE_DIR \"${CMAKE_SOURCE_DIR}/include\")\n"
        "set(FRAMEWORK_BIN_DEBUG \"${CMAKE_BINARY_DIR}/Debug/${BP_LIBRARY_PREFIX}BPFramework${BP_EXTENSION_LIB}\")\n"
        "set(FRAMEWORK_BIN_RELEASE \"${CMAKE_BINARY_DIR}/Release/${BP_LIBRARY_PREFIX}BPFramework${BP_EXTENSION_LIB}\")\n"
        "set(FRAMEWORK_ROOT \"${CMAKE_SOURCE_DIR}\")\n"
        "set(FRAMEWORK_SOURCES ${BP_SOURCES_CVT})\n"
    )
endmacro(bp_setup)
