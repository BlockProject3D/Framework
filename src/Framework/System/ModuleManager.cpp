#include "Framework/Framework.hpp"
#include "Framework/Logging/DefaultLogger.hpp"
#include "Framework/System/ModuleManager.hpp"

#define VERSION_ENGINE_INT 0x0 //TODO : Fix

using namespace Framework;

bpf::Logger *FModuleManager::Log = NULL;
bpf::Map<const char *, FModuleEntry *> FModuleManager::ModuleMap;
bpf::List<FModuleEntry *> FModuleManager::ModuleList;

void FModuleManager::Initialize()
{
    if (Log != NULL)
        return;
    Log = new bpf::Logger("ModuleManager");
    Log->AddHandler(bpf::MakeUnique<bpf::DefaultLogger>());
    Log->Info("Initializing...");
}

void FModuleManager::Shutdown()
{
    Log->Info("Shutting down...");
    Log->Info("Closing module handles...");
    for (auto it = ModuleList.End() ; it ; --it)
        UnloadModule(*it);
    delete Log;
}

bool FModuleManager::LoadModule(const char *name)
{
    bpf::String moduleFile = bpf::Paths::Modules() + bpf::String(name);
    bpf::String vname = bpf::String(name).Sub(0, bpf::String(name).LastIndexOf('.'));

    if (ModuleLoaded(name))
    {
        Log->Warning("Not loading module %s : module already loaded", *vname);
        return (false);
    }
    try
    {
        FModuleEntry *md = new FModuleEntry(moduleFile, vname);
        bpf::String moduleLnkSymbol = md->Name + "_Link";
        bpf::String moduleDescSymbol = md->Name + "_Describe";
        ModuleLinkFunc sym = (ModuleLinkFunc)md->Module.LoadSymbol(moduleLnkSymbol);
        ModuleDescribeFunc sym1 = (ModuleDescribeFunc)md->Module.LoadSymbol(moduleDescSymbol);
        int version = sym1();
        if (version > VERSION_ENGINE_INT)
        {
            Log->Error("Module %s has been built against a newer version of the engine : '%i' > '%i'", *vname, version, VERSION_ENGINE_INT);
            delete md;
            return (false);
        }
        else if (version < VERSION_ENGINE_INT)
        {
            Log->Error("Module %s has been built against an older version of the engine : '%i' < '%i'", *vname, version, VERSION_ENGINE_INT);
            delete md;
            return (false);
        }
        if ((md->Interface = sym()) == NULL)
        {
            Log->Error("ModuleInterface allocation failure for module %s", *vname);
            delete md;
            return (false);
        }
        md->Interface->OnLoadModule();
        Log->Info("Loaded module %s", *vname);
        ModuleMap.Add(*vname, md);
        ModuleList.Add(md);
        return (true);
    }
    catch (const FModuleException &ex)
    {
        Log->Error("Unable to load module %s : %s", *vname, *ex.GetMessage());
        return (false);
    }
}

void FModuleManager::UnloadModule(FModuleEntry *entry)
{
    entry->Interface->OnUnloadModule();
    delete entry;
}

void FModuleManager::UnloadModule(const char *vname)
{
    if (!ModuleLoaded(vname))
        return;
    FModuleEntry *md = ModuleMap[vname];
    ModuleList.Remove(md);
    ModuleMap.Remove(vname);
    UnloadModule(md);
}
