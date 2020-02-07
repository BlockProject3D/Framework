# BlockProject Framework
High-level C++ framework, used in BlockProject 3D Engine

## Project Status
[![Build Status](https://travis-ci.org/BlockProject3D/Framework.svg?branch=master)](https://travis-ci.org/BlockProject3D/Framework)
[![codecov](https://codecov.io/gh/BlockProject3D/Framework/branch/master/graph/badge.svg)](https://codecov.io/gh/BlockProject3D/Framework)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/0c086a3dcb5b4b27abd8c6383b83a746)](https://www.codacy.com/manual/Yuri6037/Framework?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=BlockProject3D/Framework&amp;utm_campaign=Badge_Grade)

## Building
To acomodate a bug with Markdown the note is before the requirements.
<strong>Note:</strong>
The test tool is automatically built by the root CMake.
<strong>Requirements:</strong>
-   CMake for every platforms
-   Visual Studio 2015/2017/2019 for windows builds
-   GCC or Clang for Linux and Mac builds
-   C++11 compliant compiler

### Windows
Under GitBash / Powershell
-   mkdir build
-   cd build
-   cmake -G "Visual Studio 15 Win64" ..
-   cmake --build .

### Linux / Mac
Under bash / other shells
-   mkdir build
-   cd build
-   cmake -G "Unix Makefiles" ..
-   cmake --build .

## Running
The test tool can be found under the following directory ${CMAKE_CURRENT_BINARY_DIR}/<target type either Debug or Release>.
Please run BPF.Tests using a terminal.