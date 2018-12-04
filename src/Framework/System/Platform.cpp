#include <stdlib.h>
#include "Framework/System/Platform.hpp"
#include "Framework/System/ModuleManager.hpp"
#include "Framework/IO/FileSystem.hpp"
#include "Framework/System/System.hpp"

using namespace Framework;

bool FPlatform::Exit = false;
IFileSystem *FPlatform::FileSys = Null;
ISystemManager *FPlatform::BaseSys = Null;
bpf::Map<const char *, bpf::String> FPlatform::PropMap;

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

IFileSystem *FPlatform::GetFileSystem()
{
    if (FileSys == NULL)
        FileSys = FModuleManager::GetModule<IFileSystem>("FileSystem");
    return (FileSys);
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

const bpf::String &FPlatform::GetProperty(const char *vname)
{
    if (PropMap.HasKey(vname))
        return (PropMap[vname]);
    return (bpf::String::Empty);
}

void FPlatform::SetProperty(const char *vname, const bpf::String &prop)
{
    PropMap[vname] = prop;
}
