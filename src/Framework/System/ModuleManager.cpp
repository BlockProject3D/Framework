#include "Framework/Framework.hpp"

#define VERSION_ENGINE_INT 0x0 //TODO : Fix

using namespace Framework;

FLogger *FModuleManager::Log = NULL;
bpf::Map<const char *, FModuleEntry *> FModuleManager::ModuleMap;
bpf::List<FModuleEntry *> FModuleManager::ModuleList;

void FModuleManager::Initialize()
{
    if (Log != NULL)
        return;
    Log = new FLogger("ModuleManager");
    Log->AddLogHandler(new FDefaultLogger());
    Log->Log(LOG_INFO, "Initializing...");
}

void FModuleManager::Shutdown()
{
    Log->Log(LOG_INFO, "Shutting down...");
    Log->Log(LOG_INFO, "Closing module handles...");
    for (auto it = ModuleList.End() ; it ; --it)
        UnloadModule(*it);
    delete Log;
}

bool FModuleManager::LoadModule(const char *name)
{
    bpf::String moduleFile = FPaths::Modules() + bpf::String(name);
    bpf::String vname = bpf::String(name).Sub(0, bpf::String(name).LastIndexOf('.'));

    if (ModuleLoaded(name))
    {
        Log->Log(LOG_WARN, "Not loading module %s : module already loaded", *vname);
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
            Log->Log(LOG_ERR, "Module %s has been built against a newer version of the engine : '%i' > '%i'", *vname, version, VERSION_ENGINE_INT);
            delete md;
            return (false);
        }
        else if (version < VERSION_ENGINE_INT)
        {
            Log->Log(LOG_ERR, "Module %s has been built against an older version of the engine : '%i' < '%i'", *vname, version, VERSION_ENGINE_INT);
            delete md;
            return (false);
        }
        if ((md->Interface = sym()) == NULL)
        {
            Log->Log(LOG_FATAL, "ModuleInterface allocation failure for module %s", *vname);
            delete md;
            return (false);
        }
        md->Interface->OnLoadModule();
        Log->Log(LOG_INFO, "Loaded module %s", *vname);
        ModuleMap.Add(*vname, md);
        ModuleList.Add(md);
        return (true);
    }
    catch (const FModuleException &ex)
    {
        Log->Log(LOG_ERR, "Unable to load module %s : %s", *vname, *ex.GetMessage());
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
