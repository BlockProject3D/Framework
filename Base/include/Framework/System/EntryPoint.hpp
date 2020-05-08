// Copyright (c) 2020, BlockProject 3D
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright notice,
//       this list of conditions and the following disclaimer in the documentation
//       and/or other materials provided with the distribution.
//     * Neither the name of BlockProject 3D nor the names of its contributors
//       may be used to endorse or promote products derived from this software
//       without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#define BP_DECLARE_HIGH_MAIN int Main(bpf::system::IApplication &app, const bpf::collection::Array<bpf::String> &args, const bpf::system::Paths &paths)

#ifdef WINDOWS
    #include "Framework/System/WindowsApp.hpp"
    #include <Windows.h>

    #ifdef CONSOLE
        #define BP_SETUP_ENTRY_POINT() \
        BP_DECLARE_HIGH_MAIN; \
        int main() \
        { \
            bpf::system::WindowsApp app(reinterpret_cast<void *>(GetModuleHandle(Null)), true); \
            return (Main(app, app.GetArguments(), app.GetPaths())); \
        }
    #else
        #define BP_SETUP_ENTRY_POINT() \
        BP_DECLARE_HIGH_MAIN; \
        int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) \
        { \
            bpf::system::WindowsApp app(reinterpret_cast<void *>(hInstance), false); \
            return (Main(app, app.GetArguments(), app.GetPaths())); \
        }
    #endif
#else
    #include "Framework/System/UnixApp.hpp"

    #define BP_SETUP_ENTRY_POINT() \
    BP_DECLARE_HIGH_MAIN; \
    int main(int argc, char **argv, char **env) \
    { \
        bpf::system::UnixApp app(argv, argc, env); \
        return (Main(app, app.GetArguments(), app.GetPaths())); \
    }
#endif
