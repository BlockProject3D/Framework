include(CMakeParseArguments)

set(BP_SYMBOL_IMPORT_MACRO "__attribute__((visibility(\"default\")))")
set(BP_SYMBOL_EXPORT_MACRO ${BP_SYMBOL_IMPORT_MACRO})
set(BP_EXTENSION_DYNAMIC ".so")
set(BP_EXYENSION_STATIC ".a")
set(BP_EXTENSION_LIB ".so")
set(BP_LIBRARY_PREFIX "lib")

function(bp_target_created name)
    target_compile_definitions(${name} PRIVATE "LINUX")

    get_target_property(target_type ${name} TYPE)
    if (target_type STREQUAL "EXECUTABLE")
        set_target_properties(${name} PROPERTIES
            BUILD_WITH_INSTALL_RPATH 1
            INSTALL_RPATH "$ORIGIN"
        )
    endif (target_type STREQUAL "EXECUTABLE")
endfunction(bp_target_created)

function(bp_product_properties targetname)
    set(props
        NAME
        PACKAGE
        ICON
        VERSION_MAJOR
        VERSION_MINOR
        COPYRIGHT
        COMPANY
        DESCRIPTION
    )
    set(options)
    set(multiValueArgs)
    cmake_parse_arguments(PRODUCT "${options}" "${props}" "${multiValueArgs}" ${ARGN})
endfunction(bp_product_properties)