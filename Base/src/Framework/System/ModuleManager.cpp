// Copyright (c) 2018, BlockProject
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

#include "Framework/Path.hpp"
#include "Framework/Memory/Memory.hpp"
#include "Framework/System/ModuleManager.hpp"
#include "Framework/System/Platform.hpp"

using namespace bpf::memory;
using namespace bpf::system;
using namespace bpf;

ModuleManager::~ModuleManager()
{
    for (auto &it : ModuleList)
        UnloadModule(*it->Name);
}

void ModuleManager::LoadModule(const char *name)
{
    bpf::String moduleFile = bpf::Paths::Modules() + bpf::String(name);
    bpf::String vname = bpf::String(name).Sub(0, bpf::String(name).LastIndexOf('.'));

    if (ModuleLoaded(name))
        throw ModuleException("Module already loaded !");
    ModuleEntry *md = Memory::New<ModuleEntry>(moduleFile, vname);
    String moduleLnkSymbol = md->Name + "_Link";
    String moduleDescSymbol = md->Name + "_Describe";
    ModuleLinkFunc sym = (ModuleLinkFunc)md->Handle.LoadSymbol(moduleLnkSymbol);
    ModuleDescribeFunc sym1 = (ModuleDescribeFunc)md->Handle.LoadSymbol(moduleDescSymbol);
    fint version = sym1();
    if (version > Platform::GetEnvInfo().VersionInt)
        throw ModuleException("Module has been built against a new version of the Framework");
    else if (version < Platform::GetEnvInfo().VersionInt)
        throw ModuleException("Module has been built against an older version of the Framework");
    if ((md->Interface = sym()) == NULL)
        throw ModuleException("IModuleInterface allocation failure !");
    md->Interface->OnLoadModule();
    ModuleMap.Add(*vname, md);
    ModuleList.Add(md);
}

void ModuleManager::UnloadModule(ModuleEntry *entry)
{
    entry->Interface->OnUnloadModule();
    delete entry;
}

void ModuleManager::UnloadModule(const char *vname)
{
    if (!ModuleLoaded(vname))
        return;
    ModuleEntry *md = ModuleMap[vname];
    ModuleList.Remove(md);
    ModuleMap.RemoveAt(vname);
    UnloadModule(md);
}
