bp_add_definition("WINDOWS")
set(BP_SYMBOL_IMPORT_MACRO "__declspec(dllimport)")
set(BP_SYMBOL_EXPORT_MACRO "__declspec(dllexport)")
set(BP_EXTENSION_DYNAMIC ".dll")
set(BP_EXYENSION_STATIC ".lib")
set(BP_LIBRARY_PREFIX "")

function(bp_target_created name)
endfunction(bp_target_created)
