include("${CMAKE_CURRENT_LIST_DIR}/Version.cmake")
if (UNIX)
    set(CPACK_PACKAGING_INSTALL_PREFIX /usr/local)
else (UNIX)
	set(CPACK_WIX_SIZEOF_VOID_P 8)
endif (UNIX)
set(CPACK_PACKAGE_INSTALL_DIRECTORY ".")
set(CPACK_PACKAGE_NAME "BPF")
set(CPACK_PACKAGE_FILE_NAME "BPF")
set(CPACK_PACKAGE_VENDOR "BlockProject 3D")
set(CPACK_PACKAGE_DESCRIPTION "BlockProject Framework")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "BlockProject Framework")
set(CPACK_PACKAGE_VERSION_MAJOR 3)
set(CPACK_PACKAGE_VERSION_MINOR 0)
set(CPACK_PACKAGE_VERSION_PATCH 0)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_LIST_DIR}/BPF/LICENSE.txt")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_LIST_DIR}/README.txt")
set(CPACK_INSTALLED_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/BPF/;${CPACK_PACKAGING_INSTALL_PREFIX}/BPF")
set(CPACK_PACKAGE_CONTACT "https://github.com/BlockProject3D/Framework")
set(CPACK_WIX_UPGRADE_GUID "CFAE9897-F201-4AE2-8110-B3459A8DDB4E")
