// Copyright (c) 2020, BlockProject
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
//     * Neither the name of BlockProject nor the names of its contributors
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

namespace bpf
{
    namespace system
    {
        template <typename BaseClass>
        void ModuleManager<BaseClass>::LoadModule(const Name &virtualName, const String &fileName)
        {
            String moduleFile = _modulePath + fileName;

            if (HasModule(virtualName))
                throw ModuleException("Module already loaded");
            Entry md;
            md.Handle = Module(moduleFile);
            md.Name = virtualName;
            String moduleLnkSymbol = virtualName + "_Link";
            String moduleDescSymbol = virtualName + "_Describe";
            ModuleLinkFunc sym = static_cast<ModuleLinkFunc>(md.Handle.LoadSymbol(moduleLnkSymbol));
            ModuleDescribeFunc sym1 = static_cast<ModuleDescribeFunc>(md.Handle.LoadSymbol(moduleDescSymbol));
            fint version = sym1();
            if (version > Platform::GetEnvInfo().VersionInt)
                throw ModuleException("Module has been built against a new version of the Framework");
            else if (version < Platform::GetEnvInfo().VersionInt)
                throw ModuleException("Module has been built against an older version of the Framework");
            if ((md.Interface = sym()) == Null)
                throw ModuleException("Module interface allocation failure");
            _map.Add(virtualName, std::move(md));
        }

        template <typename BaseClass>
        void ModuleManager<BaseClass>::UnloadModule(const Name &virtualName)
        {
            if (!HasModule(virtualName))
                return;
            _map[virtualName].Handle = Module();
            _map.RemoveAt(virtualName);
        }
    }
}
