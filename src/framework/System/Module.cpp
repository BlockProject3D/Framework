#ifdef WINDOWS
# include <windows.h>
#else
# include <dlfcn.h>
#endif
#include "framework/framework.h"

using namespace Framework;

FModule::FModule(const FString &path)
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

void *FModule::LoadSymbol(const FString &name)
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
