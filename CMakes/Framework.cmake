set(BP_FRAMEWORK_CMAKE_SELF ${CMAKE_CURRENT_LIST_DIR})
include("${BP_FRAMEWORK_CMAKE_SELF}/Base.cmake")
include("${BP_FRAMEWORK_CMAKE_SELF}/Program.cmake")

macro(bp_setup_framework)
    #Replace API macro by the correct one
    set(BP_API_MACRO BPF_API)

    add_library(BPFramework SHARED ${SOURCES})

    target_compile_definitions(BPFramework PRIVATE "BPF_API=${BP_SYMBOL_EXPORT_MACRO}")
    # Attempt at fixing templates problems under MSVC 2017
    target_compile_definitions(BPFramework PRIVATE "BP_TPL_API=${BP_SYMBOL_EXPORT_MACRO}")

    bp_setup_target(BPFramework include ${SOURCES})
    file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/BPFramework.cmake
        "set(FRAMEWORK_INCLUDE_DIR \"${CMAKE_CURRENT_SOURCE_DIR}/include\")\n"
        "set(FRAMEWORK_BIN_DEBUG \"${CMAKE_BINARY_DIR}/Debug/${BP_LIBRARY_PREFIX}BPFramework${BP_EXTENSION_LIB}\")\n"
        "set(FRAMEWORK_BIN_RELEASE \"${CMAKE_BINARY_DIR}/Release/${BP_LIBRARY_PREFIX}BPFramework${BP_EXTENSION_LIB}\")\n"
        "set(FRAMEWORK_ROOT \"${CMAKE_CURRENT_SOURCE_DIR}\")\n"
    )
endmacro(bp_setup_framework)
