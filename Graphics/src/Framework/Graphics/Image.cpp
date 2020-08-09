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

#include "Framework/Graphics/Image.hpp"
#include "Framework/Graphics/IImageLoader.hpp"
#include "Framework/IO/IOException.hpp"

using namespace bpf;
using namespace bpf::memory;
using namespace bpf::system;
using namespace bpf::io;
using namespace bpf::graphics;

UniquePtr<IImageLoader> Image::LoadInternal(const String &format)
{
    return (nullptr); // TODO: Implement
}

UniquePtr<Image> Image::Load(const File &file)
{
    if (!file.Exists())
        throw IOException(String("No such file or directory '") + file.Path() + '\'');
    auto loader = LoadInternal(file.Extension().ToUpper());
    if (loader == nullptr)
        throw IOException(String("Unsupported format '") + file.Extension().ToUpper() + '\'');
    return (loader->Load(file));
}

UniquePtr<Image> Image::Load(IInputStream &stream, const String &format)
{
    auto loader = LoadInternal(format);
    if (loader == nullptr)
        throw IOException(String("Unsupported format '") + format + '\'');
    return (loader->Load(stream));
}

UniquePtr<Image> Image::Load(PluginLoader &ploader, IInputStream &stream, const String &format)
{
    auto loader = LoadInternal(format);
    if (loader == nullptr)
    {
        String vname = String("Graphics_") + format;
        String fname = String("BPF.Graphics.") + format;
        if (!File(fname + Platform::GetOSInfo().ModuleExt).Exists())
            throw IOException(String("Unsupported format '") + format + '\'');
        auto module = ploader.Load<IImageLoader>(vname, fname);
        return (module.Interface->Load(stream));
    }
    return (loader->Load(stream));
}

UniquePtr<Image> Image::Load(PluginLoader &ploader, const File &file)
{
    if (!file.Exists())
        throw IOException(String("No such file or directory '") + file.Path() + '\'');
    auto format = file.Extension().ToUpper();
    auto loader = LoadInternal(format);
    if (loader == nullptr)
    {
        String vname = String("Graphics_") + format;
        String fname = String("BPF.Graphics.") + format;
        if (!File(fname + Platform::GetOSInfo().ModuleExt).Exists())
            throw IOException(String("Unsupported format '") + format + '\'');
        auto module = ploader.Load<IImageLoader>(vname, fname);
        return (module.Interface->Load(file));
    }
    return (loader->Load(file));
}

UniquePtr<Image> Image::CreateEmpty(const String &format, fsize w, fsize h, EPixelFormat pformat)
{
    auto loader = LoadInternal(format);
    if (loader == nullptr)
        throw IOException(String("Unsupported format '") + format + '\'');
    return (loader->CreateEmpty(w, h, pformat));
}

UniquePtr<Image> Image::CreateEmpty(PluginLoader &ploader, const String &format, fsize w, fsize h, EPixelFormat pformat)
{
    auto loader = LoadInternal(format);
    if (loader == nullptr)
    {
        String vname = String("Graphics_") + format;
        String fname = String("BPF.Graphics.") + format;
        if (!File(fname + Platform::GetOSInfo().ModuleExt).Exists())
            throw IOException(String("Unsupported format '") + format + '\'');
        auto module = ploader.Load<IImageLoader>(vname, fname);
        return (module.Interface->CreateEmpty(w, h, pformat));
    }
    return (loader->CreateEmpty(w, h, pformat));
}
