cmake_minimum_required(VERSION 3.10)
project(BlockProject)

include(CMakes/Base.cmake)

set(CMAKE_CXX_STANDARD 11)

set(BP_PACKAGE_NAME BPF)

add_subdirectory("${CMAKE_SOURCE_DIR}/Base/")
add_subdirectory("${CMAKE_SOURCE_DIR}/Compression/")
add_subdirectory("${CMAKE_SOURCE_DIR}/Graphics/")
add_subdirectory("${CMAKE_SOURCE_DIR}/Tests/")
add_subdirectory("${CMAKE_SOURCE_DIR}/Tests.Console/")

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt DESTINATION ${BP_PACKAGE_NAME})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/README.md DESTINATION ${BP_PACKAGE_NAME})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/BPFConfig.cmake DESTINATION ${BP_PACKAGE_NAME})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/CMakes DESTINATION ${BP_PACKAGE_NAME})
