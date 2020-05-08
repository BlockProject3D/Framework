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

#ifdef WINDOWS
    #include <Windows.h>
#else
    #include <dlfcn.h>
#endif
#include "Framework/System/Module.hpp"
#include "Framework/System/ModuleException.hpp"
#include "Framework/System/Platform.hpp"

using namespace bpf::system;
using namespace bpf;

Module::Module(const bpf::String &path)
    : _path(path + "." + Platform::GetOSInfo().ModuleExt)
{
#ifdef WINDOWS
    _handle = LoadLibrary(*_path);
    if (_handle == NULL)
        throw ModuleException(ObtainErrorString());
#else
    _handle = dlopen(*_path, RTLD_NOW | RTLD_LOCAL);
    if (_handle == Null)
        throw ModuleException(dlerror());
#endif
}

Module &Module::operator=(Module &&other)
{
#ifdef WINDOWS
    if (_handle != NULL)
        FreeLibrary((HMODULE)_handle);
#else
    if (_handle != Null)
        dlclose(_handle);
#endif
    _handle = other._handle;
    _path = std::move(other._path);
    other._handle = Null;
    return (*this);
}

Module::~Module()
{
#ifdef WINDOWS
    if (_handle != NULL)
        FreeLibrary((HMODULE)_handle);
#else
    if (_handle != Null)
        dlclose(_handle);
#endif
}

#ifdef WINDOWS
String Module::ObtainErrorString()
{
    String res = "Unknown";
    LPTSTR errtxt = Null;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER
                  | FORMAT_MESSAGE_FROM_SYSTEM
                  | FORMAT_MESSAGE_IGNORE_INSERTS,
                  Null, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  reinterpret_cast<LPTSTR>(&errtxt), 0, Null);
    if (errtxt != Null)
    {
        res = errtxt;
        LocalFree(errtxt);
    }
    return (res);
}
#endif

void *Module::LoadSymbol(const bpf::String &name)
{
#ifdef WINDOWS
    void *res = (void *)GetProcAddress((HMODULE)_handle, *name);
    if (res == Null)
        throw ModuleException(ObtainErrorString());
    return (res);
#else
    void *res = dlsym(_handle, *name);
    if (res == Null)
        throw ModuleException(dlerror());
    return (res);
#endif
}
