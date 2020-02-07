# BlockProject Framework
High-level C++ framework, used in BlockProject 3D Engine

## Project Status
[![Build Status](https://travis-ci.org/BlockProject3D/Framework.svg?branch=master)](https://travis-ci.org/BlockProject3D/Framework)
[![codecov](https://codecov.io/gh/BlockProject3D/Framework/branch/master/graph/badge.svg)](https://codecov.io/gh/BlockProject3D/Framework)

## Building
<strong>Requirements:</strong>
- CMake for every platforms
- Visual Studio 2015/2017/2019 for windows builds
- GCC or Clang for Linux and Mac builds
- C++11 compliant compiler
<strong>Note:</strong>
The test tool is automatically built by the root CMake.

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

## Running
The test tool can be found under the following directory ${CMAKE_CURRENT_BINARY_DIR}/<target type either Debug or Release>.
Please run BPF.Tests using a terminal.