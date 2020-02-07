# BPF.Tests
Framework test tool

This repository contains unit tests using Google Test.
The application is used to test reliability and usability of BlockProject Framework.

## Building
Requirements :
  - CMake for every platforms
  - Visual Studio 2015/2017 for windows builds
  - GCC or Clang for Linux and Mac builds
  - C++11 compliant compiler
  - Already built Framework Debug and/or Release binarries under \<this repository folder\>/../.

### Windows
Under GitBash / PowerShell
  - mkdir build
  - cd build
  - cmake -G "Visual Studio 15 Win64" ..
  - cmake --build .

### Linux / Mac
Under bash / other shells
  - mkdir build
  - cd build
  - cmake -G "Unix Makefiles" ..
  - cmake --build .

## Running
Requirements :
  - Already built Framework Debug and/or Release binarries under \<this repository folder\>/../.
  - Copy the dylib/so/dll of the Framework binary inside of ${CMAKE_CURRENT_BINARY_DIR}/<target type either Debug or Release>

### Windows
  This is a console based application, consider running it from inside of PowerShell, GitBash or CMD

### Linux / Mac
  This is a console based application, consider running it from inside of your favorite Terminal Emulator
