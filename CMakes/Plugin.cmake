include("${CMAKE_CURRENT_LIST_DIR}/Framework.cmake")

function(bp_setup_plugin name)
    add_library(${name} SHARED ${SOURCES} ${BP_GENERATED_SOURCE_FILES})

    # Attempt at fixing templates problems under MSVC 2017
    target_compile_definitions(${name} PRIVATE "BP_TPL_API=${BP_SYMBOL_EXPORT_MACRO}")

    bp_use_module(${name} "BPF")

    bp_setup_target(${name} include ${SOURCES})
endfunction(bp_setup_plugin name)
