bp_add_definition("MAC")
set(BP_SYMBOL_IMPORT_MACRO "__attribute__((visibility(\"default\")))")
set(BP_SYMBOL_EXPORT_MACRO ${BP_SYMBOL_IMPORT_MACRO})
set(BP_EXTENSION_DYNAMIC ".dylib")
set(BP_EXYENSION_STATIC ".a")
set(BP_EXTENSION_LIB ".dylib")
set(BP_LIBRARY_PREFIX "lib")

function(bp_target_created name)
    get_target_property(target_type ${name} TYPE)
    if (target_type STREQUAL "EXECUTABLE")
        set_target_properties(${name} PROPERTIES
            BUILD_WITH_INSTALL_RPATH 1
            INSTALL_RPATH "@executable_path"
        )
    else (target_type STREQUAL "EXECUTABLE")
        set_target_properties(${name} PROPERTIES
            BUILD_WITH_INSTALL_RPATH 1
            INSTALL_NAME_DIR "@rpath/"
        )
    endif (target_type STREQUAL "EXECUTABLE")
endfunction(bp_target_created)
