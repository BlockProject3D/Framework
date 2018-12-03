#if defined(LINUX) || defined(MAC)
# include <dlfcn.h>
#elif WINDOWS
# include <windows.h>
#endif
#include <stdlib.h>
#include "Framework/Framework.hpp"

using namespace Framework;

bool FPlatform::Exit = false;
IFileSystem *FPlatform::FileSys = NULL;
ISystemManager *FPlatform::BaseSys = NULL;
FMap<const char *, FString> FPlatform::PropMap;

void FPlatform::RequestExit(bool force)
{
    if (force)
        exit(1);
    else
        Exit = true;
}

bool FPlatform::IsExiting()
{
    return (Exit);
}

void *FPlatform::OpenDLL(const char *file)
{
#ifdef WINDOWS
    return (LoadLibrary(file));
#else
    return (dlopen(file, RTLD_NOW | RTLD_LOCAL));
#endif
}

void FPlatform::CloseDLL(void *hdl)
{
#ifdef WINDOWS
    FreeLibrary((HMODULE)hdl);
#else
    dlclose(hdl);
#endif
}

const char *FPlatform::GetDLLExt()
{
#ifdef LINUX
    return ("so");
#elif MAC
    return ("dylib");
#elif WINDOWS
    return ("dll");
#endif
}

IFileSystem *FPlatform::GetFileSystem()
{
    if (FileSys == NULL)
        FileSys = FModuleManager::GetModule<IFileSystem>("FileSystem");
    return (FileSys);
}

void *FPlatform::GetDLLSymbol(void *hdl, const char *name)
{
#ifdef WINDOWS
    return ((void *)GetProcAddress((HMODULE)hdl, name));
#else
    return (dlsym(hdl, name));
#endif
}

const char *FPlatform::GetLastError()
{
#ifdef WINDOWS
    return (NULL); // TODO : Implement windows
#else
    return (dlerror());
#endif
}

EPlatformEndianess FPlatform::GetPlatformEndianess()
{
    FTypeExpander<uint32> data = FTypeExpander<uint32>(0x1020304);

    if (data.Bytes[0] == 1)
        return (PLATFORM_BIGENDIAN);
    return (PLATFORM_LITTLEENDIAN);
}

void FPlatform::ReverseBuffer(uint8 *buf, const uint32 size)
{
    uint32 i = 0;
    uint32 j = size;
    uint8 temp;

    while (i < size / 2)
    {
        temp = buf[i];
        buf[i++] = buf[j];
        buf[j] = temp;
        j--;
    }
}

void FPlatform::ReverseBuffer(uint8 *buf, const uint32 groupsize, const uint32 size)
{
    uint32 i = 0;
    uint32 j = size;
    uint8 temp;

    while (i < size / 2)
    {
        for (uint32 k = 0 ; k < groupsize ; ++k)
        {
            if (i + k >= size || j + k >= size)
                continue;
            temp = buf[i + k];
            buf[i + k] = buf[j + k];
            buf[j + k] = temp;
        }
        i += groupsize;
        j -= groupsize;
    }
}

ISystemManager *FPlatform::GetBaseSystem()
{
    if (BaseSys == NULL)
        BaseSys = FModuleManager::GetModule<ISystemManager>("System");
    return (BaseSys);
}

void FPlatform::Initialize()
{
#if LINUX
    SetProperty("OS_MODULE_EXT", ".so");
    SetProperty("OS_NAME", "Linux");
    SetProperty("RENDER_DEFAULT", "GL33");
#elif MAC
    SetProperty("OS_MODULE_EXT", ".dylib");
    SetProperty("OS_NAME", "Mac OSX");
    SetProperty("RENDER_DEFAULT", "GL33");
#elif WINDOWS
    SetProperty("OS_MODULE_EXT", ".dll");
    SetProperty("OS_NAME", "Windows");
    SetProperty("RENDER_DEFAULT", "GL33");
#endif
    SetProperty("ENGINE_NAME", "BPEngine");
    SetProperty("ENGINE_LONG_NAME", "BlockProjectEngine");
    if (sizeof(int *) > 4)
        SetProperty("ARCHITECTURE", "x64");
    else
        SetProperty("ARCHITECTURE", "x86");
    FModuleManager::Initialize();
}

void FPlatform::Shutdown()
{
    FModuleManager::Shutdown();
}

const FString &FPlatform::GetProperty(const char *vname)
{
    if (PropMap.HasKey(vname))
        return (PropMap[vname]);
    return (FString::Empty);
}

void FPlatform::SetProperty(const char *vname, const FString &prop)
{
    PropMap[vname] = prop;
}
