#set(BP_PLATFORM_DEF "WINDOWS")
set(BP_SYMBOL_IMPORT_MACRO "__declspec(dllimport)")
set(BP_SYMBOL_EXPORT_MACRO "__declspec(dllexport)")
set(BP_EXTENSION_DYNAMIC ".dll")
set(BP_EXYENSION_STATIC ".lib")
set(BP_EXTENSION_LIB ".lib")
set(BP_LIBRARY_PREFIX "")

function(bp_target_created name)
    target_compile_definitions(${name} PRIVATE "WINDOWS")

    # Attempt at fixing templates problems under MSVC 2017
    target_compile_definitions(${name} PRIVATE "BP_TPL_API=${BP_SYMBOL_EXPORT_MACRO}")
endfunction(bp_target_created)
