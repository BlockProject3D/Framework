#include "Framework/Framework.hpp"
#include "Framework/Logging/DefaultLogger.hpp"
#include "Framework/System/ModuleManager.hpp"
#include "Framework/System/Platform.hpp"

using namespace bpf;

ModuleManager::~ModuleManager()
{
    for (auto it = ModuleList.End() ; it ; --it)
        UnloadModule(*it);
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
    ModuleLinkFunc sym = (ModuleLinkFunc)md->Module.LoadSymbol(moduleLnkSymbol);
    ModuleDescribeFunc sym1 = (ModuleDescribeFunc)md->Module.LoadSymbol(moduleDescSymbol);
    int version = sym1();
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
    ModuleMap.Remove(vname);
    UnloadModule(md);
}
