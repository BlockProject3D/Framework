# BlockProject Framework [![Build Status](https://travis-ci.org/BlockProject3D/Framework.svg?branch=master)](https://travis-ci.org/BlockProject3D/Framework)
High-level C++ framework, used in BlockProject 3D Engine

## Building
Requirements :
  - CMake for every platforms
  - Visual Studio 2015/2017 for windows builds
  - GCC or Clang for Linux and Mac builds
  - C++11 compliant compiler

### Windows
Under GitBash / Powershell
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
