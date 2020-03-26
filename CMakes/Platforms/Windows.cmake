include(CMakeParseArguments)
include("${CMAKE_CURRENT_LIST_DIR}/ThirdParty/generate_product_version.cmake")
set(__INTERNAL_BP_CUR_DIR ${CMAKE_CURRENT_LIST_DIR})

set(BP_SYMBOL_IMPORT_MACRO "__declspec(dllimport)")
set(BP_SYMBOL_EXPORT_MACRO "__declspec(dllexport)")
set(BP_EXTENSION_DYNAMIC ".dll")
set(BP_EXYENSION_STATIC ".lib")
set(BP_EXTENSION_LIB ".lib")
set(BP_LIBRARY_PREFIX "")

function(bp_target_created name)
    target_compile_definitions(${name} PRIVATE "WINDOWS")
    get_target_property(IS_GUI ${name} WIN32_EXECUTABLE)
    if (NOT IS_GUI)
        target_compile_definitions(${name} PRIVATE "CONSOLE")
    endif (NOT IS_GUI)
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
    if (NOT PRODUCT_NAME)
        set(PRODUCT_NAME ${targetname})
    endif (NOT PRODUCT_NAME)
    if (NOT PRODUCT_PACKAGE)
        set(PRODUCT_PACKAGE ${targetname})
    endif (NOT PRODUCT_PACKAGE)
    if (NOT PRODUCT_VERSION_MINOR)
        set(PRODUCT_VERSION_MINOR 0)
    endif (NOT PRODUCT_VERSION_MINOR)
    if (NOT PRODUCT_VERSION_MAJOR)
        set(PRODUCT_VERSION_MAJOR 1)
    endif (NOT PRODUCT_VERSION_MAJOR)
    if (NOT PRODUCT_COMPANY)
        set(PRODUCT_COMPANY "BlockProject")
    endif (NOT PRODUCT_COMPANY)
    if (NOT PRODUCT_COPYRIGHT)
        string(TIMESTAMP CURRENT_YEAR "%Y")
        set(PRODUCT_COPYRIGHT "Copyright (C) ${CURRENT_YEAR}, ${PRODUCT_COMPANY}")
    endif (NOT PRODUCT_COPYRIGHT)
    if (NOT PRODUCT_ICON)
        set(PRODUCT_ICON "${__INTERNAL_BP_CUR_DIR}/../logo_white.ico")
    endif(NOT PRODUCT_ICON)
    generate_product_version(tmpVar
        NAME ${PRODUCT_NAME}
        BUNDLE ${PRODUCT_PACKAGE}
        ICON ${PRODUCT_ICON}
        VERSION_MAJOR ${PRODUCT_VERSION_MAJOR}
        VERSION_MINOR ${PRODUCT_VERSION_MINOR}
        COMPANY_COPYRIGHT ${PRODUCT_COPYRIGHT}
        COMPANY_NAME ${PRODUCT_COMPANY}
        FILE_DESCRIPTION ${PRODUCT_DESCRIPTION}
    )
    set(BP_ADDITIONAL_SOURCE_FILE ${tmpVar} PARENT_SCOPE)
endfunction(bp_product_properties)
