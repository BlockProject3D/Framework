#ifdef WINDOWS
    #include <Windows.h>
#else
    #include <dlfcn.h>
#endif
#include "Framework/System/Module.hpp"
#include "Framework/System/Platform.hpp"

using namespace Framework;

FModule::FModule(const bpf::String &path)
    : Path(path + FPlatform::GetProperty("OS_MODULE_EXT"))
{
#ifdef WINDOWS
    Handle = LoadLibrary(*Path);
    if (Handle == NULL)
        throw FModuleException("Unknown");
#else
    Handle = dlopen(*Path, RTLD_NOW | RTLD_LOCAL);
    if (Handle == Null)
        throw FModuleException(dlerror());
#endif
}

FModule::~FModule()
{
#ifdef WINDOWS
    if (Handle != NULL)
        FreeLibrary((HMODULE)Handle);
#else
    if (Handle != Null)
        dlclose(Handle);
#endif
}

void *FModule::LoadSymbol(const bpf::String &name)
{
#ifdef WINDOWS
    void *res = (void *)GetProcAddress((HMODULE)Handle, *name);
    if (res == Null)
        throw FModuleException("Unknown");
    return (res);
#else
    void *res = dlsym(Handle, *name);
    if (res == Null)
        throw FModuleException(dlerror());
    return (res);
#endif
}
