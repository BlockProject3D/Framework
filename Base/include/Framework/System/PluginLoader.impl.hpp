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
#include "Framework/System/Platform.hpp"

namespace bpf
{
    namespace system
    {
        template <typename BaseClass>
        Plugin<BaseClass> PluginLoader::Load(const String &fileName)
        {
            String virtualName = fileName.Sub(fileName.IndexOf('.'));
            String moduleLnkSymbol = virtualName + "_Link";
            String moduleDescSymbol = virtualName + "_Version";
            io::File moduleFile = _modulePath + fileName;
            Plugin<BaseClass> plugin;

            plugin.Name = virtualName;
            plugin.Module = Module(moduleFile.PlatformPath());
            auto sym = reinterpret_cast<ModuleLinkFunc<BaseClass>>(plugin.Module.LoadSymbol(moduleLnkSymbol));
            auto sym1 = reinterpret_cast<ModuleDescribeFunc>(plugin.Module.LoadSymbol(moduleDescSymbol));
            fint version = sym1();
            if (version > Platform::GetEnvInfo().VersionInt)
                throw ModuleException("Plugin has been built against a newer version of the Framework");
            else if (version < Platform::GetEnvInfo().VersionInt)
                throw ModuleException("Plugin has been built against an older version of the Framework");
            if ((plugin.Interface = memory::UniquePtr<BaseClass>(sym())) == Null)
                throw ModuleException("Plugin interface allocation failure");
            return (plugin);
        }

        template <typename BaseClass>
        Plugin<BaseClass> PluginLoader::Load(const String &virtualName, const String &fileName)
        {
            String moduleLnkSymbol = virtualName + "_Link";
            String moduleDescSymbol = virtualName + "_Version";
            io::File moduleFile = _modulePath + fileName;
            Plugin<BaseClass> plugin;

            plugin.Name = virtualName;
            plugin.Module = Module(moduleFile.PlatformPath());
            auto sym = static_cast<ModuleLinkFunc<BaseClass>>(plugin.Module.LoadSymbol(moduleLnkSymbol));
            auto sym1 = static_cast<ModuleDescribeFunc>(plugin.Module.LoadSymbol(moduleDescSymbol));
            fint version = sym1();
            if (version > Platform::GetEnvInfo().VersionInt)
                throw ModuleException("Plugin has been built against a newer version of the Framework");
            else if (version < Platform::GetEnvInfo().VersionInt)
                throw ModuleException("Plugin has been built against an older version of the Framework");
            if ((plugin.Interface = memory::UniquePtr<BaseClass>(sym())) == Null)
                throw ModuleException("Plugin interface allocation failure");
            return (plugin);
        }
    }
}
