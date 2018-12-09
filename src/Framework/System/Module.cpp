#ifdef WINDOWS
    #include <Windows.h>
#else
    #include <dlfcn.h>
#endif
#include "Framework/System/Module.hpp"
#include "Framework/System/ModuleException.hpp"
#include "Framework/System/Platform.hpp"

using namespace bpf;

Module::Module(const bpf::String &path)
    : Path(path + "." + Platform::GetOSInfo().ModuleExt)
{
#ifdef WINDOWS
    Handle = LoadLibrary(*Path);
    if (Handle == NULL)
        throw ModuleException(ObtainErrorString());
#else
    Handle = dlopen(*Path, RTLD_NOW | RTLD_LOCAL);
    if (Handle == Null)
        throw ModuleException(dlerror());
#endif
}

Module::~Module()
{
#ifdef WINDOWS
    if (Handle != NULL)
        FreeLibrary((HMODULE)Handle);
#else
    if (Handle != Null)
        dlclose(Handle);
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
    void *res = (void *)GetProcAddress((HMODULE)Handle, *name);
    if (res == Null)
        throw ModuleException(ObtainErrorString());
    return (res);
#else
    void *res = dlsym(Handle, *name);
    if (res == Null)
        throw ModuleException(dlerror());
    return (res);
#endif
}
