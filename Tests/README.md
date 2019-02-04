# TestProgFramework
Framework test environment

This repository contains unit tests, with the help of the Google Test library, used to test reliability and usability of the BlockProject 3D Framework.

## Building
Requirements :
  - CMake for every platforms
  - Visual Studio 2015/2017 for windows builds
  - GCC or Clang for Linux and Mac builds
  - C++11 compliant compiler
  - Already built Framework Debug and/or Release binarries under \<this repository folder\>/../FrameworkRemake.
	The directory name is important due to the hardcoded CMake include in this repository.

### Windows
Under GitBash / PowerShell
  - mkdir build
  - cd build
  - cmake -G "Visual Studio 15 Win64" ..
  - Open Visual Studio and build from there (for some reasons cmake -build . prefers to build GTest instead of the application) or run MSBuild

### Linux / Mac
Under bash / other shells
  - mkdir build
  - cd build
  - cmake -G "Unix Makefiles" ..
  - make (for some reasons cmake -build . prefers to build GTest instead of the application)

## Running
Requirements :
  - Already built Framework Debug and/or Release binarries under \<this repository folder\>/../FrameworkRemake.
	The directory name is important due to the hardcoded CMake include in this repository
  - Copy the dylib/so/dll of the Framework binary inside of ${CMAKE_CURRENT_BINARY_DIR}/<target type either Debug or Release>

### Windows
  This is a console based application, consider running it from inside of PowerShell, GitBash or CMD

### Linux / Mac
  This is a console based application, consider running it from inside of your favorite Terminal Emulator
