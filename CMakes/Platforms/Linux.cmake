bp_add_definition("LINUX")
set(BP_SYMBOL_IMPORT_MACRO "__attribute__((visibility(\"default\")))")
set(BP_SYMBOL_EXPORT_MACRO ${BP_SYMBOL_IMPORT_MACRO})
set(BP_EXTENSION_DYNAMIC ".so")
set(BP_EXYENSION_STATIC ".a")
set(BP_EXTENSION_LIB ".so")
set(BP_LIBRARY_PREFIX "lib")

function(bp_target_created name)
	set_target_properties(${name} PROPERTIES
		BUILD_WITH_INSTALL_RPATH 1
		INSTALL_NAME_DIR "@rpath/"
	)
endfunction(bp_target_created)
